/* Ghidra draft only; not buildable/verified source. */

undefined4 FUN_8c608d88(int param_1)

{
  undefined4 local_10;
  undefined4 local_c;
  int local_8;
  
  if (param_1 == 2) {
    return *DAT_8c608e58;
  }
  local_8 = param_1;
  (*(code *)PTR_FUN_8c608e5c)(&local_10,&local_c);
  if (local_8 == 0) {
    return local_10;
  }
  return local_c;
}

