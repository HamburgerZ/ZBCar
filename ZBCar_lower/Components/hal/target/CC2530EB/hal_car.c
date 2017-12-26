/**************************************************************************************************
 *                                           INCLUDES
 **************************************************************************************************/
#include  "hal_types.h"
#include  "hal_car.h"
#include  "hal_mcu.h"

void hal_car_configure ( void )
{
  P0SEL &= ~0xF0;               //P0.6,P0.7,P0.4,P0.5
  P0DIR |= 0xF0;                //P0.6,P0.7,P0.4,P0.5
}

void hal_car_turnleft ( void )
{
  Driver_1 = 0;
  Driver_2 = 1;
  Driver_3 = 1;
  Driver_4 = 0;
}

void hal_car_turnright( void )
{
  Driver_1 = 1;
  Driver_2 = 0;
  Driver_3 = 0;
  Driver_4 = 1;
}

void hal_car_go ( void )
{
  Driver_1 = 1;
  Driver_2 = 0;
  Driver_3 = 1;
  Driver_4 = 0;
}

void hal_car_stop ( void )
{
  Driver_1 = 0;
  Driver_2 = 0;
  Driver_3 = 0;
  Driver_4 = 0;
}