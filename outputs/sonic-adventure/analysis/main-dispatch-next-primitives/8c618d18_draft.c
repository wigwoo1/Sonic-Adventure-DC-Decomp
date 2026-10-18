/* Ghidra draft only; not buildable/verified source. */

/* WARNING: Removing unreachable block (ram,0x8c618ec8) */
/* WARNING: Removing unreachable block (ram,0x8c618e22) */
/* WARNING: Removing unreachable block (ram,0x8c619256) */
/* WARNING: Removing unreachable block (ram,0x8c6191f4) */
/* WARNING: Removing unreachable block (ram,0x8c6191e4) */
/* WARNING: Removing unreachable block (ram,0x8c618fb0) */
/* WARNING: Removing unreachable block (ram,0x8c618fba) */
/* WARNING: Removing unreachable block (ram,0x8c619008) */
/* WARNING: Removing unreachable block (ram,0x8c6190b8) */
/* WARNING: Removing unreachable block (ram,0x8c619014) */
/* WARNING: Removing unreachable block (ram,0x8c618e18) */
/* WARNING: Removing unreachable block (ram,0x8c618e70) */
/* WARNING: Removing unreachable block (ram,0x8c618e7c) */
/* WARNING: Removing unreachable block (ram,0x8c619060) */
/* WARNING: Removing unreachable block (ram,0x8c618ed4) */
/* WARNING: Removing unreachable block (ram,0x8c61906c) */
/* WARNING: Removing unreachable block (ram,0x8c619266) */
/* WARNING: Removing unreachable block (ram,0x8c6190c4) */
/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_8c618d18(double param_1,undefined4 param_2,ushort *param_3)

