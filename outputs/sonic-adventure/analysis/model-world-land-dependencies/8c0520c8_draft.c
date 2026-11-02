/* Ghidra draft only; not buildable/verified source. */

/* WARNING: Removing unreachable block (ram,0x8c052138) */

void FUN_8c0520c8(undefined4 param_1,double param_2,undefined4 param_3,undefined4 param_4)

{
  double dVar1;
  int *piVar2;
  double *pdVar3;
  undefined *puVar4;
  undefined *puVar5;
  uint uVar6;
  short *psVar7;
  int iVar8;
  int iVar9;
  short sVar10;
  short sVar11;
  double *pdVar12;
  undefined8 *puVar13;
  ulonglong in_dr2;
  double dVar14;
  undefined4 uVar16;
  double dVar15;
  double dVar17;
  float fVar18;
  float fVar19;
  undefined4 uVar20;
  double dVar21;
  undefined4 uVar22;
  float fVar23;
  double in_dr14;
  undefined4 uVar24;
  double in_xd2;
  double dVar25;
  double in_xd4;
  double dVar26;
  double in_xd6;
  double in_xd8;
  double in_xd14;
  undefined4 in_PR;
  char in_FPSCR_PR;
  char in_FPSCR_SZ;
  undefined8 local_20;
  
  pdVar3 = DAT_8c052110;
  piVar2 = DAT_8c052104;
  if (in_FPSCR_SZ == '\0') {
    puVar13 = (undefined8 *)((int)&local_20 + 4);
    local_20 = (double)CONCAT44(SUB84(in_dr14,0),(undefined4)local_20);
  }
  else {
    puVar13 = &local_20;
    local_20 = in_xd14;
  }
  *(undefined4 *)((int)puVar13 + -4) = in_PR;
  pdVar12 = *(double **)(*DAT_8c052104 + 0xc);
  *DAT_8c05210c = 0xffff;
  dVar14 = (double)(in_dr2 & 0xffffffff00000000);
  uVar24 = (undefined4)((ulonglong)in_dr14 >> 0x20);
  if ((*(ushort *)(*piVar2 + 4) & 4) == 0) {
    if (in_FPSCR_SZ == '\0') {
      in_dr14 = (double)CONCAT44(uVar24,*(undefined4 *)pdVar3);
    }
    else {
      in_xd14 = *pdVar3;
    }
  }
  else if (in_FPSCR_SZ == '\0') {
    in_dr14 = (double)CONCAT44(uVar24,*(undefined4 *)(*piVar2 + 8));
  }
  else {
    in_xd14 = *(double *)(*piVar2 + 8);
  }
  if (in_FPSCR_SZ == '\0') {
    *(undefined4 *)((int)puVar13 + -0xc) = 0;
    *(undefined4 *)((int)puVar13 + -0x10) = 0;
    dVar17 = (double)CONCAT44(DAT_8c052280,param_1);
    dVar14 = (double)CONCAT44((int)(in_dr2 >> 0x20),SUB84(in_dr14,0));
  }
  else {
    *(double *)((int)puVar13 + -0xc) = in_xd2;
    *(double *)((int)puVar13 + -0x10) = in_xd2;
    dVar17 = (double)CONCAT44(PTR_DAT_8c052284,DAT_8c052280);
    in_xd2 = in_xd14;
  }
  fVar23 = (float)((ulonglong)dVar17 >> 0x20);
  if (in_FPSCR_PR == '\0') {
    dVar14 = (double)CONCAT44((int)((ulonglong)dVar14 >> 0x20),SUB84(dVar14,0) * fVar23);
  }
  else {
    dVar14 = dVar14 * dVar17;
  }
  dVar15 = (double)CONCAT44((int)((ulonglong)dVar14 >> 0x20),-SUB84(dVar14,0));
  if (in_FPSCR_SZ == '\0') {
    *(float *)((int)puVar13 + -8) = -SUB84(dVar14,0);
  }
  else {
    *(double *)((int)puVar13 + -8) = in_xd2;
  }
  if (in_FPSCR_PR == '\0') {
    fVar23 = SUB84(in_dr14,0) * fVar23;
  }
  else {
    fVar23 = SUB84(in_dr14 * dVar17,0);
  }
  (*(code *)PTR_FUN_8c052288)(PTR_DAT_8c052284);
  puVar4 = PTR_DAT_8c05228c;
  iVar8 = *(int *)(*(int *)PTR_DAT_8c05228c + 0x18);
  *(int *)((int)puVar13 + -0x14) = iVar8;
  if (iVar8 != 0) {
    (*(code *)PTR_FUN_8c052290)(0,*(undefined4 *)((int)puVar13 + -0x14));
  }
  iVar8 = *(int *)(*(int *)puVar4 + 0x14);
  *(int *)((int)puVar13 + -0x14) = iVar8;
  if (iVar8 != 0) {
    (*(code *)PTR_FUN_8c052294)(0,*(undefined4 *)((int)puVar13 + -0x14));
  }
  (*(code *)PTR_FUN_8c052298)
            (0,(undefined1 *)((int)puVar13 + -0x10),(undefined1 *)((int)puVar13 + -0x10));
  (*(code *)PTR_FUN_8c05229c)(1);
  psVar7 = DAT_8c0522ac;
  uVar6 = DAT_8c0522a8;
  iVar8 = DAT_8c0522a4;
  puVar5 = PTR_DAT_8c0522a0;
  iVar9 = *(int *)puVar4;
  if (in_FPSCR_SZ == '\0') {
    dVar15 = (double)CONCAT44((int)((ulonglong)dVar15 >> 0x20),*(undefined4 *)((int)puVar13 + -0x10)
                             );
    dVar14 = (double)CONCAT44(*(undefined4 *)(iVar9 + 0x20),param_3);
  }
  else {
    in_xd2 = *(double *)((int)puVar13 + -0x10);
    dVar14 = *(double *)(iVar9 + 0x20);
  }
  if (in_FPSCR_PR == '\0') {
    dVar14 = (double)CONCAT44((float)((ulonglong)dVar14 >> 0x20) + SUB84(dVar15,0),SUB84(dVar14,0));
  }
  else {
    dVar14 = dVar14 + dVar15;
  }
  if (in_FPSCR_SZ == '\0') {
    dVar15 = (double)CONCAT44((int)((ulonglong)dVar15 >> 0x20),*(undefined4 *)((int)puVar13 + -0xc))
    ;
  }
  else {
    in_xd2 = *(double *)((int)puVar13 + -0xc);
  }
  sVar11 = *(short *)*piVar2;
  if (in_FPSCR_SZ == '\0') {
    dVar21 = (double)CONCAT44(*(undefined4 *)(iVar9 + 0x24),param_4);
  }
  else {
    dVar21 = *(double *)(iVar9 + 0x24);
  }
  if (in_FPSCR_PR == '\0') {
    dVar21 = (double)CONCAT44((float)((ulonglong)dVar21 >> 0x20) + SUB84(dVar15,0),SUB84(dVar21,0));
  }
  else {
    dVar21 = dVar21 + dVar15;
  }
  if (in_FPSCR_SZ == '\0') {
    dVar15 = (double)CONCAT44((int)((ulonglong)dVar15 >> 0x20),*(undefined4 *)((int)puVar13 + -8));
    dVar14 = (double)CONCAT44((int)((ulonglong)dVar14 >> 0x20),*(undefined4 *)(iVar9 + 0x28));
  }
  else {
    in_xd2 = *(double *)((int)puVar13 + -8);
    in_xd8 = *(double *)(iVar9 + 0x28);
  }
  sVar10 = 0;
  if (in_FPSCR_PR == '\0') {
    dVar14 = (double)CONCAT44((int)((ulonglong)dVar14 >> 0x20),SUB84(dVar14,0) + SUB84(dVar15,0));
  }
  else {
    dVar14 = dVar14 + dVar15;
  }
  while( true ) {
    sVar11 = sVar11 + -1;
    uVar24 = SUB84(param_2,0);
    if (sVar11 < 0) break;
    if (((*(int *)((int)pdVar12 + 0x1c) == 0) ||
        ((*(uint *)((int)pdVar12 + 0x1c) & *(uint *)puVar5) != 0)) &&
       ((*(uint *)(pdVar12 + 4) & uVar6) != 0)) {
      uVar16 = (undefined4)((ulonglong)dVar15 >> 0x20);
      uVar20 = (undefined4)((ulonglong)dVar14 >> 0x20);
      uVar22 = (undefined4)((ulonglong)dVar21 >> 0x20);
      if (((ulonglong)pdVar12[4] & 0x20) == 0) {
        if (in_FPSCR_SZ == '\0') {
          dVar15 = (double)CONCAT44(uVar16,*(undefined4 *)((int)pdVar12 + 0xc));
          param_2 = (double)CONCAT44(uVar20,uVar24);
          dVar17 = (double)CONCAT44(fVar23,SUB84(dVar17,0));
        }
        else {
          in_xd2 = *(double *)((int)pdVar12 + 0xc);
          param_2 = dVar14;
          dVar17 = in_xd14;
        }
        if (in_FPSCR_PR == '\0') {
          dVar17 = (double)CONCAT44((float)((ulonglong)dVar17 >> 0x20) + SUB84(dVar15,0),
                                    SUB84(dVar17,0));
        }
        else {
          dVar17 = dVar17 + dVar15;
        }
        if (in_FPSCR_SZ == '\0') {
          dVar15 = (double)CONCAT44((int)((ulonglong)dVar15 >> 0x20),*(undefined4 *)pdVar12);
          param_2 = (double)CONCAT44((int)((ulonglong)param_2 >> 0x20),uVar22);
        }
        else {
          in_xd2 = *pdVar12;
          in_xd6 = dVar21;
        }
        if (in_FPSCR_PR == '\0') {
          param_2 = (double)CONCAT44((float)((ulonglong)param_2 >> 0x20) - SUB84(dVar15,0),
                                     SUB84(param_2,0));
        }
        else {
          param_2 = param_2 - dVar15;
        }
        if (in_FPSCR_SZ == '\0') {
          dVar15 = (double)CONCAT44((int)((ulonglong)dVar15 >> 0x20),
                                    *(undefined4 *)((int)pdVar12 + 4));
          dVar17 = (double)CONCAT44((int)((ulonglong)dVar17 >> 0x20),SUB84(dVar14,0));
          dVar26 = in_xd4;
          dVar25 = in_xd2;
        }
        else {
          dVar25 = *(double *)((int)pdVar12 + 4);
          dVar26 = in_xd8;
        }
        if (in_FPSCR_PR == '\0') {
          param_2 = (double)CONCAT44((int)((ulonglong)param_2 >> 0x20),
                                     SUB84(param_2,0) - SUB84(dVar15,0));
        }
        else {
          param_2 = param_2 - dVar15;
        }
        if (in_FPSCR_SZ == '\0') {
          dVar15 = (double)CONCAT44((int)((ulonglong)dVar15 >> 0x20),*(undefined4 *)(pdVar12 + 1));
        }
        else {
          dVar25 = pdVar12[1];
        }
        if (in_FPSCR_PR == '\0') {
          fVar18 = (float)((ulonglong)dVar17 >> 0x20);
          dVar17 = (double)CONCAT44(fVar18 * fVar18,SUB84(dVar17,0) - SUB84(dVar15,0));
        }
        else {
          dVar17 = dVar17 * dVar17 - dVar15;
        }
        fVar18 = (float)((ulonglong)param_2 >> 0x20);
        in_xd2 = in_xd6;
        dVar1 = param_2;
        if (in_FPSCR_SZ == '\0') {
          dVar15 = (double)CONCAT44((int)((ulonglong)dVar15 >> 0x20),SUB84(param_2,0));
          in_xd2 = dVar25;
          dVar1 = (double)((ulonglong)(uint)fVar18 << 0x20);
        }
        if (in_FPSCR_PR == '\0') {
          dVar15 = (double)CONCAT44((int)((ulonglong)dVar15 >> 0x20),
                                    (float)((ulonglong)dVar1 >> 0x20) * fVar18 +
                                    SUB84(dVar15,0) * SUB84(param_2,0));
        }
        else {
          dVar15 = dVar15 * param_2;
        }
        fVar18 = SUB84(dVar17,0);
        if (in_FPSCR_SZ != '\0') {
          fVar18 = (float)((ulonglong)dVar26 >> 0x20);
        }
        if (in_FPSCR_PR == '\0') {
          dVar15 = (double)CONCAT44((int)((ulonglong)dVar15 >> 0x20),
                                    fVar18 * SUB84(dVar17,0) + SUB84(dVar15,0));
        }
        in_xd4 = in_xd2;
        if (in_FPSCR_SZ == '\0') {
          dVar17 = (double)CONCAT44((int)((ulonglong)dVar17 >> 0x20),SUB84(dVar15,0));
          in_xd4 = dVar26;
        }
        if (in_FPSCR_PR != '\0' || SUB84(dVar17,0) <= (float)((ulonglong)dVar17 >> 0x20)) {
LAB_8c052234:
          iVar9 = (int)sVar10;
          sVar10 = sVar10 + 1;
          *(double **)(iVar9 * 4 + iVar8) = pdVar12;
          if (0x3ff < sVar10) break;
        }
      }
      else {
        if (in_FPSCR_SZ == '\0') {
          dVar17 = (double)CONCAT44(*(undefined4 *)pdVar3,SUB84(dVar17,0));
          dVar15 = (double)CONCAT44(uVar16,*(undefined4 *)((int)pdVar12 + 0xc));
          uVar24 = uVar20;
        }
        else {
          dVar17 = *pdVar3;
          in_xd2 = *(double *)((int)pdVar12 + 0xc);
          in_xd6 = dVar14;
        }
        fVar18 = (float)((ulonglong)in_xd6 >> 0x20);
        if (in_FPSCR_PR == '\0') {
          dVar17 = (double)CONCAT44((float)((ulonglong)dVar17 >> 0x20) + SUB84(dVar15,0),
                                    SUB84(dVar17,0));
        }
        else {
          dVar17 = dVar17 + dVar15;
        }
        if (in_FPSCR_SZ == '\0') {
          dVar15 = (double)CONCAT44((int)((ulonglong)dVar15 >> 0x20),*(undefined4 *)pdVar12);
          param_2 = (double)CONCAT44(uVar22,uVar24);
        }
        else {
          in_xd2 = *pdVar12;
          param_2 = dVar21;
        }
        if (in_FPSCR_PR == '\0') {
          param_2 = (double)CONCAT44((int)((ulonglong)param_2 >> 0x20),
                                     SUB84(param_2,0) - SUB84(dVar15,0));
        }
        else {
          param_2 = param_2 - dVar15;
        }
        if (in_FPSCR_SZ == '\0') {
          dVar15 = (double)CONCAT44((int)((ulonglong)dVar15 >> 0x20),
                                    *(undefined4 *)((int)pdVar12 + 4));
          dVar17 = (double)CONCAT44((int)((ulonglong)dVar17 >> 0x20),SUB84(dVar14,0));
          dVar26 = in_xd4;
          dVar25 = in_xd2;
        }
        else {
          dVar25 = *(double *)((int)pdVar12 + 4);
          dVar26 = in_xd8;
        }
        if (in_FPSCR_PR == '\0') {
          param_2 = (double)CONCAT44((float)((ulonglong)param_2 >> 0x20) - SUB84(dVar15,0),
                                     SUB84(param_2,0));
        }
        else {
          param_2 = param_2 - dVar15;
        }
        if (in_FPSCR_SZ == '\0') {
          dVar15 = (double)CONCAT44((int)((ulonglong)dVar15 >> 0x20),*(undefined4 *)(pdVar12 + 1));
        }
        else {
          dVar25 = pdVar12[1];
        }
        if (in_FPSCR_PR == '\0') {
          fVar19 = (float)((ulonglong)dVar17 >> 0x20);
          dVar17 = (double)CONCAT44(fVar19 * fVar19,SUB84(dVar17,0) - SUB84(dVar15,0));
        }
        else {
          dVar17 = dVar17 * dVar17 - dVar15;
        }
        fVar19 = (float)((ulonglong)param_2 >> 0x20);
        in_xd2 = param_2;
        if (in_FPSCR_SZ == '\0') {
          dVar15 = (double)CONCAT44((int)((ulonglong)dVar15 >> 0x20),fVar19);
          in_xd2 = dVar25;
          fVar18 = SUB84(param_2,0);
        }
        if (in_FPSCR_PR == '\0') {
          dVar15 = (double)CONCAT44((int)((ulonglong)dVar15 >> 0x20),
                                    fVar18 * SUB84(param_2,0) + SUB84(dVar15,0) * fVar19);
        }
        else {
          dVar15 = dVar15 * param_2;
        }
        fVar18 = SUB84(dVar17,0);
        if (in_FPSCR_SZ != '\0') {
          fVar18 = (float)((ulonglong)dVar26 >> 0x20);
        }
        if (in_FPSCR_PR == '\0') {
          dVar15 = (double)CONCAT44((int)((ulonglong)dVar15 >> 0x20),
                                    fVar18 * SUB84(dVar17,0) + SUB84(dVar15,0));
        }
        in_xd4 = in_xd2;
        if (in_FPSCR_SZ == '\0') {
          dVar17 = (double)CONCAT44((int)((ulonglong)dVar17 >> 0x20),SUB84(dVar15,0));
          in_xd4 = dVar26;
        }
        if (in_FPSCR_PR != '\0' || SUB84(dVar17,0) <= (float)((ulonglong)dVar17 >> 0x20))
        goto LAB_8c052234;
      }
    }
    pdVar12 = (double *)((int)pdVar12 + 0x24);
  }
  *psVar7 = sVar10;
  return;
}

