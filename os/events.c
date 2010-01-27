/*
 * Copyright (C) INSA Toulouse
 * Author: Sebastien DI MERCURIO
 *
 * This file is part of INSA OS.
 *
 * INSA OS is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public
 * License as published by the Free Software Foundation;
 * either version 2, or (at your option) any later version.
 *
 * INSA OS is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 * PURPOSE.  See the GNU General Public License for more
 * details.
 *
 * You should have received a copy of the GNU General Public
 * License along with INSA OS; see the file COPYING.  If not,
 * write to the Free Software Foundation, Inc., 51 Franklin Street,
 * Fifth Floor, Boston, MA 02110-1301, USA.
 */
 
#include "stm32f10x_type.h"
#include "os_config.h"
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

