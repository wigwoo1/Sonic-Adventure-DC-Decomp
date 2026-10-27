/* Ghidra draft only; not buildable/verified source. */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void callback_8c668cec(void)

{
  int iVar1;
  int *piVar2;
  
  iVar1 = 0;
  piVar2 = _DAT_8c668d84;
  do {
    if (*piVar2 != 0) {
      func_0x8c668d18(piVar2);
    }
    iVar1 = iVar1 + 1;
    piVar2 = (int *)((int)piVar2 + (int)_DAT_8c668d72);
  } while (iVar1 < 8);
  return;
}

