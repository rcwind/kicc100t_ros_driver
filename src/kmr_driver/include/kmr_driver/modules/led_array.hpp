/**
 * @file /kmr_driver/include/kmr_driver/modules/led_array.hpp
 *
 * @brief Definitions for manipulating the led's.
 *
 * License: BSD
 *   https://raw.github.com/yujinrobot/kmr_core/hydro-devel/kmr_driver/LICENSE
 **/
/*****************************************************************************
** Ifdefs
*****************************************************************************/

#ifndef KMR_LED_ARRAY_HPP_
#define KMR_LED_ARRAY_HPP_

/*****************************************************************************
** Includes
*****************************************************************************/

/*****************************************************************************
** Namespaces
*****************************************************************************/

namespace kmr {

/*****************************************************************************
** Enums
*****************************************************************************/

/**
 * The led's count from left to right.
 */
enum LedNumber {
  Led1 = 0,  //!< Led1
  Led2 = 1   //!< Led2
};

enum LedColour {
  Black = 0x00,
  Red = 0x100,
  Green = 0x200,
  Orange = 0x300,
};

} // namespace kmr

#endif /* LED_ARRAY_HPP_ */
