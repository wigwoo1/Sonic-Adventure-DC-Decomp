/* Ghidra draft only; not buildable/verified source. */

/* WARNING: Restarted to delay deadcode elimination for space: register */

longlong FUN_8c038d00(double param_1,undefined8 param_2,undefined4 param_3,double param_4)

{
  uint in_r1;
  undefined4 *puVar1;
  double *pdVar2;
  double *pdVar3;
  double *pdVar4;
  undefined4 in_fr1;
  double dVar5;
  undefined4 in_fr3;
  double dVar6;
  float fVar7;
  float fVar8;
  double dVar9;
  float fVar10;
  float fVar12;
  double dVar11;
  double dVar13;
  float fVar14;
  float in_xf0;
  float in_xf1;
  undefined8 in_xd2;
  float in_xf4;
  float in_xf5;
  double in_xd6;
  undefined8 in_xd8;
  undefined8 in_xd10;
  float in_xf12;
  float in_xf13;
  float in_xf14;
  float in_xf15;
  bool bVar16;
  char in_FPSCR_PR;
  char in_FPSCR_SZ;
  double dVar15;
  
  if (*DAT_8c038d0c != 0) {
    return (ulonglong)in_r1 << 0x20;
  }
  dVar5 = in_xd6;
  if (in_FPSCR_SZ == '\0') {
    dVar5 = (double)CONCAT44((int)param_2,in_fr1);
  }
  fVar12 = (float)((ulonglong)param_2 >> 0x20);
  dVar11 = (double)CONCAT44(fVar12,0x3f800000);
                    /* WARNING: Ignoring partial resolution of indirect */
  if (in_FPSCR_PR == '\0') {
    fVar7 = SUB84(param_1,0);
    fVar10 = (float)((ulonglong)param_1 >> 0x20);
    fVar8 = in_xf0 * fVar7 + in_xf4 * fVar10 + (float)in_xd8 * 1.0 + in_xf12 * fVar12;
    fVar8 = in_xf1 * fVar7 + in_xf5 * fVar10 + (float)((ulonglong)in_xd8 >> 0x20) * 1.0 +
            in_xf13 * fVar12;
    fVar8 = (float)in_xd2 * fVar7 + SUB84(in_xd6,0) * fVar10 + (float)in_xd10 * 1.0 +
            in_xf14 * fVar12;
    fVar12 = (float)((ulonglong)in_xd2 >> 0x20) * fVar7 +
             (float)((ulonglong)in_xd6 >> 0x20) * fVar10 + (float)((ulonglong)in_xd10 >> 0x20) * 1.0
             + in_xf15 * fVar12;
  }
  if (in_FPSCR_SZ == '\0') {
    dVar11 = (double)CONCAT44((int)((ulonglong)dVar11 >> 0x20),*DAT_8c038d84);
  }
  fVar12 = (float)((ulonglong)dVar5 >> 0x20);
  if (in_FPSCR_PR == '\0') {
    dVar11 = (double)CONCAT44((int)((ulonglong)dVar11 >> 0x20),SUB84(dVar11,0) - fVar12);
  }
  else {
    dVar11 = dVar11 - dVar5;
  }
  fVar8 = (float)((ulonglong)dVar11 >> 0x20);
  puVar1 = DAT_8c038d84;
  if (in_FPSCR_PR == '\0' && SUB84(dVar11,0) < fVar8) {
    if (in_FPSCR_SZ == '\0') {
      dVar11 = (double)CONCAT44(fVar8,*DAT_8c038d88);
    }
    if (in_FPSCR_PR == '\0') {
      dVar11 = (double)CONCAT44((int)((ulonglong)dVar11 >> 0x20),SUB84(dVar11,0) + fVar12);
    }
    else {
      dVar11 = dVar11 + dVar5;
    }
    fVar8 = (float)((ulonglong)dVar11 >> 0x20);
    puVar1 = DAT_8c038d88;
    if (in_FPSCR_PR != '\0' || fVar8 <= SUB84(dVar11,0)) {
      if (in_FPSCR_PR == '\0') {
        fVar8 = fVar8 + fVar12;
        dVar11 = (double)CONCAT44(fVar8,SUB84(dVar11,0));
        dVar5 = (double)CONCAT44(fVar12,1.0 / fVar8);
      }
      else {
        dVar11 = dVar11 + (double)CONCAT44(fVar12,0x3f800000);
        fVar8 = (float)((ulonglong)dVar11 >> 0x20);
        dVar5 = (double)CONCAT44(fVar12,0x3f800000) / dVar11;
      }
      if (in_FPSCR_SZ == '\0') {
        dVar11 = (double)CONCAT44(fVar8,*DAT_8c038d8c);
        dVar6 = (double)CONCAT44(*(undefined4 *)DAT_8c038d90,in_fr3);
      }
      else {
        dVar6 = *DAT_8c038d90;
      }
      if (in_FPSCR_PR == '\0') {
        dVar6 = (double)CONCAT44((float)((ulonglong)dVar6 >> 0x20) * SUB84(dVar11,0),SUB84(dVar6,0))
        ;
      }
      else {
        dVar6 = dVar6 * dVar11;
      }
      if (in_FPSCR_SZ == '\0') {
        dVar13 = (double)CONCAT44(*(undefined4 *)DAT_8c038d94,param_3);
        pdVar2 = (double *)((int)DAT_8c038d94 + 4);
        dVar11 = (double)CONCAT44((int)((ulonglong)dVar11 >> 0x20),DAT_8c038d8c[3]);
        puVar1 = DAT_8c038d8c + 4;
      }
      else {
        dVar13 = *DAT_8c038d94;
        pdVar2 = DAT_8c038d94 + 1;
        puVar1 = DAT_8c038d8c + 5;
      }
      if (in_FPSCR_PR == '\0') {
        dVar6 = (double)CONCAT44((float)((ulonglong)dVar6 >> 0x20) * SUB84(dVar5,0),SUB84(dVar6,0));
      }
      else {
        dVar6 = dVar6 * dVar5;
      }
      fVar12 = (float)((ulonglong)param_1 >> 0x20);
      if (in_FPSCR_SZ == '\0') {
        param_4 = (double)CONCAT44((int)((ulonglong)param_4 >> 0x20),fVar12);
      }
      fVar8 = (float)((ulonglong)dVar5 >> 0x20);
      fVar7 = (float)((ulonglong)dVar6 >> 0x20);
      fVar10 = SUB84(dVar11,0);
      if (in_FPSCR_PR == '\0') {
        fVar12 = (fVar12 + fVar8) * fVar7 + fVar10;
        dVar9 = (double)CONCAT44(fVar12,SUB84(param_1,0));
        bVar16 = (float)((ulonglong)dVar13 >> 0x20) < fVar12;
      }
      else {
        dVar9 = (param_1 + dVar5) * dVar6 + dVar11;
        bVar16 = dVar13 < dVar9;
      }
      if (bVar16) {
        if (in_FPSCR_SZ == '\0') {
          pdVar4 = pdVar2 + 1;
          pdVar3 = pdVar2;
        }
        else {
          pdVar3 = pdVar2 + 1;
          pdVar4 = pdVar2 + 2;
        }
        dVar13 = *pdVar3;
        fVar12 = (float)((ulonglong)dVar13 >> 0x20);
        if (in_FPSCR_PR == '\0') {
          fVar14 = (SUB84(param_4,0) - fVar8) * fVar7 + fVar10;
          dVar15 = (double)CONCAT44((int)((ulonglong)param_4 >> 0x20),fVar14);
          bVar16 = fVar12 < fVar14;
        }
        else {
          dVar15 = (param_4 - dVar5) * dVar6 + dVar11;
          bVar16 = dVar13 < dVar15;
        }
        if (!bVar16) {
          if (in_FPSCR_SZ == '\0') {
            dVar6 = (double)CONCAT44(fVar7,*DAT_8c038d98);
          }
          if (in_FPSCR_PR == '\0') {
            dVar6 = (double)CONCAT44((int)((ulonglong)dVar6 >> 0x20),SUB84(dVar6,0) * fVar10);
          }
          else {
            dVar6 = dVar6 * dVar11;
          }
          if (in_FPSCR_SZ == '\0') {
            dVar11 = (double)CONCAT44((int)((ulonglong)dVar11 >> 0x20),*puVar1);
            puVar1 = puVar1 + 1;
          }
          else {
            puVar1 = puVar1 + 2;
          }
          if (in_FPSCR_PR == '\0') {
            dVar6 = (double)CONCAT44((int)((ulonglong)dVar6 >> 0x20),SUB84(dVar6,0) * SUB84(dVar5,0)
                                    );
          }
          else {
            dVar6 = dVar6 * dVar5;
          }
          if (in_FPSCR_SZ == '\0') {
            dVar15 = (double)CONCAT44((int)((ulonglong)dVar15 >> 0x20),SUB84(dVar9,0));
          }
          if (in_FPSCR_PR == '\0') {
            bVar16 = SUB84(dVar13,0) < (SUB84(dVar9,0) + fVar8) * SUB84(dVar6,0) + SUB84(dVar11,0);
          }
          else {
            bVar16 = dVar13 < (dVar9 + dVar5) * dVar6 + dVar11;
          }
          if (bVar16) {
            if (in_FPSCR_SZ == '\0') {
              dVar13 = (double)CONCAT44(fVar12,*(undefined4 *)pdVar4);
            }
            if (in_FPSCR_PR == '\0') {
              bVar16 = (float)((ulonglong)dVar13 >> 0x20) <
                       (SUB84(dVar15,0) - fVar8) * (float)((ulonglong)dVar6 >> 0x20) +
                       SUB84(dVar11,0);
            }
            else {
              bVar16 = dVar13 < (dVar15 - dVar5) * dVar6 + dVar11;
            }
            if (!bVar16) {
              return CONCAT44(puVar1,1);
            }
          }
        }
      }
    }
  }
  return ZEXT48(puVar1) << 0x20;
}

