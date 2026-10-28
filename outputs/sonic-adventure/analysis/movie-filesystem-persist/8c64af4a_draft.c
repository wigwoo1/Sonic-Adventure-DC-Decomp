/* Ghidra draft only; not buildable/verified source. */

int FUN_8c64af4a(int param_1)

{
  uint uVar1;
  int iVar2;
  uint uVar3;
  int iVar4;
  int iVar5;
  int iVar6;
  int iVar7;
  
  uVar1 = DAT_8c64afd8;
  iVar4 = *(int *)(param_1 * DAT_8c64afc8 + DAT_8c64afcc + 0x1c);
  iVar5 = *(int *)(iVar4 + 0x1c);
  iVar6 = *(int *)(iVar4 + 0x10);
  iVar4 = *(int *)(iVar4 + 0x20) + iVar5 + -1;
  do {
    iVar2 = (*(code *)PTR_FUN_8c64afdc)(param_1,uVar1,iVar5,iVar4);
    if (iVar2 < 0) {
      return -1;
    }
    iVar7 = 0;
    if (0 < iVar6) {
      do {
        uVar3 = FUN_8c64b1c0(param_1,iVar2 - iVar7);
        if ((uVar3 & 0xffff) != uVar1) {
          iVar4 = (iVar2 - iVar7) + -1;
          break;
        }
        iVar7 = iVar7 + 1;
      } while (iVar7 < iVar6);
    }
    if (iVar7 == iVar6) {
      return (iVar2 - iVar6) + 1;
    }
  } while( true );
}

