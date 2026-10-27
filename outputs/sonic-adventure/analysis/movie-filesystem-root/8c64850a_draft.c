/* Ghidra draft only; not buildable/verified source. */

int FUN_8c64850a(int param_1)

{
  int iVar1;
  
  iVar1 = param_1 * DAT_8c6485ae + DAT_8c6485c8;
  if (*(char *)(iVar1 + 2) == '\0') {
    return (int)DAT_8c6485b0;
  }
  *(undefined1 *)(iVar1 + 9) = 0;
  *(undefined1 *)(iVar1 + 8) = 0;
  *(undefined1 *)(iVar1 + 10) = 0;
  *(undefined1 *)(iVar1 + 0xb) = 0;
  *(undefined1 *)(iVar1 + 0xc) = 0;
  *(undefined4 *)(iVar1 + 4) = 0;
  return 0;
}

