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

#ifndef FILEREADER_H_
#define FILEREADER_H_

#include <Generic/LibrarySpool.h>
#include <Optical/Optical.h>
#include <dos/dos.h>
#include "IFileReader.h"
#include "DTLib.h"
#include <Generic/String.h>
#include <Generic/Debug.h>

using namespace GenNS;

class FileReader : public IFileReader
{
private:

   BPTR                       fh;
   String                     sFileName;
   uint64                     offset;     // in bytes
   uint64                     size;       // in bytes
   unsigned long              position;   // in blocks
   bool                       isLE;
   unsigned short             sectorSize;
   EDataType                  dataType;

protected:
   USING_DEBUG;

protected:
                                       FileReader(const char* sFileName, EDtError &rc);
   
   BPTR                                getFile();
   const char*                         getFileName();
   
   void                                setBlockSize(unsigned short size);
   void                                setLittleEndian(bool isLittle);
   void                                setType(EDataType aType);
   void                                setDataOffset(uint64 lOffset);
   void                                setDataSize(uint64 lSize);
   
   void                                initialize();
public:  // non pure-virtual
   virtual                            ~FileReader();
   virtual bool                        readData(const IOptItem* item, void* pBuff, int len);
   virtual bool                        setUp();
   virtual void                        cleanUp();
   static bool                         checkFile(const char* sFileName, EDtError &rc);
   virtual const char     	           *getTrackName();
   virtual void                        dispose();
   virtual unsigned long               getBlockCount();
   virtual unsigned short              getBlockSize();
   virtual bool                        isAudio();
   virtual bool                        isData();
   virtual bool                        fillOptItem(IOptItem *item);
};

#endif /*FILEREADER_H_*/
// vim: ts=3 et
