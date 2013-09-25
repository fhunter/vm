/** \file buttons.h
  * \author Nikolay Zamotaev
  * \brief API for accessing watch buttons/keyboard
  */
#ifndef _BUTTONS__H_
#define _BUTTONS__H_
#include <stdint.h>

uint8_t kbhit(  );
uint8_t iskeyevent(  );
uint16_t getkeyevent(  );

#endif
