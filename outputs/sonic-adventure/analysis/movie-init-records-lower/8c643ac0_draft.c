/* Ghidra draft only; not buildable/verified source. */

int FUN_8c643ac0(int param_1)

{
  int iVar1;
  
  iVar1 = DAT_8c643af8;
  if ((*DAT_8c643af4 != 0) && (iVar1 = 0, param_1 != 0)) {
    iVar1 = (*(code *)PTR_FUN_8c643afc)(1);
  }
  if (iVar1 != 0) {
    (*(code *)PTR_FUN_8c643b04)(PTR_FUN_8c643b00,iVar1);
  }
  return iVar1;
}

