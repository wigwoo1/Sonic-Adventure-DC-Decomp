/* Ghidra draft only; not buildable/verified source. */

void FUN_8c6564e0(uint param_1)

{
  int *piVar1;
  int *piVar2;
  int iVar3;
  int iVar4;
  int *piVar5;
  uint uVar6;
  uint uVar7;
  
  piVar1 = DAT_8c656580;
  if (*DAT_8c656588 == 1) {
    *DAT_8c656580 = DAT_8c656584;
  }
  else {
    *DAT_8c656580 = param_1 * 0x14 + DAT_8c656584;
  }
  piVar2 = DAT_8c65658c;
  *(undefined4 *)*DAT_8c65658c = *(undefined4 *)*piVar1;
  *(undefined4 *)(*piVar2 + 4) = *(undefined4 *)(*piVar1 + 4);
  *(undefined4 *)(*piVar2 + 8) = *(undefined4 *)(*piVar1 + 8);
  *(undefined4 *)(*piVar2 + 0xc) = *(undefined4 *)(*piVar1 + 0xc);
  *(undefined4 *)(*piVar2 + 0x10) = *(undefined4 *)(*piVar1 + 0x10);
  iVar3 = DAT_8c656590;
  uVar6 = 0;
  do {
    uVar7 = uVar6 + 4;
    *(undefined4 *)(uVar6 + iVar3) = *(undefined4 *)(uVar6 + *piVar2);
    iVar4 = DAT_8c656598;
    uVar6 = uVar7;
  } while (uVar7 < 0x14);
  *DAT_8c656594 = param_1 & 1;
  uVar6 = 0;
  do {
    piVar1 = (int *)(uVar6 + iVar4);
    piVar5 = (int *)(*piVar2 + uVar6);
    uVar6 = uVar6 + 4;
    *piVar5 = *piVar5 + *piVar1 * 4;
  } while (uVar6 < 0x14);
  return;
}

