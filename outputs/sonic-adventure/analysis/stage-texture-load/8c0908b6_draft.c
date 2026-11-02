/* Ghidra draft only; not buildable/verified source. */

int FUN_8c0908b6(void)

{
  int iVar1;
  
  iVar1 = (*(code *)PTR_FUN_8c090950)();
  return -((*(char *)(DAT_8c090930 + *(short *)(PTR_DAT_8c09095c + iVar1 * 2)) == '\0') - 1);
}

