/* Ghidra draft only; not buildable/verified source. */

uint callback_8c5fc848(ushort param_1)

{
  uint uVar1;
  uint uVar2;
  uint uVar3;
  uint uVar4;
  byte in_MD;
  byte in_RB;
  byte in_BL;
  byte in_FD;
  byte in_M;
  byte in_Q;
  byte in_IMASK;
  byte in_S;
  bool bVar5;
  
  uVar1 = DAT_8c5fc89c;
  if ((*DAT_8c5fc8a0 >> 5 & 1) == 1) {
    uVar4 = (uint)DAT_8c5fc894;
    uVar3 = 0;
    do {
      uVar2 = uVar3 + 0x20;
      *(undefined4 *)(uVar3 | uVar1) = 0;
      uVar3 = uVar2;
    } while (uVar2 < uVar4);
    uVar4 = (uint)DAT_8c5fc896;
    uVar3 = (int)DAT_8c5fc898;
    do {
      bVar5 = uVar4 <= uVar3 + 0x20;
      *(undefined4 *)(uVar3 | uVar1) = 0;
      uVar3 = uVar3 + 0x20;
    } while (!bVar5);
  }
  else {
    uVar4 = (uint)DAT_8c5fc89a;
    uVar3 = 0;
    do {
      bVar5 = uVar4 <= uVar3 + 0x20;
      *(undefined4 *)(uVar3 | uVar1) = 0;
      uVar3 = uVar3 + 0x20;
    } while (!bVar5);
  }
  return (param_1 & 0xf) << 4 |
         ((uint)bVar5 | (uint)in_S << 1 | (uint)in_IMASK << 4 | (uint)in_Q << 8 | (uint)in_M << 9 |
          (uint)in_FD << 0xf | (uint)in_BL << 0x1c | (uint)in_RB << 0x1d | (uint)in_MD << 0x1e) &
         (int)DAT_8c5fc93a;
}

