/* Ghidra draft only; not buildable/verified source. */

undefined4 FUN_8c652d3a(uint param_1,uint param_2)

{
  char in_FPSCR_SZ;
  uint local_8;
  
  local_8 = param_2;
  if (in_FPSCR_SZ == '\0') {
    local_8 = param_1;
  }
  (*(code *)PTR_FUN_8c652d9c)(0x78,local_8 & DAT_8c652da0);
  return 0;
}

