/**
 * @file /kmr_driver/include/kmr_driver/modules/digital_output.hpp
 *
 * @brief Digital output flags.
 *
 * License: BSD
 *   https://raw.github.com/yujinrobot/kmr_core/hydro-devel/kmr_driver/LICENSE
 **/
/*****************************************************************************
 ** Ifdefs
 *****************************************************************************/

#ifndef KMR_DIGITAL_OUTPUT_HPP_
#define KMR_DIGITAL_OUTPUT_HPP_

/*****************************************************************************
 ** Namespaces
 *****************************************************************************/

namespace kmr
{

/*****************************************************************************
 ** Structures
 *****************************************************************************/
/**
 * Convenient class for setting values for digital output pins.
 */
struct DigitalOutput {
  DigitalOutput() {
    for ( unsigned int i = 0; i < 4; ++i ) {
      values[i] = false;
      mask[i] = false;
    }
  }
  bool values[4]; /**< Digital on or off for pins 0-3 respectively. **/
  bool mask[4]; /**< Set indices to true to set a pin, false to ignore. **/
};


} // namespace kmr

#endif /* KMR_DIGITAL_OUTPUT_HPP_ */
