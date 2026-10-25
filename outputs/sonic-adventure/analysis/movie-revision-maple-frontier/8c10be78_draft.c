/* Ghidra draft only; not buildable/verified source. */

/* WARNING: Restarted to delay deadcode elimination for space: stack */

undefined4 FUN_8c10be78(uint *param_1)

{
  int iVar1;
  int local_10;
  uint local_c;
  
  iVar1 = (*(code *)PTR_FUN_8c10bfd0)(DAT_8c10bfcc,&local_10,4,2,1,1);
  if (iVar1 != 0) {
    return 0xffffffff;
  }
  *param_1 = local_10 << 0x10 & DAT_8c10bfd4 | local_c & 0xffff;
  FUN_8c10c018(local_10);
  FUN_8c10c018(local_c);
  return 0;
}

