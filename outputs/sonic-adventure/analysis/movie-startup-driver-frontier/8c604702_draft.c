/* Ghidra draft only; not buildable/verified source. */

void FUN_8c604702(uint param_1,uint param_2,undefined4 param_3,undefined4 param_4)

{
  undefined4 *puVar1;
  undefined4 *puVar2;
  int *piVar3;
  int *piVar4;
  uint *puVar5;
  undefined *puVar6;
  undefined *puVar7;
  undefined4 uVar8;
  uint uVar9;
  int iVar10;
  int iVar11;
  undefined1 *puVar12;
  undefined4 uVar13;
  undefined8 in_dr2;
  double dVar14;
  undefined4 in_fr13;
  undefined8 in_dr14;
  double dVar15;
  undefined8 in_xd12;
  undefined8 in_xd14;
  bool bVar16;
  undefined4 in_PR;
  char in_FPSCR_PR;
  char in_FPSCR_SZ;
  undefined1 local_2c [12];
  undefined1 local_20 [4];
  undefined8 uStack_1c;
  
  iVar11 = DAT_8c604798;
  piVar3 = DAT_8c604790._4_4_;
  puVar2 = DAT_8c604788;
  uVar13 = (undefined4)((ulonglong)in_dr2 >> 0x20);
  if (in_FPSCR_SZ == '\0') {
    uStack_1c = CONCAT44((int)in_dr14,(undefined4)uStack_1c);
                    /* WARNING: Ignoring partial resolution of indirect */
    uStack_1c._0_4_ = (int)((ulonglong)in_dr14 >> 0x20);
    puVar12 = local_20;
  }
  else {
    puVar12 = local_2c;
    uStack_1c = in_xd14;
  }
  *(undefined4 *)(puVar12 + -4) = in_PR;
  *(undefined4 *)(puVar12 + -0x10) = param_3;
  *DAT_8c604784 = param_2;
  uVar9 = (uint)DAT_8c604770;
  *puVar2 = *(undefined4 *)(puVar12 + -0x10);
  *DAT_8c60478c = param_4;
  *(uint *)(puVar12 + -8) = uVar9 & param_2;
  uVar8 = (undefined4)DAT_8c604790;
  if (in_FPSCR_SZ != '\0') {
    in_xd12 = CONCAT44(DAT_8c604790._4_4_,(undefined4)DAT_8c604790);
    uVar8 = in_fr13;
  }
  if ((param_2 & 0x10) == 0) {
    iVar10 = (int)DAT_8c604774;
    if (in_FPSCR_SZ == '\0') {
      *(undefined4 *)(iVar11 + 4) = (undefined4)DAT_8c60479c;
    }
    else {
      *(ulonglong *)(iVar11 + 4) = CONCAT44(DAT_8c60479c._4_4_,(undefined4)DAT_8c60479c);
    }
  }
  else {
    iVar10 = (int)DAT_8c604772;
    if (in_FPSCR_SZ == '\0') {
      *(undefined4 *)(iVar11 + 4) = uVar8;
    }
    else {
      *(undefined8 *)(iVar11 + 4) = in_xd12;
    }
  }
  *piVar3 = iVar10;
  piVar4 = DAT_8c6047a8;
  if (in_FPSCR_SZ == '\0') {
    dVar15 = (double)CONCAT44(DAT_8c60479c._4_4_,(int)in_dr14);
  }
  else {
    dVar15 = (double)CONCAT44(DAT_8c6047a4,DAT_8c60479c._4_4_);
  }
  if (in_FPSCR_SZ == '\0') {
    dVar15 = (double)CONCAT44((int)((ulonglong)dVar15 >> 0x20),DAT_8c6047a4);
  }
  else {
    in_xd14 = CONCAT44(DAT_8c6047a8,DAT_8c6047a4);
  }
  if ((param_2 & 0x20) == 0) {
    iVar10 = (int)DAT_8c604858;
    if (in_FPSCR_SZ == '\0') {
      *(int *)(iVar11 + 8) = (int)((ulonglong)dVar15 >> 0x20);
    }
    else {
      *(double *)(iVar11 + 8) = dVar15;
    }
  }
  else {
    iVar10 = (int)DAT_8c604776;
    if (in_FPSCR_SZ == '\0') {
      *(int *)(iVar11 + 8) = SUB84(dVar15,0);
    }
    else {
      *(undefined8 *)(iVar11 + 8) = in_xd14;
    }
  }
  *piVar4 = iVar10;
  if ((param_2 & 3) == 1) {
    FUN_8c604594();
    FUN_8c6045b4(0);
    iVar10 = (int)DAT_8c60485a;
    if (in_FPSCR_SZ == '\0') {
      *(undefined4 *)(iVar11 + 4) = uVar8;
    }
    else {
      *(undefined8 *)(iVar11 + 4) = in_xd12;
    }
    *piVar3 = iVar10;
    iVar10 = (int)DAT_8c60485c;
    if (in_FPSCR_SZ == '\0') {
      *(int *)(iVar11 + 8) = SUB84(dVar15,0);
    }
    else {
      *(undefined8 *)(iVar11 + 8) = in_xd14;
    }
    *piVar4 = iVar10;
  }
  else {
    uVar8 = (*(code *)PTR_FUN_8c604864)();
    *(undefined4 *)(puVar12 + -0xc) = uVar8;
    FUN_8c604594();
    iVar10 = *(int *)(puVar12 + -0xc);
    if (iVar10 == 2) {
      iVar10 = (int)DAT_8c60485e;
    }
    else if (iVar10 == 3) {
      iVar10 = (int)DAT_8c604860;
    }
    else if (iVar10 == 1) {
      iVar10 = (int)DAT_8c604862;
    }
    else {
      iVar10 = (int)DAT_8c604860;
    }
    FUN_8c6045b4(iVar10);
  }
  if ((param_2 & DAT_8c604868) == 0) {
    uVar13 = 0;
  }
  else {
    if (in_FPSCR_PR == '\0') {
      uVar13 = 0x40000000;
    }
    else {
      uVar13 = SUB84((double)CONCAT44(uVar13,0x3f800000) + (double)CONCAT44(uVar13,0x3f800000),0);
    }
    if (in_FPSCR_SZ == '\0') {
      dVar14 = (double)CONCAT44(*(undefined4 *)(iVar11 + 4),uVar13);
    }
    else {
      dVar14 = *(double *)(iVar11 + 4);
    }
    if (in_FPSCR_PR == '\0') {
      dVar14 = (double)CONCAT44((float)((ulonglong)dVar14 >> 0x20) * SUB84(dVar14,0),SUB84(dVar14,0)
                               );
    }
    else {
      dVar14 = dVar14 * dVar14;
    }
    if (in_FPSCR_SZ == '\0') {
      *(int *)(iVar11 + 4) = (int)((ulonglong)dVar14 >> 0x20);
      dVar14 = (double)CONCAT44(*(undefined4 *)(iVar11 + 8),SUB84(dVar14,0));
    }
    else {
      *(double *)(iVar11 + 4) = dVar14;
      dVar14 = *(double *)(iVar11 + 8);
    }
    if (in_FPSCR_PR == '\0') {
      bVar16 = (float)((ulonglong)dVar14 >> 0x20) == (float)((ulonglong)dVar15 >> 0x20);
    }
    else {
      bVar16 = dVar14 == dVar15;
    }
    uVar13 = 1;
    if (bVar16) {
      if (in_FPSCR_SZ == '\0') {
        *(int *)(iVar11 + 8) = SUB84(dVar15,0);
      }
      else {
        *(undefined8 *)(iVar11 + 8) = in_xd14;
      }
    }
  }
  iVar10 = *(int *)(puVar12 + -0x10);
  if (iVar10 == 0) {
    iVar11 = 0;
  }
  else if (iVar10 == 1) {
    iVar11 = 1;
  }
  else if (iVar10 == 3) {
    iVar11 = 3;
  }
  else if (iVar10 == 4) {
    iVar11 = 4;
  }
  else if (iVar10 == 5) {
    iVar11 = 5;
  }
  *DAT_8c604954 = *DAT_8c604954 | 8;
  puVar5 = DAT_8c604958;
  *DAT_8c604958 = 8;
  do {
  } while ((*puVar5 & 8) == 0);
  (*(code *)PTR_FUN_8c60495c)(param_2 & 3);
  (*(code *)PTR_FUN_8c604960)(*(undefined4 *)(puVar12 + -8),iVar11,1,uVar13);
  uVar13 = DAT_8c60496c;
  puVar1 = DAT_8c604968;
  puVar2 = DAT_8c604964;
  *DAT_8c604964 = 0x60;
  puVar7 = PTR_DAT_8c604978;
  puVar6 = PTR_DAT_8c604974;
  uVar8 = DAT_8c604970;
  puVar2[1] = 1;
  *puVar1 = uVar13;
  puVar1[1] = uVar8;
  puVar2[2] = puVar1;
  puVar2[4] = 0;
  puVar2[5] = 0;
  puVar2[7] = *(undefined4 *)puVar6;
  *(undefined4 *)puVar7 = 0;
  puVar2[8] = DAT_8c60497c;
  puVar2[9] = 0;
  puVar2[10] = 0;
  if (in_FPSCR_SZ == '\0') {
    puVar2[0xb] = 0;
    puVar2[0xc] = 0;
    puVar2[0xd] = 0;
    puVar2[0xe] = 0;
    puVar1 = DAT_8c604980;
    puVar2[0xf] = 0;
  }
  else {
    *(ulonglong *)(puVar2 + 0xb) = (ulonglong)param_1;
    *(ulonglong *)(puVar2 + 0xc) = (ulonglong)param_1;
    *(ulonglong *)(puVar2 + 0xd) = (ulonglong)param_1;
    *(ulonglong *)(puVar2 + 0xe) = (ulonglong)param_1;
    puVar1 = DAT_8c604980;
    *(ulonglong *)(puVar2 + 0xf) = (ulonglong)param_1;
  }
  *puVar1 = 0;
  *DAT_8c604984 = 0;
  *DAT_8c604988 = 1;
  *DAT_8c60498c = 0;
  (*(code *)PTR_FUN_8c604994)(PTR_FUN_8c604990,0);
  (*(code *)PTR_FUN_8c60499c)(PTR_LAB_8c604998,0);
  (*(code *)PTR_FUN_8c6049a4)(PTR_LAB_8c6049a0);
  (*(code *)PTR_FUN_8c6049ac)(*DAT_8c6049a8);
  FUN_8c604ba0();
  if ((*DAT_8c6049b0 & 0xff) < 10) {
    (*(code *)PTR_FUN_8c6049b8)(DAT_8c6049b4);
                    /* WARNING: Could not recover jumptable at 0x8c60493a. Too many branches */
                    /* WARNING: Treating indirect jump as call */
    (**(code **)PTR_DAT_8c6049bc)();
    return;
  }
  return;
}

