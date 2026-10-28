/* Ghidra draft only; not buildable/verified source. */

int FUN_8c64a6fc(int param_1,int param_2)

{
  int *piVar1;
  uint uVar2;
  uint uVar3;
  int iVar4;
  int iVar5;
  undefined4 uVar6;
  
  uVar2 = DAT_8c64a778;
  piVar1 = DAT_8c64a774;
  uVar6 = 0xfffc;
  iVar4 = param_1 * DAT_8c64a76e + DAT_8c64a770;
  iVar5 = *(int *)(*(int *)(iVar4 + 0x1c) + 0x30);
  do {
    iVar5 = iVar5 + -1;
    if (iVar5 < param_2) {
      return -1;
    }
    uVar3 = (**(code **)(*piVar1 + 0xc))(*(undefined4 *)(iVar4 + 0x24),iVar5 * 2,uVar6);
  } while ((uVar3 & 0xffff) != uVar2);
  return iVar5;
}

