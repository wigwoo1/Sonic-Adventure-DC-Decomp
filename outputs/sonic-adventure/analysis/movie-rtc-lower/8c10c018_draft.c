/* Ghidra draft only; not buildable/verified source. */

void FUN_8c10c018(undefined4 param_1)

{
  int *piVar1;
  
  piVar1 = DAT_8c10c03c;
  if ((*DAT_8c10c038 & 8) != 0) {
    if ((undefined4 *)*DAT_8c10c03c < (undefined4 *)*DAT_8c10c040) {
      *(undefined4 *)*DAT_8c10c03c = param_1;
      *piVar1 = *piVar1 + 4;
    }
  }
  return;
}

