/* Ghidra draft only; not buildable/verified source. */

int FUN_8c64a4b2(int param_1)

{
  int iVar1;
  char *pcVar2;
  int iVar3;
  int iVar4;
  
  iVar3 = (int)DAT_8c64a53e;
  iVar4 = param_1 * iVar3 + DAT_8c64a548;
  if (*(char *)(iVar4 + 2) != '\0') {
    iVar1 = (*(code *)PTR_FUN_8c64a54c)(param_1);
    if (iVar1 == 0) {
      (*(code *)PTR_FUN_8c64a550)(param_1);
    }
    else {
      iVar1 = (*(code *)PTR_FUN_8c64a554)(param_1);
      if ((iVar1 != 0) && (*(char *)(param_1 * iVar3 + DAT_8c64a548 + 8) != '\0')) {
        pcVar2 = *(char **)(iVar4 + 0x20);
        iVar3 = 0;
        do {
          if (*pcVar2 != 'U') {
            *(undefined1 *)(iVar4 + 0xb) = 0;
            *(undefined1 *)(iVar4 + 0xc) = 0;
            goto LAB_8c64a528;
          }
          iVar3 = iVar3 + 1;
          pcVar2 = pcVar2 + 1;
        } while (iVar3 < 0x10);
        if (*(int *)(DAT_8c64a558 + param_1 * 4) == 0) {
          return 0;
        }
LAB_8c64a528:
        return (int)DAT_8c64a53a;
      }
    }
  }
  return (int)DAT_8c64a540;
}

