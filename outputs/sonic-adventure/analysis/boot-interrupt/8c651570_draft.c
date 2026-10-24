/* Ghidra draft only; not buildable/verified source. */

void FUN_8c651570(void)

{
  undefined4 *puVar1;
  undefined4 *puVar2;
  int *piVar3;
  int *piVar4;
  uint uVar5;
  int *piVar6;
  
  piVar3 = DAT_8c65166c;
  puVar2 = DAT_8c651668;
  puVar1 = DAT_8c651664;
  uVar5 = 0;
  piVar6 = DAT_8c65166c + -1;
  while ((*(int *)(DAT_8c651670 + uVar5 * 4) != 5 && (*(int *)(DAT_8c651670 + uVar5 * 4) != -3))) {
    uVar5 = uVar5 + 1;
    if (3 < (int)uVar5) {
      return;
    }
  }
  *(undefined4 *)(DAT_8c651670 + uVar5 * 4) = 7;
  piVar4 = DAT_8c651678;
  if (((*DAT_8c651674 & 1) == 1) || ((*DAT_8c651674 & 0x2c) != 0x20)) {
    if (*DAT_8c651688 != 1) {
      *piVar6 = DAT_8c651678[uVar5 & 1] + *DAT_8c65167c;
      *piVar3 = piVar4[uVar5 & 1] + *DAT_8c651684;
    }
  }
  else {
    *piVar6 = *DAT_8c651678 + *DAT_8c65167c;
    *piVar3 = *DAT_8c651680 + *DAT_8c651684;
  }
  if (*(int *)(DAT_8c651670 + uVar5 * 4) == -3) {
    return;
  }
  if (*DAT_8c65168c != 1) {
    return;
  }
  *puVar2 = *DAT_8c651690;
  *puVar1 = *DAT_8c651694;
  *DAT_8c65168c = 0;
  return;
}

