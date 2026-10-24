/* Ghidra draft only; not buildable/verified source. */

uint FUN_8c6412c2(int param_1,uint param_2,int param_3,undefined4 param_4)

{
  uint uVar1;
  uint uVar2;
  uint uVar3;
  uint uVar4;
  int iVar5;
  byte in_MD;
  byte bVar6;
  byte in_RB;
  byte bVar7;
  byte in_BL;
  byte bVar8;
  byte in_FD;
  byte bVar9;
  byte in_M;
  byte extraout_M;
  byte bVar10;
  byte in_Q;
  byte extraout_Q;
  byte bVar11;
  byte in_IMASK;
  byte bVar12;
  byte in_S;
  byte extraout_S;
  byte bVar13;
  byte in_T;
  byte bVar14;
  byte extraout_T;
  
  uVar1 = (uint)in_T | (uint)in_S << 1;
  uVar2 = uVar1 | (uint)in_IMASK << 4;
  uVar3 = uVar2 | (uint)in_Q << 8;
  uVar4 = uVar3 | (uint)in_M << 9;
  bVar8 = (byte)(((uint)in_BL << 0x1c) >> 0x18);
  bVar6 = bVar8 | (byte)(((uint)in_RB << 0x1d) >> 0x18);
  bVar13 = (byte)((uVar1 & 2) >> 1);
  bVar12 = (byte)((uVar2 & 0xf0) >> 4);
  bVar11 = (byte)(uVar3 >> 8) & 1;
  bVar10 = (byte)((uVar4 & 0x200) >> 9);
  bVar9 = (byte)(((uVar4 | (uint)in_FD << 0xf) & 0x8000) >> 0xf);
  bVar8 = (bVar8 & 0x10) >> 4;
  bVar7 = (bVar6 & 0x20) >> 5;
  bVar6 = ((bVar6 | (byte)(((uint)in_MD << 0x1e) >> 0x18)) & 0x40) >> 6;
  if (param_3 == 0) {
    FUN_8c6411b0();
    bVar10 = extraout_M;
    bVar11 = extraout_Q;
    bVar13 = extraout_S;
    bVar14 = extraout_T;
  }
  else {
    bVar14 = (param_2 & 0x20000000) != 0;
    iVar5 = (int)DAT_8c64130a;
    *(int *)(param_2 * 8 + DAT_8c64130a + param_1) = param_3;
    *(undefined4 *)(iVar5 + param_1 + param_2 * 8 + 4) = param_4;
  }
  return ((uint)in_T | (uint)in_S << 1 | (uint)in_IMASK << 4) & 0xf0 |
         ((uint)bVar14 | (uint)bVar13 << 1 | (uint)bVar12 << 4 | (uint)bVar11 << 8 |
          (uint)bVar10 << 9 | (uint)bVar9 << 0xf | (uint)bVar8 << 0x1c | (uint)bVar7 << 0x1d |
         (uint)bVar6 << 0x1e) & (int)DAT_8c6413c0;
}

