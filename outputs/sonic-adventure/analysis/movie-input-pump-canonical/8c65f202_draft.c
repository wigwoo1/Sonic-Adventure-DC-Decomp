/* Ghidra draft only; not buildable/verified source. */

undefined4 FUN_8c65f202(void)

{
  int *piVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  int iVar5;
  
  piVar1 = DAT_8c65f290;
  for (iVar5 = 0; iVar5 < 4; iVar5 = iVar5 + 1) {
    if (*(char *)(*(int *)(PTR_DAT_8c65f298 + iVar5 * 4) + (int)DAT_8c65f284 + *piVar1) != '\0') {
      (*(code *)PTR_FUN_8c65f29c)(iVar5);
      iVar2 = 0;
      iVar3 = iVar5 * 6;
      iVar4 = iVar3;
      do {
        if (*(char *)((int)DAT_8c65f284 + *piVar1 + iVar3) != '\0') {
          (*(code *)PTR_FUN_8c65f2a0)(iVar4,iVar5);
        }
        iVar2 = iVar2 + 1;
        iVar4 = iVar4 + 1;
        iVar3 = iVar3 + 1;
      } while (iVar2 < 6);
    }
  }
  return 0;
}

