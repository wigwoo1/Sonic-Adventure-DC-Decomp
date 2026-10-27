/* Ghidra draft only; not buildable/verified source. */

undefined4 FUN_8c608c82(int param_1)

{
  uint *puVar1;
  int iVar2;
  undefined4 uVar3;
  uint uVar4;
  
  FUN_8c608e24(1);
  puVar1 = DAT_8c608d08;
  if (*(int *)(param_1 + 4) == 0) {
    DAT_8c608d08[2] = 1;
LAB_8c608cb2:
    uVar3 = 0xffffffff;
  }
  else {
    for (uVar4 = 0; uVar4 < *(uint *)(param_1 + 4); uVar4 = uVar4 + 1) {
      iVar2 = (*(code *)PTR_FUN_8c608d0c)(param_1,uVar4);
      if (iVar2 < 0) {
        *puVar1 = uVar4;
        goto LAB_8c608cb2;
      }
    }
    uVar3 = 1;
  }
  return uVar3;
}

