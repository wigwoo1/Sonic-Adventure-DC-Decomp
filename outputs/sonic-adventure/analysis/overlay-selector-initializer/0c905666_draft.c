/* Ghidra draft only; not buildable/verified source. */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_0c905666(undefined4 param_1,undefined4 param_2,int param_3)

{
  char *pcVar1;
  bool bVar2;
  float fVar3;
  undefined4 uVar4;
  float fVar5;
  double in_dr2;
  double dVar6;
  undefined8 uVar7;
  double dVar8;
  double dVar9;
  undefined8 in_xd2;
  undefined8 in_xd4;
  bool bVar10;
  char in_FPSCR_PR;
  char in_FPSCR_SZ;
  
  pcVar1 = *(char **)(param_3 + 0xec);
  if (in_FPSCR_SZ == '\0') {
    dVar8 = (double)CONCAT44((undefined4)DAT_0c90574c,param_1);
    in_dr2 = (double)CONCAT44((int)((ulonglong)in_dr2 >> 0x20),*(undefined4 *)(pcVar1 + 0x20));
  }
  else {
    dVar8 = (double)CONCAT44(DAT_0c90574c._4_4_,(undefined4)DAT_0c90574c);
    in_xd2 = *(undefined8 *)(pcVar1 + 0x20);
  }
  fVar5 = (float)((ulonglong)dVar8 >> 0x20);
  if (in_FPSCR_PR == '\0') {
    bVar10 = fVar5 < SUB84(in_dr2,0);
  }
  else {
    bVar10 = dVar8 < in_dr2;
  }
  bVar2 = false;
  if (bVar10) {
    if (in_FPSCR_SZ == '\0') {
      *(float *)(pcVar1 + 0x20) = fVar5;
    }
    else {
      *(double *)(pcVar1 + 0x20) = dVar8;
    }
  }
  if (in_FPSCR_SZ == '\0') {
    dVar8 = (double)CONCAT44(fVar5,DAT_0c90574c._4_4_);
    in_dr2 = (double)CONCAT44((int)((ulonglong)in_dr2 >> 0x20),*(undefined4 *)(pcVar1 + 0x20));
  }
  else {
    in_xd4 = CONCAT44(_DAT_0c905754,DAT_0c90574c._4_4_);
    in_xd2 = *(undefined8 *)(pcVar1 + 0x20);
  }
  fVar5 = SUB84(dVar8,0);
  if (in_FPSCR_PR == '\0') {
    bVar10 = SUB84(in_dr2,0) < fVar5;
  }
  else {
    bVar10 = in_dr2 < dVar8;
  }
  if (bVar10) {
    if (in_FPSCR_SZ == '\0') {
      *(float *)(pcVar1 + 0x20) = fVar5;
    }
    else {
      *(undefined8 *)(pcVar1 + 0x20) = in_xd4;
    }
  }
  if (in_FPSCR_SZ == '\0') {
    in_dr2 = (double)CONCAT44((int)((ulonglong)in_dr2 >> 0x20),*(undefined4 *)(pcVar1 + 0x24));
  }
  else {
    in_xd2 = *(undefined8 *)(pcVar1 + 0x24);
  }
  fVar3 = (float)((ulonglong)dVar8 >> 0x20);
  if (in_FPSCR_PR == '\0') {
    bVar10 = fVar3 < SUB84(in_dr2,0);
  }
  else {
    bVar10 = dVar8 < in_dr2;
  }
  if (bVar10) {
    if (in_FPSCR_SZ == '\0') {
      *(float *)(pcVar1 + 0x24) = fVar3;
    }
    else {
      *(double *)(pcVar1 + 0x24) = dVar8;
    }
  }
  if (in_FPSCR_SZ == '\0') {
    in_dr2 = (double)CONCAT44((int)((ulonglong)in_dr2 >> 0x20),*(undefined4 *)(pcVar1 + 0x24));
  }
  else {
    in_xd2 = *(undefined8 *)(pcVar1 + 0x24);
  }
  if (in_FPSCR_PR == '\0') {
    bVar10 = SUB84(in_dr2,0) < fVar5;
  }
  else {
    bVar10 = in_dr2 < dVar8;
  }
  if (bVar10) {
    if (in_FPSCR_SZ == '\0') {
      *(float *)(pcVar1 + 0x24) = fVar5;
    }
    else {
      *(undefined8 *)(pcVar1 + 0x24) = in_xd4;
    }
  }
  if (in_FPSCR_SZ == '\0') {
    dVar8 = (double)CONCAT44(fVar3,_DAT_0c905754);
    dVar6 = (double)CONCAT44(*(undefined4 *)(param_3 + 0xdc),*(undefined4 *)(pcVar1 + 0x20));
  }
  else {
    in_xd2 = *(undefined8 *)(pcVar1 + 0x20);
    dVar6 = *(double *)(param_3 + 0xdc);
  }
  fVar3 = (float)((ulonglong)dVar6 >> 0x20);
  fVar5 = SUB84(dVar8,0);
  if (in_FPSCR_PR == '\0' && SUB84(dVar6,0) < fVar3) {
    bVar2 = true;
    if (in_FPSCR_SZ == '\0') {
      dVar6 = (double)CONCAT44(fVar3,*(undefined4 *)(pcVar1 + 0x20));
    }
    else {
      in_xd2 = *(undefined8 *)(pcVar1 + 0x20);
    }
    if (in_FPSCR_PR == '\0') {
      fVar3 = SUB84(dVar6,0) + fVar5;
    }
    else {
      fVar3 = SUB84(dVar6 + dVar8,0);
    }
    if (in_FPSCR_SZ == '\0') {
      *(float *)(pcVar1 + 0x20) = fVar3;
    }
    else {
      *(undefined8 *)(pcVar1 + 0x20) = in_xd2;
    }
  }
  if (in_FPSCR_SZ == '\0') {
    dVar6 = (double)CONCAT44(*(undefined4 *)(pcVar1 + 0x20),*(undefined4 *)(param_3 + 0xdc));
  }
  else {
    dVar6 = *(double *)(pcVar1 + 0x20);
    in_xd2 = *(undefined8 *)(param_3 + 0xdc);
  }
  fVar3 = (float)((ulonglong)dVar6 >> 0x20);
  if (in_FPSCR_PR == '\0' && SUB84(dVar6,0) < fVar3) {
    bVar2 = true;
    if (in_FPSCR_SZ == '\0') {
      dVar6 = (double)CONCAT44(fVar3,*(undefined4 *)(pcVar1 + 0x20));
    }
    else {
      in_xd2 = *(undefined8 *)(pcVar1 + 0x20);
    }
    if (in_FPSCR_PR == '\0') {
      fVar3 = SUB84(dVar6,0) - fVar5;
    }
    else {
      fVar3 = SUB84(dVar6 - dVar8,0);
    }
    if (in_FPSCR_SZ == '\0') {
      *(float *)(pcVar1 + 0x20) = fVar3;
    }
    else {
      *(undefined8 *)(pcVar1 + 0x20) = in_xd2;
    }
  }
  if (in_FPSCR_SZ == '\0') {
    dVar6 = (double)CONCAT44(*(undefined4 *)(pcVar1 + 0x24),*(undefined4 *)(param_3 + 0xe0));
  }
  else {
    dVar6 = *(double *)(pcVar1 + 0x24);
    in_xd2 = *(undefined8 *)(param_3 + 0xe0);
  }
  fVar3 = (float)((ulonglong)dVar6 >> 0x20);
  if (in_FPSCR_PR == '\0' && fVar3 < SUB84(dVar6,0)) {
    bVar2 = true;
    if (in_FPSCR_SZ == '\0') {
      dVar6 = (double)CONCAT44(fVar3,*(undefined4 *)(pcVar1 + 0x24));
    }
    else {
      in_xd2 = *(undefined8 *)(pcVar1 + 0x24);
    }
    if (in_FPSCR_PR == '\0') {
      fVar3 = SUB84(dVar6,0) + fVar5;
    }
    else {
      fVar3 = SUB84(dVar6 + dVar8,0);
    }
    if (in_FPSCR_SZ == '\0') {
      *(float *)(pcVar1 + 0x24) = fVar3;
    }
    else {
      *(undefined8 *)(pcVar1 + 0x24) = in_xd2;
    }
  }
  if (in_FPSCR_SZ == '\0') {
    dVar6 = (double)CONCAT44(*(undefined4 *)(pcVar1 + 0x24),*(undefined4 *)(param_3 + 0xe0));
  }
  else {
    dVar6 = *(double *)(pcVar1 + 0x24);
    in_xd2 = *(undefined8 *)(param_3 + 0xe0);
  }
  fVar3 = (float)((ulonglong)dVar6 >> 0x20);
  if (in_FPSCR_PR == '\0' && SUB84(dVar6,0) < fVar3) {
    bVar2 = true;
    if (in_FPSCR_SZ == '\0') {
      dVar6 = (double)CONCAT44(fVar3,*(undefined4 *)(pcVar1 + 0x24));
    }
    else {
      in_xd2 = *(undefined8 *)(pcVar1 + 0x24);
    }
    if (in_FPSCR_PR == '\0') {
      dVar6 = (double)CONCAT44((int)((ulonglong)dVar6 >> 0x20),SUB84(dVar6,0) - fVar5);
    }
    else {
      dVar6 = dVar6 - dVar8;
    }
    if (in_FPSCR_SZ == '\0') {
      *(int *)(pcVar1 + 0x24) = SUB84(dVar6,0);
    }
    else {
      *(undefined8 *)(pcVar1 + 0x24) = in_xd2;
    }
  }
  if ((*(ushort *)(pcVar1 + 0x36) & 1) != 0) {
    *(ushort *)(pcVar1 + 0x36) = *(ushort *)(pcVar1 + 0x36) & (ushort)_DAT_0c905758;
    uVar4 = DAT_0c905880;
    if (*pcVar1 == '\0') {
      uVar4 = DAT_0c90575c;
    }
    if (in_FPSCR_SZ == '\0') {
      dVar6 = (double)CONCAT44(*(undefined4 *)(param_3 + 0xe4),uVar4);
    }
    else {
      dVar6 = *(double *)(param_3 + 0xe4);
    }
    if (in_FPSCR_PR == '\0') {
      dVar6 = (double)CONCAT44((float)((ulonglong)dVar6 >> 0x20) + SUB84(dVar6,0),SUB84(dVar6,0));
    }
    else {
      dVar6 = dVar6 + dVar6;
    }
    uVar4 = (undefined4)((ulonglong)dVar6 >> 0x20);
    if (in_FPSCR_SZ == '\0') {
      *(undefined4 *)(param_3 + 0xe4) = uVar4;
      dVar9 = (double)CONCAT44(DAT_0c905884,param_2);
    }
    else {
      *(double *)(param_3 + 0xe4) = dVar6;
      dVar9 = (double)CONCAT44(uRam0c905888,DAT_0c905884);
    }
    fVar3 = (float)((ulonglong)dVar9 >> 0x20);
    if (*(ushort *)(pcVar1 + 0xc) < *(ushort *)(pcVar1 + 0x54)) {
      *(short *)(pcVar1 + 0xc) = *(short *)(pcVar1 + 0xc) + 10;
      if (in_FPSCR_SZ == '\0') {
        dVar6 = (double)CONCAT44(uVar4,*(undefined4 *)(param_3 + 0xe0));
      }
      else {
        in_xd2 = *(undefined8 *)(param_3 + 0xe0);
      }
      if (in_FPSCR_PR == '\0') {
        dVar6 = (double)CONCAT44((int)((ulonglong)dVar6 >> 0x20),SUB84(dVar6,0) - fVar3);
      }
      else {
        dVar6 = dVar6 - dVar9;
      }
      if (in_FPSCR_SZ == '\0') {
        *(int *)(param_3 + 0xe0) = SUB84(dVar6,0);
      }
      else {
        *(undefined8 *)(param_3 + 0xe0) = in_xd2;
      }
    }
    if (*(ushort *)(pcVar1 + 0xe) < *(ushort *)(pcVar1 + 0x56)) {
      *(short *)(pcVar1 + 0xe) = *(short *)(pcVar1 + 0xe) + 10;
      if (in_FPSCR_SZ == '\0') {
        dVar6 = (double)CONCAT44((int)((ulonglong)dVar6 >> 0x20),*(undefined4 *)(param_3 + 0xe0));
      }
      else {
        in_xd2 = *(undefined8 *)(param_3 + 0xe0);
      }
      if (in_FPSCR_PR == '\0') {
        dVar6 = (double)CONCAT44((int)((ulonglong)dVar6 >> 0x20),SUB84(dVar6,0) + fVar3);
      }
      else {
        dVar6 = dVar6 + dVar9;
      }
      if (in_FPSCR_SZ == '\0') {
        *(int *)(param_3 + 0xe0) = SUB84(dVar6,0);
      }
      else {
        *(undefined8 *)(param_3 + 0xe0) = in_xd2;
      }
    }
    if (*(ushort *)(pcVar1 + 0x10) < *(ushort *)(pcVar1 + 0x58)) {
      *(short *)(pcVar1 + 0x10) = *(short *)(pcVar1 + 0x10) + 10;
      if (in_FPSCR_SZ == '\0') {
        dVar6 = (double)CONCAT44((int)((ulonglong)dVar6 >> 0x20),*(undefined4 *)(param_3 + 0xdc));
      }
      else {
        in_xd2 = *(undefined8 *)(param_3 + 0xdc);
      }
      if (in_FPSCR_PR == '\0') {
        dVar6 = (double)CONCAT44((int)((ulonglong)dVar6 >> 0x20),SUB84(dVar6,0) + fVar3);
      }
      else {
        dVar6 = dVar6 + dVar9;
      }
      if (in_FPSCR_SZ == '\0') {
        *(int *)(param_3 + 0xdc) = SUB84(dVar6,0);
      }
      else {
        *(undefined8 *)(param_3 + 0xdc) = in_xd2;
      }
    }
    if (*(ushort *)(pcVar1 + 0x12) < *(ushort *)(pcVar1 + 0x5a)) {
      *(short *)(pcVar1 + 0x12) = *(short *)(pcVar1 + 0x12) + 10;
      if (in_FPSCR_SZ == '\0') {
        dVar6 = (double)CONCAT44((int)((ulonglong)dVar6 >> 0x20),*(undefined4 *)(param_3 + 0xdc));
      }
      else {
        in_xd2 = *(undefined8 *)(param_3 + 0xdc);
      }
      if (in_FPSCR_PR == '\0') {
        dVar6 = (double)CONCAT44((int)((ulonglong)dVar6 >> 0x20),SUB84(dVar6,0) - fVar3);
      }
      else {
        dVar6 = dVar6 - dVar9;
      }
      if (in_FPSCR_SZ == '\0') {
        *(int *)(param_3 + 0xdc) = SUB84(dVar6,0);
      }
      else {
        *(undefined8 *)(param_3 + 0xdc) = in_xd2;
      }
    }
    if (*(ushort *)(pcVar1 + 0x54) < *(ushort *)(pcVar1 + 0xc)) {
      *(undefined2 *)(pcVar1 + 0x54) = *(undefined2 *)(pcVar1 + 0xc);
    }
    if (*(ushort *)(pcVar1 + 0x56) < *(ushort *)(pcVar1 + 0xe)) {
      *(undefined2 *)(pcVar1 + 0x56) = *(undefined2 *)(pcVar1 + 0xe);
    }
    if (*(ushort *)(pcVar1 + 0x58) < *(ushort *)(pcVar1 + 0x10)) {
      *(undefined2 *)(pcVar1 + 0x58) = *(undefined2 *)(pcVar1 + 0x10);
    }
    if (*(ushort *)(pcVar1 + 0x5a) < *(ushort *)(pcVar1 + 0x12)) {
      *(undefined2 *)(pcVar1 + 0x5a) = *(undefined2 *)(pcVar1 + 0x12);
    }
  }
  if (in_FPSCR_SZ == '\0') {
    dVar6 = (double)CONCAT44((int)((ulonglong)dVar6 >> 0x20),*(undefined4 *)(param_3 + 0xe4));
  }
  else {
    in_xd2 = *(undefined8 *)(param_3 + 0xe4);
  }
  fVar3 = (float)((ulonglong)dVar8 >> 0x20);
  if (in_FPSCR_PR == '\0') {
    bVar10 = fVar3 < SUB84(dVar6,0);
  }
  else {
    bVar10 = dVar8 < dVar6;
  }
  if (bVar10) {
    if (in_FPSCR_SZ == '\0') {
      *(float *)(param_3 + 0xe4) = fVar3;
    }
    else {
      *(double *)(param_3 + 0xe4) = dVar8;
    }
  }
  if (in_FPSCR_SZ == '\0') {
    dVar6 = (double)CONCAT44(*(undefined4 *)(param_3 + 0xe4),*(undefined4 *)(pcVar1 + 0x28));
  }
  else {
    in_xd2 = *(undefined8 *)(pcVar1 + 0x28);
    dVar6 = *(double *)(param_3 + 0xe4);
  }
  fVar3 = (float)((ulonglong)dVar6 >> 0x20);
  if (in_FPSCR_PR != '\0' || fVar3 <= SUB84(dVar6,0)) {
    if (in_FPSCR_SZ == '\0') {
      uVar7 = CONCAT44(*(undefined4 *)(param_3 + 0xe4),*(undefined4 *)(pcVar1 + 0x28));
    }
    else {
      uVar7 = *(undefined8 *)(param_3 + 0xe4);
    }
    if (in_FPSCR_PR == '\0' && (float)((ulonglong)uVar7 >> 0x20) < (float)uVar7) {
      if (in_FPSCR_SZ == '\0') {
        *(undefined4 *)(pcVar1 + 0x28) = *(undefined4 *)(param_3 + 0xe4);
      }
      else {
        *(undefined8 *)(pcVar1 + 0x28) = *(undefined8 *)(param_3 + 0xe4);
      }
    }
  }
  else {
    if (in_FPSCR_SZ == '\0') {
      dVar6 = (double)CONCAT44(fVar3,*(undefined4 *)(pcVar1 + 0x28));
    }
    else {
      in_xd2 = *(undefined8 *)(pcVar1 + 0x28);
    }
    if (in_FPSCR_PR == '\0') {
      fVar5 = SUB84(dVar6,0) + fVar5;
    }
    else {
      fVar5 = SUB84(dVar6 + dVar8,0);
    }
    if (in_FPSCR_SZ == '\0') {
      *(float *)(pcVar1 + 0x28) = fVar5;
    }
    else {
      *(undefined8 *)(pcVar1 + 0x28) = in_xd2;
    }
    bVar2 = true;
  }
  if (!bVar2) {
    return;
  }
  callback_candidate_c904e80();
  return;
}

