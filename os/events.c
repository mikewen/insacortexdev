#include "stm32f10x_type.h"
#include "config.h"
#include "kernel.h"
#include "events.h"

#ifdef __WITH_EVENTS__

/* Event services declaration */
EventMaskType	DeclareEvent(st_EventInfo *EventInfo)
{
}

StatusType		SetEvent_Int(TaskType TaskID, EventMaskType Mask)
{
}

StatusType		ClearEvent_Int(EventMaskType Mask)
{
}

StatusType		GetEvent(TaskType TaskID, EventMaskType Mask)
{
}

StatusType		WaitEvent_Int(EventMaskType Mask)
{
}

#endif /* __WITH_EVENTS__ */

