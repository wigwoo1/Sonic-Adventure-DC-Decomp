/* Ghidra draft only; not buildable/verified source. */

/* WARNING: Removing unreachable block (ram,0x8c639e6a) */
/* WARNING: Removing unreachable block (ram,0x8c639d64) */
/* WARNING: Restarted to delay deadcode elimination for space: register */

void FUN_8c639cf4(double param_1,double param_2,double *param_3,uint param_4,uint param_5,
                 uint param_6)

{
  undefined4 uVar1;
  ulonglong *puVar2;
  undefined4 uVar3;
  float fVar4;
  double in_dr0;
  float fVar5;
  undefined4 uVar6;
  float fVar7;
  double dVar8;
  float fVar9;
  float fVar10;
  double dVar11;
  float fVar13;
  double dVar12;
  double dVar14;
  undefined8 in_dr12;
  ulonglong in_dr14;
  double in_xd0;
  double dVar15;
  float fVar16;
  double in_xd2;
  double dVar17;
  float fVar18;
  double in_xd4;
  double dVar19;
  float fVar20;
  double in_xd6;
  double dVar21;
  float fVar22;
  double in_xd8;
  double in_xd10;
  float fVar23;
  undefined8 in_xd12;
  float fVar24;
  ulonglong in_xd14;
  float fVar25;
  char in_FPSCR_PR;
  char in_FPSCR_SZ;
  ulonglong auStack_40 [2];
  undefined8 uStack_30;
  undefined8 uStack_28;
  undefined8 uStack_20;
  undefined8 uStack_18;
  ulonglong local_10;
  undefined4 local_8;
  undefined4 uStack_4;
  
  fVar13 = (float)((ulonglong)in_xd8 >> 0x20);
  fVar23 = (float)((ulonglong)in_xd10 >> 0x20);
  fVar24 = (float)((ulonglong)in_xd12 >> 0x20);
  fVar25 = (float)(in_xd14 >> 0x20);
  if (param_3 == (double *)0x0) {
    if ((param_6 != 0) && (in_FPSCR_PR == '\0')) {
      fVar13 = ((float)(param_6 & 0xffff) * 6.2831855) / 65536.0;
      sin(fVar13);
      cos(fVar13);
    }
    if ((param_5 != 0) && (in_FPSCR_PR == '\0')) {
      fVar13 = ((float)(param_5 & 0xffff) * 6.2831855) / 65536.0;
      sin(fVar13);
      cos(fVar13);
    }
    if ((param_4 != 0) && (in_FPSCR_PR == '\0')) {
      fVar13 = ((float)(param_4 & 0xffff) * 6.2831855) / 65536.0;
      sin(fVar13);
      cos(fVar13);
    }
    return;
  }
  if (param_6 != 0) {
    if (in_FPSCR_PR == '\0') {
      fVar16 = ((float)(param_6 & 0xffff) * 6.2831855) / 65536.0;
      fVar18 = (float)sin(fVar16);
      fVar16 = (float)cos(fVar16);
      in_dr0 = (double)CONCAT44((float)fVar18,(float)fVar16);
    }
    uVar6 = SUB84(in_dr0,0);
    if (in_FPSCR_SZ == '\0') {
      uVar3 = (undefined4)((ulonglong)in_dr0 >> 0x20);
      uVar1 = 0;
    }
    else {
      uVar3 = SUB84(in_xd0,0);
      in_xd4 = in_xd0;
      uVar1 = uVar6;
      in_xd0 = in_dr0;
    }
    if (in_FPSCR_PR != '\0') {
      uVar6 = SUB84(-in_dr0,0);
    }
    if (in_FPSCR_SZ == '\0') {
      _local_8 = CONCAT44((int)((ulonglong)in_dr12 >> 0x20),local_8);
      puVar2 = &local_10;
      in_dr0 = (double)CONCAT44(*(undefined4 *)param_3,*(undefined4 *)((int)param_3 + 4));
      dVar8 = (double)CONCAT44(*(undefined4 *)(param_3 + 1),*(undefined4 *)((int)param_3 + 0xc));
      dVar11 = (double)CONCAT44(*(undefined4 *)(param_3 + 2),*(undefined4 *)((int)param_3 + 0x14));
      dVar14 = (double)CONCAT44(*(undefined4 *)(param_3 + 3),*(undefined4 *)((int)param_3 + 0x1c));
      param_3 = param_3 + 4;
      dVar21 = in_xd6;
      dVar19 = in_xd4;
      dVar17 = in_xd2;
      dVar15 = in_xd0;
      in_xd6 = (double)(in_dr14 & 0xffffffff);
      in_xd4 = (double)CONCAT44(uVar6,(int)in_dr12);
      param_2 = (double)CONCAT44(uVar1,SUB84(param_2,0));
      param_1 = (double)CONCAT44(uVar3,SUB84(param_1,0));
    }
    else {
      puVar2 = &uStack_20;
      in_dr0 = *param_3;
      dVar15 = param_3[1];
      dVar8 = param_3[2];
      dVar17 = param_3[3];
      dVar11 = param_3[4];
      dVar19 = param_3[5];
      dVar14 = param_3[6];
      dVar21 = param_3[7];
      param_3 = param_3 + 8;
      param_2 = in_xd2;
      param_1 = in_xd0;
    }
    fVar16 = (float)((ulonglong)dVar15 >> 0x20);
    fVar18 = (float)((ulonglong)dVar17 >> 0x20);
    fVar20 = (float)((ulonglong)dVar19 >> 0x20);
    fVar22 = (float)((ulonglong)dVar21 >> 0x20);
    if (in_FPSCR_PR == '\0') {
      fVar10 = SUB84(in_dr0,0);
                    /* WARNING: Ignoring partial resolution of indirect */
      fVar5 = (float)((ulonglong)in_dr0 >> 0x20);
      fVar7 = SUB84(dVar8,0);
      fVar9 = (float)((ulonglong)dVar8 >> 0x20);
      fVar4 = SUB84(dVar15,0) * fVar10 + SUB84(dVar19,0) * fVar5 + SUB84(in_xd8,0) * fVar7 +
              (float)in_xd12 * fVar9;
      fVar4 = fVar16 * fVar10 + fVar20 * fVar5 + fVar13 * fVar7 + fVar24 * fVar9;
      fVar4 = SUB84(dVar17,0) * fVar10 + SUB84(dVar21,0) * fVar5 + SUB84(in_xd10,0) * fVar7 +
              (float)in_xd14 * fVar9;
      fVar4 = fVar18 * fVar10 + fVar22 * fVar5 + fVar23 * fVar7 + fVar25 * fVar9;
      fVar10 = SUB84(dVar11,0);
                    /* WARNING: Ignoring partial resolution of indirect */
      fVar5 = (float)((ulonglong)dVar11 >> 0x20);
      fVar7 = SUB84(dVar14,0);
      fVar9 = (float)((ulonglong)dVar14 >> 0x20);
      fVar4 = SUB84(dVar15,0) * fVar10 + SUB84(dVar19,0) * fVar5 + SUB84(in_xd8,0) * fVar7 +
              (float)in_xd12 * fVar9;
      fVar13 = fVar16 * fVar10 + fVar20 * fVar5 + fVar13 * fVar7 + fVar24 * fVar9;
      fVar13 = SUB84(dVar17,0) * fVar10 + SUB84(dVar21,0) * fVar5 + SUB84(in_xd10,0) * fVar7 +
               (float)in_xd14 * fVar9;
      fVar13 = fVar18 * fVar10 + fVar22 * fVar5 + fVar23 * fVar7 + fVar25 * fVar9;
    }
    uVar6 = (undefined4)((ulonglong)in_dr0 >> 0x20);
    if (in_FPSCR_SZ == '\0') {
      *(int *)((int)param_3 + -4) = SUB84(dVar14,0);
      *(int *)(param_3 + -1) = (int)((ulonglong)dVar14 >> 0x20);
      *(int *)((int)param_3 + -0xc) = SUB84(dVar11,0);
      *(int *)(param_3 + -2) = (int)((ulonglong)dVar11 >> 0x20);
      *(int *)((int)param_3 + -0x14) = SUB84(dVar8,0);
      *(int *)(param_3 + -3) = (int)((ulonglong)dVar8 >> 0x20);
      *(int *)((int)param_3 + -0x1c) = SUB84(in_dr0,0);
      param_3 = param_3 + -4;
      *(undefined4 *)param_3 = uVar6;
      in_dr0 = (double)CONCAT44(uVar6,(int)((ulonglong)param_1 >> 0x20));
      in_dr14 = *puVar2;
      in_dr12 = puVar2[1];
      register0x0000003c = (BADSPACEBASE *)(puVar2 + 2);
      in_xd0 = dVar15;
      in_xd2 = dVar17;
      in_xd4 = dVar19;
      in_xd6 = dVar21;
    }
    else {
      param_3[-1] = dVar21;
      param_3[-2] = dVar14;
      param_3[-3] = dVar19;
      param_3[-4] = dVar11;
      param_3[-5] = dVar17;
      param_3[-6] = dVar8;
      param_3[-7] = dVar15;
      param_3 = param_3 + -8;
      *param_3 = in_dr0;
      in_xd14 = *puVar2;
      in_dr14 = puVar2[1];
      in_xd12 = puVar2[2];
      in_dr12 = puVar2[3];
      register0x0000003c = (BADSPACEBASE *)(puVar2 + 4);
      in_xd0 = param_1;
      in_xd2 = param_2;
    }
  }
  dVar11 = in_xd0;
  dVar15 = in_xd2;
  dVar19 = in_xd8;
  dVar17 = in_xd10;
  if (param_5 != 0) {
    if (in_FPSCR_PR == '\0') {
      fVar13 = ((float)(param_5 & 0xffff) * 6.2831855) / 65536.0;
      fVar23 = (float)sin(fVar13);
      fVar13 = (float)cos(fVar13);
      in_dr0 = (double)CONCAT44((float)fVar23,(float)fVar13);
    }
    dVar21 = in_dr0;
    dVar8 = in_xd0;
    if (in_FPSCR_SZ == '\0') {
      dVar21 = (double)((ulonglong)in_dr0 & 0xffffffff00000000);
      dVar8 = (double)CONCAT44(SUB84(in_dr0,0),SUB84(in_dr0,0));
    }
    dVar8 = (double)((ulonglong)dVar8 & 0xffffffff00000000);
    if (in_FPSCR_SZ == '\0') {
      *(int *)((int)register0x0000003c + -4) = (int)((ulonglong)in_dr12 >> 0x20);
      *(int *)((int)register0x0000003c + -8) = (int)in_dr12;
      *(int *)((int)register0x0000003c + -0xc) = (int)(in_dr14 >> 0x20);
      puVar2 = (ulonglong *)((int)register0x0000003c + -0x10);
      *(int *)((int)register0x0000003c + -0x10) = (int)in_dr14;
      in_dr0 = (double)CONCAT44(*(undefined4 *)param_3,*(undefined4 *)((int)param_3 + 4));
      dVar14 = (double)CONCAT44(*(undefined4 *)(param_3 + 1),*(undefined4 *)((int)param_3 + 0xc));
      param_3 = param_3 + 2;
      dVar17 = (double)CONCAT44(SUB84(param_2,0),(int)in_dr14);
      dVar19 = (double)CONCAT44(SUB84(param_1,0),(int)in_dr12);
    }
    else {
      *(undefined8 *)((int)register0x0000003c + -8) = in_dr12;
      *(undefined8 *)((int)register0x0000003c + -0x10) = in_xd12;
      *(ulonglong *)((int)register0x0000003c + -0x18) = in_dr14;
      puVar2 = (ulonglong *)((int)register0x0000003c + -0x20);
      *(ulonglong *)((int)register0x0000003c + -0x20) = in_xd14;
      in_dr0 = *param_3;
      in_xd0 = param_3[1];
      dVar14 = param_3[2];
      in_xd2 = param_3[3];
      param_3 = param_3 + 4;
    }
    if (in_FPSCR_SZ == '\0') {
      dVar11 = (double)CONCAT44(*(undefined4 *)(param_3 + 2),*(undefined4 *)((int)param_3 + 0x14));
      dVar15 = (double)CONCAT44(*(undefined4 *)(param_3 + 3),*(undefined4 *)((int)param_3 + 0x1c));
      param_3 = param_3 + 4;
    }
    else {
      dVar11 = param_3[2];
      in_xd8 = param_3[3];
      dVar15 = param_3[4];
      in_xd10 = param_3[5];
      param_3 = param_3 + 6;
    }
    fVar13 = (float)((ulonglong)in_xd0 >> 0x20);
    fVar23 = (float)((ulonglong)in_xd2 >> 0x20);
    fVar24 = (float)((ulonglong)in_xd4 >> 0x20);
    fVar25 = (float)((ulonglong)in_xd6 >> 0x20);
    fVar16 = (float)((ulonglong)in_xd8 >> 0x20);
    fVar18 = (float)((ulonglong)in_xd10 >> 0x20);
    fVar20 = (float)((ulonglong)in_xd12 >> 0x20);
    fVar22 = (float)(in_xd14 >> 0x20);
    if (in_FPSCR_PR == '\0') {
      fVar10 = SUB84(in_dr0,0);
                    /* WARNING: Ignoring partial resolution of indirect */
      fVar5 = (float)((ulonglong)in_dr0 >> 0x20);
      fVar7 = SUB84(dVar14,0);
      fVar9 = (float)((ulonglong)dVar14 >> 0x20);
      fVar4 = SUB84(in_xd0,0) * fVar10 + SUB84(in_xd4,0) * fVar5 + SUB84(in_xd8,0) * fVar7 +
              (float)in_xd12 * fVar9;
      fVar4 = fVar13 * fVar10 + fVar24 * fVar5 + fVar16 * fVar7 + fVar20 * fVar9;
      fVar4 = SUB84(in_xd2,0) * fVar10 + SUB84(in_xd6,0) * fVar5 + SUB84(in_xd10,0) * fVar7 +
              (float)in_xd14 * fVar9;
      fVar4 = fVar23 * fVar10 + fVar25 * fVar5 + fVar18 * fVar7 + fVar22 * fVar9;
      fVar4 = SUB84(dVar21,0);
                    /* WARNING: Ignoring partial resolution of indirect */
      fVar5 = (float)((ulonglong)dVar21 >> 0x20);
      fVar7 = (float)((ulonglong)dVar8 >> 0x20);
      fVar10 = SUB84(in_xd0,0) * fVar4 + SUB84(in_xd4,0) * fVar5 + SUB84(in_xd8,0) * 0.0 +
               (float)in_xd12 * fVar7;
      fVar13 = fVar13 * fVar4 + fVar24 * fVar5 + fVar16 * 0.0 + fVar20 * fVar7;
      fVar13 = SUB84(in_xd2,0) * fVar4 + SUB84(in_xd6,0) * fVar5 + SUB84(in_xd10,0) * 0.0 +
               (float)in_xd14 * fVar7;
      fVar13 = fVar23 * fVar4 + fVar25 * fVar5 + fVar18 * 0.0 + fVar22 * fVar7;
    }
    uVar6 = (undefined4)((ulonglong)in_dr0 >> 0x20);
    if (in_FPSCR_SZ == '\0') {
      *(int *)((int)param_3 + -4) = SUB84(dVar8,0);
      *(int *)(param_3 + -1) = (int)((ulonglong)dVar8 >> 0x20);
      *(int *)((int)param_3 + -0xc) = SUB84(dVar21,0);
      *(int *)(param_3 + -2) = (int)((ulonglong)dVar21 >> 0x20);
      *(int *)((int)param_3 + -0x24) = SUB84(dVar14,0);
      *(int *)(param_3 + -5) = (int)((ulonglong)dVar14 >> 0x20);
      *(int *)((int)param_3 + -0x2c) = SUB84(in_dr0,0);
      param_3 = param_3 + -6;
      *(undefined4 *)param_3 = uVar6;
      in_dr0 = (double)CONCAT44(uVar6,(int)((ulonglong)dVar11 >> 0x20));
      in_dr14 = *puVar2;
      in_dr12 = puVar2[1];
      register0x0000003c = (BADSPACEBASE *)(puVar2 + 2);
      dVar11 = in_xd0;
      dVar15 = in_xd2;
      dVar19 = in_xd8;
      dVar17 = in_xd10;
    }
    else {
      param_3[-1] = in_xd6;
      param_3[-2] = dVar8;
      param_3[-3] = in_xd4;
      param_3[-4] = dVar21;
      param_3[-7] = in_xd2;
      param_3[-8] = dVar14;
      param_3[-9] = in_xd0;
      param_3 = param_3 + -10;
      *param_3 = in_dr0;
      in_xd14 = *puVar2;
      in_dr14 = puVar2[1];
      in_xd12 = puVar2[2];
      in_dr12 = puVar2[3];
      register0x0000003c = (BADSPACEBASE *)(puVar2 + 4);
    }
  }
  if (param_4 != 0) {
    if (in_FPSCR_PR == '\0') {
      fVar13 = ((float)(param_4 & 0xffff) * 6.2831855) / 65536.0;
      fVar23 = (float)sin(fVar13);
      fVar13 = (float)cos(fVar13);
      in_dr0 = (double)CONCAT44((float)fVar23,(float)fVar13);
    }
    dVar8 = in_dr0;
    if (in_FPSCR_SZ == '\0') {
      dVar8 = (double)((ulonglong)in_dr0 & 0xffffffff00000000);
    }
    dVar21 = (double)((ulonglong)in_dr0 & 0xffffffff);
    if (in_FPSCR_SZ == '\0') {
      *(int *)((int)register0x0000003c + -4) = (int)((ulonglong)in_dr12 >> 0x20);
      *(int *)((int)register0x0000003c + -8) = (int)in_dr12;
      *(int *)((int)register0x0000003c + -0xc) = (int)(in_dr14 >> 0x20);
      *(int *)((int)register0x0000003c + -0x10) = (int)in_dr14;
    }
    else {
      *(undefined8 *)((int)register0x0000003c + -8) = in_dr12;
      *(undefined8 *)((int)register0x0000003c + -0x10) = in_xd12;
      *(ulonglong *)((int)register0x0000003c + -0x18) = in_dr14;
      *(ulonglong *)((int)register0x0000003c + -0x20) = in_xd14;
      in_xd4 = dVar8;
    }
    if (in_FPSCR_SZ == '\0') {
      dVar12 = (double)CONCAT44(*(undefined4 *)(param_3 + 2),*(undefined4 *)((int)param_3 + 0x14));
      dVar14 = (double)CONCAT44(*(undefined4 *)(param_3 + 3),*(undefined4 *)((int)param_3 + 0x1c));
      param_3 = param_3 + 6;
    }
    else {
      dVar12 = param_3[2];
      in_xd4 = param_3[3];
      dVar14 = param_3[4];
      in_xd6 = param_3[5];
      dVar19 = param_3[7];
      dVar17 = param_3[9];
      param_3 = param_3 + 10;
    }
    fVar13 = (float)((ulonglong)dVar11 >> 0x20);
    fVar23 = (float)((ulonglong)dVar15 >> 0x20);
    fVar24 = (float)((ulonglong)in_xd4 >> 0x20);
    fVar25 = (float)((ulonglong)in_xd6 >> 0x20);
    fVar16 = (float)((ulonglong)dVar19 >> 0x20);
    fVar18 = (float)((ulonglong)dVar17 >> 0x20);
    fVar20 = (float)((ulonglong)in_xd12 >> 0x20);
    fVar22 = (float)(in_xd14 >> 0x20);
    if (in_FPSCR_PR == '\0') {
      fVar4 = SUB84(in_dr0,0);
                    /* WARNING: Ignoring partial resolution of indirect */
      fVar5 = SUB84(dVar8,0);
      fVar7 = (float)((ulonglong)dVar8 >> 0x20);
      fVar10 = SUB84(dVar11,0) * fVar4 + SUB84(in_xd4,0) * 0.0 + SUB84(dVar19,0) * fVar5 +
               (float)in_xd12 * fVar7;
      fVar10 = fVar13 * fVar4 + fVar24 * 0.0 + fVar16 * fVar5 + fVar20 * fVar7;
      fVar10 = SUB84(dVar15,0) * fVar4 + SUB84(in_xd6,0) * 0.0 + SUB84(dVar17,0) * fVar5 +
               (float)in_xd14 * fVar7;
      fVar4 = fVar23 * fVar4 + fVar25 * 0.0 + fVar18 * fVar5 + fVar22 * fVar7;
      fVar10 = SUB84(dVar12,0);
                    /* WARNING: Ignoring partial resolution of indirect */
      fVar5 = (float)((ulonglong)dVar12 >> 0x20);
      fVar7 = SUB84(dVar14,0);
      fVar9 = (float)((ulonglong)dVar14 >> 0x20);
      fVar4 = SUB84(dVar11,0) * fVar10 + SUB84(in_xd4,0) * fVar5 + SUB84(dVar19,0) * fVar7 +
              (float)in_xd12 * fVar9;
      fVar13 = fVar13 * fVar10 + fVar24 * fVar5 + fVar16 * fVar7 + fVar20 * fVar9;
      fVar13 = SUB84(dVar15,0) * fVar10 + SUB84(in_xd6,0) * fVar5 + SUB84(dVar17,0) * fVar7 +
               (float)in_xd14 * fVar9;
      fVar13 = fVar23 * fVar10 + fVar25 * fVar5 + fVar18 * fVar7 + fVar22 * fVar9;
    }
    if (in_FPSCR_SZ == '\0') {
      *(int *)((int)param_3 + -4) = SUB84(dVar14,0);
      *(int *)(param_3 + -1) = (int)((ulonglong)dVar14 >> 0x20);
      *(int *)((int)param_3 + -0xc) = SUB84(dVar12,0);
      *(int *)(param_3 + -2) = (int)((ulonglong)dVar12 >> 0x20);
      *(int *)((int)param_3 + -0x14) = SUB84(dVar8,0);
      *(int *)(param_3 + -3) = (int)((ulonglong)dVar8 >> 0x20);
      *(int *)((int)param_3 + -0x1c) = SUB84(dVar21,0);
      *(int *)(param_3 + -4) = (int)((ulonglong)dVar21 >> 0x20);
    }
    else {
      param_3[-1] = in_xd6;
      param_3[-2] = dVar14;
      param_3[-3] = in_xd4;
      param_3[-4] = dVar12;
      param_3[-5] = dVar15;
      param_3[-6] = dVar8;
      param_3[-7] = dVar11;
      param_3[-8] = dVar21;
    }
  }
  return;
}

