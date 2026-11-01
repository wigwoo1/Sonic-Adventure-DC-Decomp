/* Ghidra draft only; not buildable/verified source. */

/* WARNING: Removing unreachable block (ram,0x8c0b39e2) */
/* WARNING: Removing unreachable block (ram,0x8c0b398a) */
/* WARNING: Removing unreachable block (ram,0x8c0b3902) */
/* WARNING: Removing unreachable block (ram,0x8c0b38a2) */
/* WARNING: Removing unreachable block (ram,0x8c0b38d0) */
/* WARNING: Removing unreachable block (ram,0x8c0b3934) */
/* WARNING: Removing unreachable block (ram,0x8c0b39a4) */
/* WARNING: Removing unreachable block (ram,0x8c0b3a00) */
/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined4 FUN_8c0b37f2(int param_1)

{
  undefined *puVar1;
  undefined4 uVar2;
  int iVar3;
  int iVar4;
  undefined8 *puVar5;
  float *pfVar6;
  float *pfVar7;
  int *piVar8;
  undefined4 extraout_fr1;
  undefined4 extraout_fr1_00;
  undefined4 extraout_fr1_01;
  undefined4 extraout_fr1_02;
  double dVar9;
  longlong lVar10;
  double extraout_dr0;
  float fVar11;
  double in_dr2;
  double dVar12;
  undefined8 in_dr12;
  undefined8 uVar14;
  undefined8 in_dr14;
  double dVar15;
  undefined8 in_xd0;
  undefined8 uVar16;
  undefined8 in_xd2;
  undefined8 in_xd12;
  longlong in_xd14;
  bool bVar17;
  undefined4 in_PR;
  undefined2 uVar18;
  int iVar19;
  char in_FPSCR_PR;
  char in_FPSCR_SZ;
  undefined1 local_30 [16];
  undefined8 uStack_20;
  undefined8 uStack_18;
  float fVar13;
  
  fVar11 = (float)((ulonglong)in_xd12 >> 0x20);
  if (in_FPSCR_SZ == '\0') {
    uStack_18 = CONCAT44((int)in_dr14,(undefined4)uStack_18);
                    /* WARNING: Ignoring partial resolution of indirect */
    uStack_18._0_4_ = (int)((ulonglong)in_dr14 >> 0x20);
    puVar5 = &uStack_20;
    uStack_20 = CONCAT44((int)in_dr12,(int)((ulonglong)in_dr12 >> 0x20));
  }
  else {
    puVar5 = (undefined8 *)local_30;
    uStack_18 = in_xd14;
    uStack_20 = in_dr14;
  }
  *(undefined4 *)((int)puVar5 + -4) = in_PR;
  piVar8 = (int *)((int)puVar5 + -0x24);
  if (in_FPSCR_SZ == '\0') {
    in_dr2 = (double)CONCAT44((int)((ulonglong)in_dr2 >> 0x20),*(undefined4 *)(param_1 + 0x34));
  }
  else {
    in_xd2 = *(undefined8 *)(param_1 + 0x34);
  }
  if (in_FPSCR_PR == '\0') {
    iVar19 = (int)SUB84(in_dr2,0);
  }
  else {
    iVar19 = (int)in_dr2;
  }
  *(char *)((int)puVar5 + -0x20) = (char)iVar19;
  iVar19 = (*(code *)PTR_FUN_8c0b3aa0)(iVar19);
  uVar2 = (undefined4)((ulonglong)in_dr12 >> 0x20);
  *(undefined4 *)((int)puVar5 + -0x24) = 0;
  if ((*(uint *)(iVar19 + 500) & 2) == 0) {
    uVar2 = 0;
  }
  else {
    FUN_8c0b3480(param_1,(undefined1 *)((int)puVar5 + -0x10),(undefined1 *)((int)puVar5 + -0x1c),0);
    if (in_FPSCR_SZ == '\0') {
      dVar15 = (double)CONCAT44(*(undefined4 *)((int)puVar5 + -0xc),*(undefined4 *)(iVar19 + 0x1ec))
      ;
    }
    else {
      dVar15 = *(double *)((int)puVar5 + -0xc);
      in_xd14 = *(longlong *)(iVar19 + 0x1ec);
    }
    fVar13 = (float)((ulonglong)dVar15 >> 0x20);
    if (in_FPSCR_PR == '\0' && fVar13 < SUB84(dVar15,0)) {
      if (in_FPSCR_SZ == '\0') {
        dVar12 = (double)CONCAT44(fVar13,DAT_8c0b3aa4);
      }
      else {
        in_xd2 = CONCAT44(_DAT_8c0b3aa8,DAT_8c0b3aa4);
        dVar12 = dVar15;
      }
      if (in_FPSCR_PR == '\0') {
        fVar13 = (float)((ulonglong)dVar12 >> 0x20) + SUB84(dVar12,0);
        dVar12 = (double)CONCAT44(fVar13,SUB84(dVar12,0));
        bVar17 = SUB84(dVar15,0) < fVar13;
      }
      else {
        dVar12 = dVar12 + dVar12;
        bVar17 = dVar15 < dVar12;
      }
      piVar8 = (int *)((int)puVar5 + -0x24);
      if (bVar17) {
        iVar3 = FUN_8c0b33a0(param_1);
        puVar1 = PTR_FUN_8c0b3aac;
        *(int *)((int)puVar5 + -0x24) = iVar3;
        piVar8 = (int *)((int)puVar5 + -0x24);
        if (iVar3 != 0) {
          if (in_FPSCR_SZ == '\0') {
            dVar15 = (double)(ulonglong)_DAT_8c0b3aa8;
          }
          else {
            in_xd14 = CONCAT44(PTR_FUN_8c0b3aac,_DAT_8c0b3aa8);
          }
          iVar3 = (uint)*(byte *)(param_1 + 3) * 0x48 + *(int *)(param_1 + 8);
          *(int *)((int)puVar5 + -0x20) = iVar3;
          if (in_FPSCR_SZ == '\0') {
            dVar9 = (double)CONCAT44(*(undefined4 *)(iVar3 + 0x20),extraout_fr1);
            dVar12 = (double)CONCAT44((int)((ulonglong)dVar12 >> 0x20),*(undefined4 *)(iVar3 + 0x1c)
                                     );
          }
          else {
            dVar9 = *(double *)(iVar3 + 0x20);
            in_xd2 = *(undefined8 *)(iVar3 + 0x1c);
          }
          if (in_FPSCR_PR == '\0') {
            fVar13 = SUB84(dVar12,0) - (float)((ulonglong)dVar9 >> 0x20);
          }
          else {
            fVar13 = SUB84(dVar12 - dVar9,0);
          }
          if (in_FPSCR_SZ == '\0') {
            pfVar6 = (float *)((int)puVar5 + -0x28);
            *(float *)((int)puVar5 + -0x28) = fVar13;
          }
          else {
            pfVar6 = (float *)((int)puVar5 + -0x2c);
            *(undefined8 *)((int)puVar5 + -0x2c) = in_xd2;
          }
          iVar3 = (*(code *)PTR_FUN_8c0b3aac)();
          if (in_FPSCR_SZ == '\0') {
            fVar13 = *pfVar6;
            pfVar7 = pfVar6 + 1;
          }
          else {
            fVar13 = (float)((ulonglong)*(undefined8 *)pfVar6 >> 0x20);
            pfVar7 = pfVar6 + 2;
          }
          dVar12 = (double)CONCAT44(fVar13,(float)iVar3);
          lVar10 = in_xd14;
          if (in_FPSCR_SZ == '\0') {
            lVar10 = (longlong)dVar15 << 0x20;
          }
          if (in_FPSCR_PR == '\0') {
            fVar13 = fVar13 * (float)iVar3;
          }
          else {
            fVar13 = (float)((ulonglong)(dVar12 * dVar12) >> 0x20);
          }
          iVar3 = (uint)*(byte *)(param_1 + 3) * 0x48 + *(int *)(param_1 + 8);
          if (in_FPSCR_PR == '\0') {
            uVar2 = 0x40000000;
          }
          else {
            uVar2 = SUB84((double)CONCAT44(uVar2,0x3f800000) + (double)CONCAT44(uVar2,0x3f800000),0)
            ;
          }
          if (in_FPSCR_SZ == '\0') {
            uVar14 = CONCAT44(*(undefined4 *)(iVar3 + 0x20),uVar2);
          }
          else {
            uVar14 = *(undefined8 *)(iVar3 + 0x20);
          }
          if (in_FPSCR_PR == '\0') {
            uVar14 = CONCAT44((float)((ulonglong)lVar10 >> 0x20) * fVar13 +
                              (float)((ulonglong)uVar14 >> 0x20),(int)uVar14);
          }
          if (in_FPSCR_SZ == '\0') {
            dVar15 = (double)CONCAT44(DAT_8c0b3ab0,SUB84(dVar15,0));
          }
          else {
            dVar15 = (double)CONCAT44(_DAT_8c0b3ab4,DAT_8c0b3ab0);
          }
          iVar3 = (*(code *)puVar1)();
          dVar12 = dVar15;
          lVar10 = in_xd14;
          if (in_FPSCR_SZ == '\0') {
            dVar12 = (double)CONCAT44((int)((ulonglong)dVar15 >> 0x20),(float)iVar3);
            lVar10 = CONCAT44(SUB84(dVar15,0),extraout_fr1_00);
          }
          if (in_FPSCR_PR == '\0') {
            dVar12 = (double)CONCAT44((float)((ulonglong)lVar10 >> 0x20) * SUB84(dVar12,0) +
                                      (float)((ulonglong)dVar12 >> 0x20),SUB84(dVar12,0));
          }
          iVar3 = (uint)*(byte *)(param_1 + 3) * 0x48 + *(int *)(param_1 + 8);
          if (in_FPSCR_SZ == '\0') {
            dVar9 = (double)CONCAT44(*(undefined4 *)(iVar3 + 0x30),(int)lVar10);
            dVar12 = (double)CONCAT44((int)((ulonglong)dVar12 >> 0x20),pfVar7[2]);
          }
          else {
            dVar9 = *(double *)(iVar3 + 0x30);
            in_xd2 = *(undefined8 *)(pfVar7 + 2);
          }
          if (in_FPSCR_PR == '\0') {
            dVar12 = (double)CONCAT44((float)((ulonglong)dVar12 >> 0x20) *
                                      (float)((ulonglong)dVar9 >> 0x20),SUB84(dVar12,0));
          }
          else {
            dVar12 = dVar12 * dVar9;
          }
          fVar13 = fVar11;
          if (in_FPSCR_SZ == '\0') {
            fVar13 = (float)uVar14;
          }
          if (in_FPSCR_PR == '\0') {
            dVar12 = (double)(ulonglong)
                             (uint)(fVar13 * (float)((ulonglong)dVar12 >> 0x20) + SUB84(dVar12,0));
          }
          if (in_FPSCR_SZ == '\0') {
            pfVar7[2] = (float)SUB84(dVar12,0);
          }
          else {
            *(undefined8 *)(pfVar7 + 2) = in_xd2;
          }
          iVar3 = (*(code *)puVar1)();
          dVar12 = dVar15;
          lVar10 = in_xd14;
          if (in_FPSCR_SZ == '\0') {
            dVar12 = (double)CONCAT44((int)((ulonglong)dVar15 >> 0x20),(float)iVar3);
            lVar10 = CONCAT44(SUB84(dVar15,0),extraout_fr1_01);
          }
          if (in_FPSCR_PR == '\0') {
            dVar12 = (double)CONCAT44((float)((ulonglong)lVar10 >> 0x20) * SUB84(dVar12,0) +
                                      (float)((ulonglong)dVar12 >> 0x20),SUB84(dVar12,0));
          }
          iVar3 = (uint)*(byte *)(param_1 + 3) * 0x48 + *(int *)(param_1 + 8);
          if (in_FPSCR_SZ == '\0') {
            dVar9 = (double)CONCAT44(*(undefined4 *)(iVar3 + 0x30),(int)lVar10);
            dVar12 = (double)CONCAT44((int)((ulonglong)dVar12 >> 0x20),pfVar7[3]);
          }
          else {
            dVar9 = *(double *)(iVar3 + 0x30);
            in_xd2 = *(undefined8 *)(pfVar7 + 3);
          }
          if (in_FPSCR_PR == '\0') {
            dVar12 = (double)CONCAT44((float)((ulonglong)dVar12 >> 0x20) *
                                      (float)((ulonglong)dVar9 >> 0x20),SUB84(dVar12,0));
          }
          else {
            dVar12 = dVar12 * dVar9;
          }
          fVar13 = fVar11;
          if (in_FPSCR_SZ == '\0') {
            fVar13 = (float)uVar14;
          }
          if (in_FPSCR_PR == '\0') {
            dVar12 = (double)(ulonglong)
                             (uint)(fVar13 * (float)((ulonglong)dVar12 >> 0x20) + SUB84(dVar12,0));
          }
          if (in_FPSCR_SZ == '\0') {
            pfVar7[3] = (float)SUB84(dVar12,0);
          }
          else {
            *(undefined8 *)(pfVar7 + 3) = in_xd2;
          }
          iVar3 = (*(code *)puVar1)();
          lVar10 = in_xd14;
          dVar12 = dVar15;
          if (in_FPSCR_SZ == '\0') {
            lVar10 = CONCAT44(SUB84(dVar15,0),extraout_fr1_02);
            dVar12 = (double)CONCAT44((int)((ulonglong)dVar15 >> 0x20),(float)iVar3);
          }
          if (in_FPSCR_PR == '\0') {
            dVar12 = (double)CONCAT44((float)((ulonglong)lVar10 >> 0x20) * SUB84(dVar12,0) +
                                      (float)((ulonglong)dVar12 >> 0x20),SUB84(dVar12,0));
          }
          iVar3 = (uint)*(byte *)(param_1 + 3) * 0x48 + *(int *)(param_1 + 8);
          if (in_FPSCR_SZ == '\0') {
            dVar9 = (double)CONCAT44(*(undefined4 *)(iVar3 + 0x30),(int)lVar10);
            dVar12 = (double)CONCAT44((int)((ulonglong)dVar12 >> 0x20),pfVar7[4]);
          }
          else {
            dVar9 = *(double *)(iVar3 + 0x30);
            in_xd2 = *(undefined8 *)(pfVar7 + 4);
          }
          if (in_FPSCR_PR == '\0') {
            dVar12 = (double)CONCAT44((float)((ulonglong)dVar12 >> 0x20) *
                                      (float)((ulonglong)dVar9 >> 0x20),SUB84(dVar12,0));
          }
          else {
            dVar12 = dVar12 * dVar9;
          }
          if (in_FPSCR_SZ == '\0') {
            fVar11 = (float)uVar14;
          }
          if (in_FPSCR_PR == '\0') {
            dVar12 = (double)(ulonglong)
                             (uint)(fVar11 * (float)((ulonglong)dVar12 >> 0x20) + SUB84(dVar12,0));
          }
          if (in_FPSCR_SZ == '\0') {
            pfVar7[4] = (float)SUB84(dVar12,0);
          }
          else {
            *(undefined8 *)(pfVar7 + 4) = in_xd2;
          }
          iVar4 = (int)*pfVar7;
          iVar3 = (int)pfVar7[6];
          *(float *)(iVar4 + 4) = pfVar7[5];
          *(int *)(iVar4 + 8) = iVar3;
          *(float *)(iVar4 + 0xc) = pfVar7[7];
          iVar4 = (int)*pfVar7;
          iVar3 = (int)pfVar7[3];
          *(float *)(iVar4 + 0x10) = pfVar7[2];
          *(int *)(iVar4 + 0x14) = iVar3;
          *(float *)(iVar4 + 0x18) = pfVar7[4];
          iVar4 = (int)*pfVar7;
          iVar3 = (*(code *)puVar1)();
          fVar13 = (float)((ulonglong)in_xd14 >> 0x20);
          fVar11 = _DAT_8c0b3ab8;
          if (in_FPSCR_SZ == '\0') {
            fVar11 = _DAT_8c0b3ab4;
          }
          dVar12 = (double)CONCAT44(fVar11,(float)iVar3);
          if (in_FPSCR_PR == '\0') {
            uVar18 = (undefined2)(int)((float)iVar3 * fVar11);
          }
          else {
            uVar18 = (undefined2)(int)(dVar12 * dVar12);
          }
          *(undefined2 *)(iVar4 + 0x1c) = uVar18;
          iVar4 = (int)*pfVar7;
          iVar3 = (*(code *)puVar1)();
          if (in_FPSCR_SZ == '\0') {
            fVar13 = SUB84(dVar15,0);
          }
          if (in_FPSCR_PR == '\0') {
            dVar15 = (double)CONCAT44(fVar13 * (float)iVar3 + (float)((ulonglong)dVar15 >> 0x20),
                                      SUB84(dVar15,0));
          }
          iVar3 = (uint)*(byte *)(param_1 + 3) * 0x48 + *(int *)(param_1 + 8);
          if (in_FPSCR_SZ == '\0') {
            dVar12 = (double)CONCAT44((int)((ulonglong)dVar15 >> 0x20),*(undefined4 *)(iVar3 + 0x38)
                                     );
          }
          else {
            in_xd2 = *(undefined8 *)(iVar3 + 0x38);
            dVar12 = dVar15;
          }
          if (in_FPSCR_PR == '\0') {
            dVar12 = (double)CONCAT44((float)((ulonglong)dVar12 >> 0x20) * SUB84(dVar12,0),
                                      SUB84(dVar12,0));
          }
          else {
            dVar12 = dVar12 * dVar12;
          }
          if (in_FPSCR_SZ == '\0') {
            dVar12 = (double)CONCAT44((int)((ulonglong)dVar12 >> 0x20),_DAT_8c0b3ab8);
          }
          else {
            in_xd2 = CONCAT44(_DAT_8c0b3abc,_DAT_8c0b3ab8);
          }
          if (in_FPSCR_PR == '\0') {
            uVar18 = (undefined2)(int)((float)((ulonglong)dVar12 >> 0x20) * SUB84(dVar12,0));
          }
          else {
            uVar18 = (undefined2)(int)(dVar12 * dVar12);
          }
          *(undefined2 *)(iVar4 + 0x1e) = uVar18;
          if (in_FPSCR_SZ == '\0') {
            *(undefined4 *)*pfVar7 = *(undefined4 *)(iVar19 + 0x1ec);
          }
          else {
            *(undefined8 *)*pfVar7 = *(undefined8 *)(iVar19 + 0x1ec);
          }
          iVar3 = (int)*pfVar7;
          iVar19 = (*(code *)puVar1)();
          uVar2 = (undefined4)((ulonglong)uVar14 >> 0x20);
          dVar12 = extraout_dr0;
          uVar16 = uVar14;
          if (in_FPSCR_SZ == '\0') {
            dVar12 = (double)CONCAT44((int)((ulonglong)extraout_dr0 >> 0x20),uVar2);
            uVar16 = in_xd0;
          }
          iVar4 = (uint)*(byte *)(param_1 + 3) * 0x48 + *(int *)(param_1 + 8);
          if (in_FPSCR_SZ == '\0') {
            dVar9 = (double)CONCAT44(*(undefined4 *)(iVar4 + 0x24),(float)iVar19);
          }
          else {
            dVar9 = *(double *)(iVar4 + 0x24);
          }
          if (in_FPSCR_PR == '\0') {
            fVar11 = SUB84(dVar9,0) * (float)((ulonglong)dVar9 >> 0x20);
          }
          else {
            fVar11 = SUB84(dVar9 * dVar9,0);
          }
          if (in_FPSCR_PR == '\0') {
            dVar15 = (double)CONCAT44(0x3f800000,fVar11 * SUB84(dVar15,0));
          }
          else {
            dVar15 = (double)CONCAT44(0x3f800000,fVar11) * dVar15;
          }
          fVar11 = (float)((ulonglong)dVar15 >> 0x20);
          dVar9 = (double)CONCAT44(fVar11,-SUB84(dVar15,0));
          if (in_FPSCR_PR == '\0') {
            dVar9 = (double)CONCAT44(fVar11,-SUB84(dVar15,0) + fVar11);
          }
          else {
            dVar9 = dVar9 + dVar9;
          }
          if (in_FPSCR_SZ == '\0') {
            *(int *)(iVar3 + 0x20) = SUB84(dVar9,0);
            dVar9 = (double)CONCAT44((int)((ulonglong)dVar9 >> 0x20),_DAT_8c0b3abc);
          }
          else {
            *(undefined8 *)(iVar3 + 0x20) = in_xd2;
          }
          if (in_FPSCR_PR == '\0') {
            fVar11 = SUB84(dVar12,0) * SUB84(dVar9,0);
          }
          else {
            fVar11 = SUB84(dVar12 * dVar9,0);
          }
          if (in_FPSCR_SZ == '\0') {
            *(float *)((int)*pfVar7 + 0x24) = fVar11;
          }
          else {
            *(undefined8 *)((int)*pfVar7 + 0x24) = uVar16;
          }
          piVar8 = (int *)pfVar7;
          if (in_FPSCR_SZ == '\0') {
            *(undefined4 *)((int)*pfVar7 + 0x28) = uVar2;
          }
          else {
            *(undefined8 *)((int)*pfVar7 + 0x28) = uVar14;
          }
        }
      }
    }
    uVar2 = *piVar8;
  }
  return uVar2;
}

