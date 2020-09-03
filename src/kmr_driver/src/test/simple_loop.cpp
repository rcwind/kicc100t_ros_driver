/**                                                                           !
  * @file /kmr_driver/src/test/simple_loop.cpp
  *
  * @brief Example/test program with simple loop.
  *
  * It provides simple example of how interact with kmr by using c++ without ROS.
 **/

/*****************************************************************************
 * Includes
 ****************************************************************************/

#include <csignal>
#include <ecl/time.hpp>
#include <ecl/sigslots.hpp>
#include <ecl/geometry/legacy_pose2d.hpp>
#include <ecl/linear_algebra.hpp>
#include "kmr_driver/kmr.hpp"

/*****************************************************************************
** Classes
*****************************************************************************/

class KmrManager {
public:
  KmrManager() :
    dx(0.0), dth(0.0),
    slot_stream_data(&KmrManager::processStreamData, *this)
  {
    kmr::Parameters parameters;
    parameters.sigslots_namespace = "/kmr";
    parameters.device_port = "/dev/kmr";
    parameters.enable_acceleration_limiter = false;
    kmr.init(parameters);
    kmr.enable();
    slot_stream_data.connect("/kmr/stream_data");
  }

  ~KmrManager() {
    kmr.setBaseControl(0,0); // linear_velocity, angular_velocity in (m/s), (rad/s)
    kmr.disable();
  }

  void processStreamData() {
    ecl::LegacyPose2D<double> pose_update;
    ecl::linear_algebra::Vector3d pose_update_rates;
    kmr.updateOdometry(pose_update, pose_update_rates);
    pose *= pose_update;
    dx += pose_update.x();
    dth += pose_update.heading();
    //std::cout << dx << ", " << dth << std::endl;
    //std::cout << kmr.getHeading() << ", " << pose.heading() << std::endl;
    //std::cout << "[" << pose.x() << ", " << pose.y() << ", " << pose.heading() << "]" << std::endl;
    processMotion();
  }

  // Generate square motion
  void processMotion() {
    if (dx >= 1.0 && dth >= ecl::pi/2.0) { dx=0.0; dth=0.0; kmr.setBaseControl(0.0, 0.0); return; }
    else if (dx >= 1.0) { kmr.setBaseControl(0.0, 3.3); return; }
    else { kmr.setBaseControl(0.3, 0.0); return; }
  }

  ecl::LegacyPose2D<double> getPose() {
    return pose;
  }

private:
  double dx, dth;
  ecl::LegacyPose2D<double> pose;
  kmr::Kmr kmr;
  ecl::Slot<> slot_stream_data;
};

/*****************************************************************************
** Signal Handler
*****************************************************************************/

bool shutdown_req = false;
void signalHandler(int signum) {
  shutdown_req = true;
}

/*****************************************************************************
** Main
*****************************************************************************/

int main(int argc, char** argv)
{
  signal(SIGINT, signalHandler);

  std::cout << "Demo : Example of simple control loop." << std::endl;
  KmrManager kmr_manager;

  ecl::Sleep sleep(1);
  ecl::LegacyPose2D<double> pose;
  try {
    while (!shutdown_req){
      sleep();
      pose = kmr_manager.getPose();
      std::cout << "current pose: [" << pose.x() << ", " << pose.y() << ", " << pose.heading() << "]" << std::endl;
    }
  } catch ( ecl::StandardException &e ) {
    std::cout << e.what();
  }
  return 0;
}
