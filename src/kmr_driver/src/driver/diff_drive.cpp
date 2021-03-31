/**
 * @file /kmr_driver/src/driver/diff_drive.cpp
 *
 * @brief Differential drive abstraction (brought in from ycs).
 *
 * License: BSD
 *   https://raw.github.com/yujinrobot/kmr_core/hydro-devel/kmr_driver/LICENSE
 **/

/*****************************************************************************
** Includes
*****************************************************************************/

#include "../../include/kmr_driver/modules/diff_drive.hpp"

/*****************************************************************************
** Namespaces
*****************************************************************************/

namespace kmr {

/*****************************************************************************
** Implementation
*****************************************************************************/
DiffDrive::DiffDrive() :
  last_velocity_left(0.0),
  last_velocity_right(0.0),
  last_tick_left(0),
  last_tick_right(0),
  last_rad_left(0.0),
  last_rad_right(0.0),
//  v(0.0), w(0.0), // command velocities, in [m/s] and [rad/s]
  point_velocity(2,0.0), // command velocities, in [m/s] and [rad/s]
  radius(0.0), // command velocities, in [mm] and [mm/s]
  speed(0.0),
  bias(1.925), // wheelbase, wheel_to_wheel, in [m]
  //bias(0.223), // wheelbase, wheel_to_wheel, in [m]
  wheel_radius(0.27), // radius of main wheel, in [m]
  //tick_to_rad(0.00317332585858586),
  tick_to_rad(0.001533981),
  heading(0.0),
  diff_drive_kinematics(bias, wheel_radius)
{
  (void) imu_heading_offset;
}

/**
 * @brief Updates the odometry from firmware stamps and encoders.
 *
 * Really horrible - could do with an overhaul.
 *
 * @param time_stamp
 * @param left_encoder
 * @param right_encoder
 * @param pose_update
 * @param pose_update_rates
 */
void DiffDrive::update(const uint16_t &time_stamp,
                       const uint16_t &left_encoder,
                       const uint16_t &right_encoder,
                       const int32_t &steering,
                       ecl::LegacyPose2D<double> &pose_update,
                       ecl::linear_algebra::Vector3d &pose_update_rates) {
  state_mutex.lock();
  static bool init_l = false;
  static bool init_r = false;
  double left_diff_ticks = 0.0f;
  double right_diff_ticks = 0.0f;
  unsigned short curr_tick_left = 0;
  unsigned short curr_tick_right = 0;
  unsigned short curr_timestamp = 0;
  double steering_pos;
  double d_pos;
  double d_dist;
  double d_theta;
  double d_x;
  double d_y;

  curr_timestamp = time_stamp;

  // left
  curr_tick_left = left_encoder;
  if (!init_l)
  {
    last_tick_left = curr_tick_left;
    init_l = true;
  }
  left_diff_ticks = (double)(short)((curr_tick_left - last_tick_left) & 0xffff);
  last_tick_left = curr_tick_left;
  last_rad_left += tick_to_rad * left_diff_ticks;

  // right
  curr_tick_right = right_encoder;
  if (!init_r)
  {
    last_tick_right = curr_tick_right;
    init_r = true;
  }
  right_diff_ticks = (double)(short)((curr_tick_right - last_tick_right) & 0xffff);
  last_tick_right = curr_tick_right;
  last_rad_right += tick_to_rad * right_diff_ticks;

  // TODO this line and the last statements are really ugly; refactor, put in another place
  // pose_update = diff_drive_kinematics.forward(tick_to_rad * left_diff_ticks, tick_to_rad * right_diff_ticks);
  d_pos = (tick_to_rad * left_diff_ticks + tick_to_rad * right_diff_ticks) / 2;
  d_dist = d_pos * wheel_radius;// 弧长公式
  d_theta = tan(steering / 100.0 / 180.0 * 3.1415926) * d_dist / bias;
#if 0
  d_x = sin(d_theta) * d_dist;
  d_y = cos(d_theta) * d_dist;
  d_x = d_x * cos(heading) - d_y * sin(heading);
  d_y = d_y * sin(heading) + d_y * cos(heading);
  heading += d_theta;
#else
  // https://github.com/ros-controls/ros_controllers/blob/noetic-devel/ackermann_steering_controller/src/odometry.cpp
  if (fabs(d_theta) < 1e-6)
  {
      const double direction = heading + d_theta * 0.5;

      /// Runge-Kutta 2nd order integration:
      d_x = d_dist  * cos(direction);
      d_y = d_theta * sin(direction);
      heading += d_theta;
  }
  else
  {
      /// Exact integration (should solve problems when angular is zero):
      const double heading_old = heading;
      const double r = d_dist/d_theta;
      heading += d_theta;
      d_x =  r * (sin(heading) - sin(heading_old));
      d_y = -r * (cos(heading) - cos(heading_old));
  }
#endif

  // https://github.com/stonier/ecl_core/blob/release/1.1.x/ecl_mobile_robot/src/lib/differential_drive.cpp
  // pose_update.translation(d_dist, 0);
  // pose_update.rotation(d_theta);
  pose_update.x(d_x);// 这里不想更改函数传递的参数，直接赋值方式，node里更新pose不能用乘法，乘法适用于差速车
  pose_update.y(d_y);
  pose_update.heading(d_theta);

  if (curr_timestamp != last_timestamp)
  {
    last_diff_time = ((double)(short)((curr_timestamp - last_timestamp) & 0xffff)) / 1000.0f;
    last_timestamp = curr_timestamp;
    last_velocity_left = (tick_to_rad * left_diff_ticks) / last_diff_time;
    last_velocity_right = (tick_to_rad * right_diff_ticks) / last_diff_time;
  } else {
    // we need to set the last_velocity_xxx to zero?
  }

  pose_update_rates << d_dist/last_diff_time,
                       0,
                       // pose_update.y()/last_diff_time,
                       pose_update.heading()/last_diff_time;
  state_mutex.unlock();
}

void DiffDrive::reset() {
  state_mutex.lock();
  last_rad_left = 0.0;
  last_rad_right = 0.0;
  last_velocity_left = 0.0;
  last_velocity_right = 0.0;
  state_mutex.unlock();
}

void DiffDrive::getWheelJointStates(double &wheel_left_angle, double &wheel_left_angle_rate,
                                    double &wheel_right_angle, double &wheel_right_angle_rate) {
  state_mutex.lock();
  wheel_left_angle = last_rad_left;
  wheel_right_angle = last_rad_right;
  wheel_left_angle_rate = last_velocity_left;
  wheel_right_angle_rate = last_velocity_right;
  state_mutex.unlock();
}

void DiffDrive::setVelocityCommands(const double &vx, const double &wz) {
  // vx: in m/s
  // wz: in rad/s
  std::vector<double> cmd_vel;
  cmd_vel.push_back(vx);
  cmd_vel.push_back(wz);
  point_velocity = cmd_vel;
}

void DiffDrive::velocityCommands(const double &vx, const double &wz) {
  // vx: in m/s
  // wz: in rad/s
  velocity_mutex.lock();

  const double epsilon = 0.0001;

  // Special Case #1 : Straight Run
  if( std::abs(wz) < epsilon ) {
      radius = 0.0f;
      speed  = 1000.0f * vx;
      velocity_mutex.unlock();
      return;
  }

  radius = vx * 1000.0f / wz;

  speed = vx * 1000.0f;

  velocity_mutex.unlock();
  return;
}

void DiffDrive::velocityCommands(const short &cmd_speed, const short &cmd_radius) {
  velocity_mutex.lock();
  speed = static_cast<double>(cmd_speed);   // In [mm/s]
  radius = static_cast<double>(cmd_radius); // In [mm]
  velocity_mutex.unlock();
  return;
}

std::vector<short> DiffDrive::velocityCommands() {
  velocity_mutex.lock();
  std::vector<short> cmd(2);
  cmd[0] = bound(speed);  // In [mm/s]
  cmd[1] = bound(radius); // In [mm]
  velocity_mutex.unlock();
  return cmd;
}

std::vector<double> DiffDrive::pointVelocity() const {
  return point_velocity;
}

short DiffDrive::bound(const double &value) {
  if (value > static_cast<double>(SHRT_MAX)) return SHRT_MAX;
  if (value < static_cast<double>(SHRT_MIN)) return SHRT_MIN;
  return static_cast<short>(value);
}

} // namespace kmr
