#ifndef IE_CORE_H
#define IE_CORE_H

#include "debug_memory/iedebugmemory.h"
#include "iecoredefinitions.h"
#include "config/ieconfig.h"
#include "draw/ieprimitive.h"
#include "event/ieevent.h"
#include "log/ielog.h"
#include "loop/ieloop.h"
#include "window/iewindow.h"

void ie_core_startup(void);
void ie_core_shutdown(void);

#endif
