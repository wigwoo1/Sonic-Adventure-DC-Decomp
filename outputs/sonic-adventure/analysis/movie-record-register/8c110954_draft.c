/* Ghidra draft only; not buildable/verified source. */

undefined4
FUN_8c110954(undefined4 param_1,undefined4 param_2,undefined4 param_3,undefined4 param_4,
            undefined4 param_5,undefined4 param_6,undefined4 param_7,undefined4 param_8,int param_9,
            undefined4 param_10,undefined4 param_11,undefined4 param_12)

{
  undefined4 in_r0;
  undefined4 in_r1;
  undefined4 in_r2;
  undefined4 in_r3;
  undefined4 unaff_r8;
  undefined4 unaff_r9;
  undefined4 unaff_r10;
  undefined4 unaff_r11;
  undefined4 unaff_r13;
  undefined4 unaff_r14;
  undefined4 in_fr1;
  undefined4 in_fr0;
  undefined4 in_fr3;
  undefined4 in_fr2;
  undefined4 in_fr13;
  undefined4 in_fr12;
  undefined4 in_fr15;
  undefined4 in_fr14;
  undefined8 in_xd0;
  undefined8 in_xd2;
  undefined8 in_xd4;
  undefined8 in_xd6;
  undefined8 in_xd8;
  undefined8 in_xd10;
  undefined8 in_xd12;
  undefined8 in_xd14;
  undefined4 in_GBR;
  undefined4 in_MACH;
  undefined4 in_MACL;
  undefined4 in_PR;
  undefined4 in_FPUL;
  byte in_FPSCR_RM;
  byte in_FPSCR_FLAG;
  byte in_FPSCR_ENABLE;
  byte in_FPSCR_CAUSE;
  byte in_FPSCR_DN;
  byte in_FPSCR_PR;
  byte in_FPSCR_SZ;
  byte in_FPSCR_FR;
  
  *(BADSPACEBASE **)param_9 = register0x0000003c;
  *(undefined4 *)(param_9 + 4) = unaff_r14;
  *(undefined4 *)(param_9 + 8) = in_PR;
  *(undefined4 *)(param_9 + 0xc) = unaff_r13;
  *(undefined4 *)(param_9 + 0x10) = 0;
  *(undefined4 *)(param_9 + 0x14) = unaff_r11;
  *(undefined4 *)(param_9 + 0x18) = unaff_r10;
  *(undefined4 *)(param_9 + 0x1c) = unaff_r9;
  *(undefined4 *)(param_9 + 0x20) = unaff_r8;
  *(undefined4 *)(param_9 + 0x24) = param_12;
  *(undefined4 *)(param_9 + 0x28) = param_11;
  *(undefined4 *)(param_9 + 0x2c) = param_10;
  *(int *)(param_9 + 0x30) = param_9;
  *(undefined4 *)(param_9 + 0x34) = in_r3;
  *(undefined4 *)(param_9 + 0x38) = in_r2;
  *(undefined4 *)(param_9 + 0x3c) = in_r1;
  *(undefined4 *)(param_9 + 0x40) = in_r0;
  *(undefined4 *)(param_9 + 0x44) = in_MACH;
  *(undefined4 *)(param_9 + 0x48) = in_MACL;
  *(undefined4 *)(param_9 + 0x4c) = in_GBR;
  *(uint *)(param_9 + 0x94) =
       (uint)in_FPSCR_RM | (uint)in_FPSCR_FLAG << 2 | (uint)in_FPSCR_ENABLE << 7 |
       (uint)in_FPSCR_CAUSE << 0xc | (uint)in_FPSCR_DN << 0x12 | (uint)in_FPSCR_PR << 0x13 |
       (uint)in_FPSCR_SZ << 0x14 | (uint)in_FPSCR_FR << 0x15;
  *(undefined4 *)(param_9 + 0x90) = in_FPUL;
  if (in_FPSCR_SZ == 0) {
    *(undefined4 *)(param_9 + 0x8c) = in_fr15;
    *(undefined4 *)(param_9 + 0x88) = in_fr14;
    *(undefined4 *)(param_9 + 0x84) = in_fr13;
    *(undefined4 *)(param_9 + 0x80) = in_fr12;
    *(undefined4 *)(param_9 + 0x7c) = param_8;
    *(undefined4 *)(param_9 + 0x78) = param_7;
    *(undefined4 *)(param_9 + 0x74) = param_6;
    *(undefined4 *)(param_9 + 0x70) = param_5;
    *(undefined4 *)(param_9 + 0x6c) = param_4;
    *(undefined4 *)(param_9 + 0x68) = param_3;
    *(undefined4 *)(param_9 + 100) = param_2;
    *(undefined4 *)(param_9 + 0x60) = param_1;
    *(undefined4 *)(param_9 + 0x5c) = in_fr3;
    *(undefined4 *)(param_9 + 0x58) = in_fr2;
    *(undefined4 *)(param_9 + 0x54) = in_fr1;
    *(undefined4 *)(param_9 + 0x50) = in_fr0;
  }
  else {
    *(undefined8 *)(param_9 + 0x8c) = in_xd14;
    *(ulonglong *)(param_9 + 0x84) = CONCAT44(in_fr14,in_fr15);
    *(undefined8 *)(param_9 + 0x7c) = in_xd12;
    *(ulonglong *)(param_9 + 0x74) = CONCAT44(in_fr12,in_fr13);
    *(undefined8 *)(param_9 + 0x6c) = in_xd10;
    *(ulonglong *)(param_9 + 100) = CONCAT44(param_7,param_8);
    *(undefined8 *)(param_9 + 0x5c) = in_xd8;
    *(ulonglong *)(param_9 + 0x54) = CONCAT44(param_5,param_6);
    *(undefined8 *)(param_9 + 0x4c) = in_xd6;
    *(ulonglong *)(param_9 + 0x44) = CONCAT44(param_3,param_4);
    *(undefined8 *)(param_9 + 0x3c) = in_xd4;
    *(ulonglong *)(param_9 + 0x34) = CONCAT44(param_1,param_2);
    *(undefined8 *)(param_9 + 0x2c) = in_xd2;
    *(ulonglong *)(param_9 + 0x24) = CONCAT44(in_fr2,in_fr3);
    *(undefined8 *)(param_9 + 0x1c) = in_xd0;
    *(ulonglong *)(param_9 + 0x14) = CONCAT44(in_fr0,in_fr1);
  }
  return 0;
}

