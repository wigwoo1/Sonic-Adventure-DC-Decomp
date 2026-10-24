/* Ghidra draft only; not buildable/verified source. */

uint FUN_8c5fc06c(uint param_1)

{
  uint *puVar1;
  uint uVar2;
  uint uVar3;
  byte in_MD;
  byte bVar4;
  byte in_RB;
  byte bVar5;
  byte in_BL;
  byte bVar6;
  byte in_FD;
  byte bVar7;
  byte in_M;
  byte extraout_M;
  byte in_Q;
  byte extraout_Q;
  byte in_IMASK;
  byte bVar8;
  byte in_S;
  byte extraout_S;
  byte in_T;
  byte extraout_T;
  
  bVar6 = (byte)(((uint)in_BL << 0x1c) >> 0x18);
  bVar4 = bVar6 | (byte)(((uint)in_RB << 0x1d) >> 0x18);
  bVar8 = (byte)((((uint)in_T | (uint)in_S << 1 | (uint)in_IMASK << 4) & 0xf0) >> 4);
  bVar7 = (byte)((((uint)in_Q << 8 | (uint)in_M << 9 | (uint)in_FD << 0xf) & 0x8000) >> 0xf);
  bVar6 = (bVar6 & 0x10) >> 4;
  bVar5 = (bVar4 & 0x20) >> 5;
  bVar4 = ((bVar4 | (byte)(((uint)in_MD << 0x1e) >> 0x18)) & 0x40) >> 6;
  (*(code *)PTR_FUN_8c5fc110)();
  puVar1 = DAT_8c5fc114;
  uVar2 = (uint)DAT_8c5fc10e;
  *DAT_8c5fc114 = param_1;
  uVar3 = (uint)DAT_8c5fc10a;
  *puVar1 = *puVar1 & (int)DAT_8c5fc10c | uVar2;
  *puVar1 = *puVar1 & 0xfffffff7 | 8;
  return (int)(short)((((uint)in_T | (uint)in_S << 1 | (uint)in_IMASK << 4) & 0xf0) >> 4) << 4 |
         ((uint)extraout_T | (uint)extraout_S << 1 | (uint)bVar8 << 4 | (uint)extraout_Q << 8 |
          (uint)extraout_M << 9 | (uint)bVar7 << 0xf | (uint)bVar6 << 0x1c | (uint)bVar5 << 0x1d |
         (uint)bVar4 << 0x1e) & uVar3;
}

