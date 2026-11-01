/* Ghidra draft only; not buildable/verified source. */

void FUN_8c099750(undefined4 param_1,int *param_2)

{
  int iVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  int iVar5;
  undefined1 auStack_3810 [14336];
  undefined1 auStack_10 [12];
  
  iVar1 = DAT_8c0998c8;
  (*(code *)PTR_FUN_8c0998cc)(auStack_10,DAT_8c0998c8,auStack_3810,0x200);
  FUN_8c0997bc(param_1,auStack_10);
  iVar4 = 0;
  iVar5 = param_2[1];
  iVar2 = *param_2;
  iVar3 = 0;
  if (0 < iVar5) {
    do {
      iVar4 = iVar4 + 1;
      *(undefined4 *)(iVar3 + iVar2 + 8) = *(undefined4 *)(iVar3 + iVar1 + 8);
      *(undefined4 *)(iVar3 + iVar2 + 4) = *(undefined4 *)(iVar3 + iVar1 + 4);
      iVar3 = iVar3 + 0xc;
    } while (iVar4 < iVar5);
  }
  return;
}

