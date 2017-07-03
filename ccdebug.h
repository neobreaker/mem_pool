#ifndef __CCDEBUG_H__
#define __CCDEBUG_H__

#include "ccdebug_core.h"
#include "ccdebug_port.h"
#include "stdio.h"

#define CC_DEBUG	1

/**
 * CC_DBG_MIN_LEVEL: After masking, the value of the debug is
 * compared against this value. If it is smaller, then debugging
 * messages are written.
 */
#ifndef CC_DBG_MIN_LEVEL
#define CC_DBG_MIN_LEVEL              CC_DBG_LEVEL_ALL
#endif

#endif

