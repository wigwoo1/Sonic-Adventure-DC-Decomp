/* Ghidra draft only; not buildable/verified source. */

/* WARNING: Removing unreachable block (ram,0x0c909510) */
/* WARNING: Removing unreachable block (ram,0x0c9094f8) */
/* WARNING: Removing unreachable block (ram,0x0c909524) */
/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_0c9092da(float param_1,undefined4 param_2,int param_3)

{
  short sVar1;
  float fVar2;
  double in_dr0;
  undefined4 uVar5;
  double dVar3;
  float fVar6;
  undefined8 uVar4;
  double in_dr2;
  float fVar9;
  double dVar7;
  float fVar10;
  double dVar8;
  double dVar11;
  undefined4 uVar13;
  double dVar12;
  double dVar14;
  undefined8 in_xd0;
  undefined8 in_xd2;
  bool bVar15;
  char in_FPSCR_PR;
  char in_FPSCR_SZ;
  
  if (in_FPSCR_SZ == '\0') {
    dVar12 = (double)CONCAT44(DAT_0c909368._4_4_,(undefined4)DAT_0c909368);
    param_1 = DAT_0c909370;
  }
  else {
    dVar12 = (double)CONCAT44(DAT_0c909370,DAT_0c909368._4_4_);
  }
  sVar1 = *(short *)(param_3 + 0x848);
  dVar11 = (double)(ulonglong)(uint)param_1;
  uVar13 = (undefined4)((ulonglong)dVar12 >> 0x20);
  if (sVar1 == 0) {
    *(undefined2 *)(param_3 + 0x846) = 3;
    if (in_FPSCR_SZ == '\0') {
      *(undefined4 *)(param_3 + 0x854) = 0;
      *(undefined4 *)(param_3 + 0x858) = 0;
      *(undefined4 *)(param_3 + 0x85c) = _DAT_0c909374;
      *(undefined4 *)(param_3 + 0x864) = 0;
      *(undefined4 *)(param_3 + 0x86c) = uVar13;
      in_dr2 = (double)CONCAT44((int)((ulonglong)in_dr2 >> 0x20),DAT_0c909378);
      *(undefined4 *)(param_3 + 0x870) = DAT_0c909378;
      *(undefined4 *)(param_3 + 0x878) = 0;
      *(undefined4 *)(param_3 + 0x87c) = 0;
    }
    else {
      *(double *)(param_3 + 0x854) = dVar11;
      *(double *)(param_3 + 0x858) = dVar11;
      *(ulonglong *)(param_3 + 0x85c) = CONCAT44(DAT_0c909378,_DAT_0c909374);
      *(double *)(param_3 + 0x864) = dVar11;
      *(double *)(param_3 + 0x86c) = dVar12;
      *(ulonglong *)(param_3 + 0x870) = CONCAT44(uRam0c90937c,DAT_0c909378);
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
  else {
    fVar2 = SUB84(dVar12,0);
    uVar5 = (undefined4)((ulonglong)in_dr0 >> 0x20);
    if (sVar1 == 2) {
      if (in_FPSCR_SZ == '\0') {
        dVar8 = (double)CONCAT44((undefined4)DAT_0c909540,param_2);
        dVar14 = (double)CONCAT44(DAT_0c909540._4_4_,*(undefined4 *)(param_3 + 0x86c));
      }
      else {
        dVar8 = (double)CONCAT44(DAT_0c909540._4_4_,(undefined4)DAT_0c909540);
        dVar14 = (double)CONCAT44(_DAT_0c909548,DAT_0c909540._4_4_);
      }
      fVar10 = (float)((ulonglong)dVar8 >> 0x20);
      if (in_FPSCR_PR == '\0') {
        dVar14 = (double)CONCAT44((int)((ulonglong)dVar14 >> 0x20),SUB84(dVar14,0) + fVar10);
      }
      else {
        dVar14 = dVar14 + dVar8;
      }
      if (in_FPSCR_SZ == '\0') {
        fVar9 = *(float *)(param_3 + 0x878);
      }
      else {
        fVar9 = (float)((ulonglong)*(undefined8 *)(param_3 + 0x878) >> 0x20);
      }
      if (in_FPSCR_PR == '\0') {
        dVar7 = (double)CONCAT44(fVar9 * _DAT_0c909548,_DAT_0c909548);
      }
      else {
        dVar7 = (double)CONCAT44(fVar9,_DAT_0c909548) * (double)CONCAT44(fVar9,_DAT_0c909548);
      }
      if (in_FPSCR_SZ == '\0') {
        in_dr0 = (double)CONCAT44(uVar5,*(undefined4 *)(param_3 + 0x878));
      }
      else {
        in_xd0 = *(undefined8 *)(param_3 + 0x878);
      }
      if (in_FPSCR_PR == '\0') {
        fVar6 = (float)((ulonglong)dVar14 >> 0x20);
        fVar9 = SUB84(dVar14,0) * fVar6;
        dVar14 = (double)CONCAT44(fVar6,fVar9);
        fVar9 = -(float)((ulonglong)dVar7 >> 0x20) - fVar9;
        dVar7 = (double)CONCAT44(fVar9,SUB84(dVar7,0));
        dVar3 = (double)CONCAT44((int)((ulonglong)in_dr0 >> 0x20),SUB84(in_dr0,0) + fVar9);
      }
      else {
        dVar14 = dVar14 * dVar14;
        dVar7 = -dVar7 - dVar14;
        dVar3 = in_dr0 + dVar7;
      }
      if (in_FPSCR_SZ == '\0') {
        *(int *)(param_3 + 0x878) = SUB84(dVar3,0);
        dVar3 = (double)CONCAT44((int)((ulonglong)dVar3 >> 0x20),*(undefined4 *)(param_3 + 0x86c));
        dVar7 = (double)CONCAT44(*(undefined4 *)(param_3 + 0x878),SUB84(dVar7,0));
      }
      else {
        *(undefined8 *)(param_3 + 0x878) = in_xd0;
        in_xd0 = *(undefined8 *)(param_3 + 0x86c);
        dVar7 = *(double *)(param_3 + 0x878);
      }
      if (in_FPSCR_PR == '\0') {
        dVar3 = (double)CONCAT44((int)((ulonglong)dVar3 >> 0x20),
                                 SUB84(dVar3,0) + (float)((ulonglong)dVar7 >> 0x20));
      }
      else {
        dVar3 = dVar3 + dVar7;
      }
      if (in_FPSCR_SZ == '\0') {
        *(int *)(param_3 + 0x86c) = SUB84(dVar3,0);
        dVar14 = (double)CONCAT44((int)((ulonglong)dVar14 >> 0x20),*(undefined4 *)(param_3 + 0x870))
        ;
      }
      else {
        *(undefined8 *)(param_3 + 0x86c) = in_xd0;
      }
      if (in_FPSCR_PR == '\0') {
        dVar14 = (double)CONCAT44((int)((ulonglong)dVar14 >> 0x20),SUB84(dVar14,0) + fVar10);
      }
      else {
        dVar14 = dVar14 + dVar8;
      }
      if (in_FPSCR_SZ == '\0') {
        dVar8 = (double)CONCAT44(*(undefined4 *)(param_3 + 0x87c),SUB84(dVar7,0));
      }
      else {
        dVar8 = *(double *)(param_3 + 0x87c);
      }
      if (in_FPSCR_PR == '\0') {
        dVar14 = (double)CONCAT44((int)((ulonglong)dVar14 >> 0x20),SUB84(dVar14,0) * param_1);
      }
      else {
        dVar14 = dVar14 * dVar11;
      }
      if (in_FPSCR_SZ == '\0') {
        dVar3 = (double)CONCAT44((int)((ulonglong)dVar3 >> 0x20),*(undefined4 *)(param_3 + 0x87c));
      }
      else {
        in_xd0 = *(undefined8 *)(param_3 + 0x87c);
      }
      if (in_FPSCR_PR == '\0') {
        fVar10 = -((float)((ulonglong)dVar8 >> 0x20) * (float)((ulonglong)dVar14 >> 0x20)) -
                 SUB84(dVar14,0);
        dVar8 = (double)CONCAT44(fVar10,SUB84(dVar8,0));
        dVar3 = (double)CONCAT44((int)((ulonglong)dVar3 >> 0x20),SUB84(dVar3,0) + fVar10);
      }
      else {
        dVar8 = -(dVar8 * dVar14) - dVar14;
        dVar3 = dVar3 + dVar8;
      }
      if (in_FPSCR_SZ == '\0') {
        *(int *)(param_3 + 0x87c) = SUB84(dVar3,0);
        dVar3 = (double)CONCAT44((int)((ulonglong)dVar3 >> 0x20),*(undefined4 *)(param_3 + 0x870));
        dVar8 = (double)CONCAT44(*(undefined4 *)(param_3 + 0x87c),SUB84(dVar8,0));
      }
      else {
        *(undefined8 *)(param_3 + 0x87c) = in_xd0;
        in_xd0 = *(undefined8 *)(param_3 + 0x870);
        dVar8 = *(double *)(param_3 + 0x87c);
      }
      fVar10 = DAT_0c909550;
      if (in_FPSCR_PR == '\0') {
        fVar9 = SUB84(dVar3,0) + (float)((ulonglong)dVar8 >> 0x20);
      }
      else {
        fVar9 = SUB84(dVar3 + dVar8,0);
      }
      if (in_FPSCR_SZ == '\0') {
        *(float *)(param_3 + 0x870) = fVar9;
        dVar8 = (double)CONCAT44(DAT_0c90954c,SUB84(dVar8,0));
        dVar14 = (double)CONCAT44(*(undefined4 *)(param_3 + 0x858),SUB84(dVar14,0));
      }
      else {
        *(undefined8 *)(param_3 + 0x870) = in_xd0;
        dVar8 = (double)CONCAT44(DAT_0c909550,DAT_0c90954c);
        dVar14 = *(double *)(param_3 + 0x858);
      }
      fVar9 = (float)((ulonglong)dVar8 >> 0x20);
      if (in_FPSCR_PR == '\0') {
        dVar14 = (double)CONCAT44((float)((ulonglong)dVar14 >> 0x20) + fVar9,SUB84(dVar14,0));
      }
      else {
        dVar14 = dVar14 + dVar8;
      }
      if (in_FPSCR_SZ == '\0') {
        fVar6 = *(float *)(param_3 + 0x864);
      }
      else {
        fVar6 = (float)((ulonglong)*(undefined8 *)(param_3 + 0x864) >> 0x20);
      }
      if (in_FPSCR_PR == '\0') {
        dVar12 = (double)CONCAT44(-(fVar6 * fVar10) - (float)((ulonglong)dVar14 >> 0x20) * fVar2,
                                  fVar10);
      }
      else {
        dVar12 = -((double)CONCAT44(fVar6,fVar10) * (double)CONCAT44(fVar6,fVar10)) -
                 dVar14 * dVar12;
      }
      if (in_FPSCR_SZ == '\0') {
        dVar8 = (double)CONCAT44(fVar9,*(undefined4 *)(param_3 + 0x864));
      }
      else {
        in_xd2 = *(undefined8 *)(param_3 + 0x864);
      }
      if (in_FPSCR_PR == '\0') {
        in_dr2 = (double)CONCAT44((int)((ulonglong)dVar8 >> 0x20),
                                  SUB84(dVar8,0) + (float)((ulonglong)dVar12 >> 0x20));
      }
      else {
        in_dr2 = dVar8 + dVar12;
      }
      if (in_FPSCR_SZ == '\0') {
        *(int *)(param_3 + 0x864) = SUB84(in_dr2,0);
        dVar12 = (double)CONCAT44(*(undefined4 *)(param_3 + 0x858),SUB84(dVar12,0));
        in_dr2 = (double)CONCAT44((int)((ulonglong)in_dr2 >> 0x20),*(undefined4 *)(param_3 + 0x864))
        ;
      }
      else {
        *(undefined8 *)(param_3 + 0x864) = in_xd2;
        dVar12 = *(double *)(param_3 + 0x858);
      }
      if (in_FPSCR_PR == '\0') {
        in_dr0 = (double)CONCAT44((float)((ulonglong)dVar12 >> 0x20) + SUB84(in_dr2,0),
                                  SUB84(dVar12,0));
      }
      else {
        in_dr0 = dVar12 + in_dr2;
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
      if (in_FPSCR_SZ == '\0') {
        fVar10 = *(float *)(param_3 + 0x86c);
      }
      else {
        fVar10 = (float)((ulonglong)*(undefined8 *)(param_3 + 0x86c) >> 0x20);
      }
      if (in_FPSCR_PR == '\0') {
        dVar8 = (double)CONCAT44(fVar10 - DAT_0c909554,DAT_0c909554);
      }
      else {
        dVar8 = (double)CONCAT44(fVar10,DAT_0c909554) - (double)CONCAT44(fVar10,DAT_0c909554);
      }
      if (in_FPSCR_SZ == '\0') {
        *(int *)(param_3 + 0x86c) = (int)((ulonglong)dVar8 >> 0x20);
        in_dr0 = (double)CONCAT44(uVar5,*(undefined4 *)(param_3 + 0x86c));
      }
      else {
        *(double *)(param_3 + 0x86c) = dVar8;
        in_xd0 = *(undefined8 *)(param_3 + 0x86c);
      }
      if (in_FPSCR_PR == '\0') {
        bVar15 = SUB84(in_dr0,0) < 0.0;
      }
      else {
        bVar15 = in_dr0 < (double)((ulonglong)dVar8 & 0xffffffff);
      }
      if (bVar15) {
        if (in_FPSCR_SZ == '\0') {
          *(undefined4 *)(param_3 + 0x86c) = 0;
        }
        else {
          *(double *)(param_3 + 0x86c) = dVar11;
        }
      }
      dVar8 = (double)((ulonglong)dVar8 & 0xffffffff);
      if (in_FPSCR_SZ == '\0') {
        dVar8 = (double)(ulonglong)*(uint *)(param_3 + 0x870);
      }
      else {
        in_xd2 = *(undefined8 *)(param_3 + 0x870);
      }
      if (in_FPSCR_PR == '\0') {
        dVar8 = (double)(ulonglong)(uint)(SUB84(dVar8,0) - fVar2);
      }
      else {
        dVar8 = dVar8 - dVar12;
      }
      if (in_FPSCR_SZ == '\0') {
        *(float *)(param_3 + 0x870) = SUB84(dVar8,0);
      }
      else {
        *(undefined8 *)(param_3 + 0x870) = in_xd2;
      }
      if (in_FPSCR_PR == '\0' && SUB84(dVar8,0) < (float)((ulonglong)dVar8 >> 0x20)) {
        if (in_FPSCR_SZ == '\0') {
          *(undefined4 *)(param_3 + 0x870) = 0;
        }
        else {
          *(double *)(param_3 + 0x870) = dVar11;
        }
      }
      dVar8 = (double)((ulonglong)dVar8 & 0xffffffff);
      if (in_FPSCR_SZ == '\0') {
        dVar8 = (double)(ulonglong)*(uint *)(param_3 + 0x858);
      }
      else {
        in_xd2 = *(undefined8 *)(param_3 + 0x858);
      }
      if (in_FPSCR_PR == '\0') {
        in_dr2 = (double)(ulonglong)(uint)(SUB84(dVar8,0) - fVar2);
      }
      else {
        in_dr2 = dVar8 - dVar12;
      }
      if (in_FPSCR_SZ == '\0') {
        *(float *)(param_3 + 0x858) = SUB84(in_dr2,0);
      }
      else {
        *(undefined8 *)(param_3 + 0x858) = in_xd2;
      }
      if (in_FPSCR_PR == '\0' && SUB84(in_dr2,0) < (float)((ulonglong)in_dr2 >> 0x20)) {
        if (in_FPSCR_SZ == '\0') {
          *(undefined4 *)(param_3 + 0x858) = 0;
        }
        else {
          *(double *)(param_3 + 0x858) = dVar11;
        }
        *(undefined2 *)(param_3 + 0x812) = 0;
        *(undefined2 *)(param_3 + 0x848) = 0;
        *(undefined2 *)(param_3 + 0x888) = 0;
        *(undefined2 *)(param_3 + 0x814) = 0;
      }
    }
  }
  sVar1 = *(short *)(param_3 + 0x888);
  if (in_FPSCR_PR == '\0') {
    dVar12 = (double)CONCAT44(uVar13,0x40000000);
  }
  else {
    dVar12 = (double)CONCAT44(uVar13,0x3f800000) + (double)CONCAT44(uVar13,0x3f800000);
  }
  if (sVar1 == 0) {
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
  if (sVar1 == 1) {
    if (in_FPSCR_SZ == '\0') {
      fVar2 = *(float *)(param_3 + 0x8ac);
    }
    else {
      fVar2 = (float)((ulonglong)*(undefined8 *)(param_3 + 0x8ac) >> 0x20);
    }
    if (in_FPSCR_PR == '\0') {
      dVar11 = (double)CONCAT44(fVar2 - DAT_0c909558,DAT_0c909558);
    }
    else {
      dVar11 = (double)CONCAT44(fVar2,DAT_0c909558) - (double)CONCAT44(fVar2,DAT_0c909558);
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
      dVar11 = (double)CONCAT44(-(float)((ulonglong)dVar11 >> 0x20) + SUB84(dVar12,0),
                                SUB84(dVar11,0));
    }
    else {
      dVar11 = -dVar11 + dVar12;
    }
    if (in_FPSCR_SZ == '\0') {
      *(int *)(param_3 + 0x8b0) = (int)((ulonglong)dVar11 >> 0x20);
      dVar11 = (double)CONCAT44(*(undefined4 *)(param_3 + 0x8ac),SUB84(dVar11,0));
    }
    else {
      *(double *)(param_3 + 0x8b0) = dVar11;
      dVar11 = *(double *)(param_3 + 0x8ac);
    }
    if (in_FPSCR_PR == '\0') {
      bVar15 = (float)((ulonglong)dVar11 >> 0x20) < (float)((ulonglong)dVar12 >> 0x20);
    }
    else {
      bVar15 = dVar11 < dVar12;
    }
    if (bVar15) {
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
      dVar11 = (double)CONCAT44((float)((ulonglong)dVar11 >> 0x20) + -1.0,SUB84(dVar11,0));
    }
    else {
      dVar11 = dVar11 + (double)CONCAT44((int)((ulonglong)in_dr2 >> 0x20),0xbf800000);
    }
    if (in_FPSCR_SZ == '\0') {
      in_dr0 = (double)CONCAT44((int)((ulonglong)in_dr0 >> 0x20),*(undefined4 *)(param_3 + 0x8b8));
    }
    else {
      in_xd0 = *(undefined8 *)(param_3 + 0x8b8);
    }
    if (in_FPSCR_PR == '\0') {
      fVar2 = SUB84(in_dr0,0) * DAT_0c90955c;
    }
    else {
      fVar2 = SUB84(in_dr0 * (double)CONCAT44(DAT_0c90955c,0xbf800000),0);
    }
    if (in_FPSCR_SZ == '\0') {
      uVar4 = CONCAT44(*(undefined4 *)(param_3 + 0x8b8),fVar2);
    }
    else {
      uVar4 = *(undefined8 *)(param_3 + 0x8b8);
    }
    if (in_FPSCR_PR == '\0') {
      dVar11 = (double)CONCAT44((float)((ulonglong)dVar11 >> 0x20) * SUB84(dVar11,0),SUB84(dVar11,0)
                               );
    }
    else {
      dVar11 = dVar11 * dVar11;
    }
    fVar2 = (float)((ulonglong)uVar4 >> 0x20);
    if (in_FPSCR_PR == '\0') {
      fVar10 = -(float)uVar4 - (float)((ulonglong)dVar11 >> 0x20);
      dVar11 = (double)CONCAT44(fVar2 + fVar10,fVar10);
    }
    else {
      dVar11 = (double)CONCAT44(fVar2,-(float)uVar4) - dVar11;
      dVar11 = dVar11 + dVar11;
    }
    if (in_FPSCR_SZ == '\0') {
      *(int *)(param_3 + 0x8b8) = (int)((ulonglong)dVar11 >> 0x20);
      dVar11 = (double)CONCAT44(*(undefined4 *)(param_3 + 0x8ac),*(undefined4 *)(param_3 + 0x8b8));
    }
    else {
      *(double *)(param_3 + 0x8b8) = dVar11;
      dVar11 = *(double *)(param_3 + 0x8ac);
      in_xd0 = *(undefined8 *)(param_3 + 0x8b8);
    }
    if (in_FPSCR_PR == '\0') {
      dVar11 = (double)CONCAT44((float)((ulonglong)dVar11 >> 0x20) + SUB84(dVar11,0),SUB84(dVar11,0)
                               );
    }
    else {
      dVar11 = dVar11 + dVar11;
    }
    uVar13 = (undefined4)((ulonglong)dVar11 >> 0x20);
    if (in_FPSCR_SZ == '\0') {
      *(undefined4 *)(param_3 + 0x8ac) = uVar13;
      dVar11 = (double)CONCAT44(uVar13,*(undefined4 *)(param_3 + 0x8ac));
    }
    else {
      *(double *)(param_3 + 0x8ac) = dVar11;
      in_xd0 = *(undefined8 *)(param_3 + 0x8ac);
    }
    if (in_FPSCR_PR == '\0') {
      fVar2 = -SUB84(dVar11,0) + SUB84(dVar12,0);
    }
    else {
      fVar2 = SUB84((double)CONCAT44((int)((ulonglong)dVar11 >> 0x20),-SUB84(dVar11,0)) + dVar12,0);
    }
    if (in_FPSCR_SZ == '\0') {
      *(float *)(param_3 + 0x8b0) = fVar2;
    }
    else {
      *(undefined8 *)(param_3 + 0x8b0) = in_xd0;
    }
  }
  return;
}

