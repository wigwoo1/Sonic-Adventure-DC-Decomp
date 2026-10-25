/* Ghidra draft only; not buildable/verified source. */

byte FUN_8c10e576(int *param_1)

{
  if (*param_1 == 1) {
    return 0;
  }
  return *(byte *)(param_1[1] + 0x10) & 0x40;
}

