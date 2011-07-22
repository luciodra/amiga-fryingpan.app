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

#include "MUIList.h"
#include <Generic/LibrarySpool.h>
#include <libclass/intuition.h>
#include <mui/NList_mcc.h>
#include <mui/NListview_mcc.h>

MUIList::MUIList(const char *format, bool bms)
{
   listview       = 0;
   list           = 0;
   dragSortable   = false;
   multiSelect    = bms;
   this->format   = format;

   hHkOnSelect.Initialize(this, &MUIList::onSelect);
   hHkOnWBDrop.Initialize(this, &MUIList::onWBDrop);
   hHkOnDragSort.Initialize(this, &MUIList::onDragSort);
}

MUIList::~MUIList()
{
}

unsigned long *MUIList::getObject()
{
   if (0 != listview)
      return listview;

   list = NListObject,
      InputListFrame,
      MUIA_NList_Title,          (int)"Title",
      MUIA_NList_Format,         (int)format.Data(),
      MUIA_NList_MultiSelect,    multiSelect ? MUIV_NList_MultiSelect_Default : MUIV_NList_MultiSelect_None,
      MUIA_NList_DragSortable,   dragSortable,
   End;

   if (list != 0)
   {
      listview = NListviewObject,
         MUIA_NListview_NList,      (int)list,
      End;
      nlist = true;

      DoMtd(list, ARRAY(MUIM_Notify, MUIA_NList_Active,           (uint32)MUIV_EveryTime, (uint32)list, 2, MUIM_CallHook, (uint32)hHkOnSelect.GetHook()));
      DoMtd(list, ARRAY(MUIM_Notify, MUIA_NList_DragSortInsert,   (uint32)MUIV_EveryTime, (uint32)list, 2, MUIM_CallHook, (uint32)hHkOnDragSort.GetHook()));
   }
   else
   {
      listview = ListviewObject,
         MUIA_Listview_List,        list = ListObject,
            InputListFrame,
            MUIA_List_Format,          (int)format.Data(),
            MUIA_List_Title,           "Title",
         End,
         MUIA_Listview_MultiSelect, multiSelect ? MUIV_Listview_MultiSelect_Default : MUIV_Listview_MultiSelect_None,
      End;
      nlist = false;
      DoMtd(list, ARRAY(MUIM_Notify, MUIA_List_Active, MUIV_EveryTime, (uint32)list, 2, MUIM_CallHook, (uint32)hHkOnSelect.GetHook()));
   }

   DoMtd(list, ARRAY(MUIM_Notify, MUIA_AppMessage,   (uint32)MUIV_EveryTime, (uint32)list, 3, MUIM_CallHook, (uint32)hHkOnWBDrop.GetHook(), MUIV_TriggerValue));
   return listview;
}

void MUIList::setConstructHook(const Hook* hook)
{
   if (0 == listview)
      getObject();

   Intuition->SetAttrsA(list, (TagItem*)ARRAY(
      nlist ? MUIA_NList_ConstructHook : MUIA_List_ConstructHook,
         (int)hook,
      TAG_DONE,
         0
   ));
}

void MUIList::setDestructHook(const Hook* hook)
{
   if (0 == listview)
      getObject();

   Intuition->SetAttrsA(list, (TagItem*)ARRAY(
      nlist ? MUIA_NList_DestructHook : MUIA_List_DestructHook,
         (int)hook,
      TAG_DONE,
         0
   ));
}

void MUIList::setDisplayHook(const Hook* hook)
{
   if (0 == listview)
      getObject();

   Intuition->SetAttrsA(list, (TagItem*)ARRAY(
      nlist ? MUIA_NList_DisplayHook : MUIA_List_DisplayHook,
         (int)hook,
      TAG_DONE,
         0
   ));
}

void MUIList::setSelectionHook(const Hook* hook)
{
   hOnSelect = hook;
}

uint32 MUIList::onSelect(void*, void*)
{
   void* data;

   if (nlist)
   {
      DoMtd(list, ARRAY(MUIM_NList_GetEntry, (uint32)MUIV_NList_GetEntry_Active, (uint32)&data));
   }
   else
   {
      DoMtd(list, ARRAY(MUIM_List_GetEntry, (uint32)MUIV_List_GetEntry_Active, (uint32)&data));
   }

   return hOnSelect.Call(data, data);
}

uint32 MUIList::onWBDrop(void*, AppMessage**m)
{
   return hOnWBDrop(*m, *m);
}

uint32 MUIList::onDragSort(void*, void*)
{
   VectorT<void*> vec;
   void* data;

#warning onDragSort method still does not work - lack of support on NList side
   request("Info", "Drag Sort", "Ok", 0);

   for (int32 i=0; ; i++)
   {
      DoMtd(list, ARRAY(MUIM_NList_GetEntry, i, (int32)&data));
      if (data == 0)
         break;
      vec << data;
   }  
   return hOnSort.Call(&vec, &vec); 
}

void MUIList::addItem(void* item)
{
   if (NULL != list)
   {
      if (nlist)
      {
         DoMtd(list, ARRAY(MUIM_NList_InsertSingle, (int)item, (unsigned)MUIV_NList_Insert_Bottom));
      }
      else
      {
         DoMtd(list, ARRAY(MUIM_List_InsertSingle, (int)item, (unsigned)MUIV_List_Insert_Bottom));
      }
   }
}

void MUIList::clear()
{
   if (NULL != list)
   {
      if (nlist)
      {
         DoMtd(list, ARRAY(MUIM_NList_Clear));
      }
      else
      {
         DoMtd(list, ARRAY(MUIM_List_Clear));
      }
   }
}

VectorT<void*> &MUIList::getSelectedItems()
{
   selected.Empty();

   if (nlist)
   {
      long nextSel = MUIV_NList_NextSelected_Start;
      void* data;

      while (true)
      {
         DoMtd(list, ARRAY(MUIM_NList_NextSelected, (int)&nextSel));
         if (nextSel == MUIV_NList_NextSelected_End)
            break;
         DoMtd(list, ARRAY(MUIM_NList_GetEntry, nextSel, (int)&data));
         selected << data;
      }
   }
   else
   {
      void* data;
      DoMtd(list, ARRAY(MUIM_List_GetEntry, (unsigned)MUIV_List_GetEntry_Active, (int)&data));
      selected << data;
   }
   
   return selected;
}

void MUIList::setEnabled(bool enabled)
{
   Intuition->SetAttrsA(listview, (TagItem*)ARRAY(MUIA_Disabled, !enabled, TAG_DONE, 0));
}

void MUIList::setWBDropHook(const Hook* hook)
{
   hOnWBDrop = hook;
}

void MUIList::setDragSortHook(const Hook* hook)
{
   hOnSort = hook;
}

void MUIList::setDragSortable(bool sortable)
{
   dragSortable = sortable;
   if ((NULL != list) && (nlist))
   {
      Intuition->SetAttrsA(list, (TagItem*)ARRAY(
         MUIA_NList_DragSortable,   sortable,
         TAG_DONE,                  0));
   }
}

