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

#ifndef _W_AIFFAUDIO_H_ 
#define _W_AIFFAUDIO_H_ 

#include "FileWriter.h"
#include <Generic/HookT.h>
#include <Generic/DynList.h>


class wAIFFAudio : public FileWriter
{
   long                          offTotal;      // offset of FORM size
   long                          offSamples;    // offset of samples count
   long                          offSoundChunk; // offset of sound chunk size
protected:
                                 wAIFFAudio(const char* sFileName, const IOptItem *it, EDtError &rc);
public:
   static IFileWriter           *openWrite(const char* sFile, const IOptItem *it, EDtError &rc);
   
   virtual const char           *getName();
   virtual bool                  isAudio();
   virtual bool                  isData();

   static bool                   static_isAudio();
   static bool                   static_isData();
   static bool                   static_isSession();
   static const char            *static_getName();

   virtual bool                  setUp();
   virtual void                  cleanUp();
};

#endif /*RAWAUDIO_H_*/
