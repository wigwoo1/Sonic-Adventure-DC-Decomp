/* Ghidra draft only; not buildable/verified source. */

void callback_8c041792(void)

{
  int iVar1;
  int *piVar2;
  
  piVar2 = (int *)((char)(*DAT_8c0418e4 * '\x1c') + DAT_8c041900);
  iVar1 = *piVar2;
  piVar2[1] = iVar1;
  piVar2[2] = iVar1 + 8;
  piVar2[5] = 0;
  FUN_8c041378();
  *piVar2 = iVar1 + 0x10;
  return;
}

