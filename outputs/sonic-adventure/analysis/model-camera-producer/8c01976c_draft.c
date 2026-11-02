/* Ghidra draft only; not buildable/verified source. */

void FUN_8c01976c(short param_1)

{
  int *piVar1;
  short *psVar2;
  short *psVar3;
  undefined4 *puVar4;
  int iVar5;
  int *piVar6;
  
  iVar5 = (*(code *)PTR_FUN_8c019844)();
  psVar3 = DAT_8c01984c;
  psVar2 = DAT_8c019848;
  piVar1 = DAT_8c019820;
  if (iVar5 != 0) {
    *DAT_8c019848 = *DAT_8c019848 + param_1;
    piVar6 = (int *)(PTR_DAT_8c019850 + *psVar2 * 4 + *psVar3 * 0x20);
    *piVar1 = *piVar6;
    *DAT_8c019824 = *piVar6 + 0x40;
    FUN_8c01951e();
    puVar4 = DAT_8c019858;
    *DAT_8c019854 = 0;
    *puVar4 = 0;
  }
  return;
}

