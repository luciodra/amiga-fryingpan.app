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


#ifndef _FP_GUIMUI_MUIPAGESELECT_H_
#define _FP_GUIMUI_MUIPAGESELECT_H_

#include <Generic/GenericMUI.h>
#include <Generic/ConfigParser.h>
#include <Generic/HookT.h>
#include "MUICommon.h"
#include <Generic/HookAttrT.h>
#include "Globals.h"

class GenNS::ConfigParser;
class MUIToolBar;

using namespace GenNS;

class MUIPageSelect : public MUICommon
{
protected:
   iptr                            *all;
   ConfigParser                             *Config;
   MUIToolBar                               *ToolBar;
   Globals                                  &Glb;
   HookT<MUIPageSelect, MUIToolBar*, iptr>   hPageSel;
   HookAttrT<MUIPageSelect*, iptr>           callBack;

protected:
   iptr                       pageSel(MUIToolBar*, iptr page);
public:

                              MUIPageSelect(ConfigParser *parent, Globals &glb);
   virtual                   ~MUIPageSelect();
   virtual bool               start();
   virtual void               stop();
   virtual iptr     *getObject();
   virtual void               update();
   virtual void               setHook(const Hook *hook);
};

#endif

