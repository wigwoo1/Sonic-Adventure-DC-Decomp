/* Ghidra draft only; not buildable/verified source. */

/* WARNING: Removing unreachable block (ram,0x8c619552) */
/* WARNING: Removing unreachable block (ram,0x8c619536) */
/* WARNING: Removing unreachable block (ram,0x8c6194f8) */
/* WARNING: Removing unreachable block (ram,0x8c619452) */
/* WARNING: Removing unreachable block (ram,0x8c61983a) */
/* WARNING: Removing unreachable block (ram,0x8c6196c4) */
/* WARNING: Removing unreachable block (ram,0x8c619728) */
/* WARNING: Removing unreachable block (ram,0x8c6197d8) */
/* WARNING: Removing unreachable block (ram,0x8c61971c) */
/* WARNING: Removing unreachable block (ram,0x8c6196ce) */
/* WARNING: Removing unreachable block (ram,0x8c6197cc) */
/* WARNING: Removing unreachable block (ram,0x8c619774) */
/* WARNING: Removing unreachable block (ram,0x8c61980a) */
/* WARNING: Removing unreachable block (ram,0x8c619a9c) */
/* WARNING: Removing unreachable block (ram,0x8c619ae2) */
/* WARNING: Removing unreachable block (ram,0x8c619a1a) */
/* WARNING: Removing unreachable block (ram,0x8c619a2a) */
/* WARNING: Removing unreachable block (ram,0x8c619a8c) */
/* WARNING: Removing unreachable block (ram,0x8c619af2) */
/* WARNING: Removing unreachable block (ram,0x8c619448) */
/* WARNING: Removing unreachable block (ram,0x8c6194a0) */
/* WARNING: Removing unreachable block (ram,0x8c6194ac) */
/* WARNING: Removing unreachable block (ram,0x8c619504) */
/* WARNING: Removing unreachable block (ram,0x8c619546) */
/* WARNING: Removing unreachable block (ram,0x8c61956e) */
/* WARNING: Removing unreachable block (ram,0x8c619828) */
/* WARNING: Removing unreachable block (ram,0x8c619780) */
/* WARNING: Removing unreachable block (ram,0x8c61981a) */
/* WARNING: Removing unreachable block (ram,0x8c619562) */
/* WARNING: Removing unreachable block (ram,0x8c619580) */
/* WARNING: Removing unreachable block (ram,0x8c619b1a) */
/* WARNING: Removing unreachable block (ram,0x8c619848) */
/* WARNING: Removing unreachable block (ram,0x8c619b08) */
/* WARNING: Removing unreachable block (ram,0x8c619b3c) */
/* WARNING: Removing unreachable block (ram,0x8c61985a) */
/* WARNING: Removing unreachable block (ram,0x8c619b4c) */
/* WARNING: Removing unreachable block (ram,0x8c619868) */
/* WARNING: Removing unreachable block (ram,0x8c619878) */
/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_8c619310(double param_1,ushort *param_2)

