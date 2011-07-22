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

#ifndef _GUIMUI_MUIDRIVE_H_
#define _GUIMUI_MUIDRIVE_H_

#include <Generic/GenericMUI.h>
#include "MUICommon.h"
#include "MUIPopDevice.h"
#include "MUIPopUnit.h"
#include <Generic/HookT.h>
#include "Globals.h"
#include "../IEngine.h"
#include <Generic/ConfigParser.h>

class DiscSpeed;

using namespace GenNS;

class MUIDrive : public MUICommon
{
protected:
   enum
   {
      ID_DeviceSelect      =  1,
      ID_UnitSelect,
      
      ID_DriveVendor       =  10,
      ID_DriveProduct,
      ID_DriveVersion,

      ID_MechanismType     =  20,
      ID_ReadableMedia,
      ID_WritableMedia,
      ID_SupportedData,
      ID_Burnproof,
      ID_ISRC,
      ID_MCN,
      ID_AudioStream,
      ID_TestMode,
      ID_Multisession,
      ID_CDText,
      ID_ReadSpeeds,
      ID_WriteSpeeds,
      ID_DriveStatus,
      ID_AudioPlayback
   };

protected:
   Globals                   &Glb;
   unsigned long             *all;
   ConfigParser              *Config;

   unsigned long             *vendor;
   unsigned long             *product;
   unsigned long             *version;

   unsigned long             *mechanism;
   unsigned long             *readmedia;
   unsigned long             *writemedia;
   unsigned long             *writedata;
   unsigned long             *burnproof;
   unsigned long             *isrc;  
   unsigned long             *mcn;    
   unsigned long             *audiostream;
   unsigned long             *testwrite;
   unsigned long             *multisession;
   unsigned long             *buffersize; 
   unsigned long             *cdtext;    
   unsigned long             *readspeeds; 
   unsigned long             *writespeeds;
   unsigned long             *state;          
   uint                      *audioplayback;

   MUIPopDevice              *popDevice;
   MUIPopUnit                *popUnit;

   String                     sDevice;
   int                        lUnit;
protected:
   HookT<MUIDrive, int, void*>            hHkButtonHook;
   DbgHandler                *getDebug();

protected:
   virtual unsigned long      buttonHook(int id, void* data);
   virtual String             mechanismToString(DRT_Mechanism);
   virtual String             mediaToString(unsigned long);
   virtual String             dataToString(unsigned long);
   virtual String             speedsToString(DiscSpeed*);

public:

                              MUIDrive(ConfigParser *parent, Globals &glb);
   virtual                   ~MUIDrive();
   virtual bool               start();
   virtual void               update();
   virtual void               stop();
   virtual unsigned long     *getObject();

};

#endif

