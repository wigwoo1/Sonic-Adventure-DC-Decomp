/* Ghidra draft only; not buildable/verified source. */

void FUN_8c63cdd8(void)

{
  int iVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  int iVar5;
  
  iVar2 = DAT_8c63ce20;
  iVar1 = DAT_8c63ce1c;
  iVar5 = *(int *)(DAT_8c63ce18 + DAT_8c63ce1c);
  if (0 < iVar5) {
    iVar4 = 0;
    if (0 < iVar5) {
      do {
        if (*(int *)(*(int *)(iVar4 * 4 + iVar1) + 0x58) == 1) {
          iVar3 = (int)DAT_8c63ce18;
          *(int *)(*(int *)(iVar3 + iVar2) * 4 + iVar2) = iVar4;
          *(int *)(iVar3 + iVar2) = *(int *)(iVar3 + iVar2) + 1;
        }
        iVar4 = iVar4 + 1;
      } while (iVar4 < iVar5);
    }
    FUN_8c63c6ba();
    return;
  }
  return;
}

