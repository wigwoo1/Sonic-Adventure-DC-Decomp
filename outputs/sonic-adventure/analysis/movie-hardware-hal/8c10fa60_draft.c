/* Ghidra draft only; not buildable/verified source. */

undefined4 FUN_8c10fa60(int *param_1)

{
  int local_c [2];
  
  (*(code *)PTR_FUN_8c10fa98)(local_c);
  if (local_c[0] == 1) {
    *(undefined4 *)(**(int **)PTR_DAT_8c10fa9c + *param_1 * 0x20 + 0x1c) = 1;
    param_1[6] = 2;
  }
  return 0;
}

