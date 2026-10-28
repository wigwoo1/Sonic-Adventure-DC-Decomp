/* Ghidra draft only; not buildable/verified source. */

int FUN_8c64adc6(int param_1)

{
  int iVar1;
  byte *pbVar2;
  int iVar3;
  
  iVar1 = param_1 * DAT_8c64aeba + DAT_8c64aec0;
  pbVar2 = *(byte **)(iVar1 + 0x28);
  iVar3 = 0;
  while( true ) {
    if (*(int *)(*(int *)(iVar1 + 0x1c) + 0x30) <= iVar3) {
      return -1;
    }
    if (*pbVar2 == DAT_8c64aebc) break;
    pbVar2 = pbVar2 + 0x20;
    iVar3 = iVar3 + 1;
  }
  FUN_8c64acda(pbVar2);
  return iVar3;
}

