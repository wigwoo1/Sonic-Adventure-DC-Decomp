/* Ghidra draft only; not buildable/verified source. */

void FUN_8c605480(int param_1)

{
  int *piVar1;
  int iVar2;
  
  piVar1 = DAT_8c6054ec;
  *(int *)*DAT_8c6054ec = (int)DAT_8c6054e8;
  iVar2 = *piVar1;
  if (param_1 == 0) {
    *(undefined4 *)(iVar2 + 0x54) = 0;
  }
  else if (param_1 == 1) {
    *(undefined4 *)(iVar2 + 0x54) = 2;
  }
  else if (param_1 == 2) {
    *(undefined4 *)(iVar2 + 0x54) = 1;
  }
  (*(code *)PTR_FUN_8c6054f4)(*piVar1);
  (*(code *)PTR_FUN_8c6054f0)(*piVar1);
  return;
}

