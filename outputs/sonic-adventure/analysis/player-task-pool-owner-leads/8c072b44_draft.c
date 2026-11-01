/* Ghidra draft only; not buildable/verified source. */

/* WARNING: Removing unreachable block (ram,0x8c072c34) */
/* WARNING: Removing unreachable block (ram,0x8c072c10) */
/* WARNING: Removing unreachable block (ram,0x8c072f20) */
/* WARNING: Removing unreachable block (ram,0x8c072df4) */
/* WARNING: Removing unreachable block (ram,0x8c072e02) */
/* WARNING: Removing unreachable block (ram,0x8c072e08) */
/* WARNING: Removing unreachable block (ram,0x8c072e20) */
/* WARNING: Removing unreachable block (ram,0x8c072e80) */

int FUN_8c072b44(undefined4 param_1,int param_2,int param_3,int param_4)

{
  ushort uVar1;
  longlong lVar2;
  undefined4 uVar3;
  uint uVar4;
  undefined4 uVar5;
  int iVar6;
  uint *puVar7;
  int iVar8;
  undefined8 *puVar9;
  double in_dr0;
  float fVar11;
  double dVar10;
  undefined8 uVar12;
  double dVar13;
  float fVar16;
  longlong lVar14;
  double dVar15;
  double dVar17;
  undefined8 in_dr12;
  undefined8 in_dr14;
  double dVar18;
  double in_xd2;
  double dVar19;
  undefined8 in_xd14;
  bool bVar20;
  undefined4 in_PR;
  int iVar21;
  char in_FPSCR_PR;
  char in_FPSCR_SZ;
  undefined1 local_3c [16];
  undefined8 uStack_2c;
  undefined8 uStack_24;
  
  if (in_FPSCR_SZ == '\0') {
    uStack_24 = CONCAT44((int)in_dr14,(undefined4)uStack_24);
                    /* WARNING: Ignoring partial resolution of indirect */
    uStack_24._0_4_ = (int)((ulonglong)in_dr14 >> 0x20);
    puVar9 = &uStack_2c;
    uStack_2c = CONCAT44((int)in_dr12,(int)((ulonglong)in_dr12 >> 0x20));
  }
  else {
    puVar9 = (undefined8 *)local_3c;
    uStack_24 = in_xd14;
    uStack_2c = in_dr14;
  }
  *(undefined4 *)((int)puVar9 + -4) = in_PR;
  uVar3 = 0;
  if (in_FPSCR_SZ == '\0') {
    *(undefined4 *)(param_4 + 0x4c) = 0;
    *(undefined4 *)(param_4 + 0x48) = 0;
    *(undefined4 *)(param_4 + 0x44) = 0;
  }
  else {
    *(undefined8 *)(param_4 + 0x4c) = in_xd14;
    *(undefined8 *)(param_4 + 0x48) = in_xd14;
    *(undefined8 *)(param_4 + 0x44) = in_xd14;
  }
  iVar8 = 0;
  *(undefined4 *)(param_4 + 0x60) = *(undefined4 *)(param_4 + 0x5c);
  *(undefined4 *)(param_4 + 0x5c) = 0;
  iVar6 = *(int *)(param_2 + 0x38);
  if (((((*(ushort *)(iVar6 + 2) & 0x80) == 0) || ((*(ushort *)(param_2 + 4) & 4) != 0)) ||
      ((*(ushort *)(iVar6 + 2) & 8) == 0)) ||
     ((*(byte *)(*(short *)(iVar6 + 0x16) * 0x34 + *(int *)(*(int *)(iVar6 + 0x18) + 0xc) + 5) & 0xf
      ) == 0)) {
    if ((((*(ushort *)(*(int *)(param_2 + 0x38) + 2) & 2) != 0) &&
        (iVar6 = *(int *)(*(int *)(param_2 + 0x38) + 0x18), *(int *)(iVar6 + 0x10) != 0)) &&
       ((iVar6 = *(int *)(*(int *)(iVar6 + 0x10) + 0x20), iVar6 != 0 &&
        ((*(char *)(iVar6 + 2) == '\f' && ((*(ushort *)(param_2 + 4) & 1) != 0)))))) {
      if (in_FPSCR_SZ == '\0') {
        lVar14 = (ulonglong)*(uint *)(param_4 + 0x3c) << 0x20;
      }
      else {
        lVar14 = *(longlong *)(param_4 + 0x3c);
      }
      if (in_FPSCR_PR != '\0' || (float)((ulonglong)lVar14 >> 0x20) <= (float)lVar14) {
        if (in_FPSCR_SZ == '\0') {
          dVar15 = (double)CONCAT44(*(undefined4 *)(param_4 + 0xc4),(float)lVar14);
          in_dr0 = (double)CONCAT44((int)((ulonglong)in_dr0 >> 0x20),*(undefined4 *)(param_4 + 0x38)
                                   );
        }
        else {
          dVar15 = *(double *)(param_4 + 0xc4);
        }
        if (in_FPSCR_PR == '\0') {
          bVar20 = SUB84(in_dr0,0) < (float)((ulonglong)dVar15 >> 0x20);
        }
        else {
          bVar20 = in_dr0 < dVar15;
        }
        if (((!bVar20) || ((*(ushort *)(param_2 + 4) & 0x100) != 0)) &&
           (uVar3 = 0,
           iVar6 = (*(code *)PTR_FUN_8c073120)
                             (0x4000 - *(int *)(param_2 + 0x18),*(undefined4 *)(iVar6 + 0x18)),
           iVar6 < 0x3000)) {
          iVar8 = 3;
          FUN_8c07256a(param_3,param_4);
        }
      }
    }
  }
  else {
    iVar8 = *(int *)(param_2 + 0x38);
    *(undefined4 *)((int)puVar9 + -0x20) = *(undefined4 *)(iVar8 + 0xc);
    iVar8 = *(int *)(iVar8 + 0x18);
    uVar1 = *(ushort *)(iVar8 + 6);
    iVar8 = *(int *)(iVar8 + 0xc);
    uVar3 = *(undefined4 *)(param_3 + 8);
    *(undefined4 *)((int)puVar9 + -0x10) = *(undefined4 *)(param_3 + 4);
    *(undefined4 *)((int)puVar9 + -0xc) = uVar3;
    puVar7 = (uint *)((uint)uVar1 * 0x34 + iVar8);
    *(undefined4 *)((int)puVar9 + -8) = *(undefined4 *)(param_3 + 0xc);
    uVar3 = 0;
    (*(code *)PTR_FUN_8c072c74)(param_2,(undefined1 *)((int)puVar9 + -0x1c));
    uVar5 = *(undefined4 *)((int)puVar9 + -0x18);
    *(undefined4 *)(param_4 + 0x44) = *(undefined4 *)((int)puVar9 + -0x1c);
    *(undefined4 *)(param_4 + 0x48) = uVar5;
    *(undefined4 *)(param_4 + 0x4c) = *(undefined4 *)((int)puVar9 + -0x14);
    fVar11 = (float)(*(code *)PTR_FUN_8c072c78)((undefined1 *)((int)puVar9 + -0x1c));
    if (fVar11 == 0.0) {
      if (in_FPSCR_SZ == '\0') {
        *(undefined4 *)((int)puVar9 + -0x1c) = uVar3;
        *(undefined4 *)((int)puVar9 + -0x18) = uVar3;
        *(undefined4 *)((int)puVar9 + -0x14) = uVar3;
      }
      else {
        *(undefined8 *)((int)puVar9 + -0x1c) = in_xd14;
        *(undefined8 *)((int)puVar9 + -0x18) = in_xd14;
        *(undefined8 *)((int)puVar9 + -0x14) = in_xd14;
      }
    }
    else {
      (*(code *)PTR_FUN_8c072c7c)((undefined1 *)((int)puVar9 + -0x1c));
    }
    fVar11 = (float)(*(code *)PTR_FUN_8c072c78)((undefined1 *)((int)puVar9 + -0x10));
    if (fVar11 == 0.0) {
      if (in_FPSCR_SZ == '\0') {
        *(undefined4 *)((int)puVar9 + -0x10) = uVar3;
        *(undefined4 *)((int)puVar9 + -0xc) = uVar3;
        *(undefined4 *)((int)puVar9 + -8) = uVar3;
      }
      else {
        *(undefined8 *)((int)puVar9 + -0x10) = in_xd14;
        *(undefined8 *)((int)puVar9 + -0xc) = in_xd14;
        *(undefined8 *)((int)puVar9 + -8) = in_xd14;
      }
    }
    else {
      (*(code *)PTR_FUN_8c072e40)((undefined1 *)((int)puVar9 + -0x10));
    }
    dVar15 = (double)(*(code *)PTR_FUN_8c072e44)
                               ((undefined1 *)((int)puVar9 + -0x10),
                                (undefined1 *)((int)puVar9 + -0x1c));
    if (in_FPSCR_SZ == '\0') {
      uVar12 = CONCAT44(*(undefined4 *)(param_4 + 0x38),(undefined4)DAT_8c072e48);
    }
    else {
      in_xd2 = (double)CONCAT44(DAT_8c072e48._4_4_,(undefined4)DAT_8c072e48);
      uVar12 = *(undefined8 *)(param_4 + 0x38);
    }
    uVar5 = (undefined4)((ulonglong)dVar15 >> 0x20);
    dVar10 = dVar15;
    if (in_FPSCR_SZ == '\0') {
      dVar10 = (double)CONCAT44(uVar5,param_1);
    }
    if (in_FPSCR_PR != '\0' || (float)uVar12 <= (float)((ulonglong)uVar12 >> 0x20)) {
      iVar8 = 2;
      FUN_8c07256a(param_3,param_4);
    }
    else {
      if (in_FPSCR_SZ == '\0') {
        dVar13 = (double)CONCAT44(*(undefined4 *)(param_4 + 0xcc),(float)uVar12);
        dVar15 = (double)CONCAT44(uVar5,*(undefined4 *)(param_4 + 0x38));
      }
      else {
        dVar13 = *(double *)(param_4 + 0xcc);
      }
      if (in_FPSCR_PR == '\0') {
        bVar20 = SUB84(dVar15,0) < (float)((ulonglong)dVar13 >> 0x20);
      }
      else {
        bVar20 = dVar15 < dVar13;
      }
      fVar11 = (float)((ulonglong)dVar10 >> 0x20);
      if (!bVar20) {
        if (in_FPSCR_SZ == '\0') {
          dVar15 = (double)CONCAT44(DAT_8c072e48._4_4_,SUB84(dVar13,0));
        }
        else {
          dVar15 = (double)CONCAT44(PTR_FUN_8c072e50,DAT_8c072e48._4_4_);
        }
        if (in_FPSCR_PR == '\0') {
          bVar20 = fVar11 < (float)((ulonglong)dVar15 >> 0x20);
        }
        else {
          bVar20 = dVar10 < dVar15;
        }
        if (bVar20) {
          iVar8 = 2;
          if (in_FPSCR_SZ == '\0') {
            *(undefined4 *)(param_3 + 0xc) = uVar3;
            *(undefined4 *)(param_3 + 4) = uVar3;
          }
          else {
            *(undefined8 *)(param_3 + 0xc) = in_xd14;
            *(undefined8 *)(param_3 + 4) = in_xd14;
          }
          goto LAB_8c072fee;
        }
      }
      if (in_FPSCR_SZ == '\0') {
        dVar15 = (double)CONCAT44(*(undefined4 *)(param_4 + 0x38),*(undefined4 *)(param_4 + 0xc4));
      }
      else {
        in_xd2 = *(double *)(param_4 + 0xc4);
        dVar15 = *(double *)(param_4 + 0x38);
      }
      fVar16 = (float)((ulonglong)dVar15 >> 0x20);
      if (in_FPSCR_PR != '\0' || SUB84(dVar15,0) <= fVar16) {
        dVar15 = (double)((ulonglong)(uint)fVar16 << 0x20);
        if (in_FPSCR_PR == '\0') {
          bVar20 = fVar11 < 0.0;
        }
        else {
          bVar20 = dVar10 < dVar15;
        }
        if (bVar20) {
          iVar8 = 1;
          if (((*puVar7 & 0xf) == 0) || ((*puVar7 & 0xf) == 1)) {
            if (in_FPSCR_SZ == '\0') {
              dVar15 = (double)CONCAT44(puVar7[5],
                                        *(undefined4 *)(*(int *)((int)puVar9 + -0x20) + 0x14));
            }
            else {
              dVar15 = *(double *)(puVar7 + 5);
              in_xd2 = *(double *)(*(int *)((int)puVar9 + -0x20) + 0x14);
            }
            fVar11 = (float)((ulonglong)dVar15 >> 0x20);
            if ((in_FPSCR_PR == '\0' && SUB84(dVar15,0) < fVar11) &&
               (((byte)puVar7[1] & 3) <=
                (uint)((int)(*(byte *)(*(int *)((int)puVar9 + -0x20) + 4) & 0xc) >> 2))) {
              if (in_FPSCR_SZ == '\0') {
                dVar15 = (double)CONCAT44(fVar11,puVar7[5]);
                lVar14 = CONCAT44(*(undefined4 *)((int)puVar9 + -0x10),uVar3);
                dVar13 = (double)CONCAT44(*(undefined4 *)((int)puVar9 + -8),
                                          *(undefined4 *)(*(int *)((int)puVar9 + -0x20) + 0x14));
              }
              else {
                in_xd2 = *(double *)(puVar7 + 5);
                lVar14 = *(longlong *)((int)puVar9 + -0x10);
                dVar13 = *(double *)((int)puVar9 + -8);
              }
              if (in_FPSCR_PR == '\0') {
                dVar13 = (double)CONCAT44((int)((ulonglong)dVar13 >> 0x20),
                                          SUB84(dVar13,0) + SUB84(dVar15,0));
              }
              else {
                dVar13 = dVar13 + dVar15;
              }
              fVar16 = (float)((ulonglong)lVar14 >> 0x20);
              fVar11 = (float)((ulonglong)dVar13 >> 0x20);
              dVar19 = dVar13;
              lVar2 = lVar14;
              if (in_FPSCR_SZ == '\0') {
                dVar15 = (double)CONCAT44((int)((ulonglong)dVar15 >> 0x20),fVar11);
                dVar19 = in_xd2;
                lVar2 = (ulonglong)(uint)fVar16 << 0x20;
              }
              if (in_FPSCR_PR == '\0') {
                dVar15 = (double)CONCAT44((int)((ulonglong)dVar15 >> 0x20),
                                          (float)((ulonglong)lVar2 >> 0x20) * fVar16 +
                                          SUB84(dVar15,0) * fVar11);
              }
              else {
                dVar15 = dVar15 * dVar13;
              }
              dVar17 = dVar19;
              if (in_FPSCR_SZ == '\0') {
                dVar17 = (double)CONCAT44(SUB84(dVar15,0),SUB84(dVar10,0));
              }
              dVar10 = (double)(*(code *)PTR_FUN_8c072e50)(dVar17);
              uVar3 = (undefined4)((ulonglong)dVar10 >> 0x20);
              if (in_FPSCR_SZ == '\0') {
                dVar18 = (double)CONCAT44(DAT_8c072e54,(int)lVar14);
                dVar10 = (double)CONCAT44(uVar3,SUB84(dVar17,0));
                dVar13 = (double)CONCAT44(uVar3,SUB84(dVar13,0));
              }
              else {
                dVar18 = (double)CONCAT44(PTR_FUN_8c072e58,DAT_8c072e54);
                dVar13 = dVar10;
              }
              if (in_FPSCR_PR == '\0') {
                dVar10 = (double)CONCAT44((float)((ulonglong)dVar10 >> 0x20) / SUB84(dVar13,0),
                                          SUB84(dVar10,0));
              }
              else {
                dVar10 = dVar10 / dVar13;
              }
              in_xd2 = (double)(*(code *)PTR_FUN_8c072e58)(dVar10);
              uVar3 = SUB84(dVar10,0);
              if (in_FPSCR_SZ == '\0') {
                dVar15 = (double)CONCAT44((int)((ulonglong)dVar15 >> 0x20),
                                          (int)((ulonglong)in_xd2 >> 0x20));
                in_xd2 = dVar19;
              }
              if (in_FPSCR_PR == '\0') {
                dVar15 = (double)CONCAT44((int)((ulonglong)dVar15 >> 0x20),
                                          SUB84(dVar15,0) * (float)((ulonglong)dVar18 >> 0x20));
              }
              else {
                dVar15 = dVar15 * dVar18;
              }
              if (in_FPSCR_SZ == '\0') {
                uVar3 = *(undefined4 *)((int)puVar9 + -0x1c);
              }
              if (in_FPSCR_PR == '\0') {
                iVar6 = (int)SUB84(dVar15,0);
              }
              else {
                iVar6 = (int)dVar15;
              }
              if (in_FPSCR_SZ == '\0') {
                uVar12 = CONCAT44(*(undefined4 *)((int)puVar9 + -0x14),uVar3);
              }
              else {
                uVar12 = *(undefined8 *)((int)puVar9 + -0x14);
              }
              dVar15 = (double)(*(code *)PTR_FUN_8c072e5c)(uVar12);
              if (in_FPSCR_PR == '\0') {
                iVar21 = (int)((float)((ulonglong)dVar15 >> 0x20) *
                              (float)((ulonglong)dVar18 >> 0x20));
              }
              else {
                iVar21 = (int)(dVar15 * dVar18);
              }
              *(int *)((int)puVar9 + -0x20) = iVar21 + DAT_8c072e60;
              if (in_FPSCR_SZ == '\0') {
                uVar12 = CONCAT44(*(undefined4 *)((int)puVar9 + -8),
                                  *(undefined4 *)((int)puVar9 + -0x10));
              }
              else {
                uVar12 = *(undefined8 *)((int)puVar9 + -8);
              }
              dVar15 = (double)(*(code *)PTR_FUN_8c072e5c)(uVar12);
              if (in_FPSCR_PR == '\0') {
                iVar21 = (int)((float)((ulonglong)dVar15 >> 0x20) *
                              (float)((ulonglong)dVar18 >> 0x20));
              }
              else {
                iVar21 = (int)(dVar15 * dVar18);
              }
              iVar21 = (*(code *)PTR_FUN_8c072e64)(*(undefined4 *)((int)puVar9 + -0x20),iVar21);
              uVar3 = SUB84(dVar18,0);
              if (iVar21 < 0) {
                iVar6 = DAT_8c072e68 - iVar6;
              }
              (*(code *)PTR_FUN_8c072e6c)();
              iVar21 = -iVar6;
              *(int *)((int)puVar9 + -0x20) = iVar21;
              if (iVar21 != 0) {
                (*(code *)PTR_FUN_8c072e70)(0,iVar21);
              }
              uVar5 = *(undefined4 *)(param_3 + 8);
              *(undefined4 *)((int)puVar9 + -0x10) = *(undefined4 *)(param_3 + 4);
              *(undefined4 *)((int)puVar9 + -0xc) = uVar5;
              *(undefined4 *)((int)puVar9 + -8) = *(undefined4 *)(param_3 + 0xc);
              (*(code *)PTR_FUN_8c072e74)
                        (0,(undefined1 *)((int)puVar9 + -0x10),(undefined1 *)((int)puVar9 + -0x1c));
              (*(code *)PTR_FUN_8c072e78)(1);
              *(int *)(param_2 + 0x18) = *(int *)(param_2 + 0x18) + iVar6;
              if (in_FPSCR_SZ == '\0') {
                *(undefined4 *)(param_3 + 4) = *(undefined4 *)((int)puVar9 + -0x1c);
                *(undefined4 *)(param_3 + 0xc) = *(undefined4 *)((int)puVar9 + -0x14);
              }
              else {
                *(undefined8 *)(param_3 + 4) = *(undefined8 *)((int)puVar9 + -0x1c);
                in_xd2 = *(double *)((int)puVar9 + -0x14);
                *(double *)(param_3 + 0xc) = in_xd2;
              }
            }
          }
          goto LAB_8c072fee;
        }
      }
      if (in_FPSCR_PR == '\0') {
        bVar20 = fVar11 < 0.0;
      }
      else {
        bVar20 = dVar10 < (double)((ulonglong)dVar15 & 0xffffffff00000000);
      }
      iVar8 = 1;
      if (bVar20) {
        uVar4 = (*(code *)PTR_FUN_8c072f60)(param_2);
        if ((uVar4 & 0x400) == 0) {
          iVar6 = *(int *)(param_2 + 0x38);
          if (((((*(ushort *)(iVar6 + 2) & 0x80) == 0) || ((*(ushort *)(param_2 + 4) & 4) != 0)) ||
              ((*(uint *)(*(short *)(iVar6 + 0x16) * 0x34 + *(int *)(*(int *)(iVar6 + 0x18) + 0xc))
               & 0x800) != 0)) || ((*(ushort *)(iVar6 + 2) & 8) == 0)) {
            FUN_8c072946(param_2,param_3,param_4);
            goto LAB_8c072fee;
          }
          if (in_FPSCR_SZ == '\0') {
            dVar15 = (double)CONCAT44(*(undefined4 *)((int)puVar9 + -0x10),
                                      *(undefined4 *)((int)puVar9 + -0x1c));
          }
          else {
            in_xd2 = *(double *)((int)puVar9 + -0x1c);
            dVar15 = *(double *)((int)puVar9 + -0x10);
          }
          if (in_FPSCR_PR == '\0') {
            dVar15 = (double)CONCAT44((float)((ulonglong)dVar15 >> 0x20) + SUB84(dVar15,0),
                                      SUB84(dVar15,0));
          }
          else {
            dVar15 = dVar15 + dVar15;
          }
          if (in_FPSCR_SZ == '\0') {
            *(int *)((int)puVar9 + -0x10) = (int)((ulonglong)dVar15 >> 0x20);
            *(undefined4 *)((int)puVar9 + -0xc) = uVar3;
            dVar15 = (double)CONCAT44(*(undefined4 *)((int)puVar9 + -8),
                                      *(undefined4 *)((int)puVar9 + -0x14));
          }
          else {
            *(double *)((int)puVar9 + -0x10) = dVar15;
            *(undefined8 *)((int)puVar9 + -0xc) = in_xd14;
            in_xd2 = *(double *)((int)puVar9 + -0x14);
            dVar15 = *(double *)((int)puVar9 + -8);
          }
          if (in_FPSCR_PR == '\0') {
            dVar15 = (double)CONCAT44((float)((ulonglong)dVar15 >> 0x20) + SUB84(dVar15,0),
                                      SUB84(dVar15,0));
          }
          else {
            dVar15 = dVar15 + dVar15;
          }
          if (in_FPSCR_SZ == '\0') {
            *(int *)((int)puVar9 + -8) = (int)((ulonglong)dVar15 >> 0x20);
          }
          else {
            *(double *)((int)puVar9 + -8) = dVar15;
          }
          fVar11 = (float)(*(code *)PTR_FUN_8c072f64)((undefined1 *)((int)puVar9 + -0x10));
          if (fVar11 == 0.0) {
            if (in_FPSCR_SZ == '\0') {
              *(undefined4 *)(param_3 + 4) = uVar3;
              *(undefined4 *)(param_3 + 0xc) = uVar3;
            }
            else {
              *(undefined8 *)(param_3 + 4) = in_xd14;
              *(undefined8 *)(param_3 + 0xc) = in_xd14;
            }
            goto LAB_8c072fee;
          }
          (*(code *)PTR_FUN_8c072f68)((undefined1 *)((int)puVar9 + -0x10));
          if (in_FPSCR_SZ == '\0') {
            dVar15 = (double)CONCAT44(*(undefined4 *)(param_4 + 0x38),
                                      *(undefined4 *)((int)puVar9 + -0x10));
          }
          else {
            in_xd2 = *(double *)((int)puVar9 + -0x10);
            dVar15 = *(double *)(param_4 + 0x38);
          }
          if (in_FPSCR_PR == '\0') {
            dVar15 = (double)CONCAT44((float)((ulonglong)dVar15 >> 0x20) * SUB84(dVar15,0),
                                      SUB84(dVar15,0));
          }
          else {
            dVar15 = dVar15 * dVar15;
          }
          if (in_FPSCR_SZ == '\0') {
            *(int *)(param_3 + 4) = (int)((ulonglong)dVar15 >> 0x20);
            dVar15 = (double)CONCAT44(*(undefined4 *)(param_4 + 0x38),
                                      *(undefined4 *)((int)puVar9 + -8));
          }
          else {
            *(double *)(param_3 + 4) = dVar15;
            in_xd2 = *(double *)((int)puVar9 + -8);
            dVar15 = *(double *)(param_4 + 0x38);
          }
          if (in_FPSCR_PR == '\0') {
            dVar15 = (double)CONCAT44((float)((ulonglong)dVar15 >> 0x20) * SUB84(dVar15,0),
                                      SUB84(dVar15,0));
          }
          else {
            dVar15 = dVar15 * dVar15;
          }
        }
        else {
          if (in_FPSCR_SZ == '\0') {
            dVar15 = (double)CONCAT44(*(undefined4 *)(param_4 + 0x38),
                                      *(undefined4 *)((int)puVar9 + -0x1c));
          }
          else {
            in_xd2 = *(double *)((int)puVar9 + -0x1c);
            dVar15 = *(double *)(param_4 + 0x38);
          }
          if (in_FPSCR_PR == '\0') {
            dVar15 = (double)CONCAT44(-((float)((ulonglong)dVar15 >> 0x20) * SUB84(dVar15,0)),
                                      SUB84(dVar15,0));
          }
          else {
            dVar15 = -(dVar15 * dVar15);
          }
          if (in_FPSCR_SZ == '\0') {
            *(int *)(param_3 + 4) = (int)((ulonglong)dVar15 >> 0x20);
            dVar15 = (double)CONCAT44(*(undefined4 *)(param_4 + 0x38),
                                      *(undefined4 *)((int)puVar9 + -0x14));
          }
          else {
            *(double *)(param_3 + 4) = dVar15;
            in_xd2 = *(double *)((int)puVar9 + -0x14);
            dVar15 = *(double *)(param_4 + 0x38);
          }
          if (in_FPSCR_PR == '\0') {
            dVar15 = (double)CONCAT44(-((float)((ulonglong)dVar15 >> 0x20) * SUB84(dVar15,0)),
                                      SUB84(dVar15,0));
          }
          else {
            dVar15 = -(dVar15 * dVar15);
          }
        }
        if (in_FPSCR_SZ == '\0') {
          *(int *)(param_3 + 0xc) = (int)((ulonglong)dVar15 >> 0x20);
        }
        else {
          *(double *)(param_3 + 0xc) = dVar15;
        }
      }
    }
  }
LAB_8c072fee:
  if ((*(ushort *)(param_2 + 4) & 2) == 0) {
    iVar6 = FUN_8c0730d8(param_2,param_3,param_4);
    if (iVar8 == 0) {
      iVar8 = iVar6;
    }
  }
  else {
    iVar6 = FUN_8c0730d8(param_2,param_3,param_4);
    if (iVar8 == 0) {
      iVar8 = iVar6;
    }
    if ((*(ushort *)(param_2 + 4) & 1) == 0) {
      (*(code *)PTR_FUN_8c073124)();
      iVar6 = *(int *)(param_2 + 0x1c);
      *(int *)((int)puVar9 + -0x20) = iVar6;
      if (iVar6 != 0) {
        (*(code *)PTR_FUN_8c073128)(0,*(undefined4 *)((int)puVar9 + -0x20));
      }
      iVar6 = *(int *)(param_2 + 0x14);
      *(int *)((int)puVar9 + -0x20) = iVar6;
      if (iVar6 != 0) {
        (*(code *)PTR_FUN_8c07312c)(0,*(undefined4 *)((int)puVar9 + -0x20));
      }
      iVar6 = -*(int *)(param_2 + 0x18);
      *(int *)((int)puVar9 + -0x20) = iVar6;
      if (iVar6 != 0) {
        (*(code *)PTR_FUN_8c073130)(0,iVar6);
      }
      if (in_FPSCR_SZ == '\0') {
        *(undefined4 *)((int)puVar9 + -8) = uVar3;
        *(undefined4 *)((int)puVar9 + -0x10) = uVar3;
        *(undefined4 *)((int)puVar9 + -0xc) = 0x3f800000;
      }
      else {
        *(undefined8 *)((int)puVar9 + -8) = in_xd14;
        *(undefined8 *)((int)puVar9 + -0x10) = in_xd14;
        *(double *)((int)puVar9 + -0xc) = in_xd2;
      }
      (*(code *)PTR_FUN_8c073134)
                (0,(undefined1 *)((int)puVar9 + -0x10),(undefined1 *)((int)puVar9 + -0x1c));
      uVar5 = *(undefined4 *)((int)puVar9 + -0x18);
      *(undefined4 *)(param_4 + 0x50) = *(undefined4 *)((int)puVar9 + -0x1c);
      *(undefined4 *)(param_4 + 0x54) = uVar5;
      *(undefined4 *)(param_4 + 0x58) = *(undefined4 *)((int)puVar9 + -0x14);
      if (in_FPSCR_SZ == '\0') {
        *(undefined4 *)((int)puVar9 + -8) = uVar3;
        *(undefined4 *)((int)puVar9 + -0x10) = uVar3;
        *(undefined4 *)((int)puVar9 + -0xc) = DAT_8c073138;
      }
      else {
        *(undefined8 *)((int)puVar9 + -8) = in_xd14;
        *(undefined8 *)((int)puVar9 + -0x10) = in_xd14;
        *(ulonglong *)((int)puVar9 + -0xc) = CONCAT44(PTR_FUN_8c07313c,DAT_8c073138);
      }
      (*(code *)PTR_FUN_8c073134)
                (0,(undefined1 *)((int)puVar9 + -0x10),(undefined1 *)((int)puVar9 + -0x1c));
      (*(code *)PTR_FUN_8c07313c)(1);
      if (in_FPSCR_SZ == '\0') {
        dVar15 = (double)CONCAT44(*(undefined4 *)(param_2 + 0x24),
                                  *(undefined4 *)((int)puVar9 + -0x18));
      }
      else {
        dVar15 = *(double *)(param_2 + 0x24);
      }
      if (in_FPSCR_PR == '\0') {
        dVar15 = (double)CONCAT44((float)((ulonglong)dVar15 >> 0x20) + SUB84(dVar15,0),
                                  SUB84(dVar15,0));
      }
      else {
        dVar15 = dVar15 + dVar15;
      }
      if (in_FPSCR_SZ == '\0') {
        *(int *)(param_2 + 0x24) = (int)((ulonglong)dVar15 >> 0x20);
        *(undefined4 *)(param_3 + 8) = uVar3;
      }
      else {
        *(double *)(param_2 + 0x24) = dVar15;
        *(undefined8 *)(param_3 + 8) = in_xd14;
      }
    }
  }
  return iVar8;
}

