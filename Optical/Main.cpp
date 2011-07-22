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

#include "../Generic/LibrarySpool.h"
#include "Headers.h"
#include "Drive.h"
#include "Main.h"
#include <exec/memory.h>
#include "Humming.h"
#include "Config.h"
#include "IOptItem.h"
#include "OptDisc.h"
#include "OptTrack.h"

#include <libclass/utility.h>

uint32 StartupFlags = 0;

using namespace GenNS;

   Config               *Cfg;
   bool                  AllClear;
   bool                  Clear1, Clear2;

unsigned long DoLibMethodA(unsigned long *parm)
{
   LONG           rc  = 0;
   DriveClient   *drv = 0;

   if (!parm) return 0;
   drv = (DriveClient*)parm[1];

   switch (*parm) {
      case DRV_ScanDevice:       return Drive::ScanDevice((char*)parm[1]);
      case DRV_FreeScanResults:  return Drive::FreeScanResults((ScanData*)parm[1]);
      case DRV_NewDrive:         return (ULONG)DriveSpool::GetInstance()->NewClient((char*)parm[1], parm[2]);
      case DRV_CloneDrive:       return (ULONG)DriveSpool::GetInstance()->CloneClient(drv);
      case DRV_EndDrive:         DriveSpool::GetInstance()->DelClient(drv); return 0;
      case DRV_GetAttr:          return drv->GetDriveAttrs(parm[2]);
      case DRV_GetAttrs:         return drv->GetDriveAttrs((struct TagItem*)&parm[2]);
      case DRV_WaitForDisc:      return drv->WaitForDisc(parm[2]);
      default:                   return drv->Send(parm);
   }
   return rc;
}
      
class IOptItem* OptCreateDisc()
{
   return new OptDisc();
}
  
bool SetUp(void)
{
   LibrarySpool::Init();

   Cfg = new Config();
   Clear2 = analyze(Clear1);
   return true;
}

void CleanUp(void)
{
   DriveSpool::ExitInstance();

   while (!DbgMaster::CleanUp()) {
      DOS->Delay(25);
   }

   delete Cfg;
   LibrarySpool::Exit();
   return;
};

void showDetails(const IOptItem *di)
{
   DOS->VPrintf("%s %ld\n", ARRAY(
            di->getItemType() == Item_Disc ? (int)"Disc" :
            di->getItemType() == Item_Session ? (int)"Session" :
            di->getItemType() == Item_Track ? (int)"Track" :
            di->getItemType() == Item_Index ? (int)"Index" : (int)"Unknown",
            di->getItemNumber()
            ));

   DOS->VPrintf("\tLocation   : %ld - %ld (%ld blocks)\n", ARRAY(di->getStartAddress(), di->getEndAddress(), di->getBlockCount()));
   DOS->VPrintf("\tType       : %s\n", ARRAY(di->getDataType() == Data_Unknown   ? (int)"Unknown" :
            di->getDataType() == Data_Audio     ? (int)"Audio" :
            di->getDataType() == Data_Mode1     ? (int)"Data, Mode 1" :
            di->getDataType() == Data_Mode2     ? (int)"Data, Mode 2" :
            di->getDataType() == Data_Mode2Form1? (int)"Data, Mode 2, Form 1" :
            di->getDataType() == Data_Mode2Form2? (int)"Data, Mode 2, Form 2" :
            (int)"Illegal track type."));
   DOS->VPrintf("\tSec Size   : %ld bytes\n", ARRAY(di->getSectorSize()));
   DOS->VPrintf("\tBlank      : %s\n", ARRAY(di->isBlank()       ? (int)"Yes"         : (int)"No"));
   DOS->VPrintf("\tIncomplete : %s\n", ARRAY(di->isComplete()    ? (int)"No"          : (int)"Yes"));
   DOS->VPrintf("\tCDText     : %s\n", ARRAY(di->hasCDText()     ? (int)"Available"   : (int)"Unavailable"));
   if (di->hasCDText()) 
   {
      DOS->VPrintf("\t- Artist   : %s\n", ARRAY((int)di->getCDTArtist()));
      DOS->VPrintf("\t- Title    : %s\n", ARRAY((int)di->getCDTTitle()));
      DOS->VPrintf("\t- Message  : %s\n", ARRAY((int)di->getCDTMessage()));
      DOS->VPrintf("\t- Lyrics   : %s\n", ARRAY((int)di->getCDTLyrics()));
      DOS->VPrintf("\t- Composer : %s\n", ARRAY((int)di->getCDTComposer()));
      DOS->VPrintf("\t- Director : %s\n", ARRAY((int)di->getCDTDirector()));
   }
}

