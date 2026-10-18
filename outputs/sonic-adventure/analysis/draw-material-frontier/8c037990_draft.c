/* Ghidra draft only; not buildable/verified source. */

/* WARNING: Removing unreachable block (ram,0x8c0379e4) */
/* WARNING: Removing unreachable block (ram,0x8c037a20) */
/* WARNING: Restarted to delay deadcode elimination for space: register */

double * FUN_8c037990(double param_1,double param_2,undefined4 param_3,float param_4,int param_5,
                     uint param_6)

{
  ushort uVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  int iVar5;
  double *pdVar6;
  double *pdVar7;
  int iVar8;
  short *psVar9;
  short *psVar10;
  int in_r2;
  double *pdVar11;
  ushort *puVar12;
  double *unaff_r14;
  float fVar13;
  double dVar15;
  float fVar16;
  double dVar17;
  float fVar19;
  double dVar18;
  float fVar20;
  longlong lVar21;
  undefined4 uVar23;
  double dVar22;
  undefined4 uVar24;
  double in_xd0;
  double in_xd2;
  double in_xd4;
  float in_xf6;
  float in_xf7;
  float in_xf8;
  float in_xf9;
  undefined8 in_xd10;
  float in_xf12;
  float in_xf13;
  float in_xf14;
  float in_xf15;
  int in_GBR;
  char in_FPSCR_PR;
  char in_FPSCR_SZ;
  float fVar14;
  
  if ((param_6 & DAT_8c0379b8) == 0) {
    puVar12 = *(ushort **)(in_r2 + 4);
    iVar2 = *(int *)(in_GBR + 0x3c);
    *(int *)(in_GBR + 0x44) = (int)*(short *)(in_r2 + 2);
    psVar9 = *(short **)(in_r2 + 0x14);
    if (in_FPSCR_SZ == '\0') {
      param_4 = *(float *)PTR_DAT_8c0379bc;
    }
    iVar3 = *(int *)(in_GBR + 0x38);
    do {
      uVar24 = DAT_8c037a54;
      iVar4 = *(int *)(in_GBR + 0x54);
      if (iVar4 == 0) {
        uVar1 = *puVar12;
        iVar4 = (int)(short)uVar1;
        puVar12 = puVar12 + 1;
        if (iVar4 < 0) {
          iVar4 = (int)(short)(uVar1 & DAT_8c037a52);
        }
      }
      uVar1 = *puVar12;
      puVar12 = puVar12 + 1;
      iVar5 = (short)uVar1 * 4;
      if (in_FPSCR_SZ == '\0') {
        lVar21 = (ulonglong)*(uint *)(iVar5 + iVar3) << 0x20;
      }
      else {
        lVar21 = *(longlong *)(iVar5 + iVar3);
      }
      pdVar6 = (double *)((short)uVar1 * 0x10 + iVar2);
      if (in_FPSCR_SZ == '\0') {
        dVar15 = (double)CONCAT44(*(undefined4 *)pdVar6,*(undefined4 *)((int)pdVar6 + 4));
        dVar17 = (double)CONCAT44(*(undefined4 *)(pdVar6 + 1),*(undefined4 *)((int)pdVar6 + 0xc));
      }
      else {
        dVar15 = *pdVar6;
        in_xd0 = pdVar6[1];
        dVar17 = pdVar6[2];
        in_xd2 = pdVar6[3];
      }
      iVar4 = iVar4 + -1;
      do {
        psVar10 = psVar9;
        uVar23 = (undefined4)((ulonglong)lVar21 >> 0x20);
        if (in_FPSCR_PR == '\0') {
          dVar22 = (double)CONCAT44(uVar23,(float)(int)*psVar10 * param_4);
          param_2 = (double)CONCAT44((float)(int)psVar10[1] * param_4,SUB84(param_2,0));
        }
        else {
          dVar22 = (double)CONCAT44(uVar23,(float)(int)*psVar10) * (double)CONCAT44(param_3,param_4)
          ;
          param_2 = (double)(int)psVar10[1] * (double)CONCAT44(param_3,param_4);
        }
        if (in_FPSCR_SZ == '\0') {
          *(int *)((int)unaff_r14 + 0x1c) = (int)((ulonglong)dVar22 >> 0x20);
          *(int *)(unaff_r14 + 3) = SUB84(dVar17,0);
          *(int *)((int)unaff_r14 + 0x14) = (int)((ulonglong)param_2 >> 0x20);
          *(int *)(unaff_r14 + 2) = SUB84(dVar22,0);
          *(int *)((int)unaff_r14 + 0xc) = (int)((ulonglong)dVar17 >> 0x20);
          *(int *)(unaff_r14 + 1) = SUB84(dVar15,0);
          pdVar6 = (double *)((int)unaff_r14 + 4);
          *(int *)pdVar6 = (int)((ulonglong)dVar15 >> 0x20);
        }
        else {
          unaff_r14[3] = dVar22;
          unaff_r14[2] = in_xd2;
          unaff_r14[1] = param_2;
          *unaff_r14 = in_xd4;
          unaff_r14[-1] = dVar17;
          unaff_r14[-2] = in_xd0;
          pdVar6 = unaff_r14 + -3;
          *pdVar6 = dVar15;
        }
        *(undefined4 *)((int)pdVar6 + -4) = uVar24;
        uVar23 = DAT_8c037a58;
        unaff_r14 = (double *)((int)pdVar6 + 0x1c);
        uVar1 = *puVar12;
        puVar12 = puVar12 + 1;
        iVar5 = (short)uVar1 * 4;
        if (in_FPSCR_SZ == '\0') {
          lVar21 = (ulonglong)*(uint *)(iVar5 + iVar3) << 0x20;
        }
        else {
          lVar21 = *(longlong *)(iVar5 + iVar3);
        }
        pdVar7 = (double *)((short)uVar1 * 0x10 + iVar2);
        if (in_FPSCR_SZ == '\0') {
          dVar15 = (double)CONCAT44(*(undefined4 *)pdVar7,*(undefined4 *)((int)pdVar7 + 4));
          dVar17 = (double)CONCAT44(*(undefined4 *)(pdVar7 + 1),*(undefined4 *)((int)pdVar7 + 0xc));
        }
        else {
          dVar15 = *pdVar7;
          in_xd0 = pdVar7[1];
          dVar17 = pdVar7[2];
          in_xd2 = pdVar7[3];
        }
        iVar4 = iVar4 + -1;
        psVar9 = psVar10 + 2;
      } while (iVar4 != 0);
      uVar24 = (undefined4)((ulonglong)lVar21 >> 0x20);
      fVar20 = (float)(int)psVar10[2];
      psVar9 = psVar10 + 4;
      if (in_FPSCR_PR == '\0') {
        dVar22 = (double)CONCAT44(uVar24,fVar20 * param_4);
        param_2 = (double)CONCAT44((float)(int)psVar10[3] * param_4,SUB84(param_2,0));
      }
      else {
        dVar22 = (double)CONCAT44(uVar24,fVar20) * (double)CONCAT44(param_3,param_4);
        param_2 = (double)(int)psVar10[3] * (double)CONCAT44(param_3,param_4);
      }
      if (in_FPSCR_SZ == '\0') {
        *(int *)(pdVar6 + 7) = (int)((ulonglong)dVar22 >> 0x20);
        *(int *)((int)pdVar6 + 0x34) = SUB84(dVar17,0);
        *(int *)(pdVar6 + 6) = (int)((ulonglong)param_2 >> 0x20);
        *(int *)((int)pdVar6 + 0x2c) = SUB84(dVar22,0);
        *(int *)(pdVar6 + 5) = (int)((ulonglong)dVar17 >> 0x20);
        *(int *)((int)pdVar6 + 0x24) = SUB84(dVar15,0);
        pdVar6 = pdVar6 + 4;
        *(int *)pdVar6 = (int)((ulonglong)dVar15 >> 0x20);
      }
      else {
        *(double *)((int)pdVar6 + 0x34) = dVar22;
        *(double *)((int)pdVar6 + 0x2c) = in_xd2;
        *(double *)((int)pdVar6 + 0x24) = param_2;
        *(double *)((int)pdVar6 + 0x1c) = in_xd4;
        *(double *)((int)pdVar6 + 0x14) = dVar17;
        *(double *)((int)pdVar6 + 0xc) = in_xd0;
        pdVar6 = (double *)((int)pdVar6 + 4);
        *pdVar6 = dVar15;
      }
      *(undefined4 *)((int)pdVar6 + -4) = uVar23;
      unaff_r14 = (double *)((int)pdVar6 + 0x1c);
      iVar4 = *(int *)(in_GBR + 0x44) + -1;
      *(int *)(in_GBR + 0x44) = iVar4;
    } while (iVar4 != 0);
    return unaff_r14;
  }
  puVar12 = *(ushort **)(in_r2 + 4);
  iVar2 = *(int *)(in_GBR + 0x3c);
  *(int *)(in_GBR + 0x44) = (int)*(short *)(in_r2 + 2);
  iVar4 = *(int *)(param_5 + 4);
  iVar3 = *(int *)(in_GBR + 0x38);
  do {
    uVar24 = DAT_8c037b30;
    iVar5 = *(int *)(in_GBR + 0x54);
    if (iVar5 == 0) {
      uVar1 = *puVar12;
      iVar5 = (int)(short)uVar1;
      puVar12 = puVar12 + 1;
      if (iVar5 < 0) {
        iVar5 = (int)(short)(uVar1 & DAT_8c037b2e);
      }
    }
    iVar8 = (int)(short)*puVar12;
    puVar12 = puVar12 + 1;
    if (in_FPSCR_SZ == '\0') {
      param_1 = (double)CONCAT44(*(undefined4 *)(iVar8 * 4 + iVar3),SUB84(param_1,0));
    }
    else {
      param_1 = *(double *)(iVar8 * 4 + iVar3);
    }
    pdVar6 = (double *)(iVar8 * 0x10 + iVar2);
    pdVar7 = (double *)(iVar8 * 0xc + iVar4);
    if (in_FPSCR_SZ == '\0') {
      dVar15 = (double)CONCAT44(*(undefined4 *)pdVar7,*(undefined4 *)((int)pdVar7 + 4));
      dVar17 = (double)((ulonglong)*(uint *)(pdVar7 + 1) << 0x20);
    }
    else {
      dVar15 = *pdVar7;
      in_xd0 = pdVar7[1];
      dVar17 = pdVar7[2];
    }
    dVar17 = (double)((ulonglong)dVar17 & 0xffffffff00000000);
                    /* WARNING: Ignoring partial resolution of indirect */
    fVar20 = (float)((ulonglong)in_xd10 >> 0x20);
    if (in_FPSCR_PR == '\0') {
      fVar14 = SUB84(dVar15,0);
      fVar16 = (float)((ulonglong)dVar15 >> 0x20);
      fVar19 = (float)((ulonglong)dVar17 >> 0x20);
      fVar13 = SUB84(in_xd0,0) * fVar14 + SUB84(in_xd4,0) * fVar16 + in_xf8 * 0.0 + in_xf12 * fVar19
      ;
      fVar13 = (float)((ulonglong)in_xd0 >> 0x20) * fVar14 +
               (float)((ulonglong)in_xd4 >> 0x20) * fVar16 + in_xf9 * 0.0 + in_xf13 * fVar19;
      fVar13 = SUB84(in_xd2,0) * fVar14 + in_xf6 * fVar16 + (float)in_xd10 * 0.0 + in_xf14 * fVar19;
      fVar13 = (float)((ulonglong)in_xd2 >> 0x20) * fVar14 + in_xf7 * fVar16 + fVar20 * 0.0 +
               in_xf15 * fVar19;
    }
    if (in_FPSCR_SZ == '\0') {
      dVar17 = (double)CONCAT44((int)((ulonglong)dVar17 >> 0x20),*(undefined4 *)DAT_8c037b34);
    }
    else {
      in_xd2 = *DAT_8c037b34;
    }
    fVar13 = SUB84(dVar17,0);
    if (in_FPSCR_PR == '\0') {
      dVar17 = (double)CONCAT44((float)((ulonglong)dVar15 >> 0x20) * fVar13 + fVar13,
                                SUB84(dVar15,0) * fVar13 + fVar13);
    }
    else {
      dVar17 = (dVar15 * dVar17 + dVar17) * dVar17 + dVar17;
    }
    if (in_FPSCR_SZ == '\0') {
      param_1 = (double)CONCAT44((int)((ulonglong)param_1 >> 0x20),(int)((ulonglong)dVar17 >> 0x20))
      ;
      dVar15 = (double)CONCAT44(*(undefined4 *)pdVar6,*(undefined4 *)((int)pdVar6 + 4));
      dVar18 = (double)CONCAT44(*(undefined4 *)(pdVar6 + 1),*(undefined4 *)((int)pdVar6 + 0xc));
      dVar22 = in_xd0;
      param_2 = (double)CONCAT44(SUB84(dVar17,0),SUB84(param_2,0));
    }
    else {
      dVar15 = *pdVar6;
      dVar18 = pdVar6[2];
      in_xd2 = pdVar6[3];
      dVar22 = pdVar6[1];
      param_2 = in_xd0;
      in_xd4 = dVar17;
    }
    in_xd0 = dVar22;
    iVar5 = iVar5 + -1;
    do {
      if (in_FPSCR_SZ == '\0') {
        *(int *)((int)unaff_r14 + 0x1c) = (int)((ulonglong)param_1 >> 0x20);
        *(int *)(unaff_r14 + 3) = SUB84(dVar18,0);
        *(int *)((int)unaff_r14 + 0x14) = (int)((ulonglong)param_2 >> 0x20);
        *(int *)(unaff_r14 + 2) = SUB84(param_1,0);
        *(int *)((int)unaff_r14 + 0xc) = (int)((ulonglong)dVar18 >> 0x20);
        *(int *)(unaff_r14 + 1) = SUB84(dVar15,0);
        pdVar6 = (double *)((int)unaff_r14 + 4);
        *(int *)pdVar6 = (int)((ulonglong)dVar15 >> 0x20);
      }
      else {
        unaff_r14[3] = param_1;
        unaff_r14[2] = in_xd2;
        unaff_r14[1] = param_2;
        *unaff_r14 = in_xd4;
        unaff_r14[-1] = dVar18;
        unaff_r14[-2] = in_xd0;
        pdVar6 = unaff_r14 + -3;
        *pdVar6 = dVar15;
      }
      *(undefined4 *)((int)pdVar6 + -4) = uVar24;
      uVar23 = DAT_8c037b38;
      unaff_r14 = (double *)((int)pdVar6 + 0x1c);
      iVar8 = (int)(short)*puVar12;
      puVar12 = puVar12 + 1;
      if (in_FPSCR_SZ == '\0') {
        param_1 = (double)CONCAT44(*(undefined4 *)(iVar8 * 4 + iVar3),SUB84(param_1,0));
      }
      else {
        param_1 = *(double *)(iVar8 * 4 + iVar3);
      }
      pdVar7 = (double *)(iVar8 * 0x10 + iVar2);
      pdVar11 = (double *)(iVar8 * 0xc + iVar4);
      if (in_FPSCR_SZ == '\0') {
        dVar15 = (double)CONCAT44(*(undefined4 *)pdVar11,*(undefined4 *)((int)pdVar11 + 4));
        dVar17 = (double)((ulonglong)*(uint *)(pdVar11 + 1) << 0x20);
      }
      else {
        dVar15 = *pdVar11;
        in_xd0 = pdVar11[1];
        dVar17 = pdVar11[2];
      }
      dVar17 = (double)((ulonglong)dVar17 & 0xffffffff00000000);
                    /* WARNING: Ignoring partial resolution of indirect */
      if (in_FPSCR_PR == '\0') {
        fVar14 = SUB84(dVar15,0);
        fVar16 = (float)((ulonglong)dVar15 >> 0x20);
        fVar19 = (float)((ulonglong)dVar17 >> 0x20);
        fVar13 = SUB84(in_xd0,0) * fVar14 + SUB84(in_xd4,0) * fVar16 + in_xf8 * 0.0 +
                 in_xf12 * fVar19;
        fVar13 = (float)((ulonglong)in_xd0 >> 0x20) * fVar14 +
                 (float)((ulonglong)in_xd4 >> 0x20) * fVar16 + in_xf9 * 0.0 + in_xf13 * fVar19;
        fVar13 = SUB84(in_xd2,0) * fVar14 + in_xf6 * fVar16 + (float)in_xd10 * 0.0 +
                 in_xf14 * fVar19;
        fVar13 = (float)((ulonglong)in_xd2 >> 0x20) * fVar14 + in_xf7 * fVar16 + fVar20 * 0.0 +
                 in_xf15 * fVar19;
      }
      if (in_FPSCR_SZ == '\0') {
        dVar17 = (double)CONCAT44((int)((ulonglong)dVar17 >> 0x20),*(undefined4 *)DAT_8c037b34);
      }
      else {
        in_xd2 = *DAT_8c037b34;
      }
      fVar13 = SUB84(dVar17,0);
      if (in_FPSCR_PR == '\0') {
        dVar17 = (double)CONCAT44((float)((ulonglong)dVar15 >> 0x20) * fVar13 + fVar13,
                                  SUB84(dVar15,0) * fVar13 + fVar13);
      }
      else {
        dVar17 = (dVar15 * dVar17 + dVar17) * dVar17 + dVar17;
      }
      if (in_FPSCR_SZ == '\0') {
        param_1 = (double)CONCAT44((int)((ulonglong)param_1 >> 0x20),
                                   (int)((ulonglong)dVar17 >> 0x20));
        dVar15 = (double)CONCAT44(*(undefined4 *)pdVar7,*(undefined4 *)((int)pdVar7 + 4));
        dVar18 = (double)CONCAT44(*(undefined4 *)(pdVar7 + 1),*(undefined4 *)((int)pdVar7 + 0xc));
        dVar22 = in_xd0;
        param_2 = (double)CONCAT44(SUB84(dVar17,0),SUB84(param_2,0));
      }
      else {
        dVar15 = *pdVar7;
        dVar18 = pdVar7[2];
        in_xd2 = pdVar7[3];
        dVar22 = pdVar7[1];
        param_2 = in_xd0;
        in_xd4 = dVar17;
      }
      in_xd0 = dVar22;
      iVar5 = iVar5 + -1;
    } while (iVar5 != 0);
    if (in_FPSCR_SZ == '\0') {
      *(int *)(pdVar6 + 7) = (int)((ulonglong)param_1 >> 0x20);
      *(int *)((int)pdVar6 + 0x34) = SUB84(dVar18,0);
      *(int *)(pdVar6 + 6) = (int)((ulonglong)param_2 >> 0x20);
      *(int *)((int)pdVar6 + 0x2c) = SUB84(param_1,0);
      *(int *)(pdVar6 + 5) = (int)((ulonglong)dVar18 >> 0x20);
      *(int *)((int)pdVar6 + 0x24) = SUB84(dVar15,0);
      pdVar6 = pdVar6 + 4;
      *(int *)pdVar6 = (int)((ulonglong)dVar15 >> 0x20);
    }
    else {
      *(double *)((int)pdVar6 + 0x34) = param_1;
      *(double *)((int)pdVar6 + 0x2c) = in_xd2;
      *(double *)((int)pdVar6 + 0x24) = param_2;
      *(double *)((int)pdVar6 + 0x1c) = in_xd4;
      *(double *)((int)pdVar6 + 0x14) = dVar18;
      *(double *)((int)pdVar6 + 0xc) = in_xd0;
      pdVar6 = (double *)((int)pdVar6 + 4);
      *pdVar6 = dVar15;
    }
    *(undefined4 *)((int)pdVar6 + -4) = uVar23;
    unaff_r14 = (double *)((int)pdVar6 + 0x1c);
    iVar5 = *(int *)(in_GBR + 0x44) + -1;
    *(int *)(in_GBR + 0x44) = iVar5;
  } while (iVar5 != 0);
  return unaff_r14;
}

