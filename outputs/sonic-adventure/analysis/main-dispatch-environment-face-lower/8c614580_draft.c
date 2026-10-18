/* Ghidra draft only; not buildable/verified source. */

/* WARNING: Removing unreachable block (ram,0x8c614716) */
/* WARNING: Restarted to delay deadcode elimination for space: register */

void FUN_8c614580(double param_1,undefined4 param_2)

{
  ushort uVar1;
  undefined4 uVar2;
  int iVar3;
  double *pdVar4;
  double *unaff_r8;
  double *pdVar5;
  double *pdVar6;
  double *pdVar7;
  int iVar8;
  ushort *puVar9;
  ushort *puVar10;
  ushort *puVar11;
  undefined1 *puVar12;
  float fVar13;
  float fVar14;
  float fVar15;
  double dVar16;
  float fVar17;
  float fVar18;
  double dVar19;
  float fVar21;
  uint uVar22;
  float fVar23;
  double dVar20;
  double dVar24;
  undefined4 uVar25;
  undefined4 uVar26;
  undefined4 uVar27;
  float in_fr13;
  undefined4 in_fr12;
  double in_dr14;
  float fVar28;
  double in_xd0;
  float fVar29;
  float fVar30;
  double in_xd2;
  float fVar31;
  double in_xd4;
  float fVar32;
  float in_xf6;
  float in_xf7;
  float in_xf8;
  float in_xf9;
  float in_xf10;
  float in_xf11;
  float fVar33;
  double in_xd12;
  float fVar34;
  float fVar35;
  double in_xd14;
  float fVar36;
  bool bVar37;
  undefined4 in_PR;
  char in_FPSCR_PR;
  char in_FPSCR_SZ;
  undefined1 local_30 [12];
  undefined1 local_24 [4];
  undefined8 uStack_20;
  
  uVar2 = DAT_8c6145c4;
  if (in_FPSCR_SZ == '\0') {
    uStack_20 = (double)CONCAT44(SUB84(in_dr14,0),(undefined4)uStack_20);
                    /* WARNING: Ignoring partial resolution of indirect */
    uStack_20._0_4_ = (int)((ulonglong)in_dr14 >> 0x20);
    puVar12 = local_24;
  }
  else {
    puVar12 = local_30;
    uStack_20 = in_xd14;
  }
  *(undefined4 *)(puVar12 + -4) = in_PR;
  *(undefined4 *)(puVar12 + -0x10) = param_2;
  dVar16 = (double)(*(code *)PTR_FUN_8c6145c8)();
  puVar9 = *(ushort **)(*(int *)(puVar12 + -0x10) + 4);
  *(uint *)(puVar12 + -8) = (uint)*(ushort *)(*(int *)(puVar12 + -0x10) + 2);
  iVar8 = *DAT_8c6145cc;
  *(undefined4 *)(puVar12 + -0xc) = 0;
  *(undefined4 *)(puVar12 + -0x10) = 0;
  do {
    uVar27 = SUB84(in_dr14,0);
    uVar1 = *puVar9;
    puVar10 = puVar9 + 1;
    uVar25 = SUB84(param_1,0);
    dVar20 = (double)CONCAT44(0x3f800000,uVar25);
    *(uint *)(puVar12 + -0x14) = (uint)uVar1;
    if (in_FPSCR_SZ == '\0') {
      uVar27 = 0x3f800000;
      dVar20 = in_xd14;
    }
    in_xd14 = dVar20;
    if ((uVar1 & DAT_8c6146a4) != 0) {
      if (in_FPSCR_PR == '\0') {
        in_xd14 = (double)CONCAT44(0xbf800000,uVar25);
      }
      else {
        in_xd14 = -(double)CONCAT44(0x3f800000,uVar25);
      }
      *(uint *)(puVar12 + -0x14) = *(uint *)(puVar12 + -0x14) & (int)DAT_8c6146a0;
      if (in_FPSCR_SZ == '\0') {
        uVar27 = (undefined4)((ulonglong)in_xd14 >> 0x20);
        in_xd14 = dVar20;
      }
    }
    *(int *)(puVar12 + -0x10) = *(int *)(puVar12 + -0x10) + *(int *)(puVar12 + -0x14);
    *(int *)(puVar12 + -0x14) = *(int *)(puVar12 + -0x14) + -2;
    puVar11 = puVar9 + 2;
    puVar9 = puVar9 + 3;
    pdVar6 = (double *)((uint)*puVar11 * 0x20 + iVar8);
    pdVar4 = (double *)((uint)*puVar10 * 0x20 + iVar8);
    pdVar5 = unaff_r8;
    if (in_FPSCR_SZ == '\0') {
      in_dr14 = (double)CONCAT44(*(undefined4 *)(pdVar6 + 2),uVar27);
      in_fr13 = *(float *)((int)pdVar6 + 0x14);
      param_1 = (double)CONCAT44(*(undefined4 *)(pdVar4 + 2),*(undefined4 *)((int)pdVar4 + 0x14));
      dVar20 = in_xd12;
    }
    else {
      in_dr14 = pdVar6[2];
      param_1 = pdVar4[2];
      in_xd4 = *(double *)((int)pdVar4 + 0x14);
      dVar20 = *(double *)((int)pdVar6 + 0x14);
    }
    do {
      uVar25 = DAT_8c6147d0;
      uVar1 = *puVar9;
      puVar9 = puVar9 + 1;
      pdVar7 = (double *)((uint)uVar1 * 0x20 + iVar8);
      fVar23 = (float)((ulonglong)in_dr14 >> 0x20);
      if (in_FPSCR_SZ == '\0') {
        dVar19 = (double)CONCAT44(*(undefined4 *)((int)pdVar7 + 0x14),fVar23);
      }
      else {
        dVar19 = *(double *)((int)pdVar7 + 0x14);
        in_xd2 = in_dr14;
      }
      if (in_FPSCR_PR == '\0') {
        dVar19 = (double)CONCAT44((int)((ulonglong)dVar19 >> 0x20),
                                  SUB84(dVar19,0) - (float)((ulonglong)param_1 >> 0x20));
      }
      else {
        dVar19 = dVar19 - param_1;
      }
      if (in_FPSCR_PR == '\0') {
        dVar19 = (double)CONCAT44((float)((ulonglong)dVar19 >> 0x20) - in_fr13,SUB84(dVar19,0));
      }
      else {
        dVar19 = dVar19 - (double)CONCAT44(in_fr12,in_fr13);
      }
      if (in_FPSCR_SZ == '\0') {
        dVar16 = (double)CONCAT44((int)((ulonglong)dVar16 >> 0x20),*(undefined4 *)(pdVar7 + 2));
      }
      else {
        in_xd0 = pdVar7[2];
      }
      if (in_FPSCR_PR == '\0') {
        dVar16 = (double)CONCAT44((int)((ulonglong)dVar16 >> 0x20),SUB84(dVar16,0) - fVar23);
        fVar18 = SUB84(dVar19,0) * (float)((ulonglong)dVar19 >> 0x20);
      }
      else {
        dVar16 = dVar16 - in_dr14;
        fVar18 = SUB84(dVar19 * dVar19,0);
      }
      dVar19 = dVar20;
      if (in_FPSCR_SZ == '\0') {
        dVar19 = (double)CONCAT44(in_fr13,fVar18);
      }
      fVar18 = SUB84(dVar16,0);
      if (in_FPSCR_PR == '\0') {
        fVar21 = ((float)((ulonglong)dVar19 >> 0x20) - SUB84(param_1,0)) * fVar18;
        dVar19 = (double)CONCAT44(fVar21,SUB84(dVar19,0) - fVar21);
      }
      else {
        dVar19 = (dVar19 - param_1) * dVar16;
        dVar19 = dVar19 - dVar19;
      }
      dVar24 = in_xd2;
      if (in_FPSCR_SZ == '\0') {
        dVar24 = (double)CONCAT44(SUB84(dVar19,0),SUB84(param_1,0));
      }
      fVar21 = SUB84(in_dr14,0);
      if (in_FPSCR_PR == '\0') {
        dVar24 = (double)CONCAT44((float)((ulonglong)dVar24 >> 0x20) * fVar21,SUB84(dVar24,0));
      }
      else {
        dVar24 = dVar24 * in_dr14;
      }
      uVar22 = (uint)((ulonglong)dVar19 >> 0x20);
      fVar13 = (float)((ulonglong)dVar24 >> 0x20);
      fVar28 = SUB84(in_xd0,0);
      fVar29 = (float)((ulonglong)in_xd0 >> 0x20);
      fVar30 = SUB84(in_xd2,0);
      fVar31 = (float)((ulonglong)in_xd2 >> 0x20);
      fVar15 = SUB84(in_xd4,0);
      fVar32 = (float)((ulonglong)in_xd4 >> 0x20);
      fVar33 = SUB84(dVar20,0);
      fVar34 = (float)((ulonglong)dVar20 >> 0x20);
      fVar35 = SUB84(in_xd14,0);
      fVar36 = (float)((ulonglong)in_xd14 >> 0x20);
      uVar27 = SUB84(dVar24,0);
      if (pdVar5 < unaff_r8) {
        if (in_FPSCR_PR == '\0') {
          bVar37 = fVar13 < 0.0;
        }
        else {
          bVar37 = dVar24 < (double)((ulonglong)uVar22 << 0x20);
        }
        if (bVar37) {
LAB_8c6146d6:
          uVar25 = DAT_8c6147d0;
          uVar27 = SUB84(dVar24,0);
          if (in_FPSCR_SZ == '\0') {
            dVar16 = (double)CONCAT44(*(undefined4 *)pdVar6,SUB84(dVar16,0));
            fVar18 = *(float *)((int)pdVar6 + 4);
            pdVar4 = pdVar6 + 1;
          }
          else {
            dVar16 = *pdVar6;
            fVar18 = (float)((ulonglong)pdVar6[1] >> 0x20);
            pdVar4 = pdVar6 + 2;
          }
          dVar19 = (double)CONCAT44(fVar18,0x3f800000);
                    /* WARNING: Ignoring partial resolution of indirect */
          if (in_FPSCR_PR == '\0') {
            fVar13 = SUB84(dVar16,0);
            fVar14 = (float)((ulonglong)dVar16 >> 0x20);
            fVar15 = fVar28 * fVar13 + fVar15 * fVar14 + in_xf8 * 1.0 + fVar33 * fVar18;
            fVar15 = fVar29 * fVar13 + fVar32 * fVar14 + in_xf9 * 1.0 + fVar34 * fVar18;
            fVar15 = fVar30 * fVar13 + in_xf6 * fVar14 + in_xf10 * 1.0 + fVar35 * fVar18;
            fVar18 = fVar31 * fVar13 + in_xf7 * fVar14 + in_xf11 * 1.0 + fVar36 * fVar18;
          }
          if (in_FPSCR_SZ == '\0') {
            uVar26 = *(undefined4 *)pdVar4;
            dVar24 = (double)CONCAT44(uVar26,uVar27);
            *(undefined4 *)((int)unaff_r8 + 0xc) = *(undefined4 *)((int)pdVar4 + 4);
            *(undefined4 *)(unaff_r8 + 1) = uVar27;
            pdVar4 = (double *)((int)unaff_r8 + 4);
            *(undefined4 *)pdVar4 = uVar26;
          }
          else {
            dVar24 = *pdVar4;
            unaff_r8[1] = pdVar4[1];
            *unaff_r8 = in_xd4;
            pdVar4 = unaff_r8 + -1;
            *pdVar4 = dVar24;
          }
          *(undefined4 *)((int)pdVar4 + -4) = uVar25;
          if (in_FPSCR_SZ == '\0') {
            *(int *)((int)pdVar4 + 0x14) = (int)((ulonglong)dVar19 >> 0x20);
            *(int *)(pdVar4 + 2) = SUB84(dVar16,0);
            pdVar4 = (double *)((int)pdVar4 + 0xc);
            *(int *)pdVar4 = (int)((ulonglong)dVar16 >> 0x20);
          }
          else {
            pdVar4[2] = dVar19;
            pdVar4[1] = in_xd0;
            *pdVar4 = dVar16;
          }
          unaff_r8 = pdVar4 + 2;
        }
        else {
          if (in_FPSCR_SZ == '\0') {
            dVar16 = (double)CONCAT44(*(undefined4 *)pdVar6,fVar18);
            fVar18 = *(float *)((int)pdVar6 + 4);
            pdVar4 = pdVar6 + 1;
          }
          else {
            dVar16 = *pdVar6;
            fVar18 = (float)((ulonglong)pdVar6[1] >> 0x20);
            pdVar4 = pdVar6 + 2;
          }
          dVar19 = (double)CONCAT44(fVar18,0x3f800000);
                    /* WARNING: Ignoring partial resolution of indirect */
          if (in_FPSCR_PR == '\0') {
            fVar14 = SUB84(dVar16,0);
            fVar17 = (float)((ulonglong)dVar16 >> 0x20);
            fVar13 = fVar28 * fVar14 + fVar15 * fVar17 + in_xf8 * 1.0 + fVar33 * fVar18;
            fVar13 = fVar29 * fVar14 + fVar32 * fVar17 + in_xf9 * 1.0 + fVar34 * fVar18;
            fVar13 = fVar30 * fVar14 + in_xf6 * fVar17 + in_xf10 * 1.0 + fVar35 * fVar18;
            fVar18 = fVar31 * fVar14 + in_xf7 * fVar17 + in_xf11 * 1.0 + fVar36 * fVar18;
          }
          if (in_FPSCR_SZ == '\0') {
            uVar25 = *(undefined4 *)pdVar4;
            dVar24 = (double)CONCAT44(uVar25,uVar27);
            *(undefined4 *)((int)unaff_r8 + 0xc) = *(undefined4 *)((int)pdVar4 + 4);
            *(undefined4 *)(unaff_r8 + 1) = uVar27;
            pdVar4 = (double *)((int)unaff_r8 + 4);
            *(undefined4 *)pdVar4 = uVar25;
          }
          else {
            dVar24 = *pdVar4;
            unaff_r8[1] = pdVar4[1];
            *unaff_r8 = in_xd4;
            pdVar4 = unaff_r8 + -1;
            *pdVar4 = dVar24;
          }
          *(undefined4 *)((int)pdVar4 + -4) = uVar2;
          if (in_FPSCR_SZ == '\0') {
            *(int *)((int)pdVar4 + 0x14) = (int)((ulonglong)dVar19 >> 0x20);
            *(int *)(pdVar4 + 2) = SUB84(dVar16,0);
            pdVar4 = (double *)((int)pdVar4 + 0xc);
            *(int *)pdVar4 = (int)((ulonglong)dVar16 >> 0x20);
          }
          else {
            pdVar4[2] = dVar19;
            pdVar4[1] = in_xd0;
            *pdVar4 = dVar16;
          }
          unaff_r8 = pdVar4 + 2;
          *(int *)(puVar12 + -0xc) = (int)unaff_r8 + *(int *)(puVar12 + -0xc) + (0x20 - (int)pdVar5)
          ;
          pdVar5 = unaff_r8;
        }
      }
      else {
        if (in_FPSCR_PR == '\0') {
          bVar37 = fVar13 < 0.0;
        }
        else {
          bVar37 = dVar24 < (double)((ulonglong)uVar22 << 0x20);
        }
        if (bVar37) {
          if (in_FPSCR_SZ == '\0') {
            dVar16 = (double)CONCAT44(*(undefined4 *)pdVar4,fVar18);
            fVar18 = *(float *)((int)pdVar4 + 4);
            pdVar4 = pdVar4 + 1;
          }
          else {
            dVar16 = *pdVar4;
            fVar18 = (float)((ulonglong)pdVar4[1] >> 0x20);
            pdVar4 = pdVar4 + 2;
          }
          dVar19 = (double)CONCAT44(fVar18,0x3f800000);
                    /* WARNING: Ignoring partial resolution of indirect */
          if (in_FPSCR_PR == '\0') {
            fVar13 = SUB84(dVar16,0);
            fVar17 = (float)((ulonglong)dVar16 >> 0x20);
            fVar14 = fVar28 * fVar13 + fVar15 * fVar17 + in_xf8 * 1.0 + fVar33 * fVar18;
            fVar14 = fVar29 * fVar13 + fVar32 * fVar17 + in_xf9 * 1.0 + fVar34 * fVar18;
            fVar14 = fVar30 * fVar13 + in_xf6 * fVar17 + in_xf10 * 1.0 + fVar35 * fVar18;
            fVar18 = fVar31 * fVar13 + in_xf7 * fVar17 + in_xf11 * 1.0 + fVar36 * fVar18;
          }
          if (in_FPSCR_SZ == '\0') {
            uVar26 = *(undefined4 *)pdVar4;
            dVar24 = (double)CONCAT44(uVar26,uVar27);
            *(undefined4 *)((int)unaff_r8 + 0xc) = *(undefined4 *)((int)pdVar4 + 4);
            *(undefined4 *)(unaff_r8 + 1) = uVar27;
            pdVar4 = (double *)((int)unaff_r8 + 4);
            *(undefined4 *)pdVar4 = uVar26;
          }
          else {
            dVar24 = *pdVar4;
            unaff_r8[1] = pdVar4[1];
            *unaff_r8 = in_xd4;
            pdVar4 = unaff_r8 + -1;
            *pdVar4 = dVar24;
          }
          *(undefined4 *)((int)pdVar4 + -4) = uVar25;
          if (in_FPSCR_SZ == '\0') {
            *(int *)((int)pdVar4 + 0x14) = (int)((ulonglong)dVar19 >> 0x20);
            *(int *)(pdVar4 + 2) = SUB84(dVar16,0);
            pdVar4 = (double *)((int)pdVar4 + 0xc);
            *(int *)pdVar4 = (int)((ulonglong)dVar16 >> 0x20);
          }
          else {
            pdVar4[2] = dVar19;
            pdVar4[1] = in_xd0;
            *pdVar4 = dVar16;
          }
          unaff_r8 = pdVar4 + 2;
          goto LAB_8c6146d6;
        }
      }
      if (in_FPSCR_SZ == '\0') {
        in_dr14 = (double)CONCAT44(fVar23,SUB84(dVar24,0));
      }
      iVar3 = *(int *)(puVar12 + -0x14);
      if (in_FPSCR_SZ == '\0') {
        dVar19 = (double)CONCAT44(*(undefined4 *)(pdVar7 + 2),fVar21);
        param_1 = (double)CONCAT44((int)((ulonglong)in_dr14 >> 0x20),in_fr13);
        in_fr13 = *(float *)((int)pdVar7 + 0x14);
        in_xd12 = dVar20;
      }
      else {
        dVar19 = pdVar7[2];
        in_xd12 = *(double *)((int)pdVar7 + 0x14);
        param_1 = in_dr14;
        in_xd4 = dVar20;
      }
      in_dr14 = (double)CONCAT44((int)((ulonglong)dVar19 >> 0x20),-SUB84(dVar19,0));
      *(int *)(puVar12 + -0x14) = iVar3 + -1;
      pdVar4 = pdVar6;
      pdVar6 = pdVar7;
      dVar20 = in_xd12;
    } while (iVar3 + -1 != 0);
    if (pdVar5 < unaff_r8) {
      if (in_FPSCR_SZ == '\0') {
        dVar16 = (double)CONCAT44(*(undefined4 *)pdVar7,SUB84(dVar16,0));
        fVar23 = *(float *)((int)pdVar7 + 4);
        pdVar7 = pdVar7 + 1;
      }
      else {
        dVar16 = *pdVar7;
        fVar23 = (float)((ulonglong)pdVar7[1] >> 0x20);
        pdVar7 = pdVar7 + 2;
      }
      dVar20 = (double)CONCAT44(fVar23,0x3f800000);
                    /* WARNING: Ignoring partial resolution of indirect */
      if (in_FPSCR_PR == '\0') {
        fVar18 = SUB84(dVar16,0);
        fVar13 = (float)((ulonglong)dVar16 >> 0x20);
        fVar21 = fVar28 * fVar18 + SUB84(in_xd4,0) * fVar13 + in_xf8 * 1.0 +
                 SUB84(in_xd12,0) * fVar23;
        fVar21 = fVar29 * fVar18 + (float)((ulonglong)in_xd4 >> 0x20) * fVar13 + in_xf9 * 1.0 +
                 (float)((ulonglong)in_xd12 >> 0x20) * fVar23;
        fVar21 = fVar30 * fVar18 + in_xf6 * fVar13 + in_xf10 * 1.0 + fVar35 * fVar23;
        fVar23 = fVar31 * fVar18 + in_xf7 * fVar13 + in_xf11 * 1.0 + fVar36 * fVar23;
      }
      if (in_FPSCR_SZ == '\0') {
        uVar25 = SUB84(param_1,0);
        uVar27 = *(undefined4 *)pdVar7;
        param_1 = (double)CONCAT44(uVar27,uVar25);
        *(undefined4 *)((int)unaff_r8 + 0xc) = *(undefined4 *)((int)pdVar7 + 4);
        *(undefined4 *)(unaff_r8 + 1) = uVar25;
        pdVar4 = (double *)((int)unaff_r8 + 4);
        *(undefined4 *)pdVar4 = uVar27;
      }
      else {
        param_1 = *pdVar7;
        unaff_r8[1] = pdVar7[1];
        *unaff_r8 = in_xd4;
        pdVar4 = unaff_r8 + -1;
        *pdVar4 = param_1;
      }
      *(undefined4 *)((int)pdVar4 + -4) = uVar2;
      if (in_FPSCR_SZ == '\0') {
        *(int *)((int)pdVar4 + 0x14) = (int)((ulonglong)dVar20 >> 0x20);
        *(int *)(pdVar4 + 2) = SUB84(dVar16,0);
        pdVar4 = (double *)((int)pdVar4 + 0xc);
        *(int *)pdVar4 = (int)((ulonglong)dVar16 >> 0x20);
      }
      else {
        pdVar4[2] = dVar20;
        pdVar4[1] = in_xd0;
        *pdVar4 = dVar16;
      }
      unaff_r8 = pdVar4 + 2;
      *(int *)(puVar12 + -0xc) = (int)unaff_r8 + *(int *)(puVar12 + -0xc) + (0x20 - (int)pdVar5);
    }
    iVar3 = *(int *)(puVar12 + -8);
    *(int *)(puVar12 + -8) = iVar3 + -1;
    if (iVar3 + -1 == 0) {
      *DAT_8c6147d4 = *DAT_8c6147d4 + (*(uint *)(puVar12 + -0xc) >> 5);
      *DAT_8c6147d8 = *DAT_8c6147d8 + *(int *)(puVar12 + -0x10);
      return;
    }
  } while( true );
}

