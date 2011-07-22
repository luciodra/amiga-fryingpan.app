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


#ifndef _GUIMUI_MUISPEEDSELECT_H_
#define _GUIMUI_MUISPEEDSELECT_H_

#include "MUICommon.h"
#include <Generic/ConfigParser.h>
#include <Generic/MUI/MUIWindowMenu.h>
#include <Generic/HookT.h>
#include <Generic/VectorT.h>
#include "Globals.h"

class MUISpeedSelect : public MUICommon
{
protected:
   Globals                   &Glb;
   VectorT<const char*>      names;
   VectorT<uint16>           speeds;
   iptr            *cycle;
   GenNS::MUIWindowMenu     *menu;
   iptr             *pager;
   iptr             *cyclegroup;
   HookAttrT<iptr, iptr>      hook;
   uint32                     id;

protected:
   HookT<MUISpeedSelect, iptr, void*>    hHkBtnHook;
   
protected:
   enum
   {
      ID_SwitchSpeed    =  1,
   };

protected:
   virtual iptr      btnHook(iptr id, void* data);
   virtual void               rebuildCycle(struct DiscSpeed *, uint16);
   const char*                dupstr(const char* str);

protected:
   static bool                vecDeleteName(const char* const& data);

public:
                              MUISpeedSelect(Globals &glb);
   virtual                   ~MUISpeedSelect();
   virtual iptr     *getObject();
   virtual void               setSpeeds(struct DiscSpeed*, uint16);
   virtual bool               start();
   virtual void               stop();
   virtual void               update();
   virtual void               setCallbackHook(const Hook *hk);
   virtual void               setID(int32 id);
};

#endif

