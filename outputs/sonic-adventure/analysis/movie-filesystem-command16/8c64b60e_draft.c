/* Ghidra draft only; not buildable/verified source. */

int FUN_8c64b60e(int param_1,int param_2)

{
  undefined *puVar1;
  undefined *puVar2;
  int iVar3;
  int iVar4;
  int iVar5;
  int iVar6;
  
  puVar2 = PTR_FUN_8c64b6b4;
  puVar1 = PTR_DAT_8c64b6b0;
  iVar5 = param_1 * DAT_8c64b6a6 + DAT_8c64b6ac;
  iVar4 = (int)DAT_8c64b6a8;
  *(undefined4 *)(iVar5 + 0x30) = 1;
  *(undefined4 *)(iVar5 + 0x34) = *(undefined4 *)(puVar1 + *(int *)(iVar5 + 4) * 0x50);
  iVar6 = 0;
  while( true ) {
    if (**(int **)(iVar5 + 0x1c) + 1 <= iVar6) {
      return 0;
    }
    iVar3 = (*(code *)puVar2)(param_1,iVar6,param_2);
    if (iVar3 < 0) break;
    iVar6 = iVar6 + 1;
    param_2 = param_2 + iVar4;
  }
  return iVar3;
}

