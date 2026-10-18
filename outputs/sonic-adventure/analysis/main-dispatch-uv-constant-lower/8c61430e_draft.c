/* Ghidra draft only; not buildable/verified source. */

/* WARNING: Removing unreachable block (ram,0x8c6144b4) */
/* WARNING: Removing unreachable block (ram,0x8c614478) */
/* WARNING: Removing unreachable block (ram,0x8c614518) */
/* WARNING: Removing unreachable block (ram,0x8c614418) */
/* WARNING: Removing unreachable block (ram,0x8c6144e6) */

void FUN_8c61430e(double param_1,double param_2,int param_3)

{
  short sVar1;
  ushort uVar2;
  short sVar3;
  undefined4 uVar4;
  int iVar5;
  int iVar6;
  double *unaff_r8;
  double *pdVar7;
  double *pdVar8;
  int iVar9;
  int iVar10;
  ushort *puVar11;
  ushort *puVar12;
  ushort *puVar13;
  short *psVar14;
  short *psVar15;
  short *psVar16;
  undefined1 *puVar17;
  double dVar18;
  float fVar19;
  double dVar20;
  uint uVar22;
  float fVar23;
  float fVar24;
  double dVar21;
  undefined4 uVar26;
  double dVar25;
  float in_fr13;
  undefined4 in_fr12;
  undefined4 uVar27;
  double in_dr14;
  undefined8 uVar28;
  double in_xd0;
  double in_xd2;
  double in_xd4;
  double in_xd12;
  undefined8 in_xd14;
  bool bVar29;
  undefined4 in_PR;
  char in_FPSCR_PR;
  char in_FPSCR_SZ;
  undefined1 local_30 [12];
  undefined1 local_24 [4];
  undefined8 uStack_20;
  
  uVar4 = DAT_8c614354;
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
  dVar18 = (double)(*(code *)PTR_FUN_8c614358)();
  psVar14 = *(short **)(param_3 + 0x14);
  puVar11 = *(ushort **)(param_3 + 4);
  *(uint *)(puVar17 + -8) = (uint)*(ushort *)(param_3 + 2);
  *(undefined4 *)(puVar17 + -0x14) = *DAT_8c61435c;
  *(undefined4 *)(puVar17 + -0x10) = 0;
  *(undefined4 *)(puVar17 + -0xc) = 0;
  do {
    uVar27 = SUB84(in_dr14,0);
    uVar2 = *puVar11;
    puVar12 = puVar11 + 1;
    *(uint *)(puVar17 + -0x18) = (uint)uVar2;
    if (in_FPSCR_SZ == '\0') {
      uVar27 = 0x3f800000;
    }
    if ((uVar2 & DAT_8c61444c) != 0) {
      if (in_FPSCR_PR == '\0') {
        uVar26 = 0xbf800000;
      }
      else {
        uVar26 = (undefined4)((ulonglong)-(double)CONCAT44(0x3f800000,SUB84(param_1,0)) >> 0x20);
      }
      *(uint *)(puVar17 + -0x18) = *(uint *)(puVar17 + -0x18) & (int)DAT_8c61444a;
      if (in_FPSCR_SZ == '\0') {
        uVar27 = uVar26;
      }
    }
    *(int *)(puVar17 + -0xc) = *(int *)(puVar17 + -0xc) + *(int *)(puVar17 + -0x18);
    *(int *)(puVar17 + -0x18) = *(int *)(puVar17 + -0x18) + -2;
    puVar13 = puVar11 + 2;
    puVar11 = puVar11 + 3;
    iVar9 = (uint)*puVar13 * 0x20 + *(int *)(puVar17 + -0x14);
    iVar6 = (uint)*puVar12 * 0x20 + *(int *)(puVar17 + -0x14);
    pdVar8 = unaff_r8;
    if (in_FPSCR_SZ == '\0') {
      in_dr14 = (double)CONCAT44(*(undefined4 *)(iVar9 + 0x10),uVar27);
      in_fr13 = *(float *)(iVar9 + 0x14);
      param_1 = (double)CONCAT44(*(undefined4 *)(iVar6 + 0x10),*(undefined4 *)(iVar6 + 0x14));
      dVar21 = in_xd12;
    }
    else {
      in_dr14 = *(double *)(iVar9 + 0x10);
      param_1 = *(double *)(iVar6 + 0x10);
      in_xd4 = *(double *)(iVar6 + 0x14);
      dVar21 = *(double *)(iVar9 + 0x14);
    }
    do {
      uVar27 = DAT_8c6145b8;
      uVar2 = *puVar11;
      puVar11 = puVar11 + 1;
      iVar10 = (uint)uVar2 * 0x20 + *(int *)(puVar17 + -0x14);
      fVar24 = (float)((ulonglong)in_dr14 >> 0x20);
      if (in_FPSCR_SZ == '\0') {
        dVar20 = (double)CONCAT44(*(undefined4 *)(iVar10 + 0x14),fVar24);
      }
      else {
        dVar20 = *(double *)(iVar10 + 0x14);
        in_xd2 = in_dr14;
      }
      if (in_FPSCR_PR == '\0') {
        dVar20 = (double)CONCAT44((int)((ulonglong)dVar20 >> 0x20),
                                  SUB84(dVar20,0) - (float)((ulonglong)param_1 >> 0x20));
      }
      else {
        dVar20 = dVar20 - param_1;
      }
      if (in_FPSCR_PR == '\0') {
        dVar20 = (double)CONCAT44((float)((ulonglong)dVar20 >> 0x20) - in_fr13,SUB84(dVar20,0));
      }
      else {
        dVar20 = dVar20 - (double)CONCAT44(in_fr12,in_fr13);
      }
      if (in_FPSCR_SZ == '\0') {
        dVar18 = (double)CONCAT44((int)((ulonglong)dVar18 >> 0x20),*(undefined4 *)(iVar10 + 0x10));
      }
      else {
        in_xd0 = *(double *)(iVar10 + 0x10);
      }
      if (in_FPSCR_PR == '\0') {
        dVar18 = (double)CONCAT44((int)((ulonglong)dVar18 >> 0x20),SUB84(dVar18,0) - fVar24);
        fVar19 = SUB84(dVar20,0) * (float)((ulonglong)dVar20 >> 0x20);
      }
      else {
        dVar18 = dVar18 - in_dr14;
        fVar19 = SUB84(dVar20 * dVar20,0);
      }
      dVar20 = dVar21;
      if (in_FPSCR_SZ == '\0') {
        dVar20 = (double)CONCAT44(in_fr13,fVar19);
      }
      if (in_FPSCR_PR == '\0') {
        fVar19 = ((float)((ulonglong)dVar20 >> 0x20) - SUB84(param_1,0)) * SUB84(dVar18,0);
        dVar20 = (double)CONCAT44(fVar19,SUB84(dVar20,0) - fVar19);
      }
      else {
        dVar20 = (dVar20 - param_1) * dVar18;
        dVar20 = dVar20 - dVar20;
      }
      dVar25 = in_xd2;
      if (in_FPSCR_SZ == '\0') {
        dVar25 = (double)CONCAT44(SUB84(dVar20,0),SUB84(param_1,0));
      }
      fVar19 = SUB84(in_dr14,0);
      if (in_FPSCR_PR == '\0') {
        dVar25 = (double)CONCAT44((float)((ulonglong)dVar25 >> 0x20) * fVar19,SUB84(dVar25,0));
      }
      else {
        dVar25 = dVar25 * in_dr14;
      }
      uVar22 = (uint)((ulonglong)dVar20 >> 0x20);
      fVar23 = (float)((ulonglong)dVar25 >> 0x20);
      if (pdVar8 < unaff_r8) {
        if (in_FPSCR_PR == '\0') {
          bVar29 = fVar23 < 0.0;
        }
        else {
          bVar29 = dVar25 < (double)((ulonglong)uVar22 << 0x20);
        }
        psVar15 = psVar14;
        if (bVar29) {
LAB_8c614494:
          uVar27 = DAT_8c6145b8;
          psVar16 = psVar15 + 2;
          if (in_FPSCR_SZ == '\0') {
            dVar25 = (double)CONCAT44(*(undefined4 *)(iVar9 + 0x10),SUB84(dVar25,0));
            param_2 = (double)CONCAT44(*(undefined4 *)(iVar9 + 0x14),SUB84(param_2,0));
            dVar18 = (double)((longlong)dVar18 << 0x20);
          }
          else {
            dVar25 = *(double *)(iVar9 + 0x10);
            param_2 = *(double *)(iVar9 + 0x18);
            dVar18 = in_xd0;
          }
          sVar3 = *psVar15;
          sVar1 = psVar15[1];
          if (in_FPSCR_SZ == '\0') {
            *(int *)((int)unaff_r8 + 0xc) = (int)((ulonglong)param_2 >> 0x20);
            unaff_r8 = unaff_r8 + 1;
            *(int *)unaff_r8 = SUB84(dVar25,0);
          }
          else {
            unaff_r8[1] = param_2;
            *unaff_r8 = in_xd4;
          }
          fVar23 = (float)((ulonglong)dVar18 >> 0x20);
          dVar18 = (double)CONCAT44(fVar23,(float)(int)sVar1);
          if (in_FPSCR_SZ == '\0') {
            pdVar7 = (double *)((int)unaff_r8 + -4);
            *(int *)pdVar7 = (int)((ulonglong)dVar25 >> 0x20);
          }
          else {
            pdVar7 = unaff_r8 + -1;
            *pdVar7 = dVar25;
          }
          if (in_FPSCR_PR == '\0') {
            dVar18 = (double)CONCAT44(fVar23,(float)(int)sVar1 * fVar23);
          }
          else {
            dVar18 = dVar18 * dVar18;
          }
          *(undefined4 *)((int)pdVar7 + -4) = uVar27;
          if (in_FPSCR_PR == '\0') {
            fVar23 = (float)(int)sVar3;
          }
          else {
            fVar23 = (float)((ulonglong)(double)(int)sVar3 >> 0x20);
          }
          if (in_FPSCR_PR == '\0') {
            dVar20 = (double)CONCAT44(fVar23 * (float)((ulonglong)dVar18 >> 0x20),0x3f800000);
          }
          else {
            dVar20 = (double)CONCAT44(fVar23,0x3f800000) * dVar18;
          }
          if (in_FPSCR_SZ == '\0') {
            *(int *)((int)pdVar7 + 0x14) = SUB84(dVar20,0);
            *(int *)(pdVar7 + 2) = SUB84(dVar18,0);
            pdVar7 = (double *)((int)pdVar7 + 0xc);
            *(int *)pdVar7 = (int)((ulonglong)dVar20 >> 0x20);
          }
          else {
            pdVar7[2] = in_xd2;
            pdVar7[1] = in_xd0;
            *pdVar7 = dVar20;
          }
          unaff_r8 = pdVar7 + 2;
        }
        else {
          if (in_FPSCR_SZ == '\0') {
            dVar25 = (double)CONCAT44(*(undefined4 *)(iVar9 + 0x10),SUB84(dVar25,0));
            param_2 = (double)CONCAT44(*(undefined4 *)(iVar9 + 0x14),SUB84(param_2,0));
            dVar18 = (double)((longlong)dVar18 << 0x20);
          }
          else {
            dVar25 = *(double *)(iVar9 + 0x10);
            param_2 = *(double *)(iVar9 + 0x18);
            dVar18 = in_xd0;
          }
          sVar3 = *psVar14;
          sVar1 = psVar14[1];
          if (in_FPSCR_SZ == '\0') {
            *(int *)((int)unaff_r8 + 0xc) = (int)((ulonglong)param_2 >> 0x20);
            unaff_r8 = unaff_r8 + 1;
            *(int *)unaff_r8 = SUB84(dVar25,0);
          }
          else {
            unaff_r8[1] = param_2;
            *unaff_r8 = in_xd4;
          }
          fVar23 = (float)((ulonglong)dVar18 >> 0x20);
          dVar18 = (double)CONCAT44(fVar23,(float)(int)sVar1);
          if (in_FPSCR_SZ == '\0') {
            pdVar7 = (double *)((int)unaff_r8 + -4);
            *(int *)pdVar7 = (int)((ulonglong)dVar25 >> 0x20);
          }
          else {
            pdVar7 = unaff_r8 + -1;
            *pdVar7 = dVar25;
          }
          if (in_FPSCR_PR == '\0') {
            dVar18 = (double)CONCAT44(fVar23,(float)(int)sVar1 * fVar23);
          }
          else {
            dVar18 = dVar18 * dVar18;
          }
          *(undefined4 *)((int)pdVar7 + -4) = uVar4;
          if (in_FPSCR_PR == '\0') {
            fVar23 = (float)(int)sVar3;
          }
          else {
            fVar23 = (float)((ulonglong)(double)(int)sVar3 >> 0x20);
          }
          if (in_FPSCR_PR == '\0') {
            dVar20 = (double)CONCAT44(fVar23 * (float)((ulonglong)dVar18 >> 0x20),0x3f800000);
          }
          else {
            dVar20 = (double)CONCAT44(fVar23,0x3f800000) * dVar18;
          }
          if (in_FPSCR_SZ == '\0') {
            *(int *)((int)pdVar7 + 0x14) = SUB84(dVar20,0);
            *(int *)(pdVar7 + 2) = SUB84(dVar18,0);
            pdVar7 = (double *)((int)pdVar7 + 0xc);
            *(int *)pdVar7 = (int)((ulonglong)dVar20 >> 0x20);
          }
          else {
            pdVar7[2] = in_xd2;
            pdVar7[1] = in_xd0;
            *pdVar7 = dVar20;
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
          bVar29 = fVar23 < 0.0;
        }
        else {
          bVar29 = dVar25 < (double)((ulonglong)uVar22 << 0x20);
        }
        psVar16 = psVar14 + 2;
        if (bVar29) {
          psVar15 = psVar14 + 2;
          if (in_FPSCR_SZ == '\0') {
            dVar25 = (double)CONCAT44(*(undefined4 *)(iVar6 + 0x10),SUB84(dVar25,0));
            param_2 = (double)CONCAT44(*(undefined4 *)(iVar6 + 0x14),SUB84(param_2,0));
            dVar18 = (double)((longlong)dVar18 << 0x20);
          }
          else {
            dVar25 = *(double *)(iVar6 + 0x10);
            param_2 = *(double *)(iVar6 + 0x18);
            dVar18 = in_xd0;
          }
          sVar3 = *psVar14;
          sVar1 = psVar14[1];
          if (in_FPSCR_SZ == '\0') {
            *(int *)((int)unaff_r8 + 0xc) = (int)((ulonglong)param_2 >> 0x20);
            unaff_r8 = unaff_r8 + 1;
            *(int *)unaff_r8 = SUB84(dVar25,0);
          }
          else {
            unaff_r8[1] = param_2;
            *unaff_r8 = in_xd4;
          }
          fVar23 = (float)((ulonglong)dVar18 >> 0x20);
          dVar18 = (double)CONCAT44(fVar23,(float)(int)sVar1);
          if (in_FPSCR_SZ == '\0') {
            pdVar7 = (double *)((int)unaff_r8 + -4);
            *(int *)pdVar7 = (int)((ulonglong)dVar25 >> 0x20);
          }
          else {
            pdVar7 = unaff_r8 + -1;
            *pdVar7 = dVar25;
          }
          if (in_FPSCR_PR == '\0') {
            dVar18 = (double)CONCAT44(fVar23,(float)(int)sVar1 * fVar23);
          }
          else {
            dVar18 = dVar18 * dVar18;
          }
          *(undefined4 *)((int)pdVar7 + -4) = uVar27;
          if (in_FPSCR_PR == '\0') {
            fVar23 = (float)(int)sVar3;
          }
          else {
            fVar23 = (float)((ulonglong)(double)(int)sVar3 >> 0x20);
          }
          if (in_FPSCR_PR == '\0') {
            dVar20 = (double)CONCAT44(fVar23 * (float)((ulonglong)dVar18 >> 0x20),0x3f800000);
          }
          else {
            dVar20 = (double)CONCAT44(fVar23,0x3f800000) * dVar18;
          }
          if (in_FPSCR_SZ == '\0') {
            *(int *)((int)pdVar7 + 0x14) = SUB84(dVar20,0);
            *(int *)(pdVar7 + 2) = SUB84(dVar18,0);
            pdVar7 = (double *)((int)pdVar7 + 0xc);
            *(int *)pdVar7 = (int)((ulonglong)dVar20 >> 0x20);
          }
          else {
            pdVar7[2] = in_xd2;
            pdVar7[1] = in_xd0;
            *pdVar7 = dVar20;
          }
          unaff_r8 = pdVar7 + 2;
          goto LAB_8c614494;
        }
      }
      if (in_FPSCR_SZ == '\0') {
        in_dr14 = (double)CONCAT44(fVar24,SUB84(dVar25,0));
      }
      iVar5 = *(int *)(puVar17 + -0x18);
      if (in_FPSCR_SZ == '\0') {
        uVar28 = CONCAT44(*(undefined4 *)(iVar10 + 0x10),fVar19);
        param_1 = (double)CONCAT44((int)((ulonglong)in_dr14 >> 0x20),in_fr13);
        in_fr13 = *(float *)(iVar10 + 0x14);
        in_xd12 = dVar21;
      }
      else {
        uVar28 = *(undefined8 *)(iVar10 + 0x10);
        in_xd12 = *(double *)(iVar10 + 0x14);
        param_1 = in_dr14;
        in_xd4 = dVar21;
      }
      in_dr14 = (double)CONCAT44((int)((ulonglong)uVar28 >> 0x20),-(float)uVar28);
      *(int *)(puVar17 + -0x18) = iVar5 + -1;
      iVar6 = iVar9;
      iVar9 = iVar10;
      psVar14 = psVar16;
      dVar21 = in_xd12;
    } while (iVar5 + -1 != 0);
    psVar14 = psVar16 + 4;
    if (pdVar8 < unaff_r8) {
      psVar14 = psVar16 + 2;
      if (in_FPSCR_SZ == '\0') {
        param_1 = (double)CONCAT44(*(undefined4 *)(iVar10 + 0x10),SUB84(param_1,0));
        param_2 = (double)CONCAT44(*(undefined4 *)(iVar10 + 0x14),SUB84(param_2,0));
        dVar18 = (double)((longlong)dVar18 << 0x20);
      }
      else {
        param_1 = *(double *)(iVar10 + 0x10);
        param_2 = *(double *)(iVar10 + 0x18);
        dVar18 = in_xd0;
      }
      sVar3 = *psVar16;
      sVar1 = psVar16[1];
      if (in_FPSCR_SZ == '\0') {
        *(int *)((int)unaff_r8 + 0xc) = (int)((ulonglong)param_2 >> 0x20);
        unaff_r8 = unaff_r8 + 1;
        *(int *)unaff_r8 = SUB84(param_1,0);
      }
      else {
        unaff_r8[1] = param_2;
        *unaff_r8 = in_xd4;
      }
      fVar24 = (float)((ulonglong)dVar18 >> 0x20);
      dVar18 = (double)CONCAT44(fVar24,(float)(int)sVar1);
      if (in_FPSCR_SZ == '\0') {
        pdVar7 = (double *)((int)unaff_r8 + -4);
        *(int *)pdVar7 = (int)((ulonglong)param_1 >> 0x20);
      }
      else {
        pdVar7 = unaff_r8 + -1;
        *pdVar7 = param_1;
      }
      if (in_FPSCR_PR == '\0') {
        dVar18 = (double)CONCAT44(fVar24,(float)(int)sVar1 * fVar24);
      }
      else {
        dVar18 = dVar18 * dVar18;
      }
      *(undefined4 *)((int)pdVar7 + -4) = uVar4;
      if (in_FPSCR_PR == '\0') {
        fVar24 = (float)(int)sVar3;
      }
      else {
        fVar24 = (float)((ulonglong)(double)(int)sVar3 >> 0x20);
      }
      if (in_FPSCR_PR == '\0') {
        dVar21 = (double)CONCAT44(fVar24 * (float)((ulonglong)dVar18 >> 0x20),0x3f800000);
      }
      else {
        dVar21 = (double)CONCAT44(fVar24,0x3f800000) * dVar18;
      }
      if (in_FPSCR_SZ == '\0') {
        *(int *)((int)pdVar7 + 0x14) = SUB84(dVar21,0);
        *(int *)(pdVar7 + 2) = SUB84(dVar18,0);
        pdVar7 = (double *)((int)pdVar7 + 0xc);
        *(int *)pdVar7 = (int)((ulonglong)dVar21 >> 0x20);
      }
      else {
        pdVar7[2] = in_xd2;
        pdVar7[1] = in_xd0;
        *pdVar7 = dVar21;
      }
      unaff_r8 = pdVar7 + 2;
      *(int *)(puVar17 + -0x10) = (int)unaff_r8 + *(int *)(puVar17 + -0x10) + (0x20 - (int)pdVar8);
    }
    iVar6 = *(int *)(puVar17 + -8);
    *(int *)(puVar17 + -8) = iVar6 + -1;
    if (iVar6 + -1 == 0) {
      *DAT_8c6145bc = *DAT_8c6145bc + (*(uint *)(puVar17 + -0x10) >> 5);
      *DAT_8c6145c0 = *DAT_8c6145c0 + *(int *)(puVar17 + -0xc);
      return;
    }
  } while( true );
}

