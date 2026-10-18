/* Ghidra draft only; not buildable/verified source. */

/* WARNING: Removing unreachable block (ram,0x8c61bfec) */
/* WARNING: Removing unreachable block (ram,0x8c61bf8e) */
/* WARNING: Removing unreachable block (ram,0x8c61c186) */
/* WARNING: Removing unreachable block (ram,0x8c61c1ba) */
/* WARNING: Removing unreachable block (ram,0x8c61c178) */
/* WARNING: Removing unreachable block (ram,0x8c61c144) */
/* WARNING: Removing unreachable block (ram,0x8c61c11a) */
/* WARNING: Removing unreachable block (ram,0x8c61c310) */
/* WARNING: Removing unreachable block (ram,0x8c61c352) */
/* WARNING: Removing unreachable block (ram,0x8c61c3b0) */
/* WARNING: Removing unreachable block (ram,0x8c61c31e) */
/* WARNING: Removing unreachable block (ram,0x8c61bf82) */
/* WARNING: Removing unreachable block (ram,0x8c61bfb8) */
/* WARNING: Removing unreachable block (ram,0x8c61bfc6) */
/* WARNING: Removing unreachable block (ram,0x8c61c3a2) */
/* WARNING: Removing unreachable block (ram,0x8c61c3fc) */
/* WARNING: Removing unreachable block (ram,0x8c61c360) */
/* WARNING: Removing unreachable block (ram,0x8c61c152) */
/* WARNING: Removing unreachable block (ram,0x8c61c1ac) */
/* WARNING: Removing unreachable block (ram,0x8c61c10e) */
/* WARNING: Removing unreachable block (ram,0x8c61bffa) */
/* WARNING: Removing unreachable block (ram,0x8c61c3ee) */
/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_8c61be44(double param_1,ushort *param_2)

