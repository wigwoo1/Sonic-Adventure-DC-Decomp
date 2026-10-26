/* Ghidra draft only; not buildable/verified source. */

uint FUN_8c654392(int param_1)

{
  uint uVar1;
  uint uVar2;
  uint uVar3;
  uint uVar4;
  int iVar5;
  
  uVar4 = *(uint *)(param_1 + 0xc) * *(uint *)(param_1 + 0x10);
  if ((*(uint *)(param_1 + 0x18) & 1) != 0) {
    uVar2 = *(uint *)(param_1 + 0xc) >> 1;
    uVar3 = *(uint *)(param_1 + 0x10) >> 1;
    uVar1 = uVar3;
    if (uVar2 < uVar3) {
      uVar1 = uVar2;
    }
    for (; 1 < uVar1; uVar1 = uVar1 >> 1) {
      iVar5 = uVar2 * uVar3;
      uVar2 = uVar2 >> 1;
      uVar3 = uVar3 >> 1;
      uVar4 = uVar4 + iVar5;
    }
    if (uVar2 == uVar3) {
      iVar5 = uVar4 + 1;
    }
    else {
      iVar5 = uVar4 + uVar2 * uVar3;
    }
    if (*(int *)(param_1 + 8) == DAT_8c6543e4) {
      uVar4 = iVar5 + 0x2b;
    }
    else {
      uVar4 = iVar5 + 0xb;
    }
  }
  if (*(int *)(param_1 + 8) == DAT_8c654500) {
    if (uVar4 == 1) {
      uVar4 = 1;
    }
    else {
      uVar4 = uVar4 >> 1;
    }
  }
  return uVar4;
}

