/* Ghidra draft only; not buildable/verified source. */

undefined4 FUN_8c652f66(undefined4 param_1,int param_2)

{
  float fVar1;
  double in_dr2;
  double dVar3;
  float fVar4;
  int iVar5;
  int iVar6;
  int iVar7;
  uint uVar8;
  char in_FPSCR_PR;
  char in_FPSCR_SZ;
  double dVar2;
  
  if (in_FPSCR_SZ == '\0') {
    dVar3 = (double)CONCAT44((undefined4)DAT_8c653008,param_1);
    in_dr2 = (double)CONCAT44((int)((ulonglong)in_dr2 >> 0x20),*(undefined4 *)(param_2 + 0x70));
  }
  else {
    dVar3 = (double)CONCAT44(DAT_8c653008._4_4_,(undefined4)DAT_8c653008);
  }
  fVar4 = (float)((ulonglong)dVar3 >> 0x20);
  if (in_FPSCR_PR == '\0') {
    fVar1 = SUB84(in_dr2,0) * fVar4;
    dVar2 = (double)CONCAT44((int)((ulonglong)in_dr2 >> 0x20),fVar1);
    iVar5 = (int)fVar1;
  }
  else {
    dVar2 = in_dr2 * dVar3;
    iVar5 = (int)dVar2;
  }
  if (in_FPSCR_SZ == '\0') {
    dVar2 = (double)CONCAT44((int)((ulonglong)dVar2 >> 0x20),*(undefined4 *)(param_2 + 0x74));
  }
  if (in_FPSCR_PR == '\0') {
    fVar1 = SUB84(dVar2,0) * fVar4;
    dVar2 = (double)CONCAT44((int)((ulonglong)dVar2 >> 0x20),fVar1);
    iVar6 = (int)fVar1;
  }
  else {
    dVar2 = dVar2 * dVar3;
    iVar6 = (int)dVar2;
  }
  if (in_FPSCR_SZ == '\0') {
    dVar2 = (double)CONCAT44((int)((ulonglong)dVar2 >> 0x20),*(undefined4 *)(param_2 + 0x78));
  }
  if (in_FPSCR_PR == '\0') {
    dVar2 = (double)CONCAT44((int)((ulonglong)dVar2 >> 0x20),SUB84(dVar2,0) * fVar4);
  }
  else {
    dVar2 = dVar2 * dVar3;
  }
  if (in_FPSCR_PR == '\0') {
    iVar7 = (int)SUB84(dVar2,0);
  }
  else {
    iVar7 = (int)dVar2;
  }
  if (in_FPSCR_SZ == '\0') {
    dVar2 = (double)CONCAT44((int)((ulonglong)dVar2 >> 0x20),*(undefined4 *)(param_2 + 0x7c));
  }
  if (in_FPSCR_PR == '\0') {
    dVar2 = (double)CONCAT44((int)((ulonglong)dVar2 >> 0x20),SUB84(dVar2,0) * fVar4);
  }
  else {
    dVar2 = dVar2 * dVar3;
  }
  if (in_FPSCR_PR == '\0') {
    uVar8 = (uint)SUB84(dVar2,0);
  }
  else {
    uVar8 = (uint)dVar2;
  }
  *DAT_8c653008._4_4_ = iVar5 << 0x18 | iVar6 << 0x10 | iVar7 << 8 | uVar8;
  if (in_FPSCR_SZ == '\0') {
    dVar2 = (double)CONCAT44((int)((ulonglong)dVar2 >> 0x20),*(undefined4 *)(param_2 + 0x80));
  }
  if (in_FPSCR_PR == '\0') {
    fVar1 = SUB84(dVar2,0) * fVar4;
    dVar2 = (double)CONCAT44((int)((ulonglong)dVar2 >> 0x20),fVar1);
    iVar5 = (int)fVar1;
  }
  else {
    dVar2 = dVar2 * dVar3;
    iVar5 = (int)dVar2;
  }
  if (in_FPSCR_SZ == '\0') {
    dVar2 = (double)CONCAT44((int)((ulonglong)dVar2 >> 0x20),*(undefined4 *)(param_2 + 0x84));
  }
  if (in_FPSCR_PR == '\0') {
    fVar1 = SUB84(dVar2,0) * fVar4;
    dVar2 = (double)CONCAT44((int)((ulonglong)dVar2 >> 0x20),fVar1);
    iVar6 = (int)fVar1;
  }
  else {
    dVar2 = dVar2 * dVar3;
    iVar6 = (int)dVar2;
  }
  if (in_FPSCR_SZ == '\0') {
    dVar2 = (double)CONCAT44((int)((ulonglong)dVar2 >> 0x20),*(undefined4 *)(param_2 + 0x88));
  }
  if (in_FPSCR_PR == '\0') {
    dVar2 = (double)CONCAT44((int)((ulonglong)dVar2 >> 0x20),SUB84(dVar2,0) * fVar4);
  }
  else {
    dVar2 = dVar2 * dVar3;
  }
  if (in_FPSCR_PR == '\0') {
    iVar7 = (int)SUB84(dVar2,0);
  }
  else {
    iVar7 = (int)dVar2;
  }
  if (in_FPSCR_SZ == '\0') {
    dVar2 = (double)CONCAT44((int)((ulonglong)dVar2 >> 0x20),*(undefined4 *)(param_2 + 0x8c));
  }
  if (in_FPSCR_PR == '\0') {
    dVar2 = (double)CONCAT44((int)((ulonglong)dVar2 >> 0x20),SUB84(dVar2,0) * fVar4);
  }
  else {
    dVar2 = dVar2 * dVar3;
  }
  if (in_FPSCR_PR == '\0') {
    uVar8 = (uint)SUB84(dVar2,0);
  }
  else {
    uVar8 = (uint)dVar2;
  }
  *DAT_8c653010 = iVar5 << 0x18 | iVar6 << 0x10 | iVar7 << 8 | uVar8;
  iVar5 = (int)DAT_8c653002;
  *DAT_8c653014 = *(undefined4 *)(iVar5 + param_2);
  *DAT_8c653018 = *(undefined4 *)(iVar5 + 4 + param_2);
  *DAT_8c65301c = *(undefined4 *)(iVar5 + 8 + param_2);
  return 0;
}

