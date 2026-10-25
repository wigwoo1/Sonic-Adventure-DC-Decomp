/* Ghidra draft only; not buildable/verified source. */

int FUN_8c65a568(int param_1,undefined4 *param_2)

{
  int iVar1;
  
  iVar1 = DAT_8c65a59c;
  if ((param_1 != 0) && (iVar1 = DAT_8c65a5a0, param_2 != (undefined4 *)0x0)) {
    *param_2 = *(undefined4 *)(param_1 + 4);
    iVar1 = 0;
  }
  if (iVar1 != 0) {
    (*(code *)PTR_FUN_8c65a5a8)(PTR_FUN_8c65a5a4,iVar1);
  }
  return iVar1;
}

