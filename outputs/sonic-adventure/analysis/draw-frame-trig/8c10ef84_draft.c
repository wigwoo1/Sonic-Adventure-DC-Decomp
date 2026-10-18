/* Ghidra draft only; not buildable/verified source. */

/* WARNING: Removing unreachable block (ram,0x8c10f02e) */
/* WARNING: Removing unreachable block (ram,0x8c10eff4) */
/* WARNING: Removing unreachable block (ram,0x8c10efc8) */
/* WARNING: Removing unreachable block (ram,0x8c10efa6) */
/* WARNING: Removing unreachable block (ram,0x8c10efc4) */
/* WARNING: Removing unreachable block (ram,0x8c10efda) */
/* WARNING: Removing unreachable block (ram,0x8c10f00c) */
/* WARNING: Removing unreachable block (ram,0x8c10f0fe) */
/* WARNING: Removing unreachable block (ram,0x8c10efe2) */
/* WARNING: Removing unreachable block (ram,0x8c10efe4) */

double FUN_8c10ef84(double param_1)

{
  undefined *puVar1;
  int iVar2;
  uint uVar3;
  undefined8 *puVar4;
  uint in_fr1;
  uint extraout_fr1;
  uint extraout_fr1_00;
  double dVar5;
  ulonglong in_dr2;
  float fVar6;
  undefined4 uVar7;
  undefined4 uVar9;
  double dVar8;
  float fVar10;
  float fVar11;
  double in_dr12;
  undefined4 uVar13;
  double dVar12;
  undefined8 in_dr14;
  double dVar14;
  double in_xd0;
  double in_xd2;
  double in_xd4;
  double in_xd14;
  bool bVar15;
  undefined4 in_PR;
  char in_FPSCR_PR;
  char in_FPSCR_SZ;
  undefined1 local_28 [16];
  undefined8 uStack_18;
  undefined8 uStack_10;
  
  if (in_FPSCR_SZ == '\0') {
    uStack_10 = (double)CONCAT44((int)in_dr14,(undefined4)uStack_10);
                    /* WARNING: Ignoring partial resolution of indirect */
    uStack_10._0_4_ = (int)((ulonglong)in_dr14 >> 0x20);
    puVar4 = &uStack_18;
    uStack_18 = CONCAT44(SUB84(in_dr12,0),(int)((ulonglong)in_dr12 >> 0x20));
  }
  else {
    puVar4 = (undefined8 *)local_28;
    uStack_10 = in_xd14;
    uStack_18 = in_dr14;
  }
  *(undefined4 *)((int)puVar4 + -4) = in_PR;
  if (in_FPSCR_SZ == '\0') {
    fVar11 = (float)((ulonglong)param_1 >> 0x20);
    param_1 = (double)CONCAT44(fVar11,fVar11);
    *(float *)((int)puVar4 + -8) = fVar11;
  }
  else {
    fVar11 = SUB84(param_1,0);
    *(double *)((int)puVar4 + -8) = param_1;
    in_xd4 = param_1;
  }
  uVar3 = 0;
  if (*(int *)((int)puVar4 + -8) < 0) {
    uVar3 = 8;
  }
  dVar12 = (double)(in_dr2 & 0xffffffff00000000);
  if (fVar11 == 0.0) {
    uVar3 = uVar3 | 4;
  }
  else if ((*(uint *)((int)puVar4 + -8) & DAT_8c10f114) == DAT_8c10f118) {
    uVar3 = uVar3 | 2;
  }
  uVar9 = (undefined4)((ulonglong)param_1 >> 0x20);
  if (in_FPSCR_SZ == '\0') {
    *(undefined4 *)((int)puVar4 + -0x10) = uVar9;
  }
  else {
    *(double *)((int)puVar4 + -0x10) = param_1;
  }
  if ((*(uint *)((int)puVar4 + -0x10) & DAT_8c10f11c) != 0) {
    if (in_FPSCR_SZ == '\0') {
      dVar12 = (double)CONCAT44((int)(in_dr2 >> 0x20),*(undefined4 *)((int)puVar4 + -0x10));
    }
    else {
      in_xd2 = *(double *)((int)puVar4 + -0x10);
    }
    fVar11 = SUB84(dVar12,0);
    dVar12 = (double)CONCAT44((int)((ulonglong)dVar12 >> 0x20),-fVar11);
    if (in_FPSCR_SZ == '\0') {
      *(float *)((int)puVar4 + -0x10) = -fVar11;
    }
    else {
      *(double *)((int)puVar4 + -0x10) = in_xd2;
    }
  }
  if (((int)(*(uint *)((int)puVar4 + -0x10) & DAT_8c10f118) >> 0x17) + -0x7f < 0x20) {
    if ((uVar3 & 4) == 0) {
      if (in_FPSCR_SZ == '\0') {
        dVar12 = (double)CONCAT44((int)((ulonglong)dVar12 >> 0x20),
                                  *(undefined4 *)((int)puVar4 + -0x10));
        param_1 = (double)CONCAT44(uVar9,*(undefined4 *)PTR_DAT_8c10f128);
      }
      else {
        in_xd2 = *(double *)((int)puVar4 + -0x10);
        in_xd4 = *(double *)PTR_DAT_8c10f128;
      }
      fVar11 = SUB84(dVar12,0);
      if (in_FPSCR_PR == '\0') {
        bVar15 = fVar11 < SUB84(param_1,0);
      }
      else {
        bVar15 = dVar12 < param_1;
      }
      if (in_FPSCR_SZ == '\0') {
        dVar8 = (double)CONCAT44(*(undefined4 *)PTR_DAT_8c10f12c,SUB84(param_1,0));
      }
      else {
        dVar8 = *(double *)PTR_DAT_8c10f12c;
      }
      if (bVar15) {
        if (in_FPSCR_SZ == '\0') {
          dVar14 = (double)CONCAT44(*(undefined4 *)PTR_DAT_8c10f130,fVar11);
          in_xd2 = in_xd14;
        }
        else {
          dVar14 = *(double *)PTR_DAT_8c10f130;
        }
      }
      else {
        if (SUB84(dVar8,0) == fVar11) {
          if ((uVar3 & 8) != 0) {
            if (in_FPSCR_PR == '\0') {
              dVar8 = (double)((ulonglong)dVar8 & 0xffffffff);
            }
            else {
              dVar8 = -dVar8;
            }
          }
          if (in_FPSCR_SZ != '\0') {
            return dVar8;
          }
          return (double)(ulonglong)in_fr1;
        }
        if (in_FPSCR_SZ == '\0') {
          dVar12 = (double)CONCAT44(*(undefined4 *)PTR_DAT_8c10f134,
                                    *(undefined4 *)((int)puVar4 + -0x10));
        }
        else {
          dVar12 = *(double *)PTR_DAT_8c10f134;
        }
        fVar11 = SUB84(dVar12,0);
        if (in_FPSCR_PR == '\0') {
          dVar8 = (double)CONCAT44((int)((ulonglong)dVar8 >> 0x20),SUB84(dVar8,0) + fVar11);
        }
        else {
          dVar8 = dVar8 + dVar12;
        }
        dVar14 = dVar8;
        if (in_FPSCR_SZ == '\0') {
          dVar14 = (double)CONCAT44((int)((ulonglong)dVar8 >> 0x20),(int)in_dr14);
        }
        if (in_FPSCR_PR == '\0') {
          dVar12 = (double)CONCAT44((float)((ulonglong)dVar12 >> 0x20) + fVar11,fVar11);
        }
        else {
          dVar12 = dVar12 + dVar12;
        }
        in_xd2 = dVar12;
        if (in_FPSCR_SZ == '\0') {
          dVar14 = (double)CONCAT44((int)((ulonglong)dVar14 >> 0x20),
                                    (int)((ulonglong)dVar12 >> 0x20));
          in_xd2 = in_xd14;
        }
        if (in_FPSCR_PR == '\0') {
          dVar14 = (double)CONCAT44((int)((ulonglong)dVar14 >> 0x20),
                                    SUB84(dVar14,0) / SUB84(dVar8,0));
        }
        else {
          dVar14 = dVar14 / dVar8;
        }
      }
      uVar9 = SUB84(dVar12,0);
      if (in_FPSCR_SZ == '\0') {
        dVar12 = (double)CONCAT44(SUB84(dVar14,0),*(undefined4 *)PTR_DAT_8c10f138);
      }
      else {
        in_xd4 = *(double *)PTR_DAT_8c10f138;
        dVar12 = in_xd2;
      }
      (*(code *)PTR_FUN_8c10f13c)(dVar12,(undefined1 *)((int)puVar4 + -0xc),uVar3);
      puVar1 = PTR_FUN_8c10f140;
      iVar2 = *(int *)((int)puVar4 + -0xc);
      if (in_FPSCR_SZ == '\0') {
        in_dr12 = (double)CONCAT44((int)((ulonglong)in_dr12 >> 0x20),*(undefined4 *)PTR_DAT_8c10f128
                                  );
      }
      uVar7 = SUB84(dVar12,0);
      fVar11 = SUB84(dVar14,0);
      if (iVar2 == 0) {
        if (in_FPSCR_SZ == '\0') {
          in_xd2 = (double)CONCAT44(fVar11,uVar7);
        }
      }
      else {
        fVar10 = SUB84(in_dr12,0);
        if ((iVar2 == 1) || (iVar2 == 2)) {
          if (in_FPSCR_SZ == '\0') {
            dVar12 = (double)CONCAT44(*(undefined4 *)PTR_DAT_8c10f148,
                                      *(undefined4 *)PTR_DAT_8c10f144);
          }
          else {
            dVar12 = *(double *)PTR_DAT_8c10f148;
          }
          if (in_FPSCR_PR == '\0') {
            dVar12 = (double)CONCAT44((float)((ulonglong)dVar12 >> 0x20) + fVar11,SUB84(dVar12,0));
            dVar14 = (double)((ulonglong)dVar14 & 0xffffffff);
          }
          else {
            dVar12 = dVar12 + dVar14;
            dVar14 = dVar14 + dVar12;
          }
          dVar8 = in_xd2;
          if (in_FPSCR_SZ == '\0') {
            dVar12 = (double)CONCAT44((int)((ulonglong)dVar12 >> 0x20),fVar10);
            dVar8 = (double)CONCAT44(SUB84(dVar14,0),uVar7);
          }
          dVar5 = (double)(*(code *)PTR_FUN_8c10f14c)(dVar8,0xfffffffe);
          if (in_FPSCR_PR == '\0') {
            fVar11 = SUB84(dVar12,0) + (float)((ulonglong)dVar5 >> 0x20);
            in_xd2 = (double)CONCAT44((float)((ulonglong)dVar12 >> 0x20) / fVar11,fVar11);
          }
          else {
            in_xd2 = (dVar12 + dVar5) / (dVar12 + dVar5);
          }
          if (in_FPSCR_SZ == '\0') {
            in_xd2 = (double)CONCAT44((int)((ulonglong)in_xd2 >> 0x20),SUB84(dVar8,0));
          }
        }
        else if ((iVar2 == 3) || (iVar2 == 4)) {
          if (in_FPSCR_SZ == '\0') {
            dVar12 = (double)CONCAT44(*(undefined4 *)PTR_DAT_8c10f154,
                                      *(undefined4 *)PTR_DAT_8c10f150);
          }
          else {
            dVar12 = *(double *)PTR_DAT_8c10f154;
          }
          if (in_FPSCR_PR == '\0') {
            dVar12 = (double)CONCAT44((float)((ulonglong)dVar12 >> 0x20) + fVar11,SUB84(dVar12,0));
            dVar14 = (double)((ulonglong)dVar14 & 0xffffffff);
          }
          else {
            dVar12 = dVar12 + dVar14;
            dVar14 = dVar14 + dVar12;
          }
          dVar8 = in_xd2;
          if (in_FPSCR_SZ == '\0') {
            dVar12 = (double)CONCAT44((int)((ulonglong)dVar12 >> 0x20),fVar10);
            dVar8 = (double)CONCAT44(SUB84(dVar14,0),uVar7);
          }
          dVar5 = (double)(*(code *)PTR_FUN_8c10f14c)(dVar8,0xffffffff);
          if (in_FPSCR_PR == '\0') {
            fVar11 = SUB84(dVar12,0) + (float)((ulonglong)dVar5 >> 0x20);
            in_xd2 = (double)CONCAT44((float)((ulonglong)dVar12 >> 0x20) / fVar11,fVar11);
          }
          else {
            in_xd2 = (dVar12 + dVar5) / (dVar12 + dVar5);
          }
          if (in_FPSCR_SZ == '\0') {
            in_xd2 = (double)CONCAT44((int)((ulonglong)in_xd2 >> 0x20),SUB84(dVar8,0));
          }
        }
        else {
          uVar13 = (undefined4)((ulonglong)in_dr12 >> 0x20);
          if ((iVar2 == 5) || (iVar2 == 6)) {
            if (in_FPSCR_SZ == '\0') {
              dVar12 = (double)CONCAT44(*(undefined4 *)PTR_DAT_8c10f15c,uVar9);
              dVar8 = (double)CONCAT44(*(undefined4 *)PTR_DAT_8c10f160,fVar11);
              in_xd2 = in_xd0;
            }
            else {
              dVar12 = *(double *)PTR_DAT_8c10f15c;
              dVar8 = *(double *)PTR_DAT_8c10f160;
            }
            fVar6 = (float)((ulonglong)dVar12 >> 0x20);
            if (in_FPSCR_PR == '\0') {
              dVar8 = (double)CONCAT44((int)((ulonglong)dVar8 >> 0x20),SUB84(dVar8,0) + fVar6);
            }
            else {
              dVar8 = dVar8 + dVar12;
            }
            if (in_FPSCR_SZ == '\0') {
              dVar12 = (double)CONCAT44(fVar6,*(undefined4 *)PTR_DAT_8c10f158);
            }
            if (in_FPSCR_PR == '\0') {
              in_dr12 = (double)CONCAT44(uVar13,(float)((ulonglong)dVar8 >> 0x20) * fVar11 + fVar10)
              ;
              dVar14 = (double)((ulonglong)dVar14 & 0xffffffff);
            }
            else {
              dVar14 = dVar14 + dVar12;
            }
            if (in_FPSCR_SZ == '\0') {
              in_xd2 = (double)CONCAT44(SUB84(dVar8,0),uVar7);
            }
            if (in_FPSCR_PR == '\0') {
              in_xd2 = (double)CONCAT44((float)((ulonglong)in_xd2 >> 0x20) / SUB84(in_dr12,0),
                                        SUB84(in_xd2,0));
            }
            else {
              in_xd2 = in_xd2 / in_dr12;
            }
          }
          else {
            param_1 = in_xd4;
            in_fr1 = extraout_fr1;
            if (iVar2 != 7) goto LAB_8c10f100;
            if (in_FPSCR_PR == '\0') {
              dVar12 = (double)CONCAT44(uVar13,fVar10 + fVar11);
            }
            else {
              dVar12 = in_dr12 + dVar14;
            }
            if (in_FPSCR_SZ == '\0') {
              dVar8 = (double)CONCAT44(*(undefined4 *)PTR_DAT_8c10f134,
                                       *(undefined4 *)PTR_DAT_8c10f12c);
            }
            else {
              dVar8 = *(double *)PTR_DAT_8c10f134;
            }
            if (in_FPSCR_PR == '\0') {
              dVar8 = (double)CONCAT44((float)((ulonglong)dVar8 >> 0x20) + fVar11,SUB84(dVar8,0));
              dVar14 = (double)((ulonglong)dVar14 & 0xffffffff);
            }
            else {
              dVar8 = dVar8 + dVar14;
              dVar14 = dVar14 + dVar8;
            }
            if (in_FPSCR_SZ == '\0') {
              dVar8 = (double)CONCAT44((int)((ulonglong)dVar8 >> 0x20),uVar7);
            }
            if (in_FPSCR_PR == '\0') {
              in_xd2 = (double)CONCAT44((float)((ulonglong)dVar8 >> 0x20) / SUB84(dVar12,0),
                                        SUB84(dVar8,0));
            }
            else {
              in_xd2 = dVar8 / dVar12;
            }
          }
        }
      }
      (*(code *)puVar1)(in_xd2);
      param_1 = dVar14;
      in_fr1 = extraout_fr1_00;
      if (in_FPSCR_SZ == '\0') {
        param_1 = in_xd4;
      }
    }
    else if (in_FPSCR_SZ == '\0') {
      param_1 = in_xd4;
    }
  }
  else {
    param_1 = in_xd4;
    if (in_FPSCR_SZ != '\0') {
      param_1 = *(double *)PTR_DAT_8c10f124;
    }
  }
LAB_8c10f100:
  if (in_FPSCR_SZ == '\0') {
    param_1 = (double)(ulonglong)in_fr1;
  }
  return param_1;
}

