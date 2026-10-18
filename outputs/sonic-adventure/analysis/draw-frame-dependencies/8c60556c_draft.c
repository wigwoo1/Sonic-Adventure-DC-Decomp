/* Ghidra draft only; not buildable/verified source. */

void FUN_8c60556c(int param_1)

{
  int *piVar1;
  undefined4 uVar2;
  int iVar3;
  
  piVar1 = DAT_8c6055cc;
  *(int *)*DAT_8c6055cc = (int)DAT_8c6055c2;
  iVar3 = *piVar1;
  if (param_1 == 0) {
    *(undefined4 *)(iVar3 + 0x50) = 0;
    goto LAB_8c6055aa;
  }
  if (param_1 == 1) {
    uVar2 = 1;
  }
  else {
    if (param_1 == 2) {
      *(undefined4 *)(iVar3 + 0x50) = 2;
      goto LAB_8c6055aa;
    }
    if (param_1 != 3) goto LAB_8c6055aa;
    uVar2 = 3;
  }
  *(undefined4 *)(iVar3 + 0x50) = uVar2;
LAB_8c6055aa:
  (*(code *)PTR_FUN_8c6055c4)(*piVar1);
  (*(code *)PTR_FUN_8c6055c8)(*piVar1);
  return;
}

