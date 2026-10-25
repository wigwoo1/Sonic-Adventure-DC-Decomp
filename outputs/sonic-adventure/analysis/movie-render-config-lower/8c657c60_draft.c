/* Ghidra draft only; not buildable/verified source. */

/* WARNING: Removing unreachable block (ram,0x8c657dc4) */
/* WARNING: Removing unreachable block (ram,0x8c657dd2) */

void FUN_8c657c60(uint param_1,uint param_2,uint param_3,uint param_4,uint param_5,uint param_6,
                 uint param_7,uint param_8,uint param_9,uint param_10,uint param_11,uint param_12,
                 uint param_13,uint param_14,uint param_15,uint param_16,uint param_17,uint param_18
                 ,uint param_19,uint param_20,uint param_21,uint param_22,uint param_23,
                 uint param_24,uint param_25,uint param_26,uint param_27,byte param_28)

{
  uint *puVar1;
  uint uVar2;
  uint uVar3;
  
  uVar2 = (uint)DAT_8c657d00;
  *DAT_8c657d04 = param_22 & uVar2 | (param_21 & uVar2) << 0x10;
  *DAT_8c657d08 = param_24 & uVar2 | (param_23 & uVar2) << 0x10;
  *DAT_8c657d0c = param_18 & 0xf | (param_19 & 0x1f) << 4 | (param_20 & 0xf) << 9;
  *DAT_8c657d10 = param_1;
  *DAT_8c657d14 = param_2;
  uVar2 = (uint)DAT_8c657d02;
  uVar3 = (param_3 & uVar2) << 0x14;
  if (*DAT_8c657d18 < 1) {
    *DAT_8c657de4 = param_1 & uVar2 | (param_2 & uVar2) << 10 | uVar3;
  }
  else {
    *DAT_8c657d1c = param_1 & uVar2 | (param_2 - *DAT_8c657d18 & uVar2) << 10 | uVar3;
  }
  *DAT_8c657de8 = param_6 & 0xff | (param_7 & 0xff) << 8;
  puVar1 = DAT_8c657dec;
  *DAT_8c657dec = param_5 & 0xffff | (param_4 & 1) << 0x10 | (param_8 & 1) << 0x12;
  puVar1[-0xf] = param_17 & (int)DAT_8c657de0;
  *DAT_8c657df0 =
       param_12 & 7 | (param_11 & 1) << 3 | (param_13 & 0xff) << 8 | (param_14 & 0xff) << 0x10;
  *DAT_8c657df4 =
       param_9 & 1 | (param_10 & 1) << 1 | (param_15 & 3) << 2 | (param_16 & 7) << 4 |
       (uint)param_28 << 8 | (param_26 & 0x1f) << 0x10 | (param_25 & 1) << 0x16 |
       (param_27 & 1) << 0x17;
  return;
}

