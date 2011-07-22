/*
 * FryingPan - Amiga CD/DVD Recording Software (User Intnerface and supporting Libraries only)
 * Copyright (C) 2001-2008 Tomasz Wiszkowski Tomasz.Wiszkowski at gmail.com
 * 
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public License
 * as published by the Free Software Foundation; either version 2.1
 * of the License, or (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */


#ifndef _GUIMUI_MUIPOPUNIT_H_
#define _GUIMUI_MUIPOPUNIT_H_

#include <Generic/MUI/MUIPopText.h>
#include "Globals.h"

class MUIPopUnit : public GenNS::MUIPopText
{
protected:
   struct Entry
   {
      String         unit;
      String         vendor;
      String         product;
      String         name;
      int            lun;
   };

protected:
   Globals                         &Glb;
   String                           sDevice;
   String                           sTemp;
   int                              unit;

protected:
   virtual bool                     onOpen();
   virtual bool                     onClose();
   virtual unsigned long            construct(void*, void* arg);
   virtual unsigned long            destruct(void*, void* arg);
   virtual unsigned long            display(const char** arr, void* arg);
   virtual const char              *toString(const void* arg);

public:
                                    MUIPopUnit(Globals &glb, const char* title, const char** cols);
   virtual                         ~MUIPopUnit();
   virtual void                     setDevice(const char *device);
   virtual const void*              getValue();
   virtual void                     setValue(const void* val);
};

#endif

