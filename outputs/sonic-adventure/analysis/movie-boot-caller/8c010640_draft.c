/* Ghidra draft only; not buildable/verified source. */

void FUN_8c010640(void)

{
  undefined *puVar1;
  undefined8 *puVar2;
  undefined8 *puVar3;
  undefined *puVar4;
  int iVar5;
  int iVar6;
  int iVar7;
  int iVar8;
  int iVar9;
  char in_FPSCR_SZ;
  
  puVar3 = DAT_8c010888;
  puVar2 = DAT_8c010884;
  puVar1 = PTR_DAT_8c010880;
  if (in_FPSCR_SZ == '\0') {
    *(undefined4 *)(*(int *)PTR_DAT_8c010880 + 0x20) = *(undefined4 *)DAT_8c010884;
  }
  else {
    *(undefined8 *)(*(int *)PTR_DAT_8c010880 + 0x20) = *DAT_8c010884;
  }
  if (in_FPSCR_SZ == '\0') {
    *(undefined4 *)(*(int *)puVar1 + 0x24) = *(undefined4 *)puVar3;
  }
  else {
    *(undefined8 *)(*(int *)puVar1 + 0x24) = *puVar3;
  }
  if (in_FPSCR_SZ == '\0') {
    *(undefined4 *)(*(int *)puVar1 + 0x28) = *(undefined4 *)DAT_8c01088c;
  }
  else {
    *(undefined8 *)(*(int *)puVar1 + 0x28) = *DAT_8c01088c;
  }
  *(undefined4 *)(*(int *)puVar1 + 0x14) = *(undefined4 *)((int)puVar2 + 0xc);
  *(undefined4 *)(*(int *)puVar1 + 0x18) = *DAT_8c010890;
  puVar4 = PTR_FUN_8c010898;
  *(undefined4 *)(*(int *)puVar1 + 0x1c) = *DAT_8c010894;
  iVar9 = 0;
  do {
    iVar6 = (*(code *)puVar4)(iVar9);
    if (iVar6 != 0) {
      iVar6 = *(int *)(iVar6 + 0x20);
      iVar8 = iVar9 * 0x18;
      if (in_FPSCR_SZ == '\0') {
        *(undefined4 *)(iVar6 + 0x20) = *(undefined4 *)(DAT_8c01089c + iVar8);
        *(undefined4 *)(iVar6 + 0x24) = *(undefined4 *)(DAT_8c0108a0 + iVar8);
        *(undefined4 *)(iVar6 + 0x28) = *(undefined4 *)(DAT_8c0108a4 + iVar8);
      }
      else {
        *(undefined8 *)(iVar6 + 0x20) = *(undefined8 *)(DAT_8c01089c + iVar8);
        *(undefined8 *)(iVar6 + 0x24) = *(undefined8 *)(DAT_8c0108a0 + iVar8);
        *(undefined8 *)(iVar6 + 0x28) = *(undefined8 *)(DAT_8c0108a4 + iVar8);
      }
      iVar5 = DAT_8c0108a8;
      iVar7 = DAT_8c0108a8 + 4;
      *(undefined4 *)(iVar6 + 0x14) = *(undefined4 *)(DAT_8c0108a8 + iVar8);
      *(undefined4 *)(iVar6 + 0x18) = *(undefined4 *)(iVar7 + iVar8);
      *(undefined4 *)(iVar6 + 0x1c) = *(undefined4 *)(iVar5 + 8 + iVar8);
    }
    iVar6 = (*(code *)puVar4)(iVar9 + 1);
    if (iVar6 != 0) {
      iVar6 = *(int *)(iVar6 + 0x20);
      iVar8 = (iVar9 + 1) * 0x18;
      if (in_FPSCR_SZ == '\0') {
        *(undefined4 *)(iVar6 + 0x20) = *(undefined4 *)(DAT_8c01089c + iVar8);
        *(undefined4 *)(iVar6 + 0x24) = *(undefined4 *)(DAT_8c0108a0 + iVar8);
        *(undefined4 *)(iVar6 + 0x28) = *(undefined4 *)(DAT_8c0108a4 + iVar8);
      }
      else {
        *(undefined8 *)(iVar6 + 0x20) = *(undefined8 *)(DAT_8c01089c + iVar8);
        *(undefined8 *)(iVar6 + 0x24) = *(undefined8 *)(DAT_8c0108a0 + iVar8);
        *(undefined8 *)(iVar6 + 0x28) = *(undefined8 *)(DAT_8c0108a4 + iVar8);
      }
      iVar5 = DAT_8c0108a8;
      iVar7 = DAT_8c0108a8 + 4;
      *(undefined4 *)(iVar6 + 0x14) = *(undefined4 *)(DAT_8c0108a8 + iVar8);
      *(undefined4 *)(iVar6 + 0x18) = *(undefined4 *)(iVar7 + iVar8);
      *(undefined4 *)(iVar6 + 0x1c) = *(undefined4 *)(iVar5 + 8 + iVar8);
    }
    iVar6 = (*(code *)puVar4)(iVar9 + 2);
    if (iVar6 != 0) {
      iVar6 = *(int *)(iVar6 + 0x20);
      iVar8 = (iVar9 + 2) * 0x18;
      if (in_FPSCR_SZ == '\0') {
        *(undefined4 *)(iVar6 + 0x20) = *(undefined4 *)(DAT_8c01089c + iVar8);
        *(undefined4 *)(iVar6 + 0x24) = *(undefined4 *)(DAT_8c0108a0 + iVar8);
        *(undefined4 *)(iVar6 + 0x28) = *(undefined4 *)(DAT_8c0108a4 + iVar8);
      }
      else {
        *(undefined8 *)(iVar6 + 0x20) = *(undefined8 *)(DAT_8c01089c + iVar8);
        *(undefined8 *)(iVar6 + 0x24) = *(undefined8 *)(DAT_8c0108a0 + iVar8);
        *(undefined8 *)(iVar6 + 0x28) = *(undefined8 *)(DAT_8c0108a4 + iVar8);
      }
      iVar5 = DAT_8c0108a8;
      iVar7 = DAT_8c0108a8 + 4;
      *(undefined4 *)(iVar6 + 0x14) = *(undefined4 *)(DAT_8c0108a8 + iVar8);
      *(undefined4 *)(iVar6 + 0x18) = *(undefined4 *)(iVar7 + iVar8);
      *(undefined4 *)(iVar6 + 0x1c) = *(undefined4 *)(iVar5 + 8 + iVar8);
    }
    iVar6 = (*(code *)puVar4)(iVar9 + 3);
    if (iVar6 != 0) {
      iVar6 = *(int *)(iVar6 + 0x20);
      iVar8 = (iVar9 + 3) * 0x18;
      if (in_FPSCR_SZ == '\0') {
        *(undefined4 *)(iVar6 + 0x20) = *(undefined4 *)(DAT_8c01089c + iVar8);
        *(undefined4 *)(iVar6 + 0x24) = *(undefined4 *)(DAT_8c0108a0 + iVar8);
        *(undefined4 *)(iVar6 + 0x28) = *(undefined4 *)(DAT_8c0108a4 + iVar8);
      }
      else {
        *(undefined8 *)(iVar6 + 0x20) = *(undefined8 *)(DAT_8c01089c + iVar8);
        *(undefined8 *)(iVar6 + 0x24) = *(undefined8 *)(DAT_8c0108a0 + iVar8);
        *(undefined8 *)(iVar6 + 0x28) = *(undefined8 *)(DAT_8c0108a4 + iVar8);
      }
      iVar5 = DAT_8c0108a8;
      iVar7 = DAT_8c0108a8 + 4;
      *(undefined4 *)(iVar6 + 0x14) = *(undefined4 *)(DAT_8c0108a8 + iVar8);
      *(undefined4 *)(iVar6 + 0x18) = *(undefined4 *)(iVar7 + iVar8);
      *(undefined4 *)(iVar6 + 0x1c) = *(undefined4 *)(iVar5 + 8 + iVar8);
    }
    iVar9 = iVar9 + 4;
  } while (iVar9 < 8);
  return;
}

