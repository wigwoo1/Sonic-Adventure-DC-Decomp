/* Ghidra draft only; not buildable/verified source. */

/* WARNING: Removing unreachable block (ram,0x0c90c0b4) */
/* WARNING: Removing unreachable block (ram,0x0c90c0c2) */

void FUN_0c90bfc0(uint param_1,undefined4 *param_2)

{
  short local_64;
  short local_62;
  short local_60;
  short local_5e;
  short local_5c;
  short local_5a;
  short local_58;
  short local_56;
  undefined4 local_54;
  undefined4 local_50;
  undefined4 local_4c;
  undefined4 local_48;
  undefined4 local_44;
  undefined4 local_40;
  undefined4 local_3c;
  undefined4 local_38;
  undefined4 local_34;
  undefined4 local_30;
  undefined4 local_2c;
  undefined4 local_28;
  undefined4 local_24;
  undefined4 local_20;
  undefined4 local_1c;
  undefined4 local_18;
  undefined4 *puStack_14;
  undefined4 *puStack_10;
  short *psStack_c;
  undefined4 uStack_8;
  
  local_40 = *(undefined4 *)(PTR_DAT_0c90c140 + 4);
  puStack_14 = &local_44;
  local_44 = *(undefined4 *)PTR_DAT_0c90c140;
  local_38 = *(undefined4 *)(PTR_DAT_0c90c140 + 0xc);
  local_3c = *(undefined4 *)(PTR_DAT_0c90c140 + 8);
  local_30 = *(undefined4 *)(PTR_DAT_0c90c140 + 0x14);
  local_34 = *(undefined4 *)(PTR_DAT_0c90c140 + 0x10);
  local_28 = *(undefined4 *)(PTR_DAT_0c90c140 + 0x1c);
  local_2c = *(undefined4 *)(PTR_DAT_0c90c140 + 0x18);
  local_20 = *(undefined4 *)(PTR_DAT_0c90c140 + 0x24);
  local_24 = *(undefined4 *)(PTR_DAT_0c90c140 + 0x20);
  local_18 = *(undefined4 *)(PTR_DAT_0c90c140 + 0x2c);
  local_1c = *(undefined4 *)(PTR_DAT_0c90c140 + 0x28);
  puStack_10 = &local_54;
  uStack_8 = 4;
  if (param_2 == (undefined4 *)0x0) {
    local_54 = 0xffffffff;
    local_50 = 0xffffffff;
    local_4c = 0xffffffff;
    local_48 = 0xffffffff;
  }
  else {
    local_54 = *param_2;
    local_50 = *param_2;
    local_4c = *param_2;
    local_48 = *param_2;
  }
  psStack_c = &local_64;
  if (((param_1 & 0xff) < 0x20) || (0xbf < (param_1 & 0xff))) {
    param_1 = 0;
  }
  else {
    param_1 = param_1 + 0xe0;
  }
  local_5e = (short)((int)(param_1 & 0xff) >> 4) * 0x10;
  local_62 = local_5e + 0xf;
  local_64 = ((ushort)param_1 & 0xf) * 0x10;
  local_5c = local_64 + 0xf;
  local_60 = local_64;
  local_5a = local_5e;
  local_58 = local_5c;
  local_56 = local_62;
  (*DAT_0c90c148)(PTR_PTR_0c90c144);
  (*DAT_0c90c14c)(0);
  (*DAT_0c90c154)(&puStack_14,4,DAT_0c90c150);
  return;
}

