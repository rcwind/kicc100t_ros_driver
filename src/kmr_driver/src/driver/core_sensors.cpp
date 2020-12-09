/**
 * @file /kmr_driver/src/driver/core_sensors.cpp
 *
 * @brief Implementation of the core sensor packet data.
 *
 * License: BSD
 *   https://raw.github.com/yujinrobot/kmr_core/hydro-devel/kmr_driver/LICENSE
 **/

/*****************************************************************************
** Includes
*****************************************************************************/

#include "../../include/kmr_driver/packets/core_sensors.hpp"
#include "../../include/kmr_driver/packet_handler/payload_headers.hpp"

/*****************************************************************************
** Namespaces
*****************************************************************************/

namespace kmr {

/*****************************************************************************
** Implementation
*****************************************************************************/

bool CoreSensors::serialise(ecl::PushAndPop<unsigned char> & byteStream)
{
  buildBytes(Header::CoreSensors, byteStream);
  buildBytes(length, byteStream);
  buildBytes(data.time_stamp, byteStream);	//2
  buildBytes(data.bumper, byteStream);		//1
  // buildBytes(data.wheel_drop, byteStream);	//1
  // buildBytes(data.cliff, byteStream);		//1
  buildBytes(data.left_steering, byteStream);	//2
  buildBytes(data.left_encoder, byteStream);	//2
  buildBytes(data.right_encoder, byteStream);	//2
  buildBytes(data.right_steering, byteStream);	//2
  // buildBytes(data.left_pwm, byteStream);	//1
  // buildBytes(data.right_pwm, byteStream);	//1
  buildBytes(data.buttons, byteStream);		//1
  buildBytes(data.charger, byteStream);		//1
  buildBytes(data.battery, byteStream);		//1
  buildBytes(data.over_current, byteStream);	//1

  return true;
}
bool CoreSensors::deserialise(ecl::PushAndPop<unsigned char> & byteStream)
{
  if (byteStream.size() < length+2)
  {
    //std::cout << "kmr_node: kmr_default: deserialise failed. not enough byte stream." << std::endl;
    return false;
  }

  unsigned char header_id, length_packed;
  buildVariable(header_id, byteStream);
  buildVariable(length_packed, byteStream);
  if( header_id != Header::CoreSensors ) return false;
  if( length_packed != length ) return false;

  buildVariable(data.time_stamp, byteStream);
  buildVariable(data.bumper, byteStream);
  // buildVariable(data.wheel_drop, byteStream);
  // buildVariable(data.cliff, byteStream);
  buildVariable(data.left_steering, byteStream);
  buildVariable(data.left_encoder, byteStream);
  buildVariable(data.right_encoder, byteStream);
  buildVariable(data.right_steering, byteStream);
  // buildVariable(data.left_pwm, byteStream);
  // buildVariable(data.right_pwm, byteStream);
  buildVariable(data.buttons, byteStream);
  buildVariable(data.charger, byteStream);
  buildVariable(data.battery, byteStream);
  buildVariable(data.over_current, byteStream);

  return true;
}



} // namespace kmr
