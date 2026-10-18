/* Ghidra draft only; not buildable/verified source. */

/* WARNING: Removing unreachable block (ram,0x0c909d20) */
/* WARNING: Removing unreachable block (ram,0x0c909baa) */
/* WARNING: Removing unreachable block (ram,0x0c909d08) */
/* WARNING: Removing unreachable block (ram,0x0c909d34) */
/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_0c909aa0(float param_1,undefined4 param_2,undefined4 param_3,int param_4)

{
  short sVar1;
  float fVar2;
  float fVar3;
  double in_dr0;
  double dVar4;
  undefined4 uVar7;
  undefined8 uVar5;
  double in_dr2;
  double dVar8;
  double dVar9;
  double dVar10;
  undefined8 in_xd0;
  undefined8 in_xd2;
  double in_xd6;
  bool bVar11;
  char in_FPSCR_PR;
  char in_FPSCR_SZ;
  double dVar6;
  
  if (in_FPSCR_SZ == '\0') {
    dVar6 = (double)CONCAT44((undefined4)DAT_0c909c60,param_2);
    param_1 = DAT_0c909c60._4_4_;
  }
  else {
    dVar6 = (double)CONCAT44(DAT_0c909c60._4_4_,(undefined4)DAT_0c909c60);
  }
  sVar1 = *(short *)(param_4 + 0x848);
  dVar10 = (double)(ulonglong)(uint)param_1;
  if (sVar1 == 0) {
    *(undefined2 *)(param_4 + 0x846) = 6;
    if (in_FPSCR_SZ == '\0') {
      *(undefined4 *)(param_4 + 0x854) = 0;
      *(undefined4 *)(param_4 + 0x858) = 0;
      in_dr2 = (double)CONCAT44((int)((ulonglong)in_dr2 >> 0x20),_DAT_0c909c68);
      *(undefined4 *)(param_4 + 0x85c) = _DAT_0c909c68;
      *(undefined4 *)(param_4 + 0x864) = 0;
      *(undefined4 *)(param_4 + 0x86c) = 0;
      *(undefined4 *)(param_4 + 0x870) = 0;
      *(undefined4 *)(param_4 + 0x878) = 0;
      *(undefined4 *)(param_4 + 0x87c) = 0;
    }
    else {
      *(double *)(param_4 + 0x854) = dVar10;
      *(double *)(param_4 + 0x858) = dVar10;
      *(ulonglong *)(param_4 + 0x85c) = CONCAT44(_DAT_0c909c6c,_DAT_0c909c68);
      *(double *)(param_4 + 0x864) = dVar10;
      *(double *)(param_4 + 0x86c) = dVar10;
      *(double *)(param_4 + 0x870) = dVar10;
      *(double *)(param_4 + 0x878) = dVar10;
      *(double *)(param_4 + 0x87c) = dVar10;
    }
    *(short *)(param_4 + 0x848) = *(short *)(param_4 + 0x848) + 1;
  }
  else if (sVar1 == 1) {
    if (10 < *(ushort *)(param_4 + 0x844)) {
      *(short *)(param_4 + 0x848) = *(short *)(param_4 + 0x848) + 1;
    }
  }
  else if (sVar1 == 2) {
    if (in_FPSCR_SZ == '\0') {
      dVar6 = (double)CONCAT44((int)((ulonglong)dVar6 >> 0x20),_DAT_0c909c6c);
      dVar9 = (double)CONCAT44(*(undefined4 *)(param_4 + 0x86c),param_3);
    }
    else {
      in_xd6 = (double)CONCAT44(_DAT_0c909c70,_DAT_0c909c6c);
      dVar9 = *(double *)(param_4 + 0x86c);
    }
    if (in_FPSCR_PR == '\0') {
      dVar9 = (double)CONCAT44((float)((ulonglong)dVar9 >> 0x20) + SUB84(dVar6,0),SUB84(dVar9,0));
    }
    else {
      dVar9 = dVar9 + dVar6;
    }
    if (in_FPSCR_SZ == '\0') {
      fVar2 = *(float *)(param_4 + 0x878);
    }
    else {
      fVar2 = (float)((ulonglong)*(undefined8 *)(param_4 + 0x878) >> 0x20);
    }
    if (in_FPSCR_PR == '\0') {
      dVar8 = (double)CONCAT44(fVar2 * _DAT_0c909c70,_DAT_0c909c70);
    }
    else {
      dVar8 = (double)CONCAT44(fVar2,_DAT_0c909c70) * (double)CONCAT44(fVar2,_DAT_0c909c70);
    }
    if (in_FPSCR_SZ == '\0') {
      fVar2 = *(float *)(param_4 + 0x878);
    }
    else {
      fVar2 = (float)((ulonglong)*(undefined8 *)(param_4 + 0x878) >> 0x20);
    }
    if (in_FPSCR_PR == '\0') {
      fVar3 = (float)((ulonglong)dVar9 >> 0x20) * DAT_0c909c74;
      dVar9 = (double)CONCAT44(fVar3,SUB84(dVar9,0));
      fVar3 = -(float)((ulonglong)dVar8 >> 0x20) - fVar3;
      dVar8 = (double)CONCAT44(fVar3,SUB84(dVar8,0));
      dVar4 = (double)CONCAT44(fVar2 + fVar3,DAT_0c909c74);
    }
    else {
      dVar9 = dVar9 * (double)CONCAT44(fVar2,DAT_0c909c74);
      dVar8 = -dVar8 - dVar9;
      dVar4 = (double)CONCAT44(fVar2,DAT_0c909c74) + dVar8;
    }
    if (in_FPSCR_SZ == '\0') {
      *(int *)(param_4 + 0x878) = (int)((ulonglong)dVar4 >> 0x20);
      dVar4 = (double)CONCAT44(*(undefined4 *)(param_4 + 0x86c),SUB84(dVar4,0));
      dVar8 = (double)CONCAT44(*(undefined4 *)(param_4 + 0x878),SUB84(dVar8,0));
    }
    else {
      *(double *)(param_4 + 0x878) = dVar4;
      dVar4 = *(double *)(param_4 + 0x86c);
      dVar8 = *(double *)(param_4 + 0x878);
    }
    if (in_FPSCR_PR == '\0') {
      dVar4 = (double)CONCAT44((float)((ulonglong)dVar4 >> 0x20) + (float)((ulonglong)dVar8 >> 0x20)
                               ,SUB84(dVar4,0));
    }
    else {
      dVar4 = dVar4 + dVar8;
    }
    if (in_FPSCR_SZ == '\0') {
      in_xd6 = (double)CONCAT44(SUB84(dVar6,0),SUB84(dVar8,0));
      *(int *)(param_4 + 0x86c) = (int)((ulonglong)dVar4 >> 0x20);
      dVar6 = (double)CONCAT44((int)((ulonglong)dVar6 >> 0x20),*(undefined4 *)(param_4 + 0x870));
    }
    else {
      *(double *)(param_4 + 0x86c) = dVar4;
    }
    if (in_FPSCR_PR == '\0') {
      dVar6 = (double)CONCAT44((int)((ulonglong)dVar6 >> 0x20),
                               SUB84(dVar6,0) + (float)((ulonglong)in_xd6 >> 0x20));
    }
    else {
      dVar6 = dVar6 + in_xd6;
    }
    if (in_FPSCR_SZ == '\0') {
      in_xd6 = (double)(ulonglong)*(uint *)(param_4 + 0x87c);
      dVar8 = (double)CONCAT44(*(undefined4 *)(param_4 + 0x87c),SUB84(dVar4,0));
    }
    else {
      in_xd2 = *(undefined8 *)(param_4 + 0x87c);
      dVar8 = *(double *)(param_4 + 0x87c);
    }
    dVar4 = (double)CONCAT44(DAT_0c909c78,SUB84(in_xd6,0));
    if (in_FPSCR_PR == '\0') {
      fVar3 = (float)((ulonglong)dVar6 >> 0x20);
      fVar2 = SUB84(dVar6,0) * fVar3;
      dVar6 = (double)CONCAT44(fVar3,fVar2);
      fVar2 = -((float)((ulonglong)dVar8 >> 0x20) * DAT_0c909c78) - fVar2;
      dVar8 = (double)CONCAT44(fVar2,SUB84(dVar8,0));
      dVar4 = (double)CONCAT44(DAT_0c909c78,SUB84(in_xd6,0) + fVar2);
    }
    else {
      dVar6 = dVar6 * dVar6;
      dVar8 = -(dVar8 * dVar4) - dVar6;
      dVar4 = dVar4 + dVar8;
    }
    if (in_FPSCR_SZ == '\0') {
      *(int *)(param_4 + 0x87c) = SUB84(dVar4,0);
      dVar8 = (double)CONCAT44(*(undefined4 *)(param_4 + 0x870),SUB84(dVar8,0));
      dVar4 = (double)CONCAT44((int)((ulonglong)dVar4 >> 0x20),*(undefined4 *)(param_4 + 0x87c));
    }
    else {
      *(undefined8 *)(param_4 + 0x87c) = in_xd2;
      dVar8 = *(double *)(param_4 + 0x870);
    }
    if (in_FPSCR_PR == '\0') {
      dVar8 = (double)CONCAT44((float)((ulonglong)dVar8 >> 0x20) + SUB84(dVar4,0),SUB84(dVar8,0));
    }
    else {
      dVar8 = dVar8 + dVar4;
    }
    if (in_FPSCR_SZ == '\0') {
      *(int *)(param_4 + 0x870) = (int)((ulonglong)dVar8 >> 0x20);
      dVar4 = (double)CONCAT44((int)((ulonglong)dVar4 >> 0x20),DAT_0c909c7c);
      dVar9 = (double)CONCAT44(*(undefined4 *)(param_4 + 0x858),SUB84(dVar9,0));
      dVar6 = (double)CONCAT44((int)((ulonglong)dVar6 >> 0x20),DAT_0c909c80);
    }
    else {
      *(double *)(param_4 + 0x870) = dVar8;
      dVar9 = *(double *)(param_4 + 0x858);
    }
    if (in_FPSCR_PR == '\0') {
      dVar9 = (double)CONCAT44((float)((ulonglong)dVar9 >> 0x20) + SUB84(dVar4,0),SUB84(dVar9,0));
    }
    else {
      dVar9 = dVar9 + dVar4;
    }
    if (in_FPSCR_SZ == '\0') {
      dVar8 = (double)(ulonglong)*(uint *)(param_4 + 0x864);
    }
    else {
      in_xd0 = *(undefined8 *)(param_4 + 0x864);
    }
    if (in_FPSCR_PR == '\0') {
      dVar9 = (double)CONCAT44((float)((ulonglong)dVar9 >> 0x20) * param_1,SUB84(dVar9,0));
    }
    else {
      dVar9 = dVar9 * dVar10;
    }
    if (in_FPSCR_SZ == '\0') {
      dVar8 = (double)CONCAT44(*(undefined4 *)(param_4 + 0x864),SUB84(dVar8,0));
    }
    else {
      dVar8 = *(double *)(param_4 + 0x864);
    }
    if (in_FPSCR_PR == '\0') {
      dVar8 = (double)CONCAT44((int)((ulonglong)dVar8 >> 0x20),SUB84(dVar8,0) * SUB84(dVar6,0));
    }
    else {
      dVar8 = dVar8 * dVar6;
    }
    fVar2 = (float)((ulonglong)dVar8 >> 0x20);
    if (in_FPSCR_PR == '\0') {
      fVar3 = -SUB84(dVar8,0) - (float)((ulonglong)dVar9 >> 0x20);
      dVar8 = (double)CONCAT44(fVar2 + fVar3,fVar3);
    }
    else {
      dVar8 = (double)CONCAT44(fVar2,-SUB84(dVar8,0)) - dVar9;
      dVar8 = dVar8 + dVar8;
    }
    if (in_FPSCR_SZ == '\0') {
      *(int *)(param_4 + 0x864) = (int)((ulonglong)dVar8 >> 0x20);
      dVar8 = (double)CONCAT44(*(undefined4 *)(param_4 + 0x858),*(undefined4 *)(param_4 + 0x864));
    }
    else {
      *(double *)(param_4 + 0x864) = dVar8;
      dVar8 = *(double *)(param_4 + 0x858);
      in_xd0 = *(undefined8 *)(param_4 + 0x864);
    }
    if (in_FPSCR_PR == '\0') {
      dVar8 = (double)CONCAT44((float)((ulonglong)dVar8 >> 0x20) + SUB84(dVar8,0),SUB84(dVar8,0));
    }
    else {
      dVar8 = dVar8 + dVar8;
    }
    uVar7 = (undefined4)((ulonglong)dVar8 >> 0x20);
    if (in_FPSCR_SZ == '\0') {
      *(undefined4 *)(param_4 + 0x858) = uVar7;
      dVar8 = (double)CONCAT44(uVar7,_DAT_0c909c84);
      dVar9 = (double)CONCAT44(*(undefined4 *)(param_4 + 0x854),SUB84(dVar9,0));
    }
    else {
      *(double *)(param_4 + 0x858) = dVar8;
      in_xd0 = CONCAT44(uRam0c909c88,_DAT_0c909c84);
      dVar9 = *(double *)(param_4 + 0x854);
    }
    if (in_FPSCR_PR == '\0') {
      dVar9 = (double)CONCAT44((float)((ulonglong)dVar9 >> 0x20) + SUB84(dVar8,0),SUB84(dVar9,0));
    }
    else {
      dVar9 = dVar9 + dVar8;
    }
    if (in_FPSCR_SZ == '\0') {
      dVar4 = (double)CONCAT44(*(undefined4 *)(param_4 + 0x864),SUB84(dVar4,0));
      dVar8 = (double)CONCAT44(*(undefined4 *)(param_4 + 0x864),SUB84(dVar8,0));
    }
    else {
      dVar4 = *(double *)(param_4 + 0x864);
      dVar8 = *(double *)(param_4 + 0x864);
    }
    if (in_FPSCR_PR == '\0') {
      fVar2 = -((float)((ulonglong)dVar4 >> 0x20) * SUB84(dVar6,0)) -
              (float)((ulonglong)dVar9 >> 0x20) * param_1;
      dVar10 = (double)CONCAT44(fVar2,SUB84(dVar4,0));
      dVar8 = (double)CONCAT44((float)((ulonglong)dVar8 >> 0x20) + fVar2,SUB84(dVar8,0));
    }
    else {
      dVar10 = -(dVar4 * dVar6) - dVar9 * dVar10;
      dVar8 = dVar8 + dVar10;
    }
    if (in_FPSCR_SZ == '\0') {
      *(int *)(param_4 + 0x864) = (int)((ulonglong)dVar8 >> 0x20);
      dVar9 = (double)CONCAT44(*(undefined4 *)(param_4 + 0x858),SUB84(dVar8,0));
      in_dr2 = (double)CONCAT44(*(undefined4 *)(param_4 + 0x864),SUB84(dVar10,0));
    }
    else {
      *(double *)(param_4 + 0x864) = dVar8;
      dVar9 = *(double *)(param_4 + 0x858);
      in_dr2 = *(double *)(param_4 + 0x864);
    }
    if (in_FPSCR_PR == '\0') {
      in_dr0 = (double)CONCAT44((float)((ulonglong)dVar9 >> 0x20) +
                                (float)((ulonglong)in_dr2 >> 0x20),SUB84(dVar9,0));
    }
    else {
      in_dr0 = dVar9 + in_dr2;
    }
    if (in_FPSCR_SZ == '\0') {
      *(int *)(param_4 + 0x858) = (int)((ulonglong)in_dr0 >> 0x20);
    }
    else {
      *(double *)(param_4 + 0x858) = in_dr0;
    }
    sVar1 = *(short *)(param_4 + 0x814);
    *(short *)(param_4 + 0x814) = sVar1 + -1;
    if (sVar1 == 0) {
      *(short *)(param_4 + 0x848) = *(short *)(param_4 + 0x848) + 1;
    }
  }
  else if (sVar1 == 3) {
    dVar9 = (double)((ulonglong)in_dr2 & 0xffffffff);
    if (in_FPSCR_SZ == '\0') {
      dVar9 = (double)(ulonglong)*(uint *)(param_4 + 0x86c);
    }
    else {
      in_xd2 = *(undefined8 *)(param_4 + 0x86c);
    }
    if (in_FPSCR_PR == '\0') {
      dVar9 = (double)(ulonglong)(uint)(SUB84(dVar9,0) - param_1);
    }
    else {
      dVar9 = dVar9 - dVar10;
    }
    if (in_FPSCR_SZ == '\0') {
      *(float *)(param_4 + 0x86c) = SUB84(dVar9,0);
    }
    else {
      *(undefined8 *)(param_4 + 0x86c) = in_xd2;
    }
    if (in_FPSCR_PR == '\0' && SUB84(dVar9,0) < (float)((ulonglong)dVar9 >> 0x20)) {
      if (in_FPSCR_SZ == '\0') {
        *(undefined4 *)(param_4 + 0x86c) = 0;
      }
      else {
        *(double *)(param_4 + 0x86c) = dVar10;
      }
    }
    dVar9 = (double)((ulonglong)dVar9 & 0xffffffff);
    if (in_FPSCR_SZ == '\0') {
      dVar9 = (double)(ulonglong)*(uint *)(param_4 + 0x870);
    }
    else {
      in_xd2 = *(undefined8 *)(param_4 + 0x870);
    }
    if (in_FPSCR_PR == '\0') {
      in_dr2 = (double)(ulonglong)(uint)(SUB84(dVar9,0) - param_1);
    }
    else {
      in_dr2 = dVar9 - dVar10;
    }
    if (in_FPSCR_SZ == '\0') {
      *(float *)(param_4 + 0x870) = SUB84(in_dr2,0);
    }
    else {
      *(undefined8 *)(param_4 + 0x870) = in_xd2;
    }
    if (in_FPSCR_PR == '\0' && SUB84(in_dr2,0) < (float)((ulonglong)in_dr2 >> 0x20)) {
      if (in_FPSCR_SZ == '\0') {
        *(undefined4 *)(param_4 + 0x870) = 0;
      }
      else {
        *(double *)(param_4 + 0x870) = dVar10;
      }
      *(undefined2 *)(param_4 + 0x812) = 0;
      *(undefined2 *)(param_4 + 0x848) = 0;
      *(undefined2 *)(param_4 + 0x888) = 0;
      *(undefined2 *)(param_4 + 0x814) = 0;
    }
  }
  sVar1 = *(short *)(param_4 + 0x888);
  if (in_FPSCR_PR == '\0') {
    dVar10 = 5.30498947741318e-315;
  }
  else {
    dVar10 = 1.05270884942418e-314;
  }
  fVar2 = SUB84(dVar10,0);
  if (sVar1 == 0) {
    *(undefined2 *)(param_4 + 0x886) = 1;
    if (in_FPSCR_SZ == '\0') {
      *(undefined4 *)(param_4 + 0x894) = 0;
      *(undefined4 *)(param_4 + 0x898) = 0;
      *(undefined4 *)(param_4 + 0x89c) = 0;
    }
    else {
      *(double *)(param_4 + 0x894) = dVar10;
      *(double *)(param_4 + 0x898) = dVar10;
      *(double *)(param_4 + 0x89c) = dVar10;
    }
    if (in_FPSCR_SZ == '\0') {
      *(undefined4 *)(param_4 + 0x8ac) = 0x3f800000;
      *(undefined4 *)(param_4 + 0x8b0) = 0x3f800000;
    }
    else {
      *(ulonglong *)(param_4 + 0x8ac) = CONCAT44(0x3f800000,fVar2);
      *(ulonglong *)(param_4 + 0x8b0) = CONCAT44(0x3f800000,fVar2);
    }
    *(short *)(param_4 + 0x888) = *(short *)(param_4 + 0x888) + 1;
    return;
  }
  uVar7 = (undefined4)((ulonglong)in_dr0 >> 0x20);
  if (sVar1 == 1) {
    if (in_FPSCR_SZ == '\0') {
      fVar3 = *(float *)(param_4 + 0x8ac);
    }
    else {
      fVar3 = (float)((ulonglong)*(undefined8 *)(param_4 + 0x8ac) >> 0x20);
    }
    if (in_FPSCR_PR == '\0') {
      dVar6 = (double)CONCAT44(fVar3 - DAT_0c909d60,DAT_0c909d60);
    }
    else {
      dVar6 = (double)CONCAT44(fVar3,DAT_0c909d60) - (double)CONCAT44(fVar3,DAT_0c909d60);
    }
    if (in_FPSCR_SZ == '\0') {
      *(int *)(param_4 + 0x8ac) = (int)((ulonglong)dVar6 >> 0x20);
      dVar6 = (double)CONCAT44(*(undefined4 *)(param_4 + 0x8ac),SUB84(dVar6,0));
    }
    else {
      *(double *)(param_4 + 0x8ac) = dVar6;
      dVar6 = *(double *)(param_4 + 0x8ac);
    }
    if (in_FPSCR_PR == '\0') {
      dVar10 = (double)CONCAT44(-(float)((ulonglong)dVar6 >> 0x20) + fVar2,SUB84(dVar6,0));
    }
    else {
      dVar10 = -dVar6 + dVar10;
    }
    if (in_FPSCR_SZ == '\0') {
      *(int *)(param_4 + 0x8b0) = (int)((ulonglong)dVar10 >> 0x20);
      dVar6 = (double)CONCAT44(DAT_0c909d64,SUB84(dVar10,0));
      in_dr0 = (double)CONCAT44(uVar7,*(undefined4 *)(param_4 + 0x8ac));
    }
    else {
      *(double *)(param_4 + 0x8b0) = dVar10;
      dVar6 = (double)CONCAT44(_DAT_0c909d68,DAT_0c909d64);
    }
    if (in_FPSCR_PR == '\0') {
      bVar11 = SUB84(in_dr0,0) < (float)((ulonglong)dVar6 >> 0x20);
    }
    else {
      bVar11 = in_dr0 < dVar6;
    }
    if (bVar11) {
      *(short *)(param_4 + 0x888) = *(short *)(param_4 + 0x888) + 1;
      return;
    }
  }
  else {
    if (sVar1 != 2) {
      return;
    }
    if (in_FPSCR_SZ == '\0') {
      dVar10 = (double)CONCAT44(*(undefined4 *)(param_4 + 0x8ac),fVar2);
    }
    else {
      dVar10 = *(double *)(param_4 + 0x8ac);
    }
    if (in_FPSCR_PR == '\0') {
      dVar10 = (double)CONCAT44((float)((ulonglong)dVar10 >> 0x20) + -1.0,SUB84(dVar10,0));
    }
    else {
      dVar10 = dVar10 + (double)CONCAT44((int)((ulonglong)in_dr2 >> 0x20),0xbf800000);
    }
    if (in_FPSCR_SZ == '\0') {
      in_dr0 = (double)CONCAT44(uVar7,*(undefined4 *)(param_4 + 0x8b8));
    }
    else {
      in_xd0 = *(undefined8 *)(param_4 + 0x8b8);
    }
    if (in_FPSCR_PR == '\0') {
      fVar2 = SUB84(in_dr0,0) * _DAT_0c909d68;
    }
    else {
      fVar2 = SUB84(in_dr0 * (double)CONCAT44(_DAT_0c909d68,0xbf800000),0);
    }
    if (in_FPSCR_SZ == '\0') {
      uVar5 = CONCAT44(*(undefined4 *)(param_4 + 0x8b8),fVar2);
    }
    else {
      uVar5 = *(undefined8 *)(param_4 + 0x8b8);
    }
    if (in_FPSCR_PR == '\0') {
      dVar10 = (double)CONCAT44((float)((ulonglong)dVar10 >> 0x20) *
                                (float)((ulonglong)dVar6 >> 0x20),SUB84(dVar10,0));
    }
    else {
      dVar10 = dVar10 * dVar6;
    }
    fVar2 = (float)((ulonglong)uVar5 >> 0x20);
    if (in_FPSCR_PR == '\0') {
      fVar3 = -(float)uVar5 - (float)((ulonglong)dVar10 >> 0x20);
      dVar6 = (double)CONCAT44(fVar2 + fVar3,fVar3);
    }
    else {
      dVar6 = (double)CONCAT44(fVar2,-(float)uVar5) - dVar10;
      dVar6 = dVar6 + dVar6;
    }
    if (in_FPSCR_SZ == '\0') {
      *(int *)(param_4 + 0x8b8) = (int)((ulonglong)dVar6 >> 0x20);
      dVar6 = (double)CONCAT44(*(undefined4 *)(param_4 + 0x8ac),*(undefined4 *)(param_4 + 0x8b8));
    }
    else {
      *(double *)(param_4 + 0x8b8) = dVar6;
      dVar6 = *(double *)(param_4 + 0x8ac);
      in_xd0 = *(undefined8 *)(param_4 + 0x8b8);
    }
    if (in_FPSCR_PR == '\0') {
      dVar6 = (double)CONCAT44((float)((ulonglong)dVar6 >> 0x20) + SUB84(dVar6,0),SUB84(dVar6,0));
    }
    else {
      dVar6 = dVar6 + dVar6;
    }
    uVar7 = (undefined4)((ulonglong)dVar6 >> 0x20);
    if (in_FPSCR_SZ == '\0') {
      *(undefined4 *)(param_4 + 0x8ac) = uVar7;
      dVar6 = (double)CONCAT44(uVar7,*(undefined4 *)(param_4 + 0x8ac));
    }
    else {
      *(double *)(param_4 + 0x8ac) = dVar6;
      in_xd0 = *(undefined8 *)(param_4 + 0x8ac);
    }
    if (in_FPSCR_PR == '\0') {
      fVar2 = -SUB84(dVar6,0) + SUB84(dVar10,0);
    }
    else {
      fVar2 = SUB84((double)CONCAT44((int)((ulonglong)dVar6 >> 0x20),-SUB84(dVar6,0)) + dVar10,0);
    }
    if (in_FPSCR_SZ == '\0') {
      *(float *)(param_4 + 0x8b0) = fVar2;
    }
    else {
      *(undefined8 *)(param_4 + 0x8b0) = in_xd0;
    }
  }
  return;
}

