/* Ghidra draft only; not buildable/verified source. */

/* WARNING: Removing unreachable block (ram,0x8c6407c4) */
/* WARNING: Removing unreachable block (ram,0x8c6407da) */

void FUN_8c6407b0(float param_1,undefined4 param_2,double param_3,undefined4 param_4,short param_5,
                 short param_6,ushort param_7)

{
  int *piVar1;
  undefined4 *puVar2;
  undefined4 uVar3;
  undefined4 *puVar4;
  double *pdVar5;
  int iVar6;
  float fVar7;
  double dVar8;
  double dVar9;
  undefined4 uVar10;
  double dVar11;
  undefined4 uVar13;
  double dVar12;
  float fVar14;
  undefined4 uVar15;
  double dVar16;
  float fVar17;
  double dVar18;
  undefined4 uVar19;
  double in_xd4;
  double in_xd6;
  double dVar20;
  double in_xd8;
  double dVar21;
  char in_FPSCR_PR;
  char in_FPSCR_SZ;
  
  puVar2 = DAT_8c640a38;
  piVar1 = DAT_8c640a34;
  fVar7 = (float)DAT_8c640a28;
  if (in_FPSCR_SZ != '\0') {
    in_xd4 = (double)CONCAT44(DAT_8c640a28._4_4_,(float)DAT_8c640a28);
    fVar7 = param_1;
  }
  dVar12 = in_xd4;
  if (in_FPSCR_SZ == '\0') {
    dVar12 = (double)CONCAT44(fVar7,param_2);
  }
  fVar17 = DAT_8c640a30;
  fVar14 = DAT_8c640a28._4_4_;
  if (in_FPSCR_SZ == '\0') {
    fVar17 = DAT_8c640a28._4_4_;
    fVar14 = fVar7;
  }
  if (in_FPSCR_PR == '\0') {
    dVar12 = (double)CONCAT44(fVar17 * (float)(int)(short)(param_7 & 0xf) +
                              (float)((ulonglong)dVar12 >> 0x20),SUB84(dVar12,0));
  }
  fVar7 = (float)(int)(short)(param_7 >> 4 & 0xf);
  if (in_FPSCR_SZ == '\0') {
    dVar11 = (double)CONCAT44(DAT_8c640a30,fVar14);
  }
  else {
    dVar11 = (double)CONCAT44(DAT_8c640a34,DAT_8c640a30);
  }
  iVar6 = *DAT_8c640a34;
  uVar13 = (undefined4)((ulonglong)dVar12 >> 0x20);
  dVar18 = dVar12;
  if (in_FPSCR_SZ == '\0') {
    dVar18 = (double)CONCAT44(uVar13,param_4);
  }
  fVar14 = (float)((ulonglong)dVar11 >> 0x20);
  if (in_FPSCR_PR == '\0') {
    dVar18 = (double)CONCAT44((float)((ulonglong)dVar18 >> 0x20) + fVar14,SUB84(dVar18,0));
    dVar11 = (double)CONCAT44(fVar14,fVar17 * fVar7 + SUB84(dVar11,0));
  }
  else {
    dVar18 = dVar18 + dVar11;
  }
  dVar21 = in_xd4;
  if (in_FPSCR_SZ == '\0') {
    param_3 = (double)CONCAT44((int)((ulonglong)param_3 >> 0x20),SUB84(dVar11,0));
    dVar21 = in_xd8;
  }
  if (in_FPSCR_PR == '\0') {
    fVar14 = SUB84(param_3,0) + (float)((ulonglong)dVar11 >> 0x20);
  }
  else {
    fVar14 = SUB84(param_3 + dVar11,0);
  }
  dVar20 = in_xd4;
  if (in_FPSCR_SZ == '\0') {
    dVar12 = (double)CONCAT44(uVar13,SUB84(dVar11,0));
    dVar20 = in_xd6;
  }
  *DAT_8c640a34 = iVar6 + 0x18;
  *(undefined4 *)(iVar6 + 0x18) = *puVar2;
  pdVar5 = (double *)(*piVar1 + -4);
  *piVar1 = (int)pdVar5;
  if (in_FPSCR_SZ == '\0') {
    *(int *)pdVar5 = SUB84(dVar12,0);
  }
  else {
    *pdVar5 = dVar20;
  }
  pdVar5 = (double *)(*piVar1 + -4);
  *piVar1 = (int)pdVar5;
  uVar13 = (undefined4)((ulonglong)dVar12 >> 0x20);
  if (in_FPSCR_SZ == '\0') {
    *(undefined4 *)pdVar5 = uVar13;
  }
  else {
    *pdVar5 = dVar12;
  }
  pdVar5 = (double *)(*piVar1 + -4);
  *piVar1 = (int)pdVar5;
  if (in_FPSCR_SZ == '\0') {
    *(undefined4 *)pdVar5 = 0x3f800000;
  }
  else {
    *pdVar5 = in_xd4;
  }
  uVar3 = DAT_8c640a3c;
  if (in_FPSCR_PR == '\0') {
    dVar11 = (double)CONCAT44((float)(int)param_6,0x3f800000);
  }
  else {
    dVar11 = (double)(int)param_6;
  }
  pdVar5 = (double *)(*piVar1 + -4);
  *piVar1 = (int)pdVar5;
  if (in_FPSCR_PR == '\0') {
    dVar16 = (double)CONCAT44((float)(int)param_5,fVar14);
  }
  else {
    dVar16 = (double)(int)param_5;
  }
  fVar14 = (float)((ulonglong)dVar11 >> 0x20);
  if (in_FPSCR_SZ == '\0') {
    *(float *)pdVar5 = fVar14;
  }
  else {
    *pdVar5 = dVar11;
  }
  pdVar5 = (double *)(*piVar1 + -4);
  *piVar1 = (int)pdVar5;
  fVar17 = (float)((ulonglong)dVar16 >> 0x20);
  if (in_FPSCR_SZ == '\0') {
    *(float *)pdVar5 = fVar17;
  }
  else {
    *pdVar5 = dVar16;
  }
  iVar6 = *piVar1;
  *piVar1 = iVar6 + -4;
  *(undefined4 *)(iVar6 + -4) = uVar3;
  puVar4 = DAT_8c640a40;
  iVar6 = *piVar1;
  *piVar1 = iVar6 + 0x38;
  dVar8 = dVar11;
  if (in_FPSCR_SZ == '\0') {
    dVar8 = (double)CONCAT44(fVar14,fVar7);
  }
  *(undefined4 *)(iVar6 + 0x38) = *puVar2;
  pdVar5 = (double *)(*piVar1 + -4);
  *piVar1 = (int)pdVar5;
  uVar15 = SUB84(dVar16,0);
  if (in_FPSCR_SZ == '\0') {
    *(undefined4 *)pdVar5 = uVar15;
  }
  else {
    *pdVar5 = dVar21;
  }
  pdVar5 = (double *)(*piVar1 + -4);
  *piVar1 = (int)pdVar5;
  if (in_FPSCR_SZ == '\0') {
    *(undefined4 *)pdVar5 = uVar13;
  }
  else {
    *pdVar5 = dVar12;
  }
  pdVar5 = (double *)(*piVar1 + -4);
  *piVar1 = (int)pdVar5;
  uVar13 = SUB84(dVar11,0);
  if (in_FPSCR_SZ == '\0') {
    *(undefined4 *)pdVar5 = uVar13;
  }
  else {
    *pdVar5 = in_xd4;
  }
  pdVar5 = (double *)(*piVar1 + -4);
  *piVar1 = (int)pdVar5;
  if (in_FPSCR_SZ == '\0') {
    dVar8 = (double)CONCAT44((int)((ulonglong)dVar8 >> 0x20),*puVar4);
  }
  if (in_FPSCR_PR == '\0') {
    dVar8 = (double)CONCAT44((float)((ulonglong)dVar8 >> 0x20) + SUB84(dVar8,0),SUB84(dVar8,0));
  }
  else {
    dVar8 = dVar8 + dVar8;
  }
  if (in_FPSCR_SZ == '\0') {
    *(int *)pdVar5 = (int)((ulonglong)dVar8 >> 0x20);
  }
  else {
    *pdVar5 = dVar8;
  }
  pdVar5 = (double *)(*piVar1 + -4);
  *piVar1 = (int)pdVar5;
  if (in_FPSCR_SZ == '\0') {
    *(float *)pdVar5 = fVar17;
  }
  else {
    *pdVar5 = dVar16;
  }
  iVar6 = *piVar1;
  *piVar1 = iVar6 + -4;
  *(undefined4 *)(iVar6 + -4) = uVar3;
  iVar6 = *piVar1;
  *piVar1 = iVar6 + 0x38;
  dVar9 = dVar16;
  if (in_FPSCR_SZ == '\0') {
    dVar9 = (double)CONCAT44(fVar17,SUB84(dVar8,0));
  }
  *(undefined4 *)(iVar6 + 0x38) = *puVar2;
  pdVar5 = (double *)(*piVar1 + -4);
  *piVar1 = (int)pdVar5;
  if (in_FPSCR_SZ == '\0') {
    *(int *)pdVar5 = SUB84(dVar12,0);
  }
  else {
    *pdVar5 = dVar20;
  }
  pdVar5 = (double *)(*piVar1 + -4);
  *piVar1 = (int)pdVar5;
  uVar19 = (undefined4)((ulonglong)dVar18 >> 0x20);
  if (in_FPSCR_SZ == '\0') {
    *(undefined4 *)pdVar5 = uVar19;
  }
  else {
    *pdVar5 = dVar18;
  }
  pdVar5 = (double *)(*piVar1 + -4);
  *piVar1 = (int)pdVar5;
  if (in_FPSCR_SZ == '\0') {
    *(undefined4 *)pdVar5 = uVar13;
  }
  else {
    *pdVar5 = in_xd4;
  }
  pdVar5 = (double *)(*piVar1 + -4);
  *piVar1 = (int)pdVar5;
  if (in_FPSCR_SZ == '\0') {
    *(float *)pdVar5 = fVar14;
  }
  else {
    *pdVar5 = dVar11;
  }
  pdVar5 = (double *)(*piVar1 + -4);
  *piVar1 = (int)pdVar5;
  if (in_FPSCR_SZ == '\0') {
    dVar9 = (double)CONCAT44((int)((ulonglong)dVar9 >> 0x20),*puVar4);
  }
  if (in_FPSCR_PR == '\0') {
    dVar9 = (double)CONCAT44((float)((ulonglong)dVar9 >> 0x20) + SUB84(dVar9,0),SUB84(dVar9,0));
  }
  else {
    dVar9 = dVar9 + dVar9;
  }
  uVar10 = (undefined4)((ulonglong)dVar9 >> 0x20);
  if (in_FPSCR_SZ == '\0') {
    *(undefined4 *)pdVar5 = uVar10;
  }
  else {
    *pdVar5 = dVar9;
  }
  iVar6 = *piVar1;
  *piVar1 = iVar6 + -4;
  *(undefined4 *)(iVar6 + -4) = uVar3;
  iVar6 = *piVar1;
  *piVar1 = iVar6 + 0x38;
  *(undefined4 *)(iVar6 + 0x38) = *puVar2;
  pdVar5 = (double *)(*piVar1 + -4);
  *piVar1 = (int)pdVar5;
  if (in_FPSCR_SZ == '\0') {
    *(undefined4 *)pdVar5 = uVar15;
  }
  else {
    *pdVar5 = dVar21;
  }
  pdVar5 = (double *)(*piVar1 + -4);
  *piVar1 = (int)pdVar5;
  if (in_FPSCR_SZ == '\0') {
    *(undefined4 *)pdVar5 = uVar19;
  }
  else {
    *pdVar5 = dVar18;
  }
  pdVar5 = (double *)(*piVar1 + -4);
  *piVar1 = (int)pdVar5;
  if (in_FPSCR_SZ == '\0') {
    *(undefined4 *)pdVar5 = uVar13;
  }
  else {
    *pdVar5 = in_xd4;
  }
  pdVar5 = (double *)(*piVar1 + -4);
  *piVar1 = (int)pdVar5;
  if (in_FPSCR_SZ == '\0') {
    dVar9 = (double)CONCAT44(uVar10,*puVar4);
  }
  if (in_FPSCR_PR == '\0') {
    dVar11 = (double)CONCAT44(fVar14 + SUB84(dVar9,0),uVar13);
  }
  else {
    dVar11 = dVar11 + dVar9;
  }
  if (in_FPSCR_SZ == '\0') {
    *(int *)pdVar5 = (int)((ulonglong)dVar11 >> 0x20);
  }
  else {
    *pdVar5 = dVar11;
  }
  pdVar5 = (double *)(*piVar1 + -4);
  *piVar1 = (int)pdVar5;
  if (in_FPSCR_SZ == '\0') {
    dVar9 = (double)CONCAT44((int)((ulonglong)dVar9 >> 0x20),*puVar4);
  }
  if (in_FPSCR_PR == '\0') {
    dVar16 = (double)CONCAT44(fVar17 + SUB84(dVar9,0),uVar15);
  }
  else {
    dVar16 = dVar16 + dVar9;
  }
  if (in_FPSCR_SZ == '\0') {
    *(int *)pdVar5 = (int)((ulonglong)dVar16 >> 0x20);
  }
  else {
    *pdVar5 = dVar16;
  }
  uVar13 = DAT_8c640a44;
  iVar6 = *piVar1;
  *piVar1 = iVar6 + -4;
  *(undefined4 *)(iVar6 + -4) = uVar13;
  *piVar1 = *piVar1 + 0x20;
  return;
}

