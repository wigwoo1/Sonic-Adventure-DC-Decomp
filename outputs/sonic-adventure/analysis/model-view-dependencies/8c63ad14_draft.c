/* Ghidra draft only; not buildable/verified source. */

void FUN_8c63ad14(void)

{
  undefined4 uVar1;
  int iVar2;
  undefined4 uVar3;
  int iVar4;
  int iVar5;
  int *piVar6;
  
  uVar3 = DAT_8c63ad98;
  iVar2 = DAT_8c63ad94;
  uVar1 = DAT_8c63ad90;
  iVar5 = *(int *)(DAT_8c63ad84 + DAT_8c63ad8c);
  if (iVar5 != 0) {
    piVar6 = (int *)(iVar5 * 4 + DAT_8c63ad8c);
    do {
      piVar6 = piVar6 + -1;
      iVar4 = *(int *)(*piVar6 * 4 + iVar2);
      (*(code *)PTR_FUN_8c63ad9c)(uVar3,uVar1,0x10);
      (*(code *)PTR_FUN_8c63ada0)(uVar3,iVar4);
      (*(code *)PTR_FUN_8c63ada4)(uVar3,iVar4 + 0x40,DAT_8c63ad86 + iVar4);
      (*(code *)PTR_FUN_8c63ada8)(uVar3,iVar4 + 0x4c,DAT_8c63ad88 + iVar4);
      iVar5 = iVar5 + -1;
    } while (iVar5 != 0);
  }
  return;
}

