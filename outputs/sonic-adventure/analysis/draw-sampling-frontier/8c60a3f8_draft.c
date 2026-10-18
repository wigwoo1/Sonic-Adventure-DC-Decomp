/* Ghidra draft only; not buildable/verified source. */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void callback_8c60a3f8(undefined4 param_1,int param_2)

{
  ushort uVar1;
  ushort *puVar2;
  int iVar3;
  int iVar4;
  int iVar5;
  uint uVar6;
  uint uVar7;
  uint uVar8;
  undefined4 uVar9;
  double in_dr2;
  float fVar10;
  double dVar11;
  undefined8 in_xd2;
  bool bVar12;
  uint uVar13;
  char in_FPSCR_PR;
  char in_FPSCR_SZ;
  
  iVar4 = _DAT_8c60a578;
  puVar2 = _DAT_8c60a574;
  iVar3 = _DAT_8c60a570;
  uVar1 = *_DAT_8c60a574;
  iVar5 = (uint)uVar1 * 0x1c + _DAT_8c60a570;
  iVar5 = *(int *)(*(int *)(iVar5 + 4) + *(int *)(iVar5 + 0x14) * 4);
  if (iVar5 == 0) {
    if (in_FPSCR_SZ == '\0') {
      in_dr2 = (double)(ulonglong)*(uint *)(param_2 + 8);
    }
    else {
      in_xd2 = *(undefined8 *)(param_2 + 8);
    }
    uVar9 = SUB84(in_dr2,0);
    if (in_FPSCR_SZ == '\0') {
      *(undefined4 *)((uint)uVar1 * 0xc + _DAT_8c60a578) = uVar9;
    }
    else {
      *(undefined8 *)((uint)uVar1 * 0xc + _DAT_8c60a578) = in_xd2;
    }
    if (in_FPSCR_SZ == '\0') {
      uVar9 = *(undefined4 *)(param_2 + 0xc);
    }
    else {
      in_xd2 = *(undefined8 *)(param_2 + 0xc);
    }
    iVar5 = ((uint)*puVar2 * 3 + 1) * 4;
    if (in_FPSCR_SZ == '\0') {
      *(undefined4 *)(iVar5 + iVar4) = uVar9;
    }
    else {
      *(undefined8 *)(iVar5 + iVar4) = in_xd2;
    }
    if (in_FPSCR_SZ == '\0') {
      uVar9 = *(undefined4 *)(param_2 + 0x10);
    }
    else {
      in_xd2 = *(undefined8 *)(param_2 + 0x10);
    }
    iVar5 = ((uint)*puVar2 * 3 + 2) * 4;
    if (in_FPSCR_SZ == '\0') {
      *(undefined4 *)(iVar5 + iVar4) = uVar9;
    }
    else {
      *(undefined8 *)(iVar5 + iVar4) = in_xd2;
    }
  }
  else {
    uVar6 = 0;
    iVar4 = (uint)uVar1 * 0x1c + _DAT_8c60a570;
    uVar7 = *(uint *)(*(int *)(iVar4 + 8) + *(int *)(iVar4 + 0x14) * 4);
    if (in_FPSCR_SZ == '\0') {
      dVar11 = (double)CONCAT44(*(undefined4 *)(iVar4 + 0xc),param_1);
      in_dr2 = (double)CONCAT44((int)((ulonglong)in_dr2 >> 0x20),_DAT_8c60a57c);
    }
    else {
      dVar11 = *(double *)(iVar4 + 0xc);
    }
    fVar10 = (float)((ulonglong)dVar11 >> 0x20);
    if (in_FPSCR_PR == '\0') {
      bVar12 = SUB84(in_dr2,0) < fVar10;
    }
    else {
      bVar12 = in_dr2 < dVar11;
    }
    if (bVar12) {
      if (in_FPSCR_SZ == '\0') {
        dVar11 = (double)CONCAT44(_DAT_8c60a580,fVar10);
      }
      else {
        dVar11 = (double)CONCAT44(_DAT_8c60a584,_DAT_8c60a580);
      }
      if (in_FPSCR_PR == '\0') {
        fVar10 = (float)((ulonglong)dVar11 >> 0x20);
        in_dr2 = (double)CONCAT44(fVar10,SUB84(dVar11,0) + fVar10);
      }
      else {
        in_dr2 = dVar11 + dVar11;
      }
    }
    else if (in_FPSCR_SZ == '\0') {
      in_dr2 = (double)CONCAT44((int)((ulonglong)in_dr2 >> 0x20),fVar10);
    }
    if (in_FPSCR_PR == '\0') {
      uVar13 = (uint)SUB84(in_dr2,0);
    }
    else {
      uVar13 = (uint)in_dr2;
    }
    while (uVar8 = uVar7, 1 < uVar8 - uVar6) {
      uVar7 = uVar6 + uVar8 >> 1;
      if (*(uint *)(uVar7 * 0x10 + iVar5) <= uVar13) {
        uVar6 = uVar7;
        uVar7 = uVar8;
      }
    }
    (*(code *)*_DAT_8c60a584)(iVar5,(uint)uVar1 * 0xc + _DAT_8c60a578);
    *_DAT_8c60a588 = *_DAT_8c60a588 | *puVar2 + 1;
  }
  iVar3 = (uint)*puVar2 * 0x1c + iVar3;
  *(int *)(iVar3 + 0x14) = *(int *)(iVar3 + 0x14) + 1;
  return;
}

