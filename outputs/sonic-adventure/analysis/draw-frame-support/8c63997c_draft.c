/* Ghidra draft only; not buildable/verified source. */

/* WARNING: Restarted to delay deadcode elimination for space: register */

undefined8 FUN_8c63997c(undefined8 *param_1,undefined8 *param_2)

{
  undefined8 *puVar1;
  undefined8 *puVar2;
  undefined1 *puVar3;
  undefined1 *puVar4;
  float fVar5;
  float fVar7;
  undefined8 uVar6;
  float fVar8;
  undefined4 uVar9;
  undefined4 uVar10;
  float fVar11;
  undefined4 uVar13;
  undefined4 uVar14;
  float fVar15;
  undefined4 uVar16;
  undefined4 uVar17;
  undefined8 uVar18;
  float fVar19;
  undefined4 uVar20;
  undefined8 uVar21;
  undefined8 uVar22;
  undefined8 in_xd0;
  undefined8 in_xd2;
  float fVar24;
  undefined8 uVar23;
  undefined8 in_xd4;
  undefined8 uVar25;
  undefined8 in_xd6;
  float fVar27;
  undefined8 uVar26;
  undefined8 in_xd8;
  float fVar28;
  undefined8 in_xd10;
  float fVar29;
  undefined8 in_xd12;
  float fVar30;
  undefined8 in_xd14;
  float fVar31;
  char in_FPSCR_PR;
  char in_FPSCR_SZ;
  undefined8 uStack_80;
  undefined8 uStack_78;
  undefined8 uStack_70;
  undefined8 uStack_68;
  undefined8 uStack_60;
  undefined8 uStack_58;
  undefined8 uStack_50;
  undefined8 uStack_48;
  undefined8 local_40;
  undefined8 local_38;
  undefined8 local_30;
  undefined8 local_28;
  undefined8 local_20 [3];
  undefined8 local_8;
  float fVar12;
  
  if (param_1 != (undefined8 *)0x0) {
    if (in_FPSCR_SZ == '\0') {
      uVar21 = CONCAT44(*(undefined4 *)param_1,*(undefined4 *)(param_1 + 2));
      uVar22 = CONCAT44(*(undefined4 *)(param_1 + 4),*(undefined4 *)(param_1 + 6));
      uVar9 = *(undefined4 *)((int)param_2 + 0xc);
      uVar6 = CONCAT44(*(undefined4 *)(param_2 + 2),*(undefined4 *)((int)param_2 + 0x14));
      uVar18 = CONCAT44(*(undefined4 *)(param_2 + 3),*(undefined4 *)((int)param_2 + 0x1c));
      puVar1 = param_2 + 4;
    }
    else {
      uVar21 = *param_1;
      uVar22 = param_1[4];
      uVar9 = (undefined4)param_2[2];
      in_xd2 = param_2[3];
      uVar6 = param_2[4];
      uVar18 = param_2[6];
      in_xd6 = param_2[7];
      puVar1 = param_2 + 8;
    }
                    /* WARNING: Ignoring partial resolution of indirect */
    fVar15 = (float)((ulonglong)uVar21 >> 0x20);
    fVar24 = (float)((ulonglong)uVar22 >> 0x20);
    if (in_FPSCR_PR == '\0') {
                    /* WARNING: Ignoring partial resolution of indirect */
      fVar19 = (float)uVar6 * (float)uVar21 + (float)((ulonglong)uVar6 >> 0x20) * fVar15 +
               (float)uVar18 * (float)uVar22 + (float)((ulonglong)uVar18 >> 0x20) * fVar24;
    }
    if (in_FPSCR_SZ == '\0') {
      puVar2 = &local_8;
      local_8 = CONCAT44(uVar9,(int)uVar18);
      uVar9 = *(undefined4 *)((int)puVar1 + 0xc);
      uVar6 = CONCAT44(*(undefined4 *)(puVar1 + 2),*(undefined4 *)((int)puVar1 + 0x14));
      uVar18 = CONCAT44(*(undefined4 *)(puVar1 + 3),*(undefined4 *)((int)puVar1 + 0x1c));
    }
    else {
      local_8 = in_xd2;
      puVar2 = local_20 + 2;
      local_20[2] = in_xd6;
      uVar9 = (undefined4)puVar1[2];
      in_xd2 = puVar1[3];
      uVar6 = puVar1[4];
      uVar18 = puVar1[6];
      in_xd6 = puVar1[7];
    }
                    /* WARNING: Ignoring partial resolution of indirect */
    if (in_FPSCR_PR == '\0') {
                    /* WARNING: Ignoring partial resolution of indirect */
      fVar15 = (float)uVar6 * (float)uVar21 + (float)((ulonglong)uVar6 >> 0x20) * fVar15 +
               (float)uVar18 * (float)uVar22 + (float)((ulonglong)uVar18 >> 0x20) * fVar24;
    }
    if (in_FPSCR_SZ == '\0') {
      *(undefined4 *)((int)puVar2 + -4) = uVar9;
      puVar3 = (undefined1 *)((int)puVar2 + -8);
      *(int *)((int)puVar2 + -8) = (int)uVar18;
      uVar21 = CONCAT44(*(undefined4 *)((int)param_1 + 4),*(undefined4 *)((int)param_1 + 0x14));
      uVar22 = CONCAT44(*(undefined4 *)((int)param_1 + 0x24),*(undefined4 *)((int)param_1 + 0x34));
      uVar9 = *(undefined4 *)((int)param_2 + 0xc);
      uVar6 = CONCAT44(*(undefined4 *)(param_2 + 2),*(undefined4 *)((int)param_2 + 0x14));
      uVar18 = CONCAT44(*(undefined4 *)(param_2 + 3),*(undefined4 *)((int)param_2 + 0x1c));
      puVar1 = param_2 + 4;
    }
    else {
      *(undefined8 *)((int)puVar2 + -8) = in_xd2;
      puVar3 = (undefined1 *)((int)puVar2 + -0x10);
      *(undefined8 *)((int)puVar2 + -0x10) = in_xd6;
      uVar21 = *(undefined8 *)((int)param_1 + 4);
      uVar22 = *(undefined8 *)((int)param_1 + 0x24);
      uVar9 = (undefined4)param_2[2];
      in_xd2 = param_2[3];
      uVar6 = param_2[4];
      uVar18 = param_2[6];
      in_xd6 = param_2[7];
      puVar1 = param_2 + 8;
    }
                    /* WARNING: Ignoring partial resolution of indirect */
    fVar15 = (float)((ulonglong)uVar21 >> 0x20);
    fVar24 = (float)((ulonglong)uVar22 >> 0x20);
    if (in_FPSCR_PR == '\0') {
                    /* WARNING: Ignoring partial resolution of indirect */
      fVar19 = (float)uVar6 * (float)uVar21 + (float)((ulonglong)uVar6 >> 0x20) * fVar15 +
               (float)uVar18 * (float)uVar22 + (float)((ulonglong)uVar18 >> 0x20) * fVar24;
    }
    if (in_FPSCR_SZ == '\0') {
      *(undefined4 *)(puVar3 + -4) = uVar9;
      puVar4 = puVar3 + -8;
      *(int *)(puVar3 + -8) = (int)uVar18;
      uVar9 = *(undefined4 *)((int)puVar1 + 0xc);
      uVar6 = CONCAT44(*(undefined4 *)(puVar1 + 2),*(undefined4 *)((int)puVar1 + 0x14));
      uVar18 = CONCAT44(*(undefined4 *)(puVar1 + 3),*(undefined4 *)((int)puVar1 + 0x1c));
    }
    else {
      *(undefined8 *)(puVar3 + -8) = in_xd2;
      puVar4 = puVar3 + -0x10;
      *(undefined8 *)(puVar3 + -0x10) = in_xd6;
      uVar9 = (undefined4)puVar1[2];
      in_xd2 = puVar1[3];
      uVar6 = puVar1[4];
      uVar18 = puVar1[6];
      in_xd6 = puVar1[7];
    }
                    /* WARNING: Ignoring partial resolution of indirect */
    if (in_FPSCR_PR == '\0') {
                    /* WARNING: Ignoring partial resolution of indirect */
      fVar15 = (float)uVar6 * (float)uVar21 + (float)((ulonglong)uVar6 >> 0x20) * fVar15 +
               (float)uVar18 * (float)uVar22 + (float)((ulonglong)uVar18 >> 0x20) * fVar24;
    }
    if (in_FPSCR_SZ == '\0') {
      *(undefined4 *)(puVar4 + -4) = uVar9;
      puVar3 = puVar4 + -8;
      *(int *)(puVar4 + -8) = (int)uVar18;
      uVar21 = CONCAT44(*(undefined4 *)(param_1 + 1),*(undefined4 *)(param_1 + 3));
      uVar22 = CONCAT44(*(undefined4 *)(param_1 + 5),*(undefined4 *)(param_1 + 7));
      uVar9 = *(undefined4 *)((int)param_2 + 0xc);
      uVar6 = CONCAT44(*(undefined4 *)(param_2 + 2),*(undefined4 *)((int)param_2 + 0x14));
      uVar18 = CONCAT44(*(undefined4 *)(param_2 + 3),*(undefined4 *)((int)param_2 + 0x1c));
      puVar1 = param_2 + 4;
    }
    else {
      *(undefined8 *)(puVar4 + -8) = in_xd2;
      puVar3 = puVar4 + -0x10;
      *(undefined8 *)(puVar4 + -0x10) = in_xd6;
      uVar21 = param_1[1];
      uVar22 = param_1[5];
      uVar9 = (undefined4)param_2[2];
      in_xd2 = param_2[3];
      uVar6 = param_2[4];
      uVar18 = param_2[6];
      in_xd6 = param_2[7];
      puVar1 = param_2 + 8;
    }
                    /* WARNING: Ignoring partial resolution of indirect */
    fVar15 = (float)((ulonglong)uVar21 >> 0x20);
    fVar24 = (float)((ulonglong)uVar22 >> 0x20);
    if (in_FPSCR_PR == '\0') {
                    /* WARNING: Ignoring partial resolution of indirect */
      fVar19 = (float)uVar6 * (float)uVar21 + (float)((ulonglong)uVar6 >> 0x20) * fVar15 +
               (float)uVar18 * (float)uVar22 + (float)((ulonglong)uVar18 >> 0x20) * fVar24;
    }
    if (in_FPSCR_SZ == '\0') {
      *(undefined4 *)(puVar3 + -4) = uVar9;
      puVar4 = puVar3 + -8;
      *(int *)(puVar3 + -8) = (int)uVar18;
      uVar9 = *(undefined4 *)((int)puVar1 + 0xc);
      uVar6 = CONCAT44(*(undefined4 *)(puVar1 + 2),*(undefined4 *)((int)puVar1 + 0x14));
      uVar18 = CONCAT44(*(undefined4 *)(puVar1 + 3),*(undefined4 *)((int)puVar1 + 0x1c));
    }
    else {
      *(undefined8 *)(puVar3 + -8) = in_xd2;
      puVar4 = puVar3 + -0x10;
      *(undefined8 *)(puVar3 + -0x10) = in_xd6;
      uVar9 = (undefined4)puVar1[2];
      in_xd2 = puVar1[3];
      uVar6 = puVar1[4];
      uVar18 = puVar1[6];
      in_xd6 = puVar1[7];
    }
                    /* WARNING: Ignoring partial resolution of indirect */
    if (in_FPSCR_PR == '\0') {
                    /* WARNING: Ignoring partial resolution of indirect */
      fVar15 = (float)uVar6 * (float)uVar21 + (float)((ulonglong)uVar6 >> 0x20) * fVar15 +
               (float)uVar18 * (float)uVar22 + (float)((ulonglong)uVar18 >> 0x20) * fVar24;
    }
    if (in_FPSCR_SZ == '\0') {
      *(undefined4 *)(puVar4 + -4) = uVar9;
      puVar3 = puVar4 + -8;
      *(int *)(puVar4 + -8) = (int)uVar18;
      uVar21 = CONCAT44(*(undefined4 *)((int)param_1 + 0xc),*(undefined4 *)((int)param_1 + 0x1c));
      uVar22 = CONCAT44(*(undefined4 *)((int)param_1 + 0x2c),*(undefined4 *)((int)param_1 + 0x3c));
      uVar9 = *(undefined4 *)((int)param_2 + 0xc);
      uVar6 = CONCAT44(*(undefined4 *)(param_2 + 2),*(undefined4 *)((int)param_2 + 0x14));
      uVar18 = CONCAT44(*(undefined4 *)(param_2 + 3),*(undefined4 *)((int)param_2 + 0x1c));
      param_2 = param_2 + 4;
    }
    else {
      *(undefined8 *)(puVar4 + -8) = in_xd2;
      puVar3 = puVar4 + -0x10;
      *(undefined8 *)(puVar4 + -0x10) = in_xd6;
      uVar21 = *(undefined8 *)((int)param_1 + 0xc);
      uVar22 = *(undefined8 *)((int)param_1 + 0x2c);
      uVar9 = (undefined4)param_2[2];
      in_xd2 = param_2[3];
      uVar6 = param_2[4];
      uVar18 = param_2[6];
      in_xd6 = param_2[7];
      param_2 = param_2 + 8;
    }
                    /* WARNING: Ignoring partial resolution of indirect */
    fVar15 = (float)((ulonglong)uVar21 >> 0x20);
    fVar24 = (float)((ulonglong)uVar22 >> 0x20);
    if (in_FPSCR_PR == '\0') {
                    /* WARNING: Ignoring partial resolution of indirect */
      fVar19 = (float)uVar6 * (float)uVar21 + (float)((ulonglong)uVar6 >> 0x20) * fVar15 +
               (float)uVar18 * (float)uVar22 + (float)((ulonglong)uVar18 >> 0x20) * fVar24;
    }
    if (in_FPSCR_SZ == '\0') {
      *(undefined4 *)(puVar3 + -4) = uVar9;
      puVar1 = (undefined8 *)(puVar3 + -8);
      *(int *)(puVar3 + -8) = (int)uVar18;
      uVar9 = *(undefined4 *)((int)param_2 + 0xc);
      uVar6 = CONCAT44(*(undefined4 *)(param_2 + 2),*(undefined4 *)((int)param_2 + 0x14));
      uVar18 = CONCAT44(*(undefined4 *)(param_2 + 3),*(undefined4 *)((int)param_2 + 0x1c));
    }
    else {
      *(undefined8 *)(puVar3 + -8) = in_xd2;
      puVar1 = (undefined8 *)(puVar3 + -0x10);
      *(undefined8 *)(puVar3 + -0x10) = in_xd6;
      uVar9 = (undefined4)param_2[2];
      in_xd2 = param_2[3];
      uVar6 = param_2[4];
      uVar18 = param_2[6];
      in_xd6 = param_2[7];
    }
                    /* WARNING: Ignoring partial resolution of indirect */
    if (in_FPSCR_PR == '\0') {
                    /* WARNING: Ignoring partial resolution of indirect */
      fVar15 = (float)uVar6 * (float)uVar21 + (float)((ulonglong)uVar6 >> 0x20) * fVar15 +
               (float)uVar18 * (float)uVar22 + (float)((ulonglong)uVar18 >> 0x20) * fVar24;
    }
    if (in_FPSCR_SZ == '\0') {
      *(undefined4 *)((int)puVar1 + -4) = uVar9;
      *(int *)(puVar1 + -1) = (int)uVar18;
      uVar10 = *(undefined4 *)((int)puVar1 + -4);
      uVar14 = *(undefined4 *)puVar1;
      uVar17 = *(undefined4 *)((int)puVar1 + 4);
      uVar9 = *(undefined4 *)(puVar1 + 1);
      uVar13 = *(undefined4 *)((int)puVar1 + 0xc);
      uVar16 = *(undefined4 *)(puVar1 + 2);
      uVar20 = *(undefined4 *)((int)puVar1 + 0x14);
      *(undefined4 *)((int)param_1 + 0x3c) = *(undefined4 *)(puVar1 + -1);
      *(undefined4 *)(param_1 + 7) = uVar9;
      *(undefined4 *)((int)param_1 + 0x2c) = uVar10;
      *(undefined4 *)(param_1 + 5) = uVar13;
      *(undefined4 *)((int)param_1 + 0x1c) = uVar14;
      *(undefined4 *)(param_1 + 3) = uVar16;
      *(undefined4 *)((int)param_1 + 0xc) = uVar17;
      *(undefined4 *)(param_1 + 1) = uVar20;
      uVar10 = *(undefined4 *)((int)puVar1 + 0x1c);
      uVar14 = *(undefined4 *)(puVar1 + 4);
      uVar17 = *(undefined4 *)((int)puVar1 + 0x24);
      uVar9 = *(undefined4 *)(puVar1 + 5);
      uVar6 = CONCAT44(uVar9,*(undefined4 *)(puVar1 + 3));
      uVar13 = *(undefined4 *)((int)puVar1 + 0x2c);
      uVar16 = *(undefined4 *)(puVar1 + 6);
      uVar20 = *(undefined4 *)((int)puVar1 + 0x34);
      *(undefined4 *)((int)param_1 + 0x34) = *(undefined4 *)(puVar1 + 3);
      *(undefined4 *)(param_1 + 6) = uVar9;
      *(undefined4 *)((int)param_1 + 0x24) = uVar10;
      *(undefined4 *)(param_1 + 4) = uVar13;
      *(undefined4 *)((int)param_1 + 0x14) = uVar14;
      *(undefined4 *)(param_1 + 2) = uVar16;
      *(undefined4 *)((int)param_1 + 4) = uVar17;
      *(undefined4 *)param_1 = uVar20;
    }
    else {
      puVar1[-1] = in_xd2;
      puVar1[-2] = in_xd6;
      uVar23 = puVar1[-1];
      uVar25 = *puVar1;
      uVar26 = puVar1[1];
      uVar6 = puVar1[2];
      uVar21 = puVar1[3];
      uVar18 = puVar1[4];
      uVar22 = puVar1[5];
      *(undefined8 *)((int)param_1 + 0x3c) = puVar1[-2];
      param_1[7] = uVar6;
      *(undefined8 *)((int)param_1 + 0x2c) = uVar23;
      param_1[5] = uVar21;
      *(undefined8 *)((int)param_1 + 0x1c) = uVar25;
      param_1[3] = uVar18;
      *(undefined8 *)((int)param_1 + 0xc) = uVar26;
      param_1[1] = uVar22;
      uVar23 = puVar1[7];
      uVar25 = puVar1[8];
      uVar26 = puVar1[9];
      uVar6 = puVar1[10];
      uVar21 = puVar1[0xb];
      uVar18 = puVar1[0xc];
      uVar22 = puVar1[0xd];
      *(undefined8 *)((int)param_1 + 0x34) = puVar1[6];
      param_1[6] = uVar6;
      *(undefined8 *)((int)param_1 + 0x24) = uVar23;
      param_1[4] = uVar21;
      *(undefined8 *)((int)param_1 + 0x14) = uVar25;
      param_1[2] = uVar18;
      *(undefined8 *)((int)param_1 + 4) = uVar26;
      *param_1 = uVar22;
    }
    return uVar6;
  }
  if (in_FPSCR_SZ == '\0') {
    local_40 = CONCAT44(*(undefined4 *)param_2,*(undefined4 *)((int)param_2 + 4));
    local_30 = CONCAT44(*(undefined4 *)(param_2 + 1),*(undefined4 *)((int)param_2 + 0xc));
    local_20[0] = CONCAT44(*(undefined4 *)(param_2 + 2),*(undefined4 *)((int)param_2 + 0x14));
    local_20[2] = CONCAT44(*(undefined4 *)(param_2 + 3),*(undefined4 *)((int)param_2 + 0x1c));
  }
  else {
    local_40 = *param_2;
    in_xd0 = param_2[1];
    local_30 = param_2[2];
    in_xd2 = param_2[3];
    local_20[0] = param_2[4];
    in_xd4 = param_2[5];
    local_20[2] = param_2[6];
    in_xd6 = param_2[7];
  }
                    /* WARNING: Ignoring partial resolution of indirect */
  fVar28 = (float)((ulonglong)in_xd8 >> 0x20);
  fVar30 = (float)((ulonglong)in_xd12 >> 0x20);
  fVar15 = (float)((ulonglong)in_xd0 >> 0x20);
  fVar24 = (float)((ulonglong)in_xd2 >> 0x20);
  fVar19 = (float)((ulonglong)in_xd4 >> 0x20);
  fVar27 = (float)((ulonglong)in_xd6 >> 0x20);
  fVar29 = (float)((ulonglong)in_xd10 >> 0x20);
  fVar31 = (float)((ulonglong)in_xd14 >> 0x20);
  if (in_FPSCR_PR == '\0') {
    fVar11 = (float)local_40;
    fVar7 = (float)((ulonglong)local_40 >> 0x20);
    fVar8 = (float)local_30;
    fVar12 = (float)((ulonglong)local_30 >> 0x20);
    fVar5 = (float)in_xd0 * fVar11 + (float)in_xd4 * fVar7 + (float)in_xd8 * fVar8 +
            (float)in_xd12 * fVar12;
    fVar5 = fVar15 * fVar11 + fVar19 * fVar7 + fVar28 * fVar8 + fVar30 * fVar12;
    fVar5 = (float)in_xd2 * fVar11 + (float)in_xd6 * fVar7 + (float)in_xd10 * fVar8 +
            (float)in_xd14 * fVar12;
    fVar11 = fVar24 * fVar11 + fVar27 * fVar7 + fVar29 * fVar8 + fVar31 * fVar12;
    fVar5 = (float)local_20[0];
                    /* WARNING: Ignoring partial resolution of indirect */
    fVar7 = (float)((ulonglong)local_20[0] >> 0x20);
    fVar8 = (float)local_20[2];
    fVar12 = (float)((ulonglong)local_20[2] >> 0x20);
    fVar11 = (float)in_xd0 * fVar5 + (float)in_xd4 * fVar7 + (float)in_xd8 * fVar8 +
             (float)in_xd12 * fVar12;
    fVar15 = fVar15 * fVar5 + fVar19 * fVar7 + fVar28 * fVar8 + fVar30 * fVar12;
    fVar15 = (float)in_xd2 * fVar5 + (float)in_xd6 * fVar7 + (float)in_xd10 * fVar8 +
             (float)in_xd14 * fVar12;
    fVar15 = fVar24 * fVar5 + fVar27 * fVar7 + fVar29 * fVar8 + fVar31 * fVar12;
  }
  if (in_FPSCR_SZ == '\0') {
    local_8 = CONCAT44((int)local_20[2],(int)((ulonglong)local_20[2] >> 0x20));
    local_20[2] = CONCAT44((int)local_20[0],(int)((ulonglong)local_20[0] >> 0x20));
    local_20[1] = CONCAT44((int)local_30,(int)((ulonglong)local_30 >> 0x20));
    local_20[0] = CONCAT44((int)local_40,(int)((ulonglong)local_40 >> 0x20));
    uVar6 = CONCAT44(*(undefined4 *)local_20,*(undefined4 *)local_20);
  }
  else {
    local_8 = in_xd6;
    local_20[1] = in_xd4;
    local_28 = in_xd2;
    local_38 = in_xd0;
    uVar6 = local_40;
  }
  return uVar6;
}

