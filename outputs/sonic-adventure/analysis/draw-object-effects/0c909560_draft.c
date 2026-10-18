/* Ghidra draft only; not buildable/verified source. */

/* WARNING: Removing unreachable block (ram,0x0c909602) */
/* WARNING: Removing unreachable block (ram,0x0c909624) */
/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_0c909560(float param_1,undefined4 param_2,undefined4 param_3,int param_4)

{
  short sVar1;
  double in_dr0;
  undefined4 uVar2;
  float fVar3;
  double in_dr2;
  double dVar4;
  float fVar8;
  undefined4 uVar9;
  double dVar5;
  float fVar10;
  double dVar6;
  double dVar7;
  double dVar12;
  float fVar13;
  undefined8 in_xd0;
  undefined8 in_xd2;
  undefined8 in_xd4;
  bool bVar14;
  char in_FPSCR_PR;
  char in_FPSCR_SZ;
  float fVar11;
  
  if (in_FPSCR_SZ == '\0') {
    dVar12 = (double)CONCAT44((undefined4)DAT_0c909700,param_2);
    dVar7 = (double)CONCAT44(DAT_0c909708,DAT_0c909700._4_4_);
    param_1 = DAT_0c90970c;
  }
  else {
    dVar12 = (double)CONCAT44(DAT_0c909700._4_4_,(undefined4)DAT_0c909700);
    dVar7 = (double)CONCAT44(DAT_0c90970c,DAT_0c909708);
    in_xd4 = CONCAT44(_DAT_0c909710,DAT_0c90970c);
  }
  sVar1 = *(short *)(param_4 + 0x848);
  dVar6 = (double)(ulonglong)(uint)param_1;
  fVar11 = SUB84(dVar7,0);
  fVar13 = (float)((ulonglong)dVar7 >> 0x20);
  if (sVar1 == 0) {
    *(undefined2 *)(param_4 + 0x846) = 4;
    if (in_FPSCR_SZ == '\0') {
      *(undefined4 *)(param_4 + 0x854) = _DAT_0c909710;
      *(undefined4 *)(param_4 + 0x858) = 0;
      *(undefined4 *)(param_4 + 0x85c) = _DAT_0c909714;
      *(undefined4 *)(param_4 + 0x864) = 0;
      *(float *)(param_4 + 0x86c) = param_1;
      in_dr2 = (double)CONCAT44((int)((ulonglong)in_dr2 >> 0x20),DAT_0c909718);
      *(undefined4 *)(param_4 + 0x870) = DAT_0c909718;
      *(undefined4 *)(param_4 + 0x878) = 0;
      *(undefined4 *)(param_4 + 0x87c) = 0;
    }
    else {
      *(ulonglong *)(param_4 + 0x854) = CONCAT44(_DAT_0c909714,_DAT_0c909710);
      *(double *)(param_4 + 0x858) = dVar6;
      *(ulonglong *)(param_4 + 0x85c) = CONCAT44(DAT_0c909718,_DAT_0c909714);
      *(double *)(param_4 + 0x864) = dVar6;
      *(undefined8 *)(param_4 + 0x86c) = in_xd4;
      *(ulonglong *)(param_4 + 0x870) = CONCAT44(_DAT_0c90971c,DAT_0c909718);
      *(double *)(param_4 + 0x878) = dVar6;
      *(double *)(param_4 + 0x87c) = dVar6;
    }
    *(short *)(param_4 + 0x848) = *(short *)(param_4 + 0x848) + 1;
  }
  else if (sVar1 == 1) {
    if (10 < *(ushort *)(param_4 + 0x844)) {
      *(short *)(param_4 + 0x848) = *(short *)(param_4 + 0x848) + 1;
    }
  }
  else {
    fVar10 = (float)((ulonglong)dVar12 >> 0x20);
    uVar2 = (undefined4)((ulonglong)in_dr0 >> 0x20);
    if (sVar1 == 2) {
      if (in_FPSCR_SZ == '\0') {
        dVar5 = (double)CONCAT44(_DAT_0c90971c,param_3);
        dVar12 = (double)CONCAT44(fVar10,*(undefined4 *)(param_4 + 0x86c));
      }
      else {
        dVar5 = (double)CONCAT44(_DAT_0c909720,_DAT_0c90971c);
      }
      fVar10 = (float)((ulonglong)dVar5 >> 0x20);
      if (in_FPSCR_PR == '\0') {
        dVar12 = (double)CONCAT44((int)((ulonglong)dVar12 >> 0x20),SUB84(dVar12,0) + fVar10);
      }
      else {
        dVar12 = dVar12 + dVar5;
      }
      if (in_FPSCR_SZ == '\0') {
        dVar4 = (double)CONCAT44(*(undefined4 *)(param_4 + 0x878),*(undefined4 *)(param_4 + 0x878));
      }
      else {
        dVar4 = *(double *)(param_4 + 0x878);
      }
      if (in_FPSCR_PR == '\0') {
        dVar4 = (double)CONCAT44((int)((ulonglong)dVar4 >> 0x20),SUB84(dVar4,0) * fVar13);
        dVar12 = (double)CONCAT44((int)((ulonglong)dVar12 >> 0x20),SUB84(dVar12,0) * fVar11);
      }
      else {
        dVar4 = dVar4 * dVar7;
        dVar12 = dVar12 * dVar7;
      }
      fVar8 = (float)((ulonglong)dVar4 >> 0x20);
      if (in_FPSCR_PR == '\0') {
        fVar3 = -SUB84(dVar4,0) - SUB84(dVar12,0);
        dVar4 = (double)CONCAT44(fVar8 + fVar3,fVar3);
      }
      else {
        dVar4 = (double)CONCAT44(fVar8,-SUB84(dVar4,0)) - dVar12;
        dVar4 = dVar4 + dVar4;
      }
      if (in_FPSCR_SZ == '\0') {
        *(int *)(param_4 + 0x878) = (int)((ulonglong)dVar4 >> 0x20);
        dVar4 = (double)CONCAT44(*(undefined4 *)(param_4 + 0x86c),*(undefined4 *)(param_4 + 0x878));
      }
      else {
        *(double *)(param_4 + 0x878) = dVar4;
        dVar4 = *(double *)(param_4 + 0x86c);
      }
      if (in_FPSCR_PR == '\0') {
        dVar4 = (double)CONCAT44((float)((ulonglong)dVar4 >> 0x20) + SUB84(dVar4,0),SUB84(dVar4,0));
      }
      else {
        dVar4 = dVar4 + dVar4;
      }
      uVar9 = (undefined4)((ulonglong)dVar4 >> 0x20);
      if (in_FPSCR_SZ == '\0') {
        *(undefined4 *)(param_4 + 0x86c) = uVar9;
        dVar12 = (double)CONCAT44((int)((ulonglong)dVar12 >> 0x20),*(undefined4 *)(param_4 + 0x870))
        ;
        dVar4 = (double)CONCAT44(uVar9,*(undefined4 *)(param_4 + 0x87c));
      }
      else {
        *(double *)(param_4 + 0x86c) = dVar4;
      }
      if (in_FPSCR_PR == '\0') {
        dVar12 = (double)CONCAT44((int)((ulonglong)dVar12 >> 0x20),SUB84(dVar12,0) + fVar10);
        fVar10 = SUB84(dVar4,0) * fVar13;
      }
      else {
        dVar12 = dVar12 + dVar5;
        fVar10 = SUB84(dVar4 * dVar7,0);
      }
      if (in_FPSCR_PR == '\0') {
        dVar12 = (double)CONCAT44((int)((ulonglong)dVar12 >> 0x20),SUB84(dVar12,0) * fVar11);
      }
      else {
        dVar12 = dVar12 * dVar7;
      }
      if (in_FPSCR_SZ == '\0') {
        dVar5 = (double)CONCAT44(*(undefined4 *)(param_4 + 0x87c),-fVar10);
      }
      else {
        dVar5 = *(double *)(param_4 + 0x87c);
      }
      if (in_FPSCR_PR == '\0') {
        fVar10 = SUB84(dVar5,0) - SUB84(dVar12,0);
        dVar5 = (double)CONCAT44((float)((ulonglong)dVar5 >> 0x20) + fVar10,fVar10);
      }
      else {
        dVar5 = (dVar5 - dVar12) + (dVar5 - dVar12);
      }
      if (in_FPSCR_SZ == '\0') {
        *(int *)(param_4 + 0x87c) = (int)((ulonglong)dVar5 >> 0x20);
        dVar5 = (double)CONCAT44(*(undefined4 *)(param_4 + 0x870),*(undefined4 *)(param_4 + 0x87c));
      }
      else {
        *(double *)(param_4 + 0x87c) = dVar5;
        dVar5 = *(double *)(param_4 + 0x870);
      }
      if (in_FPSCR_PR == '\0') {
        dVar5 = (double)CONCAT44((float)((ulonglong)dVar5 >> 0x20) + SUB84(dVar5,0),SUB84(dVar5,0));
      }
      else {
        dVar5 = dVar5 + dVar5;
      }
      uVar9 = (undefined4)((ulonglong)dVar5 >> 0x20);
      if (in_FPSCR_SZ == '\0') {
        *(undefined4 *)(param_4 + 0x870) = uVar9;
        dVar5 = (double)CONCAT44(uVar9,_DAT_0c909720);
        dVar12 = (double)CONCAT44((int)((ulonglong)dVar12 >> 0x20),*(undefined4 *)(param_4 + 0x858))
        ;
      }
      else {
        *(double *)(param_4 + 0x870) = dVar5;
      }
      if (in_FPSCR_PR == '\0') {
        dVar12 = (double)CONCAT44((int)((ulonglong)dVar12 >> 0x20),SUB84(dVar12,0) + SUB84(dVar5,0))
        ;
      }
      else {
        dVar12 = dVar12 + dVar5;
      }
      if (in_FPSCR_SZ == '\0') {
        dVar5 = (double)CONCAT44(*(undefined4 *)(param_4 + 0x864),SUB84(dVar5,0));
        in_dr0 = (double)CONCAT44(uVar2,*(undefined4 *)(param_4 + 0x864));
      }
      else {
        dVar5 = *(double *)(param_4 + 0x864);
        in_xd0 = *(undefined8 *)(param_4 + 0x864);
      }
      if (in_FPSCR_PR == '\0') {
        fVar10 = -((float)((ulonglong)dVar5 >> 0x20) * param_1) -
                 SUB84(dVar12,0) * (float)((ulonglong)dVar12 >> 0x20);
        dVar12 = (double)CONCAT44(fVar10,SUB84(dVar5,0));
        dVar5 = (double)CONCAT44((int)((ulonglong)in_dr0 >> 0x20),SUB84(in_dr0,0) + fVar10);
      }
      else {
        dVar12 = -(dVar5 * dVar6) - dVar12 * dVar12;
        dVar5 = in_dr0 + dVar12;
      }
      if (in_FPSCR_SZ == '\0') {
        *(int *)(param_4 + 0x864) = SUB84(dVar5,0);
        in_dr2 = (double)CONCAT44(*(undefined4 *)(param_4 + 0x864),SUB84(dVar12,0));
        dVar5 = (double)CONCAT44((int)((ulonglong)dVar5 >> 0x20),*(undefined4 *)(param_4 + 0x858));
      }
      else {
        *(undefined8 *)(param_4 + 0x864) = in_xd0;
        in_dr2 = *(double *)(param_4 + 0x864);
        in_xd0 = *(undefined8 *)(param_4 + 0x858);
      }
      if (in_FPSCR_PR == '\0') {
        in_dr0 = (double)CONCAT44((int)((ulonglong)dVar5 >> 0x20),
                                  SUB84(dVar5,0) + (float)((ulonglong)in_dr2 >> 0x20));
      }
      else {
        in_dr0 = dVar5 + in_dr2;
      }
      if (in_FPSCR_SZ == '\0') {
        *(int *)(param_4 + 0x858) = SUB84(in_dr0,0);
      }
      else {
        *(undefined8 *)(param_4 + 0x858) = in_xd0;
      }
      sVar1 = *(short *)(param_4 + 0x814);
      *(short *)(param_4 + 0x814) = sVar1 + -1;
      if (sVar1 == 0) {
        *(short *)(param_4 + 0x848) = *(short *)(param_4 + 0x848) + 1;
      }
    }
    else if (sVar1 == 3) {
      dVar5 = (double)((ulonglong)in_dr2 & 0xffffffff);
      if (in_FPSCR_SZ == '\0') {
        dVar5 = (double)(ulonglong)*(uint *)(param_4 + 0x86c);
      }
      else {
        in_xd2 = *(undefined8 *)(param_4 + 0x86c);
      }
      if (in_FPSCR_PR == '\0') {
        dVar5 = (double)(ulonglong)(uint)(SUB84(dVar5,0) - fVar10);
      }
      else {
        dVar5 = dVar5 - dVar12;
      }
      if (in_FPSCR_SZ == '\0') {
        *(float *)(param_4 + 0x86c) = SUB84(dVar5,0);
      }
      else {
        *(undefined8 *)(param_4 + 0x86c) = in_xd2;
      }
      if (in_FPSCR_PR == '\0' && SUB84(dVar5,0) < (float)((ulonglong)dVar5 >> 0x20)) {
        if (in_FPSCR_SZ == '\0') {
          *(undefined4 *)(param_4 + 0x86c) = 0;
        }
        else {
          *(double *)(param_4 + 0x86c) = dVar6;
        }
      }
      dVar5 = (double)((ulonglong)dVar5 & 0xffffffff);
      if (in_FPSCR_SZ == '\0') {
        dVar5 = (double)(ulonglong)*(uint *)(param_4 + 0x870);
      }
      else {
        in_xd2 = *(undefined8 *)(param_4 + 0x870);
      }
      if (in_FPSCR_PR == '\0') {
        dVar5 = (double)(ulonglong)(uint)(SUB84(dVar5,0) - fVar10);
      }
      else {
        dVar5 = dVar5 - dVar12;
      }
      if (in_FPSCR_SZ == '\0') {
        *(float *)(param_4 + 0x870) = SUB84(dVar5,0);
      }
      else {
        *(undefined8 *)(param_4 + 0x870) = in_xd2;
      }
      if (in_FPSCR_PR == '\0' && SUB84(dVar5,0) < (float)((ulonglong)dVar5 >> 0x20)) {
        if (in_FPSCR_SZ == '\0') {
          *(undefined4 *)(param_4 + 0x870) = 0;
        }
        else {
          *(double *)(param_4 + 0x870) = dVar6;
        }
      }
      if (in_FPSCR_SZ == '\0') {
        fVar10 = *(float *)(param_4 + 0x858);
      }
      else {
        fVar10 = (float)((ulonglong)*(undefined8 *)(param_4 + 0x858) >> 0x20);
      }
      if (in_FPSCR_PR == '\0') {
        dVar12 = (double)CONCAT44(fVar10 - _DAT_0c909724,_DAT_0c909724);
      }
      else {
        dVar12 = (double)CONCAT44(fVar10,_DAT_0c909724) - (double)CONCAT44(fVar10,_DAT_0c909724);
      }
      if (in_FPSCR_SZ == '\0') {
        *(int *)(param_4 + 0x858) = (int)((ulonglong)dVar12 >> 0x20);
        in_dr0 = (double)CONCAT44(uVar2,*(undefined4 *)(param_4 + 0x858));
      }
      else {
        *(double *)(param_4 + 0x858) = dVar12;
        in_xd0 = *(undefined8 *)(param_4 + 0x858);
      }
      in_dr2 = (double)((ulonglong)dVar12 & 0xffffffff);
      if (in_FPSCR_PR == '\0') {
        bVar14 = SUB84(in_dr0,0) < 0.0;
      }
      else {
        bVar14 = in_dr0 < in_dr2;
      }
      if (bVar14) {
        if (in_FPSCR_SZ == '\0') {
          *(undefined4 *)(param_4 + 0x858) = 0;
        }
        else {
          *(double *)(param_4 + 0x858) = dVar6;
        }
        *(undefined2 *)(param_4 + 0x812) = 0;
        *(undefined2 *)(param_4 + 0x848) = 0;
        *(undefined2 *)(param_4 + 0x888) = 0;
        *(undefined2 *)(param_4 + 0x814) = 0;
      }
    }
  }
  sVar1 = *(short *)(param_4 + 0x888);
  if (sVar1 == 0) {
    *(undefined2 *)(param_4 + 0x886) = 1;
    if (in_FPSCR_SZ == '\0') {
      *(undefined4 *)(param_4 + 0x894) = 0;
      *(undefined4 *)(param_4 + 0x898) = 0;
      *(undefined4 *)(param_4 + 0x89c) = 0;
    }
    else {
      *(double *)(param_4 + 0x894) = dVar6;
      *(double *)(param_4 + 0x898) = dVar6;
      *(double *)(param_4 + 0x89c) = dVar6;
    }
    if (in_FPSCR_SZ == '\0') {
      *(undefined4 *)(param_4 + 0x8ac) = 0x3f800000;
      *(undefined4 *)(param_4 + 0x8b0) = 0x3f800000;
    }
    else {
      *(ulonglong *)(param_4 + 0x8ac) = CONCAT44(0x3f800000,param_1);
      *(ulonglong *)(param_4 + 0x8b0) = CONCAT44(0x3f800000,param_1);
    }
    *(short *)(param_4 + 0x888) = *(short *)(param_4 + 0x888) + 1;
    return;
  }
  uVar2 = (undefined4)((ulonglong)in_dr0 >> 0x20);
  if (sVar1 == 1) {
    if (in_FPSCR_PR == '\0') {
      dVar7 = (double)CONCAT44(uVar2,0x40000000);
    }
    else {
      dVar7 = (double)CONCAT44(uVar2,0x3f800000) + (double)CONCAT44(uVar2,0x3f800000);
    }
    if (in_FPSCR_SZ == '\0') {
      fVar11 = *(float *)(param_4 + 0x8ac);
    }
    else {
      fVar11 = (float)((ulonglong)*(undefined8 *)(param_4 + 0x8ac) >> 0x20);
    }
    if (in_FPSCR_PR == '\0') {
      dVar12 = (double)CONCAT44(fVar11 - DAT_0c909880,DAT_0c909880);
    }
    else {
      dVar12 = (double)CONCAT44(fVar11,DAT_0c909880) - (double)CONCAT44(fVar11,DAT_0c909880);
    }
    if (in_FPSCR_SZ == '\0') {
      *(int *)(param_4 + 0x8ac) = (int)((ulonglong)dVar12 >> 0x20);
      dVar12 = (double)CONCAT44(*(undefined4 *)(param_4 + 0x8ac),SUB84(dVar12,0));
    }
    else {
      *(double *)(param_4 + 0x8ac) = dVar12;
      dVar12 = *(double *)(param_4 + 0x8ac);
    }
    if (in_FPSCR_PR == '\0') {
      dVar7 = (double)CONCAT44(-(float)((ulonglong)dVar12 >> 0x20) + SUB84(dVar7,0),SUB84(dVar12,0))
      ;
    }
    else {
      dVar7 = -dVar12 + dVar7;
    }
    if (in_FPSCR_SZ == '\0') {
      *(int *)(param_4 + 0x8b0) = (int)((ulonglong)dVar7 >> 0x20);
      dVar7 = (double)CONCAT44(*(undefined4 *)(param_4 + 0x8ac),SUB84(dVar7,0));
    }
    else {
      *(double *)(param_4 + 0x8b0) = dVar7;
      dVar7 = *(double *)(param_4 + 0x8ac);
    }
    if (in_FPSCR_PR == '\0') {
      bVar14 = (float)((ulonglong)dVar7 >> 0x20) < param_1;
    }
    else {
      bVar14 = dVar7 < dVar6;
    }
    if (bVar14) {
      *(short *)(param_4 + 0x888) = *(short *)(param_4 + 0x888) + 1;
      return;
    }
  }
  else {
    if (sVar1 != 2) {
      return;
    }
    if (in_FPSCR_SZ == '\0') {
      in_dr2 = (double)CONCAT44((int)((ulonglong)in_dr2 >> 0x20),(undefined4)DAT_0c909884);
      dVar12 = (double)CONCAT44(*(undefined4 *)(param_4 + 0x8ac),param_1);
    }
    else {
      dVar12 = *(double *)(param_4 + 0x8ac);
    }
    if (in_FPSCR_PR == '\0') {
      dVar12 = (double)CONCAT44((float)((ulonglong)dVar12 >> 0x20) + SUB84(in_dr2,0),SUB84(dVar12,0)
                               );
    }
    else {
      dVar12 = dVar12 + in_dr2;
    }
    if (in_FPSCR_SZ == '\0') {
      dVar6 = (double)CONCAT44(*(undefined4 *)(param_4 + 0x8b8),SUB84(in_dr2,0));
      in_dr0 = (double)CONCAT44(uVar2,*(undefined4 *)(param_4 + 0x8b8));
    }
    else {
      dVar6 = *(double *)(param_4 + 0x8b8);
      in_xd0 = *(undefined8 *)(param_4 + 0x8b8);
    }
    if (in_FPSCR_PR == '\0') {
      fVar11 = -((float)((ulonglong)dVar6 >> 0x20) * fVar13) -
               (float)((ulonglong)dVar12 >> 0x20) * fVar11;
      dVar7 = (double)CONCAT44(fVar11,SUB84(dVar6,0));
      dVar12 = (double)CONCAT44((int)((ulonglong)in_dr0 >> 0x20),SUB84(in_dr0,0) + fVar11);
    }
    else {
      dVar7 = -(dVar6 * dVar7) - dVar12 * dVar7;
      dVar12 = in_dr0 + dVar7;
    }
    if (in_FPSCR_SZ == '\0') {
      *(int *)(param_4 + 0x8b8) = SUB84(dVar12,0);
      dVar12 = (double)CONCAT44((int)((ulonglong)dVar12 >> 0x20),*(undefined4 *)(param_4 + 0x8ac));
      dVar7 = (double)CONCAT44(*(undefined4 *)(param_4 + 0x8b8),SUB84(dVar7,0));
    }
    else {
      *(undefined8 *)(param_4 + 0x8b8) = in_xd0;
      in_xd0 = *(undefined8 *)(param_4 + 0x8ac);
      dVar7 = *(double *)(param_4 + 0x8b8);
    }
    if (in_FPSCR_PR == '\0') {
      dVar12 = (double)CONCAT44((int)((ulonglong)dVar12 >> 0x20),
                                SUB84(dVar12,0) + (float)((ulonglong)dVar7 >> 0x20));
    }
    else {
      dVar12 = dVar12 + dVar7;
    }
    if (in_FPSCR_SZ == '\0') {
      *(int *)(param_4 + 0x8ac) = SUB84(dVar12,0);
      dVar7 = (double)CONCAT44(*(undefined4 *)(param_4 + 0x8ac),SUB84(dVar7,0));
      dVar12 = (double)CONCAT44((int)((ulonglong)dVar12 >> 0x20),DAT_0c909884._4_4_);
    }
    else {
      *(undefined8 *)(param_4 + 0x8ac) = in_xd0;
      dVar7 = *(double *)(param_4 + 0x8ac);
    }
    if (in_FPSCR_PR == '\0') {
      dVar7 = (double)CONCAT44(-(float)((ulonglong)dVar7 >> 0x20) + SUB84(dVar12,0),SUB84(dVar7,0));
    }
    else {
      dVar7 = -dVar7 + dVar12;
    }
    if (in_FPSCR_SZ == '\0') {
      dVar12 = (double)CONCAT44(DAT_0c90988c,SUB84(dVar12,0));
    }
    else {
      dVar12 = (double)CONCAT44((undefined4)DAT_0c909890,DAT_0c90988c);
    }
    if (in_FPSCR_PR == '\0') {
      dVar7 = (double)CONCAT44((float)((ulonglong)dVar7 >> 0x20) *
                               (float)((ulonglong)dVar12 >> 0x20),SUB84(dVar7,0));
    }
    else {
      dVar7 = dVar7 * dVar12;
    }
    if (in_FPSCR_SZ == '\0') {
      *(int *)(param_4 + 0x8b0) = (int)((ulonglong)dVar7 >> 0x20);
    }
    else {
      *(double *)(param_4 + 0x8b0) = dVar7;
    }
  }
  return;
}

