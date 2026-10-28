/* Ghidra draft only; not buildable/verified source. */

int FUN_8c64a6ae(int param_1)

{
  int *piVar1;
  uint uVar2;
  int iVar3;
  int iVar4;
  undefined4 uVar5;
  
  piVar1 = DAT_8c64a774;
  uVar5 = 0xfffc;
  iVar3 = param_1 * DAT_8c64a76e + DAT_8c64a770;
  iVar4 = 0;
  while( true ) {
    if (*(int *)(*(int *)(iVar3 + 0x1c) + 0x30) <= iVar4) {
      return -1;
    }
    uVar2 = (**(code **)(*piVar1 + 0xc))(*(undefined4 *)(iVar3 + 0x24),iVar4 << 1,uVar5);
    if (((uVar2 & 0xffff) != DAT_8c64a778) && ((uVar2 & 0xffff) != DAT_8c64a77c)) break;
    iVar4 = iVar4 + 1;
  }
  return iVar4;
}

