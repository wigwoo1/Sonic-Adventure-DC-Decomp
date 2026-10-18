/* Ghidra draft only; not buildable/verified source. */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined4 FUN_8c0aff1a(char *param_1,undefined4 param_2,undefined8 *param_3)

{
  undefined *puVar1;
  undefined *puVar2;
  ushort uVar3;
  int iVar4;
  undefined2 uVar5;
  undefined4 uVar6;
  int iVar7;
  undefined1 *puVar8;
  float in_fr1;
  undefined4 in_fr0;
  undefined8 uVar9;
  double dVar10;
  undefined4 uVar11;
  undefined8 in_xd14;
  bool bVar12;
  undefined4 in_PR;
  char in_FPSCR_PR;
  char in_FPSCR_SZ;
  undefined1 auStack_24 [4];
  undefined1 auStack_20 [4];
  
  puVar2 = PTR_FUN_8c0b0828;
  uVar6 = DAT_8c0b0824;
  puVar1 = PTR_FUN_8c0affbc;
  if (in_FPSCR_SZ == '\0') {
    puVar8 = auStack_20;
  }
  else {
    puVar8 = auStack_24;
  }
  *(undefined4 *)(puVar8 + -4) = in_PR;
  if (*(char *)(*(int *)(param_1 + 0x3c) + 0x24) != '\0') {
    FUN_8c0afe72(param_1,param_2,param_3);
    return 0;
  }
  if (*(int *)(*(int *)(param_1 + 0x3c) + 0x4c) != 0) {
    *param_1 = 'P';
    *(ushort *)(param_1 + 4) = *(ushort *)(param_1 + 4) & 0xdaff;
    return 1;
  }
  if ((*(ushort *)(param_1 + 4) & 0x1000) == 0) {
    return 0;
  }
  uVar11 = 0;
  switch(param_1[1]) {
  case '\x01':
    *param_1 = '\x03';
    *(undefined2 *)((int)param_3 + 0x124) = 0xb;
    if (in_FPSCR_SZ == '\0') {
      *(undefined4 *)(param_3 + 3) = 0;
    }
    else {
      param_3[3] = in_xd14;
    }
    if ((*(ushort *)(param_1 + 4) & 0x100) != 0) {
      *(ushort *)(param_1 + 4) = *(ushort *)(param_1 + 4) & 0xfaff;
    }
    uVar3 = *(ushort *)(param_1 + 4) | 0x2000;
    break;
  case '\x02':
    *param_1 = '\a';
    *(undefined2 *)((int)param_3 + 0x124) = 0xf;
    if (in_FPSCR_SZ == '\0') {
      *(undefined4 *)(param_3 + 3) = 0;
    }
    else {
      param_3[3] = in_xd14;
    }
    *(ushort *)(param_1 + 4) = *(ushort *)(param_1 + 4) | 0x500;
    uVar3 = *(ushort *)(param_1 + 4) | 0x2000;
    break;
  case '\x03':
  case '\b':
    *param_1 = '\t';
    *(undefined2 *)((int)param_3 + 0x124) = 0x10;
    goto LAB_8c0b01f0;
  case '\x04':
    *param_1 = '\n';
    *(undefined2 *)((int)param_3 + 0x124) = 0x10;
    if ((*(ushort *)(param_1 + 4) & 0x100) != 0) {
      *(ushort *)(param_1 + 4) = *(ushort *)(param_1 + 4) & 0xfaff;
    }
    uVar3 = *(ushort *)(param_1 + 4) & 0xdfff;
    break;
  case '\x05':
    *param_1 = '\v';
    *(undefined2 *)((int)param_3 + 0x124) = 0x46;
    if ((*(ushort *)(param_1 + 4) & 0x100) != 0) {
      *(ushort *)(param_1 + 4) = *(ushort *)(param_1 + 4) & 0xfaff;
    }
    uVar3 = *(ushort *)(param_1 + 4) & 0xdfff;
    break;
  case '\x06':
    *param_1 = '\t';
    *(undefined2 *)((int)param_3 + 0x124) = 0x27;
    goto LAB_8c0b01f0;
  case '\a':
    *param_1 = '\t';
    *(undefined2 *)((int)param_3 + 0x124) = 0x29;
    goto LAB_8c0b01f0;
  case '\t':
    *param_1 = '\x14';
    *(undefined2 *)((int)param_3 + 0x124) = 0x2c;
    param_1[0x1c] = '\0';
    param_1[0x1d] = '\0';
    param_1[0x1e] = '\0';
    param_1[0x1f] = '\0';
    param_1[0x14] = '\0';
    param_1[0x15] = '\0';
    param_1[0x16] = '\0';
    param_1[0x17] = '\0';
    (*(code *)puVar1)(param_2,param_3);
    if ((*(ushort *)(param_1 + 4) & 0x100) != 0) {
      *(ushort *)(param_1 + 4) = *(ushort *)(param_1 + 4) & 0xfaff;
    }
    uVar3 = *(ushort *)(param_1 + 4) & 0xdfff;
    break;
  case '\n':
    *param_1 = '\x14';
    *(undefined2 *)((int)param_3 + 0x124) = 0x2d;
    param_1[0x1c] = '\0';
    param_1[0x1d] = '\0';
    param_1[0x1e] = '\0';
    param_1[0x1f] = '\0';
    param_1[0x14] = '\0';
    param_1[0x15] = '\0';
    param_1[0x16] = '\0';
    param_1[0x17] = '\0';
    (*(code *)puVar1)(param_2,param_3);
    if ((*(ushort *)(param_1 + 4) & 0x100) != 0) {
      *(ushort *)(param_1 + 4) = *(ushort *)(param_1 + 4) & 0xfaff;
    }
    uVar3 = *(ushort *)(param_1 + 4) & 0xdfff;
    break;
  case '\v':
    *param_1 = '\f';
    *(undefined2 *)((int)param_3 + 0x124) = 0x1a;
    if ((*(ushort *)(param_1 + 4) & 0x100) != 0) {
      *(ushort *)(param_1 + 4) = *(ushort *)(param_1 + 4) & 0xfaff;
    }
    uVar3 = *(ushort *)(param_1 + 4) & 0xdfff;
    break;
  case '\f':
    *param_1 = '\x14';
    *(undefined2 *)((int)param_3 + 0x124) = 0;
    param_1[0x1c] = '\0';
    param_1[0x1d] = '\0';
    param_1[0x1e] = '\0';
    param_1[0x1f] = '\0';
    param_1[0x14] = '\0';
    param_1[0x15] = '\0';
    param_1[0x16] = '\0';
    param_1[0x17] = '\0';
    (*(code *)puVar1)(param_2,param_3);
    if ((*(ushort *)(param_1 + 4) & 0x100) != 0) {
      *(ushort *)(param_1 + 4) = *(ushort *)(param_1 + 4) & 0xfaff;
    }
    uVar3 = *(ushort *)(param_1 + 4) & 0xdfff;
    break;
  case '\r':
    *param_1 = '\x14';
    *(undefined2 *)((int)param_3 + 0x124) = 0xe;
    param_1[0x1c] = '\0';
    param_1[0x1d] = '\0';
    param_1[0x1e] = '\0';
    param_1[0x1f] = '\0';
    param_1[0x14] = '\0';
    param_1[0x15] = '\0';
    param_1[0x16] = '\0';
    param_1[0x17] = '\0';
    (*(code *)puVar1)(param_2,param_3);
    puVar1 = PTR_FUN_8c0b0284;
    *(ushort *)(param_1 + 4) = *(ushort *)(param_1 + 4) | 0x500;
    if (in_FPSCR_SZ == '\0') {
      *(undefined4 *)param_3 = _DAT_8c0b0280;
    }
    else {
      *param_3 = CONCAT44(PTR_FUN_8c0b0284,_DAT_8c0b0280);
    }
    (*(code *)puVar1)(0x2d8,0,0);
    uVar3 = *(ushort *)(param_1 + 4) & 0xdfff;
    break;
  case '\x0e':
    *param_1 = '\x14';
    *(undefined2 *)((int)param_3 + 0x124) = 0xc;
    (*(code *)puVar1)(param_2,param_3);
    if ((*(ushort *)(param_1 + 4) & 0x100) != 0) {
      *(ushort *)(param_1 + 4) = *(ushort *)(param_1 + 4) & 0xfaff;
    }
    uVar3 = *(ushort *)(param_1 + 4) & 0xdfff;
    break;
  case '\x0f':
    *param_1 = '\x14';
    *(undefined2 *)((int)param_3 + 0x124) = 0x15;
    param_1[0x1c] = '\0';
    param_1[0x1d] = '\0';
    param_1[0x1e] = '\0';
    param_1[0x1f] = '\0';
    param_1[0x14] = '\0';
    param_1[0x15] = '\0';
    param_1[0x16] = '\0';
    param_1[0x17] = '\0';
    (*(code *)puVar1)(param_2,param_3);
    if ((*(ushort *)(param_1 + 4) & 0x100) != 0) {
      *(ushort *)(param_1 + 4) = *(ushort *)(param_1 + 4) & 0xfaff;
    }
    uVar3 = *(ushort *)(param_1 + 4) & 0xdfff;
    break;
  case '\x10':
    *param_1 = '\x14';
    *(undefined2 *)((int)param_3 + 0x124) = 0x2f;
    param_1[0x1c] = '\0';
    param_1[0x1d] = '\0';
    param_1[0x1e] = '\0';
    param_1[0x1f] = '\0';
    param_1[0x14] = '\0';
    param_1[0x15] = '\0';
    param_1[0x16] = '\0';
    param_1[0x17] = '\0';
    (*(code *)puVar1)(param_2,param_3);
LAB_8c0b01f0:
    uVar3 = *(ushort *)(param_1 + 4) & 0xdaff;
    break;
  case '\x11':
    *param_1 = '\x14';
    *(undefined2 *)((int)param_3 + 0x124) = 0x12;
    param_1[0x1c] = '\0';
    param_1[0x1d] = '\0';
    param_1[0x1e] = '\0';
    param_1[0x1f] = '\0';
    param_1[0x14] = '\0';
    param_1[0x15] = '\0';
    param_1[0x16] = '\0';
    param_1[0x17] = '\0';
    (*(code *)puVar1)(param_2,param_3);
    uVar3 = *(ushort *)(param_1 + 4) & 0xdaff;
    break;
  case '\x12':
    *param_1 = '-';
    *(undefined2 *)((int)param_3 + 0x124) = 0x41;
    param_1[0x1c] = '\0';
    param_1[0x1d] = '\0';
    param_1[0x1e] = '\0';
    param_1[0x1f] = '\0';
    param_1[0x14] = '\0';
    param_1[0x15] = '\0';
    param_1[0x16] = '\0';
    param_1[0x17] = '\0';
    (*(code *)puVar1)(param_2,param_3);
    uVar3 = *(ushort *)(param_1 + 4) & 0xdaff;
    break;
  case '\x13':
    *param_1 = '\x14';
    *(undefined2 *)((int)param_3 + 0x124) = 0x4b;
    param_1[0x1c] = '\0';
    param_1[0x1d] = '\0';
    param_1[0x1e] = '\0';
    param_1[0x1f] = '\0';
    param_1[0x14] = '\0';
    param_1[0x15] = '\0';
    param_1[0x16] = '\0';
    param_1[0x17] = '\0';
    (*(code *)puVar1)(param_2,param_3);
    uVar3 = *(ushort *)(param_1 + 4) & 0xdaff;
    break;
  case '\x14':
    *param_1 = '\x14';
    *(undefined2 *)((int)param_3 + 0x124) = 0x4d;
    param_1[0x1c] = '\0';
    param_1[0x1d] = '\0';
    param_1[0x1e] = '\0';
    param_1[0x1f] = '\0';
    param_1[0x14] = '\0';
    param_1[0x15] = '\0';
    param_1[0x16] = '\0';
    param_1[0x17] = '\0';
    (*(code *)puVar1)(param_2,param_3);
    uVar3 = *(ushort *)(param_1 + 4) & 0xdaff;
    break;
  case '\x15':
    *param_1 = '\x14';
    *(undefined2 *)((int)param_3 + 0x124) = 0x4f;
    param_1[0x1c] = '\0';
    param_1[0x1d] = '\0';
    param_1[0x1e] = '\0';
    param_1[0x1f] = '\0';
    param_1[0x14] = '\0';
    param_1[0x15] = '\0';
    param_1[0x16] = '\0';
    param_1[0x17] = '\0';
    (*(code *)puVar1)(param_2,param_3);
    uVar3 = *(ushort *)(param_1 + 4) & 0xdaff;
    break;
  case '\x16':
    *param_1 = '\x14';
    *(undefined2 *)((int)param_3 + 0x124) = 0x50;
    param_1[0x1c] = '\0';
    param_1[0x1d] = '\0';
    param_1[0x1e] = '\0';
    param_1[0x1f] = '\0';
    param_1[0x14] = '\0';
    param_1[0x15] = '\0';
    param_1[0x16] = '\0';
    param_1[0x17] = '\0';
    (*(code *)puVar1)(param_2,param_3);
    uVar3 = *(ushort *)(param_1 + 4) & 0xdaff;
    break;
  case '\x17':
    *param_1 = '\x14';
    *(undefined2 *)((int)param_3 + 0x124) = 0x58;
    uVar3 = *(ushort *)(param_1 + 4) & 0xdaff;
    break;
  case '\x18':
    (*(code *)PTR_FUN_8c0b03e0)(param_1,param_2,param_3);
    if ((*(ushort *)(param_1 + 4) & 0x100) != 0) {
      *(ushort *)(param_1 + 4) = *(ushort *)(param_1 + 4) & 0xfaff;
    }
    uVar3 = *(ushort *)(param_1 + 4) & 0xdfff;
    break;
  case '\x19':
    *param_1 = '\x05';
    *(undefined2 *)((int)param_3 + 0x124) = 0xf;
    uVar6 = 0x2d9;
    *(ushort *)(param_1 + 4) = *(ushort *)(param_1 + 4) | 0x500;
    goto LAB_8c0b05f0;
  case '\x1a':
    if ((*param_1 != '\x02') && (*param_1 != '\x05')) {
      if (in_FPSCR_SZ == '\0') {
        uVar9 = CONCAT44(*(undefined4 *)(param_3 + 7),*(undefined4 *)((int)param_3 + 0xc4));
      }
      else {
        uVar9 = param_3[7];
      }
      if (in_FPSCR_PR != '\0' || (float)((ulonglong)uVar9 >> 0x20) <= (float)uVar9) {
        return 0;
      }
    }
    *(ushort *)(param_1 + 4) = *(ushort *)(param_1 + 4) | 0x2000;
  default:
    return 0;
  case '\x1d':
    (*(code *)PTR_FUN_8c0b0540)(0x22,0,0);
    *param_1 = '$';
    *(undefined2 *)((int)param_3 + 0x124) = 0x43;
    if (in_FPSCR_SZ == '\0') {
      *(undefined4 *)(param_3 + 0x26) = uVar11;
    }
    else {
      param_3[0x26] = in_xd14;
    }
    param_1[0x1c] = '\0';
    param_1[0x1d] = '\0';
    param_1[0x1e] = '\0';
    param_1[0x1f] = '\0';
    param_1[0x14] = '\0';
    param_1[0x15] = '\0';
    param_1[0x16] = '\0';
    param_1[0x17] = '\0';
    (*(code *)puVar1)(param_2,param_3);
    *(ushort *)(param_1 + 4) = *(ushort *)(param_1 + 4) & 0xfaff;
    uVar3 = *(ushort *)(param_1 + 4) | 0x2000;
    break;
  case '\x1e':
    if (('6' < *param_1) && (*param_1 < '=')) {
      param_1[1] = '\0';
      return 0;
    }
    if (in_FPSCR_SZ == '\0') {
      dVar10 = (double)CONCAT44(*(undefined4 *)(param_1 + 0x24),DAT_8c0b0544);
    }
    else {
      dVar10 = *(double *)(param_1 + 0x24);
    }
    if (in_FPSCR_PR == '\0') {
      dVar10 = (double)CONCAT44((float)((ulonglong)dVar10 >> 0x20) + SUB84(dVar10,0),SUB84(dVar10,0)
                               );
    }
    else {
      dVar10 = dVar10 + dVar10;
    }
    if (in_FPSCR_SZ == '\0') {
      in_fr1 = *(float *)(*(int *)(*(int *)(param_3 + 0xd) + 0x20) + 0x24);
    }
    if (in_FPSCR_PR == '\0') {
      bVar12 = (float)((ulonglong)dVar10 >> 0x20) < in_fr1;
    }
    else {
      bVar12 = dVar10 < (double)CONCAT44(in_fr0,in_fr1);
    }
    if (bVar12) {
      *param_1 = '<';
      uVar5 = 0x65;
    }
    else {
      uVar5 = 0x5e;
      *param_1 = '7';
    }
    *(undefined2 *)((int)param_3 + 0x124) = uVar5;
    (*(code *)PTR_FUN_8c0b06c0)(0x22,0,0);
    goto LAB_8c0b0576;
  case '\x1f':
    if ((*(ushort *)(param_1 + 4) & 3) != 0) {
      return 0;
    }
    (*(code *)PTR_FUN_8c0b0540)(0x22,0,0);
    *param_1 = '&';
    *(undefined2 *)((int)param_3 + 0x124) = 0x45;
    if (in_FPSCR_SZ == '\0') {
      *(undefined4 *)(param_3 + 0x26) = uVar11;
    }
    else {
      param_3[0x26] = in_xd14;
    }
    (*(code *)puVar1)(param_2,param_3);
    *(ushort *)(param_1 + 4) = *(ushort *)(param_1 + 4) & 0xfaff;
    uVar3 = *(ushort *)(param_1 + 4) | 0x2000;
    break;
  case ' ':
    if (('2' < *param_1) && (*param_1 < '7')) {
      param_1[1] = '\0';
      return 0;
    }
    iVar7 = *(int *)(*(int *)(param_3 + 0xd) + 0x20);
    iVar4 = *(int *)(*(int *)(iVar7 + 0x38) + 0xc);
    if (in_FPSCR_SZ == '\0') {
      dVar10 = (double)CONCAT44(*(undefined4 *)(iVar7 + 0x24),*(undefined4 *)(iVar4 + 0xc));
    }
    else {
      dVar10 = *(double *)(iVar7 + 0x24);
    }
    if (in_FPSCR_PR == '\0') {
      dVar10 = (double)CONCAT44((float)((ulonglong)dVar10 >> 0x20) + SUB84(dVar10,0),SUB84(dVar10,0)
                               );
    }
    else {
      dVar10 = dVar10 + dVar10;
    }
    if (in_FPSCR_SZ == '\0') {
      dVar10 = (double)CONCAT44((int)((ulonglong)dVar10 >> 0x20),*(undefined4 *)(iVar4 + 0x18));
    }
    if (in_FPSCR_PR == '\0') {
      dVar10 = (double)CONCAT44((float)((ulonglong)dVar10 >> 0x20) - SUB84(dVar10,0),SUB84(dVar10,0)
                               );
    }
    else {
      dVar10 = dVar10 - dVar10;
    }
    if (in_FPSCR_SZ == '\0') {
      dVar10 = (double)CONCAT44((int)((ulonglong)dVar10 >> 0x20),*(undefined4 *)(param_1 + 0x24));
    }
    if (in_FPSCR_PR != '\0' || SUB84(dVar10,0) <= (float)((ulonglong)dVar10 >> 0x20)) {
      uVar5 = 0x59;
      *param_1 = '3';
    }
    else {
      *param_1 = '4';
      uVar5 = 0x5c;
    }
    *(undefined2 *)((int)param_3 + 0x124) = uVar5;
    (*(code *)PTR_FUN_8c0b0540)(0x22,0,0);
LAB_8c0b0576:
    (*(code *)puVar1)(param_2,param_3);
    *(ushort *)(param_1 + 4) = *(ushort *)(param_1 + 4) & 0xdaff;
    *(undefined2 *)((int)param_3 + 0x82) = 0;
    return 1;
  case '!':
    *param_1 = '*';
    *(undefined2 *)((int)param_3 + 0x124) = 0x51;
    *(undefined2 *)((int)param_3 + 0x82) = 0xb4;
    (*(code *)puVar1)(param_2,param_3);
    if ((*(ushort *)(param_1 + 4) & 0x100) != 0) {
      *(ushort *)(param_1 + 4) = *(ushort *)(param_1 + 4) & 0xfaff;
    }
    uVar6 = 0x56e;
    *(ushort *)(param_1 + 4) = *(ushort *)(param_1 + 4) & 0xdfff;
LAB_8c0b05f0:
    (*(code *)PTR_FUN_8c0b06c0)(uVar6,0,0);
    return 1;
  case '\"':
    *param_1 = 'O';
    *(undefined2 *)((int)param_3 + 0x124) = 3;
    uVar3 = *(ushort *)(param_1 + 4) & 0xdaff;
    break;
  case '#':
    *param_1 = '\x12';
    *(undefined2 *)((int)param_3 + 0x124) = 0x1b;
    if (param_1[8] == '\0') {
      (*(code *)PTR_FUN_8c0b06c0)(0x458,0,0);
    }
    puVar1 = PTR_FUN_8c0b06c8;
    *(undefined2 *)(param_3 + 0x10) = 0;
    (*(code *)puVar1)(param_1);
    uVar3 = *(ushort *)(param_1 + 4) & 0xdaff;
    break;
  case '$':
    *param_1 = 'O';
    *(undefined2 *)((int)param_3 + 0x124) = 0x54;
    *(undefined2 *)((int)param_3 + 0xc) = 0;
    uVar3 = *(ushort *)(param_1 + 4) & 0xdaff;
    break;
  case '%':
    *param_1 = '\x17';
    *(undefined2 *)((int)param_3 + 0x124) = 0x25;
    iVar4 = *(int *)(param_1 + 0x10);
    *(int *)(param_3 + 0xd) = iVar4;
    iVar4 = *(int *)(iVar4 + 0x20);
    *(ushort *)(iVar4 + 4) = *(ushort *)(iVar4 + 4) | 0x1000;
    *(ushort *)(param_1 + 4) = *(ushort *)(param_1 + 4) | 0x800;
    *(ushort *)(param_1 + 4) = *(ushort *)(param_1 + 4) & 0xfff7;
    uVar3 = *(ushort *)(param_1 + 4) & 0xdaff;
    break;
  case '&':
    if (*param_1 == '+') {
      return 1;
    }
    *param_1 = '+';
    *(undefined2 *)((int)param_3 + 0x124) = 0x52;
    puVar2 = PTR_FUN_8c0b06c4;
    *(undefined2 *)((int)param_3 + 0x82) = 300;
    (*(code *)puVar2)(param_1[8]);
    (*(code *)puVar1)(param_2,param_3);
    if ((*(ushort *)(param_1 + 4) & 0x100) != 0) {
      *(ushort *)(param_1 + 4) = *(ushort *)(param_1 + 4) & 0xfaff;
    }
    *(ushort *)(param_1 + 4) = *(ushort *)(param_1 + 4) & 0xdfff;
    return 1;
  case '\'':
    *param_1 = 'Q';
    *(undefined2 *)((int)param_3 + 0x124) = 0x55;
    uVar3 = *(ushort *)(param_1 + 4) & 0xdaff;
    break;
  case '(':
    *param_1 = '\x14';
    *(undefined2 *)((int)param_3 + 0x124) = 0x7e;
    param_1[0x1c] = '\0';
    param_1[0x1d] = '\0';
    param_1[0x1e] = '\0';
    param_1[0x1f] = '\0';
    param_1[0x14] = '\0';
    param_1[0x15] = '\0';
    param_1[0x16] = '\0';
    param_1[0x17] = '\0';
    (*(code *)puVar1)(param_2,param_3);
    uVar3 = *(ushort *)(param_1 + 4) & 0xdaff;
    break;
  case ')':
    *param_1 = 'O';
    *(undefined2 *)((int)param_3 + 0x124) = 0x80;
    return 1;
  case '*':
    *param_1 = 'O';
    *(undefined2 *)((int)param_3 + 0x124) = 0x81;
    param_1[0x1c] = '\0';
    param_1[0x1d] = '\0';
    param_1[0x1e] = '\0';
    param_1[0x1f] = '\0';
    param_1[0x14] = '\0';
    param_1[0x15] = '\0';
    param_1[0x16] = '\0';
    param_1[0x17] = '\0';
    (*(code *)puVar1)(param_2,param_3);
    uVar3 = *(ushort *)(param_1 + 4) & 0xdaff;
    break;
  case '+':
    *param_1 = '0';
    *(undefined2 *)((int)param_3 + 0x124) = 0x12;
    if ((*(ushort *)(param_1 + 4) & 0x100) != 0) {
      *(ushort *)(param_1 + 4) = *(ushort *)(param_1 + 4) & 0xfaff;
    }
    uVar3 = *(ushort *)(param_1 + 4) & 0xdfff;
    break;
  case ',':
    *param_1 = 'A';
    *(undefined2 *)((int)param_3 + 0x124) = 0x69;
    if ((*(ushort *)(param_1 + 4) & 0x100) != 0) {
      *(ushort *)(param_1 + 4) = *(ushort *)(param_1 + 4) & 0xfaff;
    }
    uVar3 = *(ushort *)(param_1 + 4) & 0xdfff;
    break;
  case '-':
    *param_1 = 'E';
    *(undefined2 *)((int)param_3 + 0x124) = 0x7d;
    *(ushort *)(param_1 + 4) = *(ushort *)(param_1 + 4) | 0x500;
    uVar3 = *(ushort *)(param_1 + 4) & 0xdfff;
    break;
  case '.':
    *(ushort *)((int)param_3 + 4) = *(ushort *)((int)param_3 + 4) | (ushort)DAT_8c0b0820;
    iVar4 = (*(code *)puVar2)(2,uVar6);
    if (iVar4 != 0) {
      *(char *)(*(int *)(iVar4 + 0x20) + 8) = param_1[8];
    }
    iVar4 = (*(code *)PTR_FUN_8c0b0828)(8,2,PTR_LAB_8c0b082c);
    if (iVar4 != 0) {
      **(char **)(iVar4 + 0x2c) = param_1[8];
    }
    *param_1 = 'G';
    *(undefined2 *)((int)param_3 + 0x124) = 0x82;
    uVar3 = *(ushort *)(param_1 + 4) & 0xdaff;
    break;
  case '2':
    *param_1 = 'R';
    *(undefined2 *)((int)param_3 + 0x124) = 0x56;
    puVar1 = PTR_FUN_8c0b0830;
    *(ushort *)(param_1 + 4) = *(ushort *)(param_1 + 4) & 0xdaff;
    (*(code *)puVar1)(0x570,0,0);
    return 1;
  }
  *(ushort *)(param_1 + 4) = uVar3;
  return 1;
}

