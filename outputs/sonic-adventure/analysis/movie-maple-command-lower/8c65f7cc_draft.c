/* Ghidra draft only; not buildable/verified source. */

undefined4 FUN_8c65f7cc(int param_1)

{
  int *piVar1;
  int iVar2;
  int *piVar3;
  int iVar4;
  int iVar5;
  
  piVar1 = DAT_8c65f850;
  iVar2 = (int)DAT_8c65f84a;
  iVar4 = *DAT_8c65f850;
  iVar5 = (int)DAT_8c65f84c;
  *(int *)(iVar2 + iVar4) = *(int *)(iVar2 + iVar4) + *(int *)(iVar2 + 0xc + iVar4) * 4;
  piVar3 = *(int **)(iVar2 + *piVar1);
  iVar2 = 0;
  if (0 < param_1) {
    do {
      iVar2 = iVar2 + 1;
      *piVar3 = iVar5;
      piVar3 = piVar3 + 1;
    } while (iVar2 < param_1);
  }
  iVar2 = (int)DAT_8c65f84e;
  *(int *)(iVar2 + *piVar1) = param_1;
  iVar2 = iVar2 + 4;
  *(int *)(iVar2 + *piVar1) = *(int *)(iVar2 + *piVar1) + param_1 * 4;
  return 0;
}