{
  short sVar1;
  ushort uVar2;
  int *piVar3;
  uint uVar4;
  uint uVar5;
  int iVar6;
  int iVar7;
  int iVar8;
  uint *unaff_r8;
  double *pdVar9;
  uint *puVar10;
  ushort *puVar11;
  ushort *puVar12;
  ushort *puVar13;
  ushort *puVar14;
  short *psVar15;
  short *psVar16;
  double *pdVar17;
  double *pdVar18;
  double *pdVar19;
  undefined1 *puVar20;
  double in_dr0;
  double dVar21;
  undefined4 uVar22;
  float fVar23;
  double in_dr2;
  double dVar24;
  uint uVar25;
  undefined4 uVar26;
  double dVar27;
  undefined8 uVar28;
  float in_fr15;
  undefined4 in_fr14;
  undefined8 in_xd0;
  double in_xd2;
  double in_xd4;
  bool bVar29;
  undefined4 in_PR;
  char in_FPSCR_PR;
  char in_FPSCR_SZ;
  undefined1 local_20 [4];
  undefined1 local_1c [4];
  
  piVar3 = _DAT_8c61bf0c;
  if (in_FPSCR_SZ == '\0') {
    puVar20 = local_1c;
  }
  else {
    puVar20 = local_20;
  }
  *(undefined4 *)(puVar20 + -4) = in_PR;
  *(ushort **)(puVar20 + -0x18) = param_2;
  if (in_FPSCR_SZ == '\0') {
    in_fr15 = DAT_8c61bf08;
  }
  puVar11 = *(ushort **)(param_2 + 2);
  psVar15 = *(short **)(param_2 + 10);
  uVar5 = *param_2 & DAT_8c61bf10;
  if (uVar5 == 0) {
    *_DAT_8c61bf0c = *_DAT_8c61bf0c + (uint)*(ushort *)(*(int *)(puVar20 + -0x18) + 2) * 3;
    for (iVar7 = 0; uVar26 = SUB84(param_1,0),
        iVar7 < (int)(uint)*(ushort *)(*(int *)(puVar20 + -0x18) + 2); iVar7 = iVar7 + 1) {
      uVar2 = *puVar11;
      puVar14 = puVar11 + 1;
      puVar12 = puVar11 + 2;
      iVar8 = *DAT_8c61bf18;
      puVar11 = puVar11 + 3;
      pdVar18 = (double *)((uint)*puVar14 * 0x20 + iVar8);
      if (in_FPSCR_SZ == '\0') {
        uVar26 = *(undefined4 *)(pdVar18 + 2);
      }
      else {
        in_xd4 = pdVar18[2];
      }
      pdVar19 = (double *)((uint)uVar2 * 0x20 + iVar8);
      if (in_FPSCR_SZ == '\0') {
        dVar24 = (double)CONCAT44(uVar26,*(undefined4 *)(pdVar19 + 2));
      }
      else {
        in_xd2 = pdVar19[2];
        dVar24 = in_xd4;
      }
      if (in_FPSCR_PR == '\0') {
        dVar24 = (double)CONCAT44((float)((ulonglong)dVar24 >> 0x20) - SUB84(dVar24,0),
                                  SUB84(dVar24,0));
      }
      else {
        dVar24 = dVar24 - dVar24;
      }
      pdVar17 = (double *)((uint)*puVar12 * 0x20 + iVar8);
      if (in_FPSCR_SZ == '\0') {
        fVar23 = *(float *)((int)pdVar18 + 0x14);
        dVar27 = (double)CONCAT44(fVar23,uVar26);
        dVar24 = (double)CONCAT44((int)((ulonglong)dVar24 >> 0x20),
                                  *(undefined4 *)((int)pdVar17 + 0x14));
        in_dr0 = (double)CONCAT44((int)((ulonglong)in_dr0 >> 0x20),fVar23);
      }
      else {
        dVar27 = *(double *)((int)pdVar18 + 0x14);
        in_xd2 = *(double *)((int)pdVar17 + 0x14);
        fVar23 = (float)((ulonglong)dVar27 >> 0x20);
      }
      if (in_FPSCR_PR == '\0') {
        fVar23 = SUB84(dVar24,0) - fVar23;
        param_1 = (double)CONCAT44((float)((ulonglong)dVar24 >> 0x20) * fVar23,fVar23);
      }
      else {
        param_1 = (dVar24 - dVar27) * (dVar24 - dVar27);
      }
      if (in_FPSCR_SZ == '\0') {
        param_1 = (double)CONCAT44((int)((ulonglong)param_1 >> 0x20),
                                   *(undefined4 *)((int)pdVar19 + 0x14));
      }
      else {
        in_xd2 = *(double *)((int)pdVar19 + 0x14);
      }
      if (in_FPSCR_PR == '\0') {
        dVar24 = (double)CONCAT44((int)((ulonglong)in_dr0 >> 0x20),
                                  SUB84(in_dr0,0) - SUB84(param_1,0));
      }
      else {
        dVar24 = in_dr0 - param_1;
      }
      uVar26 = (undefined4)((ulonglong)param_1 >> 0x20);
      if (in_FPSCR_SZ == '\0') {
        param_1 = (double)CONCAT44(uVar26,SUB84(dVar27,0));
        dVar27 = (double)CONCAT44(uVar26,*(undefined4 *)(pdVar17 + 2));
      }
      else {
        in_xd2 = pdVar17[2];
        dVar27 = param_1;
      }
      if (in_FPSCR_PR == '\0') {
        fVar23 = SUB84(dVar27,0) - SUB84(param_1,0);
        dVar27 = (double)CONCAT44(uVar26,fVar23);
        in_dr0 = (double)CONCAT44((int)((ulonglong)dVar24 >> 0x20),SUB84(dVar24,0) * fVar23);
      }
      else {
        dVar27 = dVar27 - param_1;
        in_dr0 = dVar24 * dVar27;
      }
      if (in_FPSCR_PR == '\0') {
        fVar23 = (float)((ulonglong)param_1 >> 0x20) - SUB84(in_dr0,0);
        param_1 = (double)CONCAT44(fVar23,SUB84(param_1,0));
        bVar29 = 0.0 < fVar23;
      }
      else {
        param_1 = param_1 - in_dr0;
        bVar29 = (double)((ulonglong)dVar27 & 0xffffffff00000000) < param_1;
      }
      uVar26 = SUB84(param_1,0);
      if (!bVar29) {
        if (in_FPSCR_SZ == '\0') {
          dVar24 = (double)CONCAT44(*(undefined4 *)pdVar19,*(undefined4 *)pdVar18);
        }
        else {
          dVar24 = *pdVar19;
          in_xd2 = *pdVar18;
        }
        if (in_FPSCR_PR == '\0') {
          dVar24 = (double)CONCAT44((float)((ulonglong)dVar24 >> 0x20) + SUB84(dVar24,0),
                                    SUB84(dVar24,0));
        }
        else {
          dVar24 = dVar24 + dVar24;
        }
        if (in_FPSCR_SZ == '\0') {
          dVar24 = (double)CONCAT44((int)((ulonglong)dVar24 >> 0x20),*(undefined4 *)pdVar17);
        }
        else {
          in_xd2 = *pdVar17;
        }
        if (in_FPSCR_PR == '\0') {
          dVar24 = (double)CONCAT44((float)((ulonglong)dVar24 >> 0x20) + SUB84(dVar24,0),
                                    SUB84(dVar24,0));
        }
        else {
          dVar24 = dVar24 + dVar24;
        }
        if (in_FPSCR_SZ == '\0') {
          *(int *)(puVar20 + -0x10) = (int)((ulonglong)dVar24 >> 0x20);
          dVar24 = (double)CONCAT44(*(undefined4 *)((int)pdVar19 + 4),
                                    *(undefined4 *)((int)pdVar18 + 4));
        }
        else {
          *(double *)(puVar20 + -0x10) = dVar24;
          dVar24 = *(double *)((int)pdVar19 + 4);
          in_xd2 = *(double *)((int)pdVar18 + 4);
        }
        if (in_FPSCR_PR == '\0') {
          dVar24 = (double)CONCAT44((float)((ulonglong)dVar24 >> 0x20) + SUB84(dVar24,0),
                                    SUB84(dVar24,0));
        }
        else {
          dVar24 = dVar24 + dVar24;
        }
        if (in_FPSCR_SZ == '\0') {
          dVar24 = (double)CONCAT44((int)((ulonglong)dVar24 >> 0x20),
                                    *(undefined4 *)((int)pdVar17 + 4));
        }
        else {
          in_xd2 = *(double *)((int)pdVar17 + 4);
        }
        if (in_FPSCR_PR == '\0') {
          dVar24 = (double)CONCAT44((float)((ulonglong)dVar24 >> 0x20) + SUB84(dVar24,0),
                                    SUB84(dVar24,0));
        }
        else {
          dVar24 = dVar24 + dVar24;
        }
        if (in_FPSCR_SZ == '\0') {
          *(int *)(puVar20 + -0xc) = (int)((ulonglong)dVar24 >> 0x20);
          dVar24 = (double)CONCAT44(*(undefined4 *)(pdVar19 + 1),*(undefined4 *)(pdVar18 + 1));
        }
        else {
          *(double *)(puVar20 + -0xc) = dVar24;
          dVar24 = pdVar19[1];
          in_xd2 = pdVar18[1];
        }
        if (in_FPSCR_PR == '\0') {
          dVar24 = (double)CONCAT44((float)((ulonglong)dVar24 >> 0x20) + SUB84(dVar24,0),
                                    SUB84(dVar24,0));
        }
        else {
          dVar24 = dVar24 + dVar24;
        }
        if (in_FPSCR_SZ == '\0') {
          dVar24 = (double)CONCAT44((int)((ulonglong)dVar24 >> 0x20),*(undefined4 *)(pdVar17 + 1));
        }
        else {
          in_xd2 = pdVar17[1];
        }
        if (in_FPSCR_PR == '\0') {
          dVar24 = (double)CONCAT44((float)((ulonglong)dVar24 >> 0x20) + SUB84(dVar24,0),
                                    SUB84(dVar24,0));
        }
        else {
          dVar24 = dVar24 + dVar24;
        }
        if (in_FPSCR_SZ == '\0') {
          *(int *)(puVar20 + -8) = (int)((ulonglong)dVar24 >> 0x20);
        }
        else {
          *(double *)(puVar20 + -8) = dVar24;
        }
        dVar24 = (double)(*(code *)PTR_FUN_8c61c204)(puVar20 + -0x10);
        if (in_FPSCR_SZ == '\0') {
          dVar27 = (double)CONCAT44(*(undefined4 *)DAT_8c61c208,*(undefined4 *)(puVar20 + -0x10));
        }
        else {
          in_xd2 = *(double *)(puVar20 + -0x10);
          dVar27 = *DAT_8c61c208;
        }
        if (in_FPSCR_PR == '\0') {
          dVar27 = (double)CONCAT44((float)((ulonglong)dVar27 >> 0x20) * SUB84(dVar27,0),
                                    SUB84(dVar27,0));
        }
        else {
          dVar27 = dVar27 * dVar27;
        }
        if (in_FPSCR_SZ == '\0') {
          dVar27 = (double)CONCAT44((int)((ulonglong)dVar27 >> 0x20),*(undefined4 *)DAT_8c61c20c);
          dVar24 = (double)CONCAT44((int)((ulonglong)dVar24 >> 0x20),*DAT_8c61c210);
        }
        else {
          in_xd2 = *DAT_8c61c20c;
        }
        if (in_FPSCR_PR == '\0') {
          fVar23 = SUB84(dVar27,0) - (float)((ulonglong)dVar27 >> 0x20);
        }
        else {
          fVar23 = SUB84(dVar27 - dVar27,0);
        }
        if (in_FPSCR_SZ == '\0') {
          dVar27 = (double)CONCAT44(*(undefined4 *)(puVar20 + -0xc),fVar23);
        }
        else {
          dVar27 = *(double *)(puVar20 + -0xc);
        }
        if (in_FPSCR_PR == '\0') {
          dVar24 = (double)CONCAT44((int)((ulonglong)dVar24 >> 0x20),
                                    SUB84(dVar24,0) * (float)((ulonglong)dVar27 >> 0x20));
        }
        else {
          dVar24 = dVar24 * dVar27;
        }
        if (in_FPSCR_SZ == '\0') {
          dVar27 = (double)CONCAT44(*(undefined4 *)(puVar20 + -8),SUB84(dVar27,0));
        }
        else {
          dVar27 = *(double *)(puVar20 + -8);
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
                                    *(undefined4 *)(DAT_8c61c208 + 1));
        }
        fVar23 = (float)((ulonglong)dVar27 >> 0x20);
        if (in_FPSCR_PR == '\0') {
          in_dr0 = (double)CONCAT44((int)((ulonglong)dVar24 >> 0x20),SUB84(dVar24,0) * fVar23);
        }
        else {
          in_dr0 = dVar24 * dVar27;
        }
        param_1 = in_xd2;
        if (in_FPSCR_SZ == '\0') {
          param_1 = (double)CONCAT44(SUB84(dVar27,0),uVar26);
        }
        if (in_FPSCR_PR == '\0') {
          param_1 = (double)CONCAT44((float)((ulonglong)param_1 >> 0x20) - SUB84(in_dr0,0),
                                     SUB84(param_1,0));
          dVar24 = (double)CONCAT44(fVar23,(float)(int)psVar15[1] * in_fr15);
        }
        else {
          param_1 = param_1 - in_dr0;
          dVar24 = (double)CONCAT44(fVar23,(float)(int)psVar15[1]) *
                   (double)CONCAT44(in_fr14,in_fr15);
        }
        if (in_FPSCR_SZ == '\0') {
          pdVar9 = (double *)(unaff_r8 + 5);
          *(uint *)pdVar9 = SUB84(dVar24,0);
        }
        else {
          pdVar9 = (double *)(unaff_r8 + 4);
          *pdVar9 = in_xd2;
        }
        uVar26 = (undefined4)((ulonglong)dVar24 >> 0x20);
        if (in_FPSCR_PR == '\0') {
          dVar24 = (double)CONCAT44(uVar26,(float)(int)*psVar15 * in_fr15);
        }
        else {
          dVar24 = (double)CONCAT44(uVar26,(float)(int)*psVar15) * (double)CONCAT44(in_fr14,in_fr15)
          ;
        }
        if (in_FPSCR_SZ == '\0') {
          *(uint *)((int)pdVar9 + -4) = SUB84(dVar24,0);
          uVar5 = DAT_8c61c214;
          *(uint *)(pdVar9 + -1) = *(uint *)(pdVar19 + 3);
          *(uint *)((int)pdVar9 + -0xc) = *(uint *)((int)pdVar19 + 0x14);
          dVar24 = (double)CONCAT44((int)((ulonglong)dVar24 >> 0x20),*(uint *)(pdVar19 + 2));
          pdVar9 = pdVar9 + -2;
          *(uint *)pdVar9 = *(uint *)(pdVar19 + 2);
        }
        else {
          pdVar9[-1] = in_xd2;
          uVar5 = DAT_8c61c214;
          pdVar9[-2] = pdVar19[3];
          pdVar9[-3] = *(double *)((int)pdVar19 + 0x14);
          in_xd2 = pdVar19[2];
          pdVar9 = pdVar9 + -4;
          *pdVar9 = in_xd2;
        }
        *(uint *)((int)pdVar9 + -4) = uVar5;
        uVar26 = (undefined4)((ulonglong)dVar24 >> 0x20);
        if (in_FPSCR_PR == '\0') {
          dVar24 = (double)CONCAT44(uVar26,(float)(int)psVar15[3] * in_fr15);
        }
        else {
          dVar24 = (double)CONCAT44(uVar26,(float)(int)psVar15[3]) *
                   (double)CONCAT44(in_fr14,in_fr15);
        }
        if (in_FPSCR_SZ == '\0') {
          pdVar9 = pdVar9 + 6;
          *(uint *)pdVar9 = SUB84(dVar24,0);
        }
        else {
          pdVar9 = (double *)((int)pdVar9 + 0x2c);
          *pdVar9 = in_xd2;
        }
        uVar5 = DAT_8c61c214;
        uVar26 = (undefined4)((ulonglong)dVar24 >> 0x20);
        if (in_FPSCR_PR == '\0') {
          dVar24 = (double)CONCAT44(uVar26,(float)(int)psVar15[2] * in_fr15);
        }
        else {
          dVar24 = (double)CONCAT44(uVar26,(float)(int)psVar15[2]) *
                   (double)CONCAT44(in_fr14,in_fr15);
        }
        if (in_FPSCR_SZ == '\0') {
          *(uint *)((int)pdVar9 + -4) = SUB84(dVar24,0);
          *(uint *)(pdVar9 + -1) = *(uint *)(pdVar18 + 3);
          *(uint *)((int)pdVar9 + -0xc) = *(uint *)((int)pdVar18 + 0x14);
          dVar24 = (double)CONCAT44((int)((ulonglong)dVar24 >> 0x20),*(uint *)(pdVar18 + 2));
          pdVar9 = pdVar9 + -2;
          *(uint *)pdVar9 = *(uint *)(pdVar18 + 2);
        }
        else {
          pdVar9[-1] = in_xd2;
          pdVar9[-2] = pdVar18[3];
          pdVar9[-3] = *(double *)((int)pdVar18 + 0x14);
          in_xd2 = pdVar18[2];
          pdVar9 = pdVar9 + -4;
          *pdVar9 = in_xd2;
        }
        *(uint *)((int)pdVar9 + -4) = uVar5;
        if (in_FPSCR_SZ == '\0') {
          pdVar9 = (double *)((int)pdVar9 + 0x34);
          *(uint *)pdVar9 = (uint)((ulonglong)param_1 >> 0x20);
        }
        else {
          pdVar9 = pdVar9 + 6;
          *pdVar9 = param_1;
        }
        uVar26 = (undefined4)((ulonglong)dVar24 >> 0x20);
        if (in_FPSCR_PR == '\0') {
          dVar24 = (double)CONCAT44(uVar26,(float)(int)psVar15[5] * in_fr15);
        }
        else {
          dVar24 = (double)CONCAT44(uVar26,(float)(int)psVar15[5]) *
                   (double)CONCAT44(in_fr14,in_fr15);
        }
        if (in_FPSCR_SZ == '\0') {
          pdVar9 = (double *)((int)pdVar9 + -4);
          *(uint *)pdVar9 = SUB84(dVar24,0);
        }
        else {
          pdVar9 = pdVar9 + -1;
          *pdVar9 = in_xd2;
        }
        uVar5 = DAT_8c61c218;
        if (in_FPSCR_PR == '\0') {
          fVar23 = (float)(int)psVar15[4] * in_fr15;
        }
        else {
          fVar23 = SUB84((double)CONCAT44((int)((ulonglong)dVar24 >> 0x20),(float)(int)psVar15[4]) *
                         (double)CONCAT44(in_fr14,in_fr15),0);
        }
        if (in_FPSCR_SZ == '\0') {
          *(float *)((int)pdVar9 + -4) = fVar23;
          *(uint *)(pdVar9 + -1) = *(uint *)(pdVar17 + 3);
          *(uint *)((int)pdVar9 + -0xc) = *(uint *)((int)pdVar17 + 0x14);
          pdVar9 = pdVar9 + -2;
          *(uint *)pdVar9 = *(uint *)(pdVar17 + 2);
        }
        else {
          pdVar9[-1] = in_xd2;
          pdVar9[-2] = pdVar17[3];
          pdVar9[-3] = *(double *)((int)pdVar17 + 0x14);
          in_xd2 = pdVar17[2];
          pdVar9 = pdVar9 + -4;
          *pdVar9 = in_xd2;
        }
        *(uint *)((int)pdVar9 + -4) = uVar5;
        unaff_r8 = (uint *)((int)pdVar9 + 0x1c);
        *DAT_8c61c21c = *DAT_8c61c21c + 3;
      }
      psVar15 = psVar15 + 6;
    }
  }
  else if (uVar5 == (int)DAT_8c61bf06) {
    *_DAT_8c61bf0c = *_DAT_8c61bf0c + (uint)*(ushort *)(*(int *)(puVar20 + -0x18) + 2) * 4;
    *(undefined4 *)(puVar20 + -0x14) = 0;
    while( true ) {
      uVar26 = SUB84(param_1,0);
      uVar22 = SUB84(in_dr2,0);
      if ((int)(uint)*(ushort *)(*(int *)(puVar20 + -0x18) + 2) <= *(int *)(puVar20 + -0x14)) break;
      uVar2 = *puVar11;
      puVar14 = puVar11 + 1;
      puVar12 = puVar11 + 2;
      iVar7 = *DAT_8c61c220;
      puVar13 = puVar11 + 3;
      puVar11 = puVar11 + 4;
      pdVar18 = (double *)((uint)*puVar14 * 0x20 + iVar7);
      pdVar19 = (double *)((uint)uVar2 * 0x20 + iVar7);
      if (in_FPSCR_SZ == '\0') {
        uVar26 = *(undefined4 *)(pdVar18 + 2);
        uVar22 = *(undefined4 *)(pdVar19 + 2);
      }
      else {
        in_xd4 = pdVar18[2];
        in_xd2 = pdVar19[2];
      }
      dVar24 = in_xd4;
      if (in_FPSCR_SZ == '\0') {
        dVar24 = (double)CONCAT44(uVar26,uVar22);
      }
      if (in_FPSCR_PR == '\0') {
        dVar24 = (double)CONCAT44((float)((ulonglong)dVar24 >> 0x20) - SUB84(dVar24,0),
                                  SUB84(dVar24,0));
      }
      else {
        dVar24 = dVar24 - dVar24;
      }
      if (in_FPSCR_SZ == '\0') {
        dVar27 = (double)CONCAT44(*(undefined4 *)((int)pdVar18 + 0x14),uVar26);
      }
      else {
        dVar27 = *(double *)((int)pdVar18 + 0x14);
      }
      pdVar17 = (double *)((uint)*puVar12 * 0x20 + iVar7);
      fVar23 = (float)((ulonglong)dVar27 >> 0x20);
      if (in_FPSCR_SZ == '\0') {
        dVar24 = (double)CONCAT44((int)((ulonglong)dVar24 >> 0x20),
                                  *(undefined4 *)((int)pdVar17 + 0x14));
        in_dr0 = (double)CONCAT44((int)((ulonglong)in_dr0 >> 0x20),fVar23);
      }
      else {
        in_xd2 = *(double *)((int)pdVar17 + 0x14);
      }
      if (in_FPSCR_PR == '\0') {
        fVar23 = SUB84(dVar24,0) - fVar23;
        dVar24 = (double)CONCAT44((float)((ulonglong)dVar24 >> 0x20) * fVar23,fVar23);
      }
      else {
        dVar24 = (dVar24 - dVar27) * (dVar24 - dVar27);
      }
      if (in_FPSCR_SZ == '\0') {
        dVar24 = (double)CONCAT44((int)((ulonglong)dVar24 >> 0x20),
                                  *(undefined4 *)((int)pdVar19 + 0x14));
      }
      else {
        in_xd2 = *(double *)((int)pdVar19 + 0x14);
      }
      if (in_FPSCR_PR == '\0') {
        dVar21 = (double)CONCAT44((int)((ulonglong)in_dr0 >> 0x20),SUB84(in_dr0,0) - SUB84(dVar24,0)
                                 );
      }
      else {
        dVar21 = in_dr0 - dVar24;
      }
      if (in_FPSCR_SZ == '\0') {
        dVar24 = (double)CONCAT44((int)((ulonglong)dVar24 >> 0x20),*(undefined4 *)(pdVar17 + 2));
      }
      else {
        in_xd2 = pdVar17[2];
      }
      if (in_FPSCR_PR == '\0') {
        dVar24 = (double)CONCAT44((int)((ulonglong)dVar24 >> 0x20),SUB84(dVar24,0) - SUB84(dVar27,0)
                                 );
      }
      else {
        dVar24 = dVar24 - dVar27;
      }
      uVar5 = (uint)((ulonglong)dVar24 >> 0x20);
      param_1 = dVar24;
      if (in_FPSCR_SZ == '\0') {
        param_1 = (double)CONCAT44(uVar5,SUB84(dVar27,0));
      }
      if (in_FPSCR_PR == '\0') {
        in_dr0 = (double)CONCAT44((int)((ulonglong)dVar21 >> 0x20),SUB84(dVar21,0) * SUB84(dVar24,0)
                                 );
      }
      else {
        in_dr0 = dVar21 * dVar24;
      }
      in_dr2 = (double)((ulonglong)uVar5 << 0x20);
      if (in_FPSCR_PR == '\0') {
        fVar23 = (float)((ulonglong)param_1 >> 0x20) - SUB84(in_dr0,0);
        param_1 = (double)CONCAT44(fVar23,SUB84(param_1,0));
        bVar29 = 0.0 < fVar23;
      }
      else {
        param_1 = param_1 - in_dr0;
        bVar29 = in_dr2 < param_1;
      }
      uVar26 = SUB84(param_1,0);
      iVar7 = (uint)*puVar13 * 0x20 + iVar7;
      if (!bVar29) {
        if (in_FPSCR_SZ == '\0') {
          dVar24 = (double)CONCAT44(*(undefined4 *)pdVar19,*(undefined4 *)pdVar18);
        }
        else {
          dVar24 = *pdVar19;
          in_xd2 = *pdVar18;
        }
        if (in_FPSCR_PR == '\0') {
          dVar24 = (double)CONCAT44((float)((ulonglong)dVar24 >> 0x20) + SUB84(dVar24,0),
                                    SUB84(dVar24,0));
        }
        else {
          dVar24 = dVar24 + dVar24;
        }
        if (in_FPSCR_SZ == '\0') {
          dVar24 = (double)CONCAT44((int)((ulonglong)dVar24 >> 0x20),*(undefined4 *)pdVar17);
        }
        else {
          in_xd2 = *pdVar17;
        }
        if (in_FPSCR_PR == '\0') {
          dVar24 = (double)CONCAT44((float)((ulonglong)dVar24 >> 0x20) + SUB84(dVar24,0),
                                    SUB84(dVar24,0));
        }
        else {
          dVar24 = dVar24 + dVar24;
        }
        if (in_FPSCR_SZ == '\0') {
          *(int *)(puVar20 + -0x10) = (int)((ulonglong)dVar24 >> 0x20);
          dVar24 = (double)CONCAT44(*(undefined4 *)((int)pdVar19 + 4),
                                    *(undefined4 *)((int)pdVar18 + 4));
        }
        else {
          *(double *)(puVar20 + -0x10) = dVar24;
          dVar24 = *(double *)((int)pdVar19 + 4);
          in_xd2 = *(double *)((int)pdVar18 + 4);
        }
        if (in_FPSCR_PR == '\0') {
          dVar24 = (double)CONCAT44((float)((ulonglong)dVar24 >> 0x20) + SUB84(dVar24,0),
                                    SUB84(dVar24,0));
        }
        else {
          dVar24 = dVar24 + dVar24;
        }
        if (in_FPSCR_SZ == '\0') {
          dVar24 = (double)CONCAT44((int)((ulonglong)dVar24 >> 0x20),
                                    *(undefined4 *)((int)pdVar17 + 4));
        }
        else {
          in_xd2 = *(double *)((int)pdVar17 + 4);
        }
        if (in_FPSCR_PR == '\0') {
          dVar24 = (double)CONCAT44((float)((ulonglong)dVar24 >> 0x20) + SUB84(dVar24,0),
                                    SUB84(dVar24,0));
        }
        else {
          dVar24 = dVar24 + dVar24;
        }
        if (in_FPSCR_SZ == '\0') {
          *(int *)(puVar20 + -0xc) = (int)((ulonglong)dVar24 >> 0x20);
          dVar24 = (double)CONCAT44(*(undefined4 *)(pdVar19 + 1),*(undefined4 *)(pdVar18 + 1));
        }
        else {
          *(double *)(puVar20 + -0xc) = dVar24;
          dVar24 = pdVar19[1];
          in_xd2 = pdVar18[1];
        }
        if (in_FPSCR_PR == '\0') {
          dVar24 = (double)CONCAT44((float)((ulonglong)dVar24 >> 0x20) + SUB84(dVar24,0),
                                    SUB84(dVar24,0));
        }
        else {
          dVar24 = dVar24 + dVar24;
        }
        if (in_FPSCR_SZ == '\0') {
          dVar24 = (double)CONCAT44((int)((ulonglong)dVar24 >> 0x20),*(undefined4 *)(pdVar17 + 1));
        }
        else {
          in_xd2 = pdVar17[1];
        }
        if (in_FPSCR_PR == '\0') {
          dVar24 = (double)CONCAT44((float)((ulonglong)dVar24 >> 0x20) + SUB84(dVar24,0),
                                    SUB84(dVar24,0));
        }
        else {
          dVar24 = dVar24 + dVar24;
        }
        if (in_FPSCR_SZ == '\0') {
          *(int *)(puVar20 + -8) = (int)((ulonglong)dVar24 >> 0x20);
        }
        else {
          *(double *)(puVar20 + -8) = dVar24;
        }
        dVar24 = (double)(*(code *)PTR_FUN_8c61c204)(puVar20 + -0x10);
        if (in_FPSCR_SZ == '\0') {
          dVar27 = (double)CONCAT44(*(undefined4 *)DAT_8c61c208,*(undefined4 *)(puVar20 + -0x10));
        }
        else {
          in_xd2 = *(double *)(puVar20 + -0x10);
          dVar27 = *DAT_8c61c208;
        }
        if (in_FPSCR_PR == '\0') {
          dVar27 = (double)CONCAT44((float)((ulonglong)dVar27 >> 0x20) * SUB84(dVar27,0),
                                    SUB84(dVar27,0));
        }
        else {
          dVar27 = dVar27 * dVar27;
        }
        if (in_FPSCR_SZ == '\0') {
          dVar27 = (double)CONCAT44((int)((ulonglong)dVar27 >> 0x20),*(undefined4 *)DAT_8c61c20c);
          dVar24 = (double)CONCAT44((int)((ulonglong)dVar24 >> 0x20),*DAT_8c61c210);
        }
        else {
          in_xd2 = *DAT_8c61c20c;
        }
        if (in_FPSCR_PR == '\0') {
          fVar23 = SUB84(dVar27,0) - (float)((ulonglong)dVar27 >> 0x20);
        }
        else {
          fVar23 = SUB84(dVar27 - dVar27,0);
        }
        if (in_FPSCR_SZ == '\0') {
          dVar27 = (double)CONCAT44(*(undefined4 *)(puVar20 + -0xc),fVar23);
        }
        else {
          dVar27 = *(double *)(puVar20 + -0xc);
        }
        if (in_FPSCR_PR == '\0') {
          dVar24 = (double)CONCAT44((int)((ulonglong)dVar24 >> 0x20),
                                    SUB84(dVar24,0) * (float)((ulonglong)dVar27 >> 0x20));
        }
        else {
          dVar24 = dVar24 * dVar27;
        }
        if (in_FPSCR_SZ == '\0') {
          dVar27 = (double)CONCAT44(*(undefined4 *)(puVar20 + -8),SUB84(dVar27,0));
        }
        else {
          dVar27 = *(double *)(puVar20 + -8);
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
                                    *(undefined4 *)(DAT_8c61c208 + 1));
        }
        fVar23 = (float)((ulonglong)dVar27 >> 0x20);
        if (in_FPSCR_PR == '\0') {
          in_dr0 = (double)CONCAT44((int)((ulonglong)dVar24 >> 0x20),SUB84(dVar24,0) * fVar23);
        }
        else {
          in_dr0 = dVar24 * dVar27;
        }
        param_1 = in_xd2;
        if (in_FPSCR_SZ == '\0') {
          param_1 = (double)CONCAT44(SUB84(dVar27,0),uVar26);
        }
        if (in_FPSCR_PR == '\0') {
          param_1 = (double)CONCAT44((float)((ulonglong)param_1 >> 0x20) - SUB84(in_dr0,0),
                                     SUB84(param_1,0));
          dVar24 = (double)CONCAT44(fVar23,(float)(int)psVar15[1] * in_fr15);
        }
        else {
          param_1 = param_1 - in_dr0;
          dVar24 = (double)CONCAT44(fVar23,(float)(int)psVar15[1]) *
                   (double)CONCAT44(in_fr14,in_fr15);
        }
        if (in_FPSCR_SZ == '\0') {
          pdVar9 = (double *)(unaff_r8 + 5);
          *(uint *)pdVar9 = SUB84(dVar24,0);
        }
        else {
          pdVar9 = (double *)(unaff_r8 + 4);
          *pdVar9 = in_xd2;
        }
        uVar26 = (undefined4)((ulonglong)dVar24 >> 0x20);
        if (in_FPSCR_PR == '\0') {
          dVar24 = (double)CONCAT44(uVar26,(float)(int)*psVar15 * in_fr15);
        }
        else {
          dVar24 = (double)CONCAT44(uVar26,(float)(int)*psVar15) * (double)CONCAT44(in_fr14,in_fr15)
          ;
        }
        if (in_FPSCR_SZ == '\0') {
          *(uint *)((int)pdVar9 + -4) = SUB84(dVar24,0);
          uVar5 = DAT_8c61c214;
          *(uint *)(pdVar9 + -1) = *(uint *)(pdVar19 + 3);
          *(uint *)((int)pdVar9 + -0xc) = *(uint *)((int)pdVar19 + 0x14);
          dVar24 = (double)CONCAT44((int)((ulonglong)dVar24 >> 0x20),*(uint *)(pdVar19 + 2));
          pdVar9 = pdVar9 + -2;
          *(uint *)pdVar9 = *(uint *)(pdVar19 + 2);
        }
        else {
          pdVar9[-1] = in_xd2;
          uVar5 = DAT_8c61c214;
          pdVar9[-2] = pdVar19[3];
          pdVar9[-3] = *(double *)((int)pdVar19 + 0x14);
          in_xd2 = pdVar19[2];
          pdVar9 = pdVar9 + -4;
          *pdVar9 = in_xd2;
        }
        *(uint *)((int)pdVar9 + -4) = uVar5;
        uVar26 = (undefined4)((ulonglong)dVar24 >> 0x20);
        if (in_FPSCR_PR == '\0') {
          dVar24 = (double)CONCAT44(uVar26,(float)(int)psVar15[3] * in_fr15);
        }
        else {
          dVar24 = (double)CONCAT44(uVar26,(float)(int)psVar15[3]) *
                   (double)CONCAT44(in_fr14,in_fr15);
        }
        if (in_FPSCR_SZ == '\0') {
          pdVar9 = pdVar9 + 6;
          *(uint *)pdVar9 = SUB84(dVar24,0);
        }
        else {
          pdVar9 = (double *)((int)pdVar9 + 0x2c);
          *pdVar9 = in_xd2;
        }
        uVar5 = DAT_8c61c214;
        uVar26 = (undefined4)((ulonglong)dVar24 >> 0x20);
        if (in_FPSCR_PR == '\0') {
          dVar24 = (double)CONCAT44(uVar26,(float)(int)psVar15[2] * in_fr15);
        }
        else {
          dVar24 = (double)CONCAT44(uVar26,(float)(int)psVar15[2]) *
                   (double)CONCAT44(in_fr14,in_fr15);
        }
        if (in_FPSCR_SZ == '\0') {
          *(uint *)((int)pdVar9 + -4) = SUB84(dVar24,0);
          *(uint *)(pdVar9 + -1) = *(uint *)(pdVar18 + 3);
          *(uint *)((int)pdVar9 + -0xc) = *(uint *)((int)pdVar18 + 0x14);
          dVar24 = (double)CONCAT44((int)((ulonglong)dVar24 >> 0x20),*(uint *)(pdVar18 + 2));
          pdVar9 = pdVar9 + -2;
          *(uint *)pdVar9 = *(uint *)(pdVar18 + 2);
        }
        else {
          pdVar9[-1] = in_xd2;
          pdVar9[-2] = pdVar18[3];
          pdVar9[-3] = *(double *)((int)pdVar18 + 0x14);
          in_xd2 = pdVar18[2];
          pdVar9 = pdVar9 + -4;
          *pdVar9 = in_xd2;
        }
        *(uint *)((int)pdVar9 + -4) = uVar5;
        uVar5 = (uint)((ulonglong)param_1 >> 0x20);
        if (in_FPSCR_SZ == '\0') {
          pdVar9 = (double *)((int)pdVar9 + 0x34);
          *(uint *)pdVar9 = uVar5;
        }
        else {
          pdVar9 = pdVar9 + 6;
          *pdVar9 = param_1;
        }
        uVar26 = (undefined4)((ulonglong)dVar24 >> 0x20);
        if (in_FPSCR_PR == '\0') {
          dVar24 = (double)CONCAT44(uVar26,(float)(int)psVar15[5] * in_fr15);
        }
        else {
          dVar24 = (double)CONCAT44(uVar26,(float)(int)psVar15[5]) *
                   (double)CONCAT44(in_fr14,in_fr15);
        }
        if (in_FPSCR_SZ == '\0') {
          pdVar9 = (double *)((int)pdVar9 + -4);
          *(uint *)pdVar9 = SUB84(dVar24,0);
        }
        else {
          pdVar9 = pdVar9 + -1;
          *pdVar9 = in_xd2;
        }
        uVar25 = DAT_8c61c214;
        uVar26 = (undefined4)((ulonglong)dVar24 >> 0x20);
        if (in_FPSCR_PR == '\0') {
          dVar24 = (double)CONCAT44(uVar26,(float)(int)psVar15[4] * in_fr15);
        }
        else {
          dVar24 = (double)CONCAT44(uVar26,(float)(int)psVar15[4]) *
                   (double)CONCAT44(in_fr14,in_fr15);
        }
        if (in_FPSCR_SZ == '\0') {
          *(uint *)((int)pdVar9 + -4) = SUB84(dVar24,0);
          *(uint *)(pdVar9 + -1) = *(uint *)(pdVar17 + 3);
          *(uint *)((int)pdVar9 + -0xc) = *(uint *)((int)pdVar17 + 0x14);
          dVar24 = (double)CONCAT44((int)((ulonglong)dVar24 >> 0x20),*(uint *)(pdVar17 + 2));
          pdVar9 = pdVar9 + -2;
          *(uint *)pdVar9 = *(uint *)(pdVar17 + 2);
        }
        else {
          pdVar9[-1] = in_xd2;
          pdVar9[-2] = pdVar17[3];
          pdVar9[-3] = *(double *)((int)pdVar17 + 0x14);
          in_xd2 = pdVar17[2];
          pdVar9 = pdVar9 + -4;
          *pdVar9 = in_xd2;
        }
        *(uint *)((int)pdVar9 + -4) = uVar25;
        if (in_FPSCR_SZ == '\0') {
          pdVar9 = (double *)((int)pdVar9 + 0x34);
          *(uint *)pdVar9 = uVar5;
        }
        else {
          pdVar9 = pdVar9 + 6;
          *pdVar9 = param_1;
        }
        uVar26 = (undefined4)((ulonglong)dVar24 >> 0x20);
        if (in_FPSCR_PR == '\0') {
          dVar24 = (double)CONCAT44(uVar26,(float)(int)psVar15[7] * in_fr15);
        }
        else {
          dVar24 = (double)CONCAT44(uVar26,(float)(int)psVar15[7]) *
                   (double)CONCAT44(in_fr14,in_fr15);
        }
        if (in_FPSCR_SZ == '\0') {
          pdVar9 = (double *)((int)pdVar9 + -4);
          *(uint *)pdVar9 = SUB84(dVar24,0);
        }
        else {
          pdVar9 = pdVar9 + -1;
          *pdVar9 = in_xd2;
        }
        uVar5 = DAT_8c61c218;
        if (in_FPSCR_PR == '\0') {
          in_dr2 = (double)(ulonglong)(uint)((float)(int)psVar15[6] * in_fr15);
        }
        else {
          in_dr2 = (double)CONCAT44((int)((ulonglong)dVar24 >> 0x20),(float)(int)psVar15[6]) *
                   (double)CONCAT44(in_fr14,in_fr15);
        }
        if (in_FPSCR_SZ == '\0') {
          *(uint *)((int)pdVar9 + -4) = SUB84(in_dr2,0);
          *(uint *)(pdVar9 + -1) = *(uint *)(iVar7 + 0x18);
          *(uint *)((int)pdVar9 + -0xc) = *(uint *)(iVar7 + 0x14);
          in_dr2 = (double)(ulonglong)*(uint *)(iVar7 + 0x10);
          pdVar9 = pdVar9 + -2;
          *(uint *)pdVar9 = *(uint *)(iVar7 + 0x10);
        }
        else {
          pdVar9[-1] = in_xd2;
          pdVar9[-2] = *(double *)(iVar7 + 0x18);
          pdVar9[-3] = *(double *)(iVar7 + 0x14);
          in_xd2 = *(double *)(iVar7 + 0x10);
          pdVar9 = pdVar9 + -4;
          *pdVar9 = in_xd2;
        }
        *(uint *)((int)pdVar9 + -4) = uVar5;
        unaff_r8 = (uint *)((int)pdVar9 + 0x1c);
        *DAT_8c61c21c = *DAT_8c61c21c + 4;
      }
      psVar15 = psVar15 + 8;
      *(int *)(puVar20 + -0x14) = *(int *)(puVar20 + -0x14) + 1;
    }
  }
  else if (uVar5 == DAT_8c61bf14) {
    for (iVar7 = 0; uVar26 = SUB84(param_1,0),
        iVar7 < (int)(uint)*(ushort *)(*(int *)(puVar20 + -0x18) + 2); iVar7 = iVar7 + 1) {
      puVar14 = puVar11 + 1;
      uVar5 = (uint)*puVar11;
      *piVar3 = *piVar3 + uVar5;
      iVar8 = *_DAT_8c61c48c;
      pdVar18 = (double *)((uint)puVar11[2] * 0x20 + iVar8);
      if (in_FPSCR_SZ == '\0') {
        uVar26 = *(undefined4 *)(pdVar18 + 2);
      }
      else {
        in_xd4 = pdVar18[2];
      }
      pdVar19 = (double *)((uint)*puVar14 * 0x20 + iVar8);
      if (in_FPSCR_SZ == '\0') {
        dVar24 = (double)CONCAT44(uVar26,*(undefined4 *)(pdVar19 + 2));
      }
      else {
        in_xd2 = pdVar19[2];
        dVar24 = in_xd4;
      }
      if (in_FPSCR_PR == '\0') {
        dVar24 = (double)CONCAT44((float)((ulonglong)dVar24 >> 0x20) - SUB84(dVar24,0),
                                  SUB84(dVar24,0));
      }
      else {
        dVar24 = dVar24 - dVar24;
      }
      if (in_FPSCR_SZ == '\0') {
        dVar27 = (double)CONCAT44(*(undefined4 *)((int)pdVar18 + 0x14),uVar26);
      }
      else {
        dVar27 = *(double *)((int)pdVar18 + 0x14);
      }
      pdVar17 = (double *)((uint)puVar11[3] * 0x20 + iVar8);
      fVar23 = (float)((ulonglong)dVar27 >> 0x20);
      if (in_FPSCR_SZ == '\0') {
        dVar24 = (double)CONCAT44((int)((ulonglong)dVar24 >> 0x20),
                                  *(undefined4 *)((int)pdVar17 + 0x14));
        in_dr0 = (double)CONCAT44((int)((ulonglong)in_dr0 >> 0x20),fVar23);
      }
      else {
        in_xd2 = *(double *)((int)pdVar17 + 0x14);
      }
      if (in_FPSCR_PR == '\0') {
        fVar23 = SUB84(dVar24,0) - fVar23;
        dVar24 = (double)CONCAT44((float)((ulonglong)dVar24 >> 0x20) * fVar23,fVar23);
      }
      else {
        dVar24 = (dVar24 - dVar27) * (dVar24 - dVar27);
      }
      if (in_FPSCR_SZ == '\0') {
        dVar24 = (double)CONCAT44((int)((ulonglong)dVar24 >> 0x20),
                                  *(undefined4 *)((int)pdVar19 + 0x14));
      }
      else {
        in_xd2 = *(double *)((int)pdVar19 + 0x14);
      }
      if (in_FPSCR_PR == '\0') {
        dVar21 = (double)CONCAT44((int)((ulonglong)in_dr0 >> 0x20),SUB84(in_dr0,0) - SUB84(dVar24,0)
                                 );
      }
      else {
        dVar21 = in_dr0 - dVar24;
      }
      if (in_FPSCR_SZ == '\0') {
        dVar24 = (double)CONCAT44((int)((ulonglong)dVar24 >> 0x20),*(undefined4 *)(pdVar17 + 2));
      }
      else {
        in_xd2 = pdVar17[2];
      }
      if (in_FPSCR_PR == '\0') {
        dVar24 = (double)CONCAT44((int)((ulonglong)dVar24 >> 0x20),SUB84(dVar24,0) - SUB84(dVar27,0)
                                 );
      }
      else {
        dVar24 = dVar24 - dVar27;
      }
      uVar25 = (uint)((ulonglong)dVar24 >> 0x20);
      param_1 = dVar24;
      if (in_FPSCR_SZ == '\0') {
        param_1 = (double)CONCAT44(uVar25,SUB84(dVar27,0));
      }
      if (in_FPSCR_PR == '\0') {
        in_dr0 = (double)CONCAT44((int)((ulonglong)dVar21 >> 0x20),SUB84(dVar21,0) * SUB84(dVar24,0)
                                 );
      }
      else {
        in_dr0 = dVar21 * dVar24;
      }
      if (in_FPSCR_PR == '\0') {
        fVar23 = (float)((ulonglong)param_1 >> 0x20) - SUB84(in_dr0,0);
        param_1 = (double)CONCAT44(fVar23,SUB84(param_1,0));
        bVar29 = 0.0 < fVar23;
      }
      else {
        param_1 = param_1 - in_dr0;
        bVar29 = (double)((ulonglong)uVar25 << 0x20) < param_1;
      }
      uVar26 = SUB84(param_1,0);
      if (bVar29) {
        puVar11 = puVar14 + uVar5;
        psVar15 = psVar15 + uVar5 * 2;
      }
      else {
        *DAT_8c61c490 = *DAT_8c61c490 + uVar5;
        if (in_FPSCR_SZ == '\0') {
          dVar24 = (double)CONCAT44(*(undefined4 *)pdVar19,*(undefined4 *)pdVar18);
        }
        else {
          in_xd2 = *pdVar18;
          dVar24 = *pdVar19;
        }
        if (in_FPSCR_PR == '\0') {
          dVar24 = (double)CONCAT44((float)((ulonglong)dVar24 >> 0x20) + SUB84(dVar24,0),
                                    SUB84(dVar24,0));
        }
        else {
          dVar24 = dVar24 + dVar24;
        }
        if (in_FPSCR_SZ == '\0') {
          dVar24 = (double)CONCAT44((int)((ulonglong)dVar24 >> 0x20),*(undefined4 *)pdVar17);
        }
        else {
          in_xd2 = *pdVar17;
        }
        if (in_FPSCR_PR == '\0') {
          dVar24 = (double)CONCAT44((float)((ulonglong)dVar24 >> 0x20) + SUB84(dVar24,0),
                                    SUB84(dVar24,0));
        }
        else {
          dVar24 = dVar24 + dVar24;
        }
        if (in_FPSCR_SZ == '\0') {
          *(int *)(puVar20 + -0x10) = (int)((ulonglong)dVar24 >> 0x20);
          dVar24 = (double)CONCAT44(*(undefined4 *)((int)pdVar19 + 4),
                                    *(undefined4 *)((int)pdVar18 + 4));
        }
        else {
          *(double *)(puVar20 + -0x10) = dVar24;
          dVar24 = *(double *)((int)pdVar19 + 4);
          in_xd2 = *(double *)((int)pdVar18 + 4);
        }
        if (in_FPSCR_PR == '\0') {
          dVar24 = (double)CONCAT44((float)((ulonglong)dVar24 >> 0x20) + SUB84(dVar24,0),
                                    SUB84(dVar24,0));
        }
        else {
          dVar24 = dVar24 + dVar24;
        }
        if (in_FPSCR_SZ == '\0') {
          dVar24 = (double)CONCAT44((int)((ulonglong)dVar24 >> 0x20),
                                    *(undefined4 *)((int)pdVar17 + 4));
        }
        else {
          in_xd2 = *(double *)((int)pdVar17 + 4);
        }
        if (in_FPSCR_PR == '\0') {
          dVar24 = (double)CONCAT44((float)((ulonglong)dVar24 >> 0x20) + SUB84(dVar24,0),
                                    SUB84(dVar24,0));
        }
        else {
          dVar24 = dVar24 + dVar24;
        }
        if (in_FPSCR_SZ == '\0') {
          *(int *)(puVar20 + -0xc) = (int)((ulonglong)dVar24 >> 0x20);
          dVar24 = (double)CONCAT44(*(undefined4 *)(pdVar19 + 1),*(undefined4 *)(pdVar18 + 1));
        }
        else {
          *(double *)(puVar20 + -0xc) = dVar24;
          in_xd2 = pdVar18[1];
          dVar24 = pdVar19[1];
        }
        if (in_FPSCR_PR == '\0') {
          dVar24 = (double)CONCAT44((float)((ulonglong)dVar24 >> 0x20) + SUB84(dVar24,0),
                                    SUB84(dVar24,0));
        }
        else {
          dVar24 = dVar24 + dVar24;
        }
        if (in_FPSCR_SZ == '\0') {
          dVar24 = (double)CONCAT44((int)((ulonglong)dVar24 >> 0x20),*(undefined4 *)(pdVar17 + 1));
        }
        else {
          in_xd2 = pdVar17[1];
        }
        if (in_FPSCR_PR == '\0') {
          dVar24 = (double)CONCAT44((float)((ulonglong)dVar24 >> 0x20) + SUB84(dVar24,0),
                                    SUB84(dVar24,0));
        }
        else {
          dVar24 = dVar24 + dVar24;
        }
        if (in_FPSCR_SZ == '\0') {
          *(int *)(puVar20 + -8) = (int)((ulonglong)dVar24 >> 0x20);
        }
        else {
          *(double *)(puVar20 + -8) = dVar24;
        }
        dVar24 = (double)(*(code *)PTR_FUN_8c61c494)(puVar20 + -0x10);
        if (in_FPSCR_SZ == '\0') {
          dVar27 = (double)CONCAT44(*(undefined4 *)DAT_8c61c498,*(undefined4 *)(puVar20 + -0x10));
        }
        else {
          in_xd2 = *(double *)(puVar20 + -0x10);
          dVar27 = *DAT_8c61c498;
        }
        if (in_FPSCR_PR == '\0') {
          dVar27 = (double)CONCAT44((float)((ulonglong)dVar27 >> 0x20) * SUB84(dVar27,0),
                                    SUB84(dVar27,0));
        }
        else {
          dVar27 = dVar27 * dVar27;
        }
        if (in_FPSCR_SZ == '\0') {
          dVar27 = (double)CONCAT44((int)((ulonglong)dVar27 >> 0x20),*(undefined4 *)DAT_8c61c49c);
          dVar24 = (double)CONCAT44((int)((ulonglong)dVar24 >> 0x20),*DAT_8c61c4a0);
        }
        else {
          in_xd2 = *DAT_8c61c49c;
        }
        if (in_FPSCR_PR == '\0') {
          fVar23 = SUB84(dVar27,0) - (float)((ulonglong)dVar27 >> 0x20);
        }
        else {
          fVar23 = SUB84(dVar27 - dVar27,0);
        }
        if (in_FPSCR_SZ == '\0') {
          dVar27 = (double)CONCAT44(*(undefined4 *)(puVar20 + -0xc),fVar23);
        }
        else {
          dVar27 = *(double *)(puVar20 + -0xc);
        }
        if (in_FPSCR_PR == '\0') {
          dVar24 = (double)CONCAT44((int)((ulonglong)dVar24 >> 0x20),
                                    SUB84(dVar24,0) * (float)((ulonglong)dVar27 >> 0x20));
        }
        else {
          dVar24 = dVar24 * dVar27;
        }
        if (in_FPSCR_SZ == '\0') {
          dVar27 = (double)CONCAT44(*(undefined4 *)(puVar20 + -8),SUB84(dVar27,0));
        }
        else {
          dVar27 = *(double *)(puVar20 + -8);
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
                                    *(undefined4 *)(DAT_8c61c498 + 1));
        }
        fVar23 = (float)((ulonglong)dVar27 >> 0x20);
        if (in_FPSCR_PR == '\0') {
          in_dr0 = (double)CONCAT44((int)((ulonglong)dVar24 >> 0x20),SUB84(dVar24,0) * fVar23);
        }
        else {
          in_dr0 = dVar24 * dVar27;
        }
        iVar8 = (uint)*puVar14 * 0x20 + *_DAT_8c61c48c;
        param_1 = in_xd2;
        if (in_FPSCR_SZ == '\0') {
          param_1 = (double)CONCAT44(SUB84(dVar27,0),uVar26);
        }
        if (in_FPSCR_PR == '\0') {
          param_1 = (double)CONCAT44((float)((ulonglong)param_1 >> 0x20) - SUB84(in_dr0,0),
                                     SUB84(param_1,0));
        }
        else {
          param_1 = param_1 - in_dr0;
        }
        if (in_FPSCR_PR == '\0') {
          dVar24 = (double)CONCAT44(fVar23,(float)(int)psVar15[1] * in_fr15);
        }
        else {
          dVar24 = (double)CONCAT44(fVar23,(float)(int)psVar15[1]) *
                   (double)CONCAT44(in_fr14,in_fr15);
        }
        if (in_FPSCR_SZ == '\0') {
          pdVar18 = (double *)(unaff_r8 + 5);
          *(uint *)pdVar18 = SUB84(dVar24,0);
        }
        else {
          pdVar18 = (double *)(unaff_r8 + 4);
          *pdVar18 = in_xd2;
        }
        uVar25 = DAT_8c61c4a4;
        uVar26 = (undefined4)((ulonglong)dVar24 >> 0x20);
        if (in_FPSCR_PR == '\0') {
          dVar24 = (double)CONCAT44(uVar26,(float)(int)*psVar15 * in_fr15);
        }
        else {
          dVar24 = (double)CONCAT44(uVar26,(float)(int)*psVar15) * (double)CONCAT44(in_fr14,in_fr15)
          ;
        }
        if (in_FPSCR_SZ == '\0') {
          *(uint *)((int)pdVar18 + -4) = SUB84(dVar24,0);
          *(uint *)(pdVar18 + -1) = *(uint *)(iVar8 + 0x18);
          *(uint *)((int)pdVar18 + -0xc) = *(uint *)(iVar8 + 0x14);
          dVar24 = (double)CONCAT44((int)((ulonglong)dVar24 >> 0x20),*(uint *)(iVar8 + 0x10));
          pdVar18 = pdVar18 + -2;
          *(uint *)pdVar18 = *(uint *)(iVar8 + 0x10);
        }
        else {
          pdVar18[-1] = in_xd2;
          pdVar18[-2] = *(double *)(iVar8 + 0x18);
          pdVar18[-3] = *(double *)(iVar8 + 0x14);
          in_xd2 = *(double *)(iVar8 + 0x10);
          pdVar18 = pdVar18 + -4;
          *pdVar18 = in_xd2;
        }
        *(uint *)((int)pdVar18 + -4) = uVar25;
        puVar14 = puVar11 + 3;
        iVar8 = (uint)puVar11[2] * 0x20 + *_DAT_8c61c48c;
        uVar26 = (undefined4)((ulonglong)dVar24 >> 0x20);
        if (in_FPSCR_PR == '\0') {
          dVar24 = (double)CONCAT44(uVar26,(float)(int)psVar15[3] * in_fr15);
        }
        else {
          dVar24 = (double)CONCAT44(uVar26,(float)(int)psVar15[3]) *
                   (double)CONCAT44(in_fr14,in_fr15);
        }
        if (in_FPSCR_SZ == '\0') {
          pdVar18 = pdVar18 + 6;
          *(uint *)pdVar18 = SUB84(dVar24,0);
        }
        else {
          pdVar18 = (double *)((int)pdVar18 + 0x2c);
          *pdVar18 = in_xd2;
        }
        uVar25 = DAT_8c61c4a4;
        psVar16 = psVar15 + 4;
        uVar26 = (undefined4)((ulonglong)dVar24 >> 0x20);
        if (in_FPSCR_PR == '\0') {
          dVar24 = (double)CONCAT44(uVar26,(float)(int)psVar15[2] * in_fr15);
        }
        else {
          dVar24 = (double)CONCAT44(uVar26,(float)(int)psVar15[2]) *
                   (double)CONCAT44(in_fr14,in_fr15);
        }
        if (in_FPSCR_SZ == '\0') {
          *(uint *)((int)pdVar18 + -4) = SUB84(dVar24,0);
          *(uint *)(pdVar18 + -1) = *(uint *)(iVar8 + 0x18);
          *(uint *)((int)pdVar18 + -0xc) = *(uint *)(iVar8 + 0x14);
          dVar24 = (double)CONCAT44((int)((ulonglong)dVar24 >> 0x20),*(uint *)(iVar8 + 0x10));
          pdVar18 = pdVar18 + -2;
          *(uint *)pdVar18 = *(uint *)(iVar8 + 0x10);
        }
        else {
          pdVar18[-1] = in_xd2;
          pdVar18[-2] = *(double *)(iVar8 + 0x18);
          pdVar18[-3] = *(double *)(iVar8 + 0x14);
          in_xd2 = *(double *)(iVar8 + 0x10);
          pdVar18 = pdVar18 + -4;
          *pdVar18 = in_xd2;
        }
        uVar26 = (undefined4)((ulonglong)dVar24 >> 0x20);
        *(uint *)((int)pdVar18 + -4) = uVar25;
        iVar8 = 2;
        puVar10 = (uint *)((int)pdVar18 + 0x1c);
        uVar25 = (uint)((ulonglong)param_1 >> 0x20);
        if (2 < (int)(uVar5 - 1)) {
          do {
            uVar2 = *puVar14;
            puVar14 = puVar14 + 1;
            iVar6 = *_DAT_8c61c48c;
            if (in_FPSCR_SZ == '\0') {
              pdVar18 = (double *)(puVar10 + 6);
              *(uint *)pdVar18 = uVar25;
            }
            else {
              pdVar18 = (double *)(puVar10 + 5);
              *pdVar18 = param_1;
            }
            iVar6 = (uint)uVar2 * 0x20 + iVar6;
            uVar26 = (undefined4)((ulonglong)dVar24 >> 0x20);
            if (in_FPSCR_PR == '\0') {
              dVar24 = (double)CONCAT44(uVar26,(float)(int)psVar16[1] * in_fr15);
            }
            else {
              dVar24 = (double)CONCAT44(uVar26,(float)(int)psVar16[1]) *
                       (double)CONCAT44(in_fr14,in_fr15);
            }
            if (in_FPSCR_SZ == '\0') {
              pdVar18 = (double *)((int)pdVar18 + -4);
              *(uint *)pdVar18 = SUB84(dVar24,0);
            }
            else {
              pdVar18 = pdVar18 + -1;
              *pdVar18 = in_xd2;
            }
            uVar4 = DAT_8c61c4a4;
            sVar1 = *psVar16;
            psVar16 = psVar16 + 2;
            uVar26 = (undefined4)((ulonglong)dVar24 >> 0x20);
            if (in_FPSCR_PR == '\0') {
              dVar24 = (double)CONCAT44(uVar26,(float)(int)sVar1 * in_fr15);
            }
            else {
              dVar24 = (double)CONCAT44(uVar26,(float)(int)sVar1) *
                       (double)CONCAT44(in_fr14,in_fr15);
            }
            if (in_FPSCR_SZ == '\0') {
              *(uint *)((int)pdVar18 + -4) = SUB84(dVar24,0);
              *(uint *)(pdVar18 + -1) = *(uint *)(iVar6 + 0x18);
              *(uint *)((int)pdVar18 + -0xc) = *(uint *)(iVar6 + 0x14);
              dVar24 = (double)CONCAT44((int)((ulonglong)dVar24 >> 0x20),*(uint *)(iVar6 + 0x10));
              pdVar18 = pdVar18 + -2;
              *(uint *)pdVar18 = *(uint *)(iVar6 + 0x10);
            }
            else {
              pdVar18[-1] = in_xd2;
              pdVar18[-2] = *(double *)(iVar6 + 0x18);
              pdVar18[-3] = *(double *)(iVar6 + 0x14);
              in_xd2 = *(double *)(iVar6 + 0x10);
              pdVar18 = pdVar18 + -4;
              *pdVar18 = in_xd2;
            }
            uVar26 = (undefined4)((ulonglong)dVar24 >> 0x20);
            *(uint *)((int)pdVar18 + -4) = uVar4;
            iVar8 = iVar8 + 1;
            puVar10 = (uint *)((int)pdVar18 + 0x1c);
          } while (iVar8 < (int)(uVar5 - 1));
        }
        uVar2 = *puVar14;
        puVar11 = puVar14 + 1;
        iVar8 = *_DAT_8c61c48c;
        if (in_FPSCR_SZ == '\0') {
          pdVar18 = (double *)(puVar10 + 6);
          *(uint *)pdVar18 = uVar25;
        }
        else {
          pdVar18 = (double *)(puVar10 + 5);
          *pdVar18 = param_1;
        }
        iVar8 = (uint)uVar2 * 0x20 + iVar8;
        if (in_FPSCR_PR == '\0') {
          dVar24 = (double)CONCAT44(uVar26,(float)(int)psVar16[1] * in_fr15);
        }
        else {
          dVar24 = (double)CONCAT44(uVar26,(float)(int)psVar16[1]) *
                   (double)CONCAT44(in_fr14,in_fr15);
        }
        if (in_FPSCR_SZ == '\0') {
          pdVar18 = (double *)((int)pdVar18 + -4);
          *(uint *)pdVar18 = SUB84(dVar24,0);
        }
        else {
          pdVar18 = pdVar18 + -1;
          *pdVar18 = in_xd2;
        }
        uVar5 = DAT_8c61c4a8;
        psVar15 = psVar16 + 2;
        if (in_FPSCR_PR == '\0') {
          fVar23 = (float)(int)*psVar16 * in_fr15;
        }
        else {
          fVar23 = SUB84((double)CONCAT44((int)((ulonglong)dVar24 >> 0x20),(float)(int)*psVar16) *
                         (double)CONCAT44(in_fr14,in_fr15),0);
        }
        if (in_FPSCR_SZ == '\0') {
          *(float *)((int)pdVar18 + -4) = fVar23;
          *(uint *)(pdVar18 + -1) = *(uint *)(iVar8 + 0x18);
          *(uint *)((int)pdVar18 + -0xc) = *(uint *)(iVar8 + 0x14);
          pdVar18 = pdVar18 + -2;
          *(uint *)pdVar18 = *(uint *)(iVar8 + 0x10);
        }
        else {
          pdVar18[-1] = in_xd2;
          pdVar18[-2] = *(double *)(iVar8 + 0x18);
          pdVar18[-3] = *(double *)(iVar8 + 0x14);
          in_xd2 = *(double *)(iVar8 + 0x10);
          pdVar18 = pdVar18 + -4;
          *pdVar18 = in_xd2;
        }
        *(uint *)((int)pdVar18 + -4) = uVar5;
        unaff_r8 = (uint *)((int)pdVar18 + 0x1c);
      }
    }
  }
  else if (uVar5 == DAT_8c61bf10) {
    *DAT_8c61c4ac = *(undefined4 *)(puVar20 + -0x18);
    if (in_FPSCR_SZ == '\0') {
      *(int *)(puVar20 + -0x1c) = (int)((ulonglong)in_dr0 >> 0x20);
      *(int *)(puVar20 + -0x20) = SUB84(in_dr0,0);
      *(int *)(puVar20 + -0x24) = (int)((ulonglong)in_dr2 >> 0x20);
      *(int *)(puVar20 + -0x28) = SUB84(in_dr2,0);
      *(int *)(puVar20 + -0x2c) = (int)((ulonglong)param_1 >> 0x20);
      *(int *)(puVar20 + -0x30) = SUB84(param_1,0);
      uVar28 = CONCAT44(DAT_8c61c488,SUB84(param_1,0));
    }
    else {
      *(double *)(puVar20 + -0x20) = in_dr0;
      *(undefined8 *)(puVar20 + -0x28) = in_xd0;
      *(double *)(puVar20 + -0x30) = in_dr2;
      *(double *)(puVar20 + -0x38) = in_xd2;
      *(double *)(puVar20 + -0x40) = param_1;
      *(double *)(puVar20 + -0x48) = in_xd4;
      uVar28 = CONCAT44(_DAT_8c61c48c,DAT_8c61c488);
    }
    (*(code *)PTR_FUN_8c61c4b0)(uVar28,*DAT_8c61c4ac);
  }
  return;
}

