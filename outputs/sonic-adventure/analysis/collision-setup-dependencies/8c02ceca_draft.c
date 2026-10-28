/* Ghidra draft only; not buildable/verified source. */

void FUN_8c02ceca(undefined4 *param_1,int param_2)

{
  undefined4 *puVar1;
  
  *DAT_8c02ceec = param_1;
  param_2 = param_2 + -1;
  puVar1 = param_1;
  do {
    puVar1 = puVar1 + 0x10;
    *param_1 = puVar1;
    param_1 = param_1 + 0x10;
    param_2 = param_2 + -1;
  } while (param_2 != 0);
  *param_1 = 0;
  *(undefined4 *)PTR_DAT_8c02cef0 = 0;
  return;
}

