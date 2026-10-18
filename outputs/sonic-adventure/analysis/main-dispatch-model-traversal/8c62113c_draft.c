/* Ghidra draft only; not buildable/verified source. */

void FUN_8c62113c(undefined8 param_1,undefined8 param_2,undefined4 param_3,undefined4 param_4,
                 undefined4 param_5,undefined4 param_6,uint *param_7)

{
  undefined *puVar1;
  undefined4 *puVar2;
  undefined4 *puVar3;
  uint uVar4;
  undefined8 *puVar5;
  undefined4 in_fr1;
  undefined4 in_fr0;
  undefined8 uVar6;
  undefined4 in_fr3;
  undefined4 extraout_fr3;
  undefined4 extraout_fr3_00;
  undefined4 extraout_fr3_01;
  undefined4 extraout_fr3_02;
  undefined4 extraout_fr3_03;
  undefined4 extraout_fr3_04;
  undefined4 uVar7;
  undefined4 extraout_fr3_05;
  undefined4 extraout_fr3_06;
  undefined4 extraout_fr3_07;
  undefined4 extraout_fr3_08;
  undefined4 extraout_fr3_09;
  undefined4 extraout_fr3_10;
  undefined4 in_fr2;
  undefined4 extraout_fr2;
  undefined4 extraout_fr2_00;
  undefined4 extraout_fr2_01;
  undefined4 extraout_fr2_02;
  undefined4 extraout_fr2_03;
  undefined4 extraout_fr2_04;
  undefined4 uVar8;
  undefined4 extraout_fr2_05;
  undefined4 extraout_fr2_06;
  undefined4 extraout_fr2_07;
  undefined4 extraout_fr2_08;
  undefined4 extraout_fr2_09;
  undefined4 extraout_fr2_10;
  undefined4 in_fr13;
  undefined4 in_fr12;
  undefined4 in_fr15;
  undefined4 in_fr14;
  undefined8 in_xd0;
  undefined8 in_xd2;
  undefined8 in_xd4;
  undefined8 in_xd6;
  undefined8 in_xd8;
  undefined8 in_xd10;
  undefined8 in_xd12;
  undefined8 in_xd14;
  char in_FPSCR_SZ;
  undefined4 uVar9;
  
  uVar6 = CONCAT44(in_fr0,in_fr1);
  uVar9 = 0x10;
  do {
    puVar3 = DAT_8c62124c;
    puVar2 = DAT_8c621248;
    puVar1 = PTR_FUN_8c621240;
    uVar4 = *param_7 & 7;
    if (uVar4 == 0) {
      (*(code *)PTR_FUN_8c621244)(0,uVar9);
      if (in_FPSCR_SZ == '\0') {
        uVar7 = (undefined4)param_2;
      }
      else {
        uVar7 = (undefined4)*(undefined8 *)(param_7 + 4);
        in_xd4 = *(undefined8 *)(param_7 + 3);
      }
      (*(code *)PTR_FUN_8c621250)(0);
      if ((*param_7 & 0x20) == 0) {
        (*(code *)PTR_FUN_8c621258)(0,param_7[5],param_7[6],param_7[7]);
      }
      else {
        (*(code *)PTR_FUN_8c621254)(0,param_7[5],param_7[6],param_7[7]);
      }
      if (in_FPSCR_SZ == '\0') {
        param_2 = CONCAT44(param_7[10],uVar7);
        param_1 = CONCAT44(param_7[8],param_7[9]);
      }
      else {
        param_2 = *(undefined8 *)(param_7 + 10);
        param_1 = *(undefined8 *)(param_7 + 8);
        in_xd4 = *(undefined8 *)(param_7 + 9);
      }
      uVar6 = (*(code *)puVar1)(0);
      puVar5 = (undefined8 *)*puVar2;
      *puVar3 = puVar5;
      if (in_FPSCR_SZ == '\0') {
        *(undefined4 *)((int)puVar5 + 0x3c) = in_fr15;
        *(undefined4 *)(puVar5 + 7) = in_fr14;
        *(undefined4 *)((int)puVar5 + 0x34) = in_fr13;
        *(undefined4 *)(puVar5 + 6) = in_fr12;
        *(undefined4 *)((int)puVar5 + 0x2c) = param_6;
        *(undefined4 *)(puVar5 + 5) = param_5;
        *(undefined4 *)((int)puVar5 + 0x24) = param_4;
        *(undefined4 *)(puVar5 + 4) = param_3;
        *(int *)((int)puVar5 + 0x1c) = (int)param_2;
        *(int *)(puVar5 + 3) = (int)((ulonglong)param_2 >> 0x20);
        *(int *)((int)puVar5 + 0x14) = (int)param_1;
        *(int *)(puVar5 + 2) = (int)((ulonglong)param_1 >> 0x20);
        *(undefined4 *)((int)puVar5 + 0xc) = extraout_fr3;
        *(undefined4 *)(puVar5 + 1) = extraout_fr2;
        *(int *)((int)puVar5 + 4) = (int)uVar6;
        *(int *)puVar5 = (int)((ulonglong)uVar6 >> 0x20);
      }
      else {
        puVar5[7] = in_xd14;
        puVar5[6] = CONCAT44(in_fr14,in_fr15);
        puVar5[5] = in_xd12;
        puVar5[4] = CONCAT44(in_fr12,in_fr13);
        puVar5[3] = in_xd10;
        puVar5[2] = CONCAT44(param_5,param_6);
        puVar5[1] = in_xd8;
        *puVar5 = CONCAT44(param_3,param_4);
        puVar5[-1] = in_xd6;
        puVar5[-2] = param_2;
        puVar5[-3] = in_xd4;
        puVar5[-4] = param_1;
        puVar5[-5] = in_xd2;
        puVar5[-6] = CONCAT44(extraout_fr2,extraout_fr3);
        puVar5[-7] = in_xd0;
        puVar5[-8] = uVar6;
      }
      if ((*param_7 & 8) == 0) {
        FUN_8c6215ba(param_7[1]);
      }
      if ((*param_7 & 0x10) == 0) {
        FUN_8c6215fa(param_7[0xb]);
      }
LAB_8c621510:
      uVar6 = (*(code *)PTR_FUN_8c621618)(1);
      in_fr3 = extraout_fr3_08;
      in_fr2 = extraout_fr2_08;
    }
    else {
      if (uVar4 == 1) {
        (*(code *)PTR_FUN_8c621244)(0,uVar9);
        uVar7 = (undefined4)param_2;
        if ((*param_7 & 0x20) == 0) {
          (*(code *)PTR_FUN_8c6214b0)(0,param_7[5],param_7[6],param_7[7]);
        }
        else {
          (*(code *)PTR_FUN_8c6214ac)(0,param_7[5],param_7[6],param_7[7]);
        }
        if (in_FPSCR_SZ == '\0') {
          param_2 = CONCAT44(param_7[10],uVar7);
          param_1 = CONCAT44(param_7[8],param_7[9]);
        }
        else {
          param_2 = *(undefined8 *)(param_7 + 10);
          param_1 = *(undefined8 *)(param_7 + 8);
          in_xd4 = *(undefined8 *)(param_7 + 9);
        }
        uVar6 = (*(code *)puVar1)(0);
        puVar5 = (undefined8 *)*puVar2;
        *puVar3 = puVar5;
        if (in_FPSCR_SZ == '\0') {
          *(undefined4 *)((int)puVar5 + 0x3c) = in_fr15;
          *(undefined4 *)(puVar5 + 7) = in_fr14;
          *(undefined4 *)((int)puVar5 + 0x34) = in_fr13;
          *(undefined4 *)(puVar5 + 6) = in_fr12;
          *(undefined4 *)((int)puVar5 + 0x2c) = param_6;
          *(undefined4 *)(puVar5 + 5) = param_5;
          *(undefined4 *)((int)puVar5 + 0x24) = param_4;
          *(undefined4 *)(puVar5 + 4) = param_3;
          *(int *)((int)puVar5 + 0x1c) = (int)param_2;
          *(int *)(puVar5 + 3) = (int)((ulonglong)param_2 >> 0x20);
          *(int *)((int)puVar5 + 0x14) = (int)param_1;
          *(int *)(puVar5 + 2) = (int)((ulonglong)param_1 >> 0x20);
          *(undefined4 *)((int)puVar5 + 0xc) = extraout_fr3_00;
          *(undefined4 *)(puVar5 + 1) = extraout_fr2_00;
          *(int *)((int)puVar5 + 4) = (int)uVar6;
          *(int *)puVar5 = (int)((ulonglong)uVar6 >> 0x20);
        }
        else {
          puVar5[7] = in_xd14;
          puVar5[6] = CONCAT44(in_fr14,in_fr15);
          puVar5[5] = in_xd12;
          puVar5[4] = CONCAT44(in_fr12,in_fr13);
          puVar5[3] = in_xd10;
          puVar5[2] = CONCAT44(param_5,param_6);
          puVar5[1] = in_xd8;
          *puVar5 = CONCAT44(param_3,param_4);
          puVar5[-1] = in_xd6;
          puVar5[-2] = param_2;
          puVar5[-3] = in_xd4;
          puVar5[-4] = param_1;
          puVar5[-5] = in_xd2;
          puVar5[-6] = CONCAT44(extraout_fr2_00,extraout_fr3_00);
          puVar5[-7] = in_xd0;
          puVar5[-8] = uVar6;
        }
        if ((*param_7 & 8) == 0) {
          FUN_8c6215ba(param_7[1]);
        }
        if ((*param_7 & 0x10) == 0) {
          FUN_8c6215fa(param_7[0xb]);
        }
        goto LAB_8c621510;
      }
      if (uVar4 == 2) {
        (*(code *)PTR_FUN_8c621244)(0,uVar9);
        if (in_FPSCR_SZ == '\0') {
          uVar7 = (undefined4)param_2;
        }
        else {
          uVar7 = (undefined4)*(undefined8 *)(param_7 + 4);
          in_xd4 = *(undefined8 *)(param_7 + 3);
        }
        (*(code *)PTR_FUN_8c6214b4)(0);
        if (in_FPSCR_SZ == '\0') {
          param_2 = CONCAT44(param_7[10],uVar7);
          param_1 = CONCAT44(param_7[8],param_7[9]);
        }
        else {
          param_2 = *(undefined8 *)(param_7 + 10);
          param_1 = *(undefined8 *)(param_7 + 8);
          in_xd4 = *(undefined8 *)(param_7 + 9);
        }
        uVar6 = (*(code *)puVar1)(0);
        puVar5 = (undefined8 *)*puVar2;
        *puVar3 = puVar5;
        if (in_FPSCR_SZ == '\0') {
          *(undefined4 *)((int)puVar5 + 0x3c) = in_fr15;
          *(undefined4 *)(puVar5 + 7) = in_fr14;
          *(undefined4 *)((int)puVar5 + 0x34) = in_fr13;
          *(undefined4 *)(puVar5 + 6) = in_fr12;
          *(undefined4 *)((int)puVar5 + 0x2c) = param_6;
          *(undefined4 *)(puVar5 + 5) = param_5;
          *(undefined4 *)((int)puVar5 + 0x24) = param_4;
          *(undefined4 *)(puVar5 + 4) = param_3;
          *(int *)((int)puVar5 + 0x1c) = (int)param_2;
          *(int *)(puVar5 + 3) = (int)((ulonglong)param_2 >> 0x20);
          *(int *)((int)puVar5 + 0x14) = (int)param_1;
          *(int *)(puVar5 + 2) = (int)((ulonglong)param_1 >> 0x20);
          *(undefined4 *)((int)puVar5 + 0xc) = extraout_fr3_01;
          *(undefined4 *)(puVar5 + 1) = extraout_fr2_01;
          *(int *)((int)puVar5 + 4) = (int)uVar6;
          *(int *)puVar5 = (int)((ulonglong)uVar6 >> 0x20);
        }
        else {
          puVar5[7] = in_xd14;
          puVar5[6] = CONCAT44(in_fr14,in_fr15);
          puVar5[5] = in_xd12;
          puVar5[4] = CONCAT44(in_fr12,in_fr13);
          puVar5[3] = in_xd10;
          puVar5[2] = CONCAT44(param_5,param_6);
          puVar5[1] = in_xd8;
          *puVar5 = CONCAT44(param_3,param_4);
          puVar5[-1] = in_xd6;
          puVar5[-2] = param_2;
          puVar5[-3] = in_xd4;
          puVar5[-4] = param_1;
          puVar5[-5] = in_xd2;
          puVar5[-6] = CONCAT44(extraout_fr2_01,extraout_fr3_01);
          puVar5[-7] = in_xd0;
          puVar5[-8] = uVar6;
        }
        if ((*param_7 & 8) == 0) {
          FUN_8c6215ba(param_7[1]);
        }
        if ((*param_7 & 0x10) == 0) {
          FUN_8c6215fa(param_7[0xb]);
        }
        goto LAB_8c621510;
      }
      if (uVar4 == 3) {
        (*(code *)PTR_FUN_8c621244)(0,uVar9);
        if (in_FPSCR_SZ == '\0') {
          param_2 = CONCAT44(param_7[10],(int)param_2);
          param_1 = CONCAT44(param_7[8],param_7[9]);
        }
        else {
          param_2 = *(undefined8 *)(param_7 + 10);
          param_1 = *(undefined8 *)(param_7 + 8);
          in_xd4 = *(undefined8 *)(param_7 + 9);
        }
        uVar6 = (*(code *)puVar1)(0);
        puVar5 = (undefined8 *)*puVar2;
        *puVar3 = puVar5;
        if (in_FPSCR_SZ == '\0') {
          *(undefined4 *)((int)puVar5 + 0x3c) = in_fr15;
          *(undefined4 *)(puVar5 + 7) = in_fr14;
          *(undefined4 *)((int)puVar5 + 0x34) = in_fr13;
          *(undefined4 *)(puVar5 + 6) = in_fr12;
          *(undefined4 *)((int)puVar5 + 0x2c) = param_6;
          *(undefined4 *)(puVar5 + 5) = param_5;
          *(undefined4 *)((int)puVar5 + 0x24) = param_4;
          *(undefined4 *)(puVar5 + 4) = param_3;
          *(int *)((int)puVar5 + 0x1c) = (int)param_2;
          *(int *)(puVar5 + 3) = (int)((ulonglong)param_2 >> 0x20);
          *(int *)((int)puVar5 + 0x14) = (int)param_1;
          *(int *)(puVar5 + 2) = (int)((ulonglong)param_1 >> 0x20);
          *(undefined4 *)((int)puVar5 + 0xc) = extraout_fr3_02;
          *(undefined4 *)(puVar5 + 1) = extraout_fr2_02;
          *(int *)((int)puVar5 + 4) = (int)uVar6;
          *(int *)puVar5 = (int)((ulonglong)uVar6 >> 0x20);
        }
        else {
          puVar5[7] = in_xd14;
          puVar5[6] = CONCAT44(in_fr14,in_fr15);
          puVar5[5] = in_xd12;
          puVar5[4] = CONCAT44(in_fr12,in_fr13);
          puVar5[3] = in_xd10;
          puVar5[2] = CONCAT44(param_5,param_6);
          puVar5[1] = in_xd8;
          *puVar5 = CONCAT44(param_3,param_4);
          puVar5[-1] = in_xd6;
          puVar5[-2] = param_2;
          puVar5[-3] = in_xd4;
          puVar5[-4] = param_1;
          puVar5[-5] = in_xd2;
          puVar5[-6] = CONCAT44(extraout_fr2_02,extraout_fr3_02);
          puVar5[-7] = in_xd0;
          puVar5[-8] = uVar6;
        }
        if ((*param_7 & 8) == 0) {
          FUN_8c6215ba(param_7[1]);
        }
        if ((*param_7 & 0x10) == 0) {
          FUN_8c6215fa(param_7[0xb]);
        }
        goto LAB_8c621510;
      }
      if (uVar4 == 4) {
        (*(code *)PTR_FUN_8c621244)(0,uVar9);
        if (in_FPSCR_SZ == '\0') {
          param_2 = CONCAT44(param_7[4],(int)param_2);
          param_1 = CONCAT44(param_7[2],param_7[3]);
        }
        else {
          param_2 = *(undefined8 *)(param_7 + 4);
          param_1 = *(undefined8 *)(param_7 + 2);
          in_xd4 = *(undefined8 *)(param_7 + 3);
        }
        (*(code *)PTR_FUN_8c6214b4)(0);
        if ((*param_7 & 0x20) == 0) {
          uVar6 = (*(code *)PTR_FUN_8c6214b0)(0,param_7[5],param_7[6],param_7[7]);
          uVar7 = extraout_fr3_04;
          uVar8 = extraout_fr2_04;
        }
        else {
          uVar6 = (*(code *)PTR_FUN_8c6214ac)(0,param_7[5],param_7[6],param_7[7]);
          uVar7 = extraout_fr3_03;
          uVar8 = extraout_fr2_03;
        }
        puVar5 = (undefined8 *)*puVar2;
        *puVar3 = puVar5;
        if (in_FPSCR_SZ == '\0') {
          *(undefined4 *)((int)puVar5 + 0x3c) = in_fr15;
          *(undefined4 *)(puVar5 + 7) = in_fr14;
          *(undefined4 *)((int)puVar5 + 0x34) = in_fr13;
          *(undefined4 *)(puVar5 + 6) = in_fr12;
          *(undefined4 *)((int)puVar5 + 0x2c) = param_6;
          *(undefined4 *)(puVar5 + 5) = param_5;
          *(undefined4 *)((int)puVar5 + 0x24) = param_4;
          *(undefined4 *)(puVar5 + 4) = param_3;
          *(int *)((int)puVar5 + 0x1c) = (int)param_2;
          *(int *)(puVar5 + 3) = (int)((ulonglong)param_2 >> 0x20);
          *(int *)((int)puVar5 + 0x14) = (int)param_1;
          *(int *)(puVar5 + 2) = (int)((ulonglong)param_1 >> 0x20);
          *(undefined4 *)((int)puVar5 + 0xc) = uVar7;
          *(undefined4 *)(puVar5 + 1) = uVar8;
          *(int *)((int)puVar5 + 4) = (int)uVar6;
          *(int *)puVar5 = (int)((ulonglong)uVar6 >> 0x20);
        }
        else {
          puVar5[7] = in_xd14;
          puVar5[6] = CONCAT44(in_fr14,in_fr15);
          puVar5[5] = in_xd12;
          puVar5[4] = CONCAT44(in_fr12,in_fr13);
          puVar5[3] = in_xd10;
          puVar5[2] = CONCAT44(param_5,param_6);
          puVar5[1] = in_xd8;
          *puVar5 = CONCAT44(param_3,param_4);
          puVar5[-1] = in_xd6;
          puVar5[-2] = param_2;
          puVar5[-3] = in_xd4;
          puVar5[-4] = param_1;
          puVar5[-5] = in_xd2;
          puVar5[-6] = CONCAT44(uVar8,uVar7);
          puVar5[-7] = in_xd0;
          puVar5[-8] = uVar6;
        }
        if ((*param_7 & 8) == 0) {
          FUN_8c6215ba(param_7[1]);
        }
        if ((*param_7 & 0x10) == 0) {
          FUN_8c6215fa(param_7[0xb]);
        }
        goto LAB_8c621510;
      }
      if (uVar4 == 5) {
        (*(code *)PTR_FUN_8c621244)(0,uVar9);
        if ((*param_7 & 0x20) == 0) {
          uVar6 = (*(code *)PTR_FUN_8c6214b0)(0,param_7[5],param_7[6],param_7[7]);
          uVar7 = extraout_fr3_06;
          uVar8 = extraout_fr2_06;
        }
        else {
          uVar6 = (*(code *)PTR_FUN_8c6214ac)(0,param_7[5],param_7[6],param_7[7]);
          uVar7 = extraout_fr3_05;
          uVar8 = extraout_fr2_05;
        }
        puVar5 = (undefined8 *)*puVar2;
        *puVar3 = puVar5;
        if (in_FPSCR_SZ == '\0') {
          *(undefined4 *)((int)puVar5 + 0x3c) = in_fr15;
          *(undefined4 *)(puVar5 + 7) = in_fr14;
          *(undefined4 *)((int)puVar5 + 0x34) = in_fr13;
          *(undefined4 *)(puVar5 + 6) = in_fr12;
          *(undefined4 *)((int)puVar5 + 0x2c) = param_6;
          *(undefined4 *)(puVar5 + 5) = param_5;
          *(undefined4 *)((int)puVar5 + 0x24) = param_4;
          *(undefined4 *)(puVar5 + 4) = param_3;
          *(int *)((int)puVar5 + 0x1c) = (int)param_2;
          *(int *)(puVar5 + 3) = (int)((ulonglong)param_2 >> 0x20);
          *(int *)((int)puVar5 + 0x14) = (int)param_1;
          *(int *)(puVar5 + 2) = (int)((ulonglong)param_1 >> 0x20);
          *(undefined4 *)((int)puVar5 + 0xc) = uVar7;
          *(undefined4 *)(puVar5 + 1) = uVar8;
          *(int *)((int)puVar5 + 4) = (int)uVar6;
          *(int *)puVar5 = (int)((ulonglong)uVar6 >> 0x20);
        }
        else {
          puVar5[7] = in_xd14;
          puVar5[6] = CONCAT44(in_fr14,in_fr15);
          puVar5[5] = in_xd12;
          puVar5[4] = CONCAT44(in_fr12,in_fr13);
          puVar5[3] = in_xd10;
          puVar5[2] = CONCAT44(param_5,param_6);
          puVar5[1] = in_xd8;
          *puVar5 = CONCAT44(param_3,param_4);
          puVar5[-1] = in_xd6;
          puVar5[-2] = param_2;
          puVar5[-3] = in_xd4;
          puVar5[-4] = param_1;
          puVar5[-5] = in_xd2;
          puVar5[-6] = CONCAT44(uVar8,uVar7);
          puVar5[-7] = in_xd0;
          puVar5[-8] = uVar6;
        }
        if ((*param_7 & 8) == 0) {
          FUN_8c6215ba(param_7[1]);
        }
        if ((*param_7 & 0x10) == 0) {
          FUN_8c6215fa(param_7[0xb]);
        }
        goto LAB_8c621510;
      }
      if (uVar4 == 6) {
        (*(code *)PTR_FUN_8c621244)(0,uVar9);
        if (in_FPSCR_SZ == '\0') {
          param_2 = CONCAT44(param_7[4],(int)param_2);
          param_1 = CONCAT44(param_7[2],param_7[3]);
        }
        else {
          param_2 = *(undefined8 *)(param_7 + 4);
          param_1 = *(undefined8 *)(param_7 + 2);
          in_xd4 = *(undefined8 *)(param_7 + 3);
        }
        uVar6 = (*(code *)PTR_FUN_8c621614)(0);
        puVar5 = (undefined8 *)*puVar2;
        *puVar3 = puVar5;
        if (in_FPSCR_SZ == '\0') {
          *(undefined4 *)((int)puVar5 + 0x3c) = in_fr15;
          *(undefined4 *)(puVar5 + 7) = in_fr14;
          *(undefined4 *)((int)puVar5 + 0x34) = in_fr13;
          *(undefined4 *)(puVar5 + 6) = in_fr12;
          *(undefined4 *)((int)puVar5 + 0x2c) = param_6;
          *(undefined4 *)(puVar5 + 5) = param_5;
          *(undefined4 *)((int)puVar5 + 0x24) = param_4;
          *(undefined4 *)(puVar5 + 4) = param_3;
          *(int *)((int)puVar5 + 0x1c) = (int)param_2;
          *(int *)(puVar5 + 3) = (int)((ulonglong)param_2 >> 0x20);
          *(int *)((int)puVar5 + 0x14) = (int)param_1;
          *(int *)(puVar5 + 2) = (int)((ulonglong)param_1 >> 0x20);
          *(undefined4 *)((int)puVar5 + 0xc) = extraout_fr3_07;
          *(undefined4 *)(puVar5 + 1) = extraout_fr2_07;
          *(int *)((int)puVar5 + 4) = (int)uVar6;
          *(int *)puVar5 = (int)((ulonglong)uVar6 >> 0x20);
        }
        else {
          puVar5[7] = in_xd14;
          puVar5[6] = CONCAT44(in_fr14,in_fr15);
          puVar5[5] = in_xd12;
          puVar5[4] = CONCAT44(in_fr12,in_fr13);
          puVar5[3] = in_xd10;
          puVar5[2] = CONCAT44(param_5,param_6);
          puVar5[1] = in_xd8;
          *puVar5 = CONCAT44(param_3,param_4);
          puVar5[-1] = in_xd6;
          puVar5[-2] = param_2;
          puVar5[-3] = in_xd4;
          puVar5[-4] = param_1;
          puVar5[-5] = in_xd2;
          puVar5[-6] = CONCAT44(extraout_fr2_07,extraout_fr3_07);
          puVar5[-7] = in_xd0;
          puVar5[-8] = uVar6;
        }
        if ((*param_7 & 8) == 0) {
          FUN_8c6215ba(param_7[1]);
        }
        if ((*param_7 & 0x10) == 0) {
          FUN_8c6215fa(param_7[0xb]);
        }
        goto LAB_8c621510;
      }
      if (uVar4 == 7) {
        puVar5 = (undefined8 *)*DAT_8c621248;
        *DAT_8c62124c = puVar5;
        if (in_FPSCR_SZ == '\0') {
          *(undefined4 *)((int)puVar5 + 0x3c) = in_fr15;
          *(undefined4 *)(puVar5 + 7) = in_fr14;
          *(undefined4 *)((int)puVar5 + 0x34) = in_fr13;
          *(undefined4 *)(puVar5 + 6) = in_fr12;
          *(undefined4 *)((int)puVar5 + 0x2c) = param_6;
          *(undefined4 *)(puVar5 + 5) = param_5;
          *(undefined4 *)((int)puVar5 + 0x24) = param_4;
          *(undefined4 *)(puVar5 + 4) = param_3;
          *(int *)((int)puVar5 + 0x1c) = (int)param_2;
          *(int *)(puVar5 + 3) = (int)((ulonglong)param_2 >> 0x20);
          *(int *)((int)puVar5 + 0x14) = (int)param_1;
          *(int *)(puVar5 + 2) = (int)((ulonglong)param_1 >> 0x20);
          *(undefined4 *)((int)puVar5 + 0xc) = in_fr3;
          *(undefined4 *)(puVar5 + 1) = in_fr2;
          *(int *)((int)puVar5 + 4) = (int)uVar6;
          *(int *)puVar5 = (int)((ulonglong)uVar6 >> 0x20);
        }
        else {
          puVar5[7] = in_xd14;
          puVar5[6] = CONCAT44(in_fr14,in_fr15);
          puVar5[5] = in_xd12;
          puVar5[4] = CONCAT44(in_fr12,in_fr13);
          puVar5[3] = in_xd10;
          puVar5[2] = CONCAT44(param_5,param_6);
          puVar5[1] = in_xd8;
          *puVar5 = CONCAT44(param_3,param_4);
          puVar5[-1] = in_xd6;
          puVar5[-2] = param_2;
          puVar5[-3] = in_xd4;
          puVar5[-4] = param_1;
          puVar5[-5] = in_xd2;
          puVar5[-6] = CONCAT44(in_fr2,in_fr3);
          puVar5[-7] = in_xd0;
          puVar5[-8] = uVar6;
        }
        if ((*param_7 & 8) == 0) {
          uVar6 = FUN_8c6215ba(param_7[1]);
          in_fr3 = extraout_fr3_09;
          in_fr2 = extraout_fr2_09;
        }
        if ((*param_7 & 0x10) == 0) {
          uVar6 = FUN_8c6215fa(param_7[0xb]);
          in_fr3 = extraout_fr3_10;
          in_fr2 = extraout_fr2_10;
        }
      }
    }
    param_7 = (uint *)param_7[0xc];
    if (param_7 == (uint *)0x0) {
      return;
    }
  } while( true );
}

