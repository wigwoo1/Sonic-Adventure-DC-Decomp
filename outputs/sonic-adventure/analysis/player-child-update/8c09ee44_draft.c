/* Ghidra draft only; not buildable/verified source. */

void FUN_8c09ee44(double param_1,uint param_2,int param_3)

{
  int iVar1;
  double *pdVar2;
  int in_r2;
  undefined4 uVar3;
  undefined8 *puVar4;
  double *pdVar5;
  double *pdVar6;
  undefined8 *puVar7;
  int iVar8;
  byte bVar9;
  uint uVar11;
  double *pdVar12;
  undefined8 *puVar13;
  undefined8 *unaff_r14;
  undefined8 *puVar14;
  double dVar15;
  float in_fr3;
  double dVar16;
  double dVar18;
  float fVar20;
  double dVar21;
  float fVar23;
  float fVar24;
  undefined4 uVar25;
  undefined4 uVar26;
  double dVar27;
  float fVar29;
  double dVar28;
  undefined4 uVar30;
  undefined4 uVar32;
  undefined8 uVar31;
  double in_xd0;
  double in_xd4;
  undefined8 in_xd6;
  double in_xd10;
  double dVar33;
  undefined8 in_xd12;
  double in_xd14;
  char in_FPSCR_PR;
  char in_FPSCR_SZ;
  double dStack_80;
  int local_78;
  undefined1 local_74 [8];
  float local_6c;
  undefined8 auStack_68 [7];
  double dStack_30;
  undefined1 auStack_28 [8];
  undefined8 uStack_20;
  byte bVar10;
  float fVar17;
  float fVar19;
  double dVar22;
  
  puVar7 = auStack_68;
  if (in_FPSCR_SZ == '\0') {
    param_1 = (double)CONCAT44((int)((ulonglong)param_1 >> 0x20),*(undefined4 *)DAT_8c09ef18);
    dVar21 = (double)CONCAT44(*(undefined4 *)DAT_8c09ef1c,*DAT_8c09ef20);
    dVar27 = (double)CONCAT44(*(undefined4 *)((int)DAT_8c09ef18 + 0xc),
                              *(undefined4 *)(DAT_8c09ef18 + 2));
    fVar29 = *DAT_8c09ef14;
  }
  else {
    in_xd6 = *DAT_8c09ef18;
    dVar21 = *DAT_8c09ef1c;
    dVar27 = *(double *)((int)DAT_8c09ef18 + 0xc);
    in_xd10 = *(double *)((int)DAT_8c09ef18 + 0x14);
    fVar29 = (float)((ulonglong)*(undefined8 *)DAT_8c09ef14 >> 0x20);
  }
  if (in_FPSCR_PR == '\0') {
    dStack_80 = (double)CONCAT44(fVar29,1.0 / fVar29);
  }
  else {
    dStack_80 = (double)CONCAT44(fVar29,0x3f800000) / (double)CONCAT44(fVar29,0x3f800000);
  }
  fVar29 = SUB84(dVar27,0);
  fVar24 = (float)((ulonglong)dVar27 >> 0x20);
  if (in_FPSCR_SZ == '\0') {
    stack0xffffff90 = (double)CONCAT44(fVar29,local_74._4_4_);
    pdVar2 = (double *)local_74;
  }
  else {
    stack0xffffff90 = in_xd10;
    pdVar2 = &dStack_80;
  }
  if (in_FPSCR_PR == '\0') {
    fVar23 = (float)((ulonglong)dVar21 >> 0x20) * SUB84(param_1,0);
    fVar20 = SUB84(dVar21,0) * SUB84(param_1,0);
    dVar21 = (double)CONCAT44(1.0 / fVar23,1.0 / fVar20);
    dVar22 = (double)CONCAT44(fVar23 * SUB84(dStack_80,0),fVar20 * SUB84(dStack_80,0));
  }
  else {
    dVar22 = dVar21 * param_1 * param_1;
    dVar21 = (0.007812501848093234 / dVar22) / dVar22;
    dVar22 = dVar22 * dStack_80 * dStack_80;
  }
  if (in_FPSCR_SZ == '\0') {
    dVar15 = (double)CONCAT44(*(undefined4 *)(param_3 + 100),*(undefined4 *)(param_3 + 0x68));
    in_fr3 = *(float *)(param_3 + 0x6c);
    param_3 = param_3 + 0x70;
  }
  else {
    dVar15 = *(double *)(param_3 + 100);
    in_xd0 = *(double *)(param_3 + 0x6c);
    param_3 = param_3 + 0x7c;
  }
  fVar23 = (float)((ulonglong)dVar21 >> 0x20);
  fVar20 = SUB84(dVar21,0);
  if (in_FPSCR_PR == '\0') {
    fVar17 = 1.0 / in_fr3;
    dVar16 = (double)CONCAT44(fVar17,in_fr3);
    dStack_30 = (double)CONCAT44(((float)((ulonglong)dVar15 >> 0x20) - fVar24) * fVar23 * fVar17,
                                 (SUB84(dVar15,0) - fVar29) * fVar20 * fVar17);
  }
  else {
    dVar16 = (double)CONCAT44(0x3f800000,in_fr3) / (double)CONCAT44(0x3f800000,in_fr3);
    fVar17 = (float)((ulonglong)dVar16 >> 0x20);
    dStack_30 = ((dVar15 - dVar27) - dVar27) * dVar21 * dVar21 * dVar16 * dVar16;
  }
  if (in_FPSCR_SZ == '\0') {
    uStack_20 = (double)CONCAT44(fVar17,(undefined4)uStack_20);
    pdVar5 = (double *)(auStack_28 + 4);
    auStack_28._4_4_ = (int)((ulonglong)dStack_30 >> 0x20);
    dVar15 = (double)CONCAT44(*(undefined4 *)(param_3 + -0x2c),*(undefined4 *)(param_3 + -0x28));
    param_1 = (double)(ulonglong)*(uint *)(param_3 + -0x24);
    param_3 = param_3 + -0x20;
  }
  else {
    uStack_20 = dVar16;
    auStack_28 = (undefined1  [8])in_xd0;
    pdVar5 = &dStack_30;
    dVar15 = *(double *)(param_3 + -0x2c);
    in_xd4 = *(double *)(param_3 + -0x24);
    in_xd6 = *(undefined8 *)(param_3 + -0x1c);
    param_3 = param_3 + -0x14;
  }
  fVar17 = SUB84(param_1,0);
  if (in_FPSCR_PR == '\0') {
    fVar19 = 1.0 / fVar17;
    dVar18 = (double)CONCAT44(fVar19,fVar17);
    dVar15 = (double)CONCAT44(((float)((ulonglong)dVar15 >> 0x20) - fVar24) * fVar23 * fVar19,
                              (SUB84(dVar15,0) - fVar29) * fVar20 * fVar19);
  }
  else {
    dVar18 = (double)CONCAT44(0x3f800000,fVar17) / (double)CONCAT44(0x3f800000,fVar17);
    fVar19 = (float)((ulonglong)dVar18 >> 0x20);
    dVar15 = ((dVar15 - dVar27) - dVar27) * dVar21 * dVar21 * dVar18 * dVar18;
  }
  if (in_FPSCR_SZ == '\0') {
    *(float *)((int)pdVar5 + -4) = fVar19;
    *(int *)(pdVar5 + -1) = SUB84(dVar15,0);
    pdVar5 = (double *)((int)pdVar5 + -0xc);
    *(int *)pdVar5 = (int)((ulonglong)dVar15 >> 0x20);
    dVar15 = (double)CONCAT44(*(undefined4 *)(param_3 + -0x2c),*(undefined4 *)(param_3 + -0x28));
    dVar16 = (double)(ulonglong)*(uint *)(param_3 + -0x24);
    param_3 = param_3 + -0x20;
  }
  else {
    pdVar5[-1] = dVar18;
    pdVar5[-2] = in_xd4;
    pdVar5 = pdVar5 + -3;
    *pdVar5 = dVar15;
    dVar15 = *(double *)(param_3 + -0x2c);
    in_xd0 = *(double *)(param_3 + -0x24);
    param_3 = param_3 + -0x14;
  }
  fVar17 = SUB84(dVar16,0);
  if (in_FPSCR_PR == '\0') {
    fVar19 = 1.0 / fVar17;
    dVar16 = (double)CONCAT44(fVar19,fVar17);
    dVar15 = (double)CONCAT44(((float)((ulonglong)dVar15 >> 0x20) - fVar24) * fVar23 * fVar19,
                              (SUB84(dVar15,0) - fVar29) * fVar20 * fVar19);
  }
  else {
    dVar16 = (double)CONCAT44(0x3f800000,fVar17) / (double)CONCAT44(0x3f800000,fVar17);
    fVar19 = (float)((ulonglong)dVar16 >> 0x20);
    dVar15 = ((dVar15 - dVar27) - dVar27) * dVar21 * dVar21 * dVar16 * dVar16;
  }
  if (in_FPSCR_SZ == '\0') {
    *(float *)((int)pdVar5 + -4) = fVar19;
    *(int *)(pdVar5 + -1) = SUB84(dVar15,0);
    pdVar5 = (double *)((int)pdVar5 + -0xc);
    *(int *)pdVar5 = (int)((ulonglong)dVar15 >> 0x20);
    dVar15 = (double)CONCAT44(*(undefined4 *)(param_3 + -0x2c),*(undefined4 *)(param_3 + -0x28));
    dVar18 = (double)(ulonglong)*(uint *)(param_3 + -0x24);
    param_3 = param_3 + -0x20;
  }
  else {
    pdVar5[-1] = dVar16;
    pdVar5[-2] = in_xd0;
    pdVar5 = pdVar5 + -3;
    *pdVar5 = dVar15;
    dVar15 = *(double *)(param_3 + -0x2c);
    in_xd4 = *(double *)(param_3 + -0x24);
    in_xd6 = *(undefined8 *)(param_3 + -0x1c);
    param_3 = param_3 + -0x14;
  }
  fVar17 = SUB84(dVar18,0);
  if (in_FPSCR_PR == '\0') {
    fVar19 = 1.0 / fVar17;
    dVar18 = (double)CONCAT44(fVar19,fVar17);
    dVar21 = (double)CONCAT44(((float)((ulonglong)dVar15 >> 0x20) - fVar24) * fVar23 * fVar19,
                              (SUB84(dVar15,0) - fVar29) * fVar20 * fVar19);
  }
  else {
    dVar18 = (double)CONCAT44(0x3f800000,fVar17) / (double)CONCAT44(0x3f800000,fVar17);
    fVar19 = (float)((ulonglong)dVar18 >> 0x20);
    dVar21 = ((dVar15 - dVar27) - dVar27) * dVar21 * dVar21 * dVar18 * dVar18;
  }
  if (in_FPSCR_SZ == '\0') {
    *(float *)((int)pdVar5 + -4) = fVar19;
    *(int *)(pdVar5 + -1) = SUB84(dVar21,0);
    pdVar5 = (double *)((int)pdVar5 + -0xc);
    *(int *)pdVar5 = (int)((ulonglong)dVar21 >> 0x20);
  }
  else {
    pdVar5[-1] = dVar18;
    pdVar5[-2] = in_xd4;
    pdVar5 = pdVar5 + -3;
    *pdVar5 = dVar21;
  }
  puVar4 = (undefined8 *)(param_3 + -0x10);
  local_78 = 2;
  uVar11 = param_2 >> 1;
  pdVar12 = pdVar5;
  while( true ) {
    pdVar6 = pdVar12 + 3;
    puVar13 = puVar4 + 8;
    bVar9 = PTR_DAT_8c09efb0[uVar11];
    iVar1 = 0;
    iVar8 = 3;
    uVar25 = DAT_8c09efb4;
    puVar14 = unaff_r14;
    do {
      bVar10 = (char)bVar9 >> 1;
      uVar3 = uVar25;
      if ((bVar9 & 1) != 0) {
        if (in_FPSCR_SZ == '\0') {
          dVar15 = (double)CONCAT44(*(undefined4 *)pdVar2,SUB84(dVar27,0));
          pdVar2 = (double *)((int)pdVar2 + 4);
          dVar21 = (double)CONCAT44(*(undefined4 *)pdVar6,*(undefined4 *)((int)pdVar6 + 4));
          dVar16 = (double)CONCAT44(*(undefined4 *)(pdVar6 + 1),SUB84(dVar16,0));
          dVar27 = (double)CONCAT44(*(undefined4 *)pdVar12,*(undefined4 *)((int)pdVar12 + 4));
          dVar18 = (double)CONCAT44(*(undefined4 *)(pdVar12 + 1),SUB84(dVar18,0));
          pdVar12 = (double *)((int)pdVar12 + 0xc);
        }
        else {
          dVar15 = *pdVar2;
          pdVar2 = pdVar2 + 1;
          dVar21 = *pdVar6;
          dVar16 = pdVar6[2];
          dVar27 = *pdVar12;
          in_xd4 = pdVar12[1];
          dVar18 = pdVar12[2];
          pdVar12 = pdVar12 + 3;
        }
        fVar29 = (float)((ulonglong)dVar16 >> 0x20);
        if (in_FPSCR_PR == '\0') {
          dVar15 = (double)CONCAT44((float)((ulonglong)dVar15 >> 0x20) - fVar29,SUB84(dVar15,0));
        }
        else {
          dVar15 = dVar15 - dVar16;
        }
        uVar26 = (undefined4)((ulonglong)dVar18 >> 0x20);
        dVar33 = dVar18;
        if (in_FPSCR_SZ == '\0') {
          dVar15 = (double)CONCAT44((int)((ulonglong)dVar15 >> 0x20),uVar26);
          dVar33 = in_xd10;
        }
        if (in_FPSCR_PR == '\0') {
          fVar24 = SUB84(dVar15,0) - fVar29;
          dVar15 = (double)CONCAT44((float)((ulonglong)dVar15 >> 0x20) / fVar24,fVar24);
        }
        else {
          dVar15 = (dVar15 - dVar16) / (dVar15 - dVar16);
        }
        dVar28 = in_xd4;
        in_xd10 = dVar27;
        if (in_FPSCR_SZ == '\0') {
          dVar15 = (double)CONCAT44((int)((ulonglong)dVar15 >> 0x20),
                                    (int)((ulonglong)dVar27 >> 0x20));
          dVar28 = (double)CONCAT44(SUB84(dVar27,0),SUB84(dStack_80,0));
          in_xd10 = dVar33;
        }
        fVar24 = (float)((ulonglong)dVar21 >> 0x20);
        if (in_FPSCR_PR == '\0') {
          fVar20 = (float)((ulonglong)dVar15 >> 0x20);
          dVar27 = (double)CONCAT44(fVar20,((SUB84(dVar15,0) - fVar24) * fVar20 + fVar24) *
                                           (float)((ulonglong)dVar22 >> 0x20));
          dStack_80 = (double)CONCAT44((((float)((ulonglong)dVar28 >> 0x20) - SUB84(dVar21,0)) *
                                        fVar20 + SUB84(dVar21,0)) * SUB84(dVar22,0),SUB84(dVar28,0))
          ;
        }
        else {
          dVar15 = (dVar15 - dVar21) * (dVar15 - dVar21);
          dVar27 = (dVar15 + dVar21) * dVar22;
          dStack_80 = ((dVar28 - dVar21) * dVar15 + dVar21) * dVar22;
        }
        if (in_FPSCR_SZ == '\0') {
          dVar21 = (double)CONCAT44(*(undefined4 *)pdVar2,*(undefined4 *)((int)pdVar2 + 4));
          pdVar2 = pdVar2 + 1;
        }
        else {
          dVar21 = *pdVar2;
          in_xd14 = pdVar2[1];
          pdVar2 = pdVar2 + 2;
        }
        pdVar2 = (double *)((int)pdVar2 + -0xc);
        if (in_FPSCR_PR == '\0') {
          dVar27 = (double)CONCAT44((int)((ulonglong)dVar27 >> 0x20),
                                    SUB84(dVar27,0) + (float)((ulonglong)dVar21 >> 0x20));
          dStack_80 = (double)CONCAT44((float)((ulonglong)dStack_80 >> 0x20) + SUB84(dVar21,0),
                                       SUB84(dStack_80,0));
        }
        else {
          dVar27 = dVar27 + dVar21;
          dStack_80 = dStack_80 + dVar21;
        }
        if (in_FPSCR_SZ == '\0') {
          dVar16 = (double)CONCAT44(fVar29,*(undefined4 *)(puVar13 + 2));
          dVar18 = (double)CONCAT44(uVar26,*(undefined4 *)((int)puVar13 + 0x14));
          dVar21 = (double)CONCAT44(*(undefined4 *)(puVar4 + 2),*(undefined4 *)((int)puVar4 + 0x14))
          ;
          puVar4 = puVar4 + 3;
        }
        else {
          in_xd6 = puVar13[3];
          dVar21 = (double)puVar4[2];
          in_xd14 = (double)puVar4[3];
          puVar4 = puVar4 + 4;
        }
        puVar4 = puVar4 + -3;
        fVar29 = (float)((ulonglong)dVar27 >> 0x20);
        if (in_FPSCR_PR == '\0') {
          dVar21 = (double)CONCAT44(((float)((ulonglong)dVar21 >> 0x20) - SUB84(dVar16,0)) * fVar29
                                    + SUB84(dVar16,0),
                                    (SUB84(dVar21,0) - SUB84(dVar18,0)) * fVar29 + SUB84(dVar18,0));
        }
        else {
          dVar21 = ((dVar21 - dVar16) - dVar18) * dVar27 * dVar27 + dVar16 + dVar18;
        }
        iVar1 = iVar1 + 1;
        uVar26 = SUB84(dVar27,0);
        uVar32 = (undefined4)((ulonglong)dVar21 >> 0x20);
        uVar30 = (undefined4)((ulonglong)dStack_80 >> 0x20);
        pdVar12 = (double *)((int)pdVar12 + -0xc);
        if ((iVar1 == 3) && (uVar3 = DAT_8c09efe4, (char)bVar10 < '\0')) {
          if (in_FPSCR_SZ == '\0') {
            *(int *)((int)puVar7 + 0x14) = SUB84(dVar21,0);
            *(undefined4 *)(puVar7 + 2) = uVar32;
            *(int *)((int)puVar7 + 0xc) = SUB84(dStack_80,0);
            *(undefined4 *)(puVar7 + 1) = uVar30;
            pdVar6 = (double *)((int)puVar7 + 4);
            *(undefined4 *)pdVar6 = uVar26;
          }
          else {
            puVar7[2] = in_xd14;
            puVar7[1] = dVar21;
            *puVar7 = in_xd12;
            puVar7[-1] = dStack_80;
            pdVar6 = (double *)(puVar7 + -2);
            *pdVar6 = in_xd10;
          }
          puVar7 = (undefined8 *)((int)pdVar6 + -4);
          uVar3 = uVar25;
          puVar14 = puVar7;
        }
        else {
          dVar27 = (double)CONCAT44(0x3f800000,uVar26);
          if (in_FPSCR_SZ == '\0') {
            *(undefined4 *)(in_r2 + 0x18) = 0x3f800000;
            *(int *)(in_r2 + 0x14) = SUB84(dVar21,0);
            *(undefined4 *)(in_r2 + 0x10) = uVar32;
            *(int *)(in_r2 + 0xc) = SUB84(dStack_80,0);
            *(undefined4 *)(in_r2 + 8) = uVar30;
            pdVar6 = (double *)(in_r2 + 4);
            *(undefined4 *)pdVar6 = uVar26;
          }
          else {
            *(double *)(in_r2 + 0x14) = dVar27;
            *(double *)(in_r2 + 0xc) = in_xd14;
            *(double *)(in_r2 + 4) = dVar21;
            *(undefined8 *)(in_r2 + -4) = in_xd12;
            *(double *)(in_r2 + -0xc) = dStack_80;
            pdVar6 = (double *)(in_r2 + -0x14);
            *pdVar6 = in_xd10;
          }
          *(undefined4 *)((int)pdVar6 + -4) = uVar3;
          in_r2 = (int)pdVar6 + 0x1c;
        }
      }
      pdVar6 = pdVar12;
      uVar25 = SUB84(dVar27,0);
      bVar9 = (char)bVar9 >> 2;
      puVar13 = puVar4;
      unaff_r14 = puVar14;
      if (((bVar10 & 1) != 0) &&
         ((iVar1 = iVar1 + 1, uVar26 = uVar3, iVar1 != 3 ||
          (uVar26 = DAT_8c09f058, unaff_r14 = puVar4, -1 < (char)bVar9)))) {
        if (in_FPSCR_SZ == '\0') {
          uVar25 = *(undefined4 *)((int)puVar4 + 4);
          dStack_80 = (double)CONCAT44(*(undefined4 *)(puVar4 + 1),SUB84(dStack_80,0));
          dVar18 = (double)(ulonglong)*(uint *)((int)puVar4 + 0xc);
          uVar31 = CONCAT44(*(undefined4 *)(puVar4 + 2),*(undefined4 *)((int)puVar4 + 0x14));
          puVar4 = puVar4 + 3;
        }
        else {
          in_xd10 = *(double *)((int)puVar4 + 4);
          dStack_80 = *(double *)((int)puVar4 + 0xc);
          in_xd6 = *(undefined8 *)((int)puVar4 + 0x14);
          uVar31 = *(undefined8 *)((int)puVar4 + 0x1c);
          in_xd14 = *(double *)((int)puVar4 + 0x24);
          puVar4 = (undefined8 *)((int)puVar4 + 0x2c);
        }
        dVar27 = (double)CONCAT44(0x3f800000,uVar25);
        if (in_FPSCR_SZ == '\0') {
          *(undefined4 *)(in_r2 + 0x18) = 0x3f800000;
          *(int *)(in_r2 + 0x14) = (int)uVar31;
          *(int *)(in_r2 + 0x10) = (int)((ulonglong)uVar31 >> 0x20);
          *(int *)(in_r2 + 0xc) = SUB84(dVar18,0);
          *(int *)(in_r2 + 8) = (int)((ulonglong)dStack_80 >> 0x20);
          pdVar12 = (double *)(in_r2 + 4);
          *(undefined4 *)pdVar12 = uVar25;
        }
        else {
          *(double *)(in_r2 + 0x14) = dVar27;
          *(double *)(in_r2 + 0xc) = in_xd14;
          *(undefined8 *)(in_r2 + 4) = uVar31;
          *(undefined8 *)(in_r2 + -4) = in_xd6;
          *(double *)(in_r2 + -0xc) = dStack_80;
          pdVar12 = (double *)(in_r2 + -0x14);
          *pdVar12 = in_xd10;
        }
        *(undefined4 *)((int)pdVar12 + -4) = uVar26;
        in_r2 = (int)pdVar12 + 0x1c;
        uVar3 = uVar26;
        puVar13 = puVar4 + -3;
        unaff_r14 = puVar14;
      }
      uVar30 = DAT_8c09f058;
      uVar26 = SUB84(dVar27,0);
      pdVar12 = (double *)((int)pdVar6 + 0xc);
      iVar8 = iVar8 + -1;
      puVar4 = puVar13 + 4;
      uVar25 = uVar3;
      puVar14 = unaff_r14;
    } while (iVar8 != 0);
    if ((char)bVar9 < '\0') {
      if (in_FPSCR_SZ == '\0') {
        uVar26 = *(undefined4 *)((int)unaff_r14 + 4);
        dStack_80 = (double)CONCAT44(*(undefined4 *)(unaff_r14 + 1),
                                     *(undefined4 *)((int)unaff_r14 + 0xc));
        uVar31 = CONCAT44(*(undefined4 *)(unaff_r14 + 2),*(undefined4 *)((int)unaff_r14 + 0x14));
        unaff_r14 = unaff_r14 + 3;
      }
      else {
        in_xd10 = *(double *)((int)unaff_r14 + 4);
        dStack_80 = *(double *)((int)unaff_r14 + 0xc);
        in_xd12 = *(undefined8 *)((int)unaff_r14 + 0x14);
        uVar31 = *(undefined8 *)((int)unaff_r14 + 0x1c);
        in_xd14 = *(double *)((int)unaff_r14 + 0x24);
        unaff_r14 = (undefined8 *)((int)unaff_r14 + 0x2c);
      }
      dVar27 = (double)CONCAT44(0x3f800000,uVar26);
      if (in_FPSCR_SZ == '\0') {
        *(undefined4 *)(in_r2 + 0x18) = 0x3f800000;
        *(int *)(in_r2 + 0x14) = (int)uVar31;
        *(int *)(in_r2 + 0x10) = (int)((ulonglong)uVar31 >> 0x20);
        *(int *)(in_r2 + 0xc) = SUB84(dStack_80,0);
        *(int *)(in_r2 + 8) = (int)((ulonglong)dStack_80 >> 0x20);
        pdVar12 = (double *)(in_r2 + 4);
        *(undefined4 *)pdVar12 = uVar26;
      }
      else {
        *(double *)(in_r2 + 0x14) = dVar27;
        *(double *)(in_r2 + 0xc) = in_xd14;
        *(undefined8 *)(in_r2 + 4) = uVar31;
        *(undefined8 *)(in_r2 + -4) = in_xd12;
        *(double *)(in_r2 + -0xc) = dStack_80;
        pdVar12 = (double *)(in_r2 + -0x14);
        *pdVar12 = in_xd10;
      }
      *(undefined4 *)((int)pdVar12 + -4) = uVar30;
      in_r2 = (int)pdVar12 + 0x1c;
    }
    local_78 = local_78 + -1;
    if (local_78 == 0) break;
    puVar4 = (undefined8 *)(param_3 + 0x10);
    pdVar12 = (double *)((int)pdVar5 + 0xc);
    uVar11 = param_2 & 7;
  }
  return;
}

