/* Ghidra draft only; not buildable/verified source. */

void FUN_8c604d68(undefined4 param_1)

{
  undefined4 *puVar1;
  undefined4 *puVar2;
  int iVar3;
  undefined8 *puVar4;
  undefined8 *puVar5;
  float fVar7;
  double in_dr2;
  double dVar8;
  undefined4 uVar9;
  double dVar10;
  undefined8 in_xd2;
  char in_FPSCR_PR;
  char in_FPSCR_SZ;
  undefined4 uStack_c;
  undefined4 local_8;
  undefined4 uStack_4;
  undefined8 *puVar6;
  
  puVar2 = DAT_8c604e40;
  puVar1 = DAT_8c604e3c;
  if (in_FPSCR_PR == '\0') {
    dVar10 = (double)CONCAT44(0x40000000,param_1);
  }
  else {
    dVar10 = (double)CONCAT44(0x3f800000,param_1) + (double)CONCAT44(0x3f800000,param_1);
  }
  *DAT_8c604e38 = 0;
  *puVar1 = 0;
  *puVar2 = 0;
  iVar3 = DAT_8c604e44;
  if (in_FPSCR_SZ == '\0') {
    in_dr2 = (double)CONCAT44((int)((ulonglong)in_dr2 >> 0x20),*(undefined4 *)(DAT_8c604e44 + 4));
  }
  else {
    in_xd2 = *(undefined8 *)(DAT_8c604e44 + 4);
  }
  fVar7 = (float)((ulonglong)dVar10 >> 0x20);
  if (in_FPSCR_PR == '\0') {
    dVar8 = (double)CONCAT44((int)((ulonglong)in_dr2 >> 0x20),SUB84(in_dr2,0) / fVar7);
  }
  else {
    dVar8 = in_dr2 / dVar10;
  }
  if (in_FPSCR_SZ == '\0') {
    *(int *)(DAT_8c604e44 + 0xc) = SUB84(dVar8,0);
    dVar8 = (double)CONCAT44((int)((ulonglong)dVar8 >> 0x20),*(undefined4 *)(iVar3 + 8));
  }
  else {
    *(undefined8 *)(DAT_8c604e44 + 0xc) = in_xd2;
    in_xd2 = *(undefined8 *)(iVar3 + 8);
  }
  if (in_FPSCR_PR == '\0') {
    dVar8 = (double)CONCAT44((int)((ulonglong)dVar8 >> 0x20),SUB84(dVar8,0) / fVar7);
  }
  else {
    dVar8 = dVar8 / dVar10;
  }
  uVar9 = SUB84(dVar10,0);
  if (in_FPSCR_SZ == '\0') {
    *(int *)(iVar3 + 0x10) = SUB84(dVar8,0);
    uStack_c = 0;
    dVar8 = (double)CONCAT44((int)((ulonglong)dVar8 >> 0x20),*(undefined4 *)(iVar3 + 4));
  }
  else {
    *(undefined8 *)(iVar3 + 0x10) = in_xd2;
    in_xd2 = *(undefined8 *)(iVar3 + 4);
    uStack_c = uVar9;
  }
  puVar4 = DAT_8c604e48;
  if (in_FPSCR_PR == '\0') {
    dVar10 = (double)CONCAT44(0xbf800000,uVar9);
    fVar7 = -1.0;
    dVar8 = (double)CONCAT44((int)((ulonglong)dVar8 >> 0x20),SUB84(dVar8,0) + -1.0);
  }
  else {
    dVar10 = -(double)CONCAT44(0x3f800000,uVar9);
    fVar7 = (float)((ulonglong)dVar10 >> 0x20);
    dVar8 = dVar8 + dVar10;
  }
  if (in_FPSCR_SZ == '\0') {
    local_8 = SUB84(dVar8,0);
    dVar8 = (double)CONCAT44((int)((ulonglong)dVar8 >> 0x20),*(undefined4 *)(iVar3 + 8));
  }
  else {
    local_8 = (undefined4)in_xd2;
    in_xd2 = *(undefined8 *)(iVar3 + 8);
  }
  if (in_FPSCR_PR == '\0') {
    fVar7 = SUB84(dVar8,0) + fVar7;
  }
  else {
    fVar7 = SUB84(dVar8 + dVar10,0);
  }
  puVar5 = DAT_8c604e48 + 1;
  puVar6 = DAT_8c604e48 + 1;
  if (in_FPSCR_SZ == '\0') {
    *(undefined4 *)DAT_8c604e48 = uStack_c;
    *(undefined4 *)((int)puVar4 + 4) = uStack_c;
    *(undefined4 *)puVar5 = local_8;
    *(float *)((int)puVar4 + 0xc) = fVar7;
  }
  else {
    uStack_4 = (undefined4)in_xd2;
    *DAT_8c604e48 = CONCAT44(uStack_c,uStack_c);
    *(ulonglong *)((int)puVar4 + 4) = CONCAT44(local_8,uStack_c);
    *puVar6 = CONCAT44(uStack_4,local_8);
    *(undefined8 *)((int)puVar4 + 0xc) = in_xd2;
  }
  return;
}

