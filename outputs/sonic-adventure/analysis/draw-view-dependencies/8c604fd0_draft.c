/* Ghidra draft only; not buildable/verified source. */

void FUN_8c604fd0(int param_1)

{
  int iVar1;
  
  iVar1 = param_1 * 0xc0 + DAT_8c6051bc;
  *DAT_8c6051c0 = iVar1;
  (*(code *)PTR_FUN_8c6051c4)(iVar1);
  *DAT_8c6051cc = param_1 * 4 + DAT_8c6051c8;
  return;
}

