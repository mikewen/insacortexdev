# Hey Emacs, this is a -*- makefile -*-

# The purpose of this file is to define the build configuration variables used
# by the generic Makefile. See Makefile header for further information.

# Copyright (c) 2009 Atmel Corporation. All rights reserved. 
#
# Redistribution and use in source and binary forms, with or without 
# modification, are permitted provided that the following conditions are met:
#
# 1. Redistributions of source code must retain the above copyright notice, this
# list of conditions and the following disclaimer. 
#
# 2. Redistributions in binary form must reproduce the above copyright notice, 
# this list of conditions and the following disclaimer in the documentation 
# and/or other materials provided with the distribution.
#
# 3. The name of Atmel may not be used to endorse or promote products derived 
# from this software without specific prior written permission.  
#
# 4. This software may only be redistributed and used in connection with an Atmel 
# AVR product. 
#
# THIS SOFTWARE IS PROVIDED BY ATMEL "AS IS" AND ANY EXPRESS OR IMPLIED 
# WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF 
# MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT ARE 
# EXPRESSLY AND SPECIFICALLY DISCLAIMED. IN NO EVENT SHALL ATMEL BE LIABLE FOR 
# ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES 
# (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; 
# LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND 
# ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT 
# (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS 
# SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE
#

# Base paths
PRJ_PATH = ../../../..
APPS_PATH = $(PRJ_PATH)/examples/avr32_a/test
TRAMPOLINE_OS_PATH = $(PRJ_PATH)/os
TRAMPOLINE_COM_PATH = $(PRJ_PATH)/com
TRAMPOLINE_SPECIFIC_PORT_PATH = $(PRJ_PATH)/machines/avr32_a
TRAMPOLINE_GENERATED_CONF_PATH = $(APPS_PATH)/avr32_test
TRAMPOLINE_DEBUG_PATH = $(PRJ_PATH)/debug

# CPU architecture: {ap|ucr1|ucr2}
ARCH = ucr2

# Part: {none|ap7xxx|uc3xxxxx}
PART = uc3a0512

# Flash memories: [{cfi|internal}@address,size]...
FLASH = internal@0x80000000,512Kb

# Clock source to use when programming: [{xtal|extclk|int}]
PROG_CLOCK = xtal

# Device/Platform/Board include path
PLATFORM_INC_PATH = \
#  $(BRDS_PATH)/

# Target name: {*.a|*.elf}
TARGET = $(PART)-avr32a-test.elf

# Definitions: [-D name[=definition]...] [-U name...]
# Things that might be added to DEFS:
#   BOARD             Board used: see $(BRDS_PATH)/board.h
#   EXT_BOARD         Extension board used (if any): see $(BRDS_PATH)/board.h
DEFS = -D BOARD=EVK1100

# Include path
INC_PATH = \
	../ \
	$(TRAMPOLINE_OS_PATH)/ \
	$(TRAMPOLINE_COM_PATH)/ \
	$(TRAMPOLINE_SPECIFIC_PORT_PATH)/ \
	$(TRAMPOLINE_GENERATED_CONF_PATH)/ \
	$(TRAMPOLINE_DEBUG_PATH)/ \

# C source files
TRAMPOLINE_OS = \
  $(TRAMPOLINE_OS_PATH)/tpl_os.c \
  $(TRAMPOLINE_OS_PATH)/tpl_os_action.c \
  $(TRAMPOLINE_OS_PATH)/tpl_os_alarm.c \
  $(TRAMPOLINE_OS_PATH)/tpl_os_alarm_kernel.c \
  $(TRAMPOLINE_OS_PATH)/tpl_os_dispatch_table.c \
  $(TRAMPOLINE_OS_PATH)/tpl_os_error.c \
  $(TRAMPOLINE_OS_PATH)/tpl_os_event.c \
  $(TRAMPOLINE_OS_PATH)/tpl_os_event_kernel.c \
  $(TRAMPOLINE_OS_PATH)/tpl_os_it.c \
  $(TRAMPOLINE_OS_PATH)/tpl_os_it_kernel.c \
  $(TRAMPOLINE_OS_PATH)/tpl_os_kernel.c \
  $(TRAMPOLINE_OS_PATH)/tpl_os_rez.c \
  $(TRAMPOLINE_OS_PATH)/tpl_os_rez_kernel.c \
  $(TRAMPOLINE_OS_PATH)/tpl_os_task.c \
  $(TRAMPOLINE_OS_PATH)/tpl_os_task_kernel.c \
  $(TRAMPOLINE_OS_PATH)/tpl_os_timeobj_kernel.c
  
