/**
 * @file include/kmr_driver/packet_handler/payload_headers.hpp
 *
 * @brief Byte id's for the individual payload headers.
 *
 * Each part of a kmr packet carries one or more payload chunks. Each chunk
 * is id'd by one of the values here.
 *
 * License: BSD
 *   https://raw.github.com/yujinrobot/kmr_core/hydro-devel/kmr_driver/LICENSE
 **/
/*****************************************************************************
** Ifdefs
*****************************************************************************/

#ifndef KMR_PAYLOAD_HEADERS_HPP_
#define KMR_PAYLOAD_HEADERS_HPP_

/*****************************************************************************
** Includes
*****************************************************************************/

/*****************************************************************************
** Namespaces
*****************************************************************************/

namespace kmr {

class Header {
public:
  enum PayloadType {
  // Streamed payloads
  CoreSensors = 1, DockInfraRed = 3, Inertia = 4, Cliff = 5, Current = 6,

  // Service Payloads
  Hardware = 10, Firmware = 11, ThreeAxisGyro = 13, Eeprom = 15, GpInput = 16, Ultrasonic = 17,

  UniqueDeviceID = 19, Reserved = 20, ControllerInfo = 21
  };
};

} // namespace kmr

#endif /* KMR_PAYLOAD_HEADERS_HPP_ */
