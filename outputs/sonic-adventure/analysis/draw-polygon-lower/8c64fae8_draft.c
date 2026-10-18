/* Ghidra draft only; not buildable/verified source. */

double FUN_8c64fae8(undefined4 param_1,undefined4 param_2,int param_3,int *param_4,int *param_5)

{
  double *pdVar1;
  int iVar2;
  int iVar3;
  double *pdVar4;
  int iVar5;
  double in_dr0;
  float fVar6;
  float fVar7;
  float fVar8;
  float fVar9;
  double dVar10;
  double dVar11;
  undefined4 uVar12;
  double dVar13;
  undefined4 uVar14;
  undefined4 uVar15;
  double in_xd6;
  double dVar16;
  char in_FPSCR_PR;
  char in_FPSCR_SZ;
  
  uVar15 = DAT_8c64fc60;
  pdVar1 = DAT_8c64fc54;
  param_3 = param_3 + -1;
  if (in_FPSCR_SZ == '\0') {
    dVar11 = (double)CONCAT44(*(undefined4 *)DAT_8c64fc54,param_1);
  }
  else {
    dVar11 = *DAT_8c64fc54;
  }
  iVar5 = 0;
  if (in_FPSCR_SZ == '\0') {
    dVar10 = (double)CONCAT44(*(undefined4 *)DAT_8c64fc5c,*DAT_8c64fc58);
    uVar12 = (undefined4)((ulonglong)dVar11 >> 0x20);
    dVar11 = (double)CONCAT44(uVar12,uVar12);
  }
  else {
    dVar10 = *DAT_8c64fc5c;
  }
  if (in_FPSCR_PR == '\0') {
    dVar11 = (double)CONCAT44((float)((ulonglong)dVar11 >> 0x20) *
                              (float)((ulonglong)dVar10 >> 0x20),SUB84(dVar11,0) * SUB84(dVar10,0));
  }
  else {
    dVar11 = dVar11 * dVar10 * dVar10;
  }
  iVar3 = *param_5;
  dVar13 = (double)CONCAT44(0x3f800000,param_2);
  do {
    dVar16 = dVar13;
    if (in_FPSCR_SZ == '\0') {
      uVar12 = (undefined4)((ulonglong)dVar13 >> 0x20);
      dVar13 = (double)CONCAT44(uVar12,uVar12);
      dVar16 = in_xd6;
    }
    if (in_FPSCR_SZ == '\0') {
      dVar10 = (double)CONCAT44((int)((ulonglong)dVar10 >> 0x20),
                                *(undefined4 *)(iVar5 * 0xc + *param_4 + 8));
    }
    if (in_FPSCR_PR == '\0') {
      dVar13 = (double)CONCAT44((int)((ulonglong)dVar13 >> 0x20),SUB84(dVar13,0) / SUB84(dVar10,0));
    }
    else {
      dVar13 = dVar13 / dVar10;
    }
    if (param_4[1] == 0) {
      *(undefined4 *)(iVar3 + 0x18) = 0xffffffff;
    }
    else {
      *(undefined4 *)(iVar3 + 0x18) = *(undefined4 *)(iVar5 * 4 + param_4[1]);
    }
    dVar10 = dVar16;
    if (in_FPSCR_SZ == '\0') {
      dVar10 = (double)CONCAT44(SUB84(dVar13,0),SUB84(in_dr0,0));
    }
    in_dr0 = dVar10;
    if (in_FPSCR_SZ == '\0') {
      pdVar4 = (double *)(iVar3 + 0xc);
      *(int *)pdVar4 = SUB84(dVar13,0);
    }
    else {
      pdVar4 = (double *)(iVar3 + 8);
      *pdVar4 = dVar16;
    }
    if (in_FPSCR_SZ == '\0') {
      dVar10 = (double)CONCAT44(*(undefined4 *)(pdVar1 + 2),
                                *(undefined4 *)(*param_4 + iVar5 * 0xc + 4));
    }
    else {
      dVar10 = pdVar1[2];
    }
    fVar9 = (float)((ulonglong)dVar11 >> 0x20);
    fVar6 = (float)((ulonglong)in_dr0 >> 0x20);
    if (in_FPSCR_PR == '\0') {
      fVar7 = SUB84(dVar10,0) * fVar9;
      dVar10 = (double)CONCAT44(fVar6 * fVar7 + (float)((ulonglong)dVar10 >> 0x20),fVar7);
    }
    else {
      dVar10 = dVar10 * dVar11;
    }
    if (in_FPSCR_SZ == '\0') {
      pdVar4 = (double *)((int)pdVar4 + -4);
      *(int *)pdVar4 = (int)((ulonglong)dVar10 >> 0x20);
    }
    else {
      pdVar4 = pdVar4 + -1;
      *pdVar4 = dVar10;
    }
    if (in_FPSCR_SZ == '\0') {
      dVar10 = (double)CONCAT44(*(undefined4 *)((int)pdVar1 + 0xc),SUB84(dVar10,0));
    }
    else {
      dVar10 = *(double *)((int)pdVar1 + 0xc);
    }
    if (in_FPSCR_SZ == '\0') {
      dVar10 = (double)CONCAT44((int)((ulonglong)dVar10 >> 0x20),
                                *(undefined4 *)(iVar5 * 0xc + *param_4));
    }
    fVar7 = SUB84(dVar11,0);
    if (in_FPSCR_PR == '\0') {
      fVar8 = SUB84(dVar10,0) * fVar7;
      dVar10 = (double)CONCAT44(fVar6 * fVar8 + (float)((ulonglong)dVar10 >> 0x20),fVar8);
    }
    else {
      dVar10 = dVar10 * dVar11;
    }
    uVar12 = (undefined4)((ulonglong)dVar10 >> 0x20);
    if (in_FPSCR_SZ == '\0') {
      pdVar4 = (double *)((int)pdVar4 + -4);
      *(undefined4 *)pdVar4 = uVar12;
    }
    else {
      pdVar4 = pdVar4 + -1;
      *pdVar4 = dVar10;
    }
    *(undefined4 *)((int)pdVar4 + -4) = uVar15;
    iVar5 = iVar5 + 1;
    iVar3 = (int)pdVar4 + 0x1c;
    if (iVar5 == param_3) break;
    in_xd6 = dVar13;
    if (in_FPSCR_SZ == '\0') {
      uVar14 = (undefined4)((ulonglong)dVar13 >> 0x20);
      dVar13 = (double)CONCAT44(uVar14,uVar14);
      in_xd6 = dVar16;
    }
    iVar3 = param_3 * 0xc;
    if (in_FPSCR_SZ == '\0') {
      dVar10 = (double)CONCAT44(uVar12,*(undefined4 *)(*param_4 + iVar3 + 8));
    }
    if (in_FPSCR_PR == '\0') {
      dVar13 = (double)CONCAT44((int)((ulonglong)dVar13 >> 0x20),SUB84(dVar13,0) / SUB84(dVar10,0));
    }
    else {
      dVar13 = dVar13 / dVar10;
    }
    if (param_4[1] == 0) {
      *(undefined4 *)((int)pdVar4 + 0x34) = 0xffffffff;
    }
    else {
      *(undefined4 *)((int)pdVar4 + 0x34) = *(undefined4 *)(param_3 * 4 + param_4[1]);
    }
    if (in_FPSCR_SZ == '\0') {
      pdVar4 = pdVar4 + 5;
      *(int *)pdVar4 = SUB84(dVar13,0);
      in_dr0 = (double)CONCAT44(SUB84(dVar13,0),SUB84(in_dr0,0));
    }
    else {
      pdVar4 = (double *)((int)pdVar4 + 0x24);
      *pdVar4 = in_xd6;
      in_dr0 = in_xd6;
    }
    if (in_FPSCR_SZ == '\0') {
      dVar10 = (double)CONCAT44(*(undefined4 *)(pdVar1 + 2),*(undefined4 *)(*param_4 + iVar3 + 4));
    }
    else {
      dVar10 = pdVar1[2];
    }
    fVar6 = (float)((ulonglong)in_dr0 >> 0x20);
    if (in_FPSCR_PR == '\0') {
      fVar8 = SUB84(dVar10,0) * fVar9;
      dVar10 = (double)CONCAT44(fVar6 * fVar8 + (float)((ulonglong)dVar10 >> 0x20),fVar8);
    }
    else {
      dVar10 = dVar10 * dVar11;
    }
    if (in_FPSCR_SZ == '\0') {
      pdVar4 = (double *)((int)pdVar4 + -4);
      *(int *)pdVar4 = (int)((ulonglong)dVar10 >> 0x20);
    }
    else {
      pdVar4 = pdVar4 + -1;
      *pdVar4 = dVar10;
    }
    if (in_FPSCR_SZ == '\0') {
      dVar10 = (double)CONCAT44(*(undefined4 *)((int)pdVar1 + 0xc),SUB84(dVar10,0));
    }
    else {
      dVar10 = *(double *)((int)pdVar1 + 0xc);
    }
    if (in_FPSCR_SZ == '\0') {
      dVar10 = (double)CONCAT44((int)((ulonglong)dVar10 >> 0x20),*(undefined4 *)(*param_4 + iVar3));
    }
    if (in_FPSCR_PR == '\0') {
      fVar8 = SUB84(dVar10,0) * fVar7;
      dVar10 = (double)CONCAT44(fVar6 * fVar8 + (float)((ulonglong)dVar10 >> 0x20),fVar8);
    }
    else {
      dVar10 = dVar10 * dVar11;
    }
    if (in_FPSCR_SZ == '\0') {
      pdVar4 = (double *)((int)pdVar4 + -4);
      *(int *)pdVar4 = (int)((ulonglong)dVar10 >> 0x20);
    }
    else {
      pdVar4 = pdVar4 + -1;
      *pdVar4 = dVar10;
    }
    *(undefined4 *)((int)pdVar4 + -4) = uVar15;
    param_3 = param_3 + -1;
    iVar3 = (int)pdVar4 + 0x1c;
  } while (param_3 != iVar5);
  iVar2 = iVar5 * 0xc;
  if (in_FPSCR_SZ == '\0') {
    dVar10 = (double)CONCAT44((int)((ulonglong)dVar10 >> 0x20),*(undefined4 *)(iVar2 + *param_4 + 8)
                             );
  }
  if (in_FPSCR_PR == '\0') {
    dVar13 = (double)CONCAT44((float)((ulonglong)dVar13 >> 0x20) / SUB84(dVar10,0),SUB84(dVar13,0));
  }
  else {
    dVar13 = dVar13 / dVar10;
  }
  if (param_4[1] == 0) {
    *(undefined4 *)(iVar3 + 0x18) = 0xffffffff;
  }
  else {
    *(undefined4 *)(iVar3 + 0x18) = *(undefined4 *)(iVar5 * 4 + param_4[1]);
  }
  uVar15 = (undefined4)((ulonglong)dVar13 >> 0x20);
  if (in_FPSCR_SZ == '\0') {
    dVar13 = (double)CONCAT44(uVar15,SUB84(in_dr0,0));
    pdVar4 = (double *)(iVar3 + 0xc);
    *(undefined4 *)pdVar4 = uVar15;
  }
  else {
    pdVar4 = (double *)(iVar3 + 8);
    *pdVar4 = dVar13;
  }
  if (in_FPSCR_SZ == '\0') {
    dVar10 = (double)CONCAT44(*(undefined4 *)(pdVar1 + 2),*(undefined4 *)(*param_4 + iVar2 + 4));
  }
  else {
    dVar10 = pdVar1[2];
  }
  fVar6 = (float)((ulonglong)dVar13 >> 0x20);
  if (in_FPSCR_PR == '\0') {
    fVar9 = SUB84(dVar10,0) * fVar9;
    dVar10 = (double)CONCAT44(fVar6 * fVar9 + (float)((ulonglong)dVar10 >> 0x20),fVar9);
  }
  else {
    dVar10 = dVar10 * dVar11;
  }
  if (in_FPSCR_SZ == '\0') {
    pdVar4 = (double *)((int)pdVar4 + -4);
    *(int *)pdVar4 = (int)((ulonglong)dVar10 >> 0x20);
  }
  else {
    pdVar4 = pdVar4 + -1;
    *pdVar4 = dVar10;
  }
  uVar15 = DAT_8c64fc64;
  if (in_FPSCR_SZ == '\0') {
    dVar10 = (double)CONCAT44(*(undefined4 *)((int)pdVar1 + 0xc),SUB84(dVar10,0));
  }
  else {
    dVar10 = *(double *)((int)pdVar1 + 0xc);
  }
  if (in_FPSCR_SZ == '\0') {
    dVar10 = (double)CONCAT44((int)((ulonglong)dVar10 >> 0x20),*(undefined4 *)(*param_4 + iVar2));
  }
  if (in_FPSCR_PR == '\0') {
    fVar7 = SUB84(dVar10,0) * fVar7;
    dVar10 = (double)CONCAT44(fVar6 * fVar7 + (float)((ulonglong)dVar10 >> 0x20),fVar7);
  }
  else {
    dVar10 = dVar10 * dVar11;
  }
  if (in_FPSCR_SZ == '\0') {
    pdVar4 = (double *)((int)pdVar4 + -4);
    *(int *)pdVar4 = (int)((ulonglong)dVar10 >> 0x20);
  }
  else {
    pdVar4 = pdVar4 + -1;
    *pdVar4 = dVar10;
  }
  *(undefined4 *)((int)pdVar4 + -4) = uVar15;
  *param_5 = (int)pdVar4 + 0x1c;
  return dVar13;
}

