/* Ghidra draft only; not buildable/verified source. */

/* WARNING: Removing unreachable block (ram,0x0c90903c) */
/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_0c908f3e(uint param_1,int param_2)

{
  undefined1 *puVar1;
  undefined8 uVar2;
  float fVar3;
  float in_fr3;
  double dVar4;
  double dVar5;
  float fVar6;
  undefined8 in_xd2;
  undefined8 in_xd14;
  undefined4 in_PR;
  char in_FPSCR_PR;
  char in_FPSCR_SZ;
  undefined1 local_c [4];
  undefined1 local_8 [4];
  
  if (in_FPSCR_SZ == '\0') {
    puVar1 = local_8;
  }
  else {
    puVar1 = local_c;
  }
  *(undefined4 *)(puVar1 + -4) = in_PR;
  fVar6 = 1.0;
  if (*(short *)(param_2 + 0x888) == 0) {
    *(undefined2 *)(param_2 + 0x846) = 0;
    if (in_FPSCR_SZ == '\0') {
      *(undefined4 *)(param_2 + 0x854) = 0;
      *(undefined4 *)(param_2 + 0x858) = DAT_0c909008;
      *(undefined4 *)(param_2 + 0x85c) = 0;
      param_1 = DAT_0c90900c;
      *(uint *)(param_2 + 0x86c) = DAT_0c90900c;
      *(uint *)(param_2 + 0x870) = param_1;
    }
    else {
      *(ulonglong *)(param_2 + 0x854) = (ulonglong)param_1;
      *(ulonglong *)(param_2 + 0x858) = CONCAT44(DAT_0c90900c,DAT_0c909008);
      *(ulonglong *)(param_2 + 0x85c) = (ulonglong)param_1;
      uVar2 = CONCAT44(uRam0c909010,DAT_0c90900c);
      *(undefined8 *)(param_2 + 0x86c) = uVar2;
      *(undefined8 *)(param_2 + 0x870) = uVar2;
    }
    *(undefined2 *)(param_2 + 0x886) = 1;
    if (in_FPSCR_SZ == '\0') {
      *(undefined4 *)(param_2 + 0x894) = 0;
      *(undefined4 *)(param_2 + 0x898) = 0;
      *(undefined4 *)(param_2 + 0x89c) = 0;
      *(undefined4 *)(param_2 + 0x8ac) = 0x3f800000;
      *(undefined4 *)(param_2 + 0x8b0) = 0x3f800000;
    }
    else {
      *(ulonglong *)(param_2 + 0x894) = (ulonglong)param_1;
      *(ulonglong *)(param_2 + 0x898) = (ulonglong)param_1;
      *(ulonglong *)(param_2 + 0x89c) = (ulonglong)param_1;
      *(undefined8 *)(param_2 + 0x8ac) = in_xd14;
      *(undefined8 *)(param_2 + 0x8b0) = in_xd14;
    }
    *(undefined4 *)(param_2 + 0x840) = 0;
    *(short *)(param_2 + 0x888) = *(short *)(param_2 + 0x888) + 1;
  }
  else if (*(short *)(param_2 + 0x888) == 1) {
    uVar2 = (*DAT_0c9091a0)(*(undefined4 *)(param_2 + 0x840));
    fVar3 = _DAT_0c9091a8;
    if (in_FPSCR_SZ == '\0') {
      in_fr3 = (float)((ulonglong)uVar2 >> 0x20);
      fVar3 = DAT_0c9091a4;
      uVar2 = in_xd2;
    }
    if (in_FPSCR_PR == '\0') {
      dVar4 = (double)CONCAT44(0x40000000,in_fr3);
      fVar6 = fVar3 * in_fr3 + fVar6;
    }
    else {
      dVar4 = (double)CONCAT44(0x3f800000,in_fr3) + (double)CONCAT44(0x3f800000,in_fr3);
    }
    if (in_FPSCR_SZ == '\0') {
      *(float *)(param_2 + 0x8ac) = fVar6;
      dVar4 = (double)CONCAT44((int)((ulonglong)dVar4 >> 0x20),*(undefined4 *)(param_2 + 0x8ac));
    }
    else {
      *(undefined8 *)(param_2 + 0x8ac) = in_xd14;
      uVar2 = *(undefined8 *)(param_2 + 0x8ac);
    }
    fVar6 = (float)((ulonglong)dVar4 >> 0x20);
    dVar5 = (double)CONCAT44(fVar6,-SUB84(dVar4,0));
    if (in_FPSCR_PR == '\0') {
      fVar6 = -SUB84(dVar4,0) + fVar6;
    }
    else {
      fVar6 = SUB84(dVar5 + dVar5,0);
    }
    if (in_FPSCR_SZ == '\0') {
      *(float *)(param_2 + 0x8b0) = fVar6;
    }
    else {
      *(undefined8 *)(param_2 + 0x8b0) = uVar2;
    }
  }
  *(int *)(param_2 + 0x840) = *(int *)(param_2 + 0x840) + 0xc00;
  return;
}

