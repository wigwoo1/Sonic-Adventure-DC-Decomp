/* Ghidra draft only; not buildable/verified source. */

void FUN_8c043a40(undefined4 param_1,undefined4 param_2,undefined4 param_3,undefined4 param_4,
                 undefined4 param_5,undefined4 param_6)

{
  undefined4 in_xf4;
  undefined4 in_xf6;
  undefined4 in_xf8;
  char in_FPSCR_SZ;
  undefined1 local_34;
  undefined1 local_33;
  undefined1 local_32;
  undefined1 local_31;
  undefined1 local_30;
  undefined1 local_2f;
  undefined1 local_2e;
  undefined1 local_2d;
  undefined4 local_2c;
  undefined4 uStack_28;
  undefined4 uStack_24;
  undefined4 local_20;
  undefined4 uStack_1c;
  undefined4 uStack_18;
  undefined4 *local_14;
  undefined1 *local_10;
  undefined4 local_c;
  undefined4 local_8;
  
  local_10 = &local_34;
  local_14 = &local_2c;
  local_2c = param_2;
  uStack_28 = in_xf4;
  uStack_24 = param_4;
  local_20 = in_xf6;
  uStack_1c = param_6;
  uStack_18 = in_xf8;
  if (in_FPSCR_SZ == '\0') {
    local_2c = param_1;
    uStack_28 = param_2;
    uStack_24 = param_3;
    local_20 = param_4;
    uStack_1c = param_5;
    uStack_18 = param_6;
  }
  local_31 = 0xff;
  local_32 = 0xff;
  local_33 = 0xff;
  local_34 = 0xff;
  local_2d = 0xff;
  local_2e = 0x88;
  local_2f = 0x88;
  local_30 = 0xbb;
  local_c = 0;
  local_8 = 2;
  (*(code *)PTR_FUN_8c043be4)(&local_14,1,0);
  return;
}