{
  short sVar1;
  ushort uVar2;
  int *piVar3;
  undefined4 *puVar4;
  int *piVar5;
  uint uVar6;
  undefined8 *puVar7;
  int iVar8;
  undefined8 *puVar9;
  undefined8 *puVar10;
  int iVar11;
  uint *unaff_r8;
  uint *puVar12;
  int iVar13;
  int iVar14;
  ushort *puVar15;
  ushort *puVar16;
  ushort *puVar17;
  ushort *puVar18;
  short *psVar19;
  short *psVar20;
  undefined8 *puVar21;
  double in_dr0;
  double dVar22;
  undefined4 uVar23;
  float fVar24;
  double in_dr2;
  double dVar25;
  uint uVar26;
  double dVar27;
  undefined4 uVar29;
  float fVar30;
  undefined8 uVar28;
  double dVar31;
  float fVar32;
  undefined8 in_xd2;
  double in_xd4;
  bool bVar33;
  char in_FPSCR_PR;
  char in_FPSCR_SZ;
  
  piVar3 = DAT_8c618dd4._4_4_;
  if (in_FPSCR_SZ == '\0') {
    dVar31 = (double)CONCAT44((undefined4)DAT_8c618dd4,param_2);
  }
  else {
    dVar31 = (double)CONCAT44(DAT_8c618dd4._4_4_,(undefined4)DAT_8c618dd4);
  }
  psVar19 = *(short **)(param_3 + 10);
  puVar15 = *(ushort **)(param_3 + 2);
  uVar6 = *param_3 & DAT_8c618ddc;
  fVar32 = (float)((ulonglong)dVar31 >> 0x20);
  if (uVar6 == 0) {
    *DAT_8c618dd4._4_4_ = *DAT_8c618dd4._4_4_ + (uint)param_3[1] * 3;
    iVar13 = 0;
    while (puVar4 = DAT_8c619110, iVar13 < (int)(uint)param_3[1]) {
      uVar2 = *puVar15;
      puVar18 = puVar15 + 1;
      puVar16 = puVar15 + 2;
      iVar14 = *DAT_8c618de4;
      puVar15 = puVar15 + 3;
      puVar9 = (undefined8 *)((uint)*puVar18 * 0x20 + iVar14);
      if (in_FPSCR_SZ == '\0') {
        dVar27 = (double)CONCAT44(*(undefined4 *)(puVar9 + 2),SUB84(param_1,0));
      }
      else {
        dVar27 = (double)puVar9[2];
      }
      puVar10 = (undefined8 *)((uint)uVar2 * 0x20 + iVar14);
      uVar23 = (undefined4)((ulonglong)dVar27 >> 0x20);
      if (in_FPSCR_SZ == '\0') {
        dVar25 = (double)CONCAT44(uVar23,*(undefined4 *)(puVar10 + 2));
      }
      else {
        in_xd2 = puVar10[2];
        dVar25 = dVar27;
      }
      if (in_FPSCR_PR == '\0') {
        dVar25 = (double)CONCAT44((float)((ulonglong)dVar25 >> 0x20) - SUB84(dVar25,0),
                                  SUB84(dVar25,0));
      }
      else {
        dVar25 = dVar25 - dVar25;
      }
      puVar7 = (undefined8 *)((uint)*puVar16 * 0x20 + iVar14);
      if (in_FPSCR_SZ == '\0') {
        fVar30 = *(float *)((int)puVar9 + 0x14);
        dVar27 = (double)CONCAT44(uVar23,fVar30);
        dVar25 = (double)CONCAT44((int)((ulonglong)dVar25 >> 0x20),
                                  *(undefined4 *)((int)puVar7 + 0x14));
        in_dr0 = (double)CONCAT44((int)((ulonglong)in_dr0 >> 0x20),fVar30);
      }
      else {
        in_xd2 = *(undefined8 *)((int)puVar7 + 0x14);
        fVar30 = SUB84(dVar27,0);
      }
      if (in_FPSCR_PR == '\0') {
        fVar24 = SUB84(dVar25,0) - fVar30;
        dVar25 = (double)CONCAT44((float)((ulonglong)dVar25 >> 0x20) * fVar24,fVar24);
      }
      else {
        dVar25 = (dVar25 - dVar27) * (dVar25 - dVar27);
      }
      if (in_FPSCR_SZ == '\0') {
        dVar25 = (double)CONCAT44((int)((ulonglong)dVar25 >> 0x20),
                                  *(undefined4 *)((int)puVar10 + 0x14));
      }
      else {
        in_xd2 = *(undefined8 *)((int)puVar10 + 0x14);
      }
      if (in_FPSCR_PR == '\0') {
        dVar22 = (double)CONCAT44((int)((ulonglong)in_dr0 >> 0x20),SUB84(in_dr0,0) - SUB84(dVar25,0)
                                 );
      }
      else {
        dVar22 = in_dr0 - dVar25;
      }
      if (in_FPSCR_SZ == '\0') {
        dVar25 = (double)CONCAT44((int)((ulonglong)dVar25 >> 0x20),*(undefined4 *)(puVar7 + 2));
      }
      else {
        in_xd2 = puVar7[2];
      }
      if (in_FPSCR_PR == '\0') {
        dVar25 = (double)CONCAT44((int)((ulonglong)dVar25 >> 0x20),
                                  SUB84(dVar25,0) - (float)((ulonglong)dVar27 >> 0x20));
      }
      else {
        dVar25 = dVar25 - dVar27;
      }
      uVar6 = (uint)((ulonglong)dVar25 >> 0x20);
      param_1 = dVar25;
      if (in_FPSCR_SZ == '\0') {
        param_1 = (double)CONCAT44(uVar6,fVar30);
      }
      if (in_FPSCR_PR == '\0') {
        in_dr0 = (double)CONCAT44((int)((ulonglong)dVar22 >> 0x20),SUB84(dVar22,0) * SUB84(dVar25,0)
                                 );
      }
      else {
        in_dr0 = dVar22 * dVar25;
      }
      if (in_FPSCR_PR == '\0') {
        fVar30 = (float)((ulonglong)param_1 >> 0x20) - SUB84(in_dr0,0);
        param_1 = (double)CONCAT44(fVar30,SUB84(param_1,0));
        bVar33 = 0.0 < fVar30;
      }
      else {
        param_1 = param_1 - in_dr0;
        bVar33 = (double)((ulonglong)uVar6 << 0x20) < param_1;
      }
      iVar13 = iVar13 + 1;
      if (bVar33) {
        psVar19 = psVar19 + 6;
      }
      else {
        if (in_FPSCR_SZ == '\0') {
          dVar27 = (double)CONCAT44(*(undefined4 *)DAT_8c619108,*(undefined4 *)puVar10);
        }
        else {
          in_xd2 = *puVar10;
          dVar27 = *DAT_8c619108;
        }
        if (in_FPSCR_PR == '\0') {
          dVar27 = (double)CONCAT44((float)((ulonglong)dVar27 >> 0x20) * SUB84(dVar27,0),
                                    SUB84(dVar27,0));
        }
        else {
          dVar27 = dVar27 * dVar27;
        }
        if (in_FPSCR_SZ == '\0') {
          dVar27 = (double)CONCAT44((int)((ulonglong)dVar27 >> 0x20),*(undefined4 *)DAT_8c61910c);
          in_dr0 = (double)CONCAT44((int)((ulonglong)in_dr0 >> 0x20),*DAT_8c619110);
        }
        else {
          in_xd2 = *DAT_8c61910c;
        }
        if (in_FPSCR_PR == '\0') {
          fVar30 = SUB84(dVar27,0) - (float)((ulonglong)dVar27 >> 0x20);
        }
        else {
          fVar30 = SUB84(dVar27 - dVar27,0);
        }
        if (in_FPSCR_SZ == '\0') {
          dVar27 = (double)CONCAT44(*(undefined4 *)((int)puVar10 + 4),fVar30);
        }
        else {
          dVar27 = *(double *)((int)puVar10 + 4);
        }
        if (in_FPSCR_PR == '\0') {
          dVar25 = (double)CONCAT44((int)((ulonglong)in_dr0 >> 0x20),
                                    SUB84(in_dr0,0) * (float)((ulonglong)dVar27 >> 0x20));
        }
        else {
          dVar25 = in_dr0 * dVar27;
        }
        if (in_FPSCR_SZ == '\0') {
          dVar27 = (double)CONCAT44(*(undefined4 *)(puVar10 + 1),SUB84(dVar27,0));
        }
        else {
          dVar27 = (double)puVar10[1];
        }
        if (in_FPSCR_PR == '\0') {
          dVar27 = (double)CONCAT44((int)((ulonglong)dVar27 >> 0x20),
                                    SUB84(dVar27,0) - SUB84(dVar25,0));
        }
        else {
          dVar27 = dVar27 - dVar25;
        }
        if (in_FPSCR_SZ == '\0') {
          dVar25 = (double)CONCAT44((int)((ulonglong)dVar25 >> 0x20),
                                    *(undefined4 *)(DAT_8c619108 + 1));
        }
        fVar30 = (float)((ulonglong)dVar27 >> 0x20);
        if (in_FPSCR_PR == '\0') {
          fVar24 = SUB84(dVar25,0) * fVar30;
          dVar25 = (double)CONCAT44((int)((ulonglong)dVar25 >> 0x20),fVar24);
          dVar27 = (double)CONCAT44(fVar30,SUB84(dVar27,0) - fVar24);
        }
        else {
          dVar25 = dVar25 * dVar27;
          dVar27 = dVar27 - dVar25;
        }
        if (in_FPSCR_SZ == '\0') {
          puVar12 = unaff_r8 + 6;
          *puVar12 = SUB84(dVar27,0);
        }
        else {
          puVar12 = unaff_r8 + 5;
          *(undefined8 *)puVar12 = in_xd2;
        }
        uVar23 = (undefined4)((ulonglong)dVar27 >> 0x20);
        if (in_FPSCR_PR == '\0') {
          dVar27 = (double)CONCAT44(uVar23,(float)(int)psVar19[1] * fVar32);
        }
        else {
          dVar27 = (double)CONCAT44(uVar23,(float)(int)psVar19[1]) * dVar31;
        }
        if (in_FPSCR_SZ == '\0') {
          puVar12 = puVar12 + -1;
          *puVar12 = SUB84(dVar27,0);
        }
        else {
          puVar12 = puVar12 + -2;
          *(undefined8 *)puVar12 = in_xd2;
        }
        if (in_FPSCR_PR == '\0') {
          fVar30 = (float)(int)*psVar19 * fVar32;
        }
        else {
          fVar30 = SUB84((double)CONCAT44((int)((ulonglong)dVar27 >> 0x20),(float)(int)*psVar19) *
                         dVar31,0);
        }
        if (in_FPSCR_SZ == '\0') {
          puVar12[-1] = (uint)fVar30;
          uVar6 = DAT_8c619114;
          puVar12[-2] = *(uint *)(puVar10 + 3);
          puVar12[-3] = *(uint *)((int)puVar10 + 0x14);
          puVar12 = puVar12 + -4;
          *puVar12 = *(uint *)(puVar10 + 2);
        }
        else {
          *(undefined8 *)(puVar12 + -2) = in_xd2;
          uVar6 = DAT_8c619114;
          *(undefined8 *)(puVar12 + -4) = puVar10[3];
          *(undefined8 *)(puVar12 + -6) = *(undefined8 *)((int)puVar10 + 0x14);
          in_xd2 = puVar10[2];
          puVar12 = puVar12 + -8;
          *(undefined8 *)puVar12 = in_xd2;
        }
        puVar12[-1] = uVar6;
        if (in_FPSCR_SZ == '\0') {
          dVar27 = (double)CONCAT44(*(undefined4 *)DAT_8c619108,*(undefined4 *)puVar9);
        }
        else {
          in_xd2 = *puVar9;
          dVar27 = *DAT_8c619108;
        }
        if (in_FPSCR_PR == '\0') {
          dVar27 = (double)CONCAT44((float)((ulonglong)dVar27 >> 0x20) * SUB84(dVar27,0),
                                    SUB84(dVar27,0));
        }
        else {
          dVar27 = dVar27 * dVar27;
        }
        if (in_FPSCR_SZ == '\0') {
          dVar25 = (double)CONCAT44((int)((ulonglong)dVar25 >> 0x20),*puVar4);
          dVar27 = (double)CONCAT44((int)((ulonglong)dVar27 >> 0x20),*(undefined4 *)DAT_8c61910c);
        }
        else {
          in_xd2 = *DAT_8c61910c;
        }
        if (in_FPSCR_PR == '\0') {
          fVar30 = SUB84(dVar27,0) - (float)((ulonglong)dVar27 >> 0x20);
        }
        else {
          fVar30 = SUB84(dVar27 - dVar27,0);
        }
        if (in_FPSCR_SZ == '\0') {
          dVar27 = (double)CONCAT44(*(undefined4 *)((int)puVar9 + 4),fVar30);
        }
        else {
          dVar27 = *(double *)((int)puVar9 + 4);
        }
        if (in_FPSCR_PR == '\0') {
          dVar25 = (double)CONCAT44((int)((ulonglong)dVar25 >> 0x20),
                                    SUB84(dVar25,0) * (float)((ulonglong)dVar27 >> 0x20));
        }
        else {
          dVar25 = dVar25 * dVar27;
        }
        if (in_FPSCR_SZ == '\0') {
          dVar27 = (double)CONCAT44(*(undefined4 *)(puVar9 + 1),SUB84(dVar27,0));
        }
        else {
          dVar27 = (double)puVar9[1];
        }
        if (in_FPSCR_PR == '\0') {
          dVar27 = (double)CONCAT44((int)((ulonglong)dVar27 >> 0x20),
                                    SUB84(dVar27,0) - SUB84(dVar25,0));
        }
        else {
          dVar27 = dVar27 - dVar25;
        }
        if (in_FPSCR_SZ == '\0') {
          dVar25 = (double)CONCAT44((int)((ulonglong)dVar25 >> 0x20),*DAT_8c619118);
        }
        fVar30 = (float)((ulonglong)dVar27 >> 0x20);
        if (in_FPSCR_PR == '\0') {
          fVar24 = SUB84(dVar25,0) * fVar30;
          dVar25 = (double)CONCAT44((int)((ulonglong)dVar25 >> 0x20),fVar24);
          dVar27 = (double)CONCAT44(fVar30,SUB84(dVar27,0) - fVar24);
        }
        else {
          dVar25 = dVar25 * dVar27;
          dVar27 = dVar27 - dVar25;
        }
        if (in_FPSCR_SZ == '\0') {
          puVar12 = puVar12 + 0xd;
          *puVar12 = SUB84(dVar27,0);
        }
        else {
          puVar12 = puVar12 + 0xc;
          *(undefined8 *)puVar12 = in_xd2;
        }
        uVar23 = (undefined4)((ulonglong)dVar27 >> 0x20);
        if (in_FPSCR_PR == '\0') {
          dVar27 = (double)CONCAT44(uVar23,(float)(int)psVar19[3] * fVar32);
        }
        else {
          dVar27 = (double)CONCAT44(uVar23,(float)(int)psVar19[3]) * dVar31;
        }
        if (in_FPSCR_SZ == '\0') {
          puVar12 = puVar12 + -1;
          *puVar12 = SUB84(dVar27,0);
        }
        else {
          puVar12 = puVar12 + -2;
          *(undefined8 *)puVar12 = in_xd2;
        }
        psVar20 = psVar19 + 4;
        if (in_FPSCR_PR == '\0') {
          fVar30 = (float)(int)psVar19[2] * fVar32;
        }
        else {
          fVar30 = SUB84((double)CONCAT44((int)((ulonglong)dVar27 >> 0x20),(float)(int)psVar19[2]) *
                         dVar31,0);
        }
        if (in_FPSCR_SZ == '\0') {
          puVar12[-1] = (uint)fVar30;
          uVar6 = DAT_8c619114;
          puVar12[-2] = *(uint *)(puVar9 + 3);
          puVar12[-3] = *(uint *)((int)puVar9 + 0x14);
          puVar12 = puVar12 + -4;
          *puVar12 = *(uint *)(puVar9 + 2);
        }
        else {
          *(undefined8 *)(puVar12 + -2) = in_xd2;
          uVar6 = DAT_8c619114;
          *(undefined8 *)(puVar12 + -4) = puVar9[3];
          *(undefined8 *)(puVar12 + -6) = *(undefined8 *)((int)puVar9 + 0x14);
          in_xd2 = puVar9[2];
          puVar12 = puVar12 + -8;
          *(undefined8 *)puVar12 = in_xd2;
        }
        puVar12[-1] = uVar6;
        if (in_FPSCR_SZ == '\0') {
          dVar27 = (double)CONCAT44(*(undefined4 *)DAT_8c619108,*(undefined4 *)puVar7);
        }
        else {
          in_xd2 = *puVar7;
          dVar27 = *DAT_8c619108;
        }
        if (in_FPSCR_PR == '\0') {
          dVar27 = (double)CONCAT44((float)((ulonglong)dVar27 >> 0x20) * SUB84(dVar27,0),
                                    SUB84(dVar27,0));
        }
        else {
          dVar27 = dVar27 * dVar27;
        }
        if (in_FPSCR_SZ == '\0') {
          dVar25 = (double)CONCAT44((int)((ulonglong)dVar25 >> 0x20),*puVar4);
          dVar27 = (double)CONCAT44((int)((ulonglong)dVar27 >> 0x20),*(undefined4 *)DAT_8c61910c);
        }
        else {
          in_xd2 = *DAT_8c61910c;
        }
        if (in_FPSCR_PR == '\0') {
          fVar30 = SUB84(dVar27,0) - (float)((ulonglong)dVar27 >> 0x20);
        }
        else {
          fVar30 = SUB84(dVar27 - dVar27,0);
        }
        if (in_FPSCR_SZ == '\0') {
          dVar27 = (double)CONCAT44(*(undefined4 *)((int)puVar7 + 4),fVar30);
        }
        else {
          dVar27 = *(double *)((int)puVar7 + 4);
        }
        if (in_FPSCR_PR == '\0') {
          dVar25 = (double)CONCAT44((int)((ulonglong)dVar25 >> 0x20),
                                    SUB84(dVar25,0) * (float)((ulonglong)dVar27 >> 0x20));
        }
        else {
          dVar25 = dVar25 * dVar27;
        }
        if (in_FPSCR_SZ == '\0') {
          dVar27 = (double)CONCAT44(*(undefined4 *)(puVar7 + 1),SUB84(dVar27,0));
        }
        else {
          dVar27 = (double)puVar7[1];
        }
        if (in_FPSCR_PR == '\0') {
          dVar27 = (double)CONCAT44((int)((ulonglong)dVar27 >> 0x20),
                                    SUB84(dVar27,0) - SUB84(dVar25,0));
        }
        else {
          dVar27 = dVar27 - dVar25;
        }
        if (in_FPSCR_SZ == '\0') {
          dVar25 = (double)CONCAT44((int)((ulonglong)dVar25 >> 0x20),*DAT_8c619118);
        }
        fVar30 = (float)((ulonglong)dVar27 >> 0x20);
        if (in_FPSCR_PR == '\0') {
          fVar24 = SUB84(dVar25,0) * fVar30;
          in_dr0 = (double)CONCAT44((int)((ulonglong)dVar25 >> 0x20),fVar24);
          dVar27 = (double)CONCAT44(fVar30,SUB84(dVar27,0) - fVar24);
        }
        else {
          in_dr0 = dVar25 * dVar27;
          dVar27 = dVar27 - in_dr0;
        }
        if (in_FPSCR_SZ == '\0') {
          puVar12 = puVar12 + 0xd;
          *puVar12 = SUB84(dVar27,0);
        }
        else {
          puVar12 = puVar12 + 0xc;
          *(undefined8 *)puVar12 = in_xd2;
        }
        uVar23 = (undefined4)((ulonglong)dVar27 >> 0x20);
        if (in_FPSCR_PR == '\0') {
          dVar27 = (double)CONCAT44(uVar23,(float)(int)psVar19[5] * fVar32);
        }
        else {
          dVar27 = (double)CONCAT44(uVar23,(float)(int)psVar19[5]) * dVar31;
        }
        if (in_FPSCR_SZ == '\0') {
          puVar12 = puVar12 + -1;
          *puVar12 = SUB84(dVar27,0);
        }
        else {
          puVar12 = puVar12 + -2;
          *(undefined8 *)puVar12 = in_xd2;
        }
        psVar19 = psVar19 + 6;
        if (in_FPSCR_PR == '\0') {
          fVar30 = (float)(int)*psVar20 * fVar32;
        }
        else {
          fVar30 = SUB84((double)CONCAT44((int)((ulonglong)dVar27 >> 0x20),(float)(int)*psVar20) *
                         dVar31,0);
        }
        if (in_FPSCR_SZ == '\0') {
          puVar12[-1] = (uint)fVar30;
          uVar6 = DAT_8c61911c;
          puVar12[-2] = *(uint *)(puVar7 + 3);
          puVar12[-3] = *(uint *)((int)puVar7 + 0x14);
          puVar12 = puVar12 + -4;
          *puVar12 = *(uint *)(puVar7 + 2);
        }
        else {
          *(undefined8 *)(puVar12 + -2) = in_xd2;
          uVar6 = DAT_8c61911c;
          *(undefined8 *)(puVar12 + -4) = puVar7[3];
          *(undefined8 *)(puVar12 + -6) = *(undefined8 *)((int)puVar7 + 0x14);
          in_xd2 = puVar7[2];
          puVar12 = puVar12 + -8;
          *(undefined8 *)puVar12 = in_xd2;
        }
        puVar12[-1] = uVar6;
        unaff_r8 = puVar12 + 7;
        *DAT_8c619120 = *DAT_8c619120 + 3;
      }
    }
  }
  else if (uVar6 == (int)DAT_8c618dd2) {
    *DAT_8c618dd4._4_4_ = *DAT_8c618dd4._4_4_ + (uint)param_3[1] * 4;
    for (iVar13 = 0; puVar4 = DAT_8c619110, uVar23 = SUB84(in_dr2,0), iVar13 < (int)(uint)param_3[1]
        ; iVar13 = iVar13 + 1) {
      uVar2 = *puVar15;
      puVar18 = puVar15 + 1;
      puVar16 = puVar15 + 2;
      iVar14 = *DAT_8c619124;
      puVar17 = puVar15 + 3;
      puVar15 = puVar15 + 4;
      puVar10 = (undefined8 *)((uint)*puVar18 * 0x20 + iVar14);
      puVar9 = (undefined8 *)((uint)uVar2 * 0x20 + iVar14);
      if (in_FPSCR_SZ == '\0') {
        dVar27 = (double)CONCAT44(*(undefined4 *)(puVar10 + 2),SUB84(param_1,0));
        uVar23 = *(undefined4 *)(puVar9 + 2);
      }
      else {
        dVar27 = (double)puVar10[2];
        in_xd2 = puVar9[2];
      }
      uVar29 = (undefined4)((ulonglong)dVar27 >> 0x20);
      dVar25 = dVar27;
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
        dVar27 = (double)CONCAT44(uVar29,*(undefined4 *)((int)puVar10 + 0x14));
      }
      puVar7 = (undefined8 *)((uint)*puVar16 * 0x20 + iVar14);
      fVar30 = SUB84(dVar27,0);
      if (in_FPSCR_SZ == '\0') {
        dVar25 = (double)CONCAT44((int)((ulonglong)dVar25 >> 0x20),
                                  *(undefined4 *)((int)puVar7 + 0x14));
        in_dr0 = (double)CONCAT44((int)((ulonglong)in_dr0 >> 0x20),fVar30);
      }
      else {
        in_xd2 = *(undefined8 *)((int)puVar7 + 0x14);
      }
      if (in_FPSCR_PR == '\0') {
        fVar24 = SUB84(dVar25,0) - fVar30;
        dVar25 = (double)CONCAT44((float)((ulonglong)dVar25 >> 0x20) * fVar24,fVar24);
      }
      else {
        dVar25 = (dVar25 - dVar27) * (dVar25 - dVar27);
      }
      if (in_FPSCR_SZ == '\0') {
        dVar25 = (double)CONCAT44((int)((ulonglong)dVar25 >> 0x20),
                                  *(undefined4 *)((int)puVar9 + 0x14));
      }
      else {
        in_xd2 = *(undefined8 *)((int)puVar9 + 0x14);
      }
      if (in_FPSCR_PR == '\0') {
        dVar22 = (double)CONCAT44((int)((ulonglong)in_dr0 >> 0x20),SUB84(in_dr0,0) - SUB84(dVar25,0)
                                 );
      }
      else {
        dVar22 = in_dr0 - dVar25;
      }
      if (in_FPSCR_SZ == '\0') {
        dVar25 = (double)CONCAT44((int)((ulonglong)dVar25 >> 0x20),*(undefined4 *)(puVar7 + 2));
      }
      else {
        in_xd2 = puVar7[2];
      }
      if (in_FPSCR_PR == '\0') {
        dVar25 = (double)CONCAT44((int)((ulonglong)dVar25 >> 0x20),
                                  SUB84(dVar25,0) - (float)((ulonglong)dVar27 >> 0x20));
      }
      else {
        dVar25 = dVar25 - dVar27;
      }
      uVar6 = (uint)((ulonglong)dVar25 >> 0x20);
      param_1 = dVar25;
      if (in_FPSCR_SZ == '\0') {
        param_1 = (double)CONCAT44(uVar6,fVar30);
      }
      if (in_FPSCR_PR == '\0') {
        in_dr0 = (double)CONCAT44((int)((ulonglong)dVar22 >> 0x20),SUB84(dVar22,0) * SUB84(dVar25,0)
                                 );
      }
      else {
        in_dr0 = dVar22 * dVar25;
      }
      in_dr2 = (double)((ulonglong)uVar6 << 0x20);
      if (in_FPSCR_PR == '\0') {
        fVar30 = (float)((ulonglong)param_1 >> 0x20) - SUB84(in_dr0,0);
        param_1 = (double)CONCAT44(fVar30,SUB84(param_1,0));
        bVar33 = 0.0 < fVar30;
      }
      else {
        param_1 = param_1 - in_dr0;
        bVar33 = in_dr2 < param_1;
      }
      puVar21 = (undefined8 *)((uint)*puVar17 * 0x20 + iVar14);
      if (!bVar33) {
        if (in_FPSCR_SZ == '\0') {
          dVar27 = (double)CONCAT44(*(undefined4 *)DAT_8c619108,*(undefined4 *)puVar9);
        }
        else {
          in_xd2 = *puVar9;
          dVar27 = *DAT_8c619108;
        }
        if (in_FPSCR_PR == '\0') {
          dVar27 = (double)CONCAT44((float)((ulonglong)dVar27 >> 0x20) * SUB84(dVar27,0),
                                    SUB84(dVar27,0));
        }
        else {
          dVar27 = dVar27 * dVar27;
        }
        if (in_FPSCR_SZ == '\0') {
          dVar27 = (double)CONCAT44((int)((ulonglong)dVar27 >> 0x20),*(undefined4 *)DAT_8c61910c);
          in_dr0 = (double)CONCAT44((int)((ulonglong)in_dr0 >> 0x20),*DAT_8c619110);
        }
        else {
          in_xd2 = *DAT_8c61910c;
        }
        if (in_FPSCR_PR == '\0') {
          fVar30 = SUB84(dVar27,0) - (float)((ulonglong)dVar27 >> 0x20);
        }
        else {
          fVar30 = SUB84(dVar27 - dVar27,0);
        }
        if (in_FPSCR_SZ == '\0') {
          dVar27 = (double)CONCAT44(*(undefined4 *)((int)puVar9 + 4),fVar30);
        }
        else {
          dVar27 = *(double *)((int)puVar9 + 4);
        }
        if (in_FPSCR_PR == '\0') {
          dVar25 = (double)CONCAT44((int)((ulonglong)in_dr0 >> 0x20),
                                    SUB84(in_dr0,0) * (float)((ulonglong)dVar27 >> 0x20));
        }
        else {
          dVar25 = in_dr0 * dVar27;
        }
        if (in_FPSCR_SZ == '\0') {
          dVar27 = (double)CONCAT44(*(undefined4 *)(puVar9 + 1),SUB84(dVar27,0));
        }
        else {
          dVar27 = (double)puVar9[1];
        }
        if (in_FPSCR_PR == '\0') {
          dVar27 = (double)CONCAT44((int)((ulonglong)dVar27 >> 0x20),
                                    SUB84(dVar27,0) - SUB84(dVar25,0));
        }
        else {
          dVar27 = dVar27 - dVar25;
        }
        if (in_FPSCR_SZ == '\0') {
          dVar25 = (double)CONCAT44((int)((ulonglong)dVar25 >> 0x20),
                                    *(undefined4 *)(DAT_8c619108 + 1));
        }
        fVar30 = (float)((ulonglong)dVar27 >> 0x20);
        if (in_FPSCR_PR == '\0') {
          fVar24 = SUB84(dVar25,0) * fVar30;
          dVar25 = (double)CONCAT44((int)((ulonglong)dVar25 >> 0x20),fVar24);
          dVar27 = (double)CONCAT44(fVar30,SUB84(dVar27,0) - fVar24);
        }
        else {
          dVar25 = dVar25 * dVar27;
          dVar27 = dVar27 - dVar25;
        }
        if (in_FPSCR_SZ == '\0') {
          puVar12 = unaff_r8 + 6;
          *puVar12 = SUB84(dVar27,0);
        }
        else {
          puVar12 = unaff_r8 + 5;
          *(undefined8 *)puVar12 = in_xd2;
        }
        uVar23 = (undefined4)((ulonglong)dVar27 >> 0x20);
        if (in_FPSCR_PR == '\0') {
          dVar27 = (double)CONCAT44(uVar23,(float)(int)psVar19[1] * fVar32);
        }
        else {
          dVar27 = (double)CONCAT44(uVar23,(float)(int)psVar19[1]) * dVar31;
        }
        if (in_FPSCR_SZ == '\0') {
          puVar12 = puVar12 + -1;
          *puVar12 = SUB84(dVar27,0);
        }
        else {
          puVar12 = puVar12 + -2;
          *(undefined8 *)puVar12 = in_xd2;
        }
        if (in_FPSCR_PR == '\0') {
          fVar30 = (float)(int)*psVar19 * fVar32;
        }
        else {
          fVar30 = SUB84((double)CONCAT44((int)((ulonglong)dVar27 >> 0x20),(float)(int)*psVar19) *
                         dVar31,0);
        }
        if (in_FPSCR_SZ == '\0') {
          puVar12[-1] = (uint)fVar30;
          uVar6 = DAT_8c619114;
          puVar12[-2] = *(uint *)(puVar9 + 3);
          puVar12[-3] = *(uint *)((int)puVar9 + 0x14);
          puVar12 = puVar12 + -4;
          *puVar12 = *(uint *)(puVar9 + 2);
        }
        else {
          *(undefined8 *)(puVar12 + -2) = in_xd2;
          uVar6 = DAT_8c619114;
          *(undefined8 *)(puVar12 + -4) = puVar9[3];
          *(undefined8 *)(puVar12 + -6) = *(undefined8 *)((int)puVar9 + 0x14);
          in_xd2 = puVar9[2];
          puVar12 = puVar12 + -8;
          *(undefined8 *)puVar12 = in_xd2;
        }
        puVar12[-1] = uVar6;
        if (in_FPSCR_SZ == '\0') {
          dVar27 = (double)CONCAT44(*(undefined4 *)DAT_8c619108,*(undefined4 *)puVar10);
        }
        else {
          in_xd2 = *puVar10;
          dVar27 = *DAT_8c619108;
        }
        if (in_FPSCR_PR == '\0') {
          dVar27 = (double)CONCAT44((float)((ulonglong)dVar27 >> 0x20) * SUB84(dVar27,0),
                                    SUB84(dVar27,0));
        }
        else {
          dVar27 = dVar27 * dVar27;
        }
        if (in_FPSCR_SZ == '\0') {
          dVar25 = (double)CONCAT44((int)((ulonglong)dVar25 >> 0x20),*puVar4);
          dVar27 = (double)CONCAT44((int)((ulonglong)dVar27 >> 0x20),*(undefined4 *)DAT_8c61910c);
        }
        else {
          in_xd2 = *DAT_8c61910c;
        }
        if (in_FPSCR_PR == '\0') {
          fVar30 = SUB84(dVar27,0) - (float)((ulonglong)dVar27 >> 0x20);
        }
        else {
          fVar30 = SUB84(dVar27 - dVar27,0);
        }
        if (in_FPSCR_SZ == '\0') {
          dVar27 = (double)CONCAT44(*(undefined4 *)((int)puVar10 + 4),fVar30);
        }
        else {
          dVar27 = *(double *)((int)puVar10 + 4);
        }
        if (in_FPSCR_PR == '\0') {
          dVar25 = (double)CONCAT44((int)((ulonglong)dVar25 >> 0x20),
                                    SUB84(dVar25,0) * (float)((ulonglong)dVar27 >> 0x20));
        }
        else {
          dVar25 = dVar25 * dVar27;
        }
        if (in_FPSCR_SZ == '\0') {
          dVar27 = (double)CONCAT44(*(undefined4 *)(puVar10 + 1),SUB84(dVar27,0));
        }
        else {
          dVar27 = (double)puVar10[1];
        }
        if (in_FPSCR_PR == '\0') {
          dVar27 = (double)CONCAT44((int)((ulonglong)dVar27 >> 0x20),
                                    SUB84(dVar27,0) - SUB84(dVar25,0));
        }
        else {
          dVar27 = dVar27 - dVar25;
        }
        if (in_FPSCR_SZ == '\0') {
          dVar25 = (double)CONCAT44((int)((ulonglong)dVar25 >> 0x20),*DAT_8c619118);
        }
        fVar30 = (float)((ulonglong)dVar27 >> 0x20);
        if (in_FPSCR_PR == '\0') {
          fVar24 = SUB84(dVar25,0) * fVar30;
          dVar25 = (double)CONCAT44((int)((ulonglong)dVar25 >> 0x20),fVar24);
          dVar27 = (double)CONCAT44(fVar30,SUB84(dVar27,0) - fVar24);
        }
        else {
          dVar25 = dVar25 * dVar27;
          dVar27 = dVar27 - dVar25;
        }
        if (in_FPSCR_SZ == '\0') {
          puVar12 = puVar12 + 0xd;
          *puVar12 = SUB84(dVar27,0);
        }
        else {
          puVar12 = puVar12 + 0xc;
          *(undefined8 *)puVar12 = in_xd2;
        }
        uVar23 = (undefined4)((ulonglong)dVar27 >> 0x20);
        if (in_FPSCR_PR == '\0') {
          dVar27 = (double)CONCAT44(uVar23,(float)(int)psVar19[3] * fVar32);
        }
        else {
          dVar27 = (double)CONCAT44(uVar23,(float)(int)psVar19[3]) * dVar31;
        }
        if (in_FPSCR_SZ == '\0') {
          puVar12 = puVar12 + -1;
          *puVar12 = SUB84(dVar27,0);
        }
        else {
          puVar12 = puVar12 + -2;
          *(undefined8 *)puVar12 = in_xd2;
        }
        if (in_FPSCR_PR == '\0') {
          fVar30 = (float)(int)psVar19[2] * fVar32;
        }
        else {
          fVar30 = SUB84((double)CONCAT44((int)((ulonglong)dVar27 >> 0x20),(float)(int)psVar19[2]) *
                         dVar31,0);
        }
        if (in_FPSCR_SZ == '\0') {
          puVar12[-1] = (uint)fVar30;
          uVar6 = DAT_8c619114;
          puVar12[-2] = *(uint *)(puVar10 + 3);
          puVar12[-3] = *(uint *)((int)puVar10 + 0x14);
          puVar12 = puVar12 + -4;
          *puVar12 = *(uint *)(puVar10 + 2);
        }
        else {
          *(undefined8 *)(puVar12 + -2) = in_xd2;
          uVar6 = DAT_8c619114;
          *(undefined8 *)(puVar12 + -4) = puVar10[3];
          *(undefined8 *)(puVar12 + -6) = *(undefined8 *)((int)puVar10 + 0x14);
          in_xd2 = puVar10[2];
          puVar12 = puVar12 + -8;
          *(undefined8 *)puVar12 = in_xd2;
        }
        puVar12[-1] = uVar6;
        if (in_FPSCR_SZ == '\0') {
          dVar27 = (double)CONCAT44(*(undefined4 *)DAT_8c619108,*(undefined4 *)puVar7);
        }
        else {
          in_xd2 = *puVar7;
          dVar27 = *DAT_8c619108;
        }
        if (in_FPSCR_PR == '\0') {
          dVar27 = (double)CONCAT44((float)((ulonglong)dVar27 >> 0x20) * SUB84(dVar27,0),
                                    SUB84(dVar27,0));
        }
        else {
          dVar27 = dVar27 * dVar27;
        }
        if (in_FPSCR_SZ == '\0') {
          dVar25 = (double)CONCAT44((int)((ulonglong)dVar25 >> 0x20),*puVar4);
          dVar27 = (double)CONCAT44((int)((ulonglong)dVar27 >> 0x20),*(undefined4 *)DAT_8c61910c);
        }
        else {
          in_xd2 = *DAT_8c61910c;
        }
        if (in_FPSCR_PR == '\0') {
          fVar30 = SUB84(dVar27,0) - (float)((ulonglong)dVar27 >> 0x20);
        }
        else {
          fVar30 = SUB84(dVar27 - dVar27,0);
        }
        if (in_FPSCR_SZ == '\0') {
          dVar27 = (double)CONCAT44(*(undefined4 *)((int)puVar7 + 4),fVar30);
        }
        else {
          dVar27 = *(double *)((int)puVar7 + 4);
        }
        if (in_FPSCR_PR == '\0') {
          dVar25 = (double)CONCAT44((int)((ulonglong)dVar25 >> 0x20),
                                    SUB84(dVar25,0) * (float)((ulonglong)dVar27 >> 0x20));
        }
        else {
          dVar25 = dVar25 * dVar27;
        }
        if (in_FPSCR_SZ == '\0') {
          dVar27 = (double)CONCAT44(*(undefined4 *)(puVar7 + 1),SUB84(dVar27,0));
        }
        else {
          dVar27 = (double)puVar7[1];
        }
        if (in_FPSCR_PR == '\0') {
          dVar27 = (double)CONCAT44((int)((ulonglong)dVar27 >> 0x20),
                                    SUB84(dVar27,0) - SUB84(dVar25,0));
        }
        else {
          dVar27 = dVar27 - dVar25;
        }
        if (in_FPSCR_SZ == '\0') {
          dVar25 = (double)CONCAT44((int)((ulonglong)dVar25 >> 0x20),*DAT_8c619118);
        }
        fVar30 = (float)((ulonglong)dVar27 >> 0x20);
        if (in_FPSCR_PR == '\0') {
          fVar24 = SUB84(dVar25,0) * fVar30;
          dVar25 = (double)CONCAT44((int)((ulonglong)dVar25 >> 0x20),fVar24);
          dVar27 = (double)CONCAT44(fVar30,SUB84(dVar27,0) - fVar24);
        }
        else {
          dVar25 = dVar25 * dVar27;
          dVar27 = dVar27 - dVar25;
        }
        if (in_FPSCR_SZ == '\0') {
          puVar12 = puVar12 + 0xd;
          *puVar12 = SUB84(dVar27,0);
        }
        else {
          puVar12 = puVar12 + 0xc;
          *(undefined8 *)puVar12 = in_xd2;
        }
        uVar23 = (undefined4)((ulonglong)dVar27 >> 0x20);
        if (in_FPSCR_PR == '\0') {
          dVar27 = (double)CONCAT44(uVar23,(float)(int)psVar19[5] * fVar32);
        }
        else {
          dVar27 = (double)CONCAT44(uVar23,(float)(int)psVar19[5]) * dVar31;
        }
        if (in_FPSCR_SZ == '\0') {
          puVar12 = puVar12 + -1;
          *puVar12 = SUB84(dVar27,0);
        }
        else {
          puVar12 = puVar12 + -2;
          *(undefined8 *)puVar12 = in_xd2;
        }
        if (in_FPSCR_PR == '\0') {
          fVar30 = (float)(int)psVar19[4] * fVar32;
        }
        else {
          fVar30 = SUB84((double)CONCAT44((int)((ulonglong)dVar27 >> 0x20),(float)(int)psVar19[4]) *
                         dVar31,0);
        }
        if (in_FPSCR_SZ == '\0') {
          puVar12[-1] = (uint)fVar30;
          uVar6 = DAT_8c619114;
          puVar12[-2] = *(uint *)(puVar7 + 3);
          puVar12[-3] = *(uint *)((int)puVar7 + 0x14);
          puVar12 = puVar12 + -4;
          *puVar12 = *(uint *)(puVar7 + 2);
        }
        else {
          *(undefined8 *)(puVar12 + -2) = in_xd2;
          uVar6 = DAT_8c619114;
          *(undefined8 *)(puVar12 + -4) = puVar7[3];
          *(undefined8 *)(puVar12 + -6) = *(undefined8 *)((int)puVar7 + 0x14);
          in_xd2 = puVar7[2];
          puVar12 = puVar12 + -8;
          *(undefined8 *)puVar12 = in_xd2;
        }
        puVar12[-1] = uVar6;
        if (in_FPSCR_SZ == '\0') {
          dVar27 = (double)CONCAT44(*(undefined4 *)DAT_8c619108,*(undefined4 *)puVar21);
        }
        else {
          in_xd2 = *puVar21;
          dVar27 = *DAT_8c619108;
        }
        if (in_FPSCR_PR == '\0') {
          dVar27 = (double)CONCAT44((float)((ulonglong)dVar27 >> 0x20) * SUB84(dVar27,0),
                                    SUB84(dVar27,0));
        }
        else {
          dVar27 = dVar27 * dVar27;
        }
        if (in_FPSCR_SZ == '\0') {
          dVar25 = (double)CONCAT44((int)((ulonglong)dVar25 >> 0x20),*puVar4);
          dVar27 = (double)CONCAT44((int)((ulonglong)dVar27 >> 0x20),*(undefined4 *)DAT_8c61910c);
        }
        else {
          in_xd2 = *DAT_8c61910c;
        }
        if (in_FPSCR_PR == '\0') {
          fVar30 = SUB84(dVar27,0) - (float)((ulonglong)dVar27 >> 0x20);
        }
        else {
          fVar30 = SUB84(dVar27 - dVar27,0);
        }
        if (in_FPSCR_SZ == '\0') {
          dVar27 = (double)CONCAT44(*(undefined4 *)((int)puVar21 + 4),fVar30);
        }
        else {
          dVar27 = *(double *)((int)puVar21 + 4);
        }
        if (in_FPSCR_PR == '\0') {
          dVar25 = (double)CONCAT44((int)((ulonglong)dVar25 >> 0x20),
                                    SUB84(dVar25,0) * (float)((ulonglong)dVar27 >> 0x20));
        }
        else {
          dVar25 = dVar25 * dVar27;
        }
        if (in_FPSCR_SZ == '\0') {
          dVar27 = (double)CONCAT44(*(undefined4 *)(puVar21 + 1),SUB84(dVar27,0));
        }
        else {
          dVar27 = (double)puVar21[1];
        }
        if (in_FPSCR_PR == '\0') {
          dVar27 = (double)CONCAT44((int)((ulonglong)dVar27 >> 0x20),
                                    SUB84(dVar27,0) - SUB84(dVar25,0));
        }
        else {
          dVar27 = dVar27 - dVar25;
        }
        if (in_FPSCR_SZ == '\0') {
          dVar25 = (double)CONCAT44((int)((ulonglong)dVar25 >> 0x20),*DAT_8c619118);
        }
        fVar30 = (float)((ulonglong)dVar27 >> 0x20);
        if (in_FPSCR_PR == '\0') {
          fVar24 = SUB84(dVar25,0) * fVar30;
          in_dr0 = (double)CONCAT44((int)((ulonglong)dVar25 >> 0x20),fVar24);
          dVar27 = (double)CONCAT44(fVar30,SUB84(dVar27,0) - fVar24);
        }
        else {
          in_dr0 = dVar25 * dVar27;
          dVar27 = dVar27 - in_dr0;
        }
        if (in_FPSCR_SZ == '\0') {
          puVar12 = puVar12 + 0xd;
          *puVar12 = SUB84(dVar27,0);
        }
        else {
          puVar12 = puVar12 + 0xc;
          *(undefined8 *)puVar12 = in_xd2;
        }
        uVar23 = (undefined4)((ulonglong)dVar27 >> 0x20);
        if (in_FPSCR_PR == '\0') {
          dVar27 = (double)CONCAT44(uVar23,(float)(int)psVar19[7] * fVar32);
        }
        else {
          dVar27 = (double)CONCAT44(uVar23,(float)(int)psVar19[7]) * dVar31;
        }
        if (in_FPSCR_SZ == '\0') {
          puVar12 = puVar12 + -1;
          *puVar12 = SUB84(dVar27,0);
        }
        else {
          puVar12 = puVar12 + -2;
          *(undefined8 *)puVar12 = in_xd2;
        }
        if (in_FPSCR_PR == '\0') {
          in_dr2 = (double)(ulonglong)(uint)((float)(int)psVar19[6] * fVar32);
        }
        else {
          in_dr2 = (double)CONCAT44((int)((ulonglong)dVar27 >> 0x20),(float)(int)psVar19[6]) *
                   dVar31;
        }
        if (in_FPSCR_SZ == '\0') {
          puVar12[-1] = SUB84(in_dr2,0);
          uVar6 = DAT_8c61911c;
          puVar12[-2] = *(uint *)(puVar21 + 3);
          puVar12[-3] = *(uint *)((int)puVar21 + 0x14);
          in_dr2 = (double)(ulonglong)*(uint *)(puVar21 + 2);
          puVar12 = puVar12 + -4;
          *puVar12 = *(uint *)(puVar21 + 2);
        }
        else {
          *(undefined8 *)(puVar12 + -2) = in_xd2;
          uVar6 = DAT_8c61911c;
          *(undefined8 *)(puVar12 + -4) = puVar21[3];
          *(undefined8 *)(puVar12 + -6) = *(undefined8 *)((int)puVar21 + 0x14);
          in_xd2 = puVar21[2];
          puVar12 = puVar12 + -8;
          *(undefined8 *)puVar12 = in_xd2;
        }
        puVar12[-1] = uVar6;
        unaff_r8 = puVar12 + 7;
        *DAT_8c619120 = *DAT_8c619120 + 4;
      }
      psVar19 = psVar19 + 8;
    }
  }
  else if (uVar6 == DAT_8c618de0) {
    for (iVar13 = 0; piVar5 = _DAT_8c6192ec, uVar23 = SUB84(param_1,0),
        iVar13 < (int)(uint)param_3[1]; iVar13 = iVar13 + 1) {
      puVar18 = puVar15 + 1;
      uVar6 = (uint)*puVar15;
      *piVar3 = *piVar3 + uVar6;
      iVar14 = *piVar5;
      iVar8 = (uint)puVar15[2] * 0x20 + iVar14;
      if (in_FPSCR_SZ == '\0') {
        uVar23 = *(undefined4 *)(iVar8 + 0x10);
      }
      else {
        in_xd4 = *(double *)(iVar8 + 0x10);
      }
      iVar11 = (uint)*puVar18 * 0x20 + iVar14;
      if (in_FPSCR_SZ == '\0') {
        dVar27 = (double)CONCAT44(uVar23,*(undefined4 *)(iVar11 + 0x10));
      }
      else {
        in_xd2 = *(undefined8 *)(iVar11 + 0x10);
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
        dVar25 = (double)CONCAT44(*(undefined4 *)(iVar8 + 0x14),uVar23);
      }
      else {
        dVar25 = *(double *)(iVar8 + 0x14);
      }
      iVar14 = (uint)puVar15[3] * 0x20 + iVar14;
      fVar30 = (float)((ulonglong)dVar25 >> 0x20);
      if (in_FPSCR_SZ == '\0') {
        dVar27 = (double)CONCAT44((int)((ulonglong)dVar27 >> 0x20),*(undefined4 *)(iVar14 + 0x14));
        in_dr0 = (double)CONCAT44((int)((ulonglong)in_dr0 >> 0x20),fVar30);
      }
      else {
        in_xd2 = *(undefined8 *)(iVar14 + 0x14);
      }
      if (in_FPSCR_PR == '\0') {
        fVar30 = SUB84(dVar27,0) - fVar30;
        dVar27 = (double)CONCAT44((float)((ulonglong)dVar27 >> 0x20) * fVar30,fVar30);
      }
      else {
        dVar27 = (dVar27 - dVar25) * (dVar27 - dVar25);
      }
      if (in_FPSCR_SZ == '\0') {
        dVar27 = (double)CONCAT44((int)((ulonglong)dVar27 >> 0x20),*(undefined4 *)(iVar11 + 0x14));
      }
      else {
        in_xd2 = *(undefined8 *)(iVar11 + 0x14);
      }
      if (in_FPSCR_PR == '\0') {
        dVar22 = (double)CONCAT44((int)((ulonglong)in_dr0 >> 0x20),SUB84(in_dr0,0) - SUB84(dVar27,0)
                                 );
      }
      else {
        dVar22 = in_dr0 - dVar27;
      }
      if (in_FPSCR_SZ == '\0') {
        dVar27 = (double)CONCAT44((int)((ulonglong)dVar27 >> 0x20),*(undefined4 *)(iVar14 + 0x10));
      }
      else {
        in_xd2 = *(undefined8 *)(iVar14 + 0x10);
      }
      if (in_FPSCR_PR == '\0') {
        dVar27 = (double)CONCAT44((int)((ulonglong)dVar27 >> 0x20),SUB84(dVar27,0) - SUB84(dVar25,0)
                                 );
      }
      else {
        dVar27 = dVar27 - dVar25;
      }
      uVar26 = (uint)((ulonglong)dVar27 >> 0x20);
      param_1 = dVar27;
      if (in_FPSCR_SZ == '\0') {
        param_1 = (double)CONCAT44(uVar26,SUB84(dVar25,0));
      }
      if (in_FPSCR_PR == '\0') {
        in_dr0 = (double)CONCAT44((int)((ulonglong)dVar22 >> 0x20),SUB84(dVar22,0) * SUB84(dVar27,0)
                                 );
      }
      else {
        in_dr0 = dVar22 * dVar27;
      }
      if (in_FPSCR_PR == '\0') {
        fVar30 = (float)((ulonglong)param_1 >> 0x20) - SUB84(in_dr0,0);
        param_1 = (double)CONCAT44(fVar30,SUB84(param_1,0));
        bVar33 = 0.0 < fVar30;
      }
      else {
        param_1 = param_1 - in_dr0;
        bVar33 = (double)((ulonglong)uVar26 << 0x20) < param_1;
      }
      if (bVar33) {
        puVar15 = puVar18 + uVar6;
        psVar19 = psVar19 + uVar6 * 2;
      }
      else {
        *DAT_8c6192f0 = *DAT_8c6192f0 + uVar6;
        iVar14 = 0;
        if (0 < (int)(uVar6 - 1)) {
          do {
            uVar2 = *puVar18;
            puVar18 = puVar18 + 1;
            puVar9 = (undefined8 *)((uint)uVar2 * 0x20 + *_DAT_8c6192ec);
            if (in_FPSCR_SZ == '\0') {
              dVar27 = (double)CONCAT44(*(undefined4 *)DAT_8c6192f4,*(undefined4 *)puVar9);
              in_dr0 = (double)CONCAT44((int)((ulonglong)in_dr0 >> 0x20),*DAT_8c6192fc);
            }
            else {
              in_xd2 = *puVar9;
              dVar27 = *DAT_8c6192f4;
            }
            if (in_FPSCR_PR == '\0') {
              dVar27 = (double)CONCAT44((float)((ulonglong)dVar27 >> 0x20) * SUB84(dVar27,0),
                                        SUB84(dVar27,0));
            }
            else {
              dVar27 = dVar27 * dVar27;
            }
            if (in_FPSCR_SZ == '\0') {
              dVar27 = (double)CONCAT44((int)((ulonglong)dVar27 >> 0x20),*(undefined4 *)DAT_8c6192f8
                                       );
            }
            else {
              in_xd2 = *DAT_8c6192f8;
            }
            if (in_FPSCR_PR == '\0') {
              fVar30 = SUB84(dVar27,0) - (float)((ulonglong)dVar27 >> 0x20);
            }
            else {
              fVar30 = SUB84(dVar27 - dVar27,0);
            }
            if (in_FPSCR_SZ == '\0') {
              dVar27 = (double)CONCAT44(*(undefined4 *)((int)puVar9 + 4),fVar30);
            }
            else {
              dVar27 = *(double *)((int)puVar9 + 4);
            }
            if (in_FPSCR_PR == '\0') {
              dVar25 = (double)CONCAT44((int)((ulonglong)in_dr0 >> 0x20),
                                        SUB84(in_dr0,0) * (float)((ulonglong)dVar27 >> 0x20));
            }
            else {
              dVar25 = in_dr0 * dVar27;
            }
            if (in_FPSCR_SZ == '\0') {
              dVar27 = (double)CONCAT44(*(undefined4 *)(puVar9 + 1),SUB84(dVar27,0));
            }
            else {
              dVar27 = (double)puVar9[1];
            }
            if (in_FPSCR_PR == '\0') {
              dVar27 = (double)CONCAT44((int)((ulonglong)dVar27 >> 0x20),
                                        SUB84(dVar27,0) - SUB84(dVar25,0));
            }
            else {
              dVar27 = dVar27 - dVar25;
            }
            if (in_FPSCR_SZ == '\0') {
              dVar25 = (double)CONCAT44((int)((ulonglong)dVar25 >> 0x20),
                                        *(undefined4 *)(DAT_8c6192f4 + 1));
            }
            fVar30 = (float)((ulonglong)dVar27 >> 0x20);
            if (in_FPSCR_PR == '\0') {
              fVar24 = SUB84(dVar25,0) * fVar30;
              in_dr0 = (double)CONCAT44((int)((ulonglong)dVar25 >> 0x20),fVar24);
              dVar27 = (double)CONCAT44(fVar30,SUB84(dVar27,0) - fVar24);
            }
            else {
              in_dr0 = dVar25 * dVar27;
              dVar27 = dVar27 - in_dr0;
            }
            if (in_FPSCR_SZ == '\0') {
              puVar12 = unaff_r8 + 6;
              *puVar12 = SUB84(dVar27,0);
            }
            else {
              puVar12 = unaff_r8 + 5;
              *(undefined8 *)puVar12 = in_xd2;
            }
            uVar23 = (undefined4)((ulonglong)dVar27 >> 0x20);
            if (in_FPSCR_PR == '\0') {
              dVar27 = (double)CONCAT44(uVar23,(float)(int)psVar19[1] * fVar32);
            }
            else {
              dVar27 = (double)CONCAT44(uVar23,(float)(int)psVar19[1]) * dVar31;
            }
            if (in_FPSCR_SZ == '\0') {
              puVar12 = puVar12 + -1;
              *puVar12 = SUB84(dVar27,0);
            }
            else {
              puVar12 = puVar12 + -2;
              *(undefined8 *)puVar12 = in_xd2;
            }
            uVar26 = DAT_8c619300;
            sVar1 = *psVar19;
            psVar19 = psVar19 + 2;
            if (in_FPSCR_PR == '\0') {
              fVar30 = (float)(int)sVar1 * fVar32;
            }
            else {
              fVar30 = SUB84((double)CONCAT44((int)((ulonglong)dVar27 >> 0x20),(float)(int)sVar1) *
                             dVar31,0);
            }
            if (in_FPSCR_SZ == '\0') {
              puVar12[-1] = (uint)fVar30;
              puVar12[-2] = *(uint *)(puVar9 + 3);
              puVar12[-3] = *(uint *)((int)puVar9 + 0x14);
              puVar12 = puVar12 + -4;
              *puVar12 = *(uint *)(puVar9 + 2);
            }
            else {
              *(undefined8 *)(puVar12 + -2) = in_xd2;
              *(undefined8 *)(puVar12 + -4) = puVar9[3];
              *(undefined8 *)(puVar12 + -6) = *(undefined8 *)((int)puVar9 + 0x14);
              in_xd2 = puVar9[2];
              puVar12 = puVar12 + -8;
              *(undefined8 *)puVar12 = in_xd2;
            }
            puVar12[-1] = uVar26;
            iVar14 = iVar14 + 1;
            unaff_r8 = puVar12 + 7;
          } while (iVar14 < (int)(uVar6 - 1));
        }
        puVar15 = puVar18 + 1;
        puVar9 = (undefined8 *)((uint)*puVar18 * 0x20 + *_DAT_8c6192ec);
        if (in_FPSCR_SZ == '\0') {
          dVar27 = (double)CONCAT44(*(undefined4 *)DAT_8c6192f4,*(undefined4 *)puVar9);
          in_dr0 = (double)CONCAT44((int)((ulonglong)in_dr0 >> 0x20),*DAT_8c6192fc);
        }
        else {
          in_xd2 = *puVar9;
          dVar27 = *DAT_8c6192f4;
        }
        if (in_FPSCR_PR == '\0') {
          dVar27 = (double)CONCAT44((float)((ulonglong)dVar27 >> 0x20) * SUB84(dVar27,0),
                                    SUB84(dVar27,0));
        }
        else {
          dVar27 = dVar27 * dVar27;
        }
        if (in_FPSCR_SZ == '\0') {
          dVar27 = (double)CONCAT44((int)((ulonglong)dVar27 >> 0x20),*(undefined4 *)DAT_8c6192f8);
        }
        else {
          in_xd2 = *DAT_8c6192f8;
        }
        if (in_FPSCR_PR == '\0') {
          fVar30 = SUB84(dVar27,0) - (float)((ulonglong)dVar27 >> 0x20);
        }
        else {
          fVar30 = SUB84(dVar27 - dVar27,0);
        }
        if (in_FPSCR_SZ == '\0') {
          dVar27 = (double)CONCAT44(*(undefined4 *)((int)puVar9 + 4),fVar30);
        }
        else {
          dVar27 = *(double *)((int)puVar9 + 4);
        }
        if (in_FPSCR_PR == '\0') {
          dVar25 = (double)CONCAT44((int)((ulonglong)in_dr0 >> 0x20),
                                    SUB84(in_dr0,0) * (float)((ulonglong)dVar27 >> 0x20));
        }
        else {
          dVar25 = in_dr0 * dVar27;
        }
        if (in_FPSCR_SZ == '\0') {
          dVar27 = (double)CONCAT44(*(undefined4 *)(puVar9 + 1),SUB84(dVar27,0));
        }
        else {
          dVar27 = (double)puVar9[1];
        }
        if (in_FPSCR_PR == '\0') {
          dVar27 = (double)CONCAT44((int)((ulonglong)dVar27 >> 0x20),
                                    SUB84(dVar27,0) - SUB84(dVar25,0));
        }
        else {
          dVar27 = dVar27 - dVar25;
        }
        if (in_FPSCR_SZ == '\0') {
          dVar25 = (double)CONCAT44((int)((ulonglong)dVar25 >> 0x20),
                                    *(undefined4 *)(DAT_8c6192f4 + 1));
        }
        fVar30 = (float)((ulonglong)dVar27 >> 0x20);
        if (in_FPSCR_PR == '\0') {
          fVar24 = SUB84(dVar25,0) * fVar30;
          in_dr0 = (double)CONCAT44((int)((ulonglong)dVar25 >> 0x20),fVar24);
          dVar27 = (double)CONCAT44(fVar30,SUB84(dVar27,0) - fVar24);
        }
        else {
          in_dr0 = dVar25 * dVar27;
          dVar27 = dVar27 - in_dr0;
        }
        if (in_FPSCR_SZ == '\0') {
          puVar12 = unaff_r8 + 6;
          *puVar12 = SUB84(dVar27,0);
        }
        else {
          puVar12 = unaff_r8 + 5;
          *(undefined8 *)puVar12 = in_xd2;
        }
        uVar23 = (undefined4)((ulonglong)dVar27 >> 0x20);
        if (in_FPSCR_PR == '\0') {
          dVar27 = (double)CONCAT44(uVar23,(float)(int)psVar19[1] * fVar32);
        }
        else {
          dVar27 = (double)CONCAT44(uVar23,(float)(int)psVar19[1]) * dVar31;
        }
        if (in_FPSCR_SZ == '\0') {
          puVar12 = puVar12 + -1;
          *puVar12 = SUB84(dVar27,0);
        }
        else {
          puVar12 = puVar12 + -2;
          *(undefined8 *)puVar12 = in_xd2;
        }
        uVar6 = DAT_8c619304;
        sVar1 = *psVar19;
        psVar19 = psVar19 + 2;
        if (in_FPSCR_PR == '\0') {
          fVar30 = (float)(int)sVar1 * fVar32;
        }
        else {
          fVar30 = SUB84((double)CONCAT44((int)((ulonglong)dVar27 >> 0x20),(float)(int)sVar1) *
                         dVar31,0);
        }
        if (in_FPSCR_SZ == '\0') {
          puVar12[-1] = (uint)fVar30;
          puVar12[-2] = *(uint *)(puVar9 + 3);
          puVar12[-3] = *(uint *)((int)puVar9 + 0x14);
          puVar12 = puVar12 + -4;
          *puVar12 = *(uint *)(puVar9 + 2);
        }
        else {
          *(undefined8 *)(puVar12 + -2) = in_xd2;
          *(undefined8 *)(puVar12 + -4) = puVar9[3];
          *(undefined8 *)(puVar12 + -6) = *(undefined8 *)((int)puVar9 + 0x14);
          in_xd2 = puVar9[2];
          puVar12 = puVar12 + -8;
          *(undefined8 *)puVar12 = in_xd2;
        }
        puVar12[-1] = uVar6;
        unaff_r8 = puVar12 + 7;
      }
    }
  }
  else if (uVar6 == DAT_8c618ddc) {
    *DAT_8c619308 = param_3;
    if (in_FPSCR_SZ == '\0') {
      uVar28 = CONCAT44(DAT_8c6192e8,SUB84(param_1,0));
    }
    else {
      uVar28 = CONCAT44(_DAT_8c6192ec,DAT_8c6192e8);
    }
    (*(code *)PTR_FUN_8c61930c)(uVar28,*DAT_8c619308);
  }
  return;
}

