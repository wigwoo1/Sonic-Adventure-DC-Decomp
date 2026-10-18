/* Ghidra draft only; not buildable/verified source. */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_8c61f108(double param_1,double param_2,ushort *param_3)

{
  ushort uVar1;
  int *piVar2;
  int *piVar3;
  int *piVar4;
  uint uVar5;
  ulonglong *puVar6;
  double *pdVar7;
  undefined8 *puVar8;
  int iVar9;
  undefined8 *puVar10;
  undefined8 *puVar11;
  int iVar12;
  undefined4 *unaff_r8;
  double *pdVar13;
  int iVar14;
  int iVar15;
  ushort *puVar16;
  ushort *puVar17;
  ushort *puVar18;
  ushort *puVar19;
  undefined8 *puVar20;
  double in_dr0;
  double dVar21;
  undefined8 uVar22;
  undefined4 uVar23;
  float fVar24;
  double in_dr2;
  double dVar25;
  ulonglong uVar26;
  uint uVar27;
  double dVar28;
  undefined4 uVar29;
  float fVar30;
  int *piVar31;
  double dVar32;
  double in_xd4;
  double in_xd6;
  bool bVar33;
  char in_FPSCR_PR;
  char in_FPSCR_SZ;
  
  piVar3 = DAT_8c61f174;
  piVar2 = DAT_8c61f16c._4_4_;
  piVar31 = DAT_8c61f16c._4_4_;
  if (in_FPSCR_SZ == '\0') {
    piVar31 = (int *)DAT_8c61f16c;
  }
  puVar16 = *(ushort **)(param_3 + 2);
  uVar5 = *param_3 & DAT_8c61f178;
  dVar32 = (double)CONCAT44(piVar31,0x3f800000);
  if (uVar5 == 0) {
    *DAT_8c61f174 = *DAT_8c61f174 + (uint)param_3[1] * 3;
    iVar14 = 0;
    while (iVar14 < (int)(uint)param_3[1]) {
      uVar1 = *puVar16;
      puVar19 = puVar16 + 1;
      puVar17 = puVar16 + 2;
      iVar15 = *DAT_8c61f354;
      puVar16 = puVar16 + 3;
      puVar8 = (undefined8 *)((uint)*puVar19 * 0x20 + iVar15);
      if (in_FPSCR_SZ == '\0') {
        dVar28 = (double)CONCAT44(*(undefined4 *)(puVar8 + 2),SUB84(param_1,0));
      }
      else {
        dVar28 = (double)puVar8[2];
      }
      puVar10 = (undefined8 *)((uint)uVar1 * 0x20 + iVar15);
      uVar23 = (undefined4)((ulonglong)dVar28 >> 0x20);
      dVar25 = dVar28;
      if (in_FPSCR_SZ == '\0') {
        dVar25 = (double)CONCAT44(uVar23,*(undefined4 *)(puVar10 + 2));
      }
      if (in_FPSCR_PR == '\0') {
        dVar25 = (double)CONCAT44((float)((ulonglong)dVar25 >> 0x20) - SUB84(dVar25,0),
                                  SUB84(dVar25,0));
      }
      else {
        dVar25 = dVar25 - dVar25;
      }
      puVar11 = (undefined8 *)((uint)*puVar17 * 0x20 + iVar15);
      if (in_FPSCR_SZ == '\0') {
        fVar30 = *(float *)((int)puVar8 + 0x14);
        dVar28 = (double)CONCAT44(uVar23,fVar30);
        dVar25 = (double)CONCAT44((int)((ulonglong)dVar25 >> 0x20),
                                  *(undefined4 *)((int)puVar11 + 0x14));
        in_dr0 = (double)CONCAT44((int)((ulonglong)in_dr0 >> 0x20),fVar30);
      }
      else {
        fVar30 = SUB84(dVar28,0);
      }
      if (in_FPSCR_PR == '\0') {
        fVar24 = SUB84(dVar25,0) - fVar30;
        dVar25 = (double)CONCAT44((float)((ulonglong)dVar25 >> 0x20) * fVar24,fVar24);
      }
      else {
        dVar25 = (dVar25 - dVar28) * (dVar25 - dVar28);
      }
      if (in_FPSCR_SZ == '\0') {
        dVar25 = (double)CONCAT44((int)((ulonglong)dVar25 >> 0x20),
                                  *(undefined4 *)((int)puVar10 + 0x14));
      }
      if (in_FPSCR_PR == '\0') {
        dVar21 = (double)CONCAT44((int)((ulonglong)in_dr0 >> 0x20),SUB84(in_dr0,0) - SUB84(dVar25,0)
                                 );
      }
      else {
        dVar21 = in_dr0 - dVar25;
      }
      if (in_FPSCR_SZ == '\0') {
        dVar25 = (double)CONCAT44((int)((ulonglong)dVar25 >> 0x20),*(undefined4 *)(puVar11 + 2));
      }
      if (in_FPSCR_PR == '\0') {
        dVar25 = (double)CONCAT44((int)((ulonglong)dVar25 >> 0x20),
                                  SUB84(dVar25,0) - (float)((ulonglong)dVar28 >> 0x20));
      }
      else {
        dVar25 = dVar25 - dVar28;
      }
      uVar5 = (uint)((ulonglong)dVar25 >> 0x20);
      param_1 = dVar25;
      if (in_FPSCR_SZ == '\0') {
        param_1 = (double)CONCAT44(uVar5,fVar30);
      }
      if (in_FPSCR_PR == '\0') {
        in_dr0 = (double)CONCAT44((int)((ulonglong)dVar21 >> 0x20),SUB84(dVar21,0) * SUB84(dVar25,0)
                                 );
      }
      else {
        in_dr0 = dVar21 * dVar25;
      }
      dVar28 = (double)((ulonglong)uVar5 << 0x20);
      if (in_FPSCR_PR == '\0') {
        fVar30 = (float)((ulonglong)param_1 >> 0x20) - SUB84(in_dr0,0);
        param_1 = (double)CONCAT44(fVar30,SUB84(param_1,0));
        bVar33 = 0.0 < fVar30;
      }
      else {
        param_1 = param_1 - in_dr0;
        bVar33 = dVar28 < param_1;
      }
      iVar14 = iVar14 + 1;
      if (!bVar33) {
        if (in_FPSCR_SZ == '\0') {
          pdVar13 = (double *)(unaff_r8 + 6);
          *(undefined4 *)pdVar13 = 0x3f800000;
        }
        else {
          pdVar13 = (double *)(unaff_r8 + 5);
          *pdVar13 = in_xd6;
        }
        iVar15 = *piVar2;
        if (in_FPSCR_SZ == '\0') {
          dVar25 = (double)CONCAT44(*(undefined4 *)((int)puVar10 + 4),SUB84(in_dr0,0));
          dVar28 = (double)CONCAT44(uVar5,*(undefined4 *)(iVar15 + 0x14));
        }
        else {
          dVar25 = *(double *)((int)puVar10 + 4);
        }
        if (in_FPSCR_PR == '\0') {
          fVar30 = SUB84(dVar28,0) * (float)((ulonglong)dVar25 >> 0x20);
        }
        else {
          fVar30 = SUB84(dVar28 * dVar25,0);
        }
        if (in_FPSCR_SZ == '\0') {
          uVar26 = CONCAT44(*(undefined4 *)(iVar15 + 4),fVar30);
          uVar22 = CONCAT44(*(undefined4 *)puVar10,SUB84(dVar25,0));
        }
        else {
          uVar26 = *(ulonglong *)(iVar15 + 4);
          uVar22 = *puVar10;
        }
        if (in_FPSCR_PR == '\0') {
          uVar26 = (ulonglong)
                   (uint)((float)((ulonglong)uVar22 >> 0x20) * (float)(uVar26 >> 0x20) +
                         (float)uVar26);
        }
        if (in_FPSCR_SZ == '\0') {
          uVar22 = CONCAT44(*(undefined4 *)(puVar10 + 1),(int)uVar22);
          uVar26 = CONCAT44(*(undefined4 *)(iVar15 + 0x24),(int)uVar26);
        }
        else {
          uVar22 = puVar10[1];
          uVar26 = *(ulonglong *)(iVar15 + 0x24);
        }
        if (in_FPSCR_PR == '\0') {
          uVar26 = (ulonglong)
                   (uint)((float)((ulonglong)uVar22 >> 0x20) * (float)(uVar26 >> 0x20) +
                         (float)uVar26);
        }
        dVar25 = dVar32;
        dVar28 = dVar32;
        if (in_FPSCR_SZ == '\0') {
          dVar25 = (double)CONCAT44(piVar31,(int)uVar22);
          dVar28 = (double)CONCAT44(piVar31,(int)uVar26);
        }
        if (in_FPSCR_PR == '\0') {
          dVar28 = (double)CONCAT44((float)((ulonglong)dVar25 >> 0x20) * SUB84(dVar28,0) +
                                    (float)((ulonglong)dVar28 >> 0x20),SUB84(dVar28,0));
        }
        uVar23 = (undefined4)((ulonglong)dVar28 >> 0x20);
        if (in_FPSCR_SZ == '\0') {
          pdVar13 = (double *)((int)pdVar13 + -4);
          *(undefined4 *)pdVar13 = uVar23;
        }
        else {
          pdVar13 = pdVar13 + -1;
          *pdVar13 = dVar28;
        }
        uVar29 = DAT_8c61f358;
        puVar6 = (ulonglong *)*piVar2;
        if (in_FPSCR_SZ == '\0') {
          dVar25 = (double)CONCAT44(*(undefined4 *)((int)puVar10 + 4),SUB84(dVar25,0));
          dVar28 = (double)CONCAT44(uVar23,(int)puVar6[2]);
        }
        else {
          dVar25 = *(double *)((int)puVar10 + 4);
        }
        if (in_FPSCR_PR == '\0') {
          fVar30 = SUB84(dVar28,0) * (float)((ulonglong)dVar25 >> 0x20);
        }
        else {
          fVar30 = SUB84(dVar28 * dVar25,0);
        }
        if (in_FPSCR_SZ == '\0') {
          uVar22 = CONCAT44(*(undefined4 *)puVar10,SUB84(dVar25,0));
          uVar26 = CONCAT44((int)*puVar6,fVar30);
        }
        else {
          uVar22 = *puVar10;
          uVar26 = *puVar6;
        }
        if (in_FPSCR_PR == '\0') {
          uVar26 = (ulonglong)
                   (uint)((float)((ulonglong)uVar22 >> 0x20) * (float)(uVar26 >> 0x20) +
                         (float)uVar26);
        }
        if (in_FPSCR_SZ == '\0') {
          uVar22 = CONCAT44(*(undefined4 *)(puVar10 + 1),(int)uVar22);
          uVar26 = CONCAT44((int)puVar6[4],(int)uVar26);
        }
        else {
          uVar22 = puVar10[1];
          uVar26 = puVar6[4];
        }
        if (in_FPSCR_PR == '\0') {
          uVar26 = (ulonglong)
                   (uint)((float)((ulonglong)uVar22 >> 0x20) * (float)(uVar26 >> 0x20) +
                         (float)uVar26);
        }
        dVar25 = dVar32;
        dVar28 = dVar32;
        if (in_FPSCR_SZ == '\0') {
          dVar25 = (double)CONCAT44(piVar31,(int)uVar26);
          dVar28 = (double)CONCAT44(piVar31,(int)uVar22);
        }
        if (in_FPSCR_PR == '\0') {
          dVar25 = (double)CONCAT44((float)((ulonglong)dVar28 >> 0x20) * SUB84(dVar25,0) +
                                    (float)((ulonglong)dVar25 >> 0x20),SUB84(dVar25,0));
        }
        uVar23 = (undefined4)((ulonglong)dVar25 >> 0x20);
        if (in_FPSCR_SZ == '\0') {
          *(undefined4 *)((int)pdVar13 + -4) = uVar23;
          *(undefined4 *)(pdVar13 + -1) = *(undefined4 *)(puVar10 + 3);
          *(undefined4 *)((int)pdVar13 + -0xc) = *(undefined4 *)((int)puVar10 + 0x14);
          dVar25 = (double)CONCAT44(uVar23,*(undefined4 *)(puVar10 + 2));
          pdVar13 = pdVar13 + -2;
          *(undefined4 *)pdVar13 = *(undefined4 *)(puVar10 + 2);
        }
        else {
          pdVar13[-1] = dVar25;
          pdVar13[-2] = (double)puVar10[3];
          pdVar13[-3] = *(double *)((int)puVar10 + 0x14);
          pdVar13 = pdVar13 + -4;
          *pdVar13 = (double)puVar10[2];
        }
        *(undefined4 *)((int)pdVar13 + -4) = uVar29;
        if (in_FPSCR_SZ == '\0') {
          pdVar13 = (double *)((int)pdVar13 + 0x34);
          *(undefined4 *)pdVar13 = 0x3f800000;
        }
        else {
          pdVar13 = pdVar13 + 6;
          *pdVar13 = in_xd6;
        }
        iVar15 = *piVar2;
        if (in_FPSCR_SZ == '\0') {
          dVar28 = (double)CONCAT44(*(undefined4 *)((int)puVar8 + 4),SUB84(dVar28,0));
          dVar25 = (double)CONCAT44((int)((ulonglong)dVar25 >> 0x20),*(undefined4 *)(iVar15 + 0x14))
          ;
        }
        else {
          dVar28 = *(double *)((int)puVar8 + 4);
        }
        if (in_FPSCR_PR == '\0') {
          fVar30 = SUB84(dVar25,0) * (float)((ulonglong)dVar28 >> 0x20);
        }
        else {
          fVar30 = SUB84(dVar25 * dVar28,0);
        }
        if (in_FPSCR_SZ == '\0') {
          uVar26 = CONCAT44(*(undefined4 *)(iVar15 + 4),fVar30);
          uVar22 = CONCAT44(*(undefined4 *)puVar8,SUB84(dVar28,0));
        }
        else {
          uVar26 = *(ulonglong *)(iVar15 + 4);
          uVar22 = *puVar8;
        }
        if (in_FPSCR_PR == '\0') {
          uVar26 = (ulonglong)
                   (uint)((float)((ulonglong)uVar22 >> 0x20) * (float)(uVar26 >> 0x20) +
                         (float)uVar26);
        }
        if (in_FPSCR_SZ == '\0') {
          uVar22 = CONCAT44(*(undefined4 *)(puVar8 + 1),(int)uVar22);
          uVar26 = CONCAT44(*(undefined4 *)(iVar15 + 0x24),(int)uVar26);
        }
        else {
          uVar22 = puVar8[1];
          uVar26 = *(ulonglong *)(iVar15 + 0x24);
        }
        if (in_FPSCR_PR == '\0') {
          uVar26 = (ulonglong)
                   (uint)((float)((ulonglong)uVar22 >> 0x20) * (float)(uVar26 >> 0x20) +
                         (float)uVar26);
        }
        dVar25 = dVar32;
        dVar28 = dVar32;
        if (in_FPSCR_SZ == '\0') {
          dVar25 = (double)CONCAT44(piVar31,(int)uVar22);
          dVar28 = (double)CONCAT44(piVar31,(int)uVar26);
        }
        if (in_FPSCR_PR == '\0') {
          dVar28 = (double)CONCAT44((float)((ulonglong)dVar25 >> 0x20) * SUB84(dVar28,0) +
                                    (float)((ulonglong)dVar28 >> 0x20),SUB84(dVar28,0));
        }
        uVar23 = (undefined4)((ulonglong)dVar28 >> 0x20);
        if (in_FPSCR_SZ == '\0') {
          pdVar13 = (double *)((int)pdVar13 + -4);
          *(undefined4 *)pdVar13 = uVar23;
        }
        else {
          pdVar13 = pdVar13 + -1;
          *pdVar13 = dVar28;
        }
        uVar29 = DAT_8c61f358;
        puVar6 = (ulonglong *)*piVar2;
        if (in_FPSCR_SZ == '\0') {
          dVar25 = (double)CONCAT44(*(undefined4 *)((int)puVar8 + 4),SUB84(dVar25,0));
          dVar28 = (double)CONCAT44(uVar23,(int)puVar6[2]);
        }
        else {
          dVar25 = *(double *)((int)puVar8 + 4);
        }
        if (in_FPSCR_PR == '\0') {
          fVar30 = SUB84(dVar28,0) * (float)((ulonglong)dVar25 >> 0x20);
        }
        else {
          fVar30 = SUB84(dVar28 * dVar25,0);
        }
        if (in_FPSCR_SZ == '\0') {
          uVar22 = CONCAT44(*(undefined4 *)puVar8,SUB84(dVar25,0));
          uVar26 = CONCAT44((int)*puVar6,fVar30);
        }
        else {
          uVar22 = *puVar8;
          uVar26 = *puVar6;
        }
        if (in_FPSCR_PR == '\0') {
          uVar26 = (ulonglong)
                   (uint)((float)((ulonglong)uVar22 >> 0x20) * (float)(uVar26 >> 0x20) +
                         (float)uVar26);
        }
        if (in_FPSCR_SZ == '\0') {
          uVar22 = CONCAT44(*(undefined4 *)(puVar8 + 1),(int)uVar22);
          uVar26 = CONCAT44((int)puVar6[4],(int)uVar26);
        }
        else {
          uVar22 = puVar8[1];
          uVar26 = puVar6[4];
        }
        if (in_FPSCR_PR == '\0') {
          uVar26 = (ulonglong)
                   (uint)((float)((ulonglong)uVar22 >> 0x20) * (float)(uVar26 >> 0x20) +
                         (float)uVar26);
        }
        dVar25 = dVar32;
        dVar28 = dVar32;
        if (in_FPSCR_SZ == '\0') {
          dVar25 = (double)CONCAT44(piVar31,(int)uVar26);
          dVar28 = (double)CONCAT44(piVar31,(int)uVar22);
        }
        if (in_FPSCR_PR == '\0') {
          dVar25 = (double)CONCAT44((float)((ulonglong)dVar28 >> 0x20) * SUB84(dVar25,0) +
                                    (float)((ulonglong)dVar25 >> 0x20),SUB84(dVar25,0));
        }
        uVar23 = (undefined4)((ulonglong)dVar25 >> 0x20);
        if (in_FPSCR_SZ == '\0') {
          *(undefined4 *)((int)pdVar13 + -4) = uVar23;
          *(undefined4 *)(pdVar13 + -1) = *(undefined4 *)(puVar8 + 3);
          *(undefined4 *)((int)pdVar13 + -0xc) = *(undefined4 *)((int)puVar8 + 0x14);
          dVar25 = (double)CONCAT44(uVar23,*(undefined4 *)(puVar8 + 2));
          pdVar13 = pdVar13 + -2;
          *(undefined4 *)pdVar13 = *(undefined4 *)(puVar8 + 2);
        }
        else {
          pdVar13[-1] = dVar25;
          pdVar13[-2] = (double)puVar8[3];
          pdVar13[-3] = *(double *)((int)puVar8 + 0x14);
          pdVar13 = pdVar13 + -4;
          *pdVar13 = (double)puVar8[2];
        }
        *(undefined4 *)((int)pdVar13 + -4) = uVar29;
        if (in_FPSCR_SZ == '\0') {
          pdVar13 = (double *)((int)pdVar13 + 0x34);
          *(undefined4 *)pdVar13 = 0x3f800000;
        }
        else {
          pdVar13 = pdVar13 + 6;
          *pdVar13 = in_xd6;
        }
        iVar15 = *piVar2;
        if (in_FPSCR_SZ == '\0') {
          dVar28 = (double)CONCAT44(*(undefined4 *)((int)puVar11 + 4),SUB84(dVar28,0));
          dVar25 = (double)CONCAT44((int)((ulonglong)dVar25 >> 0x20),*(undefined4 *)(iVar15 + 0x14))
          ;
        }
        else {
          dVar28 = *(double *)((int)puVar11 + 4);
        }
        if (in_FPSCR_PR == '\0') {
          fVar30 = SUB84(dVar25,0) * (float)((ulonglong)dVar28 >> 0x20);
        }
        else {
          fVar30 = SUB84(dVar25 * dVar28,0);
        }
        if (in_FPSCR_SZ == '\0') {
          uVar26 = CONCAT44(*(undefined4 *)(iVar15 + 4),fVar30);
          uVar22 = CONCAT44(*(undefined4 *)puVar11,SUB84(dVar28,0));
        }
        else {
          uVar26 = *(ulonglong *)(iVar15 + 4);
          uVar22 = *puVar11;
        }
        if (in_FPSCR_PR == '\0') {
          uVar26 = (ulonglong)
                   (uint)((float)((ulonglong)uVar22 >> 0x20) * (float)(uVar26 >> 0x20) +
                         (float)uVar26);
        }
        if (in_FPSCR_SZ == '\0') {
          uVar22 = CONCAT44(*(undefined4 *)(puVar11 + 1),(int)uVar22);
          uVar26 = CONCAT44(*(undefined4 *)(iVar15 + 0x24),(int)uVar26);
        }
        else {
          uVar22 = puVar11[1];
          uVar26 = *(ulonglong *)(iVar15 + 0x24);
        }
        if (in_FPSCR_PR == '\0') {
          uVar26 = (ulonglong)
                   (uint)((float)((ulonglong)uVar22 >> 0x20) * (float)(uVar26 >> 0x20) +
                         (float)uVar26);
        }
        dVar25 = dVar32;
        dVar28 = dVar32;
        if (in_FPSCR_SZ == '\0') {
          dVar25 = (double)CONCAT44(piVar31,(int)uVar22);
          dVar28 = (double)CONCAT44(piVar31,(int)uVar26);
        }
        if (in_FPSCR_PR == '\0') {
          dVar28 = (double)CONCAT44((float)((ulonglong)dVar25 >> 0x20) * SUB84(dVar28,0) +
                                    (float)((ulonglong)dVar28 >> 0x20),SUB84(dVar28,0));
        }
        uVar23 = (undefined4)((ulonglong)dVar28 >> 0x20);
        if (in_FPSCR_SZ == '\0') {
          pdVar13 = (double *)((int)pdVar13 + -4);
          *(undefined4 *)pdVar13 = uVar23;
        }
        else {
          pdVar13 = pdVar13 + -1;
          *pdVar13 = dVar28;
        }
        uVar29 = DAT_8c61f35c;
        puVar6 = (ulonglong *)*piVar2;
        if (in_FPSCR_SZ == '\0') {
          dVar25 = (double)CONCAT44(*(undefined4 *)((int)puVar11 + 4),SUB84(dVar25,0));
          dVar28 = (double)CONCAT44(uVar23,(int)puVar6[2]);
        }
        else {
          dVar25 = *(double *)((int)puVar11 + 4);
        }
        if (in_FPSCR_PR == '\0') {
          fVar30 = SUB84(dVar28,0) * (float)((ulonglong)dVar25 >> 0x20);
        }
        else {
          fVar30 = SUB84(dVar28 * dVar25,0);
        }
        if (in_FPSCR_SZ == '\0') {
          uVar22 = CONCAT44(*(undefined4 *)puVar11,SUB84(dVar25,0));
          uVar26 = CONCAT44((int)*puVar6,fVar30);
        }
        else {
          uVar22 = *puVar11;
          uVar26 = *puVar6;
        }
        if (in_FPSCR_PR == '\0') {
          uVar26 = (ulonglong)
                   (uint)((float)((ulonglong)uVar22 >> 0x20) * (float)(uVar26 >> 0x20) +
                         (float)uVar26);
        }
        if (in_FPSCR_SZ == '\0') {
          uVar22 = CONCAT44(*(undefined4 *)(puVar11 + 1),(int)uVar22);
          uVar26 = CONCAT44((int)puVar6[4],(int)uVar26);
        }
        else {
          uVar22 = puVar11[1];
          uVar26 = puVar6[4];
        }
        if (in_FPSCR_PR == '\0') {
          uVar26 = (ulonglong)
                   (uint)((float)((ulonglong)uVar22 >> 0x20) * (float)(uVar26 >> 0x20) +
                         (float)uVar26);
        }
        dVar28 = dVar32;
        in_dr0 = dVar32;
        if (in_FPSCR_SZ == '\0') {
          dVar28 = (double)CONCAT44(piVar31,(int)uVar26);
          in_dr0 = (double)CONCAT44(piVar31,(int)uVar22);
        }
        if (in_FPSCR_PR == '\0') {
          dVar28 = (double)CONCAT44((float)((ulonglong)in_dr0 >> 0x20) * SUB84(dVar28,0) +
                                    (float)((ulonglong)dVar28 >> 0x20),SUB84(dVar28,0));
        }
        if (in_FPSCR_SZ == '\0') {
          *(int *)((int)pdVar13 + -4) = (int)((ulonglong)dVar28 >> 0x20);
          *(undefined4 *)(pdVar13 + -1) = *(undefined4 *)(puVar11 + 3);
          *(undefined4 *)((int)pdVar13 + -0xc) = *(undefined4 *)((int)puVar11 + 0x14);
          pdVar13 = pdVar13 + -2;
          *(undefined4 *)pdVar13 = *(undefined4 *)(puVar11 + 2);
        }
        else {
          pdVar13[-1] = dVar28;
          pdVar13[-2] = (double)puVar11[3];
          pdVar13[-3] = *(double *)((int)puVar11 + 0x14);
          pdVar13 = pdVar13 + -4;
          *pdVar13 = (double)puVar11[2];
        }
        *(undefined4 *)((int)pdVar13 + -4) = uVar29;
        unaff_r8 = (undefined4 *)((int)pdVar13 + 0x1c);
        *DAT_8c61f360 = *DAT_8c61f360 + 3;
      }
    }
  }
  else if (uVar5 == (int)DAT_8c61f16a) {
    *DAT_8c61f174 = *DAT_8c61f174 + (uint)param_3[1] * 4;
    for (iVar14 = 0; uVar23 = SUB84(in_dr2,0), iVar14 < (int)(uint)param_3[1]; iVar14 = iVar14 + 1)
    {
      uVar1 = *puVar16;
      puVar19 = puVar16 + 1;
      puVar17 = puVar16 + 2;
      iVar15 = *DAT_8c61f604;
      puVar18 = puVar16 + 3;
      puVar16 = puVar16 + 4;
      puVar8 = (undefined8 *)((uint)*puVar19 * 0x20 + iVar15);
      puVar10 = (undefined8 *)((uint)uVar1 * 0x20 + iVar15);
      if (in_FPSCR_SZ == '\0') {
        dVar28 = (double)CONCAT44(*(undefined4 *)(puVar8 + 2),SUB84(param_1,0));
        uVar23 = *(undefined4 *)(puVar10 + 2);
      }
      else {
        dVar28 = (double)puVar8[2];
      }
      uVar29 = (undefined4)((ulonglong)dVar28 >> 0x20);
      dVar25 = dVar28;
      if (in_FPSCR_SZ == '\0') {
        dVar25 = (double)CONCAT44(uVar29,uVar23);
      }
      if (in_FPSCR_PR == '\0') {
        dVar25 = (double)CONCAT44((float)((ulonglong)dVar25 >> 0x20) - SUB84(dVar25,0),
                                  SUB84(dVar25,0));
      }
      else {
        dVar25 = dVar25 - dVar25;
      }
      if (in_FPSCR_SZ == '\0') {
        dVar28 = (double)CONCAT44(uVar29,*(undefined4 *)((int)puVar8 + 0x14));
      }
      puVar11 = (undefined8 *)((uint)*puVar17 * 0x20 + iVar15);
      fVar30 = SUB84(dVar28,0);
      if (in_FPSCR_SZ == '\0') {
        dVar25 = (double)CONCAT44((int)((ulonglong)dVar25 >> 0x20),
                                  *(undefined4 *)((int)puVar11 + 0x14));
        in_dr0 = (double)CONCAT44((int)((ulonglong)in_dr0 >> 0x20),fVar30);
      }
      if (in_FPSCR_PR == '\0') {
        fVar24 = SUB84(dVar25,0) - fVar30;
        dVar25 = (double)CONCAT44((float)((ulonglong)dVar25 >> 0x20) * fVar24,fVar24);
      }
      else {
        dVar25 = (dVar25 - dVar28) * (dVar25 - dVar28);
      }
      if (in_FPSCR_SZ == '\0') {
        dVar25 = (double)CONCAT44((int)((ulonglong)dVar25 >> 0x20),
                                  *(undefined4 *)((int)puVar10 + 0x14));
      }
      if (in_FPSCR_PR == '\0') {
        dVar21 = (double)CONCAT44((int)((ulonglong)in_dr0 >> 0x20),SUB84(in_dr0,0) - SUB84(dVar25,0)
                                 );
      }
      else {
        dVar21 = in_dr0 - dVar25;
      }
      if (in_FPSCR_SZ == '\0') {
        dVar25 = (double)CONCAT44((int)((ulonglong)dVar25 >> 0x20),*(undefined4 *)(puVar11 + 2));
      }
      if (in_FPSCR_PR == '\0') {
        dVar25 = (double)CONCAT44((int)((ulonglong)dVar25 >> 0x20),
                                  SUB84(dVar25,0) - (float)((ulonglong)dVar28 >> 0x20));
      }
      else {
        dVar25 = dVar25 - dVar28;
      }
      uVar5 = (uint)((ulonglong)dVar25 >> 0x20);
      param_1 = dVar25;
      if (in_FPSCR_SZ == '\0') {
        param_1 = (double)CONCAT44(uVar5,fVar30);
      }
      if (in_FPSCR_PR == '\0') {
        in_dr0 = (double)CONCAT44((int)((ulonglong)dVar21 >> 0x20),SUB84(dVar21,0) * SUB84(dVar25,0)
                                 );
      }
      else {
        in_dr0 = dVar21 * dVar25;
      }
      in_dr2 = (double)((ulonglong)uVar5 << 0x20);
      if (in_FPSCR_PR == '\0') {
        fVar30 = (float)((ulonglong)param_1 >> 0x20) - SUB84(in_dr0,0);
        param_1 = (double)CONCAT44(fVar30,SUB84(param_1,0));
        bVar33 = 0.0 < fVar30;
      }
      else {
        param_1 = param_1 - in_dr0;
        bVar33 = in_dr2 < param_1;
      }
      puVar20 = (undefined8 *)((uint)*puVar18 * 0x20 + iVar15);
      if (!bVar33) {
        if (in_FPSCR_SZ == '\0') {
          pdVar13 = (double *)(unaff_r8 + 6);
          *(undefined4 *)pdVar13 = 0x3f800000;
        }
        else {
          pdVar13 = (double *)(unaff_r8 + 5);
          *pdVar13 = in_xd6;
        }
        iVar15 = *piVar2;
        if (in_FPSCR_SZ == '\0') {
          dVar28 = (double)CONCAT44(*(undefined4 *)((int)puVar10 + 4),SUB84(in_dr0,0));
          in_dr2 = (double)CONCAT44(uVar5,*(undefined4 *)(iVar15 + 0x14));
        }
        else {
          dVar28 = *(double *)((int)puVar10 + 4);
        }
        if (in_FPSCR_PR == '\0') {
          fVar30 = SUB84(in_dr2,0) * (float)((ulonglong)dVar28 >> 0x20);
        }
        else {
          fVar30 = SUB84(in_dr2 * dVar28,0);
        }
        if (in_FPSCR_SZ == '\0') {
          uVar26 = CONCAT44(*(undefined4 *)(iVar15 + 4),fVar30);
          uVar22 = CONCAT44(*(undefined4 *)puVar10,SUB84(dVar28,0));
        }
        else {
          uVar26 = *(ulonglong *)(iVar15 + 4);
          uVar22 = *puVar10;
        }
        if (in_FPSCR_PR == '\0') {
          uVar26 = (ulonglong)
                   (uint)((float)((ulonglong)uVar22 >> 0x20) * (float)(uVar26 >> 0x20) +
                         (float)uVar26);
        }
        if (in_FPSCR_SZ == '\0') {
          uVar22 = CONCAT44(*(undefined4 *)(puVar10 + 1),(int)uVar22);
          uVar26 = CONCAT44(*(undefined4 *)(iVar15 + 0x24),(int)uVar26);
        }
        else {
          uVar22 = puVar10[1];
          uVar26 = *(ulonglong *)(iVar15 + 0x24);
        }
        if (in_FPSCR_PR == '\0') {
          uVar26 = (ulonglong)
                   (uint)((float)((ulonglong)uVar22 >> 0x20) * (float)(uVar26 >> 0x20) +
                         (float)uVar26);
        }
        dVar25 = dVar32;
        dVar28 = dVar32;
        if (in_FPSCR_SZ == '\0') {
          dVar25 = (double)CONCAT44(piVar31,(int)uVar22);
          dVar28 = (double)CONCAT44(piVar31,(int)uVar26);
        }
        if (in_FPSCR_PR == '\0') {
          dVar28 = (double)CONCAT44((float)((ulonglong)dVar25 >> 0x20) * SUB84(dVar28,0) +
                                    (float)((ulonglong)dVar28 >> 0x20),SUB84(dVar28,0));
        }
        uVar23 = (undefined4)((ulonglong)dVar28 >> 0x20);
        if (in_FPSCR_SZ == '\0') {
          pdVar13 = (double *)((int)pdVar13 + -4);
          *(undefined4 *)pdVar13 = uVar23;
        }
        else {
          pdVar13 = pdVar13 + -1;
          *pdVar13 = dVar28;
        }
        uVar29 = DAT_8c61f608;
        puVar6 = (ulonglong *)*piVar2;
        if (in_FPSCR_SZ == '\0') {
          dVar25 = (double)CONCAT44(*(undefined4 *)((int)puVar10 + 4),SUB84(dVar25,0));
          dVar28 = (double)CONCAT44(uVar23,(int)puVar6[2]);
        }
        else {
          dVar25 = *(double *)((int)puVar10 + 4);
        }
        if (in_FPSCR_PR == '\0') {
          fVar30 = SUB84(dVar28,0) * (float)((ulonglong)dVar25 >> 0x20);
        }
        else {
          fVar30 = SUB84(dVar28 * dVar25,0);
        }
        if (in_FPSCR_SZ == '\0') {
          uVar22 = CONCAT44(*(undefined4 *)puVar10,SUB84(dVar25,0));
          uVar26 = CONCAT44((int)*puVar6,fVar30);
        }
        else {
          uVar22 = *puVar10;
          uVar26 = *puVar6;
        }
        if (in_FPSCR_PR == '\0') {
          uVar26 = (ulonglong)
                   (uint)((float)((ulonglong)uVar22 >> 0x20) * (float)(uVar26 >> 0x20) +
                         (float)uVar26);
        }
        if (in_FPSCR_SZ == '\0') {
          uVar22 = CONCAT44(*(undefined4 *)(puVar10 + 1),(int)uVar22);
          uVar26 = CONCAT44((int)puVar6[4],(int)uVar26);
        }
        else {
          uVar22 = puVar10[1];
          uVar26 = puVar6[4];
        }
        if (in_FPSCR_PR == '\0') {
          uVar26 = (ulonglong)
                   (uint)((float)((ulonglong)uVar22 >> 0x20) * (float)(uVar26 >> 0x20) +
                         (float)uVar26);
        }
        dVar25 = dVar32;
        dVar28 = dVar32;
        if (in_FPSCR_SZ == '\0') {
          dVar25 = (double)CONCAT44(piVar31,(int)uVar26);
          dVar28 = (double)CONCAT44(piVar31,(int)uVar22);
        }
        if (in_FPSCR_PR == '\0') {
          dVar25 = (double)CONCAT44((float)((ulonglong)dVar28 >> 0x20) * SUB84(dVar25,0) +
                                    (float)((ulonglong)dVar25 >> 0x20),SUB84(dVar25,0));
        }
        uVar23 = (undefined4)((ulonglong)dVar25 >> 0x20);
        if (in_FPSCR_SZ == '\0') {
          *(undefined4 *)((int)pdVar13 + -4) = uVar23;
          *(undefined4 *)(pdVar13 + -1) = *(undefined4 *)(puVar10 + 3);
          *(undefined4 *)((int)pdVar13 + -0xc) = *(undefined4 *)((int)puVar10 + 0x14);
          dVar25 = (double)CONCAT44(uVar23,*(undefined4 *)(puVar10 + 2));
          pdVar13 = pdVar13 + -2;
          *(undefined4 *)pdVar13 = *(undefined4 *)(puVar10 + 2);
        }
        else {
          pdVar13[-1] = dVar25;
          pdVar13[-2] = (double)puVar10[3];
          pdVar13[-3] = *(double *)((int)puVar10 + 0x14);
          pdVar13 = pdVar13 + -4;
          *pdVar13 = (double)puVar10[2];
        }
        *(undefined4 *)((int)pdVar13 + -4) = uVar29;
        if (in_FPSCR_SZ == '\0') {
          pdVar13 = (double *)((int)pdVar13 + 0x34);
          *(undefined4 *)pdVar13 = 0x3f800000;
        }
        else {
          pdVar13 = pdVar13 + 6;
          *pdVar13 = in_xd6;
        }
        iVar15 = *piVar2;
        if (in_FPSCR_SZ == '\0') {
          dVar28 = (double)CONCAT44(*(undefined4 *)((int)puVar8 + 4),SUB84(dVar28,0));
          dVar25 = (double)CONCAT44((int)((ulonglong)dVar25 >> 0x20),*(undefined4 *)(iVar15 + 0x14))
          ;
        }
        else {
          dVar28 = *(double *)((int)puVar8 + 4);
        }
        if (in_FPSCR_PR == '\0') {
          fVar30 = SUB84(dVar25,0) * (float)((ulonglong)dVar28 >> 0x20);
        }
        else {
          fVar30 = SUB84(dVar25 * dVar28,0);
        }
        if (in_FPSCR_SZ == '\0') {
          uVar26 = CONCAT44(*(undefined4 *)(iVar15 + 4),fVar30);
          uVar22 = CONCAT44(*(undefined4 *)puVar8,SUB84(dVar28,0));
        }
        else {
          uVar26 = *(ulonglong *)(iVar15 + 4);
          uVar22 = *puVar8;
        }
        if (in_FPSCR_PR == '\0') {
          uVar26 = (ulonglong)
                   (uint)((float)((ulonglong)uVar22 >> 0x20) * (float)(uVar26 >> 0x20) +
                         (float)uVar26);
        }
        if (in_FPSCR_SZ == '\0') {
          uVar22 = CONCAT44(*(undefined4 *)(puVar8 + 1),(int)uVar22);
          uVar26 = CONCAT44(*(undefined4 *)(iVar15 + 0x24),(int)uVar26);
        }
        else {
          uVar22 = puVar8[1];
          uVar26 = *(ulonglong *)(iVar15 + 0x24);
        }
        if (in_FPSCR_PR == '\0') {
          uVar26 = (ulonglong)
                   (uint)((float)((ulonglong)uVar22 >> 0x20) * (float)(uVar26 >> 0x20) +
                         (float)uVar26);
        }
        dVar25 = dVar32;
        dVar28 = dVar32;
        if (in_FPSCR_SZ == '\0') {
          dVar25 = (double)CONCAT44(piVar31,(int)uVar22);
          dVar28 = (double)CONCAT44(piVar31,(int)uVar26);
        }
        if (in_FPSCR_PR == '\0') {
          dVar28 = (double)CONCAT44((float)((ulonglong)dVar25 >> 0x20) * SUB84(dVar28,0) +
                                    (float)((ulonglong)dVar28 >> 0x20),SUB84(dVar28,0));
        }
        uVar23 = (undefined4)((ulonglong)dVar28 >> 0x20);
        if (in_FPSCR_SZ == '\0') {
          pdVar13 = (double *)((int)pdVar13 + -4);
          *(undefined4 *)pdVar13 = uVar23;
        }
        else {
          pdVar13 = pdVar13 + -1;
          *pdVar13 = dVar28;
        }
        uVar29 = DAT_8c61f608;
        puVar6 = (ulonglong *)*piVar2;
        if (in_FPSCR_SZ == '\0') {
          dVar25 = (double)CONCAT44(*(undefined4 *)((int)puVar8 + 4),SUB84(dVar25,0));
          dVar28 = (double)CONCAT44(uVar23,(int)puVar6[2]);
        }
        else {
          dVar25 = *(double *)((int)puVar8 + 4);
        }
        if (in_FPSCR_PR == '\0') {
          fVar30 = SUB84(dVar28,0) * (float)((ulonglong)dVar25 >> 0x20);
        }
        else {
          fVar30 = SUB84(dVar28 * dVar25,0);
        }
        if (in_FPSCR_SZ == '\0') {
          uVar22 = CONCAT44(*(undefined4 *)puVar8,SUB84(dVar25,0));
          uVar26 = CONCAT44((int)*puVar6,fVar30);
        }
        else {
          uVar22 = *puVar8;
          uVar26 = *puVar6;
        }
        if (in_FPSCR_PR == '\0') {
          uVar26 = (ulonglong)
                   (uint)((float)((ulonglong)uVar22 >> 0x20) * (float)(uVar26 >> 0x20) +
                         (float)uVar26);
        }
        if (in_FPSCR_SZ == '\0') {
          uVar22 = CONCAT44(*(undefined4 *)(puVar8 + 1),(int)uVar22);
          uVar26 = CONCAT44((int)puVar6[4],(int)uVar26);
        }
        else {
          uVar22 = puVar8[1];
          uVar26 = puVar6[4];
        }
        if (in_FPSCR_PR == '\0') {
          uVar26 = (ulonglong)
                   (uint)((float)((ulonglong)uVar22 >> 0x20) * (float)(uVar26 >> 0x20) +
                         (float)uVar26);
        }
        dVar25 = dVar32;
        dVar28 = dVar32;
        if (in_FPSCR_SZ == '\0') {
          dVar25 = (double)CONCAT44(piVar31,(int)uVar26);
          dVar28 = (double)CONCAT44(piVar31,(int)uVar22);
        }
        if (in_FPSCR_PR == '\0') {
          dVar25 = (double)CONCAT44((float)((ulonglong)dVar28 >> 0x20) * SUB84(dVar25,0) +
                                    (float)((ulonglong)dVar25 >> 0x20),SUB84(dVar25,0));
        }
        uVar23 = (undefined4)((ulonglong)dVar25 >> 0x20);
        if (in_FPSCR_SZ == '\0') {
          *(undefined4 *)((int)pdVar13 + -4) = uVar23;
          *(undefined4 *)(pdVar13 + -1) = *(undefined4 *)(puVar8 + 3);
          *(undefined4 *)((int)pdVar13 + -0xc) = *(undefined4 *)((int)puVar8 + 0x14);
          dVar25 = (double)CONCAT44(uVar23,*(undefined4 *)(puVar8 + 2));
          pdVar13 = pdVar13 + -2;
          *(undefined4 *)pdVar13 = *(undefined4 *)(puVar8 + 2);
        }
        else {
          pdVar13[-1] = dVar25;
          pdVar13[-2] = (double)puVar8[3];
          pdVar13[-3] = *(double *)((int)puVar8 + 0x14);
          pdVar13 = pdVar13 + -4;
          *pdVar13 = (double)puVar8[2];
        }
        *(undefined4 *)((int)pdVar13 + -4) = uVar29;
        if (in_FPSCR_SZ == '\0') {
          pdVar13 = (double *)((int)pdVar13 + 0x34);
          *(undefined4 *)pdVar13 = 0x3f800000;
        }
        else {
          pdVar13 = pdVar13 + 6;
          *pdVar13 = in_xd6;
        }
        iVar15 = *piVar2;
        if (in_FPSCR_SZ == '\0') {
          dVar28 = (double)CONCAT44(*(undefined4 *)((int)puVar11 + 4),SUB84(dVar28,0));
          dVar25 = (double)CONCAT44((int)((ulonglong)dVar25 >> 0x20),*(undefined4 *)(iVar15 + 0x14))
          ;
        }
        else {
          dVar28 = *(double *)((int)puVar11 + 4);
        }
        if (in_FPSCR_PR == '\0') {
          fVar30 = SUB84(dVar25,0) * (float)((ulonglong)dVar28 >> 0x20);
        }
        else {
          fVar30 = SUB84(dVar25 * dVar28,0);
        }
        if (in_FPSCR_SZ == '\0') {
          uVar26 = CONCAT44(*(undefined4 *)(iVar15 + 4),fVar30);
          uVar22 = CONCAT44(*(undefined4 *)puVar11,SUB84(dVar28,0));
        }
        else {
          uVar26 = *(ulonglong *)(iVar15 + 4);
          uVar22 = *puVar11;
        }
        if (in_FPSCR_PR == '\0') {
          uVar26 = (ulonglong)
                   (uint)((float)((ulonglong)uVar22 >> 0x20) * (float)(uVar26 >> 0x20) +
                         (float)uVar26);
        }
        if (in_FPSCR_SZ == '\0') {
          uVar22 = CONCAT44(*(undefined4 *)(puVar11 + 1),(int)uVar22);
          uVar26 = CONCAT44(*(undefined4 *)(iVar15 + 0x24),(int)uVar26);
        }
        else {
          uVar22 = puVar11[1];
          uVar26 = *(ulonglong *)(iVar15 + 0x24);
        }
        if (in_FPSCR_PR == '\0') {
          uVar26 = (ulonglong)
                   (uint)((float)((ulonglong)uVar22 >> 0x20) * (float)(uVar26 >> 0x20) +
                         (float)uVar26);
        }
        dVar25 = dVar32;
        dVar28 = dVar32;
        if (in_FPSCR_SZ == '\0') {
          dVar25 = (double)CONCAT44(piVar31,(int)uVar22);
          dVar28 = (double)CONCAT44(piVar31,(int)uVar26);
        }
        if (in_FPSCR_PR == '\0') {
          dVar28 = (double)CONCAT44((float)((ulonglong)dVar25 >> 0x20) * SUB84(dVar28,0) +
                                    (float)((ulonglong)dVar28 >> 0x20),SUB84(dVar28,0));
        }
        uVar23 = (undefined4)((ulonglong)dVar28 >> 0x20);
        if (in_FPSCR_SZ == '\0') {
          pdVar13 = (double *)((int)pdVar13 + -4);
          *(undefined4 *)pdVar13 = uVar23;
        }
        else {
          pdVar13 = pdVar13 + -1;
          *pdVar13 = dVar28;
        }
        uVar29 = DAT_8c61f608;
        puVar6 = (ulonglong *)*piVar2;
        if (in_FPSCR_SZ == '\0') {
          dVar25 = (double)CONCAT44(*(undefined4 *)((int)puVar11 + 4),SUB84(dVar25,0));
          dVar28 = (double)CONCAT44(uVar23,(int)puVar6[2]);
        }
        else {
          dVar25 = *(double *)((int)puVar11 + 4);
        }
        if (in_FPSCR_PR == '\0') {
          fVar30 = SUB84(dVar28,0) * (float)((ulonglong)dVar25 >> 0x20);
        }
        else {
          fVar30 = SUB84(dVar28 * dVar25,0);
        }
        if (in_FPSCR_SZ == '\0') {
          uVar22 = CONCAT44(*(undefined4 *)puVar11,SUB84(dVar25,0));
          uVar26 = CONCAT44((int)*puVar6,fVar30);
        }
        else {
          uVar22 = *puVar11;
          uVar26 = *puVar6;
        }
        if (in_FPSCR_PR == '\0') {
          uVar26 = (ulonglong)
                   (uint)((float)((ulonglong)uVar22 >> 0x20) * (float)(uVar26 >> 0x20) +
                         (float)uVar26);
        }
        if (in_FPSCR_SZ == '\0') {
          uVar22 = CONCAT44(*(undefined4 *)(puVar11 + 1),(int)uVar22);
          uVar26 = CONCAT44((int)puVar6[4],(int)uVar26);
        }
        else {
          uVar22 = puVar11[1];
          uVar26 = puVar6[4];
        }
        if (in_FPSCR_PR == '\0') {
          uVar26 = (ulonglong)
                   (uint)((float)((ulonglong)uVar22 >> 0x20) * (float)(uVar26 >> 0x20) +
                         (float)uVar26);
        }
        dVar25 = dVar32;
        dVar28 = dVar32;
        if (in_FPSCR_SZ == '\0') {
          dVar25 = (double)CONCAT44(piVar31,(int)uVar26);
          dVar28 = (double)CONCAT44(piVar31,(int)uVar22);
        }
        if (in_FPSCR_PR == '\0') {
          dVar25 = (double)CONCAT44((float)((ulonglong)dVar28 >> 0x20) * SUB84(dVar25,0) +
                                    (float)((ulonglong)dVar25 >> 0x20),SUB84(dVar25,0));
        }
        uVar23 = (undefined4)((ulonglong)dVar25 >> 0x20);
        if (in_FPSCR_SZ == '\0') {
          *(undefined4 *)((int)pdVar13 + -4) = uVar23;
          *(undefined4 *)(pdVar13 + -1) = *(undefined4 *)(puVar11 + 3);
          *(undefined4 *)((int)pdVar13 + -0xc) = *(undefined4 *)((int)puVar11 + 0x14);
          dVar25 = (double)CONCAT44(uVar23,*(undefined4 *)(puVar11 + 2));
          pdVar13 = pdVar13 + -2;
          *(undefined4 *)pdVar13 = *(undefined4 *)(puVar11 + 2);
        }
        else {
          pdVar13[-1] = dVar25;
          pdVar13[-2] = (double)puVar11[3];
          pdVar13[-3] = *(double *)((int)puVar11 + 0x14);
          pdVar13 = pdVar13 + -4;
          *pdVar13 = (double)puVar11[2];
        }
        *(undefined4 *)((int)pdVar13 + -4) = uVar29;
        if (in_FPSCR_SZ == '\0') {
          pdVar13 = (double *)((int)pdVar13 + 0x34);
          *(undefined4 *)pdVar13 = 0x3f800000;
        }
        else {
          pdVar13 = pdVar13 + 6;
          *pdVar13 = in_xd6;
        }
        iVar15 = *piVar2;
        if (in_FPSCR_SZ == '\0') {
          dVar28 = (double)CONCAT44(*(undefined4 *)((int)puVar20 + 4),SUB84(dVar28,0));
          dVar25 = (double)CONCAT44((int)((ulonglong)dVar25 >> 0x20),*(undefined4 *)(iVar15 + 0x14))
          ;
        }
        else {
          dVar28 = *(double *)((int)puVar20 + 4);
        }
        if (in_FPSCR_PR == '\0') {
          fVar30 = SUB84(dVar25,0) * (float)((ulonglong)dVar28 >> 0x20);
        }
        else {
          fVar30 = SUB84(dVar25 * dVar28,0);
        }
        if (in_FPSCR_SZ == '\0') {
          uVar26 = CONCAT44(*(undefined4 *)(iVar15 + 4),fVar30);
          uVar22 = CONCAT44(*(undefined4 *)puVar20,SUB84(dVar28,0));
        }
        else {
          uVar26 = *(ulonglong *)(iVar15 + 4);
          uVar22 = *puVar20;
        }
        if (in_FPSCR_PR == '\0') {
          uVar26 = (ulonglong)
                   (uint)((float)((ulonglong)uVar22 >> 0x20) * (float)(uVar26 >> 0x20) +
                         (float)uVar26);
        }
        if (in_FPSCR_SZ == '\0') {
          uVar22 = CONCAT44(*(undefined4 *)(puVar20 + 1),(int)uVar22);
          uVar26 = CONCAT44(*(undefined4 *)(iVar15 + 0x24),(int)uVar26);
        }
        else {
          uVar22 = puVar20[1];
          uVar26 = *(ulonglong *)(iVar15 + 0x24);
        }
        if (in_FPSCR_PR == '\0') {
          uVar26 = (ulonglong)
                   (uint)((float)((ulonglong)uVar22 >> 0x20) * (float)(uVar26 >> 0x20) +
                         (float)uVar26);
        }
        dVar25 = dVar32;
        dVar28 = dVar32;
        if (in_FPSCR_SZ == '\0') {
          dVar25 = (double)CONCAT44(piVar31,(int)uVar22);
          dVar28 = (double)CONCAT44(piVar31,(int)uVar26);
        }
        if (in_FPSCR_PR == '\0') {
          dVar28 = (double)CONCAT44((float)((ulonglong)dVar25 >> 0x20) * SUB84(dVar28,0) +
                                    (float)((ulonglong)dVar28 >> 0x20),SUB84(dVar28,0));
        }
        uVar23 = (undefined4)((ulonglong)dVar28 >> 0x20);
        if (in_FPSCR_SZ == '\0') {
          pdVar13 = (double *)((int)pdVar13 + -4);
          *(undefined4 *)pdVar13 = uVar23;
        }
        else {
          pdVar13 = pdVar13 + -1;
          *pdVar13 = dVar28;
        }
        uVar29 = DAT_8c61f60c;
        puVar6 = (ulonglong *)*piVar2;
        if (in_FPSCR_SZ == '\0') {
          dVar25 = (double)CONCAT44(*(undefined4 *)((int)puVar20 + 4),SUB84(dVar25,0));
          dVar28 = (double)CONCAT44(uVar23,(int)puVar6[2]);
        }
        else {
          dVar25 = *(double *)((int)puVar20 + 4);
        }
        if (in_FPSCR_PR == '\0') {
          fVar30 = SUB84(dVar28,0) * (float)((ulonglong)dVar25 >> 0x20);
        }
        else {
          fVar30 = SUB84(dVar28 * dVar25,0);
        }
        if (in_FPSCR_SZ == '\0') {
          uVar22 = CONCAT44(*(undefined4 *)puVar20,SUB84(dVar25,0));
          uVar26 = CONCAT44((int)*puVar6,fVar30);
        }
        else {
          uVar22 = *puVar20;
          uVar26 = *puVar6;
        }
        if (in_FPSCR_PR == '\0') {
          uVar26 = (ulonglong)
                   (uint)((float)((ulonglong)uVar22 >> 0x20) * (float)(uVar26 >> 0x20) +
                         (float)uVar26);
        }
        if (in_FPSCR_SZ == '\0') {
          uVar22 = CONCAT44(*(undefined4 *)(puVar20 + 1),(int)uVar22);
          uVar26 = CONCAT44((int)puVar6[4],(int)uVar26);
        }
        else {
          uVar22 = puVar20[1];
          uVar26 = puVar6[4];
        }
        if (in_FPSCR_PR == '\0') {
          uVar26 = (ulonglong)
                   (uint)((float)((ulonglong)uVar22 >> 0x20) * (float)(uVar26 >> 0x20) +
                         (float)uVar26);
        }
        in_dr2 = dVar32;
        in_dr0 = dVar32;
        if (in_FPSCR_SZ == '\0') {
          in_dr2 = (double)CONCAT44(piVar31,(int)uVar26);
          in_dr0 = (double)CONCAT44(piVar31,(int)uVar22);
        }
        if (in_FPSCR_PR == '\0') {
          in_dr2 = (double)CONCAT44((float)((ulonglong)in_dr0 >> 0x20) * SUB84(in_dr2,0) +
                                    (float)((ulonglong)in_dr2 >> 0x20),SUB84(in_dr2,0));
        }
        if (in_FPSCR_SZ == '\0') {
          *(int *)((int)pdVar13 + -4) = (int)((ulonglong)in_dr2 >> 0x20);
          *(undefined4 *)(pdVar13 + -1) = *(undefined4 *)(puVar20 + 3);
          *(undefined4 *)((int)pdVar13 + -0xc) = *(undefined4 *)((int)puVar20 + 0x14);
          in_dr2 = (double)(ulonglong)*(uint *)(puVar20 + 2);
          pdVar13 = pdVar13 + -2;
          *(uint *)pdVar13 = *(uint *)(puVar20 + 2);
        }
        else {
          pdVar13[-1] = in_dr2;
          pdVar13[-2] = (double)puVar20[3];
          pdVar13[-3] = *(double *)((int)puVar20 + 0x14);
          pdVar13 = pdVar13 + -4;
          *pdVar13 = (double)puVar20[2];
        }
        *(undefined4 *)((int)pdVar13 + -4) = uVar29;
        unaff_r8 = (undefined4 *)((int)pdVar13 + 0x1c);
        *DAT_8c61f610 = *DAT_8c61f610 + 4;
      }
    }
  }
  else if (uVar5 == DAT_8c61f17c) {
    for (iVar14 = 0; piVar4 = DAT_8c61f604, uVar23 = SUB84(param_1,0),
        iVar14 < (int)(uint)param_3[1]; iVar14 = iVar14 + 1) {
      puVar19 = puVar16 + 1;
      uVar5 = (uint)*puVar16;
      *piVar3 = *piVar3 + uVar5;
      iVar15 = *piVar4;
      iVar9 = (uint)puVar16[2] * 0x20 + iVar15;
      if (in_FPSCR_SZ == '\0') {
        uVar23 = *(undefined4 *)(iVar9 + 0x10);
      }
      else {
        in_xd4 = *(double *)(iVar9 + 0x10);
      }
      iVar12 = (uint)*puVar19 * 0x20 + iVar15;
      dVar28 = in_xd4;
      if (in_FPSCR_SZ == '\0') {
        dVar28 = (double)CONCAT44(uVar23,*(undefined4 *)(iVar12 + 0x10));
      }
      if (in_FPSCR_PR == '\0') {
        dVar28 = (double)CONCAT44((float)((ulonglong)dVar28 >> 0x20) - SUB84(dVar28,0),
                                  SUB84(dVar28,0));
      }
      else {
        dVar28 = dVar28 - dVar28;
      }
      if (in_FPSCR_SZ == '\0') {
        dVar25 = (double)CONCAT44(*(undefined4 *)(iVar9 + 0x14),uVar23);
      }
      else {
        dVar25 = *(double *)(iVar9 + 0x14);
      }
      iVar15 = (uint)puVar16[3] * 0x20 + iVar15;
      fVar30 = (float)((ulonglong)dVar25 >> 0x20);
      if (in_FPSCR_SZ == '\0') {
        dVar28 = (double)CONCAT44((int)((ulonglong)dVar28 >> 0x20),*(undefined4 *)(iVar15 + 0x14));
        in_dr0 = (double)CONCAT44((int)((ulonglong)in_dr0 >> 0x20),fVar30);
      }
      if (in_FPSCR_PR == '\0') {
        fVar30 = SUB84(dVar28,0) - fVar30;
        dVar28 = (double)CONCAT44((float)((ulonglong)dVar28 >> 0x20) * fVar30,fVar30);
      }
      else {
        dVar28 = (dVar28 - dVar25) * (dVar28 - dVar25);
      }
      if (in_FPSCR_SZ == '\0') {
        dVar28 = (double)CONCAT44((int)((ulonglong)dVar28 >> 0x20),*(undefined4 *)(iVar12 + 0x14));
      }
      if (in_FPSCR_PR == '\0') {
        dVar21 = (double)CONCAT44((int)((ulonglong)in_dr0 >> 0x20),SUB84(in_dr0,0) - SUB84(dVar28,0)
                                 );
      }
      else {
        dVar21 = in_dr0 - dVar28;
      }
      if (in_FPSCR_SZ == '\0') {
        dVar28 = (double)CONCAT44((int)((ulonglong)dVar28 >> 0x20),*(undefined4 *)(iVar15 + 0x10));
      }
      if (in_FPSCR_PR == '\0') {
        dVar28 = (double)CONCAT44((int)((ulonglong)dVar28 >> 0x20),SUB84(dVar28,0) - SUB84(dVar25,0)
                                 );
      }
      else {
        dVar28 = dVar28 - dVar25;
      }
      uVar27 = (uint)((ulonglong)dVar28 >> 0x20);
      param_1 = dVar28;
      if (in_FPSCR_SZ == '\0') {
        param_1 = (double)CONCAT44(uVar27,SUB84(dVar25,0));
      }
      if (in_FPSCR_PR == '\0') {
        in_dr0 = (double)CONCAT44((int)((ulonglong)dVar21 >> 0x20),SUB84(dVar21,0) * SUB84(dVar28,0)
                                 );
      }
      else {
        in_dr0 = dVar21 * dVar28;
      }
      dVar28 = (double)((ulonglong)uVar27 << 0x20);
      if (in_FPSCR_PR == '\0') {
        fVar30 = (float)((ulonglong)param_1 >> 0x20) - SUB84(in_dr0,0);
        param_1 = (double)CONCAT44(fVar30,SUB84(param_1,0));
        bVar33 = 0.0 < fVar30;
      }
      else {
        param_1 = param_1 - in_dr0;
        bVar33 = dVar28 < param_1;
      }
      if (bVar33) {
        puVar16 = puVar19 + uVar5;
      }
      else {
        *_DAT_8c61f7b0 = *_DAT_8c61f7b0 + uVar5;
        iVar15 = 0;
        if (0 < (int)(uVar5 - 1)) {
          do {
            uVar1 = *puVar19;
            puVar19 = puVar19 + 1;
            iVar9 = *DAT_8c61f7b4;
            if (in_FPSCR_SZ == '\0') {
              pdVar13 = (double *)(unaff_r8 + 6);
              *(undefined4 *)pdVar13 = 0x3f800000;
            }
            else {
              pdVar13 = (double *)(unaff_r8 + 5);
              *pdVar13 = in_xd6;
            }
            puVar8 = (undefined8 *)((uint)uVar1 * 0x20 + iVar9);
            iVar9 = *piVar2;
            if (in_FPSCR_SZ == '\0') {
              dVar25 = (double)CONCAT44(*(undefined4 *)((int)puVar8 + 4),SUB84(in_dr0,0));
              dVar28 = (double)CONCAT44((int)((ulonglong)dVar28 >> 0x20),
                                        *(undefined4 *)(iVar9 + 0x14));
            }
            else {
              dVar25 = *(double *)((int)puVar8 + 4);
            }
            if (in_FPSCR_PR == '\0') {
              fVar30 = SUB84(dVar28,0) * (float)((ulonglong)dVar25 >> 0x20);
            }
            else {
              fVar30 = SUB84(dVar28 * dVar25,0);
            }
            if (in_FPSCR_SZ == '\0') {
              uVar22 = CONCAT44(*(undefined4 *)puVar8,SUB84(dVar25,0));
              uVar26 = CONCAT44(*(undefined4 *)(iVar9 + 4),fVar30);
            }
            else {
              uVar22 = *puVar8;
              uVar26 = *(ulonglong *)(iVar9 + 4);
            }
            if (in_FPSCR_PR == '\0') {
              uVar26 = (ulonglong)
                       (uint)((float)((ulonglong)uVar22 >> 0x20) * (float)(uVar26 >> 0x20) +
                             (float)uVar26);
            }
            if (in_FPSCR_SZ == '\0') {
              uVar22 = CONCAT44(*(undefined4 *)(puVar8 + 1),(int)uVar22);
              uVar26 = CONCAT44(*(undefined4 *)(iVar9 + 0x24),(int)uVar26);
            }
            else {
              uVar22 = puVar8[1];
              uVar26 = *(ulonglong *)(iVar9 + 0x24);
            }
            if (in_FPSCR_PR == '\0') {
              uVar26 = (ulonglong)
                       (uint)((float)((ulonglong)uVar22 >> 0x20) * (float)(uVar26 >> 0x20) +
                             (float)uVar26);
            }
            dVar25 = dVar32;
            dVar28 = dVar32;
            if (in_FPSCR_SZ == '\0') {
              dVar25 = (double)CONCAT44(piVar31,(int)uVar22);
              dVar28 = (double)CONCAT44(piVar31,(int)uVar26);
            }
            if (in_FPSCR_PR == '\0') {
              dVar28 = (double)CONCAT44((float)((ulonglong)dVar25 >> 0x20) * SUB84(dVar28,0) +
                                        (float)((ulonglong)dVar28 >> 0x20),SUB84(dVar28,0));
            }
            if (in_FPSCR_SZ == '\0') {
              pdVar13 = (double *)((int)pdVar13 + -4);
              *(int *)pdVar13 = (int)((ulonglong)dVar28 >> 0x20);
            }
            else {
              pdVar13 = pdVar13 + -1;
              *pdVar13 = dVar28;
            }
            uVar23 = DAT_8c61f7b8;
            pdVar7 = (double *)*piVar2;
            if (in_FPSCR_SZ == '\0') {
              dVar28 = (double)CONCAT44(*(undefined4 *)((int)puVar8 + 4),SUB84(dVar25,0));
              dVar25 = (double)CONCAT44(*(undefined4 *)pdVar7,*(undefined4 *)(pdVar7 + 2));
            }
            else {
              dVar28 = *(double *)((int)puVar8 + 4);
              dVar25 = *pdVar7;
            }
            if (in_FPSCR_PR == '\0') {
              dVar25 = (double)CONCAT44((int)((ulonglong)dVar25 >> 0x20),
                                        SUB84(dVar25,0) * (float)((ulonglong)dVar28 >> 0x20));
            }
            else {
              dVar25 = dVar25 * dVar28;
            }
            if (in_FPSCR_SZ == '\0') {
              uVar22 = CONCAT44(*(undefined4 *)puVar8,SUB84(dVar28,0));
            }
            else {
              uVar22 = *puVar8;
            }
            if (in_FPSCR_PR == '\0') {
              dVar25 = (double)(ulonglong)
                               (uint)((float)((ulonglong)uVar22 >> 0x20) *
                                      (float)((ulonglong)dVar25 >> 0x20) + SUB84(dVar25,0));
            }
            if (in_FPSCR_SZ == '\0') {
              uVar22 = CONCAT44(*(undefined4 *)(puVar8 + 1),(int)uVar22);
              dVar25 = (double)CONCAT44(*(undefined4 *)(pdVar7 + 4),SUB84(dVar25,0));
            }
            else {
              uVar22 = puVar8[1];
              dVar25 = pdVar7[4];
            }
            if (in_FPSCR_PR == '\0') {
              dVar25 = (double)(ulonglong)
                               (uint)((float)((ulonglong)uVar22 >> 0x20) *
                                      (float)((ulonglong)dVar25 >> 0x20) + SUB84(dVar25,0));
            }
            dVar28 = dVar32;
            in_dr0 = dVar32;
            if (in_FPSCR_SZ == '\0') {
              dVar28 = (double)CONCAT44(piVar31,SUB84(dVar25,0));
              in_dr0 = (double)CONCAT44(piVar31,(int)uVar22);
            }
            if (in_FPSCR_PR == '\0') {
              dVar28 = (double)CONCAT44((float)((ulonglong)in_dr0 >> 0x20) * SUB84(dVar28,0) +
                                        (float)((ulonglong)dVar28 >> 0x20),SUB84(dVar28,0));
            }
            uVar29 = (undefined4)((ulonglong)dVar28 >> 0x20);
            if (in_FPSCR_SZ == '\0') {
              *(undefined4 *)((int)pdVar13 + -4) = uVar29;
              *(undefined4 *)(pdVar13 + -1) = *(undefined4 *)(puVar8 + 3);
              *(undefined4 *)((int)pdVar13 + -0xc) = *(undefined4 *)((int)puVar8 + 0x14);
              dVar28 = (double)CONCAT44(uVar29,*(undefined4 *)(puVar8 + 2));
              pdVar13 = pdVar13 + -2;
              *(undefined4 *)pdVar13 = *(undefined4 *)(puVar8 + 2);
            }
            else {
              pdVar13[-1] = dVar28;
              pdVar13[-2] = (double)puVar8[3];
              pdVar13[-3] = *(double *)((int)puVar8 + 0x14);
              pdVar13 = pdVar13 + -4;
              *pdVar13 = (double)puVar8[2];
            }
            *(undefined4 *)((int)pdVar13 + -4) = uVar23;
            iVar15 = iVar15 + 1;
            unaff_r8 = (undefined4 *)((int)pdVar13 + 0x1c);
          } while (iVar15 < (int)(uVar5 - 1));
        }
        uVar1 = *puVar19;
        puVar16 = puVar19 + 1;
        iVar15 = *DAT_8c61f7b4;
        if (in_FPSCR_SZ == '\0') {
          pdVar13 = (double *)(unaff_r8 + 6);
          *(undefined4 *)pdVar13 = 0x3f800000;
        }
        else {
          pdVar13 = (double *)(unaff_r8 + 5);
          *pdVar13 = in_xd6;
        }
        puVar8 = (undefined8 *)((uint)uVar1 * 0x20 + iVar15);
        iVar15 = *piVar2;
        if (in_FPSCR_SZ == '\0') {
          dVar25 = (double)CONCAT44(*(undefined4 *)((int)puVar8 + 4),SUB84(in_dr0,0));
          dVar28 = (double)CONCAT44((int)((ulonglong)dVar28 >> 0x20),*(undefined4 *)(iVar15 + 0x14))
          ;
        }
        else {
          dVar25 = *(double *)((int)puVar8 + 4);
        }
        if (in_FPSCR_PR == '\0') {
          fVar30 = SUB84(dVar28,0) * (float)((ulonglong)dVar25 >> 0x20);
        }
        else {
          fVar30 = SUB84(dVar28 * dVar25,0);
        }
        if (in_FPSCR_SZ == '\0') {
          uVar22 = CONCAT44(*(undefined4 *)puVar8,SUB84(dVar25,0));
          uVar26 = CONCAT44(*(undefined4 *)(iVar15 + 4),fVar30);
        }
        else {
          uVar22 = *puVar8;
          uVar26 = *(ulonglong *)(iVar15 + 4);
        }
        if (in_FPSCR_PR == '\0') {
          uVar26 = (ulonglong)
                   (uint)((float)((ulonglong)uVar22 >> 0x20) * (float)(uVar26 >> 0x20) +
                         (float)uVar26);
        }
        if (in_FPSCR_SZ == '\0') {
          uVar22 = CONCAT44(*(undefined4 *)(puVar8 + 1),(int)uVar22);
          uVar26 = CONCAT44(*(undefined4 *)(iVar15 + 0x24),(int)uVar26);
        }
        else {
          uVar22 = puVar8[1];
          uVar26 = *(ulonglong *)(iVar15 + 0x24);
        }
        if (in_FPSCR_PR == '\0') {
          uVar26 = (ulonglong)
                   (uint)((float)((ulonglong)uVar22 >> 0x20) * (float)(uVar26 >> 0x20) +
                         (float)uVar26);
        }
        dVar25 = dVar32;
        dVar28 = dVar32;
        if (in_FPSCR_SZ == '\0') {
          dVar25 = (double)CONCAT44(piVar31,(int)uVar22);
          dVar28 = (double)CONCAT44(piVar31,(int)uVar26);
        }
        if (in_FPSCR_PR == '\0') {
          dVar28 = (double)CONCAT44((float)((ulonglong)dVar25 >> 0x20) * SUB84(dVar28,0) +
                                    (float)((ulonglong)dVar28 >> 0x20),SUB84(dVar28,0));
        }
        if (in_FPSCR_SZ == '\0') {
          pdVar13 = (double *)((int)pdVar13 + -4);
          *(int *)pdVar13 = (int)((ulonglong)dVar28 >> 0x20);
        }
        else {
          pdVar13 = pdVar13 + -1;
          *pdVar13 = dVar28;
        }
        uVar23 = DAT_8c61f7bc;
        pdVar7 = (double *)*piVar2;
        if (in_FPSCR_SZ == '\0') {
          dVar28 = (double)CONCAT44(*(undefined4 *)((int)puVar8 + 4),SUB84(dVar25,0));
          dVar25 = (double)CONCAT44(*(undefined4 *)pdVar7,*(undefined4 *)(pdVar7 + 2));
        }
        else {
          dVar28 = *(double *)((int)puVar8 + 4);
          dVar25 = *pdVar7;
        }
        if (in_FPSCR_PR == '\0') {
          dVar25 = (double)CONCAT44((int)((ulonglong)dVar25 >> 0x20),
                                    SUB84(dVar25,0) * (float)((ulonglong)dVar28 >> 0x20));
        }
        else {
          dVar25 = dVar25 * dVar28;
        }
        if (in_FPSCR_SZ == '\0') {
          uVar22 = CONCAT44(*(undefined4 *)puVar8,SUB84(dVar28,0));
        }
        else {
          uVar22 = *puVar8;
        }
        if (in_FPSCR_PR == '\0') {
          dVar25 = (double)(ulonglong)
                           (uint)((float)((ulonglong)uVar22 >> 0x20) *
                                  (float)((ulonglong)dVar25 >> 0x20) + SUB84(dVar25,0));
        }
        if (in_FPSCR_SZ == '\0') {
          uVar22 = CONCAT44(*(undefined4 *)(puVar8 + 1),(int)uVar22);
          dVar28 = (double)CONCAT44(*(undefined4 *)(pdVar7 + 4),SUB84(dVar25,0));
        }
        else {
          uVar22 = puVar8[1];
          dVar28 = pdVar7[4];
        }
        if (in_FPSCR_PR == '\0') {
          dVar28 = (double)(ulonglong)
                           (uint)((float)((ulonglong)uVar22 >> 0x20) *
                                  (float)((ulonglong)dVar28 >> 0x20) + SUB84(dVar28,0));
        }
        dVar25 = dVar32;
        in_dr0 = dVar32;
        if (in_FPSCR_SZ == '\0') {
          dVar25 = (double)CONCAT44(piVar31,SUB84(dVar28,0));
          in_dr0 = (double)CONCAT44(piVar31,(int)uVar22);
        }
        if (in_FPSCR_PR == '\0') {
          dVar25 = (double)CONCAT44((float)((ulonglong)in_dr0 >> 0x20) * SUB84(dVar25,0) +
                                    (float)((ulonglong)dVar25 >> 0x20),SUB84(dVar25,0));
        }
        if (in_FPSCR_SZ == '\0') {
          *(int *)((int)pdVar13 + -4) = (int)((ulonglong)dVar25 >> 0x20);
          *(undefined4 *)(pdVar13 + -1) = *(undefined4 *)(puVar8 + 3);
          *(undefined4 *)((int)pdVar13 + -0xc) = *(undefined4 *)((int)puVar8 + 0x14);
          pdVar13 = pdVar13 + -2;
          *(undefined4 *)pdVar13 = *(undefined4 *)(puVar8 + 2);
        }
        else {
          pdVar13[-1] = dVar25;
          pdVar13[-2] = (double)puVar8[3];
          pdVar13[-3] = *(double *)((int)puVar8 + 0x14);
          pdVar13 = pdVar13 + -4;
          *pdVar13 = (double)puVar8[2];
        }
        *(undefined4 *)((int)pdVar13 + -4) = uVar23;
        unaff_r8 = (undefined4 *)((int)pdVar13 + 0x1c);
      }
    }
  }
  else if (uVar5 == DAT_8c61f178) {
    *DAT_8c61f7c0 = param_3;
    piVar2 = _DAT_8c61f7b0;
    if (in_FPSCR_SZ == '\0') {
      piVar2 = DAT_8c61f7ac;
    }
    dVar32 = (double)CONCAT44(piVar2,DAT_8c61f7ac);
    if (in_FPSCR_PR == '\0') {
      dVar28 = (double)CONCAT44((float)((ulonglong)param_1 >> 0x20) * (float)piVar2,
                                SUB84(param_1,0) * (float)piVar2);
      dVar32 = (double)CONCAT44((float)((ulonglong)param_2 >> 0x20) * (float)piVar2,
                                SUB84(param_2,0) * (float)piVar2);
    }
    else {
      dVar28 = param_1 * dVar32 * dVar32;
      dVar32 = param_2 * dVar32 * dVar32;
    }
    (*(code *)PTR_FUN_8c61f7c4)(dVar28,dVar32,*DAT_8c61f7c0);
  }
  return;
}

