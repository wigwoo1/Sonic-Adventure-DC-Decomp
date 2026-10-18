/* Ghidra draft only; not buildable/verified source. */

/* WARNING: Removing unreachable block (ram,0x8c613fda) */
/* WARNING: Restarted to delay deadcode elimination for space: register */

void FUN_8c613dfa(double param_1,double param_2,double param_3,double param_4,undefined4 param_5)

{
  ushort uVar1;
  undefined4 uVar2;
  int iVar3;
  undefined8 *puVar4;
  double *pdVar5;
  double *unaff_r8;
  double *pdVar6;
  double *pdVar7;
  undefined8 *puVar8;
  undefined8 *puVar9;
  int iVar10;
  ushort *puVar11;
  ushort *puVar12;
  ushort *puVar13;
  undefined1 *puVar14;
  double dVar15;
  float fVar16;
  double dVar17;
  float fVar19;
  uint uVar20;
  float fVar21;
  double dVar18;
  undefined4 uVar24;
  double dVar22;
  undefined8 uVar23;
  undefined8 uVar26;
  undefined4 uVar27;
  float in_fr13;
  undefined4 in_fr12;
  double in_dr14;
  float fVar29;
  undefined8 uVar28;
  undefined8 in_xd0;
  double in_xd2;
  double in_xd4;
  double in_xd6;
  double in_xd8;
  double in_xd12;
  undefined8 in_xd14;
  bool bVar30;
  undefined4 in_PR;
  char in_FPSCR_PR;
  char in_FPSCR_SZ;
  undefined1 local_30 [12];
  undefined1 local_24 [4];
  undefined8 uStack_20;
  float fVar25;
  
  uVar2 = DAT_8c613e40;
  if (in_FPSCR_SZ == '\0') {
    uStack_20 = CONCAT44(SUB84(in_dr14,0),(undefined4)uStack_20);
                    /* WARNING: Ignoring partial resolution of indirect */
    uStack_20._0_4_ = (int)((ulonglong)in_dr14 >> 0x20);
    puVar14 = local_24;
  }
  else {
    puVar14 = local_30;
    uStack_20 = in_xd14;
  }
  *(undefined4 *)(puVar14 + -4) = in_PR;
  *(undefined4 *)(puVar14 + -0x10) = param_5;
  dVar15 = (double)(*(code *)PTR_FUN_8c613e44)();
  puVar11 = *(ushort **)(*(int *)(puVar14 + -0x10) + 4);
  *(uint *)(puVar14 + -8) = (uint)*(ushort *)(*(int *)(puVar14 + -0x10) + 2);
  iVar10 = *DAT_8c613e48;
  *(undefined4 *)(puVar14 + -0xc) = 0;
  *(undefined4 *)(puVar14 + -0x10) = 0;
  do {
    uVar27 = SUB84(in_dr14,0);
    uVar1 = *puVar11;
    puVar12 = puVar11 + 1;
    *(uint *)(puVar14 + -0x14) = (uint)uVar1;
    if (in_FPSCR_SZ == '\0') {
      uVar27 = 0x3f800000;
    }
    if ((uVar1 & DAT_8c613f38) != 0) {
      if (in_FPSCR_PR == '\0') {
        uVar24 = 0xbf800000;
      }
      else {
        uVar24 = (undefined4)((ulonglong)-(double)CONCAT44(0x3f800000,SUB84(param_1,0)) >> 0x20);
      }
      *(uint *)(puVar14 + -0x14) = *(uint *)(puVar14 + -0x14) & (int)DAT_8c613f34;
      if (in_FPSCR_SZ == '\0') {
        uVar27 = uVar24;
      }
    }
    *(int *)(puVar14 + -0x10) = *(int *)(puVar14 + -0x10) + *(int *)(puVar14 + -0x14);
    *(int *)(puVar14 + -0x14) = *(int *)(puVar14 + -0x14) + -2;
    puVar13 = puVar11 + 2;
    puVar11 = puVar11 + 3;
    puVar8 = (undefined8 *)((uint)*puVar13 * 0x20 + iVar10);
    puVar4 = (undefined8 *)((uint)*puVar12 * 0x20 + iVar10);
    pdVar7 = unaff_r8;
    if (in_FPSCR_SZ == '\0') {
      in_dr14 = (double)CONCAT44(*(undefined4 *)(puVar8 + 2),uVar27);
      in_fr13 = *(float *)((int)puVar8 + 0x14);
      param_1 = (double)CONCAT44(*(undefined4 *)(puVar4 + 2),*(undefined4 *)((int)puVar4 + 0x14));
      dVar18 = in_xd12;
    }
    else {
      in_dr14 = (double)puVar8[2];
      param_1 = (double)puVar4[2];
      in_xd4 = *(double *)((int)puVar4 + 0x14);
      dVar18 = *(double *)((int)puVar8 + 0x14);
    }
    do {
      uVar24 = DAT_8c6140b0;
      uVar27 = SUB84(param_3,0);
      uVar1 = *puVar11;
      puVar11 = puVar11 + 1;
      puVar9 = (undefined8 *)((uint)uVar1 * 0x20 + iVar10);
      fVar29 = (float)((ulonglong)in_dr14 >> 0x20);
      if (in_FPSCR_SZ == '\0') {
        dVar17 = (double)CONCAT44(*(undefined4 *)((int)puVar9 + 0x14),fVar29);
      }
      else {
        dVar17 = *(double *)((int)puVar9 + 0x14);
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
        dVar15 = (double)CONCAT44((int)((ulonglong)dVar15 >> 0x20),*(undefined4 *)(puVar9 + 2));
      }
      else {
        in_xd0 = puVar9[2];
      }
      if (in_FPSCR_PR == '\0') {
        dVar15 = (double)CONCAT44((int)((ulonglong)dVar15 >> 0x20),SUB84(dVar15,0) - fVar29);
        fVar16 = SUB84(dVar17,0) * (float)((ulonglong)dVar17 >> 0x20);
      }
      else {
        dVar15 = dVar15 - in_dr14;
        fVar16 = SUB84(dVar17 * dVar17,0);
      }
      dVar17 = dVar18;
      if (in_FPSCR_SZ == '\0') {
        dVar17 = (double)CONCAT44(in_fr13,fVar16);
      }
      fVar16 = SUB84(dVar15,0);
      if (in_FPSCR_PR == '\0') {
        fVar19 = ((float)((ulonglong)dVar17 >> 0x20) - SUB84(param_1,0)) * fVar16;
        dVar17 = (double)CONCAT44(fVar19,SUB84(dVar17,0) - fVar19);
      }
      else {
        dVar17 = (dVar17 - param_1) * dVar15;
        dVar17 = dVar17 - dVar17;
      }
      dVar22 = in_xd2;
      if (in_FPSCR_SZ == '\0') {
        dVar22 = (double)CONCAT44(SUB84(dVar17,0),SUB84(param_1,0));
      }
      fVar19 = SUB84(in_dr14,0);
      if (in_FPSCR_PR == '\0') {
        dVar22 = (double)CONCAT44((float)((ulonglong)dVar22 >> 0x20) * fVar19,SUB84(dVar22,0));
      }
      else {
        dVar22 = dVar22 * in_dr14;
      }
      uVar20 = (uint)((ulonglong)dVar17 >> 0x20);
      fVar25 = (float)((ulonglong)dVar22 >> 0x20);
      if (pdVar7 < unaff_r8) {
        dVar17 = (double)((ulonglong)uVar20 << 0x20);
        if (in_FPSCR_PR == '\0') {
          bVar30 = fVar25 < 0.0;
        }
        else {
          bVar30 = dVar22 < dVar17;
        }
        if (bVar30) {
LAB_8c613f82:
          uVar24 = DAT_8c6140b0;
          uVar27 = SUB84(param_3,0);
          if (in_FPSCR_SZ == '\0') {
            uVar23 = CONCAT44(*(undefined4 *)puVar8,SUB84(dVar22,0));
            uVar26 = CONCAT44(*(undefined4 *)((int)puVar8 + 4),SUB84(param_2,0));
            pdVar5 = (double *)(puVar8 + 1);
            dVar17 = (double)CONCAT44(SUB84(dVar17,0),SUB84(dVar17,0));
            uVar28 = CONCAT44(SUB84(dVar15,0),SUB84(dVar15,0));
          }
          else {
            uVar23 = *puVar8;
            uVar26 = puVar8[1];
            pdVar5 = (double *)(puVar8 + 2);
            dVar17 = in_xd2;
            uVar28 = in_xd0;
          }
          dVar17 = (double)((ulonglong)dVar17 & 0xffffffff00000000);
                    /* WARNING: Ignoring partial resolution of indirect */
          fVar25 = (float)uVar26;
          fVar16 = (float)uVar23;
          if (in_FPSCR_PR == '\0') {
            fVar21 = (float)uVar28 * fVar16 +
                     (float)((ulonglong)uVar28 >> 0x20) * (float)((ulonglong)uVar23 >> 0x20) +
                     fVar25 * 0.0 +
                     (float)((ulonglong)dVar17 >> 0x20) * (float)((ulonglong)uVar26 >> 0x20);
          }
          if (in_FPSCR_SZ == '\0') {
            param_3 = (double)CONCAT44(*(undefined4 *)pdVar5,uVar27);
            param_4 = (double)CONCAT44(*(undefined4 *)((int)pdVar5 + 4),SUB84(param_4,0));
            pdVar5 = pdVar5 + 1;
            dVar22 = (double)CONCAT44(fVar16,fVar16);
            dVar15 = (double)CONCAT44(uVar27,(float)uVar28);
          }
          else {
            param_3 = *pdVar5;
            param_4 = pdVar5[1];
            pdVar5 = pdVar5 + 2;
            uVar27 = SUB84(param_3,0);
            dVar22 = in_xd4;
            dVar15 = in_xd8;
          }
          if (in_FPSCR_PR == '\0') {
            dVar15 = (double)CONCAT44((float)((ulonglong)dVar15 >> 0x20) - SUB84(dVar17,0),
                                      SUB84(dVar15,0));
          }
          else {
            dVar15 = dVar15 - dVar17;
          }
          fVar16 = (float)((ulonglong)dVar15 >> 0x20);
          if (in_FPSCR_SZ == '\0') {
            pdVar6 = (double *)((int)unaff_r8 + 0xc);
            *(int *)pdVar6 = (int)((ulonglong)param_4 >> 0x20);
            pdVar5 = (double *)((int)pdVar5 + -4);
            *(float *)pdVar5 = fVar16;
            param_2 = (double)CONCAT44(fVar25,fVar25);
          }
          else {
            pdVar6 = unaff_r8 + 1;
            *pdVar6 = param_4;
            pdVar5 = pdVar5 + -1;
            *pdVar5 = dVar15;
            param_2 = in_xd6;
          }
          CacheBlockWriteBack(pdVar5);
          if (in_FPSCR_PR == '\0') {
            param_2 = (double)CONCAT44((int)((ulonglong)param_2 >> 0x20),SUB84(param_2,0) * fVar16);
          }
          else {
            param_2 = param_2 * dVar15;
          }
          if (in_FPSCR_SZ == '\0') {
            pdVar6 = (double *)((int)pdVar6 + -4);
            *(undefined4 *)pdVar6 = uVar27;
          }
          else {
            pdVar6 = pdVar6 + -1;
            *pdVar6 = in_xd8;
          }
          if (in_FPSCR_PR == '\0') {
            param_2 = (double)CONCAT44((float)((ulonglong)param_2 >> 0x20) * fVar16,SUB84(param_2,0)
                                      );
          }
          else {
            param_2 = param_2 * dVar15;
          }
          if (in_FPSCR_SZ == '\0') {
            pdVar6 = (double *)((int)pdVar6 + -4);
            *(int *)pdVar6 = (int)((ulonglong)param_3 >> 0x20);
          }
          else {
            pdVar6 = pdVar6 + -1;
            *pdVar6 = param_3;
          }
          if (in_FPSCR_PR == '\0') {
            dVar22 = (double)CONCAT44((int)((ulonglong)dVar22 >> 0x20),SUB84(dVar22,0) * fVar16);
          }
          else {
            dVar22 = dVar22 * dVar15;
          }
          *(undefined4 *)((int)pdVar6 + -4) = uVar24;
          if (in_FPSCR_SZ == '\0') {
            *(int *)(pdVar6 + 3) = SUB84(param_2,0);
            *(int *)((int)pdVar6 + 0x14) = (int)((ulonglong)param_2 >> 0x20);
            *(int *)(pdVar6 + 2) = SUB84(dVar22,0);
            pdVar6 = (double *)((int)pdVar6 + 0xc);
            *(int *)pdVar6 = (int)((ulonglong)dVar22 >> 0x20);
          }
          else {
            *(double *)((int)pdVar6 + 0x14) = in_xd6;
            *(double *)((int)pdVar6 + 0xc) = param_2;
            *(double *)((int)pdVar6 + 4) = in_xd4;
            pdVar6 = (double *)((int)pdVar6 + -4);
            *pdVar6 = dVar22;
          }
          unaff_r8 = pdVar6 + 2;
        }
        else {
          if (in_FPSCR_SZ == '\0') {
            uVar23 = CONCAT44(*(undefined4 *)puVar8,SUB84(dVar22,0));
            uVar26 = CONCAT44(*(undefined4 *)((int)puVar8 + 4),SUB84(param_2,0));
            pdVar5 = (double *)(puVar8 + 1);
            dVar17 = 0.0;
            uVar28 = CONCAT44(fVar16,fVar16);
          }
          else {
            uVar23 = *puVar8;
            uVar26 = puVar8[1];
            pdVar5 = (double *)(puVar8 + 2);
            dVar17 = in_xd2;
            uVar28 = in_xd0;
          }
          dVar17 = (double)((ulonglong)dVar17 & 0xffffffff00000000);
                    /* WARNING: Ignoring partial resolution of indirect */
          fVar25 = (float)uVar26;
          fVar16 = (float)uVar23;
          if (in_FPSCR_PR == '\0') {
            fVar21 = (float)uVar28 * fVar16 +
                     (float)((ulonglong)uVar28 >> 0x20) * (float)((ulonglong)uVar23 >> 0x20) +
                     fVar25 * 0.0 +
                     (float)((ulonglong)dVar17 >> 0x20) * (float)((ulonglong)uVar26 >> 0x20);
          }
          if (in_FPSCR_SZ == '\0') {
            param_3 = (double)CONCAT44(*(undefined4 *)pdVar5,uVar27);
            param_4 = (double)CONCAT44(*(undefined4 *)((int)pdVar5 + 4),SUB84(param_4,0));
            pdVar5 = pdVar5 + 1;
            dVar22 = (double)CONCAT44(fVar16,fVar16);
            dVar15 = (double)CONCAT44(uVar27,(float)uVar28);
          }
          else {
            param_3 = *pdVar5;
            param_4 = pdVar5[1];
            pdVar5 = pdVar5 + 2;
            uVar27 = SUB84(param_3,0);
            dVar22 = in_xd4;
            dVar15 = in_xd8;
          }
          if (in_FPSCR_PR == '\0') {
            dVar15 = (double)CONCAT44((float)((ulonglong)dVar15 >> 0x20) - SUB84(dVar17,0),
                                      SUB84(dVar15,0));
          }
          else {
            dVar15 = dVar15 - dVar17;
          }
          fVar16 = (float)((ulonglong)dVar15 >> 0x20);
          if (in_FPSCR_SZ == '\0') {
            pdVar6 = (double *)((int)unaff_r8 + 0xc);
            *(int *)pdVar6 = (int)((ulonglong)param_4 >> 0x20);
            pdVar5 = (double *)((int)pdVar5 + -4);
            *(float *)pdVar5 = fVar16;
            param_2 = (double)CONCAT44(fVar25,fVar25);
          }
          else {
            pdVar6 = unaff_r8 + 1;
            *pdVar6 = param_4;
            pdVar5 = pdVar5 + -1;
            *pdVar5 = dVar15;
            param_2 = in_xd6;
          }
          CacheBlockWriteBack(pdVar5);
          if (in_FPSCR_PR == '\0') {
            param_2 = (double)CONCAT44((int)((ulonglong)param_2 >> 0x20),SUB84(param_2,0) * fVar16);
          }
          else {
            param_2 = param_2 * dVar15;
          }
          if (in_FPSCR_SZ == '\0') {
            pdVar6 = (double *)((int)pdVar6 + -4);
            *(undefined4 *)pdVar6 = uVar27;
          }
          else {
            pdVar6 = pdVar6 + -1;
            *pdVar6 = in_xd8;
          }
          if (in_FPSCR_PR == '\0') {
            param_2 = (double)CONCAT44((float)((ulonglong)param_2 >> 0x20) * fVar16,SUB84(param_2,0)
                                      );
          }
          else {
            param_2 = param_2 * dVar15;
          }
          if (in_FPSCR_SZ == '\0') {
            pdVar6 = (double *)((int)pdVar6 + -4);
            *(int *)pdVar6 = (int)((ulonglong)param_3 >> 0x20);
          }
          else {
            pdVar6 = pdVar6 + -1;
            *pdVar6 = param_3;
          }
          if (in_FPSCR_PR == '\0') {
            dVar22 = (double)CONCAT44((int)((ulonglong)dVar22 >> 0x20),SUB84(dVar22,0) * fVar16);
          }
          else {
            dVar22 = dVar22 * dVar15;
          }
          *(undefined4 *)((int)pdVar6 + -4) = uVar2;
          if (in_FPSCR_SZ == '\0') {
            *(int *)(pdVar6 + 3) = SUB84(param_2,0);
            *(int *)((int)pdVar6 + 0x14) = (int)((ulonglong)param_2 >> 0x20);
            *(int *)(pdVar6 + 2) = SUB84(dVar22,0);
            pdVar6 = (double *)((int)pdVar6 + 0xc);
            *(int *)pdVar6 = (int)((ulonglong)dVar22 >> 0x20);
          }
          else {
            *(double *)((int)pdVar6 + 0x14) = in_xd6;
            *(double *)((int)pdVar6 + 0xc) = param_2;
            *(double *)((int)pdVar6 + 4) = in_xd4;
            pdVar6 = (double *)((int)pdVar6 + -4);
            *pdVar6 = dVar22;
          }
          unaff_r8 = pdVar6 + 2;
          *(int *)(puVar14 + -0xc) = (int)unaff_r8 + *(int *)(puVar14 + -0xc) + (0x20 - (int)pdVar7)
          ;
          pdVar7 = unaff_r8;
        }
      }
      else {
        dVar17 = (double)((ulonglong)uVar20 << 0x20);
        if (in_FPSCR_PR == '\0') {
          bVar30 = fVar25 < 0.0;
        }
        else {
          bVar30 = dVar22 < dVar17;
        }
        if (bVar30) {
          if (in_FPSCR_SZ == '\0') {
            uVar23 = CONCAT44(*(undefined4 *)puVar4,SUB84(dVar22,0));
            uVar26 = CONCAT44(*(undefined4 *)((int)puVar4 + 4),SUB84(param_2,0));
            pdVar5 = (double *)(puVar4 + 1);
            dVar17 = 0.0;
            uVar28 = CONCAT44(fVar16,fVar16);
          }
          else {
            uVar23 = *puVar4;
            uVar26 = puVar4[1];
            pdVar5 = (double *)(puVar4 + 2);
            dVar17 = in_xd2;
            uVar28 = in_xd0;
          }
          dVar17 = (double)((ulonglong)dVar17 & 0xffffffff00000000);
                    /* WARNING: Ignoring partial resolution of indirect */
          fVar25 = (float)uVar26;
          fVar16 = (float)uVar23;
          if (in_FPSCR_PR == '\0') {
            fVar21 = (float)uVar28 * fVar16 +
                     (float)((ulonglong)uVar28 >> 0x20) * (float)((ulonglong)uVar23 >> 0x20) +
                     fVar25 * 0.0 +
                     (float)((ulonglong)dVar17 >> 0x20) * (float)((ulonglong)uVar26 >> 0x20);
          }
          if (in_FPSCR_SZ == '\0') {
            param_3 = (double)CONCAT44(*(undefined4 *)pdVar5,uVar27);
            param_4 = (double)CONCAT44(*(undefined4 *)((int)pdVar5 + 4),SUB84(param_4,0));
            pdVar5 = pdVar5 + 1;
            dVar22 = (double)CONCAT44(fVar16,fVar16);
            dVar15 = (double)CONCAT44(uVar27,(float)uVar28);
          }
          else {
            param_3 = *pdVar5;
            param_4 = pdVar5[1];
            pdVar5 = pdVar5 + 2;
            uVar27 = SUB84(param_3,0);
            dVar22 = in_xd4;
            dVar15 = in_xd8;
          }
          if (in_FPSCR_PR == '\0') {
            dVar15 = (double)CONCAT44((float)((ulonglong)dVar15 >> 0x20) - SUB84(dVar17,0),
                                      SUB84(dVar15,0));
          }
          else {
            dVar15 = dVar15 - dVar17;
          }
          fVar16 = (float)((ulonglong)dVar15 >> 0x20);
          if (in_FPSCR_SZ == '\0') {
            pdVar6 = (double *)((int)unaff_r8 + 0xc);
            *(int *)pdVar6 = (int)((ulonglong)param_4 >> 0x20);
            pdVar5 = (double *)((int)pdVar5 + -4);
            *(float *)pdVar5 = fVar16;
            param_2 = (double)CONCAT44(fVar25,fVar25);
          }
          else {
            pdVar6 = unaff_r8 + 1;
            *pdVar6 = param_4;
            pdVar5 = pdVar5 + -1;
            *pdVar5 = dVar15;
            param_2 = in_xd6;
          }
          CacheBlockWriteBack(pdVar5);
          if (in_FPSCR_PR == '\0') {
            param_2 = (double)CONCAT44((int)((ulonglong)param_2 >> 0x20),SUB84(param_2,0) * fVar16);
          }
          else {
            param_2 = param_2 * dVar15;
          }
          if (in_FPSCR_SZ == '\0') {
            pdVar6 = (double *)((int)pdVar6 + -4);
            *(undefined4 *)pdVar6 = uVar27;
          }
          else {
            pdVar6 = pdVar6 + -1;
            *pdVar6 = in_xd8;
          }
          if (in_FPSCR_PR == '\0') {
            param_2 = (double)CONCAT44((float)((ulonglong)param_2 >> 0x20) * fVar16,SUB84(param_2,0)
                                      );
          }
          else {
            param_2 = param_2 * dVar15;
          }
          if (in_FPSCR_SZ == '\0') {
            pdVar6 = (double *)((int)pdVar6 + -4);
            *(int *)pdVar6 = (int)((ulonglong)param_3 >> 0x20);
          }
          else {
            pdVar6 = pdVar6 + -1;
            *pdVar6 = param_3;
          }
          if (in_FPSCR_PR == '\0') {
            dVar22 = (double)CONCAT44((int)((ulonglong)dVar22 >> 0x20),SUB84(dVar22,0) * fVar16);
          }
          else {
            dVar22 = dVar22 * dVar15;
          }
          *(undefined4 *)((int)pdVar6 + -4) = uVar24;
          if (in_FPSCR_SZ == '\0') {
            *(int *)(pdVar6 + 3) = SUB84(param_2,0);
            *(int *)((int)pdVar6 + 0x14) = (int)((ulonglong)param_2 >> 0x20);
            *(int *)(pdVar6 + 2) = SUB84(dVar22,0);
            pdVar6 = (double *)((int)pdVar6 + 0xc);
            *(int *)pdVar6 = (int)((ulonglong)dVar22 >> 0x20);
          }
          else {
            *(double *)((int)pdVar6 + 0x14) = in_xd6;
            *(double *)((int)pdVar6 + 0xc) = param_2;
            *(double *)((int)pdVar6 + 4) = in_xd4;
            pdVar6 = (double *)((int)pdVar6 + -4);
            *pdVar6 = dVar22;
          }
          unaff_r8 = pdVar6 + 2;
          goto LAB_8c613f82;
        }
      }
      uVar27 = SUB84(param_3,0);
      if (in_FPSCR_SZ == '\0') {
        in_dr14 = (double)CONCAT44(fVar29,SUB84(dVar22,0));
      }
      iVar3 = *(int *)(puVar14 + -0x14);
      if (in_FPSCR_SZ == '\0') {
        uVar28 = CONCAT44(*(undefined4 *)(puVar9 + 2),fVar19);
        param_1 = (double)CONCAT44((int)((ulonglong)in_dr14 >> 0x20),in_fr13);
        in_fr13 = *(float *)((int)puVar9 + 0x14);
        in_xd12 = dVar18;
      }
      else {
        uVar28 = puVar9[2];
        in_xd12 = *(double *)((int)puVar9 + 0x14);
        param_1 = in_dr14;
        in_xd4 = dVar18;
      }
      in_dr14 = (double)CONCAT44((int)((ulonglong)uVar28 >> 0x20),-(float)uVar28);
      *(int *)(puVar14 + -0x14) = iVar3 + -1;
      puVar4 = puVar8;
      puVar8 = puVar9;
      dVar18 = in_xd12;
    } while (iVar3 + -1 != 0);
    if (pdVar7 < unaff_r8) {
      if (in_FPSCR_SZ == '\0') {
        uVar23 = CONCAT44(*(undefined4 *)puVar9,SUB84(param_1,0));
        uVar26 = CONCAT44(*(undefined4 *)((int)puVar9 + 4),SUB84(param_2,0));
        pdVar5 = (double *)(puVar9 + 1);
        dVar18 = (double)CONCAT44(SUB84(dVar17,0),SUB84(dVar17,0));
        uVar28 = CONCAT44(SUB84(dVar15,0),SUB84(dVar15,0));
      }
      else {
        uVar23 = *puVar9;
        uVar26 = puVar9[1];
        pdVar5 = (double *)(puVar9 + 2);
        dVar18 = in_xd2;
        uVar28 = in_xd0;
      }
      dVar18 = (double)((ulonglong)dVar18 & 0xffffffff00000000);
                    /* WARNING: Ignoring partial resolution of indirect */
      fVar16 = (float)uVar26;
      fVar29 = (float)uVar23;
      if (in_FPSCR_PR == '\0') {
        fVar19 = (float)uVar28 * fVar29 +
                 (float)((ulonglong)uVar28 >> 0x20) * (float)((ulonglong)uVar23 >> 0x20) +
                 fVar16 * 0.0 +
                 (float)((ulonglong)dVar18 >> 0x20) * (float)((ulonglong)uVar26 >> 0x20);
      }
      if (in_FPSCR_SZ == '\0') {
        param_3 = (double)CONCAT44(*(undefined4 *)pdVar5,uVar27);
        param_4 = (double)CONCAT44(*(undefined4 *)((int)pdVar5 + 4),SUB84(param_4,0));
        pdVar5 = pdVar5 + 1;
        param_1 = (double)CONCAT44(fVar29,fVar29);
        dVar15 = (double)CONCAT44(uVar27,(float)uVar28);
      }
      else {
        param_3 = *pdVar5;
        param_4 = pdVar5[1];
        pdVar5 = pdVar5 + 2;
        uVar27 = SUB84(param_3,0);
        param_1 = in_xd4;
        dVar15 = in_xd8;
      }
      if (in_FPSCR_PR == '\0') {
        dVar15 = (double)CONCAT44((float)((ulonglong)dVar15 >> 0x20) - SUB84(dVar18,0),
                                  SUB84(dVar15,0));
      }
      else {
        dVar15 = dVar15 - dVar18;
      }
      fVar29 = (float)((ulonglong)dVar15 >> 0x20);
      if (in_FPSCR_SZ == '\0') {
        pdVar6 = (double *)((int)unaff_r8 + 0xc);
        *(int *)pdVar6 = (int)((ulonglong)param_4 >> 0x20);
        pdVar5 = (double *)((int)pdVar5 + -4);
        *(float *)pdVar5 = fVar29;
        param_2 = (double)CONCAT44(fVar16,fVar16);
      }
      else {
        pdVar6 = unaff_r8 + 1;
        *pdVar6 = param_4;
        pdVar5 = pdVar5 + -1;
        *pdVar5 = dVar15;
        param_2 = in_xd6;
      }
      CacheBlockWriteBack(pdVar5);
      if (in_FPSCR_PR == '\0') {
        param_2 = (double)CONCAT44((int)((ulonglong)param_2 >> 0x20),SUB84(param_2,0) * fVar29);
      }
      else {
        param_2 = param_2 * dVar15;
      }
      if (in_FPSCR_SZ == '\0') {
        pdVar6 = (double *)((int)pdVar6 + -4);
        *(undefined4 *)pdVar6 = uVar27;
      }
      else {
        pdVar6 = pdVar6 + -1;
        *pdVar6 = in_xd8;
      }
      if (in_FPSCR_PR == '\0') {
        param_2 = (double)CONCAT44((float)((ulonglong)param_2 >> 0x20) * fVar29,SUB84(param_2,0));
      }
      else {
        param_2 = param_2 * dVar15;
      }
      if (in_FPSCR_SZ == '\0') {
        pdVar6 = (double *)((int)pdVar6 + -4);
        *(int *)pdVar6 = (int)((ulonglong)param_3 >> 0x20);
      }
      else {
        pdVar6 = pdVar6 + -1;
        *pdVar6 = param_3;
      }
      if (in_FPSCR_PR == '\0') {
        param_1 = (double)CONCAT44((int)((ulonglong)param_1 >> 0x20),SUB84(param_1,0) * fVar29);
      }
      else {
        param_1 = param_1 * dVar15;
      }
      *(undefined4 *)((int)pdVar6 + -4) = uVar2;
      if (in_FPSCR_SZ == '\0') {
        *(int *)(pdVar6 + 3) = SUB84(param_2,0);
        *(int *)((int)pdVar6 + 0x14) = (int)((ulonglong)param_2 >> 0x20);
        *(int *)(pdVar6 + 2) = SUB84(param_1,0);
        pdVar6 = (double *)((int)pdVar6 + 0xc);
        *(int *)pdVar6 = (int)((ulonglong)param_1 >> 0x20);
      }
      else {
        *(double *)((int)pdVar6 + 0x14) = in_xd6;
        *(double *)((int)pdVar6 + 0xc) = param_2;
        *(double *)((int)pdVar6 + 4) = in_xd4;
        pdVar6 = (double *)((int)pdVar6 + -4);
        *pdVar6 = param_1;
      }
      unaff_r8 = pdVar6 + 2;
      *(int *)(puVar14 + -0xc) = (int)unaff_r8 + *(int *)(puVar14 + -0xc) + (0x20 - (int)pdVar7);
    }
    iVar3 = *(int *)(puVar14 + -8);
    *(int *)(puVar14 + -8) = iVar3 + -1;
    if (iVar3 + -1 == 0) {
      *DAT_8c6140b4 = *DAT_8c6140b4 + (*(uint *)(puVar14 + -0xc) >> 5);
      *DAT_8c6140b8 = *DAT_8c6140b8 + *(int *)(puVar14 + -0x10);
      return;
    }
  } while( true );
}

