/* Ghidra draft only; not buildable/verified source. */

int FUN_8c64753c(int param_1,undefined4 param_2)

{
  undefined *puVar1;
  undefined *puVar2;
  int iVar3;
  int iVar4;
  int iVar5;
  int iVar6;
  int iVar7;
  bool bVar8;
  undefined4 uVar9;
  
  puVar2 = PTR_FUN_8c647600;
  puVar1 = PTR_DAT_8c6475fc;
  uVar9 = 0;
  iVar6 = (int)DAT_8c6475ec;
  iVar5 = (int)DAT_8c6475ee;
  iVar4 = *DAT_8c6475f4;
  iVar7 = param_1 * iVar6 + DAT_8c6475f8;
  do {
    iVar3 = (*(code *)puVar2)(param_1,param_2,uVar9);
    if (iVar3 == 0) break;
    if ((*(int *)puVar1 != 0) && (iVar3 == iVar5)) goto LAB_8c6475d0;
    bVar8 = iVar4 != 0;
    *(int *)(iVar7 + 0x34) = *(int *)(iVar7 + 0x34) + 1;
    iVar4 = iVar4 + -1;
  } while ((bVar8) || (*DAT_8c6475f4 == -1));
  puVar1 = PTR_FUN_8c647608;
  iVar6 = param_1 * iVar6 + DAT_8c6475f8;
  iVar4 = param_1 * 0x80 + DAT_8c647604;
  iVar5 = *(int *)(iVar6 + 0x30);
  *(int *)(iVar6 + 0x30) = iVar5 + 1;
  (*(code *)puVar1)(param_1,*(undefined4 *)(iVar4 + 8),iVar5,*(undefined4 *)(iVar6 + 0x34));
  if (iVar3 == 0) {
    iVar4 = 0;
  }
  else {
    iVar4 = DAT_8c64760c;
    if (iVar3 == DAT_8c6475f0) {
LAB_8c6475d0:
      iVar4 = (int)DAT_8c6475f0;
    }
  }
  return iVar4;
}

