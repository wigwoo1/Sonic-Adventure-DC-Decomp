/* Ghidra draft only; not buildable/verified source. */

int FUN_8c666e50(undefined4 param_1,int param_2)

{
  undefined *puVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  
  puVar1 = PTR_DAT_8c666ed4;
  iVar2 = DAT_8c666ed8;
  if (*(int *)PTR_DAT_8c666ed4 != 0) {
    iVar2 = (int)DAT_8c666ed0;
    iVar3 = (int)DAT_8c666ed0;
    *(undefined4 *)(iVar2 + *(int *)PTR_DAT_8c666ed4) = param_1;
    iVar4 = 0;
    *(int *)(iVar2 + 4 + *(int *)puVar1) = param_2;
    *(undefined4 *)(iVar2 + 8 + *(int *)puVar1) = 0;
    *(undefined4 *)(iVar2 + 0xc + *(int *)puVar1) = 0;
    *(undefined4 *)(iVar2 + 0x10 + *(int *)puVar1) = 0;
    *(undefined4 *)(iVar2 + 0x14 + *(int *)puVar1) = 0;
    *DAT_8c666edc = iVar3 + *(int *)puVar1;
    iVar3 = 0;
    iVar2 = 0;
    if (0 < param_2) {
      do {
        (*(code *)PTR_FUN_8c666ee4)(iVar3);
        iVar4 = iVar4 + 1;
        iVar3 = iVar3 + 0x20;
      } while (iVar4 < param_2);
    }
  }
  if (iVar2 != 0) {
    (*(code *)PTR_FUN_8c666eec)(PTR_FUN_8c666ee8,iVar2);
  }
  return iVar2;
}

