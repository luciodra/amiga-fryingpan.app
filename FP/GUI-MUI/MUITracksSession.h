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

#ifndef _GUIMUI_MUITRACKSESSION_H_
#define _GUIMUI_MUITRACKSSESSION_H_

#include <Generic/HookT.h>
#include <Generic/LibrarySpool.h>
#include "Components/MUITree.h"
#include "Components/MUIPopAsl.h"
#include "MUICommon.h"
#include <Optical/IOptItem.h>
#include "Globals.h"
#include <Generic/ConfigParser.h>

using namespace GenNS;

class MUITracksSession : public MUICommon
{
protected:
   struct Entry
   {
      const IOptItem   *item;
      String            number;
      String            name;
      String            info;
   };

protected:
   enum BtnID
   {
      Btn_SetSession =  100
   };

protected:
   Globals                   &Glb;
   unsigned long             *all;
   MUITree                   *tracks;
   ConfigParser              *Config;
   MUIPopAsl                 *target;

protected:
   HookT<MUITracksSession, const char**, Entry*>     hHkDisplay;
   HookT<MUITracksSession, void*, const IOptItem*>   hHkConstruct;
   HookT<MUITracksSession, void*, Entry*>            hHkDestruct;
   HookT<MUITracksSession, BtnID, void*>             hHkButton;

protected:

   DbgHandler                *getDebug();

protected:
   virtual uint32             display(const char**, Entry*);
   virtual uint32             construct(void*, const IOptItem*);
   virtual uint32             destruct(void*, Entry*);
   virtual uint32             button(BtnID, void*);
   virtual void               addRecurse(unsigned long parent, const IOptItem* data);

public:
                              MUITracksSession(ConfigParser *parent, Globals &glb);
   virtual                   ~MUITracksSession();
   virtual unsigned long     *getObject();
   virtual bool               start();
   virtual void               stop();
   virtual void               update();
   virtual const char        *getName();
};


#endif

