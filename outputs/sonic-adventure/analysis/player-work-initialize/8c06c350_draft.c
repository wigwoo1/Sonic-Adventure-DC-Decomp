/* Ghidra draft only; not buildable/verified source. */

/* WARNING: Removing unreachable block (ram,0x8c06c5b6) */
/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_8c06c350(undefined4 param_1,int param_2)

{
  char cVar1;
  code *pcVar2;
  undefined *puVar3;
  int iVar4;
  undefined4 uVar5;
  int iVar6;
  int *piVar7;
  int iVar8;
  undefined1 *puVar9;
  double dVar10;
  undefined8 in_xd2;
  undefined8 in_xd14;
  undefined4 in_PR;
  char in_FPSCR_PR;
  char in_FPSCR_SZ;
  undefined1 local_18 [4];
  undefined1 local_14 [4];
  
  if (in_FPSCR_SZ == '\0') {
    puVar9 = local_14;
  }
  else {
    puVar9 = local_18;
  }
  *(undefined4 *)(puVar9 + -4) = in_PR;
  *(undefined4 *)(puVar9 + -8) = param_1;
  piVar7 = *(int **)(param_2 + 0x24);
  iVar8 = *(int *)(param_2 + 0x20);
  iVar4 = (*(code *)PTR_FUN_8c06c49c)(1,0x200);
  (*(code *)PTR_FUN_8c06c4a4)();
  puVar3 = PTR_FUN_8c06c4a8;
  *piVar7 = iVar4;
  (*(code *)puVar3)(iVar8);
  uVar5 = (*(code *)PTR_FUN_8c06c49c)(1,0x84);
  *(undefined4 *)(iVar4 + 100) = uVar5;
  iVar6 = *(int *)(puVar9 + -8) * 4;
  *(int *)(DAT_8c06c48c + iVar6) = param_2;
  *(int *)(DAT_8c06c488 + iVar6) = iVar8;
  *(int **)(DAT_8c06c484 + iVar6) = piVar7;
  *(int *)(DAT_8c06c480 + iVar6) = iVar4;
  *(undefined2 *)(iVar4 + 0x120) = 0;
  *(undefined2 *)(iVar4 + 0x122) = 0;
  *(int *)(iVar4 + 0x138) = iVar4 + 0x38;
  *(int *)(iVar4 + 0x13c) = iVar4;
  puVar3 = PTR_FUN_8c06c4b0;
  if (in_FPSCR_SZ == '\0') {
    dVar10 = (double)CONCAT44(*(undefined4 *)(iVar4 + 0x10c),_DAT_8c06c4ac);
  }
  else {
    in_xd2 = CONCAT44(PTR_FUN_8c06c4b0,_DAT_8c06c4ac);
    dVar10 = *(double *)(iVar4 + 0x10c);
  }
  if (in_FPSCR_PR == '\0') {
    dVar10 = (double)CONCAT44((float)((ulonglong)dVar10 >> 0x20) * SUB84(dVar10,0),SUB84(dVar10,0));
  }
  else {
    dVar10 = dVar10 * dVar10;
  }
  if (in_FPSCR_SZ == '\0') {
    *(int *)(iVar4 + 0x1fc) = (int)((ulonglong)dVar10 >> 0x20);
  }
  else {
    *(double *)(iVar4 + 0x1fc) = dVar10;
  }
  cVar1 = *(char *)(iVar8 + 9);
  if (cVar1 == '\0') {
    iVar6 = (*(code *)puVar3)(0x8d);
    if (iVar6 != 0) {
      *(ushort *)(iVar4 + 4) = *(ushort *)(iVar4 + 4) | 2;
    }
    iVar6 = (*(code *)puVar3)(0x8e);
    if (iVar6 != 0) {
      *(ushort *)(iVar4 + 4) = *(ushort *)(iVar4 + 4) | 1;
    }
    iVar6 = (*(code *)puVar3)(0x9f);
    if (iVar6 != 0) {
      *(ushort *)(iVar4 + 4) = *(ushort *)(iVar4 + 4) | 4;
    }
  }
  else if (cVar1 != '\x02') {
    if (cVar1 == '\x03') {
      iVar6 = (*(code *)puVar3)(0x119);
      if (iVar6 != 0) {
        *(ushort *)(iVar4 + 4) = *(ushort *)(iVar4 + 4) | 0x40;
      }
      iVar6 = (*(code *)puVar3)(0x11a);
      if (iVar6 != 0) {
        *(ushort *)(iVar4 + 4) = *(ushort *)(iVar4 + 4) | 0x20;
      }
    }
    else if (cVar1 == '\x05') {
      iVar6 = (*(code *)puVar3)(0x152);
      if (iVar6 != 0) {
        *(ushort *)(iVar4 + 4) = *(ushort *)(iVar4 + 4) | 0x100;
      }
      iVar6 = (*(code *)puVar3)(0x15e);
      if (iVar6 != 0) {
        *(ushort *)(iVar4 + 4) = *(ushort *)(iVar4 + 4) | 0x80;
      }
    }
    else {
      if (cVar1 == '\x06') {
        iVar6 = (*(code *)puVar3)(0x189);
        if (iVar6 != 0) {
          *(ushort *)(iVar4 + 4) = *(ushort *)(iVar4 + 4) | 0x400;
        }
        iVar6 = (*(code *)puVar3)(0x18a);
        if (iVar6 != 0) {
          *(ushort *)(iVar4 + 4) = *(ushort *)(iVar4 + 4) | 0x200;
        }
      }
      else if (cVar1 != '\a') goto LAB_8c06c576;
      iVar6 = (*(code *)puVar3)(0x1d0);
      if (iVar6 != 0) {
        *(ushort *)(iVar4 + 4) = *(ushort *)(iVar4 + 4) | 0x1000;
      }
      iVar6 = (*(code *)puVar3)(0x1d1);
      if (iVar6 != 0) {
        *(ushort *)(iVar4 + 4) = *(ushort *)(iVar4 + 4) | 0x800;
      }
    }
    goto LAB_8c06c576;
  }
  iVar6 = (*(code *)puVar3)(0xcd);
  if (iVar6 != 0) {
    *(ushort *)(iVar4 + 4) = *(ushort *)(iVar4 + 4) | 8;
  }
  iVar6 = (*(code *)puVar3)(0xdb);
  if (iVar6 != 0) {
    *(ushort *)(iVar4 + 4) = *(ushort *)(iVar4 + 4) | 0x10;
  }
LAB_8c06c576:
  (*(code *)PTR_FUN_8c06c660)(0,iVar4);
  uVar5 = 0x3f800000;
  if (in_FPSCR_SZ == '\0') {
    piVar7[0xf] = 0x3f800000;
  }
  else {
    *(undefined8 *)(piVar7 + 0xf) = in_xd14;
  }
  if (*(char *)(iVar8 + 2) == '\0') {
    *(undefined1 *)(iVar8 + 2) = 1;
  }
  (*(code *)PTR_FUN_8c06c664)(iVar4);
  puVar3 = PTR_LAB_8c06c668;
  if (in_FPSCR_SZ == '\0') {
    *(undefined4 *)(iVar8 + 0x34) = uVar5;
    *(undefined4 *)(iVar8 + 0x30) = uVar5;
    pcVar2 = (code *)PTR_FUN_8c06c66c;
    *(undefined4 *)(iVar8 + 0x2c) = uVar5;
  }
  else {
    *(undefined8 *)(iVar8 + 0x34) = in_xd14;
    *(undefined8 *)(iVar8 + 0x30) = in_xd14;
    pcVar2 = (code *)PTR_FUN_8c06c66c;
    *(undefined8 *)(iVar8 + 0x2c) = in_xd14;
  }
  iVar4 = (*pcVar2)(2,puVar3);
  if (in_FPSCR_SZ == '\0') {
    *(float *)(*(int *)(iVar4 + 0x20) + 0x34) = (float)*(int *)(puVar9 + -8);
  }
  else {
    *(undefined8 *)(*(int *)(iVar4 + 0x20) + 0x34) = in_xd2;
  }
  return;
}

