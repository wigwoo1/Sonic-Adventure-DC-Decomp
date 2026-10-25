/* Ghidra draft only; not buildable/verified source. */

undefined4 FUN_8c65a5b4(int param_1,undefined4 param_2,undefined4 param_3)

{
  undefined4 uVar1;
  
  uVar1 = DAT_8c65a5c8;
  if (param_1 != 0) {
    *(undefined4 *)(param_1 + 0x10) = param_2;
    *(undefined4 *)(param_1 + 0x14) = param_3;
    uVar1 = 0;
  }
  return uVar1;
}

