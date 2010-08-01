/*
 * Copyright (C) INSA Toulouse
 * Author: Sebastien DI MERCURIO
 *
 * This file is part of MinOS.
 *
 * MinOS is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public
 * License as published by the Free Software Foundation;
 * either version 2, or (at your option) any later version.
 *
 * MinOS is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 * PURPOSE.  See the GNU General Public License for more
 * details.
 *
 * You should have received a copy of the GNU General Public
 * License along with MinOS; see the file COPYING.  If not,
 * write to the Free Software Foundation, Inc., 51 Franklin Street,
 * Fifth Floor, Boston, MA 02110-1301, USA.
 */

#ifndef _VOICE_H_
#define _VOICE_H_

#define _NB_VOICES_ 8

struct VOICE_ST
{
	u16 freq_base;
	u16 freq_sweep;
	u8 base_waveform;
	u8 am_waveform;
	u16 freq_am;
	u16 freq_fm;
	u8 fm_waveform;
	u8 base_volume;
	u8 am_volume;
	u8 fm_volume;
	u8 attack;
	u8 decay;
	u8 sustain;
	u8 recall;
};

extern struct VOICE_ST voices[_NB_VOICES_];

#endif /* _VOICE_H_ */

