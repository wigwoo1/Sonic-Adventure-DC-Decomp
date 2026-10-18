/* Ghidra draft only; not buildable/verified source. */

int FUN_0c9044a0(int param_1,int param_2,int param_3)

{
  *(int *)(param_3 * 4 + param_2) = param_1;
  param_3 = param_3 + 1;
  if (*(int *)(param_1 + 0x2c) != 0) {
    param_3 = FUN_0c9044a0(*(undefined4 *)(param_1 + 0x2c),param_2,param_3);
  }
  if (*(int *)(param_1 + 0x30) != 0) {
    param_3 = FUN_0c9044a0(*(undefined4 *)(param_1 + 0x30),param_2,param_3);
  }
  return param_3;
}

