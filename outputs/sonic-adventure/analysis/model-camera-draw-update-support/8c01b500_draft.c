/* Ghidra draft only; not buildable/verified source. */

void FUN_8c01b500(void)

{
  int *piVar1;
  undefined4 *puVar2;
  int iVar3;
  undefined4 uVar4;
  double dVar5;
  float extraout_fr3;
  float fVar6;
  undefined4 extraout_fr2;
  bool bVar7;
  char in_FPSCR_PR;
  char in_FPSCR_SZ;
  
  piVar1 = DAT_8c01b5a0;
  (*(code *)PTR_FUN_8c01b5a8)(DAT_8c01b5a4,*DAT_8c01b5a0 + 0x20);
  dVar5 = (double)(*(code *)PTR_FUN_8c01b5ac)(DAT_8c01b5a4);
  fVar6 = extraout_fr3;
  if (in_FPSCR_SZ == '\0') {
    fVar6 = (float)DAT_8c01b5b0;
  }
  if (in_FPSCR_PR == '\0') {
    bVar7 = (float)((ulonglong)dVar5 >> 0x20) < fVar6;
  }
  else {
    bVar7 = dVar5 < (double)CONCAT44(extraout_fr2,fVar6);
  }
  if ((bVar7) && ((*(ushort *)(*piVar1 + 4) & 1) != 0)) {
    *DAT_8c01b5b0._4_4_ = 1;
  }
  else {
    *DAT_8c01b5b0._4_4_ = 0;
  }
  puVar2 = DAT_8c01b5a4;
  iVar3 = *DAT_8c01b5a0;
  uVar4 = *(undefined4 *)(iVar3 + 0x24);
  *DAT_8c01b5a4 = *(undefined4 *)(iVar3 + 0x20);
  puVar2[1] = uVar4;
  puVar2[2] = *(undefined4 *)(iVar3 + 0x28);
  return;
}

