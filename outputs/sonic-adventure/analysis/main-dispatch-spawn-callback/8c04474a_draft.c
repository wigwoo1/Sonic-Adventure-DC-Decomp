/* Ghidra draft only; not buildable/verified source. */

/* WARNING: Removing unreachable block (ram,0x8c04479c) */

void FUN_8c04474a(double param_1,undefined4 param_2)

{
  undefined *puVar1;
  undefined *puVar2;
  int iVar3;
  int iVar4;
  int iVar5;
  undefined4 uVar6;
  int iVar7;
  undefined8 *puVar8;
  float fVar9;
  double dVar10;
  float fVar12;
  double dVar11;
  undefined8 in_dr12;
  double dVar14;
  undefined8 in_dr14;
  double dVar15;
  double in_xd2;
  undefined8 in_xd14;
  bool bVar16;
  undefined4 in_PR;
  byte bVar17;
  uint uVar18;
  char in_FPSCR_PR;
  char in_FPSCR_SZ;
  undefined1 auStack_3c [16];
  undefined8 uStack_2c;
  undefined8 uStack_24;
  float fVar13;
  
  iVar3 = DAT_8c04487c;
  puVar2 = PTR_FUN_8c04486c;
  puVar1 = PTR_FUN_8c044868;
  if (in_FPSCR_SZ == '\0') {
    uStack_24 = CONCAT44((int)in_dr14,(undefined4)uStack_24);
                    /* WARNING: Ignoring partial resolution of indirect */
    uStack_24._0_4_ = (int)((ulonglong)in_dr14 >> 0x20);
    puVar8 = &uStack_2c;
    uStack_2c = CONCAT44((int)in_dr12,(int)((ulonglong)in_dr12 >> 0x20));
  }
  else {
    puVar8 = (undefined8 *)auStack_3c;
    uStack_24 = in_xd14;
    uStack_2c = in_dr14;
  }
  *(undefined4 *)((int)puVar8 + -4) = in_PR;
  *(undefined4 *)((int)puVar8 + -8) = param_2;
  if (in_FPSCR_SZ == '\0') {
    dVar14 = (double)CONCAT44(DAT_8c044870,DAT_8c044874);
    dVar15 = (double)CONCAT44(DAT_8c044878,DAT_8c044880);
  }
  else {
    dVar14 = (double)CONCAT44(DAT_8c044874,DAT_8c044870);
    dVar15 = (double)CONCAT44(DAT_8c04487c,DAT_8c044878);
  }
  iVar7 = 0;
  do {
    uVar6 = SUB84(param_1,0);
    iVar4 = (*(code *)puVar2)(2,5,PTR_LAB_8c044884);
    if (iVar4 == 0) {
      return;
    }
    iVar5 = (*(code *)puVar1)();
    fVar9 = DAT_8c044888;
    param_1 = in_xd2;
    if (in_FPSCR_SZ == '\0') {
      param_1 = (double)CONCAT44((float)iVar5,uVar6);
    }
    if (in_FPSCR_PR == '\0') {
      param_1 = (double)CONCAT44((float)((ulonglong)param_1 >> 0x20) *
                                 (float)((ulonglong)dVar14 >> 0x20),SUB84(param_1,0));
    }
    else {
      param_1 = param_1 * dVar14;
    }
    uVar6 = (undefined4)((ulonglong)param_1 >> 0x20);
    dVar10 = param_1;
    if (in_FPSCR_SZ == '\0') {
      dVar10 = (double)CONCAT44(uVar6,DAT_8c044888);
    }
    if (in_FPSCR_PR == '\0') {
      dVar10 = (double)CONCAT44((float)((ulonglong)dVar10 >> 0x20) * SUB84(dVar14,0),SUB84(dVar10,0)
                               );
    }
    else {
      dVar10 = dVar10 * dVar14;
    }
    fVar13 = SUB84(dVar10,0);
    fVar12 = (float)((ulonglong)dVar10 >> 0x20);
    iVar4 = *(int *)(iVar4 + 0x20);
    if (in_FPSCR_PR != '\0' || fVar13 <= fVar12) {
      if (in_FPSCR_PR == '\0') {
        dVar10 = (double)CONCAT44(fVar12 - (fVar13 + fVar13),fVar13 + fVar13);
      }
      else {
        dVar10 = (dVar10 + dVar10) - (dVar10 + dVar10);
      }
    }
    if (in_FPSCR_PR == '\0') {
      iVar5 = (int)(float)((ulonglong)dVar10 >> 0x20);
    }
    else {
      iVar5 = (int)dVar10;
    }
    dVar11 = param_1;
    if (in_FPSCR_SZ == '\0') {
      dVar11 = (double)CONCAT44(uVar6,SUB84(dVar10,0));
    }
    if (in_FPSCR_PR == '\0') {
      fVar13 = (float)((ulonglong)dVar11 >> 0x20) * (float)((ulonglong)dVar15 >> 0x20);
    }
    else {
      fVar13 = (float)((ulonglong)(dVar11 * dVar15) >> 0x20);
    }
    dVar10 = (double)CONCAT44(fVar13,DAT_8c044888);
    bVar16 = DAT_8c044888 <= fVar13;
    *(int *)(iVar4 + 0x18) = iVar5 << 0xb;
    if (in_FPSCR_PR != '\0' || bVar16) {
      if (in_FPSCR_PR == '\0') {
        fVar9 = fVar9 + fVar9;
        dVar10 = (double)CONCAT44(fVar13 - fVar9,fVar9);
      }
      else {
        dVar10 = (dVar10 + dVar10) - (dVar10 + dVar10);
      }
    }
    fVar9 = (float)((ulonglong)dVar10 >> 0x20);
    if (in_FPSCR_PR == '\0') {
      uVar18 = (uint)fVar9;
    }
    else {
      uVar18 = (uint)dVar10;
    }
    dVar11 = param_1;
    if (in_FPSCR_SZ == '\0') {
      dVar10 = (double)CONCAT44(fVar9,uVar6);
      dVar11 = in_xd2;
    }
    if (in_FPSCR_PR == '\0') {
      dVar10 = (double)CONCAT44((int)((ulonglong)dVar10 >> 0x20),SUB84(dVar10,0) * SUB84(dVar15,0));
    }
    else {
      dVar10 = dVar10 * dVar15;
    }
    iVar7 = iVar7 + 1;
    if (in_FPSCR_PR == '\0') {
      bVar17 = (byte)(int)SUB84(dVar10,0);
    }
    else {
      bVar17 = (byte)(int)dVar10;
    }
    *(uint *)(iVar4 + 0x14) = iVar3 - (uVar18 & 0x3fff);
    *(byte *)(iVar4 + 3) = bVar17 & 1;
    iVar5 = *(int *)((int)puVar8 + -8);
    uVar6 = *(undefined4 *)(iVar5 + 0x24);
    *(undefined4 *)(iVar4 + 0x20) = *(undefined4 *)(iVar5 + 0x20);
    *(undefined4 *)(iVar4 + 0x24) = uVar6;
    *(undefined4 *)(iVar4 + 0x28) = *(undefined4 *)(iVar5 + 0x28);
    in_xd2 = dVar11;
  } while (iVar7 < 6);
  return;
}

