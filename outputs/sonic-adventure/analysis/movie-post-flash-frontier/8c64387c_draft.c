/* Ghidra draft only; not buildable/verified source. */

int FUN_8c64387c(int param_1)

{
  undefined1 *puVar1;
  int iVar2;
  bool bVar3;
  
  puVar1 = DAT_8c6438e8;
  iVar2 = DAT_8c6438e0;
  if (*DAT_8c6438dc != 0) {
    bVar3 = param_1 == DAT_8c6438ec;
    *DAT_8c6438e4 = (int)DAT_8c6438d8;
    if (bVar3) {
      *puVar1 = (char)DAT_8c6438da;
    }
    else {
      iVar2 = DAT_8c6438f4;
      if (param_1 != DAT_8c6438f0) goto LAB_8c6438c4;
      *puVar1 = 0;
    }
    iVar2 = (*(code *)PTR_FUN_8c6438f8)(DAT_8c6438e4);
  }
LAB_8c6438c4:
  if (iVar2 != 0) {
    (*(code *)PTR_FUN_8c643900)(PTR_FUN_8c6438fc,iVar2);
  }
  return iVar2;
}

