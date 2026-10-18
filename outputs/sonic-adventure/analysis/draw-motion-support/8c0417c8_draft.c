/* Ghidra draft only; not buildable/verified source. */

/* WARNING: Removing unreachable block (ram,0x8c04186e) */
/* WARNING: Removing unreachable block (ram,0x8c041852) */
/* WARNING: Removing unreachable block (ram,0x8c041836) */
/* WARNING: Removing unreachable block (ram,0x8c041842) */
/* WARNING: Removing unreachable block (ram,0x8c04185c) */
/* WARNING: Removing unreachable block (ram,0x8c041878) */
/* WARNING: Restarted to delay deadcode elimination for space: stack */

void FUN_8c0417c8(double param_1,undefined4 param_2)

{
  double *pdVar1;
  double *pdVar2;
  undefined4 unaff_r12;
  undefined4 unaff_r14;
  double in_dr0;
  float fVar4;
  double dVar3;
  double dVar5;
  int iVar7;
  undefined8 uVar6;
  undefined4 uVar8;
  undefined4 uVar9;
  double dVar10;
  float fVar12;
  undefined8 uVar11;
  double dVar13;
  float fVar14;
  double in_xd2;
  double in_xd6;
  double in_xd8;
  int iVar15;
  char in_FPSCR_PR;
  char in_FPSCR_SZ;
  int local_30;
  int local_2c;
  int iStack_24;
  undefined4 local_20;
  undefined8 uStack_1c;
  undefined4 uStack_14;
  undefined4 local_10;
  
  uVar9 = 0x3f800000;
  if (*DAT_8c04190c == 0) {
    if (in_FPSCR_SZ == '\0') {
      param_1 = (double)CONCAT44((int)((ulonglong)param_1 >> 0x20),*(undefined4 *)DAT_8c041904);
      uStack_1c = (double)CONCAT44(*(undefined4 *)((int)DAT_8c041904 + 4),
                                   *(undefined4 *)(DAT_8c041904 + 1));
    }
    else {
      uStack_1c = DAT_8c041904[1];
      in_xd6 = DAT_8c041904[2];
    }
  }
  else {
    if (in_FPSCR_SZ == '\0') {
      pdVar1 = (double *)((int)DAT_8c041904 + 4);
      dVar5 = (double)CONCAT44(*(undefined4 *)DAT_8c041908,*(undefined4 *)DAT_8c041904);
      dVar10 = (double)CONCAT44(*(undefined4 *)((int)DAT_8c041904 + 0xc),param_2);
      pdVar2 = DAT_8c041904 + 2;
      dVar13 = 0.007812501848093234;
    }
    else {
      pdVar1 = DAT_8c041904 + 1;
      dVar5 = *DAT_8c041908;
      dVar10 = *(double *)((int)DAT_8c041904 + 0xc);
      pdVar2 = (double *)((int)DAT_8c041904 + 0x14);
      dVar13 = in_xd8;
    }
    fVar14 = (float)((ulonglong)dVar5 >> 0x20);
    if (in_FPSCR_PR == '\0') {
      dVar13 = (double)CONCAT44((float)((ulonglong)dVar13 >> 0x20) - fVar14,SUB84(dVar13,0));
    }
    else {
      dVar13 = dVar13 - dVar5;
    }
    if (in_FPSCR_SZ == '\0') {
      dVar10 = (double)CONCAT44((int)((ulonglong)dVar10 >> 0x20),*(undefined4 *)pdVar2);
      pdVar2 = (double *)((int)pdVar2 + 4);
    }
    else {
      in_xd6 = *pdVar2;
      pdVar2 = pdVar2 + 1;
    }
    fVar4 = (float)((ulonglong)dVar13 >> 0x20);
    if (in_FPSCR_PR == '\0') {
      fVar12 = (float)((ulonglong)dVar10 >> 0x20) * fVar14;
      dVar10 = (double)CONCAT44(fVar12,SUB84(dVar10,0));
      param_1 = (double)CONCAT44(fVar14,SUB84(dVar5,0) * fVar4 + fVar12);
    }
    else {
      dVar10 = dVar10 * dVar5 * dVar13;
      param_1 = dVar5 * dVar13 + dVar10;
    }
    if (in_FPSCR_SZ == '\0') {
      dVar5 = (double)CONCAT44(*(undefined4 *)pdVar1,SUB84(dVar10,0));
      pdVar1 = (double *)((int)pdVar1 + 4);
    }
    else {
      dVar5 = *pdVar1;
      pdVar1 = pdVar1 + 1;
    }
    fVar14 = (float)((ulonglong)param_1 >> 0x20);
    if (in_FPSCR_PR == '\0') {
      fVar12 = SUB84(dVar5,0) * fVar14;
      uStack_1c = (double)CONCAT44((float)((ulonglong)dVar5 >> 0x20) * fVar4 + fVar12,fVar12);
    }
    else {
      uStack_1c = dVar5 * dVar13 * param_1;
      uStack_1c = uStack_1c + uStack_1c;
    }
    if (in_FPSCR_SZ == '\0') {
      uStack_1c = (double)CONCAT44((int)((ulonglong)uStack_1c >> 0x20),*(undefined4 *)pdVar1);
    }
    else {
      in_xd6 = *pdVar1;
    }
    if (in_FPSCR_PR == '\0') {
      uStack_1c = (double)CONCAT44((int)((ulonglong)uStack_1c >> 0x20),SUB84(uStack_1c,0) * fVar4);
    }
    else {
      uStack_1c = uStack_1c * dVar13;
    }
    if (in_FPSCR_SZ == '\0') {
      dVar5 = (double)CONCAT44(*(undefined4 *)pdVar2,SUB84(dVar13,0));
    }
    else {
      dVar5 = *pdVar2;
    }
    if (in_FPSCR_PR == '\0') {
      fVar14 = (float)((ulonglong)dVar5 >> 0x20) * fVar14;
      dVar5 = (double)CONCAT44(fVar14,SUB84(dVar5,0));
      uStack_1c = (double)CONCAT44((int)((ulonglong)uStack_1c >> 0x20),SUB84(uStack_1c,0) + fVar14);
    }
    else {
      dVar5 = dVar5 * param_1;
      uStack_1c = uStack_1c + dVar5;
    }
    uVar9 = SUB84(dVar5,0);
  }
  uVar8 = SUB84(uStack_1c,0);
  if (in_FPSCR_SZ == '\0') {
    uStack_14 = (undefined4)((ulonglong)uStack_1c >> 0x20);
    uStack_1c = (double)((longlong)param_1 << 0x20);
    local_10 = uVar8;
  }
  else {
    uStack_14 = SUB84(in_xd6,0);
    local_10 = (undefined4)((ulonglong)in_xd6 >> 0x20);
  }
  if (*DAT_8c041910 == 0) {
    local_30 = *DAT_8c041914;
    local_2c = DAT_8c041914[1];
    iVar15 = DAT_8c041914[2];
  }
  else {
    if (in_FPSCR_SZ == '\0') {
      param_1 = (double)CONCAT44(*(undefined4 *)DAT_8c041908,uVar9);
    }
    else {
      param_1 = *DAT_8c041908;
    }
    if (in_FPSCR_PR == '\0') {
      fVar14 = (float)((ulonglong)param_1 >> 0x20);
      param_1 = (double)CONCAT44(fVar14,SUB84(param_1,0) - fVar14);
    }
    else {
      param_1 = param_1 - param_1;
    }
    dVar5 = in_xd2;
    if (in_FPSCR_SZ == '\0') {
      dVar5 = (double)CONCAT44((float)*DAT_8c041914,(float)*DAT_8c041914);
    }
    fVar14 = SUB84(param_1,0);
    if (in_FPSCR_PR == '\0') {
      fVar4 = (float)((ulonglong)dVar5 >> 0x20) * fVar14;
    }
    else {
      fVar4 = (float)((ulonglong)(dVar5 * param_1) >> 0x20);
    }
    dVar13 = (double)CONCAT44(fVar4,(float)DAT_8c041914[3]);
    dVar5 = dVar13;
    if (in_FPSCR_SZ == '\0') {
      in_dr0 = (double)CONCAT44((int)((ulonglong)in_dr0 >> 0x20),(float)DAT_8c041914[3]);
      dVar13 = (double)CONCAT44(fVar4,fVar4);
      dVar5 = in_xd2;
    }
    fVar4 = (float)((ulonglong)param_1 >> 0x20);
    if (in_FPSCR_PR == '\0') {
      fVar12 = SUB84(in_dr0,0) * fVar4;
      dVar10 = (double)CONCAT44((int)((ulonglong)in_dr0 >> 0x20),fVar12);
      local_30 = (int)(SUB84(dVar13,0) + fVar12);
    }
    else {
      dVar10 = in_dr0 * param_1;
      local_30 = (int)(dVar13 + dVar10);
    }
    dVar13 = dVar5;
    if (in_FPSCR_SZ == '\0') {
      dVar13 = (double)CONCAT44((float)DAT_8c041914[1],(float)DAT_8c041914[1]);
    }
    if (in_FPSCR_PR == '\0') {
      fVar12 = (float)((ulonglong)dVar13 >> 0x20) * fVar14;
    }
    else {
      fVar12 = (float)((ulonglong)(dVar13 * param_1) >> 0x20);
    }
    dVar13 = (double)CONCAT44(fVar12,(float)DAT_8c041914[4]);
    if (in_FPSCR_SZ == '\0') {
      dVar10 = (double)CONCAT44((int)((ulonglong)dVar10 >> 0x20),(float)DAT_8c041914[4]);
    }
    if (in_FPSCR_PR == '\0') {
      dVar10 = (double)CONCAT44((int)((ulonglong)dVar10 >> 0x20),SUB84(dVar10,0) * fVar4);
    }
    else {
      dVar10 = dVar10 * param_1;
    }
    dVar3 = dVar13;
    if (in_FPSCR_SZ == '\0') {
      dVar13 = (double)CONCAT44(fVar12,fVar12);
      dVar3 = dVar5;
    }
    if (in_FPSCR_PR == '\0') {
      local_2c = (int)(SUB84(dVar13,0) + SUB84(dVar10,0));
    }
    else {
      local_2c = (int)(dVar13 + dVar10);
    }
    if (in_FPSCR_SZ == '\0') {
      dVar3 = (double)CONCAT44((float)DAT_8c041914[2],(float)DAT_8c041914[2]);
    }
    if (in_FPSCR_PR == '\0') {
      fVar14 = (float)((ulonglong)dVar3 >> 0x20) * fVar14;
    }
    else {
      fVar14 = (float)((ulonglong)(dVar3 * param_1) >> 0x20);
    }
    fVar12 = (float)DAT_8c041914[5];
    if (in_FPSCR_SZ == '\0') {
      dVar10 = (double)CONCAT44((int)((ulonglong)dVar10 >> 0x20),fVar12);
    }
    if (in_FPSCR_PR == '\0') {
      dVar10 = (double)CONCAT44((int)((ulonglong)dVar10 >> 0x20),SUB84(dVar10,0) * fVar4);
    }
    else {
      dVar10 = dVar10 * param_1;
    }
    if (in_FPSCR_SZ == '\0') {
      fVar12 = fVar14;
    }
    if (in_FPSCR_PR == '\0') {
      iVar15 = (int)(fVar14 + SUB84(dVar10,0));
    }
    else {
      iVar15 = (int)((double)CONCAT44(fVar12,fVar14) + dVar10);
    }
  }
  if (*DAT_8c041918 == 0) {
    if (in_FPSCR_SZ == '\0') {
      dVar5 = (double)CONCAT44(*(undefined4 *)DAT_8c041b84,SUB84(param_1,0));
      dVar13 = (double)CONCAT44(*(undefined4 *)((int)DAT_8c041b84 + 4),
                                *(undefined4 *)(DAT_8c041b84 + 1));
    }
    else {
      dVar5 = *DAT_8c041b84;
      dVar13 = DAT_8c041b84[1];
      in_xd6 = DAT_8c041b84[2];
    }
  }
  else {
    if (in_FPSCR_SZ == '\0') {
      param_1 = (double)CONCAT44((int)((ulonglong)param_1 >> 0x20),*(undefined4 *)DAT_8c041908);
      in_xd8 = (double)CONCAT44(uVar9,uVar9);
    }
    if (in_FPSCR_PR == '\0') {
      dVar10 = (double)CONCAT44((float)((ulonglong)in_xd8 >> 0x20) - SUB84(param_1,0),
                                SUB84(in_xd8,0));
    }
    else {
      dVar10 = in_xd8 - param_1;
    }
    if (in_FPSCR_SZ == '\0') {
      dVar5 = (double)CONCAT44(*(undefined4 *)DAT_8c041904,SUB84(param_1,0));
      pdVar1 = (double *)((int)DAT_8c041904 + 4);
    }
    else {
      dVar5 = *DAT_8c041904;
      pdVar1 = DAT_8c041904 + 1;
    }
    fVar14 = (float)((ulonglong)dVar10 >> 0x20);
    if (in_FPSCR_PR == '\0') {
      dVar5 = (double)CONCAT44((float)((ulonglong)dVar5 >> 0x20) * fVar14,SUB84(dVar5,0));
    }
    else {
      dVar5 = dVar5 * dVar10;
    }
    if (in_FPSCR_SZ == '\0') {
      dVar13 = (double)CONCAT44(*(undefined4 *)((int)DAT_8c041904 + 0xc),uVar8);
      pdVar2 = DAT_8c041904 + 2;
    }
    else {
      dVar13 = *(double *)((int)DAT_8c041904 + 0xc);
      pdVar2 = (double *)((int)DAT_8c041904 + 0x14);
    }
    if (in_FPSCR_PR == '\0') {
      dVar13 = (double)CONCAT44((float)((ulonglong)dVar13 >> 0x20) * SUB84(dVar5,0),SUB84(dVar13,0))
      ;
    }
    else {
      dVar13 = dVar13 * dVar5;
    }
    if (in_FPSCR_SZ == '\0') {
      dVar13 = (double)CONCAT44((int)((ulonglong)dVar13 >> 0x20),*(undefined4 *)pdVar2);
      pdVar2 = (double *)((int)pdVar2 + 4);
    }
    else {
      in_xd6 = *pdVar2;
      pdVar2 = pdVar2 + 1;
    }
    if (in_FPSCR_PR == '\0') {
      dVar5 = (double)CONCAT44((float)((ulonglong)dVar5 >> 0x20) +
                               (float)((ulonglong)dVar13 >> 0x20),SUB84(dVar5,0));
    }
    else {
      dVar5 = dVar5 + dVar13;
    }
    if (in_FPSCR_SZ == '\0') {
      dVar13 = (double)CONCAT44(*(undefined4 *)pdVar1,SUB84(dVar13,0));
      pdVar1 = (double *)((int)pdVar1 + 4);
    }
    else {
      dVar13 = *pdVar1;
      pdVar1 = pdVar1 + 1;
    }
    if (in_FPSCR_PR == '\0') {
      fVar4 = SUB84(dVar13,0) * SUB84(dVar5,0);
      dVar13 = (double)CONCAT44((float)((ulonglong)dVar13 >> 0x20) * fVar14 + fVar4,fVar4);
    }
    else {
      dVar13 = dVar13 * dVar10 * dVar5;
      dVar13 = dVar13 + dVar13;
    }
    if (in_FPSCR_SZ == '\0') {
      dVar13 = (double)CONCAT44((int)((ulonglong)dVar13 >> 0x20),*(undefined4 *)pdVar1);
    }
    else {
      in_xd6 = *pdVar1;
    }
    if (in_FPSCR_PR == '\0') {
      dVar13 = (double)CONCAT44((int)((ulonglong)dVar13 >> 0x20),SUB84(dVar13,0) * fVar14);
    }
    else {
      dVar13 = dVar13 * dVar10;
    }
    if (in_FPSCR_SZ == '\0') {
      dVar10 = (double)CONCAT44(*(undefined4 *)pdVar2,SUB84(dVar10,0));
    }
    else {
      dVar10 = *pdVar2;
    }
    if (in_FPSCR_PR == '\0') {
      dVar13 = (double)CONCAT44((int)((ulonglong)dVar13 >> 0x20),
                                SUB84(dVar13,0) +
                                (float)((ulonglong)dVar10 >> 0x20) * SUB84(dVar5,0));
    }
    else {
      dVar13 = dVar13 + dVar10 * dVar5;
    }
  }
  iVar7 = (int)((ulonglong)dVar5 >> 0x20);
  if (in_FPSCR_SZ == '\0') {
    local_20 = (undefined4)((ulonglong)dVar13 >> 0x20);
    uVar6 = CONCAT44(uStack_1c._4_4_,uStack_14);
    uVar11 = CONCAT44(local_10,SUB84(dVar13,0));
    iStack_24 = iVar7;
    uStack_1c._0_4_ = SUB84(dVar13,0);
  }
  else {
    local_20 = SUB84(in_xd6,0);
    uStack_1c._0_4_ = (undefined4)((ulonglong)in_xd6 >> 0x20);
    local_30 = SUB84(dVar5,0);
    uVar6 = CONCAT44(uStack_14,uStack_1c._4_4_);
    uVar11 = CONCAT44(unaff_r14,unaff_r12);
    local_2c = iVar7;
  }
  (*(code *)PTR_FUN_8c041b88)(uVar6,uVar11,0);
  uVar9 = (undefined4)uVar11;
  (**(code **)PTR_DAT_8c041b8c)(0,local_30,local_2c,iVar15);
  if (in_FPSCR_SZ == '\0') {
    uVar6 = CONCAT44(iStack_24,local_20);
    uVar11 = CONCAT44((undefined4)uStack_1c,uVar9);
  }
  else {
    uVar6 = CONCAT44(local_20,iStack_24);
    uVar11 = CONCAT44(local_10,uStack_14);
  }
  (*(code *)PTR_FUN_8c041b90)(uVar6,uVar11,0);
  return;
}

