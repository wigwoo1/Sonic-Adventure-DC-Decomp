/* Ghidra draft only; not buildable/verified source. */

/* WARNING: Removing unreachable block (ram,0x8c64fda6) */
/* WARNING: Removing unreachable block (ram,0x8c64fd34) */
/* WARNING: Removing unreachable block (ram,0x8c64fcc0) */
/* WARNING: Removing unreachable block (ram,0x8c64fcce) */
/* WARNING: Removing unreachable block (ram,0x8c64fdb4) */
/* WARNING: Removing unreachable block (ram,0x8c64fd42) */
/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_8c64fc68(undefined4 param_1,double param_2,int param_3,int *param_4,int *param_5)

{
  double *pdVar1;
  int iVar2;
  int iVar3;
  double *pdVar4;
  int iVar5;
  int iVar6;
  float fVar7;
  float fVar8;
  float fVar9;
  float fVar10;
  float fVar11;
  double dVar12;
  undefined4 uVar14;
  double dVar13;
  undefined4 uVar15;
  double dVar16;
  double dVar17;
  double dVar19;
  uint uVar20;
  double in_xd2;
  double in_xd6;
  char in_FPSCR_PR;
  char in_FPSCR_SZ;
  float fVar18;
  
  uVar15 = DAT_8c64fe04;
  pdVar1 = _DAT_8c64fdf8;
  iVar5 = 0;
  param_3 = param_3 + -1;
  if (in_FPSCR_SZ == '\0') {
    fVar18 = *(float *)_DAT_8c64fdf8;
    dVar12 = (double)CONCAT44(*(undefined4 *)DAT_8c64fe00,*(undefined4 *)DAT_8c64fdfc);
    dVar16 = (double)CONCAT44(fVar18,0x3f800000);
    dVar17 = (double)CONCAT44(fVar18,param_1);
  }
  else {
    dVar16 = *_DAT_8c64fdf8;
    in_xd2 = *DAT_8c64fdfc;
    dVar12 = *DAT_8c64fe00;
    fVar18 = (float)((ulonglong)dVar16 >> 0x20);
    dVar17 = dVar16;
  }
  if (in_FPSCR_PR == '\0') {
    dVar16 = (double)CONCAT44((float)((ulonglong)dVar16 >> 0x20) * SUB84(dVar12,0),SUB84(dVar16,0));
    dVar17 = (double)CONCAT44(fVar18 * (float)((ulonglong)dVar12 >> 0x20),SUB84(dVar17,0));
  }
  else {
    dVar16 = dVar16 * dVar12;
    dVar17 = dVar17 * dVar12;
  }
  if (in_FPSCR_SZ == '\0') {
    dVar17 = (double)CONCAT44((int)((ulonglong)dVar17 >> 0x20),DAT_8c64fdf4);
  }
  iVar3 = *param_5;
  do {
    iVar6 = iVar5 * 0xc;
    iVar2 = iVar5 * 4;
    fVar18 = SUB84(dVar16,0);
    dVar19 = in_xd6;
    if (in_FPSCR_SZ == '\0') {
      dVar19 = (double)CONCAT44(fVar18,SUB84(param_2,0));
    }
    if (in_FPSCR_SZ == '\0') {
      dVar12 = (double)CONCAT44((int)((ulonglong)dVar12 >> 0x20),
                                *(undefined4 *)(*param_4 + iVar6 + 8));
    }
    else {
      in_xd2 = *(double *)(*param_4 + iVar6 + 8);
    }
    if (in_FPSCR_PR == '\0') {
      dVar19 = (double)CONCAT44((float)((ulonglong)dVar19 >> 0x20) / SUB84(dVar12,0),SUB84(dVar19,0)
                               );
    }
    else {
      dVar19 = dVar19 / dVar12;
    }
    *(undefined4 *)(iVar3 + 0x18) = *(undefined4 *)(param_4[1] + iVar2);
    uVar14 = (undefined4)((ulonglong)dVar12 >> 0x20);
    fVar7 = (float)(int)*(short *)(param_4[2] + iVar2 + 2);
    fVar10 = SUB84(dVar17,0);
    if (in_FPSCR_PR == '\0') {
      dVar12 = (double)CONCAT44(uVar14,fVar7 * fVar10);
    }
    else {
      dVar12 = (double)CONCAT44(uVar14,fVar7) * dVar17;
    }
    if (in_FPSCR_SZ == '\0') {
      pdVar4 = (double *)(iVar3 + 0x14);
      *(int *)pdVar4 = SUB84(dVar12,0);
    }
    else {
      pdVar4 = (double *)(iVar3 + 0x10);
      *pdVar4 = in_xd2;
    }
    if (in_FPSCR_PR == '\0') {
      fVar7 = (float)(int)*(short *)(iVar2 + param_4[2]) * fVar10;
    }
    else {
      fVar7 = SUB84((double)CONCAT44((int)((ulonglong)dVar12 >> 0x20),
                                     (float)(int)*(short *)(iVar2 + param_4[2])) * dVar17,0);
    }
    uVar20 = (uint)((ulonglong)dVar19 >> 0x20);
    if (in_FPSCR_SZ == '\0') {
      *(float *)((int)pdVar4 + -4) = fVar7;
      pdVar4 = pdVar4 + -1;
      *(uint *)pdVar4 = uVar20;
      dVar12 = (double)((ulonglong)uVar20 << 0x20);
    }
    else {
      pdVar4[-1] = in_xd2;
      pdVar4 = pdVar4 + -2;
      *pdVar4 = dVar19;
      dVar12 = dVar19;
    }
    if (in_FPSCR_SZ == '\0') {
      dVar13 = (double)CONCAT44(*(float *)(pdVar1 + 2),*(undefined4 *)(*param_4 + iVar6 + 4));
    }
    else {
      in_xd2 = *(double *)(*param_4 + iVar6 + 4);
      dVar13 = pdVar1[2];
    }
    fVar11 = (float)((ulonglong)dVar17 >> 0x20);
    fVar7 = (float)((ulonglong)dVar12 >> 0x20);
    if (in_FPSCR_PR == '\0') {
      fVar8 = SUB84(dVar13,0) * fVar11;
      dVar13 = (double)CONCAT44(fVar7 * fVar8 + (float)((ulonglong)dVar13 >> 0x20),fVar8);
    }
    else {
      dVar13 = dVar13 * dVar17;
    }
    if (in_FPSCR_SZ == '\0') {
      pdVar4 = (double *)((int)pdVar4 + -4);
      *(int *)pdVar4 = (int)((ulonglong)dVar13 >> 0x20);
    }
    else {
      pdVar4 = pdVar4 + -1;
      *pdVar4 = dVar13;
    }
    if (in_FPSCR_SZ == '\0') {
      dVar12 = (double)CONCAT44(*(float *)((int)pdVar1 + 0xc),SUB84(dVar13,0));
    }
    else {
      dVar12 = *(double *)((int)pdVar1 + 0xc);
    }
    if (in_FPSCR_SZ == '\0') {
      dVar12 = (double)CONCAT44((int)((ulonglong)dVar12 >> 0x20),*(undefined4 *)(iVar6 + *param_4));
    }
    else {
      in_xd2 = *(double *)(iVar6 + *param_4);
    }
    fVar8 = (float)((ulonglong)dVar16 >> 0x20);
    if (in_FPSCR_PR == '\0') {
      fVar9 = SUB84(dVar12,0) * fVar8;
      dVar12 = (double)CONCAT44(fVar7 * fVar9 + (float)((ulonglong)dVar12 >> 0x20),fVar9);
    }
    else {
      dVar12 = dVar12 * dVar16;
    }
    uVar14 = (undefined4)((ulonglong)dVar12 >> 0x20);
    if (in_FPSCR_SZ == '\0') {
      pdVar4 = (double *)((int)pdVar4 + -4);
      *(undefined4 *)pdVar4 = uVar14;
    }
    else {
      pdVar4 = pdVar4 + -1;
      *pdVar4 = dVar12;
    }
    *(undefined4 *)((int)pdVar4 + -4) = uVar15;
    iVar5 = iVar5 + 1;
    iVar3 = (int)pdVar4 + 0x1c;
    if (iVar5 == param_3) break;
    iVar2 = param_3 * 0xc;
    iVar3 = param_3 * 4;
    param_2 = in_xd6;
    if (in_FPSCR_SZ == '\0') {
      param_2 = (double)CONCAT44(fVar18,SUB84(dVar19,0));
    }
    if (in_FPSCR_SZ == '\0') {
      dVar12 = (double)CONCAT44(uVar14,*(undefined4 *)(*param_4 + iVar2 + 8));
    }
    else {
      in_xd2 = *(double *)(*param_4 + iVar2 + 8);
    }
    if (in_FPSCR_PR == '\0') {
      param_2 = (double)CONCAT44((float)((ulonglong)param_2 >> 0x20) / SUB84(dVar12,0),
                                 SUB84(param_2,0));
    }
    else {
      param_2 = param_2 / dVar12;
    }
    *(undefined4 *)((int)pdVar4 + 0x34) = *(undefined4 *)(param_4[1] + iVar3);
    uVar14 = (undefined4)((ulonglong)dVar12 >> 0x20);
    fVar7 = (float)(int)*(short *)(param_4[2] + iVar3 + 2);
    if (in_FPSCR_PR == '\0') {
      dVar12 = (double)CONCAT44(uVar14,fVar7 * fVar10);
    }
    else {
      dVar12 = (double)CONCAT44(uVar14,fVar7) * dVar17;
    }
    if (in_FPSCR_SZ == '\0') {
      pdVar4 = pdVar4 + 6;
      *(int *)pdVar4 = SUB84(dVar12,0);
    }
    else {
      pdVar4 = (double *)((int)pdVar4 + 0x2c);
      *pdVar4 = in_xd2;
    }
    if (in_FPSCR_PR == '\0') {
      fVar7 = (float)(int)*(short *)(iVar3 + param_4[2]) * fVar10;
    }
    else {
      fVar7 = SUB84((double)CONCAT44((int)((ulonglong)dVar12 >> 0x20),
                                     (float)(int)*(short *)(iVar3 + param_4[2])) * dVar17,0);
    }
    uVar20 = (uint)((ulonglong)param_2 >> 0x20);
    if (in_FPSCR_SZ == '\0') {
      *(float *)((int)pdVar4 + -4) = fVar7;
      pdVar4 = pdVar4 + -1;
      *(uint *)pdVar4 = uVar20;
      dVar12 = (double)((ulonglong)uVar20 << 0x20);
    }
    else {
      pdVar4[-1] = in_xd2;
      pdVar4 = pdVar4 + -2;
      *pdVar4 = param_2;
      dVar12 = param_2;
    }
    if (in_FPSCR_SZ == '\0') {
      dVar19 = (double)CONCAT44(*(float *)(pdVar1 + 2),*(undefined4 *)(*param_4 + iVar2 + 4));
    }
    else {
      in_xd2 = *(double *)(*param_4 + iVar2 + 4);
      dVar19 = pdVar1[2];
    }
    fVar7 = (float)((ulonglong)dVar12 >> 0x20);
    if (in_FPSCR_PR == '\0') {
      fVar9 = SUB84(dVar19,0) * fVar11;
      dVar19 = (double)CONCAT44(fVar7 * fVar9 + (float)((ulonglong)dVar19 >> 0x20),fVar9);
    }
    else {
      dVar19 = dVar19 * dVar17;
    }
    if (in_FPSCR_SZ == '\0') {
      pdVar4 = (double *)((int)pdVar4 + -4);
      *(int *)pdVar4 = (int)((ulonglong)dVar19 >> 0x20);
    }
    else {
      pdVar4 = pdVar4 + -1;
      *pdVar4 = dVar19;
    }
    if (in_FPSCR_SZ == '\0') {
      dVar12 = (double)CONCAT44(*(float *)((int)pdVar1 + 0xc),SUB84(dVar19,0));
    }
    else {
      dVar12 = *(double *)((int)pdVar1 + 0xc);
    }
    if (in_FPSCR_SZ == '\0') {
      dVar12 = (double)CONCAT44((int)((ulonglong)dVar12 >> 0x20),*(undefined4 *)(iVar2 + *param_4));
    }
    else {
      in_xd2 = *(double *)(iVar2 + *param_4);
    }
    if (in_FPSCR_PR == '\0') {
      fVar9 = SUB84(dVar12,0) * fVar8;
      dVar12 = (double)CONCAT44(fVar7 * fVar9 + (float)((ulonglong)dVar12 >> 0x20),fVar9);
    }
    else {
      dVar12 = dVar12 * dVar16;
    }
    if (in_FPSCR_SZ == '\0') {
      pdVar4 = (double *)((int)pdVar4 + -4);
      *(int *)pdVar4 = (int)((ulonglong)dVar12 >> 0x20);
    }
    else {
      pdVar4 = pdVar4 + -1;
      *pdVar4 = dVar12;
    }
    *(undefined4 *)((int)pdVar4 + -4) = uVar15;
    param_3 = param_3 + -1;
    iVar3 = (int)pdVar4 + 0x1c;
  } while (param_3 != iVar5);
  iVar2 = iVar5 * 0xc;
  iVar5 = iVar5 * 4;
  if (in_FPSCR_SZ == '\0') {
    dVar12 = (double)CONCAT44((int)((ulonglong)dVar12 >> 0x20),*(undefined4 *)(*param_4 + iVar2 + 8)
                             );
  }
  else {
    in_xd2 = *(double *)(*param_4 + iVar2 + 8);
  }
  if (in_FPSCR_PR == '\0') {
    dVar16 = (double)CONCAT44(fVar8,fVar18 / SUB84(dVar12,0));
  }
  else {
    dVar16 = dVar16 / dVar12;
  }
  *(undefined4 *)(iVar3 + 0x18) = *(undefined4 *)(param_4[1] + iVar5);
  uVar15 = (undefined4)((ulonglong)dVar12 >> 0x20);
  fVar18 = (float)(int)*(short *)(param_4[2] + iVar5 + 2);
  if (in_FPSCR_PR == '\0') {
    dVar12 = (double)CONCAT44(uVar15,fVar18 * fVar10);
  }
  else {
    dVar12 = (double)CONCAT44(uVar15,fVar18) * dVar17;
  }
  if (in_FPSCR_SZ == '\0') {
    pdVar4 = (double *)(iVar3 + 0x14);
    *(int *)pdVar4 = SUB84(dVar12,0);
  }
  else {
    pdVar4 = (double *)(iVar3 + 0x10);
    *pdVar4 = in_xd2;
  }
  if (in_FPSCR_PR == '\0') {
    fVar10 = (float)(int)*(short *)(iVar5 + param_4[2]) * fVar10;
  }
  else {
    fVar10 = SUB84((double)CONCAT44((int)((ulonglong)dVar12 >> 0x20),
                                    (float)(int)*(short *)(iVar5 + param_4[2])) * dVar17,0);
  }
  if (in_FPSCR_SZ == '\0') {
    *(float *)((int)pdVar4 + -4) = fVar10;
    in_xd6 = (double)((longlong)dVar16 << 0x20);
    pdVar4 = pdVar4 + -1;
    *(int *)pdVar4 = SUB84(dVar16,0);
  }
  else {
    pdVar4[-1] = in_xd2;
    pdVar4 = pdVar4 + -2;
    *pdVar4 = in_xd6;
  }
  if (in_FPSCR_SZ == '\0') {
    dVar12 = (double)CONCAT44(*(float *)(pdVar1 + 2),*(undefined4 *)(*param_4 + iVar2 + 4));
  }
  else {
    dVar12 = pdVar1[2];
  }
  fVar18 = (float)((ulonglong)in_xd6 >> 0x20);
  if (in_FPSCR_PR == '\0') {
    fVar11 = SUB84(dVar12,0) * fVar11;
    dVar12 = (double)CONCAT44(fVar18 * fVar11 + (float)((ulonglong)dVar12 >> 0x20),fVar11);
  }
  else {
    dVar12 = dVar12 * dVar17;
  }
  if (in_FPSCR_SZ == '\0') {
    pdVar4 = (double *)((int)pdVar4 + -4);
    *(int *)pdVar4 = (int)((ulonglong)dVar12 >> 0x20);
  }
  else {
    pdVar4 = pdVar4 + -1;
    *pdVar4 = dVar12;
  }
  uVar15 = DAT_8c64fe08;
  if (in_FPSCR_SZ == '\0') {
    dVar12 = (double)CONCAT44(*(float *)((int)pdVar1 + 0xc),SUB84(dVar12,0));
  }
  else {
    dVar12 = *(double *)((int)pdVar1 + 0xc);
  }
  if (in_FPSCR_SZ == '\0') {
    dVar12 = (double)CONCAT44((int)((ulonglong)dVar12 >> 0x20),*(undefined4 *)(iVar2 + *param_4));
  }
  if (in_FPSCR_PR == '\0') {
    fVar7 = SUB84(dVar12,0) * (float)((ulonglong)dVar16 >> 0x20);
    dVar12 = (double)CONCAT44(fVar18 * fVar7 + (float)((ulonglong)dVar12 >> 0x20),fVar7);
  }
  else {
    dVar12 = dVar12 * dVar16;
  }
  if (in_FPSCR_SZ == '\0') {
    pdVar4 = (double *)((int)pdVar4 + -4);
    *(int *)pdVar4 = (int)((ulonglong)dVar12 >> 0x20);
  }
  else {
    pdVar4 = pdVar4 + -1;
    *pdVar4 = dVar12;
  }
  *(undefined4 *)((int)pdVar4 + -4) = uVar15;
  *param_5 = (int)pdVar4 + 0x1c;
  return;
}

