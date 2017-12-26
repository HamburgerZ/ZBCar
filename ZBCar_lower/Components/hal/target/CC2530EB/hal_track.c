#include  "hal_types.h"
#include  "hal_track.h"
#include  "hal_mcu.h"

void hal_track_configure ( void )
{
  P0SEL &= ~0x02;               //P0.0,P0.1
  P0DIR |= 0x02;
}