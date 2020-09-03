/**
 * @file include/kmr_driver/packets/current.hpp
 *
 * @brief Current level packet payloads.
 *
 * License: BSD
 *   https://raw.github.com/yujinrobot/kmr_core/hydro-devel/kmr_driver/LICENSE
 */
/*****************************************************************************
** Preprocessor
*****************************************************************************/

#ifndef KMR_CURRENT_DATA_HPP__
#define KMR_CURRENT_DATA_HPP__

/*****************************************************************************
** Include
*****************************************************************************/

#include <vector>
#include "../packet_handler/payload_base.hpp"
#include "../packet_handler/payload_headers.hpp"

/*****************************************************************************
** Namespace
*****************************************************************************/

namespace kmr
{

/*****************************************************************************
** Interface
*****************************************************************************/
/**
 * This comes back in the streamed feedback. It has two values
 * (left and right) indicating the supplied current which can be useful for
 * detecting when the robot is blocked.
 */
class Current : public packet_handler::payloadBase
{
public:
  Current() : packet_handler::payloadBase(false, 2) {};
  struct Data {
    Data() : current(2) {}
    std::vector<uint8_t> current;
  } data;

  bool serialise(ecl::PushAndPop<unsigned char> & byteStream)
  {
    buildBytes(Header::Current, byteStream);
    buildBytes(length, byteStream);
    buildBytes(data.current[0], byteStream);
    buildBytes(data.current[1], byteStream);
    return true;
  }

  bool deserialise(ecl::PushAndPop<unsigned char> & byteStream)
  {
    if (byteStream.size() < length+2)
    {
      //std::cout << "kmr_node: kmr_current: deserialise failed. not enough byte stream." << std::endl;
      return false;
    }

    unsigned char header_id, length_packed;
    buildVariable(header_id, byteStream);
    buildVariable(length_packed, byteStream);
    if( header_id != Header::Current ) return false;
    if( length_packed != length ) return false;

    buildVariable(data.current[0], byteStream);
    buildVariable(data.current[1], byteStream);

    return constrain();
  }

  bool constrain()
  {
    return true;
  }

  void showMe()
  {
    //printf("--[%02x || %03d | %03d | %03d]\n", data.bump, current[2], current[1], current[0] );
  }
};

} // namespace kmr

#endif /* KMR_CURRENT_DATA_HPP__ */
