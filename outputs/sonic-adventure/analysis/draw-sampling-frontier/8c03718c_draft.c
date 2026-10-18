/* Ghidra draft only; not buildable/verified source. */

/* WARNING: Restarted to delay deadcode elimination for space: register */

void FUN_8c03718c(undefined4 param_1,float param_2,undefined4 param_3,double param_4,int param_5)

{
  undefined4 *puVar1;
  undefined4 uVar2;
  double *pdVar3;
  double *pdVar4;
  double *pdVar5;
  double *pdVar6;
  int iVar7;
  uint uVar8;
  float fVar9;
  double dVar10;
  float fVar12;
  double dVar11;
  double dVar13;
  double dVar14;
  double dVar16;
  float fVar18;
  double dVar17;
  float fVar19;
  undefined8 in_xd0;
  undefined8 in_xd2;
  float in_xf4;
  float in_xf5;
  float in_xf6;
  float in_xf7;
  undefined8 in_xd8;
  undefined8 in_xd10;
  float in_xf12;
  float in_xf13;
  float in_xf14;
  float in_xf15;
  int in_GBR;
  bool bVar20;
  char in_FPSCR_PR;
  char in_FPSCR_SZ;
  float fVar15;
  
  if (in_FPSCR_SZ == '\0') {
    dVar10 = (double)CONCAT44(*(undefined4 *)(param_5 + 0x18),*(undefined4 *)(param_5 + 0x1c));
    fVar12 = *(float *)(param_5 + 0x20);
    puVar1 = (undefined4 *)(param_5 + 0x24);
  }
  else {
    dVar10 = *(double *)(param_5 + 0x18);
    in_xd0 = *(undefined8 *)(param_5 + 0x20);
    fVar12 = (float)((ulonglong)*(undefined8 *)(param_5 + 0x28) >> 0x20);
    puVar1 = (undefined4 *)(param_5 + 0x30);
  }
  dVar11 = (double)CONCAT44(fVar12,0x3f800000);
                    /* WARNING: Ignoring partial resolution of indirect */
  if (in_FPSCR_PR == '\0') {
    fVar15 = SUB84(dVar10,0);
    fVar18 = (float)((ulonglong)dVar10 >> 0x20);
    fVar9 = (float)in_xd0 * fVar15 + in_xf4 * fVar18 + (float)in_xd8 * 1.0 + in_xf12 * fVar12;
    fVar9 = (float)((ulonglong)in_xd0 >> 0x20) * fVar15 + in_xf5 * fVar18 +
            (float)((ulonglong)in_xd8 >> 0x20) * 1.0 + in_xf13 * fVar12;
    fVar9 = (float)in_xd2 * fVar15 + in_xf6 * fVar18 + (float)in_xd10 * 1.0 + in_xf14 * fVar12;
    fVar12 = (float)((ulonglong)in_xd2 >> 0x20) * fVar15 + in_xf7 * fVar18 +
             (float)((ulonglong)in_xd10 >> 0x20) * 1.0 + in_xf15 * fVar12;
  }
  if (in_FPSCR_SZ == '\0') {
    dVar11 = (double)CONCAT44((int)((ulonglong)dVar11 >> 0x20),*puVar1);
  }
  fVar12 = SUB84(dVar11,0);
  fVar9 = (float)((ulonglong)dVar11 >> 0x20);
  if (in_FPSCR_PR == '\0') {
    fVar15 = *(float *)(in_GBR + 0x10) - fVar12;
    dVar13 = (double)CONCAT44(fVar15,param_1);
    bVar20 = fVar15 < fVar9;
  }
  else {
    dVar13 = (double)CONCAT44(*(float *)(in_GBR + 0x10),param_1) - dVar11;
    bVar20 = dVar13 < dVar11;
  }
  if (bVar20) {
    if (in_FPSCR_SZ == '\0') {
      dVar13 = (double)CONCAT44(*(undefined4 *)DAT_8c03720c,SUB84(dVar13,0));
    }
    else {
      dVar13 = *DAT_8c03720c;
    }
    if (in_FPSCR_PR == '\0') {
      fVar15 = (float)((ulonglong)dVar13 >> 0x20) + fVar12;
      bVar20 = fVar15 < fVar9;
    }
    else {
      fVar15 = (float)((ulonglong)(dVar13 + dVar11) >> 0x20);
      bVar20 = dVar13 + dVar11 < dVar11;
    }
    if (!bVar20) {
      if (in_FPSCR_PR == '\0') {
        dVar11 = (double)CONCAT44(fVar9 + fVar12,fVar12);
        fVar12 = 1.0 / (fVar9 + fVar12);
      }
      else {
        dVar11 = dVar11 + dVar11;
        fVar12 = SUB84((double)CONCAT44(fVar15,0x3f800000) / dVar11,0);
      }
      fVar9 = fVar12;
      if (in_FPSCR_SZ == '\0') {
        fVar9 = param_2;
      }
      if (in_FPSCR_SZ == '\0') {
        dVar13 = (double)CONCAT44(*(undefined4 *)DAT_8c037210,param_3);
        pdVar4 = (double *)((int)DAT_8c037210 + 4);
        dVar14 = (double)CONCAT44(*(undefined4 *)DAT_8c037214,fVar12);
        pdVar3 = (double *)((int)DAT_8c037214 + 4);
      }
      else {
        dVar13 = *DAT_8c037210;
        pdVar4 = DAT_8c037210 + 1;
        dVar14 = *DAT_8c037214;
        pdVar3 = DAT_8c037214 + 1;
      }
      if (in_FPSCR_PR == '\0') {
        dVar16 = (double)CONCAT44(*(float *)(in_GBR + 8) * SUB84(dVar14,0),fVar9);
      }
      else {
        dVar16 = (double)CONCAT44(*(float *)(in_GBR + 8),fVar9) * dVar14;
      }
      fVar12 = (float)((ulonglong)dVar10 >> 0x20);
      if (in_FPSCR_SZ == '\0') {
        param_4 = (double)CONCAT44((int)((ulonglong)param_4 >> 0x20),fVar12);
      }
      fVar9 = SUB84(dVar11,0);
      fVar18 = (float)((ulonglong)dVar16 >> 0x20);
      fVar15 = (float)((ulonglong)dVar14 >> 0x20);
      if (in_FPSCR_PR == '\0') {
        fVar12 = (fVar12 + fVar9) * fVar18 + fVar15;
        dVar10 = (double)CONCAT44(fVar12,SUB84(dVar10,0));
        bVar20 = (float)((ulonglong)dVar13 >> 0x20) < fVar12;
      }
      else {
        dVar10 = (dVar10 + dVar11) * dVar16 + dVar14;
        bVar20 = dVar13 < dVar10;
      }
      if (bVar20) {
        if (in_FPSCR_SZ == '\0') {
          pdVar6 = pdVar4 + 1;
          pdVar5 = pdVar4;
        }
        else {
          pdVar5 = pdVar4 + 1;
          pdVar6 = pdVar4 + 2;
        }
        dVar13 = *pdVar5;
        fVar12 = (float)((ulonglong)dVar13 >> 0x20);
        if (in_FPSCR_PR == '\0') {
          fVar19 = (SUB84(param_4,0) - fVar9) * fVar18 + fVar15;
          dVar16 = (double)CONCAT44((int)((ulonglong)param_4 >> 0x20),fVar19);
          bVar20 = fVar12 < fVar19;
        }
        else {
          dVar16 = (param_4 - dVar11) * dVar16 + dVar14;
          bVar20 = dVar13 < dVar16;
        }
        if (!bVar20) {
          if (in_FPSCR_PR == '\0') {
            dVar17 = (double)CONCAT44(fVar18,*(float *)(in_GBR + 0xc) * fVar15);
          }
          else {
            dVar17 = (double)CONCAT44(fVar18,*(float *)(in_GBR + 0xc)) * dVar14;
          }
          if (in_FPSCR_SZ == '\0') {
            dVar14 = (double)CONCAT44(*(undefined4 *)pdVar3,SUB84(dVar14,0));
          }
          else {
            dVar14 = *pdVar3;
          }
          if (in_FPSCR_PR == '\0') {
            dVar17 = (double)CONCAT44((int)((ulonglong)dVar17 >> 0x20),
                                      SUB84(dVar17,0) * SUB84(dVar14,0));
          }
          else {
            dVar17 = dVar17 * dVar14;
          }
          if (in_FPSCR_SZ == '\0') {
            dVar16 = (double)CONCAT44((int)((ulonglong)dVar16 >> 0x20),SUB84(dVar10,0));
          }
          fVar15 = (float)((ulonglong)dVar14 >> 0x20);
          if (in_FPSCR_PR == '\0') {
            bVar20 = SUB84(dVar13,0) < (SUB84(dVar10,0) + fVar9) * SUB84(dVar17,0) + fVar15;
          }
          else {
            bVar20 = dVar13 < (dVar10 + dVar11) * dVar17 + dVar14;
          }
          if (bVar20) {
            if (in_FPSCR_SZ == '\0') {
              dVar13 = (double)CONCAT44(fVar12,*(undefined4 *)pdVar6);
            }
            if (in_FPSCR_PR == '\0') {
              bVar20 = (float)((ulonglong)dVar13 >> 0x20) <
                       (SUB84(dVar16,0) - fVar9) * (float)((ulonglong)dVar17 >> 0x20) + fVar15;
            }
            else {
              bVar20 = dVar13 < (dVar16 - dVar11) * dVar17 + dVar14;
            }
            if (!bVar20) {
              uVar8 = *DAT_8c037278;
              *(uint *)(in_GBR + 0x1c) = (uint)((uVar8 & DAT_8c03727c) != 0);
              *(undefined4 *)(in_GBR + 0x20) = *DAT_8c037280;
              *(undefined4 *)(in_GBR + 0x24) = *DAT_8c037284;
              uVar2 = 0;
              if ((uVar8 & DAT_8c037288) != 0) {
                uVar2 = 0xffffffff;
              }
              if ((uVar8 & DAT_8c03728c) != 0) {
                uVar2 = 1;
              }
              *(undefined4 *)(in_GBR + 0x28) = uVar2;
              iVar7 = (uint)(**(ushort **)(param_5 + 0xc) & DAT_8c037276) * 0x14 +
                      *(int *)(param_5 + 0x10);
              *(int *)(in_GBR + 0x34) = iVar7;
              uVar8 = *(uint *)(iVar7 + 0x10);
              if (*DAT_8c037290 != 0) {
                uVar8 = uVar8 & DAT_8c037290[1] | DAT_8c037290[2];
              }
              *(uint *)(in_GBR + 0x2c) = uVar8;
              return;
            }
          }
        }
      }
    }
  }
  return;
}

