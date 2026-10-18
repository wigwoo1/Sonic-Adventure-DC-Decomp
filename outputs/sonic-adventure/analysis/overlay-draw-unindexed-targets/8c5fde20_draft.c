/* Ghidra draft only; not buildable/verified source. */

/* WARNING: Removing unreachable block (ram,0x8c5fde5c) */
/* WARNING: Removing unreachable block (ram,0x8c5fdecc) */
/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void callback_8c5fde20(float param_1,undefined4 param_2,undefined4 *param_3)

{
  float *pfVar1;
  undefined4 *puVar2;
  ushort *puVar3;
  ushort *puVar4;
  ushort *puVar5;
  undefined4 *puVar6;
  int iVar7;
  undefined4 *puVar8;
  int iVar9;
  code *pcVar10;
  int *piVar11;
  int iVar12;
  uint *puVar13;
  float in_fr1;
  undefined4 in_fr0;
  float fVar14;
  undefined8 in_dr2;
  undefined4 uVar16;
  double dVar15;
  bool bVar17;
  char in_FPSCR_PR;
  char in_FPSCR_SZ;
  
  puVar5 = _DAT_8c5fe050;
  puVar4 = _DAT_8c5fe04c;
  puVar3 = _DAT_8c5fe048;
  puVar2 = _DAT_8c5fe044;
  pfVar1 = _DAT_8c5fe040;
  uVar16 = (undefined4)((ulonglong)in_dr2 >> 0x20);
  dVar15 = (double)CONCAT44(uVar16,0x3f800000);
  if (in_FPSCR_PR == '\0') {
    bVar17 = 1.0 < param_1;
  }
  else {
    bVar17 = dVar15 < (double)CONCAT44(param_1,param_2);
  }
  piVar11 = (int *)param_3[1];
  puVar13 = (uint *)*param_3;
  if (bVar17) {
    return;
  }
  *_DAT_8c5fe03c = 0;
  if (in_FPSCR_SZ == '\0') {
    *pfVar1 = param_1;
  }
  else {
    *(ulonglong *)pfVar1 = CONCAT44(param_1,param_2);
  }
  if (*piVar11 == 0) {
    *puVar3 = 0;
    *puVar5 = 0;
  }
  else {
    fVar14 = (float)*(int *)(*piVar11 + 4);
    dVar15 = (double)CONCAT44(uVar16,fVar14);
    if (*(int *)(*piVar11 + 4) < 0) {
      if (in_FPSCR_PR == '\0') {
        dVar15 = (double)CONCAT44(_DAT_8c5fe054,fVar14 + _DAT_8c5fe054);
      }
      else {
        dVar15 = (double)CONCAT44(_DAT_8c5fe054,fVar14) + (double)CONCAT44(_DAT_8c5fe054,fVar14);
      }
    }
    if (in_FPSCR_SZ == '\0') {
      in_fr1 = (float)piVar11[2];
    }
    if (in_FPSCR_PR == '\0') {
      bVar17 = in_fr1 < SUB84(dVar15,0);
    }
    else {
      bVar17 = (double)CONCAT44(in_fr0,in_fr1) < dVar15;
    }
    if (!bVar17) {
      return;
    }
    *puVar3 = *(ushort *)(*piVar11 + 10) & (ushort)_DAT_8c5fe058;
    *puVar4 = (ushort)((*(ushort *)(*piVar11 + 10) & 0xc0) != 0);
    *puVar5 = *(ushort *)(*piVar11 + 8);
    *puVar2 = *(undefined4 *)*piVar11;
    puVar2[4] = *(undefined4 *)(*piVar11 + 4);
    if (in_FPSCR_SZ == '\0') {
      dVar15 = (double)CONCAT44((int)((ulonglong)dVar15 >> 0x20),piVar11[2]);
      puVar2[3] = piVar11[2];
    }
    else {
      *(undefined8 *)(puVar2 + 3) = *(undefined8 *)(piVar11 + 2);
    }
  }
  puVar2[5] = 0;
  puVar2[6] = 0;
  if (piVar11[1] == 0) {
    puVar3[1] = 0;
    puVar5[1] = 0;
  }
  else {
    fVar14 = (float)*(int *)(piVar11[1] + 4);
    dVar15 = (double)CONCAT44((int)((ulonglong)dVar15 >> 0x20),fVar14);
    if (*(int *)(piVar11[1] + 4) < 0) {
      if (in_FPSCR_PR == '\0') {
        dVar15 = (double)CONCAT44(_DAT_8c5fe054,fVar14 + _DAT_8c5fe054);
      }
      else {
        dVar15 = (double)CONCAT44(_DAT_8c5fe054,fVar14) + (double)CONCAT44(_DAT_8c5fe054,fVar14);
      }
    }
    if (in_FPSCR_SZ == '\0') {
      in_fr1 = (float)piVar11[3];
    }
    if (in_FPSCR_PR == '\0') {
      bVar17 = in_fr1 < SUB84(dVar15,0);
    }
    else {
      bVar17 = (double)CONCAT44(in_fr0,in_fr1) < dVar15;
    }
    if (!bVar17) {
      return;
    }
    puVar3[1] = *(ushort *)(piVar11[1] + 10) & (ushort)_DAT_8c5fe058;
    *puVar4 = (ushort)((*(ushort *)(piVar11[1] + 10) & 0xc0) != 0);
    puVar5[1] = *(ushort *)(piVar11[1] + 8);
    puVar2[7] = *(undefined4 *)piVar11[1];
    puVar2[0xb] = *(undefined4 *)(piVar11[1] + 4);
    if (in_FPSCR_SZ == '\0') {
      puVar2[10] = piVar11[3];
    }
    else {
      *(undefined8 *)(puVar2 + 10) = *(undefined8 *)(piVar11 + 3);
    }
  }
  puVar8 = _DAT_8c5fe064;
  iVar7 = _DAT_8c5fe060;
  puVar6 = _DAT_8c5fe05c;
  puVar2[0xc] = 0;
  puVar2[0xd] = 0;
  *puVar6 = *(undefined4 *)(iVar7 + (uint)*puVar3 * 4);
  if ((*puVar13 & 0x20) == 0) {
    *puVar8 = 0;
  }
  else {
    *puVar8 = 1;
  }
  if ((*puVar5 & 1) == 0) {
    *_DAT_8c5fe068 = _DAT_8c5fe070;
  }
  else {
    *_DAT_8c5fe068 = _DAT_8c5fe06c;
  }
  if ((*puVar5 & 2) == 0) {
    *_DAT_8c5fe074 = _DAT_8c5fe07c;
  }
  else {
    *_DAT_8c5fe074 = _DAT_8c5fe078;
  }
  if ((*puVar5 & 4) == 0) {
    *_DAT_8c5fe080 = _DAT_8c5fe088;
  }
  else {
    *_DAT_8c5fe080 = _DAT_8c5fe084;
  }
  puVar2 = _DAT_8c5fe098;
  iVar7 = _DAT_8c5fe094;
  iVar12 = (uint)*puVar4 * 4;
  *_DAT_8c5fe090 = *(undefined4 *)(_DAT_8c5fe08c + iVar12);
  iVar9 = _DAT_8c5fe09c;
  *puVar2 = *(undefined4 *)(iVar7 + iVar12);
  iVar7 = _DAT_8c5fe0a4;
  *_DAT_8c5fe0a0 = *(undefined4 *)(iVar9 + iVar12);
  puVar2 = _DAT_8c5fe0ac;
  *_DAT_8c5fe0a8 = *(undefined4 *)(iVar7 + iVar12);
  *puVar2 = *(undefined4 *)((uint)puVar3[1] * 4 + _DAT_8c5fe060);
  if ((*puVar13 & 0x20) == 0) {
    *puVar8 = 0;
  }
  else {
    *puVar8 = 1;
  }
  if ((puVar5[1] & 1) == 0) {
    *_DAT_8c5fe0b0 = _DAT_8c5fe070;
  }
  else {
    *_DAT_8c5fe0b0 = _DAT_8c5fe06c;
  }
  if ((puVar5[1] & 2) == 0) {
    *_DAT_8c5fe0b4 = _DAT_8c5fe07c;
  }
  else {
    *_DAT_8c5fe0b4 = _DAT_8c5fe078;
  }
  if ((puVar5[1] & 4) == 0) {
    *_DAT_8c5fe0b8 = _DAT_8c5fe088;
  }
  else {
    *_DAT_8c5fe0b8 = _DAT_8c5fe084;
  }
  puVar2 = _DAT_8c5fe098;
  iVar7 = _DAT_8c5fe094;
  iVar12 = (uint)*puVar4 * 4;
  *_DAT_8c5fe090 = *(undefined4 *)(_DAT_8c5fe08c + iVar12);
  iVar9 = _DAT_8c5fe09c;
  *puVar2 = *(undefined4 *)(iVar7 + iVar12);
  iVar7 = _DAT_8c5fe0a4;
  *_DAT_8c5fe0a0 = *(undefined4 *)(iVar9 + iVar12);
  pcVar10 = _DAT_8c5fe0bc;
  *_DAT_8c5fe0a8 = *(undefined4 *)(iVar7 + iVar12);
  (*pcVar10)();
  func_0x8c5fdbec(puVar13);
                    /* WARNING: Could not recover jumptable at 0x8c5fe038. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  (*_DAT_8c5fe0c0)();
  return;
}

