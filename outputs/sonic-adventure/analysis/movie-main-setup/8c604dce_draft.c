/* Ghidra draft only; not buildable/verified source. */

/* WARNING: Removing unreachable block (ram,0x8c604f76) */
/* WARNING: Removing unreachable block (ram,0x8c604f64) */
/* WARNING: Removing unreachable block (ram,0x8c604ee4) */
/* WARNING: Removing unreachable block (ram,0x8c604ea6) */
/* WARNING: Removing unreachable block (ram,0x8c604f08) */
/* WARNING: Removing unreachable block (ram,0x8c604f6c) */
/* WARNING: Removing unreachable block (ram,0x8c604f88) */
/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_8c604dce(undefined4 param_1,double param_2,undefined4 param_3,int param_4,
                 undefined4 param_5,int param_6,int param_7)

{
  int iVar1;
  undefined *puVar2;
  undefined *puVar3;
  undefined *puVar4;
  int iVar5;
  uint uVar6;
  undefined4 uVar7;
  int iVar8;
  undefined8 *puVar9;
  float fVar10;
  float fVar11;
  double dVar12;
  double dVar13;
  double dVar14;
  double dVar15;
  float fVar16;
  double in_dr14;
  undefined4 uVar17;
  double in_xd2;
  double in_xd4;
  double dVar18;
  double in_xd6;
  undefined8 in_xd14;
  undefined4 in_PR;
  char in_FPSCR_PR;
  char in_FPSCR_SZ;
  undefined8 local_24;
  
  uVar7 = DAT_8c604e54;
  puVar2 = PTR_DAT_8c604e50;
  iVar1 = DAT_8c604e4c;
  if (in_FPSCR_SZ == '\0') {
    puVar9 = (undefined8 *)((int)&local_24 + 4);
    local_24 = CONCAT44(SUB84(in_dr14,0),(undefined4)local_24);
  }
  else {
    puVar9 = &local_24;
    local_24 = in_xd14;
  }
  *(undefined4 *)((int)puVar9 + -4) = in_PR;
  *(undefined4 *)((int)puVar9 + -0xc) = param_5;
  iVar8 = *(int *)((int)puVar9 + 0x20);
  *(undefined4 *)PTR_DAT_8c604e50 = 0;
  if (param_4 < 0) {
    param_4 = 100;
    *(undefined4 *)(iVar1 + 4) = uVar7;
    *(undefined4 *)(iVar1 + 0x5c) = 0x20;
    *(undefined4 *)puVar2 = 1;
  }
  else {
    if (param_6 < 0) {
      *(undefined4 *)(iVar1 + 4) = uVar7;
      *(undefined4 *)(iVar1 + 0x5c) = 0x22;
      param_6 = 100;
    }
    else {
      if (-1 < iVar8) {
        *(undefined4 *)(iVar1 + 4) = DAT_8c604f1c;
        *(undefined4 *)(iVar1 + 0x5c) = 0;
        goto LAB_8c604e60;
      }
      iVar8 = 100;
      *(undefined4 *)(iVar1 + 4) = 1;
      *(undefined4 *)(iVar1 + 0x5c) = 0x24;
    }
    *(undefined4 *)puVar2 = 2;
  }
LAB_8c604e60:
  puVar4 = PTR_DAT_8c604f34;
  puVar2 = PTR_DAT_8c604f24;
  *(int *)PTR_DAT_8c604f20 = param_4;
  puVar3 = PTR_DAT_8c604f2c;
  *(undefined4 *)puVar2 = *(undefined4 *)((int)puVar9 + -0xc);
  puVar2 = PTR_DAT_8c604f30;
  *(int *)PTR_DAT_8c604f28 = param_6;
  *(int *)puVar3 = param_7;
  *(int *)puVar2 = iVar8;
  *(int *)puVar4 = *(int *)((int)puVar9 + -0xc) + param_4 + param_6 + param_7 + iVar8;
  if (*(int *)PTR_DAT_8c604f38 == 0) {
    iVar5 = *(int *)(iVar1 + 0x24);
    *(int *)((int)puVar9 + -8) = iVar5;
    if (iVar5 != 0) {
      (*(code *)PTR_FUN_8c604f40)(*(uint *)((int)puVar9 + -8) & DAT_8c604f3c);
    }
    fVar10 = (float)*(int *)puVar4;
    dVar14 = in_xd2;
    if (in_FPSCR_SZ == '\0') {
      dVar14 = (double)CONCAT44(fVar10,param_1);
    }
    uVar7 = (undefined4)((ulonglong)dVar14 >> 0x20);
    uVar17 = (undefined4)((ulonglong)in_dr14 >> 0x20);
    if (in_FPSCR_PR != '\0' || 0.0 <= fVar10) {
      if (in_FPSCR_SZ == '\0') {
        in_dr14 = (double)CONCAT44(uVar17,uVar7);
      }
    }
    else {
      if (in_FPSCR_SZ == '\0') {
        in_dr14 = (double)CONCAT44(uVar17,uVar7);
        fVar10 = DAT_8c604f44;
      }
      else {
        in_xd2 = (double)CONCAT44(PTR_FUN_8c604f48,DAT_8c604f44);
      }
      if (in_FPSCR_PR == '\0') {
        in_dr14 = (double)CONCAT44((int)((ulonglong)in_dr14 >> 0x20),SUB84(in_dr14,0) + fVar10);
      }
      else {
        in_dr14 = in_dr14 + (double)(ulonglong)(uint)fVar10;
      }
    }
    uVar7 = 0;
    uVar6 = (*(code *)PTR_FUN_8c604f48)(*(int *)puVar4 * 4 + (int)DAT_8c604f1a);
    *(uint *)(iVar1 + 0x24) = uVar6 | DAT_8c604f4c;
  }
  else {
    uVar7 = (undefined4)((ulonglong)in_dr14 >> 0x20);
    dVar14 = in_xd2;
    if (*(uint *)PTR_DAT_8c604f50 < (uint)(*(int *)puVar4 * 4 + (int)DAT_8c604f1a)) {
      fVar10 = (float)*(int *)puVar4;
      if (in_FPSCR_SZ == '\0') {
        dVar14 = (double)CONCAT44(fVar10,param_1);
      }
      uVar17 = (undefined4)((ulonglong)dVar14 >> 0x20);
      if (in_FPSCR_PR != '\0' || 0.0 <= fVar10) {
        if (in_FPSCR_SZ == '\0') {
          in_dr14 = (double)CONCAT44(uVar7,uVar17);
        }
      }
      else {
        if (in_FPSCR_SZ == '\0') {
          in_dr14 = (double)CONCAT44(uVar7,uVar17);
          fVar10 = DAT_8c604f44;
        }
        else {
          in_xd2 = (double)CONCAT44(PTR_FUN_8c604f48,DAT_8c604f44);
        }
        if (in_FPSCR_PR == '\0') {
          in_dr14 = (double)CONCAT44((int)((ulonglong)in_dr14 >> 0x20),SUB84(in_dr14,0) + fVar10);
        }
        else {
          in_dr14 = in_dr14 + (double)(ulonglong)(uint)fVar10;
        }
      }
      uVar7 = 0;
      *(undefined4 *)(iVar1 + 0x24) = 0;
      *(undefined4 *)(iVar1 + 0x28) = 0;
      goto LAB_8c604f5a;
    }
    fVar10 = (float)*(int *)puVar4;
    if (in_FPSCR_SZ == '\0') {
      dVar14 = (double)CONCAT44(fVar10,param_1);
    }
    uVar17 = (undefined4)((ulonglong)dVar14 >> 0x20);
    if (in_FPSCR_PR != '\0' || 0.0 <= fVar10) {
      if (in_FPSCR_SZ == '\0') {
        in_dr14 = (double)CONCAT44(uVar7,uVar17);
      }
    }
    else {
      if (in_FPSCR_SZ == '\0') {
        in_dr14 = (double)CONCAT44(uVar7,uVar17);
        fVar10 = DAT_8c604f44;
      }
      else {
        in_xd2 = (double)CONCAT44(PTR_FUN_8c604f48,DAT_8c604f44);
      }
      if (in_FPSCR_PR == '\0') {
        in_dr14 = (double)CONCAT44((int)((ulonglong)in_dr14 >> 0x20),SUB84(in_dr14,0) + fVar10);
      }
      else {
        in_dr14 = in_dr14 + (double)(ulonglong)(uint)fVar10;
      }
    }
    uVar7 = 0;
    *(int *)(iVar1 + 0x24) = *(int *)PTR_DAT_8c604f38;
  }
  *(int *)(iVar1 + 0x28) = *(int *)puVar4 + 0x58;
LAB_8c604f5a:
  if (in_FPSCR_SZ == '\0') {
    dVar14 = (double)CONCAT44(DAT_8c6051a8,SUB84(dVar14,0));
  }
  else {
    dVar14 = (double)CONCAT44(_DAT_8c6051ac,DAT_8c6051a8);
  }
  fVar10 = (float)((ulonglong)dVar14 >> 0x20);
  if (in_FPSCR_PR == '\0') {
    dVar12 = (double)CONCAT44(uVar7,(float)param_4 * fVar10);
  }
  else {
    dVar12 = (double)CONCAT44(uVar7,(float)param_4) * dVar14;
  }
  dVar15 = in_xd2;
  if (in_FPSCR_SZ == '\0') {
    dVar15 = (double)CONCAT44(SUB84(dVar12,0),param_3);
  }
  fVar16 = SUB84(in_dr14,0);
  if (in_FPSCR_PR == '\0') {
    dVar15 = (double)CONCAT44((float)((ulonglong)dVar15 >> 0x20) / fVar16,SUB84(dVar15,0));
    fVar11 = (float)*(int *)((int)puVar9 + -0xc) * fVar10;
  }
  else {
    dVar15 = dVar15 / in_dr14;
    fVar11 = SUB84((double)CONCAT44((int)((ulonglong)dVar12 >> 0x20),
                                    (float)*(int *)((int)puVar9 + -0xc)) * dVar14,0);
  }
  dVar12 = in_xd2;
  if (in_FPSCR_SZ == '\0') {
    param_2 = (double)CONCAT44((int)((ulonglong)param_2 >> 0x20),fVar11);
    dVar12 = in_xd6;
  }
  if (in_FPSCR_PR == '\0') {
    fVar11 = SUB84(param_2,0) / fVar16;
    dVar13 = (double)CONCAT44((float)param_7,(float)param_6 * fVar10);
  }
  else {
    fVar11 = SUB84(param_2 / in_dr14,0);
    dVar13 = (double)param_7 * dVar14;
  }
  if (in_FPSCR_SZ == '\0') {
    *(int *)(iVar1 + 0x2c) = (int)((ulonglong)dVar15 >> 0x20);
    dVar15 = (double)CONCAT44(SUB84(dVar13,0),fVar11);
  }
  else {
    *(double *)(iVar1 + 0x2c) = dVar15;
    dVar15 = in_xd2;
  }
  fVar11 = (float)((ulonglong)dVar13 >> 0x20);
  if (in_FPSCR_PR == '\0') {
    dVar15 = (double)CONCAT44((float)((ulonglong)dVar15 >> 0x20) / fVar16,SUB84(dVar15,0));
    dVar13 = (double)CONCAT44(fVar11 * fVar10,(float)iVar8 * fVar10);
  }
  else {
    dVar15 = dVar15 / in_dr14;
    dVar13 = (double)CONCAT44(fVar11,(float)iVar8) * dVar14 * dVar14;
  }
  dVar18 = dVar13;
  if (in_FPSCR_SZ == '\0') {
    dVar14 = (double)CONCAT44(fVar10,(int)((ulonglong)dVar13 >> 0x20));
    dVar18 = in_xd4;
  }
  if (in_FPSCR_PR == '\0') {
    fVar10 = SUB84(dVar14,0) / fVar16;
  }
  else {
    fVar10 = SUB84(dVar14 / in_dr14,0);
  }
  if (in_FPSCR_SZ == '\0') {
    *(int *)(iVar1 + 0x30) = SUB84(dVar15,0);
    in_xd2 = (double)CONCAT44(SUB84(dVar13,0),fVar10);
  }
  else {
    *(double *)(iVar1 + 0x30) = dVar12;
  }
  if (in_FPSCR_PR == '\0') {
    dVar14 = (double)CONCAT44((float)((ulonglong)in_xd2 >> 0x20) / fVar16,SUB84(in_xd2,0));
  }
  else {
    dVar14 = in_xd2 / in_dr14;
  }
  if (in_FPSCR_SZ == '\0') {
    *(int *)(iVar1 + 0x34) = (int)((ulonglong)dVar15 >> 0x20);
    *(int *)(iVar1 + 0x38) = SUB84(dVar14,0);
    *(int *)(iVar1 + 0x3c) = (int)((ulonglong)dVar14 >> 0x20);
  }
  else {
    *(double *)(iVar1 + 0x34) = dVar15;
    *(double *)(iVar1 + 0x38) = dVar18;
    *(double *)(iVar1 + 0x3c) = dVar14;
  }
  FUN_8c604594();
  (*(code *)PTR_FUN_8c6051b0)(_DAT_8c6051ac);
  uVar7 = (*(code *)PTR_FUN_8c6051b4)(0);
  *DAT_8c6051b8 = uVar7;
  return;
}

