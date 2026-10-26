/* Ghidra draft only; not buildable/verified source. */

int FUN_8c6436e8(undefined1 *param_1)

{
  undefined1 uVar1;
  int iVar2;
  char cVar3;
  
  iVar2 = DAT_8c64373c;
  if (*DAT_8c643738 != 0) {
    *DAT_8c643740 = (int)DAT_8c643734;
    for (cVar3 = '\0'; cVar3 < '\a'; cVar3 = cVar3 + '\x01') {
      uVar1 = *param_1;
      param_1 = param_1 + 1;
      *(undefined1 *)(DAT_8c643744 + cVar3) = uVar1;
    }
    iVar2 = (*(code *)PTR_FUN_8c643748)(DAT_8c643740);
  }
  if (iVar2 != 0) {
    (*(code *)PTR_FUN_8c643750)(PTR_FUN_8c64374c,iVar2);
  }
  return iVar2;
}

