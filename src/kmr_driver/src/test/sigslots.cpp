/**
 * @file /kmr_driver/src/test/sigslots.cpp
 *
 * @brief Example/test program for kmr sigslots.
 **/
/*****************************************************************************
** Includes
*****************************************************************************/

#include <ecl/time.hpp>
#include <ecl/sigslots.hpp>
#include <iostream>
#include <kmr_driver/kmr.hpp>

/*****************************************************************************
** Classes
*****************************************************************************/

class KmrManager {
public:
  KmrManager() :
      slot_stream_data(&KmrManager::processStreamData, *this) // establish the callback
  {
    kmr::Parameters parameters;
    parameters.sigslots_namespace = "/mobile_base"; // configure the first part of the sigslot namespace
    parameters.device_port = "/dev/kmr";         // the serial port to connect to (windows COM1..)
    // configure other parameters here
    kmr.init(parameters);
    slot_stream_data.connect("/mobile_base/stream_data");
  }

  void spin() {
    ecl::Sleep sleep(1);
    while ( true ) {
      sleep();
    }
  }

  /*
   * Called whenever the kmr receives a data packet. Up to you from here to process it.
   *
   * Note that special processing is done for the various events which discretely change
   * state (bumpers, cliffs etc) and updates for these are informed via the xxxEvent
   * signals provided by the kmr driver.
   */
  void processStreamData() {
    kmr::CoreSensors::Data data = kmr.getCoreSensorData();
    std::cout << "Encoders [" <<  data.left_encoder << "," << data.right_encoder << "]" << std::endl;
  }

private:
  kmr::Kmr kmr;
  ecl::Slot<> slot_stream_data;
};

/*****************************************************************************
** Main
*****************************************************************************/

int main() {
  KmrManager kmr_manager;
  kmr_manager.spin();
  return 0;
}
