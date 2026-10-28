/* Ghidra draft only; not buildable/verified source. */

double FUN_8c64f3ec(undefined4 param_1,undefined4 param_2,undefined4 param_3,undefined4 param_4,
                   double *param_5)

{
  double *pdVar1;
  double *pdVar2;
  undefined4 in_fr1;
  float fVar3;
  float fVar4;
  float fVar5;
  float fVar7;
  undefined4 in_fr3;
  float fVar8;
  double dVar9;
  float fVar11;
  double dVar12;
  double dVar13;
  double dVar14;
  double dVar15;
  double dVar16;
  float fVar18;
  double dVar17;
  double dVar19;
  undefined4 in_fr13;
  undefined4 in_fr15;
  double in_xd0;
  double in_xd2;
  double in_xd4;
  double in_xd6;
  double in_xd8;
  double in_xd10;
  double in_xd12;
  double in_xd14;
  char in_FPSCR_PR;
  char in_FPSCR_SZ;
  double dVar6;
  float fVar10;
  
  if (param_5 == (double *)0x0) {
    dVar19 = in_xd10;
    dVar14 = in_xd8;
    dVar12 = in_xd14;
    dVar16 = in_xd12;
    dVar6 = in_xd10;
    dVar13 = in_xd8;
    if (in_FPSCR_SZ == '\0') {
      dVar19 = (double)CONCAT44(param_4,param_4);
      dVar14 = (double)CONCAT44(param_3,param_3);
      dVar12 = (double)CONCAT44(in_fr15,param_2);
      dVar16 = (double)CONCAT44(in_fr13,param_1);
      dVar6 = (double)CONCAT44(param_4,in_fr3);
      dVar13 = (double)CONCAT44(param_3,in_fr1);
    }
    fVar5 = SUB84(dVar12,0);
    fVar7 = (float)((ulonglong)dVar12 >> 0x20);
    fVar11 = SUB84(dVar16,0);
    fVar3 = (float)((ulonglong)dVar16 >> 0x20);
    if (in_FPSCR_PR == '\0') {
      fVar8 = SUB84(dVar6,0) * fVar7;
      fVar10 = (float)((ulonglong)dVar6 >> 0x20) * fVar11;
      dVar9 = (double)CONCAT44(fVar10,fVar8);
      dVar19 = (double)CONCAT44((float)((ulonglong)dVar19 >> 0x20) * fVar5 - fVar8,
                                SUB84(dVar19,0) * fVar3 - (float)((ulonglong)dVar13 >> 0x20) * fVar5
                               );
      fVar3 = SUB84(dVar13,0) * fVar3;
      dVar6 = (double)CONCAT44((float)((ulonglong)dVar14 >> 0x20) * fVar11 - fVar3,
                               SUB84(dVar14,0) * fVar7 - fVar10);
    }
    else {
      dVar9 = dVar6 * dVar12 * dVar16;
      dVar19 = (dVar19 * dVar12 - dVar6 * dVar12) * dVar16 - dVar13 * dVar12;
      dVar6 = dVar13 * dVar12 * dVar16;
      fVar3 = SUB84(dVar6,0);
      dVar6 = (dVar14 * dVar12 - dVar9) * dVar16 - dVar6;
    }
    fVar8 = SUB84(dVar6,0);
    fVar7 = SUB84(dVar19,0);
    if (in_FPSCR_SZ == '\0') {
      in_xd8 = (double)CONCAT44(fVar8,fVar3);
      in_xd10 = (double)CONCAT44(fVar7,SUB84(dVar9,0));
      in_xd12 = (double)CONCAT44(in_fr13,fVar11);
      in_xd14 = (double)CONCAT44(in_fr15,fVar5);
    }
    if (in_FPSCR_PR == '\0') {
      dVar14 = (double)CONCAT44((float)((ulonglong)in_xd14 >> 0x20) *
                                (float)((ulonglong)in_xd8 >> 0x20),
                                SUB84(in_xd14,0) * SUB84(in_xd8,0));
      dVar12 = (double)CONCAT44((float)((ulonglong)in_xd12 >> 0x20) *
                                (float)((ulonglong)in_xd10 >> 0x20),
                                SUB84(in_xd12,0) * SUB84(in_xd10,0));
    }
    else {
      dVar14 = in_xd14 * in_xd8 * in_xd8;
      dVar12 = in_xd12 * in_xd10 * in_xd10;
    }
    fVar11 = (float)((ulonglong)dVar14 >> 0x20);
    fVar3 = (float)((ulonglong)dVar12 >> 0x20);
    dVar9 = in_xd6;
    dVar17 = dVar12;
    dVar16 = in_xd4;
    dVar13 = dVar14;
    if (in_FPSCR_SZ == '\0') {
      dVar9 = (double)CONCAT44(SUB84(dVar14,0),fVar3);
      dVar17 = in_xd2;
      dVar16 = (double)CONCAT44(SUB84(dVar12,0),fVar11);
      dVar13 = in_xd0;
    }
    if (in_FPSCR_PR == '\0') {
      dVar15 = (double)CONCAT44(fVar11 - SUB84(dVar9,0),
                                SUB84(dVar14,0) - (float)((ulonglong)dVar16 >> 0x20));
      dVar14 = (double)CONCAT44(fVar3 - SUB84(dVar16,0),
                                SUB84(dVar12,0) - (float)((ulonglong)dVar9 >> 0x20));
    }
    else {
      dVar15 = (dVar14 - dVar9) - dVar16;
      dVar14 = (dVar12 - dVar9) - dVar16;
    }
    fVar3 = SUB84(dVar14,0);
    dVar12 = in_xd4;
    if (in_FPSCR_SZ == '\0') {
      in_xd4 = (double)CONCAT44(fVar3,SUB84(dVar9,0));
      dVar12 = (double)CONCAT44(fVar3,SUB84(dVar16,0));
    }
    fVar11 = (float)((ulonglong)in_xd4 >> 0x20);
    fVar10 = (float)((ulonglong)dVar19 >> 0x20);
    fVar5 = (float)((ulonglong)dVar14 >> 0x20);
    if (in_FPSCR_PR == '\0') {
      dVar9 = (double)CONCAT44(fVar10 * SUB84(in_xd4,0),fVar7);
      dVar16 = (double)CONCAT44((int)((ulonglong)dVar15 >> 0x20),
                                SUB84(dVar15,0) * fVar11 + SUB84(dVar12,0) * fVar7);
      dVar12 = (double)CONCAT44(fVar11 * fVar10,
                                SUB84(in_xd4,0) * fVar5 + (float)((ulonglong)dVar12 >> 0x20) * fVar8
                               );
    }
    else {
      dVar9 = dVar19 * in_xd4 * dVar19;
      fVar7 = SUB84(dVar9,0);
      dVar12 = dVar12 * dVar6 * dVar9;
      dVar16 = dVar15 * in_xd4 + dVar12;
      dVar12 = in_xd4 * dVar19 * dVar14 + dVar12;
    }
    fVar11 = SUB84(dVar16,0);
    dVar19 = in_xd6;
    if (in_FPSCR_SZ == '\0') {
      in_xd6 = (double)CONCAT44(fVar11,fVar11);
      dVar19 = dVar13;
    }
    fVar10 = (float)((ulonglong)in_xd6 >> 0x20);
    fVar18 = (float)((ulonglong)dVar6 >> 0x20);
    if (in_FPSCR_PR == '\0') {
      fVar4 = SUB84(in_xd6,0) * fVar18;
      dVar15 = (double)CONCAT44(fVar10,fVar4);
      dVar13 = (double)CONCAT44(fVar5,fVar3 * fVar10 + (float)((ulonglong)dVar9 >> 0x20));
      dVar14 = (double)CONCAT44((float)((ulonglong)dVar12 >> 0x20) + fVar7 * SUB84(in_xd6,0),
                                SUB84(dVar12,0) + fVar4);
    }
    else {
      dVar15 = in_xd6 * dVar6;
      dVar13 = dVar14 * in_xd6 + dVar9 * in_xd6;
      dVar14 = dVar12 + dVar9 * in_xd6 + dVar15;
    }
    dVar12 = dVar19;
    if (in_FPSCR_SZ == '\0') {
      dVar12 = (double)CONCAT44(SUB84(dVar15,0),SUB84(dVar15,0));
    }
    fVar3 = (float)((ulonglong)dVar12 >> 0x20);
    if (in_FPSCR_PR == '\0') {
      fVar8 = fVar8 * fVar3;
      dVar13 = (double)CONCAT44((int)((ulonglong)dVar13 >> 0x20),SUB84(dVar13,0) + fVar8);
      dVar14 = (double)CONCAT44((float)((ulonglong)dVar14 >> 0x20) +
                                (float)((ulonglong)dVar16 >> 0x20) * SUB84(dVar12,0),SUB84(dVar14,0)
                               );
      dVar16 = (double)CONCAT44(fVar3 * fVar18 + fVar11,SUB84(dVar12,0));
    }
    else {
      dVar6 = dVar6 * dVar12;
      dVar16 = dVar16 * dVar12;
      fVar8 = SUB84(dVar6,0);
      dVar13 = dVar13 + dVar6;
      dVar14 = dVar14 + dVar16;
      fVar11 = SUB84(dVar16,0);
      dVar16 = dVar12 * dVar6 + dVar16;
    }
    if (in_FPSCR_SZ == '\0') {
      dVar19 = (double)CONCAT44(SUB84(dVar16,0),fVar11);
      dVar17 = (double)CONCAT44(SUB84(dVar14,0),fVar8);
    }
    if (in_FPSCR_PR == '\0') {
      dVar14 = (double)CONCAT44(((float)((ulonglong)dVar16 >> 0x20) *
                                 (float)((ulonglong)dVar17 >> 0x20) -
                                SUB84(dVar14,0) * SUB84(dVar17,0)) +
                                (SUB84(dVar13,0) * (float)((ulonglong)dVar19 >> 0x20) -
                                (float)((ulonglong)dVar14 >> 0x20) * SUB84(dVar19,0)),
                                SUB84(dVar16,0));
    }
    else {
      dVar14 = dVar14 * dVar19 * dVar17;
      dVar14 = (dVar16 * dVar17 - dVar14) + (dVar13 * dVar19 - dVar14);
    }
    return dVar14;
  }
  if (in_FPSCR_SZ == '\0') {
    fVar7 = *(float *)(param_5 + 4);
    fVar5 = *(float *)((int)param_5 + 0x24);
    dVar16 = (double)CONCAT44(fVar7,fVar5);
    fVar11 = *(float *)(param_5 + 5);
    fVar3 = *(float *)((int)param_5 + 0x2c);
    dVar12 = (double)CONCAT44(*(undefined4 *)(param_5 + 6),*(undefined4 *)((int)param_5 + 0x34));
    dVar14 = (double)CONCAT44(*(undefined4 *)(param_5 + 7),*(undefined4 *)((int)param_5 + 0x3c));
    pdVar1 = param_5 + 8;
    dVar6 = (double)CONCAT44(fVar11,fVar3);
  }
  else {
    dVar16 = param_5[4];
    dVar6 = param_5[6];
    dVar12 = param_5[8];
    in_xd4 = param_5[9];
    dVar14 = param_5[10];
    in_xd6 = param_5[0xb];
    pdVar1 = param_5 + 0xc;
    fVar7 = (float)((ulonglong)dVar16 >> 0x20);
    fVar5 = SUB84(dVar16,0);
    fVar11 = (float)((ulonglong)dVar6 >> 0x20);
    fVar3 = SUB84(dVar6,0);
  }
  fVar10 = (float)((ulonglong)dVar14 >> 0x20);
  fVar8 = (float)((ulonglong)dVar12 >> 0x20);
  if (in_FPSCR_PR == '\0') {
    dVar13 = (double)CONCAT44((float)((ulonglong)dVar6 >> 0x20) * SUB84(dVar14,0) - fVar3 * fVar10,
                              SUB84(dVar6,0) * fVar8 - fVar7 * SUB84(dVar14,0));
    dVar14 = (double)CONCAT44((float)((ulonglong)dVar16 >> 0x20) * SUB84(dVar12,0) - fVar5 * fVar8,
                              SUB84(dVar16,0) * fVar10 - fVar11 * SUB84(dVar12,0));
  }
  else {
    dVar13 = (dVar6 * dVar14 - dVar6 * dVar14) * dVar12 - dVar16 * dVar14;
    dVar14 = (dVar16 * dVar14 - dVar6 * dVar14 * dVar12) * dVar12 - dVar16 * dVar14 * dVar12;
  }
  if (in_FPSCR_SZ == '\0') {
    dVar6 = (double)CONCAT44(*(undefined4 *)(pdVar1 + -4),*(undefined4 *)((int)pdVar1 + -0x1c));
    dVar16 = (double)CONCAT44(*(undefined4 *)(pdVar1 + -3),*(undefined4 *)((int)pdVar1 + -0x14));
    dVar12 = (double)CONCAT44(*(undefined4 *)(pdVar1 + -2),*(undefined4 *)((int)pdVar1 + -0xc));
    dVar19 = (double)CONCAT44(*(undefined4 *)(pdVar1 + -1),*(undefined4 *)((int)pdVar1 + -4));
  }
  else {
    dVar6 = pdVar1[-4];
    dVar16 = pdVar1[-2];
    dVar12 = *pdVar1;
    in_xd4 = pdVar1[1];
    dVar19 = pdVar1[2];
    in_xd6 = pdVar1[3];
    pdVar1 = pdVar1 + 4;
  }
  if (in_FPSCR_PR == '\0') {
    dVar6 = (double)CONCAT44((float)((ulonglong)dVar19 >> 0x20) * (float)((ulonglong)dVar6 >> 0x20),
                             SUB84(dVar19,0) * SUB84(dVar6,0));
    dVar16 = (double)CONCAT44((float)((ulonglong)dVar12 >> 0x20) *
                              (float)((ulonglong)dVar16 >> 0x20),SUB84(dVar12,0) * SUB84(dVar16,0));
  }
  else {
    dVar6 = dVar19 * dVar6 * dVar6;
    dVar16 = dVar12 * dVar16 * dVar16;
  }
  fVar11 = (float)((ulonglong)dVar6 >> 0x20);
  fVar3 = (float)((ulonglong)dVar16 >> 0x20);
  if (in_FPSCR_SZ == '\0') {
    in_xd4 = (double)CONCAT44(SUB84(dVar16,0),fVar11);
    in_xd6 = (double)CONCAT44(SUB84(dVar6,0),fVar3);
  }
  if (in_FPSCR_PR == '\0') {
    dVar12 = (double)CONCAT44(fVar11 - SUB84(in_xd6,0),
                              SUB84(dVar6,0) - (float)((ulonglong)in_xd4 >> 0x20));
    dVar16 = (double)CONCAT44(fVar3 - SUB84(in_xd4,0),
                              SUB84(dVar16,0) - (float)((ulonglong)in_xd6 >> 0x20));
  }
  else {
    dVar12 = (dVar6 - in_xd6) - in_xd4;
    dVar16 = (dVar16 - in_xd6) - in_xd4;
  }
  if (in_FPSCR_SZ == '\0') {
    fVar11 = *(float *)(pdVar1 + -6);
    fVar3 = *(float *)((int)pdVar1 + -0x2c);
    pdVar1 = pdVar1 + -5;
    dVar6 = (double)CONCAT44(fVar11,fVar3);
  }
  else {
    dVar6 = pdVar1[-6];
    pdVar1 = pdVar1 + -4;
    fVar11 = (float)((ulonglong)dVar6 >> 0x20);
    fVar3 = SUB84(dVar6,0);
  }
  fVar5 = (float)((ulonglong)dVar6 >> 0x20);
  fVar8 = (float)((ulonglong)dVar13 >> 0x20);
  fVar7 = (float)((ulonglong)dVar16 >> 0x20);
  if (in_FPSCR_PR == '\0') {
    fVar10 = SUB84(dVar13,0);
    dVar19 = (double)CONCAT44(fVar8 * SUB84(dVar6,0),fVar10);
    dVar12 = (double)CONCAT44((int)((ulonglong)dVar12 >> 0x20),
                              SUB84(dVar12,0) * fVar5 + fVar3 * fVar10);
    dVar9 = (double)CONCAT44(fVar5 * fVar8,SUB84(dVar6,0) * fVar7 + fVar11 * SUB84(dVar14,0));
  }
  else {
    dVar19 = dVar13 * dVar6 * dVar13;
    fVar10 = SUB84(dVar19,0);
    dVar9 = dVar6 * dVar14 * dVar19;
    dVar12 = dVar12 * dVar6 + dVar9;
    dVar9 = dVar6 * dVar13 * dVar16 + dVar9;
  }
  if (in_FPSCR_SZ == '\0') {
    pdVar2 = (double *)((int)pdVar1 + 4);
    dVar6 = (double)CONCAT44(*(undefined4 *)pdVar1,*(undefined4 *)pdVar1);
  }
  else {
    dVar6 = *pdVar1;
    pdVar2 = pdVar1 + 1;
  }
  fVar3 = (float)((ulonglong)dVar14 >> 0x20);
  if (in_FPSCR_PR == '\0') {
    dVar16 = (double)CONCAT44(fVar7,SUB84(dVar16,0) * (float)((ulonglong)dVar6 >> 0x20) +
                                    (float)((ulonglong)dVar19 >> 0x20));
    dVar6 = (double)CONCAT44((float)((ulonglong)dVar9 >> 0x20) + fVar10 * SUB84(dVar6,0),
                             SUB84(dVar9,0) + SUB84(dVar6,0) * fVar3);
  }
  else {
    dVar16 = dVar16 * dVar6 + dVar19 * dVar6;
    dVar6 = dVar9 + dVar19 * dVar6 + dVar6 * dVar14;
  }
  if (in_FPSCR_SZ == '\0') {
    dVar13 = (double)CONCAT44(*(undefined4 *)pdVar2,*(undefined4 *)pdVar2);
  }
  else {
    dVar13 = *pdVar2;
  }
  fVar11 = (float)((ulonglong)dVar13 >> 0x20);
  if (in_FPSCR_PR == '\0') {
    dVar16 = (double)CONCAT44((int)((ulonglong)dVar16 >> 0x20),
                              SUB84(dVar16,0) + SUB84(dVar14,0) * fVar11);
    dVar6 = (double)CONCAT44((float)((ulonglong)dVar6 >> 0x20) +
                             (float)((ulonglong)dVar12 >> 0x20) * SUB84(dVar13,0),SUB84(dVar6,0));
    dVar14 = (double)CONCAT44(fVar11 * fVar3 + SUB84(dVar12,0),SUB84(dVar13,0));
  }
  else {
    dVar16 = dVar16 + dVar14 * dVar13;
    dVar6 = dVar6 + dVar12 * dVar13;
    dVar14 = dVar13 * dVar14 * dVar13 + dVar12 * dVar13;
  }
  if (in_FPSCR_SZ == '\0') {
    dVar12 = (double)CONCAT44(*(undefined4 *)param_5,*(undefined4 *)((int)param_5 + 4));
    dVar13 = (double)CONCAT44(*(undefined4 *)(param_5 + 1),*(undefined4 *)((int)param_5 + 0xc));
  }
  else {
    dVar12 = *param_5;
    dVar13 = param_5[2];
  }
  if (in_FPSCR_PR == '\0') {
    dVar14 = (double)CONCAT44(((float)((ulonglong)dVar14 >> 0x20) *
                               (float)((ulonglong)dVar13 >> 0x20) - SUB84(dVar6,0) * SUB84(dVar13,0)
                              ) + (SUB84(dVar16,0) * (float)((ulonglong)dVar12 >> 0x20) -
                                  (float)((ulonglong)dVar6 >> 0x20) * SUB84(dVar12,0)),
                              SUB84(dVar14,0));
  }
  else {
    dVar6 = dVar6 * dVar12 * dVar13;
    dVar14 = (dVar14 * dVar13 - dVar6) + (dVar16 * dVar12 - dVar6);
  }
  return dVar14;
}

