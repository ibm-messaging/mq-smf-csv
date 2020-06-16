/*
 * Copyright (c) 2016 IBM Corporation and other Contributors.
 *
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors:
 *   Mark Taylor - Initial Contribution
 */

#include <stdio.h>
#include "mqsmf.h"

SMFPRINTGLOB;

void printQSRS(qsrs *p)
{
  SMFPRINTSTART("QSRS", p, conv16(p->qsrsll));

  ADDS32("PHB_Global_Fixed"   , p->qsrsphbgf);
  ADDS32("PHB_Global_Variable", p->qsrsphbgv);
  ADDS32("PHB_Local_Fixed"    , p->qsrsphblf);
  ADDS32("PHB_Local_Variable" , p->qsrsphblv);
  ADDS32("GMB_Total"          , p->qsrsgmbt);
  ADDS32("Stack_Total"        , p->qsrsstkt);

  ADDS32("Region_24_Start"         , p->qsrsstrta);
  ADDS32("Region_24_Size"          , p->qsrssiza);
  ADDS32("Region_24_Top"           , p->qsrscrgtp);
  ADDS32("Region_24_Limit"         , p->qsrslimit);
  ADDS32("User_24_Alloc"           , p->qsrsloal);
  ADDS32("User_24_Alloc_Previous"  , p->qsrsoloal);
  ADDS32("System_24_Alloc"         , p->qsrshial);
  ADDS32("System_24_Alloc_Previous", p->qsrsohial);
  ADDS32("SMF_24_Limit"            , p->qsrssmfl);
  ADDS32("SMF_24_Average"          , p->qsrssmfr);

  ADDS32("Region_31_Start"         , p->qsrsestra);
  ADDS32("Region_31_Size"          , p->qsrsesiza);
  ADDS32("Region_31_Top"           , p->qsrsergtp);
  ADDS32("Region_31_Limit"         , p->qsrselim);
  ADDS32("User_31_Alloc"           , p->qsrseloal);
  ADDS32("User_31_Alloc_Previous"  , p->qsrsoeloal);
  ADDS32("System_31_Alloc"         , p->qsrsehial);
  ADDS32("System_31_Alloc_Previous", p->qsrsoehial);
  ADDS32("SMF_31_Limit"            , p->qsrssmfel);
  ADDS32("SMF_31_Average"          , p->qsrssmfer);


  ADDU64("MemLimit"         , p->qsrsmemlim);
  ADDU64("Total_Object_Size", p->qsrsabytes);
  ADDU64("High_Water_Usable", p->qsrsgbytes);
  ADDS32("Object_Allocated" , p->qsrsnmomb);
  ADDBYTE("MemLimit_Source" , p->qsrsmemlims);

  ADDS32("RAX_Frames",   p->qsrsfmct);


  ADDS32("Avail_31"        , p->qsrsaval);
  ADDS32("MVS_31_Reserved" , p->qsrsmvs);
  ADDS32("CRIT_31_Reserved", p->qsrscrit);
  ADDS32("SOS_31_Reserved" , p->qsrssos);



  ADDU64("Avail_64"  , p->qsrsaval64);
  ADDU64("Cushion_64", p->qsrscush64);
  ADDU64("SMDS_64"   , p->qsrssmds64);


  ADDU64("PHB_Latch_Global_Fixed"  , p->qsrsphblgf);
  ADDU64("PHB_Latch_Global_Variable", p->qsrsphblgv);
  ADDU64("PHB_Latch_Local_Fixed"   , p->qsrsphbllf);
  ADDU64("PHB_Latch_Local_Variable", p->qsrsphbllv);
  ADDU64("GMB_Latch"               , p->qsrsgmbl);


  SMFPRINTSTOP;

  return;
}

