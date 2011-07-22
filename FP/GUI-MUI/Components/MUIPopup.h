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

#ifndef _GUIMUI_COMPONENTS_MUIPOPUP_H_
#define _GUIMUI_COMPONENTS_MUIPOPUP_H_

#include <Generic/HookAttrT.h>
#include "MUI.h"
#include <Generic/HookT.h>
#include <Generic/String.h>

class MUIPopup : public MUI 
{
protected:
   unsigned long                *all;
   unsigned long                *display;
   unsigned long                *button;
   unsigned long                *object;
   unsigned long                *label;
   unsigned long                *popup;
   String                        name;

protected:
   HookT<MUIPopup, void*, void*> hHkOpen;
   HookT<MUIPopup, void*, void*> hHkClose;
   HookAttrT<uint, const void*>  hNotify;
   int                           id;

private:
   virtual unsigned long         doOpen(void*, void*);
   virtual unsigned long         doClose(void*, void*);

protected:
   virtual bool                  onOpen()                         =  0;
   virtual bool                  onClose()                        =  0;
   virtual unsigned long        *getPopDisplay()                  =  0;
   virtual unsigned long        *getPopButton()                   =  0;
   virtual unsigned long        *getPopObject()                   =  0;

   virtual void                  update();
public:
   virtual void                  setValue(const void* string)     =  0;
   virtual const void           *getValue()                       =  0;

                                 MUIPopup(const char* name);
   virtual                      ~MUIPopup();
   virtual void                  setID(int id);
   virtual void                  setCallbackHook(const Hook*);
   virtual unsigned long        *getObject();
   virtual void                  open();
   virtual void                  close();
};

#endif

