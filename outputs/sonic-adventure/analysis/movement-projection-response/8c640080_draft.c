/* Ghidra draft only; not buildable/verified source. */

/* WARNING: Removing unreachable block (ram,0x8c6400e0) */

double FUN_8c640080(undefined4 param_1,undefined4 param_2,double *param_3,double *param_4,
                   double *param_5)

{
  undefined8 *puVar1;
  float fVar2;
  double in_dr0;
  double dVar3;
  double dVar4;
  undefined8 uVar5;
  double dVar6;
  float fVar7;
  double dVar8;
  float fVar10;
  float fVar11;
  double in_dr14;
  undefined4 uVar13;
  double dVar12;
  double in_xd2;
  double dVar14;
  double in_xd4;
  undefined8 in_xd6;
  double in_xd14;
  undefined4 in_PR;
  char in_FPSCR_PR;
  char in_FPSCR_SZ;
  undefined1 auStack_10 [8];
  undefined8 local_8;
  undefined4 uVar9;
  
  uVar13 = (undefined4)((ulonglong)in_dr14 >> 0x20);
  if (in_FPSCR_SZ == '\0') {
    local_8 = (double)CONCAT44(SUB84(in_dr14,0),(undefined4)local_8);
    puVar1 = &local_8;
                    /* WARNING: Ignoring partial resolution of indirect */
    local_8._0_4_ = uVar13;
  }
  else {
    puVar1 = (undefined8 *)auStack_10;
    local_8 = in_xd14;
  }
  *(undefined4 *)((int)puVar1 + -4) = in_PR;
  if (in_FPSCR_SZ == '\0') {
    uVar9 = *(undefined4 *)((int)param_4 + 0xc);
    param_1 = *(undefined4 *)(param_4 + 2);
    dVar8 = (double)CONCAT44(uVar9,*(undefined4 *)param_4);
    dVar12 = (double)CONCAT44(uVar9,param_2);
  }
  else {
    dVar8 = *(double *)((int)param_4 + 0xc);
    in_xd2 = *param_4;
    in_xd4 = param_4[2];
    uVar9 = (undefined4)((ulonglong)dVar8 >> 0x20);
    dVar12 = dVar8;
  }
  if (in_FPSCR_PR == '\0') {
    dVar8 = (double)CONCAT44((float)((ulonglong)dVar8 >> 0x20) * SUB84(dVar8,0),SUB84(dVar8,0));
  }
  else {
    dVar8 = dVar8 * dVar8;
  }
  if (in_FPSCR_SZ == '\0') {
    dVar8 = (double)CONCAT44((int)((ulonglong)dVar8 >> 0x20),*(undefined4 *)((int)param_4 + 4));
    in_dr0 = (double)CONCAT44((int)((ulonglong)in_dr0 >> 0x20),param_1);
  }
  else {
    in_xd2 = *(double *)((int)param_4 + 4);
  }
  if (in_FPSCR_PR == '\0') {
    fVar2 = SUB84(in_dr0,0) * SUB84(dVar8,0);
  }
  else {
    fVar2 = SUB84(in_dr0 * dVar8,0);
  }
  if (in_FPSCR_SZ == '\0') {
    dVar12 = (double)CONCAT44(uVar9,*(undefined4 *)((int)param_4 + 0x14));
  }
  else {
    in_xd6 = *(undefined8 *)((int)param_4 + 0x14);
  }
  if (in_FPSCR_PR == '\0') {
    dVar8 = (double)CONCAT44(-(float)((ulonglong)dVar8 >> 0x20),SUB84(dVar8,0));
  }
  else {
    dVar8 = -dVar8;
  }
  if (in_FPSCR_SZ == '\0') {
    dVar8 = (double)CONCAT44((int)((ulonglong)dVar8 >> 0x20),*(undefined4 *)(param_4 + 1));
    dVar3 = (double)CONCAT44(*(undefined4 *)((int)param_3 + 4),fVar2);
  }
  else {
    in_xd2 = param_4[1];
    dVar3 = *(double *)((int)param_3 + 4);
  }
  if (in_FPSCR_PR == '\0') {
    dVar8 = (double)CONCAT44((float)((ulonglong)dVar8 >> 0x20) - SUB84(dVar3,0),SUB84(dVar8,0));
  }
  else {
    dVar8 = dVar8 - dVar3;
  }
  fVar2 = SUB84(dVar12,0);
  if (in_FPSCR_SZ == '\0') {
    dVar3 = (double)CONCAT44((int)((ulonglong)dVar3 >> 0x20),fVar2);
  }
  if (in_FPSCR_PR == '\0') {
    dVar3 = (double)CONCAT44((int)((ulonglong)dVar3 >> 0x20),SUB84(dVar3,0) * SUB84(dVar8,0));
  }
  else {
    dVar3 = dVar3 * dVar8;
  }
  dVar14 = in_xd4;
  if (in_FPSCR_SZ == '\0') {
    dVar8 = (double)CONCAT44((int)((ulonglong)dVar8 >> 0x20),param_1);
    dVar14 = in_xd2;
  }
  if (in_FPSCR_PR == '\0') {
    dVar8 = (double)CONCAT44((int)((ulonglong)dVar8 >> 0x20),
                             SUB84(dVar8,0) * (float)((ulonglong)dVar3 >> 0x20));
  }
  else {
    dVar8 = dVar8 * dVar3;
  }
  fVar10 = (float)((ulonglong)dVar12 >> 0x20);
  if (in_FPSCR_SZ == '\0') {
    dVar6 = (double)CONCAT44(*(undefined4 *)param_3,SUB84(dVar8,0));
    dVar8 = (double)CONCAT44((int)((ulonglong)dVar8 >> 0x20),param_1);
    dVar3 = (double)CONCAT44(fVar10,SUB84(dVar3,0));
  }
  else {
    dVar6 = *param_3;
    dVar3 = dVar12;
  }
  if (in_FPSCR_PR == '\0') {
    dVar8 = (double)CONCAT44((float)((ulonglong)dVar8 >> 0x20) - SUB84(dVar3,0),SUB84(dVar8,0));
    fVar11 = (float)((ulonglong)dVar6 >> 0x20);
    dVar6 = (double)CONCAT44(fVar11,(float)((ulonglong)dVar3 >> 0x20) * fVar11 + SUB84(dVar6,0));
  }
  else {
    dVar8 = dVar8 - dVar3;
  }
  if (in_FPSCR_SZ == '\0') {
    dVar3 = (double)(ulonglong)*(uint *)(param_3 + 1);
  }
  uVar5 = in_xd6;
  if (in_FPSCR_SZ == '\0') {
    uVar5 = CONCAT44(fVar2,SUB84(dVar3,0));
  }
  if (in_FPSCR_PR == '\0') {
    dVar6 = (double)CONCAT44((int)((ulonglong)dVar6 >> 0x20),
                             (float)((ulonglong)uVar5 >> 0x20) * (float)uVar5 + SUB84(dVar6,0));
  }
  dVar3 = dVar14;
  if (in_FPSCR_SZ == '\0') {
    dVar12 = (double)((ulonglong)(uint)fVar10 << 0x20);
    in_dr14 = (double)CONCAT44(uVar13,SUB84(dVar6,0));
    dVar3 = in_xd14;
  }
  if (in_FPSCR_PR == '\0') {
    fVar11 = SUB84(in_dr14,0) + (float)((ulonglong)dVar8 >> 0x20);
  }
  else {
    fVar11 = SUB84(in_dr14 + dVar8,0);
  }
  fVar7 = SUB84(dVar8,0);
  if (in_FPSCR_SZ == '\0') {
    dVar6 = (double)CONCAT44((int)((ulonglong)dVar6 >> 0x20),fVar7);
    in_xd4 = dVar14;
  }
  if (in_FPSCR_PR == '\0') {
    dVar6 = (double)CONCAT44((int)((ulonglong)dVar6 >> 0x20),
                             (float)((ulonglong)dVar12 >> 0x20) * fVar10 + SUB84(dVar6,0) * fVar7);
  }
  else {
    dVar6 = dVar6 * dVar8;
  }
  fVar10 = fVar2;
  if (in_FPSCR_SZ != '\0') {
    fVar10 = (float)((ulonglong)in_xd6 >> 0x20);
  }
  if (in_FPSCR_PR == '\0') {
    dVar6 = (double)CONCAT44((int)((ulonglong)dVar6 >> 0x20),fVar10 * fVar2 + SUB84(dVar6,0));
  }
  dVar12 = in_xd4;
  if (in_FPSCR_SZ == '\0') {
    dVar12 = (double)CONCAT44(SUB84(dVar6,0),fVar11);
  }
  fVar2 = (float)((ulonglong)dVar12 >> 0x20);
  if (param_5 != (double *)0x0) {
    dVar14 = dVar3;
    if (in_FPSCR_SZ == '\0') {
      dVar6 = (double)((ulonglong)dVar12 & 0xffffffff);
      dVar14 = in_xd4;
    }
    dVar8 = dVar14;
    if (in_FPSCR_SZ == '\0') {
      dVar8 = (double)CONCAT44(-SUB84(dVar6,0),fVar7);
    }
    if (in_FPSCR_PR == '\0') {
      dVar8 = (double)CONCAT44((float)((ulonglong)dVar8 >> 0x20) / fVar2,SUB84(dVar8,0));
    }
    else {
      dVar8 = dVar8 / dVar12;
    }
    if (in_FPSCR_SZ == '\0') {
      dVar6 = (double)CONCAT44(*(undefined4 *)param_3,*(undefined4 *)((int)param_4 + 0xc));
      dVar4 = (double)((ulonglong)dVar8 & 0xffffffff00000000);
    }
    else {
      dVar14 = *(double *)((int)param_4 + 0xc);
      dVar6 = *param_3;
      dVar4 = dVar8;
    }
    fVar10 = (float)((ulonglong)dVar4 >> 0x20);
    if (in_FPSCR_PR == '\0') {
      dVar6 = (double)CONCAT44(fVar10 * SUB84(dVar6,0) + (float)((ulonglong)dVar6 >> 0x20),
                               SUB84(dVar6,0));
    }
    if (in_FPSCR_SZ == '\0') {
      *(int *)param_5 = (int)((ulonglong)dVar6 >> 0x20);
      dVar6 = (double)CONCAT44(*(undefined4 *)(param_4 + 2),*(undefined4 *)((int)param_3 + 4));
    }
    else {
      *param_5 = dVar6;
      dVar14 = *(double *)((int)param_3 + 4);
      dVar6 = param_4[2];
    }
    if (in_FPSCR_PR == '\0') {
      dVar6 = (double)(ulonglong)(uint)(fVar10 * (float)((ulonglong)dVar6 >> 0x20) + SUB84(dVar6,0))
      ;
    }
    if (in_FPSCR_SZ == '\0') {
      *(int *)((int)param_5 + 4) = SUB84(dVar6,0);
      dVar6 = (double)CONCAT44(*(undefined4 *)((int)param_4 + 0x14),*(undefined4 *)(param_3 + 1));
    }
    else {
      *(double *)((int)param_5 + 4) = dVar14;
      dVar14 = param_3[1];
      dVar6 = *(double *)((int)param_4 + 0x14);
    }
    if (in_FPSCR_PR == '\0') {
      fVar11 = (float)((ulonglong)dVar6 >> 0x20);
      dVar6 = (double)CONCAT44(fVar11,fVar10 * fVar11 + SUB84(dVar6,0));
    }
    if (in_FPSCR_SZ == '\0') {
      *(int *)(param_5 + 1) = SUB84(dVar6,0);
    }
    else {
      param_5[1] = dVar14;
    }
  }
  if (in_FPSCR_PR == '\0') {
    dVar12 = (double)CONCAT44(fVar2,ABS(SUB84(dVar12,0)));
  }
  else {
    dVar12 = ABS(dVar12);
  }
  uVar13 = SUB84(dVar12,0);
  if (in_FPSCR_SZ == '\0') {
    dVar12 = (double)((ulonglong)dVar8 & 0xffffffff);
  }
  uVar5 = (*(code *)PTR_FUN_8c640124)(dVar12);
  if (in_FPSCR_SZ == '\0') {
    dVar6 = (double)CONCAT44((int)((ulonglong)dVar6 >> 0x20),(int)((ulonglong)uVar5 >> 0x20));
    dVar3 = (double)CONCAT44(uVar13,(int)uVar5);
  }
  if (in_FPSCR_PR == '\0') {
    dVar3 = (double)CONCAT44((float)((ulonglong)dVar3 >> 0x20) * SUB84(dVar6,0),SUB84(dVar3,0));
  }
  else {
    dVar3 = dVar3 * dVar6;
  }
  return dVar3;
}

