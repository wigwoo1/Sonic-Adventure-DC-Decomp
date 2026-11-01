/* Ghidra draft only; not buildable/verified source. */

void FUN_8c64e58e(int param_1,int param_2)

{
  if (param_2 == 1) {
    *(int *)PTR_DAT_8c64e5d8 = *(int *)PTR_DAT_8c64e5d8 + param_1;
  }
  else if (param_2 == 0) {
    *(int *)PTR_DAT_8c64e5d8 = param_1;
  }
  else if (param_2 == 2) {
    *(int *)PTR_DAT_8c64e5d8 = *(int *)PTR_DAT_8c64e5dc - param_1;
  }
  return;
}

