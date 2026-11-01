/* Ghidra draft only; not buildable/verified source. */

int FUN_8c64e8e4(void)

{
  int iVar1;
  uint uVar2;
  
  uVar2 = *DAT_8c64e944;
  iVar1 = *(int *)PTR_DAT_8c64e938;
  if (*(int *)(uVar2 * 0x44 + iVar1) != -1) {
    for (uVar2 = 0; (uVar2 < *(uint *)PTR_DAT_8c64e940 && (*(int *)(uVar2 * 0x44 + iVar1) != -1));
        uVar2 = uVar2 + 1) {
    }
  }
  return uVar2 * 0x44 + iVar1 + 0x14;
}

