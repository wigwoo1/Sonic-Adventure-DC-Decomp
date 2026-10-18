/* Ghidra draft only; not buildable/verified source. */

undefined4 FUN_8c652dd0(int param_1)

{
  undefined *puVar1;
  int iVar2;
  uint uVar3;
  int iVar4;
  undefined1 *puVar5;
  undefined4 in_fr3;
  double dVar6;
  float in_fr15;
  undefined4 in_fr14;
  double in_xd2;
  undefined4 in_PR;
  uint uVar8;
  uint uVar9;
  char in_FPSCR_PR;
  char in_FPSCR_SZ;
  undefined1 local_20 [4];
  undefined1 local_1c [4];
  float fVar7;
  
  puVar1 = PTR_FUN_8c652e6c;
  if (in_FPSCR_SZ == '\0') {
    puVar5 = local_1c;
  }
  else {
    puVar5 = local_20;
  }
  iVar4 = (int)DAT_8c652e62;
  *(undefined4 *)(puVar5 + -4) = in_PR;
  if (in_FPSCR_SZ == '\0') {
    in_fr15 = DAT_8c652e70;
  }
  uVar3 = iVar4 + 0x7f;
  if (in_FPSCR_SZ == '\0') {
    in_fr3 = *(undefined4 *)(DAT_8c652e64 + param_1);
  }
  else {
    in_xd2 = *(double *)(DAT_8c652e64 + param_1);
  }
  iVar2 = (int)DAT_8c652e66;
  if (in_FPSCR_SZ == '\0') {
    in_xd2 = (double)CONCAT44(in_fr3,in_fr3);
  }
  if (in_FPSCR_PR == '\0') {
    fVar7 = (float)((ulonglong)in_xd2 >> 0x20) * in_fr15;
    dVar6 = (double)CONCAT44(fVar7,SUB84(in_xd2,0));
    uVar8 = (uint)fVar7;
  }
  else {
    dVar6 = in_xd2 * (double)CONCAT44(in_fr14,in_fr15);
    uVar8 = (uint)dVar6;
  }
  uVar8 = uVar8 & uVar3;
  do {
    iVar4 = iVar4 + -1;
    if (in_FPSCR_SZ == '\0') {
      dVar6 = (double)CONCAT44(*(undefined4 *)(iVar4 * 4 + param_1),SUB84(dVar6,0));
    }
    else {
      dVar6 = *(double *)(iVar4 * 4 + param_1);
    }
    if (in_FPSCR_PR == '\0') {
      fVar7 = (float)((ulonglong)dVar6 >> 0x20) * in_fr15;
      dVar6 = (double)CONCAT44(fVar7,SUB84(dVar6,0));
      uVar9 = (uint)fVar7;
    }
    else {
      dVar6 = dVar6 * (double)CONCAT44(in_fr14,in_fr15);
      uVar9 = (uint)dVar6;
    }
    uVar9 = uVar9 & uVar3;
    (*(code *)puVar1)(iVar4 * 4 + iVar2,(uVar9 << 8 | uVar8) & 0xffff);
    uVar8 = uVar9;
  } while (iVar4 != 0);
  return 0;
}

