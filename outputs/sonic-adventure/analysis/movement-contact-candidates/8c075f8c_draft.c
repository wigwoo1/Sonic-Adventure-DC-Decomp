/* Ghidra draft only; not buildable/verified source. */

/* WARNING: Removing unreachable block (ram,0x8c0761f0) */
/* WARNING: Removing unreachable block (ram,0x8c0761c2) */
/* WARNING: Removing unreachable block (ram,0x8c0761aa) */
/* WARNING: Removing unreachable block (ram,0x8c076098) */
/* WARNING: Removing unreachable block (ram,0x8c076142) */
/* WARNING: Removing unreachable block (ram,0x8c0761b6) */
/* WARNING: Removing unreachable block (ram,0x8c0761cc) */
/* WARNING: Removing unreachable block (ram,0x8c0762fa) */
/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined4 FUN_8c075f8c(undefined8 param_1,undefined4 param_2,undefined4 param_3,int param_4)

{
  undefined *puVar1;
  undefined *puVar2;
  int iVar3;
  undefined4 uVar4;
  undefined4 uVar5;
  int iVar6;
  int iVar7;
  int iVar8;
  undefined1 *puVar9;
  float fVar10;
  undefined8 uVar11;
  float fVar14;
  double dVar12;
  double dVar13;
  double in_dr2;
  uint uVar15;
  float in_fr13;
  undefined4 in_fr12;
  double in_dr14;
  undefined8 in_xd0;
  undefined8 in_xd2;
  undefined8 in_xd14;
  bool bVar16;
  undefined4 in_PR;
  char in_FPSCR_PR;
  char in_FPSCR_SZ;
  undefined1 auStack_34 [12];
  undefined1 local_28 [4];
  undefined8 uStack_24;
  
  puVar1 = PTR_FUN_8c076064;
  if (in_FPSCR_SZ == '\0') {
    uStack_24 = CONCAT44(SUB84(in_dr14,0),(undefined4)uStack_24);
                    /* WARNING: Ignoring partial resolution of indirect */
    uStack_24._0_4_ = (int)((ulonglong)in_dr14 >> 0x20);
    puVar9 = local_28;
  }
  else {
    puVar9 = auStack_34;
    uStack_24 = in_xd14;
  }
  *(undefined4 *)(puVar9 + -4) = in_PR;
  iVar6 = 0;
  *(undefined4 *)(puVar9 + -0x104) = param_2;
  *(undefined4 *)(puVar9 + -0xfc) = param_3;
  *(undefined4 *)(puVar9 + -0xf8) = 0;
  *(undefined4 *)(puVar9 + -0xf4) = 0xffffffff;
  if (in_FPSCR_SZ == '\0') {
    in_dr14 = (double)(ulonglong)DAT_8c076060;
  }
  do {
    uVar4 = (undefined4)param_1;
    if ((int)(uint)*(ushort *)(param_4 + 0x28) <= iVar6) {
      if (0 < *(int *)(puVar9 + -0xf8)) {
        dVar12 = (double)(*(code *)puVar1)(param_4 + 0x10);
        if (in_FPSCR_SZ == '\0') {
          dVar12 = (double)CONCAT44((int)((ulonglong)dVar12 >> 0x20),uVar4);
        }
        if (in_FPSCR_SZ == '\0') {
          in_dr2 = (double)CONCAT44((int)((ulonglong)in_dr2 >> 0x20),
                                    *(undefined4 *)(puVar9 + *(int *)(puVar9 + -0xf4) * 4 + -0xa4));
        }
        else {
          in_xd2 = *(undefined8 *)(puVar9 + *(int *)(puVar9 + -0xf4) * 4 + -0xa4);
        }
        if (in_FPSCR_PR == '\0') {
          bVar16 = SUB84(in_dr2,0) < (float)((ulonglong)dVar12 >> 0x20);
        }
        else {
          bVar16 = in_dr2 < dVar12;
        }
        if ((bVar16) && ((*(uint *)(*(int *)(puVar9 + -0xfc) + 0x60) & 0x4000) == 0)) {
          dVar13 = (double)(*(code *)puVar1)(param_4 + 0x6c);
          uVar4 = SUB84(dVar12,0);
          fVar14 = (float)((ulonglong)dVar13 >> 0x20);
          if (fVar14 == 0.0) {
            if (in_FPSCR_SZ == '\0') {
              *(undefined4 *)(param_4 + 0x74) = 0;
              *(undefined4 *)(param_4 + 0x70) = 0;
              *(undefined4 *)(param_4 + 0x6c) = 0;
            }
            else {
              *(undefined8 *)(param_4 + 0x74) = in_xd2;
              *(undefined8 *)(param_4 + 0x70) = in_xd2;
              *(undefined8 *)(param_4 + 0x6c) = in_xd2;
            }
          }
          else {
            if (in_FPSCR_SZ == '\0') {
              dVar12 = (double)((ulonglong)*(uint *)(param_4 + 0x78) << 0x20);
              dVar13 = (double)CONCAT44(fVar14,*(undefined4 *)(param_4 + 0x6c));
            }
            else {
              dVar12 = *(double *)(param_4 + 0x78);
              in_xd0 = *(undefined8 *)(param_4 + 0x6c);
            }
            if (in_FPSCR_PR == '\0') {
              dVar13 = (double)CONCAT44((int)((ulonglong)dVar13 >> 0x20),
                                        SUB84(dVar13,0) + (float)((ulonglong)dVar12 >> 0x20));
            }
            else {
              dVar13 = dVar13 + dVar12;
            }
            if (in_FPSCR_SZ == '\0') {
              *(int *)(param_4 + 0x6c) = SUB84(dVar13,0);
              dVar13 = (double)CONCAT44((int)((ulonglong)dVar13 >> 0x20),
                                        *(undefined4 *)(param_4 + 0x70));
              dVar12 = (double)CONCAT44(*(undefined4 *)(param_4 + 0x7c),SUB84(dVar12,0));
            }
            else {
              *(undefined8 *)(param_4 + 0x6c) = in_xd0;
              in_xd0 = *(undefined8 *)(param_4 + 0x70);
              dVar12 = *(double *)(param_4 + 0x7c);
            }
            if (in_FPSCR_PR == '\0') {
              dVar13 = (double)CONCAT44((int)((ulonglong)dVar13 >> 0x20),
                                        SUB84(dVar13,0) + (float)((ulonglong)dVar12 >> 0x20));
            }
            else {
              dVar13 = dVar13 + dVar12;
            }
            if (in_FPSCR_SZ == '\0') {
              *(int *)(param_4 + 0x70) = SUB84(dVar13,0);
              dVar13 = (double)CONCAT44((int)((ulonglong)dVar13 >> 0x20),
                                        *(undefined4 *)(param_4 + 0x74));
              dVar12 = (double)CONCAT44(*(undefined4 *)(param_4 + 0x80),SUB84(dVar12,0));
            }
            else {
              *(undefined8 *)(param_4 + 0x70) = in_xd0;
              in_xd0 = *(undefined8 *)(param_4 + 0x74);
              dVar12 = *(double *)(param_4 + 0x80);
            }
            iVar6 = _DAT_8c076230;
            fVar14 = (float)((ulonglong)dVar12 >> 0x20);
            if (in_FPSCR_PR == '\0') {
              fVar10 = SUB84(dVar13,0) + fVar14;
            }
            else {
              fVar10 = SUB84(dVar13 + dVar12,0);
            }
            if (in_FPSCR_SZ == '\0') {
              *(float *)(param_4 + 0x74) = fVar10;
            }
            else {
              *(undefined8 *)(param_4 + 0x74) = in_xd0;
            }
            iVar7 = *(int *)(puVar9 + -0xfc);
            iVar6 = *(int *)(puVar9 + -0xf4) * 0x58 + iVar6;
            uVar5 = *(undefined4 *)(iVar6 + 0x18);
            *(undefined4 *)(iVar7 + 0x44) = *(undefined4 *)(iVar6 + 0x14);
            *(undefined4 *)(iVar7 + 0x48) = uVar5;
            *(undefined4 *)(iVar7 + 0x4c) = *(undefined4 *)(iVar6 + 0x1c);
            iVar7 = *(int *)(puVar9 + -0x104);
            if (in_FPSCR_SZ == '\0') {
              dVar12 = (double)CONCAT44(fVar14,*(undefined4 *)(iVar7 + 4));
            }
            else {
              in_xd2 = *(undefined8 *)(iVar7 + 4);
            }
            uVar15 = (uint)((ulonglong)dVar12 >> 0x20);
            fVar14 = -SUB84(dVar12,0);
            if (in_FPSCR_SZ == '\0') {
              *(float *)(puVar9 + -100) = fVar14;
              fVar14 = *(float *)(iVar7 + 8);
            }
            else {
              *(undefined8 *)(puVar9 + -100) = in_xd2;
              in_xd2 = *(undefined8 *)(iVar7 + 8);
            }
            fVar14 = -fVar14;
            if (in_FPSCR_SZ == '\0') {
              *(float *)(puVar9 + -0x60) = fVar14;
              fVar14 = *(float *)(iVar7 + 0xc);
            }
            else {
              *(undefined8 *)(puVar9 + -0x60) = in_xd2;
              in_xd2 = *(undefined8 *)(iVar7 + 0xc);
            }
            if (in_FPSCR_SZ == '\0') {
              *(float *)(puVar9 + -0x5c) = -fVar14;
            }
            else {
              *(undefined8 *)(puVar9 + -0x5c) = in_xd2;
            }
            fVar14 = (float)(*(code *)puVar1)(puVar9 + -100);
            if (fVar14 != 0.0) {
              (*(code *)PTR_FUN_8c076234)
                        (param_4 + 0x6c,*(int *)(puVar9 + -0xfc) + 0x44,puVar9 + -0x58);
              fVar14 = (float)(*(code *)puVar1)(puVar9 + -0x58);
              if (fVar14 == 0.0) {
                uVar5 = *(undefined4 *)(param_4 + 0x20);
                *(undefined4 *)(puVar9 + -0x58) = *(undefined4 *)(param_4 + 0x1c);
                *(undefined4 *)(puVar9 + -0x54) = uVar5;
                *(undefined4 *)(puVar9 + -0x50) = *(undefined4 *)(param_4 + 0x24);
              }
              else {
                (*(code *)PTR_FUN_8c076420)(puVar9 + -0x58);
              }
              if (in_FPSCR_SZ == '\0') {
                *(undefined4 *)(puVar9 + -0x1c) = *(undefined4 *)(iVar6 + 0x20);
                *(undefined4 *)(puVar9 + -0x18) = *(undefined4 *)(iVar6 + 0x24);
                *(undefined4 *)(puVar9 + -0x14) = *(undefined4 *)(iVar6 + 0x28);
                *(undefined4 *)(puVar9 + -0x10) = *(undefined4 *)(iVar6 + 0x14);
                *(undefined4 *)(puVar9 + -0xc) = *(undefined4 *)(iVar6 + 0x18);
                *(undefined4 *)(puVar9 + -8) = *(undefined4 *)(iVar6 + 0x1c);
                *(undefined4 *)(puVar9 + -0x34) = *(undefined4 *)(param_4 + 0x60);
                *(undefined4 *)(puVar9 + -0x30) = *(undefined4 *)(param_4 + 100);
                *(undefined4 *)(puVar9 + -0x2c) = *(undefined4 *)(param_4 + 0x68);
                *(undefined4 *)(puVar9 + -0x28) = *(undefined4 *)(puVar9 + -0x58);
                *(undefined4 *)(puVar9 + -0x24) = *(undefined4 *)(puVar9 + -0x54);
                *(undefined4 *)(puVar9 + -0x20) = *(undefined4 *)(puVar9 + -0x50);
              }
              else {
                *(undefined8 *)(puVar9 + -0x1c) = *(undefined8 *)(iVar6 + 0x20);
                *(undefined8 *)(puVar9 + -0x18) = *(undefined8 *)(iVar6 + 0x24);
                *(undefined8 *)(puVar9 + -0x14) = *(undefined8 *)(iVar6 + 0x28);
                *(undefined8 *)(puVar9 + -0x10) = *(undefined8 *)(iVar6 + 0x14);
                *(undefined8 *)(puVar9 + -0xc) = *(undefined8 *)(iVar6 + 0x18);
                *(undefined8 *)(puVar9 + -8) = *(undefined8 *)(iVar6 + 0x1c);
                *(undefined8 *)(puVar9 + -0x34) = *(undefined8 *)(param_4 + 0x60);
                *(undefined8 *)(puVar9 + -0x30) = *(undefined8 *)(param_4 + 100);
                *(undefined8 *)(puVar9 + -0x2c) = *(undefined8 *)(param_4 + 0x68);
                *(undefined8 *)(puVar9 + -0x28) = *(undefined8 *)(puVar9 + -0x58);
                *(undefined8 *)(puVar9 + -0x24) = *(undefined8 *)(puVar9 + -0x54);
                in_xd2 = *(undefined8 *)(puVar9 + -0x50);
                *(undefined8 *)(puVar9 + -0x20) = in_xd2;
              }
              (*(code *)PTR_FUN_8c076424)(puVar9 + -0x1c,puVar9 + -0x34,puVar9 + -0x4c);
              uVar5 = *(undefined4 *)(param_4 + 0x70);
              *(undefined4 *)(puVar9 + -0xf0) = *(undefined4 *)(param_4 + 0x6c);
              *(undefined4 *)(puVar9 + -0xec) = uVar5;
              *(undefined4 *)(puVar9 + -0xe8) = *(undefined4 *)(param_4 + 0x74);
              fVar14 = (float)(*(code *)puVar1)(puVar9 + -0xf0);
              if (fVar14 != 0.0) {
                (*(code *)PTR_FUN_8c076420)(puVar9 + -0xf0);
              }
              dVar12 = (double)(*(code *)PTR_FUN_8c076428)(puVar9 + -0x40,puVar9 + -0xf0);
              dVar13 = (double)((ulonglong)uVar15 << 0x20);
              if (in_FPSCR_PR == '\0') {
                bVar16 = (float)((ulonglong)dVar12 >> 0x20) < 0.0;
              }
              else {
                bVar16 = dVar12 < dVar13;
              }
              if (bVar16) {
                if (in_FPSCR_SZ == '\0') {
                  dVar12 = (double)((ulonglong)*(uint *)(puVar9 + -0x40) << 0x20);
                }
                else {
                  dVar12 = *(double *)(puVar9 + -0x40);
                }
                if (in_FPSCR_PR == '\0') {
                  dVar12 = (double)CONCAT44(-(float)((ulonglong)dVar12 >> 0x20),SUB84(dVar12,0));
                }
                else {
                  dVar12 = -dVar12;
                }
                if (in_FPSCR_SZ == '\0') {
                  *(int *)(puVar9 + -0x40) = (int)((ulonglong)dVar12 >> 0x20);
                  dVar12 = (double)CONCAT44(*(undefined4 *)(puVar9 + -0x3c),SUB84(dVar12,0));
                }
                else {
                  *(double *)(puVar9 + -0x40) = dVar12;
                  dVar12 = *(double *)(puVar9 + -0x3c);
                }
                if (in_FPSCR_PR == '\0') {
                  dVar12 = (double)CONCAT44(-(float)((ulonglong)dVar12 >> 0x20),SUB84(dVar12,0));
                }
                else {
                  dVar12 = -dVar12;
                }
                if (in_FPSCR_SZ == '\0') {
                  *(int *)(puVar9 + -0x3c) = (int)((ulonglong)dVar12 >> 0x20);
                  dVar13 = (double)CONCAT44(*(undefined4 *)(puVar9 + -0x38),SUB84(dVar12,0));
                }
                else {
                  *(double *)(puVar9 + -0x3c) = dVar12;
                  dVar13 = *(double *)(puVar9 + -0x38);
                }
                if (in_FPSCR_PR == '\0') {
                  dVar13 = (double)CONCAT44(-(float)((ulonglong)dVar13 >> 0x20),SUB84(dVar13,0));
                }
                else {
                  dVar13 = -dVar13;
                }
                if (in_FPSCR_SZ == '\0') {
                  *(int *)(puVar9 + -0x38) = (int)((ulonglong)dVar13 >> 0x20);
                }
                else {
                  *(double *)(puVar9 + -0x38) = dVar13;
                }
              }
              iVar6 = *(int *)(puVar9 + -0x104);
              dVar12 = (double)(*(code *)puVar1)(iVar6 + 4);
              if (in_FPSCR_SZ == '\0') {
                dVar12 = (double)CONCAT44((int)((ulonglong)dVar12 >> 0x20),uVar4);
                dVar13 = (double)CONCAT44((int)((ulonglong)dVar13 >> 0x20),
                                          *(undefined4 *)(puVar9 + -0x40));
              }
              else {
                in_xd2 = *(undefined8 *)(puVar9 + -0x40);
              }
              fVar14 = (float)((ulonglong)dVar12 >> 0x20);
              if (in_FPSCR_PR == '\0') {
                dVar13 = (double)CONCAT44((int)((ulonglong)dVar13 >> 0x20),SUB84(dVar13,0) * fVar14)
                ;
              }
              else {
                dVar13 = dVar13 * dVar12;
              }
              if (in_FPSCR_SZ == '\0') {
                *(int *)(iVar6 + 4) = SUB84(dVar13,0);
                dVar13 = (double)CONCAT44((int)((ulonglong)dVar13 >> 0x20),
                                          *(undefined4 *)(puVar9 + -0x3c));
              }
              else {
                *(undefined8 *)(iVar6 + 4) = in_xd2;
                in_xd2 = *(undefined8 *)(puVar9 + -0x3c);
              }
              if (in_FPSCR_PR == '\0') {
                dVar13 = (double)CONCAT44((int)((ulonglong)dVar13 >> 0x20),SUB84(dVar13,0) * fVar14)
                ;
              }
              else {
                dVar13 = dVar13 * dVar12;
              }
              if (in_FPSCR_SZ == '\0') {
                *(int *)(iVar6 + 8) = SUB84(dVar13,0);
                dVar13 = (double)CONCAT44((int)((ulonglong)dVar13 >> 0x20),
                                          *(undefined4 *)(puVar9 + -0x38));
              }
              else {
                *(undefined8 *)(iVar6 + 8) = in_xd2;
                in_xd2 = *(undefined8 *)(puVar9 + -0x38);
              }
              if (in_FPSCR_PR == '\0') {
                fVar14 = SUB84(dVar13,0) * fVar14;
              }
              else {
                fVar14 = SUB84(dVar13 * dVar12,0);
              }
              if (in_FPSCR_SZ == '\0') {
                *(float *)(iVar6 + 0xc) = fVar14;
              }
              else {
                *(undefined8 *)(iVar6 + 0xc) = in_xd2;
              }
              uVar4 = *(undefined4 *)(iVar6 + 8);
              *(undefined4 *)(param_4 + 0x6c) = *(undefined4 *)(iVar6 + 4);
              *(undefined4 *)(param_4 + 0x70) = uVar4;
              *(undefined4 *)(param_4 + 0x74) = *(undefined4 *)(iVar6 + 0xc);
              return 0xc;
            }
          }
        }
      }
      return 0;
    }
    iVar8 = iVar6 * 4;
    *(undefined1 **)(puVar9 + -0x108) = puVar9 + iVar8 + -0xa4;
    iVar7 = iVar6 * 0x58 + DAT_8c076068;
    uVar11 = (*(code *)PTR_FUN_8c07606c)(iVar7 + 0x2c,param_4 + 4);
    puVar2 = PTR_FUN_8c076070;
    if (in_FPSCR_SZ == '\0') {
      *(int *)*(undefined8 **)(puVar9 + -0x108) = (int)((ulonglong)uVar11 >> 0x20);
    }
    else {
      **(undefined8 **)(puVar9 + -0x108) = uVar11;
    }
    *(undefined4 *)(puVar9 + iVar8 + -0xe4) = 0;
    iVar3 = (*(code *)puVar2)(param_4 + 0x1c,iVar7 + 0x14,0);
    *(int *)(puVar9 + -0x100) = iVar3;
    if (iVar3 < 0x2c01) {
LAB_8c076016:
      *(uint *)(puVar9 + iVar8 + -0xe4) = *(uint *)(puVar9 + iVar8 + -0xe4) | 2;
    }
    else {
      if (in_FPSCR_SZ == '\0') {
        in_dr2 = (double)CONCAT44(*(undefined4 *)(param_4 + 0x20),*(undefined4 *)(iVar7 + 0x18));
      }
      else {
        in_xd2 = *(undefined8 *)(iVar7 + 0x18);
        in_dr2 = *(double *)(param_4 + 0x20);
      }
      if (in_FPSCR_PR == '\0' && (float)((ulonglong)in_dr2 >> 0x20) < SUB84(in_dr2,0))
      goto LAB_8c076016;
      uVar4 = *(undefined4 *)(param_4 + 0x14);
      *(undefined4 *)(puVar9 + -0xf0) = *(undefined4 *)(param_4 + 0x10);
      *(undefined4 *)(puVar9 + -0xec) = uVar4;
      *(undefined4 *)(puVar9 + -0xe8) = *(undefined4 *)(param_4 + 0x18);
      fVar14 = (float)(*(code *)puVar1)(puVar9 + -0xf0);
      in_dr2 = (double)((ulonglong)in_dr2 & 0xffffffff00000000);
      if (fVar14 != 0.0) {
        (*(code *)PTR_FUN_8c076220)(puVar9 + -0xf0);
      }
      if (*(int *)(puVar9 + -0x100) < 0x7000) {
        dVar12 = (double)(*(code *)PTR_FUN_8c076224)(puVar9 + -0xf0,iVar7 + 0x14);
        uVar4 = SUB84(in_dr14,0);
        in_dr2 = (double)((ulonglong)in_dr2 & 0xffffffff00000000);
        fVar14 = (float)((ulonglong)dVar12 >> 0x20);
        if (in_FPSCR_PR == '\0') {
          bVar16 = fVar14 < 0.0;
        }
        else {
          bVar16 = dVar12 < in_dr2;
        }
        if (in_FPSCR_SZ == '\0') {
          in_fr13 = fVar14;
        }
        if (bVar16) {
          in_dr14 = (double)(*(code *)PTR_FUN_8c076224)(param_4 + 0x10,iVar7 + 0x14);
          if (in_FPSCR_SZ == '\0') {
            in_dr14 = (double)CONCAT44((int)((ulonglong)in_dr14 >> 0x20),uVar4);
          }
          dVar12 = (double)(*(code *)PTR_FUN_8c076228)(0x600);
          if (in_FPSCR_PR == '\0') {
            bVar16 = in_fr13 < -(float)((ulonglong)dVar12 >> 0x20);
          }
          else {
            bVar16 = (double)CONCAT44(in_fr12,in_fr13) < -dVar12;
          }
          if (!bVar16) {
            if (in_FPSCR_SZ == '\0') {
              in_dr2 = (double)CONCAT44((int)((ulonglong)in_dr2 >> 0x20),DAT_8c07622c);
            }
            else {
              in_xd2 = CONCAT44(_DAT_8c076230,DAT_8c07622c);
            }
            if (in_FPSCR_PR == '\0') {
              bVar16 = (float)((ulonglong)in_dr14 >> 0x20) < SUB84(in_dr2,0);
            }
            else {
              bVar16 = in_dr14 < in_dr2;
            }
            if (!bVar16) goto LAB_8c076104;
          }
          *(uint *)(puVar9 + iVar8 + -0xe4) = *(uint *)(puVar9 + iVar8 + -0xe4) | 8;
          *(int *)(puVar9 + -0xf8) = *(int *)(puVar9 + -0xf8) + 1;
          if (in_FPSCR_SZ == '\0') {
            in_dr2 = (double)CONCAT44((int)((ulonglong)in_dr2 >> 0x20),
                                      *(undefined4 *)(puVar9 + iVar8 + -0xa4));
          }
          else {
            in_xd2 = *(undefined8 *)(puVar9 + iVar8 + -0xa4);
          }
          if (in_FPSCR_PR == '\0') {
            bVar16 = SUB84(in_dr2,0) < SUB84(in_dr14,0);
          }
          else {
            bVar16 = in_dr2 < in_dr14;
          }
          if (bVar16) {
            if (in_FPSCR_SZ == '\0') {
              in_dr14 = (double)(ulonglong)*(uint *)(puVar9 + iVar8 + -0xa4);
            }
            *(int *)(puVar9 + -0xf4) = iVar6;
          }
        }
      }
    }
LAB_8c076104:
    iVar6 = iVar6 + 1;
  } while( true );
}

