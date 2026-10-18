/* Ghidra draft only; not buildable/verified source. */

void callback_8c04073c(void)

{
  int *piVar1;
  int iVar2;
  
  piVar1 = DAT_8c040858;
  iVar2 = *DAT_8c040858;
  DAT_8c040858[1] = iVar2;
  piVar1[2] = iVar2 + 4;
  piVar1[5] = 0;
  FUN_8c040498();
  *piVar1 = iVar2 + 8;
  return;
}

