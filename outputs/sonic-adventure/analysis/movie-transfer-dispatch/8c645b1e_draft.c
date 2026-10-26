/* Ghidra draft only; not buildable/verified source. */

uint callback_8c645b1e(void)

{
  uint uVar1;
  undefined4 *puVar2;
  byte in_MD;
  byte in_RB;
  byte in_BL;
  byte in_FD;
  byte in_M;
  byte in_Q;
  byte in_IMASK;
  byte in_S;
  byte in_T;
  
  uVar1 = ((uint)in_T | (uint)in_S << 1 | (uint)in_IMASK << 4 | (uint)in_Q << 8 | (uint)in_M << 9 |
           (uint)in_FD << 0xf | (uint)in_BL << 0x1c | (uint)in_RB << 0x1d | (uint)in_MD << 0x1e) &
          (int)DAT_8c645b48 | 0x60;
  for (puVar2 = (undefined4 *)*DAT_8c645b64; puVar2 != (undefined4 *)0x0;
      puVar2 = (undefined4 *)puVar2[1]) {
    uVar1 = (*(code *)*puVar2)(puVar2[2]);
  }
  return uVar1;
}

