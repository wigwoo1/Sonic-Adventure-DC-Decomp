/* Ghidra draft only; not buildable/verified source. */

/* WARNING: Removing unreachable block (ram,0x8c614b36) */

void FUN_8c6149b4(double param_1,undefined4 param_2)

{
  ushort uVar1;
  undefined4 uVar2;
  int iVar3;
  int iVar4;
  uint unaff_r8;
  double *pdVar5;
  uint uVar6;
  int iVar7;
  int iVar8;
  int iVar9;
  ushort *puVar10;
  ushort *puVar11;
  ushort *puVar12;
  undefined1 *puVar13;
  undefined4 uVar14;
  double dVar15;
  float fVar16;
  double dVar17;
  float fVar18;
  uint uVar19;
  double dVar20;
  undefined4 uVar22;
  undefined4 uVar23;
  undefined4 uVar24;
  undefined4 uVar25;
  undefined4 uVar26;
  float in_fr13;
  undefined4 in_fr12;
  double in_dr14;
  float fVar28;
  undefined8 uVar27;
  double in_xd0;
  double in_xd2;
  double in_xd4;
  double in_xd12;
  double dVar29;
  undefined8 in_xd14;
  bool bVar30;
  undefined4 in_PR;
  char in_FPSCR_PR;
  char in_FPSCR_SZ;
  undefined1 local_30 [12];
  undefined1 local_24 [4];
  undefined8 uStack_20;
  float fVar21;
  
  uVar2 = DAT_8c6149f8;
  if (in_FPSCR_SZ == '\0') {
    uStack_20 = CONCAT44(SUB84(in_dr14,0),(undefined4)uStack_20);
                    /* WARNING: Ignoring partial resolution of indirect */
    uStack_20._0_4_ = (int)((ulonglong)in_dr14 >> 0x20);
    puVar13 = local_24;
  }
  else {
    puVar13 = local_30;
    uStack_20 = in_xd14;
  }
  *(undefined4 *)(puVar13 + -4) = in_PR;
  *(undefined4 *)(puVar13 + -0x10) = param_2;
  dVar15 = (double)(*(code *)PTR_FUN_8c6149fc)();
  puVar10 = *(ushort **)(*(int *)(puVar13 + -0x10) + 4);
  *(uint *)(puVar13 + -8) = (uint)*(ushort *)(*(int *)(puVar13 + -0x10) + 2);
  iVar9 = *DAT_8c614a00;
  *(undefined4 *)(puVar13 + -0xc) = 0;
  *(undefined4 *)(puVar13 + -0x10) = 0;
  do {
    uVar22 = SUB84(in_dr14,0);
    uVar1 = *puVar10;
    puVar11 = puVar10 + 1;
    *(uint *)(puVar13 + -0x14) = (uint)uVar1;
    if (in_FPSCR_SZ == '\0') {
      uVar22 = 0x3f800000;
    }
    if ((uVar1 & DAT_8c614ad0) != 0) {
      if (in_FPSCR_PR == '\0') {
        uVar14 = 0xbf800000;
      }
      else {
        uVar14 = (undefined4)((ulonglong)-(double)CONCAT44(0x3f800000,SUB84(param_1,0)) >> 0x20);
      }
      *(uint *)(puVar13 + -0x14) = *(uint *)(puVar13 + -0x14) & (int)DAT_8c614acc;
      if (in_FPSCR_SZ == '\0') {
        uVar22 = uVar14;
      }
    }
    *(int *)(puVar13 + -0x10) = *(int *)(puVar13 + -0x10) + *(int *)(puVar13 + -0x14);
    *(int *)(puVar13 + -0x14) = *(int *)(puVar13 + -0x14) + -2;
    puVar12 = puVar10 + 2;
    puVar10 = puVar10 + 3;
    iVar7 = (uint)*puVar12 * 0x20 + iVar9;
    iVar4 = (uint)*puVar11 * 0x20 + iVar9;
    uVar6 = unaff_r8;
    if (in_FPSCR_SZ == '\0') {
      in_dr14 = (double)CONCAT44(*(undefined4 *)(iVar7 + 0x10),uVar22);
      in_fr13 = *(float *)(iVar7 + 0x14);
      param_1 = (double)CONCAT44(*(undefined4 *)(iVar4 + 0x10),*(undefined4 *)(iVar4 + 0x14));
      dVar29 = in_xd12;
    }
    else {
      in_dr14 = *(double *)(iVar7 + 0x10);
      param_1 = *(double *)(iVar4 + 0x10);
      in_xd4 = *(double *)(iVar4 + 0x14);
      dVar29 = *(double *)(iVar7 + 0x14);
    }
    do {
      uVar22 = DAT_8c614be8;
      uVar1 = *puVar10;
      puVar10 = puVar10 + 1;
      iVar8 = (uint)uVar1 * 0x20 + iVar9;
      fVar28 = (float)((ulonglong)in_dr14 >> 0x20);
      if (in_FPSCR_SZ == '\0') {
        dVar17 = (double)CONCAT44(*(undefined4 *)(iVar8 + 0x14),fVar28);
      }
      else {
        dVar17 = *(double *)(iVar8 + 0x14);
        in_xd2 = in_dr14;
      }
      if (in_FPSCR_PR == '\0') {
        dVar17 = (double)CONCAT44((int)((ulonglong)dVar17 >> 0x20),
                                  SUB84(dVar17,0) - (float)((ulonglong)param_1 >> 0x20));
      }
      else {
        dVar17 = dVar17 - param_1;
      }
      if (in_FPSCR_PR == '\0') {
        dVar17 = (double)CONCAT44((float)((ulonglong)dVar17 >> 0x20) - in_fr13,SUB84(dVar17,0));
      }
      else {
        dVar17 = dVar17 - (double)CONCAT44(in_fr12,in_fr13);
      }
      if (in_FPSCR_SZ == '\0') {
        dVar15 = (double)CONCAT44((int)((ulonglong)dVar15 >> 0x20),*(undefined4 *)(iVar8 + 0x10));
      }
      else {
        in_xd0 = *(double *)(iVar8 + 0x10);
      }
      if (in_FPSCR_PR == '\0') {
        dVar15 = (double)CONCAT44((int)((ulonglong)dVar15 >> 0x20),SUB84(dVar15,0) - fVar28);
        fVar16 = SUB84(dVar17,0) * (float)((ulonglong)dVar17 >> 0x20);
      }
      else {
        dVar15 = dVar15 - in_dr14;
        fVar16 = SUB84(dVar17 * dVar17,0);
      }
      dVar17 = dVar29;
      if (in_FPSCR_SZ == '\0') {
        dVar17 = (double)CONCAT44(in_fr13,fVar16);
      }
      fVar16 = SUB84(dVar15,0);
      if (in_FPSCR_PR == '\0') {
        fVar18 = ((float)((ulonglong)dVar17 >> 0x20) - SUB84(param_1,0)) * fVar16;
        dVar17 = (double)CONCAT44(fVar18,SUB84(dVar17,0) - fVar18);
      }
      else {
        dVar17 = (dVar17 - param_1) * dVar15;
        dVar17 = dVar17 - dVar17;
      }
      dVar20 = in_xd2;
      if (in_FPSCR_SZ == '\0') {
        dVar20 = (double)CONCAT44(SUB84(dVar17,0),SUB84(param_1,0));
      }
      fVar18 = SUB84(in_dr14,0);
      if (in_FPSCR_PR == '\0') {
        dVar20 = (double)CONCAT44((float)((ulonglong)dVar20 >> 0x20) * fVar18,SUB84(dVar20,0));
      }
      else {
        dVar20 = dVar20 * in_dr14;
      }
      uVar19 = (uint)((ulonglong)dVar17 >> 0x20);
      uVar14 = SUB84(dVar20,0);
      fVar21 = (float)((ulonglong)dVar20 >> 0x20);
      if (uVar6 < unaff_r8) {
        dVar17 = (double)((ulonglong)uVar19 << 0x20);
        if (in_FPSCR_PR == '\0') {
          bVar30 = fVar21 < 0.0;
        }
        else {
          bVar30 = dVar20 < dVar17;
        }
        if (bVar30) {
LAB_8c614afc:
          uVar22 = DAT_8c614be8;
          if (in_FPSCR_SZ == '\0') {
            uVar14 = SUB84(dVar15,0);
            uVar24 = SUB84(dVar17,0);
            uVar26 = SUB84(dVar20,0);
            uVar23 = *(undefined4 *)(iVar7 + 0x10);
            dVar20 = (double)CONCAT44(uVar23,uVar26);
            uVar25 = *(undefined4 *)(iVar7 + 0x14);
            *(undefined4 *)(unaff_r8 + 0x1c) = uVar24;
            *(undefined4 *)(unaff_r8 + 0x18) = uVar14;
            *(undefined4 *)(unaff_r8 + 0x14) = uVar25;
            *(undefined4 *)(unaff_r8 + 0x10) = uVar26;
            pdVar5 = (double *)(unaff_r8 + 0xc);
            *(undefined4 *)pdVar5 = uVar23;
            dVar17 = (double)CONCAT44(uVar24,uVar24);
            dVar15 = (double)CONCAT44(uVar14,uVar14);
          }
          else {
            dVar20 = *(double *)(iVar7 + 0x10);
            dVar15 = *(double *)(iVar7 + 0x18);
            *(double *)(unaff_r8 + 0x18) = in_xd2;
            pdVar5 = (double *)(unaff_r8 + 0x10);
            *pdVar5 = in_xd0;
            pdVar5[-1] = dVar15;
            pdVar5[-2] = in_xd4;
            pdVar5 = pdVar5 + -3;
            *pdVar5 = dVar20;
            dVar17 = in_xd2;
            dVar15 = in_xd0;
          }
          *(undefined4 *)((int)pdVar5 + -4) = uVar22;
          unaff_r8 = (int)pdVar5 + 0x1c;
        }
        else {
          if (in_FPSCR_SZ == '\0') {
            uVar22 = *(undefined4 *)(iVar7 + 0x10);
            dVar20 = (double)CONCAT44(uVar22,uVar14);
            uVar24 = *(undefined4 *)(iVar7 + 0x14);
            *(undefined4 *)(unaff_r8 + 0x1c) = 0;
            *(float *)(unaff_r8 + 0x18) = fVar16;
            *(undefined4 *)(unaff_r8 + 0x14) = uVar24;
            *(undefined4 *)(unaff_r8 + 0x10) = uVar14;
            pdVar5 = (double *)(unaff_r8 + 0xc);
            *(undefined4 *)pdVar5 = uVar22;
            dVar17 = 0.0;
            dVar15 = (double)CONCAT44(fVar16,fVar16);
          }
          else {
            dVar20 = *(double *)(iVar7 + 0x10);
            dVar15 = *(double *)(iVar7 + 0x18);
            *(double *)(unaff_r8 + 0x18) = in_xd2;
            pdVar5 = (double *)(unaff_r8 + 0x10);
            *pdVar5 = in_xd0;
            pdVar5[-1] = dVar15;
            pdVar5[-2] = in_xd4;
            pdVar5 = pdVar5 + -3;
            *pdVar5 = dVar20;
            dVar17 = in_xd2;
            dVar15 = in_xd0;
          }
          *(undefined4 *)((int)pdVar5 + -4) = uVar2;
          unaff_r8 = (int)pdVar5 + 0x1c;
          *(uint *)(puVar13 + -0xc) = *(int *)(puVar13 + -0xc) + (unaff_r8 - uVar6) + 0x20;
          uVar6 = unaff_r8;
        }
      }
      else {
        dVar17 = (double)((ulonglong)uVar19 << 0x20);
        if (in_FPSCR_PR == '\0') {
          bVar30 = fVar21 < 0.0;
        }
        else {
          bVar30 = dVar20 < dVar17;
        }
        if (bVar30) {
          if (in_FPSCR_SZ == '\0') {
            uVar24 = *(undefined4 *)(iVar4 + 0x10);
            dVar20 = (double)CONCAT44(uVar24,uVar14);
            uVar26 = *(undefined4 *)(iVar4 + 0x14);
            *(undefined4 *)(unaff_r8 + 0x1c) = 0;
            *(float *)(unaff_r8 + 0x18) = fVar16;
            *(undefined4 *)(unaff_r8 + 0x14) = uVar26;
            *(undefined4 *)(unaff_r8 + 0x10) = uVar14;
            pdVar5 = (double *)(unaff_r8 + 0xc);
            *(undefined4 *)pdVar5 = uVar24;
            dVar17 = 0.0;
            dVar15 = (double)CONCAT44(fVar16,fVar16);
          }
          else {
            dVar20 = *(double *)(iVar4 + 0x10);
            dVar15 = *(double *)(iVar4 + 0x18);
            *(double *)(unaff_r8 + 0x18) = in_xd2;
            pdVar5 = (double *)(unaff_r8 + 0x10);
            *pdVar5 = in_xd0;
            pdVar5[-1] = dVar15;
            pdVar5[-2] = in_xd4;
            pdVar5 = pdVar5 + -3;
            *pdVar5 = dVar20;
            dVar17 = in_xd2;
            dVar15 = in_xd0;
          }
          *(undefined4 *)((int)pdVar5 + -4) = uVar22;
          unaff_r8 = (int)pdVar5 + 0x1c;
          goto LAB_8c614afc;
        }
      }
      if (in_FPSCR_SZ == '\0') {
        in_dr14 = (double)CONCAT44(fVar28,SUB84(dVar20,0));
      }
      iVar3 = *(int *)(puVar13 + -0x14);
      if (in_FPSCR_SZ == '\0') {
        uVar27 = CONCAT44(*(undefined4 *)(iVar8 + 0x10),fVar18);
        param_1 = (double)CONCAT44((int)((ulonglong)in_dr14 >> 0x20),in_fr13);
        in_fr13 = *(float *)(iVar8 + 0x14);
        in_xd12 = dVar29;
      }
      else {
        uVar27 = *(undefined8 *)(iVar8 + 0x10);
        in_xd12 = *(double *)(iVar8 + 0x14);
        param_1 = in_dr14;
        in_xd4 = dVar29;
      }
      in_dr14 = (double)CONCAT44((int)((ulonglong)uVar27 >> 0x20),-(float)uVar27);
      *(int *)(puVar13 + -0x14) = iVar3 + -1;
      iVar4 = iVar7;
      iVar7 = iVar8;
      dVar29 = in_xd12;
    } while (iVar3 + -1 != 0);
    if (uVar6 < unaff_r8) {
      if (in_FPSCR_SZ == '\0') {
        uVar22 = SUB84(dVar15,0);
        uVar14 = SUB84(param_1,0);
        uVar24 = *(undefined4 *)(iVar8 + 0x10);
        param_1 = (double)CONCAT44(uVar24,uVar14);
        uVar26 = *(undefined4 *)(iVar8 + 0x14);
        *(int *)(unaff_r8 + 0x1c) = SUB84(dVar17,0);
        *(undefined4 *)(unaff_r8 + 0x18) = uVar22;
        *(undefined4 *)(unaff_r8 + 0x14) = uVar26;
        *(undefined4 *)(unaff_r8 + 0x10) = uVar14;
        pdVar5 = (double *)(unaff_r8 + 0xc);
        *(undefined4 *)pdVar5 = uVar24;
        dVar15 = (double)CONCAT44(uVar22,uVar22);
      }
      else {
        param_1 = *(double *)(iVar8 + 0x10);
        dVar15 = *(double *)(iVar8 + 0x18);
        *(double *)(unaff_r8 + 0x18) = in_xd2;
        pdVar5 = (double *)(unaff_r8 + 0x10);
        *pdVar5 = in_xd0;
        pdVar5[-1] = dVar15;
        pdVar5[-2] = in_xd4;
        pdVar5 = pdVar5 + -3;
        *pdVar5 = param_1;
        dVar15 = in_xd0;
      }
      *(undefined4 *)((int)pdVar5 + -4) = uVar2;
      unaff_r8 = (int)pdVar5 + 0x1c;
      *(uint *)(puVar13 + -0xc) = *(int *)(puVar13 + -0xc) + (unaff_r8 - uVar6) + 0x20;
    }
    iVar4 = *(int *)(puVar13 + -8);
    *(int *)(puVar13 + -8) = iVar4 + -1;
    if (iVar4 + -1 == 0) {
      *DAT_8c614bec = *DAT_8c614bec + (*(uint *)(puVar13 + -0xc) >> 5);
      *DAT_8c614bf0 = *DAT_8c614bf0 + *(int *)(puVar13 + -0x10);
      return;
    }
  } while( true );
}

