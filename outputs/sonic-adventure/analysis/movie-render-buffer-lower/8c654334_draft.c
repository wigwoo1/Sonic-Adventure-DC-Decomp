/* Ghidra draft only; not buildable/verified source. */

int FUN_8c654334(int param_1)

{
  uint uVar1;
  uint uVar2;
  uint uVar3;
  int iVar4;
  int iVar5;
  
  iVar4 = (*(uint *)(param_1 + 0xc) >> 1) * (*(uint *)(param_1 + 0x10) >> 1);
  if ((*(uint *)(param_1 + 0x18) & 1) == 0) {
    if ((*(int *)(param_1 + 0xc) == 8) && (*(int *)(param_1 + 0x10) == 8)) {
      iVar4 = iVar4 + 0x10;
    }
  }
  else {
    uVar3 = *(uint *)(param_1 + 0xc) >> 2;
    uVar2 = *(uint *)(param_1 + 0x10) >> 2;
    uVar1 = uVar2;
    if (uVar3 < uVar2) {
      uVar1 = uVar3;
    }
    for (; 1 < uVar1; uVar1 = uVar1 >> 1) {
      iVar5 = uVar3 * uVar2;
      uVar3 = uVar3 >> 1;
      uVar2 = uVar2 >> 1;
      iVar4 = iVar4 + iVar5;
    }
    if (uVar3 == uVar2) {
      iVar4 = iVar4 + 2;
    }
    else {
      iVar4 = iVar4 + uVar3 * uVar2;
    }
    iVar4 = iVar4 + 10;
  }
  return iVar4 + DAT_8c6543e0;
}

