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


#ifndef _GUIMUI_COMPONENTS_POPACTION_H_
#define _GUIMUI_COMPONENTS_POPACTION_H_

#include <Generic/MUI/MUIPopup.h>
#include <Generic/MUI/MUI.h>
#include <Generic/HookT.h>
#include <Generic/HookAttrT.h>
#include <Generic/String.h>
#include "Globals.h"

class MUIPopAction : public GenNS::MUIPopup
{
protected:
   enum
   {
      ID_MediaType,
      ID_DiscSize,
      ID_PrepareWrite,
      ID_Eject,
      ID_Load
   };
protected:
   Globals                         &Glb;
   HookAttrT<void*, void*>          hCallback;

protected:
   HookT<MUIPopAction, int, void*>  hHkBtnHook;

protected:
   virtual bool                     onOpen();
   virtual bool                     onClose();
   virtual unsigned long           *getPopDisplay();
   virtual unsigned long           *getPopButton();
   virtual unsigned long           *getPopObject();
   virtual unsigned long            buttonHandler(int id, void* data);

   String                           discType();
   String                           discSize();

public:
                                    MUIPopAction(Globals &glb);
   virtual                         ~MUIPopAction();                                    
   virtual void                     setValue(const void* string);
   virtual const void              *getValue();
   void                             update();
};

#endif

