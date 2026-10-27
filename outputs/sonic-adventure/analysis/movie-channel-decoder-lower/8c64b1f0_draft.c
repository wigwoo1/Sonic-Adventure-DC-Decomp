/* Ghidra draft only; not buildable/verified source. */

int FUN_8c64b1f0(int param_1)

{
  int iVar1;
  int iVar2;
  
  iVar2 = param_1 * DAT_8c64b2e0 + DAT_8c64b2e4;
  *(undefined4 *)(DAT_8c64b2e8 + param_1 * 4) = 0;
  if ((*(char *)(iVar2 + 10) != '\0') || (iVar1 = FUN_8c64ae32(param_1,0), -1 < iVar1)) {
    iVar1 = FUN_8c64a4b2(param_1);
    if (-1 < iVar1) {
      if (((*(char *)(iVar2 + 0xb) == '\0') && (iVar1 = FUN_8c64ae62(param_1), iVar1 < 0)) ||
         ((*(char *)(iVar2 + 0xc) == '\0' && (iVar1 = FUN_8c64a9a8(param_1), iVar1 < 0)))) {
        if (iVar1 != DAT_8c64b2ec) {
          return iVar1;
        }
        *(undefined4 *)(DAT_8c64b2e8 + param_1 * 4) = 1;
      }
      else {
        *(undefined1 *)(iVar2 + 10) = 1;
        *(undefined1 *)(iVar2 + 0xb) = 1;
        *(undefined1 *)(iVar2 + 0xc) = 1;
      }
    }
    iVar1 = 0;
  }
  return iVar1;
}

