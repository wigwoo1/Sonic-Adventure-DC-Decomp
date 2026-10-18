/* Ghidra draft only; not buildable/verified source. */

/* WARNING: Removing unreachable block (ram,0x8c609e0a) */
/* WARNING: Removing unreachable block (ram,0x8c609dc6) */
/* WARNING: Removing unreachable block (ram,0x8c609d9e) */
/* WARNING: Removing unreachable block (ram,0x8c609df0) */
/* WARNING: Removing unreachable block (ram,0x8c609e20) */
/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined8 callback_8c609d68(undefined4 param_1,int param_2,int param_3,int *param_4)

{
  int iVar1;
  int *piVar2;
  int *piVar3;
  double extraout_dr0;
  double dVar4;
  float fVar5;
  double dVar6;
  undefined8 uVar7;
  float fVar9;
  double in_xd0;
  undefined8 in_xd2;
  undefined8 in_xd4;
  undefined4 in_PR;
  char in_FPSCR_PR;
  char in_FPSCR_SZ;
  undefined4 uStack_14;
  undefined8 uStack_10;
  uint uVar8;
  
  uStack_10._4_4_ = in_PR;
  (*(code *)PTR_FUN_8c609eb0)();
  piVar3 = (int *)(param_3 * 0x10 + param_2);
  iVar1 = (*(code *)PTR_FUN_8c609eb4)();
  piVar2 = (int *)(param_2 + iVar1 * 0x10);
  fVar5 = (float)(*piVar2 - *piVar3);
  dVar6 = (double)(ulonglong)(uint)fVar5;
  uVar7 = in_xd2;
  if (in_FPSCR_SZ == '\0') {
    uVar7 = CONCAT44(fVar5,param_1);
  }
  uVar8 = (uint)((ulonglong)uVar7 >> 0x20);
  dVar4 = extraout_dr0;
  if (in_FPSCR_PR != '\0' || 0.0 <= fVar5) {
    if (in_FPSCR_SZ == '\0') {
      dVar6 = (double)(ulonglong)uVar8;
      uVar7 = in_xd2;
    }
  }
  else {
    if (in_FPSCR_SZ == '\0') {
      dVar6 = (double)(ulonglong)uVar8;
      dVar4 = (double)CONCAT44((int)((ulonglong)extraout_dr0 >> 0x20),DAT_8c609eb8);
      uVar7 = in_xd2;
    }
    else {
      in_xd0 = (double)CONCAT44(_DAT_8c609ebc,DAT_8c609eb8);
    }
    if (in_FPSCR_PR == '\0') {
      dVar6 = (double)(ulonglong)(uint)(SUB84(dVar6,0) + SUB84(dVar4,0));
    }
    else {
      dVar6 = dVar6 + dVar4;
    }
  }
  if (in_FPSCR_SZ == '\0') {
    uStack_10 = CONCAT44(uStack_10._4_4_,SUB84(dVar6,0));
  }
  else {
    uStack_10 = uVar7;
  }
  iVar1 = *piVar3;
  fVar5 = (float)iVar1;
  dVar6 = (double)CONCAT44((int)((ulonglong)dVar6 >> 0x20),fVar5);
  if (iVar1 < 0) {
    if (in_FPSCR_PR == '\0') {
      dVar6 = (double)CONCAT44(DAT_8c609eb8,fVar5 + DAT_8c609eb8);
    }
    else {
      dVar6 = (double)CONCAT44(DAT_8c609eb8,fVar5) + (double)CONCAT44(DAT_8c609eb8,fVar5);
    }
  }
  if (in_FPSCR_SZ == '\0') {
    dVar4 = (double)CONCAT44((int)((ulonglong)dVar4 >> 0x20),
                             *(undefined4 *)(DAT_8c609ec0 + (uint)*_DAT_8c609ebc * 0x1c));
  }
  else {
    in_xd0 = *(double *)(DAT_8c609ec0 + (uint)*_DAT_8c609ebc * 0x1c);
  }
  if (in_FPSCR_PR == '\0') {
    fVar5 = SUB84(dVar4,0) - SUB84(dVar6,0);
  }
  else {
    fVar5 = SUB84(dVar4 - dVar6,0);
  }
  if (in_FPSCR_SZ == '\0') {
    in_xd0 = (double)CONCAT44(fVar5,0x3f800000);
    dVar6 = (double)CONCAT44((int)((ulonglong)dVar6 >> 0x20),(&uStack_14)[0 < *piVar2 - iVar1]);
  }
  if (in_FPSCR_PR == '\0') {
    in_xd0 = (double)CONCAT44((float)((ulonglong)in_xd0 >> 0x20) / SUB84(dVar6,0),SUB84(in_xd0,0));
  }
  else {
    in_xd0 = in_xd0 / dVar6;
  }
  if (in_FPSCR_PR == '\0') {
    fVar9 = (float)((ulonglong)in_xd0 >> 0x20);
    in_xd0 = (double)CONCAT44(fVar9,SUB84(in_xd0,0) - fVar9);
    dVar6 = (double)CONCAT44((float)piVar2[1] * fVar9,(float)piVar3[1]);
  }
  else {
    in_xd0 = in_xd0 - in_xd0;
    fVar9 = (float)((ulonglong)in_xd0 >> 0x20);
    dVar6 = (double)piVar2[1] * in_xd0;
  }
  if (in_FPSCR_SZ == '\0') {
    in_xd4 = CONCAT44(SUB84(in_xd0,0),fVar5);
  }
  fVar5 = (float)((ulonglong)in_xd4 >> 0x20);
  if (in_FPSCR_PR == '\0') {
    iVar1 = (int)(fVar5 * SUB84(dVar6,0) + (float)((ulonglong)dVar6 >> 0x20));
  }
  else {
    iVar1 = (int)dVar6;
  }
  *param_4 = iVar1;
  if (in_FPSCR_PR == '\0') {
    iVar1 = (int)(fVar5 * (float)piVar3[2] + (float)piVar2[2] * fVar9);
  }
  else {
    iVar1 = (int)((double)piVar2[2] * in_xd0);
  }
  param_4[1] = iVar1;
  if (in_FPSCR_PR == '\0') {
    iVar1 = (int)(fVar5 * (float)piVar3[3] + (float)piVar2[3] * fVar9);
  }
  else {
    iVar1 = (int)((double)piVar2[3] * in_xd0);
  }
  param_4[2] = iVar1;
  return in_xd4;
}

