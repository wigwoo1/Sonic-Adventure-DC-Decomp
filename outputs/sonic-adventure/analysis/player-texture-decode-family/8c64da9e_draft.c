/* Ghidra draft only; not buildable/verified source. */

/* WARNING: Removing unreachable block (ram,0x8c64dad8) */

void FUN_8c64da9e(int param_1,uint param_2)

{
  uint uVar1;
  
  uVar1 = *(uint *)(param_1 + 0x18) >> 0x10 & DAT_8c64db98;
  if ((uVar1 == (int)DAT_8c64db86) || (uVar1 == (int)DAT_8c64db88)) {
    uVar1 = 0x3f;
  }
  else {
    if ((uVar1 != (int)DAT_8c64db8a) && (uVar1 != (int)DAT_8c64db8c)) {
      return;
    }
    uVar1 = 0x30;
  }
  *(uint *)(param_1 + 0xc) = *(uint *)(param_1 + 0xc) & DAT_8c64db9c;
  *(uint *)(param_1 + 0xc) = *(uint *)(param_1 + 0xc) | (uVar1 & param_2) << 0x15;
  *(uint *)(param_1 + 4) = param_2;
  return;
}

