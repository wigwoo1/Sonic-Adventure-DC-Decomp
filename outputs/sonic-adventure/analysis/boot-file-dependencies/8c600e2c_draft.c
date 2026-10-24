/* Ghidra draft only; not buildable/verified source. */

undefined4 FUN_8c600e2c(int param_1,undefined4 *param_2)

{
  if ((param_1 != 0) && (*(short *)(param_1 + 0x4c) != 0)) {
    *param_2 = *(undefined4 *)(param_1 + 0xc);
    return 1;
  }
  return 0;
}

