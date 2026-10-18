/* Ghidra draft only; not buildable/verified source. */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_8c61db98(double param_1,undefined8 param_2,double param_3,undefined8 param_4,int param_5)

{
  ushort uVar1;
  float fVar2;
  int *piVar3;
  int *piVar4;
  undefined4 uVar5;
  uint uVar6;
  double *pdVar7;
  ulonglong *puVar8;
  int iVar9;
  int iVar10;
  int unaff_r8;
  double *pdVar11;
  int iVar12;
  int iVar13;
  ushort *puVar14;
  ushort *puVar15;
  ushort *puVar16;
  ushort *puVar17;
  undefined8 *puVar18;
  double *pdVar19;
  double *pdVar20;
  double *pdVar21;
  undefined8 *puVar22;
  double in_dr0;
  double dVar23;
  undefined8 uVar24;
  undefined4 uVar25;
  float fVar26;
  double in_dr2;
  double dVar27;
  double dVar28;
  ulonglong uVar29;
  undefined4 uVar30;
  uint uVar31;
  undefined4 uVar32;
  double dVar33;
  float fVar34;
  double dVar35;
  undefined8 in_dr12;
  undefined8 in_dr14;
  undefined8 in_xd0;
  double in_xd2;
  double in_xd4;
  undefined8 in_xd6;
  undefined8 in_xd8;
  undefined8 in_xd10;
  undefined8 in_xd12;
  double in_xd14;
  bool bVar36;
  undefined4 in_PR;
  char in_FPSCR_PR;
  char in_FPSCR_SZ;
  undefined8 local_20;
  
  piVar3 = _DAT_8c61dc10;
  if (in_FPSCR_SZ == '\0') {
    puVar22 = (undefined8 *)((int)&local_20 + 4);
    local_20 = (double)CONCAT44((int)in_dr14,(undefined4)local_20);
  }
  else {
    puVar22 = &local_20;
    local_20 = in_xd14;
  }
  *(undefined4 *)((int)puVar22 + -4) = in_PR;
  *(int *)((int)puVar22 + -0x14) = param_5;
  if (in_FPSCR_SZ == '\0') {
    in_dr14 = CONCAT44((int)((ulonglong)in_dr14 >> 0x20),DAT_8c61dc0c);
  }
  else {
    in_xd14 = (double)CONCAT44(_DAT_8c61dc10,DAT_8c61dc0c);
  }
  puVar14 = *(ushort **)(param_5 + 4);
  uVar6 = **(ushort **)((int)puVar22 + -0x14) & DAT_8c61dc14;
  if (uVar6 == 0) {
    *_DAT_8c61dc10 = *_DAT_8c61dc10 + (uint)*(ushort *)(*(int *)((int)puVar22 + -0x14) + 2) * 3;
    for (iVar13 = 0; uVar32 = SUB84(param_1,0),
        iVar13 < (int)(uint)*(ushort *)(*(int *)((int)puVar22 + -0x14) + 2); iVar13 = iVar13 + 1) {
      uVar1 = *puVar14;
      puVar17 = puVar14 + 1;
      puVar15 = puVar14 + 2;
      iVar10 = *DAT_8c61de58;
      puVar14 = puVar14 + 3;
      pdVar20 = (double *)((uint)*puVar17 * 0x20 + iVar10);
      if (in_FPSCR_SZ == '\0') {
        uVar32 = *(undefined4 *)(pdVar20 + 2);
      }
      else {
        in_xd4 = pdVar20[2];
      }
      pdVar21 = (double *)((uint)uVar1 * 0x20 + iVar10);
      if (in_FPSCR_SZ == '\0') {
        dVar28 = (double)CONCAT44(uVar32,*(undefined4 *)(pdVar21 + 2));
      }
      else {
        in_xd2 = pdVar21[2];
        dVar28 = in_xd4;
      }
      if (in_FPSCR_PR == '\0') {
        dVar28 = (double)CONCAT44((float)((ulonglong)dVar28 >> 0x20) - SUB84(dVar28,0),
                                  SUB84(dVar28,0));
      }
      else {
        dVar28 = dVar28 - dVar28;
      }
      pdVar19 = (double *)((uint)*puVar15 * 0x20 + iVar10);
      if (in_FPSCR_SZ == '\0') {
        fVar26 = *(float *)((int)pdVar20 + 0x14);
        dVar33 = (double)CONCAT44(fVar26,uVar32);
        dVar28 = (double)CONCAT44((int)((ulonglong)dVar28 >> 0x20),
                                  *(undefined4 *)((int)pdVar19 + 0x14));
        in_dr0 = (double)CONCAT44((int)((ulonglong)in_dr0 >> 0x20),fVar26);
      }
      else {
        dVar33 = *(double *)((int)pdVar20 + 0x14);
        in_xd2 = *(double *)((int)pdVar19 + 0x14);
        fVar26 = (float)((ulonglong)dVar33 >> 0x20);
      }
      if (in_FPSCR_PR == '\0') {
        fVar26 = SUB84(dVar28,0) - fVar26;
        param_1 = (double)CONCAT44((float)((ulonglong)dVar28 >> 0x20) * fVar26,fVar26);
      }
      else {
        param_1 = (dVar28 - dVar33) * (dVar28 - dVar33);
      }
      if (in_FPSCR_SZ == '\0') {
        param_1 = (double)CONCAT44((int)((ulonglong)param_1 >> 0x20),
                                   *(undefined4 *)((int)pdVar21 + 0x14));
      }
      else {
        in_xd2 = *(double *)((int)pdVar21 + 0x14);
      }
      if (in_FPSCR_PR == '\0') {
        dVar28 = (double)CONCAT44((int)((ulonglong)in_dr0 >> 0x20),
                                  SUB84(in_dr0,0) - SUB84(param_1,0));
      }
      else {
        dVar28 = in_dr0 - param_1;
      }
      uVar32 = (undefined4)((ulonglong)param_1 >> 0x20);
      if (in_FPSCR_SZ == '\0') {
        param_1 = (double)CONCAT44(uVar32,SUB84(dVar33,0));
        dVar33 = (double)CONCAT44(uVar32,*(undefined4 *)(pdVar19 + 2));
      }
      else {
        in_xd2 = pdVar19[2];
        dVar33 = param_1;
      }
      if (in_FPSCR_PR == '\0') {
        fVar26 = SUB84(dVar33,0) - SUB84(param_1,0);
        dVar33 = (double)CONCAT44(uVar32,fVar26);
        in_dr0 = (double)CONCAT44((int)((ulonglong)dVar28 >> 0x20),SUB84(dVar28,0) * fVar26);
      }
      else {
        dVar33 = dVar33 - param_1;
        in_dr0 = dVar28 * dVar33;
      }
      if (in_FPSCR_PR == '\0') {
        fVar26 = (float)((ulonglong)param_1 >> 0x20) - SUB84(in_dr0,0);
        param_1 = (double)CONCAT44(fVar26,SUB84(param_1,0));
        bVar36 = 0.0 < fVar26;
      }
      else {
        param_1 = param_1 - in_dr0;
        bVar36 = (double)((ulonglong)dVar33 & 0xffffffff00000000) < param_1;
      }
      uVar32 = SUB84(param_1,0);
      if (!bVar36) {
        if (in_FPSCR_SZ == '\0') {
          dVar28 = (double)CONCAT44(*(undefined4 *)pdVar21,*(undefined4 *)pdVar20);
        }
        else {
          dVar28 = *pdVar21;
          in_xd2 = *pdVar20;
        }
        if (in_FPSCR_PR == '\0') {
          dVar28 = (double)CONCAT44((float)((ulonglong)dVar28 >> 0x20) + SUB84(dVar28,0),
                                    SUB84(dVar28,0));
        }
        else {
          dVar28 = dVar28 + dVar28;
        }
        if (in_FPSCR_SZ == '\0') {
          dVar28 = (double)CONCAT44((int)((ulonglong)dVar28 >> 0x20),*(undefined4 *)pdVar19);
        }
        else {
          in_xd2 = *pdVar19;
        }
        if (in_FPSCR_PR == '\0') {
          dVar28 = (double)CONCAT44((float)((ulonglong)dVar28 >> 0x20) + SUB84(dVar28,0),
                                    SUB84(dVar28,0));
        }
        else {
          dVar28 = dVar28 + dVar28;
        }
        if (in_FPSCR_SZ == '\0') {
          *(int *)((int)puVar22 + -0x10) = (int)((ulonglong)dVar28 >> 0x20);
          dVar28 = (double)CONCAT44(*(undefined4 *)((int)pdVar21 + 4),
                                    *(undefined4 *)((int)pdVar20 + 4));
        }
        else {
          *(double *)((int)puVar22 + -0x10) = dVar28;
          dVar28 = *(double *)((int)pdVar21 + 4);
          in_xd2 = *(double *)((int)pdVar20 + 4);
        }
        if (in_FPSCR_PR == '\0') {
          dVar28 = (double)CONCAT44((float)((ulonglong)dVar28 >> 0x20) + SUB84(dVar28,0),
                                    SUB84(dVar28,0));
        }
        else {
          dVar28 = dVar28 + dVar28;
        }
        if (in_FPSCR_SZ == '\0') {
          dVar28 = (double)CONCAT44((int)((ulonglong)dVar28 >> 0x20),
                                    *(undefined4 *)((int)pdVar19 + 4));
        }
        else {
          in_xd2 = *(double *)((int)pdVar19 + 4);
        }
        if (in_FPSCR_PR == '\0') {
          dVar28 = (double)CONCAT44((float)((ulonglong)dVar28 >> 0x20) + SUB84(dVar28,0),
                                    SUB84(dVar28,0));
        }
        else {
          dVar28 = dVar28 + dVar28;
        }
        if (in_FPSCR_SZ == '\0') {
          *(int *)((int)puVar22 + -0xc) = (int)((ulonglong)dVar28 >> 0x20);
          dVar28 = (double)CONCAT44(*(undefined4 *)(pdVar21 + 1),*(undefined4 *)(pdVar20 + 1));
        }
        else {
          *(double *)((int)puVar22 + -0xc) = dVar28;
          dVar28 = pdVar21[1];
          in_xd2 = pdVar20[1];
        }
        if (in_FPSCR_PR == '\0') {
          dVar28 = (double)CONCAT44((float)((ulonglong)dVar28 >> 0x20) + SUB84(dVar28,0),
                                    SUB84(dVar28,0));
        }
        else {
          dVar28 = dVar28 + dVar28;
        }
        if (in_FPSCR_SZ == '\0') {
          dVar28 = (double)CONCAT44((int)((ulonglong)dVar28 >> 0x20),*(undefined4 *)(pdVar19 + 1));
        }
        else {
          in_xd2 = pdVar19[1];
        }
        if (in_FPSCR_PR == '\0') {
          dVar28 = (double)CONCAT44((float)((ulonglong)dVar28 >> 0x20) + SUB84(dVar28,0),
                                    SUB84(dVar28,0));
        }
        else {
          dVar28 = dVar28 + dVar28;
        }
        if (in_FPSCR_SZ == '\0') {
          *(int *)((int)puVar22 + -8) = (int)((ulonglong)dVar28 >> 0x20);
        }
        else {
          *(double *)((int)puVar22 + -8) = dVar28;
        }
        dVar28 = (double)(*(code *)PTR_FUN_8c61de5c)((undefined1 *)((int)puVar22 + -0x10));
        piVar3 = DAT_8c61de6c;
        if (in_FPSCR_SZ == '\0') {
          dVar33 = (double)CONCAT44(*(undefined4 *)DAT_8c61de60,
                                    *(undefined4 *)((int)puVar22 + -0x10));
        }
        else {
          in_xd2 = *(double *)((int)puVar22 + -0x10);
          dVar33 = *DAT_8c61de60;
        }
        if (in_FPSCR_PR == '\0') {
          dVar33 = (double)CONCAT44((float)((ulonglong)dVar33 >> 0x20) * SUB84(dVar33,0),
                                    SUB84(dVar33,0));
        }
        else {
          dVar33 = dVar33 * dVar33;
        }
        if (in_FPSCR_SZ == '\0') {
          dVar33 = (double)CONCAT44((int)((ulonglong)dVar33 >> 0x20),*(undefined4 *)DAT_8c61de64);
          dVar28 = (double)CONCAT44((int)((ulonglong)dVar28 >> 0x20),*DAT_8c61de68);
        }
        else {
          in_xd2 = *DAT_8c61de64;
        }
        if (in_FPSCR_PR == '\0') {
          fVar26 = SUB84(dVar33,0) - (float)((ulonglong)dVar33 >> 0x20);
        }
        else {
          fVar26 = SUB84(dVar33 - dVar33,0);
        }
        if (in_FPSCR_SZ == '\0') {
          dVar33 = (double)CONCAT44(*(undefined4 *)((int)puVar22 + -0xc),fVar26);
        }
        else {
          dVar33 = *(double *)((int)puVar22 + -0xc);
        }
        if (in_FPSCR_PR == '\0') {
          dVar28 = (double)CONCAT44((int)((ulonglong)dVar28 >> 0x20),
                                    SUB84(dVar28,0) * (float)((ulonglong)dVar33 >> 0x20));
        }
        else {
          dVar28 = dVar28 * dVar33;
        }
        if (in_FPSCR_SZ == '\0') {
          dVar33 = (double)CONCAT44(*(undefined4 *)((int)puVar22 + -8),SUB84(dVar33,0));
        }
        else {
          dVar33 = *(double *)((int)puVar22 + -8);
        }
        iVar10 = *DAT_8c61de6c;
        if (in_FPSCR_PR == '\0') {
          dVar33 = (double)CONCAT44((int)((ulonglong)dVar33 >> 0x20),
                                    SUB84(dVar33,0) - SUB84(dVar28,0));
        }
        else {
          dVar33 = dVar33 - dVar28;
        }
        if (in_FPSCR_SZ == '\0') {
          dVar28 = (double)CONCAT44(*(undefined4 *)((int)pdVar21 + 4),
                                    *(undefined4 *)(DAT_8c61de60 + 1));
        }
        else {
          dVar28 = *(double *)((int)pdVar21 + 4);
        }
        fVar26 = (float)((ulonglong)dVar33 >> 0x20);
        if (in_FPSCR_PR == '\0') {
          dVar28 = (double)CONCAT44((int)((ulonglong)dVar28 >> 0x20),SUB84(dVar28,0) * fVar26);
        }
        else {
          dVar28 = dVar28 * dVar33;
        }
        if (in_FPSCR_SZ == '\0') {
          uVar25 = SUB84(dVar33,0);
          dVar33 = (double)CONCAT44(fVar26,*(undefined4 *)(iVar10 + 0x14));
          dVar23 = in_xd2;
          in_xd2 = (double)CONCAT44(uVar25,uVar32);
        }
        else {
          dVar23 = *(double *)(iVar10 + 0x14);
        }
        if (in_FPSCR_PR == '\0') {
          param_1 = (double)CONCAT44((float)((ulonglong)in_xd2 >> 0x20) - SUB84(dVar28,0),
                                     SUB84(in_xd2,0));
          fVar26 = SUB84(dVar33,0) * (float)((ulonglong)dVar28 >> 0x20);
        }
        else {
          param_1 = in_xd2 - dVar28;
          fVar26 = SUB84(dVar33 * dVar28,0);
        }
        if (in_FPSCR_SZ == '\0') {
          dVar28 = (double)CONCAT44(*(undefined4 *)pdVar21,SUB84(dVar28,0));
          uVar29 = CONCAT44(*(undefined4 *)(iVar10 + 4),fVar26);
        }
        else {
          dVar28 = *pdVar21;
          uVar29 = *(ulonglong *)(iVar10 + 4);
        }
        if (in_FPSCR_PR == '\0') {
          uVar29 = (ulonglong)
                   (uint)((float)((ulonglong)dVar28 >> 0x20) * (float)(uVar29 >> 0x20) +
                         (float)uVar29);
        }
        if (in_FPSCR_SZ == '\0') {
          dVar28 = (double)CONCAT44(*(undefined4 *)(pdVar21 + 1),SUB84(dVar28,0));
          uVar29 = CONCAT44(*(undefined4 *)(iVar10 + 0x24),(int)uVar29);
        }
        else {
          dVar28 = pdVar21[1];
          uVar29 = *(ulonglong *)(iVar10 + 0x24);
        }
        if (in_FPSCR_PR == '\0') {
          uVar29 = (ulonglong)
                   (uint)((float)((ulonglong)dVar28 >> 0x20) * (float)(uVar29 >> 0x20) +
                         (float)uVar29);
        }
        uVar32 = (undefined4)in_dr14;
        dVar35 = in_xd14;
        dVar33 = in_xd14;
        if (in_FPSCR_SZ == '\0') {
          dVar35 = (double)CONCAT44(uVar32,(int)uVar29);
          dVar33 = (double)CONCAT44(uVar32,SUB84(dVar28,0));
        }
        if (in_FPSCR_PR == '\0') {
          dVar35 = (double)CONCAT44((float)((ulonglong)dVar33 >> 0x20) * SUB84(dVar35,0) +
                                    (float)((ulonglong)dVar35 >> 0x20),SUB84(dVar35,0));
        }
        if (in_FPSCR_SZ == '\0') {
          pdVar11 = (double *)(unaff_r8 + 0x14);
          *(int *)pdVar11 = (int)((ulonglong)dVar35 >> 0x20);
        }
        else {
          pdVar11 = (double *)(unaff_r8 + 0x10);
          *pdVar11 = dVar35;
        }
        pdVar7 = (double *)*piVar3;
        if (in_FPSCR_SZ == '\0') {
          dVar28 = (double)CONCAT44(*(undefined4 *)((int)pdVar21 + 4),SUB84(dVar33,0));
          dVar33 = (double)CONCAT44(*(undefined4 *)pdVar7,*(undefined4 *)(pdVar7 + 2));
        }
        else {
          dVar28 = *(double *)((int)pdVar21 + 4);
          dVar23 = pdVar7[2];
          dVar33 = *pdVar7;
        }
        if (in_FPSCR_PR == '\0') {
          dVar33 = (double)CONCAT44((int)((ulonglong)dVar33 >> 0x20),
                                    SUB84(dVar33,0) * (float)((ulonglong)dVar28 >> 0x20));
        }
        else {
          dVar33 = dVar33 * dVar28;
        }
        if (in_FPSCR_SZ == '\0') {
          dVar28 = (double)CONCAT44(*(undefined4 *)pdVar21,SUB84(dVar28,0));
        }
        else {
          dVar28 = *pdVar21;
        }
        if (in_FPSCR_PR == '\0') {
          dVar33 = (double)(ulonglong)
                           (uint)((float)((ulonglong)dVar28 >> 0x20) *
                                  (float)((ulonglong)dVar33 >> 0x20) + SUB84(dVar33,0));
        }
        if (in_FPSCR_SZ == '\0') {
          dVar28 = (double)CONCAT44(*(undefined4 *)(pdVar21 + 1),SUB84(dVar28,0));
          dVar33 = (double)CONCAT44(*(undefined4 *)(pdVar7 + 4),SUB84(dVar33,0));
        }
        else {
          dVar28 = pdVar21[1];
          dVar33 = pdVar7[4];
        }
        if (in_FPSCR_PR == '\0') {
          dVar33 = (double)(ulonglong)
                           (uint)((float)((ulonglong)dVar28 >> 0x20) *
                                  (float)((ulonglong)dVar33 >> 0x20) + SUB84(dVar33,0));
        }
        dVar27 = in_xd14;
        dVar35 = in_xd14;
        if (in_FPSCR_SZ == '\0') {
          dVar27 = (double)CONCAT44(uVar32,SUB84(dVar33,0));
          dVar35 = (double)CONCAT44(uVar32,SUB84(dVar28,0));
        }
        if (in_FPSCR_PR == '\0') {
          dVar27 = (double)CONCAT44((float)((ulonglong)dVar35 >> 0x20) * SUB84(dVar27,0) +
                                    (float)((ulonglong)dVar27 >> 0x20),SUB84(dVar27,0));
        }
        uVar25 = (undefined4)((ulonglong)dVar27 >> 0x20);
        if (in_FPSCR_SZ == '\0') {
          *(undefined4 *)((int)pdVar11 + -4) = uVar25;
          uVar30 = DAT_8c61de70;
          *(undefined4 *)(pdVar11 + -1) = *(undefined4 *)(pdVar21 + 3);
          *(undefined4 *)((int)pdVar11 + -0xc) = *(undefined4 *)((int)pdVar21 + 0x14);
          dVar27 = (double)CONCAT44(uVar25,*(undefined4 *)(pdVar21 + 2));
          pdVar11 = pdVar11 + -2;
          *(undefined4 *)pdVar11 = *(undefined4 *)(pdVar21 + 2);
        }
        else {
          pdVar11[-1] = dVar27;
          uVar30 = DAT_8c61de70;
          pdVar11[-2] = pdVar21[3];
          pdVar11[-3] = *(double *)((int)pdVar21 + 0x14);
          dVar23 = pdVar21[2];
          pdVar11 = pdVar11 + -4;
          *pdVar11 = dVar23;
        }
        *(undefined4 *)((int)pdVar11 + -4) = uVar30;
        piVar3 = DAT_8c61de6c;
        if (in_FPSCR_SZ == '\0') {
          dVar28 = (double)CONCAT44(*(undefined4 *)((int)pdVar20 + 4),SUB84(dVar35,0));
        }
        else {
          dVar28 = *(double *)((int)pdVar20 + 4);
        }
        iVar10 = *DAT_8c61de6c;
        if (in_FPSCR_SZ == '\0') {
          dVar27 = (double)CONCAT44((int)((ulonglong)dVar27 >> 0x20),*(undefined4 *)(iVar10 + 0x14))
          ;
        }
        else {
          dVar23 = *(double *)(iVar10 + 0x14);
        }
        if (in_FPSCR_PR == '\0') {
          fVar26 = SUB84(dVar27,0) * (float)((ulonglong)dVar28 >> 0x20);
        }
        else {
          fVar26 = SUB84(dVar27 * dVar28,0);
        }
        if (in_FPSCR_SZ == '\0') {
          uVar29 = CONCAT44(*(undefined4 *)(iVar10 + 4),fVar26);
          dVar28 = (double)CONCAT44(*(undefined4 *)pdVar20,SUB84(dVar28,0));
        }
        else {
          uVar29 = *(ulonglong *)(iVar10 + 4);
          dVar28 = *pdVar20;
        }
        if (in_FPSCR_PR == '\0') {
          uVar29 = (ulonglong)
                   (uint)((float)((ulonglong)dVar28 >> 0x20) * (float)(uVar29 >> 0x20) +
                         (float)uVar29);
        }
        if (in_FPSCR_SZ == '\0') {
          dVar28 = (double)CONCAT44(*(undefined4 *)(pdVar20 + 1),SUB84(dVar28,0));
          uVar29 = CONCAT44(*(undefined4 *)(iVar10 + 0x24),(int)uVar29);
        }
        else {
          dVar28 = pdVar20[1];
          uVar29 = *(ulonglong *)(iVar10 + 0x24);
        }
        if (in_FPSCR_PR == '\0') {
          uVar29 = (ulonglong)
                   (uint)((float)((ulonglong)dVar28 >> 0x20) * (float)(uVar29 >> 0x20) +
                         (float)uVar29);
        }
        dVar35 = in_xd14;
        dVar33 = in_xd14;
        if (in_FPSCR_SZ == '\0') {
          dVar35 = (double)CONCAT44(uVar32,(int)uVar29);
          dVar33 = (double)CONCAT44(uVar32,SUB84(dVar28,0));
        }
        if (in_FPSCR_PR == '\0') {
          dVar35 = (double)CONCAT44((float)((ulonglong)dVar33 >> 0x20) * SUB84(dVar35,0) +
                                    (float)((ulonglong)dVar35 >> 0x20),SUB84(dVar35,0));
        }
        uVar25 = (undefined4)((ulonglong)dVar35 >> 0x20);
        if (in_FPSCR_SZ == '\0') {
          pdVar11 = pdVar11 + 6;
          *(undefined4 *)pdVar11 = uVar25;
        }
        else {
          pdVar11 = (double *)((int)pdVar11 + 0x2c);
          *pdVar11 = dVar35;
        }
        uVar30 = DAT_8c61de70;
        puVar8 = (ulonglong *)*piVar3;
        if (in_FPSCR_SZ == '\0') {
          dVar28 = (double)CONCAT44(*(undefined4 *)((int)pdVar20 + 4),SUB84(dVar33,0));
          dVar35 = (double)CONCAT44(uVar25,(int)puVar8[2]);
        }
        else {
          dVar28 = *(double *)((int)pdVar20 + 4);
          dVar23 = (double)puVar8[2];
        }
        if (in_FPSCR_PR == '\0') {
          fVar26 = SUB84(dVar35,0) * (float)((ulonglong)dVar28 >> 0x20);
        }
        else {
          fVar26 = SUB84(dVar35 * dVar28,0);
        }
        if (in_FPSCR_SZ == '\0') {
          uVar29 = CONCAT44((int)*puVar8,fVar26);
          dVar28 = (double)CONCAT44(*(undefined4 *)pdVar20,SUB84(dVar28,0));
        }
        else {
          uVar29 = *puVar8;
          dVar28 = *pdVar20;
        }
        if (in_FPSCR_PR == '\0') {
          uVar29 = (ulonglong)
                   (uint)((float)((ulonglong)dVar28 >> 0x20) * (float)(uVar29 >> 0x20) +
                         (float)uVar29);
        }
        if (in_FPSCR_SZ == '\0') {
          dVar28 = (double)CONCAT44(*(undefined4 *)(pdVar20 + 1),SUB84(dVar28,0));
          uVar29 = CONCAT44((int)puVar8[4],(int)uVar29);
        }
        else {
          dVar28 = pdVar20[1];
          uVar29 = puVar8[4];
        }
        if (in_FPSCR_PR == '\0') {
          uVar29 = (ulonglong)
                   (uint)((float)((ulonglong)dVar28 >> 0x20) * (float)(uVar29 >> 0x20) +
                         (float)uVar29);
        }
        dVar35 = in_xd14;
        dVar33 = in_xd14;
        if (in_FPSCR_SZ == '\0') {
          dVar35 = (double)CONCAT44(uVar32,(int)uVar29);
          dVar33 = (double)CONCAT44(uVar32,SUB84(dVar28,0));
        }
        if (in_FPSCR_PR == '\0') {
          dVar35 = (double)CONCAT44((float)((ulonglong)dVar33 >> 0x20) * SUB84(dVar35,0) +
                                    (float)((ulonglong)dVar35 >> 0x20),SUB84(dVar35,0));
        }
        uVar25 = (undefined4)((ulonglong)dVar35 >> 0x20);
        if (in_FPSCR_SZ == '\0') {
          *(undefined4 *)((int)pdVar11 + -4) = uVar25;
          *(undefined4 *)(pdVar11 + -1) = *(undefined4 *)(pdVar20 + 3);
          *(undefined4 *)((int)pdVar11 + -0xc) = *(undefined4 *)((int)pdVar20 + 0x14);
          dVar35 = (double)CONCAT44(uVar25,*(undefined4 *)(pdVar20 + 2));
          pdVar11 = pdVar11 + -2;
          *(undefined4 *)pdVar11 = *(undefined4 *)(pdVar20 + 2);
        }
        else {
          pdVar11[-1] = dVar35;
          pdVar11[-2] = pdVar20[3];
          pdVar11[-3] = *(double *)((int)pdVar20 + 0x14);
          dVar23 = pdVar20[2];
          pdVar11 = pdVar11 + -4;
          *pdVar11 = dVar23;
        }
        *(undefined4 *)((int)pdVar11 + -4) = uVar30;
        piVar3 = DAT_8c61de6c;
        if (in_FPSCR_SZ == '\0') {
          pdVar11 = (double *)((int)pdVar11 + 0x34);
          *(int *)pdVar11 = (int)((ulonglong)param_1 >> 0x20);
        }
        else {
          pdVar11 = pdVar11 + 6;
          *pdVar11 = param_1;
        }
        piVar4 = DAT_8c61de6c;
        iVar10 = *piVar3;
        if (in_FPSCR_SZ == '\0') {
          dVar28 = (double)CONCAT44(*(undefined4 *)((int)pdVar19 + 4),SUB84(dVar33,0));
          dVar35 = (double)CONCAT44((int)((ulonglong)dVar35 >> 0x20),*(undefined4 *)(iVar10 + 0x14))
          ;
        }
        else {
          dVar28 = *(double *)((int)pdVar19 + 4);
          dVar23 = *(double *)(iVar10 + 0x14);
        }
        if (in_FPSCR_PR == '\0') {
          fVar26 = SUB84(dVar35,0) * (float)((ulonglong)dVar28 >> 0x20);
        }
        else {
          fVar26 = SUB84(dVar35 * dVar28,0);
        }
        if (in_FPSCR_SZ == '\0') {
          uVar29 = CONCAT44(*(undefined4 *)(iVar10 + 4),fVar26);
          dVar28 = (double)CONCAT44(*(undefined4 *)pdVar19,SUB84(dVar28,0));
        }
        else {
          uVar29 = *(ulonglong *)(iVar10 + 4);
          dVar28 = *pdVar19;
        }
        if (in_FPSCR_PR == '\0') {
          uVar29 = (ulonglong)
                   (uint)((float)((ulonglong)dVar28 >> 0x20) * (float)(uVar29 >> 0x20) +
                         (float)uVar29);
        }
        if (in_FPSCR_SZ == '\0') {
          dVar28 = (double)CONCAT44(*(undefined4 *)(pdVar19 + 1),SUB84(dVar28,0));
          uVar29 = CONCAT44(*(undefined4 *)(iVar10 + 0x24),(int)uVar29);
        }
        else {
          dVar28 = pdVar19[1];
          uVar29 = *(ulonglong *)(iVar10 + 0x24);
        }
        if (in_FPSCR_PR == '\0') {
          uVar29 = (ulonglong)
                   (uint)((float)((ulonglong)dVar28 >> 0x20) * (float)(uVar29 >> 0x20) +
                         (float)uVar29);
        }
        dVar35 = in_xd14;
        dVar33 = in_xd14;
        if (in_FPSCR_SZ == '\0') {
          dVar35 = (double)CONCAT44(uVar32,SUB84(dVar28,0));
          dVar33 = (double)CONCAT44(uVar32,(int)uVar29);
        }
        if (in_FPSCR_PR == '\0') {
          dVar33 = (double)CONCAT44((float)((ulonglong)dVar35 >> 0x20) * SUB84(dVar33,0) +
                                    (float)((ulonglong)dVar33 >> 0x20),SUB84(dVar33,0));
        }
        uVar25 = (undefined4)((ulonglong)dVar33 >> 0x20);
        if (in_FPSCR_SZ == '\0') {
          pdVar11 = (double *)((int)pdVar11 + -4);
          *(undefined4 *)pdVar11 = uVar25;
        }
        else {
          pdVar11 = pdVar11 + -1;
          *pdVar11 = dVar33;
        }
        uVar30 = DAT_8c61de74;
        puVar8 = (ulonglong *)*piVar4;
        if (in_FPSCR_SZ == '\0') {
          dVar28 = (double)CONCAT44(*(undefined4 *)((int)pdVar19 + 4),SUB84(dVar35,0));
          dVar33 = (double)CONCAT44(uVar25,(int)puVar8[2]);
        }
        else {
          dVar28 = *(double *)((int)pdVar19 + 4);
          dVar23 = (double)puVar8[2];
        }
        if (in_FPSCR_PR == '\0') {
          fVar26 = SUB84(dVar33,0) * (float)((ulonglong)dVar28 >> 0x20);
        }
        else {
          fVar26 = SUB84(dVar33 * dVar28,0);
        }
        if (in_FPSCR_SZ == '\0') {
          dVar28 = (double)CONCAT44(*(undefined4 *)pdVar19,SUB84(dVar28,0));
          uVar29 = CONCAT44((int)*puVar8,fVar26);
        }
        else {
          dVar28 = *pdVar19;
          uVar29 = *puVar8;
        }
        if (in_FPSCR_PR == '\0') {
          uVar29 = (ulonglong)
                   (uint)((float)((ulonglong)dVar28 >> 0x20) * (float)(uVar29 >> 0x20) +
                         (float)uVar29);
        }
        if (in_FPSCR_SZ == '\0') {
          dVar28 = (double)CONCAT44(*(undefined4 *)(pdVar19 + 1),SUB84(dVar28,0));
          uVar29 = CONCAT44((int)puVar8[4],(int)uVar29);
        }
        else {
          dVar28 = pdVar19[1];
          uVar29 = puVar8[4];
        }
        if (in_FPSCR_PR == '\0') {
          uVar29 = (ulonglong)
                   (uint)((float)((ulonglong)dVar28 >> 0x20) * (float)(uVar29 >> 0x20) +
                         (float)uVar29);
        }
        dVar33 = in_xd14;
        in_dr0 = in_xd14;
        if (in_FPSCR_SZ == '\0') {
          dVar33 = (double)CONCAT44(uVar32,(int)uVar29);
          in_dr0 = (double)CONCAT44(uVar32,SUB84(dVar28,0));
        }
        if (in_FPSCR_PR == '\0') {
          dVar33 = (double)CONCAT44((float)((ulonglong)in_dr0 >> 0x20) * SUB84(dVar33,0) +
                                    (float)((ulonglong)dVar33 >> 0x20),SUB84(dVar33,0));
        }
        if (in_FPSCR_SZ == '\0') {
          *(int *)((int)pdVar11 + -4) = (int)((ulonglong)dVar33 >> 0x20);
          *(undefined4 *)(pdVar11 + -1) = *(undefined4 *)(pdVar19 + 3);
          *(undefined4 *)((int)pdVar11 + -0xc) = *(undefined4 *)((int)pdVar19 + 0x14);
          pdVar11 = pdVar11 + -2;
          *(undefined4 *)pdVar11 = *(undefined4 *)(pdVar19 + 2);
          in_xd2 = dVar23;
        }
        else {
          pdVar11[-1] = dVar33;
          pdVar11[-2] = pdVar19[3];
          pdVar11[-3] = *(double *)((int)pdVar19 + 0x14);
          in_xd2 = pdVar19[2];
          pdVar11 = pdVar11 + -4;
          *pdVar11 = in_xd2;
        }
        *(undefined4 *)((int)pdVar11 + -4) = uVar30;
        unaff_r8 = (int)pdVar11 + 0x1c;
        *DAT_8c61de78 = *DAT_8c61de78 + 3;
      }
    }
  }
  else if (uVar6 == (int)DAT_8c61dc00) {
    *_DAT_8c61dc10 = *_DAT_8c61dc10 + (uint)*(ushort *)(*(int *)((int)puVar22 + -0x14) + 2) * 4;
    iVar13 = 0;
    while( true ) {
      uVar32 = SUB84(param_1,0);
      uVar25 = SUB84(in_dr2,0);
      if ((int)(uint)*(ushort *)(*(int *)((int)puVar22 + -0x14) + 2) <= iVar13) break;
      uVar1 = *puVar14;
      puVar17 = puVar14 + 1;
      puVar15 = puVar14 + 2;
      iVar10 = *DAT_8c61e18c;
      puVar16 = puVar14 + 3;
      puVar14 = puVar14 + 4;
      pdVar20 = (double *)((uint)*puVar17 * 0x20 + iVar10);
      pdVar21 = (double *)((uint)uVar1 * 0x20 + iVar10);
      if (in_FPSCR_SZ == '\0') {
        uVar32 = *(undefined4 *)(pdVar20 + 2);
        uVar25 = *(undefined4 *)(pdVar21 + 2);
      }
      else {
        in_xd4 = pdVar20[2];
        in_xd2 = pdVar21[2];
      }
      dVar28 = in_xd4;
      if (in_FPSCR_SZ == '\0') {
        dVar28 = (double)CONCAT44(uVar32,uVar25);
      }
      if (in_FPSCR_PR == '\0') {
        dVar28 = (double)CONCAT44((float)((ulonglong)dVar28 >> 0x20) - SUB84(dVar28,0),
                                  SUB84(dVar28,0));
      }
      else {
        dVar28 = dVar28 - dVar28;
      }
      if (in_FPSCR_SZ == '\0') {
        dVar33 = (double)CONCAT44(*(undefined4 *)((int)pdVar20 + 0x14),uVar32);
      }
      else {
        dVar33 = *(double *)((int)pdVar20 + 0x14);
      }
      pdVar19 = (double *)((uint)*puVar15 * 0x20 + iVar10);
      fVar26 = (float)((ulonglong)dVar33 >> 0x20);
      if (in_FPSCR_SZ == '\0') {
        dVar28 = (double)CONCAT44((int)((ulonglong)dVar28 >> 0x20),
                                  *(undefined4 *)((int)pdVar19 + 0x14));
        in_dr0 = (double)CONCAT44((int)((ulonglong)in_dr0 >> 0x20),fVar26);
      }
      else {
        in_xd2 = *(double *)((int)pdVar19 + 0x14);
      }
      if (in_FPSCR_PR == '\0') {
        fVar26 = SUB84(dVar28,0) - fVar26;
        dVar28 = (double)CONCAT44((float)((ulonglong)dVar28 >> 0x20) * fVar26,fVar26);
      }
      else {
        dVar28 = (dVar28 - dVar33) * (dVar28 - dVar33);
      }
      if (in_FPSCR_SZ == '\0') {
        dVar28 = (double)CONCAT44((int)((ulonglong)dVar28 >> 0x20),
                                  *(undefined4 *)((int)pdVar21 + 0x14));
      }
      else {
        in_xd2 = *(double *)((int)pdVar21 + 0x14);
      }
      if (in_FPSCR_PR == '\0') {
        dVar23 = (double)CONCAT44((int)((ulonglong)in_dr0 >> 0x20),SUB84(in_dr0,0) - SUB84(dVar28,0)
                                 );
      }
      else {
        dVar23 = in_dr0 - dVar28;
      }
      if (in_FPSCR_SZ == '\0') {
        dVar28 = (double)CONCAT44((int)((ulonglong)dVar28 >> 0x20),*(undefined4 *)(pdVar19 + 2));
      }
      else {
        in_xd2 = pdVar19[2];
      }
      if (in_FPSCR_PR == '\0') {
        dVar28 = (double)CONCAT44((int)((ulonglong)dVar28 >> 0x20),SUB84(dVar28,0) - SUB84(dVar33,0)
                                 );
      }
      else {
        dVar28 = dVar28 - dVar33;
      }
      uVar6 = (uint)((ulonglong)dVar28 >> 0x20);
      param_1 = dVar28;
      if (in_FPSCR_SZ == '\0') {
        param_1 = (double)CONCAT44(uVar6,SUB84(dVar33,0));
      }
      if (in_FPSCR_PR == '\0') {
        in_dr0 = (double)CONCAT44((int)((ulonglong)dVar23 >> 0x20),SUB84(dVar23,0) * SUB84(dVar28,0)
                                 );
      }
      else {
        in_dr0 = dVar23 * dVar28;
      }
      in_dr2 = (double)((ulonglong)uVar6 << 0x20);
      if (in_FPSCR_PR == '\0') {
        fVar26 = (float)((ulonglong)param_1 >> 0x20) - SUB84(in_dr0,0);
        param_1 = (double)CONCAT44(fVar26,SUB84(param_1,0));
        bVar36 = 0.0 < fVar26;
      }
      else {
        param_1 = param_1 - in_dr0;
        bVar36 = in_dr2 < param_1;
      }
      uVar32 = SUB84(param_1,0);
      puVar18 = (undefined8 *)((uint)*puVar16 * 0x20 + iVar10);
      if (!bVar36) {
        if (in_FPSCR_SZ == '\0') {
          dVar28 = (double)CONCAT44(*(undefined4 *)pdVar21,*(undefined4 *)pdVar20);
        }
        else {
          dVar28 = *pdVar21;
          in_xd2 = *pdVar20;
        }
        if (in_FPSCR_PR == '\0') {
          dVar28 = (double)CONCAT44((float)((ulonglong)dVar28 >> 0x20) + SUB84(dVar28,0),
                                    SUB84(dVar28,0));
        }
        else {
          dVar28 = dVar28 + dVar28;
        }
        if (in_FPSCR_SZ == '\0') {
          dVar28 = (double)CONCAT44((int)((ulonglong)dVar28 >> 0x20),*(undefined4 *)pdVar19);
        }
        else {
          in_xd2 = *pdVar19;
        }
        if (in_FPSCR_PR == '\0') {
          dVar28 = (double)CONCAT44((float)((ulonglong)dVar28 >> 0x20) + SUB84(dVar28,0),
                                    SUB84(dVar28,0));
        }
        else {
          dVar28 = dVar28 + dVar28;
        }
        if (in_FPSCR_SZ == '\0') {
          *(int *)((int)puVar22 + -0x10) = (int)((ulonglong)dVar28 >> 0x20);
          dVar28 = (double)CONCAT44(*(undefined4 *)((int)pdVar21 + 4),
                                    *(undefined4 *)((int)pdVar20 + 4));
        }
        else {
          *(double *)((int)puVar22 + -0x10) = dVar28;
          dVar28 = *(double *)((int)pdVar21 + 4);
          in_xd2 = *(double *)((int)pdVar20 + 4);
        }
        if (in_FPSCR_PR == '\0') {
          dVar28 = (double)CONCAT44((float)((ulonglong)dVar28 >> 0x20) + SUB84(dVar28,0),
                                    SUB84(dVar28,0));
        }
        else {
          dVar28 = dVar28 + dVar28;
        }
        if (in_FPSCR_SZ == '\0') {
          dVar28 = (double)CONCAT44((int)((ulonglong)dVar28 >> 0x20),
                                    *(undefined4 *)((int)pdVar19 + 4));
        }
        else {
          in_xd2 = *(double *)((int)pdVar19 + 4);
        }
        if (in_FPSCR_PR == '\0') {
          dVar28 = (double)CONCAT44((float)((ulonglong)dVar28 >> 0x20) + SUB84(dVar28,0),
                                    SUB84(dVar28,0));
        }
        else {
          dVar28 = dVar28 + dVar28;
        }
        if (in_FPSCR_SZ == '\0') {
          *(int *)((int)puVar22 + -0xc) = (int)((ulonglong)dVar28 >> 0x20);
          dVar28 = (double)CONCAT44(*(undefined4 *)(pdVar21 + 1),*(undefined4 *)(pdVar20 + 1));
        }
        else {
          *(double *)((int)puVar22 + -0xc) = dVar28;
          dVar28 = pdVar21[1];
          in_xd2 = pdVar20[1];
        }
        if (in_FPSCR_PR == '\0') {
          dVar28 = (double)CONCAT44((float)((ulonglong)dVar28 >> 0x20) + SUB84(dVar28,0),
                                    SUB84(dVar28,0));
        }
        else {
          dVar28 = dVar28 + dVar28;
        }
        if (in_FPSCR_SZ == '\0') {
          dVar28 = (double)CONCAT44((int)((ulonglong)dVar28 >> 0x20),*(undefined4 *)(pdVar19 + 1));
        }
        else {
          in_xd2 = pdVar19[1];
        }
        if (in_FPSCR_PR == '\0') {
          dVar28 = (double)CONCAT44((float)((ulonglong)dVar28 >> 0x20) + SUB84(dVar28,0),
                                    SUB84(dVar28,0));
        }
        else {
          dVar28 = dVar28 + dVar28;
        }
        if (in_FPSCR_SZ == '\0') {
          *(int *)((int)puVar22 + -8) = (int)((ulonglong)dVar28 >> 0x20);
        }
        else {
          *(double *)((int)puVar22 + -8) = dVar28;
        }
        dVar28 = (double)(*(code *)PTR_FUN_8c61e190)((undefined1 *)((int)puVar22 + -0x10));
        piVar3 = DAT_8c61e1a0;
        if (in_FPSCR_SZ == '\0') {
          dVar33 = (double)CONCAT44(*(undefined4 *)DAT_8c61e194,
                                    *(undefined4 *)((int)puVar22 + -0x10));
        }
        else {
          in_xd2 = *(double *)((int)puVar22 + -0x10);
          dVar33 = *DAT_8c61e194;
        }
        if (in_FPSCR_PR == '\0') {
          dVar33 = (double)CONCAT44((float)((ulonglong)dVar33 >> 0x20) * SUB84(dVar33,0),
                                    SUB84(dVar33,0));
        }
        else {
          dVar33 = dVar33 * dVar33;
        }
        if (in_FPSCR_SZ == '\0') {
          dVar33 = (double)CONCAT44((int)((ulonglong)dVar33 >> 0x20),*(undefined4 *)DAT_8c61e198);
          dVar28 = (double)CONCAT44((int)((ulonglong)dVar28 >> 0x20),*DAT_8c61e19c);
        }
        else {
          in_xd2 = *DAT_8c61e198;
        }
        if (in_FPSCR_PR == '\0') {
          fVar26 = SUB84(dVar33,0) - (float)((ulonglong)dVar33 >> 0x20);
        }
        else {
          fVar26 = SUB84(dVar33 - dVar33,0);
        }
        if (in_FPSCR_SZ == '\0') {
          dVar33 = (double)CONCAT44(*(undefined4 *)((int)puVar22 + -0xc),fVar26);
        }
        else {
          dVar33 = *(double *)((int)puVar22 + -0xc);
        }
        if (in_FPSCR_PR == '\0') {
          dVar28 = (double)CONCAT44((int)((ulonglong)dVar28 >> 0x20),
                                    SUB84(dVar28,0) * (float)((ulonglong)dVar33 >> 0x20));
        }
        else {
          dVar28 = dVar28 * dVar33;
        }
        if (in_FPSCR_SZ == '\0') {
          dVar33 = (double)CONCAT44(*(undefined4 *)((int)puVar22 + -8),SUB84(dVar33,0));
        }
        else {
          dVar33 = *(double *)((int)puVar22 + -8);
        }
        iVar10 = *DAT_8c61e1a0;
        if (in_FPSCR_PR == '\0') {
          dVar33 = (double)CONCAT44((int)((ulonglong)dVar33 >> 0x20),
                                    SUB84(dVar33,0) - SUB84(dVar28,0));
        }
        else {
          dVar33 = dVar33 - dVar28;
        }
        if (in_FPSCR_SZ == '\0') {
          dVar28 = (double)CONCAT44(*(undefined4 *)((int)pdVar21 + 4),
                                    *(undefined4 *)(DAT_8c61e194 + 1));
        }
        else {
          dVar28 = *(double *)((int)pdVar21 + 4);
        }
        fVar26 = (float)((ulonglong)dVar33 >> 0x20);
        if (in_FPSCR_PR == '\0') {
          dVar28 = (double)CONCAT44((int)((ulonglong)dVar28 >> 0x20),SUB84(dVar28,0) * fVar26);
        }
        else {
          dVar28 = dVar28 * dVar33;
        }
        if (in_FPSCR_SZ == '\0') {
          uVar25 = SUB84(dVar33,0);
          dVar33 = (double)CONCAT44(fVar26,*(undefined4 *)(iVar10 + 0x14));
          dVar23 = in_xd2;
          in_xd2 = (double)CONCAT44(uVar25,uVar32);
        }
        else {
          dVar23 = *(double *)(iVar10 + 0x14);
        }
        if (in_FPSCR_PR == '\0') {
          param_1 = (double)CONCAT44((float)((ulonglong)in_xd2 >> 0x20) - SUB84(dVar28,0),
                                     SUB84(in_xd2,0));
          fVar26 = SUB84(dVar33,0) * (float)((ulonglong)dVar28 >> 0x20);
        }
        else {
          param_1 = in_xd2 - dVar28;
          fVar26 = SUB84(dVar33 * dVar28,0);
        }
        if (in_FPSCR_SZ == '\0') {
          dVar28 = (double)CONCAT44(*(undefined4 *)pdVar21,SUB84(dVar28,0));
          uVar29 = CONCAT44(*(undefined4 *)(iVar10 + 4),fVar26);
        }
        else {
          dVar28 = *pdVar21;
          uVar29 = *(ulonglong *)(iVar10 + 4);
        }
        if (in_FPSCR_PR == '\0') {
          uVar29 = (ulonglong)
                   (uint)((float)((ulonglong)dVar28 >> 0x20) * (float)(uVar29 >> 0x20) +
                         (float)uVar29);
        }
        if (in_FPSCR_SZ == '\0') {
          dVar28 = (double)CONCAT44(*(undefined4 *)(pdVar21 + 1),SUB84(dVar28,0));
          uVar29 = CONCAT44(*(undefined4 *)(iVar10 + 0x24),(int)uVar29);
        }
        else {
          dVar28 = pdVar21[1];
          uVar29 = *(ulonglong *)(iVar10 + 0x24);
        }
        if (in_FPSCR_PR == '\0') {
          uVar29 = (ulonglong)
                   (uint)((float)((ulonglong)dVar28 >> 0x20) * (float)(uVar29 >> 0x20) +
                         (float)uVar29);
        }
        uVar32 = (undefined4)in_dr14;
        dVar35 = in_xd14;
        dVar33 = in_xd14;
        if (in_FPSCR_SZ == '\0') {
          dVar35 = (double)CONCAT44(uVar32,(int)uVar29);
          dVar33 = (double)CONCAT44(uVar32,SUB84(dVar28,0));
        }
        if (in_FPSCR_PR == '\0') {
          dVar35 = (double)CONCAT44((float)((ulonglong)dVar33 >> 0x20) * SUB84(dVar35,0) +
                                    (float)((ulonglong)dVar35 >> 0x20),SUB84(dVar35,0));
        }
        if (in_FPSCR_SZ == '\0') {
          pdVar11 = (double *)(unaff_r8 + 0x14);
          *(int *)pdVar11 = (int)((ulonglong)dVar35 >> 0x20);
        }
        else {
          pdVar11 = (double *)(unaff_r8 + 0x10);
          *pdVar11 = dVar35;
        }
        pdVar7 = (double *)*piVar3;
        if (in_FPSCR_SZ == '\0') {
          dVar28 = (double)CONCAT44(*(undefined4 *)((int)pdVar21 + 4),SUB84(dVar33,0));
          dVar33 = (double)CONCAT44(*(undefined4 *)pdVar7,*(undefined4 *)(pdVar7 + 2));
        }
        else {
          dVar28 = *(double *)((int)pdVar21 + 4);
          dVar23 = pdVar7[2];
          dVar33 = *pdVar7;
        }
        if (in_FPSCR_PR == '\0') {
          dVar33 = (double)CONCAT44((int)((ulonglong)dVar33 >> 0x20),
                                    SUB84(dVar33,0) * (float)((ulonglong)dVar28 >> 0x20));
        }
        else {
          dVar33 = dVar33 * dVar28;
        }
        if (in_FPSCR_SZ == '\0') {
          dVar28 = (double)CONCAT44(*(undefined4 *)pdVar21,SUB84(dVar28,0));
        }
        else {
          dVar28 = *pdVar21;
        }
        if (in_FPSCR_PR == '\0') {
          dVar33 = (double)(ulonglong)
                           (uint)((float)((ulonglong)dVar28 >> 0x20) *
                                  (float)((ulonglong)dVar33 >> 0x20) + SUB84(dVar33,0));
        }
        if (in_FPSCR_SZ == '\0') {
          dVar28 = (double)CONCAT44(*(undefined4 *)(pdVar21 + 1),SUB84(dVar28,0));
          dVar33 = (double)CONCAT44(*(undefined4 *)(pdVar7 + 4),SUB84(dVar33,0));
        }
        else {
          dVar28 = pdVar21[1];
          dVar33 = pdVar7[4];
        }
        if (in_FPSCR_PR == '\0') {
          dVar33 = (double)(ulonglong)
                           (uint)((float)((ulonglong)dVar28 >> 0x20) *
                                  (float)((ulonglong)dVar33 >> 0x20) + SUB84(dVar33,0));
        }
        dVar27 = in_xd14;
        dVar35 = in_xd14;
        if (in_FPSCR_SZ == '\0') {
          dVar27 = (double)CONCAT44(uVar32,SUB84(dVar33,0));
          dVar35 = (double)CONCAT44(uVar32,SUB84(dVar28,0));
        }
        if (in_FPSCR_PR == '\0') {
          dVar27 = (double)CONCAT44((float)((ulonglong)dVar35 >> 0x20) * SUB84(dVar27,0) +
                                    (float)((ulonglong)dVar27 >> 0x20),SUB84(dVar27,0));
        }
        uVar25 = (undefined4)((ulonglong)dVar27 >> 0x20);
        if (in_FPSCR_SZ == '\0') {
          *(undefined4 *)((int)pdVar11 + -4) = uVar25;
          uVar30 = DAT_8c61e1a4;
          *(undefined4 *)(pdVar11 + -1) = *(undefined4 *)(pdVar21 + 3);
          *(undefined4 *)((int)pdVar11 + -0xc) = *(undefined4 *)((int)pdVar21 + 0x14);
          dVar27 = (double)CONCAT44(uVar25,*(undefined4 *)(pdVar21 + 2));
          pdVar11 = pdVar11 + -2;
          *(undefined4 *)pdVar11 = *(undefined4 *)(pdVar21 + 2);
        }
        else {
          pdVar11[-1] = dVar27;
          uVar30 = DAT_8c61e1a4;
          pdVar11[-2] = pdVar21[3];
          pdVar11[-3] = *(double *)((int)pdVar21 + 0x14);
          dVar23 = pdVar21[2];
          pdVar11 = pdVar11 + -4;
          *pdVar11 = dVar23;
        }
        *(undefined4 *)((int)pdVar11 + -4) = uVar30;
        piVar3 = DAT_8c61e1a0;
        if (in_FPSCR_SZ == '\0') {
          dVar28 = (double)CONCAT44(*(undefined4 *)((int)pdVar20 + 4),SUB84(dVar35,0));
        }
        else {
          dVar28 = *(double *)((int)pdVar20 + 4);
        }
        iVar10 = *DAT_8c61e1a0;
        if (in_FPSCR_SZ == '\0') {
          dVar27 = (double)CONCAT44((int)((ulonglong)dVar27 >> 0x20),*(undefined4 *)(iVar10 + 0x14))
          ;
        }
        else {
          dVar23 = *(double *)(iVar10 + 0x14);
        }
        if (in_FPSCR_PR == '\0') {
          fVar26 = SUB84(dVar27,0) * (float)((ulonglong)dVar28 >> 0x20);
        }
        else {
          fVar26 = SUB84(dVar27 * dVar28,0);
        }
        if (in_FPSCR_SZ == '\0') {
          uVar29 = CONCAT44(*(undefined4 *)(iVar10 + 4),fVar26);
          dVar28 = (double)CONCAT44(*(undefined4 *)pdVar20,SUB84(dVar28,0));
        }
        else {
          uVar29 = *(ulonglong *)(iVar10 + 4);
          dVar28 = *pdVar20;
        }
        if (in_FPSCR_PR == '\0') {
          uVar29 = (ulonglong)
                   (uint)((float)((ulonglong)dVar28 >> 0x20) * (float)(uVar29 >> 0x20) +
                         (float)uVar29);
        }
        if (in_FPSCR_SZ == '\0') {
          dVar28 = (double)CONCAT44(*(undefined4 *)(pdVar20 + 1),SUB84(dVar28,0));
          uVar29 = CONCAT44(*(undefined4 *)(iVar10 + 0x24),(int)uVar29);
        }
        else {
          dVar28 = pdVar20[1];
          uVar29 = *(ulonglong *)(iVar10 + 0x24);
        }
        if (in_FPSCR_PR == '\0') {
          uVar29 = (ulonglong)
                   (uint)((float)((ulonglong)dVar28 >> 0x20) * (float)(uVar29 >> 0x20) +
                         (float)uVar29);
        }
        dVar35 = in_xd14;
        dVar33 = in_xd14;
        if (in_FPSCR_SZ == '\0') {
          dVar35 = (double)CONCAT44(uVar32,(int)uVar29);
          dVar33 = (double)CONCAT44(uVar32,SUB84(dVar28,0));
        }
        if (in_FPSCR_PR == '\0') {
          dVar35 = (double)CONCAT44((float)((ulonglong)dVar33 >> 0x20) * SUB84(dVar35,0) +
                                    (float)((ulonglong)dVar35 >> 0x20),SUB84(dVar35,0));
        }
        uVar25 = (undefined4)((ulonglong)dVar35 >> 0x20);
        if (in_FPSCR_SZ == '\0') {
          pdVar11 = pdVar11 + 6;
          *(undefined4 *)pdVar11 = uVar25;
        }
        else {
          pdVar11 = (double *)((int)pdVar11 + 0x2c);
          *pdVar11 = dVar35;
        }
        uVar30 = DAT_8c61e1a4;
        puVar8 = (ulonglong *)*piVar3;
        if (in_FPSCR_SZ == '\0') {
          dVar28 = (double)CONCAT44(*(undefined4 *)((int)pdVar20 + 4),SUB84(dVar33,0));
          dVar35 = (double)CONCAT44(uVar25,(int)puVar8[2]);
        }
        else {
          dVar28 = *(double *)((int)pdVar20 + 4);
          dVar23 = (double)puVar8[2];
        }
        if (in_FPSCR_PR == '\0') {
          fVar26 = SUB84(dVar35,0) * (float)((ulonglong)dVar28 >> 0x20);
        }
        else {
          fVar26 = SUB84(dVar35 * dVar28,0);
        }
        if (in_FPSCR_SZ == '\0') {
          uVar29 = CONCAT44((int)*puVar8,fVar26);
          dVar28 = (double)CONCAT44(*(undefined4 *)pdVar20,SUB84(dVar28,0));
        }
        else {
          uVar29 = *puVar8;
          dVar28 = *pdVar20;
        }
        if (in_FPSCR_PR == '\0') {
          uVar29 = (ulonglong)
                   (uint)((float)((ulonglong)dVar28 >> 0x20) * (float)(uVar29 >> 0x20) +
                         (float)uVar29);
        }
        if (in_FPSCR_SZ == '\0') {
          dVar28 = (double)CONCAT44(*(undefined4 *)(pdVar20 + 1),SUB84(dVar28,0));
          uVar29 = CONCAT44((int)puVar8[4],(int)uVar29);
        }
        else {
          dVar28 = pdVar20[1];
          uVar29 = puVar8[4];
        }
        if (in_FPSCR_PR == '\0') {
          uVar29 = (ulonglong)
                   (uint)((float)((ulonglong)dVar28 >> 0x20) * (float)(uVar29 >> 0x20) +
                         (float)uVar29);
        }
        dVar35 = in_xd14;
        dVar33 = in_xd14;
        if (in_FPSCR_SZ == '\0') {
          dVar35 = (double)CONCAT44(uVar32,(int)uVar29);
          dVar33 = (double)CONCAT44(uVar32,SUB84(dVar28,0));
        }
        if (in_FPSCR_PR == '\0') {
          dVar35 = (double)CONCAT44((float)((ulonglong)dVar33 >> 0x20) * SUB84(dVar35,0) +
                                    (float)((ulonglong)dVar35 >> 0x20),SUB84(dVar35,0));
        }
        uVar25 = (undefined4)((ulonglong)dVar35 >> 0x20);
        if (in_FPSCR_SZ == '\0') {
          *(undefined4 *)((int)pdVar11 + -4) = uVar25;
          *(undefined4 *)(pdVar11 + -1) = *(undefined4 *)(pdVar20 + 3);
          *(undefined4 *)((int)pdVar11 + -0xc) = *(undefined4 *)((int)pdVar20 + 0x14);
          dVar35 = (double)CONCAT44(uVar25,*(undefined4 *)(pdVar20 + 2));
          pdVar11 = pdVar11 + -2;
          *(undefined4 *)pdVar11 = *(undefined4 *)(pdVar20 + 2);
        }
        else {
          pdVar11[-1] = dVar35;
          pdVar11[-2] = pdVar20[3];
          pdVar11[-3] = *(double *)((int)pdVar20 + 0x14);
          dVar23 = pdVar20[2];
          pdVar11 = pdVar11 + -4;
          *pdVar11 = dVar23;
        }
        *(undefined4 *)((int)pdVar11 + -4) = uVar30;
        piVar3 = DAT_8c61e1a0;
        uVar25 = (undefined4)((ulonglong)param_1 >> 0x20);
        if (in_FPSCR_SZ == '\0') {
          pdVar11 = (double *)((int)pdVar11 + 0x34);
          *(undefined4 *)pdVar11 = uVar25;
        }
        else {
          pdVar11 = pdVar11 + 6;
          *pdVar11 = param_1;
        }
        piVar4 = DAT_8c61e1a0;
        iVar10 = *piVar3;
        if (in_FPSCR_SZ == '\0') {
          dVar28 = (double)CONCAT44(*(undefined4 *)((int)pdVar19 + 4),SUB84(dVar33,0));
          dVar35 = (double)CONCAT44((int)((ulonglong)dVar35 >> 0x20),*(undefined4 *)(iVar10 + 0x14))
          ;
        }
        else {
          dVar28 = *(double *)((int)pdVar19 + 4);
          dVar23 = *(double *)(iVar10 + 0x14);
        }
        if (in_FPSCR_PR == '\0') {
          fVar26 = SUB84(dVar35,0) * (float)((ulonglong)dVar28 >> 0x20);
        }
        else {
          fVar26 = SUB84(dVar35 * dVar28,0);
        }
        if (in_FPSCR_SZ == '\0') {
          uVar29 = CONCAT44(*(undefined4 *)(iVar10 + 4),fVar26);
          dVar28 = (double)CONCAT44(*(undefined4 *)pdVar19,SUB84(dVar28,0));
        }
        else {
          uVar29 = *(ulonglong *)(iVar10 + 4);
          dVar28 = *pdVar19;
        }
        if (in_FPSCR_PR == '\0') {
          uVar29 = (ulonglong)
                   (uint)((float)((ulonglong)dVar28 >> 0x20) * (float)(uVar29 >> 0x20) +
                         (float)uVar29);
        }
        if (in_FPSCR_SZ == '\0') {
          dVar28 = (double)CONCAT44(*(undefined4 *)(pdVar19 + 1),SUB84(dVar28,0));
          uVar29 = CONCAT44(*(undefined4 *)(iVar10 + 0x24),(int)uVar29);
        }
        else {
          dVar28 = pdVar19[1];
          uVar29 = *(ulonglong *)(iVar10 + 0x24);
        }
        if (in_FPSCR_PR == '\0') {
          uVar29 = (ulonglong)
                   (uint)((float)((ulonglong)dVar28 >> 0x20) * (float)(uVar29 >> 0x20) +
                         (float)uVar29);
        }
        dVar35 = in_xd14;
        dVar33 = in_xd14;
        if (in_FPSCR_SZ == '\0') {
          dVar35 = (double)CONCAT44(uVar32,(int)uVar29);
          dVar33 = (double)CONCAT44(uVar32,SUB84(dVar28,0));
        }
        if (in_FPSCR_PR == '\0') {
          dVar35 = (double)CONCAT44((float)((ulonglong)dVar33 >> 0x20) * SUB84(dVar35,0) +
                                    (float)((ulonglong)dVar35 >> 0x20),SUB84(dVar35,0));
        }
        uVar30 = (undefined4)((ulonglong)dVar35 >> 0x20);
        if (in_FPSCR_SZ == '\0') {
          pdVar11 = (double *)((int)pdVar11 + -4);
          *(undefined4 *)pdVar11 = uVar30;
        }
        else {
          pdVar11 = pdVar11 + -1;
          *pdVar11 = dVar35;
        }
        uVar5 = DAT_8c61e1a4;
        puVar8 = (ulonglong *)*piVar4;
        if (in_FPSCR_SZ == '\0') {
          dVar28 = (double)CONCAT44(*(undefined4 *)((int)pdVar19 + 4),SUB84(dVar33,0));
          dVar35 = (double)CONCAT44(uVar30,(int)puVar8[2]);
        }
        else {
          dVar28 = *(double *)((int)pdVar19 + 4);
          dVar23 = (double)puVar8[2];
        }
        if (in_FPSCR_PR == '\0') {
          fVar26 = SUB84(dVar35,0) * (float)((ulonglong)dVar28 >> 0x20);
        }
        else {
          fVar26 = SUB84(dVar35 * dVar28,0);
        }
        if (in_FPSCR_SZ == '\0') {
          uVar29 = CONCAT44((int)*puVar8,fVar26);
          dVar28 = (double)CONCAT44(*(undefined4 *)pdVar19,SUB84(dVar28,0));
        }
        else {
          uVar29 = *puVar8;
          dVar28 = *pdVar19;
        }
        if (in_FPSCR_PR == '\0') {
          uVar29 = (ulonglong)
                   (uint)((float)((ulonglong)dVar28 >> 0x20) * (float)(uVar29 >> 0x20) +
                         (float)uVar29);
        }
        if (in_FPSCR_SZ == '\0') {
          dVar28 = (double)CONCAT44(*(undefined4 *)(pdVar19 + 1),SUB84(dVar28,0));
          uVar29 = CONCAT44((int)puVar8[4],(int)uVar29);
        }
        else {
          dVar28 = pdVar19[1];
          uVar29 = puVar8[4];
        }
        if (in_FPSCR_PR == '\0') {
          uVar29 = (ulonglong)
                   (uint)((float)((ulonglong)dVar28 >> 0x20) * (float)(uVar29 >> 0x20) +
                         (float)uVar29);
        }
        dVar35 = in_xd14;
        dVar33 = in_xd14;
        if (in_FPSCR_SZ == '\0') {
          dVar35 = (double)CONCAT44(uVar32,(int)uVar29);
          dVar33 = (double)CONCAT44(uVar32,SUB84(dVar28,0));
        }
        if (in_FPSCR_PR == '\0') {
          dVar35 = (double)CONCAT44((float)((ulonglong)dVar33 >> 0x20) * SUB84(dVar35,0) +
                                    (float)((ulonglong)dVar35 >> 0x20),SUB84(dVar35,0));
        }
        uVar30 = (undefined4)((ulonglong)dVar35 >> 0x20);
        if (in_FPSCR_SZ == '\0') {
          *(undefined4 *)((int)pdVar11 + -4) = uVar30;
          *(undefined4 *)(pdVar11 + -1) = *(undefined4 *)(pdVar19 + 3);
          *(undefined4 *)((int)pdVar11 + -0xc) = *(undefined4 *)((int)pdVar19 + 0x14);
          dVar35 = (double)CONCAT44(uVar30,*(undefined4 *)(pdVar19 + 2));
          pdVar11 = pdVar11 + -2;
          *(undefined4 *)pdVar11 = *(undefined4 *)(pdVar19 + 2);
        }
        else {
          pdVar11[-1] = dVar35;
          pdVar11[-2] = pdVar19[3];
          pdVar11[-3] = *(double *)((int)pdVar19 + 0x14);
          dVar23 = pdVar19[2];
          pdVar11 = pdVar11 + -4;
          *pdVar11 = dVar23;
        }
        *(undefined4 *)((int)pdVar11 + -4) = uVar5;
        piVar3 = DAT_8c61e1a0;
        if (in_FPSCR_SZ == '\0') {
          pdVar11 = (double *)((int)pdVar11 + 0x34);
          *(undefined4 *)pdVar11 = uVar25;
        }
        else {
          pdVar11 = pdVar11 + 6;
          *pdVar11 = param_1;
        }
        piVar4 = DAT_8c61e1a0;
        iVar10 = *piVar3;
        if (in_FPSCR_SZ == '\0') {
          dVar28 = (double)CONCAT44(*(undefined4 *)((int)puVar18 + 4),SUB84(dVar33,0));
          dVar35 = (double)CONCAT44((int)((ulonglong)dVar35 >> 0x20),*(undefined4 *)(iVar10 + 0x14))
          ;
        }
        else {
          dVar28 = *(double *)((int)puVar18 + 4);
          dVar23 = *(double *)(iVar10 + 0x14);
        }
        if (in_FPSCR_PR == '\0') {
          fVar26 = SUB84(dVar35,0) * (float)((ulonglong)dVar28 >> 0x20);
        }
        else {
          fVar26 = SUB84(dVar35 * dVar28,0);
        }
        if (in_FPSCR_SZ == '\0') {
          uVar29 = CONCAT44(*(undefined4 *)(iVar10 + 4),fVar26);
          uVar24 = CONCAT44(*(undefined4 *)puVar18,SUB84(dVar28,0));
        }
        else {
          uVar29 = *(ulonglong *)(iVar10 + 4);
          uVar24 = *puVar18;
        }
        if (in_FPSCR_PR == '\0') {
          uVar29 = (ulonglong)
                   (uint)((float)((ulonglong)uVar24 >> 0x20) * (float)(uVar29 >> 0x20) +
                         (float)uVar29);
        }
        if (in_FPSCR_SZ == '\0') {
          uVar24 = CONCAT44(*(undefined4 *)(puVar18 + 1),(int)uVar24);
          uVar29 = CONCAT44(*(undefined4 *)(iVar10 + 0x24),(int)uVar29);
        }
        else {
          uVar24 = puVar18[1];
          uVar29 = *(ulonglong *)(iVar10 + 0x24);
        }
        if (in_FPSCR_PR == '\0') {
          uVar29 = (ulonglong)
                   (uint)((float)((ulonglong)uVar24 >> 0x20) * (float)(uVar29 >> 0x20) +
                         (float)uVar29);
        }
        dVar33 = in_xd14;
        dVar28 = in_xd14;
        if (in_FPSCR_SZ == '\0') {
          dVar33 = (double)CONCAT44(uVar32,(int)uVar29);
          dVar28 = (double)CONCAT44(uVar32,(int)uVar24);
        }
        if (in_FPSCR_PR == '\0') {
          dVar33 = (double)CONCAT44((float)((ulonglong)dVar28 >> 0x20) * SUB84(dVar33,0) +
                                    (float)((ulonglong)dVar33 >> 0x20),SUB84(dVar33,0));
        }
        uVar25 = (undefined4)((ulonglong)dVar33 >> 0x20);
        if (in_FPSCR_SZ == '\0') {
          pdVar11 = (double *)((int)pdVar11 + -4);
          *(undefined4 *)pdVar11 = uVar25;
        }
        else {
          pdVar11 = pdVar11 + -1;
          *pdVar11 = dVar33;
        }
        uVar30 = DAT_8c61e1a8;
        puVar8 = (ulonglong *)*piVar4;
        if (in_FPSCR_SZ == '\0') {
          dVar28 = (double)CONCAT44(*(undefined4 *)((int)puVar18 + 4),SUB84(dVar28,0));
          dVar33 = (double)CONCAT44(uVar25,(int)puVar8[2]);
        }
        else {
          dVar28 = *(double *)((int)puVar18 + 4);
          dVar23 = (double)puVar8[2];
        }
        if (in_FPSCR_PR == '\0') {
          fVar26 = SUB84(dVar33,0) * (float)((ulonglong)dVar28 >> 0x20);
        }
        else {
          fVar26 = SUB84(dVar33 * dVar28,0);
        }
        if (in_FPSCR_SZ == '\0') {
          uVar29 = CONCAT44((int)*puVar8,fVar26);
          uVar24 = CONCAT44(*(undefined4 *)puVar18,SUB84(dVar28,0));
        }
        else {
          uVar29 = *puVar8;
          uVar24 = *puVar18;
        }
        if (in_FPSCR_PR == '\0') {
          uVar29 = (ulonglong)
                   (uint)((float)((ulonglong)uVar24 >> 0x20) * (float)(uVar29 >> 0x20) +
                         (float)uVar29);
        }
        if (in_FPSCR_SZ == '\0') {
          uVar24 = CONCAT44(*(undefined4 *)(puVar18 + 1),(int)uVar24);
          uVar29 = CONCAT44((int)puVar8[4],(int)uVar29);
        }
        else {
          uVar24 = puVar18[1];
          uVar29 = puVar8[4];
        }
        if (in_FPSCR_PR == '\0') {
          uVar29 = (ulonglong)
                   (uint)((float)((ulonglong)uVar24 >> 0x20) * (float)(uVar29 >> 0x20) +
                         (float)uVar29);
        }
        in_dr2 = in_xd14;
        in_dr0 = in_xd14;
        if (in_FPSCR_SZ == '\0') {
          in_dr2 = (double)CONCAT44(uVar32,(int)uVar29);
          in_dr0 = (double)CONCAT44(uVar32,(int)uVar24);
        }
        if (in_FPSCR_PR == '\0') {
          in_dr2 = (double)CONCAT44((float)((ulonglong)in_dr0 >> 0x20) * SUB84(in_dr2,0) +
                                    (float)((ulonglong)in_dr2 >> 0x20),SUB84(in_dr2,0));
        }
        if (in_FPSCR_SZ == '\0') {
          *(int *)((int)pdVar11 + -4) = (int)((ulonglong)in_dr2 >> 0x20);
          *(undefined4 *)(pdVar11 + -1) = *(undefined4 *)(puVar18 + 3);
          *(undefined4 *)((int)pdVar11 + -0xc) = *(undefined4 *)((int)puVar18 + 0x14);
          in_dr2 = (double)(ulonglong)*(uint *)(puVar18 + 2);
          pdVar11 = pdVar11 + -2;
          *(uint *)pdVar11 = *(uint *)(puVar18 + 2);
          in_xd2 = dVar23;
        }
        else {
          pdVar11[-1] = in_dr2;
          pdVar11[-2] = (double)puVar18[3];
          pdVar11[-3] = *(double *)((int)puVar18 + 0x14);
          in_xd2 = (double)puVar18[2];
          pdVar11 = pdVar11 + -4;
          *pdVar11 = in_xd2;
        }
        *(undefined4 *)((int)pdVar11 + -4) = uVar30;
        unaff_r8 = (int)pdVar11 + 0x1c;
        *DAT_8c61e1ac = *DAT_8c61e1ac + 4;
      }
      iVar13 = iVar13 + 1;
    }
  }
  else if (uVar6 == DAT_8c61dc18) {
    for (iVar13 = 0; uVar32 = SUB84(param_1,0),
        iVar13 < (int)(uint)*(ushort *)(*(int *)((int)puVar22 + -0x14) + 2); iVar13 = iVar13 + 1) {
      puVar17 = puVar14 + 1;
      uVar6 = (uint)*puVar14;
      *piVar3 = *piVar3 + uVar6;
      iVar10 = *DAT_8c61e18c;
      pdVar20 = (double *)((uint)puVar14[2] * 0x20 + iVar10);
      if (in_FPSCR_SZ == '\0') {
        uVar32 = *(undefined4 *)(pdVar20 + 2);
      }
      else {
        in_xd4 = pdVar20[2];
      }
      pdVar21 = (double *)((uint)*puVar17 * 0x20 + iVar10);
      if (in_FPSCR_SZ == '\0') {
        dVar28 = (double)CONCAT44(uVar32,*(undefined4 *)(pdVar21 + 2));
      }
      else {
        in_xd2 = pdVar21[2];
        dVar28 = in_xd4;
      }
      if (in_FPSCR_PR == '\0') {
        dVar28 = (double)CONCAT44((float)((ulonglong)dVar28 >> 0x20) - SUB84(dVar28,0),
                                  SUB84(dVar28,0));
      }
      else {
        dVar28 = dVar28 - dVar28;
      }
      if (in_FPSCR_SZ == '\0') {
        dVar33 = (double)CONCAT44(*(undefined4 *)((int)pdVar20 + 0x14),uVar32);
      }
      else {
        dVar33 = *(double *)((int)pdVar20 + 0x14);
      }
      pdVar19 = (double *)((uint)puVar14[3] * 0x20 + iVar10);
      fVar26 = (float)((ulonglong)dVar33 >> 0x20);
      if (in_FPSCR_SZ == '\0') {
        dVar28 = (double)CONCAT44((int)((ulonglong)dVar28 >> 0x20),
                                  *(undefined4 *)((int)pdVar19 + 0x14));
        in_dr0 = (double)CONCAT44((int)((ulonglong)in_dr0 >> 0x20),fVar26);
      }
      else {
        in_xd2 = *(double *)((int)pdVar19 + 0x14);
      }
      if (in_FPSCR_PR == '\0') {
        fVar26 = SUB84(dVar28,0) - fVar26;
        dVar28 = (double)CONCAT44((float)((ulonglong)dVar28 >> 0x20) * fVar26,fVar26);
      }
      else {
        dVar28 = (dVar28 - dVar33) * (dVar28 - dVar33);
      }
      if (in_FPSCR_SZ == '\0') {
        dVar28 = (double)CONCAT44((int)((ulonglong)dVar28 >> 0x20),
                                  *(undefined4 *)((int)pdVar21 + 0x14));
      }
      else {
        in_xd2 = *(double *)((int)pdVar21 + 0x14);
      }
      if (in_FPSCR_PR == '\0') {
        dVar23 = (double)CONCAT44((int)((ulonglong)in_dr0 >> 0x20),SUB84(in_dr0,0) - SUB84(dVar28,0)
                                 );
      }
      else {
        dVar23 = in_dr0 - dVar28;
      }
      if (in_FPSCR_SZ == '\0') {
        dVar28 = (double)CONCAT44((int)((ulonglong)dVar28 >> 0x20),*(undefined4 *)(pdVar19 + 2));
      }
      else {
        in_xd2 = pdVar19[2];
      }
      if (in_FPSCR_PR == '\0') {
        dVar28 = (double)CONCAT44((int)((ulonglong)dVar28 >> 0x20),SUB84(dVar28,0) - SUB84(dVar33,0)
                                 );
      }
      else {
        dVar28 = dVar28 - dVar33;
      }
      uVar31 = (uint)((ulonglong)dVar28 >> 0x20);
      param_1 = dVar28;
      if (in_FPSCR_SZ == '\0') {
        param_1 = (double)CONCAT44(uVar31,SUB84(dVar33,0));
      }
      if (in_FPSCR_PR == '\0') {
        in_dr0 = (double)CONCAT44((int)((ulonglong)dVar23 >> 0x20),SUB84(dVar23,0) * SUB84(dVar28,0)
                                 );
      }
      else {
        in_dr0 = dVar23 * dVar28;
      }
      if (in_FPSCR_PR == '\0') {
        fVar26 = (float)((ulonglong)param_1 >> 0x20) - SUB84(in_dr0,0);
        param_1 = (double)CONCAT44(fVar26,SUB84(param_1,0));
        bVar36 = 0.0 < fVar26;
      }
      else {
        param_1 = param_1 - in_dr0;
        bVar36 = (double)((ulonglong)uVar31 << 0x20) < param_1;
      }
      uVar32 = SUB84(param_1,0);
      if (bVar36) {
        puVar14 = puVar17 + uVar6;
      }
      else {
        *DAT_8c61e4c8 = *DAT_8c61e4c8 + uVar6;
        if (in_FPSCR_SZ == '\0') {
          dVar28 = (double)CONCAT44(*(undefined4 *)pdVar21,*(undefined4 *)pdVar20);
        }
        else {
          in_xd2 = *pdVar20;
          dVar28 = *pdVar21;
        }
        if (in_FPSCR_PR == '\0') {
          dVar28 = (double)CONCAT44((float)((ulonglong)dVar28 >> 0x20) + SUB84(dVar28,0),
                                    SUB84(dVar28,0));
        }
        else {
          dVar28 = dVar28 + dVar28;
        }
        if (in_FPSCR_SZ == '\0') {
          dVar28 = (double)CONCAT44((int)((ulonglong)dVar28 >> 0x20),*(undefined4 *)pdVar19);
        }
        else {
          in_xd2 = *pdVar19;
        }
        if (in_FPSCR_PR == '\0') {
          dVar28 = (double)CONCAT44((float)((ulonglong)dVar28 >> 0x20) + SUB84(dVar28,0),
                                    SUB84(dVar28,0));
        }
        else {
          dVar28 = dVar28 + dVar28;
        }
        if (in_FPSCR_SZ == '\0') {
          *(int *)((int)puVar22 + -0x10) = (int)((ulonglong)dVar28 >> 0x20);
          dVar28 = (double)CONCAT44(*(undefined4 *)((int)pdVar21 + 4),
                                    *(undefined4 *)((int)pdVar20 + 4));
        }
        else {
          *(double *)((int)puVar22 + -0x10) = dVar28;
          dVar28 = *(double *)((int)pdVar21 + 4);
          in_xd2 = *(double *)((int)pdVar20 + 4);
        }
        if (in_FPSCR_PR == '\0') {
          dVar28 = (double)CONCAT44((float)((ulonglong)dVar28 >> 0x20) + SUB84(dVar28,0),
                                    SUB84(dVar28,0));
        }
        else {
          dVar28 = dVar28 + dVar28;
        }
        if (in_FPSCR_SZ == '\0') {
          dVar28 = (double)CONCAT44((int)((ulonglong)dVar28 >> 0x20),
                                    *(undefined4 *)((int)pdVar19 + 4));
        }
        else {
          in_xd2 = *(double *)((int)pdVar19 + 4);
        }
        if (in_FPSCR_PR == '\0') {
          dVar28 = (double)CONCAT44((float)((ulonglong)dVar28 >> 0x20) + SUB84(dVar28,0),
                                    SUB84(dVar28,0));
        }
        else {
          dVar28 = dVar28 + dVar28;
        }
        if (in_FPSCR_SZ == '\0') {
          *(int *)((int)puVar22 + -0xc) = (int)((ulonglong)dVar28 >> 0x20);
          dVar28 = (double)CONCAT44(*(undefined4 *)(pdVar21 + 1),*(undefined4 *)(pdVar20 + 1));
        }
        else {
          *(double *)((int)puVar22 + -0xc) = dVar28;
          in_xd2 = pdVar20[1];
          dVar28 = pdVar21[1];
        }
        if (in_FPSCR_PR == '\0') {
          dVar28 = (double)CONCAT44((float)((ulonglong)dVar28 >> 0x20) + SUB84(dVar28,0),
                                    SUB84(dVar28,0));
        }
        else {
          dVar28 = dVar28 + dVar28;
        }
        if (in_FPSCR_SZ == '\0') {
          dVar28 = (double)CONCAT44((int)((ulonglong)dVar28 >> 0x20),*(undefined4 *)(pdVar19 + 1));
        }
        else {
          in_xd2 = pdVar19[1];
        }
        if (in_FPSCR_PR == '\0') {
          dVar28 = (double)CONCAT44((float)((ulonglong)dVar28 >> 0x20) + SUB84(dVar28,0),
                                    SUB84(dVar28,0));
        }
        else {
          dVar28 = dVar28 + dVar28;
        }
        if (in_FPSCR_SZ == '\0') {
          *(int *)((int)puVar22 + -8) = (int)((ulonglong)dVar28 >> 0x20);
        }
        else {
          *(double *)((int)puVar22 + -8) = dVar28;
        }
        dVar28 = (double)(*(code *)PTR_FUN_8c61e4cc)((undefined1 *)((int)puVar22 + -0x10));
        piVar4 = DAT_8c61e4e0;
        if (in_FPSCR_SZ == '\0') {
          dVar33 = (double)CONCAT44(*(undefined4 *)DAT_8c61e4d0,
                                    *(undefined4 *)((int)puVar22 + -0x10));
        }
        else {
          in_xd2 = *(double *)((int)puVar22 + -0x10);
          dVar33 = *DAT_8c61e4d0;
        }
        if (in_FPSCR_PR == '\0') {
          dVar33 = (double)CONCAT44((float)((ulonglong)dVar33 >> 0x20) * SUB84(dVar33,0),
                                    SUB84(dVar33,0));
        }
        else {
          dVar33 = dVar33 * dVar33;
        }
        if (in_FPSCR_SZ == '\0') {
          dVar33 = (double)CONCAT44((int)((ulonglong)dVar33 >> 0x20),*(undefined4 *)DAT_8c61e4d4);
          dVar28 = (double)CONCAT44((int)((ulonglong)dVar28 >> 0x20),*DAT_8c61e4d8);
        }
        else {
          in_xd2 = *DAT_8c61e4d4;
        }
        if (in_FPSCR_PR == '\0') {
          fVar26 = SUB84(dVar33,0) - (float)((ulonglong)dVar33 >> 0x20);
        }
        else {
          fVar26 = SUB84(dVar33 - dVar33,0);
        }
        if (in_FPSCR_SZ == '\0') {
          dVar33 = (double)CONCAT44(*(undefined4 *)((int)puVar22 + -0xc),fVar26);
        }
        else {
          dVar33 = *(double *)((int)puVar22 + -0xc);
        }
        if (in_FPSCR_PR == '\0') {
          dVar28 = (double)CONCAT44((int)((ulonglong)dVar28 >> 0x20),
                                    SUB84(dVar28,0) * (float)((ulonglong)dVar33 >> 0x20));
        }
        else {
          dVar28 = dVar28 * dVar33;
        }
        if (in_FPSCR_SZ == '\0') {
          dVar33 = (double)CONCAT44(*(undefined4 *)((int)puVar22 + -8),SUB84(dVar33,0));
        }
        else {
          dVar33 = *(double *)((int)puVar22 + -8);
        }
        if (in_FPSCR_PR == '\0') {
          dVar33 = (double)CONCAT44((int)((ulonglong)dVar33 >> 0x20),
                                    SUB84(dVar33,0) - SUB84(dVar28,0));
        }
        else {
          dVar33 = dVar33 - dVar28;
        }
        if (in_FPSCR_SZ == '\0') {
          dVar28 = (double)CONCAT44((int)((ulonglong)dVar28 >> 0x20),
                                    *(undefined4 *)(DAT_8c61e4d0 + 1));
        }
        iVar10 = *DAT_8c61e4e0;
        if (in_FPSCR_PR == '\0') {
          dVar28 = (double)CONCAT44((int)((ulonglong)dVar28 >> 0x20),
                                    SUB84(dVar28,0) * (float)((ulonglong)dVar33 >> 0x20));
        }
        else {
          dVar28 = dVar28 * dVar33;
        }
        puVar18 = (undefined8 *)((uint)*puVar17 * 0x20 + *DAT_8c61e4dc);
        param_1 = in_xd2;
        if (in_FPSCR_SZ == '\0') {
          param_1 = (double)CONCAT44(SUB84(dVar33,0),uVar32);
        }
        if (in_FPSCR_PR == '\0') {
          param_1 = (double)CONCAT44((float)((ulonglong)param_1 >> 0x20) - SUB84(dVar28,0),
                                     SUB84(param_1,0));
        }
        else {
          param_1 = param_1 - dVar28;
        }
        if (in_FPSCR_SZ == '\0') {
          dVar28 = (double)CONCAT44(*(undefined4 *)((int)puVar18 + 4),SUB84(dVar28,0));
          dVar33 = (double)CONCAT44(*(undefined4 *)(iVar10 + 4),*(undefined4 *)(iVar10 + 0x14));
        }
        else {
          dVar28 = *(double *)((int)puVar18 + 4);
          in_xd2 = *(double *)(iVar10 + 0x14);
          dVar33 = *(double *)(iVar10 + 4);
        }
        if (in_FPSCR_PR == '\0') {
          dVar33 = (double)CONCAT44((int)((ulonglong)dVar33 >> 0x20),
                                    SUB84(dVar33,0) * (float)((ulonglong)dVar28 >> 0x20));
        }
        else {
          dVar33 = dVar33 * dVar28;
        }
        if (in_FPSCR_SZ == '\0') {
          uVar24 = CONCAT44(*(undefined4 *)puVar18,SUB84(dVar28,0));
        }
        else {
          uVar24 = *puVar18;
        }
        if (in_FPSCR_PR == '\0') {
          dVar33 = (double)(ulonglong)
                           (uint)((float)((ulonglong)uVar24 >> 0x20) *
                                  (float)((ulonglong)dVar33 >> 0x20) + SUB84(dVar33,0));
        }
        if (in_FPSCR_SZ == '\0') {
          uVar24 = CONCAT44(*(undefined4 *)(puVar18 + 1),(int)uVar24);
          uVar29 = CONCAT44(*(undefined4 *)(iVar10 + 0x24),SUB84(dVar33,0));
        }
        else {
          uVar24 = puVar18[1];
          uVar29 = *(ulonglong *)(iVar10 + 0x24);
        }
        if (in_FPSCR_PR == '\0') {
          uVar29 = (ulonglong)
                   (uint)((float)((ulonglong)uVar24 >> 0x20) * (float)(uVar29 >> 0x20) +
                         (float)uVar29);
        }
        uVar32 = (undefined4)in_dr14;
        dVar33 = in_xd14;
        dVar28 = in_xd14;
        if (in_FPSCR_SZ == '\0') {
          dVar33 = (double)CONCAT44(uVar32,(int)uVar29);
          dVar28 = (double)CONCAT44(uVar32,(int)uVar24);
        }
        if (in_FPSCR_PR == '\0') {
          dVar33 = (double)CONCAT44((float)((ulonglong)dVar28 >> 0x20) * SUB84(dVar33,0) +
                                    (float)((ulonglong)dVar33 >> 0x20),SUB84(dVar33,0));
        }
        if (in_FPSCR_SZ == '\0') {
          pdVar20 = (double *)(unaff_r8 + 0x14);
          *(int *)pdVar20 = (int)((ulonglong)dVar33 >> 0x20);
        }
        else {
          pdVar20 = (double *)(unaff_r8 + 0x10);
          *pdVar20 = dVar33;
        }
        uVar25 = DAT_8c61e4e4;
        pdVar21 = (double *)*piVar4;
        if (in_FPSCR_SZ == '\0') {
          dVar28 = (double)CONCAT44(*(undefined4 *)((int)puVar18 + 4),SUB84(dVar28,0));
          dVar33 = (double)CONCAT44(*(undefined4 *)pdVar21,*(undefined4 *)(pdVar21 + 2));
        }
        else {
          dVar28 = *(double *)((int)puVar18 + 4);
          in_xd2 = pdVar21[2];
          dVar33 = *pdVar21;
        }
        if (in_FPSCR_PR == '\0') {
          dVar33 = (double)CONCAT44((int)((ulonglong)dVar33 >> 0x20),
                                    SUB84(dVar33,0) * (float)((ulonglong)dVar28 >> 0x20));
        }
        else {
          dVar33 = dVar33 * dVar28;
        }
        if (in_FPSCR_SZ == '\0') {
          uVar24 = CONCAT44(*(undefined4 *)puVar18,SUB84(dVar28,0));
        }
        else {
          uVar24 = *puVar18;
        }
        if (in_FPSCR_PR == '\0') {
          dVar33 = (double)(ulonglong)
                           (uint)((float)((ulonglong)uVar24 >> 0x20) *
                                  (float)((ulonglong)dVar33 >> 0x20) + SUB84(dVar33,0));
        }
        if (in_FPSCR_SZ == '\0') {
          uVar24 = CONCAT44(*(undefined4 *)(puVar18 + 1),(int)uVar24);
          dVar28 = (double)CONCAT44(*(undefined4 *)(pdVar21 + 4),SUB84(dVar33,0));
        }
        else {
          uVar24 = puVar18[1];
          dVar28 = pdVar21[4];
        }
        if (in_FPSCR_PR == '\0') {
          dVar28 = (double)(ulonglong)
                           (uint)((float)((ulonglong)uVar24 >> 0x20) *
                                  (float)((ulonglong)dVar28 >> 0x20) + SUB84(dVar28,0));
        }
        dVar23 = in_xd14;
        dVar33 = in_xd14;
        if (in_FPSCR_SZ == '\0') {
          dVar23 = (double)CONCAT44(uVar32,SUB84(dVar28,0));
          dVar33 = (double)CONCAT44(uVar32,(int)uVar24);
        }
        if (in_FPSCR_PR == '\0') {
          dVar23 = (double)CONCAT44((float)((ulonglong)dVar33 >> 0x20) * SUB84(dVar23,0) +
                                    (float)((ulonglong)dVar23 >> 0x20),SUB84(dVar23,0));
        }
        if (in_FPSCR_SZ == '\0') {
          *(int *)((int)pdVar20 + -4) = (int)((ulonglong)dVar23 >> 0x20);
          *(undefined4 *)(pdVar20 + -1) = *(undefined4 *)(puVar18 + 3);
          *(undefined4 *)((int)pdVar20 + -0xc) = *(undefined4 *)((int)puVar18 + 0x14);
          pdVar20 = pdVar20 + -2;
          *(undefined4 *)pdVar20 = *(undefined4 *)(puVar18 + 2);
        }
        else {
          pdVar20[-1] = dVar23;
          pdVar20[-2] = (double)puVar18[3];
          pdVar20[-3] = *(double *)((int)puVar18 + 0x14);
          in_xd2 = (double)puVar18[2];
          pdVar20 = pdVar20 + -4;
          *pdVar20 = in_xd2;
        }
        *(undefined4 *)((int)pdVar20 + -4) = uVar25;
        piVar4 = DAT_8c61e4e0;
        puVar17 = puVar14 + 3;
        iVar10 = *DAT_8c61e4e0;
        puVar18 = (undefined8 *)((uint)puVar14[2] * 0x20 + *DAT_8c61e4dc);
        if (in_FPSCR_SZ == '\0') {
          dVar28 = (double)CONCAT44(*(undefined4 *)((int)puVar18 + 4),SUB84(dVar33,0));
          dVar33 = (double)CONCAT44(*(undefined4 *)(iVar10 + 4),*(undefined4 *)(iVar10 + 0x14));
        }
        else {
          dVar28 = *(double *)((int)puVar18 + 4);
          in_xd2 = *(double *)(iVar10 + 0x14);
          dVar33 = *(double *)(iVar10 + 4);
        }
        if (in_FPSCR_PR == '\0') {
          dVar33 = (double)CONCAT44((int)((ulonglong)dVar33 >> 0x20),
                                    SUB84(dVar33,0) * (float)((ulonglong)dVar28 >> 0x20));
        }
        else {
          dVar33 = dVar33 * dVar28;
        }
        if (in_FPSCR_SZ == '\0') {
          uVar24 = CONCAT44(*(undefined4 *)puVar18,SUB84(dVar28,0));
        }
        else {
          uVar24 = *puVar18;
        }
        if (in_FPSCR_PR == '\0') {
          dVar33 = (double)(ulonglong)
                           (uint)((float)((ulonglong)uVar24 >> 0x20) *
                                  (float)((ulonglong)dVar33 >> 0x20) + SUB84(dVar33,0));
        }
        if (in_FPSCR_SZ == '\0') {
          uVar24 = CONCAT44(*(undefined4 *)(puVar18 + 1),(int)uVar24);
          uVar29 = CONCAT44(*(undefined4 *)(iVar10 + 0x24),SUB84(dVar33,0));
        }
        else {
          uVar24 = puVar18[1];
          uVar29 = *(ulonglong *)(iVar10 + 0x24);
        }
        if (in_FPSCR_PR == '\0') {
          uVar29 = (ulonglong)
                   (uint)((float)((ulonglong)uVar24 >> 0x20) * (float)(uVar29 >> 0x20) +
                         (float)uVar29);
        }
        dVar33 = in_xd14;
        dVar28 = in_xd14;
        if (in_FPSCR_SZ == '\0') {
          dVar33 = (double)CONCAT44(uVar32,(int)uVar29);
          dVar28 = (double)CONCAT44(uVar32,(int)uVar24);
        }
        if (in_FPSCR_PR == '\0') {
          dVar33 = (double)CONCAT44((float)((ulonglong)dVar28 >> 0x20) * SUB84(dVar33,0) +
                                    (float)((ulonglong)dVar33 >> 0x20),SUB84(dVar33,0));
        }
        if (in_FPSCR_SZ == '\0') {
          pdVar20 = pdVar20 + 6;
          *(int *)pdVar20 = (int)((ulonglong)dVar33 >> 0x20);
        }
        else {
          pdVar20 = (double *)((int)pdVar20 + 0x2c);
          *pdVar20 = dVar33;
        }
        uVar25 = DAT_8c61e4e4;
        pdVar21 = (double *)*piVar4;
        if (in_FPSCR_SZ == '\0') {
          dVar28 = (double)CONCAT44(*(undefined4 *)((int)puVar18 + 4),SUB84(dVar28,0));
          dVar33 = (double)CONCAT44(*(undefined4 *)pdVar21,*(undefined4 *)(pdVar21 + 2));
        }
        else {
          dVar28 = *(double *)((int)puVar18 + 4);
          in_xd2 = pdVar21[2];
          dVar33 = *pdVar21;
        }
        if (in_FPSCR_PR == '\0') {
          dVar33 = (double)CONCAT44((int)((ulonglong)dVar33 >> 0x20),
                                    SUB84(dVar33,0) * (float)((ulonglong)dVar28 >> 0x20));
        }
        else {
          dVar33 = dVar33 * dVar28;
        }
        if (in_FPSCR_SZ == '\0') {
          uVar24 = CONCAT44(*(undefined4 *)puVar18,SUB84(dVar28,0));
        }
        else {
          uVar24 = *puVar18;
        }
        if (in_FPSCR_PR == '\0') {
          dVar33 = (double)(ulonglong)
                           (uint)((float)((ulonglong)uVar24 >> 0x20) *
                                  (float)((ulonglong)dVar33 >> 0x20) + SUB84(dVar33,0));
        }
        if (in_FPSCR_SZ == '\0') {
          uVar24 = CONCAT44(*(undefined4 *)(puVar18 + 1),(int)uVar24);
          dVar28 = (double)CONCAT44(*(undefined4 *)(pdVar21 + 4),SUB84(dVar33,0));
        }
        else {
          uVar24 = puVar18[1];
          dVar28 = pdVar21[4];
        }
        if (in_FPSCR_PR == '\0') {
          dVar28 = (double)(ulonglong)
                           (uint)((float)((ulonglong)uVar24 >> 0x20) *
                                  (float)((ulonglong)dVar28 >> 0x20) + SUB84(dVar28,0));
        }
        dVar23 = in_xd14;
        dVar33 = in_xd14;
        if (in_FPSCR_SZ == '\0') {
          dVar23 = (double)CONCAT44(uVar32,SUB84(dVar28,0));
          dVar33 = (double)CONCAT44(uVar32,(int)uVar24);
        }
        if (in_FPSCR_PR == '\0') {
          dVar23 = (double)CONCAT44((float)((ulonglong)dVar33 >> 0x20) * SUB84(dVar23,0) +
                                    (float)((ulonglong)dVar23 >> 0x20),SUB84(dVar23,0));
        }
        if (in_FPSCR_SZ == '\0') {
          *(int *)((int)pdVar20 + -4) = (int)((ulonglong)dVar23 >> 0x20);
          *(undefined4 *)(pdVar20 + -1) = *(undefined4 *)(puVar18 + 3);
          *(undefined4 *)((int)pdVar20 + -0xc) = *(undefined4 *)((int)puVar18 + 0x14);
          pdVar20 = pdVar20 + -2;
          *(undefined4 *)pdVar20 = *(undefined4 *)(puVar18 + 2);
        }
        else {
          pdVar20[-1] = dVar23;
          pdVar20[-2] = (double)puVar18[3];
          pdVar20[-3] = *(double *)((int)puVar18 + 0x14);
          in_xd2 = (double)puVar18[2];
          pdVar20 = pdVar20 + -4;
          *pdVar20 = in_xd2;
        }
        *(undefined4 *)((int)pdVar20 + -4) = uVar25;
        iVar10 = 2;
        iVar12 = (int)pdVar20 + 0x1c;
        uVar25 = (undefined4)((ulonglong)param_1 >> 0x20);
        if (2 < (int)(uVar6 - 1)) {
          do {
            uVar1 = *puVar17;
            puVar17 = puVar17 + 1;
            iVar9 = *DAT_8c61e4dc;
            if (in_FPSCR_SZ == '\0') {
              pdVar20 = (double *)(iVar12 + 0x18);
              *(undefined4 *)pdVar20 = uVar25;
            }
            else {
              pdVar20 = (double *)(iVar12 + 0x14);
              *pdVar20 = param_1;
            }
            piVar4 = DAT_8c61e4e0;
            puVar18 = (undefined8 *)((uint)uVar1 * 0x20 + iVar9);
            if (in_FPSCR_SZ == '\0') {
              dVar28 = (double)CONCAT44(*(undefined4 *)((int)puVar18 + 4),SUB84(dVar33,0));
            }
            else {
              dVar28 = *(double *)((int)puVar18 + 4);
            }
            iVar12 = *DAT_8c61e4e0;
            if (in_FPSCR_SZ == '\0') {
              dVar33 = (double)CONCAT44(*(undefined4 *)(iVar12 + 4),*(undefined4 *)(iVar12 + 0x14));
            }
            else {
              in_xd2 = *(double *)(iVar12 + 0x14);
              dVar33 = *(double *)(iVar12 + 4);
            }
            if (in_FPSCR_PR == '\0') {
              dVar33 = (double)CONCAT44((int)((ulonglong)dVar33 >> 0x20),
                                        SUB84(dVar33,0) * (float)((ulonglong)dVar28 >> 0x20));
            }
            else {
              dVar33 = dVar33 * dVar28;
            }
            if (in_FPSCR_SZ == '\0') {
              uVar24 = CONCAT44(*(undefined4 *)puVar18,SUB84(dVar28,0));
            }
            else {
              uVar24 = *puVar18;
            }
            if (in_FPSCR_PR == '\0') {
              dVar33 = (double)(ulonglong)
                               (uint)((float)((ulonglong)uVar24 >> 0x20) *
                                      (float)((ulonglong)dVar33 >> 0x20) + SUB84(dVar33,0));
            }
            if (in_FPSCR_SZ == '\0') {
              uVar24 = CONCAT44(*(undefined4 *)(puVar18 + 1),(int)uVar24);
              uVar29 = CONCAT44(*(undefined4 *)(iVar12 + 0x24),SUB84(dVar33,0));
            }
            else {
              uVar24 = puVar18[1];
              uVar29 = *(ulonglong *)(iVar12 + 0x24);
            }
            if (in_FPSCR_PR == '\0') {
              uVar29 = (ulonglong)
                       (uint)((float)((ulonglong)uVar24 >> 0x20) * (float)(uVar29 >> 0x20) +
                             (float)uVar29);
            }
            dVar33 = in_xd14;
            dVar28 = in_xd14;
            if (in_FPSCR_SZ == '\0') {
              dVar33 = (double)CONCAT44(uVar32,(int)uVar24);
              dVar28 = (double)CONCAT44(uVar32,(int)uVar29);
            }
            if (in_FPSCR_PR == '\0') {
              dVar28 = (double)CONCAT44((float)((ulonglong)dVar33 >> 0x20) * SUB84(dVar28,0) +
                                        (float)((ulonglong)dVar28 >> 0x20),SUB84(dVar28,0));
            }
            if (in_FPSCR_SZ == '\0') {
              pdVar20 = (double *)((int)pdVar20 + -4);
              *(int *)pdVar20 = (int)((ulonglong)dVar28 >> 0x20);
            }
            else {
              pdVar20 = pdVar20 + -1;
              *pdVar20 = dVar28;
            }
            uVar30 = DAT_8c61e4e4;
            pdVar21 = (double *)*piVar4;
            if (in_FPSCR_SZ == '\0') {
              dVar28 = (double)CONCAT44(*(undefined4 *)((int)puVar18 + 4),SUB84(dVar33,0));
              dVar33 = (double)CONCAT44(*(undefined4 *)pdVar21,*(undefined4 *)(pdVar21 + 2));
            }
            else {
              dVar28 = *(double *)((int)puVar18 + 4);
              in_xd2 = pdVar21[2];
              dVar33 = *pdVar21;
            }
            if (in_FPSCR_PR == '\0') {
              dVar33 = (double)CONCAT44((int)((ulonglong)dVar33 >> 0x20),
                                        SUB84(dVar33,0) * (float)((ulonglong)dVar28 >> 0x20));
            }
            else {
              dVar33 = dVar33 * dVar28;
            }
            if (in_FPSCR_SZ == '\0') {
              uVar24 = CONCAT44(*(undefined4 *)puVar18,SUB84(dVar28,0));
            }
            else {
              uVar24 = *puVar18;
            }
            if (in_FPSCR_PR == '\0') {
              dVar33 = (double)(ulonglong)
                               (uint)((float)((ulonglong)uVar24 >> 0x20) *
                                      (float)((ulonglong)dVar33 >> 0x20) + SUB84(dVar33,0));
            }
            if (in_FPSCR_SZ == '\0') {
              uVar24 = CONCAT44(*(undefined4 *)(puVar18 + 1),(int)uVar24);
              dVar28 = (double)CONCAT44(*(undefined4 *)(pdVar21 + 4),SUB84(dVar33,0));
            }
            else {
              uVar24 = puVar18[1];
              dVar28 = pdVar21[4];
            }
            if (in_FPSCR_PR == '\0') {
              dVar28 = (double)(ulonglong)
                               (uint)((float)((ulonglong)uVar24 >> 0x20) *
                                      (float)((ulonglong)dVar28 >> 0x20) + SUB84(dVar28,0));
            }
            dVar23 = in_xd14;
            dVar33 = in_xd14;
            if (in_FPSCR_SZ == '\0') {
              dVar23 = (double)CONCAT44(uVar32,SUB84(dVar28,0));
              dVar33 = (double)CONCAT44(uVar32,(int)uVar24);
            }
            if (in_FPSCR_PR == '\0') {
              dVar23 = (double)CONCAT44((float)((ulonglong)dVar33 >> 0x20) * SUB84(dVar23,0) +
                                        (float)((ulonglong)dVar23 >> 0x20),SUB84(dVar23,0));
            }
            if (in_FPSCR_SZ == '\0') {
              *(int *)((int)pdVar20 + -4) = (int)((ulonglong)dVar23 >> 0x20);
              *(undefined4 *)(pdVar20 + -1) = *(undefined4 *)(puVar18 + 3);
              *(undefined4 *)((int)pdVar20 + -0xc) = *(undefined4 *)((int)puVar18 + 0x14);
              pdVar20 = pdVar20 + -2;
              *(undefined4 *)pdVar20 = *(undefined4 *)(puVar18 + 2);
            }
            else {
              pdVar20[-1] = dVar23;
              pdVar20[-2] = (double)puVar18[3];
              pdVar20[-3] = *(double *)((int)puVar18 + 0x14);
              in_xd2 = (double)puVar18[2];
              pdVar20 = pdVar20 + -4;
              *pdVar20 = in_xd2;
            }
            *(undefined4 *)((int)pdVar20 + -4) = uVar30;
            iVar10 = iVar10 + 1;
            iVar12 = (int)pdVar20 + 0x1c;
          } while (iVar10 < (int)(uVar6 - 1));
        }
        uVar1 = *puVar17;
        puVar14 = puVar17 + 1;
        iVar10 = *DAT_8c61e4dc;
        if (in_FPSCR_SZ == '\0') {
          pdVar20 = (double *)(iVar12 + 0x18);
          *(undefined4 *)pdVar20 = uVar25;
        }
        else {
          pdVar20 = (double *)(iVar12 + 0x14);
          *pdVar20 = param_1;
        }
        piVar4 = DAT_8c61e4e0;
        puVar18 = (undefined8 *)((uint)uVar1 * 0x20 + iVar10);
        if (in_FPSCR_SZ == '\0') {
          dVar28 = (double)CONCAT44(*(undefined4 *)((int)puVar18 + 4),SUB84(dVar33,0));
        }
        else {
          dVar28 = *(double *)((int)puVar18 + 4);
        }
        iVar10 = *DAT_8c61e4e0;
        if (in_FPSCR_SZ == '\0') {
          dVar33 = (double)CONCAT44(*(undefined4 *)(iVar10 + 4),*(undefined4 *)(iVar10 + 0x14));
        }
        else {
          in_xd2 = *(double *)(iVar10 + 0x14);
          dVar33 = *(double *)(iVar10 + 4);
        }
        if (in_FPSCR_PR == '\0') {
          dVar33 = (double)CONCAT44((int)((ulonglong)dVar33 >> 0x20),
                                    SUB84(dVar33,0) * (float)((ulonglong)dVar28 >> 0x20));
        }
        else {
          dVar33 = dVar33 * dVar28;
        }
        if (in_FPSCR_SZ == '\0') {
          uVar24 = CONCAT44(*(undefined4 *)puVar18,SUB84(dVar28,0));
        }
        else {
          uVar24 = *puVar18;
        }
        if (in_FPSCR_PR == '\0') {
          dVar33 = (double)(ulonglong)
                           (uint)((float)((ulonglong)uVar24 >> 0x20) *
                                  (float)((ulonglong)dVar33 >> 0x20) + SUB84(dVar33,0));
        }
        if (in_FPSCR_SZ == '\0') {
          uVar24 = CONCAT44(*(undefined4 *)(puVar18 + 1),(int)uVar24);
          uVar29 = CONCAT44(*(undefined4 *)(iVar10 + 0x24),SUB84(dVar33,0));
        }
        else {
          uVar24 = puVar18[1];
          uVar29 = *(ulonglong *)(iVar10 + 0x24);
        }
        if (in_FPSCR_PR == '\0') {
          uVar29 = (ulonglong)
                   (uint)((float)((ulonglong)uVar24 >> 0x20) * (float)(uVar29 >> 0x20) +
                         (float)uVar29);
        }
        dVar33 = in_xd14;
        dVar28 = in_xd14;
        if (in_FPSCR_SZ == '\0') {
          dVar33 = (double)CONCAT44(uVar32,(int)uVar24);
          dVar28 = (double)CONCAT44(uVar32,(int)uVar29);
        }
        if (in_FPSCR_PR == '\0') {
          dVar28 = (double)CONCAT44((float)((ulonglong)dVar33 >> 0x20) * SUB84(dVar28,0) +
                                    (float)((ulonglong)dVar28 >> 0x20),SUB84(dVar28,0));
        }
        if (in_FPSCR_SZ == '\0') {
          pdVar20 = (double *)((int)pdVar20 + -4);
          *(int *)pdVar20 = (int)((ulonglong)dVar28 >> 0x20);
        }
        else {
          pdVar20 = pdVar20 + -1;
          *pdVar20 = dVar28;
        }
        uVar25 = DAT_8c61e4e8;
        pdVar21 = (double *)*piVar4;
        if (in_FPSCR_SZ == '\0') {
          dVar28 = (double)CONCAT44(*(undefined4 *)((int)puVar18 + 4),SUB84(dVar33,0));
          dVar33 = (double)CONCAT44(*(undefined4 *)pdVar21,*(undefined4 *)(pdVar21 + 2));
        }
        else {
          dVar28 = *(double *)((int)puVar18 + 4);
          in_xd2 = pdVar21[2];
          dVar33 = *pdVar21;
        }
        if (in_FPSCR_PR == '\0') {
          dVar33 = (double)CONCAT44((int)((ulonglong)dVar33 >> 0x20),
                                    SUB84(dVar33,0) * (float)((ulonglong)dVar28 >> 0x20));
        }
        else {
          dVar33 = dVar33 * dVar28;
        }
        if (in_FPSCR_SZ == '\0') {
          uVar24 = CONCAT44(*(undefined4 *)puVar18,SUB84(dVar28,0));
        }
        else {
          uVar24 = *puVar18;
        }
        if (in_FPSCR_PR == '\0') {
          dVar33 = (double)(ulonglong)
                           (uint)((float)((ulonglong)uVar24 >> 0x20) *
                                  (float)((ulonglong)dVar33 >> 0x20) + SUB84(dVar33,0));
        }
        if (in_FPSCR_SZ == '\0') {
          uVar24 = CONCAT44(*(undefined4 *)(puVar18 + 1),(int)uVar24);
          dVar28 = (double)CONCAT44(*(undefined4 *)(pdVar21 + 4),SUB84(dVar33,0));
        }
        else {
          uVar24 = puVar18[1];
          dVar28 = pdVar21[4];
        }
        if (in_FPSCR_PR == '\0') {
          dVar28 = (double)(ulonglong)
                           (uint)((float)((ulonglong)uVar24 >> 0x20) *
                                  (float)((ulonglong)dVar28 >> 0x20) + SUB84(dVar28,0));
        }
        dVar33 = in_xd14;
        in_dr0 = in_xd14;
        if (in_FPSCR_SZ == '\0') {
          dVar33 = (double)CONCAT44(uVar32,SUB84(dVar28,0));
          in_dr0 = (double)CONCAT44(uVar32,(int)uVar24);
        }
        if (in_FPSCR_PR == '\0') {
          dVar33 = (double)CONCAT44((float)((ulonglong)in_dr0 >> 0x20) * SUB84(dVar33,0) +
                                    (float)((ulonglong)dVar33 >> 0x20),SUB84(dVar33,0));
        }
        if (in_FPSCR_SZ == '\0') {
          *(int *)((int)pdVar20 + -4) = (int)((ulonglong)dVar33 >> 0x20);
          *(undefined4 *)(pdVar20 + -1) = *(undefined4 *)(puVar18 + 3);
          *(undefined4 *)((int)pdVar20 + -0xc) = *(undefined4 *)((int)puVar18 + 0x14);
          pdVar20 = pdVar20 + -2;
          *(undefined4 *)pdVar20 = *(undefined4 *)(puVar18 + 2);
        }
        else {
          pdVar20[-1] = dVar33;
          pdVar20[-2] = (double)puVar18[3];
          pdVar20[-3] = *(double *)((int)puVar18 + 0x14);
          in_xd2 = (double)puVar18[2];
          pdVar20 = pdVar20 + -4;
          *pdVar20 = in_xd2;
        }
        *(undefined4 *)((int)pdVar20 + -4) = uVar25;
        unaff_r8 = (int)pdVar20 + 0x1c;
      }
    }
  }
  else if (uVar6 == DAT_8c61dc14) {
    *DAT_8c61e4ec = *(undefined4 *)((int)puVar22 + -0x14);
    fVar26 = (float)((ulonglong)param_1 >> 0x20);
    fVar34 = (float)((ulonglong)param_3 >> 0x20);
    if (in_FPSCR_SZ == '\0') {
      *(int *)((int)puVar22 + -0x18) = (int)((ulonglong)in_dr0 >> 0x20);
      *(int *)((int)puVar22 + -0x1c) = SUB84(in_dr0,0);
      *(int *)((int)puVar22 + -0x20) = (int)((ulonglong)in_dr2 >> 0x20);
      *(int *)((int)puVar22 + -0x24) = SUB84(in_dr2,0);
      *(float *)((int)puVar22 + -0x28) = fVar26;
      *(float *)((int)puVar22 + -0x2c) = SUB84(param_1,0);
      *(int *)((int)puVar22 + -0x30) = (int)((ulonglong)param_2 >> 0x20);
      *(int *)((int)puVar22 + -0x34) = (int)param_2;
      *(float *)((int)puVar22 + -0x38) = fVar34;
      *(float *)((int)puVar22 + -0x3c) = SUB84(param_3,0);
      *(int *)((int)puVar22 + -0x40) = (int)((ulonglong)param_4 >> 0x20);
      *(int *)((int)puVar22 + -0x44) = (int)param_4;
      *(int *)((int)puVar22 + -0x48) = (int)((ulonglong)in_dr12 >> 0x20);
      *(int *)((int)puVar22 + -0x4c) = (int)in_dr12;
      *(int *)((int)puVar22 + -0x50) = (int)((ulonglong)in_dr14 >> 0x20);
      *(int *)((int)puVar22 + -0x54) = (int)in_dr14;
      fVar2 = DAT_8c61e4bc;
    }
    else {
      *(double *)((int)puVar22 + -0x1c) = in_dr0;
      *(undefined8 *)((int)puVar22 + -0x24) = in_xd0;
      *(double *)((int)puVar22 + -0x2c) = in_dr2;
      *(double *)((int)puVar22 + -0x34) = in_xd2;
      *(double *)((int)puVar22 + -0x3c) = param_1;
      *(double *)((int)puVar22 + -0x44) = in_xd4;
      *(undefined8 *)((int)puVar22 + -0x4c) = param_2;
      *(undefined8 *)((int)puVar22 + -0x54) = in_xd6;
      *(double *)((int)puVar22 + -0x5c) = param_3;
      *(undefined8 *)((int)puVar22 + -100) = in_xd8;
      *(undefined8 *)((int)puVar22 + -0x6c) = param_4;
      *(undefined8 *)((int)puVar22 + -0x74) = in_xd10;
      *(undefined8 *)((int)puVar22 + -0x7c) = in_dr12;
      *(undefined8 *)((int)puVar22 + -0x84) = in_xd12;
      *(undefined8 *)((int)puVar22 + -0x8c) = in_dr14;
      *(double *)((int)puVar22 + -0x94) = in_xd14;
      fVar2 = DAT_8c61e4c0;
    }
    dVar28 = (double)CONCAT44(fVar2,DAT_8c61e4bc);
    if (in_FPSCR_PR == '\0') {
      dVar33 = (double)CONCAT44(fVar26 * fVar2,SUB84(param_1,0) * fVar2);
      dVar28 = (double)CONCAT44(fVar34 * fVar2,SUB84(param_3,0) * fVar2);
    }
    else {
      dVar33 = param_1 * dVar28 * dVar28;
      dVar28 = param_3 * dVar28 * dVar28;
    }
    if (in_FPSCR_SZ == '\0') {
      dVar23 = (double)CONCAT44(*(undefined4 *)((int)DAT_8c61e4c0 + 4),(int)param_2);
      dVar35 = (double)CONCAT44(*(undefined4 *)((int)DAT_8c61e4c0 + 8),(int)param_4);
    }
    else {
      dVar23 = *(double *)((int)DAT_8c61e4c0 + 8);
      dVar35 = *(double *)((int)DAT_8c61e4c0 + 0x10);
    }
    if (in_FPSCR_PR == '\0') {
      dVar23 = (double)CONCAT44(-(float)((ulonglong)dVar23 >> 0x20),SUB84(dVar23,0));
      dVar35 = (double)CONCAT44(-(float)((ulonglong)dVar35 >> 0x20),SUB84(dVar35,0));
    }
    else {
      dVar23 = -dVar23;
      dVar35 = -dVar35;
    }
    (*(code *)PTR_FUN_8c61e4f0)(dVar33,dVar23,dVar28,dVar35,*DAT_8c61e4ec);
  }
  return;
}

