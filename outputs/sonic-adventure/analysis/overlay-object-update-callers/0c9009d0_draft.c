/* Ghidra draft only; not buildable/verified source. */

/* WARNING: Removing unreachable block (ram,0x0c900ac2) */
/* WARNING: Removing unreachable block (ram,0x0c900a90) */
/* WARNING: Removing unreachable block (ram,0x0c900a76) */
/* WARNING: Removing unreachable block (ram,0x0c900aae) */
/* WARNING: Removing unreachable block (ram,0x0c900ad0) */
/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void callback_candidate_c9009d0(int param_1)

{
  short sVar1;
  code *pcVar2;
  int iVar3;
  int iVar4;
  int iVar5;
  double *pdVar6;
  code *pcVar7;
  float fVar8;
  undefined4 uVar12;
  double dVar9;
  float fVar13;
  double dVar10;
  undefined8 uVar11;
  undefined4 uVar14;
  undefined8 in_dr12;
  uint uVar15;
  double in_dr14;
  double dVar16;
  undefined8 in_xd2;
  undefined8 in_xd14;
  code *pcVar17;
  bool bVar18;
  undefined4 in_PR;
  char in_FPSCR_PR;
  char in_FPSCR_SZ;
  undefined1 auStack_30 [16];
  double dStack_20;
  undefined8 uStack_18;
  
  if (in_FPSCR_SZ == '\0') {
    uStack_18 = CONCAT44(SUB84(in_dr14,0),(undefined4)uStack_18);
                    /* WARNING: Ignoring partial resolution of indirect */
    uStack_18._0_4_ = (int)((ulonglong)in_dr14 >> 0x20);
    pdVar6 = &dStack_20;
    dStack_20 = (double)CONCAT44((int)in_dr12,(int)((ulonglong)in_dr12 >> 0x20));
  }
  else {
    pdVar6 = (double *)auStack_30;
    uStack_18 = in_xd14;
    dStack_20 = in_dr14;
  }
  *(undefined4 *)((int)pdVar6 + -4) = in_PR;
  iVar5 = *(int *)(param_1 + 0x20);
  iVar4 = *(int *)(param_1 + 0x24);
  *(undefined4 *)((int)pdVar6 + -0x14) = *(undefined4 *)(param_1 + 0x2c);
  if (*(int *)PTR_DAT_0c900b08 == 0) {
    sVar1 = *(short *)(iVar4 + 0x5c);
    if (sVar1 == 0) {
      FUN_0c900340(param_1);
    }
    else if (sVar1 == 1) {
      FUN_0c9003c2(param_1);
    }
    else if (sVar1 == 2) {
      FUN_0c9006ee(param_1);
    }
    else if (sVar1 == 3) {
      FUN_0c900912(param_1);
    }
    pcVar7 = (code *)((ulonglong)in_xd14 >> 0x20);
    uVar14 = (undefined4)in_dr12;
    (*(code *)PTR_FUN_0c900b0c)(iVar5,iVar4);
    (*(code *)PTR_FUN_0c900b10)(iVar5,iVar4);
    (*(code *)PTR_FUN_0c900b14)(iVar5,iVar4);
    (*(code *)PTR_FUN_0c900b18)(iVar5,iVar4);
    (*(code *)PTR_callback_candidate_c9058a0_0c900b1c)(iVar5,iVar4);
    pcVar2 = DAT_0c900b20._4_4_;
    if (**(char **)(iVar4 + 0xec) == '\x06') {
      pcVar17 = DAT_0c900b20._4_4_;
      if (in_FPSCR_SZ == '\0') {
        in_dr14 = (double)CONCAT44((int)((ulonglong)in_dr14 >> 0x20),(undefined4)DAT_0c900b20);
        pcVar17 = pcVar7;
      }
      iVar3 = (*DAT_0c900b20._4_4_)();
      uVar12 = DAT_0c900b2c;
      if (in_FPSCR_SZ == '\0') {
        uVar12 = DAT_0c900b28;
      }
      if (in_FPSCR_PR == '\0') {
        dVar9 = (double)CONCAT44(uVar12,(float)iVar3 * SUB84(in_dr14,0));
      }
      else {
        dVar9 = (double)CONCAT44(uVar12,(float)iVar3) * in_dr14;
      }
      if (in_FPSCR_PR == '\0' && SUB84(dVar9,0) < (float)((ulonglong)dVar9 >> 0x20)) {
        if (in_FPSCR_PR == '\0') {
          dVar9 = (double)CONCAT44(0x40000000,uVar14);
        }
        else {
          dVar9 = (double)CONCAT44(0x3f800000,uVar14) + (double)CONCAT44(0x3f800000,uVar14);
        }
        if (in_FPSCR_SZ == '\0') {
          dVar16 = (double)CONCAT44(DAT_0c900b2c,SUB84(in_dr14,0));
        }
        else {
          dVar16 = (double)CONCAT44(_DAT_0c900b30,DAT_0c900b2c);
        }
        iVar3 = (*pcVar2)();
        fVar13 = (float)((ulonglong)dVar16 >> 0x20);
        pcVar7 = pcVar17;
        if (in_FPSCR_SZ == '\0') {
          pcVar7 = SUB84(dVar16,0);
        }
        fVar8 = (float)iVar3;
        if (in_FPSCR_PR == '\0') {
          fVar13 = (float)pcVar7 * fVar8 + fVar13;
        }
        uVar15 = (uint)((ulonglong)dVar9 >> 0x20);
        if (in_FPSCR_SZ == '\0') {
          fVar8 = *(float *)(iVar5 + 0x20);
          dVar10 = (double)((ulonglong)uVar15 << 0x20);
        }
        else {
          in_xd2 = *(undefined8 *)(iVar5 + 0x20);
          dVar10 = dVar9;
        }
        if (in_FPSCR_PR == '\0') {
          fVar8 = (float)((ulonglong)dVar10 >> 0x20) * fVar13 + fVar8;
        }
        if (in_FPSCR_SZ == '\0') {
          *(float *)((int)pdVar6 + -0x10) = fVar8;
          dVar9 = (double)CONCAT44(uVar15,_DAT_0c900b30);
        }
        else {
          *(undefined8 *)((int)pdVar6 + -0x10) = in_xd2;
        }
        iVar3 = (*pcVar2)();
        if (in_FPSCR_SZ == '\0') {
          fVar13 = *(float *)(iVar5 + 0x24);
          pcVar7 = SUB84(dVar16,0);
        }
        else {
          fVar13 = (float)((ulonglong)*(undefined8 *)(iVar5 + 0x24) >> 0x20);
          pcVar7 = pcVar17;
        }
        fVar8 = (float)iVar3;
        if (in_FPSCR_PR == '\0') {
          dVar10 = (double)CONCAT44((float)pcVar7 * fVar8 + fVar13 + SUB84(dVar9,0),fVar8);
        }
        else {
          dVar10 = (double)CONCAT44(fVar13,fVar8) + dVar9;
        }
        if (in_FPSCR_SZ == '\0') {
          *(int *)((int)pdVar6 + -0xc) = (int)((ulonglong)dVar10 >> 0x20);
        }
        else {
          *(double *)((int)pdVar6 + -0xc) = dVar10;
        }
        iVar3 = (*pcVar2)();
        if (in_FPSCR_SZ == '\0') {
          fVar13 = *(float *)(iVar5 + 0x28);
          pcVar17 = SUB84(dVar16,0);
        }
        else {
          fVar13 = (float)((ulonglong)*(undefined8 *)(iVar5 + 0x28) >> 0x20);
        }
        if (in_FPSCR_PR == '\0') {
          fVar8 = (float)pcVar17 * (float)iVar3 + (float)((ulonglong)dVar16 >> 0x20);
          dVar16 = (double)CONCAT44(fVar8,SUB84(dVar16,0));
          fVar13 = (float)((ulonglong)dVar9 >> 0x20) * fVar8 + fVar13;
        }
        uVar11 = CONCAT44(fVar13,(float)iVar3);
        if (in_FPSCR_SZ == '\0') {
          *(float *)((int)pdVar6 + -8) = fVar13;
        }
        else {
          *(undefined8 *)((int)pdVar6 + -8) = uVar11;
        }
        iVar3 = (*pcVar2)();
        if (in_FPSCR_PR == '\0') {
          bVar18 = (float)iVar3 * SUB84(dVar16,0) < SUB84(dVar9,0);
        }
        else {
          bVar18 = (double)CONCAT44((int)((ulonglong)uVar11 >> 0x20),(float)iVar3) * dVar16 < dVar9;
        }
        if (bVar18) {
          (*(code *)PTR_callback_candidate_c906fc0_0c900b34)((undefined1 *)((int)pdVar6 + -0x10));
        }
        else {
          (*(code *)PTR_callback_candidate_c90707c_0c900d00)((undefined1 *)((int)pdVar6 + -0x10));
        }
      }
    }
    (*(code *)PTR_FUN_0c900d04)(iVar5,iVar4);
    *(char *)(iVar5 + 3) = *(char *)(iVar5 + 3) + '\x01';
    *(short *)(iVar5 + 6) = *(short *)(iVar5 + 6) + 1;
  }
  else {
    if (*(short *)(iVar4 + 0x5c) == 3) {
      FUN_0c900912(param_1);
    }
    (*(code *)PTR_callback_candidate_c9058a0_0c900d08)(iVar5,iVar4);
  }
  (*DAT_0c900d0c)(param_1);
  *(uint *)(*(int *)((int)pdVar6 + -0x14) + 4) =
       *(uint *)(*(int *)((int)pdVar6 + -0x14) + 4) & 0xffffffdf;
  return;
}

