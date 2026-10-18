/* Ghidra draft only; not buildable/verified source. */

void FUN_8c61ae3c(double param_1,undefined4 param_2,ushort *param_3)

{
  ushort uVar1;
  int *piVar2;
  uint uVar3;
  int iVar4;
  int iVar5;
  undefined8 *unaff_r8;
  double *pdVar6;
  int iVar7;
  int iVar8;
  ushort *puVar9;
  ushort *puVar10;
  ushort *puVar11;
  ushort *puVar12;
  double *pdVar13;
  double *pdVar14;
  double *pdVar15;
  double in_dr0;
  double dVar16;
  undefined4 uVar17;
  float fVar18;
  undefined4 uVar19;
  double in_dr2;
  double dVar20;
  uint uVar21;
  double dVar22;
  undefined4 uVar24;
  float fVar25;
  undefined8 uVar23;
  undefined8 uVar26;
  undefined8 uVar27;
  double in_xd2;
  double in_xd4;
  bool bVar28;
  undefined4 in_PR;
  char in_FPSCR_PR;
  char in_FPSCR_SZ;
  undefined4 uStack_38;
  undefined4 uStack_34;
  uint uStack_30;
  float fStack_2c;
  undefined4 uStack_28;
  undefined4 uStack_24;
  undefined4 uStack_20;
  undefined4 local_1c;
  
  piVar2 = DAT_8c61aea0;
  puVar9 = *(ushort **)(param_3 + 2);
  uVar3 = *param_3 & DAT_8c61aea4;
  local_1c = in_PR;
  if (uVar3 == 0) {
    *DAT_8c61aea0 = *DAT_8c61aea0 + (uint)param_3[1] * 3;
    for (iVar8 = 0; uVar17 = SUB84(param_1,0), iVar8 < (int)(uint)param_3[1]; iVar8 = iVar8 + 1) {
      uVar1 = *puVar9;
      puVar12 = puVar9 + 1;
      puVar10 = puVar9 + 2;
      iVar5 = *DAT_8c61b1bc;
      puVar9 = puVar9 + 3;
      pdVar14 = (double *)((uint)*puVar12 * 0x20 + iVar5);
      if (in_FPSCR_SZ == '\0') {
        uVar17 = *(undefined4 *)(pdVar14 + 2);
      }
      else {
        in_xd4 = pdVar14[2];
      }
      pdVar15 = (double *)((uint)uVar1 * 0x20 + iVar5);
      if (in_FPSCR_SZ == '\0') {
        dVar22 = (double)CONCAT44(uVar17,*(undefined4 *)(pdVar15 + 2));
      }
      else {
        in_xd2 = pdVar15[2];
        dVar22 = in_xd4;
      }
      if (in_FPSCR_PR == '\0') {
        dVar22 = (double)CONCAT44((float)((ulonglong)dVar22 >> 0x20) - SUB84(dVar22,0),
                                  SUB84(dVar22,0));
      }
      else {
        dVar22 = dVar22 - dVar22;
      }
      pdVar13 = (double *)((uint)*puVar10 * 0x20 + iVar5);
      if (in_FPSCR_SZ == '\0') {
        fVar25 = *(float *)((int)pdVar14 + 0x14);
        dVar20 = (double)CONCAT44(fVar25,uVar17);
        dVar22 = (double)CONCAT44((int)((ulonglong)dVar22 >> 0x20),
                                  *(undefined4 *)((int)pdVar13 + 0x14));
        in_dr0 = (double)CONCAT44((int)((ulonglong)in_dr0 >> 0x20),fVar25);
      }
      else {
        dVar20 = *(double *)((int)pdVar14 + 0x14);
        in_xd2 = *(double *)((int)pdVar13 + 0x14);
        fVar25 = (float)((ulonglong)dVar20 >> 0x20);
      }
      if (in_FPSCR_PR == '\0') {
        fVar25 = SUB84(dVar22,0) - fVar25;
        param_1 = (double)CONCAT44((float)((ulonglong)dVar22 >> 0x20) * fVar25,fVar25);
      }
      else {
        param_1 = (dVar22 - dVar20) * (dVar22 - dVar20);
      }
      if (in_FPSCR_SZ == '\0') {
        param_1 = (double)CONCAT44((int)((ulonglong)param_1 >> 0x20),
                                   *(undefined4 *)((int)pdVar15 + 0x14));
      }
      else {
        in_xd2 = *(double *)((int)pdVar15 + 0x14);
      }
      if (in_FPSCR_PR == '\0') {
        dVar22 = (double)CONCAT44((int)((ulonglong)in_dr0 >> 0x20),
                                  SUB84(in_dr0,0) - SUB84(param_1,0));
      }
      else {
        dVar22 = in_dr0 - param_1;
      }
      uVar17 = (undefined4)((ulonglong)param_1 >> 0x20);
      if (in_FPSCR_SZ == '\0') {
        param_1 = (double)CONCAT44(uVar17,SUB84(dVar20,0));
        dVar20 = (double)CONCAT44(uVar17,*(undefined4 *)(pdVar13 + 2));
      }
      else {
        in_xd2 = pdVar13[2];
        dVar20 = param_1;
      }
      if (in_FPSCR_PR == '\0') {
        fVar25 = SUB84(dVar20,0) - SUB84(param_1,0);
        dVar20 = (double)CONCAT44(uVar17,fVar25);
        in_dr0 = (double)CONCAT44((int)((ulonglong)dVar22 >> 0x20),SUB84(dVar22,0) * fVar25);
      }
      else {
        dVar20 = dVar20 - param_1;
        in_dr0 = dVar22 * dVar20;
      }
      if (in_FPSCR_PR == '\0') {
        fVar25 = (float)((ulonglong)param_1 >> 0x20) - SUB84(in_dr0,0);
        param_1 = (double)CONCAT44(fVar25,SUB84(param_1,0));
        bVar28 = 0.0 < fVar25;
      }
      else {
        param_1 = param_1 - in_dr0;
        bVar28 = (double)((ulonglong)dVar20 & 0xffffffff00000000) < param_1;
      }
      uVar17 = SUB84(param_1,0);
      if (!bVar28) {
        if (in_FPSCR_SZ == '\0') {
          dVar22 = (double)CONCAT44(*(undefined4 *)pdVar15,*(undefined4 *)pdVar14);
        }
        else {
          dVar22 = *pdVar15;
          in_xd2 = *pdVar14;
        }
        if (in_FPSCR_PR == '\0') {
          dVar22 = (double)CONCAT44((float)((ulonglong)dVar22 >> 0x20) + SUB84(dVar22,0),
                                    SUB84(dVar22,0));
        }
        else {
          dVar22 = dVar22 + dVar22;
        }
        if (in_FPSCR_SZ == '\0') {
          dVar22 = (double)CONCAT44((int)((ulonglong)dVar22 >> 0x20),*(undefined4 *)pdVar13);
        }
        else {
          in_xd2 = *pdVar13;
        }
        if (in_FPSCR_PR == '\0') {
          dVar22 = (double)CONCAT44((float)((ulonglong)dVar22 >> 0x20) + SUB84(dVar22,0),
                                    SUB84(dVar22,0));
        }
        else {
          dVar22 = dVar22 + dVar22;
        }
        if (in_FPSCR_SZ == '\0') {
          uStack_28 = (undefined4)((ulonglong)dVar22 >> 0x20);
          dVar22 = (double)CONCAT44(*(undefined4 *)((int)pdVar15 + 4),
                                    *(undefined4 *)((int)pdVar14 + 4));
        }
        else {
          uStack_28 = SUB84(dVar22,0);
          dVar22 = *(double *)((int)pdVar15 + 4);
          in_xd2 = *(double *)((int)pdVar14 + 4);
        }
        if (in_FPSCR_PR == '\0') {
          dVar22 = (double)CONCAT44((float)((ulonglong)dVar22 >> 0x20) + SUB84(dVar22,0),
                                    SUB84(dVar22,0));
        }
        else {
          dVar22 = dVar22 + dVar22;
        }
        if (in_FPSCR_SZ == '\0') {
          dVar22 = (double)CONCAT44((int)((ulonglong)dVar22 >> 0x20),
                                    *(undefined4 *)((int)pdVar13 + 4));
        }
        else {
          in_xd2 = *(double *)((int)pdVar13 + 4);
        }
        if (in_FPSCR_PR == '\0') {
          dVar22 = (double)CONCAT44((float)((ulonglong)dVar22 >> 0x20) + SUB84(dVar22,0),
                                    SUB84(dVar22,0));
        }
        else {
          dVar22 = dVar22 + dVar22;
        }
        if (in_FPSCR_SZ == '\0') {
          uStack_24 = (undefined4)((ulonglong)dVar22 >> 0x20);
          dVar22 = (double)CONCAT44(*(undefined4 *)(pdVar15 + 1),*(undefined4 *)(pdVar14 + 1));
        }
        else {
          uStack_24 = SUB84(dVar22,0);
          dVar22 = pdVar15[1];
          in_xd2 = pdVar14[1];
        }
        if (in_FPSCR_PR == '\0') {
          dVar22 = (double)CONCAT44((float)((ulonglong)dVar22 >> 0x20) + SUB84(dVar22,0),
                                    SUB84(dVar22,0));
        }
        else {
          dVar22 = dVar22 + dVar22;
        }
        if (in_FPSCR_SZ == '\0') {
          dVar22 = (double)CONCAT44((int)((ulonglong)dVar22 >> 0x20),*(undefined4 *)(pdVar13 + 1));
        }
        else {
          in_xd2 = pdVar13[1];
        }
        if (in_FPSCR_PR == '\0') {
          dVar22 = (double)CONCAT44((float)((ulonglong)dVar22 >> 0x20) + SUB84(dVar22,0),
                                    SUB84(dVar22,0));
        }
        else {
          dVar22 = dVar22 + dVar22;
        }
        uVar24 = (undefined4)((ulonglong)dVar22 >> 0x20);
        uStack_20 = uVar24;
        if (in_FPSCR_SZ != '\0') {
          uStack_20 = SUB84(dVar22,0);
          local_1c = uVar24;
        }
        dVar22 = (double)(*(code *)PTR_FUN_8c61b1c0)(&uStack_28);
        uVar24 = DAT_8c61b1dc;
        if (in_FPSCR_SZ == '\0') {
          dVar20 = (double)CONCAT44(*(undefined4 *)DAT_8c61b1c4,uStack_28);
        }
        else {
          in_xd2 = (double)CONCAT44(uStack_24,uStack_28);
          dVar20 = *DAT_8c61b1c4;
        }
        if (in_FPSCR_PR == '\0') {
          dVar20 = (double)CONCAT44((float)((ulonglong)dVar20 >> 0x20) * SUB84(dVar20,0),
                                    SUB84(dVar20,0));
        }
        else {
          dVar20 = dVar20 * dVar20;
        }
        if (in_FPSCR_SZ == '\0') {
          dVar20 = (double)CONCAT44((int)((ulonglong)dVar20 >> 0x20),*(undefined4 *)DAT_8c61b1c8);
          dVar22 = (double)CONCAT44((int)((ulonglong)dVar22 >> 0x20),*DAT_8c61b1cc);
        }
        else {
          in_xd2 = *DAT_8c61b1c8;
        }
        if (in_FPSCR_PR == '\0') {
          fVar25 = SUB84(dVar20,0) - (float)((ulonglong)dVar20 >> 0x20);
        }
        else {
          fVar25 = SUB84(dVar20 - dVar20,0);
        }
        if (in_FPSCR_SZ == '\0') {
          dVar20 = (double)CONCAT44(uStack_24,fVar25);
        }
        else {
          dVar20 = (double)CONCAT44(uStack_20,uStack_24);
        }
        if (in_FPSCR_PR == '\0') {
          dVar22 = (double)CONCAT44((int)((ulonglong)dVar22 >> 0x20),
                                    SUB84(dVar22,0) * (float)((ulonglong)dVar20 >> 0x20));
        }
        else {
          dVar22 = dVar22 * dVar20;
        }
        if (in_FPSCR_SZ == '\0') {
          dVar20 = (double)CONCAT44(uStack_20,SUB84(dVar20,0));
        }
        else {
          dVar20 = (double)CONCAT44(local_1c,uStack_20);
        }
        if (in_FPSCR_PR == '\0') {
          dVar20 = (double)CONCAT44((int)((ulonglong)dVar20 >> 0x20),
                                    SUB84(dVar20,0) - SUB84(dVar22,0));
        }
        else {
          dVar20 = dVar20 - dVar22;
        }
        if (in_FPSCR_SZ == '\0') {
          dVar22 = (double)CONCAT44((int)((ulonglong)dVar22 >> 0x20),
                                    *(undefined4 *)(DAT_8c61b1c4 + 1));
        }
        fVar25 = (float)((ulonglong)dVar20 >> 0x20);
        if (in_FPSCR_PR == '\0') {
          in_dr0 = (double)CONCAT44((int)((ulonglong)dVar22 >> 0x20),SUB84(dVar22,0) * fVar25);
        }
        else {
          in_dr0 = dVar22 * dVar20;
        }
        if (in_FPSCR_SZ == '\0') {
          uVar19 = SUB84(dVar20,0);
          dVar20 = (double)CONCAT44(fVar25,*(undefined4 *)DAT_8c61b1d0);
          dVar22 = in_xd2;
          in_xd2 = (double)CONCAT44(uVar19,uVar17);
        }
        else {
          dVar22 = *DAT_8c61b1d0;
        }
        if (in_FPSCR_PR == '\0') {
          param_1 = (double)CONCAT44((float)((ulonglong)in_xd2 >> 0x20) - SUB84(in_dr0,0),
                                     SUB84(in_xd2,0));
        }
        else {
          param_1 = in_xd2 - in_dr0;
        }
        if (in_FPSCR_SZ == '\0') {
          uStack_38 = SUB84(dVar20,0);
          dVar20 = (double)CONCAT44((int)((ulonglong)dVar20 >> 0x20),*(undefined4 *)DAT_8c61b1d4);
        }
        else {
          uStack_38 = SUB84(dVar22,0);
          dVar22 = *DAT_8c61b1d4;
        }
        fStack_2c = (float)((ulonglong)param_1 >> 0x20);
        if (in_FPSCR_PR == '\0') {
          dVar20 = (double)CONCAT44((int)((ulonglong)dVar20 >> 0x20),SUB84(dVar20,0) * fStack_2c);
        }
        else {
          dVar20 = dVar20 * param_1;
        }
        if (in_FPSCR_SZ == '\0') {
          uStack_34 = SUB84(dVar20,0);
          dVar20 = (double)CONCAT44((int)((ulonglong)dVar20 >> 0x20),*(undefined4 *)DAT_8c61b1d8);
        }
        else {
          uStack_34 = SUB84(dVar22,0);
          dVar22 = *DAT_8c61b1d8;
        }
        if (in_FPSCR_PR == '\0') {
          dVar20 = (double)CONCAT44((int)((ulonglong)dVar20 >> 0x20),SUB84(dVar20,0) * fStack_2c);
        }
        else {
          dVar20 = dVar20 * param_1;
        }
        if (in_FPSCR_SZ == '\0') {
          uStack_30 = SUB84(dVar20,0);
          dVar20 = (double)CONCAT44((int)((ulonglong)dVar20 >> 0x20),
                                    *(undefined4 *)((int)DAT_8c61b1d0 + 0xc));
        }
        else {
          uStack_30 = SUB84(dVar22,0);
          dVar22 = *(double *)((int)DAT_8c61b1d0 + 0xc);
        }
        if (in_FPSCR_PR == '\0') {
          fStack_2c = SUB84(dVar20,0) * fStack_2c;
        }
        else {
          fStack_2c = SUB84(dVar20 * param_1,0);
        }
        if (in_FPSCR_SZ == '\0') {
          *(undefined4 *)((int)unaff_r8 + 0xc) = *(undefined4 *)(pdVar15 + 3);
          *(undefined4 *)(unaff_r8 + 1) = *(undefined4 *)((int)pdVar15 + 0x14);
          pdVar6 = (double *)((int)unaff_r8 + 4);
          *(undefined4 *)pdVar6 = *(undefined4 *)(pdVar15 + 2);
        }
        else {
          fStack_2c = SUB84(dVar22,0);
          uStack_28 = (undefined4)((ulonglong)dVar22 >> 0x20);
          unaff_r8[1] = pdVar15[3];
          *unaff_r8 = *(undefined8 *)((int)pdVar15 + 0x14);
          dVar22 = pdVar15[2];
          pdVar6 = (double *)(unaff_r8 + -1);
          *pdVar6 = dVar22;
        }
        *(undefined4 *)((int)pdVar6 + -4) = uVar24;
        if (in_FPSCR_SZ == '\0') {
          *(undefined4 *)(pdVar6 + 5) = *(undefined4 *)(pdVar14 + 3);
          *(undefined4 *)((int)pdVar6 + 0x24) = *(undefined4 *)((int)pdVar14 + 0x14);
          pdVar6 = pdVar6 + 4;
          *(undefined4 *)pdVar6 = *(undefined4 *)(pdVar14 + 2);
        }
        else {
          *(double *)((int)pdVar6 + 0x24) = pdVar14[3];
          *(undefined8 *)((int)pdVar6 + 0x1c) = *(undefined8 *)((int)pdVar14 + 0x14);
          dVar22 = pdVar14[2];
          pdVar6 = (double *)((int)pdVar6 + 0x14);
          *pdVar6 = dVar22;
        }
        *(undefined4 *)((int)pdVar6 + -4) = uVar24;
        uVar17 = DAT_8c61b1e0;
        if (in_FPSCR_SZ == '\0') {
          *(float *)(pdVar6 + 7) = fStack_2c;
          *(uint *)((int)pdVar6 + 0x34) = uStack_30;
          *(undefined4 *)(pdVar6 + 6) = uStack_34;
          *(undefined4 *)((int)pdVar6 + 0x2c) = uStack_38;
          *(undefined4 *)(pdVar6 + 5) = *(undefined4 *)(pdVar13 + 3);
          *(undefined4 *)((int)pdVar6 + 0x24) = *(undefined4 *)((int)pdVar13 + 0x14);
          pdVar6 = pdVar6 + 4;
          *(undefined4 *)pdVar6 = *(undefined4 *)(pdVar13 + 2);
          in_xd2 = dVar22;
        }
        else {
          *(ulonglong *)((int)pdVar6 + 0x34) = CONCAT44(uStack_28,fStack_2c);
          *(ulonglong *)((int)pdVar6 + 0x2c) = CONCAT44(fStack_2c,uStack_30);
          *(ulonglong *)((int)pdVar6 + 0x24) = CONCAT44(uStack_30,uStack_34);
          *(ulonglong *)((int)pdVar6 + 0x1c) = CONCAT44(uStack_34,uStack_38);
          *(double *)((int)pdVar6 + 0x14) = pdVar13[3];
          *(undefined8 *)((int)pdVar6 + 0xc) = *(undefined8 *)((int)pdVar13 + 0x14);
          in_xd2 = pdVar13[2];
          pdVar6 = (double *)((int)pdVar6 + 4);
          *pdVar6 = in_xd2;
        }
        *(undefined4 *)((int)pdVar6 + -4) = uVar17;
        unaff_r8 = (undefined8 *)((int)pdVar6 + 0x1c);
        *DAT_8c61b1e4 = *DAT_8c61b1e4 + 3;
      }
    }
  }
  else if (uVar3 == (int)DAT_8c61ae9c) {
    *DAT_8c61aea0 = *DAT_8c61aea0 + (uint)param_3[1] * 4;
    for (iVar8 = 0; uVar17 = SUB84(in_dr2,0), iVar8 < (int)(uint)param_3[1]; iVar8 = iVar8 + 1) {
      uVar1 = *puVar9;
      puVar12 = puVar9 + 1;
      puVar10 = puVar9 + 2;
      iVar5 = *DAT_8c61b1bc;
      puVar11 = puVar9 + 3;
      puVar9 = puVar9 + 4;
      pdVar14 = (double *)((uint)*puVar12 * 0x20 + iVar5);
      pdVar15 = (double *)((uint)uVar1 * 0x20 + iVar5);
      if (in_FPSCR_SZ == '\0') {
        dVar22 = (double)CONCAT44(*(undefined4 *)(pdVar14 + 2),SUB84(param_1,0));
        uVar17 = *(undefined4 *)(pdVar15 + 2);
      }
      else {
        dVar22 = pdVar14[2];
        in_xd2 = pdVar15[2];
      }
      uVar24 = (undefined4)((ulonglong)dVar22 >> 0x20);
      dVar20 = dVar22;
      if (in_FPSCR_SZ == '\0') {
        dVar20 = (double)CONCAT44(uVar24,uVar17);
      }
      if (in_FPSCR_PR == '\0') {
        dVar20 = (double)CONCAT44((float)((ulonglong)dVar20 >> 0x20) - SUB84(dVar20,0),
                                  SUB84(dVar20,0));
      }
      else {
        dVar20 = dVar20 - dVar20;
      }
      if (in_FPSCR_SZ == '\0') {
        dVar22 = (double)CONCAT44(uVar24,*(undefined4 *)((int)pdVar14 + 0x14));
      }
      pdVar13 = (double *)((uint)*puVar10 * 0x20 + iVar5);
      fVar25 = SUB84(dVar22,0);
      if (in_FPSCR_SZ == '\0') {
        dVar20 = (double)CONCAT44((int)((ulonglong)dVar20 >> 0x20),
                                  *(undefined4 *)((int)pdVar13 + 0x14));
        in_dr0 = (double)CONCAT44((int)((ulonglong)in_dr0 >> 0x20),fVar25);
      }
      else {
        in_xd2 = *(double *)((int)pdVar13 + 0x14);
      }
      if (in_FPSCR_PR == '\0') {
        fVar18 = SUB84(dVar20,0) - fVar25;
        dVar20 = (double)CONCAT44((float)((ulonglong)dVar20 >> 0x20) * fVar18,fVar18);
      }
      else {
        dVar20 = (dVar20 - dVar22) * (dVar20 - dVar22);
      }
      if (in_FPSCR_SZ == '\0') {
        dVar20 = (double)CONCAT44((int)((ulonglong)dVar20 >> 0x20),
                                  *(undefined4 *)((int)pdVar15 + 0x14));
      }
      else {
        in_xd2 = *(double *)((int)pdVar15 + 0x14);
      }
      if (in_FPSCR_PR == '\0') {
        dVar16 = (double)CONCAT44((int)((ulonglong)in_dr0 >> 0x20),SUB84(in_dr0,0) - SUB84(dVar20,0)
                                 );
      }
      else {
        dVar16 = in_dr0 - dVar20;
      }
      if (in_FPSCR_SZ == '\0') {
        dVar20 = (double)CONCAT44((int)((ulonglong)dVar20 >> 0x20),*(undefined4 *)(pdVar13 + 2));
      }
      else {
        in_xd2 = pdVar13[2];
      }
      if (in_FPSCR_PR == '\0') {
        dVar20 = (double)CONCAT44((int)((ulonglong)dVar20 >> 0x20),
                                  SUB84(dVar20,0) - (float)((ulonglong)dVar22 >> 0x20));
      }
      else {
        dVar20 = dVar20 - dVar22;
      }
      uVar3 = (uint)((ulonglong)dVar20 >> 0x20);
      param_1 = dVar20;
      if (in_FPSCR_SZ == '\0') {
        param_1 = (double)CONCAT44(uVar3,fVar25);
      }
      if (in_FPSCR_PR == '\0') {
        in_dr0 = (double)CONCAT44((int)((ulonglong)dVar16 >> 0x20),SUB84(dVar16,0) * SUB84(dVar20,0)
                                 );
      }
      else {
        in_dr0 = dVar16 * dVar20;
      }
      in_dr2 = (double)((ulonglong)uVar3 << 0x20);
      if (in_FPSCR_PR == '\0') {
        fVar25 = (float)((ulonglong)param_1 >> 0x20) - SUB84(in_dr0,0);
        param_1 = (double)CONCAT44(fVar25,SUB84(param_1,0));
        bVar28 = 0.0 < fVar25;
      }
      else {
        param_1 = param_1 - in_dr0;
        bVar28 = in_dr2 < param_1;
      }
      uVar17 = SUB84(param_1,0);
      iVar5 = (uint)*puVar11 * 0x20 + iVar5;
      if (!bVar28) {
        if (in_FPSCR_SZ == '\0') {
          dVar22 = (double)CONCAT44(*(undefined4 *)pdVar15,*(undefined4 *)pdVar14);
        }
        else {
          dVar22 = *pdVar15;
          in_xd2 = *pdVar14;
        }
        if (in_FPSCR_PR == '\0') {
          dVar22 = (double)CONCAT44((float)((ulonglong)dVar22 >> 0x20) + SUB84(dVar22,0),
                                    SUB84(dVar22,0));
        }
        else {
          dVar22 = dVar22 + dVar22;
        }
        if (in_FPSCR_SZ == '\0') {
          dVar22 = (double)CONCAT44((int)((ulonglong)dVar22 >> 0x20),*(undefined4 *)pdVar13);
        }
        else {
          in_xd2 = *pdVar13;
        }
        if (in_FPSCR_PR == '\0') {
          dVar22 = (double)CONCAT44((float)((ulonglong)dVar22 >> 0x20) + SUB84(dVar22,0),
                                    SUB84(dVar22,0));
        }
        else {
          dVar22 = dVar22 + dVar22;
        }
        if (in_FPSCR_SZ == '\0') {
          uStack_28 = (undefined4)((ulonglong)dVar22 >> 0x20);
          dVar22 = (double)CONCAT44(*(undefined4 *)((int)pdVar15 + 4),
                                    *(undefined4 *)((int)pdVar14 + 4));
        }
        else {
          uStack_28 = SUB84(dVar22,0);
          dVar22 = *(double *)((int)pdVar15 + 4);
          in_xd2 = *(double *)((int)pdVar14 + 4);
        }
        if (in_FPSCR_PR == '\0') {
          dVar22 = (double)CONCAT44((float)((ulonglong)dVar22 >> 0x20) + SUB84(dVar22,0),
                                    SUB84(dVar22,0));
        }
        else {
          dVar22 = dVar22 + dVar22;
        }
        if (in_FPSCR_SZ == '\0') {
          dVar22 = (double)CONCAT44((int)((ulonglong)dVar22 >> 0x20),
                                    *(undefined4 *)((int)pdVar13 + 4));
        }
        else {
          in_xd2 = *(double *)((int)pdVar13 + 4);
        }
        if (in_FPSCR_PR == '\0') {
          dVar22 = (double)CONCAT44((float)((ulonglong)dVar22 >> 0x20) + SUB84(dVar22,0),
                                    SUB84(dVar22,0));
        }
        else {
          dVar22 = dVar22 + dVar22;
        }
        if (in_FPSCR_SZ == '\0') {
          uStack_24 = (undefined4)((ulonglong)dVar22 >> 0x20);
          dVar22 = (double)CONCAT44(*(undefined4 *)(pdVar15 + 1),*(undefined4 *)(pdVar14 + 1));
        }
        else {
          uStack_24 = SUB84(dVar22,0);
          dVar22 = pdVar15[1];
          in_xd2 = pdVar14[1];
        }
        if (in_FPSCR_PR == '\0') {
          dVar22 = (double)CONCAT44((float)((ulonglong)dVar22 >> 0x20) + SUB84(dVar22,0),
                                    SUB84(dVar22,0));
        }
        else {
          dVar22 = dVar22 + dVar22;
        }
        if (in_FPSCR_SZ == '\0') {
          dVar22 = (double)CONCAT44((int)((ulonglong)dVar22 >> 0x20),*(undefined4 *)(pdVar13 + 1));
        }
        else {
          in_xd2 = pdVar13[1];
        }
        if (in_FPSCR_PR == '\0') {
          dVar22 = (double)CONCAT44((float)((ulonglong)dVar22 >> 0x20) + SUB84(dVar22,0),
                                    SUB84(dVar22,0));
        }
        else {
          dVar22 = dVar22 + dVar22;
        }
        uVar24 = (undefined4)((ulonglong)dVar22 >> 0x20);
        uStack_20 = uVar24;
        if (in_FPSCR_SZ != '\0') {
          uStack_20 = SUB84(dVar22,0);
          local_1c = uVar24;
        }
        dVar22 = (double)(*(code *)PTR_FUN_8c61b1c0)(&uStack_28);
        uVar24 = DAT_8c61b1dc;
        if (in_FPSCR_SZ == '\0') {
          dVar20 = (double)CONCAT44(*(undefined4 *)DAT_8c61b1c4,uStack_28);
        }
        else {
          in_xd2 = (double)CONCAT44(uStack_24,uStack_28);
          dVar20 = *DAT_8c61b1c4;
        }
        if (in_FPSCR_PR == '\0') {
          dVar20 = (double)CONCAT44((float)((ulonglong)dVar20 >> 0x20) * SUB84(dVar20,0),
                                    SUB84(dVar20,0));
        }
        else {
          dVar20 = dVar20 * dVar20;
        }
        if (in_FPSCR_SZ == '\0') {
          dVar20 = (double)CONCAT44((int)((ulonglong)dVar20 >> 0x20),*(undefined4 *)DAT_8c61b1c8);
          dVar22 = (double)CONCAT44((int)((ulonglong)dVar22 >> 0x20),*DAT_8c61b1cc);
        }
        else {
          in_xd2 = *DAT_8c61b1c8;
        }
        if (in_FPSCR_PR == '\0') {
          fVar25 = SUB84(dVar20,0) - (float)((ulonglong)dVar20 >> 0x20);
        }
        else {
          fVar25 = SUB84(dVar20 - dVar20,0);
        }
        if (in_FPSCR_SZ == '\0') {
          dVar20 = (double)CONCAT44(uStack_24,fVar25);
        }
        else {
          dVar20 = (double)CONCAT44(uStack_20,uStack_24);
        }
        if (in_FPSCR_PR == '\0') {
          dVar22 = (double)CONCAT44((int)((ulonglong)dVar22 >> 0x20),
                                    SUB84(dVar22,0) * (float)((ulonglong)dVar20 >> 0x20));
        }
        else {
          dVar22 = dVar22 * dVar20;
        }
        if (in_FPSCR_SZ == '\0') {
          dVar20 = (double)CONCAT44(uStack_20,SUB84(dVar20,0));
        }
        else {
          dVar20 = (double)CONCAT44(local_1c,uStack_20);
        }
        if (in_FPSCR_PR == '\0') {
          dVar20 = (double)CONCAT44((int)((ulonglong)dVar20 >> 0x20),
                                    SUB84(dVar20,0) - SUB84(dVar22,0));
        }
        else {
          dVar20 = dVar20 - dVar22;
        }
        if (in_FPSCR_SZ == '\0') {
          dVar22 = (double)CONCAT44((int)((ulonglong)dVar22 >> 0x20),
                                    *(undefined4 *)(DAT_8c61b1c4 + 1));
        }
        fVar25 = (float)((ulonglong)dVar20 >> 0x20);
        if (in_FPSCR_PR == '\0') {
          in_dr0 = (double)CONCAT44((int)((ulonglong)dVar22 >> 0x20),SUB84(dVar22,0) * fVar25);
        }
        else {
          in_dr0 = dVar22 * dVar20;
        }
        if (in_FPSCR_SZ == '\0') {
          uVar19 = SUB84(dVar20,0);
          dVar20 = (double)CONCAT44(fVar25,*(undefined4 *)DAT_8c61b1d0);
          dVar22 = in_xd2;
          in_xd2 = (double)CONCAT44(uVar19,uVar17);
        }
        else {
          dVar22 = *DAT_8c61b1d0;
        }
        if (in_FPSCR_PR == '\0') {
          param_1 = (double)CONCAT44((float)((ulonglong)in_xd2 >> 0x20) - SUB84(in_dr0,0),
                                     SUB84(in_xd2,0));
        }
        else {
          param_1 = in_xd2 - in_dr0;
        }
        if (in_FPSCR_SZ == '\0') {
          uStack_38 = SUB84(dVar20,0);
          dVar20 = (double)CONCAT44((int)((ulonglong)dVar20 >> 0x20),*(undefined4 *)DAT_8c61b1d4);
        }
        else {
          uStack_38 = SUB84(dVar22,0);
          dVar22 = *DAT_8c61b1d4;
        }
        fVar25 = (float)((ulonglong)param_1 >> 0x20);
        if (in_FPSCR_PR == '\0') {
          dVar20 = (double)CONCAT44((int)((ulonglong)dVar20 >> 0x20),SUB84(dVar20,0) * fVar25);
        }
        else {
          dVar20 = dVar20 * param_1;
        }
        if (in_FPSCR_SZ == '\0') {
          uStack_34 = SUB84(dVar20,0);
          dVar20 = (double)CONCAT44((int)((ulonglong)dVar20 >> 0x20),*(undefined4 *)DAT_8c61b1d8);
        }
        else {
          uStack_34 = SUB84(dVar22,0);
          dVar22 = *DAT_8c61b1d8;
        }
        if (in_FPSCR_PR == '\0') {
          dVar20 = (double)CONCAT44((int)((ulonglong)dVar20 >> 0x20),SUB84(dVar20,0) * fVar25);
        }
        else {
          dVar20 = dVar20 * param_1;
        }
        if (in_FPSCR_SZ == '\0') {
          uStack_30 = SUB84(dVar20,0);
          dVar20 = (double)CONCAT44((int)((ulonglong)dVar20 >> 0x20),
                                    *(undefined4 *)((int)DAT_8c61b1d0 + 0xc));
        }
        else {
          uStack_30 = SUB84(dVar22,0);
          dVar22 = *(double *)((int)DAT_8c61b1d0 + 0xc);
        }
        if (in_FPSCR_PR == '\0') {
          in_dr2 = (double)(ulonglong)(uint)(SUB84(dVar20,0) * fVar25);
        }
        else {
          in_dr2 = dVar20 * param_1;
        }
        if (in_FPSCR_SZ == '\0') {
          fStack_2c = SUB84(in_dr2,0);
          *(undefined4 *)((int)unaff_r8 + 0xc) = *(undefined4 *)(pdVar15 + 3);
          *(undefined4 *)(unaff_r8 + 1) = *(undefined4 *)((int)pdVar15 + 0x14);
          in_dr2 = (double)(ulonglong)*(uint *)(pdVar15 + 2);
          pdVar6 = (double *)((int)unaff_r8 + 4);
          *(uint *)pdVar6 = *(uint *)(pdVar15 + 2);
        }
        else {
          fStack_2c = SUB84(dVar22,0);
          uStack_28 = (undefined4)((ulonglong)dVar22 >> 0x20);
          unaff_r8[1] = pdVar15[3];
          *unaff_r8 = *(undefined8 *)((int)pdVar15 + 0x14);
          dVar22 = pdVar15[2];
          pdVar6 = (double *)(unaff_r8 + -1);
          *pdVar6 = dVar22;
        }
        *(undefined4 *)((int)pdVar6 + -4) = uVar24;
        if (in_FPSCR_SZ == '\0') {
          *(undefined4 *)(pdVar6 + 5) = *(undefined4 *)(pdVar14 + 3);
          *(undefined4 *)((int)pdVar6 + 0x24) = *(undefined4 *)((int)pdVar14 + 0x14);
          in_dr2 = (double)(ulonglong)*(uint *)(pdVar14 + 2);
          pdVar6 = pdVar6 + 4;
          *(uint *)pdVar6 = *(uint *)(pdVar14 + 2);
        }
        else {
          *(double *)((int)pdVar6 + 0x24) = pdVar14[3];
          *(undefined8 *)((int)pdVar6 + 0x1c) = *(undefined8 *)((int)pdVar14 + 0x14);
          dVar22 = pdVar14[2];
          pdVar6 = (double *)((int)pdVar6 + 0x14);
          *pdVar6 = dVar22;
        }
        *(undefined4 *)((int)pdVar6 + -4) = uVar24;
        if (in_FPSCR_SZ == '\0') {
          *(float *)(pdVar6 + 7) = fStack_2c;
          *(uint *)((int)pdVar6 + 0x34) = uStack_30;
          *(undefined4 *)(pdVar6 + 6) = uStack_34;
          *(undefined4 *)((int)pdVar6 + 0x2c) = uStack_38;
          *(undefined4 *)(pdVar6 + 5) = *(undefined4 *)(pdVar13 + 3);
          *(undefined4 *)((int)pdVar6 + 0x24) = *(undefined4 *)((int)pdVar13 + 0x14);
          in_dr2 = (double)(ulonglong)*(uint *)(pdVar13 + 2);
          pdVar6 = pdVar6 + 4;
          *(uint *)pdVar6 = *(uint *)(pdVar13 + 2);
        }
        else {
          *(ulonglong *)((int)pdVar6 + 0x34) = CONCAT44(uStack_28,fStack_2c);
          *(ulonglong *)((int)pdVar6 + 0x2c) = CONCAT44(fStack_2c,uStack_30);
          *(ulonglong *)((int)pdVar6 + 0x24) = CONCAT44(uStack_30,uStack_34);
          *(ulonglong *)((int)pdVar6 + 0x1c) = CONCAT44(uStack_34,uStack_38);
          *(double *)((int)pdVar6 + 0x14) = pdVar13[3];
          *(undefined8 *)((int)pdVar6 + 0xc) = *(undefined8 *)((int)pdVar13 + 0x14);
          dVar22 = pdVar13[2];
          pdVar6 = (double *)((int)pdVar6 + 4);
          *pdVar6 = dVar22;
        }
        *(undefined4 *)((int)pdVar6 + -4) = uVar24;
        uVar17 = DAT_8c61b1e0;
        if (in_FPSCR_SZ == '\0') {
          *(float *)(pdVar6 + 7) = fStack_2c;
          *(uint *)((int)pdVar6 + 0x34) = uStack_30;
          *(undefined4 *)(pdVar6 + 6) = uStack_34;
          *(undefined4 *)((int)pdVar6 + 0x2c) = uStack_38;
          *(undefined4 *)(pdVar6 + 5) = *(undefined4 *)(iVar5 + 0x18);
          *(undefined4 *)((int)pdVar6 + 0x24) = *(undefined4 *)(iVar5 + 0x14);
          in_dr2 = (double)(ulonglong)*(uint *)(iVar5 + 0x10);
          pdVar6 = pdVar6 + 4;
          *(uint *)pdVar6 = *(uint *)(iVar5 + 0x10);
          in_xd2 = dVar22;
        }
        else {
          *(ulonglong *)((int)pdVar6 + 0x34) = CONCAT44(uStack_28,fStack_2c);
          *(ulonglong *)((int)pdVar6 + 0x2c) = CONCAT44(fStack_2c,uStack_30);
          *(ulonglong *)((int)pdVar6 + 0x24) = CONCAT44(uStack_30,uStack_34);
          *(ulonglong *)((int)pdVar6 + 0x1c) = CONCAT44(uStack_34,uStack_38);
          *(undefined8 *)((int)pdVar6 + 0x14) = *(undefined8 *)(iVar5 + 0x18);
          *(undefined8 *)((int)pdVar6 + 0xc) = *(undefined8 *)(iVar5 + 0x14);
          in_xd2 = *(double *)(iVar5 + 0x10);
          pdVar6 = (double *)((int)pdVar6 + 4);
          *pdVar6 = in_xd2;
        }
        *(undefined4 *)((int)pdVar6 + -4) = uVar17;
        unaff_r8 = (undefined8 *)((int)pdVar6 + 0x1c);
        *DAT_8c61b1e4 = *DAT_8c61b1e4 + 4;
      }
    }
  }
  else if (uVar3 == DAT_8c61aea8) {
    for (iVar8 = 0; uVar17 = SUB84(param_1,0), iVar8 < (int)(uint)param_3[1]; iVar8 = iVar8 + 1) {
      puVar12 = puVar9 + 1;
      uVar3 = (uint)*puVar9;
      *piVar2 = *piVar2 + uVar3;
      iVar5 = *DAT_8c61b454;
      pdVar14 = (double *)((uint)puVar9[2] * 0x20 + iVar5);
      if (in_FPSCR_SZ == '\0') {
        uVar17 = *(undefined4 *)(pdVar14 + 2);
      }
      else {
        in_xd4 = pdVar14[2];
      }
      pdVar15 = (double *)((uint)*puVar12 * 0x20 + iVar5);
      if (in_FPSCR_SZ == '\0') {
        dVar22 = (double)CONCAT44(uVar17,*(undefined4 *)(pdVar15 + 2));
      }
      else {
        in_xd2 = pdVar15[2];
        dVar22 = in_xd4;
      }
      if (in_FPSCR_PR == '\0') {
        dVar22 = (double)CONCAT44((float)((ulonglong)dVar22 >> 0x20) - SUB84(dVar22,0),
                                  SUB84(dVar22,0));
      }
      else {
        dVar22 = dVar22 - dVar22;
      }
      if (in_FPSCR_SZ == '\0') {
        dVar20 = (double)CONCAT44(*(undefined4 *)((int)pdVar14 + 0x14),uVar17);
      }
      else {
        dVar20 = *(double *)((int)pdVar14 + 0x14);
      }
      pdVar13 = (double *)((uint)puVar9[3] * 0x20 + iVar5);
      fVar25 = (float)((ulonglong)dVar20 >> 0x20);
      if (in_FPSCR_SZ == '\0') {
        dVar22 = (double)CONCAT44((int)((ulonglong)dVar22 >> 0x20),
                                  *(undefined4 *)((int)pdVar13 + 0x14));
        in_dr0 = (double)CONCAT44((int)((ulonglong)in_dr0 >> 0x20),fVar25);
      }
      else {
        in_xd2 = *(double *)((int)pdVar13 + 0x14);
      }
      if (in_FPSCR_PR == '\0') {
        fVar25 = SUB84(dVar22,0) - fVar25;
        dVar22 = (double)CONCAT44((float)((ulonglong)dVar22 >> 0x20) * fVar25,fVar25);
      }
      else {
        dVar22 = (dVar22 - dVar20) * (dVar22 - dVar20);
      }
      if (in_FPSCR_SZ == '\0') {
        dVar22 = (double)CONCAT44((int)((ulonglong)dVar22 >> 0x20),
                                  *(undefined4 *)((int)pdVar15 + 0x14));
      }
      else {
        in_xd2 = *(double *)((int)pdVar15 + 0x14);
      }
      if (in_FPSCR_PR == '\0') {
        dVar16 = (double)CONCAT44((int)((ulonglong)in_dr0 >> 0x20),SUB84(in_dr0,0) - SUB84(dVar22,0)
                                 );
      }
      else {
        dVar16 = in_dr0 - dVar22;
      }
      if (in_FPSCR_SZ == '\0') {
        dVar22 = (double)CONCAT44((int)((ulonglong)dVar22 >> 0x20),*(undefined4 *)(pdVar13 + 2));
      }
      else {
        in_xd2 = pdVar13[2];
      }
      if (in_FPSCR_PR == '\0') {
        dVar22 = (double)CONCAT44((int)((ulonglong)dVar22 >> 0x20),SUB84(dVar22,0) - SUB84(dVar20,0)
                                 );
      }
      else {
        dVar22 = dVar22 - dVar20;
      }
      uVar21 = (uint)((ulonglong)dVar22 >> 0x20);
      param_1 = dVar22;
      if (in_FPSCR_SZ == '\0') {
        param_1 = (double)CONCAT44(uVar21,SUB84(dVar20,0));
      }
      if (in_FPSCR_PR == '\0') {
        in_dr0 = (double)CONCAT44((int)((ulonglong)dVar16 >> 0x20),SUB84(dVar16,0) * SUB84(dVar22,0)
                                 );
      }
      else {
        in_dr0 = dVar16 * dVar22;
      }
      if (in_FPSCR_PR == '\0') {
        fVar25 = (float)((ulonglong)param_1 >> 0x20) - SUB84(in_dr0,0);
        param_1 = (double)CONCAT44(fVar25,SUB84(param_1,0));
        bVar28 = 0.0 < fVar25;
      }
      else {
        param_1 = param_1 - in_dr0;
        bVar28 = (double)((ulonglong)uVar21 << 0x20) < param_1;
      }
      uVar17 = SUB84(param_1,0);
      if (bVar28) {
        puVar9 = puVar12 + uVar3;
      }
      else {
        *DAT_8c61b458 = *DAT_8c61b458 + uVar3;
        if (in_FPSCR_SZ == '\0') {
          dVar22 = (double)CONCAT44(*(undefined4 *)pdVar15,*(undefined4 *)pdVar14);
        }
        else {
          in_xd2 = *pdVar14;
          dVar22 = *pdVar15;
        }
        if (in_FPSCR_PR == '\0') {
          dVar22 = (double)CONCAT44((float)((ulonglong)dVar22 >> 0x20) + SUB84(dVar22,0),
                                    SUB84(dVar22,0));
        }
        else {
          dVar22 = dVar22 + dVar22;
        }
        if (in_FPSCR_SZ == '\0') {
          dVar22 = (double)CONCAT44((int)((ulonglong)dVar22 >> 0x20),*(undefined4 *)pdVar13);
        }
        else {
          in_xd2 = *pdVar13;
        }
        if (in_FPSCR_PR == '\0') {
          dVar22 = (double)CONCAT44((float)((ulonglong)dVar22 >> 0x20) + SUB84(dVar22,0),
                                    SUB84(dVar22,0));
        }
        else {
          dVar22 = dVar22 + dVar22;
        }
        if (in_FPSCR_SZ == '\0') {
          uStack_28 = (undefined4)((ulonglong)dVar22 >> 0x20);
          dVar22 = (double)CONCAT44(*(undefined4 *)((int)pdVar15 + 4),
                                    *(undefined4 *)((int)pdVar14 + 4));
        }
        else {
          uStack_28 = SUB84(dVar22,0);
          dVar22 = *(double *)((int)pdVar15 + 4);
          in_xd2 = *(double *)((int)pdVar14 + 4);
        }
        if (in_FPSCR_PR == '\0') {
          dVar22 = (double)CONCAT44((float)((ulonglong)dVar22 >> 0x20) + SUB84(dVar22,0),
                                    SUB84(dVar22,0));
        }
        else {
          dVar22 = dVar22 + dVar22;
        }
        if (in_FPSCR_SZ == '\0') {
          dVar22 = (double)CONCAT44((int)((ulonglong)dVar22 >> 0x20),
                                    *(undefined4 *)((int)pdVar13 + 4));
        }
        else {
          in_xd2 = *(double *)((int)pdVar13 + 4);
        }
        if (in_FPSCR_PR == '\0') {
          dVar22 = (double)CONCAT44((float)((ulonglong)dVar22 >> 0x20) + SUB84(dVar22,0),
                                    SUB84(dVar22,0));
        }
        else {
          dVar22 = dVar22 + dVar22;
        }
        if (in_FPSCR_SZ == '\0') {
          uStack_24 = (undefined4)((ulonglong)dVar22 >> 0x20);
          dVar22 = (double)CONCAT44(*(undefined4 *)(pdVar15 + 1),*(undefined4 *)(pdVar14 + 1));
        }
        else {
          uStack_24 = SUB84(dVar22,0);
          in_xd2 = pdVar14[1];
          dVar22 = pdVar15[1];
        }
        if (in_FPSCR_PR == '\0') {
          dVar22 = (double)CONCAT44((float)((ulonglong)dVar22 >> 0x20) + SUB84(dVar22,0),
                                    SUB84(dVar22,0));
        }
        else {
          dVar22 = dVar22 + dVar22;
        }
        if (in_FPSCR_SZ == '\0') {
          dVar22 = (double)CONCAT44((int)((ulonglong)dVar22 >> 0x20),*(undefined4 *)(pdVar13 + 1));
        }
        else {
          in_xd2 = pdVar13[1];
        }
        if (in_FPSCR_PR == '\0') {
          dVar22 = (double)CONCAT44((float)((ulonglong)dVar22 >> 0x20) + SUB84(dVar22,0),
                                    SUB84(dVar22,0));
        }
        else {
          dVar22 = dVar22 + dVar22;
        }
        uVar24 = (undefined4)((ulonglong)dVar22 >> 0x20);
        uStack_20 = uVar24;
        if (in_FPSCR_SZ != '\0') {
          uStack_20 = SUB84(dVar22,0);
          local_1c = uVar24;
        }
        dVar22 = (double)(*(code *)PTR_FUN_8c61b45c)(&uStack_28);
        uVar24 = DAT_8c61b478;
        if (in_FPSCR_SZ == '\0') {
          dVar20 = (double)CONCAT44(*(undefined4 *)DAT_8c61b460,uStack_28);
        }
        else {
          in_xd2 = (double)CONCAT44(uStack_24,uStack_28);
          dVar20 = *DAT_8c61b460;
        }
        if (in_FPSCR_PR == '\0') {
          dVar20 = (double)CONCAT44((float)((ulonglong)dVar20 >> 0x20) * SUB84(dVar20,0),
                                    SUB84(dVar20,0));
        }
        else {
          dVar20 = dVar20 * dVar20;
        }
        if (in_FPSCR_SZ == '\0') {
          dVar20 = (double)CONCAT44((int)((ulonglong)dVar20 >> 0x20),*(undefined4 *)DAT_8c61b464);
          dVar22 = (double)CONCAT44((int)((ulonglong)dVar22 >> 0x20),*DAT_8c61b468);
        }
        else {
          in_xd2 = *DAT_8c61b464;
        }
        if (in_FPSCR_PR == '\0') {
          fVar25 = SUB84(dVar20,0) - (float)((ulonglong)dVar20 >> 0x20);
        }
        else {
          fVar25 = SUB84(dVar20 - dVar20,0);
        }
        if (in_FPSCR_SZ == '\0') {
          dVar20 = (double)CONCAT44(uStack_24,fVar25);
        }
        else {
          dVar20 = (double)CONCAT44(uStack_20,uStack_24);
        }
        if (in_FPSCR_PR == '\0') {
          dVar22 = (double)CONCAT44((int)((ulonglong)dVar22 >> 0x20),
                                    SUB84(dVar22,0) * (float)((ulonglong)dVar20 >> 0x20));
        }
        else {
          dVar22 = dVar22 * dVar20;
        }
        if (in_FPSCR_SZ == '\0') {
          dVar20 = (double)CONCAT44(uStack_20,SUB84(dVar20,0));
        }
        else {
          dVar20 = (double)CONCAT44(local_1c,uStack_20);
        }
        if (in_FPSCR_PR == '\0') {
          dVar20 = (double)CONCAT44((int)((ulonglong)dVar20 >> 0x20),
                                    SUB84(dVar20,0) - SUB84(dVar22,0));
        }
        else {
          dVar20 = dVar20 - dVar22;
        }
        if (in_FPSCR_SZ == '\0') {
          dVar22 = (double)CONCAT44((int)((ulonglong)dVar22 >> 0x20),
                                    *(undefined4 *)(DAT_8c61b460 + 1));
        }
        fVar25 = (float)((ulonglong)dVar20 >> 0x20);
        if (in_FPSCR_PR == '\0') {
          in_dr0 = (double)CONCAT44((int)((ulonglong)dVar22 >> 0x20),SUB84(dVar22,0) * fVar25);
        }
        else {
          in_dr0 = dVar22 * dVar20;
        }
        if (in_FPSCR_SZ == '\0') {
          uVar19 = SUB84(dVar20,0);
          dVar20 = (double)CONCAT44(fVar25,*(undefined4 *)DAT_8c61b46c);
          dVar22 = in_xd2;
          in_xd2 = (double)CONCAT44(uVar19,uVar17);
        }
        else {
          dVar22 = *DAT_8c61b46c;
        }
        if (in_FPSCR_PR == '\0') {
          param_1 = (double)CONCAT44((float)((ulonglong)in_xd2 >> 0x20) - SUB84(in_dr0,0),
                                     SUB84(in_xd2,0));
        }
        else {
          param_1 = in_xd2 - in_dr0;
        }
        if (in_FPSCR_SZ == '\0') {
          uStack_38 = SUB84(dVar20,0);
          dVar20 = (double)CONCAT44((int)((ulonglong)dVar20 >> 0x20),*(undefined4 *)DAT_8c61b470);
        }
        else {
          uStack_38 = SUB84(dVar22,0);
          dVar22 = *DAT_8c61b470;
        }
        fVar25 = (float)((ulonglong)param_1 >> 0x20);
        if (in_FPSCR_PR == '\0') {
          dVar20 = (double)CONCAT44((int)((ulonglong)dVar20 >> 0x20),SUB84(dVar20,0) * fVar25);
        }
        else {
          dVar20 = dVar20 * param_1;
        }
        if (in_FPSCR_SZ == '\0') {
          uStack_34 = SUB84(dVar20,0);
          dVar20 = (double)CONCAT44((int)((ulonglong)dVar20 >> 0x20),*(undefined4 *)DAT_8c61b474);
        }
        else {
          uStack_34 = SUB84(dVar22,0);
          dVar22 = *DAT_8c61b474;
        }
        if (in_FPSCR_PR == '\0') {
          dVar20 = (double)CONCAT44((int)((ulonglong)dVar20 >> 0x20),SUB84(dVar20,0) * fVar25);
        }
        else {
          dVar20 = dVar20 * param_1;
        }
        if (in_FPSCR_SZ == '\0') {
          uStack_30 = SUB84(dVar20,0);
          dVar20 = (double)CONCAT44((int)((ulonglong)dVar20 >> 0x20),
                                    *(undefined4 *)((int)DAT_8c61b46c + 0xc));
        }
        else {
          uStack_30 = SUB84(dVar22,0);
          dVar22 = *(double *)((int)DAT_8c61b46c + 0xc);
        }
        if (in_FPSCR_PR == '\0') {
          dVar20 = (double)(ulonglong)(uint)(SUB84(dVar20,0) * fVar25);
        }
        else {
          dVar20 = dVar20 * param_1;
        }
        if (in_FPSCR_SZ == '\0') {
          fStack_2c = SUB84(dVar20,0);
        }
        else {
          fStack_2c = SUB84(dVar22,0);
          uStack_28 = (undefined4)((ulonglong)dVar22 >> 0x20);
        }
        iVar5 = (uint)*puVar12 * 0x20 + *DAT_8c61b454;
        if (in_FPSCR_SZ == '\0') {
          *(undefined4 *)((int)unaff_r8 + 0xc) = *(undefined4 *)(iVar5 + 0x18);
          *(undefined4 *)(unaff_r8 + 1) = *(undefined4 *)(iVar5 + 0x14);
          dVar20 = (double)(ulonglong)*(uint *)(iVar5 + 0x10);
          pdVar14 = (double *)((int)unaff_r8 + 4);
          *(uint *)pdVar14 = *(uint *)(iVar5 + 0x10);
        }
        else {
          unaff_r8[1] = *(undefined8 *)(iVar5 + 0x18);
          *unaff_r8 = *(undefined8 *)(iVar5 + 0x14);
          dVar22 = *(double *)(iVar5 + 0x10);
          pdVar14 = (double *)(unaff_r8 + -1);
          *pdVar14 = dVar22;
        }
        *(undefined4 *)((int)pdVar14 + -4) = uVar24;
        uVar17 = DAT_8c61b478;
        puVar12 = puVar9 + 3;
        iVar5 = (uint)puVar9[2] * 0x20 + *DAT_8c61b454;
        if (in_FPSCR_SZ == '\0') {
          *(undefined4 *)(pdVar14 + 5) = *(undefined4 *)(iVar5 + 0x18);
          *(undefined4 *)((int)pdVar14 + 0x24) = *(undefined4 *)(iVar5 + 0x14);
          dVar20 = (double)(ulonglong)*(uint *)(iVar5 + 0x10);
          pdVar14 = pdVar14 + 4;
          *(uint *)pdVar14 = *(uint *)(iVar5 + 0x10);
        }
        else {
          *(undefined8 *)((int)pdVar14 + 0x24) = *(undefined8 *)(iVar5 + 0x18);
          *(undefined8 *)((int)pdVar14 + 0x1c) = *(undefined8 *)(iVar5 + 0x14);
          dVar22 = *(double *)(iVar5 + 0x10);
          pdVar14 = (double *)((int)pdVar14 + 0x14);
          *pdVar14 = dVar22;
        }
        uVar21 = SUB84(dVar20,0);
        *(undefined4 *)((int)pdVar14 + -4) = uVar17;
        iVar5 = 2;
        iVar7 = (int)pdVar14 + 0x1c;
        if (2 < (int)(uVar3 - 1)) {
          do {
            uVar1 = *puVar12;
            puVar12 = puVar12 + 1;
            if (in_FPSCR_SZ == '\0') {
              dVar20 = (double)(ulonglong)(uint)fStack_2c;
            }
            else {
              dVar22 = (double)CONCAT44(uStack_28,fStack_2c);
            }
            iVar4 = *DAT_8c61b454;
            if (in_FPSCR_SZ == '\0') {
              pdVar14 = (double *)(iVar7 + 0x1c);
              *(int *)pdVar14 = SUB84(dVar20,0);
              dVar20 = (double)(ulonglong)uStack_30;
            }
            else {
              pdVar14 = (double *)(iVar7 + 0x18);
              *pdVar14 = dVar22;
              dVar22 = (double)CONCAT44(fStack_2c,uStack_30);
            }
            if (in_FPSCR_SZ == '\0') {
              pdVar14 = (double *)((int)pdVar14 + -4);
              *(int *)pdVar14 = SUB84(dVar20,0);
            }
            else {
              pdVar14 = pdVar14 + -1;
              *pdVar14 = dVar22;
            }
            uVar17 = DAT_8c61b478;
            iVar4 = (uint)uVar1 * 0x20 + iVar4;
            if (in_FPSCR_SZ == '\0') {
              *(undefined4 *)((int)pdVar14 + -4) = uStack_34;
              *(undefined4 *)(pdVar14 + -1) = uStack_38;
              *(undefined4 *)((int)pdVar14 + -0xc) = *(undefined4 *)(iVar4 + 0x18);
              *(undefined4 *)(pdVar14 + -2) = *(undefined4 *)(iVar4 + 0x14);
              dVar20 = (double)(ulonglong)*(uint *)(iVar4 + 0x10);
              pdVar14 = (double *)((int)pdVar14 + -0x14);
              *(uint *)pdVar14 = *(uint *)(iVar4 + 0x10);
            }
            else {
              pdVar14[-1] = (double)CONCAT44(uStack_30,uStack_34);
              pdVar14[-2] = (double)CONCAT44(uStack_34,uStack_38);
              pdVar14[-3] = *(double *)(iVar4 + 0x18);
              pdVar14[-4] = *(double *)(iVar4 + 0x14);
              dVar22 = *(double *)(iVar4 + 0x10);
              pdVar14 = pdVar14 + -5;
              *pdVar14 = dVar22;
            }
            uVar21 = SUB84(dVar20,0);
            *(undefined4 *)((int)pdVar14 + -4) = uVar17;
            iVar5 = iVar5 + 1;
            iVar7 = (int)pdVar14 + 0x1c;
          } while (iVar5 < (int)(uVar3 - 1));
        }
        uVar1 = *puVar12;
        puVar9 = puVar12 + 1;
        fVar25 = fStack_2c;
        if (in_FPSCR_SZ != '\0') {
          dVar22 = (double)CONCAT44(uStack_28,fStack_2c);
          fVar25 = (float)uVar21;
        }
        iVar5 = *DAT_8c61b454;
        if (in_FPSCR_SZ == '\0') {
          pdVar14 = (double *)(iVar7 + 0x1c);
          *(float *)pdVar14 = fVar25;
          fVar25 = (float)uStack_30;
        }
        else {
          pdVar14 = (double *)(iVar7 + 0x18);
          *pdVar14 = dVar22;
          dVar22 = (double)CONCAT44(fStack_2c,uStack_30);
        }
        if (in_FPSCR_SZ == '\0') {
          pdVar14 = (double *)((int)pdVar14 + -4);
          *(float *)pdVar14 = fVar25;
        }
        else {
          pdVar14 = pdVar14 + -1;
          *pdVar14 = dVar22;
        }
        uVar17 = DAT_8c61b47c;
        iVar5 = (uint)uVar1 * 0x20 + iVar5;
        if (in_FPSCR_SZ == '\0') {
          *(undefined4 *)((int)pdVar14 + -4) = uStack_34;
          *(undefined4 *)(pdVar14 + -1) = uStack_38;
          *(undefined4 *)((int)pdVar14 + -0xc) = *(undefined4 *)(iVar5 + 0x18);
          *(undefined4 *)(pdVar14 + -2) = *(undefined4 *)(iVar5 + 0x14);
          pdVar14 = (double *)((int)pdVar14 + -0x14);
          *(undefined4 *)pdVar14 = *(undefined4 *)(iVar5 + 0x10);
          in_xd2 = dVar22;
        }
        else {
          pdVar14[-1] = (double)CONCAT44(uStack_30,uStack_34);
          pdVar14[-2] = (double)CONCAT44(uStack_34,uStack_38);
          pdVar14[-3] = *(double *)(iVar5 + 0x18);
          pdVar14[-4] = *(double *)(iVar5 + 0x14);
          in_xd2 = *(double *)(iVar5 + 0x10);
          pdVar14 = pdVar14 + -5;
          *pdVar14 = in_xd2;
        }
        *(undefined4 *)((int)pdVar14 + -4) = uVar17;
        unaff_r8 = (undefined8 *)((int)pdVar14 + 0x1c);
      }
    }
  }
  else if (uVar3 == DAT_8c61aea4) {
    *DAT_8c61b480 = param_3;
    if (in_FPSCR_SZ == '\0') {
      uVar23 = CONCAT44(*(undefined4 *)(DAT_8c61b448 + 0x10),*(undefined4 *)(DAT_8c61b448 + 0x14));
      uVar26 = CONCAT44(*(undefined4 *)(DAT_8c61b448 + 0x18),*(undefined4 *)(DAT_8c61b448 + 0x1c));
      uVar27 = CONCAT44(*(undefined4 *)DAT_8c61b44c,param_2);
    }
    else {
      uVar23 = *(undefined8 *)(DAT_8c61b448 + 0x10);
      uVar26 = *(undefined8 *)(DAT_8c61b448 + 0x20);
      uVar27 = *DAT_8c61b44c;
    }
    (*(code *)PTR_FUN_8c61b484)(uVar23,uVar26,uVar27,*DAT_8c61b480);
  }
  return;
}

