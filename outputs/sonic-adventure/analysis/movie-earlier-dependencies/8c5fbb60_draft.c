/* Ghidra draft only; not buildable/verified source. */

undefined4 FUN_8c5fbb60(int param_1,int param_2)

{
  if (param_1 == 0) {
    return 0xffffffff;
  }
  param_1 = param_2 * 4 + param_1;
  *(undefined4 *)PTR_DAT_8c5fbd0c = DAT_8c5fbd08;
  *(int *)PTR_DAT_8c5fbd10 = param_1;
  FUN_8c5fbcb6(param_1);
  return 0;
}

