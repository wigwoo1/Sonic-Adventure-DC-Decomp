/* Ghidra draft only; not buildable/verified source. */

int FUN_8c0b33a0(int param_1)

{
  int iVar1;
  
  iVar1 = (*(code *)PTR_FUN_8c0b34e0)(0x34);
  if (iVar1 != 0) {
    *(undefined4 *)(iVar1 + 0x2c) = 0;
    *(undefined4 *)(iVar1 + 0x30) = *(undefined4 *)(param_1 + 0x10);
    if (*(int *)(param_1 + 0x10) != 0) {
      *(int *)(*(int *)(param_1 + 0x10) + 0x2c) = iVar1;
    }
    *(int *)(param_1 + 0x10) = iVar1;
  }
  return iVar1;
}

