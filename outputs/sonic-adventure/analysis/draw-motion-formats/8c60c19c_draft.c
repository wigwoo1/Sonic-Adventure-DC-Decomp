/* Ghidra draft only; not buildable/verified source. */

/* WARNING: Removing unreachable block (ram,0x8c60c2e6) */
/* WARNING: Removing unreachable block (ram,0x8c60c2be) */
/* WARNING: Removing unreachable block (ram,0x8c60c28a) */
/* WARNING: Removing unreachable block (ram,0x8c60c1fe) */
/* WARNING: Removing unreachable block (ram,0x8c60c21c) */
/* WARNING: Removing unreachable block (ram,0x8c60c29a) */
/* WARNING: Removing unreachable block (ram,0x8c60c2c8) */
/* WARNING: Removing unreachable block (ram,0x8c60c2f6) */
/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void callback_8c60c19c(undefined4 param_1,double param_2,undefined4 param_3,undefined4 param_4,
                      int param_5,int param_6,int *param_7)

{
  int iVar1;
  int iVar2;
  int *piVar3;
  int *piVar4;
  undefined8 *puVar5;
  double extraout_dr0;
  double dVar6;
  double dVar7;
  float fVar8;
  double dVar9;
  undefined4 uVar11;
  double dVar10;
  float fVar12;
  uint uVar13;
  float fVar14;
  float fVar15;
  float fVar16;
  double dVar17;
  double dVar18;
  double dVar19;
  undefined4 uVar20;
  undefined8 in_dr12;
  float fVar21;
  double in_xd0;
  double in_xd2;
  double dVar22;
  double in_xd4;
  double in_xd6;
  double in_xd8;
  double in_xd10;
  undefined4 in_PR;
  char in_FPSCR_PR;
  char in_FPSCR_SZ;
  undefined4 auStack_24 [3];
  undefined8 uStack_18;
  
  if (in_FPSCR_SZ == '\0') {
    puVar5 = (undefined8 *)((int)&uStack_18 + 4);
    uStack_18 = CONCAT44((int)((ulonglong)in_dr12 >> 0x20),(undefined4)uStack_18);
  }
  else {
    puVar5 = &uStack_18;
    uStack_18 = in_dr12;
  }
  *(undefined4 *)((int)puVar5 + -4) = in_PR;
  (*(code *)PTR_FUN_8c60c454)();
  uVar20 = (undefined4)in_dr12;
  iVar1 = (*(code *)PTR_FUN_8c60c458)();
  iVar2 = iVar1 * 0x10 + param_5;
  piVar3 = (int *)(param_6 * 0x10 + param_5);
  iVar1 = (*(code *)PTR_FUN_8c60c458)();
  piVar4 = (int *)(iVar1 * 0x10 + param_5);
  iVar1 = (*(code *)PTR_FUN_8c60c458)();
  param_5 = param_5 + iVar1 * 0x10;
  fVar8 = (float)(*piVar4 - *piVar3);
  dVar9 = (double)(ulonglong)(uint)fVar8;
  dVar19 = in_xd2;
  if (in_FPSCR_SZ == '\0') {
    dVar19 = (double)CONCAT44(fVar8,param_1);
  }
  uVar13 = (uint)((ulonglong)dVar19 >> 0x20);
  dVar6 = extraout_dr0;
  dVar22 = dVar19;
  if (in_FPSCR_PR != '\0' || 0.0 <= fVar8) {
    if (in_FPSCR_SZ == '\0') {
      dVar9 = (double)(ulonglong)uVar13;
      dVar22 = in_xd2;
    }
  }
  else {
    if (in_FPSCR_SZ == '\0') {
      dVar9 = (double)(ulonglong)uVar13;
      dVar6 = (double)CONCAT44((int)((ulonglong)extraout_dr0 >> 0x20),DAT_8c60c45c);
      dVar22 = in_xd2;
    }
    else {
      in_xd0 = (double)CONCAT44(DAT_8c60c460,DAT_8c60c45c);
    }
    if (in_FPSCR_PR == '\0') {
      dVar9 = (double)(ulonglong)(uint)(SUB84(dVar9,0) + SUB84(dVar6,0));
    }
    else {
      dVar9 = dVar9 + dVar6;
    }
  }
  if (in_FPSCR_SZ == '\0') {
    *(int *)((int)puVar5 + -8) = SUB84(dVar9,0);
  }
  else {
    *(double *)((int)puVar5 + -8) = dVar22;
  }
  iVar1 = *piVar3;
  fVar8 = (float)iVar1;
  dVar9 = (double)CONCAT44((int)((ulonglong)dVar9 >> 0x20),fVar8);
  if (iVar1 < 0) {
    if (in_FPSCR_PR == '\0') {
      dVar9 = (double)CONCAT44(DAT_8c60c45c,fVar8 + DAT_8c60c45c);
    }
    else {
      dVar9 = (double)CONCAT44(DAT_8c60c45c,fVar8) + (double)CONCAT44(DAT_8c60c45c,fVar8);
    }
  }
  if (in_FPSCR_SZ == '\0') {
    dVar6 = (double)CONCAT44((int)((ulonglong)dVar6 >> 0x20),*(undefined4 *)DAT_8c60c460);
  }
  else {
    in_xd0 = *DAT_8c60c460;
  }
  if (in_FPSCR_PR == '\0') {
    fVar8 = SUB84(dVar6,0) - SUB84(dVar9,0);
  }
  else {
    fVar8 = SUB84(dVar6 - dVar9,0);
  }
  iVar1 = (uint)(0 < *piVar4 - iVar1) * 4;
  if (in_FPSCR_SZ == '\0') {
    dVar9 = (double)CONCAT44((int)((ulonglong)dVar9 >> 0x20),
                             *(undefined4 *)((int)puVar5 + iVar1 + -0xc));
    in_xd0 = (double)CONCAT44(fVar8,SUB84(dVar19,0));
  }
  else {
    dVar22 = *(double *)((int)puVar5 + iVar1 + -0xc);
  }
  if (in_FPSCR_PR == '\0') {
    in_xd0 = (double)CONCAT44((float)((ulonglong)in_xd0 >> 0x20) / SUB84(dVar9,0),SUB84(in_xd0,0));
  }
  else {
    in_xd0 = in_xd0 / dVar9;
  }
  uVar11 = (undefined4)((ulonglong)dVar9 >> 0x20);
  dVar9 = (double)CONCAT44(uVar11,0x3f800000);
  if (in_FPSCR_PR == '\0') {
    dVar9 = (double)CONCAT44(uVar11,0x40000000);
  }
  else {
    dVar9 = dVar9 + dVar9;
  }
  if (in_FPSCR_SZ == '\0') {
    dVar19 = (double)CONCAT44(DAT_8c60c464,param_4);
    uVar11 = (undefined4)((ulonglong)in_xd0 >> 0x20);
    in_xd0 = (double)CONCAT44(uVar11,uVar11);
  }
  else {
    dVar19 = (double)CONCAT44(DAT_8c60c468,DAT_8c60c464);
    in_xd4 = in_xd0;
  }
  if (in_FPSCR_PR == '\0') {
    fVar14 = (float)((ulonglong)in_xd0 >> 0x20);
    in_xd0 = (double)CONCAT44(fVar14,SUB84(in_xd0,0) * fVar14);
  }
  else {
    in_xd0 = in_xd0 * in_xd0;
  }
  fVar16 = (float)((ulonglong)in_xd0 >> 0x20);
  fVar14 = SUB84(in_xd0,0);
  dVar17 = in_xd4;
  dVar6 = in_xd0;
  if (in_FPSCR_SZ == '\0') {
    param_2 = (double)CONCAT44((int)((ulonglong)param_2 >> 0x20),fVar16);
    dVar17 = (double)CONCAT44(fVar14,param_3);
    dVar6 = in_xd6;
  }
  if (in_FPSCR_PR == '\0') {
    dVar17 = (double)CONCAT44((float)((ulonglong)dVar17 >> 0x20) * SUB84(dVar9,0),SUB84(dVar17,0));
    param_2 = (double)CONCAT44((int)((ulonglong)param_2 >> 0x20),SUB84(param_2,0) * fVar14);
  }
  else {
    dVar17 = dVar17 * dVar9;
    param_2 = param_2 * in_xd0;
  }
  uVar11 = (undefined4)((ulonglong)dVar17 >> 0x20);
  dVar10 = dVar17;
  if (in_FPSCR_SZ == '\0') {
    dVar10 = (double)CONCAT44(uVar11,SUB84(dVar9,0));
  }
  if (in_FPSCR_PR == '\0') {
    dVar10 = (double)CONCAT44((float)((ulonglong)dVar10 >> 0x20) - fVar16,SUB84(dVar10,0));
  }
  else {
    dVar10 = dVar10 - in_xd0;
  }
  fVar15 = SUB84(param_2,0);
  dVar9 = dVar6;
  if (in_FPSCR_SZ == '\0') {
    dVar19 = (double)CONCAT44((int)((ulonglong)dVar19 >> 0x20),fVar15);
    dVar9 = in_xd10;
  }
  if (in_FPSCR_PR == '\0') {
    dVar10 = (double)CONCAT44((float)((ulonglong)dVar10 >> 0x20) - fVar15,SUB84(dVar10,0));
  }
  else {
    dVar10 = dVar10 - param_2;
  }
  dVar7 = dVar10;
  if (in_FPSCR_SZ == '\0') {
    dVar17 = (double)CONCAT44(uVar11,(int)((ulonglong)dVar10 >> 0x20));
    dVar7 = in_xd8;
  }
  fVar21 = (float)((ulonglong)dVar19 >> 0x20);
  if (in_FPSCR_PR == '\0') {
    dVar17 = (double)CONCAT44((int)((ulonglong)dVar17 >> 0x20),SUB84(dVar17,0) * fVar21);
  }
  else {
    dVar17 = dVar17 * dVar19;
  }
  if (in_FPSCR_SZ == '\0') {
    dVar10 = (double)CONCAT44(DAT_8c60c468,SUB84(dVar10,0));
    fVar8 = DAT_8c60c46c;
  }
  else {
    dVar10 = (double)CONCAT44(DAT_8c60c46c,DAT_8c60c468);
  }
  fVar12 = (float)((ulonglong)dVar10 >> 0x20);
  if (in_FPSCR_PR == '\0') {
    dVar19 = (double)CONCAT44(fVar21,SUB84(dVar19,0) * fVar12);
  }
  else {
    dVar19 = dVar19 * dVar10;
  }
  if (in_FPSCR_SZ == '\0') {
    in_xd4 = (double)CONCAT44(fVar14,fVar8);
  }
  fVar8 = (float)((ulonglong)dVar19 >> 0x20);
  if (in_FPSCR_PR == '\0') {
    in_xd4 = (double)CONCAT44((float)((ulonglong)in_xd4 >> 0x20) * SUB84(in_xd4,0),SUB84(in_xd4,0));
    in_xd0 = (double)CONCAT44(fVar16 * fVar8,fVar14);
  }
  else {
    in_xd4 = in_xd4 * in_xd4;
    in_xd0 = in_xd0 * dVar19;
  }
  if (in_FPSCR_SZ == '\0') {
    dVar10 = (double)CONCAT44(fVar12,SUB84(dVar19,0));
    dVar9 = dVar22;
  }
  if (in_FPSCR_PR == '\0') {
    fVar15 = fVar15 - SUB84(in_xd0,0);
  }
  else {
    fVar15 = SUB84(param_2 - in_xd0,0);
  }
  if (in_FPSCR_PR == '\0') {
    dVar10 = (double)CONCAT44((int)((ulonglong)dVar10 >> 0x20),
                              SUB84(dVar10,0) - (float)((ulonglong)in_xd4 >> 0x20));
    in_xd0 = (double)CONCAT44((float)((ulonglong)in_xd0 >> 0x20) - SUB84(dVar19,0),SUB84(in_xd0,0));
  }
  else {
    dVar10 = dVar10 - in_xd4;
    in_xd0 = in_xd0 - dVar19;
  }
  dVar22 = dVar9;
  if (in_FPSCR_SZ == '\0') {
    dVar22 = (double)CONCAT44(SUB84(dVar10,0),fVar15);
  }
  if (in_FPSCR_PR == '\0') {
    dVar22 = (double)CONCAT44((float)((ulonglong)dVar22 >> 0x20) + 1.0,SUB84(dVar22,0));
  }
  else {
    dVar22 = dVar22 + (double)CONCAT44(0x3f800000,SUB84(in_xd4,0));
  }
  fVar14 = (float)((ulonglong)dVar17 >> 0x20);
  dVar18 = in_xd0;
  if (in_FPSCR_SZ == '\0') {
    dVar10 = (double)CONCAT44((int)((ulonglong)dVar10 >> 0x20),fVar14);
    fVar14 = (float)((ulonglong)dVar9 >> 0x20);
    dVar18 = (double)CONCAT44((int)((ulonglong)in_xd0 >> 0x20),SUB84(dVar17,0));
  }
  if (in_FPSCR_PR == '\0') {
    dVar18 = (double)CONCAT44((float)((ulonglong)dVar18 >> 0x20) + SUB84(dVar10,0),SUB84(dVar18,0));
  }
  else {
    dVar18 = dVar18 + dVar10;
  }
  fVar16 = (float)((ulonglong)dVar22 >> 0x20);
  dVar9 = dVar22;
  if (in_FPSCR_SZ == '\0') {
    dVar9 = (double)CONCAT44(fVar16,uVar20);
  }
  if (in_FPSCR_PR == '\0') {
    dVar17 = (double)CONCAT44((float)piVar4[1],(float)*(int *)(param_5 + 4));
  }
  else {
    dVar17 = (double)piVar4[1];
  }
  if (in_FPSCR_SZ == '\0') {
    dVar6 = (double)CONCAT44(SUB84(dVar22,0),SUB84(in_xd0,0));
  }
  if (in_FPSCR_PR == '\0') {
    fVar8 = (float)((ulonglong)dVar6 >> 0x20) * fVar8;
  }
  else {
    fVar8 = (float)((ulonglong)(dVar6 * dVar19) >> 0x20);
  }
  if (in_FPSCR_PR == '\0') {
    fVar15 = (float)piVar3[1];
    dVar19 = (double)CONCAT44(fVar15,(float)*(int *)(iVar2 + 4));
    dVar9 = (double)CONCAT44((float)((ulonglong)dVar9 >> 0x20) * fVar15,SUB84(dVar9,0));
  }
  else {
    dVar19 = (double)piVar3[1];
    dVar9 = dVar9 * dVar19;
  }
  uVar20 = SUB84(dVar18,0);
  dVar6 = dVar7;
  if (in_FPSCR_SZ == '\0') {
    dVar6 = (double)CONCAT44(uVar20,SUB84(dVar19,0));
  }
  fVar15 = (float)((ulonglong)dVar18 >> 0x20);
  if (in_FPSCR_PR == '\0') {
    fVar21 = fVar8 * SUB84(dVar17,0) +
             fVar15 * (float)((ulonglong)dVar17 >> 0x20) +
             (float)((ulonglong)dVar6 >> 0x20) * SUB84(dVar6,0) + (float)((ulonglong)dVar9 >> 0x20);
    dVar9 = (double)CONCAT44(fVar21,SUB84(dVar9,0));
    iVar1 = (int)fVar21;
  }
  else {
    iVar1 = (int)dVar9;
  }
  dVar19 = dVar22;
  if (in_FPSCR_SZ == '\0') {
    dVar19 = (double)CONCAT44(fVar16,SUB84(dVar9,0));
  }
  *param_7 = iVar1;
  if (in_FPSCR_PR == '\0') {
    dVar9 = (double)CONCAT44((float)piVar4[2],(float)*(int *)(param_5 + 8));
  }
  else {
    dVar9 = (double)piVar4[2];
  }
  if (in_FPSCR_PR == '\0') {
    fVar21 = (float)piVar3[2];
    dVar6 = (double)CONCAT44(fVar21,(float)*(int *)(iVar2 + 8));
    dVar19 = (double)CONCAT44((float)((ulonglong)dVar19 >> 0x20) * fVar21,SUB84(dVar19,0));
  }
  else {
    dVar6 = (double)piVar3[2];
    dVar19 = dVar19 * dVar6;
  }
  dVar17 = dVar7;
  if (in_FPSCR_SZ == '\0') {
    dVar17 = (double)CONCAT44(uVar20,SUB84(dVar6,0));
  }
  if (in_FPSCR_PR == '\0') {
    iVar1 = (int)(fVar8 * SUB84(dVar9,0) +
                 fVar15 * (float)((ulonglong)dVar9 >> 0x20) +
                 (float)((ulonglong)dVar17 >> 0x20) * SUB84(dVar17,0) +
                 (float)((ulonglong)dVar19 >> 0x20));
  }
  else {
    iVar1 = (int)dVar19;
  }
  param_7[1] = iVar1;
  if (in_FPSCR_SZ == '\0') {
    fVar14 = (float)*(int *)(param_5 + 0xc);
  }
  if (in_FPSCR_PR == '\0') {
    dVar9 = (double)CONCAT44((float)piVar4[3],(float)*(int *)(param_5 + 0xc));
  }
  else {
    dVar9 = (double)piVar4[3];
  }
  if (in_FPSCR_PR == '\0') {
    fVar21 = (float)piVar3[3];
    dVar19 = (double)CONCAT44(fVar21,(float)*(int *)(iVar2 + 0xc));
    fVar16 = fVar16 * fVar21;
  }
  else {
    dVar19 = (double)piVar3[3];
    fVar16 = (float)((ulonglong)(dVar22 * dVar19) >> 0x20);
  }
  if (in_FPSCR_SZ == '\0') {
    dVar7 = (double)CONCAT44(uVar20,SUB84(dVar19,0));
  }
  if (in_FPSCR_PR == '\0') {
    fVar16 = (float)((ulonglong)dVar7 >> 0x20) * SUB84(dVar7,0) + fVar16;
  }
  if (in_FPSCR_SZ == '\0') {
    dVar18 = (double)((ulonglong)(uint)fVar15 << 0x20);
    dVar9 = (double)CONCAT44((int)((ulonglong)dVar9 >> 0x20),fVar16);
  }
  if (in_FPSCR_PR == '\0') {
    iVar1 = (int)(fVar8 * fVar14 +
                 (float)((ulonglong)dVar18 >> 0x20) * (float)((ulonglong)dVar9 >> 0x20) +
                 SUB84(dVar9,0));
  }
  else {
    iVar1 = (int)dVar9;
  }
  param_7[2] = iVar1;
  return;
}

