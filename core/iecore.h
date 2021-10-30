#ifndef IE_CORE_H
#define IE_CORE_H

#include "core/debug_memory/iedebugmemory.h"
#include "core/iecoredefinitions.h"
#include "core/config/ieconfig.h"
#include "core/draw/ieprimitive.h"
#include "core/event/ieevent.h"
#include "core/log/ielog.h"
#include "core/loop/ieloop.h"
#include "core/window/iewindow.h"

void ie_core_startup(void);
void ie_core_shutdown(void);

#endif
