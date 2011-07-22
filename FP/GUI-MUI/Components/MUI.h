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

#ifndef _GUIMUI_COMPONENTS_MUI_H_
#define _GUIMUI_COMPONENTS_MUI_H_

#include <Generic/GenericMUI.h>
#include <Generic/HookAttrT.h>
#include <Generic/HookT.h>
#include <Generic/VectorT.h>

using namespace GenNS;

class MUI : public virtual GenericMUI
{
public:
   enum Alignment
   {
      Align_Left  =  -1,
      Align_Center,
      Align_Right
   };

   enum ID
   {
      ID_Default  = -1
   };

protected:
   enum elemType
   {
      T_Unknown,
      T_Label,
      T_Button,
      T_CheckBox,
      T_Cycle,
      T_String,
      T_Gauge,
      T_Slider
   };

   struct elemAssoc 
   {
      elemType          type;
      int               id;
      unsigned long    *elem;

      elemAssoc(elemType t, int i, unsigned long *e) :
         type(t),
         id(i),
         elem(e)
      {
      };
   };

private:
   HookT<MUI, iptr, iptr>              gateBtns;
   HookAttrT<uint, iptr>               cbButtonHook;
   
protected:
   VectorT<elemAssoc*>                 muiElements;

   static bool             freeItem(elemAssoc* const& item);

protected:
   virtual unsigned long   gateButtons(iptr btn, iptr idptr);

protected:
   virtual void            setButtonCallBack(const Hook *hook);

public:
                           MUI();
   virtual                ~MUI();
   virtual uint32         *muiLabel(const char* contents, char key=0, int id=ID_Default, Alignment align=Align_Center);
   virtual uint32         *muiButton(const char* contents, char key=0, int id=ID_Default);
   virtual uint32         *muiCycle(const char** contents, char key=0, int id=ID_Default, int active=0);
   virtual uint32         *muiCheckBox(const char* name, char key=0, int id=ID_Default, Alignment align=Align_Left, bool checked=false);
   virtual uint32         *muiBar(const char *name=0, bool vertical=false);        
   virtual uint32         *muiString(const char* contents, char key=0, int id=ID_Default, Alignment align=Align_Left);
   virtual uint32         *muiGauge(const char* contents, int id=ID_Default);
   virtual uint32         *muiSpace(int weight=100);
   virtual uint32         *muiSlider(int32 min, int32 max, int32 level, char key=0, int id=ID_Default);

   virtual void            muiSetEnabled(int id, bool enabled);
   virtual void            muiSetSelected(int id, int selected);
   virtual void            muiSetVisible(int id, bool visible);
   virtual void            muiSetText(int id, const char *text);
   virtual void            muiSetValue(int id, int value);
};

#endif

