/* Ghidra draft only; not buildable/verified source. */

int FUN_8c65a378(void)

{
  int iVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  int iVar5;
  char local_24 [8];
  
  iVar5 = *(int *)PTR_DAT_8c65a444;
  iVar1 = DAT_8c65a448;
  if (((iVar5 != 0) &&
      (iVar4 = DAT_8c65a43a + iVar5, iVar1 = DAT_8c65a450, *(int *)(iVar4 + 8) != 1)) &&
     ((*(code *)PTR_FUN_8c65a44c)(*(undefined4 *)(DAT_8c65a43c + iVar5),local_24),
     iVar1 = DAT_8c65a450, local_24[0] == '\0')) {
    iVar2 = 0;
    iVar3 = *(int *)((int)DAT_8c65a43e + *(int *)PTR_DAT_8c65a444);
    iVar5 = 0;
    if (0 < iVar3) {
      do {
        iVar1 = *(int *)(iVar4 + 0x1c);
        *(int *)(iVar4 + 0x1c) = iVar1 + 1;
        if (*(int *)(iVar4 + 0x1c) == iVar3) {
          *(undefined4 *)(iVar4 + 0x1c) = 0;
        }
        if (*(int *)(iVar1 * 0x1c + *(int *)(iVar4 + 0xc) + 0x14) == 1) {
          iVar1 = (*(code *)PTR_FUN_8c65a454)();
          if (iVar1 != 0) goto LAB_8c65a418;
          iVar2 = iVar2 + 1;
          if ((iVar2 == 0x20) || (*(int *)((int)DAT_8c65a440 + *(int *)PTR_DAT_8c65a444) == 0))
          break;
        }
        iVar5 = iVar5 + 1;
      } while (iVar5 < iVar3);
    }
    iVar1 = (*(code *)PTR_FUN_8c65a458)();
  }
LAB_8c65a418:
  if (iVar1 != 0) {
    (*(code *)PTR_FUN_8c65a460)(PTR_FUN_8c65a45c,iVar1);
  }
  return iVar1;
}

