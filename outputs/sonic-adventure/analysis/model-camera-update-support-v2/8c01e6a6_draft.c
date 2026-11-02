/* Ghidra draft only; not buildable/verified source. */

void original_entry_8c01e6a6(undefined4 param_1)

{
  undefined *puVar1;
  undefined *puVar2;
  int *piVar3;
  undefined *puVar4;
  int iVar5;
  int iVar6;
  undefined8 *puVar7;
  undefined4 extraout_fr1;
  double dVar8;
  undefined4 uVar9;
  float fVar10;
  double in_dr2;
  undefined8 in_dr12;
  double dVar11;
  undefined8 in_xd2;
  undefined4 in_PR;
  char in_FPSCR_PR;
  char in_FPSCR_SZ;
  undefined8 uStack_18;
  
  piVar3 = DAT_8c01e7c8;
  puVar2 = PTR_DAT_8c01e7c4;
  if (in_FPSCR_SZ == '\0') {
    puVar7 = (undefined8 *)((int)&uStack_18 + 4);
    uStack_18 = CONCAT44((int)((ulonglong)in_dr12 >> 0x20),(undefined4)uStack_18);
  }
  else {
    puVar7 = &uStack_18;
    uStack_18 = in_dr12;
  }
  *(undefined4 *)((int)puVar7 + -4) = in_PR;
  if (in_FPSCR_SZ == '\0') {
    uVar9 = *(undefined4 *)(*DAT_8c01e7c8 + 0x20);
    in_dr2 = (double)CONCAT44((int)((ulonglong)in_dr2 >> 0x20),uVar9);
    *(undefined4 *)(*(int *)PTR_DAT_8c01e7c4 + 0x18) = uVar9;
  }
  else {
    in_xd2 = *(undefined8 *)(*DAT_8c01e7c8 + 0x20);
    *(undefined8 *)(*(int *)PTR_DAT_8c01e7c4 + 0x18) = in_xd2;
  }
  iVar6 = *(int *)puVar2;
  if (in_FPSCR_SZ == '\0') {
    dVar11 = (double)CONCAT44(*(undefined4 *)(*piVar3 + 0x24),(int)in_dr12);
  }
  else {
    dVar11 = *(double *)(*piVar3 + 0x24);
  }
  iVar5 = (*(code *)PTR_FUN_8c01e7cc)(0);
  puVar4 = PTR_DAT_8c01e7d4;
  if (in_FPSCR_SZ == '\0') {
    dVar8 = (double)CONCAT44(*(undefined4 *)(iVar5 + 0x118),extraout_fr1);
  }
  else {
    dVar8 = *(double *)(iVar5 + 0x118);
  }
  if (in_FPSCR_PR == '\0') {
    dVar11 = (double)CONCAT44((float)((ulonglong)dVar11 >> 0x20) + (float)((ulonglong)dVar8 >> 0x20)
                              ,SUB84(dVar11,0));
  }
  else {
    dVar11 = dVar11 + dVar8;
  }
  if (in_FPSCR_SZ == '\0') {
    in_dr2 = (double)CONCAT44((int)((ulonglong)in_dr2 >> 0x20),*(undefined4 *)(*(int *)puVar2 + 8));
    puVar1 = DAT_8c01e7d0;
  }
  else {
    in_xd2 = *(undefined8 *)(*(int *)puVar2 + 8);
    puVar1 = PTR_DAT_8c01e7d4;
  }
  if (in_FPSCR_PR == '\0') {
    dVar11 = (double)CONCAT44((float)puVar1 * SUB84(in_dr2,0) + (float)((ulonglong)dVar11 >> 0x20),
                              SUB84(dVar11,0));
  }
  if (in_FPSCR_SZ == '\0') {
    *(int *)(iVar6 + 0x1c) = (int)((ulonglong)dVar11 >> 0x20);
  }
  else {
    *(double *)(iVar6 + 0x1c) = dVar11;
  }
  if (in_FPSCR_SZ == '\0') {
    *(undefined4 *)(*(int *)puVar2 + 0x20) = *(undefined4 *)(*piVar3 + 0x28);
    in_dr2 = (double)CONCAT44((int)((ulonglong)in_dr2 >> 0x20),*(undefined4 *)puVar4);
  }
  else {
    *(undefined8 *)(*(int *)puVar2 + 0x20) = *(undefined8 *)(*piVar3 + 0x28);
    in_xd2 = *(undefined8 *)puVar4;
  }
  if (in_FPSCR_SZ == '\0') {
    *(int *)(*(int *)puVar2 + 8) = SUB84(in_dr2,0);
  }
  else {
    *(undefined8 *)(*(int *)puVar2 + 8) = in_xd2;
  }
  *(undefined4 *)(*(int *)puVar2 + 0x24) = 0xfffff800;
  *(undefined4 *)(*(int *)puVar2 + 0x2c) = 0;
  iVar6 = *(int *)puVar2;
  if ((((char *)*piVar3)[9] == '\x03') && (*(char *)*piVar3 == '\r')) {
    iVar5 = (*(code *)PTR_FUN_8c01e7dc)
                      (*(undefined4 *)(iVar6 + 0x28),DAT_8c01e7d8 - *(int *)(*DAT_8c01e7c8 + 0x18));
    *(int *)(iVar6 + 0x28) = *(int *)(iVar6 + 0x28) + (iVar5 >> 2);
  }
  else if ((*(uint *)(DAT_8c01e7e8 + 8) & 0x200) == 0) {
    if ((DAT_8c01e7e4 & *(uint *)(DAT_8c01e7e8 + 8)) == 0) {
      if ((DAT_8c01e7e0 & *(uint *)(DAT_8c01e7e8 + 8)) != 0) {
        *(ushort *)(iVar6 + 6) = *(ushort *)(iVar6 + 6) | 1;
        *(int *)(*(int *)puVar2 + 0x28) = *(int *)(*(int *)puVar2 + 0x28) + 0x200;
      }
    }
    else {
      *(ushort *)(iVar6 + 6) = *(ushort *)(iVar6 + 6) | 1;
      *(int *)(*(int *)puVar2 + 0x28) = *(int *)(*(int *)puVar2 + 0x28) + -0x200;
    }
  }
  else {
    if (in_FPSCR_PR == '\0') {
      dVar11 = (double)CONCAT44(0x40000000,param_1);
    }
    else {
      dVar11 = (double)CONCAT44(0x3f800000,param_1) + (double)CONCAT44(0x3f800000,param_1);
    }
    uVar9 = (undefined4)((ulonglong)in_dr2 >> 0x20);
    fVar10 = (float)((ulonglong)dVar11 >> 0x20);
    if ((DAT_8c01e7e4 & *(uint *)(DAT_8c01e7e8 + 8)) == 0) {
      if ((DAT_8c01e7e0 & *(uint *)(DAT_8c01e7e8 + 8)) != 0) {
        if (in_FPSCR_SZ == '\0') {
          in_dr2 = (double)CONCAT44(uVar9,*(undefined4 *)puVar4);
        }
        else {
          in_xd2 = *(undefined8 *)puVar4;
        }
        if (in_FPSCR_PR == '\0') {
          fVar10 = SUB84(in_dr2,0) - fVar10;
        }
        else {
          fVar10 = SUB84(in_dr2 - dVar11,0);
        }
        if (in_FPSCR_SZ == '\0') {
          *(float *)puVar4 = fVar10;
        }
        else {
          *(undefined8 *)puVar4 = in_xd2;
        }
      }
    }
    else {
      if (in_FPSCR_SZ == '\0') {
        in_dr2 = (double)CONCAT44(uVar9,*(undefined4 *)puVar4);
      }
      else {
        in_xd2 = *(undefined8 *)puVar4;
      }
      if (in_FPSCR_PR == '\0') {
        fVar10 = SUB84(in_dr2,0) + fVar10;
      }
      else {
        fVar10 = SUB84(in_dr2 + dVar11,0);
      }
      if (in_FPSCR_SZ == '\0') {
        *(float *)puVar4 = fVar10;
      }
      else {
        *(undefined8 *)puVar4 = in_xd2;
      }
    }
  }
  return;
}

