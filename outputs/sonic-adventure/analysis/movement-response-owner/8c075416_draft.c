/* Ghidra draft only; not buildable/verified source. */

/* WARNING: Removing unreachable block (ram,0x8c075538) */
/* WARNING: Removing unreachable block (ram,0x8c075624) */
/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined4 FUN_8c075416(double param_1,int param_2,undefined4 param_3,int param_4)

{
  undefined *puVar1;
  undefined *puVar2;
  int *piVar3;
  int iVar4;
  undefined4 *puVar5;
  int iVar6;
  int iVar7;
  ulonglong *puVar8;
  int *piVar9;
  int *piVar10;
  undefined8 uVar11;
  double dVar12;
  double dVar13;
  float fVar14;
  float fVar15;
  double in_dr2;
  uint uVar17;
  double dVar16;
  undefined4 uVar18;
  undefined8 in_dr12;
  ulonglong in_dr14;
  undefined4 uVar19;
  undefined8 in_xd2;
  undefined8 in_xd14;
  bool bVar20;
  undefined4 in_PR;
  char in_FPSCR_PR;
  char in_FPSCR_SZ;
  undefined1 auStack_3c [16];
  ulonglong uStack_2c;
  undefined8 uStack_24;
  
  puVar1 = PTR_FUN_8c0754ec;
  uVar18 = SUB84(param_1,0);
  if (in_FPSCR_SZ == '\0') {
    uStack_24 = CONCAT44((int)in_dr14,(undefined4)uStack_24);
                    /* WARNING: Ignoring partial resolution of indirect */
    uStack_24._0_4_ = (int)(in_dr14 >> 0x20);
    puVar8 = &uStack_2c;
    uStack_2c = CONCAT44((int)in_dr12,(int)((ulonglong)in_dr12 >> 0x20));
  }
  else {
    puVar8 = (ulonglong *)auStack_3c;
    uStack_24 = in_xd14;
    uStack_2c = in_dr14;
  }
  *(undefined4 *)((int)puVar8 + -4) = in_PR;
  piVar10 = (int *)((int)puVar8 + -0xac);
  dVar16 = (double)(in_dr14 & 0xffffffff);
  iVar7 = 0;
  *(undefined4 *)((int)puVar8 + -0xa4) = param_3;
  *(undefined4 *)((int)puVar8 + -0xa8) = 0;
  *(undefined4 *)((int)puVar8 + -0xa0) = 0xffffffff;
  if (in_FPSCR_SZ == '\0') {
    *(undefined4 *)((int)puVar8 + -0x88) = 0;
    *(undefined4 *)((int)puVar8 + -0x8c) = 0;
    *(undefined4 *)((int)puVar8 + -0x90) = 0;
    dVar13 = (double)CONCAT44(_DAT_8c0754e8,DAT_8c0754e0._4_4_);
  }
  else {
    *(double *)((int)puVar8 + -0x88) = dVar16;
    *(double *)((int)puVar8 + -0x8c) = dVar16;
    *(double *)((int)puVar8 + -0x90) = dVar16;
    dVar13 = (double)CONCAT44(PTR_FUN_8c0754ec,_DAT_8c0754e8);
  }
  piVar3 = (int *)((int)puVar8 + -0xac);
  if (*(short *)(param_4 + 0x28) != 0) {
    do {
      piVar10 = piVar3;
      *piVar10 = iVar7 * 4;
      piVar10[-1] = (int)((int)puVar8 + iVar7 * 4 + -0x44);
      iVar6 = iVar7 * 0x58 + DAT_8c0754f0;
      uVar11 = (*(code *)PTR_FUN_8c0754f4)(iVar6 + 0x2c,param_4 + 4);
      puVar2 = PTR_FUN_8c0754f8;
      if (in_FPSCR_SZ == '\0') {
        *(int *)piVar10[-1] = (int)((ulonglong)uVar11 >> 0x20);
      }
      else {
        *(undefined8 *)piVar10[-1] = uVar11;
      }
      *(undefined4 *)((int)puVar8 + *piVar10 + -0x84) = 0;
      iVar4 = (*(code *)puVar2)(param_4 + 0x1c,iVar6 + 0x14,0);
      *piVar10 = iVar4;
      if (iVar4 < 0x2c01) {
LAB_8c0754d2:
        *(uint *)((int)puVar8 + iVar7 * 4 + -0x84) = *(uint *)((int)puVar8 + iVar7 * 4 + -0x84) | 2;
      }
      else {
        uVar11 = (*(code *)PTR_FUN_8c075500)(DAT_8c0754fc,param_4 + 0x1c);
        if (in_FPSCR_SZ == '\0') {
          piVar9 = piVar10 + -1;
          piVar10[-1] = (int)((ulonglong)uVar11 >> 0x20);
        }
        else {
          piVar9 = piVar10 + -2;
          *(undefined8 *)(piVar10 + -2) = uVar11;
        }
        dVar12 = (double)(*(code *)PTR_FUN_8c075500)(DAT_8c0754fc,iVar6 + 0x14);
        if (in_FPSCR_SZ == '\0') {
          in_dr2 = (double)CONCAT44((int)((ulonglong)in_dr2 >> 0x20),*piVar9);
          piVar10 = (int *)((int)piVar9 + 4);
        }
        else {
          in_xd2 = *(undefined8 *)piVar9;
          piVar10 = (int *)((int)piVar9 + 8);
        }
        if (in_FPSCR_PR == '\0') {
          bVar20 = (float)((ulonglong)dVar12 >> 0x20) < SUB84(in_dr2,0);
        }
        else {
          bVar20 = dVar12 < in_dr2;
        }
        if (bVar20) goto LAB_8c0754d2;
        iVar4 = *(int *)(param_4 + 0x14);
        piVar10[4] = *(int *)(param_4 + 0x10);
        piVar10[5] = iVar4;
        piVar10[6] = *(int *)(param_4 + 0x18);
        fVar14 = (float)(*(code *)puVar1)(piVar10 + 4);
        uVar17 = (uint)((ulonglong)in_dr2 >> 0x20);
        if (fVar14 != 0.0) {
          (*(code *)PTR_FUN_8c0756c0)(piVar10 + 4);
        }
        dVar12 = (double)(*(code *)PTR_FUN_8c0756c4)(piVar10 + 4,iVar6 + 0x14);
        uVar18 = SUB84(param_1,0);
        in_dr2 = (double)((ulonglong)uVar17 << 0x20);
        fVar14 = (float)((ulonglong)dVar12 >> 0x20);
        if (in_FPSCR_PR == '\0') {
          bVar20 = fVar14 < 0.0;
        }
        else {
          bVar20 = dVar12 < in_dr2;
        }
        if (in_FPSCR_SZ == '\0') {
          dVar16 = (double)CONCAT44((int)((ulonglong)dVar16 >> 0x20),fVar14);
        }
        if (bVar20) {
          iVar4 = iVar7 * 4;
          *piVar10 = iVar4;
          *(uint *)((int)puVar8 + iVar4 + -0x84) = *(uint *)((int)puVar8 + iVar4 + -0x84) | 8;
          piVar10[1] = piVar10[1] + 1;
          if (in_FPSCR_SZ == '\0') {
            in_dr2 = (double)((ulonglong)*(uint *)((int)puVar8 + *piVar10 + -0x44) << 0x20);
          }
          else {
            in_dr2 = *(double *)((int)puVar8 + *piVar10 + -0x44);
          }
          if (in_FPSCR_PR == '\0') {
            bVar20 = (float)((ulonglong)in_dr2 >> 0x20) < SUB84(dVar13,0);
          }
          else {
            bVar20 = in_dr2 < dVar13;
          }
          if (bVar20) {
            if (in_FPSCR_SZ == '\0') {
              dVar13 = (double)CONCAT44((int)((ulonglong)dVar13 >> 0x20),
                                        *(undefined4 *)((int)puVar8 + *piVar10 + -0x44));
            }
            piVar10[3] = iVar7;
          }
          if ((piVar10[0x36] & DAT_8c0756c8) == 0) {
            dVar12 = (double)(*(code *)PTR_FUN_8c0756cc)(0x600);
            if (in_FPSCR_PR == '\0') {
              bVar20 = SUB84(dVar16,0) < -(float)((ulonglong)dVar12 >> 0x20);
            }
            else {
              bVar20 = dVar16 < -dVar12;
            }
            if (bVar20) {
              uVar19 = (undefined4)((ulonglong)dVar16 >> 0x20);
              if (in_FPSCR_SZ == '\0') {
                *(undefined4 *)(param_2 + 0xc) = uVar19;
                *(undefined4 *)(param_2 + 8) = uVar19;
                *(undefined4 *)(param_2 + 4) = uVar19;
              }
              else {
                *(double *)(param_2 + 0xc) = dVar16;
                *(double *)(param_2 + 8) = dVar16;
                *(double *)(param_2 + 4) = dVar16;
              }
            }
            param_1 = (double)(*(code *)PTR_FUN_8c0756c4)(param_4 + 0x10,iVar6 + 0x14);
            if (in_FPSCR_SZ == '\0') {
              param_1 = (double)CONCAT44((int)((ulonglong)param_1 >> 0x20),uVar18);
            }
            if (in_FPSCR_PR == '\0') {
              bVar20 = (float)((ulonglong)param_1 >> 0x20) < (float)((ulonglong)dVar13 >> 0x20);
            }
            else {
              bVar20 = param_1 < dVar13;
            }
            if (bVar20) {
              if (in_FPSCR_SZ == '\0') {
                dVar16 = (double)CONCAT44((int)((ulonglong)dVar16 >> 0x20),
                                          *(undefined4 *)(piVar10[2] + 0xc4));
              }
              dVar12 = (double)(*(code *)puVar1)(param_2 + 4);
              if (in_FPSCR_PR == '\0') {
                bVar20 = SUB84(dVar16,0) < (float)((ulonglong)dVar12 >> 0x20);
              }
              else {
                bVar20 = dVar16 < dVar12;
              }
              if (bVar20) {
                (*(code *)PTR_FUN_8c0756c0)(param_2 + 4);
                if (in_FPSCR_SZ == '\0') {
                  in_dr2 = (double)CONCAT44((int)((ulonglong)in_dr2 >> 0x20),
                                            *(undefined4 *)(param_2 + 4));
                }
                else {
                  in_xd2 = *(undefined8 *)(param_2 + 4);
                }
                fVar14 = SUB84(dVar16,0);
                if (in_FPSCR_PR == '\0') {
                  fVar15 = SUB84(in_dr2,0) * fVar14;
                }
                else {
                  fVar15 = SUB84(in_dr2 * dVar16,0);
                }
                if (in_FPSCR_SZ == '\0') {
                  *(float *)(param_2 + 4) = fVar15;
                  in_dr2 = (double)CONCAT44(*(undefined4 *)(param_2 + 8),fVar15);
                }
                else {
                  *(undefined8 *)(param_2 + 4) = in_xd2;
                  in_dr2 = *(double *)(param_2 + 8);
                }
                if (in_FPSCR_PR == '\0') {
                  in_dr2 = (double)CONCAT44((float)((ulonglong)in_dr2 >> 0x20) * fVar14,
                                            SUB84(in_dr2,0));
                }
                else {
                  in_dr2 = in_dr2 * dVar16;
                }
                uVar18 = (undefined4)((ulonglong)in_dr2 >> 0x20);
                if (in_FPSCR_SZ == '\0') {
                  *(undefined4 *)(param_2 + 8) = uVar18;
                  in_dr2 = (double)CONCAT44(uVar18,*(undefined4 *)(param_2 + 0xc));
                }
                else {
                  *(double *)(param_2 + 8) = in_dr2;
                  in_xd2 = *(undefined8 *)(param_2 + 0xc);
                }
                if (in_FPSCR_PR == '\0') {
                  in_dr2 = (double)CONCAT44((int)((ulonglong)in_dr2 >> 0x20),
                                            SUB84(in_dr2,0) * fVar14);
                }
                else {
                  in_dr2 = in_dr2 * dVar16;
                }
                if (in_FPSCR_SZ == '\0') {
                  *(int *)(param_2 + 0xc) = SUB84(in_dr2,0);
                }
                else {
                  *(undefined8 *)(param_2 + 0xc) = in_xd2;
                }
              }
            }
          }
          (*(code *)PTR_FUN_8c0756d0)(piVar10 + 7,iVar6 + 0x14);
        }
      }
      uVar18 = SUB84(param_1,0);
      iVar7 = iVar7 + 1;
      piVar3 = piVar10;
    } while (iVar7 < (int)(uint)*(ushort *)(param_4 + 0x28));
  }
  if (0 < *(int *)((int)piVar10 + 4)) {
    dVar13 = (double)(*(code *)puVar1)(param_4 + 0x10);
    iVar7 = *(int *)((int)piVar10 + 0xc);
    if (in_FPSCR_SZ == '\0') {
      dVar13 = (double)CONCAT44((int)((ulonglong)dVar13 >> 0x20),uVar18);
    }
    if (in_FPSCR_SZ == '\0') {
      in_dr2 = (double)CONCAT44((int)((ulonglong)in_dr2 >> 0x20),
                                *(undefined4 *)((int)puVar8 + iVar7 * 4 + -0x44));
    }
    if (in_FPSCR_PR == '\0') {
      bVar20 = SUB84(in_dr2,0) < (float)((ulonglong)dVar13 >> 0x20);
    }
    else {
      bVar20 = in_dr2 < dVar13;
    }
    if (bVar20) {
      fVar14 = (float)(*(code *)puVar1)(param_4 + 0x10);
      if (fVar14 == 0.0) {
        uVar18 = (undefined4)((ulonglong)dVar16 >> 0x20);
        if (in_FPSCR_SZ == '\0') {
          *(undefined4 *)(param_4 + 0x18) = uVar18;
          *(undefined4 *)(param_4 + 0x14) = uVar18;
          *(undefined4 *)(param_4 + 0x10) = uVar18;
        }
        else {
          *(double *)(param_4 + 0x18) = dVar16;
          *(double *)(param_4 + 0x14) = dVar16;
          *(double *)(param_4 + 0x10) = dVar16;
        }
      }
      else {
        (*(code *)PTR_FUN_8c0756c0)(param_4 + 0x10);
        puVar5 = (undefined4 *)((int)puVar8 + iVar7 * 4 + -0x44);
        if (in_FPSCR_SZ == '\0') {
          dVar16 = (double)CONCAT44(*(undefined4 *)(param_4 + 0x10),*puVar5);
        }
        else {
          dVar16 = *(double *)(param_4 + 0x10);
        }
        if (in_FPSCR_PR == '\0') {
          dVar16 = (double)CONCAT44((float)((ulonglong)dVar16 >> 0x20) * SUB84(dVar16,0),
                                    SUB84(dVar16,0));
        }
        else {
          dVar16 = dVar16 * dVar16;
        }
        if (in_FPSCR_SZ == '\0') {
          *(int *)(param_4 + 0x10) = (int)((ulonglong)dVar16 >> 0x20);
          dVar16 = (double)CONCAT44(*(undefined4 *)(param_4 + 0x14),*puVar5);
        }
        else {
          *(double *)(param_4 + 0x10) = dVar16;
          dVar16 = *(double *)(param_4 + 0x14);
        }
        if (in_FPSCR_PR == '\0') {
          dVar16 = (double)CONCAT44((float)((ulonglong)dVar16 >> 0x20) * SUB84(dVar16,0),
                                    SUB84(dVar16,0));
        }
        else {
          dVar16 = dVar16 * dVar16;
        }
        if (in_FPSCR_SZ == '\0') {
          *(int *)(param_4 + 0x14) = (int)((ulonglong)dVar16 >> 0x20);
          dVar16 = (double)CONCAT44(*(undefined4 *)(param_4 + 0x18),*puVar5);
        }
        else {
          *(double *)(param_4 + 0x14) = dVar16;
          dVar16 = *(double *)(param_4 + 0x18);
        }
        if (in_FPSCR_PR == '\0') {
          dVar16 = (double)CONCAT44((float)((ulonglong)dVar16 >> 0x20) * SUB84(dVar16,0),
                                    SUB84(dVar16,0));
        }
        else {
          dVar16 = dVar16 * dVar16;
        }
        if (in_FPSCR_SZ == '\0') {
          *(int *)(param_4 + 0x18) = (int)((ulonglong)dVar16 >> 0x20);
        }
        else {
          *(double *)(param_4 + 0x18) = dVar16;
        }
      }
      return 1;
    }
  }
  return 0;
}

