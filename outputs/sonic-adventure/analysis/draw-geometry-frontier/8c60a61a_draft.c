/* Ghidra draft only; not buildable/verified source. */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void callback_8c60a61a(undefined4 param_1,int param_2)

{
  ushort uVar1;
  ushort *puVar2;
  int iVar3;
  int iVar4;
  int iVar5;
  uint uVar6;
  uint uVar7;
  uint uVar8;
  double in_dr2;
  float fVar9;
  double dVar10;
  bool bVar11;
  uint uVar12;
  char in_FPSCR_PR;
  char in_FPSCR_SZ;
  
  iVar4 = _DAT_8c60a880;
  puVar2 = DAT_8c60a87c;
  iVar3 = _DAT_8c60a878;
  uVar1 = *DAT_8c60a87c;
  iVar5 = (uint)uVar1 * 0x1c + _DAT_8c60a878;
  iVar5 = *(int *)(*(int *)(iVar5 + 4) + *(int *)(iVar5 + 0x14) * 4);
  if (iVar5 == 0) {
    *(undefined4 *)((uint)uVar1 * 0xc + _DAT_8c60a880) = *(undefined4 *)(param_2 + 0x14);
    *(undefined4 *)(((uint)*puVar2 * 3 + 1) * 4 + iVar4) = *(undefined4 *)(param_2 + 0x18);
    *(undefined4 *)(((uint)*puVar2 * 3 + 2) * 4 + iVar4) = *(undefined4 *)(param_2 + 0x1c);
  }
  else {
    uVar6 = 0;
    iVar4 = (uint)uVar1 * 0x1c + _DAT_8c60a878;
    uVar7 = *(uint *)(*(int *)(iVar4 + 8) + *(int *)(iVar4 + 0x14) * 4);
    if (in_FPSCR_SZ == '\0') {
      dVar10 = (double)CONCAT44(*(undefined4 *)(iVar4 + 0xc),param_1);
      in_dr2 = (double)CONCAT44((int)((ulonglong)in_dr2 >> 0x20),_DAT_8c60a884);
    }
    else {
      dVar10 = *(double *)(iVar4 + 0xc);
    }
    fVar9 = (float)((ulonglong)dVar10 >> 0x20);
    if (in_FPSCR_PR == '\0') {
      bVar11 = SUB84(in_dr2,0) < fVar9;
    }
    else {
      bVar11 = in_dr2 < dVar10;
    }
    if (bVar11) {
      if (in_FPSCR_SZ == '\0') {
        dVar10 = (double)CONCAT44(_DAT_8c60a888,fVar9);
      }
      else {
        dVar10 = (double)CONCAT44(_DAT_8c60a88c,_DAT_8c60a888);
      }
      if (in_FPSCR_PR == '\0') {
        fVar9 = (float)((ulonglong)dVar10 >> 0x20);
        in_dr2 = (double)CONCAT44(fVar9,SUB84(dVar10,0) + fVar9);
      }
      else {
        in_dr2 = dVar10 + dVar10;
      }
    }
    else if (in_FPSCR_SZ == '\0') {
      in_dr2 = (double)CONCAT44((int)((ulonglong)in_dr2 >> 0x20),fVar9);
    }
    if (in_FPSCR_PR == '\0') {
      uVar12 = (uint)SUB84(in_dr2,0);
    }
    else {
      uVar12 = (uint)in_dr2;
    }
    while (uVar8 = uVar7, 1 < uVar8 - uVar6) {
      uVar7 = uVar6 + uVar8 >> 1;
      if (*(uint *)(uVar7 * 0x10 + iVar5) <= uVar12) {
        uVar6 = uVar7;
        uVar7 = uVar8;
      }
    }
    (*(code *)*_DAT_8c60a88c)(iVar5,(uint)uVar1 * 0xc + _DAT_8c60a880);
    *_DAT_8c60a890 = *_DAT_8c60a890 | *puVar2 + 1;
  }
  iVar3 = (uint)*puVar2 * 0x1c + iVar3;
  *(int *)(iVar3 + 0x14) = *(int *)(iVar3 + 0x14) + 1;
  return;
}

