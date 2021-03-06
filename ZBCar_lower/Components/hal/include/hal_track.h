


#ifndef HAL_TRACK_H
#define HAL_TRACK_H

#ifdef __cplusplus
extern "C"
{
#endif

/**************************************************************************************************
 * INCLUDES
 **************************************************************************************************/
#include "hal_board.h"
#include "hal_types.h"

/**************************************************************************************************
 * MACROS
 **************************************************************************************************/

/**************************************************************************************************
 * CONSTANTS
 **************************************************************************************************/

/* Resolution */
#define Color_1 P0_0
#define Color_2 P0_1

/**************************************************************************************************
 *                                        GLOBAL VARIABLES
 **************************************************************************************************/

/**************************************************************************************************
 *                                        FUNCTIONS - API
 **************************************************************************************************/

/**************************************************************************************************
**************************************************************************************************/
extern void hal_track_configure ( void );

#ifdef __cplusplus
}
#endif

#endif
