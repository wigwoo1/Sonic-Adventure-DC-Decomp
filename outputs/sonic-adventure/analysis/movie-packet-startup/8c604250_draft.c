/* Ghidra draft only; not buildable/verified source. */

undefined4 FUN_8c604250(void)

{
  undefined *puVar1;
  int iVar2;
  int iVar3;
  
  puVar1 = PTR_DAT_8c604320;
  iVar3 = *(int *)PTR_DAT_8c604320;
  if (iVar3 == 0) {
    return 0;
  }
  iVar2 = *(int *)(iVar3 + 0x1c);
  if (iVar2 == 0) goto LAB_8c6042c6;
  if (iVar2 == 1) {
    iVar3 = FUN_8c603f8c();
    *(undefined4 *)(*(int *)puVar1 + 0x1c) = 0;
    if (iVar3 == 0) {
      *(undefined4 *)(*(int *)puVar1 + 0x20) = 1;
    }
    else {
      *(undefined4 *)(*(int *)puVar1 + 0x20) = 3;
    }
  }
  else {
    if (iVar2 != 2) {
      *(undefined4 *)(iVar3 + 0x1c) = 0;
      *(undefined4 *)(*(int *)puVar1 + 0x20) = 0;
      goto LAB_8c6042c6;
    }
    iVar3 = FUN_8c603fe4();
    if (iVar3 == 1) goto LAB_8c6042c6;
    *(undefined4 *)(*(int *)puVar1 + 0x1c) = 0;
    if (iVar3 == 0) {
      *(undefined4 *)(*(int *)puVar1 + 0x20) = 1;
    }
    else {
      *(undefined4 *)(*(int *)puVar1 + 0x20) = 3;
    }
  }
  *(int *)(*(int *)puVar1 + 0x2c) = iVar3;
LAB_8c6042c6:
  return *(undefined4 *)(*(int *)puVar1 + 0x20);
}

