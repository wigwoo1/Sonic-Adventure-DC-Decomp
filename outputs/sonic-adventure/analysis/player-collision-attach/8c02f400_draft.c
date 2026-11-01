/* Ghidra draft only; not buildable/verified source. */

void FUN_8c02f400(double param_1,int param_2)

{
  undefined *puVar1;
  undefined *puVar2;
  uint *puVar3;
  uint *puVar4;
  undefined8 *puVar5;
  double *pdVar6;
  double *pdVar7;
  undefined8 uVar8;
  double dVar9;
  undefined4 uVar10;
  undefined4 uVar11;
  float fVar12;
  undefined8 in_dr12;
  uint uVar14;
  double dVar13;
  undefined8 in_dr14;
  double dVar15;
  double in_xd12;
  undefined8 in_xd14;
  bool bVar16;
  undefined4 in_PR;
  char in_FPSCR_PR;
  char in_FPSCR_SZ;
  undefined1 auStack_38 [16];
  undefined8 uStack_28;
  undefined8 uStack_20;
  
  puVar2 = PTR_FUN_8c02f5a4;
  puVar1 = PTR_FUN_8c02f5a0;
  uVar14 = (uint)((ulonglong)in_dr12 >> 0x20);
  if (in_FPSCR_SZ == '\0') {
    uStack_20 = CONCAT44((int)in_dr14,(undefined4)uStack_20);
                    /* WARNING: Ignoring partial resolution of indirect */
    uStack_20._0_4_ = (int)((ulonglong)in_dr14 >> 0x20);
    puVar5 = &uStack_28;
    uStack_28 = CONCAT44((int)in_dr12,uVar14);
  }
  else {
    puVar5 = (undefined8 *)auStack_38;
    uStack_20 = in_xd14;
    uStack_28 = in_dr14;
  }
  pdVar7 = (double *)((int)puVar5 + -4);
  *(undefined4 *)((int)puVar5 + -4) = in_PR;
  puVar3 = *(uint **)(param_2 + 0xc);
  dVar13 = (double)((ulonglong)uVar14 << 0x20);
  puVar4 = puVar3 + (uint)*(ushort *)(param_2 + 4) * 0xd;
  while (dVar9 = in_xd12, fVar12 = SUB84(dVar13,0), puVar3 < puVar4) {
    uVar10 = SUB84(param_1,0);
    if (in_FPSCR_SZ == '\0') {
      dVar15 = (double)CONCAT44(puVar3[5],puVar3[6]);
      dVar13 = (double)CONCAT44(puVar3[7],fVar12);
    }
    else {
      dVar15 = *(double *)(puVar3 + 5);
      in_xd14 = *(undefined8 *)(puVar3 + 6);
      dVar13 = *(double *)(puVar3 + 7);
    }
    if ((*puVar3 & 0xf) == 3) {
      param_1 = (double)(*(code *)puVar2)(puVar3 + 2);
      if (in_FPSCR_PR == '\0') {
        param_1 = (double)CONCAT44((float)((ulonglong)param_1 >> 0x20) +
                                   (float)((ulonglong)dVar15 >> 0x20),SUB84(param_1,0));
      }
      else {
        param_1 = param_1 + dVar15;
      }
      if (in_FPSCR_SZ == '\0') {
        param_1 = (double)CONCAT44((int)((ulonglong)param_1 >> 0x20),uVar10);
      }
      if (in_FPSCR_PR == '\0') {
        param_1 = (double)CONCAT44((float)((ulonglong)param_1 >> 0x20) + SUB84(dVar15,0),
                                   SUB84(param_1,0));
      }
      else {
        param_1 = param_1 + dVar15;
      }
    }
    else {
      uVar8 = (*(code *)puVar2)(puVar3 + 2);
      uVar11 = (undefined4)in_xd14;
      if (in_FPSCR_SZ == '\0') {
        pdVar6 = (double *)((int)pdVar7 + -4);
        *(int *)((int)pdVar7 + -4) = (int)((ulonglong)uVar8 >> 0x20);
        uVar11 = uVar10;
      }
      else {
        pdVar6 = (double *)((int)pdVar7 + -8);
        *(undefined8 *)((int)pdVar7 + -8) = uVar8;
      }
      dVar15 = (double)(*(code *)puVar1)();
      if (in_FPSCR_SZ == '\0') {
        param_1 = (double)CONCAT44(*(undefined4 *)pdVar6,uVar11);
        pdVar7 = (double *)((int)pdVar6 + 4);
      }
      else {
        param_1 = *pdVar6;
        pdVar7 = pdVar6 + 1;
      }
      if (in_FPSCR_PR == '\0') {
        param_1 = (double)CONCAT44((float)((ulonglong)param_1 >> 0x20) +
                                   (float)((ulonglong)dVar15 >> 0x20),SUB84(param_1,0));
      }
      else {
        param_1 = param_1 + dVar15;
      }
    }
    fVar12 = (float)((ulonglong)param_1 >> 0x20);
    if (in_FPSCR_PR == '\0') {
      bVar16 = SUB84(dVar13,0) < fVar12;
    }
    else {
      bVar16 = dVar13 < param_1;
    }
    puVar3 = puVar3 + 0xd;
    in_xd12 = dVar9;
    if ((bVar16) && (in_xd12 = param_1, in_FPSCR_SZ == '\0')) {
      dVar13 = (double)CONCAT44((int)((ulonglong)dVar13 >> 0x20),fVar12);
      in_xd12 = dVar9;
    }
  }
  if (in_FPSCR_SZ == '\0') {
    dVar9 = (double)CONCAT44(fVar12,DAT_8c02f5a8);
  }
  if (in_FPSCR_PR == '\0') {
    dVar9 = (double)CONCAT44((float)((ulonglong)dVar9 >> 0x20) * SUB84(dVar9,0),SUB84(dVar9,0));
  }
  else {
    dVar9 = dVar9 * dVar9;
  }
  if (in_FPSCR_SZ == '\0') {
    *(int *)(param_2 + 8) = (int)((ulonglong)dVar9 >> 0x20);
    dVar9 = (double)CONCAT44(DAT_8c02f5ac,SUB84(dVar9,0));
  }
  else {
    *(double *)(param_2 + 8) = dVar9;
    dVar9 = (double)CONCAT44(DAT_8c02f5b0,DAT_8c02f5ac);
  }
  if (in_FPSCR_PR == '\0') {
    bVar16 = fVar12 < (float)((ulonglong)dVar9 >> 0x20);
  }
  else {
    bVar16 = dVar13 < dVar9;
  }
  if (bVar16) {
    if (in_FPSCR_SZ == '\0') {
      *(undefined4 *)(param_2 + 8) = DAT_8c02f5b0;
    }
    else {
      *(ulonglong *)(param_2 + 8) = CONCAT44(PTR_FUN_8c02f5b4,DAT_8c02f5b0);
    }
  }
  return;
}

