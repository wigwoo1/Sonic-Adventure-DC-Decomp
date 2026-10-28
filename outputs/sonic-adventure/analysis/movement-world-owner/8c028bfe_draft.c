/* Ghidra draft only; not buildable/verified source. */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

int FUN_8c028bfe(uint param_1,double param_2,double param_3,double *param_4)

{
  int *piVar1;
  undefined *puVar2;
  char *pcVar3;
  short *psVar4;
  int iVar5;
  int *piVar6;
  undefined4 *puVar7;
  int iVar8;
  double *pdVar9;
  double *pdVar10;
  int *piVar11;
  undefined8 *puVar12;
  double dVar13;
  float fVar14;
  double in_dr2;
  undefined4 uVar15;
  double dVar16;
  float fVar17;
  float fVar18;
  float fVar19;
  double dVar20;
  double dVar21;
  float fVar22;
  undefined8 in_dr14;
  undefined4 uVar23;
  double in_xd2;
  double in_xd4;
  double in_xd6;
  double dVar24;
  double in_xd14;
  bool bVar25;
  undefined4 in_PR;
  char in_FPSCR_PR;
  char in_FPSCR_SZ;
  undefined1 auStack_2c [8];
  undefined8 uStack_24;
  
  puVar2 = PTR_FUN_8c028d48;
  piVar11 = DAT_8c028d44;
  piVar1 = DAT_8c028d40;
  if (in_FPSCR_SZ == '\0') {
    uStack_24 = (double)CONCAT44((int)in_dr14,(undefined4)uStack_24);
    puVar12 = &uStack_24;
                    /* WARNING: Ignoring partial resolution of indirect */
    uStack_24._0_4_ = (int)((ulonglong)in_dr14 >> 0x20);
  }
  else {
    puVar12 = (undefined8 *)auStack_2c;
    uStack_24 = in_xd14;
  }
  *(undefined4 *)((int)puVar12 + -4) = in_PR;
  *DAT_8c028d40 = 0;
  dVar13 = (double)(*(code *)puVar2)(param_4 + 2);
  if (in_FPSCR_SZ == '\0') {
    param_2 = (double)CONCAT44((int)((ulonglong)param_2 >> 0x20),*(undefined4 *)param_4);
    dVar13 = (double)CONCAT44((int)((ulonglong)dVar13 >> 0x20),(int)in_dr14);
  }
  else {
    in_xd6 = *param_4;
  }
  fVar14 = SUB84(param_2,0);
  if (in_FPSCR_PR == '\0') {
    dVar13 = (double)CONCAT44((float)((ulonglong)dVar13 >> 0x20) + fVar14,SUB84(dVar13,0));
  }
  else {
    dVar13 = dVar13 + param_2;
  }
  if (in_FPSCR_SZ == '\0') {
    param_1 = *(uint *)(param_4 + 1);
    uVar23 = (undefined4)((ulonglong)dVar13 >> 0x20);
    dVar13 = (double)CONCAT44(uVar23,uVar23);
  }
  else {
    fVar14 = (float)*(undefined8 *)((int)param_4 + 0xc);
    in_xd4 = param_4[1];
    in_xd14 = dVar13;
  }
  if (in_FPSCR_PR == '\0') {
    fVar19 = (float)((ulonglong)dVar13 >> 0x20);
    dVar13 = (double)CONCAT44(fVar19,SUB84(dVar13,0) * fVar19);
  }
  else {
    dVar13 = dVar13 * dVar13;
  }
  if (in_FPSCR_SZ == '\0') {
    dVar16 = (double)(ulonglong)param_1;
  }
  else {
    dVar16 = *(double *)((int)param_4 + 4);
  }
  (*(code *)PTR_FUN_8c028d4c)();
  uVar23 = _DAT_8c028d60;
  pcVar3 = DAT_8c028d58;
  puVar2 = PTR_FUN_8c028d54;
  puVar7 = *(undefined4 **)PTR_DAT_8c028d50;
  if (in_FPSCR_SZ == '\0') {
    dVar20 = (double)CONCAT44(DAT_8c028d5c,fVar14);
  }
  else {
    dVar20 = (double)CONCAT44(_DAT_8c028d60,DAT_8c028d5c);
  }
  do {
    iVar8 = DAT_8c028d68;
    psVar4 = DAT_8c028d64;
    uVar15 = SUB84(dVar16,0);
    if (puVar7 == (undefined4 *)0x0) {
LAB_8c028e18:
      return *piVar1;
    }
    puVar7[7] = 0;
    puVar7[9] = 0;
    bVar25 = false;
    for (iVar5 = (int)*psVar4; 0 < iVar5; iVar5 = iVar5 + -1) {
      if (*(int *)(iVar8 + 4) == puVar7[2]) {
        bVar25 = true;
        break;
      }
      iVar8 = iVar8 + 0xc;
    }
    if (bVar25) {
      piVar6 = (int *)puVar7[4];
      if (in_FPSCR_SZ == '\0') {
        uVar15 = *(undefined4 *)((int)param_4 + 0xc);
        dVar20 = (double)CONCAT44((int)((ulonglong)dVar20 >> 0x20),puVar7[10]);
        in_dr2 = (double)CONCAT44((int)((ulonglong)in_dr2 >> 0x20),uVar15);
      }
      else {
        in_xd2 = *(double *)((int)param_4 + 0xc);
        in_xd6 = *(double *)(puVar7 + 10);
        in_xd4 = in_xd2;
      }
      if (in_FPSCR_PR == '\0') {
        fVar14 = SUB84(in_dr2,0) - SUB84(dVar20,0);
      }
      else {
        fVar14 = SUB84(in_dr2 - dVar20,0);
      }
      if (in_FPSCR_SZ == '\0') {
        in_dr2 = (double)CONCAT44(piVar6[4],fVar14);
      }
      else {
        in_dr2 = *(double *)(piVar6 + 4);
      }
      fVar14 = (float)((ulonglong)dVar13 >> 0x20);
      if (in_FPSCR_PR == '\0') {
        dVar20 = (double)CONCAT44((int)((ulonglong)dVar20 >> 0x20),SUB84(dVar20,0) + fVar14);
      }
      else {
        dVar20 = dVar20 + dVar13;
      }
      iVar8 = puVar7[5];
      dVar16 = in_xd2;
      if (in_FPSCR_SZ == '\0') {
        dVar16 = (double)CONCAT44(SUB84(in_dr2,0),uVar15);
      }
      if (in_FPSCR_PR == '\0') {
        dVar16 = (double)CONCAT44((float)((ulonglong)dVar16 >> 0x20) - fVar14,SUB84(dVar16,0));
      }
      else {
        dVar16 = dVar16 - dVar13;
      }
      dVar21 = in_xd6;
      if (in_FPSCR_SZ == '\0') {
        dVar21 = (double)CONCAT44(SUB84(dVar20,0),SUB84(param_3,0));
      }
      fVar14 = (float)((ulonglong)in_dr2 >> 0x20);
      fVar19 = (float)((ulonglong)dVar16 >> 0x20);
      if (in_FPSCR_PR == '\0') {
        bVar25 = fVar14 < fVar19;
        param_3 = (double)CONCAT44((float)((ulonglong)dVar21 >> 0x20) + SUB84(dVar16,0),
                                   SUB84(dVar21,0));
      }
      else {
        bVar25 = in_dr2 < dVar16;
        param_3 = dVar21 + dVar16;
      }
      fVar22 = (float)((ulonglong)param_3 >> 0x20);
      if (bVar25) {
        if (in_FPSCR_SZ == '\0') {
          in_dr2 = (double)CONCAT44(fVar14,*(undefined4 *)(iVar8 + 0x10));
        }
        else {
          in_xd2 = *(double *)(iVar8 + 0x10);
        }
        if (in_FPSCR_PR == '\0') {
          bVar25 = fVar22 < SUB84(in_dr2,0);
        }
        else {
          bVar25 = param_3 < in_dr2;
        }
        if (bVar25) {
          if (in_FPSCR_SZ == '\0') {
            in_dr2 = (double)CONCAT44((int)((ulonglong)in_dr2 >> 0x20),piVar6[4]);
          }
          else {
            in_xd2 = *(double *)(piVar6 + 4);
          }
          if (in_FPSCR_PR == '\0') {
            bVar25 = fVar19 < SUB84(in_dr2,0);
          }
          else {
            bVar25 = dVar16 < in_dr2;
          }
          while (!bVar25) {
            piVar6 = (int *)*piVar6;
            if (piVar6 == (int *)0x0) goto LAB_8c028e02;
            if (in_FPSCR_SZ == '\0') {
              in_dr2 = (double)CONCAT44((int)((ulonglong)in_dr2 >> 0x20),piVar6[4]);
            }
            else {
              in_xd2 = *(double *)(piVar6 + 4);
            }
            if (in_FPSCR_PR == '\0') {
              bVar25 = fVar19 < SUB84(in_dr2,0);
            }
            else {
              bVar25 = dVar16 < in_dr2;
            }
          }
        }
      }
      if (piVar6 == (int *)0x0) {
LAB_8c028e02:
        do {
          if (in_FPSCR_SZ == '\0') {
            in_dr2 = (double)CONCAT44((int)((ulonglong)in_dr2 >> 0x20),*(undefined4 *)(iVar8 + 0x10)
                                     );
          }
          else {
            in_xd2 = *(double *)(iVar8 + 0x10);
          }
          fVar14 = (float)((ulonglong)dVar16 >> 0x20);
          if (in_FPSCR_PR == '\0') {
            bVar25 = fVar14 < SUB84(in_dr2,0);
          }
          else {
            bVar25 = dVar16 < in_dr2;
          }
          if ((!bVar25) || (iVar8 == 0)) goto LAB_8c028e0e;
          if (in_FPSCR_SZ == '\0') {
            in_dr2 = (double)CONCAT44(SUB84(dVar13,0),*(undefined4 *)(iVar8 + 0x14));
          }
          else {
            in_xd2 = *(double *)(iVar8 + 0x14);
            in_dr2 = in_xd14;
          }
          if (in_FPSCR_PR == '\0') {
            in_dr2 = (double)CONCAT44((float)((ulonglong)in_dr2 >> 0x20) + SUB84(in_dr2,0),
                                      SUB84(in_dr2,0));
          }
          else {
            in_dr2 = in_dr2 + in_dr2;
          }
          if (in_FPSCR_SZ == '\0') {
            in_dr2 = (double)CONCAT44((int)((ulonglong)in_dr2 >> 0x20),
                                      *(undefined4 *)((int)param_4 + 4));
            pdVar9 = param_4 + 1;
            param_3 = (double)CONCAT44(*(undefined4 *)(iVar8 + 8),SUB84(param_3,0));
            pdVar10 = (double *)(iVar8 + 0xc);
          }
          else {
            in_xd2 = *(double *)((int)param_4 + 4);
            pdVar9 = (double *)((int)param_4 + 0xc);
            param_3 = *(double *)(iVar8 + 8);
            pdVar10 = (double *)(iVar8 + 0x10);
          }
          if (in_FPSCR_PR == '\0') {
            param_3 = (double)CONCAT44((float)((ulonglong)param_3 >> 0x20) - SUB84(in_dr2,0),
                                       SUB84(param_3,0));
          }
          else {
            param_3 = param_3 - in_dr2;
          }
          uVar15 = (undefined4)((ulonglong)in_dr2 >> 0x20);
          dVar21 = in_dr2;
          if (in_FPSCR_SZ == '\0') {
            dVar16 = (double)CONCAT44(fVar14,uVar15);
            dVar21 = in_xd4;
          }
          fVar14 = (float)((ulonglong)dVar20 >> 0x20);
          if (in_FPSCR_PR == '\0') {
            dVar16 = (double)CONCAT44((int)((ulonglong)dVar16 >> 0x20),SUB84(dVar16,0) * fVar14);
          }
          else {
            dVar16 = dVar16 * dVar20;
          }
          fVar19 = (float)((ulonglong)param_3 >> 0x20);
          dVar24 = param_3;
          if (in_FPSCR_SZ == '\0') {
            dVar20 = (double)CONCAT44(fVar14,fVar19);
            dVar24 = in_xd6;
          }
          fVar14 = SUB84(dVar16,0);
          if (in_FPSCR_PR == '\0') {
            fVar19 = SUB84(dVar20,0) * fVar19;
            dVar20 = (double)CONCAT44((int)((ulonglong)dVar20 >> 0x20),fVar19);
            bVar25 = fVar14 < fVar19;
          }
          else {
            dVar20 = dVar20 * param_3;
            fVar19 = SUB84(dVar20,0);
            bVar25 = dVar16 < dVar20;
          }
          if (!bVar25) {
            if (in_FPSCR_SZ == '\0') {
              param_3 = (double)CONCAT44(*(undefined4 *)pdVar10,SUB84(param_3,0));
              pdVar10 = (double *)((int)pdVar10 + 4);
              in_dr2 = (double)CONCAT44(uVar15,*(undefined4 *)pdVar9);
              pdVar9 = (double *)((int)pdVar9 + 4);
            }
            else {
              param_3 = *pdVar10;
              pdVar10 = pdVar10 + 1;
              in_xd2 = *pdVar9;
              pdVar9 = pdVar9 + 1;
            }
            fVar22 = SUB84(dVar20,0);
            if (in_FPSCR_PR == '\0') {
              fVar22 = (float)((ulonglong)param_3 >> 0x20) - SUB84(in_dr2,0);
              param_3 = (double)CONCAT44(fVar22,SUB84(param_3,0));
              fVar22 = fVar22 * fVar22 + fVar19;
              dVar20 = (double)CONCAT44((int)((ulonglong)dVar20 >> 0x20),fVar22);
              bVar25 = fVar14 < fVar22;
            }
            else {
              param_3 = param_3 - in_dr2;
              bVar25 = dVar16 < dVar20;
            }
            if (!bVar25) {
              if (in_FPSCR_SZ == '\0') {
                param_3 = (double)CONCAT44(*(undefined4 *)pdVar10,SUB84(param_3,0));
                in_dr2 = (double)CONCAT44((int)((ulonglong)in_dr2 >> 0x20),*(undefined4 *)pdVar9);
              }
              else {
                param_3 = *pdVar10;
                in_xd2 = *pdVar9;
              }
              if (in_FPSCR_PR == '\0') {
                fVar19 = (float)((ulonglong)param_3 >> 0x20) - SUB84(in_dr2,0);
                param_3 = (double)CONCAT44(fVar19,SUB84(param_3,0));
                fVar22 = fVar19 * fVar19 + fVar22;
                dVar20 = (double)CONCAT44((int)((ulonglong)dVar20 >> 0x20),fVar22);
                bVar25 = fVar14 < fVar22;
              }
              else {
                param_3 = param_3 - in_dr2;
                bVar25 = dVar16 < dVar20;
              }
              if (!bVar25) {
                if (puVar7[7] == 0) {
                  puVar7[7] = piVar11;
                }
                puVar7[9] = puVar7[9] + 1;
                *piVar11 = iVar8;
                piVar11 = piVar11 + 1;
                iVar5 = *piVar1;
                *piVar1 = iVar5 + 1;
                if (iVar5 + 1 == 0x60) {
                  if (-1 < *pcVar3) {
                    (*(code *)puVar2)(uVar23,PTR_s_WARNING__NEAR_poly_is_FULL_8c028e40);
                  }
                  goto LAB_8c028e18;
                }
              }
            }
          }
          iVar8 = *(int *)(iVar8 + 4);
          in_xd4 = dVar21;
          in_xd6 = dVar24;
        } while( true );
      }
      if (in_FPSCR_SZ == '\0') {
        in_dr2 = (double)CONCAT44((int)((ulonglong)in_dr2 >> 0x20),piVar6[4]);
      }
      else {
        in_xd2 = *(double *)(piVar6 + 4);
      }
      if (in_FPSCR_PR == '\0') {
        bVar25 = fVar19 < SUB84(in_dr2,0);
      }
      else {
        bVar25 = dVar16 < in_dr2;
      }
      if (!bVar25) goto LAB_8c028e02;
      for (; piVar6 != (int *)0x0; piVar6 = (int *)*piVar6) {
        if (in_FPSCR_SZ == '\0') {
          in_dr2 = (double)CONCAT44((int)((ulonglong)in_dr2 >> 0x20),piVar6[4]);
        }
        else {
          in_xd2 = *(double *)(piVar6 + 4);
        }
        if (in_FPSCR_PR == '\0') {
          bVar25 = SUB84(in_dr2,0) < fVar22;
        }
        else {
          bVar25 = in_dr2 < param_3;
        }
        if (!bVar25) break;
        if (in_FPSCR_SZ == '\0') {
          in_dr2 = (double)CONCAT44(SUB84(dVar13,0),piVar6[5]);
        }
        else {
          in_xd2 = *(double *)(piVar6 + 5);
          in_dr2 = in_xd14;
        }
        if (in_FPSCR_PR == '\0') {
          in_dr2 = (double)CONCAT44((float)((ulonglong)in_dr2 >> 0x20) + SUB84(in_dr2,0),
                                    SUB84(in_dr2,0));
        }
        else {
          in_dr2 = in_dr2 + in_dr2;
        }
        if (in_FPSCR_SZ == '\0') {
          in_dr2 = (double)CONCAT44((int)((ulonglong)in_dr2 >> 0x20),
                                    *(undefined4 *)((int)param_4 + 4));
          pdVar9 = param_4 + 1;
          dVar16 = (double)CONCAT44((int)((ulonglong)dVar16 >> 0x20),piVar6[2]);
          pdVar10 = (double *)(piVar6 + 3);
        }
        else {
          in_xd2 = *(double *)((int)param_4 + 4);
          pdVar9 = (double *)((int)param_4 + 0xc);
          in_xd4 = *(double *)(piVar6 + 2);
          pdVar10 = (double *)(piVar6 + 4);
        }
        if (in_FPSCR_PR == '\0') {
          fVar14 = SUB84(dVar16,0) - SUB84(in_dr2,0);
        }
        else {
          fVar14 = SUB84(dVar16 - in_dr2,0);
        }
        uVar15 = (undefined4)((ulonglong)in_dr2 >> 0x20);
        dVar16 = in_dr2;
        if (in_FPSCR_SZ == '\0') {
          dVar16 = (double)CONCAT44(uVar15,fVar14);
        }
        fVar14 = (float)((ulonglong)dVar20 >> 0x20);
        if (in_FPSCR_PR == '\0') {
          dVar16 = (double)CONCAT44((float)((ulonglong)dVar16 >> 0x20) * fVar14,SUB84(dVar16,0));
        }
        else {
          dVar16 = dVar16 * dVar20;
        }
        dVar21 = in_xd4;
        if (in_FPSCR_SZ == '\0') {
          dVar20 = (double)CONCAT44(fVar14,SUB84(dVar16,0));
          dVar21 = in_xd6;
        }
        fVar14 = (float)((ulonglong)dVar16 >> 0x20);
        if (in_FPSCR_PR == '\0') {
          fVar19 = SUB84(dVar20,0) * SUB84(dVar16,0);
          dVar20 = (double)CONCAT44((int)((ulonglong)dVar20 >> 0x20),fVar19);
          bVar25 = fVar14 < fVar19;
        }
        else {
          dVar20 = dVar20 * dVar16;
          fVar19 = SUB84(dVar20,0);
          bVar25 = dVar16 < dVar20;
        }
        if (!bVar25) {
          if (in_FPSCR_SZ == '\0') {
            dVar16 = (double)CONCAT44(fVar14,*(undefined4 *)pdVar10);
            pdVar10 = (double *)((int)pdVar10 + 4);
            in_dr2 = (double)CONCAT44(uVar15,*(undefined4 *)pdVar9);
            pdVar9 = (double *)((int)pdVar9 + 4);
          }
          else {
            in_xd4 = *pdVar10;
            pdVar10 = pdVar10 + 1;
            in_xd2 = *pdVar9;
            pdVar9 = pdVar9 + 1;
          }
          if (in_FPSCR_PR == '\0') {
            dVar16 = (double)CONCAT44((int)((ulonglong)dVar16 >> 0x20),
                                      SUB84(dVar16,0) - SUB84(in_dr2,0));
          }
          else {
            dVar16 = dVar16 - in_dr2;
          }
          fVar14 = SUB84(dVar16,0);
          if (in_FPSCR_SZ != '\0') {
            fVar14 = (float)((ulonglong)in_xd4 >> 0x20);
          }
          fVar18 = SUB84(dVar20,0);
          fVar17 = (float)((ulonglong)dVar16 >> 0x20);
          if (in_FPSCR_PR == '\0') {
            fVar18 = fVar14 * SUB84(dVar16,0) + fVar19;
            dVar20 = (double)CONCAT44((int)((ulonglong)dVar20 >> 0x20),fVar18);
            bVar25 = fVar17 < fVar18;
          }
          else {
            bVar25 = dVar16 < dVar20;
          }
          if (!bVar25) {
            if (in_FPSCR_SZ == '\0') {
              dVar16 = (double)CONCAT44(fVar17,*(undefined4 *)pdVar10);
              in_dr2 = (double)CONCAT44((int)((ulonglong)in_dr2 >> 0x20),*(undefined4 *)pdVar9);
            }
            else {
              in_xd4 = *pdVar10;
              in_xd2 = *pdVar9;
            }
            if (in_FPSCR_PR == '\0') {
              dVar16 = (double)CONCAT44((int)((ulonglong)dVar16 >> 0x20),
                                        SUB84(dVar16,0) - SUB84(in_dr2,0));
            }
            else {
              dVar16 = dVar16 - in_dr2;
            }
            fVar14 = SUB84(dVar16,0);
            if (in_FPSCR_SZ != '\0') {
              fVar14 = (float)((ulonglong)in_xd4 >> 0x20);
            }
            if (in_FPSCR_PR == '\0') {
              fVar18 = fVar14 * SUB84(dVar16,0) + fVar18;
              dVar20 = (double)CONCAT44((int)((ulonglong)dVar20 >> 0x20),fVar18);
              bVar25 = (float)((ulonglong)dVar16 >> 0x20) < fVar18;
            }
            else {
              bVar25 = dVar16 < dVar20;
            }
            if (!bVar25) {
              if (puVar7[7] == 0) {
                puVar7[7] = piVar11;
              }
              puVar7[9] = puVar7[9] + 1;
              *piVar11 = (int)piVar6;
              piVar11 = piVar11 + 1;
              iVar8 = *piVar1;
              *piVar1 = iVar8 + 1;
              if (iVar8 + 1 == 0x60) {
                if (-1 < *pcVar3) {
                  (*(code *)puVar2)(uVar23,PTR_s_WARNING__NEAR_poly_is_FULL_8c028d6c);
                }
                goto LAB_8c028e18;
              }
            }
          }
        }
        in_xd6 = dVar21;
      }
    }
LAB_8c028e0e:
    puVar7 = (undefined4 *)*puVar7;
  } while( true );
}

