/**
 * @file /include/kmr_driver/macros.hpp
 *
 * @brief Macros for kmr_driver.
 *
 * @date April, 2013
 **/
/*****************************************************************************
** Ifdefs
*****************************************************************************/

#ifndef KMR_DRIVER_MACROS_HPP_
#define KMR_DRIVER_MACROS_HPP_

/*****************************************************************************
** Includes
*****************************************************************************/

#include <ecl/config/macros.hpp>
#include <ecl/config/ecl.hpp>

/*****************************************************************************
** Declspecs
*****************************************************************************/

/*
 * Import/exports symbols for the library
 */
#ifdef ECL_HAS_SHARED_LIBS // ecl is being built around shared libraries
  #ifdef kmr_EXPORTS // we are building a shared lib/dll
    #define kmr_PUBLIC ECL_HELPER_EXPORT
    #define EXP_TEMPLATE
  #else // we are using shared lib/dll
    #define kmr_PUBLIC ECL_HELPER_IMPORT
    #define EXP_TEMPLATE extern
  #endif
  #define kmr_LOCAL ECL_HELPERS_LOCAL
#else // ecl is being built around static libraries
  #define kmr_PUBLIC
  #define kmr_LOCAL
  #define EXP_TEMPLATE
#endif

#endif /* KMR_DRIVER_MACROS_HPP_ */
