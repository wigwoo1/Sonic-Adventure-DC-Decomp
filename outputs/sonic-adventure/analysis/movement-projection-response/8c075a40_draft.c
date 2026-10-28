/* Ghidra draft only; not buildable/verified source. */

/* WARNING: Removing unreachable block (ram,0x8c075d98) */
/* WARNING: Removing unreachable block (ram,0x8c075cea) */
/* WARNING: Removing unreachable block (ram,0x8c075cce) */
/* WARNING: Removing unreachable block (ram,0x8c075b5a) */
/* WARNING: Removing unreachable block (ram,0x8c075c1a) */
/* WARNING: Removing unreachable block (ram,0x8c075cdc) */
/* WARNING: Removing unreachable block (ram,0x8c075cf4) */
/* WARNING: Removing unreachable block (ram,0x8c075e9a) */

int FUN_8c075a40(undefined8 param_1,undefined4 param_2,int param_3,int param_4,int param_5)

{
  undefined *puVar1;
  int *piVar2;
  int iVar3;
  undefined4 uVar4;
  undefined4 uVar5;
  undefined4 uVar6;
  undefined8 *puVar7;
  int iVar8;
  int iVar9;
  double *pdVar10;
  int *piVar11;
  int *piVar12;
  double dVar13;
  float fVar14;
  float fVar15;
  double in_dr2;
  undefined8 uVar16;
  double dVar17;
  ulonglong in_dr12;
  double in_dr14;
  undefined8 in_xd2;
  undefined8 in_xd14;
  bool bVar18;
  undefined4 in_PR;
  uint uVar19;
  char in_FPSCR_PR;
  char in_FPSCR_SZ;
  undefined1 auStack_3c [16];
  double dStack_2c;
  undefined8 uStack_24;
  
  puVar1 = PTR_FUN_8c075c44;
  uVar5 = (undefined4)param_1;
  uVar4 = (undefined4)((ulonglong)in_dr14 >> 0x20);
  if (in_FPSCR_SZ == '\0') {
    uStack_24 = CONCAT44(SUB84(in_dr14,0),(undefined4)uStack_24);
                    /* WARNING: Ignoring partial resolution of indirect */
    uStack_24._0_4_ = uVar4;
    pdVar10 = &dStack_2c;
    dStack_2c = (double)CONCAT44((int)in_dr12,(int)(in_dr12 >> 0x20));
  }
  else {
    pdVar10 = (double *)auStack_3c;
    uStack_24 = in_xd14;
    dStack_2c = in_dr14;
  }
  *(undefined4 *)((int)pdVar10 + -4) = in_PR;
  piVar12 = (int *)((int)pdVar10 + -0x11c);
  dVar17 = (double)(in_dr12 & 0xffffffff);
  *(undefined4 *)((int)pdVar10 + -0x118) = param_2;
  *(undefined4 *)((int)pdVar10 + -0x114) = 0;
  *(undefined4 *)((int)pdVar10 + -0x10c) = 0xffffffff;
  if (in_FPSCR_SZ == '\0') {
    in_dr14 = (double)CONCAT44(uVar4,DAT_8c075c40);
    *(undefined4 *)((int)pdVar10 + -0x68) = 0;
    *(undefined4 *)((int)pdVar10 + -0x6c) = 0;
    *(undefined4 *)((int)pdVar10 + -0x70) = 0;
  }
  else {
    *(double *)((int)pdVar10 + -0x68) = dVar17;
    *(double *)((int)pdVar10 + -0x6c) = dVar17;
    *(double *)((int)pdVar10 + -0x70) = dVar17;
  }
  *(undefined4 *)((int)pdVar10 + -0x7c) = *(undefined4 *)((int)pdVar10 + -0x70);
  *(undefined4 *)((int)pdVar10 + -0x78) = *(undefined4 *)((int)pdVar10 + -0x6c);
  *(undefined4 *)((int)pdVar10 + -0x74) = *(undefined4 *)((int)pdVar10 + -0x68);
  *(uint *)((int)pdVar10 + -0x110) = *(uint *)((int)pdVar10 + 0x2c) & 0x4000;
  iVar8 = 0;
  piVar2 = (int *)((int)pdVar10 + -0x11c);
  if (*(short *)(param_5 + 0x28) != 0) {
    do {
      piVar12 = piVar2;
      *piVar12 = iVar8 * 4;
      piVar12[-1] = (int)((int)pdVar10 + iVar8 * 4 + -0xbc);
      iVar9 = iVar8 * 0x58 + DAT_8c075c48;
      uVar16 = (*(code *)PTR_FUN_8c075c4c)(iVar9 + 0x2c,param_5 + 4);
      if (in_FPSCR_SZ == '\0') {
        *(int *)piVar12[-1] = (int)((ulonglong)uVar16 >> 0x20);
      }
      else {
        *(undefined8 *)piVar12[-1] = uVar16;
      }
      *(undefined4 *)((int)piVar12 + *piVar12 + 0x20) = 0;
      iVar3 = (*(code *)PTR_FUN_8c075c50)(param_5 + 0x1c,iVar9 + 0x14,0);
      *piVar12 = iVar3;
      if (iVar3 < 0x2c01) {
LAB_8c075b2e:
        *(uint *)((int)piVar12 + iVar8 * 4 + 0x20) = *(uint *)((int)piVar12 + iVar8 * 4 + 0x20) | 2;
      }
      else {
        uVar16 = (*(code *)PTR_FUN_8c075c58)(DAT_8c075c54,param_5 + 0x1c);
        if (in_FPSCR_SZ == '\0') {
          piVar11 = piVar12 + -1;
          piVar12[-1] = (int)((ulonglong)uVar16 >> 0x20);
        }
        else {
          piVar11 = piVar12 + -2;
          *(undefined8 *)(piVar12 + -2) = uVar16;
        }
        dVar13 = (double)(*(code *)PTR_FUN_8c075c58)(DAT_8c075c54,iVar9 + 0x14);
        if (in_FPSCR_SZ == '\0') {
          in_dr2 = (double)CONCAT44((int)((ulonglong)in_dr2 >> 0x20),*piVar11);
          piVar12 = (int *)((int)piVar11 + 4);
        }
        else {
          in_xd2 = *(undefined8 *)piVar11;
          piVar12 = (int *)((int)piVar11 + 8);
        }
        if (in_FPSCR_PR == '\0') {
          bVar18 = (float)((ulonglong)dVar13 >> 0x20) < SUB84(in_dr2,0);
        }
        else {
          bVar18 = dVar13 < in_dr2;
        }
        if ((bVar18) || (piVar12[3] != 0)) goto LAB_8c075b2e;
        iVar3 = *(int *)(param_5 + 0x14);
        piVar12[5] = *(int *)(param_5 + 0x10);
        piVar12[6] = iVar3;
        piVar12[7] = *(int *)(param_5 + 0x18);
        fVar14 = (float)(*(code *)puVar1)(piVar12 + 5);
        uVar5 = SUB84(in_dr14,0);
        uVar19 = (uint)((ulonglong)in_dr2 >> 0x20);
        if (fVar14 != 0.0) {
          (*(code *)PTR_FUN_8c075c5c)(piVar12 + 5);
        }
        in_dr14 = (double)(*(code *)PTR_FUN_8c075c58)(piVar12 + 5,iVar9 + 0x14);
        in_dr2 = (double)((ulonglong)uVar19 << 0x20);
        fVar14 = (float)((ulonglong)in_dr14 >> 0x20);
        if (in_FPSCR_PR == '\0') {
          bVar18 = fVar14 < 0.0;
        }
        else {
          bVar18 = in_dr14 < in_dr2;
        }
        if (in_FPSCR_SZ == '\0') {
          in_dr14 = (double)CONCAT44(fVar14,uVar5);
        }
        if (bVar18) {
          uVar5 = (*(code *)PTR_FUN_8c075c58)(param_5 + 0x10,iVar9 + 0x14);
          if (in_FPSCR_SZ == '\0') {
            dVar17 = (double)CONCAT44((int)((ulonglong)dVar17 >> 0x20),uVar5);
          }
          dVar13 = (double)(*(code *)PTR_FUN_8c075c60)(0x600);
          fVar14 = (float)((ulonglong)in_dr14 >> 0x20);
          if (in_FPSCR_PR == '\0') {
            bVar18 = fVar14 < -(float)((ulonglong)dVar13 >> 0x20);
          }
          else {
            bVar18 = in_dr14 < -dVar13;
          }
          if (bVar18) {
LAB_8c075b9e:
            *piVar12 = iVar8 * 4;
            piVar12[iVar8 + 8] = piVar12[iVar8 + 8] | 8;
            piVar12[2] = piVar12[2] + 1;
            if (in_FPSCR_SZ == '\0') {
              in_dr2 = (double)CONCAT44((int)((ulonglong)in_dr2 >> 0x20),
                                        *(undefined4 *)((int)pdVar10 + *piVar12 + -0xbc));
            }
            else {
              in_xd2 = *(undefined8 *)((int)pdVar10 + *piVar12 + -0xbc);
            }
            if (in_FPSCR_PR == '\0') {
              bVar18 = SUB84(in_dr2,0) < SUB84(in_dr14,0);
            }
            else {
              bVar18 = in_dr2 < in_dr14;
            }
            if (bVar18) {
              if (in_FPSCR_SZ == '\0') {
                in_dr14 = (double)CONCAT44(fVar14,*(undefined4 *)((int)pdVar10 + *piVar12 + -0xbc));
              }
              piVar12[4] = iVar8;
            }
          }
          else {
            if (in_FPSCR_SZ == '\0') {
              in_dr2 = (double)CONCAT44((int)((ulonglong)in_dr2 >> 0x20),DAT_8c075c64);
            }
            else {
              in_xd2 = CONCAT44(PTR_FUN_8c075c68,DAT_8c075c64);
            }
            if (in_FPSCR_PR == '\0') {
              bVar18 = SUB84(dVar17,0) < SUB84(in_dr2,0);
            }
            else {
              bVar18 = dVar17 < in_dr2;
            }
            if (bVar18) goto LAB_8c075b9e;
          }
          (*(code *)PTR_FUN_8c075c68)(piVar12 + 0x2b,iVar9 + 0x14);
        }
      }
      uVar5 = (undefined4)param_1;
      iVar8 = iVar8 + 1;
      piVar2 = piVar12;
    } while (iVar8 < (int)(uint)*(ushort *)(param_5 + 0x28));
  }
  if (0 < *(int *)((int)piVar12 + 8)) {
    dVar13 = (double)(*(code *)puVar1)(param_5 + 0x10);
    if (in_FPSCR_SZ == '\0') {
      dVar13 = (double)CONCAT44((int)((ulonglong)dVar13 >> 0x20),uVar5);
    }
    iVar8 = *(int *)((int)piVar12 + 0x10) * 4;
    if (in_FPSCR_SZ == '\0') {
      in_dr2 = (double)CONCAT44((int)((ulonglong)in_dr2 >> 0x20),
                                *(undefined4 *)((int)pdVar10 + iVar8 + -0xbc));
    }
    else {
      in_xd2 = *(undefined8 *)((int)pdVar10 + iVar8 + -0xbc);
    }
    if (in_FPSCR_PR == '\0') {
      bVar18 = SUB84(in_dr2,0) < (float)((ulonglong)dVar13 >> 0x20);
    }
    else {
      bVar18 = in_dr2 < dVar13;
    }
    if ((bVar18) && ((*(uint *)(param_4 + 0x60) & 0x4000) == 0)) {
      fVar14 = (float)(*(code *)puVar1)(param_5 + 0x6c);
      uVar5 = SUB84(dVar13,0);
      if (fVar14 == 0.0) {
        uVar5 = (undefined4)((ulonglong)dVar17 >> 0x20);
        if (in_FPSCR_SZ == '\0') {
          *(undefined4 *)(param_5 + 0x74) = uVar5;
          *(undefined4 *)(param_5 + 0x70) = uVar5;
          *(undefined4 *)(param_5 + 0x6c) = uVar5;
        }
        else {
          *(double *)(param_5 + 0x74) = dVar17;
          *(double *)(param_5 + 0x70) = dVar17;
          *(double *)(param_5 + 0x6c) = dVar17;
        }
      }
      else {
        (*(code *)PTR_FUN_8c075dc0)(param_5 + 0x6c);
        iVar9 = DAT_8c075dc4;
        puVar7 = (undefined8 *)((int)pdVar10 + iVar8 + -0xbc);
        if (in_FPSCR_SZ == '\0') {
          dVar13 = (double)CONCAT44(*(undefined4 *)(param_5 + 0x6c),*(undefined4 *)puVar7);
        }
        else {
          in_xd2 = *puVar7;
          dVar13 = *(double *)(param_5 + 0x6c);
        }
        if (in_FPSCR_PR == '\0') {
          dVar13 = (double)CONCAT44((float)((ulonglong)dVar13 >> 0x20) * SUB84(dVar13,0),
                                    SUB84(dVar13,0));
        }
        else {
          dVar13 = dVar13 * dVar13;
        }
        if (in_FPSCR_SZ == '\0') {
          *(int *)(param_5 + 0x6c) = (int)((ulonglong)dVar13 >> 0x20);
          dVar13 = (double)CONCAT44(*(undefined4 *)(param_5 + 0x70),*(undefined4 *)puVar7);
        }
        else {
          *(double *)(param_5 + 0x6c) = dVar13;
          dVar13 = *(double *)(param_5 + 0x70);
          in_xd2 = *puVar7;
        }
        if (in_FPSCR_PR == '\0') {
          dVar13 = (double)CONCAT44((float)((ulonglong)dVar13 >> 0x20) * SUB84(dVar13,0),
                                    SUB84(dVar13,0));
        }
        else {
          dVar13 = dVar13 * dVar13;
        }
        if (in_FPSCR_SZ == '\0') {
          *(int *)(param_5 + 0x70) = (int)((ulonglong)dVar13 >> 0x20);
          dVar13 = (double)CONCAT44(*(undefined4 *)(param_5 + 0x74),*(undefined4 *)puVar7);
        }
        else {
          *(double *)(param_5 + 0x70) = dVar13;
          in_xd2 = *puVar7;
          dVar13 = *(double *)(param_5 + 0x74);
        }
        if (in_FPSCR_PR == '\0') {
          dVar13 = (double)CONCAT44((float)((ulonglong)dVar13 >> 0x20) * SUB84(dVar13,0),
                                    SUB84(dVar13,0));
        }
        else {
          dVar13 = dVar13 * dVar13;
        }
        uVar4 = (undefined4)((ulonglong)dVar13 >> 0x20);
        if (in_FPSCR_SZ == '\0') {
          *(undefined4 *)(param_5 + 0x74) = uVar4;
        }
        else {
          *(double *)(param_5 + 0x74) = dVar13;
        }
        iVar9 = *(int *)((int)piVar12 + 0x10) * 0x58 + iVar9;
        uVar6 = *(undefined4 *)(iVar9 + 0x18);
        *(undefined4 *)(param_4 + 0x44) = *(undefined4 *)(iVar9 + 0x14);
        *(undefined4 *)(param_4 + 0x48) = uVar6;
        *(undefined4 *)(param_4 + 0x4c) = *(undefined4 *)(iVar9 + 0x1c);
        if (in_FPSCR_SZ == '\0') {
          dVar13 = (double)CONCAT44(uVar4,*(undefined4 *)(param_3 + 4));
        }
        else {
          in_xd2 = *(undefined8 *)(param_3 + 4);
        }
        uVar19 = (uint)((ulonglong)dVar13 >> 0x20);
        fVar14 = -SUB84(dVar13,0);
        if (in_FPSCR_SZ == '\0') {
          *(float *)((int)piVar12 + 0xb8) = fVar14;
          fVar14 = *(float *)(param_3 + 8);
        }
        else {
          *(undefined8 *)((int)piVar12 + 0xb8) = in_xd2;
          in_xd2 = *(undefined8 *)(param_3 + 8);
        }
        fVar14 = -fVar14;
        if (in_FPSCR_SZ == '\0') {
          *(float *)((int)piVar12 + 0xbc) = fVar14;
          fVar14 = *(float *)(param_3 + 0xc);
        }
        else {
          *(undefined8 *)((int)piVar12 + 0xbc) = in_xd2;
          in_xd2 = *(undefined8 *)(param_3 + 0xc);
        }
        if (in_FPSCR_SZ == '\0') {
          *(float *)((int)piVar12 + 0xc0) = -fVar14;
        }
        else {
          *(undefined8 *)((int)piVar12 + 0xc0) = in_xd2;
        }
        fVar14 = (float)(*(code *)puVar1)((undefined1 *)((int)piVar12 + 0xb8));
        if (fVar14 != 0.0) {
          (*(code *)PTR_FUN_8c075dc0)((undefined1 *)((int)piVar12 + 0xb8));
          iVar8 = (*(code *)PTR_FUN_8c075dc8)((undefined1 *)((int)piVar12 + 0xb8),param_4 + 0x44,0);
          if (0x1000 < iVar8) {
            (*(code *)PTR_FUN_8c075dd0)
                      (param_5 + 0x6c,param_4 + 0x44,(undefined1 *)((int)piVar12 + 0xc4));
            fVar14 = (float)(*(code *)puVar1)((undefined1 *)((int)piVar12 + 0xc4));
            if (fVar14 == 0.0) {
              uVar4 = *(undefined4 *)(param_5 + 0x20);
              *(undefined4 *)((int)piVar12 + 0xc4) = *(undefined4 *)(param_5 + 0x1c);
              *(undefined4 *)((int)piVar12 + 200) = uVar4;
              *(undefined4 *)((int)piVar12 + 0xcc) = *(undefined4 *)(param_5 + 0x24);
            }
            else {
              (*(code *)PTR_FUN_8c076040)((undefined1 *)((int)piVar12 + 0xc4));
            }
            if (in_FPSCR_SZ == '\0') {
              *(undefined4 *)((int)piVar12 + 0x100) = *(undefined4 *)(iVar9 + 0x20);
              *(undefined4 *)((int)piVar12 + 0x104) = *(undefined4 *)(iVar9 + 0x24);
              *(undefined4 *)((int)piVar12 + 0x108) = *(undefined4 *)(iVar9 + 0x28);
              *(undefined4 *)((int)piVar12 + 0x10c) = *(undefined4 *)(param_4 + 0x44);
              *(undefined4 *)((int)piVar12 + 0x110) = *(undefined4 *)(param_4 + 0x48);
              *(undefined4 *)((int)piVar12 + 0x114) = *(undefined4 *)(param_4 + 0x4c);
              *(undefined4 *)((int)piVar12 + 0xe8) = *(undefined4 *)(param_5 + 0x60);
              *(undefined4 *)((int)piVar12 + 0xec) = *(undefined4 *)(param_5 + 100);
              *(undefined4 *)((int)piVar12 + 0xf0) = *(undefined4 *)(param_5 + 0x68);
              *(undefined4 *)((int)piVar12 + 0xf4) = *(undefined4 *)((int)piVar12 + 0xc4);
              *(undefined4 *)((int)piVar12 + 0xf8) = *(undefined4 *)((int)piVar12 + 200);
              *(undefined4 *)((int)piVar12 + 0xfc) = *(undefined4 *)((int)piVar12 + 0xcc);
            }
            else {
              *(undefined8 *)((int)piVar12 + 0x100) = *(undefined8 *)(iVar9 + 0x20);
              *(undefined8 *)((int)piVar12 + 0x104) = *(undefined8 *)(iVar9 + 0x24);
              *(undefined8 *)((int)piVar12 + 0x108) = *(undefined8 *)(iVar9 + 0x28);
              *(undefined8 *)((int)piVar12 + 0x10c) = *(undefined8 *)(param_4 + 0x44);
              *(undefined8 *)((int)piVar12 + 0x110) = *(undefined8 *)(param_4 + 0x48);
              *(undefined8 *)((int)piVar12 + 0x114) = *(undefined8 *)(param_4 + 0x4c);
              *(undefined8 *)((int)piVar12 + 0xe8) = *(undefined8 *)(param_5 + 0x60);
              *(undefined8 *)((int)piVar12 + 0xec) = *(undefined8 *)(param_5 + 100);
              *(undefined8 *)((int)piVar12 + 0xf0) = *(undefined8 *)(param_5 + 0x68);
              *(undefined8 *)((int)piVar12 + 0xf4) = *(undefined8 *)((int)piVar12 + 0xc4);
              *(undefined8 *)((int)piVar12 + 0xf8) = *(undefined8 *)((int)piVar12 + 200);
              in_xd2 = *(undefined8 *)((int)piVar12 + 0xcc);
              *(undefined8 *)((int)piVar12 + 0xfc) = in_xd2;
            }
            (*(code *)PTR_FUN_8c076044)
                      ((undefined1 *)((int)piVar12 + 0x100),(undefined1 *)((int)piVar12 + 0xe8),
                       (undefined1 *)((int)piVar12 + 0xd0));
            uVar4 = *(undefined4 *)(param_5 + 0x70);
            *(undefined4 *)((int)piVar12 + 0x14) = *(undefined4 *)(param_5 + 0x6c);
            *(undefined4 *)((int)piVar12 + 0x18) = uVar4;
            *(undefined4 *)((int)piVar12 + 0x1c) = *(undefined4 *)(param_5 + 0x74);
            fVar14 = (float)(*(code *)puVar1)((undefined1 *)((int)piVar12 + 0x14));
            if (fVar14 != 0.0) {
              (*(code *)PTR_FUN_8c076040)((undefined1 *)((int)piVar12 + 0x14));
            }
            dVar17 = (double)(*(code *)PTR_FUN_8c076048)
                                       ((undefined1 *)((int)piVar12 + 0xdc),
                                        (undefined1 *)((int)piVar12 + 0x14));
            dVar13 = (double)((ulonglong)uVar19 << 0x20);
            if (in_FPSCR_PR == '\0') {
              bVar18 = (float)((ulonglong)dVar17 >> 0x20) < 0.0;
            }
            else {
              bVar18 = dVar17 < dVar13;
            }
            if (bVar18) {
              if (in_FPSCR_SZ == '\0') {
                dVar17 = (double)((ulonglong)*(uint *)((int)piVar12 + 0xdc) << 0x20);
              }
              else {
                dVar17 = *(double *)((int)piVar12 + 0xdc);
              }
              if (in_FPSCR_PR == '\0') {
                dVar17 = (double)CONCAT44(-(float)((ulonglong)dVar17 >> 0x20),SUB84(dVar17,0));
              }
              else {
                dVar17 = -dVar17;
              }
              if (in_FPSCR_SZ == '\0') {
                *(int *)((int)piVar12 + 0xdc) = (int)((ulonglong)dVar17 >> 0x20);
                dVar17 = (double)CONCAT44(*(undefined4 *)((int)piVar12 + 0xe0),SUB84(dVar17,0));
              }
              else {
                *(double *)((int)piVar12 + 0xdc) = dVar17;
                dVar17 = *(double *)((int)piVar12 + 0xe0);
              }
              if (in_FPSCR_PR == '\0') {
                dVar17 = (double)CONCAT44(-(float)((ulonglong)dVar17 >> 0x20),SUB84(dVar17,0));
              }
              else {
                dVar17 = -dVar17;
              }
              if (in_FPSCR_SZ == '\0') {
                *(int *)((int)piVar12 + 0xe0) = (int)((ulonglong)dVar17 >> 0x20);
                dVar13 = (double)CONCAT44(*(undefined4 *)((int)piVar12 + 0xe4),SUB84(dVar17,0));
              }
              else {
                *(double *)((int)piVar12 + 0xe0) = dVar17;
                dVar13 = *(double *)((int)piVar12 + 0xe4);
              }
              if (in_FPSCR_PR == '\0') {
                dVar13 = (double)CONCAT44(-(float)((ulonglong)dVar13 >> 0x20),SUB84(dVar13,0));
              }
              else {
                dVar13 = -dVar13;
              }
              if (in_FPSCR_SZ == '\0') {
                *(int *)((int)piVar12 + 0xe4) = (int)((ulonglong)dVar13 >> 0x20);
              }
              else {
                *(double *)((int)piVar12 + 0xe4) = dVar13;
              }
            }
            dVar17 = (double)(*(code *)puVar1)(param_3 + 4);
            if (in_FPSCR_SZ == '\0') {
              dVar17 = (double)CONCAT44((int)((ulonglong)dVar17 >> 0x20),uVar5);
              dVar13 = (double)CONCAT44((int)((ulonglong)dVar13 >> 0x20),
                                        *(undefined4 *)((int)piVar12 + 0xdc));
            }
            else {
              in_xd2 = *(undefined8 *)((int)piVar12 + 0xdc);
            }
            fVar14 = (float)((ulonglong)dVar17 >> 0x20);
            if (in_FPSCR_PR == '\0') {
              dVar13 = (double)CONCAT44((int)((ulonglong)dVar13 >> 0x20),SUB84(dVar13,0) * fVar14);
            }
            else {
              dVar13 = dVar13 * dVar17;
            }
            if (in_FPSCR_SZ == '\0') {
              *(int *)(param_3 + 4) = SUB84(dVar13,0);
              dVar13 = (double)CONCAT44((int)((ulonglong)dVar13 >> 0x20),
                                        *(undefined4 *)((int)piVar12 + 0xe0));
            }
            else {
              *(undefined8 *)(param_3 + 4) = in_xd2;
              in_xd2 = *(undefined8 *)((int)piVar12 + 0xe0);
            }
            if (in_FPSCR_PR == '\0') {
              dVar13 = (double)CONCAT44((int)((ulonglong)dVar13 >> 0x20),SUB84(dVar13,0) * fVar14);
            }
            else {
              dVar13 = dVar13 * dVar17;
            }
            if (in_FPSCR_SZ == '\0') {
              *(int *)(param_3 + 8) = SUB84(dVar13,0);
              dVar13 = (double)CONCAT44((int)((ulonglong)dVar13 >> 0x20),
                                        *(undefined4 *)((int)piVar12 + 0xe4));
            }
            else {
              *(undefined8 *)(param_3 + 8) = in_xd2;
              in_xd2 = *(undefined8 *)((int)piVar12 + 0xe4);
            }
            if (in_FPSCR_PR == '\0') {
              dVar13 = (double)CONCAT44((int)((ulonglong)dVar13 >> 0x20),SUB84(dVar13,0) * fVar14);
            }
            else {
              dVar13 = dVar13 * dVar17;
            }
            if (in_FPSCR_SZ == '\0') {
              *(int *)(param_3 + 0xc) = SUB84(dVar13,0);
            }
            else {
              *(undefined8 *)(param_3 + 0xc) = in_xd2;
            }
            uVar5 = *(undefined4 *)(param_3 + 8);
            *(undefined4 *)(param_5 + 0x6c) = *(undefined4 *)(param_3 + 4);
            *(undefined4 *)(param_5 + 0x70) = uVar5;
            *(undefined4 *)(param_5 + 0x74) = *(undefined4 *)(param_3 + 0xc);
            *(int *)((int)piVar12 + -4) = *(int *)((int)piVar12 + 4) + 0x18;
            if (in_FPSCR_SZ == '\0') {
              uVar16 = CONCAT44(*(undefined4 *)((int)piVar12 + 0xe4),
                                *(undefined4 *)((int)piVar12 + 0xdc));
            }
            else {
              uVar16 = *(undefined8 *)((int)piVar12 + 0xe4);
            }
            dVar17 = (double)(*(code *)PTR_FUN_8c07604c)(uVar16);
            if (in_FPSCR_SZ == '\0') {
              dVar13 = (double)CONCAT44((int)((ulonglong)dVar13 >> 0x20),DAT_8c076050);
            }
            if (in_FPSCR_PR == '\0') {
              uVar19 = (uint)((float)((ulonglong)dVar17 >> 0x20) * SUB84(dVar13,0));
            }
            else {
              uVar19 = (uint)(dVar17 * dVar13);
            }
            **(uint **)((int)piVar12 + -4) = uVar19 & DAT_8c076054;
            iVar8 = (*(code *)PTR_FUN_8c076058)
                              (*(undefined4 *)(param_3 + 0x20),
                               *(undefined4 *)(*(int *)((int)piVar12 + 4) + 0x18));
            if (0x6000 < iVar8) {
              iVar8 = *(int *)(*(int *)((int)piVar12 + 4) + 0x18) + DAT_8c07605c;
              *(int *)(*(int *)((int)piVar12 + 4) + 0x18) = iVar8;
            }
            *(undefined4 *)(param_3 + 0x20) = *(undefined4 *)(*(int *)((int)piVar12 + 4) + 0x18);
            return iVar8;
          }
          if (in_FPSCR_SZ == '\0') {
            uVar16 = CONCAT44(*(undefined4 *)(param_4 + 0x38),*(undefined4 *)(param_4 + 0xcc));
          }
          else {
            in_xd2 = *(undefined8 *)(param_4 + 0xcc);
            uVar16 = *(undefined8 *)(param_4 + 0x38);
          }
          if (in_FPSCR_PR != '\0' || (float)uVar16 <= (float)((ulonglong)uVar16 >> 0x20)) {
            uVar5 = (undefined4)((ulonglong)dVar17 >> 0x20);
            if (in_FPSCR_SZ == '\0') {
              *(undefined4 *)(param_5 + 0x74) = uVar5;
              *(undefined4 *)(param_5 + 0x70) = uVar5;
              *(undefined4 *)(param_5 + 0x6c) = uVar5;
            }
            else {
              *(double *)(param_5 + 0x74) = dVar17;
              *(double *)(param_5 + 0x70) = dVar17;
              *(double *)(param_5 + 0x6c) = dVar17;
            }
            return 2;
          }
          if (in_FPSCR_SZ == '\0') {
            dVar17 = (double)CONCAT44(*(undefined4 *)(param_4 + 0x38),
                                      *(undefined4 *)(param_4 + 0xc0));
          }
          else {
            in_xd2 = *(undefined8 *)(param_4 + 0xc0);
            dVar17 = *(double *)(param_4 + 0x38);
          }
          fVar14 = (float)((ulonglong)dVar17 >> 0x20);
          if (in_FPSCR_PR == '\0' && SUB84(dVar17,0) < fVar14) {
            if (in_FPSCR_SZ == '\0') {
              dVar17 = (double)CONCAT44(fVar14,DAT_8c075dcc);
              in_dr14 = (double)CONCAT44((int)((ulonglong)in_dr14 >> 0x20),
                                         *(undefined4 *)(param_4 + 0xc0));
            }
            else {
              in_xd2 = CONCAT44(PTR_FUN_8c075dd0,DAT_8c075dcc);
            }
            if (in_FPSCR_PR == '\0') {
              dVar13 = (double)CONCAT44((int)((ulonglong)in_dr14 >> 0x20),
                                        SUB84(in_dr14,0) * SUB84(dVar17,0));
            }
            else {
              dVar13 = in_dr14 * dVar17;
            }
            (*(code *)PTR_FUN_8c075dc0)(param_3 + 4);
            if (in_FPSCR_SZ == '\0') {
              dVar17 = (double)CONCAT44((int)((ulonglong)dVar17 >> 0x20),
                                        *(undefined4 *)(param_3 + 4));
            }
            else {
              in_xd2 = *(undefined8 *)(param_3 + 4);
            }
            fVar14 = SUB84(dVar13,0);
            if (in_FPSCR_PR == '\0') {
              fVar15 = SUB84(dVar17,0) * fVar14;
            }
            else {
              fVar15 = SUB84(dVar17 * dVar13,0);
            }
            if (in_FPSCR_SZ == '\0') {
              *(float *)(param_3 + 4) = fVar15;
              dVar17 = (double)CONCAT44(*(undefined4 *)(param_3 + 8),fVar15);
            }
            else {
              *(undefined8 *)(param_3 + 4) = in_xd2;
              dVar17 = *(double *)(param_3 + 8);
            }
            if (in_FPSCR_PR == '\0') {
              dVar17 = (double)CONCAT44((float)((ulonglong)dVar17 >> 0x20) * fVar14,SUB84(dVar17,0))
              ;
            }
            else {
              dVar17 = dVar17 * dVar13;
            }
            uVar5 = (undefined4)((ulonglong)dVar17 >> 0x20);
            if (in_FPSCR_SZ == '\0') {
              *(undefined4 *)(param_3 + 8) = uVar5;
              dVar17 = (double)CONCAT44(uVar5,*(undefined4 *)(param_3 + 0xc));
            }
            else {
              *(double *)(param_3 + 8) = dVar17;
              in_xd2 = *(undefined8 *)(param_3 + 0xc);
            }
            if (in_FPSCR_PR == '\0') {
              fVar14 = SUB84(dVar17,0) * fVar14;
            }
            else {
              fVar14 = SUB84(dVar17 * dVar13,0);
            }
            if (in_FPSCR_SZ == '\0') {
              *(float *)(param_3 + 0xc) = fVar14;
            }
            else {
              *(undefined8 *)(param_3 + 0xc) = in_xd2;
            }
          }
          return 1;
        }
      }
    }
  }
  return 0;
}

