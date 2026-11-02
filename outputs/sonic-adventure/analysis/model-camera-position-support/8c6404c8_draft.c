/* Ghidra draft only; not buildable/verified source. */

void FUN_8c6404c8(int param_1,int param_2)

{
  if (param_1 == 0) {
    param_1 = *DAT_8c6404dc;
  }
  *(int *)(param_1 + 0x58) = *(int *)(param_1 + 0x74) + param_2;
  return;
}

