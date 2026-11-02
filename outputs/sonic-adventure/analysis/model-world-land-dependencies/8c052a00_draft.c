/* Ghidra draft only; not buildable/verified source. */

void FUN_8c052a00(void)

{
  short sVar1;
  undefined1 *puVar2;
  
  sVar1 = 0xff;
  puVar2 = (undefined1 *)(DAT_8c052b60 + 0x100);
  do {
    sVar1 = sVar1 + -2;
    puVar2[-1] = 0;
    puVar2 = puVar2 + -2;
    *puVar2 = 0;
  } while (-1 < sVar1);
  return;
}

