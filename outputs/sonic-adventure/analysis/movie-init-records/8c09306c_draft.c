/* Ghidra draft only; not buildable/verified source. */

void FUN_8c09306c(void)

{
  int iVar1;
  
  iVar1 = 0;
  *(undefined4 *)PTR_DAT_8c093114 = 0;
  do {
    FUN_8c093094(iVar1);
    FUN_8c093094(iVar1 + 1);
    iVar1 = iVar1 + 2;
  } while (iVar1 < 0x20);
  return;
}

