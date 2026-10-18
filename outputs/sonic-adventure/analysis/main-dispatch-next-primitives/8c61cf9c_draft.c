/* Ghidra draft only; not buildable/verified source. */

/* WARNING: Removing unreachable block (ram,0x8c61d0e0) */
/* WARNING: Removing unreachable block (ram,0x8c61d086) */
/* WARNING: Removing unreachable block (ram,0x8c61d1e4) */
/* WARNING: Removing unreachable block (ram,0x8c61d23e) */
/* WARNING: Removing unreachable block (ram,0x8c61d1d6) */
/* WARNING: Removing unreachable block (ram,0x8c61d1b0) */
/* WARNING: Removing unreachable block (ram,0x8c61d1a2) */
/* WARNING: Removing unreachable block (ram,0x8c61d32e) */
/* WARNING: Removing unreachable block (ram,0x8c61d33c) */
/* WARNING: Removing unreachable block (ram,0x8c61d078) */
/* WARNING: Removing unreachable block (ram,0x8c61d0ac) */
/* WARNING: Removing unreachable block (ram,0x8c61d0ba) */
/* WARNING: Removing unreachable block (ram,0x8c61d388) */
/* WARNING: Removing unreachable block (ram,0x8c61d37a) */
/* WARNING: Removing unreachable block (ram,0x8c61d20a) */
/* WARNING: Removing unreachable block (ram,0x8c61d218) */
/* WARNING: Removing unreachable block (ram,0x8c61d0ee) */
/* WARNING: Removing unreachable block (ram,0x8c61d24c) */
/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_8c61cf9c(double param_1,ushort *param_2)

