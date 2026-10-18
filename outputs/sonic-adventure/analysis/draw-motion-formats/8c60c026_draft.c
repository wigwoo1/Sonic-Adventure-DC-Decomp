/* Ghidra draft only; not buildable/verified source. */

/* WARNING: Removing unreachable block (ram,0x8c60c086) */
/* WARNING: Removing unreachable block (ram,0x8c60c0a4) */
/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void callback_8c60c026(undefined4 param_1,double param_2,undefined4 param_3,undefined4 param_4,
                      int param_5,int param_6,double *param_7)

{
  double dVar1;
  int iVar2;
  int iVar3;
  int *piVar4;
  int *piVar5;
  double extraout_dr0;
  double dVar6;
  float fVar7;
  float fVar8;
  undefined4 uVar9;
  double dVar10;
  double dVar11;
  ulonglong uVar12;
  uint uVar14;
  float fVar15;
  double dVar13;
  float fVar16;
  double dVar17;
  double dVar18;
  float fVar20;
  double dVar19;
  double in_xd0;
  double in_xd2;
  double dVar21;
  double in_xd4;
  double in_xd6;
  double in_xd8;
  double in_xd10;
  undefined4 in_PR;
  char in_FPSCR_PR;
  char in_FPSCR_SZ;
  undefined1 auStack_1c [8];
  undefined4 uStack_14;
  
  uStack_14 = in_PR;
  (*(code *)PTR_FUN_8c60c178)();
  iVar2 = (*(code *)PTR_FUN_8c60c17c)();
  iVar3 = iVar2 * 0x10 + param_5;
  piVar4 = (int *)(param_6 * 0x10 + param_5);
  iVar2 = (*(code *)PTR_FUN_8c60c17c)();
  piVar5 = (int *)(iVar2 * 0x10 + param_5);
  iVar2 = (*(code *)PTR_FUN_8c60c17c)();
  param_5 = param_5 + iVar2 * 0x10;
  fVar7 = (float)(*piVar5 - *piVar4);
  dVar10 = (double)(ulonglong)(uint)fVar7;
  dVar19 = in_xd2;
  if (in_FPSCR_SZ == '\0') {
    dVar19 = (double)CONCAT44(fVar7,param_1);
  }
  uVar14 = (uint)((ulonglong)dVar19 >> 0x20);
  dVar13 = extraout_dr0;
  dVar21 = dVar19;
  if (in_FPSCR_PR != '\0' || 0.0 <= fVar7) {
    if (in_FPSCR_SZ == '\0') {
      dVar10 = (double)(ulonglong)uVar14;
      dVar21 = in_xd2;
    }
  }
  else {
    if (in_FPSCR_SZ == '\0') {
      dVar10 = (double)(ulonglong)uVar14;
      dVar13 = (double)CONCAT44((int)((ulonglong)extraout_dr0 >> 0x20),DAT_8c60c180);
      dVar21 = in_xd2;
    }
    else {
      in_xd0 = (double)CONCAT44(DAT_8c60c184,DAT_8c60c180);
    }
    if (in_FPSCR_PR == '\0') {
      dVar10 = (double)(ulonglong)(uint)(SUB84(dVar10,0) + SUB84(dVar13,0));
    }
    else {
      dVar10 = dVar10 + dVar13;
    }
  }
  if (in_FPSCR_SZ == '\0') {
    stack0xffffffe8 = (double)CONCAT44(uStack_14,SUB84(dVar10,0));
  }
  else {
    stack0xffffffe8 = dVar21;
  }
  iVar2 = *piVar4;
  fVar7 = (float)iVar2;
  dVar10 = (double)CONCAT44((int)((ulonglong)dVar10 >> 0x20),fVar7);
  if (iVar2 < 0) {
    if (in_FPSCR_PR == '\0') {
      dVar10 = (double)CONCAT44(DAT_8c60c180,fVar7 + DAT_8c60c180);
    }
    else {
      dVar10 = (double)CONCAT44(DAT_8c60c180,fVar7) + (double)CONCAT44(DAT_8c60c180,fVar7);
    }
  }
  if (in_FPSCR_SZ == '\0') {
    dVar13 = (double)CONCAT44((int)((ulonglong)dVar13 >> 0x20),*(undefined4 *)DAT_8c60c184);
  }
  else {
    in_xd0 = *DAT_8c60c184;
  }
  if (in_FPSCR_PR == '\0') {
    fVar7 = SUB84(dVar13,0) - SUB84(dVar10,0);
  }
  else {
    fVar7 = SUB84(dVar13 - dVar10,0);
  }
  iVar2 = (uint)(0 < *piVar5 - iVar2) * 4;
  if (in_FPSCR_SZ == '\0') {
    dVar10 = (double)CONCAT44((int)((ulonglong)dVar10 >> 0x20),*(undefined4 *)(auStack_1c + iVar2));
    in_xd0 = (double)CONCAT44(fVar7,SUB84(dVar19,0));
  }
  else {
    dVar21 = *(double *)(auStack_1c + iVar2);
  }
  if (in_FPSCR_PR == '\0') {
    in_xd0 = (double)CONCAT44((float)((ulonglong)in_xd0 >> 0x20) / SUB84(dVar10,0),SUB84(in_xd0,0));
  }
  else {
    in_xd0 = in_xd0 / dVar10;
  }
  uVar9 = (undefined4)((ulonglong)dVar10 >> 0x20);
  dVar10 = (double)CONCAT44(uVar9,0x3f800000);
  if (in_FPSCR_PR == '\0') {
    dVar10 = (double)CONCAT44(uVar9,0x40000000);
  }
  else {
    dVar10 = dVar10 + dVar10;
  }
  if (in_FPSCR_SZ == '\0') {
    dVar19 = (double)CONCAT44(_DAT_8c60c190,param_4);
    uVar9 = (undefined4)((ulonglong)in_xd0 >> 0x20);
    in_xd0 = (double)CONCAT44(uVar9,uVar9);
  }
  else {
    dVar19 = (double)CONCAT44(_DAT_8c60c194,_DAT_8c60c190);
    in_xd4 = in_xd0;
  }
  if (in_FPSCR_PR == '\0') {
    fVar15 = (float)((ulonglong)in_xd0 >> 0x20);
    in_xd0 = (double)CONCAT44(fVar15,SUB84(in_xd0,0) * fVar15);
  }
  else {
    in_xd0 = in_xd0 * in_xd0;
  }
  fVar8 = (float)((ulonglong)in_xd0 >> 0x20);
  fVar15 = SUB84(in_xd0,0);
  dVar17 = in_xd4;
  dVar13 = in_xd0;
  if (in_FPSCR_SZ == '\0') {
    param_2 = (double)CONCAT44((int)((ulonglong)param_2 >> 0x20),fVar8);
    dVar17 = (double)CONCAT44(fVar15,param_3);
    dVar13 = in_xd6;
  }
  if (in_FPSCR_PR == '\0') {
    dVar17 = (double)CONCAT44((float)((ulonglong)dVar17 >> 0x20) * SUB84(dVar10,0),SUB84(dVar17,0));
    param_2 = (double)CONCAT44((int)((ulonglong)param_2 >> 0x20),SUB84(param_2,0) * fVar15);
  }
  else {
    dVar17 = dVar17 * dVar10;
    param_2 = param_2 * in_xd0;
  }
  uVar9 = (undefined4)((ulonglong)dVar17 >> 0x20);
  dVar11 = dVar17;
  if (in_FPSCR_SZ == '\0') {
    dVar11 = (double)CONCAT44(uVar9,SUB84(dVar10,0));
  }
  if (in_FPSCR_PR == '\0') {
    dVar11 = (double)CONCAT44((float)((ulonglong)dVar11 >> 0x20) - fVar8,SUB84(dVar11,0));
  }
  else {
    dVar11 = dVar11 - in_xd0;
  }
  fVar16 = SUB84(param_2,0);
  dVar10 = dVar13;
  if (in_FPSCR_SZ == '\0') {
    dVar19 = (double)CONCAT44((int)((ulonglong)dVar19 >> 0x20),fVar16);
    dVar10 = in_xd10;
  }
  if (in_FPSCR_PR == '\0') {
    dVar11 = (double)CONCAT44((float)((ulonglong)dVar11 >> 0x20) - fVar16,SUB84(dVar11,0));
  }
  else {
    dVar11 = dVar11 - param_2;
  }
  dVar6 = dVar11;
  if (in_FPSCR_SZ == '\0') {
    dVar17 = (double)CONCAT44(uVar9,(int)((ulonglong)dVar11 >> 0x20));
    dVar6 = in_xd8;
  }
  fVar20 = (float)((ulonglong)dVar19 >> 0x20);
  if (in_FPSCR_PR == '\0') {
    dVar17 = (double)CONCAT44((int)((ulonglong)dVar17 >> 0x20),SUB84(dVar17,0) * fVar20);
  }
  else {
    dVar17 = dVar17 * dVar19;
  }
  if (in_FPSCR_SZ == '\0') {
    dVar11 = (double)CONCAT44(_DAT_8c60c194,SUB84(dVar11,0));
    fVar7 = _DAT_8c60c198;
  }
  else {
    dVar11 = (double)CONCAT44(_DAT_8c60c198,_DAT_8c60c194);
  }
  if (in_FPSCR_PR == '\0') {
    dVar19 = (double)CONCAT44(fVar20,SUB84(dVar19,0) * (float)((ulonglong)dVar11 >> 0x20));
  }
  else {
    dVar19 = dVar19 * dVar11;
  }
  if (in_FPSCR_SZ == '\0') {
    in_xd4 = (double)CONCAT44(fVar15,fVar7);
  }
  fVar7 = (float)((ulonglong)dVar19 >> 0x20);
  if (in_FPSCR_PR == '\0') {
    in_xd4 = (double)CONCAT44((float)((ulonglong)in_xd4 >> 0x20) * SUB84(in_xd4,0),SUB84(in_xd4,0));
    in_xd0 = (double)CONCAT44(fVar8 * fVar7,fVar15);
  }
  else {
    in_xd4 = in_xd4 * in_xd4;
    in_xd0 = in_xd0 * dVar19;
  }
  if (in_FPSCR_SZ == '\0') {
    dVar11 = (double)((ulonglong)dVar19 & 0xffffffff);
    dVar10 = dVar21;
  }
  if (in_FPSCR_PR == '\0') {
    fVar16 = fVar16 - SUB84(in_xd0,0);
  }
  else {
    fVar16 = SUB84(param_2 - in_xd0,0);
  }
  if (in_FPSCR_SZ == '\0') {
    dVar21 = (double)CONCAT44(*(undefined4 *)(iVar3 + 4),SUB84(dVar11,0));
  }
  else {
    dVar21 = *(double *)(iVar3 + 4);
  }
  if (in_FPSCR_PR == '\0') {
    dVar21 = (double)CONCAT44((int)((ulonglong)dVar21 >> 0x20),
                              SUB84(dVar21,0) - (float)((ulonglong)in_xd4 >> 0x20));
  }
  else {
    dVar21 = dVar21 - in_xd4;
  }
  if (in_FPSCR_PR == '\0') {
    in_xd0 = (double)CONCAT44((float)((ulonglong)in_xd0 >> 0x20) - SUB84(dVar19,0),SUB84(in_xd0,0));
  }
  else {
    in_xd0 = in_xd0 - dVar19;
  }
  dVar11 = dVar10;
  if (in_FPSCR_SZ == '\0') {
    dVar11 = (double)CONCAT44(SUB84(dVar21,0),fVar16);
  }
  if (in_FPSCR_PR == '\0') {
    dVar11 = (double)CONCAT44((float)((ulonglong)dVar11 >> 0x20) + 1.0,SUB84(dVar11,0));
  }
  else {
    dVar11 = dVar11 + (double)CONCAT44(0x3f800000,SUB84(in_xd4,0));
  }
  dVar18 = in_xd0;
  if (in_FPSCR_SZ == '\0') {
    dVar21 = (double)CONCAT44((int)((ulonglong)dVar21 >> 0x20),(int)((ulonglong)dVar17 >> 0x20));
    dVar18 = (double)CONCAT44((int)((ulonglong)in_xd0 >> 0x20),SUB84(dVar17,0));
    dVar17 = dVar10;
  }
  if (in_FPSCR_PR == '\0') {
    dVar18 = (double)CONCAT44((float)((ulonglong)dVar18 >> 0x20) + SUB84(dVar21,0),SUB84(dVar18,0));
  }
  else {
    dVar18 = dVar18 + dVar21;
  }
  fVar15 = (float)((ulonglong)dVar11 >> 0x20);
  if (in_FPSCR_SZ == '\0') {
    dVar10 = (double)CONCAT44(piVar4[1],SUB84(in_xd4,0));
    dVar21 = (double)CONCAT44((int)((ulonglong)dVar21 >> 0x20),fVar15);
  }
  else {
    dVar10 = *(double *)(piVar4 + 1);
    dVar17 = dVar11;
  }
  if (in_FPSCR_PR == '\0') {
    dVar21 = (double)CONCAT44((int)((ulonglong)dVar21 >> 0x20),
                              SUB84(dVar21,0) * (float)((ulonglong)dVar10 >> 0x20));
  }
  else {
    dVar21 = dVar21 * dVar10;
  }
  dVar1 = dVar6;
  if (in_FPSCR_SZ == '\0') {
    dVar13 = (double)CONCAT44(SUB84(dVar11,0),SUB84(in_xd0,0));
    dVar1 = (double)CONCAT44(SUB84(dVar18,0),SUB84(dVar10,0));
  }
  if (in_FPSCR_PR == '\0') {
    dVar13 = (double)CONCAT44((float)((ulonglong)dVar13 >> 0x20) * fVar7,SUB84(dVar13,0));
    dVar21 = (double)(ulonglong)
                     (uint)((float)((ulonglong)dVar1 >> 0x20) * (float)((ulonglong)dVar21 >> 0x20) +
                           SUB84(dVar21,0));
  }
  else {
    dVar13 = dVar13 * dVar19;
  }
  uVar14 = (uint)((ulonglong)dVar18 >> 0x20);
  if (in_FPSCR_SZ == '\0') {
    uVar12 = CONCAT44(piVar5[1],SUB84(dVar21,0));
    dVar10 = (double)CONCAT44(uVar14,SUB84(dVar1,0));
  }
  else {
    uVar12 = *(ulonglong *)(piVar5 + 1);
    dVar10 = dVar18;
  }
  if (in_FPSCR_PR == '\0') {
    uVar12 = (ulonglong)
             (uint)((float)((ulonglong)dVar10 >> 0x20) * (float)(uVar12 >> 0x20) + (float)uVar12);
  }
  fVar7 = (float)((ulonglong)dVar13 >> 0x20);
  if (in_FPSCR_SZ == '\0') {
    dVar19 = (double)CONCAT44(*(undefined4 *)(param_5 + 4),(int)uVar12);
    dVar10 = (double)CONCAT44(fVar7,SUB84(dVar10,0));
  }
  else {
    dVar19 = *(double *)(param_5 + 4);
    dVar10 = dVar13;
  }
  if (in_FPSCR_PR == '\0') {
    fVar8 = (float)((ulonglong)dVar19 >> 0x20);
    dVar19 = (double)CONCAT44(fVar8,(float)((ulonglong)dVar10 >> 0x20) * fVar8 + SUB84(dVar19,0));
  }
  if (in_FPSCR_SZ == '\0') {
    *(int *)param_7 = SUB84(dVar19,0);
    dVar10 = (double)CONCAT44(piVar4[2],SUB84(dVar10,0));
    dVar19 = (double)CONCAT44((int)((ulonglong)dVar19 >> 0x20),fVar15);
  }
  else {
    *param_7 = dVar17;
    dVar10 = *(double *)(piVar4 + 2);
    dVar17 = dVar11;
  }
  if (in_FPSCR_PR == '\0') {
    fVar8 = SUB84(dVar19,0) * (float)((ulonglong)dVar10 >> 0x20);
  }
  else {
    fVar8 = SUB84(dVar19 * dVar10,0);
  }
  if (in_FPSCR_SZ == '\0') {
    uVar12 = CONCAT44(*(undefined4 *)(iVar3 + 8),fVar8);
    dVar10 = (double)CONCAT44(SUB84(dVar18,0),SUB84(dVar10,0));
  }
  else {
    uVar12 = *(ulonglong *)(iVar3 + 8);
    dVar10 = dVar6;
  }
  if (in_FPSCR_PR == '\0') {
    uVar12 = (ulonglong)
             (uint)((float)((ulonglong)dVar10 >> 0x20) * (float)(uVar12 >> 0x20) + (float)uVar12);
  }
  if (in_FPSCR_SZ == '\0') {
    uVar12 = CONCAT44(piVar5[2],(int)uVar12);
    dVar10 = (double)CONCAT44(uVar14,SUB84(dVar10,0));
  }
  else {
    uVar12 = *(ulonglong *)(piVar5 + 2);
    dVar10 = dVar18;
  }
  if (in_FPSCR_PR == '\0') {
    uVar12 = (ulonglong)
             (uint)((float)((ulonglong)dVar10 >> 0x20) * (float)(uVar12 >> 0x20) + (float)uVar12);
  }
  if (in_FPSCR_SZ == '\0') {
    uVar12 = CONCAT44(*(undefined4 *)(param_5 + 8),(int)uVar12);
    dVar13 = (double)CONCAT44(fVar7,SUB84(dVar10,0));
  }
  else {
    uVar12 = *(ulonglong *)(param_5 + 8);
  }
  if (in_FPSCR_PR == '\0') {
    uVar12 = (ulonglong)
             (uint)((float)((ulonglong)dVar13 >> 0x20) * (float)(uVar12 >> 0x20) + (float)uVar12);
  }
  uVar9 = (undefined4)uVar12;
  if (in_FPSCR_SZ == '\0') {
    *(undefined4 *)((int)param_7 + 4) = uVar9;
    dVar10 = (double)CONCAT44(piVar4[3],SUB84(dVar13,0));
    uVar9 = *(undefined4 *)(iVar3 + 0xc);
  }
  else {
    *(double *)((int)param_7 + 4) = dVar17;
    dVar10 = *(double *)(piVar4 + 3);
    dVar17 = *(double *)(iVar3 + 0xc);
  }
  if (in_FPSCR_PR == '\0') {
    dVar11 = (double)CONCAT44(fVar15 * (float)((ulonglong)dVar10 >> 0x20),SUB84(dVar11,0));
  }
  else {
    dVar11 = dVar11 * dVar10;
  }
  if (in_FPSCR_SZ == '\0') {
    dVar6 = (double)((longlong)dVar18 << 0x20);
    uVar12 = CONCAT44(piVar5[3],uVar9);
  }
  else {
    uVar12 = *(ulonglong *)(piVar5 + 3);
  }
  if (in_FPSCR_PR == '\0') {
    dVar11 = (double)CONCAT44((float)((ulonglong)dVar6 >> 0x20) * (float)uVar12 +
                              (float)((ulonglong)dVar11 >> 0x20),SUB84(dVar11,0));
  }
  if (in_FPSCR_SZ == '\0') {
    dVar18 = (double)((ulonglong)uVar14 << 0x20);
    uVar12 = CONCAT44((int)(uVar12 >> 0x20),(int)((ulonglong)dVar11 >> 0x20));
    dVar11 = dVar17;
  }
  if (in_FPSCR_PR == '\0') {
    uVar12 = (ulonglong)
             (uint)((float)((ulonglong)dVar18 >> 0x20) * (float)(uVar12 >> 0x20) + (float)uVar12);
  }
  if (in_FPSCR_SZ == '\0') {
    uVar12 = CONCAT44(*(undefined4 *)(param_5 + 0xc),(int)uVar12);
  }
  else {
    uVar12 = *(ulonglong *)(param_5 + 0xc);
  }
  if (in_FPSCR_PR == '\0') {
    uVar12 = (ulonglong)(uint)(fVar7 * (float)(uVar12 >> 0x20) + (float)uVar12);
  }
  if (in_FPSCR_SZ == '\0') {
    *(int *)(param_7 + 1) = (int)uVar12;
  }
  else {
    param_7[1] = dVar11;
  }
  return;
}

