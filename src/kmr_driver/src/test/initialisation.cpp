/**
 * @file /kmr_driver/src/test/initialisation.cpp
 *
 * @brief Demo program for kmr initialisation.
 **/
/*****************************************************************************
** Includes
*****************************************************************************/

#include <iostream>
#include <kmr_driver/kmr.hpp>
#include <ecl/time.hpp>

class KmrManager {
public:
  KmrManager() {
    kmr::Parameters parameters;
    // change the default device port from /dev/kmr to /dev/ttyUSB0
    parameters.device_port = "/dev/ttyUSB0";
    // Other parameters are typically happy enough as defaults
    // namespaces all sigslot connection names under this value, only important if you want to
    parameters.sigslots_namespace = "/kmr";
    // Most people will prefer to do their own velocity smoothing/acceleration limiting.
    // If you wish to utilise kmr's minimal acceleration limiter, set to true
    parameters.enable_acceleration_limiter = false;
    // If your battery levels are showing significant variance from factory defaults, adjust thresholds.
    // This will affect the led on the front of the robot as well as when signals are emitted by the driver.
    parameters.battery_capacity = 16.5;
    parameters.battery_low = 14.0;
    parameters.battery_dangerous = 13.2;

    // initialise - it will throw an exception if parameter validation or initialisation fails.
    try {
      kmr.init(parameters);
    } catch ( ecl::StandardException &e ) {
      std::cout << e.what();
    }
  }
private:
  kmr::Kmr kmr;
};

int main() {
  KmrManager kmr_manager;
  ecl::Sleep()(5);
  return 0;
}