{
  short sVar1;
  ushort uVar2;
  int *piVar3;
  int *piVar4;
  uint uVar5;
  int iVar6;
  int iVar7;
  int iVar8;
  uint *unaff_r8;
  uint *puVar9;
  int iVar10;
  int iVar11;
  ushort *puVar12;
  ushort *puVar13;
  ushort *puVar14;
  ushort *puVar15;
  short *psVar16;
  short *psVar17;
  double in_dr0;
  double dVar18;
  undefined4 uVar19;
  float fVar20;
  double in_dr2;
  double dVar21;
  uint uVar22;
  double dVar23;
  undefined4 uVar24;
  float fVar25;
  int *piVar27;
  double dVar26;
  undefined8 in_xd2;
  double in_xd4;
  undefined8 in_xd6;
  bool bVar28;
  char in_FPSCR_PR;
  char in_FPSCR_SZ;
  
  piVar3 = DAT_8c61d058._4_4_;
  piVar27 = DAT_8c61d058._4_4_;
  if (in_FPSCR_SZ == '\0') {
    piVar27 = (int *)DAT_8c61d058;
  }
  puVar12 = *(ushort **)(param_2 + 2);
  psVar16 = *(short **)(param_2 + 10);
  uVar5 = *param_2 & DAT_8c61d060;
  dVar26 = (double)CONCAT44(piVar27,0x3f800000);
  if (uVar5 == 0) {
    *DAT_8c61d058._4_4_ = *DAT_8c61d058._4_4_ + (uint)param_2[1] * 3;
    iVar10 = 0;
    while (iVar10 < (int)(uint)param_2[1]) {
      uVar2 = *puVar12;
      puVar15 = puVar12 + 1;
      puVar13 = puVar12 + 2;
      iVar8 = *DAT_8c61d068;
      puVar12 = puVar12 + 3;
      iVar7 = (uint)*puVar15 * 0x20 + iVar8;
      if (in_FPSCR_SZ == '\0') {
        dVar23 = (double)CONCAT44(*(undefined4 *)(iVar7 + 0x10),SUB84(param_1,0));
      }
      else {
        dVar23 = *(double *)(iVar7 + 0x10);
      }
      iVar11 = (uint)uVar2 * 0x20 + iVar8;
      uVar19 = (undefined4)((ulonglong)dVar23 >> 0x20);
      if (in_FPSCR_SZ == '\0') {
        dVar21 = (double)CONCAT44(uVar19,*(undefined4 *)(iVar11 + 0x10));
      }
      else {
        in_xd2 = *(undefined8 *)(iVar11 + 0x10);
        dVar21 = dVar23;
      }
      if (in_FPSCR_PR == '\0') {
        dVar21 = (double)CONCAT44((float)((ulonglong)dVar21 >> 0x20) - SUB84(dVar21,0),
                                  SUB84(dVar21,0));
      }
      else {
        dVar21 = dVar21 - dVar21;
      }
      iVar8 = (uint)*puVar13 * 0x20 + iVar8;
      if (in_FPSCR_SZ == '\0') {
        fVar25 = *(float *)(iVar7 + 0x14);
        dVar23 = (double)CONCAT44(uVar19,fVar25);
        dVar21 = (double)CONCAT44((int)((ulonglong)dVar21 >> 0x20),*(undefined4 *)(iVar8 + 0x14));
        in_dr0 = (double)CONCAT44((int)((ulonglong)in_dr0 >> 0x20),fVar25);
      }
      else {
        in_xd2 = *(undefined8 *)(iVar8 + 0x14);
        fVar25 = SUB84(dVar23,0);
      }
      if (in_FPSCR_PR == '\0') {
        fVar20 = SUB84(dVar21,0) - fVar25;
        dVar21 = (double)CONCAT44((float)((ulonglong)dVar21 >> 0x20) * fVar20,fVar20);
      }
      else {
        dVar21 = (dVar21 - dVar23) * (dVar21 - dVar23);
      }
      if (in_FPSCR_SZ == '\0') {
        dVar21 = (double)CONCAT44((int)((ulonglong)dVar21 >> 0x20),*(undefined4 *)(iVar11 + 0x14));
      }
      else {
        in_xd2 = *(undefined8 *)(iVar11 + 0x14);
      }
      if (in_FPSCR_PR == '\0') {
        dVar18 = (double)CONCAT44((int)((ulonglong)in_dr0 >> 0x20),SUB84(in_dr0,0) - SUB84(dVar21,0)
                                 );
      }
      else {
        dVar18 = in_dr0 - dVar21;
      }
      if (in_FPSCR_SZ == '\0') {
        dVar21 = (double)CONCAT44((int)((ulonglong)dVar21 >> 0x20),*(undefined4 *)(iVar8 + 0x10));
      }
      else {
        in_xd2 = *(undefined8 *)(iVar8 + 0x10);
      }
      if (in_FPSCR_PR == '\0') {
        dVar21 = (double)CONCAT44((int)((ulonglong)dVar21 >> 0x20),
                                  SUB84(dVar21,0) - (float)((ulonglong)dVar23 >> 0x20));
      }
      else {
        dVar21 = dVar21 - dVar23;
      }
      uVar5 = (uint)((ulonglong)dVar21 >> 0x20);
      param_1 = dVar21;
      if (in_FPSCR_SZ == '\0') {
        param_1 = (double)CONCAT44(uVar5,fVar25);
      }
      if (in_FPSCR_PR == '\0') {
        in_dr0 = (double)CONCAT44((int)((ulonglong)dVar18 >> 0x20),SUB84(dVar18,0) * SUB84(dVar21,0)
                                 );
      }
      else {
        in_dr0 = dVar18 * dVar21;
      }
      if (in_FPSCR_PR == '\0') {
        fVar25 = (float)((ulonglong)param_1 >> 0x20) - SUB84(in_dr0,0);
        param_1 = (double)CONCAT44(fVar25,SUB84(param_1,0));
        bVar28 = 0.0 < fVar25;
      }
      else {
        param_1 = param_1 - in_dr0;
        bVar28 = (double)((ulonglong)uVar5 << 0x20) < param_1;
      }
      iVar10 = iVar10 + 1;
      if (bVar28) {
        psVar16 = psVar16 + 6;
      }
      else {
        if (in_FPSCR_SZ == '\0') {
          puVar9 = unaff_r8 + 6;
          *puVar9 = 0x3f800000;
        }
        else {
          puVar9 = unaff_r8 + 5;
          *(undefined8 *)puVar9 = in_xd6;
        }
        if (in_FPSCR_PR == '\0') {
          dVar23 = (double)CONCAT44(uVar5,(float)(int)psVar16[1] * (float)piVar27);
        }
        else {
          dVar23 = (double)CONCAT44(uVar5,(float)(int)psVar16[1]) * dVar26;
        }
        if (in_FPSCR_SZ == '\0') {
          puVar9 = puVar9 + -1;
          *puVar9 = SUB84(dVar23,0);
        }
        else {
          puVar9 = puVar9 + -2;
          *(undefined8 *)puVar9 = in_xd2;
        }
        uVar5 = DAT_8c61d2f0;
        uVar19 = (undefined4)((ulonglong)dVar23 >> 0x20);
        if (in_FPSCR_PR == '\0') {
          dVar23 = (double)CONCAT44(uVar19,(float)(int)*psVar16 * (float)piVar27);
        }
        else {
          dVar23 = (double)CONCAT44(uVar19,(float)(int)*psVar16) * dVar26;
        }
        if (in_FPSCR_SZ == '\0') {
          puVar9[-1] = SUB84(dVar23,0);
          puVar9[-2] = *(uint *)(iVar11 + 0x18);
          puVar9[-3] = *(uint *)(iVar11 + 0x14);
          dVar23 = (double)CONCAT44((int)((ulonglong)dVar23 >> 0x20),*(uint *)(iVar11 + 0x10));
          puVar9 = puVar9 + -4;
          *puVar9 = *(uint *)(iVar11 + 0x10);
        }
        else {
          *(undefined8 *)(puVar9 + -2) = in_xd2;
          *(undefined8 *)(puVar9 + -4) = *(undefined8 *)(iVar11 + 0x18);
          *(undefined8 *)(puVar9 + -6) = *(undefined8 *)(iVar11 + 0x14);
          in_xd2 = *(undefined8 *)(iVar11 + 0x10);
          puVar9 = puVar9 + -8;
          *(undefined8 *)puVar9 = in_xd2;
        }
        puVar9[-1] = uVar5;
        if (in_FPSCR_SZ == '\0') {
          puVar9 = puVar9 + 0xd;
          *puVar9 = 0x3f800000;
        }
        else {
          puVar9 = puVar9 + 0xc;
          *(undefined8 *)puVar9 = in_xd6;
        }
        uVar19 = (undefined4)((ulonglong)dVar23 >> 0x20);
        if (in_FPSCR_PR == '\0') {
          dVar23 = (double)CONCAT44(uVar19,(float)(int)psVar16[3] * (float)piVar27);
        }
        else {
          dVar23 = (double)CONCAT44(uVar19,(float)(int)psVar16[3]) * dVar26;
        }
        if (in_FPSCR_SZ == '\0') {
          puVar9 = puVar9 + -1;
          *puVar9 = SUB84(dVar23,0);
        }
        else {
          puVar9 = puVar9 + -2;
          *(undefined8 *)puVar9 = in_xd2;
        }
        uVar5 = DAT_8c61d2f0;
        psVar17 = psVar16 + 4;
        uVar19 = (undefined4)((ulonglong)dVar23 >> 0x20);
        if (in_FPSCR_PR == '\0') {
          dVar23 = (double)CONCAT44(uVar19,(float)(int)psVar16[2] * (float)piVar27);
        }
        else {
          dVar23 = (double)CONCAT44(uVar19,(float)(int)psVar16[2]) * dVar26;
        }
        if (in_FPSCR_SZ == '\0') {
          puVar9[-1] = SUB84(dVar23,0);
          puVar9[-2] = *(uint *)(iVar7 + 0x18);
          puVar9[-3] = *(uint *)(iVar7 + 0x14);
          dVar23 = (double)CONCAT44((int)((ulonglong)dVar23 >> 0x20),*(uint *)(iVar7 + 0x10));
          puVar9 = puVar9 + -4;
          *puVar9 = *(uint *)(iVar7 + 0x10);
        }
        else {
          *(undefined8 *)(puVar9 + -2) = in_xd2;
          *(undefined8 *)(puVar9 + -4) = *(undefined8 *)(iVar7 + 0x18);
          *(undefined8 *)(puVar9 + -6) = *(undefined8 *)(iVar7 + 0x14);
          in_xd2 = *(undefined8 *)(iVar7 + 0x10);
          puVar9 = puVar9 + -8;
          *(undefined8 *)puVar9 = in_xd2;
        }
        puVar9[-1] = uVar5;
        if (in_FPSCR_SZ == '\0') {
          puVar9 = puVar9 + 0xd;
          *puVar9 = 0x3f800000;
        }
        else {
          puVar9 = puVar9 + 0xc;
          *(undefined8 *)puVar9 = in_xd6;
        }
        uVar19 = (undefined4)((ulonglong)dVar23 >> 0x20);
        if (in_FPSCR_PR == '\0') {
          dVar23 = (double)CONCAT44(uVar19,(float)(int)psVar16[5] * (float)piVar27);
        }
        else {
          dVar23 = (double)CONCAT44(uVar19,(float)(int)psVar16[5]) * dVar26;
        }
        if (in_FPSCR_SZ == '\0') {
          puVar9 = puVar9 + -1;
          *puVar9 = SUB84(dVar23,0);
        }
        else {
          puVar9 = puVar9 + -2;
          *(undefined8 *)puVar9 = in_xd2;
        }
        uVar5 = DAT_8c61d2f4;
        psVar16 = psVar16 + 6;
        if (in_FPSCR_PR == '\0') {
          fVar25 = (float)(int)*psVar17 * (float)piVar27;
        }
        else {
          fVar25 = SUB84((double)CONCAT44((int)((ulonglong)dVar23 >> 0x20),(float)(int)*psVar17) *
                         dVar26,0);
        }
        if (in_FPSCR_SZ == '\0') {
          puVar9[-1] = (uint)fVar25;
          puVar9[-2] = *(uint *)(iVar8 + 0x18);
          puVar9[-3] = *(uint *)(iVar8 + 0x14);
          puVar9 = puVar9 + -4;
          *puVar9 = *(uint *)(iVar8 + 0x10);
        }
        else {
          *(undefined8 *)(puVar9 + -2) = in_xd2;
          *(undefined8 *)(puVar9 + -4) = *(undefined8 *)(iVar8 + 0x18);
          *(undefined8 *)(puVar9 + -6) = *(undefined8 *)(iVar8 + 0x14);
          in_xd2 = *(undefined8 *)(iVar8 + 0x10);
          puVar9 = puVar9 + -8;
          *(undefined8 *)puVar9 = in_xd2;
        }
        puVar9[-1] = uVar5;
        unaff_r8 = puVar9 + 7;
        *DAT_8c61d2f8 = *DAT_8c61d2f8 + 3;
      }
    }
  }
  else if (uVar5 == (int)DAT_8c61d054) {
    *DAT_8c61d058._4_4_ = *DAT_8c61d058._4_4_ + (uint)param_2[1] * 4;
    for (iVar10 = 0; uVar19 = SUB84(in_dr2,0), iVar10 < (int)(uint)param_2[1]; iVar10 = iVar10 + 1)
    {
      uVar2 = *puVar12;
      puVar15 = puVar12 + 1;
      puVar13 = puVar12 + 2;
      iVar11 = *DAT_8c61d2fc;
      puVar14 = puVar12 + 3;
      puVar12 = puVar12 + 4;
      iVar8 = (uint)*puVar15 * 0x20 + iVar11;
      iVar7 = (uint)uVar2 * 0x20 + iVar11;
      if (in_FPSCR_SZ == '\0') {
        dVar23 = (double)CONCAT44(*(undefined4 *)(iVar8 + 0x10),SUB84(param_1,0));
        uVar19 = *(undefined4 *)(iVar7 + 0x10);
      }
      else {
        dVar23 = *(double *)(iVar8 + 0x10);
        in_xd2 = *(undefined8 *)(iVar7 + 0x10);
      }
      uVar24 = (undefined4)((ulonglong)dVar23 >> 0x20);
      dVar21 = dVar23;
      if (in_FPSCR_SZ == '\0') {
        dVar21 = (double)CONCAT44(uVar24,uVar19);
      }
      if (in_FPSCR_PR == '\0') {
        dVar21 = (double)CONCAT44((float)((ulonglong)dVar21 >> 0x20) - SUB84(dVar21,0),
                                  SUB84(dVar21,0));
      }
      else {
        dVar21 = dVar21 - dVar21;
      }
      if (in_FPSCR_SZ == '\0') {
        dVar23 = (double)CONCAT44(uVar24,*(undefined4 *)(iVar8 + 0x14));
      }
      iVar6 = (uint)*puVar13 * 0x20 + iVar11;
      fVar25 = SUB84(dVar23,0);
      if (in_FPSCR_SZ == '\0') {
        dVar21 = (double)CONCAT44((int)((ulonglong)dVar21 >> 0x20),*(undefined4 *)(iVar6 + 0x14));
        in_dr0 = (double)CONCAT44((int)((ulonglong)in_dr0 >> 0x20),fVar25);
      }
      else {
        in_xd2 = *(undefined8 *)(iVar6 + 0x14);
      }
      if (in_FPSCR_PR == '\0') {
        fVar20 = SUB84(dVar21,0) - fVar25;
        dVar21 = (double)CONCAT44((float)((ulonglong)dVar21 >> 0x20) * fVar20,fVar20);
      }
      else {
        dVar21 = (dVar21 - dVar23) * (dVar21 - dVar23);
      }
      if (in_FPSCR_SZ == '\0') {
        dVar21 = (double)CONCAT44((int)((ulonglong)dVar21 >> 0x20),*(undefined4 *)(iVar7 + 0x14));
      }
      else {
        in_xd2 = *(undefined8 *)(iVar7 + 0x14);
      }
      if (in_FPSCR_PR == '\0') {
        dVar18 = (double)CONCAT44((int)((ulonglong)in_dr0 >> 0x20),SUB84(in_dr0,0) - SUB84(dVar21,0)
                                 );
      }
      else {
        dVar18 = in_dr0 - dVar21;
      }
      if (in_FPSCR_SZ == '\0') {
        dVar21 = (double)CONCAT44((int)((ulonglong)dVar21 >> 0x20),*(undefined4 *)(iVar6 + 0x10));
      }
      else {
        in_xd2 = *(undefined8 *)(iVar6 + 0x10);
      }
      if (in_FPSCR_PR == '\0') {
        dVar21 = (double)CONCAT44((int)((ulonglong)dVar21 >> 0x20),
                                  SUB84(dVar21,0) - (float)((ulonglong)dVar23 >> 0x20));
      }
      else {
        dVar21 = dVar21 - dVar23;
      }
      uVar5 = (uint)((ulonglong)dVar21 >> 0x20);
      param_1 = dVar21;
      if (in_FPSCR_SZ == '\0') {
        param_1 = (double)CONCAT44(uVar5,fVar25);
      }
      if (in_FPSCR_PR == '\0') {
        in_dr0 = (double)CONCAT44((int)((ulonglong)dVar18 >> 0x20),SUB84(dVar18,0) * SUB84(dVar21,0)
                                 );
      }
      else {
        in_dr0 = dVar18 * dVar21;
      }
      in_dr2 = (double)((ulonglong)uVar5 << 0x20);
      if (in_FPSCR_PR == '\0') {
        fVar25 = (float)((ulonglong)param_1 >> 0x20) - SUB84(in_dr0,0);
        param_1 = (double)CONCAT44(fVar25,SUB84(param_1,0));
        bVar28 = 0.0 < fVar25;
      }
      else {
        param_1 = param_1 - in_dr0;
        bVar28 = in_dr2 < param_1;
      }
      iVar11 = (uint)*puVar14 * 0x20 + iVar11;
      if (!bVar28) {
        if (in_FPSCR_SZ == '\0') {
          puVar9 = unaff_r8 + 6;
          *puVar9 = 0x3f800000;
        }
        else {
          puVar9 = unaff_r8 + 5;
          *(undefined8 *)puVar9 = in_xd6;
        }
        if (in_FPSCR_PR == '\0') {
          dVar23 = (double)CONCAT44(uVar5,(float)(int)psVar16[1] * (float)piVar27);
        }
        else {
          dVar23 = (double)CONCAT44(uVar5,(float)(int)psVar16[1]) * dVar26;
        }
        if (in_FPSCR_SZ == '\0') {
          puVar9 = puVar9 + -1;
          *puVar9 = SUB84(dVar23,0);
        }
        else {
          puVar9 = puVar9 + -2;
          *(undefined8 *)puVar9 = in_xd2;
        }
        uVar5 = DAT_8c61d2f0;
        uVar19 = (undefined4)((ulonglong)dVar23 >> 0x20);
        if (in_FPSCR_PR == '\0') {
          dVar23 = (double)CONCAT44(uVar19,(float)(int)*psVar16 * (float)piVar27);
        }
        else {
          dVar23 = (double)CONCAT44(uVar19,(float)(int)*psVar16) * dVar26;
        }
        if (in_FPSCR_SZ == '\0') {
          puVar9[-1] = SUB84(dVar23,0);
          puVar9[-2] = *(uint *)(iVar7 + 0x18);
          puVar9[-3] = *(uint *)(iVar7 + 0x14);
          dVar23 = (double)CONCAT44((int)((ulonglong)dVar23 >> 0x20),*(uint *)(iVar7 + 0x10));
          puVar9 = puVar9 + -4;
          *puVar9 = *(uint *)(iVar7 + 0x10);
        }
        else {
          *(undefined8 *)(puVar9 + -2) = in_xd2;
          *(undefined8 *)(puVar9 + -4) = *(undefined8 *)(iVar7 + 0x18);
          *(undefined8 *)(puVar9 + -6) = *(undefined8 *)(iVar7 + 0x14);
          in_xd2 = *(undefined8 *)(iVar7 + 0x10);
          puVar9 = puVar9 + -8;
          *(undefined8 *)puVar9 = in_xd2;
        }
        puVar9[-1] = uVar5;
        if (in_FPSCR_SZ == '\0') {
          puVar9 = puVar9 + 0xd;
          *puVar9 = 0x3f800000;
        }
        else {
          puVar9 = puVar9 + 0xc;
          *(undefined8 *)puVar9 = in_xd6;
        }
        uVar19 = (undefined4)((ulonglong)dVar23 >> 0x20);
        if (in_FPSCR_PR == '\0') {
          dVar23 = (double)CONCAT44(uVar19,(float)(int)psVar16[3] * (float)piVar27);
        }
        else {
          dVar23 = (double)CONCAT44(uVar19,(float)(int)psVar16[3]) * dVar26;
        }
        if (in_FPSCR_SZ == '\0') {
          puVar9 = puVar9 + -1;
          *puVar9 = SUB84(dVar23,0);
        }
        else {
          puVar9 = puVar9 + -2;
          *(undefined8 *)puVar9 = in_xd2;
        }
        uVar5 = DAT_8c61d2f0;
        uVar19 = (undefined4)((ulonglong)dVar23 >> 0x20);
        if (in_FPSCR_PR == '\0') {
          dVar23 = (double)CONCAT44(uVar19,(float)(int)psVar16[2] * (float)piVar27);
        }
        else {
          dVar23 = (double)CONCAT44(uVar19,(float)(int)psVar16[2]) * dVar26;
        }
        if (in_FPSCR_SZ == '\0') {
          puVar9[-1] = SUB84(dVar23,0);
          puVar9[-2] = *(uint *)(iVar8 + 0x18);
          puVar9[-3] = *(uint *)(iVar8 + 0x14);
          dVar23 = (double)CONCAT44((int)((ulonglong)dVar23 >> 0x20),*(uint *)(iVar8 + 0x10));
          puVar9 = puVar9 + -4;
          *puVar9 = *(uint *)(iVar8 + 0x10);
        }
        else {
          *(undefined8 *)(puVar9 + -2) = in_xd2;
          *(undefined8 *)(puVar9 + -4) = *(undefined8 *)(iVar8 + 0x18);
          *(undefined8 *)(puVar9 + -6) = *(undefined8 *)(iVar8 + 0x14);
          in_xd2 = *(undefined8 *)(iVar8 + 0x10);
          puVar9 = puVar9 + -8;
          *(undefined8 *)puVar9 = in_xd2;
        }
        puVar9[-1] = uVar5;
        if (in_FPSCR_SZ == '\0') {
          puVar9 = puVar9 + 0xd;
          *puVar9 = 0x3f800000;
        }
        else {
          puVar9 = puVar9 + 0xc;
          *(undefined8 *)puVar9 = in_xd6;
        }
        uVar19 = (undefined4)((ulonglong)dVar23 >> 0x20);
        if (in_FPSCR_PR == '\0') {
          dVar23 = (double)CONCAT44(uVar19,(float)(int)psVar16[5] * (float)piVar27);
        }
        else {
          dVar23 = (double)CONCAT44(uVar19,(float)(int)psVar16[5]) * dVar26;
        }
        if (in_FPSCR_SZ == '\0') {
          puVar9 = puVar9 + -1;
          *puVar9 = SUB84(dVar23,0);
        }
        else {
          puVar9 = puVar9 + -2;
          *(undefined8 *)puVar9 = in_xd2;
        }
        uVar5 = DAT_8c61d2f0;
        uVar19 = (undefined4)((ulonglong)dVar23 >> 0x20);
        if (in_FPSCR_PR == '\0') {
          dVar23 = (double)CONCAT44(uVar19,(float)(int)psVar16[4] * (float)piVar27);
        }
        else {
          dVar23 = (double)CONCAT44(uVar19,(float)(int)psVar16[4]) * dVar26;
        }
        if (in_FPSCR_SZ == '\0') {
          puVar9[-1] = SUB84(dVar23,0);
          puVar9[-2] = *(uint *)(iVar6 + 0x18);
          puVar9[-3] = *(uint *)(iVar6 + 0x14);
          dVar23 = (double)CONCAT44((int)((ulonglong)dVar23 >> 0x20),*(uint *)(iVar6 + 0x10));
          puVar9 = puVar9 + -4;
          *puVar9 = *(uint *)(iVar6 + 0x10);
        }
        else {
          *(undefined8 *)(puVar9 + -2) = in_xd2;
          *(undefined8 *)(puVar9 + -4) = *(undefined8 *)(iVar6 + 0x18);
          *(undefined8 *)(puVar9 + -6) = *(undefined8 *)(iVar6 + 0x14);
          in_xd2 = *(undefined8 *)(iVar6 + 0x10);
          puVar9 = puVar9 + -8;
          *(undefined8 *)puVar9 = in_xd2;
        }
        puVar9[-1] = uVar5;
        if (in_FPSCR_SZ == '\0') {
          puVar9 = puVar9 + 0xd;
          *puVar9 = 0x3f800000;
        }
        else {
          puVar9 = puVar9 + 0xc;
          *(undefined8 *)puVar9 = in_xd6;
        }
        uVar19 = (undefined4)((ulonglong)dVar23 >> 0x20);
        if (in_FPSCR_PR == '\0') {
          dVar23 = (double)CONCAT44(uVar19,(float)(int)psVar16[7] * (float)piVar27);
        }
        else {
          dVar23 = (double)CONCAT44(uVar19,(float)(int)psVar16[7]) * dVar26;
        }
        if (in_FPSCR_SZ == '\0') {
          puVar9 = puVar9 + -1;
          *puVar9 = SUB84(dVar23,0);
        }
        else {
          puVar9 = puVar9 + -2;
          *(undefined8 *)puVar9 = in_xd2;
        }
        uVar5 = DAT_8c61d2f4;
        if (in_FPSCR_PR == '\0') {
          in_dr2 = (double)(ulonglong)(uint)((float)(int)psVar16[6] * (float)piVar27);
        }
        else {
          in_dr2 = (double)CONCAT44((int)((ulonglong)dVar23 >> 0x20),(float)(int)psVar16[6]) *
                   dVar26;
        }
        if (in_FPSCR_SZ == '\0') {
          puVar9[-1] = SUB84(in_dr2,0);
          puVar9[-2] = *(uint *)(iVar11 + 0x18);
          puVar9[-3] = *(uint *)(iVar11 + 0x14);
          in_dr2 = (double)(ulonglong)*(uint *)(iVar11 + 0x10);
          puVar9 = puVar9 + -4;
          *puVar9 = *(uint *)(iVar11 + 0x10);
        }
        else {
          *(undefined8 *)(puVar9 + -2) = in_xd2;
          *(undefined8 *)(puVar9 + -4) = *(undefined8 *)(iVar11 + 0x18);
          *(undefined8 *)(puVar9 + -6) = *(undefined8 *)(iVar11 + 0x14);
          in_xd2 = *(undefined8 *)(iVar11 + 0x10);
          puVar9 = puVar9 + -8;
          *(undefined8 *)puVar9 = in_xd2;
        }
        puVar9[-1] = uVar5;
        unaff_r8 = puVar9 + 7;
        *DAT_8c61d2f8 = *DAT_8c61d2f8 + 4;
      }
      psVar16 = psVar16 + 8;
    }
  }
  else if (uVar5 == DAT_8c61d064) {
    for (iVar10 = 0; piVar4 = DAT_8c61d2fc, uVar19 = SUB84(param_1,0),
        iVar10 < (int)(uint)param_2[1]; iVar10 = iVar10 + 1) {
      puVar15 = puVar12 + 1;
      uVar5 = (uint)*puVar12;
      *piVar3 = *piVar3 + uVar5;
      iVar7 = *piVar4;
      iVar8 = (uint)puVar12[2] * 0x20 + iVar7;
      if (in_FPSCR_SZ == '\0') {
        uVar19 = *(undefined4 *)(iVar8 + 0x10);
      }
      else {
        in_xd4 = *(double *)(iVar8 + 0x10);
      }
      iVar11 = (uint)*puVar15 * 0x20 + iVar7;
      if (in_FPSCR_SZ == '\0') {
        dVar23 = (double)CONCAT44(uVar19,*(undefined4 *)(iVar11 + 0x10));
      }
      else {
        in_xd2 = *(undefined8 *)(iVar11 + 0x10);
        dVar23 = in_xd4;
      }
      if (in_FPSCR_PR == '\0') {
        dVar23 = (double)CONCAT44((float)((ulonglong)dVar23 >> 0x20) - SUB84(dVar23,0),
                                  SUB84(dVar23,0));
      }
      else {
        dVar23 = dVar23 - dVar23;
      }
      if (in_FPSCR_SZ == '\0') {
        dVar21 = (double)CONCAT44(*(undefined4 *)(iVar8 + 0x14),uVar19);
      }
      else {
        dVar21 = *(double *)(iVar8 + 0x14);
      }
      iVar7 = (uint)puVar12[3] * 0x20 + iVar7;
      fVar25 = (float)((ulonglong)dVar21 >> 0x20);
      if (in_FPSCR_SZ == '\0') {
        dVar23 = (double)CONCAT44((int)((ulonglong)dVar23 >> 0x20),*(undefined4 *)(iVar7 + 0x14));
        in_dr0 = (double)CONCAT44((int)((ulonglong)in_dr0 >> 0x20),fVar25);
      }
      else {
        in_xd2 = *(undefined8 *)(iVar7 + 0x14);
      }
      if (in_FPSCR_PR == '\0') {
        fVar25 = SUB84(dVar23,0) - fVar25;
        dVar23 = (double)CONCAT44((float)((ulonglong)dVar23 >> 0x20) * fVar25,fVar25);
      }
      else {
        dVar23 = (dVar23 - dVar21) * (dVar23 - dVar21);
      }
      if (in_FPSCR_SZ == '\0') {
        dVar23 = (double)CONCAT44((int)((ulonglong)dVar23 >> 0x20),*(undefined4 *)(iVar11 + 0x14));
      }
      else {
        in_xd2 = *(undefined8 *)(iVar11 + 0x14);
      }
      if (in_FPSCR_PR == '\0') {
        dVar18 = (double)CONCAT44((int)((ulonglong)in_dr0 >> 0x20),SUB84(in_dr0,0) - SUB84(dVar23,0)
                                 );
      }
      else {
        dVar18 = in_dr0 - dVar23;
      }
      if (in_FPSCR_SZ == '\0') {
        dVar23 = (double)CONCAT44((int)((ulonglong)dVar23 >> 0x20),*(undefined4 *)(iVar7 + 0x10));
      }
      else {
        in_xd2 = *(undefined8 *)(iVar7 + 0x10);
      }
      if (in_FPSCR_PR == '\0') {
        dVar23 = (double)CONCAT44((int)((ulonglong)dVar23 >> 0x20),SUB84(dVar23,0) - SUB84(dVar21,0)
                                 );
      }
      else {
        dVar23 = dVar23 - dVar21;
      }
      uVar22 = (uint)((ulonglong)dVar23 >> 0x20);
      param_1 = dVar23;
      if (in_FPSCR_SZ == '\0') {
        param_1 = (double)CONCAT44(uVar22,SUB84(dVar21,0));
      }
      if (in_FPSCR_PR == '\0') {
        in_dr0 = (double)CONCAT44((int)((ulonglong)dVar18 >> 0x20),SUB84(dVar18,0) * SUB84(dVar23,0)
                                 );
      }
      else {
        in_dr0 = dVar18 * dVar23;
      }
      dVar23 = (double)((ulonglong)uVar22 << 0x20);
      if (in_FPSCR_PR == '\0') {
        fVar25 = (float)((ulonglong)param_1 >> 0x20) - SUB84(in_dr0,0);
        param_1 = (double)CONCAT44(fVar25,SUB84(param_1,0));
        bVar28 = 0.0 < fVar25;
      }
      else {
        param_1 = param_1 - in_dr0;
        bVar28 = dVar23 < param_1;
      }
      if (bVar28) {
        puVar12 = puVar15 + uVar5;
        psVar16 = psVar16 + uVar5 * 2;
      }
      else {
        *DAT_8c61d3bc = *DAT_8c61d3bc + uVar5;
        iVar7 = 0;
        if (0 < (int)(uVar5 - 1)) {
          do {
            uVar2 = *puVar15;
            puVar15 = puVar15 + 1;
            iVar8 = *DAT_8c61d3c0;
            if (in_FPSCR_SZ == '\0') {
              puVar9 = unaff_r8 + 6;
              *puVar9 = 0x3f800000;
            }
            else {
              puVar9 = unaff_r8 + 5;
              *(undefined8 *)puVar9 = in_xd6;
            }
            iVar8 = (uint)uVar2 * 0x20 + iVar8;
            uVar19 = (undefined4)((ulonglong)dVar23 >> 0x20);
            if (in_FPSCR_PR == '\0') {
              dVar23 = (double)CONCAT44(uVar19,(float)(int)psVar16[1] * (float)piVar27);
            }
            else {
              dVar23 = (double)CONCAT44(uVar19,(float)(int)psVar16[1]) * dVar26;
            }
            if (in_FPSCR_SZ == '\0') {
              puVar9 = puVar9 + -1;
              *puVar9 = SUB84(dVar23,0);
            }
            else {
              puVar9 = puVar9 + -2;
              *(undefined8 *)puVar9 = in_xd2;
            }
            uVar22 = DAT_8c61d3c4;
            sVar1 = *psVar16;
            psVar16 = psVar16 + 2;
            uVar19 = (undefined4)((ulonglong)dVar23 >> 0x20);
            if (in_FPSCR_PR == '\0') {
              dVar23 = (double)CONCAT44(uVar19,(float)(int)sVar1 * (float)piVar27);
            }
            else {
              dVar23 = (double)CONCAT44(uVar19,(float)(int)sVar1) * dVar26;
            }
            if (in_FPSCR_SZ == '\0') {
              puVar9[-1] = SUB84(dVar23,0);
              puVar9[-2] = *(uint *)(iVar8 + 0x18);
              puVar9[-3] = *(uint *)(iVar8 + 0x14);
              dVar23 = (double)CONCAT44((int)((ulonglong)dVar23 >> 0x20),*(uint *)(iVar8 + 0x10));
              puVar9 = puVar9 + -4;
              *puVar9 = *(uint *)(iVar8 + 0x10);
            }
            else {
              *(undefined8 *)(puVar9 + -2) = in_xd2;
              *(undefined8 *)(puVar9 + -4) = *(undefined8 *)(iVar8 + 0x18);
              *(undefined8 *)(puVar9 + -6) = *(undefined8 *)(iVar8 + 0x14);
              in_xd2 = *(undefined8 *)(iVar8 + 0x10);
              puVar9 = puVar9 + -8;
              *(undefined8 *)puVar9 = in_xd2;
            }
            puVar9[-1] = uVar22;
            iVar7 = iVar7 + 1;
            unaff_r8 = puVar9 + 7;
          } while (iVar7 < (int)(uVar5 - 1));
        }
        uVar2 = *puVar15;
        puVar12 = puVar15 + 1;
        iVar7 = *DAT_8c61d3c0;
        if (in_FPSCR_SZ == '\0') {
          puVar9 = unaff_r8 + 6;
          *puVar9 = 0x3f800000;
        }
        else {
          puVar9 = unaff_r8 + 5;
          *(undefined8 *)puVar9 = in_xd6;
        }
        iVar7 = (uint)uVar2 * 0x20 + iVar7;
        uVar19 = (undefined4)((ulonglong)dVar23 >> 0x20);
        if (in_FPSCR_PR == '\0') {
          dVar23 = (double)CONCAT44(uVar19,(float)(int)psVar16[1] * (float)piVar27);
        }
        else {
          dVar23 = (double)CONCAT44(uVar19,(float)(int)psVar16[1]) * dVar26;
        }
        if (in_FPSCR_SZ == '\0') {
          puVar9 = puVar9 + -1;
          *puVar9 = SUB84(dVar23,0);
        }
        else {
          puVar9 = puVar9 + -2;
          *(undefined8 *)puVar9 = in_xd2;
        }
        uVar5 = DAT_8c61d3c8;
        sVar1 = *psVar16;
        psVar16 = psVar16 + 2;
        if (in_FPSCR_PR == '\0') {
          fVar25 = (float)(int)sVar1 * (float)piVar27;
        }
        else {
          fVar25 = SUB84((double)CONCAT44((int)((ulonglong)dVar23 >> 0x20),(float)(int)sVar1) *
                         dVar26,0);
        }
        if (in_FPSCR_SZ == '\0') {
          puVar9[-1] = (uint)fVar25;
          puVar9[-2] = *(uint *)(iVar7 + 0x18);
          puVar9[-3] = *(uint *)(iVar7 + 0x14);
          puVar9 = puVar9 + -4;
          *puVar9 = *(uint *)(iVar7 + 0x10);
        }
        else {
          *(undefined8 *)(puVar9 + -2) = in_xd2;
          *(undefined8 *)(puVar9 + -4) = *(undefined8 *)(iVar7 + 0x18);
          *(undefined8 *)(puVar9 + -6) = *(undefined8 *)(iVar7 + 0x14);
          in_xd2 = *(undefined8 *)(iVar7 + 0x10);
          puVar9 = puVar9 + -8;
          *(undefined8 *)puVar9 = in_xd2;
        }
        puVar9[-1] = uVar5;
        unaff_r8 = puVar9 + 7;
      }
    }
  }
  else if (uVar5 == DAT_8c61d060) {
    *DAT_8c61d3d4 = param_2;
    (*(code *)PTR_FUN_8c61d4fc)(*DAT_8c61d4f8);
  }
  return;
}

