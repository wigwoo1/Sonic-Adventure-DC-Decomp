/* Ghidra draft only; not buildable/verified source. */

/* WARNING: Removing unreachable block (ram,0x8c64bdb0) */

uint FUN_8c64bda0(int param_1,byte *param_2)

{
  byte bVar1;
  uint uVar2;
  
  uVar2 = DAT_8c64bdd4;
  while (param_1 = param_1 + -1, -1 < param_1) {
    bVar1 = *param_2;
    param_2 = param_2 + 1;
    uVar2 = (uVar2 & 0xffff) << 8 ^
            (int)*(short *)(PTR_DAT_8c64bdd8 + ((int)(uVar2 & 0xffff) >> 8 ^ (uint)bVar1) * 2);
  }
  return ~(uVar2 & 0xffff) & 0xffff;
}

