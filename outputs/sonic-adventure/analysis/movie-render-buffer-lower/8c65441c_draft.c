/* Ghidra draft only; not buildable/verified source. */

int FUN_8c65441c(int param_1)

{
  uint uVar1;
  int iVar2;
  uint uVar3;
  uint uVar4;
  int iVar5;
  int iVar6;
  
  uVar3 = *(uint *)(param_1 + 0xc);
  uVar4 = *(uint *)(param_1 + 0x10);
  iVar5 = (uVar3 >> 1) * (uVar4 >> 1);
  iVar2 = FUN_8c6541e0(uVar3,uVar4,1);
  if ((*(uint *)(param_1 + 0x18) & 1) == 0) {
    iVar5 = iVar5 + *(int *)(PTR_DAT_8c654508 + iVar2 * 4);
  }
  else {
    uVar3 = uVar3 >> 2;
    uVar4 = uVar4 >> 2;
    uVar1 = uVar4;
    if (uVar3 < uVar4) {
      uVar1 = uVar3;
    }
    for (; 1 < uVar1; uVar1 = uVar1 >> 1) {
      iVar6 = uVar3 * uVar4;
      uVar3 = uVar3 >> 1;
      uVar4 = uVar4 >> 1;
      iVar5 = iVar5 + iVar6;
    }
    if (uVar3 == uVar4) {
      iVar5 = iVar5 + 2;
    }
    else {
      iVar5 = iVar5 + uVar3 * uVar4;
    }
    iVar5 = iVar5 + 10 + *(int *)(PTR_DAT_8c654504 + iVar2 * 4);
  }
  return iVar5;
}

