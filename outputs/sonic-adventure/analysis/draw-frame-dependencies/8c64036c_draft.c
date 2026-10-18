/* Ghidra draft only; not buildable/verified source. */

/* WARNING: Removing unreachable block (ram,0x8c64038e) */

void FUN_8c64036c(double param_1)

{
  double *pdVar1;
  float fVar2;
  double dVar3;
  float fVar4;
  float fVar7;
  double dVar5;
  double dVar6;
  undefined4 uVar8;
  double in_xd0;
  double in_xd4;
  bool bVar9;
  bool bVar10;
  char in_FPSCR_PR;
  char in_FPSCR_SZ;
  
  pdVar1 = DAT_8c6403a8;
  if (in_FPSCR_SZ == '\0') {
    dVar3 = (double)CONCAT44((undefined4)DAT_8c64039c,DAT_8c64039c._4_4_);
  }
  else {
    dVar3 = (double)CONCAT44(DAT_8c64039c._4_4_,(undefined4)DAT_8c64039c);
    in_xd0 = (double)CONCAT44(DAT_8c6403a4,DAT_8c64039c._4_4_);
  }
  fVar4 = (float)((ulonglong)dVar3 >> 0x20);
  fVar7 = (float)((ulonglong)param_1 >> 0x20);
  fVar2 = SUB84(dVar3,0);
  if (in_FPSCR_PR == '\0') {
    bVar9 = fVar4 < fVar7;
    bVar10 = fVar7 < fVar2;
  }
  else {
    bVar9 = dVar3 < param_1;
    bVar10 = param_1 < dVar3;
  }
  dVar5 = param_1;
  if ((bVar9) && (dVar5 = dVar3, in_FPSCR_SZ == '\0')) {
    dVar5 = (double)CONCAT44(fVar4,SUB84(param_1,0));
  }
  if (in_FPSCR_PR == '\0') {
    bVar9 = fVar4 < SUB84(dVar5,0);
  }
  else {
    bVar9 = dVar3 < dVar5;
  }
  dVar6 = dVar5;
  if ((bVar10) && (dVar6 = in_xd0, in_FPSCR_SZ == '\0')) {
    dVar6 = (double)CONCAT44(fVar2,SUB84(dVar5,0));
  }
  if (in_FPSCR_PR == '\0') {
    bVar10 = SUB84(dVar6,0) < fVar2;
  }
  else {
    bVar10 = dVar6 < dVar3;
  }
  dVar5 = in_xd4;
  if ((bVar9) && (dVar5 = dVar3, in_FPSCR_SZ == '\0')) {
    dVar6 = (double)CONCAT44((int)((ulonglong)dVar6 >> 0x20),fVar4);
    dVar5 = in_xd4;
  }
  if (in_FPSCR_PR == '\0') {
    dVar6 = (double)CONCAT44(-(float)((ulonglong)dVar6 >> 0x20),SUB84(dVar6,0));
  }
  else {
    dVar6 = -dVar6;
  }
  dVar3 = dVar5;
  if ((bVar10) && (dVar3 = in_xd0, in_FPSCR_SZ == '\0')) {
    dVar6 = (double)CONCAT44((int)((ulonglong)dVar6 >> 0x20),fVar2);
    dVar3 = dVar5;
  }
  uVar8 = (undefined4)((ulonglong)dVar6 >> 0x20);
  if (in_FPSCR_SZ == '\0') {
    *(undefined4 *)DAT_8c6403a4 = uVar8;
    *(float *)pdVar1 = -SUB84(dVar6,0);
  }
  else {
    *DAT_8c6403a4 = CONCAT44(uVar8,-SUB84(dVar6,0));
    *pdVar1 = dVar3;
  }
  return;
}

