#ifndef	__msvc_h__
#define __msvc_h__

/*
 *  vc7inc.h
 *  Nuvie
 *
 *  Created by Nelno the Amoeba on Fri Nov 14 13 2003.
 *  Copyright (c) 2003. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 *
 */

#define __func__ __FUNCTION__

#define strcasecmp	_stricmp
#define strdup		_strdup

#include <locale>
#include <direct.h>
#include <math.h>
#include <assert.h>

#pragma	warning (disable : 4244)	//	conversion from 'x' to 'x', possible loss of data
#pragma	warning (disable : 4100)	//	unreferenced formal parameter
#pragma warning (disable : 4800)	//	forcing value to bool 'true' or 'false' (performance warning)
#pragma warning (disable : 4121)	//	alignment of member was sensitive to packing
#pragma warning (disable : 4068)	//	unknown pragma
#pragma warning (disable : 4099)	//	'$' : type name first seen using 'class' now seen using 'struct'
#pragma warning (disable : 4996)	//	'_strdup': The POSIX name for this item is deprecated. Instead, use the ISO C++ conformant name: _strdup.


#if _MSC_VER < 1600
__inline float roundf (float val) { return fmod( val, 1.0f ) >= 0.5f ? ceilf (val) : floorf (val); }
#define nearbyint roundf
#define mkdir _mkdir
#endif



#endif	/* __msvc_h__ */