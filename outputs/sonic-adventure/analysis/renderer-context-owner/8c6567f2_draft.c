/* Ghidra draft only; not buildable/verified source. */

int FUN_8c6567f2(void)

{
  int iVar1;
  undefined4 *puVar2;
  undefined4 *puVar3;
  
  iVar1 = (*(code *)PTR_FUN_8c656850)(*DAT_8c65684c,*DAT_8c656848);
  if (iVar1 != 0) {
    return iVar1;
  }
  puVar3 = DAT_8c656854 + 5;
  for (puVar2 = DAT_8c656854; puVar2 < puVar3; puVar2 = puVar2 + 1) {
    *puVar2 = 0;
  }
  return 0;
}