TRAMPOLINE_COM = \
  $(TRAMPOLINE_COM_PATH)/tpl_com_queue.c \
  $(TRAMPOLINE_COM_PATH)/tpl_com_app_copy.c \
  $(TRAMPOLINE_COM_PATH)/tpl_com_errorhook.c \
  $(TRAMPOLINE_COM_PATH)/tpl_com_external_com.c \
  $(TRAMPOLINE_COM_PATH)/tpl_com_filtering.c \
  $(TRAMPOLINE_COM_PATH)/tpl_com_filters.c \
  $(TRAMPOLINE_COM_PATH)/tpl_com_internal_com.c \
  $(TRAMPOLINE_COM_PATH)/tpl_com_message_management.c \
  $(TRAMPOLINE_COM_PATH)/tpl_com_notification.c
  
TRAMPOLINE_PORT = \
  $(TRAMPOLINE_SPECIFIC_PORT_PATH)/tpl_machine.c
  
TRAMPOLINE_DEBUG = \
  $(TRAMPOLINE_DEBUG_PATH)/tpl_debug.c

TRAMPOLINE_GENERATED_CONF = \
  $(TRAMPOLINE_GENERATED_CONF_PATH)/tpl_os_generated_configuration.c
   	
CSRCS = \
  $(TRAMPOLINE_OS) \
  $(TRAMPOLINE_COM) \
  $(TRAMPOLINE_DEBUG) \
  $(TRAMPOLINE_GENERATED_CONF) \
  $(APPS_PATH)/avr32_test_main.c \
  $(APPS_PATH)/intc.c \
  $(TRAMPOLINE_PORT) \

# Assembler source files
ASSRCS = \
  $(APPS_PATH)/boot.S \
  $(APPS_PATH)/exception.S \

# Library path
LIB_PATH =
# LIB_PATH = $(UTIL_PATH)/LIBS/NEWLIB_ADDONS/AT32UCR2/

# Libraries to link with the project
LIBS =
# LIBS = newlib_addons-at32ucr2-debug

# Linker script file if any
LINKER_SCRIPT = link_uc3a0512.lds

# Options to request or suppress warnings: [-fsyntax-only] [-pedantic[-errors]] [-w] [-Wwarning...]
# For further details, refer to the chapter "GCC Command Options" of the GCC manual.
WARNINGS = -Wall

# Options for debugging: [-g]...
# For further details, refer to the chapter "GCC Command Options" of the GCC manual.
DEBUG = -g

# Options that control optimization: [-O[0|1|2|3|s]]...
# For further details, refer to the chapter "GCC Command Options" of the GCC manual.
OPTIMIZATION = -O0 -ffunction-sections -fdata-sections

# Extra flags to use when preprocessing
CPP_EXTRA_FLAGS =

# Extra flags to use when compiling
C_EXTRA_FLAGS =

# Extra flags to use when assembling
AS_EXTRA_FLAGS =

# Extra flags to use when linking
LD_EXTRA_FLAGS = -Wl,--gc-sections -Wl,-e,_entry_point

# Documentation path
DOC_PATH = \
  ../../DOC/

# Documentation configuration file
DOC_CFG = \
  ../doxyfile.doxygen
