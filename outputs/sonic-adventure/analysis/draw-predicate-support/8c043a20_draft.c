/* Ghidra draft only; not buildable/verified source. */

void FUN_8c043a20(undefined4 param_1,undefined4 param_2,undefined4 param_3,undefined4 param_4)

{
  undefined4 in_xf4;
  undefined8 in_xd6;
  undefined4 in_PR;
  char in_FPSCR_SZ;
  undefined4 local_14;
  undefined4 uStack_10;
  undefined4 uStack_c;
  undefined4 uStack_8;
  undefined4 local_4;
  
  local_14 = param_1;
  uStack_10 = param_2;
  uStack_c = param_3;
  uStack_8 = param_4;
  local_4 = in_PR;
  if (in_FPSCR_SZ != '\0') {
    uStack_8 = (undefined4)in_xd6;
    local_4 = (undefined4)((ulonglong)in_xd6 >> 0x20);
    local_14 = param_2;
    uStack_10 = in_xf4;
    uStack_c = param_4;
  }
  (*(code *)PTR_FUN_8c043be0)(&local_14);
  return;
}

