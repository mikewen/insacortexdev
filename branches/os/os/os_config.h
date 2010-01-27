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
 
#ifndef __OS_CONFIG_H__
#define __OS_CONFIG_H__

#define STACK_SIZE			32	/* Exprimé en u32 */
#define OS_STACK_SIZE 		32	/* Exprimé en u32 */

#define MAX_TASK_NBR		16
#define MAX_ALARM_NBR		8
#define MAX_RESOURCE_NBR	16
#define MAX_EVENT_NBR		16

// #define __WITH_EVENTS__

#endif /* __OS_CONFIG_H__ */
