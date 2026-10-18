/* Ghidra draft only; not buildable/verified source. */

undefined4 FUN_8c608d14(int param_1)

{
  int *piVar1;
  uint uVar2;
  int iVar3;
  int *piVar4;
  
  piVar1 = DAT_8c608e48;
  piVar4 = *(int **)(param_1 * 0xc + *(int *)*DAT_8c608e3c + 8);
  if ((*piVar4 != *(int *)PTR_DAT_8c608e40) || (*(int *)PTR_DAT_8c608e44 != piVar4[1])) {
    *(int *)PTR_DAT_8c608e40 = *piVar4;
    iVar3 = (int)DAT_8c608e36;
    *(int *)PTR_DAT_8c608e44 = piVar4[1];
    uVar2 = DAT_8c608e50;
    *(uint *)((int)DAT_8c608e36 + *piVar1) =
         *(uint *)(iVar3 + *piVar1) & 0xffffffc0 | piVar4[2] & 0x3fU;
    if (uVar2 < (DAT_8c608e4c & piVar4[3])) {
      return 0xffffffff;
    }
    *(int *)((int)DAT_8c608e38 + *piVar1) = piVar4[3];
    (*(code *)PTR_FUN_8c608e54)(*piVar1);
  }
  return 1;
}

