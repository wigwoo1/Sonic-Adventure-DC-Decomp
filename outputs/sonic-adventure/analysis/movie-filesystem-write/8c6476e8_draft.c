/* Ghidra draft only; not buildable/verified source. */

int FUN_8c6476e8(int param_1,undefined4 param_2,undefined4 param_3)

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
  
  puVar2 = PTR_FUN_8c6477bc;
  puVar1 = PTR_DAT_8c6477b8;
  uVar9 = 0xfffc;
  iVar6 = (int)DAT_8c6477a8;
  iVar5 = (int)DAT_8c6477aa;
  iVar4 = *DAT_8c6477b0;
  iVar7 = param_1 * iVar6 + DAT_8c6477b4;
  do {
    iVar3 = (*(code *)puVar2)(param_1,param_2,param_3,uVar9);
    if (iVar3 == 0) break;
    if ((*(int *)puVar1 != 0) && (iVar3 == iVar5)) goto LAB_8c647786;
    bVar8 = iVar4 != 0;
    *(int *)(iVar7 + 0x34) = *(int *)(iVar7 + 0x34) + 1;
    iVar4 = iVar4 + -1;
  } while ((bVar8) || (*DAT_8c6477b0 == -1));
  puVar1 = PTR_FUN_8c6477c4;
  iVar6 = param_1 * iVar6 + DAT_8c6477b4;
  iVar4 = param_1 * 0x80 + DAT_8c6477c0;
  iVar5 = *(int *)(iVar6 + 0x30);
  *(int *)(iVar6 + 0x30) = iVar5 + 1;
  (*(code *)puVar1)(param_1,*(undefined4 *)(iVar4 + 8),iVar5,*(undefined4 *)(iVar6 + 0x34));
  if (iVar3 == 0) {
    iVar4 = 0;
  }
  else if (iVar3 == DAT_8c6477ac) {
LAB_8c647786:
    iVar4 = (int)DAT_8c6477ac;
  }
  else {
    iVar4 = DAT_8c6477c8;
    if (iVar3 == DAT_8c6477ae) {
      iVar4 = (int)DAT_8c6477ae;
    }
  }
  return iVar4;
}

