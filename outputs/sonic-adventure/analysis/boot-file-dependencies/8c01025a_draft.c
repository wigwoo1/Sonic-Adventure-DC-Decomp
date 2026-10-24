/* Ghidra draft only; not buildable/verified source. */

uint FUN_8c01025a(void)

{
  byte in_MD;
  byte in_RB;
  byte in_BL;
  byte in_FD;
  byte extraout_M;
  byte extraout_Q;
  byte in_IMASK;
  byte extraout_S;
  byte extraout_T;
  
  (*(code *)PTR_FUN_8c0102f0)();
  (*(code *)PTR_FUN_8c0102f4)();
  (*(code *)PTR_FUN_8c0102f8)();
  (*(code *)PTR_FUN_8c0102fc)();
  (*(code *)PTR_FUN_8c010300)();
  (*(code *)PTR_FUN_8c010304)();
  return ((uint)extraout_T | (uint)extraout_S << 1 | (uint)in_IMASK << 4) & 0xffffff0f |
         (uint)extraout_Q << 8 | (uint)extraout_M << 9 | (uint)in_FD << 0xf | (uint)in_BL << 0x1c |
         (uint)in_RB << 0x1d | (uint)in_MD << 0x1e | 0xf0;
}

