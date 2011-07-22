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

#ifndef __DISC_DVD_MINUSRW_H
#define __DISC_DVD_MINUSRW_H

#include "Disc_DVD_MinusR.h"

using namespace GenNS;

class Disc_DVD_MinusRW : public Disc_DVD_MinusR
{
   class cmd_ReadFormatCapacities  *readfmtcaps;
   class CfgIOMeas                 *meas;
public:
            Disc_DVD_MinusRW(class Drive*);
   virtual                ~Disc_DVD_MinusRW(void);

   virtual void            Init(void); // OUGHT TO BE INHERITED EVERYWHERE!

   virtual bool            AllowMultiSessionLayout()  {  return false;                    };
   virtual bool            AllowMultiTrackLayout()    {  return false;                    };
   virtual int             SessionGapSize()           {  return 0;                        };
   virtual int             TrackGapSize()             {  return 0;                        };
   virtual bool            IsErasable(void)           {  return true;                     };
   virtual bool            IsFormatable(void)         {  return true;                     };
   virtual int             DiscSubType(void)          {  return 0;                        };

   virtual int             DiscType(void);
   virtual bool            IsFormatted(void);
   virtual bool            IsWritable(void);
   virtual bool            IsOverwritable(void);
   virtual int             EraseDisc(int);
   virtual int             FormatDisc(int);
   virtual int             StructureDisc(void);
   virtual int16           GetOperationProgress();
   virtual uint32          GetDiscSize();
};


#endif
