/* Ghidra draft only; not buildable/verified source. */

/* WARNING: Removing unreachable block (ram,0x0c9092ba) */
/* WARNING: Removing unreachable block (ram,0x0c90912c) */
/* WARNING: Removing unreachable block (ram,0x0c9092a2) */
/* WARNING: Removing unreachable block (ram,0x0c9092ce) */
/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_0c909054(float param_1,int param_2)

{
  short sVar1;
  float fVar2;
  float fVar3;
  float fVar4;
  float fVar5;
  double in_dr0;
  double dVar6;
  undefined8 uVar7;
  double dVar8;
  undefined4 uVar9;
  double in_dr2;
  double dVar10;
  double dVar11;
  double dVar12;
  undefined8 in_xd0;
  undefined8 in_xd2;
  undefined8 in_xd6;
  bool bVar13;
  char in_FPSCR_PR;
  char in_FPSCR_SZ;
  
  if (in_FPSCR_SZ == '\0') {
    dVar8 = (double)CONCAT44(_DAT_0c9091ac,_DAT_0c9091a8);
    param_1 = DAT_0c9091b0;
  }
  else {
    in_xd6 = CONCAT44(_DAT_0c9091ac,_DAT_0c9091a8);
    dVar8 = (double)CONCAT44(DAT_0c9091b0,_DAT_0c9091ac);
  }
  sVar1 = *(short *)(param_2 + 0x848);
  dVar12 = (double)(ulonglong)(uint)param_1;
  fVar5 = (float)((ulonglong)dVar8 >> 0x20);
  if (sVar1 == 0) {
    *(undefined2 *)(param_2 + 0x846) = 2;
    if (in_FPSCR_SZ == '\0') {
      *(undefined4 *)(param_2 + 0x854) = 0;
      *(undefined4 *)(param_2 + 0x858) = 0;
      *(undefined4 *)(param_2 + 0x85c) = _DAT_0c9091b4;
      *(undefined4 *)(param_2 + 0x864) = 0;
      *(float *)(param_2 + 0x86c) = SUB84(dVar8,0);
      in_dr2 = (double)CONCAT44((int)((ulonglong)in_dr2 >> 0x20),DAT_0c9091b8);
      *(undefined4 *)(param_2 + 0x870) = DAT_0c9091b8;
      *(undefined4 *)(param_2 + 0x878) = 0;
      *(undefined4 *)(param_2 + 0x87c) = 0;
    }
    else {
      *(double *)(param_2 + 0x854) = dVar12;
      *(double *)(param_2 + 0x858) = dVar12;
      *(ulonglong *)(param_2 + 0x85c) = CONCAT44(DAT_0c9091b8,_DAT_0c9091b4);
      *(double *)(param_2 + 0x864) = dVar12;
      *(undefined8 *)(param_2 + 0x86c) = in_xd6;
      *(ulonglong *)(param_2 + 0x870) = CONCAT44(_DAT_0c9091bc,DAT_0c9091b8);
      *(double *)(param_2 + 0x878) = dVar12;
      *(double *)(param_2 + 0x87c) = dVar12;
    }
    *(short *)(param_2 + 0x848) = *(short *)(param_2 + 0x848) + 1;
  }
  else if (sVar1 == 1) {
    if (10 < *(ushort *)(param_2 + 0x844)) {
      *(short *)(param_2 + 0x848) = *(short *)(param_2 + 0x848) + 1;
    }
  }
  else if (sVar1 == 2) {
    if (in_FPSCR_SZ == '\0') {
      dVar11 = (double)CONCAT44(*(undefined4 *)(param_2 + 0x86c),_DAT_0c9091bc);
    }
    else {
      dVar11 = *(double *)(param_2 + 0x86c);
    }
    if (in_FPSCR_PR == '\0') {
      dVar11 = (double)CONCAT44((float)((ulonglong)dVar11 >> 0x20) + SUB84(dVar11,0),SUB84(dVar11,0)
                               );
    }
    else {
      dVar11 = dVar11 + dVar11;
    }
    if (in_FPSCR_SZ == '\0') {
      fVar4 = *(float *)(param_2 + 0x878);
    }
    else {
      fVar4 = (float)((ulonglong)*(undefined8 *)(param_2 + 0x878) >> 0x20);
    }
    if (in_FPSCR_PR == '\0') {
      dVar10 = (double)CONCAT44(fVar4 * _DAT_0c9091c0,_DAT_0c9091c0);
    }
    else {
      dVar10 = (double)CONCAT44(fVar4,_DAT_0c9091c0) * (double)CONCAT44(fVar4,_DAT_0c9091c0);
    }
    if (in_FPSCR_SZ == '\0') {
      in_dr0 = (double)CONCAT44((int)((ulonglong)in_dr0 >> 0x20),*(undefined4 *)(param_2 + 0x878));
    }
    else {
      in_xd0 = *(undefined8 *)(param_2 + 0x878);
    }
    if (in_FPSCR_PR == '\0') {
      fVar4 = (float)((ulonglong)dVar11 >> 0x20) * fVar5;
      dVar11 = (double)CONCAT44(fVar4,SUB84(dVar11,0));
      fVar4 = -(float)((ulonglong)dVar10 >> 0x20) - fVar4;
      dVar10 = (double)CONCAT44(fVar4,SUB84(dVar10,0));
      dVar6 = (double)CONCAT44((int)((ulonglong)in_dr0 >> 0x20),SUB84(in_dr0,0) + fVar4);
    }
    else {
      dVar11 = dVar11 * dVar8;
      dVar10 = -dVar10 - dVar11;
      dVar6 = in_dr0 + dVar10;
    }
    if (in_FPSCR_SZ == '\0') {
      *(int *)(param_2 + 0x878) = SUB84(dVar6,0);
      dVar6 = (double)CONCAT44((int)((ulonglong)dVar6 >> 0x20),*(undefined4 *)(param_2 + 0x86c));
      dVar10 = (double)CONCAT44(*(undefined4 *)(param_2 + 0x878),SUB84(dVar10,0));
    }
    else {
      *(undefined8 *)(param_2 + 0x878) = in_xd0;
      in_xd0 = *(undefined8 *)(param_2 + 0x86c);
      dVar10 = *(double *)(param_2 + 0x878);
    }
    fVar4 = DAT_0c9091c4;
    if (in_FPSCR_PR == '\0') {
      dVar6 = (double)CONCAT44((int)((ulonglong)dVar6 >> 0x20),
                               SUB84(dVar6,0) + (float)((ulonglong)dVar10 >> 0x20));
    }
    else {
      dVar6 = dVar6 + dVar10;
    }
    if (in_FPSCR_SZ == '\0') {
      *(int *)(param_2 + 0x86c) = SUB84(dVar6,0);
      dVar11 = (double)CONCAT44(*(undefined4 *)(param_2 + 0x870),SUB84(dVar11,0));
    }
    else {
      *(undefined8 *)(param_2 + 0x86c) = in_xd0;
      dVar11 = *(double *)(param_2 + 0x870);
    }
    if (in_FPSCR_PR == '\0') {
      dVar11 = (double)CONCAT44((float)((ulonglong)dVar11 >> 0x20) + SUB84(dVar11,0),SUB84(dVar11,0)
                               );
    }
    else {
      dVar11 = dVar11 + dVar11;
    }
    if (in_FPSCR_SZ == '\0') {
      dVar6 = (double)CONCAT44((int)((ulonglong)dVar6 >> 0x20),*(undefined4 *)(param_2 + 0x87c));
    }
    else {
      in_xd0 = *(undefined8 *)(param_2 + 0x87c);
    }
    if (in_FPSCR_PR == '\0') {
      dVar11 = (double)CONCAT44((float)((ulonglong)dVar11 >> 0x20) * fVar5,SUB84(dVar11,0));
    }
    else {
      dVar11 = dVar11 * dVar8;
    }
    if (in_FPSCR_PR == '\0') {
      fVar2 = SUB84(dVar6,0) * fVar4;
    }
    else {
      fVar2 = SUB84(dVar6 * (double)CONCAT44(fVar4,SUB84(dVar10,0)),0);
    }
    if (in_FPSCR_SZ == '\0') {
      uVar7 = CONCAT44(*(undefined4 *)(param_2 + 0x87c),fVar2);
    }
    else {
      uVar7 = *(undefined8 *)(param_2 + 0x87c);
    }
    fVar2 = (float)((ulonglong)uVar7 >> 0x20);
    if (in_FPSCR_PR == '\0') {
      fVar3 = -(float)uVar7 - (float)((ulonglong)dVar11 >> 0x20);
      dVar10 = (double)CONCAT44(fVar2 + fVar3,fVar3);
    }
    else {
      dVar10 = (double)CONCAT44(fVar2,-(float)uVar7) - dVar11;
      dVar10 = dVar10 + dVar10;
    }
    if (in_FPSCR_SZ == '\0') {
      *(int *)(param_2 + 0x87c) = (int)((ulonglong)dVar10 >> 0x20);
      dVar10 = (double)CONCAT44(*(undefined4 *)(param_2 + 0x870),*(undefined4 *)(param_2 + 0x87c));
    }
    else {
      *(double *)(param_2 + 0x87c) = dVar10;
      dVar10 = *(double *)(param_2 + 0x870);
      in_xd0 = *(undefined8 *)(param_2 + 0x87c);
    }
    fVar2 = DAT_0c9091cc;
    if (in_FPSCR_PR == '\0') {
      dVar10 = (double)CONCAT44((float)((ulonglong)dVar10 >> 0x20) + SUB84(dVar10,0),SUB84(dVar10,0)
                               );
    }
    else {
      dVar10 = dVar10 + dVar10;
    }
    if (in_FPSCR_SZ == '\0') {
      *(int *)(param_2 + 0x870) = (int)((ulonglong)dVar10 >> 0x20);
      dVar11 = (double)CONCAT44(*(undefined4 *)(param_2 + 0x858),SUB84(dVar11,0));
      dVar10 = (double)CONCAT44(*(undefined4 *)(param_2 + 0x864),DAT_0c9091c8);
    }
    else {
      *(double *)(param_2 + 0x870) = dVar10;
      in_xd0 = CONCAT44(DAT_0c9091cc,DAT_0c9091c8);
      dVar11 = *(double *)(param_2 + 0x858);
      dVar10 = *(double *)(param_2 + 0x864);
    }
    if (in_FPSCR_PR == '\0') {
      dVar11 = (double)CONCAT44((float)((ulonglong)dVar11 >> 0x20) + SUB84(dVar10,0),SUB84(dVar11,0)
                               );
    }
    else {
      dVar11 = dVar11 + dVar10;
    }
    if (in_FPSCR_PR == '\0') {
      dVar10 = (double)CONCAT44(-((float)((ulonglong)dVar10 >> 0x20) * fVar2),SUB84(dVar10,0));
      dVar11 = (double)CONCAT44((float)((ulonglong)dVar11 >> 0x20) * param_1,SUB84(dVar11,0));
    }
    else {
      dVar10 = -(dVar10 * (double)CONCAT44(fVar4,fVar2));
      dVar11 = dVar11 * dVar12;
    }
    if (in_FPSCR_SZ == '\0') {
      dVar12 = (double)CONCAT44(*(undefined4 *)(param_2 + 0x864),fVar2);
    }
    else {
      dVar12 = *(double *)(param_2 + 0x864);
    }
    if (in_FPSCR_PR == '\0') {
      fVar4 = (float)((ulonglong)dVar10 >> 0x20) - (float)((ulonglong)dVar11 >> 0x20);
      dVar10 = (double)CONCAT44(fVar4,SUB84(dVar10,0));
      dVar12 = (double)CONCAT44((float)((ulonglong)dVar12 >> 0x20) + fVar4,SUB84(dVar12,0));
    }
    else {
      dVar10 = dVar10 - dVar11;
      dVar12 = dVar12 + dVar10;
    }
    if (in_FPSCR_SZ == '\0') {
      *(int *)(param_2 + 0x864) = (int)((ulonglong)dVar12 >> 0x20);
      dVar11 = (double)CONCAT44(*(undefined4 *)(param_2 + 0x858),SUB84(dVar10,0));
      in_dr2 = (double)CONCAT44(*(undefined4 *)(param_2 + 0x864),SUB84(dVar12,0));
    }
    else {
      *(double *)(param_2 + 0x864) = dVar12;
      dVar11 = *(double *)(param_2 + 0x858);
      in_dr2 = *(double *)(param_2 + 0x864);
    }
    if (in_FPSCR_PR == '\0') {
      in_dr0 = (double)CONCAT44((float)((ulonglong)dVar11 >> 0x20) +
                                (float)((ulonglong)in_dr2 >> 0x20),SUB84(dVar11,0));
    }
    else {
      in_dr0 = dVar11 + in_dr2;
    }
    if (in_FPSCR_SZ == '\0') {
      *(int *)(param_2 + 0x858) = (int)((ulonglong)in_dr0 >> 0x20);
    }
    else {
      *(double *)(param_2 + 0x858) = in_dr0;
    }
    sVar1 = *(short *)(param_2 + 0x814);
    *(short *)(param_2 + 0x814) = sVar1 + -1;
    if (sVar1 == 0) {
      *(short *)(param_2 + 0x848) = *(short *)(param_2 + 0x848) + 1;
    }
  }
  else if (sVar1 == 3) {
    dVar11 = (double)((ulonglong)in_dr2 & 0xffffffff);
    if (in_FPSCR_SZ == '\0') {
      dVar11 = (double)(ulonglong)*(uint *)(param_2 + 0x86c);
    }
    else {
      in_xd2 = *(undefined8 *)(param_2 + 0x86c);
    }
    if (in_FPSCR_PR == '\0') {
      dVar11 = (double)(ulonglong)(uint)(SUB84(dVar11,0) - param_1);
    }
    else {
      dVar11 = dVar11 - dVar12;
    }
    if (in_FPSCR_SZ == '\0') {
      *(float *)(param_2 + 0x86c) = SUB84(dVar11,0);
    }
    else {
      *(undefined8 *)(param_2 + 0x86c) = in_xd2;
    }
    if (in_FPSCR_PR == '\0' && SUB84(dVar11,0) < (float)((ulonglong)dVar11 >> 0x20)) {
      if (in_FPSCR_SZ == '\0') {
        *(undefined4 *)(param_2 + 0x86c) = 0;
      }
      else {
        *(double *)(param_2 + 0x86c) = dVar12;
      }
    }
    dVar11 = (double)((ulonglong)dVar11 & 0xffffffff);
    if (in_FPSCR_SZ == '\0') {
      dVar11 = (double)(ulonglong)*(uint *)(param_2 + 0x870);
    }
    else {
      in_xd2 = *(undefined8 *)(param_2 + 0x870);
    }
    if (in_FPSCR_PR == '\0') {
      dVar11 = (double)(ulonglong)(uint)(SUB84(dVar11,0) - param_1);
    }
    else {
      dVar11 = dVar11 - dVar12;
    }
    if (in_FPSCR_SZ == '\0') {
      *(float *)(param_2 + 0x870) = SUB84(dVar11,0);
    }
    else {
      *(undefined8 *)(param_2 + 0x870) = in_xd2;
    }
    if (in_FPSCR_PR == '\0' && SUB84(dVar11,0) < (float)((ulonglong)dVar11 >> 0x20)) {
      if (in_FPSCR_SZ == '\0') {
        *(undefined4 *)(param_2 + 0x870) = 0;
      }
      else {
        *(double *)(param_2 + 0x870) = dVar12;
      }
    }
    dVar11 = (double)((ulonglong)dVar11 & 0xffffffff);
    if (in_FPSCR_SZ == '\0') {
      dVar11 = (double)(ulonglong)*(uint *)(param_2 + 0x858);
    }
    else {
      in_xd2 = *(undefined8 *)(param_2 + 0x858);
    }
    if (in_FPSCR_PR == '\0') {
      in_dr2 = (double)(ulonglong)(uint)(SUB84(dVar11,0) - param_1);
    }
    else {
      in_dr2 = dVar11 - dVar12;
    }
    if (in_FPSCR_SZ == '\0') {
      *(float *)(param_2 + 0x858) = SUB84(in_dr2,0);
    }
    else {
      *(undefined8 *)(param_2 + 0x858) = in_xd2;
    }
    if (in_FPSCR_PR == '\0' && SUB84(in_dr2,0) < (float)((ulonglong)in_dr2 >> 0x20)) {
      if (in_FPSCR_SZ == '\0') {
        *(undefined4 *)(param_2 + 0x858) = 0;
      }
      else {
        *(double *)(param_2 + 0x858) = dVar12;
      }
      *(undefined2 *)(param_2 + 0x812) = 0;
      *(undefined2 *)(param_2 + 0x848) = 0;
      *(undefined2 *)(param_2 + 0x888) = 0;
      *(undefined2 *)(param_2 + 0x814) = 0;
    }
  }
  sVar1 = *(short *)(param_2 + 0x888);
  if (in_FPSCR_PR == '\0') {
    dVar12 = 5.30498947741318e-315;
  }
  else {
    dVar12 = 1.05270884942418e-314;
  }
  fVar4 = SUB84(dVar12,0);
  if (sVar1 == 0) {
    *(undefined2 *)(param_2 + 0x886) = 1;
    if (in_FPSCR_SZ == '\0') {
      *(undefined4 *)(param_2 + 0x894) = 0;
      *(undefined4 *)(param_2 + 0x898) = 0;
      *(undefined4 *)(param_2 + 0x89c) = 0;
    }
    else {
      *(double *)(param_2 + 0x894) = dVar12;
      *(double *)(param_2 + 0x898) = dVar12;
      *(double *)(param_2 + 0x89c) = dVar12;
    }
    if (in_FPSCR_SZ == '\0') {
      *(undefined4 *)(param_2 + 0x8ac) = 0x3f800000;
      *(undefined4 *)(param_2 + 0x8b0) = 0x3f800000;
    }
    else {
      *(ulonglong *)(param_2 + 0x8ac) = CONCAT44(0x3f800000,fVar4);
      *(ulonglong *)(param_2 + 0x8b0) = CONCAT44(0x3f800000,fVar4);
    }
    *(short *)(param_2 + 0x888) = *(short *)(param_2 + 0x888) + 1;
    return;
  }
  if (sVar1 == 1) {
    if (in_FPSCR_SZ == '\0') {
      fVar5 = *(float *)(param_2 + 0x8ac);
    }
    else {
      fVar5 = (float)((ulonglong)*(undefined8 *)(param_2 + 0x8ac) >> 0x20);
    }
    if (in_FPSCR_PR == '\0') {
      dVar11 = (double)CONCAT44(fVar5 - DAT_0c909360,DAT_0c909360);
    }
    else {
      dVar11 = (double)CONCAT44(fVar5,DAT_0c909360) - (double)CONCAT44(fVar5,DAT_0c909360);
    }
    if (in_FPSCR_SZ == '\0') {
      *(int *)(param_2 + 0x8ac) = (int)((ulonglong)dVar11 >> 0x20);
      dVar11 = (double)CONCAT44(*(undefined4 *)(param_2 + 0x8ac),SUB84(dVar11,0));
    }
    else {
      *(double *)(param_2 + 0x8ac) = dVar11;
      dVar11 = *(double *)(param_2 + 0x8ac);
    }
    if (in_FPSCR_PR == '\0') {
      dVar12 = (double)CONCAT44(-(float)((ulonglong)dVar11 >> 0x20) + fVar4,SUB84(dVar11,0));
    }
    else {
      dVar12 = -dVar11 + dVar12;
    }
    if (in_FPSCR_SZ == '\0') {
      *(int *)(param_2 + 0x8b0) = (int)((ulonglong)dVar12 >> 0x20);
      dVar12 = (double)CONCAT44(*(undefined4 *)(param_2 + 0x8ac),SUB84(dVar12,0));
    }
    else {
      *(double *)(param_2 + 0x8b0) = dVar12;
      dVar12 = *(double *)(param_2 + 0x8ac);
    }
    if (in_FPSCR_PR == '\0') {
      bVar13 = (float)((ulonglong)dVar12 >> 0x20) < SUB84(dVar8,0);
    }
    else {
      bVar13 = dVar12 < dVar8;
    }
    if (bVar13) {
      *(short *)(param_2 + 0x888) = *(short *)(param_2 + 0x888) + 1;
      return;
    }
  }
  else {
    if (sVar1 != 2) {
      return;
    }
    if (in_FPSCR_SZ == '\0') {
      dVar12 = (double)CONCAT44(*(undefined4 *)(param_2 + 0x8ac),fVar4);
    }
    else {
      dVar12 = *(double *)(param_2 + 0x8ac);
    }
    if (in_FPSCR_PR == '\0') {
      dVar12 = (double)CONCAT44((float)((ulonglong)dVar12 >> 0x20) + -1.0,SUB84(dVar12,0));
    }
    else {
      dVar12 = dVar12 + (double)CONCAT44((int)((ulonglong)in_dr2 >> 0x20),0xbf800000);
    }
    if (in_FPSCR_SZ == '\0') {
      in_dr0 = (double)CONCAT44((int)((ulonglong)in_dr0 >> 0x20),*(undefined4 *)(param_2 + 0x8b8));
    }
    else {
      in_xd0 = *(undefined8 *)(param_2 + 0x8b8);
    }
    if (in_FPSCR_PR == '\0') {
      fVar4 = SUB84(in_dr0,0) * DAT_0c909364;
    }
    else {
      fVar4 = SUB84(in_dr0 * (double)CONCAT44(DAT_0c909364,0xbf800000),0);
    }
    if (in_FPSCR_SZ == '\0') {
      uVar7 = CONCAT44(*(undefined4 *)(param_2 + 0x8b8),fVar4);
    }
    else {
      uVar7 = *(undefined8 *)(param_2 + 0x8b8);
    }
    if (in_FPSCR_PR == '\0') {
      dVar12 = (double)CONCAT44((float)((ulonglong)dVar12 >> 0x20) * fVar5,SUB84(dVar12,0));
    }
    else {
      dVar12 = dVar12 * dVar8;
    }
    fVar5 = (float)((ulonglong)uVar7 >> 0x20);
    if (in_FPSCR_PR == '\0') {
      fVar4 = -(float)uVar7 - (float)((ulonglong)dVar12 >> 0x20);
      dVar8 = (double)CONCAT44(fVar5 + fVar4,fVar4);
    }
    else {
      dVar8 = (double)CONCAT44(fVar5,-(float)uVar7) - dVar12;
      dVar8 = dVar8 + dVar8;
    }
    if (in_FPSCR_SZ == '\0') {
      *(int *)(param_2 + 0x8b8) = (int)((ulonglong)dVar8 >> 0x20);
      dVar8 = (double)CONCAT44(*(undefined4 *)(param_2 + 0x8ac),*(undefined4 *)(param_2 + 0x8b8));
    }
    else {
      *(double *)(param_2 + 0x8b8) = dVar8;
      dVar8 = *(double *)(param_2 + 0x8ac);
      in_xd0 = *(undefined8 *)(param_2 + 0x8b8);
    }
    if (in_FPSCR_PR == '\0') {
      dVar8 = (double)CONCAT44((float)((ulonglong)dVar8 >> 0x20) + SUB84(dVar8,0),SUB84(dVar8,0));
    }
    else {
      dVar8 = dVar8 + dVar8;
    }
    uVar9 = (undefined4)((ulonglong)dVar8 >> 0x20);
    if (in_FPSCR_SZ == '\0') {
      *(undefined4 *)(param_2 + 0x8ac) = uVar9;
      dVar8 = (double)CONCAT44(uVar9,*(undefined4 *)(param_2 + 0x8ac));
    }
    else {
      *(double *)(param_2 + 0x8ac) = dVar8;
      in_xd0 = *(undefined8 *)(param_2 + 0x8ac);
    }
    if (in_FPSCR_PR == '\0') {
      fVar5 = -SUB84(dVar8,0) + SUB84(dVar12,0);
    }
    else {
      fVar5 = SUB84((double)CONCAT44((int)((ulonglong)dVar8 >> 0x20),-SUB84(dVar8,0)) + dVar12,0);
    }
    if (in_FPSCR_SZ == '\0') {
      *(float *)(param_2 + 0x8b0) = fVar5;
    }
    else {
      *(undefined8 *)(param_2 + 0x8b0) = in_xd0;
    }
  }
  return;
}

