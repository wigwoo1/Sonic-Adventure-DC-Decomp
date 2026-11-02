/* Ghidra draft only; not buildable/verified source. */

void FUN_8c63c96c(double param_1,double param_2,double *param_3)

{
  int iVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  undefined8 *puVar5;
  double in_dr0;
  double dVar6;
  double dVar7;
  undefined8 uVar8;
  undefined4 uVar9;
  float fVar10;
  double in_dr2;
  double dVar11;
  double dVar12;
  ulonglong uVar13;
  float fVar14;
  float fVar15;
  undefined8 in_dr14;
  double in_xd2;
  double in_xd4;
  double in_xd14;
  bool bVar16;
  undefined4 in_PR;
  char in_FPSCR_PR;
  char in_FPSCR_SZ;
  undefined1 auStack_24 [8];
  undefined8 uStack_1c;
  
  iVar1 = DAT_8c63c9ec;
  if (in_FPSCR_SZ == '\0') {
    uStack_1c = (double)CONCAT44((int)in_dr14,(undefined4)uStack_1c);
    puVar5 = &uStack_1c;
                    /* WARNING: Ignoring partial resolution of indirect */
    uStack_1c._0_4_ = (int)((ulonglong)in_dr14 >> 0x20);
  }
  else {
    puVar5 = (undefined8 *)auStack_24;
    uStack_1c = in_xd14;
  }
  *(undefined4 *)((int)puVar5 + -4) = in_PR;
  iVar3 = *(int *)(DAT_8c63c9c6 + DAT_8c63c9ec);
LAB_8c63cb6c:
  while( true ) {
    if (iVar3 == 0) {
      return;
    }
    iVar3 = iVar3 + -1;
    iVar4 = *(int *)(*(int *)(iVar3 * 4 + iVar1) * 4 + DAT_8c63c9f0);
    iVar2 = *(int *)(DAT_8c63c9c8 + iVar4);
    uVar9 = (undefined4)((ulonglong)in_dr2 >> 0x20);
    if ((((iVar2 != DAT_8c63c9ca) && (iVar2 != DAT_8c63c9cc)) && (iVar2 != DAT_8c63c9ce)) &&
       (iVar2 != DAT_8c63c9d0)) goto code_r0x8c63c9bc;
    if (in_FPSCR_SZ == '\0') {
      in_dr2 = (double)CONCAT44(uVar9,*(undefined4 *)param_3);
      param_2 = (double)CONCAT44(*(undefined4 *)(DAT_8c63cb2e + iVar4),SUB84(param_2,0));
      dVar6 = (double)CONCAT44(*(undefined4 *)((int)param_3 + 4),SUB84(in_dr0,0));
    }
    else {
      in_xd2 = *param_3;
      param_2 = *(double *)(DAT_8c63cb2e + iVar4);
      dVar6 = *(double *)((int)param_3 + 4);
    }
    if (in_FPSCR_PR == '\0') {
      param_2 = (double)CONCAT44((float)((ulonglong)param_2 >> 0x20) - SUB84(in_dr2,0),
                                 SUB84(param_2,0));
    }
    else {
      param_2 = param_2 - in_dr2;
    }
    if (in_FPSCR_SZ == '\0') {
      dVar11 = (double)CONCAT44(*(undefined4 *)(DAT_8c63cb30 + iVar4),SUB84(param_1,0));
      dVar7 = (double)CONCAT44(*(undefined4 *)(param_3 + 1),SUB84(in_dr2,0));
    }
    else {
      dVar11 = *(double *)(DAT_8c63cb30 + iVar4);
      dVar7 = param_3[1];
    }
    if (in_FPSCR_PR == '\0') {
      dVar11 = (double)CONCAT44((float)((ulonglong)dVar11 >> 0x20) -
                                (float)((ulonglong)dVar6 >> 0x20),SUB84(dVar11,0));
    }
    else {
      dVar11 = dVar11 - dVar6;
    }
    fVar10 = (float)((ulonglong)param_2 >> 0x20);
    fVar14 = (float)((ulonglong)dVar11 >> 0x20);
    if (in_FPSCR_SZ == '\0') {
      dVar7 = (double)CONCAT44((int)((ulonglong)dVar7 >> 0x20),fVar14);
      dVar11 = in_xd2;
      param_1 = (double)CONCAT44(fVar14,*(undefined4 *)(DAT_8c63cb32 + iVar4));
      dVar6 = (double)CONCAT44(fVar10,SUB84(dVar6,0));
    }
    else {
      in_xd4 = *(double *)(DAT_8c63cb32 + iVar4);
      param_1 = dVar11;
      dVar6 = param_2;
    }
    if (in_FPSCR_PR == '\0') {
      fVar15 = (float)((ulonglong)dVar7 >> 0x20);
      param_1 = (double)(ulonglong)(uint)(SUB84(param_1,0) - fVar15);
      in_dr2 = (double)CONCAT44(fVar15,(float)((ulonglong)dVar6 >> 0x20) * fVar10 +
                                       SUB84(dVar7,0) * fVar14);
    }
    else {
      in_dr2 = dVar7 * param_1;
      param_1 = param_1 - in_dr2;
    }
    fVar14 = SUB84(param_1,0);
    in_dr0 = in_xd4;
    if (in_FPSCR_SZ == '\0') {
      in_dr0 = (double)CONCAT44(fVar14,SUB84(dVar6,0));
    }
    if (in_FPSCR_PR == '\0') {
      in_dr2 = (double)CONCAT44((int)((ulonglong)in_dr2 >> 0x20),
                                (float)((ulonglong)in_dr0 >> 0x20) * fVar14 + SUB84(in_dr2,0));
    }
    if (in_FPSCR_SZ == '\0') {
      uVar9 = SUB84(in_dr2,0);
      in_dr2 = (double)CONCAT44((int)((ulonglong)in_dr2 >> 0x20),*(undefined4 *)(iVar4 + 0x6c));
      in_xd2 = dVar11;
      dVar11 = (double)CONCAT44(uVar9,0x3f800000);
    }
    else {
      in_xd2 = *(double *)(iVar4 + 0x6c);
    }
    fVar10 = (float)((ulonglong)dVar11 >> 0x20);
    if (in_FPSCR_PR == '\0') {
      bVar16 = fVar10 < SUB84(in_dr2,0);
    }
    else {
      bVar16 = dVar11 < in_dr2;
    }
    if (!bVar16) break;
    dVar6 = dVar11;
    if (in_FPSCR_SZ == '\0') {
      dVar6 = (double)CONCAT44(fVar10,fVar14);
    }
    uVar9 = SUB84(dVar6,0);
    in_dr0 = (double)(*(code *)PTR_FUN_8c63c9f4)();
    param_1 = in_xd14;
    if (in_FPSCR_SZ == '\0') {
      param_1 = (double)CONCAT44(SUB84(dVar11,0),uVar9);
    }
    if (in_FPSCR_PR == '\0') {
      param_1 = (double)CONCAT44((float)((ulonglong)param_1 >> 0x20) /
                                 (float)((ulonglong)in_dr0 >> 0x20),SUB84(param_1,0));
    }
    else {
      param_1 = param_1 / in_dr0;
    }
    iVar2 = (int)DAT_8c63cb2e;
    if (in_FPSCR_SZ == '\0') {
      dVar6 = (double)CONCAT44(*(undefined4 *)(iVar2 + iVar4),*(undefined4 *)param_3);
    }
    else {
      in_xd2 = *param_3;
      dVar6 = *(double *)(iVar2 + iVar4);
    }
    fVar14 = (float)((ulonglong)param_1 >> 0x20);
    if (in_FPSCR_PR == '\0') {
      dVar6 = (double)CONCAT44(((float)((ulonglong)dVar6 >> 0x20) - SUB84(dVar6,0)) * fVar14,
                               SUB84(dVar6,0));
    }
    else {
      dVar6 = (dVar6 - dVar6) * param_1;
    }
    if (in_FPSCR_SZ == '\0') {
      *(int *)(iVar2 + 0x18 + iVar4) = (int)((ulonglong)dVar6 >> 0x20);
      dVar6 = (double)(ulonglong)*(uint *)((int)param_3 + 4);
    }
    else {
      *(double *)(iVar2 + 0x18 + iVar4) = dVar6;
      in_xd2 = *(double *)((int)param_3 + 4);
    }
    iVar2 = (int)DAT_8c63cb30;
    if (in_FPSCR_SZ == '\0') {
      dVar6 = (double)CONCAT44(*(undefined4 *)(iVar2 + iVar4),SUB84(dVar6,0));
    }
    else {
      dVar6 = *(double *)(iVar2 + iVar4);
    }
    if (in_FPSCR_PR == '\0') {
      dVar6 = (double)CONCAT44(((float)((ulonglong)dVar6 >> 0x20) - SUB84(dVar6,0)) * fVar14,
                               SUB84(dVar6,0));
    }
    else {
      dVar6 = (dVar6 - dVar6) * param_1;
    }
    if (in_FPSCR_SZ == '\0') {
      *(int *)(iVar2 + 0x18 + iVar4) = (int)((ulonglong)dVar6 >> 0x20);
      dVar6 = (double)(ulonglong)*(uint *)(param_3 + 1);
    }
    else {
      *(double *)(iVar2 + 0x18 + iVar4) = dVar6;
      in_xd2 = param_3[1];
    }
    iVar2 = (int)DAT_8c63cb32;
    if (in_FPSCR_SZ == '\0') {
      dVar6 = (double)CONCAT44(*(undefined4 *)(iVar2 + iVar4),SUB84(dVar6,0));
    }
    else {
      dVar6 = *(double *)(iVar2 + iVar4);
    }
    if (in_FPSCR_PR == '\0') {
      in_dr2 = (double)CONCAT44(((float)((ulonglong)dVar6 >> 0x20) - SUB84(dVar6,0)) * fVar14,
                                SUB84(dVar6,0));
    }
    else {
      in_dr2 = (dVar6 - dVar6) * param_1;
    }
    uVar9 = (undefined4)((ulonglong)in_dr2 >> 0x20);
    if (in_FPSCR_SZ == '\0') {
      *(undefined4 *)(iVar2 + 0x18 + iVar4) = uVar9;
      in_dr2 = (double)CONCAT44(uVar9,*(undefined4 *)(iVar4 + 0x68));
    }
    else {
      *(double *)(iVar2 + 0x18 + iVar4) = in_dr2;
      in_xd2 = *(double *)(iVar4 + 0x68);
    }
    fVar14 = (float)((ulonglong)dVar11 >> 0x20);
    if (in_FPSCR_PR == '\0') {
      bVar16 = SUB84(in_dr2,0) < fVar14;
    }
    else {
      bVar16 = in_dr2 < dVar11;
    }
    if (bVar16) {
      if (in_FPSCR_SZ == '\0') {
        in_dr2 = (double)CONCAT44((int)((ulonglong)in_dr2 >> 0x20),*(undefined4 *)(iVar4 + 0x68));
      }
      else {
        in_xd2 = *(double *)(iVar4 + 0x68);
      }
      if (in_FPSCR_PR == '\0') {
        in_dr2 = (double)CONCAT44((int)((ulonglong)in_dr2 >> 0x20),SUB84(in_dr2,0) / fVar14);
      }
      else {
        in_dr2 = in_dr2 / dVar11;
      }
      if (in_FPSCR_SZ == '\0') {
        *(int *)(iVar4 + 0x60) = SUB84(in_dr2,0);
      }
      else {
        *(double *)(iVar4 + 0x60) = in_xd2;
      }
    }
    else if (in_FPSCR_SZ == '\0') {
      *(int *)(iVar4 + 0x60) = SUB84(dVar11,0);
    }
    else {
      *(double *)(iVar4 + 0x60) = in_xd14;
    }
  }
  goto LAB_8c63cb66;
code_r0x8c63c9bc:
  if (iVar2 == DAT_8c63c9d2) {
    if (in_FPSCR_SZ == '\0') {
      in_dr2 = (double)CONCAT44(uVar9,*(undefined4 *)param_3);
      param_2 = (double)CONCAT44(*(undefined4 *)(DAT_8c63cb2e + iVar4),SUB84(param_2,0));
      dVar6 = (double)CONCAT44(*(undefined4 *)((int)param_3 + 4),SUB84(in_dr0,0));
    }
    else {
      in_xd2 = *param_3;
      param_2 = *(double *)(DAT_8c63cb2e + iVar4);
      dVar6 = *(double *)((int)param_3 + 4);
    }
    if (in_FPSCR_PR == '\0') {
      param_2 = (double)CONCAT44((float)((ulonglong)param_2 >> 0x20) - SUB84(in_dr2,0),
                                 SUB84(param_2,0));
    }
    else {
      param_2 = param_2 - in_dr2;
    }
    if (in_FPSCR_SZ == '\0') {
      param_1 = (double)CONCAT44((int)((ulonglong)param_1 >> 0x20),
                                 *(undefined4 *)(DAT_8c63cb30 + iVar4));
      dVar11 = (double)CONCAT44(*(undefined4 *)(param_3 + 1),SUB84(in_dr2,0));
    }
    else {
      in_xd4 = *(double *)(DAT_8c63cb30 + iVar4);
      dVar11 = param_3[1];
    }
    if (in_FPSCR_PR == '\0') {
      fVar14 = SUB84(param_1,0) - (float)((ulonglong)dVar6 >> 0x20);
    }
    else {
      fVar14 = SUB84(param_1 - dVar6,0);
    }
    fVar10 = (float)((ulonglong)param_2 >> 0x20);
    if (in_FPSCR_SZ == '\0') {
      dVar6 = (double)CONCAT44(*(undefined4 *)(DAT_8c63cb32 + iVar4),fVar14);
      dVar11 = (double)CONCAT44((int)((ulonglong)dVar11 >> 0x20),fVar14);
      dVar7 = (double)((ulonglong)(uint)fVar10 << 0x20);
    }
    else {
      dVar6 = *(double *)(DAT_8c63cb32 + iVar4);
      fVar14 = SUB84(dVar6,0);
      in_xd2 = in_xd4;
      dVar7 = param_2;
    }
    if (in_FPSCR_PR == '\0') {
      fVar15 = (float)((ulonglong)dVar6 >> 0x20) - (float)((ulonglong)dVar11 >> 0x20);
      fVar10 = fVar15 * fVar15 +
               (float)((ulonglong)dVar7 >> 0x20) * fVar10 + SUB84(dVar11,0) * fVar14;
    }
    else {
      fVar14 = SUB84(dVar6 - dVar11 * dVar6,0);
      fVar10 = SUB84(dVar11 * dVar6,0);
    }
    dVar11 = in_xd2;
    dVar6 = in_xd2;
    if (in_FPSCR_SZ == '\0') {
      dVar11 = (double)CONCAT44(fVar10,fVar14);
      dVar6 = (double)CONCAT44(fVar10,0x3f800000);
    }
    uVar9 = SUB84(dVar11,0);
    dVar7 = (double)(*(code *)PTR_FUN_8c63c9f4)();
    dVar11 = in_xd14;
    if (in_FPSCR_SZ == '\0') {
      dVar11 = (double)CONCAT44(SUB84(dVar6,0),uVar9);
    }
    if (in_FPSCR_PR == '\0') {
      dVar11 = (double)CONCAT44((float)((ulonglong)dVar11 >> 0x20) /
                                (float)((ulonglong)dVar7 >> 0x20),SUB84(dVar11,0));
    }
    else {
      dVar11 = dVar11 / dVar7;
    }
    iVar2 = (int)DAT_8c63cb2e;
    if (in_FPSCR_SZ == '\0') {
      dVar12 = (double)CONCAT44(*(undefined4 *)(iVar2 + iVar4),*(undefined4 *)param_3);
    }
    else {
      in_xd2 = *param_3;
      dVar12 = *(double *)(iVar2 + iVar4);
    }
    fVar14 = (float)((ulonglong)dVar11 >> 0x20);
    if (in_FPSCR_PR == '\0') {
      dVar12 = (double)CONCAT44(((float)((ulonglong)dVar12 >> 0x20) - SUB84(dVar12,0)) * fVar14,
                                SUB84(dVar12,0));
    }
    else {
      dVar12 = (dVar12 - dVar12) * dVar11;
    }
    if (in_FPSCR_SZ == '\0') {
      *(int *)(iVar2 + 0x18 + iVar4) = (int)((ulonglong)dVar12 >> 0x20);
      dVar12 = (double)(ulonglong)*(uint *)((int)param_3 + 4);
    }
    else {
      *(double *)(iVar2 + 0x18 + iVar4) = dVar12;
      in_xd2 = *(double *)((int)param_3 + 4);
    }
    iVar2 = (int)DAT_8c63cb30;
    if (in_FPSCR_SZ == '\0') {
      dVar12 = (double)CONCAT44(*(undefined4 *)(iVar2 + iVar4),SUB84(dVar12,0));
    }
    else {
      dVar12 = *(double *)(iVar2 + iVar4);
    }
    if (in_FPSCR_PR == '\0') {
      dVar12 = (double)CONCAT44(((float)((ulonglong)dVar12 >> 0x20) - SUB84(dVar12,0)) * fVar14,
                                SUB84(dVar12,0));
    }
    else {
      dVar12 = (dVar12 - dVar12) * dVar11;
    }
    if (in_FPSCR_SZ == '\0') {
      *(int *)(iVar2 + 0x18 + iVar4) = (int)((ulonglong)dVar12 >> 0x20);
      dVar12 = (double)(ulonglong)*(uint *)(param_3 + 1);
    }
    else {
      *(double *)(iVar2 + 0x18 + iVar4) = dVar12;
      in_xd2 = param_3[1];
    }
    iVar2 = (int)DAT_8c63cb32;
    if (in_FPSCR_SZ == '\0') {
      dVar12 = (double)CONCAT44(*(undefined4 *)(iVar2 + iVar4),SUB84(dVar12,0));
    }
    else {
      dVar12 = *(double *)(iVar2 + iVar4);
    }
    if (in_FPSCR_PR == '\0') {
      dVar12 = (double)CONCAT44(((float)((ulonglong)dVar12 >> 0x20) - SUB84(dVar12,0)) * fVar14,
                                SUB84(dVar12,0));
    }
    else {
      dVar12 = (dVar12 - dVar12) * dVar11;
    }
    uVar9 = (undefined4)((ulonglong)dVar12 >> 0x20);
    if (in_FPSCR_SZ == '\0') {
      *(undefined4 *)(iVar2 + 0x18 + iVar4) = uVar9;
      dVar7 = (double)CONCAT44(*(undefined4 *)(iVar2 + 8 + iVar4),SUB84(dVar7,0));
      dVar12 = (double)CONCAT44(uVar9,*(undefined4 *)(iVar2 + 0x14 + iVar4));
    }
    else {
      *(double *)(iVar2 + 0x18 + iVar4) = dVar12;
      dVar7 = *(double *)(iVar2 + 8 + iVar4);
      in_xd2 = *(double *)(iVar2 + 0x14 + iVar4);
    }
    if (in_FPSCR_PR == '\0') {
      fVar14 = SUB84(dVar12,0) * (float)((ulonglong)dVar7 >> 0x20);
    }
    else {
      fVar14 = SUB84(dVar12 * dVar7,0);
    }
    if (in_FPSCR_SZ == '\0') {
      uVar8 = CONCAT44(*(undefined4 *)(iVar2 + 4 + iVar4),SUB84(dVar7,0));
      uVar13 = CONCAT44(*(undefined4 *)(iVar2 + 0x10 + iVar4),fVar14);
    }
    else {
      uVar8 = *(undefined8 *)(iVar2 + 4 + iVar4);
      uVar13 = *(ulonglong *)(iVar2 + 0x10 + iVar4);
    }
    if (in_FPSCR_PR == '\0') {
      uVar13 = (ulonglong)
               (uint)((float)((ulonglong)uVar8 >> 0x20) * (float)(uVar13 >> 0x20) + (float)uVar13);
    }
    if (in_FPSCR_SZ == '\0') {
      in_dr0 = (double)CONCAT44(*(undefined4 *)(iVar2 + 0xc + iVar4),(int)uVar8);
      dVar7 = (double)CONCAT44(*(undefined4 *)(iVar2 + 0x18 + iVar4),(int)uVar13);
    }
    else {
      in_dr0 = *(double *)(iVar2 + 0xc + iVar4);
      dVar7 = *(double *)(iVar2 + 0x18 + iVar4);
    }
    fVar14 = (float)((ulonglong)in_dr0 >> 0x20);
    if (in_FPSCR_PR == '\0') {
      fVar10 = (float)((ulonglong)dVar7 >> 0x20);
      dVar7 = (double)CONCAT44(fVar10,fVar14 * fVar10 + SUB84(dVar7,0));
    }
    param_1 = in_xd2;
    if (in_FPSCR_SZ == '\0') {
      param_1 = (double)CONCAT44(SUB84(dVar7,0),SUB84(dVar11,0));
    }
    fVar10 = (float)((ulonglong)param_1 >> 0x20);
    if (in_FPSCR_PR == '\0') {
      in_dr2 = (double)CONCAT44((int)((ulonglong)dVar7 >> 0x20),SUB84(dVar7,0) * fVar10);
    }
    else {
      in_dr2 = dVar7 * param_1;
    }
    if (in_FPSCR_SZ == '\0') {
      *(int *)(iVar4 + 100) = SUB84(in_dr2,0);
      in_dr2 = (double)CONCAT44((int)((ulonglong)in_dr2 >> 0x20),*(undefined4 *)(iVar4 + 0x6c));
    }
    else {
      *(double *)(iVar4 + 100) = in_xd2;
      in_xd2 = *(double *)(iVar4 + 0x6c);
    }
    fVar15 = (float)((ulonglong)dVar6 >> 0x20);
    if (in_FPSCR_PR == '\0') {
      bVar16 = fVar15 < SUB84(in_dr2,0);
    }
    else {
      bVar16 = dVar6 < in_dr2;
    }
    if (bVar16) {
      in_dr2 = (double)((ulonglong)in_dr2 & 0xffffffff00000000);
      if (in_FPSCR_PR == '\0') {
        bVar16 = fVar10 < 0.0;
      }
      else {
        bVar16 = param_1 < in_dr2;
      }
      if (bVar16) {
        if (in_FPSCR_SZ == '\0') {
          in_dr2 = (double)((ulonglong)*(uint *)(iVar4 + 0x74) << 0x20);
          in_dr0 = (double)CONCAT44(fVar14,*(undefined4 *)(iVar4 + 100));
        }
        else {
          in_dr2 = *(double *)(iVar4 + 0x74);
        }
        fVar14 = (float)((ulonglong)in_dr2 >> 0x20);
        if (in_FPSCR_PR == '\0') {
          bVar16 = fVar14 < SUB84(in_dr0,0);
        }
        else {
          bVar16 = in_dr2 < in_dr0;
        }
        if (bVar16) {
          if (in_FPSCR_SZ == '\0') {
            in_dr2 = (double)CONCAT44(fVar14,*(undefined4 *)(iVar4 + 0x68));
          }
          else {
            in_xd2 = *(double *)(iVar4 + 0x68);
          }
          if (in_FPSCR_PR == '\0') {
            bVar16 = SUB84(in_dr2,0) < fVar15;
          }
          else {
            bVar16 = in_dr2 < dVar6;
          }
          if (bVar16) {
            if (in_FPSCR_SZ == '\0') {
              in_dr2 = (double)CONCAT44((int)((ulonglong)in_dr2 >> 0x20),
                                        *(undefined4 *)(iVar4 + 0x68));
            }
            else {
              in_xd2 = *(double *)(iVar4 + 0x68);
            }
            if (in_FPSCR_PR == '\0') {
              fVar15 = SUB84(in_dr2,0) / fVar15;
            }
            else {
              fVar15 = SUB84(in_dr2 / dVar6,0);
            }
            if (in_FPSCR_SZ == '\0') {
              *(float *)(iVar4 + 0x60) = fVar15;
            }
            else {
              *(double *)(iVar4 + 0x60) = in_xd2;
            }
          }
          else if (in_FPSCR_SZ == '\0') {
            *(int *)(iVar4 + 0x60) = SUB84(dVar6,0);
          }
          else {
            *(double *)(iVar4 + 0x60) = in_xd14;
          }
          if (in_FPSCR_SZ == '\0') {
            in_dr2 = (double)CONCAT44(*(undefined4 *)(iVar4 + 100),*(undefined4 *)(iVar4 + 0x70));
          }
          else {
            in_xd2 = *(double *)(iVar4 + 0x70);
            in_dr2 = *(double *)(iVar4 + 100);
          }
          if (in_FPSCR_PR == '\0' && (float)((ulonglong)in_dr2 >> 0x20) < SUB84(in_dr2,0)) {
            if (in_FPSCR_SZ == '\0') {
              dVar6 = (double)CONCAT44(*(undefined4 *)(iVar4 + 100),*(undefined4 *)(iVar4 + 0x74));
            }
            else {
              in_xd2 = *(double *)(iVar4 + 0x74);
              dVar6 = *(double *)(iVar4 + 100);
            }
            if (in_FPSCR_PR == '\0') {
              dVar6 = (double)CONCAT44((float)((ulonglong)dVar6 >> 0x20) - SUB84(dVar6,0),
                                       SUB84(dVar6,0));
            }
            else {
              dVar6 = dVar6 - dVar6;
            }
            if (in_FPSCR_SZ == '\0') {
              dVar6 = (double)CONCAT44((int)((ulonglong)dVar6 >> 0x20),*(undefined4 *)(iVar4 + 0x80)
                                      );
            }
            else {
              in_xd2 = *(double *)(iVar4 + 0x80);
            }
            if (in_FPSCR_PR == '\0') {
              fVar14 = SUB84(dVar6,0) * (float)((ulonglong)dVar6 >> 0x20);
            }
            else {
              fVar14 = SUB84(dVar6 * dVar6,0);
            }
            if (in_FPSCR_SZ == '\0') {
              dVar6 = (double)CONCAT44(*(undefined4 *)(iVar4 + 0x60),fVar14);
            }
            else {
              dVar6 = *(double *)(iVar4 + 0x60);
            }
            if (in_FPSCR_PR == '\0') {
              in_dr2 = (double)CONCAT44((float)((ulonglong)dVar6 >> 0x20) * SUB84(dVar6,0),
                                        SUB84(dVar6,0));
            }
            else {
              in_dr2 = dVar6 * dVar6;
            }
            if (in_FPSCR_SZ == '\0') {
              *(int *)(iVar4 + 0x60) = (int)((ulonglong)in_dr2 >> 0x20);
            }
            else {
              *(double *)(iVar4 + 0x60) = in_dr2;
            }
          }
          goto LAB_8c63cb6c;
        }
      }
    }
LAB_8c63cb66:
    param_1 = (double)((ulonglong)param_1 & 0xffffffff);
    if (in_FPSCR_SZ == '\0') {
      *(undefined4 *)(iVar4 + 0x60) = 0;
    }
    else {
      *(double *)(iVar4 + 0x60) = param_1;
    }
  }
  goto LAB_8c63cb6c;
}