{
  short sVar1;
  ushort uVar2;
  ushort uVar3;
  short *psVar4;
  undefined2 *puVar5;
  undefined2 *puVar6;
  undefined4 *puVar7;
  undefined2 *puVar8;
  undefined *puVar9;
  int *piVar10;
  uint uVar11;
  undefined8 *puVar12;
  uint uVar13;
  int iVar14;
  ushort *puVar15;
  undefined4 *unaff_r8;
  undefined8 *puVar16;
  ushort *puVar17;
  ushort *puVar18;
  ushort *puVar19;
  short *psVar20;
  undefined8 *puVar21;
  int iVar22;
  undefined8 *puVar23;
  uint uVar24;
  int iVar25;
  undefined8 *puVar26;
  int iVar27;
  uint uVar28;
  undefined1 *puVar29;
  float fVar30;
  double in_dr0;
  double dVar31;
  double in_dr2;
  double dVar32;
  undefined4 uVar33;
  double dVar34;
  float fVar37;
  undefined8 uVar35;
  float in_fr15;
  undefined4 in_fr14;
  undefined8 in_xd0;
  undefined8 in_xd2;
  double in_xd4;
  bool bVar38;
  undefined4 in_PR;
  undefined2 uVar39;
  char in_FPSCR_PR;
  char in_FPSCR_SZ;
  undefined1 local_20 [4];
  undefined1 local_1c [4];
  undefined4 uVar36;
  
  uVar36 = DAT_8c619688;
  puVar8 = DAT_8c61967c;
  puVar6 = DAT_8c619410;
  puVar5 = DAT_8c61940c;
  if (in_FPSCR_SZ == '\0') {
    puVar29 = local_1c;
  }
  else {
    puVar29 = local_20;
  }
  *(undefined4 *)(puVar29 + -4) = in_PR;
  if (in_FPSCR_SZ == '\0') {
    in_fr15 = DAT_8c6193fc;
  }
  *(ushort **)(puVar29 + -0x14) = param_2;
  psVar20 = *(short **)(param_2 + 10);
  uVar11 = *param_2 & _DAT_8c619400;
  puVar17 = *(ushort **)(param_2 + 2);
  if (uVar11 == 0) {
    *DAT_8c619408 = *DAT_8c619408 + (uint)*(ushort *)(*(int *)(puVar29 + -0x14) + 2) * 3;
    for (iVar14 = 0; puVar7 = DAT_8c619664, uVar36 = SUB84(param_1,0),
        iVar14 < (int)(uint)*(ushort *)(*(int *)(puVar29 + -0x14) + 2); iVar14 = iVar14 + 1) {
      uVar2 = *puVar17;
      puVar15 = puVar17 + 1;
      puVar19 = puVar17 + 2;
      iVar27 = *DAT_8c619414;
      puVar17 = puVar17 + 3;
      puVar23 = (undefined8 *)((uint)*puVar19 * 0x20 + iVar27);
      puVar12 = (undefined8 *)((uint)uVar2 * 0x20 + iVar27);
      puVar21 = (undefined8 *)((uint)*puVar15 * 0x20 + iVar27);
      if ((*(uint *)((int)puVar12 + 0xc) & *(uint *)((int)puVar21 + 0xc) &
           *(uint *)((int)puVar23 + 0xc) & 0x3f) == 0) {
        if (((*(uint *)((int)puVar12 + 0xc) | *(uint *)((int)puVar21 + 0xc) |
             *(uint *)((int)puVar23 + 0xc)) & 1) == 0) {
          if (in_FPSCR_SZ == '\0') {
            uVar36 = *(undefined4 *)(puVar21 + 2);
            dVar32 = (double)CONCAT44(uVar36,*(undefined4 *)(puVar12 + 2));
          }
          else {
            dVar32 = (double)puVar21[2];
            in_xd2 = puVar12[2];
          }
          if (in_FPSCR_PR == '\0') {
            dVar32 = (double)CONCAT44((float)((ulonglong)dVar32 >> 0x20) - SUB84(dVar32,0),
                                      SUB84(dVar32,0));
          }
          else {
            dVar32 = dVar32 - dVar32;
          }
          if (in_FPSCR_SZ == '\0') {
            dVar32 = (double)CONCAT44((int)((ulonglong)dVar32 >> 0x20),
                                      *(undefined4 *)((int)puVar23 + 0x14));
            dVar34 = (double)CONCAT44(*(undefined4 *)((int)puVar21 + 0x14),uVar36);
          }
          else {
            in_xd2 = *(undefined8 *)((int)puVar23 + 0x14);
            dVar34 = *(double *)((int)puVar21 + 0x14);
          }
          fVar37 = (float)((ulonglong)dVar34 >> 0x20);
          if (in_FPSCR_PR == '\0') {
            dVar32 = (double)CONCAT44((int)((ulonglong)dVar32 >> 0x20),SUB84(dVar32,0) - fVar37);
          }
          else {
            dVar32 = dVar32 - dVar34;
          }
          if (in_FPSCR_SZ == '\0') {
            in_dr0 = (double)CONCAT44((int)((ulonglong)in_dr0 >> 0x20),fVar37);
          }
          if (in_FPSCR_PR == '\0') {
            dVar32 = (double)CONCAT44((float)((ulonglong)dVar32 >> 0x20) * SUB84(dVar32,0),
                                      SUB84(dVar32,0));
          }
          else {
            dVar32 = dVar32 * dVar32;
          }
          if (in_FPSCR_SZ == '\0') {
            dVar32 = (double)CONCAT44((int)((ulonglong)dVar32 >> 0x20),
                                      *(undefined4 *)((int)puVar12 + 0x14));
          }
          else {
            in_xd2 = *(undefined8 *)((int)puVar12 + 0x14);
          }
          if (in_FPSCR_PR == '\0') {
            dVar31 = (double)CONCAT44((int)((ulonglong)in_dr0 >> 0x20),
                                      SUB84(in_dr0,0) - SUB84(dVar32,0));
          }
          else {
            dVar31 = in_dr0 - dVar32;
          }
          if (in_FPSCR_SZ == '\0') {
            dVar32 = (double)CONCAT44((int)((ulonglong)dVar32 >> 0x20),*(undefined4 *)(puVar23 + 2))
            ;
          }
          else {
            in_xd2 = puVar23[2];
          }
          if (in_FPSCR_PR == '\0') {
            dVar32 = (double)CONCAT44((int)((ulonglong)dVar32 >> 0x20),
                                      SUB84(dVar32,0) - SUB84(dVar34,0));
          }
          else {
            dVar32 = dVar32 - dVar34;
          }
          uVar11 = (uint)((ulonglong)dVar32 >> 0x20);
          param_1 = dVar32;
          if (in_FPSCR_SZ == '\0') {
            param_1 = (double)CONCAT44(uVar11,SUB84(dVar34,0));
          }
          if (in_FPSCR_PR == '\0') {
            in_dr0 = (double)CONCAT44((int)((ulonglong)dVar31 >> 0x20),
                                      SUB84(dVar31,0) * SUB84(dVar32,0));
          }
          else {
            in_dr0 = dVar31 * dVar32;
          }
          in_dr2 = (double)((ulonglong)uVar11 << 0x20);
          if (in_FPSCR_PR == '\0') {
            fVar37 = (float)((ulonglong)param_1 >> 0x20) - SUB84(in_dr0,0);
            param_1 = (double)CONCAT44(fVar37,SUB84(param_1,0));
            bVar38 = 0.0 < fVar37;
          }
          else {
            param_1 = param_1 - in_dr0;
            bVar38 = in_dr2 < param_1;
          }
          if (!bVar38) {
            if (in_FPSCR_SZ == '\0') {
              dVar32 = (double)CONCAT44(*(undefined4 *)DAT_8c61965c,*(undefined4 *)puVar12);
            }
            else {
              in_xd2 = *puVar12;
              dVar32 = *DAT_8c61965c;
            }
            if (in_FPSCR_PR == '\0') {
              dVar32 = (double)CONCAT44((float)((ulonglong)dVar32 >> 0x20) * SUB84(dVar32,0),
                                        SUB84(dVar32,0));
            }
            else {
              dVar32 = dVar32 * dVar32;
            }
            if (in_FPSCR_SZ == '\0') {
              dVar32 = (double)CONCAT44((int)((ulonglong)dVar32 >> 0x20),*(undefined4 *)DAT_8c619660
                                       );
              in_dr0 = (double)CONCAT44((int)((ulonglong)in_dr0 >> 0x20),*DAT_8c619664);
            }
            else {
              in_xd2 = *DAT_8c619660;
            }
            if (in_FPSCR_PR == '\0') {
              fVar37 = SUB84(dVar32,0) - (float)((ulonglong)dVar32 >> 0x20);
            }
            else {
              fVar37 = SUB84(dVar32 - dVar32,0);
            }
            if (in_FPSCR_SZ == '\0') {
              dVar32 = (double)CONCAT44(*(undefined4 *)((int)puVar12 + 4),fVar37);
            }
            else {
              dVar32 = *(double *)((int)puVar12 + 4);
            }
            if (in_FPSCR_PR == '\0') {
              dVar34 = (double)CONCAT44((int)((ulonglong)in_dr0 >> 0x20),
                                        SUB84(in_dr0,0) * (float)((ulonglong)dVar32 >> 0x20));
            }
            else {
              dVar34 = in_dr0 * dVar32;
            }
            if (in_FPSCR_SZ == '\0') {
              dVar32 = (double)CONCAT44(*(undefined4 *)(puVar12 + 1),SUB84(dVar32,0));
            }
            else {
              dVar32 = (double)puVar12[1];
            }
            if (in_FPSCR_PR == '\0') {
              dVar32 = (double)CONCAT44((int)((ulonglong)dVar32 >> 0x20),
                                        SUB84(dVar32,0) - SUB84(dVar34,0));
            }
            else {
              dVar32 = dVar32 - dVar34;
            }
            if (in_FPSCR_SZ == '\0') {
              dVar34 = (double)CONCAT44((int)((ulonglong)dVar34 >> 0x20),
                                        *(undefined4 *)(DAT_8c61965c + 1));
            }
            fVar37 = (float)((ulonglong)dVar32 >> 0x20);
            if (in_FPSCR_PR == '\0') {
              fVar30 = SUB84(dVar34,0) * fVar37;
              dVar34 = (double)CONCAT44((int)((ulonglong)dVar34 >> 0x20),fVar30);
              dVar32 = (double)CONCAT44(fVar37,SUB84(dVar32,0) - fVar30);
            }
            else {
              dVar34 = dVar34 * dVar32;
              dVar32 = dVar32 - dVar34;
            }
            if (in_FPSCR_SZ == '\0') {
              puVar26 = (undefined8 *)(unaff_r8 + 6);
              *(int *)puVar26 = SUB84(dVar32,0);
            }
            else {
              puVar26 = (undefined8 *)(unaff_r8 + 5);
              *puVar26 = in_xd2;
            }
            uVar36 = (undefined4)((ulonglong)dVar32 >> 0x20);
            if (in_FPSCR_PR == '\0') {
              dVar32 = (double)CONCAT44(uVar36,(float)(int)psVar20[1] * in_fr15);
            }
            else {
              dVar32 = (double)CONCAT44(uVar36,(float)(int)psVar20[1]) *
                       (double)CONCAT44(in_fr14,in_fr15);
            }
            if (in_FPSCR_SZ == '\0') {
              puVar26 = (undefined8 *)((int)puVar26 + -4);
              *(int *)puVar26 = SUB84(dVar32,0);
            }
            else {
              puVar26 = puVar26 + -1;
              *puVar26 = in_xd2;
            }
            if (in_FPSCR_PR == '\0') {
              fVar37 = (float)(int)*psVar20 * in_fr15;
            }
            else {
              fVar37 = SUB84((double)CONCAT44((int)((ulonglong)dVar32 >> 0x20),(float)(int)*psVar20)
                             * (double)CONCAT44(in_fr14,in_fr15),0);
            }
            if (in_FPSCR_SZ == '\0') {
              *(float *)((int)puVar26 + -4) = fVar37;
              uVar36 = DAT_8c619668;
              *(undefined4 *)(puVar26 + -1) = *(undefined4 *)(puVar12 + 3);
              *(undefined4 *)((int)puVar26 + -0xc) = *(undefined4 *)((int)puVar12 + 0x14);
              puVar26 = puVar26 + -2;
              *(undefined4 *)puVar26 = *(undefined4 *)(puVar12 + 2);
            }
            else {
              puVar26[-1] = in_xd2;
              uVar36 = DAT_8c619668;
              puVar26[-2] = puVar12[3];
              puVar26[-3] = *(undefined8 *)((int)puVar12 + 0x14);
              in_xd2 = puVar12[2];
              puVar26 = puVar26 + -4;
              *puVar26 = in_xd2;
            }
            *(undefined4 *)((int)puVar26 + -4) = uVar36;
            if (in_FPSCR_SZ == '\0') {
              dVar32 = (double)CONCAT44(*(undefined4 *)DAT_8c61965c,*(undefined4 *)puVar21);
            }
            else {
              in_xd2 = *puVar21;
              dVar32 = *DAT_8c61965c;
            }
            if (in_FPSCR_PR == '\0') {
              dVar32 = (double)CONCAT44((float)((ulonglong)dVar32 >> 0x20) * SUB84(dVar32,0),
                                        SUB84(dVar32,0));
            }
            else {
              dVar32 = dVar32 * dVar32;
            }
            if (in_FPSCR_SZ == '\0') {
              dVar34 = (double)CONCAT44((int)((ulonglong)dVar34 >> 0x20),*puVar7);
              dVar32 = (double)CONCAT44((int)((ulonglong)dVar32 >> 0x20),*(undefined4 *)DAT_8c619660
                                       );
            }
            else {
              in_xd2 = *DAT_8c619660;
            }
            if (in_FPSCR_PR == '\0') {
              fVar37 = SUB84(dVar32,0) - (float)((ulonglong)dVar32 >> 0x20);
            }
            else {
              fVar37 = SUB84(dVar32 - dVar32,0);
            }
            if (in_FPSCR_SZ == '\0') {
              dVar32 = (double)CONCAT44(*(undefined4 *)((int)puVar21 + 4),fVar37);
            }
            else {
              dVar32 = *(double *)((int)puVar21 + 4);
            }
            if (in_FPSCR_PR == '\0') {
              dVar34 = (double)CONCAT44((int)((ulonglong)dVar34 >> 0x20),
                                        SUB84(dVar34,0) * (float)((ulonglong)dVar32 >> 0x20));
            }
            else {
              dVar34 = dVar34 * dVar32;
            }
            if (in_FPSCR_SZ == '\0') {
              dVar32 = (double)CONCAT44(*(undefined4 *)(puVar21 + 1),SUB84(dVar32,0));
            }
            else {
              dVar32 = (double)puVar21[1];
            }
            if (in_FPSCR_PR == '\0') {
              dVar32 = (double)CONCAT44((int)((ulonglong)dVar32 >> 0x20),
                                        SUB84(dVar32,0) - SUB84(dVar34,0));
            }
            else {
              dVar32 = dVar32 - dVar34;
            }
            if (in_FPSCR_SZ == '\0') {
              dVar34 = (double)CONCAT44((int)((ulonglong)dVar34 >> 0x20),*DAT_8c61966c);
            }
            fVar37 = (float)((ulonglong)dVar32 >> 0x20);
            if (in_FPSCR_PR == '\0') {
              fVar30 = SUB84(dVar34,0) * fVar37;
              dVar34 = (double)CONCAT44((int)((ulonglong)dVar34 >> 0x20),fVar30);
              dVar32 = (double)CONCAT44(fVar37,SUB84(dVar32,0) - fVar30);
            }
            else {
              dVar34 = dVar34 * dVar32;
              dVar32 = dVar32 - dVar34;
            }
            if (in_FPSCR_SZ == '\0') {
              puVar26 = (undefined8 *)((int)puVar26 + 0x34);
              *(int *)puVar26 = SUB84(dVar32,0);
            }
            else {
              puVar26 = puVar26 + 6;
              *puVar26 = in_xd2;
            }
            uVar36 = (undefined4)((ulonglong)dVar32 >> 0x20);
            if (in_FPSCR_PR == '\0') {
              dVar32 = (double)CONCAT44(uVar36,(float)(int)psVar20[3] * in_fr15);
            }
            else {
              dVar32 = (double)CONCAT44(uVar36,(float)(int)psVar20[3]) *
                       (double)CONCAT44(in_fr14,in_fr15);
            }
            if (in_FPSCR_SZ == '\0') {
              puVar26 = (undefined8 *)((int)puVar26 + -4);
              *(int *)puVar26 = SUB84(dVar32,0);
            }
            else {
              puVar26 = puVar26 + -1;
              *puVar26 = in_xd2;
            }
            if (in_FPSCR_PR == '\0') {
              fVar37 = (float)(int)psVar20[2] * in_fr15;
            }
            else {
              fVar37 = SUB84((double)CONCAT44((int)((ulonglong)dVar32 >> 0x20),
                                              (float)(int)psVar20[2]) *
                             (double)CONCAT44(in_fr14,in_fr15),0);
            }
            if (in_FPSCR_SZ == '\0') {
              *(float *)((int)puVar26 + -4) = fVar37;
              uVar36 = DAT_8c619668;
              *(undefined4 *)(puVar26 + -1) = *(undefined4 *)(puVar21 + 3);
              *(undefined4 *)((int)puVar26 + -0xc) = *(undefined4 *)((int)puVar21 + 0x14);
              puVar26 = puVar26 + -2;
              *(undefined4 *)puVar26 = *(undefined4 *)(puVar21 + 2);
            }
            else {
              puVar26[-1] = in_xd2;
              uVar36 = DAT_8c619668;
              puVar26[-2] = puVar21[3];
              puVar26[-3] = *(undefined8 *)((int)puVar21 + 0x14);
              in_xd2 = puVar21[2];
              puVar26 = puVar26 + -4;
              *puVar26 = in_xd2;
            }
            *(undefined4 *)((int)puVar26 + -4) = uVar36;
            if (in_FPSCR_SZ == '\0') {
              dVar32 = (double)CONCAT44(*(undefined4 *)DAT_8c61965c,*(undefined4 *)puVar23);
            }
            else {
              in_xd2 = *puVar23;
              dVar32 = *DAT_8c61965c;
            }
            if (in_FPSCR_PR == '\0') {
              dVar32 = (double)CONCAT44((float)((ulonglong)dVar32 >> 0x20) * SUB84(dVar32,0),
                                        SUB84(dVar32,0));
            }
            else {
              dVar32 = dVar32 * dVar32;
            }
            if (in_FPSCR_SZ == '\0') {
              dVar34 = (double)CONCAT44((int)((ulonglong)dVar34 >> 0x20),*puVar7);
              dVar32 = (double)CONCAT44((int)((ulonglong)dVar32 >> 0x20),*(undefined4 *)DAT_8c619660
                                       );
            }
            else {
              in_xd2 = *DAT_8c619660;
            }
            if (in_FPSCR_PR == '\0') {
              fVar37 = SUB84(dVar32,0) - (float)((ulonglong)dVar32 >> 0x20);
            }
            else {
              fVar37 = SUB84(dVar32 - dVar32,0);
            }
            if (in_FPSCR_SZ == '\0') {
              dVar32 = (double)CONCAT44(*(undefined4 *)((int)puVar23 + 4),fVar37);
            }
            else {
              dVar32 = *(double *)((int)puVar23 + 4);
            }
            if (in_FPSCR_PR == '\0') {
              dVar34 = (double)CONCAT44((int)((ulonglong)dVar34 >> 0x20),
                                        SUB84(dVar34,0) * (float)((ulonglong)dVar32 >> 0x20));
            }
            else {
              dVar34 = dVar34 * dVar32;
            }
            if (in_FPSCR_SZ == '\0') {
              dVar32 = (double)CONCAT44(*(undefined4 *)(puVar23 + 1),SUB84(dVar32,0));
            }
            else {
              dVar32 = (double)puVar23[1];
            }
            if (in_FPSCR_PR == '\0') {
              dVar32 = (double)CONCAT44((int)((ulonglong)dVar32 >> 0x20),
                                        SUB84(dVar32,0) - SUB84(dVar34,0));
            }
            else {
              dVar32 = dVar32 - dVar34;
            }
            if (in_FPSCR_SZ == '\0') {
              dVar34 = (double)CONCAT44((int)((ulonglong)dVar34 >> 0x20),*DAT_8c61966c);
            }
            fVar37 = (float)((ulonglong)dVar32 >> 0x20);
            if (in_FPSCR_PR == '\0') {
              fVar30 = SUB84(dVar34,0) * fVar37;
              in_dr0 = (double)CONCAT44((int)((ulonglong)dVar34 >> 0x20),fVar30);
              dVar32 = (double)CONCAT44(fVar37,SUB84(dVar32,0) - fVar30);
            }
            else {
              in_dr0 = dVar34 * dVar32;
              dVar32 = dVar32 - in_dr0;
            }
            if (in_FPSCR_SZ == '\0') {
              puVar26 = (undefined8 *)((int)puVar26 + 0x34);
              *(int *)puVar26 = SUB84(dVar32,0);
            }
            else {
              puVar26 = puVar26 + 6;
              *puVar26 = in_xd2;
            }
            uVar36 = (undefined4)((ulonglong)dVar32 >> 0x20);
            if (in_FPSCR_PR == '\0') {
              dVar32 = (double)CONCAT44(uVar36,(float)(int)psVar20[5] * in_fr15);
            }
            else {
              dVar32 = (double)CONCAT44(uVar36,(float)(int)psVar20[5]) *
                       (double)CONCAT44(in_fr14,in_fr15);
            }
            if (in_FPSCR_SZ == '\0') {
              puVar26 = (undefined8 *)((int)puVar26 + -4);
              *(int *)puVar26 = SUB84(dVar32,0);
            }
            else {
              puVar26 = puVar26 + -1;
              *puVar26 = in_xd2;
            }
            uVar36 = (undefined4)((ulonglong)dVar32 >> 0x20);
            if (in_FPSCR_PR == '\0') {
              in_dr2 = (double)CONCAT44(uVar36,(float)(int)psVar20[4] * in_fr15);
            }
            else {
              in_dr2 = (double)CONCAT44(uVar36,(float)(int)psVar20[4]) *
                       (double)CONCAT44(in_fr14,in_fr15);
            }
            if (in_FPSCR_SZ == '\0') {
              *(int *)((int)puVar26 + -4) = SUB84(in_dr2,0);
              uVar36 = DAT_8c619670;
              *(undefined4 *)(puVar26 + -1) = *(undefined4 *)(puVar23 + 3);
              *(undefined4 *)((int)puVar26 + -0xc) = *(undefined4 *)((int)puVar23 + 0x14);
              in_dr2 = (double)CONCAT44((int)((ulonglong)in_dr2 >> 0x20),
                                        *(undefined4 *)(puVar23 + 2));
              puVar26 = puVar26 + -2;
              *(undefined4 *)puVar26 = *(undefined4 *)(puVar23 + 2);
            }
            else {
              puVar26[-1] = in_xd2;
              uVar36 = DAT_8c619670;
              puVar26[-2] = puVar23[3];
              puVar26[-3] = *(undefined8 *)((int)puVar23 + 0x14);
              in_xd2 = puVar23[2];
              puVar26 = puVar26 + -4;
              *puVar26 = in_xd2;
            }
            *(undefined4 *)((int)puVar26 + -4) = uVar36;
            unaff_r8 = (undefined4 *)((int)puVar26 + 0x1c);
            *DAT_8c619674 = *DAT_8c619674 + 3;
          }
        }
        else {
          *DAT_8c619678 = 1;
          uVar36 = (undefined4)((ulonglong)in_dr2 >> 0x20);
          if (in_FPSCR_PR == '\0') {
            uVar39 = (undefined2)(int)(float)(int)*psVar20;
          }
          else {
            uVar39 = (undefined2)(int)(double)CONCAT44(uVar36,(float)(int)*psVar20);
          }
          *puVar5 = uVar39;
          if (in_FPSCR_PR == '\0') {
            uVar39 = (undefined2)(int)(float)(int)psVar20[1];
          }
          else {
            uVar39 = (undefined2)(int)(double)CONCAT44(uVar36,(float)(int)psVar20[1]);
          }
          puVar5[1] = uVar39;
          if (in_FPSCR_PR == '\0') {
            uVar39 = (undefined2)(int)(float)(int)psVar20[2];
          }
          else {
            uVar39 = (undefined2)(int)(double)CONCAT44(uVar36,(float)(int)psVar20[2]);
          }
          puVar5[2] = uVar39;
          if (in_FPSCR_PR == '\0') {
            uVar39 = (undefined2)(int)(float)(int)psVar20[3];
          }
          else {
            uVar39 = (undefined2)(int)(double)CONCAT44(uVar36,(float)(int)psVar20[3]);
          }
          puVar5[3] = uVar39;
          puVar8 = DAT_8c61967c;
          if (in_FPSCR_PR == '\0') {
            uVar39 = (undefined2)(int)(float)(int)psVar20[4];
          }
          else {
            uVar39 = (undefined2)(int)(double)CONCAT44(uVar36,(float)(int)psVar20[4]);
          }
          *puVar6 = uVar39;
          puVar9 = PTR_FUN_8c619680;
          in_dr2 = (double)CONCAT44(uVar36,(float)(int)psVar20[5]);
          if (in_FPSCR_PR == '\0') {
            uVar39 = (undefined2)(int)(float)(int)psVar20[5];
          }
          else {
            uVar39 = (undefined2)(int)in_dr2;
          }
          puVar6[1] = uVar39;
          in_dr0 = (double)(*(code *)puVar9)(puVar8);
        }
      }
      psVar20 = psVar20 + 6;
    }
  }
  else if (uVar11 == (int)DAT_8c6193fa) {
    *DAT_8c619684 = *DAT_8c619684 + (uint)*(ushort *)(*(int *)(puVar29 + -0x14) + 2) * 4;
    *(undefined4 *)(puVar29 + -0xc) = 0;
    *(undefined2 **)(puVar29 + -0x1c) = puVar8 + 2;
    *(undefined4 *)(puVar29 + -0x10) = uVar36;
    *(undefined4 *)(puVar29 + -0x18) = DAT_8c61968c;
    while (puVar7 = DAT_8c6199a0,
          *(int *)(puVar29 + -0xc) < (int)(uint)*(ushort *)(*(int *)(puVar29 + -0x14) + 2)) {
      uVar2 = *puVar17;
      puVar15 = puVar17 + 1;
      puVar19 = puVar17 + 2;
      iVar14 = *DAT_8c619690;
      puVar18 = puVar17 + 3;
      puVar17 = puVar17 + 4;
      puVar12 = (undefined8 *)((uint)uVar2 * 0x20 + iVar14);
      puVar26 = (undefined8 *)((uint)*puVar15 * 0x20 + iVar14);
      puVar23 = (undefined8 *)((uint)*puVar19 * 0x20 + iVar14);
      puVar21 = (undefined8 *)((uint)*puVar18 * 0x20 + iVar14);
      if ((*(uint *)((int)puVar12 + 0xc) & *(uint *)((int)puVar26 + 0xc) &
           *(uint *)((int)puVar23 + 0xc) & *(uint *)((int)puVar21 + 0xc) & 0x3f) == 0) {
        if (((*(uint *)((int)puVar12 + 0xc) | *(uint *)((int)puVar26 + 0xc) |
              *(uint *)((int)puVar23 + 0xc) | *(uint *)((int)puVar21 + 0xc)) & 1) == 0) {
          if (in_FPSCR_SZ == '\0') {
            uVar36 = *(undefined4 *)(puVar26 + 2);
            dVar34 = (double)CONCAT44(uVar36,*(undefined4 *)(puVar12 + 2));
            dVar32 = (double)CONCAT44(uVar36,SUB84(param_1,0));
          }
          else {
            dVar34 = (double)puVar26[2];
            in_xd2 = puVar12[2];
            uVar36 = (undefined4)((ulonglong)dVar34 >> 0x20);
            dVar32 = dVar34;
          }
          if (in_FPSCR_PR == '\0') {
            dVar34 = (double)CONCAT44((float)((ulonglong)dVar34 >> 0x20) - SUB84(dVar34,0),
                                      SUB84(dVar34,0));
          }
          else {
            dVar34 = dVar34 - dVar34;
          }
          if (in_FPSCR_SZ == '\0') {
            dVar34 = (double)CONCAT44((int)((ulonglong)dVar34 >> 0x20),
                                      *(undefined4 *)((int)puVar23 + 0x14));
            dVar32 = (double)CONCAT44(uVar36,*(undefined4 *)((int)puVar26 + 0x14));
          }
          else {
            in_xd2 = *(undefined8 *)((int)puVar23 + 0x14);
          }
          fVar37 = SUB84(dVar32,0);
          if (in_FPSCR_PR == '\0') {
            dVar34 = (double)CONCAT44((int)((ulonglong)dVar34 >> 0x20),SUB84(dVar34,0) - fVar37);
          }
          else {
            dVar34 = dVar34 - dVar32;
          }
          if (in_FPSCR_SZ == '\0') {
            in_dr0 = (double)CONCAT44((int)((ulonglong)in_dr0 >> 0x20),fVar37);
          }
          if (in_FPSCR_PR == '\0') {
            dVar34 = (double)CONCAT44((float)((ulonglong)dVar34 >> 0x20) * SUB84(dVar34,0),
                                      SUB84(dVar34,0));
          }
          else {
            dVar34 = dVar34 * dVar34;
          }
          if (in_FPSCR_SZ == '\0') {
            dVar34 = (double)CONCAT44((int)((ulonglong)dVar34 >> 0x20),
                                      *(undefined4 *)((int)puVar12 + 0x14));
          }
          else {
            in_xd2 = *(undefined8 *)((int)puVar12 + 0x14);
          }
          if (in_FPSCR_PR == '\0') {
            dVar31 = (double)CONCAT44((int)((ulonglong)in_dr0 >> 0x20),
                                      SUB84(in_dr0,0) - SUB84(dVar34,0));
          }
          else {
            dVar31 = in_dr0 - dVar34;
          }
          if (in_FPSCR_SZ == '\0') {
            dVar34 = (double)CONCAT44((int)((ulonglong)dVar34 >> 0x20),*(undefined4 *)(puVar23 + 2))
            ;
          }
          else {
            in_xd2 = puVar23[2];
          }
          if (in_FPSCR_PR == '\0') {
            dVar34 = (double)CONCAT44((int)((ulonglong)dVar34 >> 0x20),
                                      SUB84(dVar34,0) - (float)((ulonglong)dVar32 >> 0x20));
          }
          else {
            dVar34 = dVar34 - dVar32;
          }
          uVar11 = (uint)((ulonglong)dVar34 >> 0x20);
          param_1 = dVar34;
          if (in_FPSCR_SZ == '\0') {
            param_1 = (double)CONCAT44(uVar11,fVar37);
          }
          if (in_FPSCR_PR == '\0') {
            in_dr0 = (double)CONCAT44((int)((ulonglong)dVar31 >> 0x20),
                                      SUB84(dVar31,0) * SUB84(dVar34,0));
          }
          else {
            in_dr0 = dVar31 * dVar34;
          }
          in_dr2 = (double)((ulonglong)uVar11 << 0x20);
          if (in_FPSCR_PR == '\0') {
            fVar37 = (float)((ulonglong)param_1 >> 0x20) - SUB84(in_dr0,0);
            param_1 = (double)CONCAT44(fVar37,SUB84(param_1,0));
            bVar38 = 0.0 < fVar37;
          }
          else {
            param_1 = param_1 - in_dr0;
            bVar38 = in_dr2 < param_1;
          }
          if (!bVar38) {
            if (in_FPSCR_SZ == '\0') {
              dVar32 = (double)CONCAT44(*(undefined4 *)DAT_8c619998,*(undefined4 *)puVar12);
            }
            else {
              in_xd2 = *puVar12;
              dVar32 = *DAT_8c619998;
            }
            if (in_FPSCR_PR == '\0') {
              dVar32 = (double)CONCAT44((float)((ulonglong)dVar32 >> 0x20) * SUB84(dVar32,0),
                                        SUB84(dVar32,0));
            }
            else {
              dVar32 = dVar32 * dVar32;
            }
            if (in_FPSCR_SZ == '\0') {
              dVar32 = (double)CONCAT44((int)((ulonglong)dVar32 >> 0x20),*(undefined4 *)DAT_8c61999c
                                       );
              in_dr0 = (double)CONCAT44((int)((ulonglong)in_dr0 >> 0x20),*DAT_8c6199a0);
            }
            else {
              in_xd2 = *DAT_8c61999c;
            }
            if (in_FPSCR_PR == '\0') {
              fVar37 = SUB84(dVar32,0) - (float)((ulonglong)dVar32 >> 0x20);
            }
            else {
              fVar37 = SUB84(dVar32 - dVar32,0);
            }
            if (in_FPSCR_SZ == '\0') {
              dVar32 = (double)CONCAT44(*(undefined4 *)((int)puVar12 + 4),fVar37);
            }
            else {
              dVar32 = *(double *)((int)puVar12 + 4);
            }
            if (in_FPSCR_PR == '\0') {
              dVar34 = (double)CONCAT44((int)((ulonglong)in_dr0 >> 0x20),
                                        SUB84(in_dr0,0) * (float)((ulonglong)dVar32 >> 0x20));
            }
            else {
              dVar34 = in_dr0 * dVar32;
            }
            if (in_FPSCR_SZ == '\0') {
              dVar32 = (double)CONCAT44(*(undefined4 *)(puVar12 + 1),SUB84(dVar32,0));
            }
            else {
              dVar32 = (double)puVar12[1];
            }
            if (in_FPSCR_PR == '\0') {
              dVar32 = (double)CONCAT44((int)((ulonglong)dVar32 >> 0x20),
                                        SUB84(dVar32,0) - SUB84(dVar34,0));
            }
            else {
              dVar32 = dVar32 - dVar34;
            }
            if (in_FPSCR_SZ == '\0') {
              dVar34 = (double)CONCAT44((int)((ulonglong)dVar34 >> 0x20),
                                        *(undefined4 *)(DAT_8c619998 + 1));
            }
            fVar37 = (float)((ulonglong)dVar32 >> 0x20);
            if (in_FPSCR_PR == '\0') {
              fVar30 = SUB84(dVar34,0) * fVar37;
              dVar34 = (double)CONCAT44((int)((ulonglong)dVar34 >> 0x20),fVar30);
              dVar32 = (double)CONCAT44(fVar37,SUB84(dVar32,0) - fVar30);
            }
            else {
              dVar34 = dVar34 * dVar32;
              dVar32 = dVar32 - dVar34;
            }
            if (in_FPSCR_SZ == '\0') {
              puVar16 = (undefined8 *)(unaff_r8 + 6);
              *(int *)puVar16 = SUB84(dVar32,0);
            }
            else {
              puVar16 = (undefined8 *)(unaff_r8 + 5);
              *puVar16 = in_xd2;
            }
            uVar36 = (undefined4)((ulonglong)dVar32 >> 0x20);
            if (in_FPSCR_PR == '\0') {
              dVar32 = (double)CONCAT44(uVar36,(float)(int)psVar20[1] * in_fr15);
            }
            else {
              dVar32 = (double)CONCAT44(uVar36,(float)(int)psVar20[1]) *
                       (double)CONCAT44(in_fr14,in_fr15);
            }
            if (in_FPSCR_SZ == '\0') {
              puVar16 = (undefined8 *)((int)puVar16 + -4);
              *(int *)puVar16 = SUB84(dVar32,0);
            }
            else {
              puVar16 = puVar16 + -1;
              *puVar16 = in_xd2;
            }
            if (in_FPSCR_PR == '\0') {
              fVar37 = (float)(int)*psVar20 * in_fr15;
            }
            else {
              fVar37 = SUB84((double)CONCAT44((int)((ulonglong)dVar32 >> 0x20),(float)(int)*psVar20)
                             * (double)CONCAT44(in_fr14,in_fr15),0);
            }
            if (in_FPSCR_SZ == '\0') {
              *(float *)((int)puVar16 + -4) = fVar37;
              uVar36 = DAT_8c6199a4;
              *(undefined4 *)(puVar16 + -1) = *(undefined4 *)(puVar12 + 3);
              *(undefined4 *)((int)puVar16 + -0xc) = *(undefined4 *)((int)puVar12 + 0x14);
              puVar16 = puVar16 + -2;
              *(undefined4 *)puVar16 = *(undefined4 *)(puVar12 + 2);
            }
            else {
              puVar16[-1] = in_xd2;
              uVar36 = DAT_8c6199a4;
              puVar16[-2] = puVar12[3];
              puVar16[-3] = *(undefined8 *)((int)puVar12 + 0x14);
              in_xd2 = puVar12[2];
              puVar16 = puVar16 + -4;
              *puVar16 = in_xd2;
            }
            *(undefined4 *)((int)puVar16 + -4) = uVar36;
            if (in_FPSCR_SZ == '\0') {
              dVar32 = (double)CONCAT44(*(undefined4 *)DAT_8c619998,*(undefined4 *)puVar26);
            }
            else {
              in_xd2 = *puVar26;
              dVar32 = *DAT_8c619998;
            }
            if (in_FPSCR_PR == '\0') {
              dVar32 = (double)CONCAT44((float)((ulonglong)dVar32 >> 0x20) * SUB84(dVar32,0),
                                        SUB84(dVar32,0));
            }
            else {
              dVar32 = dVar32 * dVar32;
            }
            if (in_FPSCR_SZ == '\0') {
              dVar34 = (double)CONCAT44((int)((ulonglong)dVar34 >> 0x20),*puVar7);
              dVar32 = (double)CONCAT44((int)((ulonglong)dVar32 >> 0x20),*(undefined4 *)DAT_8c61999c
                                       );
            }
            else {
              in_xd2 = *DAT_8c61999c;
            }
            if (in_FPSCR_PR == '\0') {
              fVar37 = SUB84(dVar32,0) - (float)((ulonglong)dVar32 >> 0x20);
            }
            else {
              fVar37 = SUB84(dVar32 - dVar32,0);
            }
            if (in_FPSCR_SZ == '\0') {
              dVar32 = (double)CONCAT44(*(undefined4 *)((int)puVar26 + 4),fVar37);
            }
            else {
              dVar32 = *(double *)((int)puVar26 + 4);
            }
            if (in_FPSCR_PR == '\0') {
              dVar34 = (double)CONCAT44((int)((ulonglong)dVar34 >> 0x20),
                                        SUB84(dVar34,0) * (float)((ulonglong)dVar32 >> 0x20));
            }
            else {
              dVar34 = dVar34 * dVar32;
            }
            if (in_FPSCR_SZ == '\0') {
              dVar32 = (double)CONCAT44(*(undefined4 *)(puVar26 + 1),SUB84(dVar32,0));
            }
            else {
              dVar32 = (double)puVar26[1];
            }
            if (in_FPSCR_PR == '\0') {
              dVar32 = (double)CONCAT44((int)((ulonglong)dVar32 >> 0x20),
                                        SUB84(dVar32,0) - SUB84(dVar34,0));
            }
            else {
              dVar32 = dVar32 - dVar34;
            }
            if (in_FPSCR_SZ == '\0') {
              dVar34 = (double)CONCAT44((int)((ulonglong)dVar34 >> 0x20),*DAT_8c6199a8);
            }
            fVar37 = (float)((ulonglong)dVar32 >> 0x20);
            if (in_FPSCR_PR == '\0') {
              fVar30 = SUB84(dVar34,0) * fVar37;
              dVar34 = (double)CONCAT44((int)((ulonglong)dVar34 >> 0x20),fVar30);
              dVar32 = (double)CONCAT44(fVar37,SUB84(dVar32,0) - fVar30);
            }
            else {
              dVar34 = dVar34 * dVar32;
              dVar32 = dVar32 - dVar34;
            }
            if (in_FPSCR_SZ == '\0') {
              puVar16 = (undefined8 *)((int)puVar16 + 0x34);
              *(int *)puVar16 = SUB84(dVar32,0);
            }
            else {
              puVar16 = puVar16 + 6;
              *puVar16 = in_xd2;
            }
            uVar36 = (undefined4)((ulonglong)dVar32 >> 0x20);
            if (in_FPSCR_PR == '\0') {
              dVar32 = (double)CONCAT44(uVar36,(float)(int)psVar20[3] * in_fr15);
            }
            else {
              dVar32 = (double)CONCAT44(uVar36,(float)(int)psVar20[3]) *
                       (double)CONCAT44(in_fr14,in_fr15);
            }
            if (in_FPSCR_SZ == '\0') {
              puVar16 = (undefined8 *)((int)puVar16 + -4);
              *(int *)puVar16 = SUB84(dVar32,0);
            }
            else {
              puVar16 = puVar16 + -1;
              *puVar16 = in_xd2;
            }
            if (in_FPSCR_PR == '\0') {
              fVar37 = (float)(int)psVar20[2] * in_fr15;
            }
            else {
              fVar37 = SUB84((double)CONCAT44((int)((ulonglong)dVar32 >> 0x20),
                                              (float)(int)psVar20[2]) *
                             (double)CONCAT44(in_fr14,in_fr15),0);
            }
            if (in_FPSCR_SZ == '\0') {
              *(float *)((int)puVar16 + -4) = fVar37;
              uVar36 = DAT_8c6199a4;
              *(undefined4 *)(puVar16 + -1) = *(undefined4 *)(puVar26 + 3);
              *(undefined4 *)((int)puVar16 + -0xc) = *(undefined4 *)((int)puVar26 + 0x14);
              puVar16 = puVar16 + -2;
              *(undefined4 *)puVar16 = *(undefined4 *)(puVar26 + 2);
            }
            else {
              puVar16[-1] = in_xd2;
              uVar36 = DAT_8c6199a4;
              puVar16[-2] = puVar26[3];
              puVar16[-3] = *(undefined8 *)((int)puVar26 + 0x14);
              in_xd2 = puVar26[2];
              puVar16 = puVar16 + -4;
              *puVar16 = in_xd2;
            }
            *(undefined4 *)((int)puVar16 + -4) = uVar36;
            if (in_FPSCR_SZ == '\0') {
              dVar32 = (double)CONCAT44(*(undefined4 *)DAT_8c619998,*(undefined4 *)puVar23);
            }
            else {
              in_xd2 = *puVar23;
              dVar32 = *DAT_8c619998;
            }
            if (in_FPSCR_PR == '\0') {
              dVar32 = (double)CONCAT44((float)((ulonglong)dVar32 >> 0x20) * SUB84(dVar32,0),
                                        SUB84(dVar32,0));
            }
            else {
              dVar32 = dVar32 * dVar32;
            }
            if (in_FPSCR_SZ == '\0') {
              dVar34 = (double)CONCAT44((int)((ulonglong)dVar34 >> 0x20),*puVar7);
              dVar32 = (double)CONCAT44((int)((ulonglong)dVar32 >> 0x20),*(undefined4 *)DAT_8c61999c
                                       );
            }
            else {
              in_xd2 = *DAT_8c61999c;
            }
            if (in_FPSCR_PR == '\0') {
              fVar37 = SUB84(dVar32,0) - (float)((ulonglong)dVar32 >> 0x20);
            }
            else {
              fVar37 = SUB84(dVar32 - dVar32,0);
            }
            if (in_FPSCR_SZ == '\0') {
              dVar32 = (double)CONCAT44(*(undefined4 *)((int)puVar23 + 4),fVar37);
            }
            else {
              dVar32 = *(double *)((int)puVar23 + 4);
            }
            if (in_FPSCR_PR == '\0') {
              dVar34 = (double)CONCAT44((int)((ulonglong)dVar34 >> 0x20),
                                        SUB84(dVar34,0) * (float)((ulonglong)dVar32 >> 0x20));
            }
            else {
              dVar34 = dVar34 * dVar32;
            }
            if (in_FPSCR_SZ == '\0') {
              dVar32 = (double)CONCAT44(*(undefined4 *)(puVar23 + 1),SUB84(dVar32,0));
            }
            else {
              dVar32 = (double)puVar23[1];
            }
            if (in_FPSCR_PR == '\0') {
              dVar32 = (double)CONCAT44((int)((ulonglong)dVar32 >> 0x20),
                                        SUB84(dVar32,0) - SUB84(dVar34,0));
            }
            else {
              dVar32 = dVar32 - dVar34;
            }
            if (in_FPSCR_SZ == '\0') {
              dVar34 = (double)CONCAT44((int)((ulonglong)dVar34 >> 0x20),*DAT_8c6199a8);
            }
            fVar37 = (float)((ulonglong)dVar32 >> 0x20);
            if (in_FPSCR_PR == '\0') {
              fVar30 = SUB84(dVar34,0) * fVar37;
              dVar34 = (double)CONCAT44((int)((ulonglong)dVar34 >> 0x20),fVar30);
              dVar32 = (double)CONCAT44(fVar37,SUB84(dVar32,0) - fVar30);
            }
            else {
              dVar34 = dVar34 * dVar32;
              dVar32 = dVar32 - dVar34;
            }
            if (in_FPSCR_SZ == '\0') {
              puVar16 = (undefined8 *)((int)puVar16 + 0x34);
              *(int *)puVar16 = SUB84(dVar32,0);
            }
            else {
              puVar16 = puVar16 + 6;
              *puVar16 = in_xd2;
            }
            uVar36 = (undefined4)((ulonglong)dVar32 >> 0x20);
            if (in_FPSCR_PR == '\0') {
              dVar32 = (double)CONCAT44(uVar36,(float)(int)psVar20[5] * in_fr15);
            }
            else {
              dVar32 = (double)CONCAT44(uVar36,(float)(int)psVar20[5]) *
                       (double)CONCAT44(in_fr14,in_fr15);
            }
            if (in_FPSCR_SZ == '\0') {
              puVar16 = (undefined8 *)((int)puVar16 + -4);
              *(int *)puVar16 = SUB84(dVar32,0);
            }
            else {
              puVar16 = puVar16 + -1;
              *puVar16 = in_xd2;
            }
            if (in_FPSCR_PR == '\0') {
              fVar37 = (float)(int)psVar20[4] * in_fr15;
            }
            else {
              fVar37 = SUB84((double)CONCAT44((int)((ulonglong)dVar32 >> 0x20),
                                              (float)(int)psVar20[4]) *
                             (double)CONCAT44(in_fr14,in_fr15),0);
            }
            if (in_FPSCR_SZ == '\0') {
              *(float *)((int)puVar16 + -4) = fVar37;
              uVar36 = DAT_8c6199a4;
              *(undefined4 *)(puVar16 + -1) = *(undefined4 *)(puVar23 + 3);
              *(undefined4 *)((int)puVar16 + -0xc) = *(undefined4 *)((int)puVar23 + 0x14);
              puVar16 = puVar16 + -2;
              *(undefined4 *)puVar16 = *(undefined4 *)(puVar23 + 2);
            }
            else {
              puVar16[-1] = in_xd2;
              uVar36 = DAT_8c6199a4;
              puVar16[-2] = puVar23[3];
              puVar16[-3] = *(undefined8 *)((int)puVar23 + 0x14);
              in_xd2 = puVar23[2];
              puVar16 = puVar16 + -4;
              *puVar16 = in_xd2;
            }
            *(undefined4 *)((int)puVar16 + -4) = uVar36;
            if (in_FPSCR_SZ == '\0') {
              dVar32 = (double)CONCAT44(*(undefined4 *)DAT_8c619998,*(undefined4 *)puVar21);
            }
            else {
              in_xd2 = *puVar21;
              dVar32 = *DAT_8c619998;
            }
            if (in_FPSCR_PR == '\0') {
              dVar32 = (double)CONCAT44((float)((ulonglong)dVar32 >> 0x20) * SUB84(dVar32,0),
                                        SUB84(dVar32,0));
            }
            else {
              dVar32 = dVar32 * dVar32;
            }
            if (in_FPSCR_SZ == '\0') {
              dVar34 = (double)CONCAT44((int)((ulonglong)dVar34 >> 0x20),*puVar7);
              dVar32 = (double)CONCAT44((int)((ulonglong)dVar32 >> 0x20),*(undefined4 *)DAT_8c61999c
                                       );
            }
            else {
              in_xd2 = *DAT_8c61999c;
            }
            if (in_FPSCR_PR == '\0') {
              fVar37 = SUB84(dVar32,0) - (float)((ulonglong)dVar32 >> 0x20);
            }
            else {
              fVar37 = SUB84(dVar32 - dVar32,0);
            }
            if (in_FPSCR_SZ == '\0') {
              dVar32 = (double)CONCAT44(*(undefined4 *)((int)puVar21 + 4),fVar37);
            }
            else {
              dVar32 = *(double *)((int)puVar21 + 4);
            }
            if (in_FPSCR_PR == '\0') {
              dVar34 = (double)CONCAT44((int)((ulonglong)dVar34 >> 0x20),
                                        SUB84(dVar34,0) * (float)((ulonglong)dVar32 >> 0x20));
            }
            else {
              dVar34 = dVar34 * dVar32;
            }
            if (in_FPSCR_SZ == '\0') {
              dVar32 = (double)CONCAT44(*(undefined4 *)(puVar21 + 1),SUB84(dVar32,0));
            }
            else {
              dVar32 = (double)puVar21[1];
            }
            if (in_FPSCR_PR == '\0') {
              dVar32 = (double)CONCAT44((int)((ulonglong)dVar32 >> 0x20),
                                        SUB84(dVar32,0) - SUB84(dVar34,0));
            }
            else {
              dVar32 = dVar32 - dVar34;
            }
            if (in_FPSCR_SZ == '\0') {
              dVar34 = (double)CONCAT44((int)((ulonglong)dVar34 >> 0x20),*DAT_8c6199a8);
            }
            fVar37 = (float)((ulonglong)dVar32 >> 0x20);
            if (in_FPSCR_PR == '\0') {
              fVar30 = SUB84(dVar34,0) * fVar37;
              in_dr0 = (double)CONCAT44((int)((ulonglong)dVar34 >> 0x20),fVar30);
              dVar32 = (double)CONCAT44(fVar37,SUB84(dVar32,0) - fVar30);
            }
            else {
              in_dr0 = dVar34 * dVar32;
              dVar32 = dVar32 - in_dr0;
            }
            if (in_FPSCR_SZ == '\0') {
              puVar16 = (undefined8 *)((int)puVar16 + 0x34);
              *(int *)puVar16 = SUB84(dVar32,0);
            }
            else {
              puVar16 = puVar16 + 6;
              *puVar16 = in_xd2;
            }
            uVar36 = (undefined4)((ulonglong)dVar32 >> 0x20);
            if (in_FPSCR_PR == '\0') {
              dVar32 = (double)CONCAT44(uVar36,(float)(int)psVar20[7] * in_fr15);
            }
            else {
              dVar32 = (double)CONCAT44(uVar36,(float)(int)psVar20[7]) *
                       (double)CONCAT44(in_fr14,in_fr15);
            }
            if (in_FPSCR_SZ == '\0') {
              puVar16 = (undefined8 *)((int)puVar16 + -4);
              *(int *)puVar16 = SUB84(dVar32,0);
            }
            else {
              puVar16 = puVar16 + -1;
              *puVar16 = in_xd2;
            }
            uVar36 = (undefined4)((ulonglong)dVar32 >> 0x20);
            if (in_FPSCR_PR == '\0') {
              in_dr2 = (double)CONCAT44(uVar36,(float)(int)psVar20[6] * in_fr15);
            }
            else {
              in_dr2 = (double)CONCAT44(uVar36,(float)(int)psVar20[6]) *
                       (double)CONCAT44(in_fr14,in_fr15);
            }
            if (in_FPSCR_SZ == '\0') {
              *(int *)((int)puVar16 + -4) = SUB84(in_dr2,0);
              uVar36 = DAT_8c6199ac;
              *(undefined4 *)(puVar16 + -1) = *(undefined4 *)(puVar21 + 3);
              *(undefined4 *)((int)puVar16 + -0xc) = *(undefined4 *)((int)puVar21 + 0x14);
              in_dr2 = (double)CONCAT44((int)((ulonglong)in_dr2 >> 0x20),
                                        *(undefined4 *)(puVar21 + 2));
              puVar16 = puVar16 + -2;
              *(undefined4 *)puVar16 = *(undefined4 *)(puVar21 + 2);
            }
            else {
              puVar16[-1] = in_xd2;
              uVar36 = DAT_8c6199ac;
              puVar16[-2] = puVar21[3];
              puVar16[-3] = *(undefined8 *)((int)puVar21 + 0x14);
              in_xd2 = puVar21[2];
              puVar16 = puVar16 + -4;
              *puVar16 = in_xd2;
            }
            *(undefined4 *)((int)puVar16 + -4) = uVar36;
            unaff_r8 = (undefined4 *)((int)puVar16 + 0x1c);
            *DAT_8c6199b0 = *DAT_8c6199b0 + 4;
          }
        }
        else {
          *DAT_8c6199b4 = 1;
          uVar36 = (undefined4)((ulonglong)in_dr2 >> 0x20);
          if (in_FPSCR_PR == '\0') {
            uVar39 = (undefined2)(int)(float)(int)*psVar20;
          }
          else {
            uVar39 = (undefined2)(int)(double)CONCAT44(uVar36,(float)(int)*psVar20);
          }
          *puVar8 = uVar39;
          if (in_FPSCR_PR == '\0') {
            uVar39 = (undefined2)(int)(float)(int)psVar20[1];
          }
          else {
            uVar39 = (undefined2)(int)(double)CONCAT44(uVar36,(float)(int)psVar20[1]);
          }
          puVar8[1] = uVar39;
          if (in_FPSCR_PR == '\0') {
            uVar39 = (undefined2)(int)(float)(int)psVar20[2];
          }
          else {
            uVar39 = (undefined2)(int)(double)CONCAT44(uVar36,(float)(int)psVar20[2]);
          }
          **(undefined2 **)(puVar29 + -0x1c) = uVar39;
          if (in_FPSCR_PR == '\0') {
            uVar39 = (undefined2)(int)(float)(int)psVar20[3];
          }
          else {
            uVar39 = (undefined2)(int)(double)CONCAT44(uVar36,(float)(int)psVar20[3]);
          }
          *(undefined2 *)(*(int *)(puVar29 + -0x1c) + 2) = uVar39;
          if (in_FPSCR_PR == '\0') {
            uVar39 = (undefined2)(int)(float)(int)psVar20[4];
          }
          else {
            uVar39 = (undefined2)(int)(double)CONCAT44(uVar36,(float)(int)psVar20[4]);
          }
          **(undefined2 **)(puVar29 + -0x10) = uVar39;
          if (in_FPSCR_PR == '\0') {
            uVar39 = (undefined2)(int)(float)(int)psVar20[5];
          }
          else {
            uVar39 = (undefined2)(int)(double)CONCAT44(uVar36,(float)(int)psVar20[5]);
          }
          *(undefined2 *)(*(int *)(puVar29 + -0x10) + 2) = uVar39;
          if (in_FPSCR_PR == '\0') {
            uVar39 = (undefined2)(int)(float)(int)psVar20[6];
          }
          else {
            uVar39 = (undefined2)(int)(double)CONCAT44(uVar36,(float)(int)psVar20[6]);
          }
          **(undefined2 **)(puVar29 + -0x18) = uVar39;
          in_dr2 = (double)CONCAT44(uVar36,(float)(int)psVar20[7]);
          if (in_FPSCR_PR == '\0') {
            uVar39 = (undefined2)(int)(float)(int)psVar20[7];
          }
          else {
            uVar39 = (undefined2)(int)in_dr2;
          }
          *(undefined2 *)(*(int *)(puVar29 + -0x18) + 2) = uVar39;
          if ((*(uint *)((int)puVar12 + 0xc) & *(uint *)((int)puVar26 + 0xc) &
               *(uint *)((int)puVar23 + 0xc) & 1) == 0) {
            in_dr0 = (double)(*(code *)PTR_FUN_8c6199b8)(puVar26,puVar23,puVar8);
          }
          if ((*(uint *)((int)puVar26 + 0xc) & *(uint *)((int)puVar23 + 0xc) &
               *(uint *)((int)puVar21 + 0xc) & 1) == 0) {
            in_dr0 = (double)(*(code *)PTR_FUN_8c6199b8)
                                       (puVar26,puVar23,puVar21,*(undefined4 *)(puVar29 + -0x1c));
          }
        }
      }
      psVar20 = psVar20 + 8;
      *(int *)(puVar29 + -0xc) = *(int *)(puVar29 + -0xc) + 1;
    }
  }
  else if (uVar11 == DAT_8c619404) {
    *(undefined4 *)(puVar29 + -0xc) = 0;
    *(int *)(puVar29 + -0x18) = DAT_8c6199bc + 8;
    *(int *)(puVar29 + -0x10) = DAT_8c6199bc;
    *(int *)(puVar29 + -0x1c) = DAT_8c6199bc + 4;
    while (uVar36 = SUB84(param_1,0),
          *(int *)(puVar29 + -0xc) < (int)(uint)*(ushort *)(*(int *)(puVar29 + -0x14) + 2)) {
      uVar13 = 0;
      puVar19 = puVar17 + 1;
      iVar14 = 0;
      uVar24 = (uint)*puVar17;
      *DAT_8c6199c0 = *DAT_8c6199c0 + uVar24;
      piVar10 = DAT_8c619c08;
      uVar11 = 0xffffffff;
      puVar15 = puVar19;
      if (uVar24 != 0) {
        do {
          uVar2 = *puVar15;
          puVar15 = puVar15 + 1;
          iVar14 = iVar14 + 1;
          iVar27 = (uint)uVar2 * 0x20 + *DAT_8c6199c4;
          *(int *)(puVar29 + -8) = iVar27;
          uVar28 = *(uint *)(iVar27 + 0xc);
          uVar11 = uVar11 & uVar28;
          uVar13 = uVar13 | uVar28;
        } while (iVar14 < (int)uVar24);
      }
      if ((uVar11 & 0x3f) == 0) {
        if ((uVar13 & 1) == 0) {
          iVar27 = *DAT_8c6199c4;
          iVar14 = (uint)puVar17[2] * 0x20 + iVar27;
          if (in_FPSCR_SZ == '\0') {
            uVar36 = *(undefined4 *)(iVar14 + 0x10);
          }
          else {
            in_xd4 = *(double *)(iVar14 + 0x10);
          }
          iVar22 = (uint)*puVar19 * 0x20 + iVar27;
          if (in_FPSCR_SZ == '\0') {
            dVar32 = (double)CONCAT44(uVar36,*(undefined4 *)(iVar22 + 0x10));
          }
          else {
            in_xd2 = *(undefined8 *)(iVar22 + 0x10);
            dVar32 = in_xd4;
          }
          if (in_FPSCR_PR == '\0') {
            dVar32 = (double)CONCAT44((float)((ulonglong)dVar32 >> 0x20) - SUB84(dVar32,0),
                                      SUB84(dVar32,0));
          }
          else {
            dVar32 = dVar32 - dVar32;
          }
          iVar27 = (uint)puVar17[3] * 0x20 + iVar27;
          if (in_FPSCR_SZ == '\0') {
            fVar37 = *(float *)(iVar14 + 0x14);
            dVar34 = (double)CONCAT44(fVar37,uVar36);
            dVar32 = (double)CONCAT44((int)((ulonglong)dVar32 >> 0x20),
                                      *(undefined4 *)(iVar27 + 0x14));
            in_dr0 = (double)CONCAT44((int)((ulonglong)in_dr0 >> 0x20),fVar37);
          }
          else {
            dVar34 = *(double *)(iVar14 + 0x14);
            in_xd2 = *(undefined8 *)(iVar27 + 0x14);
            fVar37 = (float)((ulonglong)dVar34 >> 0x20);
          }
          if (in_FPSCR_PR == '\0') {
            fVar37 = SUB84(dVar32,0) - fVar37;
            param_1 = (double)CONCAT44((float)((ulonglong)dVar32 >> 0x20) * fVar37,fVar37);
          }
          else {
            param_1 = (dVar32 - dVar34) * (dVar32 - dVar34);
          }
          if (in_FPSCR_SZ == '\0') {
            param_1 = (double)CONCAT44((int)((ulonglong)param_1 >> 0x20),
                                       *(undefined4 *)(iVar22 + 0x14));
          }
          else {
            in_xd2 = *(undefined8 *)(iVar22 + 0x14);
          }
          if (in_FPSCR_PR == '\0') {
            dVar32 = (double)CONCAT44((int)((ulonglong)in_dr0 >> 0x20),
                                      SUB84(in_dr0,0) - SUB84(param_1,0));
          }
          else {
            dVar32 = in_dr0 - param_1;
          }
          uVar36 = (undefined4)((ulonglong)param_1 >> 0x20);
          if (in_FPSCR_SZ == '\0') {
            param_1 = (double)CONCAT44(uVar36,SUB84(dVar34,0));
            dVar34 = (double)CONCAT44(uVar36,*(undefined4 *)(iVar27 + 0x10));
          }
          else {
            in_xd2 = *(undefined8 *)(iVar27 + 0x10);
            dVar34 = param_1;
          }
          if (in_FPSCR_PR == '\0') {
            fVar37 = SUB84(dVar34,0) - SUB84(param_1,0);
            dVar34 = (double)CONCAT44(uVar36,fVar37);
            in_dr0 = (double)CONCAT44((int)((ulonglong)dVar32 >> 0x20),SUB84(dVar32,0) * fVar37);
          }
          else {
            dVar34 = dVar34 - param_1;
            in_dr0 = dVar32 * dVar34;
          }
          in_dr2 = (double)((ulonglong)dVar34 & 0xffffffff00000000);
          if (in_FPSCR_PR == '\0') {
            fVar37 = (float)((ulonglong)param_1 >> 0x20) - SUB84(in_dr0,0);
            param_1 = (double)CONCAT44(fVar37,SUB84(param_1,0));
            bVar38 = 0.0 < fVar37;
          }
          else {
            param_1 = param_1 - in_dr0;
            bVar38 = in_dr2 < param_1;
          }
          if (bVar38) {
            puVar17 = puVar19 + uVar24;
            psVar20 = psVar20 + uVar24 * 2;
          }
          else {
            *_DAT_8c619c04 = *_DAT_8c619c04 + uVar24;
            iVar14 = 0;
            if (0 < (int)(uVar24 - 1)) {
              do {
                uVar2 = *puVar19;
                puVar19 = puVar19 + 1;
                puVar12 = (undefined8 *)((uint)uVar2 * 0x20 + *DAT_8c619c08);
                if (in_FPSCR_SZ == '\0') {
                  dVar32 = (double)CONCAT44(*(undefined4 *)DAT_8c619c0c,*(undefined4 *)puVar12);
                  in_dr0 = (double)CONCAT44((int)((ulonglong)in_dr0 >> 0x20),*DAT_8c619c14);
                }
                else {
                  in_xd2 = *puVar12;
                  dVar32 = *DAT_8c619c0c;
                }
                if (in_FPSCR_PR == '\0') {
                  dVar32 = (double)CONCAT44((float)((ulonglong)dVar32 >> 0x20) * SUB84(dVar32,0),
                                            SUB84(dVar32,0));
                }
                else {
                  dVar32 = dVar32 * dVar32;
                }
                if (in_FPSCR_SZ == '\0') {
                  dVar32 = (double)CONCAT44((int)((ulonglong)dVar32 >> 0x20),
                                            *(undefined4 *)DAT_8c619c10);
                }
                else {
                  in_xd2 = *DAT_8c619c10;
                }
                if (in_FPSCR_PR == '\0') {
                  fVar37 = SUB84(dVar32,0) - (float)((ulonglong)dVar32 >> 0x20);
                }
                else {
                  fVar37 = SUB84(dVar32 - dVar32,0);
                }
                if (in_FPSCR_SZ == '\0') {
                  dVar32 = (double)CONCAT44(*(undefined4 *)((int)puVar12 + 4),fVar37);
                }
                else {
                  dVar32 = *(double *)((int)puVar12 + 4);
                }
                if (in_FPSCR_PR == '\0') {
                  dVar34 = (double)CONCAT44((int)((ulonglong)in_dr0 >> 0x20),
                                            SUB84(in_dr0,0) * (float)((ulonglong)dVar32 >> 0x20));
                }
                else {
                  dVar34 = in_dr0 * dVar32;
                }
                if (in_FPSCR_SZ == '\0') {
                  dVar32 = (double)CONCAT44(*(undefined4 *)(puVar12 + 1),SUB84(dVar32,0));
                }
                else {
                  dVar32 = (double)puVar12[1];
                }
                if (in_FPSCR_PR == '\0') {
                  dVar32 = (double)CONCAT44((int)((ulonglong)dVar32 >> 0x20),
                                            SUB84(dVar32,0) - SUB84(dVar34,0));
                }
                else {
                  dVar32 = dVar32 - dVar34;
                }
                if (in_FPSCR_SZ == '\0') {
                  dVar34 = (double)CONCAT44((int)((ulonglong)dVar34 >> 0x20),
                                            *(undefined4 *)(DAT_8c619c0c + 1));
                }
                fVar37 = (float)((ulonglong)dVar32 >> 0x20);
                if (in_FPSCR_PR == '\0') {
                  fVar30 = SUB84(dVar34,0) * fVar37;
                  in_dr0 = (double)CONCAT44((int)((ulonglong)dVar34 >> 0x20),fVar30);
                  dVar32 = (double)CONCAT44(fVar37,SUB84(dVar32,0) - fVar30);
                }
                else {
                  in_dr0 = dVar34 * dVar32;
                  dVar32 = dVar32 - in_dr0;
                }
                if (in_FPSCR_SZ == '\0') {
                  puVar21 = (undefined8 *)(unaff_r8 + 6);
                  *(int *)puVar21 = SUB84(dVar32,0);
                }
                else {
                  puVar21 = (undefined8 *)(unaff_r8 + 5);
                  *puVar21 = in_xd2;
                }
                uVar36 = (undefined4)((ulonglong)dVar32 >> 0x20);
                if (in_FPSCR_PR == '\0') {
                  dVar32 = (double)CONCAT44(uVar36,(float)(int)psVar20[1] * in_fr15);
                }
                else {
                  dVar32 = (double)CONCAT44(uVar36,(float)(int)psVar20[1]) *
                           (double)CONCAT44(in_fr14,in_fr15);
                }
                if (in_FPSCR_SZ == '\0') {
                  puVar21 = (undefined8 *)((int)puVar21 + -4);
                  *(int *)puVar21 = SUB84(dVar32,0);
                }
                else {
                  puVar21 = puVar21 + -1;
                  *puVar21 = in_xd2;
                }
                uVar36 = DAT_8c619c18;
                sVar1 = *psVar20;
                psVar20 = psVar20 + 2;
                if (in_FPSCR_PR == '\0') {
                  fVar37 = (float)(int)sVar1 * in_fr15;
                }
                else {
                  fVar37 = SUB84((double)CONCAT44((int)((ulonglong)dVar32 >> 0x20),(float)(int)sVar1
                                                 ) * (double)CONCAT44(in_fr14,in_fr15),0);
                }
                if (in_FPSCR_SZ == '\0') {
                  *(float *)((int)puVar21 + -4) = fVar37;
                  *(undefined4 *)(puVar21 + -1) = *(undefined4 *)(puVar12 + 3);
                  *(undefined4 *)((int)puVar21 + -0xc) = *(undefined4 *)((int)puVar12 + 0x14);
                  puVar21 = puVar21 + -2;
                  *(undefined4 *)puVar21 = *(undefined4 *)(puVar12 + 2);
                }
                else {
                  puVar21[-1] = in_xd2;
                  puVar21[-2] = puVar12[3];
                  puVar21[-3] = *(undefined8 *)((int)puVar12 + 0x14);
                  in_xd2 = puVar12[2];
                  puVar21 = puVar21 + -4;
                  *puVar21 = in_xd2;
                }
                *(undefined4 *)((int)puVar21 + -4) = uVar36;
                iVar14 = iVar14 + 1;
                unaff_r8 = (undefined4 *)((int)puVar21 + 0x1c);
              } while (iVar14 < (int)(uVar24 - 1));
            }
            puVar17 = puVar19 + 1;
            puVar12 = (undefined8 *)((uint)*puVar19 * 0x20 + *DAT_8c619c08);
            if (in_FPSCR_SZ == '\0') {
              dVar32 = (double)CONCAT44(*(undefined4 *)DAT_8c619c0c,*(undefined4 *)puVar12);
              in_dr0 = (double)CONCAT44((int)((ulonglong)in_dr0 >> 0x20),*DAT_8c619c14);
            }
            else {
              in_xd2 = *puVar12;
              dVar32 = *DAT_8c619c0c;
            }
            if (in_FPSCR_PR == '\0') {
              dVar32 = (double)CONCAT44((float)((ulonglong)dVar32 >> 0x20) * SUB84(dVar32,0),
                                        SUB84(dVar32,0));
            }
            else {
              dVar32 = dVar32 * dVar32;
            }
            if (in_FPSCR_SZ == '\0') {
              dVar32 = (double)CONCAT44((int)((ulonglong)dVar32 >> 0x20),*(undefined4 *)DAT_8c619c10
                                       );
            }
            else {
              in_xd2 = *DAT_8c619c10;
            }
            if (in_FPSCR_PR == '\0') {
              fVar37 = SUB84(dVar32,0) - (float)((ulonglong)dVar32 >> 0x20);
            }
            else {
              fVar37 = SUB84(dVar32 - dVar32,0);
            }
            if (in_FPSCR_SZ == '\0') {
              dVar32 = (double)CONCAT44(*(undefined4 *)((int)puVar12 + 4),fVar37);
            }
            else {
              dVar32 = *(double *)((int)puVar12 + 4);
            }
            if (in_FPSCR_PR == '\0') {
              dVar34 = (double)CONCAT44((int)((ulonglong)in_dr0 >> 0x20),
                                        SUB84(in_dr0,0) * (float)((ulonglong)dVar32 >> 0x20));
            }
            else {
              dVar34 = in_dr0 * dVar32;
            }
            if (in_FPSCR_SZ == '\0') {
              dVar32 = (double)CONCAT44(*(undefined4 *)(puVar12 + 1),SUB84(dVar32,0));
            }
            else {
              dVar32 = (double)puVar12[1];
            }
            if (in_FPSCR_PR == '\0') {
              dVar32 = (double)CONCAT44((int)((ulonglong)dVar32 >> 0x20),
                                        SUB84(dVar32,0) - SUB84(dVar34,0));
            }
            else {
              dVar32 = dVar32 - dVar34;
            }
            if (in_FPSCR_SZ == '\0') {
              dVar34 = (double)CONCAT44((int)((ulonglong)dVar34 >> 0x20),
                                        *(undefined4 *)(DAT_8c619c0c + 1));
            }
            fVar37 = (float)((ulonglong)dVar32 >> 0x20);
            if (in_FPSCR_PR == '\0') {
              fVar30 = SUB84(dVar34,0) * fVar37;
              in_dr0 = (double)CONCAT44((int)((ulonglong)dVar34 >> 0x20),fVar30);
              dVar32 = (double)CONCAT44(fVar37,SUB84(dVar32,0) - fVar30);
            }
            else {
              in_dr0 = dVar34 * dVar32;
              dVar32 = dVar32 - in_dr0;
            }
            if (in_FPSCR_SZ == '\0') {
              puVar21 = (undefined8 *)(unaff_r8 + 6);
              *(int *)puVar21 = SUB84(dVar32,0);
            }
            else {
              puVar21 = (undefined8 *)(unaff_r8 + 5);
              *puVar21 = in_xd2;
            }
            uVar36 = (undefined4)((ulonglong)dVar32 >> 0x20);
            if (in_FPSCR_PR == '\0') {
              dVar32 = (double)CONCAT44(uVar36,(float)(int)psVar20[1] * in_fr15);
            }
            else {
              dVar32 = (double)CONCAT44(uVar36,(float)(int)psVar20[1]) *
                       (double)CONCAT44(in_fr14,in_fr15);
            }
            if (in_FPSCR_SZ == '\0') {
              puVar21 = (undefined8 *)((int)puVar21 + -4);
              *(int *)puVar21 = SUB84(dVar32,0);
            }
            else {
              puVar21 = puVar21 + -1;
              *puVar21 = in_xd2;
            }
            uVar36 = DAT_8c619c1c;
            sVar1 = *psVar20;
            psVar20 = psVar20 + 2;
            uVar33 = (undefined4)((ulonglong)dVar32 >> 0x20);
            if (in_FPSCR_PR == '\0') {
              in_dr2 = (double)CONCAT44(uVar33,(float)(int)sVar1 * in_fr15);
            }
            else {
              in_dr2 = (double)CONCAT44(uVar33,(float)(int)sVar1) *
                       (double)CONCAT44(in_fr14,in_fr15);
            }
            if (in_FPSCR_SZ == '\0') {
              *(int *)((int)puVar21 + -4) = SUB84(in_dr2,0);
              *(undefined4 *)(puVar21 + -1) = *(undefined4 *)(puVar12 + 3);
              *(undefined4 *)((int)puVar21 + -0xc) = *(undefined4 *)((int)puVar12 + 0x14);
              in_dr2 = (double)CONCAT44((int)((ulonglong)in_dr2 >> 0x20),
                                        *(undefined4 *)(puVar12 + 2));
              puVar21 = puVar21 + -2;
              *(undefined4 *)puVar21 = *(undefined4 *)(puVar12 + 2);
            }
            else {
              puVar21[-1] = in_xd2;
              puVar21[-2] = puVar12[3];
              puVar21[-3] = *(undefined8 *)((int)puVar12 + 0x14);
              in_xd2 = puVar12[2];
              puVar21 = puVar21 + -4;
              *puVar21 = in_xd2;
            }
            *(undefined4 *)((int)puVar21 + -4) = uVar36;
            unaff_r8 = (undefined4 *)((int)puVar21 + 0x1c);
          }
        }
        else {
          *DAT_8c619c20 = 0;
          uVar2 = *puVar19;
          iVar14 = *piVar10;
          uVar3 = puVar17[2];
          puVar17 = puVar17 + 3;
          uVar36 = (undefined4)((ulonglong)in_dr2 >> 0x20);
          if (in_FPSCR_PR == '\0') {
            uVar39 = (undefined2)(int)(float)(int)*psVar20;
          }
          else {
            uVar39 = (undefined2)(int)(double)CONCAT44(uVar36,(float)(int)*psVar20);
          }
          **(undefined2 **)(puVar29 + -0x10) = uVar39;
          if (in_FPSCR_PR == '\0') {
            uVar39 = (undefined2)(int)(float)(int)psVar20[1];
          }
          else {
            uVar39 = (undefined2)(int)(double)CONCAT44(uVar36,(float)(int)psVar20[1]);
          }
          *(undefined2 *)(*(int *)(puVar29 + -0x10) + 2) = uVar39;
          if (in_FPSCR_PR == '\0') {
            uVar39 = (undefined2)(int)(float)(int)psVar20[2];
          }
          else {
            uVar39 = (undefined2)(int)(double)CONCAT44(uVar36,(float)(int)psVar20[2]);
          }
          **(undefined2 **)(puVar29 + -0x1c) = uVar39;
          psVar4 = psVar20 + 3;
          psVar20 = psVar20 + 4;
          in_dr2 = (double)CONCAT44(uVar36,(float)(int)*psVar4);
          if (in_FPSCR_PR == '\0') {
            uVar39 = (undefined2)(int)(float)(int)*psVar4;
          }
          else {
            uVar39 = (undefined2)(int)in_dr2;
          }
          *(undefined2 *)(*(int *)(puVar29 + -0x1c) + 2) = uVar39;
          iVar22 = 0;
          iVar27 = (uint)uVar2 * 0x20 + iVar14;
          iVar14 = (uint)uVar3 * 0x20 + iVar14;
          if (0 < (int)(uVar24 - 2)) {
            do {
              uVar2 = *puVar17;
              puVar17 = puVar17 + 1;
              iVar25 = (uint)uVar2 * 0x20 + *DAT_8c619c08;
              uVar36 = (undefined4)((ulonglong)in_dr2 >> 0x20);
              if (in_FPSCR_PR == '\0') {
                uVar39 = (undefined2)(int)(float)(int)*psVar20;
              }
              else {
                uVar39 = (undefined2)(int)(double)CONCAT44(uVar36,(float)(int)*psVar20);
              }
              **(undefined2 **)(puVar29 + -0x18) = uVar39;
              in_dr2 = (double)CONCAT44(uVar36,(float)(int)psVar20[1]);
              if (in_FPSCR_PR == '\0') {
                uVar39 = (undefined2)(int)(float)(int)psVar20[1];
              }
              else {
                uVar39 = (undefined2)(int)in_dr2;
              }
              *(undefined2 *)(*(int *)(puVar29 + -0x18) + 2) = uVar39;
              psVar20 = psVar20 + 2;
              if ((*(uint *)(iVar27 + 0xc) & *(uint *)(iVar14 + 0xc) & *(uint *)(iVar25 + 0xc) & 1)
                  == 0) {
                in_dr0 = (double)(*(code *)PTR_FUN_8c619c28)(iVar14,iVar25,DAT_8c619c24);
              }
              iVar22 = iVar22 + 1;
              **(undefined4 **)(puVar29 + -0x10) = **(undefined4 **)(puVar29 + -0x1c);
              **(undefined4 **)(puVar29 + -0x1c) = **(undefined4 **)(puVar29 + -0x18);
              iVar27 = iVar14;
              iVar14 = iVar25;
            } while (iVar22 < (int)(uVar24 - 2));
          }
        }
      }
      else {
        psVar20 = psVar20 + uVar24 * 2;
        puVar17 = puVar19 + uVar24;
      }
      *(int *)(puVar29 + -0xc) = *(int *)(puVar29 + -0xc) + 1;
    }
  }
  else if (uVar11 == _DAT_8c619400) {
    *DAT_8c619c2c = *(undefined4 *)(puVar29 + -0x14);
    if (in_FPSCR_SZ == '\0') {
      *(int *)(puVar29 + -0x20) = (int)((ulonglong)in_dr0 >> 0x20);
      *(int *)(puVar29 + -0x24) = SUB84(in_dr0,0);
      *(int *)(puVar29 + -0x28) = (int)((ulonglong)in_dr2 >> 0x20);
      *(int *)(puVar29 + -0x2c) = SUB84(in_dr2,0);
      *(int *)(puVar29 + -0x30) = (int)((ulonglong)param_1 >> 0x20);
      *(int *)(puVar29 + -0x34) = SUB84(param_1,0);
      uVar35 = CONCAT44(DAT_8c619c00,SUB84(param_1,0));
    }
    else {
      *(double *)(puVar29 + -0x24) = in_dr0;
      *(undefined8 *)(puVar29 + -0x2c) = in_xd0;
      *(double *)(puVar29 + -0x34) = in_dr2;
      *(undefined8 *)(puVar29 + -0x3c) = in_xd2;
      *(double *)(puVar29 + -0x44) = param_1;
      *(double *)(puVar29 + -0x4c) = in_xd4;
      uVar35 = CONCAT44(_DAT_8c619c04,DAT_8c619c00);
    }
    (*(code *)PTR_FUN_8c619c30)(uVar35,*DAT_8c619c2c);
  }
  return;
}

