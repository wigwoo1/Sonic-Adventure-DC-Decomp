/* Ghidra draft only; not buildable/verified source. */

/* WARNING: Removing unreachable block (ram,0x8c63a102) */
/* WARNING: Removing unreachable block (ram,0x8c63a03c) */
/* WARNING: Restarted to delay deadcode elimination for space: register */

void FUN_8c639ff8(undefined4 param_1,undefined4 param_2,ulonglong *param_3,uint param_4,uint param_5
                 ,uint param_6)

{
  undefined8 *puVar1;
  float fVar2;
  ulonglong in_dr0;
  float fVar4;
  undefined4 uVar5;
  float fVar6;
  ulonglong uVar7;
  float fVar9;
  ulonglong uVar8;
  ulonglong uVar10;
  ulonglong uVar11;
  ulonglong uVar13;
  ulonglong uVar14;
  ulonglong uVar15;
  ulonglong uVar16;
  ulonglong uVar17;
  undefined8 in_dr12;
  undefined8 in_dr14;
  ulonglong in_xd0;
  float fVar18;
  ulonglong in_xd2;
  float fVar19;
  ulonglong in_xd4;
  ulonglong uVar20;
  ulonglong in_xd6;
  float fVar21;
  ulonglong in_xd8;
  float fVar23;
  ulonglong uVar22;
  ulonglong in_xd10;
  float fVar25;
  ulonglong uVar24;
  undefined8 in_xd12;
  float fVar26;
  undefined8 in_xd14;
  float fVar27;
  char in_FPSCR_PR;
  char in_FPSCR_SZ;
  undefined8 auStack_40 [2];
  undefined8 uStack_30;
  undefined8 uStack_28;
  undefined8 uStack_20;
  undefined8 uStack_18;
  undefined8 local_10;
  undefined4 local_8;
  undefined4 uStack_4;
  float fVar3;
  float fVar12;
  
  fVar12 = (float)(in_xd4 >> 0x20);
  fVar21 = (float)(in_xd6 >> 0x20);
  fVar26 = (float)((ulonglong)in_xd12 >> 0x20);
  fVar27 = (float)((ulonglong)in_xd14 >> 0x20);
  if (param_3 != (ulonglong *)0x0) {
    uVar16 = in_xd0;
    uVar17 = in_xd2;
    uVar22 = in_xd8;
    uVar24 = in_xd10;
    if (param_5 != 0) {
      if (in_FPSCR_PR == '\0') {
        fVar18 = ((float)(param_5 & 0xffff) * 6.2831855) / 65536.0;
        fVar19 = (float)sin(fVar18);
        fVar18 = (float)cos(fVar18);
        in_dr0 = CONCAT44((float)fVar19,(float)fVar18);
      }
      uVar10 = in_dr0;
      uVar13 = in_xd0;
      if (in_FPSCR_SZ == '\0') {
        uVar10 = in_dr0 & 0xffffffff00000000;
        uVar13 = CONCAT44((int)in_dr0,(int)in_dr0);
      }
      uVar13 = uVar13 & 0xffffffff00000000;
      if (in_FPSCR_SZ == '\0') {
        _local_8 = CONCAT44((int)((ulonglong)in_dr12 >> 0x20),local_8);
        puVar1 = &local_10;
        in_dr0 = CONCAT44((int)*param_3,*(undefined4 *)((int)param_3 + 4));
        uVar7 = CONCAT44((int)param_3[1],*(undefined4 *)((int)param_3 + 0xc));
        param_3 = param_3 + 2;
        uVar24 = CONCAT44(param_2,(int)in_dr14);
        uVar22 = CONCAT44(param_1,(int)in_dr12);
      }
      else {
        puVar1 = &uStack_20;
        in_dr0 = *param_3;
        in_xd0 = param_3[1];
        uVar7 = param_3[2];
        in_xd2 = param_3[3];
        param_3 = param_3 + 4;
      }
      if (in_FPSCR_SZ == '\0') {
        uVar16 = CONCAT44((int)param_3[2],*(undefined4 *)((int)param_3 + 0x14));
        uVar17 = CONCAT44((int)param_3[3],*(undefined4 *)((int)param_3 + 0x1c));
        param_3 = param_3 + 4;
      }
      else {
        uVar16 = param_3[2];
        in_xd8 = param_3[3];
        uVar17 = param_3[4];
        in_xd10 = param_3[5];
        param_3 = param_3 + 6;
      }
      fVar18 = (float)(in_xd0 >> 0x20);
      fVar19 = (float)(in_xd2 >> 0x20);
      fVar23 = (float)(in_xd8 >> 0x20);
      fVar25 = (float)(in_xd10 >> 0x20);
      if (in_FPSCR_PR == '\0') {
        fVar3 = (float)in_dr0;
                    /* WARNING: Ignoring partial resolution of indirect */
        fVar4 = (float)(in_dr0 >> 0x20);
        fVar6 = (float)uVar7;
        fVar9 = (float)(uVar7 >> 0x20);
        fVar2 = (float)in_xd0 * fVar3 + (float)in_xd4 * fVar4 + (float)in_xd8 * fVar6 +
                (float)in_xd12 * fVar9;
        fVar2 = fVar18 * fVar3 + fVar12 * fVar4 + fVar23 * fVar6 + fVar26 * fVar9;
        fVar2 = (float)in_xd2 * fVar3 + (float)in_xd6 * fVar4 + (float)in_xd10 * fVar6 +
                (float)in_xd14 * fVar9;
        fVar2 = fVar19 * fVar3 + fVar21 * fVar4 + fVar25 * fVar6 + fVar27 * fVar9;
        fVar3 = (float)uVar10;
                    /* WARNING: Ignoring partial resolution of indirect */
        fVar4 = (float)(uVar10 >> 0x20);
        fVar6 = (float)(uVar13 >> 0x20);
        fVar2 = (float)in_xd0 * fVar3 + (float)in_xd4 * fVar4 + (float)in_xd8 * 0.0 +
                (float)in_xd12 * fVar6;
        fVar12 = fVar18 * fVar3 + fVar12 * fVar4 + fVar23 * 0.0 + fVar26 * fVar6;
        fVar12 = (float)in_xd2 * fVar3 + (float)in_xd6 * fVar4 + (float)in_xd10 * 0.0 +
                 (float)in_xd14 * fVar6;
        fVar12 = fVar19 * fVar3 + fVar21 * fVar4 + fVar25 * 0.0 + fVar27 * fVar6;
      }
      uVar5 = (undefined4)(in_dr0 >> 0x20);
      param_2 = (undefined4)uVar17;
      param_1 = (undefined4)uVar16;
      uVar8 = uVar22 >> 0x20;
      if (in_FPSCR_SZ == '\0') {
        *(int *)((int)param_3 + -4) = (int)uVar13;
        *(int *)(param_3 + -1) = (int)(uVar13 >> 0x20);
        *(int *)((int)param_3 + -0xc) = (int)uVar10;
        *(int *)(param_3 + -2) = (int)(uVar10 >> 0x20);
        *(int *)((int)param_3 + -0x24) = (int)uVar7;
        *(int *)(param_3 + -5) = (int)(uVar7 >> 0x20);
        *(int *)((int)param_3 + -0x2c) = (int)in_dr0;
        param_3 = param_3 + -6;
        *(undefined4 *)param_3 = uVar5;
        in_dr0 = CONCAT44(uVar5,(int)(uVar16 >> 0x20));
        param_2 = (undefined4)(uVar24 >> 0x20);
        in_dr14 = *puVar1;
        in_dr12 = puVar1[1];
        register0x0000003c = (BADSPACEBASE *)(puVar1 + 2);
        uVar16 = in_xd0;
        uVar17 = in_xd2;
        uVar22 = in_xd8;
        uVar24 = in_xd10;
        param_1 = (int)uVar8;
      }
      else {
        param_3[-1] = in_xd6;
        param_3[-2] = uVar13;
        param_3[-3] = in_xd4;
        param_3[-4] = uVar10;
        param_3[-7] = in_xd2;
        param_3[-8] = uVar7;
        param_3[-9] = in_xd0;
        param_3 = param_3 + -10;
        *param_3 = in_dr0;
        in_xd14 = *puVar1;
        in_dr14 = puVar1[1];
        in_xd12 = puVar1[2];
        in_dr12 = puVar1[3];
        register0x0000003c = (BADSPACEBASE *)(puVar1 + 4);
      }
    }
    uVar7 = in_xd6;
    uVar13 = uVar22;
    uVar10 = uVar24;
    if (param_4 != 0) {
      if (in_FPSCR_PR == '\0') {
        fVar12 = ((float)(param_4 & 0xffff) * 6.2831855) / 65536.0;
        fVar21 = (float)sin(fVar12);
        fVar12 = (float)cos(fVar12);
        in_dr0 = CONCAT44((float)fVar21,(float)fVar12);
      }
      uVar8 = in_dr0;
      if (in_FPSCR_SZ == '\0') {
        uVar8 = in_dr0 & 0xffffffff00000000;
      }
      uVar15 = in_dr0 & 0xffffffff;
      if (in_FPSCR_SZ == '\0') {
        *(int *)((int)register0x0000003c + -4) = (int)((ulonglong)in_dr12 >> 0x20);
        *(int *)((int)register0x0000003c + -8) = (int)in_dr12;
        *(int *)((int)register0x0000003c + -0xc) = (int)((ulonglong)in_dr14 >> 0x20);
        puVar1 = (undefined8 *)((int)register0x0000003c + -0x10);
        *(int *)((int)register0x0000003c + -0x10) = (int)in_dr14;
        uVar10 = CONCAT44(param_2,(int)in_dr14);
        uVar13 = CONCAT44(param_1,(int)in_dr12);
        uVar20 = in_xd4;
      }
      else {
        *(undefined8 *)((int)register0x0000003c + -8) = in_dr12;
        *(undefined8 *)((int)register0x0000003c + -0x10) = in_xd12;
        *(undefined8 *)((int)register0x0000003c + -0x18) = in_dr14;
        puVar1 = (undefined8 *)((int)register0x0000003c + -0x20);
        *(undefined8 *)((int)register0x0000003c + -0x20) = in_xd14;
        uVar20 = uVar8;
      }
      if (in_FPSCR_SZ == '\0') {
        uVar11 = CONCAT44((int)param_3[2],*(undefined4 *)((int)param_3 + 0x14));
        uVar14 = CONCAT44((int)param_3[3],*(undefined4 *)((int)param_3 + 0x1c));
        in_xd4 = CONCAT44((int)param_3[4],*(undefined4 *)((int)param_3 + 0x24));
        uVar7 = CONCAT44((int)param_3[5],*(undefined4 *)((int)param_3 + 0x2c));
        param_3 = param_3 + 6;
      }
      else {
        uVar11 = param_3[2];
        uVar20 = param_3[3];
        uVar14 = param_3[4];
        in_xd6 = param_3[5];
        in_xd4 = param_3[6];
        uVar22 = param_3[7];
        uVar7 = param_3[8];
        uVar24 = param_3[9];
        param_3 = param_3 + 10;
      }
      fVar12 = (float)(uVar16 >> 0x20);
      fVar21 = (float)(uVar17 >> 0x20);
      fVar26 = (float)(uVar20 >> 0x20);
      fVar27 = (float)(in_xd6 >> 0x20);
      fVar18 = (float)(uVar22 >> 0x20);
      fVar19 = (float)(uVar24 >> 0x20);
      fVar23 = (float)((ulonglong)in_xd12 >> 0x20);
      fVar25 = (float)((ulonglong)in_xd14 >> 0x20);
      if (in_FPSCR_PR == '\0') {
        fVar3 = (float)in_dr0;
                    /* WARNING: Ignoring partial resolution of indirect */
        fVar4 = (float)uVar8;
        fVar6 = (float)(uVar8 >> 0x20);
        fVar2 = (float)uVar16 * fVar3 + (float)uVar20 * 0.0 + (float)uVar22 * fVar4 +
                (float)in_xd12 * fVar6;
        fVar2 = fVar12 * fVar3 + fVar26 * 0.0 + fVar18 * fVar4 + fVar23 * fVar6;
        fVar2 = (float)uVar17 * fVar3 + (float)in_xd6 * 0.0 + (float)uVar24 * fVar4 +
                (float)in_xd14 * fVar6;
        fVar2 = fVar21 * fVar3 + fVar27 * 0.0 + fVar19 * fVar4 + fVar25 * fVar6;
        fVar3 = (float)uVar11;
                    /* WARNING: Ignoring partial resolution of indirect */
        fVar4 = (float)(uVar11 >> 0x20);
        fVar6 = (float)uVar14;
        fVar9 = (float)(uVar14 >> 0x20);
        fVar2 = (float)uVar16 * fVar3 + (float)uVar20 * fVar4 + (float)uVar22 * fVar6 +
                (float)in_xd12 * fVar9;
        fVar12 = fVar12 * fVar3 + fVar26 * fVar4 + fVar18 * fVar6 + fVar23 * fVar9;
        fVar12 = (float)uVar17 * fVar3 + (float)in_xd6 * fVar4 + (float)uVar24 * fVar6 +
                 (float)in_xd14 * fVar9;
        fVar12 = fVar21 * fVar3 + fVar27 * fVar4 + fVar19 * fVar6 + fVar25 * fVar9;
      }
      if (in_FPSCR_SZ == '\0') {
        *(int *)((int)param_3 + -4) = (int)uVar14;
        *(int *)(param_3 + -1) = (int)(uVar14 >> 0x20);
        *(int *)((int)param_3 + -0xc) = (int)uVar11;
        *(int *)(param_3 + -2) = (int)(uVar11 >> 0x20);
        *(int *)((int)param_3 + -0x14) = (int)uVar8;
        *(int *)(param_3 + -3) = (int)(uVar8 >> 0x20);
        *(int *)((int)param_3 + -0x1c) = (int)uVar15;
        param_3 = param_3 + -4;
        *(int *)param_3 = (int)(uVar15 >> 0x20);
      }
      else {
        param_3[-1] = in_xd6;
        param_3[-2] = uVar14;
        param_3[-3] = uVar20;
        param_3[-4] = uVar11;
        param_3[-5] = uVar17;
        param_3[-6] = uVar8;
        param_3[-7] = uVar16;
        param_3 = param_3 + -8;
        *param_3 = uVar15;
      }
      param_3 = param_3 + -2;
      if (in_FPSCR_SZ == '\0') {
        in_dr14 = *puVar1;
        in_dr12 = puVar1[1];
        register0x0000003c = (BADSPACEBASE *)(puVar1 + 2);
        in_xd4 = uVar20;
        uVar7 = in_xd6;
        uVar13 = uVar22;
        uVar10 = uVar24;
      }
      else {
        in_xd14 = *puVar1;
        in_dr14 = puVar1[1];
        in_xd12 = puVar1[2];
        in_dr12 = puVar1[3];
        register0x0000003c = (BADSPACEBASE *)(puVar1 + 4);
      }
    }
    if (param_6 != 0) {
      if (in_FPSCR_PR == '\0') {
        fVar12 = ((float)(param_6 & 0xffff) * 6.2831855) / 65536.0;
        sin(fVar12);
        cos(fVar12);
      }
      if (in_FPSCR_SZ == '\0') {
        *(int *)((int)register0x0000003c + -4) = (int)((ulonglong)in_dr12 >> 0x20);
        *(int *)((int)register0x0000003c + -8) = (int)in_dr12;
        *(int *)((int)register0x0000003c + -0xc) = (int)((ulonglong)in_dr14 >> 0x20);
        *(int *)((int)register0x0000003c + -0x10) = (int)in_dr14;
        uVar24 = CONCAT44((int)*param_3,*(undefined4 *)((int)param_3 + 4));
        uVar8 = CONCAT44((int)param_3[1],*(undefined4 *)((int)param_3 + 0xc));
        uVar22 = CONCAT44((int)param_3[2],*(undefined4 *)((int)param_3 + 0x14));
        uVar15 = CONCAT44((int)param_3[3],*(undefined4 *)((int)param_3 + 0x1c));
        param_3 = param_3 + 4;
      }
      else {
        *(undefined8 *)((int)register0x0000003c + -8) = in_dr12;
        *(undefined8 *)((int)register0x0000003c + -0x10) = in_xd12;
        *(undefined8 *)((int)register0x0000003c + -0x18) = in_dr14;
        *(undefined8 *)((int)register0x0000003c + -0x20) = in_xd14;
        uVar24 = *param_3;
        uVar16 = param_3[1];
        uVar8 = param_3[2];
        uVar17 = param_3[3];
        uVar22 = param_3[4];
        in_xd4 = param_3[5];
        uVar15 = param_3[6];
        uVar7 = param_3[7];
        param_3 = param_3 + 8;
      }
      fVar12 = (float)(uVar16 >> 0x20);
      fVar21 = (float)(uVar17 >> 0x20);
      fVar26 = (float)(in_xd4 >> 0x20);
      fVar27 = (float)(uVar7 >> 0x20);
      fVar18 = (float)(uVar13 >> 0x20);
      fVar19 = (float)(uVar10 >> 0x20);
      fVar23 = (float)((ulonglong)in_xd12 >> 0x20);
      fVar25 = (float)((ulonglong)in_xd14 >> 0x20);
      if (in_FPSCR_PR == '\0') {
        fVar3 = (float)uVar24;
                    /* WARNING: Ignoring partial resolution of indirect */
        fVar4 = (float)(uVar24 >> 0x20);
        fVar6 = (float)uVar8;
        fVar9 = (float)(uVar8 >> 0x20);
        fVar2 = (float)uVar16 * fVar3 + (float)in_xd4 * fVar4 + (float)uVar13 * fVar6 +
                (float)in_xd12 * fVar9;
        fVar2 = fVar12 * fVar3 + fVar26 * fVar4 + fVar18 * fVar6 + fVar23 * fVar9;
        fVar2 = (float)uVar17 * fVar3 + (float)uVar7 * fVar4 + (float)uVar10 * fVar6 +
                (float)in_xd14 * fVar9;
        fVar2 = fVar21 * fVar3 + fVar27 * fVar4 + fVar19 * fVar6 + fVar25 * fVar9;
        fVar3 = (float)uVar22;
                    /* WARNING: Ignoring partial resolution of indirect */
        fVar4 = (float)(uVar22 >> 0x20);
        fVar6 = (float)uVar15;
        fVar9 = (float)(uVar15 >> 0x20);
        fVar2 = (float)uVar16 * fVar3 + (float)in_xd4 * fVar4 + (float)uVar13 * fVar6 +
                (float)in_xd12 * fVar9;
        fVar12 = fVar12 * fVar3 + fVar26 * fVar4 + fVar18 * fVar6 + fVar23 * fVar9;
        fVar12 = (float)uVar17 * fVar3 + (float)uVar7 * fVar4 + (float)uVar10 * fVar6 +
                 (float)in_xd14 * fVar9;
        fVar12 = fVar21 * fVar3 + fVar27 * fVar4 + fVar19 * fVar6 + fVar25 * fVar9;
      }
      if (in_FPSCR_SZ == '\0') {
        *(int *)((int)param_3 + -4) = (int)uVar15;
        *(int *)(param_3 + -1) = (int)(uVar15 >> 0x20);
        *(int *)((int)param_3 + -0xc) = (int)uVar22;
        *(int *)(param_3 + -2) = (int)(uVar22 >> 0x20);
        *(int *)((int)param_3 + -0x14) = (int)uVar8;
        *(int *)(param_3 + -3) = (int)(uVar8 >> 0x20);
        *(int *)((int)param_3 + -0x1c) = (int)uVar24;
        *(int *)(param_3 + -4) = (int)(uVar24 >> 0x20);
      }
      else {
        param_3[-1] = uVar7;
        param_3[-2] = uVar15;
        param_3[-3] = in_xd4;
        param_3[-4] = uVar22;
        param_3[-5] = uVar17;
        param_3[-6] = uVar8;
        param_3[-7] = uVar16;
        param_3[-8] = uVar24;
      }
    }
    return;
  }
  if ((param_5 != 0) && (in_FPSCR_PR == '\0')) {
    fVar12 = ((float)(param_5 & 0xffff) * 6.2831855) / 65536.0;
    sin(fVar12);
    cos(fVar12);
  }
  if ((param_4 != 0) && (in_FPSCR_PR == '\0')) {
    fVar12 = ((float)(param_4 & 0xffff) * 6.2831855) / 65536.0;
    sin(fVar12);
    cos(fVar12);
  }
  if ((param_6 != 0) && (in_FPSCR_PR == '\0')) {
    fVar12 = ((float)(param_6 & 0xffff) * 6.2831855) / 65536.0;
    sin(fVar12);
    cos(fVar12);
  }
  return;
}

