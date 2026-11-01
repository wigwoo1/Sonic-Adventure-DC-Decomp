/* Ghidra draft only; not buildable/verified source. */

/* WARNING: Removing unreachable block (ram,0x8c0729c8) */
/* WARNING: Removing unreachable block (ram,0x8c072a76) */

void FUN_8c072946(undefined4 param_1,int param_2,int param_3,int param_4)

{
  ushort uVar1;
  undefined *puVar2;
  undefined *puVar3;
  undefined4 uVar4;
  int iVar5;
  undefined1 *puVar6;
  double dVar7;
  float fVar9;
  undefined8 uVar8;
  undefined8 in_dr2;
  uint uVar11;
  double dVar10;
  undefined4 uVar12;
  undefined8 in_xd14;
  bool bVar13;
  undefined4 in_PR;
  char in_FPSCR_PR;
  char in_FPSCR_SZ;
  undefined1 local_1c [4];
  undefined1 local_18 [4];
  
  puVar2 = PTR_FUN_8c0729f0;
  uVar4 = (undefined4)((ulonglong)in_dr2 >> 0x20);
  if (in_FPSCR_SZ == '\0') {
    puVar6 = local_18;
  }
  else {
    puVar6 = local_1c;
  }
  *(undefined4 *)(puVar6 + -4) = in_PR;
  if ((*(ushort *)(param_2 + 4) & 3) != 0) {
    dVar7 = (double)(*(code *)PTR_FUN_8c0729f0)(param_3 + 4);
    dVar10 = (double)CONCAT44(uVar4,0x3f800000);
    if (in_FPSCR_PR == '\0') {
      bVar13 = (float)((ulonglong)dVar7 >> 0x20) < 1.0;
    }
    else {
      bVar13 = dVar7 < dVar10;
    }
    if ((((!bVar13) && (uVar1 = *(ushort *)(*(int *)(param_2 + 0x38) + 2), (uVar1 & 0x80) != 0)) &&
        ((*(ushort *)(param_2 + 4) & 4) == 0)) && ((uVar1 & 8) != 0)) {
      (*(code *)PTR_FUN_8c0729f4)(param_2,puVar6 + -0x10);
      uVar11 = (uint)((ulonglong)dVar10 >> 0x20);
      uVar4 = *(undefined4 *)(puVar6 + -0xc);
      *(undefined4 *)(param_4 + 0x44) = *(undefined4 *)(puVar6 + -0x10);
      puVar3 = PTR_FUN_8c0729f8;
      uVar12 = 0;
      *(undefined4 *)(param_4 + 0x48) = uVar4;
      *(undefined4 *)(param_4 + 0x4c) = *(undefined4 *)(puVar6 + -8);
      fVar9 = (float)(*(code *)puVar2)(puVar6 + -0x10);
      if (fVar9 == 0.0) {
        if (in_FPSCR_SZ == '\0') {
          *(undefined4 *)(puVar6 + -0x10) = uVar12;
          *(undefined4 *)(puVar6 + -0xc) = uVar12;
          *(undefined4 *)(puVar6 + -8) = uVar12;
        }
        else {
          *(undefined8 *)(puVar6 + -0x10) = in_xd14;
          *(undefined8 *)(puVar6 + -0xc) = in_xd14;
          *(undefined8 *)(puVar6 + -8) = in_xd14;
        }
      }
      else {
        (*(code *)puVar3)(puVar6 + -0x10);
      }
      if (in_FPSCR_SZ == '\0') {
        *(undefined4 *)(puVar6 + -0x28) = uVar12;
        *(undefined4 *)(puVar6 + -0x24) = uVar12;
        *(undefined4 *)(puVar6 + -0x20) = uVar12;
        *(undefined4 *)(puVar6 + -0x1c) = *(undefined4 *)(param_4 + 0x44);
        *(undefined4 *)(puVar6 + -0x18) = *(undefined4 *)(param_4 + 0x48);
        *(undefined4 *)(puVar6 + -0x14) = *(undefined4 *)(param_4 + 0x4c);
        *(undefined4 *)(puVar6 + -0x40) = uVar12;
        *(undefined4 *)(puVar6 + -0x3c) = uVar12;
        *(undefined4 *)(puVar6 + -0x38) = uVar12;
        *(undefined4 *)(puVar6 + -0x34) = *(undefined4 *)(param_4 + 0x50);
        *(undefined4 *)(puVar6 + -0x30) = *(undefined4 *)(param_4 + 0x54);
        *(undefined4 *)(puVar6 + -0x2c) = *(undefined4 *)(param_4 + 0x58);
      }
      else {
        *(undefined8 *)(puVar6 + -0x28) = in_xd14;
        *(undefined8 *)(puVar6 + -0x24) = in_xd14;
        *(undefined8 *)(puVar6 + -0x20) = in_xd14;
        *(undefined8 *)(puVar6 + -0x1c) = *(undefined8 *)(param_4 + 0x44);
        *(undefined8 *)(puVar6 + -0x18) = *(undefined8 *)(param_4 + 0x48);
        *(undefined8 *)(puVar6 + -0x14) = *(undefined8 *)(param_4 + 0x4c);
        *(undefined8 *)(puVar6 + -0x40) = in_xd14;
        *(undefined8 *)(puVar6 + -0x3c) = in_xd14;
        *(undefined8 *)(puVar6 + -0x38) = in_xd14;
        *(undefined8 *)(puVar6 + -0x34) = *(undefined8 *)(param_4 + 0x50);
        *(undefined8 *)(puVar6 + -0x30) = *(undefined8 *)(param_4 + 0x54);
        *(undefined8 *)(puVar6 + -0x2c) = *(undefined8 *)(param_4 + 0x58);
      }
      (*(code *)PTR_FUN_8c072c60)(puVar6 + -0x28,puVar6 + -0x40,puVar6 + -0x58);
      uVar4 = *(undefined4 *)(param_3 + 8);
      *(undefined4 *)(puVar6 + -100) = *(undefined4 *)(param_3 + 4);
      *(undefined4 *)(puVar6 + -0x60) = uVar4;
      *(undefined4 *)(puVar6 + -0x5c) = *(undefined4 *)(param_3 + 0xc);
      fVar9 = (float)(*(code *)puVar2)(puVar6 + -100);
      if (fVar9 != 0.0) {
        (*(code *)puVar3)(puVar6 + -100);
      }
      dVar7 = (double)(*(code *)PTR_FUN_8c072c64)(puVar6 + -0x4c,puVar6 + -100);
      dVar10 = (double)((ulonglong)uVar11 << 0x20);
      if (in_FPSCR_PR == '\0') {
        bVar13 = (float)((ulonglong)dVar7 >> 0x20) < 0.0;
      }
      else {
        bVar13 = dVar7 < dVar10;
      }
      if (bVar13) {
        if (in_FPSCR_SZ == '\0') {
          dVar7 = (double)((ulonglong)*(uint *)(puVar6 + -0x4c) << 0x20);
        }
        else {
          dVar7 = *(double *)(puVar6 + -0x4c);
        }
        if (in_FPSCR_PR == '\0') {
          dVar7 = (double)CONCAT44(-(float)((ulonglong)dVar7 >> 0x20),SUB84(dVar7,0));
        }
        else {
          dVar7 = -dVar7;
        }
        if (in_FPSCR_SZ == '\0') {
          *(int *)(puVar6 + -0x4c) = (int)((ulonglong)dVar7 >> 0x20);
          dVar7 = (double)CONCAT44(*(undefined4 *)(puVar6 + -0x48),SUB84(dVar7,0));
        }
        else {
          *(double *)(puVar6 + -0x4c) = dVar7;
          dVar7 = *(double *)(puVar6 + -0x48);
        }
        if (in_FPSCR_PR == '\0') {
          dVar7 = (double)CONCAT44(-(float)((ulonglong)dVar7 >> 0x20),SUB84(dVar7,0));
        }
        else {
          dVar7 = -dVar7;
        }
        if (in_FPSCR_SZ == '\0') {
          *(int *)(puVar6 + -0x48) = (int)((ulonglong)dVar7 >> 0x20);
          dVar10 = (double)CONCAT44(*(undefined4 *)(puVar6 + -0x44),SUB84(dVar7,0));
        }
        else {
          *(double *)(puVar6 + -0x48) = dVar7;
          dVar10 = *(double *)(puVar6 + -0x44);
        }
        if (in_FPSCR_PR == '\0') {
          dVar10 = (double)CONCAT44(-(float)((ulonglong)dVar10 >> 0x20),SUB84(dVar10,0));
        }
        else {
          dVar10 = -dVar10;
        }
        if (in_FPSCR_SZ == '\0') {
          *(int *)(puVar6 + -0x44) = (int)((ulonglong)dVar10 >> 0x20);
        }
        else {
          *(double *)(puVar6 + -0x44) = dVar10;
        }
      }
      uVar12 = SUB84(dVar10,0);
      uVar8 = (*(code *)puVar2)(param_3 + 4);
      fVar9 = (float)((ulonglong)uVar8 >> 0x20);
      uVar4 = (undefined4)uVar8;
      if (in_FPSCR_SZ == '\0') {
        uVar4 = param_1;
      }
      dVar10 = (double)CONCAT44(fVar9,uVar4);
      dVar7 = dVar10;
      if (in_FPSCR_SZ == '\0') {
        dVar7 = (double)CONCAT44(fVar9,uVar12);
      }
      uVar12 = *(undefined4 *)(param_3 + 8);
      *(undefined4 *)(puVar6 + -100) = *(undefined4 *)(param_3 + 4);
      *(undefined4 *)(puVar6 + -0x60) = uVar12;
      *(undefined4 *)(puVar6 + -0x5c) = *(undefined4 *)(param_3 + 0xc);
      if (in_FPSCR_SZ == '\0') {
        dVar7 = (double)CONCAT44((int)((ulonglong)dVar7 >> 0x20),*(undefined4 *)(puVar6 + -0x4c));
      }
      if (in_FPSCR_PR == '\0') {
        dVar7 = (double)CONCAT44((float)((ulonglong)dVar7 >> 0x20) * SUB84(dVar7,0),SUB84(dVar7,0));
      }
      else {
        dVar7 = dVar7 * dVar7;
      }
      if (in_FPSCR_SZ == '\0') {
        *(int *)(param_3 + 4) = (int)((ulonglong)dVar7 >> 0x20);
        dVar7 = (double)CONCAT44(fVar9,*(undefined4 *)(puVar6 + -0x48));
      }
      else {
        *(double *)(param_3 + 4) = dVar7;
        dVar7 = dVar10;
      }
      if (in_FPSCR_PR == '\0') {
        dVar7 = (double)CONCAT44((float)((ulonglong)dVar7 >> 0x20) * SUB84(dVar7,0),SUB84(dVar7,0));
      }
      else {
        dVar7 = dVar7 * dVar7;
      }
      uVar12 = (undefined4)((ulonglong)dVar7 >> 0x20);
      if (in_FPSCR_SZ == '\0') {
        *(undefined4 *)(param_3 + 8) = uVar12;
        dVar7 = (double)CONCAT44(uVar12,*(undefined4 *)(puVar6 + -0x44));
      }
      else {
        *(double *)(param_3 + 8) = dVar7;
      }
      if (in_FPSCR_PR == '\0') {
        dVar10 = (double)CONCAT44(fVar9 * SUB84(dVar7,0),uVar4);
      }
      else {
        dVar10 = dVar10 * dVar7;
      }
      if (in_FPSCR_SZ == '\0') {
        *(int *)(param_3 + 0xc) = (int)((ulonglong)dVar10 >> 0x20);
      }
      else {
        *(double *)(param_3 + 0xc) = dVar10;
      }
      uVar4 = *(undefined4 *)(param_3 + 8);
      *(undefined4 *)(puVar6 + -0x70) = *(undefined4 *)(param_3 + 4);
      *(undefined4 *)(puVar6 + -0x6c) = uVar4;
      *(undefined4 *)(puVar6 + -0x68) = *(undefined4 *)(param_3 + 0xc);
      (*(code *)puVar3)(puVar6 + -100);
      (*(code *)puVar3)(puVar6 + -0x70);
      iVar5 = (*(code *)PTR_FUN_8c072c68)(puVar6 + -100,puVar6 + -0x70,param_4 + 0x50);
      puVar2 = PTR_FUN_8c072c6c;
      iVar5 = *(int *)(param_2 + 0x18) - iVar5;
      *(int *)(param_2 + 0x18) = iVar5;
      iVar5 = (*(code *)puVar2)(*(undefined4 *)(param_3 + 0x20),iVar5);
      if (0x6000 < iVar5) {
        *(int *)(param_2 + 0x18) = *(int *)(param_2 + 0x18) + DAT_8c072c70;
      }
      *(undefined4 *)(param_3 + 0x20) = *(undefined4 *)(param_2 + 0x18);
    }
  }
  return;
}

