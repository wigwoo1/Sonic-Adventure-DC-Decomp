/* Ghidra draft only; not buildable/verified source. */

undefined4 FUN_8c06cebe(int param_1)

{
  int iVar1;
  
  if (((*(ushort *)(param_1 + 4) & 0x4000) == 0) &&
     ((*(uint *)((uint)*(byte *)(param_1 + 8) * 0x34 + DAT_8c06cf50) & *(uint *)PTR_DAT_8c06cf4c) !=
      0)) {
    iVar1 = (*(code *)PTR_FUN_8c06cf54)();
    if (0 < iVar1) {
      return 2;
    }
    return 1;
  }
  return 0;
}

