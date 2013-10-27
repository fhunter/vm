/** \file peripherials.h
  * \author Nikolay Zamotaev
  */
#ifndef _PERIPHERIALS__H_
#define _PERIPHERIALS__H_

#include "lcd.h"
#include "buttons.h"

#define P_PERIPH_BASE	0xfc00
#define P_BUTTONS_BASE	P_PERIPH_BASE
#define P_BUTTONS_LENGTH	16
#define P_SYSCALL_BASE	(P_BUTTONS_BASE+ P_BUTTONS_LENGTH)
#define P_SYSCALL_LENGTH 128
#define P_TIMER_BASE	(P_SYSCALL_BASE+ P_SYSCALL_LENGTH)
#define P_TIMER_LENGTH  16
//FIXME: add assert to check that video RAM is not overrun
#define P_VIDEO_BASE	0xfe00
#define P_VIDEO_LENGTH  0x200

#endif
