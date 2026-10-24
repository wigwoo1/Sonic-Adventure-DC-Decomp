/* Ghidra draft only; not buildable/verified source. */

void FUN_8c6046c0(uint param_1)

{
  uint local_10;
  uint local_c;
  
  local_c = param_1;
  (*(code *)PTR_FUN_8c60477c)(DAT_8c604778,&local_10,4,1,0,0);
  local_10 = local_10 & (int)DAT_8c60476c | local_c;
  (*(code *)PTR_FUN_8c604780)(&local_10,DAT_8c604778,4,1,0,0);
  return;
}

