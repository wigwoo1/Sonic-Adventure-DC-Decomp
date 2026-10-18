/* Ghidra draft only; not buildable/verified source. */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_8c60526a(undefined4 param_1,undefined4 param_2,undefined4 param_3,undefined4 param_4)

{
  undefined4 uVar1;
  undefined4 uVar2;
  undefined4 in_fr3;
  undefined4 in_xf2;
  undefined8 in_xd4;
  char in_FPSCR_SZ;
  undefined4 *local_74;
  undefined4 *local_70;
  undefined4 *local_6c;
  undefined4 local_68;
  undefined4 uStack_64;
  undefined4 uStack_60;
  undefined4 uStack_5c;
  undefined8 *puStack_58;
  undefined4 local_50;
  undefined4 local_48;
  undefined4 uStack_44;
  undefined4 uStack_40;
  undefined4 uStack_3c;
  undefined4 uStack_38;
  undefined4 local_30;
  undefined4 local_28;
  undefined4 uStack_24;
  undefined4 uStack_20;
  undefined4 uStack_1c;
  undefined4 uStack_18;
  undefined4 local_10;
  
  uVar1 = uStack_38;
  local_74 = &local_68;
  local_70 = &local_48;
  local_68 = DAT_8c605380;
  if (in_FPSCR_SZ == '\0') {
    uStack_60 = 0;
    param_1 = DAT_8c605384;
  }
  else {
    in_xd4 = CONCAT44(_DAT_8c605388,DAT_8c605384);
    puStack_58 = _DAT_8c605388;
    uStack_60 = param_1;
  }
  uStack_5c = DAT_8c605384;
  local_48 = DAT_8c605380;
  local_6c = &local_28;
  if (in_FPSCR_SZ == '\0') {
    in_fr3 = *(undefined4 *)_DAT_8c605388;
    uStack_44 = in_xf2;
  }
  else {
    uStack_44 = (undefined4)*_DAT_8c605388;
  }
  uStack_3c = (undefined4)in_xd4;
  uStack_1c = uStack_3c;
  uStack_38 = (undefined4)((ulonglong)in_xd4 >> 0x20);
  uVar2 = uStack_38;
  uStack_40 = param_1;
  if (in_FPSCR_SZ == '\0') {
    uStack_40 = 0;
    uStack_44 = in_fr3;
    uStack_3c = param_1;
    uStack_38 = uVar1;
  }
  local_28 = DAT_8c60538c;
  if (in_FPSCR_SZ == '\0') {
    uStack_24 = 0;
    uStack_20 = *(undefined4 *)DAT_8c605390;
    uStack_1c = param_1;
  }
  else {
    uStack_20 = (undefined4)*DAT_8c605390;
    uStack_24 = param_1;
    uStack_18 = uVar2;
  }
  uStack_64 = uStack_60;
  local_50 = param_2;
  local_30 = param_3;
  local_10 = param_4;
  (*(code *)PTR_FUN_8c605394)(&local_74,0,0x20);
  return;
}

