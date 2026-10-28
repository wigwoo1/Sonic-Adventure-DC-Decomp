/* Ghidra draft only; not buildable/verified source. */

/* WARNING: Removing unreachable block (ram,0x8c0763e4) */
/* WARNING: Removing unreachable block (ram,0x8c0764d0) */
/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_8c0763a0(float param_1,int param_2,int param_3,int param_4,undefined4 *param_5)

{
  float fVar1;
  undefined *puVar2;
  undefined *puVar3;
  int iVar4;
  int iVar5;
  double dVar6;
  float fVar9;
  double dVar7;
  undefined8 uVar8;
  double in_dr2;
  uint uVar11;
  undefined4 uVar12;
  bool bVar13;
  undefined4 in_PR;
  char in_FPSCR_PR;
  char in_FPSCR_SZ;
  undefined4 local_88;
  undefined4 local_84;
  undefined4 local_80;
  undefined4 local_7c;
  undefined4 local_78;
  undefined4 local_74;
  undefined4 local_70;
  undefined4 local_6c;
  undefined4 local_68;
  undefined4 auStack_64 [3];
  float fStack_58;
  undefined4 uStack_54;
  undefined4 uStack_50;
  undefined4 uStack_4c;
  undefined4 uStack_48;
  undefined4 uStack_44;
  undefined4 uStack_40;
  undefined4 uStack_3c;
  undefined4 uStack_38;
  undefined4 uStack_34;
  undefined4 uStack_30;
  undefined4 uStack_2c;
  undefined4 uStack_28;
  undefined4 uStack_24;
  undefined4 uStack_20;
  undefined4 local_1c;
  float fVar10;
  
  local_1c = in_PR;
  if ((*(ushort *)(param_3 + 0x10) & 3) == 0) {
    dVar6 = (double)(*(code *)PTR_FUN_8c07642c)(param_3 + 0x38);
    if (in_FPSCR_SZ == '\0') {
      in_dr2 = (double)CONCAT44((int)((ulonglong)in_dr2 >> 0x20),*(undefined4 *)(param_3 + 0xc0));
    }
    if (in_FPSCR_PR == '\0') {
      bVar13 = SUB84(in_dr2,0) < (float)((ulonglong)dVar6 >> 0x20);
    }
    else {
      bVar13 = in_dr2 < dVar6;
    }
    if (!bVar13) {
      return;
    }
  }
  uVar11 = (uint)((ulonglong)in_dr2 >> 0x20);
  fVar9 = (float)(*(code *)PTR_FUN_8c076430)(param_5,param_4 + 0x48,&local_70);
  puVar3 = PTR_FUN_8c07642c;
  puVar2 = PTR_FUN_8c076420;
  dVar6 = (double)((ulonglong)uVar11 << 0x20);
  if (fVar9 != 0.0) {
    dVar7 = (double)(*(code *)PTR_FUN_8c07642c)(&local_70);
    if (in_FPSCR_SZ == '\0') {
      dVar6 = (double)CONCAT44((int)((ulonglong)dVar6 >> 0x20),(undefined4)DAT_8c076434);
    }
    if (in_FPSCR_PR == '\0') {
      bVar13 = SUB84(dVar6,0) < (float)((ulonglong)dVar7 >> 0x20);
    }
    else {
      bVar13 = dVar6 < dVar7;
    }
    if (bVar13) {
      (*(code *)puVar2)(&local_70);
    }
    else {
      local_70 = *(undefined4 *)(param_4 + 0x1c);
      local_6c = *(undefined4 *)(param_4 + 0x20);
      local_68 = *(undefined4 *)(param_4 + 0x24);
    }
    uVar11 = (uint)((ulonglong)dVar6 >> 0x20);
    if (in_FPSCR_SZ == '\0') {
      uStack_34 = *(undefined4 *)(param_4 + 0x54);
      uStack_30 = *(undefined4 *)(param_4 + 0x58);
      uStack_2c = *(undefined4 *)(param_4 + 0x5c);
      uStack_28 = *(undefined4 *)(param_4 + 0x48);
      uStack_24 = *(undefined4 *)(param_4 + 0x4c);
      uStack_20 = *(undefined4 *)(param_4 + 0x50);
      uStack_4c = *(undefined4 *)(param_4 + 4);
      uStack_48 = *(undefined4 *)(param_4 + 8);
      uStack_44 = *(undefined4 *)(param_4 + 0xc);
    }
    else {
      uStack_30 = (undefined4)*(undefined8 *)(param_4 + 0x58);
      uStack_2c = (undefined4)*(undefined8 *)(param_4 + 0x5c);
      uStack_28 = (undefined4)*(undefined8 *)(param_4 + 0x48);
      uStack_24 = (undefined4)*(undefined8 *)(param_4 + 0x4c);
      uStack_20 = (undefined4)*(undefined8 *)(param_4 + 0x50);
      local_1c = (undefined4)((ulonglong)*(undefined8 *)(param_4 + 0x50) >> 0x20);
      uStack_4c = (undefined4)*(undefined8 *)(param_4 + 4);
      uStack_48 = (undefined4)*(undefined8 *)(param_4 + 8);
      uStack_44 = (undefined4)*(undefined8 *)(param_4 + 0xc);
      uStack_34 = auStack_64[0];
    }
    uStack_38 = local_68;
    uStack_3c = local_6c;
    uStack_40 = local_70;
    (*(code *)PTR_FUN_8c076620)(&uStack_34,&uStack_4c,auStack_64);
    local_7c = *param_5;
    local_78 = param_5[1];
    local_74 = param_5[2];
    fVar9 = (float)(*(code *)puVar3)(&local_7c);
    if (fVar9 != 0.0) {
      (*(code *)puVar2)(&local_7c);
    }
    dVar6 = (double)(*(code *)PTR_FUN_8c076624)(&fStack_58,&local_7c);
    if (in_FPSCR_PR == '\0') {
      bVar13 = 0.0 < (float)((ulonglong)dVar6 >> 0x20);
    }
    else {
      bVar13 = (double)((ulonglong)uVar11 << 0x20) < dVar6;
    }
    if (bVar13) {
      if (in_FPSCR_SZ == '\0') {
        dVar6 = (double)((ulonglong)(uint)fStack_58 << 0x20);
      }
      else {
        dVar6 = (double)CONCAT44(uStack_54,fStack_58);
      }
      if (in_FPSCR_PR == '\0') {
        dVar6 = (double)CONCAT44(-(float)((ulonglong)dVar6 >> 0x20),SUB84(dVar6,0));
      }
      else {
        dVar6 = -dVar6;
      }
      fStack_58 = SUB84(dVar6,0);
      fVar9 = (float)((ulonglong)dVar6 >> 0x20);
      if (in_FPSCR_SZ == '\0') {
        dVar6 = (double)CONCAT44(uStack_54,fStack_58);
        fStack_58 = fVar9;
      }
      else {
        dVar6 = (double)CONCAT44(uStack_50,fVar9);
      }
      if (in_FPSCR_PR == '\0') {
        dVar6 = (double)CONCAT44(-(float)((ulonglong)dVar6 >> 0x20),SUB84(dVar6,0));
      }
      else {
        dVar6 = -dVar6;
      }
      uStack_54 = SUB84(dVar6,0);
      uVar12 = (undefined4)((ulonglong)dVar6 >> 0x20);
      if (in_FPSCR_SZ == '\0') {
        dVar6 = (double)CONCAT44(uStack_50,uStack_54);
        uStack_54 = uVar12;
      }
      else {
        dVar6 = (double)CONCAT44(uStack_4c,uVar12);
      }
      if (in_FPSCR_PR == '\0') {
        dVar6 = (double)CONCAT44(-(float)((ulonglong)dVar6 >> 0x20),SUB84(dVar6,0));
      }
      else {
        dVar6 = -dVar6;
      }
      uVar12 = (undefined4)((ulonglong)dVar6 >> 0x20);
      uStack_50 = uVar12;
      if (in_FPSCR_SZ != '\0') {
        uStack_50 = SUB84(dVar6,0);
        uStack_4c = uVar12;
      }
    }
    if ((*(ushort *)(param_3 + 0x10) & 3) == 0) {
      iVar4 = param_4 + 0x6c;
    }
    else {
      iVar4 = param_4 + 0x10;
    }
    uVar8 = (*(code *)puVar3)(iVar4);
    uVar11 = DAT_8c07662c;
    iVar4 = DAT_8c076628;
    fVar10 = (float)((ulonglong)uVar8 >> 0x20);
    fVar9 = (float)uVar8;
    fVar1 = fVar9;
    if (in_FPSCR_SZ == '\0') {
      fVar1 = fStack_58;
    }
    if (in_FPSCR_SZ == '\0') {
      fVar9 = param_1;
    }
    if (in_FPSCR_PR == '\0') {
      dVar6 = (double)CONCAT44(fVar10 * fVar1,fVar1);
    }
    else {
      dVar6 = (double)CONCAT44(fVar10,fVar1) * (double)CONCAT44(fVar10,fVar1);
    }
    if (in_FPSCR_SZ == '\0') {
      *(int *)(param_4 + 0x6c) = (int)((ulonglong)dVar6 >> 0x20);
      dVar6 = (double)CONCAT44(fVar10,uStack_54);
    }
    else {
      *(double *)(param_4 + 0x6c) = dVar6;
      dVar6 = (double)CONCAT44(fVar10,fVar9);
    }
    if (in_FPSCR_PR == '\0') {
      dVar6 = (double)CONCAT44((float)((ulonglong)dVar6 >> 0x20) * SUB84(dVar6,0),SUB84(dVar6,0));
    }
    else {
      dVar6 = dVar6 * dVar6;
    }
    uVar12 = (undefined4)((ulonglong)dVar6 >> 0x20);
    if (in_FPSCR_SZ == '\0') {
      *(undefined4 *)(param_4 + 0x70) = uVar12;
      dVar6 = (double)CONCAT44(uVar12,uStack_50);
    }
    else {
      *(double *)(param_4 + 0x70) = dVar6;
    }
    if (in_FPSCR_PR == '\0') {
      dVar7 = (double)CONCAT44(fVar10 * SUB84(dVar6,0),fVar9);
    }
    else {
      dVar7 = (double)CONCAT44(fVar10,fVar9) * dVar6;
    }
    if (in_FPSCR_SZ == '\0') {
      *(int *)(param_4 + 0x74) = (int)((ulonglong)dVar7 >> 0x20);
    }
    else {
      *(double *)(param_4 + 0x74) = dVar7;
    }
    if ((*(uint *)(param_3 + 0x60) & uVar11) != 0) {
      iVar5 = *(int *)((uint)*(byte *)(param_2 + 8) * 4 + iVar4);
      uVar8 = (*(code *)puVar3)(iVar5 + 4);
      fVar10 = (float)((ulonglong)uVar8 >> 0x20);
      fVar9 = (float)uVar8;
      fVar1 = fVar9;
      if (in_FPSCR_SZ == '\0') {
        fVar1 = fStack_58;
      }
      if (in_FPSCR_SZ == '\0') {
        fVar9 = SUB84(dVar7,0);
      }
      if (in_FPSCR_PR == '\0') {
        dVar6 = (double)CONCAT44(fVar10 * fVar1,fVar1);
      }
      else {
        dVar6 = (double)CONCAT44(fVar10,fVar1) * (double)CONCAT44(fVar10,fVar1);
      }
      if (in_FPSCR_SZ == '\0') {
        *(int *)(iVar5 + 4) = (int)((ulonglong)dVar6 >> 0x20);
        dVar6 = (double)CONCAT44(fVar10,uStack_54);
      }
      else {
        *(double *)(iVar5 + 4) = dVar6;
        dVar6 = (double)CONCAT44(fVar10,fVar9);
      }
      if (in_FPSCR_PR == '\0') {
        dVar6 = (double)CONCAT44((float)((ulonglong)dVar6 >> 0x20) * SUB84(dVar6,0),SUB84(dVar6,0));
      }
      else {
        dVar6 = dVar6 * dVar6;
      }
      uVar12 = (undefined4)((ulonglong)dVar6 >> 0x20);
      if (in_FPSCR_SZ == '\0') {
        *(undefined4 *)(iVar5 + 8) = uVar12;
        dVar6 = (double)CONCAT44(uVar12,uStack_50);
      }
      else {
        *(double *)(iVar5 + 8) = dVar6;
      }
      if (in_FPSCR_PR == '\0') {
        dVar7 = (double)CONCAT44(fVar10 * SUB84(dVar6,0),fVar9);
      }
      else {
        dVar7 = (double)CONCAT44(fVar10,fVar9) * dVar6;
      }
      if (in_FPSCR_SZ == '\0') {
        *(int *)(iVar5 + 0xc) = (int)((ulonglong)dVar7 >> 0x20);
      }
      else {
        *(double *)(iVar5 + 0xc) = dVar7;
      }
    }
    local_7c = *(undefined4 *)(param_4 + 0x10);
    local_78 = *(undefined4 *)(param_4 + 0x14);
    local_74 = *(undefined4 *)(param_4 + 0x18);
    dVar7 = (double)(*(code *)puVar3)(&local_7c);
    if (in_FPSCR_SZ == '\0') {
      dVar6 = (double)CONCAT44((int)((ulonglong)dVar6 >> 0x20),_DAT_8c076630);
    }
    if (in_FPSCR_PR == '\0') {
      bVar13 = SUB84(dVar6,0) < (float)((ulonglong)dVar7 >> 0x20);
    }
    else {
      bVar13 = dVar6 < dVar7;
    }
    if (bVar13) {
      local_88 = *(undefined4 *)(param_4 + 0x6c);
      local_84 = *(undefined4 *)(param_4 + 0x70);
      local_80 = *(undefined4 *)(param_4 + 0x74);
      (*(code *)puVar2)(&local_7c);
      (*(code *)puVar2)(&local_88);
      iVar5 = (*(code *)PTR_FUN_8c076634)(&local_7c,&local_88,param_4 + 0x1c);
      *(int *)(param_2 + 0x18) = *(int *)(param_2 + 0x18) - iVar5;
    }
    iVar5 = (*(code *)PTR_FUN_8c076638)
                      (*(undefined4 *)(*(int *)((uint)*(byte *)(param_2 + 8) * 4 + iVar4) + 0x20),
                       *(undefined4 *)(param_2 + 0x18));
    if (0x6000 < iVar5) {
      *(int *)(param_2 + 0x18) = *(int *)(param_2 + 0x18) + DAT_8c07663c;
    }
    *(undefined4 *)(*(int *)((uint)*(byte *)(param_2 + 8) * 4 + iVar4) + 0x20) =
         *(undefined4 *)(param_2 + 0x18);
  }
  return;
}

