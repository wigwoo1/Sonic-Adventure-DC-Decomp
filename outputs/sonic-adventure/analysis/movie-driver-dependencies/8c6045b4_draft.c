/* Ghidra draft only; not buildable/verified source. */

uint FUN_8c6045b4(uint param_1)

{
  uint uVar1;
  uint uVar2;
  uint uVar3;
  uint uVar4;
  undefined *puVar5;
  int iVar6;
  int *piVar7;
  int iVar8;
  int iVar9;
  int *piVar10;
  byte in_MD;
  byte bVar11;
  byte in_RB;
  byte bVar12;
  byte in_BL;
  byte bVar13;
  byte in_FD;
  byte bVar14;
  byte in_M;
  byte extraout_M;
  byte bVar15;
  byte in_Q;
  byte extraout_Q;
  byte bVar16;
  byte in_IMASK;
  byte bVar17;
  byte in_S;
  byte extraout_S;
  byte bVar18;
  byte in_T;
  int local_30;
  int *local_2c;
  uint local_28;
  uint local_24;
  int local_20 [4];
  int local_10;
  undefined1 *local_c;
  
  puVar5 = PTR_DAT_8c604668;
  local_c = &DAT_8c88f644;
  local_30 = 0;
  local_24 = (((uint)in_T | (uint)in_S << 1 | (uint)in_IMASK << 4) & 0xf0) >> 4;
  uVar1 = (uint)in_T | (uint)in_S << 1;
  uVar2 = uVar1 | (uint)in_IMASK << 4;
  uVar3 = uVar2 | (uint)in_Q << 8;
  uVar4 = uVar3 | (uint)in_M << 9;
  bVar13 = (byte)(((uint)in_BL << 0x1c) >> 0x18);
  bVar11 = bVar13 | (byte)(((uint)in_RB << 0x1d) >> 0x18);
  bVar18 = (byte)((uVar1 & 2) >> 1);
  bVar17 = (byte)((uVar2 & 0xf0) >> 4);
  bVar16 = (byte)(uVar3 >> 8) & 1;
  bVar15 = (byte)((uVar4 & 0x200) >> 9);
  bVar14 = (byte)(((uVar4 | (uint)in_FD << 0xf) & 0x8000) >> 0xf);
  bVar13 = (bVar13 & 0x10) >> 4;
  bVar12 = (bVar11 & 0x20) >> 5;
  bVar11 = ((bVar11 | (byte)(((uint)in_MD << 0x1e) >> 0x18)) & 0x40) >> 6;
  local_2c = local_20;
  iVar8 = 0;
  iVar9 = 0;
  piVar7 = local_2c;
  piVar10 = local_2c;
  do {
    iVar6 = iVar8 * 0x20;
    iVar8 = iVar8 + 1;
    if ((*(uint *)(iVar6 + *(int *)puVar5 + 0x18) & 1) == 0) {
      *piVar10 = 0;
    }
    else {
      *(undefined4 *)(*(int *)puVar5 + iVar9 + 0x1c) = 0;
      *piVar7 = 1;
    }
    piVar7 = piVar7 + 1;
    piVar10 = piVar10 + 1;
    iVar9 = iVar9 + 0x20;
    local_28 = param_1;
  } while (iVar8 < 4);
  while (local_30 != 1) {
    FUN_8c6045a8(&local_30);
    bVar15 = extraout_M;
    bVar16 = extraout_Q;
    bVar18 = extraout_S;
  }
  *DAT_8c604778 = *DAT_8c604778 & (int)DAT_8c60476c | local_28;
  iVar8 = 0;
  for (; &local_10 > local_2c; local_2c = local_2c + 1) {
    if (*local_2c == 1) {
      *(undefined4 *)(*(int *)puVar5 + iVar8 + 0x1c) = 1;
    }
    iVar8 = iVar8 + 0x20;
  }
  return (local_24 & 0xf) << 4 |
         ((uint)(&local_10 <= local_2c) | (uint)bVar18 << 1 | (uint)bVar17 << 4 | (uint)bVar16 << 8
          | (uint)bVar15 << 9 | (uint)bVar14 << 0xf | (uint)bVar13 << 0x1c | (uint)bVar12 << 0x1d |
         (uint)bVar11 << 0x1e) & (int)DAT_8c60476e;
}

