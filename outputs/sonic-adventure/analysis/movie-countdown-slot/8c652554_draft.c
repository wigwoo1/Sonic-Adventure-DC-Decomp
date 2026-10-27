/* Ghidra draft only; not buildable/verified source. */

undefined4
FUN_8c652554(undefined8 param_1,undefined8 param_2,undefined8 param_3,undefined8 param_4,
            code *param_5,undefined4 param_6)

{
  undefined1 *puVar1;
  undefined4 *puVar2;
  undefined8 in_dr0;
  undefined8 in_dr2;
  undefined8 in_dr12;
  undefined8 in_xd2;
  undefined8 in_xd4;
  undefined4 in_PR;
  byte in_FPSCR_RM;
  byte in_FPSCR_FLAG;
  byte in_FPSCR_ENABLE;
  byte in_FPSCR_CAUSE;
  byte in_FPSCR_DN;
  byte in_FPSCR_PR;
  byte in_FPSCR_SZ;
  char cVar3;
  byte in_FPSCR_FR;
  undefined1 auStack_84 [40];
  undefined8 uStack_5c;
  undefined8 uStack_4c;
  undefined1 auStack_44 [8];
  undefined8 uStack_3c;
  undefined8 uStack_34;
  undefined8 uStack_2c;
  undefined8 uStack_24;
  undefined8 uStack_1c;
  undefined8 uStack_c;
  uint uStack_4;
  
  uStack_4 = (uint)in_FPSCR_RM | (uint)in_FPSCR_FLAG << 2 | (uint)in_FPSCR_ENABLE << 7 |
             (uint)in_FPSCR_CAUSE << 0xc | (uint)in_FPSCR_DN << 0x12 | (uint)in_FPSCR_PR << 0x13 |
             (uint)in_FPSCR_SZ << 0x14 | (uint)in_FPSCR_FR << 0x15;
  cVar3 = (char)((~(~uStack_4 | 0x100000) & 0x100000) >> 0x14);
  if (cVar3 == '\0') {
    uStack_c = CONCAT44((int)((ulonglong)in_dr0 >> 0x20),(undefined4)uStack_c);
                    /* WARNING: Ignoring partial resolution of indirect */
    uStack_c._0_4_ = (int)in_dr0;
    puVar1 = auStack_44;
    uStack_3c = in_dr12;
    uStack_34 = param_4;
    uStack_2c = param_3;
    uStack_24 = param_2;
    uStack_1c = param_1;
  }
  else {
    puVar1 = auStack_84;
    uStack_c = in_dr0;
    uStack_5c = param_4;
    uStack_4c = param_3;
    uStack_3c = param_2;
    uStack_34 = in_xd4;
    uStack_2c = param_1;
    uStack_24 = in_xd2;
    uStack_1c = in_dr2;
  }
  *(undefined4 *)(puVar1 + -4) = in_PR;
  (*param_5)(param_6);
  if (cVar3 == '\0') {
    puVar2 = (undefined4 *)(puVar1 + 0x40);
  }
  else {
    puVar2 = (undefined4 *)(puVar1 + 0x80);
  }
  return *puVar2;
}

