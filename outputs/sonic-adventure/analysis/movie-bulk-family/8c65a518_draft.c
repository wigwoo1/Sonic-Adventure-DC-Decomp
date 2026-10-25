/* Ghidra draft only; not buildable/verified source. */

int FUN_8c65a518(int param_1)

{
  int iVar1;
  int iVar2;
  
  iVar1 = 0;
  iVar2 = DAT_8c65a55c;
  if ((param_1 != 0) && (iVar2 = iVar1, *(int *)(param_1 + 0x10) != 0)) {
    if (*(int *)(param_1 + 0x10) == -1) {
      *(undefined4 *)(param_1 + 0x18) = 1;
    }
    else {
      (**(code **)(param_1 + 0x10))(*(undefined4 *)(param_1 + 0x14));
    }
  }
  if (iVar2 != 0) {
    (*(code *)PTR_FUN_8c65a564)(PTR_FUN_8c65a560,iVar2);
  }
  return iVar2;
}

