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

#define _NB_VOICES_ 4

#define _DO_	0
#define _DO_D_	1
#define _RE_	2
#define _RE_D_	3
#define _MI_	4
#define _FA_	5
#define _FA_D_	6
#define _SOL_	7
#define _SOL_D_	8
#define _LA_	9
#define _LA_D_	10
#define _SI_	11

#define _VOICE_OFF_	0xFF
#define _VOICE_EMPTY_	0xFE

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
	u8 release;
};

extern struct VOICE_ST voices[_NB_VOICES_];
extern u16 note_array[5][12];
extern volatile u16 voice_buffer[4];

void Init_Voice (void);
void Regle_Canal(int canal, int note, int octave);

#endif /* _VOICE_H_ */

