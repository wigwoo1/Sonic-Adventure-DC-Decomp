/* Ghidra draft only; not buildable/verified source. */

void FUN_8c60551e(int param_1)

{
  int *piVar1;
  undefined4 uVar2;
  int iVar3;
  
  piVar1 = DAT_8c6055cc;
  *(int *)*DAT_8c6055cc = (int)DAT_8c6055c0;
  iVar3 = *piVar1;
  if (param_1 == 0) {
    *(undefined4 *)(iVar3 + 0x4c) = 0;
    goto LAB_8c60555c;
  }
  if (param_1 == 1) {
    uVar2 = 1;
  }
  else {
    if (param_1 == 2) {
      *(undefined4 *)(iVar3 + 0x4c) = 2;
      goto LAB_8c60555c;
    }
    if (param_1 != 3) goto LAB_8c60555c;
    uVar2 = 3;
  }
  *(undefined4 *)(iVar3 + 0x4c) = uVar2;
LAB_8c60555c:
  (*(code *)PTR_FUN_8c6055c4)(*piVar1);
  (*(code *)PTR_FUN_8c6055c8)(*piVar1);
  return;
}

