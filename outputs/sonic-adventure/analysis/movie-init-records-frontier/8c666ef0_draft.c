/* Ghidra draft only; not buildable/verified source. */

int FUN_8c666ef0(undefined4 param_1,int param_2)

{
  undefined *puVar1;
  undefined4 *puVar2;
  int *piVar3;
  int iVar4;
  int iVar5;
  int iVar6;
  
  puVar1 = PTR_DAT_8c666f74;
  iVar4 = DAT_8c666f78;
  if (*(int *)PTR_DAT_8c666f74 != 0) {
    iVar4 = (int)DAT_8c666f72;
    iVar5 = (int)DAT_8c666f72;
    *(undefined4 *)(iVar4 + *(int *)PTR_DAT_8c666f74) = param_1;
    puVar2 = DAT_8c666f80;
    iVar6 = 0;
    *(int *)(iVar4 + 4 + *(int *)puVar1) = param_2;
    *(undefined4 *)(iVar4 + 8 + *(int *)puVar1) = 0;
    *(undefined4 *)(iVar4 + 0xc + *(int *)puVar1) = 0;
    *(undefined4 *)(iVar4 + 0x10 + *(int *)puVar1) = 0;
    *DAT_8c666f7c = iVar5 + *(int *)puVar1;
    piVar3 = DAT_8c666f84;
    *puVar2 = param_1;
    *piVar3 = param_2;
    iVar5 = 0;
    iVar4 = 0;
    if (0 < param_2) {
      do {
        (*(code *)PTR_FUN_8c666f8c)(iVar5);
        iVar6 = iVar6 + 1;
        iVar5 = iVar5 + 8;
      } while (iVar6 < param_2);
    }
  }
  if (iVar4 != 0) {
    (*(code *)PTR_FUN_8c666f94)(PTR_FUN_8c666f90,iVar4);
  }
  return iVar4;
}

