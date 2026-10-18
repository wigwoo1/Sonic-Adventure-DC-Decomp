/* Ghidra draft only; not buildable/verified source. */

void FUN_8c07cb54(undefined4 param_1,undefined4 param_2,undefined4 param_3,undefined4 param_4,
                 uint param_5)

{
  int iVar1;
  undefined8 in_xd4;
  char in_FPSCR_SZ;
  
  iVar1 = *(int *)(DAT_8c07cca4 + (param_5 & 0xff) * 4);
  if (in_FPSCR_SZ == '\0') {
    *(undefined4 *)(iVar1 + 0x10) = param_1;
    *(undefined4 *)(iVar1 + 0x14) = param_2;
    *(undefined4 *)(iVar1 + 0x18) = param_3;
  }
  else {
    *(ulonglong *)(iVar1 + 0x10) = CONCAT44(param_1,param_2);
    *(undefined8 *)(iVar1 + 0x14) = in_xd4;
    *(ulonglong *)(iVar1 + 0x18) = CONCAT44(param_3,param_4);
  }
  return;
}

