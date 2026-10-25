/* Ghidra draft only; not buildable/verified source. */

int FUN_8c66abfc(undefined4 *param_1)

{
  int iVar1;
  
  iVar1 = DAT_8c66ac30;
  if (*DAT_8c66ac2c != 0) {
    *param_1 = *(undefined4 *)((int)DAT_8c66ac2a + *DAT_8c66ac2c);
    iVar1 = 0;
  }
  if (iVar1 != 0) {
    (*(code *)PTR_FUN_8c66ac38)(PTR_FUN_8c66ac34,iVar1);
  }
  return iVar1;
}

