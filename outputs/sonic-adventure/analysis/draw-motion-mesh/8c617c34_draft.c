/* Ghidra draft only; not buildable/verified source. */

/* WARNING: Restarted to delay deadcode elimination for space: stack */

void FUN_8c617c34(void)

{
  double *pdVar1;
  double *pdVar2;
  float *pfVar3;
  float *pfVar4;
  double *pdVar5;
  undefined *puVar6;
  double *pdVar7;
  int iVar8;
  double *pdVar9;
  double *pdVar10;
  double *pdVar11;
  double *pdVar12;
  undefined8 *puVar13;
  double *pdVar14;
  float *pfVar15;
  undefined4 in_fr1;
  double dVar16;
  undefined8 uVar17;
  float fVar18;
  float fVar19;
  double dVar20;
  double dVar21;
  double in_xd2;
  char in_FPSCR_PR;
  char in_FPSCR_SZ;
  float local_5c;
  float fStack_58;
  float local_54;
  float fStack_50;
  undefined4 local_4c;
  undefined4 uStack_48;
  float local_44;
  undefined4 uStack_40;
  undefined8 local_3c;
  float local_34;
  float fStack_30;
  
  pdVar5 = DAT_8c617e1c;
  puVar13 = (undefined8 *)*DAT_8c617e14;
  pdVar9 = (double *)(DAT_8c617e18 + 8);
  pdVar10 = (double *)(puVar13 + 1);
  pdVar12 = (double *)(DAT_8c617e18 + 4);
  pdVar14 = (double *)((int)puVar13 + 4);
  if (in_FPSCR_SZ == '\0') {
    dVar16 = (double)CONCAT44(*(undefined4 *)pdVar9,in_fr1);
    dVar20 = (double)CONCAT44(*(undefined4 *)pdVar14,*(undefined4 *)pdVar10);
  }
  else {
    dVar16 = *pdVar9;
    in_xd2 = *pdVar10;
    dVar20 = *pdVar14;
  }
  pdVar11 = (double *)(DAT_8c617e18 + 5);
  if (in_FPSCR_PR == '\0') {
    dVar20 = (double)CONCAT44((int)((ulonglong)dVar20 >> 0x20),
                              SUB84(dVar20,0) * (float)((ulonglong)dVar16 >> 0x20));
  }
  else {
    dVar20 = dVar20 * dVar16;
  }
  if (in_FPSCR_SZ == '\0') {
    dVar16 = (double)CONCAT44(*(undefined4 *)pdVar12,*(undefined4 *)puVar13);
  }
  else {
    dVar16 = *pdVar12;
  }
  if (in_FPSCR_PR == '\0') {
    dVar20 = (double)(ulonglong)
                     (uint)((float)((ulonglong)dVar16 >> 0x20) * (float)((ulonglong)dVar20 >> 0x20)
                           + SUB84(dVar20,0));
  }
  local_5c = SUB84(dVar20,0);
  if (in_FPSCR_SZ == '\0') {
    uVar17 = CONCAT44(*DAT_8c617e18,SUB84(dVar16,0));
  }
  else {
    uVar17 = *(undefined8 *)DAT_8c617e18;
  }
  if (in_FPSCR_PR == '\0') {
    local_5c = (float)((ulonglong)uVar17 >> 0x20) * (float)uVar17 + local_5c;
  }
  if (in_FPSCR_SZ == '\0') {
    dVar16 = (double)CONCAT44(DAT_8c617e18[9],(float)uVar17);
    dVar20 = (double)CONCAT44(*(undefined4 *)pdVar14,*(undefined4 *)pdVar10);
  }
  else {
    local_5c = SUB84(in_xd2,0);
    in_xd2 = *pdVar10;
    dVar16 = *(double *)(DAT_8c617e18 + 9);
    dVar20 = *pdVar14;
  }
  pfVar4 = DAT_8c617e18 + 9;
  pfVar3 = DAT_8c617e18 + 1;
  pdVar2 = (double *)(DAT_8c617e18 + 10);
  pdVar1 = (double *)(DAT_8c617e18 + 6);
  if (in_FPSCR_PR == '\0') {
    dVar20 = (double)CONCAT44((int)((ulonglong)dVar20 >> 0x20),
                              SUB84(dVar20,0) * (float)((ulonglong)dVar16 >> 0x20));
  }
  else {
    dVar20 = dVar20 * dVar16;
  }
  if (in_FPSCR_SZ == '\0') {
    dVar16 = (double)CONCAT44(*(float *)pdVar11,*pfVar3);
  }
  else {
    dVar16 = *pdVar11;
  }
  if (in_FPSCR_PR == '\0') {
    dVar20 = (double)(ulonglong)
                     (uint)((float)((ulonglong)dVar16 >> 0x20) * (float)((ulonglong)dVar20 >> 0x20)
                           + SUB84(dVar20,0));
  }
  fStack_58 = SUB84(dVar20,0);
  if (in_FPSCR_SZ == '\0') {
    uVar17 = CONCAT44(*(undefined4 *)puVar13,SUB84(dVar16,0));
  }
  else {
    uVar17 = *puVar13;
  }
  if (in_FPSCR_PR == '\0') {
    fStack_58 = (float)((ulonglong)uVar17 >> 0x20) * (float)uVar17 + fStack_58;
  }
  if (in_FPSCR_SZ == '\0') {
    dVar16 = (double)CONCAT44(*(undefined4 *)pdVar2,(float)uVar17);
    dVar20 = (double)CONCAT44(*(undefined4 *)pdVar14,*(undefined4 *)pdVar10);
  }
  else {
    fStack_58 = SUB84(in_xd2,0);
    dVar16 = *pdVar2;
    in_xd2 = *pdVar10;
    dVar20 = *pdVar14;
  }
  if (in_FPSCR_PR == '\0') {
    dVar20 = (double)CONCAT44((int)((ulonglong)dVar20 >> 0x20),
                              SUB84(dVar20,0) * (float)((ulonglong)dVar16 >> 0x20));
  }
  else {
    dVar20 = dVar20 * dVar16;
  }
  if (in_FPSCR_SZ == '\0') {
    fVar18 = *(float *)pdVar1;
  }
  else {
    fVar18 = (float)((ulonglong)*pdVar1 >> 0x20);
  }
  if (in_FPSCR_PR == '\0') {
    dVar20 = (double)(ulonglong)
                     (uint)(fVar18 * (float)((ulonglong)dVar20 >> 0x20) + SUB84(dVar20,0));
  }
  local_54 = SUB84(dVar20,0);
  pfVar15 = DAT_8c617e18 + 2;
  if (in_FPSCR_SZ == '\0') {
    uVar17 = CONCAT44(*(undefined4 *)puVar13,*pfVar15);
  }
  else {
    uVar17 = *puVar13;
  }
  pdVar10 = (double *)(puVar13 + 3);
  if (in_FPSCR_PR == '\0') {
    local_54 = (float)((ulonglong)uVar17 >> 0x20) * (float)uVar17 + local_54;
  }
  pdVar14 = (double *)((int)puVar13 + 0x14);
  if (in_FPSCR_SZ == '\0') {
    dVar16 = (double)CONCAT44(*(undefined4 *)pdVar9,(float)uVar17);
    dVar20 = (double)CONCAT44(*(undefined4 *)pdVar14,*(undefined4 *)pdVar10);
  }
  else {
    local_54 = SUB84(in_xd2,0);
    fStack_50 = (float)((ulonglong)in_xd2 >> 0x20);
    in_xd2 = *pdVar10;
    dVar16 = *pdVar9;
    dVar20 = *pdVar14;
  }
  pdVar7 = (double *)(puVar13 + 2);
  if (in_FPSCR_PR == '\0') {
    dVar20 = (double)CONCAT44((int)((ulonglong)dVar20 >> 0x20),
                              SUB84(dVar20,0) * (float)((ulonglong)dVar16 >> 0x20));
  }
  else {
    dVar20 = dVar20 * dVar16;
  }
  if (in_FPSCR_SZ == '\0') {
    fVar18 = *(float *)pdVar12;
  }
  else {
    fVar18 = (float)((ulonglong)*pdVar12 >> 0x20);
  }
  if (in_FPSCR_PR == '\0') {
    dVar20 = (double)(ulonglong)
                     (uint)(fVar18 * (float)((ulonglong)dVar20 >> 0x20) + SUB84(dVar20,0));
  }
  if (in_FPSCR_SZ == '\0') {
    fVar18 = *DAT_8c617e18;
    dVar16 = (double)CONCAT44(*(undefined4 *)pdVar7,SUB84(dVar20,0));
  }
  else {
    fVar18 = (float)((ulonglong)*(undefined8 *)DAT_8c617e18 >> 0x20);
    dVar16 = *pdVar7;
  }
  if (in_FPSCR_PR == '\0') {
    fVar19 = (float)((ulonglong)dVar16 >> 0x20);
    dVar16 = (double)CONCAT44(fVar19,fVar18 * fVar19 + SUB84(dVar16,0));
  }
  if (in_FPSCR_SZ == '\0') {
    local_4c = SUB84(dVar16,0);
    dVar16 = (double)CONCAT44((int)((ulonglong)dVar16 >> 0x20),*(undefined4 *)pdVar14);
    dVar20 = (double)CONCAT44(*(float *)pdVar11,*(undefined4 *)pdVar10);
  }
  else {
    local_4c = SUB84(in_xd2,0);
    in_xd2 = *pdVar14;
    dVar20 = *pdVar11;
  }
  if (in_FPSCR_PR == '\0') {
    fVar18 = SUB84(dVar16,0) * (float)((ulonglong)dVar20 >> 0x20);
  }
  else {
    fVar18 = SUB84(dVar16 * dVar20,0);
  }
  if (in_FPSCR_SZ == '\0') {
    uVar17 = CONCAT44(*pfVar3,SUB84(dVar20,0));
    dVar16 = (double)CONCAT44(*(undefined4 *)pdVar7,fVar18);
  }
  else {
    uVar17 = *(undefined8 *)pfVar3;
    dVar16 = *pdVar7;
  }
  if (in_FPSCR_PR == '\0') {
    fVar18 = (float)((ulonglong)dVar16 >> 0x20);
    dVar16 = (double)CONCAT44(fVar18,(float)((ulonglong)uVar17 >> 0x20) * fVar18 + SUB84(dVar16,0));
  }
  if (in_FPSCR_SZ == '\0') {
    uVar17 = CONCAT44(*pfVar4,(int)uVar17);
  }
  else {
    uVar17 = *(undefined8 *)pfVar4;
  }
  if (in_FPSCR_PR == '\0') {
    dVar16 = (double)CONCAT44((int)((ulonglong)dVar16 >> 0x20),
                              (float)((ulonglong)uVar17 >> 0x20) * (float)uVar17 + SUB84(dVar16,0));
  }
  if (in_FPSCR_SZ == '\0') {
    uStack_48 = SUB84(dVar16,0);
    dVar20 = (double)CONCAT44(*(undefined4 *)pdVar1,(float)uVar17);
    dVar16 = (double)CONCAT44((int)((ulonglong)dVar16 >> 0x20),*(undefined4 *)pdVar14);
  }
  else {
    uStack_48 = SUB84(in_xd2,0);
    dVar20 = *pdVar1;
    in_xd2 = *pdVar14;
  }
  if (in_FPSCR_PR == '\0') {
    fVar18 = SUB84(dVar16,0) * (float)((ulonglong)dVar20 >> 0x20);
  }
  else {
    fVar18 = SUB84(dVar16 * dVar20,0);
  }
  if (in_FPSCR_SZ == '\0') {
    fVar19 = *pfVar15;
    dVar16 = (double)CONCAT44(*(undefined4 *)pdVar7,fVar18);
  }
  else {
    fVar19 = (float)((ulonglong)*(undefined8 *)pfVar15 >> 0x20);
    dVar16 = *pdVar7;
  }
  if (in_FPSCR_PR == '\0') {
    dVar16 = (double)(ulonglong)
                     (uint)(fVar19 * (float)((ulonglong)dVar16 >> 0x20) + SUB84(dVar16,0));
  }
  local_44 = SUB84(dVar16,0);
  if (in_FPSCR_SZ == '\0') {
    dVar16 = (double)CONCAT44(*(undefined4 *)pdVar2,*(undefined4 *)pdVar10);
  }
  else {
    dVar16 = *pdVar2;
  }
  if (in_FPSCR_PR == '\0') {
    local_44 = (float)((ulonglong)dVar16 >> 0x20) * SUB84(dVar16,0) + local_44;
  }
  if (in_FPSCR_SZ == '\0') {
    dVar16 = (double)CONCAT44(*(undefined4 *)pdVar12,*(undefined4 *)(puVar13 + 5));
    dVar20 = (double)CONCAT44(*(undefined4 *)pdVar9,*(undefined4 *)((int)puVar13 + 0x24));
  }
  else {
    local_44 = SUB84(in_xd2,0);
    uStack_40 = (undefined4)((ulonglong)in_xd2 >> 0x20);
    dVar16 = *pdVar12;
    in_xd2 = (double)puVar13[5];
    dVar20 = *pdVar9;
  }
  pdVar10 = (double *)((int)puVar13 + 0x24);
  pdVar9 = (double *)(puVar13 + 4);
  if (in_FPSCR_PR == '\0') {
    dVar16 = (double)CONCAT44((int)((ulonglong)dVar16 >> 0x20),
                              SUB84(dVar16,0) * (float)((ulonglong)dVar20 >> 0x20));
  }
  else {
    dVar16 = dVar16 * dVar20;
  }
  if (in_FPSCR_SZ == '\0') {
    dVar21 = (double)CONCAT44(*(undefined4 *)pdVar9,SUB84(dVar16,0));
    dVar16 = (double)CONCAT44((int)((ulonglong)dVar16 >> 0x20),SUB84(dVar20,0));
  }
  else {
    dVar21 = *pdVar9;
  }
  if (in_FPSCR_PR == '\0') {
    dVar21 = (double)CONCAT44((int)((ulonglong)dVar21 >> 0x20),
                              (float)((ulonglong)dVar16 >> 0x20) * SUB84(dVar16,0) + SUB84(dVar21,0)
                             );
  }
  if (in_FPSCR_SZ == '\0') {
    uVar17 = CONCAT44(*DAT_8c617e18,SUB84(dVar16,0));
  }
  else {
    uVar17 = *(undefined8 *)DAT_8c617e18;
  }
  if (in_FPSCR_PR == '\0') {
    fVar18 = (float)((ulonglong)dVar21 >> 0x20);
    dVar21 = (double)CONCAT44(fVar18,(float)((ulonglong)uVar17 >> 0x20) * fVar18 + SUB84(dVar21,0));
  }
  if (in_FPSCR_SZ == '\0') {
    local_3c = (double)((ulonglong)dVar21 & 0xffffffff);
    dVar21 = (double)CONCAT44((int)((ulonglong)dVar21 >> 0x20),*(undefined4 *)pdVar10);
    dVar20 = (double)CONCAT44(*(float *)pdVar11,(int)uVar17);
    dVar16 = in_xd2;
  }
  else {
    dVar16 = *pdVar10;
    dVar20 = *pdVar11;
    local_3c = in_xd2;
  }
  if (in_FPSCR_PR == '\0') {
    fVar18 = SUB84(dVar21,0) * (float)((ulonglong)dVar20 >> 0x20);
  }
  else {
    fVar18 = SUB84(dVar21 * dVar20,0);
  }
  if (in_FPSCR_SZ == '\0') {
    dVar20 = (double)CONCAT44(*(undefined4 *)pdVar9,fVar18);
    uVar17 = CONCAT44(*pfVar3,*(undefined4 *)(puVar13 + 5));
  }
  else {
    uVar17 = *(undefined8 *)pfVar3;
    dVar20 = *pdVar9;
  }
  if (in_FPSCR_PR == '\0') {
    dVar20 = (double)(ulonglong)
                     (uint)((float)((ulonglong)uVar17 >> 0x20) * (float)((ulonglong)dVar20 >> 0x20)
                           + SUB84(dVar20,0));
  }
  fVar18 = SUB84(dVar20,0);
  if (in_FPSCR_SZ == '\0') {
    uVar17 = CONCAT44(*pfVar4,(int)uVar17);
  }
  else {
    uVar17 = *(undefined8 *)pfVar4;
  }
  if (in_FPSCR_PR == '\0') {
    fVar18 = (float)((ulonglong)uVar17 >> 0x20) * (float)uVar17 + fVar18;
  }
  if (in_FPSCR_SZ != '\0') {
    local_3c._4_4_ = SUB84(dVar16,0);
    fVar18 = local_3c._4_4_;
  }
  local_3c = (double)CONCAT44(fVar18,(undefined4)local_3c);
  if (in_FPSCR_SZ == '\0') {
    dVar20 = (double)CONCAT44(*(undefined4 *)pdVar1,(float)uVar17);
    dVar21 = (double)CONCAT44(*(undefined4 *)pdVar9,*(undefined4 *)pdVar10);
  }
  else {
    dVar20 = *pdVar1;
    dVar16 = *pdVar10;
    dVar21 = *pdVar9;
  }
  if (in_FPSCR_PR == '\0') {
    dVar21 = (double)CONCAT44((int)((ulonglong)dVar21 >> 0x20),
                              SUB84(dVar21,0) * (float)((ulonglong)dVar20 >> 0x20));
  }
  else {
    dVar21 = dVar21 * dVar20;
  }
  if (in_FPSCR_SZ == '\0') {
    uVar17 = CONCAT44(*pfVar15,*(undefined4 *)(puVar13 + 5));
  }
  else {
    uVar17 = *(undefined8 *)pfVar15;
  }
  if (in_FPSCR_PR == '\0') {
    dVar21 = (double)(ulonglong)
                     (uint)((float)((ulonglong)uVar17 >> 0x20) * (float)((ulonglong)dVar21 >> 0x20)
                           + SUB84(dVar21,0));
  }
  local_34 = SUB84(dVar21,0);
  if (in_FPSCR_SZ == '\0') {
    dVar20 = (double)CONCAT44(*(undefined4 *)pdVar2,(int)uVar17);
  }
  else {
    dVar20 = *pdVar2;
  }
  if (in_FPSCR_PR == '\0') {
    local_34 = (float)((ulonglong)dVar20 >> 0x20) * SUB84(dVar20,0) + local_34;
  }
  if (in_FPSCR_SZ == '\0') {
    dVar20 = (double)CONCAT44(*(undefined4 *)(puVar13 + 10),SUB84(dVar20,0));
    dVar21 = (double)CONCAT44(local_5c,local_4c);
  }
  else {
    local_34 = SUB84(dVar16,0);
    fStack_30 = (float)((ulonglong)dVar16 >> 0x20);
    dVar20 = (double)puVar13[10];
    dVar16 = (double)CONCAT44(uStack_48,local_4c);
    dVar21 = (double)CONCAT44(fStack_58,local_5c);
  }
  if (in_FPSCR_PR == '\0') {
    dVar21 = (double)CONCAT44((int)((ulonglong)dVar21 >> 0x20),
                              SUB84(dVar21,0) * (float)((ulonglong)dVar20 >> 0x20));
  }
  else {
    dVar21 = dVar21 * dVar20;
  }
  if (in_FPSCR_SZ == '\0') {
    uVar17 = CONCAT44(*(undefined4 *)((int)puVar13 + 0x4c),SUB84(dVar20,0));
  }
  else {
    uVar17 = *(undefined8 *)((int)puVar13 + 0x4c);
  }
  if (in_FPSCR_PR == '\0') {
    dVar21 = (double)(ulonglong)
                     (uint)((float)((ulonglong)uVar17 >> 0x20) * (float)((ulonglong)dVar21 >> 0x20)
                           + SUB84(dVar21,0));
  }
  if (in_FPSCR_SZ == '\0') {
    local_3c = (double)CONCAT44((undefined4)local_3c,SUB84(dVar21,0));
    uVar17 = CONCAT44(*(undefined4 *)((int)puVar13 + 0x54),(int)uVar17);
  }
  else {
    uVar17 = *(undefined8 *)((int)puVar13 + 0x54);
  }
  if (in_FPSCR_PR == '\0') {
    fVar19 = (float)((ulonglong)local_3c >> 0x20);
    local_3c = (double)CONCAT44(fVar19,(float)((ulonglong)uVar17 >> 0x20) * fVar19 +
                                       SUB84(local_3c,0));
  }
  if (in_FPSCR_SZ == '\0') {
    *(int *)DAT_8c617e1c = SUB84(local_3c,0);
    dVar20 = (double)CONCAT44(*(undefined4 *)(puVar13 + 10),(int)uVar17);
    local_3c = (double)CONCAT44((int)((ulonglong)local_3c >> 0x20),uStack_48);
  }
  else {
    *DAT_8c617e1c = dVar16;
    dVar20 = (double)puVar13[10];
    dVar16 = (double)CONCAT44(local_44,uStack_48);
  }
  if (in_FPSCR_PR == '\0') {
    fVar19 = SUB84(local_3c,0) * (float)((ulonglong)dVar20 >> 0x20);
  }
  else {
    fVar19 = SUB84(local_3c * dVar20,0);
  }
  if (in_FPSCR_SZ == '\0') {
    uVar17 = CONCAT44(*(undefined4 *)((int)puVar13 + 0x4c),SUB84(dVar20,0));
  }
  else {
    uVar17 = *(undefined8 *)((int)puVar13 + 0x4c);
    fVar19 = fStack_58;
    fStack_58 = local_54;
  }
  if (in_FPSCR_PR == '\0') {
    fVar19 = (float)((ulonglong)uVar17 >> 0x20) * fStack_58 + fVar19;
  }
  if (in_FPSCR_SZ == '\0') {
    dVar20 = (double)CONCAT44(fVar18,fVar19);
    uVar17 = CONCAT44(*(undefined4 *)((int)puVar13 + 0x54),(int)uVar17);
  }
  else {
    dVar20 = (double)CONCAT44(local_34,fVar18);
    uVar17 = *(undefined8 *)((int)puVar13 + 0x54);
  }
  if (in_FPSCR_PR == '\0') {
    fVar18 = (float)((ulonglong)dVar20 >> 0x20);
    dVar20 = (double)CONCAT44(fVar18,(float)((ulonglong)uVar17 >> 0x20) * fVar18 + SUB84(dVar20,0));
  }
  if (in_FPSCR_SZ == '\0') {
    *(int *)((int)pdVar5 + 4) = SUB84(dVar20,0);
    dVar21 = (double)CONCAT44(*(undefined4 *)(puVar13 + 10),(int)uVar17);
    dVar20 = (double)CONCAT44((int)((ulonglong)dVar20 >> 0x20),local_44);
  }
  else {
    *(double *)((int)pdVar5 + 4) = dVar16;
    dVar21 = (double)puVar13[10];
    dVar16 = (double)CONCAT44(uStack_40,local_44);
  }
  if (in_FPSCR_PR == '\0') {
    fVar18 = SUB84(dVar20,0) * (float)((ulonglong)dVar21 >> 0x20);
  }
  else {
    fVar18 = SUB84(dVar20 * dVar21,0);
  }
  if (in_FPSCR_SZ == '\0') {
    fVar19 = *(float *)((int)puVar13 + 0x4c);
    fStack_50 = local_54;
  }
  else {
    fVar19 = (float)((ulonglong)*(undefined8 *)((int)puVar13 + 0x4c) >> 0x20);
    fVar18 = local_54;
  }
  if (in_FPSCR_PR == '\0') {
    fVar18 = fVar19 * fStack_50 + fVar18;
  }
  if (in_FPSCR_SZ == '\0') {
    fVar19 = *(float *)((int)puVar13 + 0x54);
  }
  else {
    fVar19 = (float)((ulonglong)*(undefined8 *)((int)puVar13 + 0x54) >> 0x20);
    fVar18 = local_34;
    local_34 = fStack_30;
  }
  if (in_FPSCR_PR == '\0') {
    fVar18 = fVar19 * local_34 + fVar18;
  }
  if (in_FPSCR_SZ == '\0') {
    *(float *)(pdVar5 + 1) = fVar18;
  }
  else {
    pdVar5[1] = dVar16;
  }
  puVar6 = PTR_FUN_8c617eb0;
  iVar8 = (int)DAT_8c617ea2;
  if (in_FPSCR_SZ == '\0') {
    *(undefined4 *)DAT_8c617ea8 = *(undefined4 *)(iVar8 + (int)puVar13);
    *(undefined4 *)DAT_8c617eac = *(undefined4 *)(iVar8 + -4 + (int)puVar13);
  }
  else {
    *DAT_8c617ea8 = *(undefined8 *)(iVar8 + (int)puVar13);
    *DAT_8c617eac = *(undefined8 *)(iVar8 + -4 + (int)puVar13);
  }
  (*(code *)puVar6)();
  return;
}

