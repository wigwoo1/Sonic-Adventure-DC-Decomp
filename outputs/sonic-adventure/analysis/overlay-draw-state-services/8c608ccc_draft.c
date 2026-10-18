/* Ghidra draft only; not buildable/verified source. */

undefined4 FUN_8c608ccc(int param_1)

{
  if (param_1 == 0) {
    return 0xffffffff;
  }
  *(int *)PTR_DAT_8c608d10 = param_1;
  return 1;
}

