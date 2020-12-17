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

#include <fcntl.h> 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
/*****************************************************************************
** Namespaces
*****************************************************************************/

namespace kmr {

char print_buf[1024];
char file_name[128];
int file_fd;
char *gen_file_name(void)
{
	time_t timep;
	struct tm *p;
	time(&timep);
	p=localtime(&timep); 
	sprintf(file_name, "test-%d-%02d-%02d_%02d-%02d-%02d.txt", 
			(1900+p->tm_year),
			1+p->tm_mon, 
			p->tm_mday,
			p->tm_hour, 
			p->tm_min, 
			p->tm_sec);

	return file_name;
}
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
  point_velocity(3, 0.0), // command velocities, in [m/s] and [rad/s] and [rad]
  radius(0.0), // command velocities, in [mm] and [mm/s]
  speed(0.0),
  bias(0.6), //横向距离 wheelbase, wheel_to_wheel, in [m]
  lng(1.35), // 纵向距离 wheelbase, wheel_to_wheel, in [m]
  wheel_radius(0.075), // radius of main wheel, in [m]
  //tick_to_rad(0.00317332585858586),
  tick_to_rad(0.0000766990380859375),
  diff_drive_kinematics(bias, wheel_radius)
{
  (void) imu_heading_offset;
  file_fd = open(gen_file_name(), O_RDWR|O_CREAT, S_IWUSR|S_IRUSR|S_IROTH);
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
                       const int16_t &left_steering,
                       const int16_t &right_steering,
                       const double &heading,
                       const double &delta_heading,
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
  double left_delta_s, right_delta_s;
  double left_delta_x, right_delta_x;
  double left_delta_y, right_delta_y;
  double left_steering_rad, right_steering_rad;
  double left_rad, right_rad;
  double delta_x, delta_y;
  static double x = 0, y = 0;
  curr_timestamp = time_stamp;
  curr_tick_left = left_encoder;
  int len, total_len;

  if (!init_l)
  {
    last_tick_left = curr_tick_left;
    init_l = true;
  }

  left_diff_ticks = (double)(short)((curr_tick_left - last_tick_left) & 0xffff);
  last_tick_left = curr_tick_left;
  last_rad_left += tick_to_rad * left_diff_ticks;

  curr_tick_right = right_encoder;
  if (!init_r)
  {
    last_tick_right = curr_tick_right;
    init_r = true;
  }
  right_diff_ticks = (double)(short)((curr_tick_right - last_tick_right) & 0xffff);
  last_tick_right = curr_tick_right;
  last_rad_right += tick_to_rad * right_diff_ticks;

  left_delta_s  = wheel_radius * (tick_to_rad * left_diff_ticks);
  right_delta_s = wheel_radius * (tick_to_rad * right_diff_ticks);

  left_steering_rad   = left_steering / 100.0 / 180.0 * M_PI;
  right_steering_rad  = right_steering / 100.0 / 180.0 * M_PI;

  left_rad = left_steering_rad + heading + delta_heading;
  right_rad = right_steering_rad + heading + delta_heading;

  total_len = 0;

  len = sprintf(&print_buf[total_len], "speed:%f radius:%f: heading:%fdeg\n", speed, radius, heading / M_PI * 180);
  total_len += len;
  len = sprintf(&print_buf[total_len], "l:ds:%f rad:%f=ste:%f+yaw:%f+dh:%f\n", left_delta_s, left_rad, left_steering_rad, heading, delta_heading);
  total_len += len;
  len = sprintf(&print_buf[total_len], "r:ds:%f rad:%f=ste:%f+yaw:%f+dh:%f\n", right_delta_s, right_rad, right_steering_rad, heading, delta_heading);
  total_len += len;

  left_delta_x = left_delta_s * cos(left_rad);
  left_delta_y = left_delta_s * sin(left_rad);

  right_delta_x = right_delta_s * cos(right_rad);
  right_delta_y = right_delta_s * sin(right_rad);

  delta_x = (left_delta_x + right_delta_x) / 2.0;
  delta_y = (left_delta_y + right_delta_y) / 2.0;

  len = sprintf(&print_buf[total_len], "dx:%f=lx:%f+rx:%f, dy:%f=ly:%f+ry:%f\n", delta_x, left_delta_x, right_delta_x, delta_y, left_delta_y, right_delta_y);
  total_len += len;

  x += delta_x;
  y += delta_y;

  len = sprintf(&print_buf[total_len], "x:%f y:%f\n\n", x, y);
  total_len += len;

  write(file_fd, print_buf, total_len);

  pose_update.x(delta_x);
  pose_update.y(delta_y);
  pose_update.heading(delta_heading);
  // TODO this line and the last statements are really ugly; refactor, put in another place
  // pose_update = diff_drive_kinematics.forward(tick_to_rad * left_diff_ticks, tick_to_rad * right_diff_ticks);

  if (curr_timestamp != last_timestamp)
  {
    last_diff_time = ((double)(short)((curr_timestamp - last_timestamp) & 0xffff)) / 1000.0f;
    last_timestamp = curr_timestamp;
    last_velocity_left = (tick_to_rad * left_diff_ticks) / last_diff_time;
    last_velocity_right = (tick_to_rad * right_diff_ticks) / last_diff_time;
  } else {
    // we need to set the last_velocity_xxx to zero?
  }

  pose_update_rates << pose_update.x()/last_diff_time,
                       pose_update.y()/last_diff_time,
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

void DiffDrive::setVelocityCommands(const double &vx, const double &wz, const double &yaw) {
  // vx: in m/s
  // wz: in rad/s
  std::vector<double> cmd_vel;
  cmd_vel.push_back(vx);
  cmd_vel.push_back(wz);
  cmd_vel.push_back(yaw);
  point_velocity = cmd_vel;
}

void DiffDrive::velocityCommands(const double &vx, const double &wz, const double &yaw) {
  // vx: in m/s
  // wz: in rad/s
  velocity_mutex.lock();
  const double epsilon = 0.0001;
  angle = yaw / M_PI * 180 * 100;// 0.01degree

  // Special Case #1 : Straight Run
  if( std::abs(wz) < epsilon ) {
    radius = 0.0f;
    speed  = 1000.0f * vx;
    velocity_mutex.unlock();
    return;
  }

  radius = vx * 1000.0f / wz;
  // Special Case #2 : Pure Rotation or Radius is less than or equal to 1.0 mm
  if( std::abs(vx) < epsilon || std::abs(radius) <= 1.0f ) {
    speed  = 1000.0f * std::sqrt(std::pow(bias / 2, 2) + std::pow(lng / 2, 2)) * wz;
    radius = 1.0f;
    velocity_mutex.unlock();
    return;
  }

  // General Case :
  // if( radius > 0.0f ) {
  speed  = 1000.0f * vx;//(radius + 1000.0f * bias / 2.0f) * wz;
  // } else {
    // speed  = (radius - 1000.0f * bias / 2.0f) * wz;
  // }
  velocity_mutex.unlock();
  return;
}

void DiffDrive::velocityCommands(const short &cmd_speed, const short &cmd_radius,  const short &cmd_angle) {
  velocity_mutex.lock();
  speed = static_cast<double>(cmd_speed);   // In [mm/s]
  radius = static_cast<double>(cmd_radius); // In [mm]
  angle = static_cast<double>(cmd_angle); // In [0.01degree]
  velocity_mutex.unlock();
  return;
}

std::vector<short> DiffDrive::velocityCommands() {
  velocity_mutex.lock();
  std::vector<short> cmd(3);
  cmd[0] = bound(speed);  // In [mm/s]
  cmd[1] = bound(radius); // In [mm]
  cmd[2] = bound(angle); // In [0.01degree]
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
