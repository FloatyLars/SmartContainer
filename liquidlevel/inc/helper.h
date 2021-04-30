/******************************************************************************
 * File           : Helper funcions used throughout all targets
******************************************************************************/
#ifndef _HELPER_H_
#define _HELPER_H_

#include "stm32f0xx.h"

// ----------------------------------------------------------------------------
// Function prototypes
// ----------------------------------------------------------------------------

#define TIME_DELAY SystemCoreClock / 8

void  delay(const int d);

#endif /* _HELPER_H_ */
