/* Ghidra draft only; not buildable/verified source. */

undefined4 FUN_8c6541e0(int param_1,int param_2)

{
  if (param_2 <= param_1) {
    param_1 = param_2;
  }
  if (param_1 == 1) {
    return 0;
  }
  if (param_1 == 2) {
    return 1;
  }
  if (param_1 == 4) {
    return 2;
  }
  if (param_1 != 8) {
    if (param_1 == 0x10) {
      return 4;
    }
    if (param_1 == 0x20) {
      return 5;
    }
    if (param_1 == 0x40) {
      return 6;
    }
    if (param_1 == DAT_8c654240) {
      return 7;
    }
    if (param_1 == DAT_8c654242) {
      return 8;
    }
    if (param_1 == DAT_8c654244) {
      return 9;
    }
    if (param_1 == DAT_8c654246) {
      return 10;
    }
  }
  return 3;
}

