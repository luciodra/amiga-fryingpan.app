/*
 * FryingPan - Amiga CD/DVD Recording Software (User Intnerface and supporting Libraries only)
 * Copyright (C) 2001-2011 Tomasz Wiszkowski Tomasz.Wiszkowski at gmail.com
 * 
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public License
 * as published by the Free Software Foundation; either version 2.1
 * of the License, or (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

#ifndef _GUIMUI_MUIPOPDOSDEVICE_H_
#define _GUIMUI_MUIPOPDOSDEVICE_H_

#include "Components/MUIPopString.h"

class MUIPopDOSDevice : public MUIPopString
{
   VectorT<String*>           data;
protected:
   virtual bool               onOpen();
   virtual bool               onClose();
   virtual const char        *toString(const void* arg);
   static bool                freeString(String* const& s);
public:
                              MUIPopDOSDevice();
   virtual                   ~MUIPopDOSDevice();
};

#endif

