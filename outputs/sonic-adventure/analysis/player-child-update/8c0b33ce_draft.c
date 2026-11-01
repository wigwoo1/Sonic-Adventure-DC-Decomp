/* Ghidra draft only; not buildable/verified source. */

void FUN_8c0b33ce(int param_1,int param_2)

{
  int iVar1;
  int iVar2;
  
  iVar2 = *(int *)(param_2 + 0x2c);
  iVar1 = *(int *)(param_2 + 0x30);
  if (iVar2 == 0) {
    *(int *)(param_1 + 0x10) = iVar1;
  }
  else {
    *(int *)(iVar2 + 0x30) = iVar1;
  }
  if (iVar1 != 0) {
    *(int *)(iVar1 + 0x2c) = iVar2;
  }
  (*(code *)PTR_FUN_8c0b34e4)(param_2);
  return;
}

