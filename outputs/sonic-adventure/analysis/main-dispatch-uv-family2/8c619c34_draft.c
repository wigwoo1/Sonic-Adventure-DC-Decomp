/* Ghidra draft only; not buildable/verified source. */

void FUN_8c619c34(double param_1,undefined4 param_2,double param_3,undefined4 param_4,
                 ushort *param_5)

{
  ushort uVar1;
  int *piVar2;
  int *piVar3;
  int *piVar4;
  uint uVar5;
  double *pdVar6;
  double *pdVar7;
  int iVar8;
  double *pdVar9;
  double *pdVar10;
  int iVar11;
  int unaff_r8;
  double *pdVar12;
  int iVar13;
  int iVar14;
  ushort *puVar15;
  ushort *puVar16;
  ushort *puVar17;
  ushort *puVar18;
  double *pdVar19;
  double in_dr0;
  double dVar20;
  undefined4 uVar21;
  float fVar22;
  double in_dr2;
  double dVar23;
  double dVar24;
  ulonglong uVar25;
  uint uVar26;
  double dVar27;
  undefined4 uVar28;
  float fVar29;
  undefined4 uVar31;
  double dVar30;
  double in_xd2;
  double in_xd4;
  bool bVar32;
  char in_FPSCR_PR;
  char in_FPSCR_SZ;
  
  piVar3 = DAT_8c619c9c;
  piVar2 = DAT_8c619c94._4_4_;
  if (in_FPSCR_SZ == '\0') {
    dVar30 = (double)CONCAT44((undefined4)DAT_8c619c94,param_2);
  }
  else {
    dVar30 = (double)CONCAT44(DAT_8c619c94._4_4_,(undefined4)DAT_8c619c94);
  }
  puVar15 = *(ushort **)(param_5 + 2);
  uVar5 = *param_5 & DAT_8c619ca0;
  uVar31 = (undefined4)((ulonglong)dVar30 >> 0x20);
  if (uVar5 == 0) {
    *DAT_8c619c9c = *DAT_8c619c9c + (uint)param_5[1] * 3;
    iVar13 = 0;
    while (iVar13 < (int)(uint)param_5[1]) {
      uVar1 = *puVar15;
      puVar18 = puVar15 + 1;
      puVar16 = puVar15 + 2;
      iVar14 = *DAT_8c619ee8;
      puVar15 = puVar15 + 3;
      pdVar7 = (double *)((uint)*puVar18 * 0x20 + iVar14);
      if (in_FPSCR_SZ == '\0') {
        dVar27 = (double)CONCAT44(*(undefined4 *)(pdVar7 + 2),SUB84(param_1,0));
      }
      else {
        dVar27 = pdVar7[2];
      }
      pdVar9 = (double *)((uint)uVar1 * 0x20 + iVar14);
      uVar21 = (undefined4)((ulonglong)dVar27 >> 0x20);
      if (in_FPSCR_SZ == '\0') {
        dVar24 = (double)CONCAT44(uVar21,*(undefined4 *)(pdVar9 + 2));
      }
      else {
        in_xd2 = pdVar9[2];
        dVar24 = dVar27;
      }
      if (in_FPSCR_PR == '\0') {
        dVar24 = (double)CONCAT44((float)((ulonglong)dVar24 >> 0x20) - SUB84(dVar24,0),
                                  SUB84(dVar24,0));
      }
      else {
        dVar24 = dVar24 - dVar24;
      }
      pdVar10 = (double *)((uint)*puVar16 * 0x20 + iVar14);
      if (in_FPSCR_SZ == '\0') {
        fVar29 = *(float *)((int)pdVar7 + 0x14);
        dVar27 = (double)CONCAT44(uVar21,fVar29);
        dVar24 = (double)CONCAT44((int)((ulonglong)dVar24 >> 0x20),
                                  *(undefined4 *)((int)pdVar10 + 0x14));
        in_dr0 = (double)CONCAT44((int)((ulonglong)in_dr0 >> 0x20),fVar29);
      }
      else {
        in_xd2 = *(double *)((int)pdVar10 + 0x14);
        fVar29 = SUB84(dVar27,0);
      }
      if (in_FPSCR_PR == '\0') {
        fVar22 = SUB84(dVar24,0) - fVar29;
        dVar24 = (double)CONCAT44((float)((ulonglong)dVar24 >> 0x20) * fVar22,fVar22);
      }
      else {
        dVar24 = (dVar24 - dVar27) * (dVar24 - dVar27);
      }
      if (in_FPSCR_SZ == '\0') {
        dVar24 = (double)CONCAT44((int)((ulonglong)dVar24 >> 0x20),
                                  *(undefined4 *)((int)pdVar9 + 0x14));
      }
      else {
        in_xd2 = *(double *)((int)pdVar9 + 0x14);
      }
      if (in_FPSCR_PR == '\0') {
        dVar20 = (double)CONCAT44((int)((ulonglong)in_dr0 >> 0x20),SUB84(in_dr0,0) - SUB84(dVar24,0)
                                 );
      }
      else {
        dVar20 = in_dr0 - dVar24;
      }
      if (in_FPSCR_SZ == '\0') {
        dVar24 = (double)CONCAT44((int)((ulonglong)dVar24 >> 0x20),*(undefined4 *)(pdVar10 + 2));
      }
      else {
        in_xd2 = pdVar10[2];
      }
      if (in_FPSCR_PR == '\0') {
        dVar24 = (double)CONCAT44((int)((ulonglong)dVar24 >> 0x20),
                                  SUB84(dVar24,0) - (float)((ulonglong)dVar27 >> 0x20));
      }
      else {
        dVar24 = dVar24 - dVar27;
      }
      uVar5 = (uint)((ulonglong)dVar24 >> 0x20);
      param_1 = dVar24;
      if (in_FPSCR_SZ == '\0') {
        param_1 = (double)CONCAT44(uVar5,fVar29);
      }
      if (in_FPSCR_PR == '\0') {
        in_dr0 = (double)CONCAT44((int)((ulonglong)dVar20 >> 0x20),SUB84(dVar20,0) * SUB84(dVar24,0)
                                 );
      }
      else {
        in_dr0 = dVar20 * dVar24;
      }
      if (in_FPSCR_PR == '\0') {
        fVar29 = (float)((ulonglong)param_1 >> 0x20) - SUB84(in_dr0,0);
        param_1 = (double)CONCAT44(fVar29,SUB84(param_1,0));
        bVar32 = 0.0 < fVar29;
      }
      else {
        param_1 = param_1 - in_dr0;
        bVar32 = (double)((ulonglong)uVar5 << 0x20) < param_1;
      }
      iVar13 = iVar13 + 1;
      if (!bVar32) {
        if (in_FPSCR_SZ == '\0') {
          dVar27 = (double)CONCAT44(*(undefined4 *)DAT_8c619eec,*(undefined4 *)pdVar9);
        }
        else {
          in_xd2 = *pdVar9;
          dVar27 = *DAT_8c619eec;
        }
        if (in_FPSCR_PR == '\0') {
          dVar27 = (double)CONCAT44((float)((ulonglong)dVar27 >> 0x20) * SUB84(dVar27,0),
                                    SUB84(dVar27,0));
        }
        else {
          dVar27 = dVar27 * dVar27;
        }
        if (in_FPSCR_SZ == '\0') {
          dVar27 = (double)CONCAT44((int)((ulonglong)dVar27 >> 0x20),*(undefined4 *)DAT_8c619ef0);
          in_dr0 = (double)CONCAT44((int)((ulonglong)in_dr0 >> 0x20),*DAT_8c619ef4);
        }
        else {
          in_xd2 = *DAT_8c619ef0;
        }
        if (in_FPSCR_PR == '\0') {
          fVar29 = SUB84(dVar27,0) - (float)((ulonglong)dVar27 >> 0x20);
        }
        else {
          fVar29 = SUB84(dVar27 - dVar27,0);
        }
        if (in_FPSCR_SZ == '\0') {
          dVar27 = (double)CONCAT44(*(undefined4 *)((int)pdVar9 + 4),fVar29);
        }
        else {
          dVar27 = *(double *)((int)pdVar9 + 4);
        }
        if (in_FPSCR_PR == '\0') {
          dVar24 = (double)CONCAT44((int)((ulonglong)in_dr0 >> 0x20),
                                    SUB84(in_dr0,0) * (float)((ulonglong)dVar27 >> 0x20));
        }
        else {
          dVar24 = in_dr0 * dVar27;
        }
        if (in_FPSCR_SZ == '\0') {
          dVar27 = (double)CONCAT44(*(undefined4 *)(pdVar9 + 1),SUB84(dVar27,0));
        }
        else {
          dVar27 = pdVar9[1];
        }
        if (in_FPSCR_PR == '\0') {
          dVar27 = (double)CONCAT44((int)((ulonglong)dVar27 >> 0x20),
                                    SUB84(dVar27,0) - SUB84(dVar24,0));
        }
        else {
          dVar27 = dVar27 - dVar24;
        }
        if (in_FPSCR_SZ == '\0') {
          dVar24 = (double)CONCAT44((int)((ulonglong)dVar24 >> 0x20),
                                    *(undefined4 *)(DAT_8c619eec + 1));
        }
        fVar29 = (float)((ulonglong)dVar27 >> 0x20);
        if (in_FPSCR_PR == '\0') {
          fVar22 = SUB84(dVar24,0) * fVar29;
          dVar27 = (double)CONCAT44(fVar29,SUB84(dVar27,0) - fVar22);
        }
        else {
          fVar22 = SUB84(dVar24 * dVar27,0);
          dVar27 = dVar27 - dVar24 * dVar27;
        }
        if (in_FPSCR_SZ == '\0') {
          pdVar19 = (double *)(unaff_r8 + 0x18);
          *(int *)pdVar19 = SUB84(dVar27,0);
        }
        else {
          pdVar19 = (double *)(unaff_r8 + 0x14);
          *pdVar19 = in_xd2;
        }
        iVar14 = *piVar2;
        if (in_FPSCR_SZ == '\0') {
          dVar24 = (double)CONCAT44(*(undefined4 *)((int)pdVar9 + 4),fVar22);
          dVar27 = (double)CONCAT44((int)((ulonglong)dVar27 >> 0x20),*(undefined4 *)(iVar14 + 0x14))
          ;
        }
        else {
          dVar24 = *(double *)((int)pdVar9 + 4);
          in_xd2 = *(double *)(iVar14 + 0x14);
        }
        if (in_FPSCR_PR == '\0') {
          fVar29 = SUB84(dVar27,0) * (float)((ulonglong)dVar24 >> 0x20);
        }
        else {
          fVar29 = SUB84(dVar27 * dVar24,0);
        }
        if (in_FPSCR_SZ == '\0') {
          uVar25 = CONCAT44(*(undefined4 *)(iVar14 + 4),fVar29);
          dVar27 = (double)CONCAT44(*(undefined4 *)pdVar9,SUB84(dVar24,0));
        }
        else {
          uVar25 = *(ulonglong *)(iVar14 + 4);
          dVar27 = *pdVar9;
        }
        if (in_FPSCR_PR == '\0') {
          uVar25 = (ulonglong)
                   (uint)((float)((ulonglong)dVar27 >> 0x20) * (float)(uVar25 >> 0x20) +
                         (float)uVar25);
        }
        if (in_FPSCR_SZ == '\0') {
          dVar27 = (double)CONCAT44(*(undefined4 *)(pdVar9 + 1),SUB84(dVar27,0));
          uVar25 = CONCAT44(*(undefined4 *)(iVar14 + 0x24),(int)uVar25);
        }
        else {
          dVar27 = pdVar9[1];
          uVar25 = *(ulonglong *)(iVar14 + 0x24);
        }
        if (in_FPSCR_PR == '\0') {
          uVar25 = (ulonglong)
                   (uint)((float)((ulonglong)dVar27 >> 0x20) * (float)(uVar25 >> 0x20) +
                         (float)uVar25);
        }
        dVar20 = dVar30;
        dVar24 = dVar30;
        if (in_FPSCR_SZ == '\0') {
          dVar20 = (double)CONCAT44(uVar31,(int)uVar25);
          dVar24 = (double)CONCAT44(uVar31,SUB84(dVar27,0));
        }
        if (in_FPSCR_PR == '\0') {
          dVar20 = (double)CONCAT44((float)((ulonglong)dVar24 >> 0x20) * SUB84(dVar20,0) +
                                    (float)((ulonglong)dVar20 >> 0x20),SUB84(dVar20,0));
        }
        if (in_FPSCR_SZ == '\0') {
          pdVar19 = (double *)((int)pdVar19 + -4);
          *(int *)pdVar19 = (int)((ulonglong)dVar20 >> 0x20);
        }
        else {
          pdVar19 = pdVar19 + -1;
          *pdVar19 = dVar20;
        }
        pdVar12 = (double *)*piVar2;
        if (in_FPSCR_SZ == '\0') {
          dVar27 = (double)CONCAT44(*(undefined4 *)((int)pdVar9 + 4),SUB84(dVar24,0));
          dVar24 = (double)CONCAT44(*(undefined4 *)pdVar12,*(undefined4 *)(pdVar12 + 2));
        }
        else {
          dVar27 = *(double *)((int)pdVar9 + 4);
          in_xd2 = pdVar12[2];
          dVar24 = *pdVar12;
        }
        if (in_FPSCR_PR == '\0') {
          dVar24 = (double)CONCAT44((int)((ulonglong)dVar24 >> 0x20),
                                    SUB84(dVar24,0) * (float)((ulonglong)dVar27 >> 0x20));
        }
        else {
          dVar24 = dVar24 * dVar27;
        }
        if (in_FPSCR_SZ == '\0') {
          dVar27 = (double)CONCAT44(*(undefined4 *)pdVar9,SUB84(dVar27,0));
        }
        else {
          dVar27 = *pdVar9;
        }
        if (in_FPSCR_PR == '\0') {
          dVar24 = (double)(ulonglong)
                           (uint)((float)((ulonglong)dVar27 >> 0x20) *
                                  (float)((ulonglong)dVar24 >> 0x20) + SUB84(dVar24,0));
        }
        if (in_FPSCR_SZ == '\0') {
          dVar27 = (double)CONCAT44(*(undefined4 *)(pdVar9 + 1),SUB84(dVar27,0));
          dVar24 = (double)CONCAT44(*(undefined4 *)(pdVar12 + 4),SUB84(dVar24,0));
        }
        else {
          dVar27 = pdVar9[1];
          dVar24 = pdVar12[4];
        }
        if (in_FPSCR_PR == '\0') {
          dVar24 = (double)(ulonglong)
                           (uint)((float)((ulonglong)dVar27 >> 0x20) *
                                  (float)((ulonglong)dVar24 >> 0x20) + SUB84(dVar24,0));
        }
        dVar23 = dVar30;
        dVar20 = dVar30;
        if (in_FPSCR_SZ == '\0') {
          dVar23 = (double)CONCAT44(uVar31,SUB84(dVar24,0));
          dVar20 = (double)CONCAT44(uVar31,SUB84(dVar27,0));
        }
        fVar29 = (float)((ulonglong)dVar20 >> 0x20);
        if (in_FPSCR_PR == '\0') {
          dVar23 = (double)CONCAT44(fVar29 * SUB84(dVar23,0) + (float)((ulonglong)dVar23 >> 0x20),
                                    SUB84(dVar23,0));
        }
        if (in_FPSCR_SZ == '\0') {
          *(int *)((int)pdVar19 + -4) = (int)((ulonglong)dVar23 >> 0x20);
          *(undefined4 *)(pdVar19 + -1) = *(undefined4 *)(pdVar9 + 3);
          uVar21 = DAT_8c619ef8;
          *(undefined4 *)((int)pdVar19 + -0xc) = *(undefined4 *)((int)pdVar9 + 0x14);
          pdVar19 = pdVar19 + -2;
          *(undefined4 *)pdVar19 = *(undefined4 *)(pdVar9 + 2);
        }
        else {
          pdVar19[-1] = dVar23;
          pdVar19[-2] = pdVar9[3];
          uVar21 = DAT_8c619ef8;
          pdVar19[-3] = *(double *)((int)pdVar9 + 0x14);
          in_xd2 = pdVar9[2];
          pdVar19 = pdVar19 + -4;
          *pdVar19 = in_xd2;
        }
        *(undefined4 *)((int)pdVar19 + -4) = uVar21;
        if (in_FPSCR_SZ == '\0') {
          dVar27 = (double)CONCAT44(*(undefined4 *)DAT_8c619eec,*(undefined4 *)pdVar7);
        }
        else {
          in_xd2 = *pdVar7;
          dVar27 = *DAT_8c619eec;
        }
        if (in_FPSCR_PR == '\0') {
          dVar27 = (double)CONCAT44((float)((ulonglong)dVar27 >> 0x20) * SUB84(dVar27,0),
                                    SUB84(dVar27,0));
        }
        else {
          dVar27 = dVar27 * dVar27;
        }
        if (in_FPSCR_SZ == '\0') {
          dVar27 = (double)CONCAT44((int)((ulonglong)dVar27 >> 0x20),*(undefined4 *)DAT_8c619ef0);
          dVar20 = (double)CONCAT44(fVar29,*DAT_8c619ef4);
        }
        else {
          in_xd2 = *DAT_8c619ef0;
        }
        if (in_FPSCR_PR == '\0') {
          fVar29 = SUB84(dVar27,0) - (float)((ulonglong)dVar27 >> 0x20);
        }
        else {
          fVar29 = SUB84(dVar27 - dVar27,0);
        }
        if (in_FPSCR_SZ == '\0') {
          dVar27 = (double)CONCAT44(*(undefined4 *)((int)pdVar7 + 4),fVar29);
        }
        else {
          dVar27 = *(double *)((int)pdVar7 + 4);
        }
        if (in_FPSCR_PR == '\0') {
          dVar20 = (double)CONCAT44((int)((ulonglong)dVar20 >> 0x20),
                                    SUB84(dVar20,0) * (float)((ulonglong)dVar27 >> 0x20));
        }
        else {
          dVar20 = dVar20 * dVar27;
        }
        if (in_FPSCR_SZ == '\0') {
          dVar27 = (double)CONCAT44(*(undefined4 *)(pdVar7 + 1),SUB84(dVar27,0));
        }
        else {
          dVar27 = pdVar7[1];
        }
        if (in_FPSCR_PR == '\0') {
          dVar27 = (double)CONCAT44((int)((ulonglong)dVar27 >> 0x20),
                                    SUB84(dVar27,0) - SUB84(dVar20,0));
        }
        else {
          dVar27 = dVar27 - dVar20;
        }
        if (in_FPSCR_SZ == '\0') {
          dVar20 = (double)CONCAT44((int)((ulonglong)dVar20 >> 0x20),
                                    *(undefined4 *)(DAT_8c619eec + 1));
        }
        fVar29 = (float)((ulonglong)dVar27 >> 0x20);
        if (in_FPSCR_PR == '\0') {
          fVar22 = SUB84(dVar20,0) * fVar29;
          dVar27 = (double)CONCAT44(fVar29,SUB84(dVar27,0) - fVar22);
        }
        else {
          fVar22 = SUB84(dVar20 * dVar27,0);
          dVar27 = dVar27 - dVar20 * dVar27;
        }
        if (in_FPSCR_SZ == '\0') {
          pdVar19 = (double *)((int)pdVar19 + 0x34);
          *(int *)pdVar19 = SUB84(dVar27,0);
        }
        else {
          pdVar19 = pdVar19 + 6;
          *pdVar19 = in_xd2;
        }
        iVar14 = *piVar2;
        if (in_FPSCR_SZ == '\0') {
          dVar24 = (double)CONCAT44(*(undefined4 *)((int)pdVar7 + 4),fVar22);
          dVar27 = (double)CONCAT44((int)((ulonglong)dVar27 >> 0x20),*(undefined4 *)(iVar14 + 0x14))
          ;
        }
        else {
          dVar24 = *(double *)((int)pdVar7 + 4);
          in_xd2 = *(double *)(iVar14 + 0x14);
        }
        if (in_FPSCR_PR == '\0') {
          fVar29 = SUB84(dVar27,0) * (float)((ulonglong)dVar24 >> 0x20);
        }
        else {
          fVar29 = SUB84(dVar27 * dVar24,0);
        }
        if (in_FPSCR_SZ == '\0') {
          uVar25 = CONCAT44(*(undefined4 *)(iVar14 + 4),fVar29);
          dVar27 = (double)CONCAT44(*(undefined4 *)pdVar7,SUB84(dVar24,0));
        }
        else {
          uVar25 = *(ulonglong *)(iVar14 + 4);
          dVar27 = *pdVar7;
        }
        if (in_FPSCR_PR == '\0') {
          uVar25 = (ulonglong)
                   (uint)((float)((ulonglong)dVar27 >> 0x20) * (float)(uVar25 >> 0x20) +
                         (float)uVar25);
        }
        if (in_FPSCR_SZ == '\0') {
          dVar27 = (double)CONCAT44(*(undefined4 *)(pdVar7 + 1),SUB84(dVar27,0));
          uVar25 = CONCAT44(*(undefined4 *)(iVar14 + 0x24),(int)uVar25);
        }
        else {
          dVar27 = pdVar7[1];
          uVar25 = *(ulonglong *)(iVar14 + 0x24);
        }
        if (in_FPSCR_PR == '\0') {
          uVar25 = (ulonglong)
                   (uint)((float)((ulonglong)dVar27 >> 0x20) * (float)(uVar25 >> 0x20) +
                         (float)uVar25);
        }
        dVar20 = dVar30;
        dVar24 = dVar30;
        if (in_FPSCR_SZ == '\0') {
          dVar20 = (double)CONCAT44(uVar31,(int)uVar25);
          dVar24 = (double)CONCAT44(uVar31,SUB84(dVar27,0));
        }
        if (in_FPSCR_PR == '\0') {
          dVar20 = (double)CONCAT44((float)((ulonglong)dVar24 >> 0x20) * SUB84(dVar20,0) +
                                    (float)((ulonglong)dVar20 >> 0x20),SUB84(dVar20,0));
        }
        if (in_FPSCR_SZ == '\0') {
          pdVar19 = (double *)((int)pdVar19 + -4);
          *(int *)pdVar19 = (int)((ulonglong)dVar20 >> 0x20);
        }
        else {
          pdVar19 = pdVar19 + -1;
          *pdVar19 = dVar20;
        }
        pdVar9 = (double *)*piVar2;
        if (in_FPSCR_SZ == '\0') {
          dVar27 = (double)CONCAT44(*(undefined4 *)((int)pdVar7 + 4),SUB84(dVar24,0));
          dVar24 = (double)CONCAT44(*(undefined4 *)pdVar9,*(undefined4 *)(pdVar9 + 2));
        }
        else {
          dVar27 = *(double *)((int)pdVar7 + 4);
          in_xd2 = pdVar9[2];
          dVar24 = *pdVar9;
        }
        if (in_FPSCR_PR == '\0') {
          dVar24 = (double)CONCAT44((int)((ulonglong)dVar24 >> 0x20),
                                    SUB84(dVar24,0) * (float)((ulonglong)dVar27 >> 0x20));
        }
        else {
          dVar24 = dVar24 * dVar27;
        }
        if (in_FPSCR_SZ == '\0') {
          dVar27 = (double)CONCAT44(*(undefined4 *)pdVar7,SUB84(dVar27,0));
        }
        else {
          dVar27 = *pdVar7;
        }
        if (in_FPSCR_PR == '\0') {
          dVar24 = (double)(ulonglong)
                           (uint)((float)((ulonglong)dVar27 >> 0x20) *
                                  (float)((ulonglong)dVar24 >> 0x20) + SUB84(dVar24,0));
        }
        if (in_FPSCR_SZ == '\0') {
          dVar27 = (double)CONCAT44(*(undefined4 *)(pdVar7 + 1),SUB84(dVar27,0));
          dVar24 = (double)CONCAT44(*(undefined4 *)(pdVar9 + 4),SUB84(dVar24,0));
        }
        else {
          dVar27 = pdVar7[1];
          dVar24 = pdVar9[4];
        }
        if (in_FPSCR_PR == '\0') {
          dVar24 = (double)(ulonglong)
                           (uint)((float)((ulonglong)dVar27 >> 0x20) *
                                  (float)((ulonglong)dVar24 >> 0x20) + SUB84(dVar24,0));
        }
        dVar23 = dVar30;
        dVar20 = dVar30;
        if (in_FPSCR_SZ == '\0') {
          dVar23 = (double)CONCAT44(uVar31,SUB84(dVar24,0));
          dVar20 = (double)CONCAT44(uVar31,SUB84(dVar27,0));
        }
        fVar29 = (float)((ulonglong)dVar20 >> 0x20);
        if (in_FPSCR_PR == '\0') {
          dVar23 = (double)CONCAT44(fVar29 * SUB84(dVar23,0) + (float)((ulonglong)dVar23 >> 0x20),
                                    SUB84(dVar23,0));
        }
        if (in_FPSCR_SZ == '\0') {
          *(int *)((int)pdVar19 + -4) = (int)((ulonglong)dVar23 >> 0x20);
          *(undefined4 *)(pdVar19 + -1) = *(undefined4 *)(pdVar7 + 3);
          uVar21 = DAT_8c619ef8;
          *(undefined4 *)((int)pdVar19 + -0xc) = *(undefined4 *)((int)pdVar7 + 0x14);
          pdVar19 = pdVar19 + -2;
          *(undefined4 *)pdVar19 = *(undefined4 *)(pdVar7 + 2);
        }
        else {
          pdVar19[-1] = dVar23;
          pdVar19[-2] = pdVar7[3];
          uVar21 = DAT_8c619ef8;
          pdVar19[-3] = *(double *)((int)pdVar7 + 0x14);
          in_xd2 = pdVar7[2];
          pdVar19 = pdVar19 + -4;
          *pdVar19 = in_xd2;
        }
        *(undefined4 *)((int)pdVar19 + -4) = uVar21;
        if (in_FPSCR_SZ == '\0') {
          dVar27 = (double)CONCAT44(*(undefined4 *)DAT_8c619eec,*(undefined4 *)pdVar10);
        }
        else {
          in_xd2 = *pdVar10;
          dVar27 = *DAT_8c619eec;
        }
        if (in_FPSCR_PR == '\0') {
          dVar27 = (double)CONCAT44((float)((ulonglong)dVar27 >> 0x20) * SUB84(dVar27,0),
                                    SUB84(dVar27,0));
        }
        else {
          dVar27 = dVar27 * dVar27;
        }
        if (in_FPSCR_SZ == '\0') {
          dVar27 = (double)CONCAT44((int)((ulonglong)dVar27 >> 0x20),*(undefined4 *)DAT_8c619ef0);
          dVar20 = (double)CONCAT44(fVar29,*DAT_8c619ef4);
        }
        else {
          in_xd2 = *DAT_8c619ef0;
        }
        if (in_FPSCR_PR == '\0') {
          fVar29 = SUB84(dVar27,0) - (float)((ulonglong)dVar27 >> 0x20);
        }
        else {
          fVar29 = SUB84(dVar27 - dVar27,0);
        }
        if (in_FPSCR_SZ == '\0') {
          dVar27 = (double)CONCAT44(*(undefined4 *)((int)pdVar10 + 4),fVar29);
        }
        else {
          dVar27 = *(double *)((int)pdVar10 + 4);
        }
        if (in_FPSCR_PR == '\0') {
          dVar20 = (double)CONCAT44((int)((ulonglong)dVar20 >> 0x20),
                                    SUB84(dVar20,0) * (float)((ulonglong)dVar27 >> 0x20));
        }
        else {
          dVar20 = dVar20 * dVar27;
        }
        if (in_FPSCR_SZ == '\0') {
          dVar27 = (double)CONCAT44(*(undefined4 *)(pdVar10 + 1),SUB84(dVar27,0));
        }
        else {
          dVar27 = pdVar10[1];
        }
        if (in_FPSCR_PR == '\0') {
          dVar27 = (double)CONCAT44((int)((ulonglong)dVar27 >> 0x20),
                                    SUB84(dVar27,0) - SUB84(dVar20,0));
        }
        else {
          dVar27 = dVar27 - dVar20;
        }
        if (in_FPSCR_SZ == '\0') {
          dVar20 = (double)CONCAT44((int)((ulonglong)dVar20 >> 0x20),
                                    *(undefined4 *)(DAT_8c619eec + 1));
        }
        fVar29 = (float)((ulonglong)dVar27 >> 0x20);
        if (in_FPSCR_PR == '\0') {
          fVar22 = SUB84(dVar20,0) * fVar29;
          dVar27 = (double)CONCAT44(fVar29,SUB84(dVar27,0) - fVar22);
        }
        else {
          fVar22 = SUB84(dVar20 * dVar27,0);
          dVar27 = dVar27 - dVar20 * dVar27;
        }
        if (in_FPSCR_SZ == '\0') {
          pdVar19 = (double *)((int)pdVar19 + 0x34);
          *(int *)pdVar19 = SUB84(dVar27,0);
        }
        else {
          pdVar19 = pdVar19 + 6;
          *pdVar19 = in_xd2;
        }
        iVar14 = *piVar2;
        if (in_FPSCR_SZ == '\0') {
          dVar24 = (double)CONCAT44(*(undefined4 *)((int)pdVar10 + 4),fVar22);
          dVar27 = (double)CONCAT44((int)((ulonglong)dVar27 >> 0x20),*(undefined4 *)(iVar14 + 0x14))
          ;
        }
        else {
          dVar24 = *(double *)((int)pdVar10 + 4);
          in_xd2 = *(double *)(iVar14 + 0x14);
        }
        if (in_FPSCR_PR == '\0') {
          fVar29 = SUB84(dVar27,0) * (float)((ulonglong)dVar24 >> 0x20);
        }
        else {
          fVar29 = SUB84(dVar27 * dVar24,0);
        }
        if (in_FPSCR_SZ == '\0') {
          uVar25 = CONCAT44(*(undefined4 *)(iVar14 + 4),fVar29);
          dVar27 = (double)CONCAT44(*(undefined4 *)pdVar10,SUB84(dVar24,0));
        }
        else {
          uVar25 = *(ulonglong *)(iVar14 + 4);
          dVar27 = *pdVar10;
        }
        if (in_FPSCR_PR == '\0') {
          uVar25 = (ulonglong)
                   (uint)((float)((ulonglong)dVar27 >> 0x20) * (float)(uVar25 >> 0x20) +
                         (float)uVar25);
        }
        if (in_FPSCR_SZ == '\0') {
          dVar27 = (double)CONCAT44(*(undefined4 *)(pdVar10 + 1),SUB84(dVar27,0));
          uVar25 = CONCAT44(*(undefined4 *)(iVar14 + 0x24),(int)uVar25);
        }
        else {
          dVar27 = pdVar10[1];
          uVar25 = *(ulonglong *)(iVar14 + 0x24);
        }
        if (in_FPSCR_PR == '\0') {
          uVar25 = (ulonglong)
                   (uint)((float)((ulonglong)dVar27 >> 0x20) * (float)(uVar25 >> 0x20) +
                         (float)uVar25);
        }
        dVar20 = dVar30;
        dVar24 = dVar30;
        if (in_FPSCR_SZ == '\0') {
          dVar20 = (double)CONCAT44(uVar31,(int)uVar25);
          dVar24 = (double)CONCAT44(uVar31,SUB84(dVar27,0));
        }
        if (in_FPSCR_PR == '\0') {
          dVar20 = (double)CONCAT44((float)((ulonglong)dVar24 >> 0x20) * SUB84(dVar20,0) +
                                    (float)((ulonglong)dVar20 >> 0x20),SUB84(dVar20,0));
        }
        if (in_FPSCR_SZ == '\0') {
          pdVar19 = (double *)((int)pdVar19 + -4);
          *(int *)pdVar19 = (int)((ulonglong)dVar20 >> 0x20);
        }
        else {
          pdVar19 = pdVar19 + -1;
          *pdVar19 = dVar20;
        }
        pdVar7 = (double *)*piVar2;
        if (in_FPSCR_SZ == '\0') {
          dVar27 = (double)CONCAT44(*(undefined4 *)((int)pdVar10 + 4),SUB84(dVar24,0));
          dVar24 = (double)CONCAT44(*(undefined4 *)pdVar7,*(undefined4 *)(pdVar7 + 2));
        }
        else {
          dVar27 = *(double *)((int)pdVar10 + 4);
          in_xd2 = pdVar7[2];
          dVar24 = *pdVar7;
        }
        if (in_FPSCR_PR == '\0') {
          dVar24 = (double)CONCAT44((int)((ulonglong)dVar24 >> 0x20),
                                    SUB84(dVar24,0) * (float)((ulonglong)dVar27 >> 0x20));
        }
        else {
          dVar24 = dVar24 * dVar27;
        }
        if (in_FPSCR_SZ == '\0') {
          dVar27 = (double)CONCAT44(*(undefined4 *)pdVar10,SUB84(dVar27,0));
        }
        else {
          dVar27 = *pdVar10;
        }
        if (in_FPSCR_PR == '\0') {
          dVar24 = (double)(ulonglong)
                           (uint)((float)((ulonglong)dVar27 >> 0x20) *
                                  (float)((ulonglong)dVar24 >> 0x20) + SUB84(dVar24,0));
        }
        if (in_FPSCR_SZ == '\0') {
          dVar27 = (double)CONCAT44(*(undefined4 *)(pdVar10 + 1),SUB84(dVar27,0));
          dVar24 = (double)CONCAT44(*(undefined4 *)(pdVar7 + 4),SUB84(dVar24,0));
        }
        else {
          dVar27 = pdVar10[1];
          dVar24 = pdVar7[4];
        }
        if (in_FPSCR_PR == '\0') {
          dVar24 = (double)(ulonglong)
                           (uint)((float)((ulonglong)dVar27 >> 0x20) *
                                  (float)((ulonglong)dVar24 >> 0x20) + SUB84(dVar24,0));
        }
        dVar20 = dVar30;
        in_dr0 = dVar30;
        if (in_FPSCR_SZ == '\0') {
          dVar20 = (double)CONCAT44(uVar31,SUB84(dVar24,0));
          in_dr0 = (double)CONCAT44(uVar31,SUB84(dVar27,0));
        }
        if (in_FPSCR_PR == '\0') {
          dVar20 = (double)CONCAT44((float)((ulonglong)in_dr0 >> 0x20) * SUB84(dVar20,0) +
                                    (float)((ulonglong)dVar20 >> 0x20),SUB84(dVar20,0));
        }
        if (in_FPSCR_SZ == '\0') {
          *(int *)((int)pdVar19 + -4) = (int)((ulonglong)dVar20 >> 0x20);
          *(undefined4 *)(pdVar19 + -1) = *(undefined4 *)(pdVar10 + 3);
          uVar21 = DAT_8c619efc;
          *(undefined4 *)((int)pdVar19 + -0xc) = *(undefined4 *)((int)pdVar10 + 0x14);
          pdVar19 = pdVar19 + -2;
          *(undefined4 *)pdVar19 = *(undefined4 *)(pdVar10 + 2);
        }
        else {
          pdVar19[-1] = dVar20;
          pdVar19[-2] = pdVar10[3];
          uVar21 = DAT_8c619efc;
          pdVar19[-3] = *(double *)((int)pdVar10 + 0x14);
          in_xd2 = pdVar10[2];
          pdVar19 = pdVar19 + -4;
          *pdVar19 = in_xd2;
        }
        *(undefined4 *)((int)pdVar19 + -4) = uVar21;
        unaff_r8 = (int)pdVar19 + 0x1c;
        *DAT_8c619f00 = *DAT_8c619f00 + 3;
      }
    }
  }
  else if (uVar5 == (int)DAT_8c619c90) {
    *DAT_8c619c9c = *DAT_8c619c9c + (uint)param_5[1] * 4;
    for (iVar13 = 0; uVar21 = SUB84(in_dr2,0), iVar13 < (int)(uint)param_5[1]; iVar13 = iVar13 + 1)
    {
      uVar1 = *puVar15;
      puVar18 = puVar15 + 1;
      puVar16 = puVar15 + 2;
      iVar14 = *DAT_8c61a23c;
      puVar17 = puVar15 + 3;
      puVar15 = puVar15 + 4;
      pdVar7 = (double *)((uint)*puVar18 * 0x20 + iVar14);
      pdVar9 = (double *)((uint)uVar1 * 0x20 + iVar14);
      if (in_FPSCR_SZ == '\0') {
        dVar27 = (double)CONCAT44(*(undefined4 *)(pdVar7 + 2),SUB84(param_1,0));
        uVar21 = *(undefined4 *)(pdVar9 + 2);
      }
      else {
        dVar27 = pdVar7[2];
        in_xd2 = pdVar9[2];
      }
      uVar28 = (undefined4)((ulonglong)dVar27 >> 0x20);
      dVar24 = dVar27;
      if (in_FPSCR_SZ == '\0') {
        dVar24 = (double)CONCAT44(uVar28,uVar21);
      }
      if (in_FPSCR_PR == '\0') {
        dVar24 = (double)CONCAT44((float)((ulonglong)dVar24 >> 0x20) - SUB84(dVar24,0),
                                  SUB84(dVar24,0));
      }
      else {
        dVar24 = dVar24 - dVar24;
      }
      if (in_FPSCR_SZ == '\0') {
        dVar27 = (double)CONCAT44(uVar28,*(undefined4 *)((int)pdVar7 + 0x14));
      }
      pdVar10 = (double *)((uint)*puVar16 * 0x20 + iVar14);
      fVar29 = SUB84(dVar27,0);
      if (in_FPSCR_SZ == '\0') {
        dVar24 = (double)CONCAT44((int)((ulonglong)dVar24 >> 0x20),
                                  *(undefined4 *)((int)pdVar10 + 0x14));
        in_dr0 = (double)CONCAT44((int)((ulonglong)in_dr0 >> 0x20),fVar29);
      }
      else {
        in_xd2 = *(double *)((int)pdVar10 + 0x14);
      }
      if (in_FPSCR_PR == '\0') {
        fVar22 = SUB84(dVar24,0) - fVar29;
        dVar24 = (double)CONCAT44((float)((ulonglong)dVar24 >> 0x20) * fVar22,fVar22);
      }
      else {
        dVar24 = (dVar24 - dVar27) * (dVar24 - dVar27);
      }
      if (in_FPSCR_SZ == '\0') {
        dVar24 = (double)CONCAT44((int)((ulonglong)dVar24 >> 0x20),
                                  *(undefined4 *)((int)pdVar9 + 0x14));
      }
      else {
        in_xd2 = *(double *)((int)pdVar9 + 0x14);
      }
      if (in_FPSCR_PR == '\0') {
        dVar20 = (double)CONCAT44((int)((ulonglong)in_dr0 >> 0x20),SUB84(in_dr0,0) - SUB84(dVar24,0)
                                 );
      }
      else {
        dVar20 = in_dr0 - dVar24;
      }
      if (in_FPSCR_SZ == '\0') {
        dVar24 = (double)CONCAT44((int)((ulonglong)dVar24 >> 0x20),*(undefined4 *)(pdVar10 + 2));
      }
      else {
        in_xd2 = pdVar10[2];
      }
      if (in_FPSCR_PR == '\0') {
        dVar24 = (double)CONCAT44((int)((ulonglong)dVar24 >> 0x20),
                                  SUB84(dVar24,0) - (float)((ulonglong)dVar27 >> 0x20));
      }
      else {
        dVar24 = dVar24 - dVar27;
      }
      uVar5 = (uint)((ulonglong)dVar24 >> 0x20);
      param_1 = dVar24;
      if (in_FPSCR_SZ == '\0') {
        param_1 = (double)CONCAT44(uVar5,fVar29);
      }
      if (in_FPSCR_PR == '\0') {
        in_dr0 = (double)CONCAT44((int)((ulonglong)dVar20 >> 0x20),SUB84(dVar20,0) * SUB84(dVar24,0)
                                 );
      }
      else {
        in_dr0 = dVar20 * dVar24;
      }
      in_dr2 = (double)((ulonglong)uVar5 << 0x20);
      if (in_FPSCR_PR == '\0') {
        fVar29 = (float)((ulonglong)param_1 >> 0x20) - SUB84(in_dr0,0);
        param_1 = (double)CONCAT44(fVar29,SUB84(param_1,0));
        bVar32 = 0.0 < fVar29;
      }
      else {
        param_1 = param_1 - in_dr0;
        bVar32 = in_dr2 < param_1;
      }
      pdVar19 = (double *)((uint)*puVar17 * 0x20 + iVar14);
      if (!bVar32) {
        if (in_FPSCR_SZ == '\0') {
          dVar27 = (double)CONCAT44(*(undefined4 *)DAT_8c61a240,*(undefined4 *)pdVar9);
        }
        else {
          in_xd2 = *pdVar9;
          dVar27 = *DAT_8c61a240;
        }
        if (in_FPSCR_PR == '\0') {
          dVar27 = (double)CONCAT44((float)((ulonglong)dVar27 >> 0x20) * SUB84(dVar27,0),
                                    SUB84(dVar27,0));
        }
        else {
          dVar27 = dVar27 * dVar27;
        }
        if (in_FPSCR_SZ == '\0') {
          dVar27 = (double)CONCAT44((int)((ulonglong)dVar27 >> 0x20),*(undefined4 *)DAT_8c61a244);
          in_dr0 = (double)CONCAT44((int)((ulonglong)in_dr0 >> 0x20),*DAT_8c61a248);
        }
        else {
          in_xd2 = *DAT_8c61a244;
        }
        if (in_FPSCR_PR == '\0') {
          fVar29 = SUB84(dVar27,0) - (float)((ulonglong)dVar27 >> 0x20);
        }
        else {
          fVar29 = SUB84(dVar27 - dVar27,0);
        }
        if (in_FPSCR_SZ == '\0') {
          dVar27 = (double)CONCAT44(*(undefined4 *)((int)pdVar9 + 4),fVar29);
        }
        else {
          dVar27 = *(double *)((int)pdVar9 + 4);
        }
        if (in_FPSCR_PR == '\0') {
          dVar24 = (double)CONCAT44((int)((ulonglong)in_dr0 >> 0x20),
                                    SUB84(in_dr0,0) * (float)((ulonglong)dVar27 >> 0x20));
        }
        else {
          dVar24 = in_dr0 * dVar27;
        }
        if (in_FPSCR_SZ == '\0') {
          dVar27 = (double)CONCAT44(*(undefined4 *)(pdVar9 + 1),SUB84(dVar27,0));
        }
        else {
          dVar27 = pdVar9[1];
        }
        if (in_FPSCR_PR == '\0') {
          dVar27 = (double)CONCAT44((int)((ulonglong)dVar27 >> 0x20),
                                    SUB84(dVar27,0) - SUB84(dVar24,0));
        }
        else {
          dVar27 = dVar27 - dVar24;
        }
        if (in_FPSCR_SZ == '\0') {
          dVar24 = (double)CONCAT44((int)((ulonglong)dVar24 >> 0x20),
                                    *(undefined4 *)(DAT_8c61a240 + 1));
        }
        fVar29 = (float)((ulonglong)dVar27 >> 0x20);
        if (in_FPSCR_PR == '\0') {
          fVar22 = SUB84(dVar24,0) * fVar29;
          dVar27 = (double)CONCAT44(fVar29,SUB84(dVar27,0) - fVar22);
        }
        else {
          fVar22 = SUB84(dVar24 * dVar27,0);
          dVar27 = dVar27 - dVar24 * dVar27;
        }
        if (in_FPSCR_SZ == '\0') {
          pdVar12 = (double *)(unaff_r8 + 0x18);
          *(int *)pdVar12 = SUB84(dVar27,0);
        }
        else {
          pdVar12 = (double *)(unaff_r8 + 0x14);
          *pdVar12 = in_xd2;
        }
        iVar14 = *piVar2;
        if (in_FPSCR_SZ == '\0') {
          dVar24 = (double)CONCAT44(*(undefined4 *)((int)pdVar9 + 4),fVar22);
          dVar27 = (double)CONCAT44((int)((ulonglong)dVar27 >> 0x20),*(undefined4 *)(iVar14 + 0x14))
          ;
        }
        else {
          dVar24 = *(double *)((int)pdVar9 + 4);
          in_xd2 = *(double *)(iVar14 + 0x14);
        }
        if (in_FPSCR_PR == '\0') {
          fVar29 = SUB84(dVar27,0) * (float)((ulonglong)dVar24 >> 0x20);
        }
        else {
          fVar29 = SUB84(dVar27 * dVar24,0);
        }
        if (in_FPSCR_SZ == '\0') {
          uVar25 = CONCAT44(*(undefined4 *)(iVar14 + 4),fVar29);
          dVar27 = (double)CONCAT44(*(undefined4 *)pdVar9,SUB84(dVar24,0));
        }
        else {
          uVar25 = *(ulonglong *)(iVar14 + 4);
          dVar27 = *pdVar9;
        }
        if (in_FPSCR_PR == '\0') {
          uVar25 = (ulonglong)
                   (uint)((float)((ulonglong)dVar27 >> 0x20) * (float)(uVar25 >> 0x20) +
                         (float)uVar25);
        }
        if (in_FPSCR_SZ == '\0') {
          dVar27 = (double)CONCAT44(*(undefined4 *)(pdVar9 + 1),SUB84(dVar27,0));
          uVar25 = CONCAT44(*(undefined4 *)(iVar14 + 0x24),(int)uVar25);
        }
        else {
          dVar27 = pdVar9[1];
          uVar25 = *(ulonglong *)(iVar14 + 0x24);
        }
        if (in_FPSCR_PR == '\0') {
          uVar25 = (ulonglong)
                   (uint)((float)((ulonglong)dVar27 >> 0x20) * (float)(uVar25 >> 0x20) +
                         (float)uVar25);
        }
        dVar20 = dVar30;
        dVar24 = dVar30;
        if (in_FPSCR_SZ == '\0') {
          dVar20 = (double)CONCAT44(uVar31,(int)uVar25);
          dVar24 = (double)CONCAT44(uVar31,SUB84(dVar27,0));
        }
        if (in_FPSCR_PR == '\0') {
          dVar20 = (double)CONCAT44((float)((ulonglong)dVar24 >> 0x20) * SUB84(dVar20,0) +
                                    (float)((ulonglong)dVar20 >> 0x20),SUB84(dVar20,0));
        }
        if (in_FPSCR_SZ == '\0') {
          pdVar12 = (double *)((int)pdVar12 + -4);
          *(int *)pdVar12 = (int)((ulonglong)dVar20 >> 0x20);
        }
        else {
          pdVar12 = pdVar12 + -1;
          *pdVar12 = dVar20;
        }
        pdVar6 = (double *)*piVar2;
        if (in_FPSCR_SZ == '\0') {
          dVar27 = (double)CONCAT44(*(undefined4 *)((int)pdVar9 + 4),SUB84(dVar24,0));
          dVar24 = (double)CONCAT44(*(undefined4 *)pdVar6,*(undefined4 *)(pdVar6 + 2));
        }
        else {
          dVar27 = *(double *)((int)pdVar9 + 4);
          in_xd2 = pdVar6[2];
          dVar24 = *pdVar6;
        }
        if (in_FPSCR_PR == '\0') {
          dVar24 = (double)CONCAT44((int)((ulonglong)dVar24 >> 0x20),
                                    SUB84(dVar24,0) * (float)((ulonglong)dVar27 >> 0x20));
        }
        else {
          dVar24 = dVar24 * dVar27;
        }
        if (in_FPSCR_SZ == '\0') {
          dVar27 = (double)CONCAT44(*(undefined4 *)pdVar9,SUB84(dVar27,0));
        }
        else {
          dVar27 = *pdVar9;
        }
        if (in_FPSCR_PR == '\0') {
          dVar24 = (double)(ulonglong)
                           (uint)((float)((ulonglong)dVar27 >> 0x20) *
                                  (float)((ulonglong)dVar24 >> 0x20) + SUB84(dVar24,0));
        }
        if (in_FPSCR_SZ == '\0') {
          dVar27 = (double)CONCAT44(*(undefined4 *)(pdVar9 + 1),SUB84(dVar27,0));
          dVar24 = (double)CONCAT44(*(undefined4 *)(pdVar6 + 4),SUB84(dVar24,0));
        }
        else {
          dVar27 = pdVar9[1];
          dVar24 = pdVar6[4];
        }
        if (in_FPSCR_PR == '\0') {
          dVar24 = (double)(ulonglong)
                           (uint)((float)((ulonglong)dVar27 >> 0x20) *
                                  (float)((ulonglong)dVar24 >> 0x20) + SUB84(dVar24,0));
        }
        dVar23 = dVar30;
        dVar20 = dVar30;
        if (in_FPSCR_SZ == '\0') {
          dVar23 = (double)CONCAT44(uVar31,SUB84(dVar24,0));
          dVar20 = (double)CONCAT44(uVar31,SUB84(dVar27,0));
        }
        fVar29 = (float)((ulonglong)dVar20 >> 0x20);
        if (in_FPSCR_PR == '\0') {
          dVar23 = (double)CONCAT44(fVar29 * SUB84(dVar23,0) + (float)((ulonglong)dVar23 >> 0x20),
                                    SUB84(dVar23,0));
        }
        if (in_FPSCR_SZ == '\0') {
          *(int *)((int)pdVar12 + -4) = (int)((ulonglong)dVar23 >> 0x20);
          *(undefined4 *)(pdVar12 + -1) = *(undefined4 *)(pdVar9 + 3);
          uVar21 = DAT_8c61a24c;
          *(undefined4 *)((int)pdVar12 + -0xc) = *(undefined4 *)((int)pdVar9 + 0x14);
          pdVar12 = pdVar12 + -2;
          *(undefined4 *)pdVar12 = *(undefined4 *)(pdVar9 + 2);
        }
        else {
          pdVar12[-1] = dVar23;
          pdVar12[-2] = pdVar9[3];
          uVar21 = DAT_8c61a24c;
          pdVar12[-3] = *(double *)((int)pdVar9 + 0x14);
          in_xd2 = pdVar9[2];
          pdVar12 = pdVar12 + -4;
          *pdVar12 = in_xd2;
        }
        *(undefined4 *)((int)pdVar12 + -4) = uVar21;
        if (in_FPSCR_SZ == '\0') {
          dVar27 = (double)CONCAT44(*(undefined4 *)DAT_8c61a240,*(undefined4 *)pdVar7);
        }
        else {
          in_xd2 = *pdVar7;
          dVar27 = *DAT_8c61a240;
        }
        if (in_FPSCR_PR == '\0') {
          dVar27 = (double)CONCAT44((float)((ulonglong)dVar27 >> 0x20) * SUB84(dVar27,0),
                                    SUB84(dVar27,0));
        }
        else {
          dVar27 = dVar27 * dVar27;
        }
        if (in_FPSCR_SZ == '\0') {
          dVar27 = (double)CONCAT44((int)((ulonglong)dVar27 >> 0x20),*(undefined4 *)DAT_8c61a244);
          dVar20 = (double)CONCAT44(fVar29,*DAT_8c61a248);
        }
        else {
          in_xd2 = *DAT_8c61a244;
        }
        if (in_FPSCR_PR == '\0') {
          fVar29 = SUB84(dVar27,0) - (float)((ulonglong)dVar27 >> 0x20);
        }
        else {
          fVar29 = SUB84(dVar27 - dVar27,0);
        }
        if (in_FPSCR_SZ == '\0') {
          dVar27 = (double)CONCAT44(*(undefined4 *)((int)pdVar7 + 4),fVar29);
        }
        else {
          dVar27 = *(double *)((int)pdVar7 + 4);
        }
        if (in_FPSCR_PR == '\0') {
          dVar20 = (double)CONCAT44((int)((ulonglong)dVar20 >> 0x20),
                                    SUB84(dVar20,0) * (float)((ulonglong)dVar27 >> 0x20));
        }
        else {
          dVar20 = dVar20 * dVar27;
        }
        if (in_FPSCR_SZ == '\0') {
          dVar27 = (double)CONCAT44(*(undefined4 *)(pdVar7 + 1),SUB84(dVar27,0));
        }
        else {
          dVar27 = pdVar7[1];
        }
        if (in_FPSCR_PR == '\0') {
          dVar27 = (double)CONCAT44((int)((ulonglong)dVar27 >> 0x20),
                                    SUB84(dVar27,0) - SUB84(dVar20,0));
        }
        else {
          dVar27 = dVar27 - dVar20;
        }
        if (in_FPSCR_SZ == '\0') {
          dVar20 = (double)CONCAT44((int)((ulonglong)dVar20 >> 0x20),
                                    *(undefined4 *)(DAT_8c61a240 + 1));
        }
        fVar29 = (float)((ulonglong)dVar27 >> 0x20);
        if (in_FPSCR_PR == '\0') {
          fVar22 = SUB84(dVar20,0) * fVar29;
          dVar27 = (double)CONCAT44(fVar29,SUB84(dVar27,0) - fVar22);
        }
        else {
          fVar22 = SUB84(dVar20 * dVar27,0);
          dVar27 = dVar27 - dVar20 * dVar27;
        }
        if (in_FPSCR_SZ == '\0') {
          pdVar12 = (double *)((int)pdVar12 + 0x34);
          *(int *)pdVar12 = SUB84(dVar27,0);
        }
        else {
          pdVar12 = pdVar12 + 6;
          *pdVar12 = in_xd2;
        }
        iVar14 = *piVar2;
        if (in_FPSCR_SZ == '\0') {
          dVar24 = (double)CONCAT44(*(undefined4 *)((int)pdVar7 + 4),fVar22);
          dVar27 = (double)CONCAT44((int)((ulonglong)dVar27 >> 0x20),*(undefined4 *)(iVar14 + 0x14))
          ;
        }
        else {
          dVar24 = *(double *)((int)pdVar7 + 4);
          in_xd2 = *(double *)(iVar14 + 0x14);
        }
        if (in_FPSCR_PR == '\0') {
          fVar29 = SUB84(dVar27,0) * (float)((ulonglong)dVar24 >> 0x20);
        }
        else {
          fVar29 = SUB84(dVar27 * dVar24,0);
        }
        if (in_FPSCR_SZ == '\0') {
          uVar25 = CONCAT44(*(undefined4 *)(iVar14 + 4),fVar29);
          dVar27 = (double)CONCAT44(*(undefined4 *)pdVar7,SUB84(dVar24,0));
        }
        else {
          uVar25 = *(ulonglong *)(iVar14 + 4);
          dVar27 = *pdVar7;
        }
        if (in_FPSCR_PR == '\0') {
          uVar25 = (ulonglong)
                   (uint)((float)((ulonglong)dVar27 >> 0x20) * (float)(uVar25 >> 0x20) +
                         (float)uVar25);
        }
        if (in_FPSCR_SZ == '\0') {
          dVar27 = (double)CONCAT44(*(undefined4 *)(pdVar7 + 1),SUB84(dVar27,0));
          uVar25 = CONCAT44(*(undefined4 *)(iVar14 + 0x24),(int)uVar25);
        }
        else {
          dVar27 = pdVar7[1];
          uVar25 = *(ulonglong *)(iVar14 + 0x24);
        }
        if (in_FPSCR_PR == '\0') {
          uVar25 = (ulonglong)
                   (uint)((float)((ulonglong)dVar27 >> 0x20) * (float)(uVar25 >> 0x20) +
                         (float)uVar25);
        }
        dVar20 = dVar30;
        dVar24 = dVar30;
        if (in_FPSCR_SZ == '\0') {
          dVar20 = (double)CONCAT44(uVar31,(int)uVar25);
          dVar24 = (double)CONCAT44(uVar31,SUB84(dVar27,0));
        }
        if (in_FPSCR_PR == '\0') {
          dVar20 = (double)CONCAT44((float)((ulonglong)dVar24 >> 0x20) * SUB84(dVar20,0) +
                                    (float)((ulonglong)dVar20 >> 0x20),SUB84(dVar20,0));
        }
        if (in_FPSCR_SZ == '\0') {
          pdVar12 = (double *)((int)pdVar12 + -4);
          *(int *)pdVar12 = (int)((ulonglong)dVar20 >> 0x20);
        }
        else {
          pdVar12 = pdVar12 + -1;
          *pdVar12 = dVar20;
        }
        pdVar9 = (double *)*piVar2;
        if (in_FPSCR_SZ == '\0') {
          dVar27 = (double)CONCAT44(*(undefined4 *)((int)pdVar7 + 4),SUB84(dVar24,0));
          dVar24 = (double)CONCAT44(*(undefined4 *)pdVar9,*(undefined4 *)(pdVar9 + 2));
        }
        else {
          dVar27 = *(double *)((int)pdVar7 + 4);
          in_xd2 = pdVar9[2];
          dVar24 = *pdVar9;
        }
        if (in_FPSCR_PR == '\0') {
          dVar24 = (double)CONCAT44((int)((ulonglong)dVar24 >> 0x20),
                                    SUB84(dVar24,0) * (float)((ulonglong)dVar27 >> 0x20));
        }
        else {
          dVar24 = dVar24 * dVar27;
        }
        if (in_FPSCR_SZ == '\0') {
          dVar27 = (double)CONCAT44(*(undefined4 *)pdVar7,SUB84(dVar27,0));
        }
        else {
          dVar27 = *pdVar7;
        }
        if (in_FPSCR_PR == '\0') {
          dVar24 = (double)(ulonglong)
                           (uint)((float)((ulonglong)dVar27 >> 0x20) *
                                  (float)((ulonglong)dVar24 >> 0x20) + SUB84(dVar24,0));
        }
        if (in_FPSCR_SZ == '\0') {
          dVar27 = (double)CONCAT44(*(undefined4 *)(pdVar7 + 1),SUB84(dVar27,0));
          dVar24 = (double)CONCAT44(*(undefined4 *)(pdVar9 + 4),SUB84(dVar24,0));
        }
        else {
          dVar27 = pdVar7[1];
          dVar24 = pdVar9[4];
        }
        if (in_FPSCR_PR == '\0') {
          dVar24 = (double)(ulonglong)
                           (uint)((float)((ulonglong)dVar27 >> 0x20) *
                                  (float)((ulonglong)dVar24 >> 0x20) + SUB84(dVar24,0));
        }
        dVar23 = dVar30;
        dVar20 = dVar30;
        if (in_FPSCR_SZ == '\0') {
          dVar23 = (double)CONCAT44(uVar31,SUB84(dVar24,0));
          dVar20 = (double)CONCAT44(uVar31,SUB84(dVar27,0));
        }
        fVar29 = (float)((ulonglong)dVar20 >> 0x20);
        if (in_FPSCR_PR == '\0') {
          dVar23 = (double)CONCAT44(fVar29 * SUB84(dVar23,0) + (float)((ulonglong)dVar23 >> 0x20),
                                    SUB84(dVar23,0));
        }
        if (in_FPSCR_SZ == '\0') {
          *(int *)((int)pdVar12 + -4) = (int)((ulonglong)dVar23 >> 0x20);
          *(undefined4 *)(pdVar12 + -1) = *(undefined4 *)(pdVar7 + 3);
          uVar21 = DAT_8c61a24c;
          *(undefined4 *)((int)pdVar12 + -0xc) = *(undefined4 *)((int)pdVar7 + 0x14);
          pdVar12 = pdVar12 + -2;
          *(undefined4 *)pdVar12 = *(undefined4 *)(pdVar7 + 2);
        }
        else {
          pdVar12[-1] = dVar23;
          pdVar12[-2] = pdVar7[3];
          uVar21 = DAT_8c61a24c;
          pdVar12[-3] = *(double *)((int)pdVar7 + 0x14);
          in_xd2 = pdVar7[2];
          pdVar12 = pdVar12 + -4;
          *pdVar12 = in_xd2;
        }
        *(undefined4 *)((int)pdVar12 + -4) = uVar21;
        if (in_FPSCR_SZ == '\0') {
          dVar27 = (double)CONCAT44(*(undefined4 *)DAT_8c61a240,*(undefined4 *)pdVar10);
        }
        else {
          in_xd2 = *pdVar10;
          dVar27 = *DAT_8c61a240;
        }
        if (in_FPSCR_PR == '\0') {
          dVar27 = (double)CONCAT44((float)((ulonglong)dVar27 >> 0x20) * SUB84(dVar27,0),
                                    SUB84(dVar27,0));
        }
        else {
          dVar27 = dVar27 * dVar27;
        }
        if (in_FPSCR_SZ == '\0') {
          dVar27 = (double)CONCAT44((int)((ulonglong)dVar27 >> 0x20),*(undefined4 *)DAT_8c61a244);
          dVar20 = (double)CONCAT44(fVar29,*DAT_8c61a248);
        }
        else {
          in_xd2 = *DAT_8c61a244;
        }
        if (in_FPSCR_PR == '\0') {
          fVar29 = SUB84(dVar27,0) - (float)((ulonglong)dVar27 >> 0x20);
        }
        else {
          fVar29 = SUB84(dVar27 - dVar27,0);
        }
        if (in_FPSCR_SZ == '\0') {
          dVar27 = (double)CONCAT44(*(undefined4 *)((int)pdVar10 + 4),fVar29);
        }
        else {
          dVar27 = *(double *)((int)pdVar10 + 4);
        }
        if (in_FPSCR_PR == '\0') {
          dVar20 = (double)CONCAT44((int)((ulonglong)dVar20 >> 0x20),
                                    SUB84(dVar20,0) * (float)((ulonglong)dVar27 >> 0x20));
        }
        else {
          dVar20 = dVar20 * dVar27;
        }
        if (in_FPSCR_SZ == '\0') {
          dVar27 = (double)CONCAT44(*(undefined4 *)(pdVar10 + 1),SUB84(dVar27,0));
        }
        else {
          dVar27 = pdVar10[1];
        }
        if (in_FPSCR_PR == '\0') {
          dVar27 = (double)CONCAT44((int)((ulonglong)dVar27 >> 0x20),
                                    SUB84(dVar27,0) - SUB84(dVar20,0));
        }
        else {
          dVar27 = dVar27 - dVar20;
        }
        if (in_FPSCR_SZ == '\0') {
          dVar20 = (double)CONCAT44((int)((ulonglong)dVar20 >> 0x20),
                                    *(undefined4 *)(DAT_8c61a240 + 1));
        }
        fVar29 = (float)((ulonglong)dVar27 >> 0x20);
        if (in_FPSCR_PR == '\0') {
          fVar22 = SUB84(dVar20,0) * fVar29;
          dVar27 = (double)CONCAT44(fVar29,SUB84(dVar27,0) - fVar22);
        }
        else {
          fVar22 = SUB84(dVar20 * dVar27,0);
          dVar27 = dVar27 - dVar20 * dVar27;
        }
        if (in_FPSCR_SZ == '\0') {
          pdVar12 = (double *)((int)pdVar12 + 0x34);
          *(int *)pdVar12 = SUB84(dVar27,0);
        }
        else {
          pdVar12 = pdVar12 + 6;
          *pdVar12 = in_xd2;
        }
        iVar14 = *piVar2;
        if (in_FPSCR_SZ == '\0') {
          dVar24 = (double)CONCAT44(*(undefined4 *)((int)pdVar10 + 4),fVar22);
          dVar27 = (double)CONCAT44((int)((ulonglong)dVar27 >> 0x20),*(undefined4 *)(iVar14 + 0x14))
          ;
        }
        else {
          dVar24 = *(double *)((int)pdVar10 + 4);
          in_xd2 = *(double *)(iVar14 + 0x14);
        }
        if (in_FPSCR_PR == '\0') {
          fVar29 = SUB84(dVar27,0) * (float)((ulonglong)dVar24 >> 0x20);
        }
        else {
          fVar29 = SUB84(dVar27 * dVar24,0);
        }
        if (in_FPSCR_SZ == '\0') {
          uVar25 = CONCAT44(*(undefined4 *)(iVar14 + 4),fVar29);
          dVar27 = (double)CONCAT44(*(undefined4 *)pdVar10,SUB84(dVar24,0));
        }
        else {
          uVar25 = *(ulonglong *)(iVar14 + 4);
          dVar27 = *pdVar10;
        }
        if (in_FPSCR_PR == '\0') {
          uVar25 = (ulonglong)
                   (uint)((float)((ulonglong)dVar27 >> 0x20) * (float)(uVar25 >> 0x20) +
                         (float)uVar25);
        }
        if (in_FPSCR_SZ == '\0') {
          dVar27 = (double)CONCAT44(*(undefined4 *)(pdVar10 + 1),SUB84(dVar27,0));
          uVar25 = CONCAT44(*(undefined4 *)(iVar14 + 0x24),(int)uVar25);
        }
        else {
          dVar27 = pdVar10[1];
          uVar25 = *(ulonglong *)(iVar14 + 0x24);
        }
        if (in_FPSCR_PR == '\0') {
          uVar25 = (ulonglong)
                   (uint)((float)((ulonglong)dVar27 >> 0x20) * (float)(uVar25 >> 0x20) +
                         (float)uVar25);
        }
        dVar20 = dVar30;
        dVar24 = dVar30;
        if (in_FPSCR_SZ == '\0') {
          dVar20 = (double)CONCAT44(uVar31,(int)uVar25);
          dVar24 = (double)CONCAT44(uVar31,SUB84(dVar27,0));
        }
        if (in_FPSCR_PR == '\0') {
          dVar20 = (double)CONCAT44((float)((ulonglong)dVar24 >> 0x20) * SUB84(dVar20,0) +
                                    (float)((ulonglong)dVar20 >> 0x20),SUB84(dVar20,0));
        }
        if (in_FPSCR_SZ == '\0') {
          pdVar12 = (double *)((int)pdVar12 + -4);
          *(int *)pdVar12 = (int)((ulonglong)dVar20 >> 0x20);
        }
        else {
          pdVar12 = pdVar12 + -1;
          *pdVar12 = dVar20;
        }
        pdVar7 = (double *)*piVar2;
        if (in_FPSCR_SZ == '\0') {
          dVar27 = (double)CONCAT44(*(undefined4 *)((int)pdVar10 + 4),SUB84(dVar24,0));
          dVar24 = (double)CONCAT44(*(undefined4 *)pdVar7,*(undefined4 *)(pdVar7 + 2));
        }
        else {
          dVar27 = *(double *)((int)pdVar10 + 4);
          in_xd2 = pdVar7[2];
          dVar24 = *pdVar7;
        }
        if (in_FPSCR_PR == '\0') {
          dVar24 = (double)CONCAT44((int)((ulonglong)dVar24 >> 0x20),
                                    SUB84(dVar24,0) * (float)((ulonglong)dVar27 >> 0x20));
        }
        else {
          dVar24 = dVar24 * dVar27;
        }
        if (in_FPSCR_SZ == '\0') {
          dVar27 = (double)CONCAT44(*(undefined4 *)pdVar10,SUB84(dVar27,0));
        }
        else {
          dVar27 = *pdVar10;
        }
        if (in_FPSCR_PR == '\0') {
          dVar24 = (double)(ulonglong)
                           (uint)((float)((ulonglong)dVar27 >> 0x20) *
                                  (float)((ulonglong)dVar24 >> 0x20) + SUB84(dVar24,0));
        }
        if (in_FPSCR_SZ == '\0') {
          dVar27 = (double)CONCAT44(*(undefined4 *)(pdVar10 + 1),SUB84(dVar27,0));
          dVar24 = (double)CONCAT44(*(undefined4 *)(pdVar7 + 4),SUB84(dVar24,0));
        }
        else {
          dVar27 = pdVar10[1];
          dVar24 = pdVar7[4];
        }
        if (in_FPSCR_PR == '\0') {
          dVar24 = (double)(ulonglong)
                           (uint)((float)((ulonglong)dVar27 >> 0x20) *
                                  (float)((ulonglong)dVar24 >> 0x20) + SUB84(dVar24,0));
        }
        dVar23 = dVar30;
        dVar20 = dVar30;
        if (in_FPSCR_SZ == '\0') {
          dVar23 = (double)CONCAT44(uVar31,SUB84(dVar24,0));
          dVar20 = (double)CONCAT44(uVar31,SUB84(dVar27,0));
        }
        fVar29 = (float)((ulonglong)dVar20 >> 0x20);
        if (in_FPSCR_PR == '\0') {
          dVar23 = (double)CONCAT44(fVar29 * SUB84(dVar23,0) + (float)((ulonglong)dVar23 >> 0x20),
                                    SUB84(dVar23,0));
        }
        if (in_FPSCR_SZ == '\0') {
          *(int *)((int)pdVar12 + -4) = (int)((ulonglong)dVar23 >> 0x20);
          *(undefined4 *)(pdVar12 + -1) = *(undefined4 *)(pdVar10 + 3);
          uVar21 = DAT_8c61a24c;
          *(undefined4 *)((int)pdVar12 + -0xc) = *(undefined4 *)((int)pdVar10 + 0x14);
          pdVar12 = pdVar12 + -2;
          *(undefined4 *)pdVar12 = *(undefined4 *)(pdVar10 + 2);
        }
        else {
          pdVar12[-1] = dVar23;
          pdVar12[-2] = pdVar10[3];
          uVar21 = DAT_8c61a24c;
          pdVar12[-3] = *(double *)((int)pdVar10 + 0x14);
          in_xd2 = pdVar10[2];
          pdVar12 = pdVar12 + -4;
          *pdVar12 = in_xd2;
        }
        *(undefined4 *)((int)pdVar12 + -4) = uVar21;
        if (in_FPSCR_SZ == '\0') {
          dVar27 = (double)CONCAT44(*(undefined4 *)DAT_8c61a240,*(undefined4 *)pdVar19);
        }
        else {
          in_xd2 = *pdVar19;
          dVar27 = *DAT_8c61a240;
        }
        if (in_FPSCR_PR == '\0') {
          dVar27 = (double)CONCAT44((float)((ulonglong)dVar27 >> 0x20) * SUB84(dVar27,0),
                                    SUB84(dVar27,0));
        }
        else {
          dVar27 = dVar27 * dVar27;
        }
        if (in_FPSCR_SZ == '\0') {
          dVar27 = (double)CONCAT44((int)((ulonglong)dVar27 >> 0x20),*(undefined4 *)DAT_8c61a244);
          dVar20 = (double)CONCAT44(fVar29,*DAT_8c61a248);
        }
        else {
          in_xd2 = *DAT_8c61a244;
        }
        if (in_FPSCR_PR == '\0') {
          fVar29 = SUB84(dVar27,0) - (float)((ulonglong)dVar27 >> 0x20);
        }
        else {
          fVar29 = SUB84(dVar27 - dVar27,0);
        }
        if (in_FPSCR_SZ == '\0') {
          dVar27 = (double)CONCAT44(*(undefined4 *)((int)pdVar19 + 4),fVar29);
        }
        else {
          dVar27 = *(double *)((int)pdVar19 + 4);
        }
        if (in_FPSCR_PR == '\0') {
          dVar20 = (double)CONCAT44((int)((ulonglong)dVar20 >> 0x20),
                                    SUB84(dVar20,0) * (float)((ulonglong)dVar27 >> 0x20));
        }
        else {
          dVar20 = dVar20 * dVar27;
        }
        if (in_FPSCR_SZ == '\0') {
          dVar27 = (double)CONCAT44(*(undefined4 *)(pdVar19 + 1),SUB84(dVar27,0));
        }
        else {
          dVar27 = pdVar19[1];
        }
        if (in_FPSCR_PR == '\0') {
          dVar27 = (double)CONCAT44((int)((ulonglong)dVar27 >> 0x20),
                                    SUB84(dVar27,0) - SUB84(dVar20,0));
        }
        else {
          dVar27 = dVar27 - dVar20;
        }
        if (in_FPSCR_SZ == '\0') {
          dVar20 = (double)CONCAT44((int)((ulonglong)dVar20 >> 0x20),
                                    *(undefined4 *)(DAT_8c61a240 + 1));
        }
        fVar29 = (float)((ulonglong)dVar27 >> 0x20);
        if (in_FPSCR_PR == '\0') {
          fVar22 = SUB84(dVar20,0) * fVar29;
          dVar27 = (double)CONCAT44(fVar29,SUB84(dVar27,0) - fVar22);
        }
        else {
          fVar22 = SUB84(dVar20 * dVar27,0);
          dVar27 = dVar27 - dVar20 * dVar27;
        }
        if (in_FPSCR_SZ == '\0') {
          pdVar12 = (double *)((int)pdVar12 + 0x34);
          *(int *)pdVar12 = SUB84(dVar27,0);
        }
        else {
          pdVar12 = pdVar12 + 6;
          *pdVar12 = in_xd2;
        }
        iVar14 = *piVar2;
        if (in_FPSCR_SZ == '\0') {
          dVar24 = (double)CONCAT44(*(undefined4 *)((int)pdVar19 + 4),fVar22);
          dVar27 = (double)CONCAT44((int)((ulonglong)dVar27 >> 0x20),*(undefined4 *)(iVar14 + 0x14))
          ;
        }
        else {
          dVar24 = *(double *)((int)pdVar19 + 4);
          in_xd2 = *(double *)(iVar14 + 0x14);
        }
        if (in_FPSCR_PR == '\0') {
          fVar29 = SUB84(dVar27,0) * (float)((ulonglong)dVar24 >> 0x20);
        }
        else {
          fVar29 = SUB84(dVar27 * dVar24,0);
        }
        if (in_FPSCR_SZ == '\0') {
          uVar25 = CONCAT44(*(undefined4 *)(iVar14 + 4),fVar29);
          dVar27 = (double)CONCAT44(*(undefined4 *)pdVar19,SUB84(dVar24,0));
        }
        else {
          uVar25 = *(ulonglong *)(iVar14 + 4);
          dVar27 = *pdVar19;
        }
        if (in_FPSCR_PR == '\0') {
          uVar25 = (ulonglong)
                   (uint)((float)((ulonglong)dVar27 >> 0x20) * (float)(uVar25 >> 0x20) +
                         (float)uVar25);
        }
        if (in_FPSCR_SZ == '\0') {
          dVar27 = (double)CONCAT44(*(undefined4 *)(pdVar19 + 1),SUB84(dVar27,0));
          uVar25 = CONCAT44(*(undefined4 *)(iVar14 + 0x24),(int)uVar25);
        }
        else {
          dVar27 = pdVar19[1];
          uVar25 = *(ulonglong *)(iVar14 + 0x24);
        }
        if (in_FPSCR_PR == '\0') {
          uVar25 = (ulonglong)
                   (uint)((float)((ulonglong)dVar27 >> 0x20) * (float)(uVar25 >> 0x20) +
                         (float)uVar25);
        }
        dVar20 = dVar30;
        dVar24 = dVar30;
        if (in_FPSCR_SZ == '\0') {
          dVar20 = (double)CONCAT44(uVar31,(int)uVar25);
          dVar24 = (double)CONCAT44(uVar31,SUB84(dVar27,0));
        }
        if (in_FPSCR_PR == '\0') {
          dVar20 = (double)CONCAT44((float)((ulonglong)dVar24 >> 0x20) * SUB84(dVar20,0) +
                                    (float)((ulonglong)dVar20 >> 0x20),SUB84(dVar20,0));
        }
        if (in_FPSCR_SZ == '\0') {
          pdVar12 = (double *)((int)pdVar12 + -4);
          *(int *)pdVar12 = (int)((ulonglong)dVar20 >> 0x20);
        }
        else {
          pdVar12 = pdVar12 + -1;
          *pdVar12 = dVar20;
        }
        pdVar7 = (double *)*piVar2;
        if (in_FPSCR_SZ == '\0') {
          dVar27 = (double)CONCAT44(*(undefined4 *)((int)pdVar19 + 4),SUB84(dVar24,0));
          dVar24 = (double)CONCAT44(*(undefined4 *)pdVar7,*(undefined4 *)(pdVar7 + 2));
        }
        else {
          dVar27 = *(double *)((int)pdVar19 + 4);
          in_xd2 = pdVar7[2];
          dVar24 = *pdVar7;
        }
        if (in_FPSCR_PR == '\0') {
          dVar24 = (double)CONCAT44((int)((ulonglong)dVar24 >> 0x20),
                                    SUB84(dVar24,0) * (float)((ulonglong)dVar27 >> 0x20));
        }
        else {
          dVar24 = dVar24 * dVar27;
        }
        if (in_FPSCR_SZ == '\0') {
          dVar27 = (double)CONCAT44(*(undefined4 *)pdVar19,SUB84(dVar27,0));
        }
        else {
          dVar27 = *pdVar19;
        }
        if (in_FPSCR_PR == '\0') {
          dVar24 = (double)(ulonglong)
                           (uint)((float)((ulonglong)dVar27 >> 0x20) *
                                  (float)((ulonglong)dVar24 >> 0x20) + SUB84(dVar24,0));
        }
        if (in_FPSCR_SZ == '\0') {
          dVar27 = (double)CONCAT44(*(undefined4 *)(pdVar19 + 1),SUB84(dVar27,0));
          dVar24 = (double)CONCAT44(*(undefined4 *)(pdVar7 + 4),SUB84(dVar24,0));
        }
        else {
          dVar27 = pdVar19[1];
          dVar24 = pdVar7[4];
        }
        if (in_FPSCR_PR == '\0') {
          dVar24 = (double)(ulonglong)
                           (uint)((float)((ulonglong)dVar27 >> 0x20) *
                                  (float)((ulonglong)dVar24 >> 0x20) + SUB84(dVar24,0));
        }
        in_dr2 = dVar30;
        in_dr0 = dVar30;
        if (in_FPSCR_SZ == '\0') {
          in_dr2 = (double)CONCAT44(uVar31,SUB84(dVar24,0));
          in_dr0 = (double)CONCAT44(uVar31,SUB84(dVar27,0));
        }
        if (in_FPSCR_PR == '\0') {
          in_dr2 = (double)CONCAT44((float)((ulonglong)in_dr0 >> 0x20) * SUB84(in_dr2,0) +
                                    (float)((ulonglong)in_dr2 >> 0x20),SUB84(in_dr2,0));
        }
        if (in_FPSCR_SZ == '\0') {
          *(int *)((int)pdVar12 + -4) = (int)((ulonglong)in_dr2 >> 0x20);
          *(undefined4 *)(pdVar12 + -1) = *(undefined4 *)(pdVar19 + 3);
          uVar21 = DAT_8c61a250;
          *(undefined4 *)((int)pdVar12 + -0xc) = *(undefined4 *)((int)pdVar19 + 0x14);
          in_dr2 = (double)(ulonglong)*(uint *)(pdVar19 + 2);
          pdVar12 = pdVar12 + -2;
          *(uint *)pdVar12 = *(uint *)(pdVar19 + 2);
        }
        else {
          pdVar12[-1] = in_dr2;
          pdVar12[-2] = pdVar19[3];
          uVar21 = DAT_8c61a250;
          pdVar12[-3] = *(double *)((int)pdVar19 + 0x14);
          in_xd2 = pdVar19[2];
          pdVar12 = pdVar12 + -4;
          *pdVar12 = in_xd2;
        }
        *(undefined4 *)((int)pdVar12 + -4) = uVar21;
        unaff_r8 = (int)pdVar12 + 0x1c;
        *DAT_8c61a254 = *DAT_8c61a254 + 4;
      }
    }
  }
  else if (uVar5 == DAT_8c619ca4) {
    for (iVar13 = 0; piVar4 = DAT_8c61a23c, uVar21 = SUB84(param_1,0),
        iVar13 < (int)(uint)param_5[1]; iVar13 = iVar13 + 1) {
      puVar18 = puVar15 + 1;
      uVar5 = (uint)*puVar15;
      *piVar3 = *piVar3 + uVar5;
      iVar14 = *piVar4;
      iVar8 = (uint)puVar15[2] * 0x20 + iVar14;
      if (in_FPSCR_SZ == '\0') {
        uVar21 = *(undefined4 *)(iVar8 + 0x10);
      }
      else {
        in_xd4 = *(double *)(iVar8 + 0x10);
      }
      iVar11 = (uint)*puVar18 * 0x20 + iVar14;
      if (in_FPSCR_SZ == '\0') {
        dVar27 = (double)CONCAT44(uVar21,*(undefined4 *)(iVar11 + 0x10));
      }
      else {
        in_xd2 = *(double *)(iVar11 + 0x10);
        dVar27 = in_xd4;
      }
      if (in_FPSCR_PR == '\0') {
        dVar27 = (double)CONCAT44((float)((ulonglong)dVar27 >> 0x20) - SUB84(dVar27,0),
                                  SUB84(dVar27,0));
      }
      else {
        dVar27 = dVar27 - dVar27;
      }
      if (in_FPSCR_SZ == '\0') {
        dVar24 = (double)CONCAT44(*(undefined4 *)(iVar8 + 0x14),uVar21);
      }
      else {
        dVar24 = *(double *)(iVar8 + 0x14);
      }
      iVar14 = (uint)puVar15[3] * 0x20 + iVar14;
      fVar29 = (float)((ulonglong)dVar24 >> 0x20);
      if (in_FPSCR_SZ == '\0') {
        dVar27 = (double)CONCAT44((int)((ulonglong)dVar27 >> 0x20),*(undefined4 *)(iVar14 + 0x14));
        in_dr0 = (double)CONCAT44((int)((ulonglong)in_dr0 >> 0x20),fVar29);
      }
      else {
        in_xd2 = *(double *)(iVar14 + 0x14);
      }
      if (in_FPSCR_PR == '\0') {
        fVar29 = SUB84(dVar27,0) - fVar29;
        dVar27 = (double)CONCAT44((float)((ulonglong)dVar27 >> 0x20) * fVar29,fVar29);
      }
      else {
        dVar27 = (dVar27 - dVar24) * (dVar27 - dVar24);
      }
      if (in_FPSCR_SZ == '\0') {
        dVar27 = (double)CONCAT44((int)((ulonglong)dVar27 >> 0x20),*(undefined4 *)(iVar11 + 0x14));
      }
      else {
        in_xd2 = *(double *)(iVar11 + 0x14);
      }
      if (in_FPSCR_PR == '\0') {
        dVar20 = (double)CONCAT44((int)((ulonglong)in_dr0 >> 0x20),SUB84(in_dr0,0) - SUB84(dVar27,0)
                                 );
      }
      else {
        dVar20 = in_dr0 - dVar27;
      }
      if (in_FPSCR_SZ == '\0') {
        dVar27 = (double)CONCAT44((int)((ulonglong)dVar27 >> 0x20),*(undefined4 *)(iVar14 + 0x10));
      }
      else {
        in_xd2 = *(double *)(iVar14 + 0x10);
      }
      if (in_FPSCR_PR == '\0') {
        dVar27 = (double)CONCAT44((int)((ulonglong)dVar27 >> 0x20),SUB84(dVar27,0) - SUB84(dVar24,0)
                                 );
      }
      else {
        dVar27 = dVar27 - dVar24;
      }
      uVar26 = (uint)((ulonglong)dVar27 >> 0x20);
      param_1 = dVar27;
      if (in_FPSCR_SZ == '\0') {
        param_1 = (double)CONCAT44(uVar26,SUB84(dVar24,0));
      }
      if (in_FPSCR_PR == '\0') {
        in_dr0 = (double)CONCAT44((int)((ulonglong)dVar20 >> 0x20),SUB84(dVar20,0) * SUB84(dVar27,0)
                                 );
      }
      else {
        in_dr0 = dVar20 * dVar27;
      }
      if (in_FPSCR_PR == '\0') {
        fVar29 = (float)((ulonglong)param_1 >> 0x20) - SUB84(in_dr0,0);
        param_1 = (double)CONCAT44(fVar29,SUB84(param_1,0));
        bVar32 = 0.0 < fVar29;
      }
      else {
        param_1 = param_1 - in_dr0;
        bVar32 = (double)((ulonglong)uVar26 << 0x20) < param_1;
      }
      if (bVar32) {
        puVar15 = puVar18 + uVar5;
      }
      else {
        *DAT_8c61a464 = *DAT_8c61a464 + uVar5;
        iVar14 = 0;
        if (0 < (int)(uVar5 - 1)) {
          do {
            uVar1 = *puVar18;
            puVar18 = puVar18 + 1;
            pdVar7 = (double *)((uint)uVar1 * 0x20 + *DAT_8c61a468);
            if (in_FPSCR_SZ == '\0') {
              dVar27 = (double)CONCAT44(*(undefined4 *)DAT_8c61a46c,*(undefined4 *)pdVar7);
              in_dr0 = (double)CONCAT44((int)((ulonglong)in_dr0 >> 0x20),*DAT_8c61a474);
            }
            else {
              in_xd2 = *pdVar7;
              dVar27 = *DAT_8c61a46c;
            }
            if (in_FPSCR_PR == '\0') {
              dVar27 = (double)CONCAT44((float)((ulonglong)dVar27 >> 0x20) * SUB84(dVar27,0),
                                        SUB84(dVar27,0));
            }
            else {
              dVar27 = dVar27 * dVar27;
            }
            if (in_FPSCR_SZ == '\0') {
              dVar27 = (double)CONCAT44((int)((ulonglong)dVar27 >> 0x20),*(undefined4 *)DAT_8c61a470
                                       );
            }
            else {
              in_xd2 = *DAT_8c61a470;
            }
            if (in_FPSCR_PR == '\0') {
              fVar29 = SUB84(dVar27,0) - (float)((ulonglong)dVar27 >> 0x20);
            }
            else {
              fVar29 = SUB84(dVar27 - dVar27,0);
            }
            if (in_FPSCR_SZ == '\0') {
              dVar27 = (double)CONCAT44(*(undefined4 *)((int)pdVar7 + 4),fVar29);
            }
            else {
              dVar27 = *(double *)((int)pdVar7 + 4);
            }
            if (in_FPSCR_PR == '\0') {
              dVar24 = (double)CONCAT44((int)((ulonglong)in_dr0 >> 0x20),
                                        SUB84(in_dr0,0) * (float)((ulonglong)dVar27 >> 0x20));
            }
            else {
              dVar24 = in_dr0 * dVar27;
            }
            if (in_FPSCR_SZ == '\0') {
              dVar27 = (double)CONCAT44(*(undefined4 *)(pdVar7 + 1),SUB84(dVar27,0));
            }
            else {
              dVar27 = pdVar7[1];
            }
            if (in_FPSCR_PR == '\0') {
              dVar27 = (double)CONCAT44((int)((ulonglong)dVar27 >> 0x20),
                                        SUB84(dVar27,0) - SUB84(dVar24,0));
            }
            else {
              dVar27 = dVar27 - dVar24;
            }
            if (in_FPSCR_SZ == '\0') {
              dVar24 = (double)CONCAT44((int)((ulonglong)dVar24 >> 0x20),
                                        *(undefined4 *)(DAT_8c61a46c + 1));
            }
            if (in_FPSCR_PR == '\0') {
              fVar29 = SUB84(dVar24,0) * (float)((ulonglong)dVar27 >> 0x20);
              fVar22 = SUB84(dVar27,0) - fVar29;
            }
            else {
              fVar29 = SUB84(dVar24 * dVar27,0);
              fVar22 = SUB84(dVar27 - dVar24 * dVar27,0);
            }
            if (in_FPSCR_SZ == '\0') {
              pdVar9 = (double *)(unaff_r8 + 0x18);
              *(float *)pdVar9 = fVar22;
            }
            else {
              pdVar9 = (double *)(unaff_r8 + 0x14);
              *pdVar9 = in_xd2;
            }
            iVar8 = *piVar2;
            if (in_FPSCR_SZ == '\0') {
              dVar27 = (double)CONCAT44(*(undefined4 *)((int)pdVar7 + 4),fVar29);
              dVar24 = (double)CONCAT44(*(undefined4 *)(iVar8 + 4),*(undefined4 *)(iVar8 + 0x14));
            }
            else {
              dVar27 = *(double *)((int)pdVar7 + 4);
              in_xd2 = *(double *)(iVar8 + 0x14);
              dVar24 = *(double *)(iVar8 + 4);
            }
            if (in_FPSCR_PR == '\0') {
              dVar24 = (double)CONCAT44((int)((ulonglong)dVar24 >> 0x20),
                                        SUB84(dVar24,0) * (float)((ulonglong)dVar27 >> 0x20));
            }
            else {
              dVar24 = dVar24 * dVar27;
            }
            if (in_FPSCR_SZ == '\0') {
              dVar27 = (double)CONCAT44(*(undefined4 *)pdVar7,SUB84(dVar27,0));
            }
            else {
              dVar27 = *pdVar7;
            }
            if (in_FPSCR_PR == '\0') {
              dVar24 = (double)(ulonglong)
                               (uint)((float)((ulonglong)dVar27 >> 0x20) *
                                      (float)((ulonglong)dVar24 >> 0x20) + SUB84(dVar24,0));
            }
            if (in_FPSCR_SZ == '\0') {
              dVar27 = (double)CONCAT44(*(undefined4 *)(pdVar7 + 1),SUB84(dVar27,0));
              uVar25 = CONCAT44(*(undefined4 *)(iVar8 + 0x24),SUB84(dVar24,0));
            }
            else {
              dVar27 = pdVar7[1];
              uVar25 = *(ulonglong *)(iVar8 + 0x24);
            }
            if (in_FPSCR_PR == '\0') {
              uVar25 = (ulonglong)
                       (uint)((float)((ulonglong)dVar27 >> 0x20) * (float)(uVar25 >> 0x20) +
                             (float)uVar25);
            }
            dVar20 = dVar30;
            dVar24 = dVar30;
            if (in_FPSCR_SZ == '\0') {
              dVar20 = (double)CONCAT44(uVar31,(int)uVar25);
              dVar24 = (double)CONCAT44(uVar31,SUB84(dVar27,0));
            }
            if (in_FPSCR_PR == '\0') {
              dVar20 = (double)CONCAT44((float)((ulonglong)dVar24 >> 0x20) * SUB84(dVar20,0) +
                                        (float)((ulonglong)dVar20 >> 0x20),SUB84(dVar20,0));
            }
            if (in_FPSCR_SZ == '\0') {
              pdVar9 = (double *)((int)pdVar9 + -4);
              *(int *)pdVar9 = (int)((ulonglong)dVar20 >> 0x20);
            }
            else {
              pdVar9 = pdVar9 + -1;
              *pdVar9 = dVar20;
            }
            uVar21 = DAT_8c61a478;
            pdVar10 = (double *)*piVar2;
            if (in_FPSCR_SZ == '\0') {
              dVar27 = (double)CONCAT44(*(undefined4 *)((int)pdVar7 + 4),SUB84(dVar24,0));
              dVar24 = (double)CONCAT44(*(undefined4 *)pdVar10,*(undefined4 *)(pdVar10 + 2));
            }
            else {
              dVar27 = *(double *)((int)pdVar7 + 4);
              in_xd2 = pdVar10[2];
              dVar24 = *pdVar10;
            }
            if (in_FPSCR_PR == '\0') {
              dVar24 = (double)CONCAT44((int)((ulonglong)dVar24 >> 0x20),
                                        SUB84(dVar24,0) * (float)((ulonglong)dVar27 >> 0x20));
            }
            else {
              dVar24 = dVar24 * dVar27;
            }
            if (in_FPSCR_SZ == '\0') {
              dVar27 = (double)CONCAT44(*(undefined4 *)pdVar7,SUB84(dVar27,0));
            }
            else {
              dVar27 = *pdVar7;
            }
            if (in_FPSCR_PR == '\0') {
              dVar24 = (double)(ulonglong)
                               (uint)((float)((ulonglong)dVar27 >> 0x20) *
                                      (float)((ulonglong)dVar24 >> 0x20) + SUB84(dVar24,0));
            }
            if (in_FPSCR_SZ == '\0') {
              dVar27 = (double)CONCAT44(*(undefined4 *)(pdVar7 + 1),SUB84(dVar27,0));
              dVar24 = (double)CONCAT44(*(undefined4 *)(pdVar10 + 4),SUB84(dVar24,0));
            }
            else {
              dVar27 = pdVar7[1];
              dVar24 = pdVar10[4];
            }
            if (in_FPSCR_PR == '\0') {
              dVar24 = (double)(ulonglong)
                               (uint)((float)((ulonglong)dVar27 >> 0x20) *
                                      (float)((ulonglong)dVar24 >> 0x20) + SUB84(dVar24,0));
            }
            dVar20 = dVar30;
            in_dr0 = dVar30;
            if (in_FPSCR_SZ == '\0') {
              dVar20 = (double)CONCAT44(uVar31,SUB84(dVar24,0));
              in_dr0 = (double)CONCAT44(uVar31,SUB84(dVar27,0));
            }
            if (in_FPSCR_PR == '\0') {
              dVar20 = (double)CONCAT44((float)((ulonglong)in_dr0 >> 0x20) * SUB84(dVar20,0) +
                                        (float)((ulonglong)dVar20 >> 0x20),SUB84(dVar20,0));
            }
            if (in_FPSCR_SZ == '\0') {
              *(int *)((int)pdVar9 + -4) = (int)((ulonglong)dVar20 >> 0x20);
              *(undefined4 *)(pdVar9 + -1) = *(undefined4 *)(pdVar7 + 3);
              *(undefined4 *)((int)pdVar9 + -0xc) = *(undefined4 *)((int)pdVar7 + 0x14);
              pdVar9 = pdVar9 + -2;
              *(undefined4 *)pdVar9 = *(undefined4 *)(pdVar7 + 2);
            }
            else {
              pdVar9[-1] = dVar20;
              pdVar9[-2] = pdVar7[3];
              pdVar9[-3] = *(double *)((int)pdVar7 + 0x14);
              in_xd2 = pdVar7[2];
              pdVar9 = pdVar9 + -4;
              *pdVar9 = in_xd2;
            }
            *(undefined4 *)((int)pdVar9 + -4) = uVar21;
            iVar14 = iVar14 + 1;
            unaff_r8 = (int)pdVar9 + 0x1c;
          } while (iVar14 < (int)(uVar5 - 1));
        }
        puVar15 = puVar18 + 1;
        pdVar7 = (double *)((uint)*puVar18 * 0x20 + *DAT_8c61a468);
        if (in_FPSCR_SZ == '\0') {
          dVar27 = (double)CONCAT44(*(undefined4 *)DAT_8c61a46c,*(undefined4 *)pdVar7);
          in_dr0 = (double)CONCAT44((int)((ulonglong)in_dr0 >> 0x20),*DAT_8c61a474);
        }
        else {
          in_xd2 = *pdVar7;
          dVar27 = *DAT_8c61a46c;
        }
        if (in_FPSCR_PR == '\0') {
          dVar27 = (double)CONCAT44((float)((ulonglong)dVar27 >> 0x20) * SUB84(dVar27,0),
                                    SUB84(dVar27,0));
        }
        else {
          dVar27 = dVar27 * dVar27;
        }
        if (in_FPSCR_SZ == '\0') {
          dVar27 = (double)CONCAT44((int)((ulonglong)dVar27 >> 0x20),*(undefined4 *)DAT_8c61a470);
        }
        else {
          in_xd2 = *DAT_8c61a470;
        }
        if (in_FPSCR_PR == '\0') {
          fVar29 = SUB84(dVar27,0) - (float)((ulonglong)dVar27 >> 0x20);
        }
        else {
          fVar29 = SUB84(dVar27 - dVar27,0);
        }
        if (in_FPSCR_SZ == '\0') {
          dVar27 = (double)CONCAT44(*(undefined4 *)((int)pdVar7 + 4),fVar29);
        }
        else {
          dVar27 = *(double *)((int)pdVar7 + 4);
        }
        if (in_FPSCR_PR == '\0') {
          dVar24 = (double)CONCAT44((int)((ulonglong)in_dr0 >> 0x20),
                                    SUB84(in_dr0,0) * (float)((ulonglong)dVar27 >> 0x20));
        }
        else {
          dVar24 = in_dr0 * dVar27;
        }
        if (in_FPSCR_SZ == '\0') {
          dVar27 = (double)CONCAT44(*(undefined4 *)(pdVar7 + 1),SUB84(dVar27,0));
        }
        else {
          dVar27 = pdVar7[1];
        }
        if (in_FPSCR_PR == '\0') {
          dVar27 = (double)CONCAT44((int)((ulonglong)dVar27 >> 0x20),
                                    SUB84(dVar27,0) - SUB84(dVar24,0));
        }
        else {
          dVar27 = dVar27 - dVar24;
        }
        if (in_FPSCR_SZ == '\0') {
          dVar24 = (double)CONCAT44((int)((ulonglong)dVar24 >> 0x20),
                                    *(undefined4 *)(DAT_8c61a46c + 1));
        }
        if (in_FPSCR_PR == '\0') {
          fVar29 = SUB84(dVar24,0) * (float)((ulonglong)dVar27 >> 0x20);
          fVar22 = SUB84(dVar27,0) - fVar29;
        }
        else {
          fVar29 = SUB84(dVar24 * dVar27,0);
          fVar22 = SUB84(dVar27 - dVar24 * dVar27,0);
        }
        if (in_FPSCR_SZ == '\0') {
          pdVar9 = (double *)(unaff_r8 + 0x18);
          *(float *)pdVar9 = fVar22;
        }
        else {
          pdVar9 = (double *)(unaff_r8 + 0x14);
          *pdVar9 = in_xd2;
        }
        iVar14 = *piVar2;
        if (in_FPSCR_SZ == '\0') {
          dVar27 = (double)CONCAT44(*(undefined4 *)((int)pdVar7 + 4),fVar29);
          dVar24 = (double)CONCAT44(*(undefined4 *)(iVar14 + 4),*(undefined4 *)(iVar14 + 0x14));
        }
        else {
          dVar27 = *(double *)((int)pdVar7 + 4);
          in_xd2 = *(double *)(iVar14 + 0x14);
          dVar24 = *(double *)(iVar14 + 4);
        }
        if (in_FPSCR_PR == '\0') {
          dVar24 = (double)CONCAT44((int)((ulonglong)dVar24 >> 0x20),
                                    SUB84(dVar24,0) * (float)((ulonglong)dVar27 >> 0x20));
        }
        else {
          dVar24 = dVar24 * dVar27;
        }
        if (in_FPSCR_SZ == '\0') {
          dVar27 = (double)CONCAT44(*(undefined4 *)pdVar7,SUB84(dVar27,0));
        }
        else {
          dVar27 = *pdVar7;
        }
        if (in_FPSCR_PR == '\0') {
          dVar24 = (double)(ulonglong)
                           (uint)((float)((ulonglong)dVar27 >> 0x20) *
                                  (float)((ulonglong)dVar24 >> 0x20) + SUB84(dVar24,0));
        }
        if (in_FPSCR_SZ == '\0') {
          dVar27 = (double)CONCAT44(*(undefined4 *)(pdVar7 + 1),SUB84(dVar27,0));
          uVar25 = CONCAT44(*(undefined4 *)(iVar14 + 0x24),SUB84(dVar24,0));
        }
        else {
          dVar27 = pdVar7[1];
          uVar25 = *(ulonglong *)(iVar14 + 0x24);
        }
        if (in_FPSCR_PR == '\0') {
          uVar25 = (ulonglong)
                   (uint)((float)((ulonglong)dVar27 >> 0x20) * (float)(uVar25 >> 0x20) +
                         (float)uVar25);
        }
        dVar20 = dVar30;
        dVar24 = dVar30;
        if (in_FPSCR_SZ == '\0') {
          dVar20 = (double)CONCAT44(uVar31,(int)uVar25);
          dVar24 = (double)CONCAT44(uVar31,SUB84(dVar27,0));
        }
        if (in_FPSCR_PR == '\0') {
          dVar20 = (double)CONCAT44((float)((ulonglong)dVar24 >> 0x20) * SUB84(dVar20,0) +
                                    (float)((ulonglong)dVar20 >> 0x20),SUB84(dVar20,0));
        }
        if (in_FPSCR_SZ == '\0') {
          pdVar9 = (double *)((int)pdVar9 + -4);
          *(int *)pdVar9 = (int)((ulonglong)dVar20 >> 0x20);
        }
        else {
          pdVar9 = pdVar9 + -1;
          *pdVar9 = dVar20;
        }
        uVar21 = DAT_8c61a47c;
        pdVar10 = (double *)*piVar2;
        if (in_FPSCR_SZ == '\0') {
          dVar27 = (double)CONCAT44(*(undefined4 *)((int)pdVar7 + 4),SUB84(dVar24,0));
          dVar24 = (double)CONCAT44(*(undefined4 *)pdVar10,*(undefined4 *)(pdVar10 + 2));
        }
        else {
          dVar27 = *(double *)((int)pdVar7 + 4);
          in_xd2 = pdVar10[2];
          dVar24 = *pdVar10;
        }
        if (in_FPSCR_PR == '\0') {
          dVar24 = (double)CONCAT44((int)((ulonglong)dVar24 >> 0x20),
                                    SUB84(dVar24,0) * (float)((ulonglong)dVar27 >> 0x20));
        }
        else {
          dVar24 = dVar24 * dVar27;
        }
        if (in_FPSCR_SZ == '\0') {
          dVar27 = (double)CONCAT44(*(undefined4 *)pdVar7,SUB84(dVar27,0));
        }
        else {
          dVar27 = *pdVar7;
        }
        if (in_FPSCR_PR == '\0') {
          dVar24 = (double)(ulonglong)
                           (uint)((float)((ulonglong)dVar27 >> 0x20) *
                                  (float)((ulonglong)dVar24 >> 0x20) + SUB84(dVar24,0));
        }
        if (in_FPSCR_SZ == '\0') {
          dVar27 = (double)CONCAT44(*(undefined4 *)(pdVar7 + 1),SUB84(dVar27,0));
          dVar24 = (double)CONCAT44(*(undefined4 *)(pdVar10 + 4),SUB84(dVar24,0));
        }
        else {
          dVar27 = pdVar7[1];
          dVar24 = pdVar10[4];
        }
        if (in_FPSCR_PR == '\0') {
          dVar24 = (double)(ulonglong)
                           (uint)((float)((ulonglong)dVar27 >> 0x20) *
                                  (float)((ulonglong)dVar24 >> 0x20) + SUB84(dVar24,0));
        }
        dVar20 = dVar30;
        in_dr0 = dVar30;
        if (in_FPSCR_SZ == '\0') {
          dVar20 = (double)CONCAT44(uVar31,SUB84(dVar24,0));
          in_dr0 = (double)CONCAT44(uVar31,SUB84(dVar27,0));
        }
        if (in_FPSCR_PR == '\0') {
          dVar20 = (double)CONCAT44((float)((ulonglong)in_dr0 >> 0x20) * SUB84(dVar20,0) +
                                    (float)((ulonglong)dVar20 >> 0x20),SUB84(dVar20,0));
        }
        if (in_FPSCR_SZ == '\0') {
          *(int *)((int)pdVar9 + -4) = (int)((ulonglong)dVar20 >> 0x20);
          *(undefined4 *)(pdVar9 + -1) = *(undefined4 *)(pdVar7 + 3);
          *(undefined4 *)((int)pdVar9 + -0xc) = *(undefined4 *)((int)pdVar7 + 0x14);
          pdVar9 = pdVar9 + -2;
          *(undefined4 *)pdVar9 = *(undefined4 *)(pdVar7 + 2);
        }
        else {
          pdVar9[-1] = dVar20;
          pdVar9[-2] = pdVar7[3];
          pdVar9[-3] = *(double *)((int)pdVar7 + 0x14);
          in_xd2 = pdVar7[2];
          pdVar9 = pdVar9 + -4;
          *pdVar9 = in_xd2;
        }
        *(undefined4 *)((int)pdVar9 + -4) = uVar21;
        unaff_r8 = (int)pdVar9 + 0x1c;
      }
    }
  }
  else if (uVar5 == DAT_8c619ca0) {
    *DAT_8c61a480 = param_5;
    fVar29 = DAT_8c61a45c;
    if (in_FPSCR_SZ == '\0') {
      fVar29 = DAT_8c61a458;
    }
    dVar27 = (double)CONCAT44(fVar29,DAT_8c61a458);
    if (in_FPSCR_PR == '\0') {
      dVar24 = (double)CONCAT44((float)((ulonglong)param_1 >> 0x20) * fVar29,
                                SUB84(param_1,0) * fVar29);
      dVar27 = (double)CONCAT44((float)((ulonglong)param_3 >> 0x20) * fVar29,
                                SUB84(param_3,0) * fVar29);
    }
    else {
      dVar24 = param_1 * dVar27 * dVar27;
      dVar27 = param_3 * dVar27 * dVar27;
    }
    if (in_FPSCR_SZ == '\0') {
      dVar30 = (double)CONCAT44(*(undefined4 *)((int)DAT_8c61a45c + 4),SUB84(dVar30,0));
      dVar20 = (double)CONCAT44(*(undefined4 *)((int)DAT_8c61a45c + 8),param_4);
    }
    else {
      dVar30 = *(double *)((int)DAT_8c61a45c + 8);
      dVar20 = *(double *)((int)DAT_8c61a45c + 0x10);
    }
    if (in_FPSCR_PR == '\0') {
      dVar30 = (double)CONCAT44(-(float)((ulonglong)dVar30 >> 0x20),SUB84(dVar30,0));
      dVar20 = (double)CONCAT44(-(float)((ulonglong)dVar20 >> 0x20),SUB84(dVar20,0));
    }
    else {
      dVar30 = -dVar30;
      dVar20 = -dVar20;
    }
    (*(code *)PTR_FUN_8c61a484)(dVar24,dVar30,dVar27,dVar20,*DAT_8c61a480);
  }
  return;
}

