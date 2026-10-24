/* Ghidra draft only; not buildable/verified source. */

undefined4 FUN_8c10f9bc(int *param_1)

{
  int local_c;
  int *local_8;
  
  local_8 = param_1;
  (*(code *)PTR_FUN_8c10f9ec)(&local_c);
  if (local_c == 1) {
    *(undefined4 *)(**(int **)PTR_DAT_8c10f9f0 + *local_8 * 0x20 + 0x1c) = 0;
  }
  return 0;
}

