/* Ghidra draft only; not buildable/verified source. */

undefined8 FUN_8c646304(undefined4 param_1,undefined4 param_2)

{
  undefined4 uVar1;
  byte in_MD;
  byte bVar2;
  byte in_RB;
  byte bVar3;
  byte in_BL;
  byte bVar4;
  byte in_FD;
  byte bVar5;
  byte in_M;
  byte extraout_M;
  byte in_Q;
  byte extraout_Q;
  byte in_IMASK;
  byte bVar6;
  byte in_S;
  byte extraout_S;
  byte in_T;
  byte extraout_T;
  
  bVar4 = (byte)(((uint)in_BL << 0x1c) >> 0x18);
  bVar2 = bVar4 | (byte)(((uint)in_RB << 0x1d) >> 0x18);
  bVar6 = (byte)((((uint)in_T | (uint)in_S << 1 | (uint)in_IMASK << 4) & 0xf0) >> 4);
  bVar5 = (byte)((((uint)in_Q << 8 | (uint)in_M << 9 | (uint)in_FD << 0xf) & 0x8000) >> 0xf);
  bVar4 = (bVar4 & 0x10) >> 4;
  bVar3 = (bVar2 & 0x20) >> 5;
  bVar2 = ((bVar2 | (byte)(((uint)in_MD << 0x1e) >> 0x18)) & 0x40) >> 6;
  FUN_8c6461f0((((uint)in_T | (uint)in_S << 1 | (uint)in_IMASK << 4) & 0xf0) >> 4);
  uVar1 = (*(code *)PTR_FUN_8c6463cc)(param_2,DAT_8c6463c8,param_1);
  FUN_8c6461ae();
  *DAT_8c6463d0 = 1;
  return CONCAT44(((uint)extraout_T | (uint)extraout_S << 1 | (uint)bVar6 << 4 |
                   (uint)extraout_Q << 8 | (uint)extraout_M << 9 | (uint)bVar5 << 0xf |
                   (uint)bVar4 << 0x1c | (uint)bVar3 << 0x1d | (uint)bVar2 << 0x1e) &
                  (int)DAT_8c6463c4,uVar1);
}

