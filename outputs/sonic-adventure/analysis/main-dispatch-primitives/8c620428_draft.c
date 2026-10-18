/* Ghidra draft only; not buildable/verified source. */

void FUN_8c620428(double param_1,ushort *param_2)

{
  ushort uVar1;
  int *piVar2;
  uint uVar3;
  uint *puVar4;
  uint *puVar5;
  uint *puVar6;
  int iVar8;
  ushort *puVar9;
  int iVar10;
  uint uVar11;
  uint *unaff_r8;
  uint *puVar12;
  uint *puVar13;
  int iVar14;
  ushort *puVar15;
  ushort *puVar16;
  ushort *puVar17;
  int iVar18;
  uint uVar19;
  int iVar20;
  uint uVar21;
  double in_dr0;
  double dVar22;
  double dVar23;
  double dVar24;
  float fVar26;
  undefined8 in_xd2;
  double in_xd4;
  bool bVar27;
  char in_FPSCR_PR;
  char in_FPSCR_SZ;
  uint *puVar7;
  undefined4 uVar25;
  
  uVar3 = *param_2 & DAT_8c62048c;
  puVar15 = *(ushort **)(param_2 + 2);
  if (uVar3 == 0) {
    *DAT_8c620494 = *DAT_8c620494 + (uint)param_2[1] * 3;
    for (iVar14 = 0; puVar12 = DAT_8c620790, puVar4 = DAT_8c62078c, puVar13 = DAT_8c620788,
        iVar14 < (int)(uint)param_2[1]; iVar14 = iVar14 + 1) {
      uVar1 = *puVar15;
      puVar9 = puVar15 + 1;
      puVar17 = puVar15 + 2;
      iVar18 = *DAT_8c620784;
      puVar15 = puVar15 + 3;
      iVar8 = (uint)uVar1 * 0x20 + iVar18;
      iVar10 = (uint)*puVar9 * 0x20 + iVar18;
      iVar18 = (uint)*puVar17 * 0x20 + iVar18;
      if ((*(uint *)(iVar8 + 0xc) & *(uint *)(iVar10 + 0xc) & *(uint *)(iVar18 + 0xc) & 0x3f) == 0)
      {
        if (((*(uint *)(iVar8 + 0xc) | *(uint *)(iVar10 + 0xc) | *(uint *)(iVar18 + 0xc)) & 1) == 0)
        {
          if (in_FPSCR_SZ == '\0') {
            uVar25 = *(undefined4 *)(iVar10 + 0x10);
            dVar24 = (double)CONCAT44(uVar25,*(undefined4 *)(iVar8 + 0x10));
            dVar23 = (double)CONCAT44(uVar25,SUB84(param_1,0));
          }
          else {
            dVar24 = *(double *)(iVar10 + 0x10);
            uVar25 = (undefined4)((ulonglong)dVar24 >> 0x20);
            dVar23 = dVar24;
          }
          if (in_FPSCR_PR == '\0') {
            dVar24 = (double)CONCAT44((float)((ulonglong)dVar24 >> 0x20) - SUB84(dVar24,0),
                                      SUB84(dVar24,0));
          }
          else {
            dVar24 = dVar24 - dVar24;
          }
          if (in_FPSCR_SZ == '\0') {
            dVar24 = (double)CONCAT44((int)((ulonglong)dVar24 >> 0x20),
                                      *(undefined4 *)(iVar18 + 0x14));
            dVar23 = (double)CONCAT44(uVar25,*(undefined4 *)(iVar10 + 0x14));
          }
          fVar26 = SUB84(dVar23,0);
          if (in_FPSCR_PR == '\0') {
            dVar24 = (double)CONCAT44((int)((ulonglong)dVar24 >> 0x20),SUB84(dVar24,0) - fVar26);
          }
          else {
            dVar24 = dVar24 - dVar23;
          }
          if (in_FPSCR_SZ == '\0') {
            in_dr0 = (double)CONCAT44((int)((ulonglong)in_dr0 >> 0x20),fVar26);
          }
          if (in_FPSCR_PR == '\0') {
            dVar24 = (double)CONCAT44((float)((ulonglong)dVar24 >> 0x20) * SUB84(dVar24,0),
                                      SUB84(dVar24,0));
          }
          else {
            dVar24 = dVar24 * dVar24;
          }
          if (in_FPSCR_SZ == '\0') {
            dVar24 = (double)CONCAT44((int)((ulonglong)dVar24 >> 0x20),*(undefined4 *)(iVar8 + 0x14)
                                     );
          }
          if (in_FPSCR_PR == '\0') {
            dVar22 = (double)CONCAT44((int)((ulonglong)in_dr0 >> 0x20),
                                      SUB84(in_dr0,0) - SUB84(dVar24,0));
          }
          else {
            dVar22 = in_dr0 - dVar24;
          }
          if (in_FPSCR_SZ == '\0') {
            dVar24 = (double)CONCAT44((int)((ulonglong)dVar24 >> 0x20),
                                      *(undefined4 *)(iVar18 + 0x10));
          }
          if (in_FPSCR_PR == '\0') {
            dVar24 = (double)CONCAT44((int)((ulonglong)dVar24 >> 0x20),
                                      SUB84(dVar24,0) - (float)((ulonglong)dVar23 >> 0x20));
          }
          else {
            dVar24 = dVar24 - dVar23;
          }
          uVar3 = (uint)((ulonglong)dVar24 >> 0x20);
          param_1 = dVar24;
          if (in_FPSCR_SZ == '\0') {
            param_1 = (double)CONCAT44(uVar3,fVar26);
          }
          if (in_FPSCR_PR == '\0') {
            in_dr0 = (double)CONCAT44((int)((ulonglong)dVar22 >> 0x20),
                                      SUB84(dVar22,0) * SUB84(dVar24,0));
          }
          else {
            in_dr0 = dVar22 * dVar24;
          }
          if (in_FPSCR_PR == '\0') {
            fVar26 = (float)((ulonglong)param_1 >> 0x20) - SUB84(in_dr0,0);
            param_1 = (double)CONCAT44(fVar26,SUB84(param_1,0));
            bVar27 = 0.0 < fVar26;
          }
          else {
            param_1 = param_1 - in_dr0;
            bVar27 = (double)((ulonglong)uVar3 << 0x20) < param_1;
          }
          if (!bVar27) {
            puVar6 = DAT_8c620788 + -3;
            puVar7 = DAT_8c620788 + -3;
            if (in_FPSCR_SZ == '\0') {
              unaff_r8[7] = *DAT_8c620788;
              uVar3 = DAT_8c620794;
              unaff_r8[6] = *puVar4;
              puVar4 = puVar12 + 1;
              unaff_r8[5] = *puVar12;
              puVar13 = puVar13 + -2;
              unaff_r8[4] = *puVar6;
              unaff_r8[3] = *(uint *)(iVar8 + 0x18);
              unaff_r8[2] = *(uint *)(iVar8 + 0x14);
              puVar12 = unaff_r8 + 1;
              *puVar12 = *(uint *)(iVar8 + 0x10);
            }
            else {
              *(undefined8 *)(unaff_r8 + 6) = *(undefined8 *)DAT_8c620788;
              uVar3 = DAT_8c620794;
              *(undefined8 *)(unaff_r8 + 4) = *(undefined8 *)puVar4;
              puVar4 = puVar12 + 2;
              *(undefined8 *)(unaff_r8 + 2) = *(undefined8 *)puVar12;
              puVar13 = puVar13 + -1;
              *(undefined8 *)unaff_r8 = *(undefined8 *)puVar7;
              *(undefined8 *)(unaff_r8 + -2) = *(undefined8 *)(iVar8 + 0x18);
              *(undefined8 *)(unaff_r8 + -4) = *(undefined8 *)(iVar8 + 0x14);
              puVar12 = unaff_r8 + -6;
              *(undefined8 *)puVar12 = *(undefined8 *)(iVar8 + 0x10);
            }
            puVar12[-1] = uVar3;
            puVar5 = DAT_8c620788;
            puVar6 = DAT_8c620788 + -3;
            puVar7 = DAT_8c620788 + -3;
            if (in_FPSCR_SZ == '\0') {
              puVar12[0xe] = *DAT_8c620788;
              uVar3 = DAT_8c620794;
              puVar12[0xd] = *puVar4;
              puVar4 = puVar13 + 1;
              puVar12[0xc] = *puVar13;
              puVar5 = puVar5 + -2;
              puVar12[0xb] = *puVar7;
              puVar12[10] = *(uint *)(iVar10 + 0x18);
              puVar12[9] = *(uint *)(iVar10 + 0x14);
              puVar12 = puVar12 + 8;
              *puVar12 = *(uint *)(iVar10 + 0x10);
            }
            else {
              *(undefined8 *)(puVar12 + 0xd) = *(undefined8 *)DAT_8c620788;
              uVar3 = DAT_8c620794;
              *(undefined8 *)(puVar12 + 0xb) = *(undefined8 *)puVar4;
              puVar4 = puVar13 + 2;
              *(undefined8 *)(puVar12 + 9) = *(undefined8 *)puVar13;
              puVar5 = puVar5 + -1;
              *(undefined8 *)(puVar12 + 7) = *(undefined8 *)puVar6;
              *(undefined8 *)(puVar12 + 5) = *(undefined8 *)(iVar10 + 0x18);
              *(undefined8 *)(puVar12 + 3) = *(undefined8 *)(iVar10 + 0x14);
              puVar12 = puVar12 + 1;
              *(undefined8 *)puVar12 = *(undefined8 *)(iVar10 + 0x10);
            }
            puVar12[-1] = uVar3;
            puVar13 = DAT_8c620788 + -3;
            puVar6 = DAT_8c620788 + -3;
            if (in_FPSCR_SZ == '\0') {
              puVar12[0xe] = *DAT_8c620788;
              uVar3 = DAT_8c620798;
              puVar12[0xd] = *puVar4;
              puVar12[0xc] = *puVar5;
              puVar12[0xb] = *puVar6;
              puVar12[10] = *(uint *)(iVar18 + 0x18);
              puVar12[9] = *(uint *)(iVar18 + 0x14);
              puVar12 = puVar12 + 8;
              *puVar12 = *(uint *)(iVar18 + 0x10);
            }
            else {
              *(undefined8 *)(puVar12 + 0xd) = *(undefined8 *)DAT_8c620788;
              uVar3 = DAT_8c620798;
              *(undefined8 *)(puVar12 + 0xb) = *(undefined8 *)puVar4;
              *(undefined8 *)(puVar12 + 9) = *(undefined8 *)puVar5;
              *(undefined8 *)(puVar12 + 7) = *(undefined8 *)puVar13;
              *(undefined8 *)(puVar12 + 5) = *(undefined8 *)(iVar18 + 0x18);
              *(undefined8 *)(puVar12 + 3) = *(undefined8 *)(iVar18 + 0x14);
              puVar12 = puVar12 + 1;
              *(undefined8 *)puVar12 = *(undefined8 *)(iVar18 + 0x10);
            }
            puVar12[-1] = uVar3;
            unaff_r8 = puVar12 + 7;
            *DAT_8c62079c = *DAT_8c62079c + 3;
          }
        }
        else {
          *DAT_8c6207a0 = 1;
          in_dr0 = (double)(*(code *)PTR_FUN_8c6207a4)();
        }
      }
    }
  }
  else if (uVar3 == (int)DAT_8c62048a) {
    *DAT_8c6207a8 = *DAT_8c6207a8 + (uint)param_2[1] * 4;
    for (iVar14 = 0; puVar12 = DAT_8c620790, puVar4 = DAT_8c62078c, puVar13 = DAT_8c620788,
        iVar14 < (int)(uint)param_2[1]; iVar14 = iVar14 + 1) {
      uVar1 = *puVar15;
      puVar9 = puVar15 + 1;
      puVar17 = puVar15 + 2;
      iVar10 = *DAT_8c620784;
      puVar16 = puVar15 + 3;
      puVar15 = puVar15 + 4;
      iVar8 = (uint)uVar1 * 0x20 + iVar10;
      iVar18 = (uint)*puVar9 * 0x20 + iVar10;
      iVar20 = (uint)*puVar17 * 0x20 + iVar10;
      iVar10 = (uint)*puVar16 * 0x20 + iVar10;
      if ((*(uint *)(iVar8 + 0xc) & *(uint *)(iVar18 + 0xc) & *(uint *)(iVar20 + 0xc) &
           *(uint *)(iVar10 + 0xc) & 0x3f) == 0) {
        if (((*(uint *)(iVar8 + 0xc) | *(uint *)(iVar18 + 0xc) | *(uint *)(iVar20 + 0xc) |
             *(uint *)(iVar10 + 0xc)) & 1) == 0) {
          if (in_FPSCR_SZ == '\0') {
            uVar25 = *(undefined4 *)(iVar18 + 0x10);
            dVar24 = (double)CONCAT44(uVar25,*(undefined4 *)(iVar8 + 0x10));
            dVar23 = (double)CONCAT44(uVar25,SUB84(param_1,0));
          }
          else {
            dVar24 = *(double *)(iVar18 + 0x10);
            uVar25 = (undefined4)((ulonglong)dVar24 >> 0x20);
            dVar23 = dVar24;
          }
          if (in_FPSCR_PR == '\0') {
            dVar24 = (double)CONCAT44((float)((ulonglong)dVar24 >> 0x20) - SUB84(dVar24,0),
                                      SUB84(dVar24,0));
          }
          else {
            dVar24 = dVar24 - dVar24;
          }
          if (in_FPSCR_SZ == '\0') {
            dVar24 = (double)CONCAT44((int)((ulonglong)dVar24 >> 0x20),
                                      *(undefined4 *)(iVar20 + 0x14));
            dVar23 = (double)CONCAT44(uVar25,*(undefined4 *)(iVar18 + 0x14));
          }
          fVar26 = SUB84(dVar23,0);
          if (in_FPSCR_PR == '\0') {
            dVar24 = (double)CONCAT44((int)((ulonglong)dVar24 >> 0x20),SUB84(dVar24,0) - fVar26);
          }
          else {
            dVar24 = dVar24 - dVar23;
          }
          if (in_FPSCR_SZ == '\0') {
            in_dr0 = (double)CONCAT44((int)((ulonglong)in_dr0 >> 0x20),fVar26);
          }
          if (in_FPSCR_PR == '\0') {
            dVar24 = (double)CONCAT44((float)((ulonglong)dVar24 >> 0x20) * SUB84(dVar24,0),
                                      SUB84(dVar24,0));
          }
          else {
            dVar24 = dVar24 * dVar24;
          }
          if (in_FPSCR_SZ == '\0') {
            dVar24 = (double)CONCAT44((int)((ulonglong)dVar24 >> 0x20),*(undefined4 *)(iVar8 + 0x14)
                                     );
          }
          if (in_FPSCR_PR == '\0') {
            dVar22 = (double)CONCAT44((int)((ulonglong)in_dr0 >> 0x20),
                                      SUB84(in_dr0,0) - SUB84(dVar24,0));
          }
          else {
            dVar22 = in_dr0 - dVar24;
          }
          if (in_FPSCR_SZ == '\0') {
            dVar24 = (double)CONCAT44((int)((ulonglong)dVar24 >> 0x20),
                                      *(undefined4 *)(iVar20 + 0x10));
          }
          if (in_FPSCR_PR == '\0') {
            dVar24 = (double)CONCAT44((int)((ulonglong)dVar24 >> 0x20),
                                      SUB84(dVar24,0) - (float)((ulonglong)dVar23 >> 0x20));
          }
          else {
            dVar24 = dVar24 - dVar23;
          }
          uVar3 = (uint)((ulonglong)dVar24 >> 0x20);
          param_1 = dVar24;
          if (in_FPSCR_SZ == '\0') {
            param_1 = (double)CONCAT44(uVar3,fVar26);
          }
          if (in_FPSCR_PR == '\0') {
            in_dr0 = (double)CONCAT44((int)((ulonglong)dVar22 >> 0x20),
                                      SUB84(dVar22,0) * SUB84(dVar24,0));
          }
          else {
            in_dr0 = dVar22 * dVar24;
          }
          if (in_FPSCR_PR == '\0') {
            fVar26 = (float)((ulonglong)param_1 >> 0x20) - SUB84(in_dr0,0);
            param_1 = (double)CONCAT44(fVar26,SUB84(param_1,0));
            bVar27 = 0.0 < fVar26;
          }
          else {
            param_1 = param_1 - in_dr0;
            bVar27 = (double)((ulonglong)uVar3 << 0x20) < param_1;
          }
          if (!bVar27) {
            puVar6 = DAT_8c620788 + -3;
            puVar7 = DAT_8c620788 + -3;
            if (in_FPSCR_SZ == '\0') {
              unaff_r8[7] = *DAT_8c620788;
              uVar3 = DAT_8c620794;
              unaff_r8[6] = *puVar4;
              puVar4 = puVar12 + 1;
              unaff_r8[5] = *puVar12;
              puVar13 = puVar13 + -2;
              unaff_r8[4] = *puVar6;
              unaff_r8[3] = *(uint *)(iVar8 + 0x18);
              unaff_r8[2] = *(uint *)(iVar8 + 0x14);
              puVar12 = unaff_r8 + 1;
              *puVar12 = *(uint *)(iVar8 + 0x10);
            }
            else {
              *(undefined8 *)(unaff_r8 + 6) = *(undefined8 *)DAT_8c620788;
              uVar3 = DAT_8c620794;
              *(undefined8 *)(unaff_r8 + 4) = *(undefined8 *)puVar4;
              puVar4 = puVar12 + 2;
              *(undefined8 *)(unaff_r8 + 2) = *(undefined8 *)puVar12;
              puVar13 = puVar13 + -1;
              *(undefined8 *)unaff_r8 = *(undefined8 *)puVar7;
              *(undefined8 *)(unaff_r8 + -2) = *(undefined8 *)(iVar8 + 0x18);
              *(undefined8 *)(unaff_r8 + -4) = *(undefined8 *)(iVar8 + 0x14);
              puVar12 = unaff_r8 + -6;
              *(undefined8 *)puVar12 = *(undefined8 *)(iVar8 + 0x10);
            }
            puVar12[-1] = uVar3;
            puVar5 = DAT_8c620788;
            puVar7 = DAT_8c620788 + -3;
            puVar6 = DAT_8c620788 + -3;
            if (in_FPSCR_SZ == '\0') {
              puVar12[0xe] = *DAT_8c620788;
              uVar3 = DAT_8c620794;
              puVar12[0xd] = *puVar4;
              puVar4 = puVar13 + 1;
              puVar12[0xc] = *puVar13;
              puVar5 = puVar5 + -2;
              puVar12[0xb] = *puVar6;
              puVar12[10] = *(uint *)(iVar18 + 0x18);
              puVar12[9] = *(uint *)(iVar18 + 0x14);
              puVar12 = puVar12 + 8;
              *puVar12 = *(uint *)(iVar18 + 0x10);
            }
            else {
              *(undefined8 *)(puVar12 + 0xd) = *(undefined8 *)DAT_8c620788;
              uVar3 = DAT_8c620794;
              *(undefined8 *)(puVar12 + 0xb) = *(undefined8 *)puVar4;
              puVar4 = puVar13 + 2;
              *(undefined8 *)(puVar12 + 9) = *(undefined8 *)puVar13;
              puVar5 = puVar5 + -1;
              *(undefined8 *)(puVar12 + 7) = *(undefined8 *)puVar7;
              *(undefined8 *)(puVar12 + 5) = *(undefined8 *)(iVar18 + 0x18);
              *(undefined8 *)(puVar12 + 3) = *(undefined8 *)(iVar18 + 0x14);
              puVar12 = puVar12 + 1;
              *(undefined8 *)puVar12 = *(undefined8 *)(iVar18 + 0x10);
            }
            puVar12[-1] = uVar3;
            puVar7 = DAT_8c620788;
            puVar6 = DAT_8c620788 + -3;
            puVar13 = DAT_8c620788 + -3;
            if (in_FPSCR_SZ == '\0') {
              puVar12[0xe] = *DAT_8c620788;
              uVar3 = DAT_8c620794;
              puVar12[0xd] = *puVar4;
              puVar4 = puVar5 + 1;
              puVar12[0xc] = *puVar5;
              puVar7 = puVar7 + -2;
              puVar12[0xb] = *puVar13;
              puVar12[10] = *(uint *)(iVar20 + 0x18);
              puVar12[9] = *(uint *)(iVar20 + 0x14);
              puVar12 = puVar12 + 8;
              *puVar12 = *(uint *)(iVar20 + 0x10);
            }
            else {
              *(undefined8 *)(puVar12 + 0xd) = *(undefined8 *)DAT_8c620788;
              uVar3 = DAT_8c620794;
              *(undefined8 *)(puVar12 + 0xb) = *(undefined8 *)puVar4;
              puVar4 = puVar5 + 2;
              *(undefined8 *)(puVar12 + 9) = *(undefined8 *)puVar5;
              puVar7 = puVar7 + -1;
              *(undefined8 *)(puVar12 + 7) = *(undefined8 *)puVar6;
              *(undefined8 *)(puVar12 + 5) = *(undefined8 *)(iVar20 + 0x18);
              *(undefined8 *)(puVar12 + 3) = *(undefined8 *)(iVar20 + 0x14);
              puVar12 = puVar12 + 1;
              *(undefined8 *)puVar12 = *(undefined8 *)(iVar20 + 0x10);
            }
            puVar12[-1] = uVar3;
            puVar13 = DAT_8c620788 + -3;
            puVar6 = DAT_8c620788 + -3;
            if (in_FPSCR_SZ == '\0') {
              puVar12[0xe] = *DAT_8c620788;
              uVar3 = DAT_8c620798;
              puVar12[0xd] = *puVar4;
              puVar12[0xc] = *puVar7;
              puVar12[0xb] = *puVar6;
              puVar12[10] = *(uint *)(iVar10 + 0x18);
              puVar12[9] = *(uint *)(iVar10 + 0x14);
              puVar12 = puVar12 + 8;
              *puVar12 = *(uint *)(iVar10 + 0x10);
            }
            else {
              *(undefined8 *)(puVar12 + 0xd) = *(undefined8 *)DAT_8c620788;
              uVar3 = DAT_8c620798;
              *(undefined8 *)(puVar12 + 0xb) = *(undefined8 *)puVar4;
              *(undefined8 *)(puVar12 + 9) = *(undefined8 *)puVar7;
              *(undefined8 *)(puVar12 + 7) = *(undefined8 *)puVar13;
              *(undefined8 *)(puVar12 + 5) = *(undefined8 *)(iVar10 + 0x18);
              *(undefined8 *)(puVar12 + 3) = *(undefined8 *)(iVar10 + 0x14);
              puVar12 = puVar12 + 1;
              *(undefined8 *)puVar12 = *(undefined8 *)(iVar10 + 0x10);
            }
            puVar12[-1] = uVar3;
            unaff_r8 = puVar12 + 7;
            *DAT_8c62079c = *DAT_8c62079c + 4;
          }
        }
        else {
          *DAT_8c6207a0 = 1;
          if ((*(uint *)(iVar8 + 0xc) & *(uint *)(iVar18 + 0xc) & *(uint *)(iVar20 + 0xc) & 1) == 0)
          {
            in_dr0 = (double)(*(code *)PTR_FUN_8c6207a4)(iVar18,iVar20);
          }
          if ((*(uint *)(iVar18 + 0xc) & *(uint *)(iVar20 + 0xc) & *(uint *)(iVar10 + 0xc) & 1) == 0
             ) {
            in_dr0 = (double)(*(code *)PTR_FUN_8c6207a4)(iVar18,iVar20,iVar10);
          }
        }
      }
    }
  }
  else if (uVar3 == DAT_8c620490) {
    for (iVar14 = 0; uVar25 = SUB84(param_1,0), iVar14 < (int)(uint)param_2[1]; iVar14 = iVar14 + 1)
    {
      uVar11 = 0;
      puVar17 = puVar15 + 1;
      iVar8 = 0;
      uVar19 = (uint)*puVar15;
      *DAT_8c6209ec = *DAT_8c6209ec + uVar19;
      piVar2 = DAT_8c6209f0;
      uVar3 = 0xffffffff;
      puVar9 = puVar17;
      if (uVar19 != 0) {
        do {
          uVar1 = *puVar9;
          puVar9 = puVar9 + 1;
          iVar8 = iVar8 + 1;
          uVar21 = *(uint *)((uint)uVar1 * 0x20 + *DAT_8c6209f0 + 0xc);
          uVar3 = uVar3 & uVar21;
          uVar11 = uVar11 | uVar21;
        } while (iVar8 < (int)uVar19);
      }
      if ((uVar3 & 0x3f) == 0) {
        if ((uVar11 & 1) == 0) {
          iVar10 = *DAT_8c6209f0;
          iVar8 = (uint)puVar15[2] * 0x20 + iVar10;
          if (in_FPSCR_SZ == '\0') {
            uVar25 = *(undefined4 *)(iVar8 + 0x10);
          }
          else {
            in_xd4 = *(double *)(iVar8 + 0x10);
          }
          iVar18 = (uint)*puVar17 * 0x20 + iVar10;
          if (in_FPSCR_SZ == '\0') {
            dVar23 = (double)CONCAT44(uVar25,*(undefined4 *)(iVar18 + 0x10));
          }
          else {
            in_xd2 = *(undefined8 *)(iVar18 + 0x10);
            dVar23 = in_xd4;
          }
          if (in_FPSCR_PR == '\0') {
            dVar23 = (double)CONCAT44((float)((ulonglong)dVar23 >> 0x20) - SUB84(dVar23,0),
                                      SUB84(dVar23,0));
          }
          else {
            dVar23 = dVar23 - dVar23;
          }
          iVar10 = (uint)puVar15[3] * 0x20 + iVar10;
          if (in_FPSCR_SZ == '\0') {
            fVar26 = *(float *)(iVar8 + 0x14);
            dVar24 = (double)CONCAT44(fVar26,uVar25);
            dVar23 = (double)CONCAT44((int)((ulonglong)dVar23 >> 0x20),
                                      *(undefined4 *)(iVar10 + 0x14));
            in_dr0 = (double)CONCAT44((int)((ulonglong)in_dr0 >> 0x20),fVar26);
          }
          else {
            dVar24 = *(double *)(iVar8 + 0x14);
            in_xd2 = *(undefined8 *)(iVar10 + 0x14);
            fVar26 = (float)((ulonglong)dVar24 >> 0x20);
          }
          if (in_FPSCR_PR == '\0') {
            fVar26 = SUB84(dVar23,0) - fVar26;
            param_1 = (double)CONCAT44((float)((ulonglong)dVar23 >> 0x20) * fVar26,fVar26);
          }
          else {
            param_1 = (dVar23 - dVar24) * (dVar23 - dVar24);
          }
          if (in_FPSCR_SZ == '\0') {
            param_1 = (double)CONCAT44((int)((ulonglong)param_1 >> 0x20),
                                       *(undefined4 *)(iVar18 + 0x14));
          }
          else {
            in_xd2 = *(undefined8 *)(iVar18 + 0x14);
          }
          if (in_FPSCR_PR == '\0') {
            dVar23 = (double)CONCAT44((int)((ulonglong)in_dr0 >> 0x20),
                                      SUB84(in_dr0,0) - SUB84(param_1,0));
          }
          else {
            dVar23 = in_dr0 - param_1;
          }
          uVar25 = (undefined4)((ulonglong)param_1 >> 0x20);
          if (in_FPSCR_SZ == '\0') {
            param_1 = (double)CONCAT44(uVar25,SUB84(dVar24,0));
            dVar24 = (double)CONCAT44(uVar25,*(undefined4 *)(iVar10 + 0x10));
          }
          else {
            in_xd2 = *(undefined8 *)(iVar10 + 0x10);
            dVar24 = param_1;
          }
          if (in_FPSCR_PR == '\0') {
            fVar26 = SUB84(dVar24,0) - SUB84(param_1,0);
            dVar24 = (double)CONCAT44(uVar25,fVar26);
            in_dr0 = (double)CONCAT44((int)((ulonglong)dVar23 >> 0x20),SUB84(dVar23,0) * fVar26);
          }
          else {
            dVar24 = dVar24 - param_1;
            in_dr0 = dVar23 * dVar24;
          }
          dVar24 = (double)((ulonglong)dVar24 & 0xffffffff00000000);
          if (in_FPSCR_PR == '\0') {
            fVar26 = (float)((ulonglong)param_1 >> 0x20) - SUB84(in_dr0,0);
            param_1 = (double)CONCAT44(fVar26,SUB84(param_1,0));
            bVar27 = 0.0 < fVar26;
          }
          else {
            param_1 = param_1 - in_dr0;
            bVar27 = dVar24 < param_1;
          }
          if (bVar27) goto LAB_8c620844;
          *DAT_8c6209f4 = *DAT_8c6209f4 + uVar19;
          iVar8 = 0;
          if (0 < (int)(uVar19 - 1)) {
            do {
              puVar4 = DAT_8c620a00;
              puVar13 = DAT_8c6209fc;
              uVar1 = *puVar17;
              puVar17 = puVar17 + 1;
              if (in_FPSCR_SZ == '\0') {
                dVar24 = (double)(ulonglong)*DAT_8c6209f8;
              }
              else {
                in_xd2 = *(undefined8 *)DAT_8c6209f8;
              }
              iVar10 = (uint)uVar1 * 0x20 + *DAT_8c6209f0;
              puVar12 = DAT_8c6209f8 + -3;
              puVar6 = DAT_8c6209f8 + -3;
              if (in_FPSCR_SZ == '\0') {
                unaff_r8[7] = SUB84(dVar24,0);
                uVar3 = DAT_8c620a04;
                unaff_r8[6] = *puVar13;
                unaff_r8[5] = *puVar4;
                unaff_r8[4] = *puVar6;
                unaff_r8[3] = *(uint *)(iVar10 + 0x18);
                unaff_r8[2] = *(uint *)(iVar10 + 0x14);
                dVar24 = (double)(ulonglong)*(uint *)(iVar10 + 0x10);
                puVar13 = unaff_r8 + 1;
                *puVar13 = *(uint *)(iVar10 + 0x10);
              }
              else {
                *(undefined8 *)(unaff_r8 + 6) = in_xd2;
                uVar3 = DAT_8c620a04;
                *(undefined8 *)(unaff_r8 + 4) = *(undefined8 *)puVar13;
                *(undefined8 *)(unaff_r8 + 2) = *(undefined8 *)puVar4;
                *(undefined8 *)unaff_r8 = *(undefined8 *)puVar12;
                *(undefined8 *)(unaff_r8 + -2) = *(undefined8 *)(iVar10 + 0x18);
                *(undefined8 *)(unaff_r8 + -4) = *(undefined8 *)(iVar10 + 0x14);
                in_xd2 = *(undefined8 *)(iVar10 + 0x10);
                puVar13 = unaff_r8 + -6;
                *(undefined8 *)puVar13 = in_xd2;
              }
              puVar13[-1] = uVar3;
              iVar8 = iVar8 + 1;
              unaff_r8 = puVar13 + 7;
            } while (iVar8 < (int)(uVar19 - 1));
          }
          puVar4 = DAT_8c620a00;
          puVar13 = DAT_8c6209fc;
          uVar3 = SUB84(dVar24,0);
          puVar15 = puVar17 + 1;
          if (in_FPSCR_SZ == '\0') {
            uVar3 = *DAT_8c6209f8;
          }
          else {
            in_xd2 = *(undefined8 *)DAT_8c6209f8;
          }
          iVar8 = (uint)*puVar17 * 0x20 + *DAT_8c6209f0;
          puVar12 = DAT_8c6209f8 + -3;
          puVar6 = DAT_8c6209f8 + -3;
          if (in_FPSCR_SZ == '\0') {
            unaff_r8[7] = uVar3;
            uVar3 = DAT_8c620a08;
            unaff_r8[6] = *puVar13;
            unaff_r8[5] = *puVar4;
            unaff_r8[4] = *puVar6;
            unaff_r8[3] = *(uint *)(iVar8 + 0x18);
            unaff_r8[2] = *(uint *)(iVar8 + 0x14);
            puVar13 = unaff_r8 + 1;
            *puVar13 = *(uint *)(iVar8 + 0x10);
          }
          else {
            *(undefined8 *)(unaff_r8 + 6) = in_xd2;
            uVar3 = DAT_8c620a08;
            *(undefined8 *)(unaff_r8 + 4) = *(undefined8 *)puVar13;
            *(undefined8 *)(unaff_r8 + 2) = *(undefined8 *)puVar4;
            *(undefined8 *)unaff_r8 = *(undefined8 *)puVar12;
            *(undefined8 *)(unaff_r8 + -2) = *(undefined8 *)(iVar8 + 0x18);
            *(undefined8 *)(unaff_r8 + -4) = *(undefined8 *)(iVar8 + 0x14);
            in_xd2 = *(undefined8 *)(iVar8 + 0x10);
            puVar13 = unaff_r8 + -6;
            *(undefined8 *)puVar13 = in_xd2;
          }
          puVar13[-1] = uVar3;
          unaff_r8 = puVar13 + 7;
        }
        else {
          *DAT_8c620a0c = 0;
          puVar9 = puVar15 + 2;
          puVar15 = puVar15 + 3;
          iVar18 = 0;
          iVar8 = (uint)*puVar17 * 0x20 + *piVar2;
          iVar10 = (uint)*puVar9 * 0x20 + *piVar2;
          if (0 < (int)(uVar19 - 2)) {
            do {
              uVar1 = *puVar15;
              puVar15 = puVar15 + 1;
              iVar20 = (uint)uVar1 * 0x20 + *DAT_8c6209f0;
              if ((*(uint *)(iVar8 + 0xc) & *(uint *)(iVar10 + 0xc) & *(uint *)(iVar20 + 0xc) & 1)
                  == 0) {
                in_dr0 = (double)(*(code *)PTR_FUN_8c620a10)(iVar10,iVar20);
              }
              iVar18 = iVar18 + 1;
              iVar8 = iVar10;
              iVar10 = iVar20;
            } while (iVar18 < (int)(uVar19 - 2));
          }
        }
      }
      else {
LAB_8c620844:
        puVar15 = puVar17 + uVar19;
      }
    }
  }
  else if (uVar3 == DAT_8c62048c) {
    *DAT_8c620a14 = param_2;
    (*(code *)PTR_FUN_8c620a18)(*DAT_8c620a14);
  }
  return;
}

