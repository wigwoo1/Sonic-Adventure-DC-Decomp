/* Ghidra draft only; not buildable/verified source. */

/* WARNING: Removing unreachable block (ram,0x0c9098c8) */
/* WARNING: Removing unreachable block (ram,0x0c9097ee) */
/* WARNING: Removing unreachable block (ram,0x0c9098fc) */
/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_0c9097e4(float param_1,undefined4 param_2,int param_3)

{
  short sVar1;
  float fVar2;
  float fVar3;
  float fVar4;
  double in_dr0;
  double dVar5;
  undefined8 uVar6;
  undefined4 uVar7;
  double in_dr2;
  double dVar8;
  double dVar9;
  float fVar10;
  double dVar11;
  float fVar12;
  double dVar13;
  undefined8 in_xd0;
  undefined8 in_xd2;
  bool bVar14;
  char in_FPSCR_PR;
  char in_FPSCR_SZ;
  
  fVar12 = (float)DAT_0c909890;
  fVar10 = DAT_0c909890._4_4_;
  if (in_FPSCR_SZ == '\0') {
    fVar12 = 1.0;
    fVar10 = (float)DAT_0c909890;
  }
  fVar12 = -fVar12;
  dVar13 = (double)CONCAT44(fVar10,fVar12);
  if (in_FPSCR_SZ == '\0') {
    param_1 = DAT_0c909890._4_4_;
  }
  sVar1 = *(short *)(param_3 + 0x848);
  dVar11 = (double)(ulonglong)(uint)param_1;
  if (sVar1 == 0) {
    *(undefined2 *)(param_3 + 0x846) = 5;
    if (in_FPSCR_SZ == '\0') {
      *(undefined4 *)(param_3 + 0x854) = 0;
      *(undefined4 *)(param_3 + 0x858) = 0;
      *(undefined4 *)(param_3 + 0x85c) = _DAT_0c909898;
      *(undefined4 *)(param_3 + 0x864) = 0;
      *(undefined4 *)(param_3 + 0x86c) = 0;
      *(undefined4 *)(param_3 + 0x870) = 0;
      *(undefined4 *)(param_3 + 0x878) = 0;
      *(undefined4 *)(param_3 + 0x87c) = 0;
    }
    else {
      *(double *)(param_3 + 0x854) = dVar11;
      *(double *)(param_3 + 0x858) = dVar11;
      *(ulonglong *)(param_3 + 0x85c) = CONCAT44(uRam0c90989c,_DAT_0c909898);
      *(double *)(param_3 + 0x864) = dVar11;
      *(double *)(param_3 + 0x86c) = dVar11;
      *(double *)(param_3 + 0x870) = dVar11;
      *(double *)(param_3 + 0x878) = dVar11;
      *(double *)(param_3 + 0x87c) = dVar11;
    }
    *(short *)(param_3 + 0x848) = *(short *)(param_3 + 0x848) + 1;
  }
  else if (sVar1 == 1) {
    if (10 < *(ushort *)(param_3 + 0x844)) {
      *(short *)(param_3 + 0x848) = *(short *)(param_3 + 0x848) + 1;
    }
  }
  else if (sVar1 == 2) {
    if (in_FPSCR_SZ == '\0') {
      in_dr2 = (double)CONCAT44((int)((ulonglong)in_dr2 >> 0x20),(undefined4)DAT_0c909a60);
      dVar9 = (double)CONCAT44(*(undefined4 *)(param_3 + 0x86c),param_2);
    }
    else {
      in_xd2 = CONCAT44(DAT_0c909a60._4_4_,(undefined4)DAT_0c909a60);
      dVar9 = *(double *)(param_3 + 0x86c);
    }
    if (in_FPSCR_PR == '\0') {
      dVar9 = (double)CONCAT44((float)((ulonglong)dVar9 >> 0x20) + SUB84(in_dr2,0),SUB84(dVar9,0));
    }
    else {
      dVar9 = dVar9 + in_dr2;
    }
    if (in_FPSCR_SZ == '\0') {
      in_dr0 = (double)CONCAT44((int)((ulonglong)in_dr0 >> 0x20),*(undefined4 *)(param_3 + 0x878));
    }
    else {
      in_xd0 = *(undefined8 *)(param_3 + 0x878);
    }
    dVar8 = (double)CONCAT44(DAT_0c909a60._4_4_,SUB84(in_dr2,0));
    if (in_FPSCR_PR == '\0') {
      fVar2 = SUB84(in_dr0,0) * DAT_0c909a60._4_4_;
    }
    else {
      fVar2 = SUB84(in_dr0 * dVar8,0);
    }
    if (in_FPSCR_SZ == '\0') {
      dVar8 = (double)CONCAT44(DAT_0c909a60._4_4_,*(undefined4 *)(param_3 + 0x878));
    }
    else {
      in_xd2 = *(undefined8 *)(param_3 + 0x878);
    }
    if (in_FPSCR_PR == '\0') {
      dVar9 = (double)CONCAT44((float)((ulonglong)dVar9 >> 0x20) * DAT_0c909a68,SUB84(dVar9,0));
    }
    else {
      dVar9 = dVar9 * (double)CONCAT44(DAT_0c909a68,fVar2);
    }
    if (in_FPSCR_PR == '\0') {
      fVar2 = -fVar2 - (float)((ulonglong)dVar9 >> 0x20);
      dVar5 = (double)CONCAT44(DAT_0c909a68,fVar2);
      dVar8 = (double)CONCAT44((int)((ulonglong)dVar8 >> 0x20),SUB84(dVar8,0) + fVar2);
    }
    else {
      dVar5 = (double)CONCAT44(DAT_0c909a68,-fVar2) - dVar9;
      dVar8 = dVar8 + dVar5;
    }
    if (in_FPSCR_SZ == '\0') {
      *(int *)(param_3 + 0x878) = SUB84(dVar8,0);
      dVar5 = (double)CONCAT44((int)((ulonglong)dVar5 >> 0x20),*(undefined4 *)(param_3 + 0x86c));
      dVar8 = (double)CONCAT44((int)((ulonglong)dVar8 >> 0x20),*(undefined4 *)(param_3 + 0x878));
    }
    else {
      *(undefined8 *)(param_3 + 0x878) = in_xd2;
      in_xd0 = *(undefined8 *)(param_3 + 0x86c);
      in_xd2 = *(undefined8 *)(param_3 + 0x878);
    }
    if (in_FPSCR_PR == '\0') {
      dVar5 = (double)CONCAT44((int)((ulonglong)dVar5 >> 0x20),SUB84(dVar5,0) + SUB84(dVar8,0));
    }
    else {
      dVar5 = dVar5 + dVar8;
    }
    if (in_FPSCR_SZ == '\0') {
      *(int *)(param_3 + 0x86c) = SUB84(dVar5,0);
      dVar8 = (double)CONCAT44((int)((ulonglong)dVar8 >> 0x20),DAT_0c909a6c);
      dVar9 = (double)CONCAT44(*(undefined4 *)(param_3 + 0x870),SUB84(dVar9,0));
      fVar2 = DAT_0c909a70;
    }
    else {
      *(undefined8 *)(param_3 + 0x86c) = in_xd0;
      in_xd2 = CONCAT44(DAT_0c909a70,DAT_0c909a6c);
      dVar9 = *(double *)(param_3 + 0x870);
      fVar2 = DAT_0c909a70;
    }
    if (in_FPSCR_PR == '\0') {
      dVar9 = (double)CONCAT44((float)((ulonglong)dVar9 >> 0x20) + SUB84(dVar8,0),SUB84(dVar9,0));
    }
    else {
      dVar9 = dVar9 + dVar8;
    }
    if (in_FPSCR_SZ == '\0') {
      dVar5 = (double)CONCAT44((int)((ulonglong)dVar5 >> 0x20),*(undefined4 *)(param_3 + 0x87c));
    }
    else {
      in_xd0 = *(undefined8 *)(param_3 + 0x87c);
    }
    dVar8 = (double)CONCAT44(fVar2,SUB84(dVar8,0));
    if (in_FPSCR_PR == '\0') {
      fVar3 = SUB84(dVar5,0) * fVar2;
    }
    else {
      fVar3 = SUB84(dVar5 * dVar8,0);
    }
    if (in_FPSCR_SZ == '\0') {
      uVar6 = CONCAT44(*(undefined4 *)(param_3 + 0x87c),fVar3);
    }
    else {
      uVar6 = *(undefined8 *)(param_3 + 0x87c);
    }
    if (in_FPSCR_PR == '\0') {
      dVar9 = (double)CONCAT44((float)((ulonglong)dVar9 >> 0x20) * param_1,SUB84(dVar9,0));
    }
    else {
      dVar9 = dVar9 * dVar11;
    }
    fVar3 = (float)((ulonglong)uVar6 >> 0x20);
    if (in_FPSCR_PR == '\0') {
      fVar4 = -(float)uVar6 - (float)((ulonglong)dVar9 >> 0x20);
      dVar5 = (double)CONCAT44(fVar3 + fVar4,fVar4);
    }
    else {
      dVar5 = (double)CONCAT44(fVar3,-(float)uVar6) - dVar9;
      dVar5 = dVar5 + dVar5;
    }
    DAT_0c909a70 = fVar2;
    if (in_FPSCR_SZ == '\0') {
      *(int *)(param_3 + 0x87c) = (int)((ulonglong)dVar5 >> 0x20);
      dVar5 = (double)CONCAT44(*(undefined4 *)(param_3 + 0x870),*(undefined4 *)(param_3 + 0x87c));
    }
    else {
      *(double *)(param_3 + 0x87c) = dVar5;
      dVar5 = *(double *)(param_3 + 0x870);
      in_xd0 = *(undefined8 *)(param_3 + 0x87c);
    }
    uVar7 = DAT_0c909a74;
    if (in_FPSCR_PR == '\0') {
      dVar5 = (double)CONCAT44((float)((ulonglong)dVar5 >> 0x20) + SUB84(dVar5,0),SUB84(dVar5,0));
    }
    else {
      dVar5 = dVar5 + dVar5;
    }
    if (in_FPSCR_SZ == '\0') {
      *(int *)(param_3 + 0x870) = (int)((ulonglong)dVar5 >> 0x20);
      dVar9 = (double)CONCAT44(*(undefined4 *)(param_3 + 0x858),SUB84(dVar9,0));
    }
    else {
      *(double *)(param_3 + 0x870) = dVar5;
      dVar9 = *(double *)(param_3 + 0x858);
    }
    if (in_FPSCR_PR == '\0') {
      dVar9 = (double)CONCAT44((float)((ulonglong)dVar9 >> 0x20) + fVar12,SUB84(dVar9,0));
    }
    else {
      dVar9 = dVar9 + dVar13;
    }
    if (in_FPSCR_SZ == '\0') {
      dVar5 = (double)CONCAT44(*(undefined4 *)(param_3 + 0x864),uVar7);
    }
    else {
      dVar5 = *(double *)(param_3 + 0x864);
    }
    if (in_FPSCR_PR == '\0') {
      dVar9 = (double)CONCAT44((float)((ulonglong)dVar9 >> 0x20) * fVar10,SUB84(dVar9,0));
    }
    else {
      dVar9 = dVar9 * dVar13;
    }
    if (in_FPSCR_SZ == '\0') {
      dVar8 = (double)CONCAT44(fVar2,*(undefined4 *)(param_3 + 0x864));
    }
    else {
      in_xd2 = *(undefined8 *)(param_3 + 0x864);
    }
    if (in_FPSCR_PR == '\0') {
      fVar10 = -((float)((ulonglong)dVar5 >> 0x20) * SUB84(dVar5,0)) -
               (float)((ulonglong)dVar9 >> 0x20);
      dVar9 = (double)CONCAT44(fVar10,SUB84(dVar5,0));
      dVar8 = (double)CONCAT44((int)((ulonglong)dVar8 >> 0x20),SUB84(dVar8,0) + fVar10);
    }
    else {
      dVar9 = -(dVar5 * dVar5) - dVar9;
      dVar8 = dVar8 + dVar9;
    }
    if (in_FPSCR_SZ == '\0') {
      *(int *)(param_3 + 0x864) = SUB84(dVar8,0);
      dVar13 = (double)CONCAT44(*(undefined4 *)(param_3 + 0x858),SUB84(dVar9,0));
      dVar8 = (double)CONCAT44((int)((ulonglong)dVar8 >> 0x20),*(undefined4 *)(param_3 + 0x864));
    }
    else {
      *(undefined8 *)(param_3 + 0x864) = in_xd2;
      dVar13 = *(double *)(param_3 + 0x858);
    }
    if (in_FPSCR_PR == '\0') {
      in_dr0 = (double)CONCAT44((float)((ulonglong)dVar13 >> 0x20) + SUB84(dVar8,0),SUB84(dVar13,0))
      ;
    }
    else {
      in_dr0 = dVar13 + dVar8;
    }
    if (in_FPSCR_SZ == '\0') {
      *(int *)(param_3 + 0x858) = (int)((ulonglong)in_dr0 >> 0x20);
    }
    else {
      *(double *)(param_3 + 0x858) = in_dr0;
    }
    sVar1 = *(short *)(param_3 + 0x814);
    *(short *)(param_3 + 0x814) = sVar1 + -1;
    if (sVar1 == 0) {
      *(short *)(param_3 + 0x848) = *(short *)(param_3 + 0x848) + 1;
    }
  }
  else if (sVar1 == 3) {
    dVar9 = (double)((ulonglong)in_dr2 & 0xffffffff);
    if (in_FPSCR_SZ == '\0') {
      dVar9 = (double)(ulonglong)*(uint *)(param_3 + 0x86c);
    }
    else {
      in_xd2 = *(undefined8 *)(param_3 + 0x86c);
    }
    if (in_FPSCR_PR == '\0') {
      dVar9 = (double)(ulonglong)(uint)(SUB84(dVar9,0) - fVar10);
    }
    else {
      dVar9 = dVar9 - dVar13;
    }
    if (in_FPSCR_SZ == '\0') {
      *(float *)(param_3 + 0x86c) = SUB84(dVar9,0);
    }
    else {
      *(undefined8 *)(param_3 + 0x86c) = in_xd2;
    }
    if (in_FPSCR_PR == '\0' && SUB84(dVar9,0) < (float)((ulonglong)dVar9 >> 0x20)) {
      if (in_FPSCR_SZ == '\0') {
        *(undefined4 *)(param_3 + 0x86c) = 0;
      }
      else {
        *(double *)(param_3 + 0x86c) = dVar11;
      }
    }
    dVar9 = (double)((ulonglong)dVar9 & 0xffffffff);
    if (in_FPSCR_SZ == '\0') {
      dVar9 = (double)(ulonglong)*(uint *)(param_3 + 0x870);
    }
    else {
      in_xd2 = *(undefined8 *)(param_3 + 0x870);
    }
    if (in_FPSCR_PR == '\0') {
      dVar9 = (double)(ulonglong)(uint)(SUB84(dVar9,0) - fVar10);
    }
    else {
      dVar9 = dVar9 - dVar13;
    }
    if (in_FPSCR_SZ == '\0') {
      *(float *)(param_3 + 0x870) = SUB84(dVar9,0);
    }
    else {
      *(undefined8 *)(param_3 + 0x870) = in_xd2;
    }
    if (in_FPSCR_PR == '\0' && SUB84(dVar9,0) < (float)((ulonglong)dVar9 >> 0x20)) {
      if (in_FPSCR_SZ == '\0') {
        *(undefined4 *)(param_3 + 0x870) = 0;
      }
      else {
        *(double *)(param_3 + 0x870) = dVar11;
      }
      *(undefined2 *)(param_3 + 0x812) = 0;
      *(undefined2 *)(param_3 + 0x848) = 0;
      *(undefined2 *)(param_3 + 0x888) = 0;
      *(undefined2 *)(param_3 + 0x814) = 0;
    }
  }
  sVar1 = *(short *)(param_3 + 0x888);
  if (in_FPSCR_PR == '\0') {
    dVar13 = (double)CONCAT44(0x40000000,fVar12);
  }
  else {
    dVar13 = (double)CONCAT44(0x3f800000,fVar12) + (double)CONCAT44(0x3f800000,fVar12);
  }
  if (sVar1 != 0) {
    fVar12 = (float)((ulonglong)dVar13 >> 0x20);
    uVar7 = (undefined4)((ulonglong)in_dr0 >> 0x20);
    if (sVar1 == 1) {
      if (in_FPSCR_SZ == '\0') {
        fVar10 = *(float *)(param_3 + 0x8ac);
      }
      else {
        fVar10 = (float)((ulonglong)*(undefined8 *)(param_3 + 0x8ac) >> 0x20);
      }
      if (in_FPSCR_PR == '\0') {
        dVar11 = (double)CONCAT44(fVar10 - DAT_0c909a78,DAT_0c909a78);
      }
      else {
        dVar11 = (double)CONCAT44(fVar10,DAT_0c909a78) - (double)CONCAT44(fVar10,DAT_0c909a78);
      }
      if (in_FPSCR_SZ == '\0') {
        *(int *)(param_3 + 0x8ac) = (int)((ulonglong)dVar11 >> 0x20);
        dVar11 = (double)CONCAT44(*(undefined4 *)(param_3 + 0x8ac),SUB84(dVar11,0));
      }
      else {
        *(double *)(param_3 + 0x8ac) = dVar11;
        dVar11 = *(double *)(param_3 + 0x8ac);
      }
      if (in_FPSCR_PR == '\0') {
        dVar13 = (double)CONCAT44(-(float)((ulonglong)dVar11 >> 0x20) + fVar12,SUB84(dVar11,0));
      }
      else {
        dVar13 = -dVar11 + dVar13;
      }
      if (in_FPSCR_SZ == '\0') {
        *(int *)(param_3 + 0x8b0) = (int)((ulonglong)dVar13 >> 0x20);
        dVar13 = (double)CONCAT44(DAT_0c909a7c,SUB84(dVar13,0));
        in_dr0 = (double)CONCAT44(uVar7,*(undefined4 *)(param_3 + 0x8ac));
      }
      else {
        *(double *)(param_3 + 0x8b0) = dVar13;
        dVar13 = (double)CONCAT44(_DAT_0c909a80,DAT_0c909a7c);
      }
      if (in_FPSCR_PR == '\0') {
        bVar14 = SUB84(in_dr0,0) < (float)((ulonglong)dVar13 >> 0x20);
      }
      else {
        bVar14 = in_dr0 < dVar13;
      }
      if (bVar14) {
        *(short *)(param_3 + 0x888) = *(short *)(param_3 + 0x888) + 1;
        return;
      }
    }
    else {
      if (sVar1 != 2) {
        return;
      }
      if (in_FPSCR_SZ == '\0') {
        dVar11 = (double)CONCAT44(*(undefined4 *)(param_3 + 0x8ac),param_1);
      }
      else {
        dVar11 = *(double *)(param_3 + 0x8ac);
      }
      if (in_FPSCR_PR == '\0') {
        dVar11 = (double)CONCAT44((float)((ulonglong)dVar11 >> 0x20) + SUB84(dVar13,0),
                                  SUB84(dVar11,0));
      }
      else {
        dVar11 = dVar11 + dVar13;
      }
      if (in_FPSCR_SZ == '\0') {
        fVar10 = *(float *)(param_3 + 0x8b8);
      }
      else {
        fVar10 = (float)((ulonglong)*(undefined8 *)(param_3 + 0x8b8) >> 0x20);
      }
      if (in_FPSCR_PR == '\0') {
        dVar9 = (double)CONCAT44(fVar10 * _DAT_0c909a80,_DAT_0c909a80);
      }
      else {
        dVar9 = (double)CONCAT44(fVar10,_DAT_0c909a80) * (double)CONCAT44(fVar10,_DAT_0c909a80);
      }
      if (in_FPSCR_SZ == '\0') {
        in_dr0 = (double)CONCAT44(uVar7,*(undefined4 *)(param_3 + 0x8b8));
      }
      else {
        in_xd0 = *(undefined8 *)(param_3 + 0x8b8);
      }
      if (in_FPSCR_PR == '\0') {
        fVar10 = -(float)((ulonglong)dVar9 >> 0x20) -
                 (float)((ulonglong)dVar11 >> 0x20) * SUB84(dVar11,0);
        dVar11 = (double)CONCAT44(fVar10,SUB84(dVar9,0));
        dVar9 = (double)CONCAT44((int)((ulonglong)in_dr0 >> 0x20),SUB84(in_dr0,0) + fVar10);
      }
      else {
        dVar11 = -dVar9 - dVar11 * dVar11;
        dVar9 = in_dr0 + dVar11;
      }
      if (in_FPSCR_SZ == '\0') {
        *(int *)(param_3 + 0x8b8) = SUB84(dVar9,0);
        dVar9 = (double)CONCAT44((int)((ulonglong)dVar9 >> 0x20),*(undefined4 *)(param_3 + 0x8ac));
        dVar11 = (double)CONCAT44(*(undefined4 *)(param_3 + 0x8b8),SUB84(dVar11,0));
      }
      else {
        *(undefined8 *)(param_3 + 0x8b8) = in_xd0;
        in_xd0 = *(undefined8 *)(param_3 + 0x8ac);
        dVar11 = *(double *)(param_3 + 0x8b8);
      }
      if (in_FPSCR_PR == '\0') {
        fVar10 = SUB84(dVar9,0) + (float)((ulonglong)dVar11 >> 0x20);
      }
      else {
        fVar10 = SUB84(dVar9 + dVar11,0);
      }
      if (in_FPSCR_SZ == '\0') {
        *(float *)(param_3 + 0x8ac) = fVar10;
        dVar11 = (double)CONCAT44(*(undefined4 *)(param_3 + 0x8ac),SUB84(dVar11,0));
      }
      else {
        *(undefined8 *)(param_3 + 0x8ac) = in_xd0;
        dVar11 = *(double *)(param_3 + 0x8ac);
      }
      if (in_FPSCR_PR == '\0') {
        dVar13 = (double)CONCAT44(-(float)((ulonglong)dVar11 >> 0x20) + fVar12,SUB84(dVar11,0));
      }
      else {
        dVar13 = -dVar11 + dVar13;
      }
      if (in_FPSCR_SZ == '\0') {
        *(int *)(param_3 + 0x8b0) = (int)((ulonglong)dVar13 >> 0x20);
      }
      else {
        *(double *)(param_3 + 0x8b0) = dVar13;
      }
    }
    return;
  }
  *(undefined2 *)(param_3 + 0x886) = 1;
  if (in_FPSCR_SZ == '\0') {
    *(undefined4 *)(param_3 + 0x894) = 0;
    *(undefined4 *)(param_3 + 0x898) = 0;
    *(undefined4 *)(param_3 + 0x89c) = 0;
  }
  else {
    *(double *)(param_3 + 0x894) = dVar11;
    *(double *)(param_3 + 0x898) = dVar11;
    *(double *)(param_3 + 0x89c) = dVar11;
  }
  if (in_FPSCR_SZ == '\0') {
    *(undefined4 *)(param_3 + 0x8ac) = 0x3f800000;
    *(undefined4 *)(param_3 + 0x8b0) = 0x3f800000;
  }
  else {
    *(ulonglong *)(param_3 + 0x8ac) = CONCAT44(0x3f800000,param_1);
    *(ulonglong *)(param_3 + 0x8b0) = CONCAT44(0x3f800000,param_1);
  }
  *(short *)(param_3 + 0x888) = *(short *)(param_3 + 0x888) + 1;
  return;
}

