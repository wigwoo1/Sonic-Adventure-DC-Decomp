/* Ghidra draft only; not buildable/verified source. */

int FUN_8c64ad56(int param_1,undefined4 param_2,undefined4 param_3)

{
  int iVar1;
  int iVar2;
  int iVar3;
  byte *pbVar4;
  undefined4 uVar5;
  
  uVar5 = 0;
  iVar2 = param_1 * DAT_8c64ada0 + DAT_8c64ada8;
  pbVar4 = *(byte **)(iVar2 + 0x28);
  iVar3 = 0;
  while( true ) {
    if (*(int *)(*(int *)(iVar2 + 0x1c) + 0x30) <= iVar3) {
      return -1;
    }
    if (((*pbVar4 == 0x33) || (*pbVar4 == DAT_8c64ada2)) &&
       (iVar1 = FUN_8c64ac90(param_2,pbVar4 + 4,uVar5), iVar1 != 0)) break;
    pbVar4 = pbVar4 + 0x20;
    iVar3 = iVar3 + 1;
  }
  FUN_8c64acda(pbVar4,param_3);
  return iVar3;
}

