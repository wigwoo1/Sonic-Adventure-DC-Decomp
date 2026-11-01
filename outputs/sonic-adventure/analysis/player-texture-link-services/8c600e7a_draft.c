/* Ghidra draft only; not buildable/verified source. */

undefined4 FUN_8c600e7a(int param_1,int param_2,int param_3)

{
  if ((param_1 == 0) || (*(short *)(param_1 + 0x4c) == 0)) {
    return 0xfffffff6;
  }
  if (param_3 != 0) {
    if (param_3 == 1) {
      param_2 = *(int *)(param_1 + 0x14) + param_2;
    }
    else {
      if (param_3 != 2) {
        return 0xfffffff0;
      }
      param_2 = param_2 + *(int *)(param_1 + 0x10);
    }
  }
  if ((-1 < param_2) && (param_2 <= *(int *)(param_1 + 0x10))) {
    *(int *)(param_1 + 0x14) = param_2;
    return 0;
  }
  return 0xffffffef;
}

