/* Ghidra draft only; not buildable/verified source. */

undefined4 FUN_8c6041d4(int param_1,undefined4 param_2)

{
  undefined *puVar1;
  int iVar2;
  
  puVar1 = PTR_DAT_8c604320;
  iVar2 = *(int *)PTR_DAT_8c604320;
  if (iVar2 == 0) {
    return 0xfffffff5;
  }
  if (*(int *)(iVar2 + 0x1c) != 0) {
    return 0xfffffff7;
  }
  if ((-1 < param_1) && (param_1 < *(int *)(iVar2 + 0x14))) {
    *(undefined4 *)(iVar2 + 0x1c) = 1;
    *(undefined4 *)(*(int *)puVar1 + 0x20) = 2;
    *(int *)(*(int *)puVar1 + 0x28) = param_1;
    *(undefined4 *)(*(int *)puVar1 + 0x30) = param_2;
    return 0;
  }
  return 0xfffffffb;
}

