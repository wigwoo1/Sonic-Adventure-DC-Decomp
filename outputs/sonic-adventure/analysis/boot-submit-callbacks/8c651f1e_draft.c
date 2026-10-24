/* Ghidra draft only; not buildable/verified source. */

void FUN_8c651f1e(void)

{
  int iVar1;
  uint *puVar2;
  undefined4 uVar3;
  int iVar4;
  undefined4 *puVar5;
  int iVar6;
  undefined4 *puVar7;
  
  iVar1 = DAT_8c651f94;
  if (*(int *)(*DAT_8c651f98 * 4 + DAT_8c651f60) != -1) {
    if (*DAT_8c651f9c == 0) {
      *DAT_8c651fa0 = *DAT_8c651fa0 | 0x20;
      DAT_8c651f98[*DAT_8c651f98 + -6] = 3;
    }
    puVar2 = DAT_8c652064;
    if ((*DAT_8c651fa8 & 1) == 0) {
      iVar4 = *(int *)(DAT_8c652068 + (*DAT_8c652064 & 1) * 0x20);
    }
    else {
      iVar4 = *DAT_8c651fa4;
    }
    *(int *)((*DAT_8c652064 & 1) * 4 + iVar1 + 0x24) = *DAT_8c651fa4;
    *DAT_8c65207c =
         *DAT_8c652070 & DAT_8c65206c |
         (iVar4 - *(int *)(DAT_8c652074 + (*puVar2 & 1) * 0x20)) * 2 & DAT_8c652078;
    iVar6 = 0x30;
    puVar5 = (undefined4 *)(iVar4 + DAT_8c652084);
    puVar7 = DAT_8c652080;
    do {
      uVar3 = *puVar7;
      puVar7 = puVar7 + 1;
      iVar6 = iVar6 + -1;
      *puVar5 = uVar3;
      puVar5 = puVar5 + 1;
    } while (iVar6 != 0);
  }
  *DAT_8c652088 = 0;
  *(int *)(iVar1 + 0x4c) = *(int *)(iVar1 + 0x48) - *DAT_8c65208c;
  return;
}

