/* Ghidra draft only; not buildable/verified source. */

void FUN_8c098ee0(int *param_1)

{
  undefined *puVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  int iVar5;
  int iVar6;
  undefined4 *puVar7;
  bool bVar8;
  undefined4 local_220 [64];
  undefined4 local_120 [65];
  
  iVar6 = 0;
  iVar3 = *param_1;
  iVar5 = 0;
  iVar4 = param_1[1];
  if (0 < iVar4) {
    do {
      puVar1 = PTR_DAT_8c099060;
      puVar7 = (undefined4 *)(iVar5 * 0xc + iVar3);
      if ((*(int *)PTR_DAT_8c099060 == *DAT_8c099064) &&
         (iVar2 = (*(code *)PTR_FUN_8c099068)(*(undefined4 *)(PTR_DAT_8c099060 + 4),*puVar7),
         iVar2 == 0)) {
        local_120[iVar6] = puVar7;
        local_220[iVar6] = *puVar7;
        *puVar7 = *(undefined4 *)(puVar1 + 8);
        iVar6 = iVar6 + 1;
      }
      iVar5 = iVar5 + 1;
    } while ((iVar5 < iVar4) && (iVar6 < 0x3f));
  }
  (*(code *)PTR_FUN_8c09906c)(param_1);
  while (bVar8 = iVar6 != 0, iVar6 = iVar6 + -1, bVar8) {
    *(undefined4 *)local_120[iVar6] = local_220[iVar6];
  }
  return;
}

