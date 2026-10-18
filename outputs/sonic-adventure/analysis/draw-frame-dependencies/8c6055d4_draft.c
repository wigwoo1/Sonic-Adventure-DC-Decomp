/* Ghidra draft only; not buildable/verified source. */

void FUN_8c6055d4(int param_1)

{
  int *piVar1;
  int iVar2;
  
  piVar1 = DAT_8c60561c;
  *(undefined4 *)*DAT_8c60561c = 2;
  iVar2 = *piVar1;
  if (param_1 == 0) {
    *(undefined4 *)(iVar2 + 0x18) = 0;
  }
  else if (param_1 == 1) {
    *(undefined4 *)(iVar2 + 0x18) = 1;
  }
  else if (param_1 == 2) {
    *(undefined4 *)(iVar2 + 0x18) = 2;
  }
  else if (param_1 == 3) {
    *(undefined4 *)(iVar2 + 0x18) = 3;
  }
  (*(code *)PTR_FUN_8c605620)(*piVar1);
  (*(code *)PTR_FUN_8c605624)(*piVar1);
  return;
}

