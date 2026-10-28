/* Ghidra draft only; not buildable/verified source. */

int FUN_8c64b080(int param_1,short param_2)

{
  int *piVar1;
  short sVar2;
  int iVar3;
  int iVar4;
  undefined4 uVar5;
  
  piVar1 = DAT_8c64b0dc;
  uVar5 = 0xfffa;
  iVar4 = param_1 * DAT_8c64b0d2 + DAT_8c64b0d8;
  iVar3 = *(int *)(*(int *)(iVar4 + 0x1c) + 0x30);
  do {
    iVar3 = iVar3 + -1;
    if (iVar3 < 0) {
      return -1;
    }
    sVar2 = (**(code **)(*piVar1 + 0xc))(*(undefined4 *)(iVar4 + 0x24),iVar3 * 2,uVar5);
  } while (sVar2 != param_2);
  return iVar3;
}

