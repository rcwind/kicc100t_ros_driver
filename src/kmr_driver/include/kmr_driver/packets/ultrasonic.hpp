#ifndef KMR_ULTRASONIC_DATA_HPP__
#define KMR_ULTRASONIC_DATA_HPP__

#include "../packet_handler/payload_base.hpp"
#include "../packet_handler/payload_headers.hpp"

/*****************************************************************************
** Defines
*****************************************************************************/

#define MAX_ULT_DATA_SIZE (2*32) //derived from ST_GYRO_MAX_DATA_SIZE in firmware

/*****************************************************************************
** Namespaces
*****************************************************************************/

namespace kmr
{

/*****************************************************************************
** Interface
*****************************************************************************/

class Ultrasonic : public packet_handler::payloadBase
{
public:
  Ultrasonic() : packet_handler::payloadBase(true, 3) {};
  struct Data {
    unsigned char followed_data_length;
    unsigned short data[MAX_ULT_DATA_SIZE];
  } data;

  virtual ~Ultrasonic() {};

  bool serialise(ecl::PushAndPop<unsigned char> & byteStream)
  {
    unsigned char length = 1 + 2 * data.followed_data_length;
    buildBytes(Header::Ultrasonic, byteStream);
    buildBytes(length, byteStream);
    buildBytes(data.followed_data_length, byteStream);
    for (unsigned int i=0; i<data.followed_data_length; i++)
      buildBytes(data.data[i], byteStream);
    return true;
  }

  bool deserialise(ecl::PushAndPop<unsigned char> & byteStream)
  {
    if (byteStream.size() < length+2)
    {
      //std::cout << "kmr_node: three_axis_gyro: deserialise failed. not enough byte stream." << std::endl;
      return false;
    }

    unsigned char header_id, length_packed;
    buildVariable(header_id, byteStream);
    buildVariable(length_packed, byteStream);
    if( header_id != Header::Ultrasonic ) return false;
    if( length > length_packed ) return false;

    buildVariable(data.followed_data_length, byteStream);
    if( length_packed != 1 + 2 * data.followed_data_length ) return false;

    for (unsigned int i=0; i<data.followed_data_length; i++)
      buildVariable(data.data[i], byteStream);

    //showMe();
    return constrain();
  }

  bool constrain()
  {
    return true;
  }

  void showMe()
  {
  }
};

} // namespace kmr

#endif 

