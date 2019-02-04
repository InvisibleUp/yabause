/*  Copyright 2003-2005 Guillaume Duhamel
    Copyright 2004-2006 Theo Berkau
    Copyright 2018-2019 Allison Hancock

    This file is part of Yabause.

    Yabause is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    Yabause is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Yabause; if not, write to the Free Software
    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301  USA
*/

#ifndef VDP1DEBUG_H
#define VDP1DEBUG_H

#include "memory.h"
#include "vdp1.h"

void Vdp1DebugGetCommandNumberName(u32 number, char *outstring);
void Vdp1DebugCommand(u32 number, char *outstring);
u32 *Vdp1DebugTexture(u32 number, int *w, int *h);
u32 *Vdp1DebugGouraudOverlay(u32 number, int w, int h);
u32 Vdp1DebugGouraudPoint(int x, int y, int w, int h, COLOR *colors);

#endif