int main()
{ 
   struct args {
      char    *drive;
      int     *unit;
      int      show_contents;
      int      quick_erase;
      int      complete_erase;
      int      quick_format;
      int      complete_format;
      int      prepare_disc;
      int      check_writable;
      int      check_erasable;
      int      check_formatable;
      int      start;
      int      stop;
      int      load;
      int      eject;
      int      idle;
      int      standby;
      int      sleep;
      int      check_overwritable;
      int      write_image;
      int     *read_track;
      char    *read_to;
      int      layout_track;
      int     *writekbps;
      int     *readkbps;
      int      testmode;
      int      closetrack;
      int      closesession;
      int      closedisc;
      int      getwritabletrks;
      int      writedao;
      char    *data;
      char   **audio;
      int      wait_forever;
      int      close_tracks;

      const char* GetTemplate(void)
      {
         return "DRIVE/A,UNIT/A/K/N,SHOWCONTENTS=SC/S,QUICKERASE/S,COMPLETEERASE/S,"
            "QUICKFORMAT/S,COMPLETEFORMAT/S,PREPAREDISC/S,CHECKWRITABLE/S,CHECKERASABLE/S,"
            "CHECKFORMATABLE/S,START/S,STOP/S,LOAD/S,EJECT/S,IDLE/S,STANDBY/S,SLEEP/S,CHECKOVERWRITABLE/S,"
            "WRITEDISC/S,READTRACK/K/N,TO/K,LAYOUTDISC/S,WRITEKBPS/N,READKBPS/N,TESTMODE/S,"
            "CLOSETRACK/K/N,CLOSESESSION/S,CLOSEDISC/S,GETWRITABLETRACKS/S,DAOMODE/S,DATATRACK/K,AUDIOTRACKS/K/M,WAITFOREVER/S,CLOSETRACKS/S";
      };

      void Init()
      {
         drive                = 0;
         unit                 = 0;
         show_contents        = 0;
         quick_erase          = 0;
         complete_erase       = 0;
         prepare_disc         = 0;
         check_writable       = 0;
         check_erasable       = 0;
         check_formatable     = 0;
         quick_format         = 0;
         complete_format      = 0;
         start                = 0;
         stop                 = 0;
         load                 = 0;
         eject                = 0;
         idle                 = 0;
         standby              = 0;
         sleep                = 0;
         check_overwritable   = 0;
         write_image          = 0;
         read_track           = 0;
         read_to              = 0;
         layout_track         = 0;
         readkbps             = 0;
         writekbps            = 0;
         testmode             = 0;
         closetrack           = 0;
         closesession         = 0;
         getwritabletrks      = 0;
         writedao             = 0;
         data                 = 0;
         audio                = 0;
         wait_forever         = 0;
         closedisc            = 0;
      };
   };

   ULONG    dcl;
   args     arg;
   RDArgs  *rda;
   int      rc;

   arg.Init();

   if (false == SetUp())
   {
      return 20;
   }


   rda = DOS->ReadArgs(const_cast<char*>(arg.GetTemplate()), (int32*)&arg, 0);

   if (rda != NULL) 
   {
      dcl = DoLibMethodA(ARRAY(DRV_NewDrive, (int)arg.drive, *arg.unit));
   } 
   else 
   {
      DOS->VPrintf("Bad arguments.\n", 0);
      CleanUp();
      return 20;
   }
   
   if (!dcl)
   {
      DOS->VPrintf("Unable to open drive. Exiting.\n", 0);
      DOS->FreeArgs(rda); 
      CleanUp();
      return 20;
   }

   DOS->VPrintf("Please insert disc in drive (unless already inserted)\n", 0);

   
   if ((long)DoLibMethodA(ARRAY(DRV_WaitForDisc, dcl, arg.wait_forever ? 0xffffffff : 30)) == ODE_NoDisc)
   {
      DOS->VPrintf("No disc inserted.\n", 0);
      DoLibMethodA(ARRAY(DRV_EndDrive, dcl));
      DOS->FreeArgs(rda);
      CleanUp();
      return 20;
   }

   int spd_read=0,
       spd_write=0;

   if (arg.readkbps)  spd_read  = *arg.readkbps;
   if (arg.writekbps) spd_write = *arg.writekbps;

   DOS->VPrintf("Selected read speed:  %ld\n", ARRAY(spd_read));
   DOS->VPrintf("Selected write speed: %ld\n", ARRAY(spd_write));

   DoLibMethodA(ARRAY(DRV_SetAttr, dcl, DRA_Drive_TestMode, arg.testmode));

   if (arg.quick_erase) 
   {
      DOS->VPrintf("Blanking... \n", 0);
      rc = DoLibMethodA(ARRAY(DRV_Blank, dcl, DRT_Blank_Fast));
      DOS->VPrintf("Blanking result: %ld\n", ARRAY(rc));
   }
   else if (arg.complete_erase) 
   {
      DOS->VPrintf("Blanking... \n", 0);
      rc = DoLibMethodA(ARRAY(DRV_Blank, dcl, DRT_Blank_Complete));
      DOS->VPrintf("Blanking result: %ld\n", ARRAY(rc));
   }
   else if (arg.quick_format) 
   {
      DOS->VPrintf("Formatting disc... \n", 0);
      rc = DoLibMethodA(ARRAY(DRV_Format, dcl, DRT_Format_Fast));
      DOS->VPrintf("Result: %ld\n", ARRAY(rc));
   }
   else if (arg.complete_format) 
   {
      DOS->VPrintf("Formatting disc... \n", 0);
      rc = DoLibMethodA(ARRAY(DRV_Format, dcl, DRT_Format_Complete));
      DOS->VPrintf("Result: %ld\n", ARRAY(rc));
   }
   else if (arg.prepare_disc) 
   {
      DOS->VPrintf("Preparing disc... \n", 0);
      rc = DoLibMethodA(ARRAY(DRV_StructureDisc, dcl));
      DOS->VPrintf("Result: %ld\n", ARRAY(rc));
   }
   else if (arg.show_contents) 
   {
      int         num;
      IOptItem   *di;

      DOS->VPrintf("Please insert a disc (unless it is already inserted).\n", 0);

      DoLibMethodA(ARRAY( DRV_GetAttrs,           dcl,
                        DRA_Disc_NumTracks,     (int)&num,
                        DRA_Disc_Contents,      (int)&di,
                        TAG_DONE,               0));

      DOS->VPrintf("Number of tracks: %ld\n", ARRAY(num));

      showDetails(di);
      for (int i=0; i<di->getChildCount(); i++)
      {
         const IOptItem *sess = di->getChild(i);
         showDetails(sess);
         
         for (int j=0; j<sess->getChildCount(); j++)
         {
            const IOptItem *trak = sess->getChild(j);
            showDetails(trak);

            for (int k=0; k<trak->getChildCount(); k++)
            {
               const IOptItem *indx = trak->getChild(k);
               showDetails(indx);
            }
         }
      }
   }
   else if (arg.check_writable) 
   {
      int         num;
      num = DoLibMethodA(ARRAY(DRV_GetAttr, dcl, DRA_Disc_IsWritable));
      DOS->VPrintf("Disc is %swritable.\n", ARRAY(num? (int)"" : (int)"not "));
   }
   else if (arg.check_erasable) 
   {
      int         num;
      num = DoLibMethodA(ARRAY(DRV_GetAttr, dcl, DRA_Disc_IsErasable));
      DOS->VPrintf("Disc is %serasable.\n", ARRAY(num? (int)"" : (int)"not "));
   }
   else if (arg.check_formatable) 
   {
      int         num;
      num = DoLibMethodA(ARRAY(DRV_GetAttr, dcl, DRA_Disc_IsFormatable));
      DOS->VPrintf("Disc is %sformatable.\n", ARRAY(num? (int)"" : (int)"not "));
   }
   else if (arg.check_overwritable) 
   {
      int         num;
      num = DoLibMethodA(ARRAY(DRV_GetAttr, dcl, DRA_Disc_IsOverwritable));
      DOS->VPrintf("Disc is %soverwritable.\n", ARRAY(num? (int)"" : (int)"not "));
   }
   else if ((arg.layout_track) || (arg.write_image))
   {
      int         num;

      num = DoLibMethodA(ARRAY(DRV_GetAttr, dcl, DRA_Disc_IsWritable));

      if (!num) 
      {
         DOS->VPrintf("Disc is not writable!!!!\n", 0);
      } 
      else 
      {
         IOptItem      *disc, *sess, *trak;
         IOptItem     **tbl;
         FileInfoBlock  fib;
         BPTR           fh;

         DOS->VPrintf("Got writable disc!\n", 0);

         int d=0, a=0, t=0;
         
         if (arg.data)
            d = 1;
         if (arg.audio)
            while (arg.audio[a++]);
         if (a) a--;
         t = d + a;                    // total number of tracks;   
      
         DOS->VPrintf("Got %ld data and %ld audio tracks (total: %ld tracks)\n", ARRAY(d, a, t));
     
         /*
          * create disc and session items.
          */
         disc = OptCreateDisc();
         sess = disc->addChild();
         tbl  = new IOptItem*[t];

         /*
          * create data track
          */
         if (0 != d)
         {
            DOS->VPrintf("Accessing file %s\n", ARRAY((ULONG)arg.data));
            fh = DOS->Open(arg.data, MODE_OLDFILE);
            if (0 != fh)
            {
               if (DOS->ExamineFH(fh, &fib))
               {
                  trak = sess->addChild();
                  trak->setDataType(Data_Mode1);
                  trak->setDataBlockCount(fib.fib_Size >> 11);
                  tbl[0] = trak;
               }
               DOS->Close(fh);
            }
         }

         /*
          * create tracks.
          */
         for (int i=0; i<a; i++)
         {
            DOS->VPrintf("Accessing file %s\n", ARRAY((ULONG)arg.audio[i]));
            fh = DOS->Open(arg.audio[i], MODE_OLDFILE);
            if (0 != fh)
            {
               if (0 != DOS->ExamineFH(fh, &fib))
               {
                  trak = sess->addChild();
                  trak->setDataType(Data_Audio);
                  trak->setDataBlockCount(fib.fib_Size / 2352);
                  tbl[i+d] = trak;
               }
               DOS->Close(fh);
            }
         }
  
         /*
          * set disc flags
          */
         disc->setFlags((arg.writedao     ? DIF_Disc_MasterizeCD  : 0) |
                        (arg.closesession ? DIF_Disc_CloseSession : 
                         arg.closedisc    ? DIF_Disc_CloseDisc    : 0));
            
         DOS->VPrintf("Lying tracks...\n", 0);
   
         num = DoLibMethodA(ARRAY(DRV_LayoutTracks, dcl, (int)disc));
   
         DOS->VPrintf("Layout returned: %ld\n", ARRAY(num));
   
         if (arg.write_image)
         {
            DOS->VPrintf("Preparing for upload...\n", 0);
            num = DoLibMethodA(ARRAY(DRV_UploadLayout, dcl, (int)disc));
            if (!num) 
            {
               for (int i=0; i<t; i++)
               {
                  int   sec = 0;
                  int   count = 0;
                  int   size = 0;
                  int   rem = 0;
                  uint8*buf = 0;
                  
                  trak  = tbl[i];
                  count = trak->getPacketSize();
                  rem   = trak->getDataBlockCount();
                  size  = count * trak->getSectorSize();
                  buf   = new uint8[size];
                     
                  if ((d) && (!i))
                  {
                     DOS->VPrintf("Opening file: %s\n", ARRAY((ULONG)arg.data));
                     fh = DOS->Open(arg.data, MODE_OLDFILE);\
                  }
                  else                     
                  {
                     DOS->VPrintf("Opening file: %s\n", ARRAY((ULONG)arg.audio[i-d]));
                     fh = DOS->Open(arg.audio[i-d], MODE_OLDFILE);
                  }
                     
                  while (rem) 
                  {
                     DOS->VPrintf("Writing sector %ld, remaining: %ld\n", ARRAY(sec, rem));
                     size  = rem > count ? count : rem;
                     DOS->Read(fh, buf, size * trak->getSectorSize());

                     num = DoLibMethodA(ARRAY(DRV_WriteSequential, dcl, (int)buf, size));
                     sec += size;
                     rem -= size;
                     if (num) break;
                  }
                  if (num) 
                  {
                     DOS->VPrintf("Error while uploading track data: %ld\n", ARRAY(num));
                  }
                  DOS->Close(fh); 
                  delete [] buf;
               }
            } 
            else 
            {
               DOS->VPrintf("Layout upload failed %ld. Aborting.\n", ARRAY(num));
            }
         }
   
         /*
          * and this is meant to dispose all items: disc, session and tracks.
          */
         disc->dispose();
         delete [] tbl;
      }
   }
   else if (arg.read_track) 
   {
      int               num;
      IOptItem         *di;
      const IOptItem   *trak = 0;

      if ((arg.read_to) && (*arg.read_track>0)) 
      {
         BPTR  fh;

         fh = DOS->Open(arg.read_to, MODE_NEWFILE);

         if (fh) 
         {
            DOS->VPrintf("Please insert a disc (unless it is already inserted).\n", 0);
            DoLibMethodA(ARRAY( DRV_GetAttrs,           dcl,
                              DRA_Disc_NumTracks,     (int)&num,
                              DRA_Disc_Contents,      (int)&di,
                              TAG_DONE,               0));
            if (num >= *arg.read_track) 
            {
               for (int i=0; i<di->getChildCount(); i++)
               {
                  for (int j=0; j<di->getChild(i)->getChildCount(); j++)
                  {
                     if (di->getChild(i)->getChild(j)->getItemNumber() == *arg.read_track)
                     {
                        trak = di->getChild(i)->getChild(j);
                        break;
                     }
                  }
                  if (0 != trak)
                     break;
               }

               if (0 != trak) 
               {
                  trak->claim();
                  uint8 *buff = new uint8[32*trak->getSectorSize()];

                  if (0 != buff) 
                  {
                     int csec, rsec;

                     csec = 0;
                     rsec = trak->getDataBlockCount();

                     while (rsec) 
                     {
                        int len = (rsec > 32) ? 32 : rsec;

                        DoLibMethodA(ARRAY( DRV_ReadTrackRelative, dcl, (int)trak, csec, len, (int)buff));
                        DOS->Write(fh, buff, len*trak->getSectorSize());

                        rsec -= len;
                        csec += len;
                     }
                     delete [] buff;
                  }  

                  trak->dispose();
               }
            }
            DOS->Close(fh);
         }
      }
   }
   else if (arg.getwritabletrks) 
   {
      IOptItem *di = 0;

      DOS->VPrintf("Please insert a disc (unless it is already inserted).\n", 0);

      for (DoLibMethodA(ARRAY(DRV_GetAttrs, dcl, DRA_Disc_NextWritableTrack, (int)&di, TAG_DONE, 0)); di;
         DoLibMethodA(ARRAY(DRV_GetAttrs, dcl, DRA_Disc_NextWritableTrack, (int)&di, TAG_DONE, 0)))
      {
         DOS->VPrintf("Track %ld\n", ARRAY(di->getItemNumber()));
         if (di->isIncremental())
         {
            DOS->VPrintf("\tLocation   : %ld - %ld (%ld blocks total, random writable)\n", ARRAY(di->getStartAddress(), di->getEndAddress(), di->getBlockCount()));
         }
         else
         {
            DOS->VPrintf("\tLocation   : %ld - %ld (%ld blocks written, %ld blocks total)\n", ARRAY(di->getStartAddress(), di->getEndAddress(), 0, di->getBlockCount()));
         }
         DOS->VPrintf("\tType       : %s\n", ARRAY(  di->getDataType() == Data_Unknown     ? (int)"Unknown" :
                                                di->getDataType() == Data_Audio       ? (int)"Audio" :
                                                di->getDataType() == Data_Mode1       ? (int)"Data, Mode 1" :
                                                di->getDataType() == Data_Mode2       ? (int)"Data, Mode 2" :
                                                di->getDataType() == Data_Mode2Form1  ? (int)"Data, Mode 2, Form 1" :
                                                di->getDataType() == Data_Mode2Form2  ? (int)"Data, Mode 2, Form 2" :
                                                                                        (int)"Illegal track type."));
         DOS->VPrintf("\tSec Size   : %ld bytes\n", ARRAY(di->getSectorSize()));
         DOS->VPrintf("\tBlank      : %s\n", ARRAY(di->isBlank() == 1 ? (int)"Yes" : (int)"No"));
      }
   }
   else if (arg.start) 
   {
      DoLibMethodA(ARRAY(DRV_ControlUnit, dcl, DRT_Unit_Start));
   }
   else if (arg.stop) 
   {
      DoLibMethodA(ARRAY(DRV_ControlUnit, dcl, DRT_Unit_Stop));
   }
   else if (arg.load) 
   {
      DoLibMethodA(ARRAY(DRV_ControlUnit, dcl, DRT_Unit_Load));
   }
   else if (arg.eject) 
   {
      DoLibMethodA(ARRAY(DRV_ControlUnit, dcl, DRT_Unit_Eject));
   }
   else if (arg.idle) 
   {
      DoLibMethodA(ARRAY(DRV_ControlUnit, dcl, DRT_Unit_Idle));
   }
   else if (arg.standby) 
   {
      DoLibMethodA(ARRAY(DRV_ControlUnit, dcl, DRT_Unit_Standby));
   }
   else if (arg.closetrack) 
   {
      DOS->VPrintf("Close track... \n", 0);
      rc = DoLibMethodA(ARRAY(DRV_CloseDisc, dcl, DRT_Close_Track, *((int*)arg.closetrack)));
      DOS->VPrintf("Close track result: %ld\n", ARRAY(rc));
   }
   else if (arg.closesession) 
   {
      DOS->VPrintf("Close session... \n", 0);
      rc = DoLibMethodA(ARRAY(DRV_CloseDisc, dcl, DRT_Close_Session));
      DOS->VPrintf("Close session result: %ld\n", ARRAY(rc));
   }
   else if (arg.closedisc) 
   {
      DOS->VPrintf("Close disc... \n", 0);
      rc = DoLibMethodA(ARRAY(DRV_CloseDisc, dcl, DRT_Close_Finalize));
      DOS->VPrintf("Close disc result: %ld\n", ARRAY(rc));
   }
   else if (arg.sleep) 
   {
      DoLibMethodA(ARRAY(DRV_ControlUnit, dcl, DRT_Unit_Sleep));
   }
   else if (arg.close_tracks)
   {
      int         num;
      IOptItem   *di;

      DOS->VPrintf("Please insert a disc (unless it is already inserted).\n", 0);

      DoLibMethodA(ARRAY( DRV_GetAttrs,           dcl,
                        DRA_Disc_NumTracks,     (int)&num,
                        DRA_Disc_Contents,      (int)&di,
                        TAG_DONE,               0));

      DOS->VPrintf("Number of tracks: %ld\n", ARRAY(num));

      showDetails(di);
      for (int i=0; i<di->getChildCount(); i++)
      {
         const IOptItem *sess = di->getChild(i);
         showDetails(sess);
         
         for (int j=0; j<sess->getChildCount(); j++)
         {
            const IOptItem *trak = sess->getChild(j);
            showDetails(trak);
            if (!trak->isComplete())
               DoLibMethodA(ARRAY(DRV_CloseDisc, dcl, DRT_Close_Track, trak->getItemNumber()));
         }
      }
   }

   do 
   {
      DOS->VPrintf("%08lx: Current drive status: %ld\n", ARRAY((ULONG)dcl, DoLibMethodA(ARRAY(DRV_GetAttr, dcl, DRA_Drive_Status))));
      DOS->Delay(50);
   } while (!(Exec->SetSignal(0, 0) & SIGBREAKF_CTRL_C));

   if (rda) DOS->FreeArgs(rda);

   DoLibMethodA(ARRAY(DRV_EndDrive, dcl));
   CleanUp();
}
