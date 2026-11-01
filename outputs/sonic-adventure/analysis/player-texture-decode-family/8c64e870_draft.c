/* Ghidra draft only; not buildable/verified source. */

void FUN_8c64e870(int param_1,uint param_2,int param_3)

{
  undefined *puVar1;
  undefined *puVar2;
  uint uVar3;
  int iVar4;
  int *piVar5;
  undefined1 *puVar6;
  
  puVar1 = PTR_FUN_8c64e934;
  puVar6 = &DAT_8c88f7a8;
  iVar4 = 0;
  for (uVar3 = 0; uVar3 < *(uint *)PTR_DAT_8c64e940; uVar3 = uVar3 + 1) {
    piVar5 = (int *)(*(int *)PTR_DAT_8c64e938 + iVar4);
    if (*piVar5 == param_1) {
      piVar5[4] = piVar5[4] | param_2;
      piVar5[5] = param_3;
      puVar2 = PTR_FUN_8c64e93c;
      if ((param_2 & 2) != 0) {
        *(undefined2 *)(piVar5 + 0x10) = 1;
        (*(code *)puVar2)(piVar5,puVar6);
        (*(code *)puVar1)(piVar5,piVar5[1]);
      }
    }
    iVar4 = iVar4 + 0x44;
  }
  return;
}

