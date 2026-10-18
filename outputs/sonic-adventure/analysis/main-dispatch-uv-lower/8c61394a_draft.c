/* Ghidra draft only; not buildable/verified source. */

/* WARNING: Removing unreachable block (ram,0x8c613b10) */
/* WARNING: Removing unreachable block (ram,0x8c613aca) */
/* WARNING: Removing unreachable block (ram,0x8c613b7e) */
/* WARNING: Removing unreachable block (ram,0x8c613a62) */
/* WARNING: Removing unreachable block (ram,0x8c613b3e) */
/* WARNING: Restarted to delay deadcode elimination for space: register */

void FUN_8c61394a(double param_1,double param_2,double param_3,double param_4,int param_5)

{
  short sVar1;
  ushort uVar2;
  short sVar3;
  undefined4 uVar4;
  int iVar5;
  undefined8 *puVar6;
  double *unaff_r8;
  double *pdVar7;
  double *pdVar8;
  undefined8 *puVar9;
  undefined8 *puVar10;
  ushort *puVar11;
  ushort *puVar12;
  ushort *puVar13;
  short *psVar14;
  short *psVar15;
  short *psVar16;
  undefined1 *puVar17;
  double dVar18;
  undefined8 uVar19;
  float fVar20;
  double dVar21;
  float fVar25;
  uint uVar26;
  ulonglong uVar22;
  double dVar23;
  float fVar27;
  double dVar24;
  undefined4 uVar28;
  undefined4 uVar29;
  double dVar30;
  float in_fr13;
  undefined4 in_fr12;
  double in_dr14;
  float fVar31;
  double in_xd0;
  double in_xd2;
  double in_xd4;
  double in_xd10;
  double in_xd12;
  undefined8 in_xd14;
  bool bVar32;
  undefined4 in_PR;
  char in_FPSCR_PR;
  char in_FPSCR_SZ;
  undefined1 local_30 [12];
  undefined1 local_24 [4];
  undefined8 uStack_20;
  
  uVar4 = DAT_8c613990;
  if (in_FPSCR_SZ == '\0') {
    uStack_20 = CONCAT44(SUB84(in_dr14,0),(undefined4)uStack_20);
                    /* WARNING: Ignoring partial resolution of indirect */
    uStack_20._0_4_ = (int)((ulonglong)in_dr14 >> 0x20);
    puVar17 = local_24;
  }
  else {
    puVar17 = local_30;
    uStack_20 = in_xd14;
  }
  *(undefined4 *)(puVar17 + -4) = in_PR;
  dVar18 = (double)(*(code *)PTR_FUN_8c613994)();
  psVar14 = *(short **)(param_5 + 0x14);
  puVar11 = *(ushort **)(param_5 + 4);
  *(uint *)(puVar17 + -8) = (uint)*(ushort *)(param_5 + 2);
  *(undefined4 *)(puVar17 + -0x14) = *DAT_8c613998;
  *(undefined4 *)(puVar17 + -0x10) = 0;
  *(undefined4 *)(puVar17 + -0xc) = 0;
  do {
    uVar29 = SUB84(in_dr14,0);
    uVar2 = *puVar11;
    puVar12 = puVar11 + 1;
    *(uint *)(puVar17 + -0x18) = (uint)uVar2;
    if (in_FPSCR_SZ == '\0') {
      uVar29 = 0x3f800000;
    }
    if ((uVar2 & DAT_8c613a90) != 0) {
      if (in_FPSCR_PR == '\0') {
        uVar28 = 0xbf800000;
      }
      else {
        uVar28 = (undefined4)((ulonglong)-(double)CONCAT44(0x3f800000,SUB84(param_1,0)) >> 0x20);
      }
      *(uint *)(puVar17 + -0x18) = *(uint *)(puVar17 + -0x18) & (int)DAT_8c613a8e;
      if (in_FPSCR_SZ == '\0') {
        uVar29 = uVar28;
      }
    }
    *(int *)(puVar17 + -0xc) = *(int *)(puVar17 + -0xc) + *(int *)(puVar17 + -0x18);
    *(int *)(puVar17 + -0x18) = *(int *)(puVar17 + -0x18) + -2;
    puVar13 = puVar11 + 2;
    puVar11 = puVar11 + 3;
    puVar9 = (undefined8 *)((uint)*puVar13 * 0x20 + *(int *)(puVar17 + -0x14));
    puVar6 = (undefined8 *)((uint)*puVar12 * 0x20 + *(int *)(puVar17 + -0x14));
    pdVar8 = unaff_r8;
    if (in_FPSCR_SZ == '\0') {
      in_dr14 = (double)CONCAT44(*(undefined4 *)(puVar9 + 2),uVar29);
      in_fr13 = *(float *)((int)puVar9 + 0x14);
      param_1 = (double)CONCAT44(*(undefined4 *)(puVar6 + 2),*(undefined4 *)((int)puVar6 + 0x14));
      dVar24 = in_xd12;
    }
    else {
      in_dr14 = (double)puVar9[2];
      param_1 = (double)puVar6[2];
      in_xd4 = *(double *)((int)puVar6 + 0x14);
      dVar24 = *(double *)((int)puVar9 + 0x14);
    }
    do {
      uVar29 = DAT_8c613c1c;
      uVar2 = *puVar11;
      puVar11 = puVar11 + 1;
      puVar10 = (undefined8 *)((uint)uVar2 * 0x20 + *(int *)(puVar17 + -0x14));
      fVar31 = (float)((ulonglong)in_dr14 >> 0x20);
      if (in_FPSCR_SZ == '\0') {
        dVar21 = (double)CONCAT44(*(undefined4 *)((int)puVar10 + 0x14),fVar31);
      }
      else {
        dVar21 = *(double *)((int)puVar10 + 0x14);
        in_xd2 = in_dr14;
      }
      if (in_FPSCR_PR == '\0') {
        dVar21 = (double)CONCAT44((int)((ulonglong)dVar21 >> 0x20),
                                  SUB84(dVar21,0) - (float)((ulonglong)param_1 >> 0x20));
      }
      else {
        dVar21 = dVar21 - param_1;
      }
      if (in_FPSCR_PR == '\0') {
        dVar21 = (double)CONCAT44((float)((ulonglong)dVar21 >> 0x20) - in_fr13,SUB84(dVar21,0));
      }
      else {
        dVar21 = dVar21 - (double)CONCAT44(in_fr12,in_fr13);
      }
      if (in_FPSCR_SZ == '\0') {
        dVar18 = (double)CONCAT44((int)((ulonglong)dVar18 >> 0x20),*(undefined4 *)(puVar10 + 2));
      }
      else {
        in_xd0 = (double)puVar10[2];
      }
      if (in_FPSCR_PR == '\0') {
        dVar18 = (double)CONCAT44((int)((ulonglong)dVar18 >> 0x20),SUB84(dVar18,0) - fVar31);
        fVar20 = SUB84(dVar21,0) * (float)((ulonglong)dVar21 >> 0x20);
      }
      else {
        dVar18 = dVar18 - in_dr14;
        fVar20 = SUB84(dVar21 * dVar21,0);
      }
      dVar21 = dVar24;
      if (in_FPSCR_SZ == '\0') {
        dVar21 = (double)CONCAT44(in_fr13,fVar20);
      }
      fVar20 = SUB84(dVar18,0);
      if (in_FPSCR_PR == '\0') {
        fVar25 = ((float)((ulonglong)dVar21 >> 0x20) - SUB84(param_1,0)) * fVar20;
        dVar21 = (double)CONCAT44(fVar25,SUB84(dVar21,0) - fVar25);
      }
      else {
        dVar21 = (dVar21 - param_1) * dVar18;
        dVar21 = dVar21 - dVar21;
      }
      dVar30 = in_xd2;
      if (in_FPSCR_SZ == '\0') {
        dVar30 = (double)CONCAT44(SUB84(dVar21,0),SUB84(param_1,0));
      }
      fVar25 = SUB84(in_dr14,0);
      if (in_FPSCR_PR == '\0') {
        dVar30 = (double)CONCAT44((float)((ulonglong)dVar30 >> 0x20) * fVar25,SUB84(dVar30,0));
      }
      else {
        dVar30 = dVar30 * in_dr14;
      }
      uVar26 = (uint)((ulonglong)dVar21 >> 0x20);
      fVar27 = (float)((ulonglong)dVar30 >> 0x20);
      uVar28 = SUB84(dVar30,0);
      if (pdVar8 < unaff_r8) {
        if (in_FPSCR_PR == '\0') {
          bVar32 = fVar27 < 0.0;
        }
        else {
          bVar32 = dVar30 < (double)((ulonglong)uVar26 << 0x20);
        }
        psVar15 = psVar14;
        if (bVar32) {
LAB_8c613ae2:
          uVar28 = DAT_8c613c1c;
          uVar29 = SUB84(dVar30,0);
          psVar16 = psVar15 + 2;
          if (in_FPSCR_SZ == '\0') {
            uVar19 = CONCAT44(*(undefined4 *)puVar9,SUB84(dVar18,0));
            uVar22 = (ulonglong)*(uint *)((int)puVar9 + 4) << 0x20;
            pdVar7 = (double *)(puVar9 + 1);
          }
          else {
            uVar19 = *puVar9;
            uVar22 = puVar9[1];
            pdVar7 = (double *)(puVar9 + 2);
          }
          dVar23 = (double)(uVar22 & 0xffffffff00000000);
          dVar18 = in_xd2;
          dVar21 = in_xd0;
          if (in_FPSCR_SZ == '\0') {
            dVar18 = (double)((ulonglong)param_4 & 0xffffffff);
            dVar21 = (double)CONCAT44((float)uVar19,SUB84(param_3,0));
          }
                    /* WARNING: Ignoring partial resolution of indirect */
          fVar20 = (float)((ulonglong)uVar19 >> 0x20);
          if (in_FPSCR_PR == '\0') {
            fVar27 = (float)uVar19 * SUB84(dVar21,0) + fVar20 * (float)((ulonglong)dVar21 >> 0x20) +
                     SUB84(dVar18,0) * 0.0 +
                     (float)((ulonglong)dVar23 >> 0x20) * (float)((ulonglong)dVar18 >> 0x20);
          }
          if (in_FPSCR_SZ == '\0') {
            dVar30 = (double)CONCAT44(*(undefined4 *)pdVar7,uVar29);
            param_2 = (double)CONCAT44(*(undefined4 *)((int)pdVar7 + 4),SUB84(param_2,0));
            param_3 = (double)CONCAT44(uVar29,SUB84(dVar21,0));
          }
          else {
            dVar30 = *pdVar7;
            param_2 = pdVar7[1];
            uVar29 = SUB84(dVar30,0);
            param_3 = in_xd4;
          }
          sVar3 = *psVar15;
          sVar1 = psVar15[1];
          if (in_FPSCR_SZ == '\0') {
            *(int *)((int)unaff_r8 + 0xc) = (int)((ulonglong)param_2 >> 0x20);
            unaff_r8 = unaff_r8 + 1;
            *(undefined4 *)unaff_r8 = uVar29;
          }
          else {
            unaff_r8[1] = param_2;
            *unaff_r8 = in_xd4;
          }
          if (in_FPSCR_PR == '\0') {
            param_4 = (double)(ulonglong)(uint)(SUB84(dVar18,0) - SUB84(dVar23,0));
          }
          else {
            param_4 = dVar18 - dVar23;
          }
          if (in_FPSCR_SZ == '\0') {
            pdVar7 = (double *)((int)unaff_r8 + -4);
            *(int *)pdVar7 = (int)((ulonglong)dVar30 >> 0x20);
          }
          else {
            pdVar7 = unaff_r8 + -1;
            *pdVar7 = dVar30;
          }
          fVar27 = (float)((ulonglong)param_3 >> 0x20);
          if (in_FPSCR_PR == '\0') {
            dVar18 = (double)CONCAT44(fVar20,(float)(int)sVar1 * fVar27);
          }
          else {
            dVar18 = (double)CONCAT44(fVar20,(float)(int)sVar1) * param_3;
          }
          *(undefined4 *)((int)pdVar7 + -4) = uVar28;
          if (in_FPSCR_PR == '\0') {
            dVar21 = (double)CONCAT44((float)(int)sVar3 * fVar27,SUB84(dVar23,0));
          }
          else {
            dVar21 = (double)(int)sVar3 * param_3;
          }
          if (in_FPSCR_SZ == '\0') {
            *(int *)((int)pdVar7 + 0x14) = SUB84(param_4,0);
            *(int *)(pdVar7 + 2) = SUB84(dVar18,0);
            pdVar7 = (double *)((int)pdVar7 + 0xc);
            *(int *)pdVar7 = (int)((ulonglong)dVar21 >> 0x20);
          }
          else {
            pdVar7[2] = in_xd10;
            pdVar7[1] = in_xd0;
            *pdVar7 = dVar21;
          }
          unaff_r8 = pdVar7 + 2;
        }
        else {
          if (in_FPSCR_SZ == '\0') {
            uVar19 = CONCAT44(*(undefined4 *)puVar9,fVar20);
            uVar22 = (ulonglong)*(uint *)((int)puVar9 + 4) << 0x20;
            pdVar7 = (double *)(puVar9 + 1);
          }
          else {
            uVar19 = *puVar9;
            uVar22 = puVar9[1];
            pdVar7 = (double *)(puVar9 + 2);
          }
          dVar23 = (double)(uVar22 & 0xffffffff00000000);
          dVar18 = in_xd2;
          dVar21 = in_xd0;
          if (in_FPSCR_SZ == '\0') {
            dVar18 = (double)((ulonglong)param_4 & 0xffffffff);
            dVar21 = (double)CONCAT44((float)uVar19,SUB84(param_3,0));
          }
                    /* WARNING: Ignoring partial resolution of indirect */
          fVar20 = (float)((ulonglong)uVar19 >> 0x20);
          if (in_FPSCR_PR == '\0') {
            fVar27 = (float)uVar19 * SUB84(dVar21,0) + fVar20 * (float)((ulonglong)dVar21 >> 0x20) +
                     SUB84(dVar18,0) * 0.0 +
                     (float)((ulonglong)dVar23 >> 0x20) * (float)((ulonglong)dVar18 >> 0x20);
          }
          if (in_FPSCR_SZ == '\0') {
            dVar30 = (double)CONCAT44(*(undefined4 *)pdVar7,uVar28);
            param_2 = (double)CONCAT44(*(undefined4 *)((int)pdVar7 + 4),SUB84(param_2,0));
            param_3 = (double)CONCAT44(uVar28,SUB84(dVar21,0));
          }
          else {
            dVar30 = *pdVar7;
            param_2 = pdVar7[1];
            uVar28 = SUB84(dVar30,0);
            param_3 = in_xd4;
          }
          sVar3 = *psVar14;
          sVar1 = psVar14[1];
          if (in_FPSCR_SZ == '\0') {
            *(int *)((int)unaff_r8 + 0xc) = (int)((ulonglong)param_2 >> 0x20);
            unaff_r8 = unaff_r8 + 1;
            *(undefined4 *)unaff_r8 = uVar28;
          }
          else {
            unaff_r8[1] = param_2;
            *unaff_r8 = in_xd4;
          }
          if (in_FPSCR_PR == '\0') {
            param_4 = (double)(ulonglong)(uint)(SUB84(dVar18,0) - SUB84(dVar23,0));
          }
          else {
            param_4 = dVar18 - dVar23;
          }
          if (in_FPSCR_SZ == '\0') {
            pdVar7 = (double *)((int)unaff_r8 + -4);
            *(int *)pdVar7 = (int)((ulonglong)dVar30 >> 0x20);
          }
          else {
            pdVar7 = unaff_r8 + -1;
            *pdVar7 = dVar30;
          }
          fVar27 = (float)((ulonglong)param_3 >> 0x20);
          if (in_FPSCR_PR == '\0') {
            dVar18 = (double)CONCAT44(fVar20,(float)(int)sVar1 * fVar27);
          }
          else {
            dVar18 = (double)CONCAT44(fVar20,(float)(int)sVar1) * param_3;
          }
          *(undefined4 *)((int)pdVar7 + -4) = uVar4;
          if (in_FPSCR_PR == '\0') {
            dVar21 = (double)CONCAT44((float)(int)sVar3 * fVar27,SUB84(dVar23,0));
          }
          else {
            dVar21 = (double)(int)sVar3 * param_3;
          }
          if (in_FPSCR_SZ == '\0') {
            *(int *)((int)pdVar7 + 0x14) = SUB84(param_4,0);
            *(int *)(pdVar7 + 2) = SUB84(dVar18,0);
            pdVar7 = (double *)((int)pdVar7 + 0xc);
            *(int *)pdVar7 = (int)((ulonglong)dVar21 >> 0x20);
          }
          else {
            pdVar7[2] = in_xd10;
            pdVar7[1] = in_xd0;
            *pdVar7 = dVar21;
          }
          unaff_r8 = pdVar7 + 2;
          *(int *)(puVar17 + -0x10) =
               (int)unaff_r8 + *(int *)(puVar17 + -0x10) + (0x20 - (int)pdVar8);
          pdVar8 = unaff_r8;
          psVar16 = psVar14;
        }
      }
      else {
        if (in_FPSCR_PR == '\0') {
          bVar32 = fVar27 < 0.0;
        }
        else {
          bVar32 = dVar30 < (double)((ulonglong)uVar26 << 0x20);
        }
        psVar16 = psVar14 + 2;
        if (bVar32) {
          psVar15 = psVar14 + 2;
          if (in_FPSCR_SZ == '\0') {
            uVar19 = CONCAT44(*(undefined4 *)puVar6,fVar20);
            uVar22 = (ulonglong)*(uint *)((int)puVar6 + 4) << 0x20;
            pdVar7 = (double *)(puVar6 + 1);
          }
          else {
            uVar19 = *puVar6;
            uVar22 = puVar6[1];
            pdVar7 = (double *)(puVar6 + 2);
          }
          dVar23 = (double)(uVar22 & 0xffffffff00000000);
          dVar18 = in_xd2;
          dVar21 = in_xd0;
          if (in_FPSCR_SZ == '\0') {
            dVar18 = (double)((ulonglong)param_4 & 0xffffffff);
            dVar21 = (double)CONCAT44((float)uVar19,SUB84(param_3,0));
          }
                    /* WARNING: Ignoring partial resolution of indirect */
          fVar20 = (float)((ulonglong)uVar19 >> 0x20);
          if (in_FPSCR_PR == '\0') {
            fVar27 = (float)uVar19 * SUB84(dVar21,0) + fVar20 * (float)((ulonglong)dVar21 >> 0x20) +
                     SUB84(dVar18,0) * 0.0 +
                     (float)((ulonglong)dVar23 >> 0x20) * (float)((ulonglong)dVar18 >> 0x20);
          }
          if (in_FPSCR_SZ == '\0') {
            dVar30 = (double)CONCAT44(*(undefined4 *)pdVar7,uVar28);
            param_2 = (double)CONCAT44(*(undefined4 *)((int)pdVar7 + 4),SUB84(param_2,0));
            param_3 = (double)CONCAT44(uVar28,SUB84(dVar21,0));
          }
          else {
            dVar30 = *pdVar7;
            param_2 = pdVar7[1];
            uVar28 = SUB84(dVar30,0);
            param_3 = in_xd4;
          }
          sVar3 = *psVar14;
          sVar1 = psVar14[1];
          if (in_FPSCR_SZ == '\0') {
            *(int *)((int)unaff_r8 + 0xc) = (int)((ulonglong)param_2 >> 0x20);
            unaff_r8 = unaff_r8 + 1;
            *(undefined4 *)unaff_r8 = uVar28;
          }
          else {
            unaff_r8[1] = param_2;
            *unaff_r8 = in_xd4;
          }
          if (in_FPSCR_PR == '\0') {
            param_4 = (double)(ulonglong)(uint)(SUB84(dVar18,0) - SUB84(dVar23,0));
          }
          else {
            param_4 = dVar18 - dVar23;
          }
          if (in_FPSCR_SZ == '\0') {
            pdVar7 = (double *)((int)unaff_r8 + -4);
            *(int *)pdVar7 = (int)((ulonglong)dVar30 >> 0x20);
          }
          else {
            pdVar7 = unaff_r8 + -1;
            *pdVar7 = dVar30;
          }
          fVar27 = (float)((ulonglong)param_3 >> 0x20);
          if (in_FPSCR_PR == '\0') {
            dVar18 = (double)(ulonglong)(uint)((float)(int)sVar1 * fVar27);
          }
          else {
            dVar18 = (double)CONCAT44(fVar20,(float)(int)sVar1) * param_3;
          }
          *(undefined4 *)((int)pdVar7 + -4) = uVar29;
          if (in_FPSCR_PR == '\0') {
            dVar21 = (double)CONCAT44((float)(int)sVar3 * fVar27,SUB84(dVar23,0));
          }
          else {
            dVar21 = (double)(int)sVar3 * param_3;
          }
          if (in_FPSCR_SZ == '\0') {
            *(int *)((int)pdVar7 + 0x14) = SUB84(param_4,0);
            *(int *)(pdVar7 + 2) = SUB84(dVar18,0);
            pdVar7 = (double *)((int)pdVar7 + 0xc);
            *(int *)pdVar7 = (int)((ulonglong)dVar21 >> 0x20);
          }
          else {
            pdVar7[2] = in_xd10;
            pdVar7[1] = in_xd0;
            *pdVar7 = dVar21;
          }
          unaff_r8 = pdVar7 + 2;
          goto LAB_8c613ae2;
        }
      }
      if (in_FPSCR_SZ == '\0') {
        in_dr14 = (double)CONCAT44(fVar31,SUB84(dVar30,0));
      }
      iVar5 = *(int *)(puVar17 + -0x18);
      if (in_FPSCR_SZ == '\0') {
        uVar19 = CONCAT44(*(undefined4 *)(puVar10 + 2),fVar25);
        param_1 = (double)CONCAT44((int)((ulonglong)in_dr14 >> 0x20),in_fr13);
        in_fr13 = *(float *)((int)puVar10 + 0x14);
        in_xd12 = dVar24;
      }
      else {
        uVar19 = puVar10[2];
        in_xd12 = *(double *)((int)puVar10 + 0x14);
        param_1 = in_dr14;
        in_xd4 = dVar24;
      }
      in_dr14 = (double)CONCAT44((int)((ulonglong)uVar19 >> 0x20),-(float)uVar19);
      *(int *)(puVar17 + -0x18) = iVar5 + -1;
      puVar6 = puVar9;
      puVar9 = puVar10;
      psVar14 = psVar16;
      dVar24 = in_xd12;
    } while (iVar5 + -1 != 0);
    psVar14 = psVar16 + 4;
    if (pdVar8 < unaff_r8) {
      psVar14 = psVar16 + 2;
      if (in_FPSCR_SZ == '\0') {
        uVar19 = CONCAT44(*(undefined4 *)puVar10,SUB84(dVar18,0));
        uVar22 = (ulonglong)*(uint *)((int)puVar10 + 4) << 0x20;
        pdVar7 = (double *)(puVar10 + 1);
      }
      else {
        uVar19 = *puVar10;
        uVar22 = puVar10[1];
        pdVar7 = (double *)(puVar10 + 2);
      }
      dVar21 = (double)(uVar22 & 0xffffffff00000000);
      dVar18 = in_xd2;
      dVar24 = in_xd0;
      if (in_FPSCR_SZ == '\0') {
        dVar18 = (double)((ulonglong)param_4 & 0xffffffff);
        dVar24 = (double)CONCAT44((float)uVar19,SUB84(param_3,0));
      }
                    /* WARNING: Ignoring partial resolution of indirect */
      fVar31 = (float)((ulonglong)uVar19 >> 0x20);
      if (in_FPSCR_PR == '\0') {
        fVar20 = (float)uVar19 * SUB84(dVar24,0) + fVar31 * (float)((ulonglong)dVar24 >> 0x20) +
                 SUB84(dVar18,0) * 0.0 +
                 (float)((ulonglong)dVar21 >> 0x20) * (float)((ulonglong)dVar18 >> 0x20);
      }
      if (in_FPSCR_SZ == '\0') {
        uVar29 = SUB84(param_1,0);
        param_1 = (double)CONCAT44(*(undefined4 *)pdVar7,uVar29);
        param_2 = (double)CONCAT44(*(undefined4 *)((int)pdVar7 + 4),SUB84(param_2,0));
        param_3 = (double)CONCAT44(uVar29,SUB84(dVar24,0));
      }
      else {
        param_1 = *pdVar7;
        param_2 = pdVar7[1];
        uVar29 = SUB84(param_1,0);
        param_3 = in_xd4;
      }
      sVar3 = *psVar16;
      sVar1 = psVar16[1];
      if (in_FPSCR_SZ == '\0') {
        *(int *)((int)unaff_r8 + 0xc) = (int)((ulonglong)param_2 >> 0x20);
        unaff_r8 = unaff_r8 + 1;
        *(undefined4 *)unaff_r8 = uVar29;
      }
      else {
        unaff_r8[1] = param_2;
        *unaff_r8 = in_xd4;
      }
      if (in_FPSCR_PR == '\0') {
        param_4 = (double)(ulonglong)(uint)(SUB84(dVar18,0) - SUB84(dVar21,0));
      }
      else {
        param_4 = dVar18 - dVar21;
      }
      if (in_FPSCR_SZ == '\0') {
        pdVar7 = (double *)((int)unaff_r8 + -4);
        *(int *)pdVar7 = (int)((ulonglong)param_1 >> 0x20);
      }
      else {
        pdVar7 = unaff_r8 + -1;
        *pdVar7 = param_1;
      }
      fVar20 = (float)((ulonglong)param_3 >> 0x20);
      if (in_FPSCR_PR == '\0') {
        dVar18 = (double)CONCAT44(fVar31,(float)(int)sVar1 * fVar20);
      }
      else {
        dVar18 = (double)CONCAT44(fVar31,(float)(int)sVar1) * param_3;
      }
      *(undefined4 *)((int)pdVar7 + -4) = uVar4;
      if (in_FPSCR_PR == '\0') {
        dVar24 = (double)CONCAT44((float)(int)sVar3 * fVar20,SUB84(dVar21,0));
      }
      else {
        dVar24 = (double)(int)sVar3 * param_3;
      }
      if (in_FPSCR_SZ == '\0') {
        *(int *)((int)pdVar7 + 0x14) = SUB84(param_4,0);
        *(int *)(pdVar7 + 2) = SUB84(dVar18,0);
        pdVar7 = (double *)((int)pdVar7 + 0xc);
        *(int *)pdVar7 = (int)((ulonglong)dVar24 >> 0x20);
      }
      else {
        pdVar7[2] = in_xd10;
        pdVar7[1] = in_xd0;
        *pdVar7 = dVar24;
      }
      unaff_r8 = pdVar7 + 2;
      *(int *)(puVar17 + -0x10) = (int)unaff_r8 + *(int *)(puVar17 + -0x10) + (0x20 - (int)pdVar8);
    }
    iVar5 = *(int *)(puVar17 + -8);
    *(int *)(puVar17 + -8) = iVar5 + -1;
    if (iVar5 + -1 == 0) {
      *DAT_8c613c20 = *DAT_8c613c20 + (*(uint *)(puVar17 + -0x10) >> 5);
      *DAT_8c613c24 = *DAT_8c613c24 + *(int *)(puVar17 + -0xc);
      return;
    }
  } while( true );
}

