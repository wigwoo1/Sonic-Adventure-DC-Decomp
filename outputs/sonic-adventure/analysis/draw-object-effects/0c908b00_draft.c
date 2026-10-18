/* Ghidra draft only; not buildable/verified source. */

/* WARNING: Removing unreachable block (ram,0x0c908b78) */
/* WARNING: Removing unreachable block (ram,0x0c908b2c) */
/* WARNING: Removing unreachable block (ram,0x0c908b54) */
/* WARNING: Removing unreachable block (ram,0x0c908cae) */
/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void callback_candidate_c908b00(undefined4 param_1,int param_2,int param_3)

{
  code *pcVar1;
  code *pcVar2;
  byte *pbVar3;
  undefined1 *puVar4;
  double dVar5;
  float fVar6;
  undefined4 uVar7;
  double dVar8;
  undefined4 uVar12;
  undefined8 uVar9;
  undefined8 uVar10;
  undefined4 in_fr15;
  undefined4 uVar13;
  undefined8 in_xd2;
  undefined8 in_xd4;
  undefined8 in_xd14;
  undefined4 in_PR;
  undefined1 uVar14;
  char in_FPSCR_PR;
  char in_FPSCR_SZ;
  undefined1 local_14 [4];
  undefined1 local_10 [4];
  float fVar11;
  
  if (in_FPSCR_SZ == '\0') {
    puVar4 = local_10;
  }
  else {
    puVar4 = local_14;
  }
  *(undefined4 *)(puVar4 + -4) = in_PR;
  if (in_FPSCR_SZ == '\0') {
    in_fr15 = *(undefined4 *)(param_2 + 0xd8);
  }
  else {
    in_xd14 = *(undefined8 *)(param_2 + 0xd8);
  }
  pbVar3 = PTR_DAT_0c908d40 + (uint)**(byte **)(param_2 + 0xec) * 4;
  if (in_FPSCR_SZ == '\0') {
    dVar8 = (double)CONCAT44(DAT_0c908d44,(float)*(byte *)(param_3 + 0xb6));
  }
  else {
    dVar8 = (double)CONCAT44(_DAT_0c908d48,DAT_0c908d44);
    in_xd4 = in_xd2;
  }
  if (in_FPSCR_PR == '\0') {
    fVar11 = (float)((ulonglong)dVar8 >> 0x20);
    fVar6 = SUB84(dVar8,0) * (float)pbVar3[2] * fVar11;
    dVar8 = (double)CONCAT44(fVar11,fVar6);
    uVar14 = (undefined1)(int)fVar6;
  }
  else {
    dVar8 = dVar8 * (double)pbVar3[2];
    dVar8 = dVar8 * dVar8;
    uVar14 = (undefined1)(int)dVar8;
  }
  puVar4[-0x66] = uVar14;
  if (in_FPSCR_PR == '\0') {
    dVar5 = (double)CONCAT44((float)pbVar3[1],(float)*(byte *)(param_3 + 0xb5));
  }
  else {
    dVar5 = (double)pbVar3[1];
  }
  uVar10 = in_xd2;
  if (in_FPSCR_SZ == '\0') {
    dVar8 = (double)CONCAT44((int)((ulonglong)dVar8 >> 0x20),SUB84(dVar5,0));
    uVar10 = in_xd4;
  }
  if (in_FPSCR_PR == '\0') {
    fVar11 = (float)((ulonglong)dVar8 >> 0x20);
    fVar6 = SUB84(dVar8,0) * (float)((ulonglong)dVar5 >> 0x20) * fVar11;
    dVar8 = (double)CONCAT44(fVar11,fVar6);
    uVar14 = (undefined1)(int)fVar6;
  }
  else {
    dVar8 = dVar8 * dVar5 * dVar8 * dVar5;
    uVar14 = (undefined1)(int)dVar8;
  }
  puVar4[-0x67] = uVar14;
  if (in_FPSCR_PR == '\0') {
    dVar5 = (double)CONCAT44((float)*pbVar3,(float)*(byte *)(param_3 + 0xb4));
  }
  else {
    dVar5 = (double)*pbVar3;
  }
  if (in_FPSCR_SZ == '\0') {
    dVar8 = (double)CONCAT44((int)((ulonglong)dVar8 >> 0x20),SUB84(dVar5,0));
    in_xd2 = uVar10;
  }
  if (in_FPSCR_PR == '\0') {
    fVar11 = (float)((ulonglong)dVar8 >> 0x20);
    fVar6 = SUB84(dVar8,0) * (float)((ulonglong)dVar5 >> 0x20) * fVar11;
    dVar8 = (double)CONCAT44(fVar11,fVar6);
    uVar14 = (undefined1)(int)fVar6;
  }
  else {
    dVar8 = dVar8 * dVar5 * dVar8 * dVar5;
    uVar14 = (undefined1)(int)dVar8;
  }
  uVar12 = SUB84(dVar8,0);
  puVar4[-0x68] = uVar14;
  puVar4[-0x65] = 0xff;
  *(undefined1 **)(puVar4 + -0x14) = puVar4 + -0x44;
  *(undefined1 **)(puVar4 + -0x10) = puVar4 + -0x54;
  *(undefined1 **)(puVar4 + -0xc) = puVar4 + -100;
  *(undefined4 *)(puVar4 + -8) = 4;
  if (in_FPSCR_SZ == '\0') {
    uVar10 = CONCAT44(_DAT_0c908d48,param_1);
  }
  else {
    uVar10 = CONCAT44(_DAT_0c908d4c,_DAT_0c908d48);
  }
  uVar13 = (undefined4)((ulonglong)uVar10 >> 0x20);
  if (in_FPSCR_SZ == '\0') {
    *(undefined4 *)*(undefined8 **)(puVar4 + -0x14) = uVar13;
    uVar12 = _DAT_0c908d4c;
  }
  else {
    **(undefined8 **)(puVar4 + -0x14) = uVar10;
    in_xd2 = CONCAT44(DAT_0c908d50,_DAT_0c908d4c);
  }
  if (in_FPSCR_SZ == '\0') {
    *(undefined4 *)(*(int *)(puVar4 + -0x14) + 4) = uVar12;
    uVar9 = CONCAT44(DAT_0c908d50,uVar12);
  }
  else {
    *(undefined8 *)(*(int *)(puVar4 + -0x14) + 4) = in_xd2;
    uVar9 = CONCAT44(PTR_PTR_0c908d54,DAT_0c908d50);
  }
  uVar12 = (undefined4)((ulonglong)uVar9 >> 0x20);
  if (in_FPSCR_SZ == '\0') {
    *(undefined4 *)(*(int *)(puVar4 + -0x14) + 8) = uVar12;
  }
  else {
    *(undefined8 *)(*(int *)(puVar4 + -0x14) + 8) = uVar9;
  }
  **(undefined2 **)(puVar4 + -0xc) = 0;
  *(undefined2 *)(*(int *)(puVar4 + -0xc) + 2) = 0;
  **(undefined4 **)(puVar4 + -0x10) = *(undefined4 *)(puVar4 + -0x68);
  if (in_FPSCR_SZ == '\0') {
    *(undefined4 *)(*(int *)(puVar4 + -0x14) + 0xc) = uVar13;
  }
  else {
    *(undefined8 *)(*(int *)(puVar4 + -0x14) + 0xc) = uVar10;
  }
  if (in_FPSCR_SZ == '\0') {
    *(undefined4 *)(*(int *)(puVar4 + -0x14) + 0x10) = uVar13;
  }
  else {
    *(undefined8 *)(*(int *)(puVar4 + -0x14) + 0x10) = uVar10;
  }
  if (in_FPSCR_SZ == '\0') {
    *(undefined4 *)(*(int *)(puVar4 + -0x14) + 0x14) = uVar12;
  }
  else {
    *(undefined8 *)(*(int *)(puVar4 + -0x14) + 0x14) = uVar9;
  }
  *(undefined2 *)(*(int *)(puVar4 + -0xc) + 4) = 0;
  *(undefined2 *)(*(int *)(puVar4 + -0xc) + 6) = 0xff;
  *(undefined4 *)(*(int *)(puVar4 + -0x10) + 4) = *(undefined4 *)(puVar4 + -0x68);
  uVar7 = (undefined4)uVar9;
  if (in_FPSCR_SZ == '\0') {
    *(undefined4 *)(*(int *)(puVar4 + -0x14) + 0x18) = uVar7;
  }
  else {
    *(undefined8 *)(*(int *)(puVar4 + -0x14) + 0x18) = in_xd2;
  }
  if (in_FPSCR_SZ == '\0') {
    *(undefined4 *)(*(int *)(puVar4 + -0x14) + 0x1c) = uVar13;
  }
  else {
    *(undefined8 *)(*(int *)(puVar4 + -0x14) + 0x1c) = uVar10;
  }
  if (in_FPSCR_SZ == '\0') {
    *(undefined4 *)(*(int *)(puVar4 + -0x14) + 0x20) = uVar12;
  }
  else {
    *(undefined8 *)(*(int *)(puVar4 + -0x14) + 0x20) = uVar9;
  }
  *(undefined2 *)(*(int *)(puVar4 + -0xc) + 8) = 0xff;
  *(undefined2 *)(*(int *)(puVar4 + -0xc) + 10) = 0xff;
  *(undefined4 *)(*(int *)(puVar4 + -0x10) + 8) = *(undefined4 *)(puVar4 + -0x68);
  if (in_FPSCR_SZ == '\0') {
    *(undefined4 *)(*(int *)(puVar4 + -0x14) + 0x24) = uVar7;
  }
  else {
    *(undefined8 *)(*(int *)(puVar4 + -0x14) + 0x24) = in_xd2;
  }
  if (in_FPSCR_SZ == '\0') {
    *(undefined4 *)(*(int *)(puVar4 + -0x14) + 0x28) = uVar7;
  }
  else {
    *(undefined8 *)(*(int *)(puVar4 + -0x14) + 0x28) = in_xd2;
  }
  if (in_FPSCR_SZ == '\0') {
    *(undefined4 *)(*(int *)(puVar4 + -0x14) + 0x2c) = uVar12;
  }
  else {
    *(undefined8 *)(*(int *)(puVar4 + -0x14) + 0x2c) = uVar9;
  }
  *(undefined2 *)(*(int *)(puVar4 + -0xc) + 0xc) = 0xff;
  *(undefined2 *)(*(int *)(puVar4 + -0xc) + 0xe) = 0;
  *(undefined4 *)(*(int *)(puVar4 + -0x10) + 0xc) = *(undefined4 *)(puVar4 + -0x68);
  (*DAT_0c908d58)(PTR_PTR_0c908d54);
  uVar12 = (undefined4)uVar10;
  (*DAT_0c908d60)(DAT_0c908d5c);
  pcVar1 = DAT_0c908d64;
  (*DAT_0c908d64)(0,param_3 + 0xc);
  pcVar2 = DAT_0c908d68;
  uVar10 = in_xd14;
  if (in_FPSCR_SZ == '\0') {
    uVar10 = CONCAT44(in_fr15,in_fr15);
  }
  uVar9 = in_xd14;
  if (in_FPSCR_SZ == '\0') {
    uVar9 = CONCAT44(in_fr15,uVar12);
  }
  (*DAT_0c908d68)(-(float)uVar10,uVar9,0);
  uVar12 = (undefined4)uVar9;
  (*DAT_0c908d60)(0);
  (*pcVar1)(0,param_3 + 0x84);
  (*DAT_0c908d6c)(*(undefined2 *)(param_3 + 0x76));
  uVar13 = 0x3f800000;
  if (in_FPSCR_SZ == '\0') {
    uVar9 = CONCAT44(*(undefined4 *)(param_3 + 0x9c),*(undefined4 *)(param_3 + 0xa0));
    uVar10 = CONCAT44(0x3f800000,uVar12);
  }
  else {
    uVar9 = *(undefined8 *)(param_3 + 0x9c);
    uVar10 = in_xd14;
  }
  (*pcVar2)(uVar9,uVar10,0);
  uVar12 = (undefined4)uVar10;
  (*DAT_0c908d74)(puVar4 + -0x14,4,DAT_0c908d70);
  (*DAT_0c908d78)(1);
  (*pcVar1)(0,param_3 + 0x44);
  (*DAT_0c908d6c)(*(undefined2 *)(param_3 + 0x36));
  if (in_FPSCR_SZ == '\0') {
    in_xd14 = CONCAT44(uVar13,uVar12);
    uVar10 = CONCAT44(*(undefined4 *)(param_3 + 0x5c),*(undefined4 *)(param_3 + 0x60));
  }
  else {
    uVar10 = *(undefined8 *)(param_3 + 0x5c);
  }
  (*pcVar2)(uVar10,in_xd14,0);
  (*DAT_0c908d74)(puVar4 + -0x14,4,DAT_0c908d70);
  (*DAT_0c908d78)(1);
  return;
}

