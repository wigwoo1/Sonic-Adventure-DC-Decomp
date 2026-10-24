/* Ghidra draft only; not buildable/verified source. */

void FUN_8c600df4(int param_1)

{
  short sVar1;
  
  if ((param_1 != 0) && (*(short *)(param_1 + 0x4c) != 0)) {
    sVar1 = *(short *)(param_1 + 0x50);
    if ((sVar1 == 2) || ((sVar1 == 3 || (sVar1 == 4)))) {
      FUN_8c6013d2(param_1);
    }
    *(undefined2 *)(param_1 + 0x4c) = 0;
  }
  return;
}

