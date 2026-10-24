/* Ghidra draft only; not buildable/verified source. */

void FUN_8c6049c0(uint param_1,undefined4 param_2,undefined4 param_3)

{
  undefined4 *puVar1;
  int *piVar2;
  int iVar3;
  int *piVar4;
  undefined4 uVar5;
  uint uVar6;
  int iVar7;
  undefined1 *puVar8;
  undefined4 uVar9;
  undefined8 in_dr2;
  double dVar10;
  undefined4 in_fr13;
  undefined8 in_dr14;
  double dVar11;
  undefined8 in_xd12;
  undefined8 in_xd14;
  bool bVar12;
  undefined4 in_PR;
  char in_FPSCR_PR;
  char in_FPSCR_SZ;
  undefined1 local_28 [12];
  undefined1 local_1c [4];
  undefined8 uStack_18;
  
  iVar3 = DAT_8c604ac4;
  piVar2 = DAT_8c604abc._4_4_;
  puVar1 = DAT_8c604ab8;
  uVar9 = (undefined4)((ulonglong)in_dr2 >> 0x20);
  if (in_FPSCR_SZ == '\0') {
    uStack_18 = CONCAT44((int)in_dr14,(undefined4)uStack_18);
                    /* WARNING: Ignoring partial resolution of indirect */
    uStack_18._0_4_ = (int)((ulonglong)in_dr14 >> 0x20);
    puVar8 = local_1c;
  }
  else {
    puVar8 = local_28;
    uStack_18 = in_xd14;
  }
  *(undefined4 *)(puVar8 + -4) = in_PR;
  uVar6 = (uint)DAT_8c604aa4;
  *(undefined4 *)(puVar8 + -0x10) = param_2;
  *DAT_8c604ab4 = param_2;
  *puVar1 = param_3;
  *(uint *)(puVar8 + -8) = uVar6 & param_1;
  uVar5 = (undefined4)DAT_8c604abc;
  if (in_FPSCR_SZ != '\0') {
    in_xd12 = CONCAT44(DAT_8c604abc._4_4_,(undefined4)DAT_8c604abc);
    uVar5 = in_fr13;
  }
  if ((param_1 & 0x10) == 0) {
    iVar7 = (int)DAT_8c604aa8;
    if (in_FPSCR_SZ == '\0') {
      *(undefined4 *)(iVar3 + 4) = (undefined4)DAT_8c604ac8;
    }
    else {
      *(ulonglong *)(iVar3 + 4) = CONCAT44(DAT_8c604ac8._4_4_,(undefined4)DAT_8c604ac8);
    }
  }
  else {
    iVar7 = (int)DAT_8c604aa6;
    if (in_FPSCR_SZ == '\0') {
      *(undefined4 *)(iVar3 + 4) = uVar5;
    }
    else {
      *(undefined8 *)(iVar3 + 4) = in_xd12;
    }
  }
  *piVar2 = iVar7;
  piVar4 = DAT_8c604ad0;
  if (in_FPSCR_SZ == '\0') {
    dVar11 = (double)CONCAT44(DAT_8c604ac8._4_4_,(int)in_dr14);
  }
  else {
    dVar11 = (double)CONCAT44(DAT_8c604ad0,DAT_8c604ac8._4_4_);
  }
  if (in_FPSCR_SZ == '\0') {
    dVar11 = (double)CONCAT44((int)((ulonglong)dVar11 >> 0x20),DAT_8c604ad4);
  }
  else {
    in_xd14 = CONCAT44(PTR_FUN_8c604ad8,DAT_8c604ad4);
  }
  if ((param_1 & 0x20) == 0) {
    iVar7 = (int)DAT_8c604aac;
    if (in_FPSCR_SZ == '\0') {
      *(int *)(iVar3 + 8) = (int)((ulonglong)dVar11 >> 0x20);
    }
    else {
      *(double *)(iVar3 + 8) = dVar11;
    }
  }
  else {
    iVar7 = (int)DAT_8c604aaa;
    if (in_FPSCR_SZ == '\0') {
      *(int *)(iVar3 + 8) = SUB84(dVar11,0);
    }
    else {
      *(undefined8 *)(iVar3 + 8) = in_xd14;
    }
  }
  *piVar4 = iVar7;
  if ((param_1 & 3) == 1) {
    FUN_8c604594();
    FUN_8c6045b4(0);
    iVar7 = (int)DAT_8c604aa6;
    if (in_FPSCR_SZ == '\0') {
      *(undefined4 *)(iVar3 + 4) = uVar5;
    }
    else {
      *(undefined8 *)(iVar3 + 4) = in_xd12;
    }
    *piVar2 = iVar7;
    iVar7 = (int)DAT_8c604aaa;
    if (in_FPSCR_SZ == '\0') {
      *(int *)(iVar3 + 8) = SUB84(dVar11,0);
    }
    else {
      *(undefined8 *)(iVar3 + 8) = in_xd14;
    }
    *piVar4 = iVar7;
  }
  else {
    uVar5 = (*(code *)PTR_FUN_8c604ad8)();
    *(undefined4 *)(puVar8 + -0xc) = uVar5;
    FUN_8c604594();
    iVar7 = *(int *)(puVar8 + -0xc);
    if (iVar7 == 2) {
      iVar7 = (int)DAT_8c604aae;
    }
    else if (iVar7 == 3) {
      iVar7 = (int)DAT_8c604ab0;
    }
    else if (iVar7 == 1) {
      iVar7 = (int)DAT_8c604ab2;
    }
    else {
      iVar7 = (int)DAT_8c604ab0;
    }
    FUN_8c6046c0(iVar7);
  }
  if ((param_1 & DAT_8c604adc) == 0) {
    uVar9 = 0;
  }
  else {
    if (in_FPSCR_PR == '\0') {
      uVar9 = 0x40000000;
    }
    else {
      uVar9 = SUB84((double)CONCAT44(uVar9,0x3f800000) + (double)CONCAT44(uVar9,0x3f800000),0);
    }
    if (in_FPSCR_SZ == '\0') {
      dVar10 = (double)CONCAT44(*(undefined4 *)(iVar3 + 4),uVar9);
    }
    else {
      dVar10 = *(double *)(iVar3 + 4);
    }
    if (in_FPSCR_PR == '\0') {
      dVar10 = (double)CONCAT44((float)((ulonglong)dVar10 >> 0x20) * SUB84(dVar10,0),SUB84(dVar10,0)
                               );
    }
    else {
      dVar10 = dVar10 * dVar10;
    }
    if (in_FPSCR_SZ == '\0') {
      *(int *)(iVar3 + 4) = (int)((ulonglong)dVar10 >> 0x20);
      dVar10 = (double)CONCAT44(*(undefined4 *)(iVar3 + 8),SUB84(dVar10,0));
    }
    else {
      *(double *)(iVar3 + 4) = dVar10;
      dVar10 = *(double *)(iVar3 + 8);
    }
    if (in_FPSCR_PR == '\0') {
      bVar12 = (float)((ulonglong)dVar10 >> 0x20) == (float)((ulonglong)dVar11 >> 0x20);
    }
    else {
      bVar12 = dVar10 == dVar11;
    }
    uVar9 = 1;
    if (bVar12) {
      if (in_FPSCR_SZ == '\0') {
        *(int *)(iVar3 + 8) = SUB84(dVar11,0);
      }
      else {
        *(undefined8 *)(iVar3 + 8) = in_xd14;
      }
    }
  }
  *DAT_8c604b68 = *(undefined4 *)PTR_DAT_8c604b64;
  (*(code *)PTR_FUN_8c604b6c)(*(undefined4 *)(puVar8 + -8),1,uVar9);
  (*(code *)PTR_FUN_8c604b74)(*DAT_8c604b70);
  *DAT_8c604b78 = 0;
  *DAT_8c604b7c = 0;
  *DAT_8c604b80 = 1;
  *DAT_8c604b84 = 0;
  (*(code *)PTR_FUN_8c604b8c)(PTR_FUN_8c604b88,0);
  (*(code *)PTR_FUN_8c604b94)(PTR_LAB_8c604b90,0);
  (*(code *)PTR_FUN_8c604b9c)(PTR_LAB_8c604b98);
  FUN_8c604d68();
  return;
}

