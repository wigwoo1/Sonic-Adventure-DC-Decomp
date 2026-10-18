/* Ghidra draft only; not buildable/verified source. */

/* WARNING: Removing unreachable block (ram,0x0c90bd9c) */

void callback_candidate_c90bd40(int param_1)

{
  double *pdVar1;
  undefined *puVar2;
  float fVar3;
  int iVar4;
  undefined8 *puVar5;
  float fVar6;
  float fVar7;
  undefined8 in_dr2;
  undefined4 uVar9;
  double dVar8;
  undefined8 in_dr14;
  double dVar10;
  undefined8 in_xd2;
  undefined8 in_xd14;
  undefined4 in_PR;
  char in_FPSCR_PR;
  char in_FPSCR_SZ;
  undefined1 auStack_14 [8];
  undefined8 uStack_c;
  
  puVar2 = PTR_PTR_0c90bf00;
  uVar9 = (undefined4)((ulonglong)in_dr2 >> 0x20);
  if (in_FPSCR_SZ == '\0') {
    uStack_c = CONCAT44((int)in_dr14,(undefined4)uStack_c);
    puVar5 = &uStack_c;
                    /* WARNING: Ignoring partial resolution of indirect */
    uStack_c._0_4_ = (int)((ulonglong)in_dr14 >> 0x20);
  }
  else {
    puVar5 = (undefined8 *)auStack_14;
    uStack_c = in_xd14;
  }
  *(undefined4 *)((int)puVar5 + -4) = in_PR;
  *(undefined2 *)(param_1 + 0x11e) = 0;
  *(undefined2 *)(param_1 + 0x120) = 0;
  *(undefined2 *)(param_1 + 0x122) = 0;
  *(undefined **)(param_1 + 0x138) = puVar2;
  *(undefined2 *)(param_1 + 0x118) =
       *(undefined2 *)(*(short *)(param_1 + 0x11e) * 0x18 + *(int *)(param_1 + 0x138) + 4);
  if (in_FPSCR_SZ == '\0') {
    *(undefined4 *)(param_1 + 0x134) = 0x3f800000;
  }
  else {
    *(undefined8 *)(param_1 + 0x134) = in_xd2;
  }
  iVar4 = *(int *)(param_1 + 0x138) + *(short *)(param_1 + 0x11e) * 0x18;
  if (in_FPSCR_SZ == '\0') {
    pdVar1 = (double *)(iVar4 + 8);
  }
  else {
    in_xd14 = *(undefined8 *)(iVar4 + 8);
    pdVar1 = (double *)(iVar4 + 0xc);
  }
  dVar10 = *pdVar1;
  if (in_FPSCR_PR == '\0') {
    dVar10 = (double)CONCAT44((float)((ulonglong)dVar10 >> 0x20) - SUB84(dVar10,0),SUB84(dVar10,0));
  }
  else {
    dVar10 = dVar10 - dVar10;
  }
  iVar4 = (*DAT_0c90bf04)();
  fVar3 = DAT_0c90bf0c;
  fVar6 = DAT_0c90bf0c;
  if (in_FPSCR_SZ == '\0') {
    fVar6 = DAT_0c90bf08;
  }
  dVar8 = (double)CONCAT44(uVar9,(float)iVar4);
  if (in_FPSCR_PR == '\0') {
    dVar10 = (double)CONCAT44((float)((ulonglong)dVar10 >> 0x20) * (float)iVar4,SUB84(dVar10,0));
  }
  else {
    dVar10 = dVar10 * dVar8;
  }
  if (in_FPSCR_SZ == '\0') {
    dVar8 = (double)((ulonglong)dVar10 & 0xffffffff);
    in_xd14 = in_xd2;
  }
  fVar7 = SUB84(dVar8,0);
  if (in_FPSCR_PR == '\0') {
    fVar7 = fVar6 * (float)((ulonglong)dVar10 >> 0x20) + fVar7;
  }
  if (in_FPSCR_SZ == '\0') {
    *(float *)(param_1 + 0x124) = fVar7;
  }
  else {
    *(undefined8 *)(param_1 + 0x124) = in_xd14;
  }
  *(ushort *)(param_1 + 0x11a) = *(ushort *)(param_1 + 0x11a) & SUB42(fVar3,0);
  return;
}

