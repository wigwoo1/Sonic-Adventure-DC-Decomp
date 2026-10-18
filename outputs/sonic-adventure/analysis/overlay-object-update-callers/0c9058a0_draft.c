/* Ghidra draft only; not buildable/verified source. */

/* WARNING: Removing unreachable block (ram,0x0c905be8) */
/* WARNING: Removing unreachable block (ram,0x0c905f20) */
/* WARNING: Removing unreachable block (ram,0x0c905ef8) */
/* WARNING: Removing unreachable block (ram,0x0c905f0e) */
/* WARNING: Removing unreachable block (ram,0x0c905bd2) */
/* WARNING: Removing unreachable block (ram,0x0c905bfa) */
/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void callback_candidate_c9058a0(undefined4 param_1,int param_2,int param_3)

{
  code *pcVar1;
  code *pcVar2;
  int iVar3;
  undefined *puVar4;
  undefined4 uVar5;
  undefined4 *puVar6;
  undefined4 *puVar7;
  undefined8 *puVar8;
  double dVar9;
  double in_dr2;
  double dVar10;
  double dVar11;
  float fVar12;
  float fVar13;
  double dVar14;
  double dVar15;
  undefined8 uVar17;
  undefined4 uVar18;
  float fVar19;
  double in_dr12;
  undefined4 uVar20;
  uint uVar21;
  undefined8 in_dr14;
  float fVar23;
  double dVar22;
  undefined8 in_xd0;
  double in_xd2;
  double in_xd12;
  double dVar24;
  double in_xd14;
  bool bVar25;
  undefined4 in_PR;
  char in_FPSCR_PR;
  char in_FPSCR_SZ;
  undefined1 local_3c [16];
  undefined8 uStack_2c;
  undefined8 uStack_24;
  undefined4 uVar16;
  
  uVar5 = (undefined4)((ulonglong)in_dr12 >> 0x20);
  if (in_FPSCR_SZ == '\0') {
    uStack_24 = (double)CONCAT44((int)in_dr14,(undefined4)uStack_24);
                    /* WARNING: Ignoring partial resolution of indirect */
    uStack_24._0_4_ = (int)((ulonglong)in_dr14 >> 0x20);
    puVar8 = &uStack_2c;
    uStack_2c = CONCAT44(SUB84(in_dr12,0),uVar5);
  }
  else {
    puVar8 = (undefined8 *)local_3c;
    uStack_24 = in_xd14;
    uStack_2c = in_dr14;
  }
  *(undefined4 *)((int)puVar8 + -4) = in_PR;
  if (in_FPSCR_SZ == '\0') {
    uVar16 = *(undefined4 *)(param_3 + 0xd8);
    in_dr2 = (double)CONCAT44((int)((ulonglong)in_dr2 >> 0x20),*(undefined4 *)(param_2 + 0x2c));
    dVar14 = (double)CONCAT44(uVar16,(int)in_dr14);
    dVar24 = (double)CONCAT44(uVar16,param_1);
  }
  else {
    dVar14 = *(double *)(param_3 + 0xd8);
    in_xd2 = *(double *)(param_2 + 0x2c);
    uVar16 = (undefined4)((ulonglong)dVar14 >> 0x20);
    dVar24 = dVar14;
  }
  if (in_FPSCR_PR == '\0') {
    fVar23 = (float)((ulonglong)dVar14 >> 0x20) * SUB84(in_dr2,0);
  }
  else {
    fVar23 = (float)((ulonglong)(dVar14 * in_dr2) >> 0x20);
  }
  if (in_FPSCR_SZ == '\0') {
    in_dr2 = (double)CONCAT44((int)((ulonglong)in_dr2 >> 0x20),*(undefined4 *)(param_2 + 0x30));
  }
  else {
    in_xd2 = *(double *)(param_2 + 0x30);
  }
  if (in_FPSCR_SZ == '\0') {
    in_dr12 = (double)CONCAT44(uVar5,uVar16);
    dVar24 = in_xd12;
  }
  if (in_FPSCR_PR == '\0') {
    fVar19 = SUB84(in_dr12,0) * SUB84(in_dr2,0);
  }
  else {
    fVar19 = SUB84(in_dr12 * in_dr2,0);
  }
  puVar7 = (undefined4 *)(*(int *)(param_3 + 0x138) + *(short *)(param_3 + 0x11e) * 0x18);
  puVar6 = (undefined4 *)(*(int *)(param_3 + 0x138) + *(short *)(param_3 + 0x120) * 0x18);
  FUN_0c905618(param_2);
  if (in_FPSCR_SZ == '\0') {
    uVar17 = CONCAT44(*(undefined4 *)(param_2 + 0x28),(undefined4)DAT_0c905a80);
  }
  else {
    uVar17 = *(undefined8 *)(param_2 + 0x28);
  }
  iVar3 = (*DAT_0c905a80._4_4_)(uVar17);
  if (iVar3 == 0) {
    return;
  }
  (*DAT_0c905a88)(0x200);
  pcVar1 = DAT_0c905a8c;
  dVar14 = (double)CONCAT44(0x3f800000,fVar19);
  dVar22 = (double)((ulonglong)(uint)fVar23 << 0x20);
  if ((*(uint *)(param_3 + 0x60) & 0x10) == 0) goto LAB_0c905fd2;
  (*DAT_0c905a90)(*(undefined4 *)(param_3 + 0x7c4));
  dVar9 = in_xd14;
  if (in_FPSCR_SZ == '\0') {
    dVar9 = (double)CONCAT44(SUB84(dVar22,0),SUB84(dVar22,0));
  }
  (*pcVar1)(dVar9);
  uVar5 = SUB84(dVar9,0);
  (*DAT_0c905a94)(0);
  (*DAT_0c905a98)(0,param_2 + 0x20);
  iVar3 = *(int *)(param_2 + 0x1c);
  *(int *)((int)puVar8 + -0x94) = iVar3;
  if (iVar3 != 0) {
    (*DAT_0c905a9c)(0,*(undefined4 *)((int)puVar8 + -0x94));
  }
  iVar3 = *(int *)(param_2 + 0x14);
  *(int *)((int)puVar8 + -0x94) = iVar3;
  if (iVar3 != 0) {
    (*DAT_0c905aa0)(0,*(undefined4 *)((int)puVar8 + -0x94));
  }
  iVar3 = *(int *)(param_2 + 0x18);
  *(int *)((int)puVar8 + -0x94) = iVar3;
  if (iVar3 != 0) {
    (*DAT_0c905aa4)(0,*(undefined4 *)((int)puVar8 + -0x94));
  }
  dVar9 = in_xd14;
  if (in_FPSCR_SZ == '\0') {
    dVar9 = (double)CONCAT44(SUB84(dVar22,0),uVar5);
  }
  (*_DAT_0c905aac)(dVar9,0);
  uVar5 = SUB84(dVar9,0);
  iVar3 = *(int *)(param_3 + 0x80);
  *(int *)((int)puVar8 + -0x94) = iVar3;
  if (iVar3 != 0) {
    (*DAT_0c905aa0)(0,*(undefined4 *)((int)puVar8 + -0x94));
  }
  iVar3 = *(int *)(param_3 + 0x7c);
  *(int *)((int)puVar8 + -0x94) = iVar3;
  if (iVar3 != 0) {
    (*DAT_0c905a9c)(0,*(undefined4 *)((int)puVar8 + -0x94));
  }
  dVar9 = in_xd14;
  if (in_FPSCR_SZ == '\0') {
    dVar9 = (double)CONCAT44(SUB84(dVar22,0),uVar5);
  }
  (*_DAT_0c905aac)(dVar9,0);
  dVar15 = dVar22;
  dVar10 = dVar22;
  if (in_FPSCR_SZ == '\0') {
    dVar15 = (double)((ulonglong)dVar14 & 0xffffffff);
    dVar10 = (double)CONCAT44((int)((ulonglong)dVar22 >> 0x20),SUB84(dVar9,0));
  }
  uVar5 = SUB84(dVar15,0);
  (*_DAT_0c905ab4)(dVar10,0);
  uVar16 = SUB84(dVar22,0);
  dVar9 = (double)(*DAT_0c905aa4)(0,0x4000);
  pcVar2 = DAT_0c905ab8;
  if (in_FPSCR_SZ == '\0') {
    dVar14 = (double)CONCAT44((int)((ulonglong)dVar14 >> 0x20),(undefined4)DAT_0c905abc);
    dVar22 = (double)CONCAT44(DAT_0c905abc._4_4_,uVar16);
  }
  else {
    dVar24 = (double)CONCAT44(DAT_0c905abc._4_4_,(undefined4)DAT_0c905abc);
    dVar22 = (double)CONCAT44(_DAT_0c905ac4,DAT_0c905abc._4_4_);
  }
  *(undefined4 *)((int)puVar8 + -0x94) = *_DAT_0c905ac4;
  if ((*(ushort *)(param_3 + 0x11a) & 1) == 0) {
    if ((*(uint *)(param_3 + 0x60) & 0x100) == 0) {
      dVar9 = (double)(*DAT_0c905e78)(param_2 + 0x20,*(int *)((int)puVar8 + -0x94) + 0x20);
      dVar15 = dVar9;
      if (in_FPSCR_SZ == '\0') {
        dVar15 = (double)CONCAT44((int)((ulonglong)dVar9 >> 0x20),uVar5);
      }
    }
    else {
      dVar15 = in_xd14;
      if (in_FPSCR_SZ == '\0') {
        dVar15 = (double)CONCAT44(SUB84(dVar22,0),uVar5);
      }
    }
    uVar5 = SUB84(dVar15,0);
    if ((*(ushort *)(*(int *)(param_3 + 0xec) + 0x36) & 0x40) == 0) {
      if ((*(ushort *)(*(int *)(param_3 + 0xec) + 0x36) & 0xc) == 0) {
        if (in_FPSCR_SZ == '\0') {
          in_dr2 = (double)CONCAT44((int)((ulonglong)in_dr2 >> 0x20),_DAT_0c9060a0);
        }
        else {
          in_xd2 = (double)CONCAT44(PTR_DAT_0c9060a4,_DAT_0c9060a0);
        }
        fVar19 = (float)((ulonglong)dVar15 >> 0x20);
        fVar23 = SUB84(in_dr2,0);
        if (in_FPSCR_PR == '\0') {
          bVar25 = fVar23 < fVar19;
        }
        else {
          bVar25 = in_dr2 < dVar15;
        }
        uVar16 = (undefined4)((ulonglong)dVar9 >> 0x20);
        fVar12 = SUB84(dVar14,0);
        uVar20 = (undefined4)((ulonglong)dVar14 >> 0x20);
        uVar18 = (undefined4)((ulonglong)dVar10 >> 0x20);
        fVar13 = (float)((ulonglong)dVar22 >> 0x20);
        if (bVar25) {
          if (in_FPSCR_SZ == '\0') {
            dVar9 = (double)CONCAT44(uVar16,fVar12);
          }
          if (in_FPSCR_PR == '\0') {
            dVar11 = (double)CONCAT44((float)*(byte *)(param_3 + 0x8c6) * fVar12,fVar23);
          }
          else {
            dVar11 = (double)*(byte *)(param_3 + 0x8c6) * dVar14;
          }
          if (in_FPSCR_SZ == '\0') {
            *(int *)((int)puVar8 + -0x90) = (int)((ulonglong)dVar11 >> 0x20);
          }
          else {
            *(double *)((int)puVar8 + -0x90) = dVar11;
          }
          if (in_FPSCR_PR == '\0') {
            dVar11 = (double)CONCAT44((float)*(byte *)(param_3 + 0x8c5) * fVar12,SUB84(dVar11,0));
          }
          else {
            dVar11 = (double)*(byte *)(param_3 + 0x8c5) * dVar14;
          }
          if (in_FPSCR_SZ == '\0') {
            *(int *)((int)puVar8 + -0x8c) = (int)((ulonglong)dVar11 >> 0x20);
          }
          else {
            *(double *)((int)puVar8 + -0x8c) = dVar11;
          }
          if (in_FPSCR_PR == '\0') {
            in_dr2 = (double)CONCAT44((float)*(byte *)(param_3 + 0x8c4),SUB84(dVar11,0));
          }
          else {
            in_dr2 = (double)*(byte *)(param_3 + 0x8c4);
          }
          in_xd2 = in_dr2;
          if (in_FPSCR_SZ == '\0') {
            dVar14 = (double)CONCAT44(uVar20,(int)((ulonglong)in_dr2 >> 0x20));
            in_xd2 = dVar24;
          }
          if (in_FPSCR_PR == '\0') {
            dVar14 = (double)CONCAT44((int)((ulonglong)dVar14 >> 0x20),
                                      SUB84(dVar14,0) * SUB84(dVar9,0));
          }
          else {
            dVar14 = dVar14 * dVar9;
          }
          if (in_FPSCR_SZ == '\0') {
            dVar10 = (double)CONCAT44(uVar18,SUB84(dVar14,0));
          }
          if (in_FPSCR_PR == '\0') {
            fVar23 = SUB84(dVar10,0) + fVar13;
          }
          else {
            fVar23 = SUB84(dVar10 + dVar22,0);
          }
          if (in_FPSCR_SZ == '\0') {
            dVar24 = (double)CONCAT44(*(undefined4 *)((int)puVar8 + -0x8c),fVar23);
            dVar15 = (double)CONCAT44(fVar19,*(undefined4 *)((int)puVar8 + -0x90));
          }
          else {
            dVar24 = *(double *)((int)puVar8 + -0x8c);
          }
          if (in_FPSCR_PR == '\0') {
            dVar24 = (double)CONCAT44((float)((ulonglong)dVar24 >> 0x20) + fVar13,SUB84(dVar24,0));
            fVar13 = SUB84(dVar15,0) + fVar13;
          }
          else {
            dVar24 = dVar24 + dVar22;
            fVar13 = SUB84(dVar15 + dVar22,0);
          }
          dVar9 = in_xd14;
          if (in_FPSCR_SZ == '\0') {
            dVar9 = (double)CONCAT44(SUB84(dVar22,0),fVar13);
          }
          (*pcVar1)(dVar9,dVar24);
          if (in_FPSCR_SZ == '\0') {
            uVar17 = CONCAT44(*(undefined4 *)(param_3 + 0x124),SUB84(dVar9,0));
          }
          else {
            uVar17 = *(undefined8 *)(param_3 + 0x124);
          }
          uVar5 = *puVar7;
          puVar4 = PTR_DAT_0c9060a4;
        }
        else {
          if (in_FPSCR_SZ == '\0') {
            in_dr2 = (double)CONCAT44(_DAT_0c9060a8,fVar23);
          }
          else {
            in_dr2 = (double)CONCAT44(PTR_DAT_0c9060ac,_DAT_0c9060a8);
          }
          fVar23 = (float)((ulonglong)in_dr2 >> 0x20);
          if (in_FPSCR_PR == '\0') {
            bVar25 = fVar23 < fVar19;
          }
          else {
            bVar25 = in_dr2 < dVar15;
          }
          if (!bVar25) {
            if (in_FPSCR_SZ == '\0') {
              uVar17 = CONCAT44(*(undefined4 *)(param_3 + 0x124),uVar5);
            }
            else {
              uVar17 = *(undefined8 *)(param_3 + 0x124);
            }
            (*pcVar2)(uVar17,*(undefined4 *)(param_3 + 0x248),*puVar7);
            uVar5 = (undefined4)uVar17;
            goto LAB_0c905f64;
          }
          if (in_FPSCR_PR == '\0') {
            dVar9 = (double)CONCAT44(uVar16,(float)*(byte *)(param_3 + 0x8c6) * fVar12);
          }
          else {
            dVar9 = (double)CONCAT44(uVar16,(float)*(byte *)(param_3 + 0x8c6)) * dVar14;
          }
          if (in_FPSCR_SZ == '\0') {
            *(int *)((int)puVar8 + -0x88) = SUB84(dVar9,0);
            dVar9 = (double)CONCAT44((int)((ulonglong)dVar9 >> 0x20),fVar12);
          }
          else {
            *(undefined8 *)((int)puVar8 + -0x88) = in_xd0;
          }
          if (in_FPSCR_PR == '\0') {
            dVar11 = (double)CONCAT44(fVar23,(float)*(byte *)(param_3 + 0x8c5) * fVar12);
          }
          else {
            dVar11 = (double)CONCAT44(fVar23,(float)*(byte *)(param_3 + 0x8c5)) * dVar14;
          }
          if (in_FPSCR_SZ == '\0') {
            *(int *)((int)puVar8 + -0x74) = SUB84(dVar11,0);
          }
          else {
            *(double *)((int)puVar8 + -0x74) = in_xd2;
          }
          in_dr2 = (double)CONCAT44((int)((ulonglong)dVar11 >> 0x20),
                                    (float)*(byte *)(param_3 + 0x8c4));
          if (in_FPSCR_SZ == '\0') {
            dVar14 = (double)CONCAT44(uVar20,(float)*(byte *)(param_3 + 0x8c4));
            in_xd2 = dVar24;
          }
          if (in_FPSCR_PR == '\0') {
            dVar14 = (double)CONCAT44((int)((ulonglong)dVar14 >> 0x20),
                                      SUB84(dVar14,0) * SUB84(dVar9,0));
          }
          else {
            dVar14 = dVar14 * dVar9;
          }
          if (in_FPSCR_SZ == '\0') {
            dVar10 = (double)CONCAT44(uVar18,SUB84(dVar14,0));
          }
          if (in_FPSCR_PR == '\0') {
            fVar23 = SUB84(dVar10,0) + fVar13;
          }
          else {
            fVar23 = SUB84(dVar10 + dVar22,0);
          }
          if (in_FPSCR_SZ == '\0') {
            dVar24 = (double)CONCAT44(*(undefined4 *)((int)puVar8 + -0x74),fVar23);
            dVar15 = (double)CONCAT44(fVar19,*(undefined4 *)((int)puVar8 + -0x88));
          }
          else {
            dVar24 = *(double *)((int)puVar8 + -0x74);
          }
          if (in_FPSCR_PR == '\0') {
            dVar24 = (double)CONCAT44((float)((ulonglong)dVar24 >> 0x20) + fVar13,SUB84(dVar24,0));
            fVar13 = SUB84(dVar15,0) + fVar13;
          }
          else {
            dVar24 = dVar24 + dVar22;
            fVar13 = SUB84(dVar15 + dVar22,0);
          }
          dVar9 = in_xd14;
          if (in_FPSCR_SZ == '\0') {
            dVar9 = (double)CONCAT44(SUB84(dVar22,0),fVar13);
          }
          (*pcVar1)(dVar9,dVar24);
          uVar5 = *puVar7;
          puVar4 = PTR_DAT_0c9060ac;
          if (in_FPSCR_SZ == '\0') {
            uVar17 = CONCAT44(*(undefined4 *)(param_3 + 0x124),SUB84(dVar9,0));
          }
          else {
            uVar17 = *(undefined8 *)(param_3 + 0x124);
          }
        }
        (*pcVar2)(uVar17,puVar4,uVar5);
        dVar24 = in_xd14;
        if (in_FPSCR_SZ == '\0') {
          dVar24 = (double)CONCAT44(SUB84(dVar22,0),SUB84(dVar22,0));
        }
        (*pcVar1)(dVar24,dVar24);
        uVar5 = SUB84(dVar24,0);
        dVar24 = in_xd2;
      }
      else if ((*(ushort *)(*(int *)(param_3 + 0xec) + 0x36) & 2) == 0) {
        if (in_FPSCR_SZ == '\0') {
          uVar17 = CONCAT44(*(undefined4 *)(param_3 + 0x124),uVar5);
        }
        else {
          uVar17 = *(undefined8 *)(param_3 + 0x124);
        }
        (*DAT_0c905e7c)(uVar17,*(undefined4 *)(param_3 + 0x248),*puVar7);
        uVar5 = (undefined4)uVar17;
      }
      else {
        if (in_FPSCR_SZ == '\0') {
          uVar17 = CONCAT44(*(undefined4 *)(param_3 + 0x124),uVar5);
        }
        else {
          uVar17 = *(undefined8 *)(param_3 + 0x124);
        }
        (*pcVar2)(uVar17,*(undefined4 *)(param_3 + 0x248),*puVar7);
        uVar5 = (undefined4)uVar17;
      }
    }
    else {
      if (in_FPSCR_SZ == '\0') {
        uVar17 = CONCAT44(*(undefined4 *)(param_3 + 0x124),uVar5);
      }
      else {
        uVar17 = *(undefined8 *)(param_3 + 0x124);
      }
      (*DAT_0c905e7c)(uVar17,*(undefined4 *)(param_3 + 0x248),*puVar7);
      uVar5 = (undefined4)uVar17;
    }
LAB_0c905f64:
    if ((*(uint *)(param_3 + 0x60) & 4) == 0) {
      uVar16 = *(undefined4 *)(PTR_DAT_0c9060b0 + 4);
      *(undefined4 *)((int)puVar8 + -0x10) = *(undefined4 *)PTR_DAT_0c9060b0;
      *(undefined4 *)((int)puVar8 + -0xc) = uVar16;
      *(undefined4 *)((int)puVar8 + -8) = *(undefined4 *)(PTR_DAT_0c9060b0 + 8);
      *(undefined4 *)((int)puVar8 + -0x18) = *(undefined4 *)(param_3 + 0x248);
      *(undefined4 *)((int)puVar8 + -0x14) = *puVar7;
      if (in_FPSCR_SZ == '\0') {
        uVar17 = CONCAT44(*(undefined4 *)(param_3 + 0x124),uVar5);
      }
      else {
        uVar17 = *(undefined8 *)(param_3 + 0x124);
      }
      (*DAT_0c9060b4)(uVar17,0,(undefined1 *)((int)puVar8 + -0x18),0x20,
                      (undefined1 *)((int)puVar8 + -0x58));
      (*DAT_0c9060b8)((undefined1 *)((int)puVar8 + -0x58),(undefined1 *)((int)puVar8 + -0x10),
                      param_3 + 0x81c);
      (*(code *)PTR_callback_candidate_c909d80_0c9060bc)(param_2,param_3);
    }
  }
  else {
    if ((*(uint *)(param_3 + 0x60) & 0x100) == 0) {
      dVar9 = (double)(*DAT_0c905ac8)(param_2 + 0x20,*(int *)((int)puVar8 + -0x94) + 0x20);
      dVar15 = dVar9;
      if (in_FPSCR_SZ == '\0') {
        dVar15 = (double)CONCAT44((int)((ulonglong)dVar9 >> 0x20),uVar5);
      }
    }
    else {
      dVar15 = in_xd14;
      if (in_FPSCR_SZ == '\0') {
        dVar15 = (double)CONCAT44(SUB84(dVar22,0),uVar5);
      }
    }
    uVar5 = SUB84(dVar15,0);
    if ((*(ushort *)(*(int *)(param_3 + 0xec) + 0x36) & 0x40) == 0) {
      if ((*(ushort *)(*(int *)(param_3 + 0xec) + 0x36) & 0xc) == 0) {
        if (in_FPSCR_SZ == '\0') {
          in_dr2 = (double)CONCAT44((int)((ulonglong)in_dr2 >> 0x20),_DAT_0c905c48);
        }
        else {
          in_xd2 = (double)CONCAT44(PTR_DAT_0c905c4c,_DAT_0c905c48);
        }
        fVar19 = (float)((ulonglong)dVar15 >> 0x20);
        fVar23 = SUB84(in_dr2,0);
        if (in_FPSCR_PR == '\0') {
          bVar25 = fVar23 < fVar19;
        }
        else {
          bVar25 = in_dr2 < dVar15;
        }
        uVar16 = (undefined4)((ulonglong)dVar9 >> 0x20);
        fVar13 = SUB84(dVar14,0);
        uVar20 = (undefined4)((ulonglong)dVar14 >> 0x20);
        uVar18 = (undefined4)((ulonglong)dVar10 >> 0x20);
        fVar12 = (float)((ulonglong)dVar22 >> 0x20);
        if (bVar25) {
          if (in_FPSCR_SZ == '\0') {
            dVar9 = (double)CONCAT44(uVar16,fVar13);
          }
          if (in_FPSCR_PR == '\0') {
            dVar11 = (double)CONCAT44((float)*(byte *)(param_3 + 0x8c6) * fVar13,fVar23);
          }
          else {
            dVar11 = (double)*(byte *)(param_3 + 0x8c6) * dVar14;
          }
          if (in_FPSCR_SZ == '\0') {
            *(int *)((int)puVar8 + -0x80) = (int)((ulonglong)dVar11 >> 0x20);
          }
          else {
            *(double *)((int)puVar8 + -0x80) = dVar11;
          }
          if (in_FPSCR_PR == '\0') {
            dVar11 = (double)CONCAT44((float)*(byte *)(param_3 + 0x8c5) * fVar13,SUB84(dVar11,0));
          }
          else {
            dVar11 = (double)*(byte *)(param_3 + 0x8c5) * dVar14;
          }
          if (in_FPSCR_SZ == '\0') {
            *(int *)((int)puVar8 + -0x84) = (int)((ulonglong)dVar11 >> 0x20);
          }
          else {
            *(double *)((int)puVar8 + -0x84) = dVar11;
          }
          if (in_FPSCR_PR == '\0') {
            dVar11 = (double)CONCAT44((float)*(byte *)(param_3 + 0x8c4),SUB84(dVar11,0));
          }
          else {
            dVar11 = (double)*(byte *)(param_3 + 0x8c4);
          }
          if (in_FPSCR_SZ == '\0') {
            dVar14 = (double)CONCAT44(uVar20,(int)((ulonglong)dVar11 >> 0x20));
            dVar11 = dVar24;
          }
          if (in_FPSCR_PR == '\0') {
            dVar14 = (double)CONCAT44((int)((ulonglong)dVar14 >> 0x20),
                                      SUB84(dVar14,0) * SUB84(dVar9,0));
          }
          else {
            dVar14 = dVar14 * dVar9;
          }
          if (in_FPSCR_SZ == '\0') {
            dVar10 = (double)CONCAT44(uVar18,SUB84(dVar14,0));
          }
          if (in_FPSCR_PR == '\0') {
            fVar23 = SUB84(dVar10,0) + fVar12;
          }
          else {
            fVar23 = SUB84(dVar10 + dVar22,0);
          }
          if (in_FPSCR_SZ == '\0') {
            dVar24 = (double)CONCAT44(*(undefined4 *)((int)puVar8 + -0x84),fVar23);
            dVar15 = (double)CONCAT44(fVar19,*(undefined4 *)((int)puVar8 + -0x80));
          }
          else {
            dVar24 = *(double *)((int)puVar8 + -0x84);
          }
          if (in_FPSCR_PR == '\0') {
            dVar24 = (double)CONCAT44((float)((ulonglong)dVar24 >> 0x20) + fVar12,SUB84(dVar24,0));
            fVar12 = SUB84(dVar15,0) + fVar12;
          }
          else {
            dVar24 = dVar24 + dVar22;
            fVar12 = SUB84(dVar15 + dVar22,0);
          }
          dVar9 = in_xd14;
          if (in_FPSCR_SZ == '\0') {
            dVar9 = (double)CONCAT44(SUB84(dVar22,0),fVar12);
          }
          (*pcVar1)(dVar9,dVar24);
          if (in_FPSCR_SZ == '\0') {
            uVar17 = CONCAT44(*(undefined4 *)(param_3 + 0x124),SUB84(dVar9,0));
          }
          else {
            uVar17 = *(undefined8 *)(param_3 + 0x124);
          }
          uVar5 = *puVar7;
          puVar4 = PTR_DAT_0c905c4c;
        }
        else {
          if (in_FPSCR_SZ == '\0') {
            dVar9 = (double)CONCAT44(_DAT_0c905c50,fVar23);
          }
          else {
            dVar9 = (double)CONCAT44(PTR_DAT_0c905c54,_DAT_0c905c50);
          }
          fVar23 = (float)((ulonglong)dVar9 >> 0x20);
          if (in_FPSCR_PR == '\0') {
            bVar25 = fVar23 < fVar19;
          }
          else {
            bVar25 = dVar9 < dVar15;
          }
          if (!bVar25) {
            *(undefined4 *)((int)puVar8 + -0x14) = *puVar7;
            *(undefined4 *)((int)puVar8 + -0x10) = *puVar6;
            if (in_FPSCR_SZ == '\0') {
              *(undefined4 *)((int)puVar8 + -0xc) = *(undefined4 *)(param_3 + 0x124);
              *(undefined4 *)((int)puVar8 + -8) = *(undefined4 *)(param_3 + 300);
            }
            else {
              *(undefined8 *)((int)puVar8 + -0xc) = *(undefined8 *)(param_3 + 0x124);
              *(undefined8 *)((int)puVar8 + -8) = *(undefined8 *)(param_3 + 300);
            }
            *(undefined1 **)((int)puVar8 + -0x18) = (undefined1 *)((int)puVar8 + -0x14);
            *(undefined4 *)((int)puVar8 + -0x1c) = *(undefined4 *)(param_3 + 0x248);
            if (in_FPSCR_SZ == '\0') {
              uVar17 = CONCAT44(*(undefined4 *)(param_3 + 0x130),uVar5);
            }
            else {
              uVar17 = *(undefined8 *)(param_3 + 0x130);
            }
            (*DAT_0c905e60)(uVar17,(undefined1 *)((int)puVar8 + -0x1c));
            uVar5 = (undefined4)uVar17;
            goto LAB_0c905ca8;
          }
          if (in_FPSCR_PR == '\0') {
            dVar9 = (double)CONCAT44(uVar16,(float)*(byte *)(param_3 + 0x8c6) * fVar13);
          }
          else {
            dVar9 = (double)CONCAT44(uVar16,(float)*(byte *)(param_3 + 0x8c6)) * dVar14;
          }
          if (in_FPSCR_SZ == '\0') {
            *(int *)((int)puVar8 + -0x78) = SUB84(dVar9,0);
            dVar9 = (double)CONCAT44((int)((ulonglong)dVar9 >> 0x20),fVar13);
          }
          else {
            *(undefined8 *)((int)puVar8 + -0x78) = in_xd0;
          }
          if (in_FPSCR_PR == '\0') {
            fVar13 = (float)*(byte *)(param_3 + 0x8c5) * fVar13;
          }
          else {
            fVar13 = SUB84((double)CONCAT44(fVar23,(float)*(byte *)(param_3 + 0x8c5)) * dVar14,0);
          }
          if (in_FPSCR_SZ == '\0') {
            *(float *)((int)puVar8 + -0x7c) = fVar13;
          }
          else {
            *(double *)((int)puVar8 + -0x7c) = in_xd2;
          }
          dVar11 = in_xd2;
          if (in_FPSCR_SZ == '\0') {
            dVar14 = (double)CONCAT44(uVar20,(float)*(byte *)(param_3 + 0x8c4));
            dVar11 = dVar24;
          }
          if (in_FPSCR_PR == '\0') {
            dVar14 = (double)CONCAT44((int)((ulonglong)dVar14 >> 0x20),
                                      SUB84(dVar14,0) * SUB84(dVar9,0));
          }
          else {
            dVar14 = dVar14 * dVar9;
          }
          if (in_FPSCR_SZ == '\0') {
            dVar10 = (double)CONCAT44(uVar18,SUB84(dVar14,0));
          }
          if (in_FPSCR_PR == '\0') {
            fVar23 = SUB84(dVar10,0) + fVar12;
          }
          else {
            fVar23 = SUB84(dVar10 + dVar22,0);
          }
          if (in_FPSCR_SZ == '\0') {
            dVar24 = (double)CONCAT44(*(undefined4 *)((int)puVar8 + -0x7c),fVar23);
            dVar15 = (double)CONCAT44(fVar19,*(undefined4 *)((int)puVar8 + -0x78));
          }
          else {
            dVar24 = *(double *)((int)puVar8 + -0x7c);
          }
          if (in_FPSCR_PR == '\0') {
            dVar24 = (double)CONCAT44((float)((ulonglong)dVar24 >> 0x20) + fVar12,SUB84(dVar24,0));
            fVar12 = SUB84(dVar15,0) + fVar12;
          }
          else {
            dVar24 = dVar24 + dVar22;
            fVar12 = SUB84(dVar15 + dVar22,0);
          }
          dVar9 = in_xd14;
          if (in_FPSCR_SZ == '\0') {
            dVar9 = (double)CONCAT44(SUB84(dVar22,0),fVar12);
          }
          (*pcVar1)(dVar9,dVar24);
          uVar5 = *puVar7;
          puVar4 = PTR_DAT_0c905c54;
          if (in_FPSCR_SZ == '\0') {
            uVar17 = CONCAT44(*(undefined4 *)(param_3 + 0x124),SUB84(dVar9,0));
          }
          else {
            uVar17 = *(undefined8 *)(param_3 + 0x124);
          }
        }
        (*pcVar2)(uVar17,puVar4,uVar5);
        dVar24 = in_xd14;
        if (in_FPSCR_SZ == '\0') {
          dVar24 = (double)CONCAT44(SUB84(dVar22,0),SUB84(dVar22,0));
        }
        (*pcVar1)(dVar24,dVar24);
        uVar5 = SUB84(dVar24,0);
        dVar24 = dVar11;
      }
      else {
        *(undefined4 *)((int)puVar8 + -0x14) = *puVar7;
        *(undefined4 *)((int)puVar8 + -0x10) = *puVar6;
        if (in_FPSCR_SZ == '\0') {
          *(undefined4 *)((int)puVar8 + -0xc) = *(undefined4 *)(param_3 + 0x124);
          *(undefined4 *)((int)puVar8 + -8) = *(undefined4 *)(param_3 + 300);
        }
        else {
          *(undefined8 *)((int)puVar8 + -0xc) = *(undefined8 *)(param_3 + 0x124);
          in_xd2 = *(double *)(param_3 + 300);
          *(double *)((int)puVar8 + -8) = in_xd2;
        }
        *(undefined1 **)((int)puVar8 + -0x18) = (undefined1 *)((int)puVar8 + -0x14);
        *(undefined4 *)((int)puVar8 + -0x1c) = *(undefined4 *)(param_3 + 0x248);
        if ((*(ushort *)(*(int *)(param_3 + 0xec) + 0x36) & 2) == 0) {
          if (in_FPSCR_SZ == '\0') {
            uVar17 = CONCAT44(*(undefined4 *)(param_3 + 0x130),uVar5);
          }
          else {
            uVar17 = *(undefined8 *)(param_3 + 0x130);
          }
          (*DAT_0c905c44)(uVar17,(undefined1 *)((int)puVar8 + -0x1c));
          uVar5 = (undefined4)uVar17;
        }
        else {
          if (in_FPSCR_SZ == '\0') {
            uVar17 = CONCAT44(*(undefined4 *)(param_3 + 0x130),uVar5);
          }
          else {
            uVar17 = *(undefined8 *)(param_3 + 0x130);
          }
          (*DAT_0c905c40)(uVar17,(undefined1 *)((int)puVar8 + -0x1c));
          uVar5 = (undefined4)uVar17;
        }
      }
    }
    else {
      *(undefined4 *)((int)puVar8 + -0x14) = *puVar7;
      *(undefined4 *)((int)puVar8 + -0x10) = *puVar6;
      if (in_FPSCR_SZ == '\0') {
        *(undefined4 *)((int)puVar8 + -0xc) = *(undefined4 *)(param_3 + 0x124);
        *(undefined4 *)((int)puVar8 + -8) = *(undefined4 *)(param_3 + 300);
      }
      else {
        *(undefined8 *)((int)puVar8 + -0xc) = *(undefined8 *)(param_3 + 0x124);
        in_xd2 = *(double *)(param_3 + 300);
        *(double *)((int)puVar8 + -8) = in_xd2;
      }
      *(undefined1 **)((int)puVar8 + -0x18) = (undefined1 *)((int)puVar8 + -0x14);
      *(undefined4 *)((int)puVar8 + -0x1c) = *(undefined4 *)(param_3 + 0x248);
      if (in_FPSCR_SZ == '\0') {
        uVar17 = CONCAT44(*(undefined4 *)(param_3 + 0x130),uVar5);
      }
      else {
        uVar17 = *(undefined8 *)(param_3 + 0x130);
      }
      (*DAT_0c905acc)(uVar17,(undefined1 *)((int)puVar8 + -0x1c));
      uVar5 = (undefined4)uVar17;
    }
LAB_0c905ca8:
    if ((*(uint *)(param_3 + 0x60) & 4) == 0) {
      dVar9 = dVar14;
      if (in_FPSCR_SZ == '\0') {
        uVar16 = (undefined4)((ulonglong)dVar14 >> 0x20);
        dVar14 = (double)CONCAT44(uVar16,uVar16);
        dVar9 = dVar24;
      }
      dVar24 = dVar9;
      uVar16 = *(undefined4 *)(PTR_DAT_0c905e64 + 4);
      *(undefined4 *)((int)puVar8 + -0x10) = *(undefined4 *)PTR_DAT_0c905e64;
      *(undefined4 *)((int)puVar8 + -0xc) = uVar16;
      *(undefined4 *)((int)puVar8 + -8) = *(undefined4 *)(PTR_DAT_0c905e64 + 8);
      if (in_FPSCR_SZ == '\0') {
        dVar22 = (double)CONCAT44(*(undefined4 *)(param_3 + 0x130),SUB84(dVar22,0));
      }
      else {
        dVar22 = *(double *)(param_3 + 0x130);
      }
      if (in_FPSCR_PR == '\0') {
        dVar14 = (double)CONCAT44((int)((ulonglong)dVar14 >> 0x20),
                                  SUB84(dVar14,0) - (float)((ulonglong)dVar22 >> 0x20));
      }
      else {
        dVar14 = dVar14 - dVar22;
      }
      *(undefined4 *)((int)puVar8 + -0x18) = *(undefined4 *)(param_3 + 0x248);
      *(undefined4 *)((int)puVar8 + -0x14) = *puVar7;
      if (in_FPSCR_SZ == '\0') {
        uVar17 = CONCAT44(*(undefined4 *)(param_3 + 0x124),uVar5);
      }
      else {
        uVar17 = *(undefined8 *)(param_3 + 0x124);
      }
      (*DAT_0c905e68)(uVar17,0,(undefined1 *)((int)puVar8 + -0x18),0x20,
                      (undefined1 *)((int)puVar8 + -0x58));
      uVar5 = (undefined4)uVar17;
      (*DAT_0c905e6c)((undefined1 *)((int)puVar8 + -0x58),(undefined1 *)((int)puVar8 + -0x10),
                      (undefined1 *)((int)puVar8 + -100));
      *(undefined4 *)((int)puVar8 + -0x14) = *puVar6;
      if (in_FPSCR_SZ == '\0') {
        uVar17 = CONCAT44(*(undefined4 *)(param_3 + 300),uVar5);
      }
      else {
        uVar17 = *(undefined8 *)(param_3 + 300);
      }
      (*DAT_0c905e68)(uVar17,0,(undefined1 *)((int)puVar8 + -0x18),0x20,
                      (undefined1 *)((int)puVar8 + -0x58));
      (*DAT_0c905e6c)((undefined1 *)((int)puVar8 + -0x58),(undefined1 *)((int)puVar8 + -0x10),
                      (undefined1 *)((int)puVar8 + -0x70));
      if (in_FPSCR_SZ == '\0') {
        dVar10 = (double)CONCAT44(*(undefined4 *)((int)puVar8 + -100),
                                  *(undefined4 *)((int)puVar8 + -0x70));
        dVar9 = (double)((longlong)dVar14 << 0x20);
      }
      else {
        in_xd2 = *(double *)((int)puVar8 + -0x70);
        dVar10 = *(double *)((int)puVar8 + -100);
        dVar9 = dVar24;
      }
      fVar19 = (float)((ulonglong)dVar22 >> 0x20);
      fVar23 = (float)((ulonglong)dVar9 >> 0x20);
      if (in_FPSCR_PR == '\0') {
        fVar13 = fVar23 * (float)((ulonglong)dVar10 >> 0x20) + SUB84(dVar10,0) * fVar19;
      }
      else {
        fVar13 = SUB84(dVar10 * dVar22,0);
      }
      if (in_FPSCR_SZ == '\0') {
        *(float *)(param_3 + 0x81c) = fVar13;
        dVar9 = (double)CONCAT44(*(undefined4 *)((int)puVar8 + -0x60),
                                 *(undefined4 *)((int)puVar8 + -0x6c));
      }
      else {
        *(double *)(param_3 + 0x81c) = in_xd2;
        in_xd2 = *(double *)((int)puVar8 + -0x6c);
        dVar9 = *(double *)((int)puVar8 + -0x60);
      }
      if (in_FPSCR_PR == '\0') {
        fVar13 = fVar23 * (float)((ulonglong)dVar9 >> 0x20) + SUB84(dVar9,0) * fVar19;
      }
      else {
        fVar13 = SUB84(dVar9 * dVar22,0);
      }
      if (in_FPSCR_SZ == '\0') {
        *(float *)(param_3 + 0x820) = fVar13;
        dVar9 = (double)CONCAT44(*(undefined4 *)((int)puVar8 + -0x5c),
                                 *(undefined4 *)((int)puVar8 + -0x68));
      }
      else {
        *(double *)(param_3 + 0x820) = in_xd2;
        in_xd2 = *(double *)((int)puVar8 + -0x68);
        dVar9 = *(double *)((int)puVar8 + -0x5c);
      }
      if (in_FPSCR_PR == '\0') {
        fVar23 = fVar23 * (float)((ulonglong)dVar9 >> 0x20) + SUB84(dVar9,0) * fVar19;
      }
      else {
        fVar23 = SUB84(dVar9 * dVar22,0);
      }
      if (in_FPSCR_SZ == '\0') {
        *(float *)(param_3 + 0x824) = fVar23;
      }
      else {
        *(double *)(param_3 + 0x824) = in_xd2;
      }
      (*(code *)PTR_callback_candidate_c909d80_0c905e70)(param_2,param_3);
    }
    if (in_FPSCR_SZ == '\0') {
      dVar9 = (double)CONCAT44(*(undefined4 *)(param_3 + 0x130),puVar6[5]);
    }
    else {
      dVar9 = *(double *)(param_3 + 0x130);
    }
    if (in_FPSCR_PR == '\0') {
      in_dr2 = (double)CONCAT44((float)((ulonglong)dVar9 >> 0x20) + SUB84(dVar9,0),SUB84(dVar9,0));
    }
    else {
      in_dr2 = dVar9 + dVar9;
    }
    if (in_FPSCR_SZ == '\0') {
      uVar5 = (undefined4)((ulonglong)in_dr2 >> 0x20);
      in_dr2 = (double)CONCAT44(uVar5,uVar5);
    }
    fVar23 = (float)((ulonglong)dVar14 >> 0x20);
    if (in_FPSCR_PR == '\0') {
      bVar25 = fVar23 < SUB84(in_dr2,0);
    }
    else {
      bVar25 = dVar14 < in_dr2;
    }
    uVar5 = (undefined4)((ulonglong)in_dr2 >> 0x20);
    if (in_FPSCR_SZ == '\0') {
      *(undefined4 *)(param_3 + 0x130) = uVar5;
    }
    else {
      *(double *)(param_3 + 0x130) = in_dr2;
    }
    if (bVar25) {
      if (in_FPSCR_SZ == '\0') {
        *(int *)(param_3 + 0x130) = SUB84(dVar22,0);
        uVar16 = DAT_0c905e74;
        in_dr2 = (double)CONCAT44(uVar5,*(undefined4 *)(param_3 + 300));
        *(undefined4 *)(param_3 + 0x124) = *(undefined4 *)(param_3 + 300);
      }
      else {
        *(double *)(param_3 + 0x130) = in_xd14;
        uVar16 = DAT_0c905e74;
        *(undefined8 *)(param_3 + 0x124) = *(undefined8 *)(param_3 + 300);
      }
      *(undefined2 *)(param_3 + 0x11e) = *(undefined2 *)(param_3 + 0x120);
      *(undefined2 *)(param_3 + 0x118) = *(undefined2 *)(puVar6 + 1);
      *(ushort *)(param_3 + 0x11a) = *(ushort *)(param_3 + 0x11a) & (ushort)uVar16;
      *(ushort *)(param_3 + 0x11a) = *(ushort *)(param_3 + 0x11a) | 8;
      if (in_FPSCR_SZ == '\0') {
        *(float *)(param_3 + 0x134) = fVar23;
      }
      else {
        *(double *)(param_3 + 0x134) = dVar14;
      }
    }
  }
  (*DAT_0c9060c0)(1);
LAB_0c905fd2:
  uVar5 = SUB84(dVar22,0);
  (*DAT_0c9060c4)();
  (*DAT_0c9060cc)(0,DAT_0c9060c8);
  if ((*(uint *)(param_3 + 0x60) & 4) != 0) {
    if (in_FPSCR_SZ == '\0') {
      in_dr2 = (double)CONCAT44((int)((ulonglong)in_dr2 >> 0x20),_DAT_0c9060d0);
      dVar22 = (double)CONCAT44(*(undefined4 *)(param_3 + 0x6c),uVar5);
    }
    else {
      dVar22 = *(double *)(param_3 + 0x6c);
    }
    if (in_FPSCR_PR == '\0') {
      dVar22 = (double)CONCAT44((float)((ulonglong)dVar22 >> 0x20) * SUB84(in_dr2,0),SUB84(dVar22,0)
                               );
    }
    else {
      dVar22 = dVar22 * in_dr2;
    }
    if (in_FPSCR_SZ == '\0') {
      dVar14 = (double)CONCAT44((int)((ulonglong)dVar14 >> 0x20),*(undefined4 *)(param_3 + 0x70));
    }
    else {
      dVar24 = *(double *)(param_3 + 0x70);
    }
    if (in_FPSCR_PR == '\0') {
      dVar14 = (double)CONCAT44((int)((ulonglong)dVar14 >> 0x20),SUB84(dVar14,0) + -1.0);
    }
    else {
      dVar14 = dVar14 + -(double)CONCAT44(0x3f800000,SUB84(in_dr2,0));
    }
    (*DAT_0c9060d8)(PTR_PTR_0c9060d4);
    (*DAT_0c9060dc)(0);
    uVar21 = (uint)((ulonglong)dVar14 >> 0x20);
    if (in_FPSCR_SZ == '\0') {
      dVar14 = (double)CONCAT44(uVar21,uVar21);
      dVar24 = (double)(ulonglong)uVar21;
    }
    uVar5 = SUB84(dVar14,0);
    (*pcVar1)(dVar24,dVar14);
    (*DAT_0c9060e0)(0,param_2 + 0x20);
    iVar3 = *(int *)(param_2 + 0x18) + 0x4000;
    *(int *)((int)puVar8 + -0x94) = iVar3;
    if (iVar3 != 0) {
      (*DAT_0c9060e4)(0,iVar3);
    }
    uVar16 = (undefined4)((ulonglong)dVar22 >> 0x20);
    dVar14 = dVar22;
    dVar24 = dVar22;
    if (in_FPSCR_SZ == '\0') {
      dVar14 = (double)CONCAT44(uVar16,uVar16);
      dVar24 = (double)CONCAT44(uVar16,uVar5);
    }
    (*DAT_0c9060e8)(dVar14,dVar24,0);
    uVar5 = SUB84(dVar22,0);
    (*DAT_0c9060f0)(PTR_DAT_0c9060ec);
    (*DAT_0c9060c0)(1);
  }
  (*DAT_0c9060f4)();
  (*(code *)PTR_callback_candidate_c90c384_0c9060f8)(param_2,param_3);
  (*DAT_0c9060fc)(0x200);
  if (in_FPSCR_SZ == '\0') {
    in_xd14 = (double)CONCAT44(uVar5,uVar5);
  }
  (*pcVar1)(in_xd14,in_xd14);
  return;
}

