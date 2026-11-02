/* Ghidra draft only; not buildable/verified source. */

undefined4 FUN_8c014242(ushort param_1,char param_2)

{
  undefined4 uVar1;
  int iVar2;
  undefined4 uVar3;
  undefined1 local_1c;
  undefined1 local_1b;
  undefined1 local_1a;
  char local_19;
  char local_18;
  undefined1 local_17;
  undefined1 local_16;
  undefined1 local_15;
  undefined1 local_14;
  undefined1 local_13;
  undefined1 local_12;
  undefined1 local_11;
  undefined1 local_10;
  undefined1 local_f;
  undefined1 local_e;
  undefined1 local_d;
  
  uVar1 = DAT_8c01439c;
  local_1c = *PTR_DAT_8c014398;
  local_1b = PTR_DAT_8c014398[1];
  local_1a = PTR_DAT_8c014398[2];
  local_11 = PTR_DAT_8c014398[0xb];
  local_10 = PTR_DAT_8c014398[0xc];
  local_f = PTR_DAT_8c014398[0xd];
  local_e = PTR_DAT_8c014398[0xe];
  local_d = PTR_DAT_8c014398[0xf];
  if (param_1 < 10) {
    local_19 = (char)param_1 + '0';
  }
  else {
    local_19 = (char)param_1 + '7';
  }
  if ('Z' < local_19) {
    local_19 = local_19 + -0x1a;
    local_1a = 0x31;
  }
  local_18 = param_2 + '0';
  local_17 = 0x42;
  local_16 = 0x2e;
  local_15 = 0x62;
  local_14 = 0x69;
  local_13 = 0x6e;
  local_12 = 0;
  iVar2 = FUN_8c0141e0(&local_1c,DAT_8c01439c);
  uVar3 = 1;
  if (iVar2 != 1) {
    local_18 = 0x30;
    uVar3 = FUN_8c0141e0(&local_1c,uVar1);
  }
  return uVar3;
}

