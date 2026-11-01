/* Ghidra draft only; not buildable/verified source. */

/* WARNING: Removing unreachable block (ram,0x8c64dfd6) */
/* WARNING: Removing unreachable block (ram,0x8c64dfa8) */
/* WARNING: Removing unreachable block (ram,0x8c64e018) */

void FUN_8c64df66(int param_1)

{
  int iVar1;
  uint uVar2;
  uint uVar3;
  uint uVar4;
  
  iVar1 = FUN_8c64df10(*(undefined4 *)(param_1 + 0x24),0);
  uVar2 = FUN_8c64df10(*(undefined4 *)(param_1 + 0x28));
  uVar3 = DAT_8c64e034;
  *(uint *)(param_1 + 8) = iVar1 << 3 | uVar2;
  uVar4 = *(uint *)(param_1 + 0x30);
  uVar2 = *(uint *)(param_1 + 0x18) >> 0x10 & DAT_8c64e04c;
  uVar3 = (((((uint)((uVar4 & 1) != 0) << 0x1f & uVar3 | (uVar4 & 8) << 0x1b) & DAT_8c64e038 |
            *(uint *)(param_1 + 0x20) & DAT_8c64e03c) & DAT_8c64e040 | (~(uVar4 & 4) & 4) << 0x18) &
           DAT_8c64e044 | (uVar4 & 0x10) << 0x15) & DAT_8c64e048 | *(uint *)(param_1 + 0x34) >> 3;
  if ((uVar2 == (int)DAT_8c64e02a) || (uVar2 == (int)DAT_8c64e02c)) {
    uVar2 = *(uint *)(param_1 + 4) & 0x3f;
  }
  else {
    if ((uVar2 != (int)DAT_8c64e02e) && (uVar2 != (int)DAT_8c64e030)) goto LAB_8c64e01e;
    uVar2 = *(uint *)(param_1 + 4) & 0x30;
  }
  uVar3 = uVar2 << 0x15 | DAT_8c64e050 & uVar3;
LAB_8c64e01e:
  *(uint *)(param_1 + 0xc) = uVar3;
  return;
}

