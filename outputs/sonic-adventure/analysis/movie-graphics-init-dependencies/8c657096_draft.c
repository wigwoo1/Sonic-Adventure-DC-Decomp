/* Ghidra draft only; not buildable/verified source. */

/* WARNING: Removing unreachable block (ram,0x8c657198) */
/* WARNING: Removing unreachable block (ram,0x8c65719c) */

uint FUN_8c657096(uint param_1,int param_2,uint param_3,int param_4,int param_5,int param_6)

{
  undefined4 uVar1;
  uint uVar2;
  uint uVar3;
  uint uVar4;
  int iVar5;
  int iVar6;
  int iVar7;
  
  uVar2 = *DAT_8c657130;
  iVar5 = 0;
  iVar7 = 0;
  iVar6 = 0;
  uVar4 = (uint)(param_2 != 0);
  uVar3 = (uint)(param_4 != 0);
  if (param_5 != 0) {
    iVar5 = 2;
  }
  if (param_6 == 1) {
    if (param_1 != 0) {
      iVar6 = 2;
    }
    if (param_3 != 0) {
      iVar7 = 2;
    }
  }
  else {
    if (param_1 != 0) {
      iVar6 = 3;
    }
    if (param_3 != 0) {
      iVar7 = 3;
    }
    if ((param_1 != 0) && (param_3 != 0)) {
      if (param_1 < param_3) {
        iVar6 = 2;
      }
      else {
        iVar7 = 2;
      }
    }
  }
  if (*DAT_8c657200 == 1) {
    if (*DAT_8c657204 == 0x20) {
      iVar6 = 3;
    }
    if (*DAT_8c657204 == 0x21) {
      uVar4 = 2;
    }
    if (*DAT_8c657204 == 0x22) {
      iVar7 = 3;
    }
    if (*DAT_8c657204 == 0x23) {
      uVar3 = 2;
    }
    if (*DAT_8c657204 == 0x24) {
      iVar5 = 3;
    }
  }
  uVar3 = iVar6 << 0x11 & DAT_8c657208 | uVar4 << 0x13 & DAT_8c65720c | iVar7 << 0x15 & DAT_8c657210
          | uVar3 << 0x17 & DAT_8c657210 << 2 | iVar5 << 0x19 & DAT_8c657214 | DAT_8c657218;
  if ((*DAT_8c65721c & 0x20) != 0) {
    if (((uVar4 == 0) || (iVar7 == 0)) || (iVar5 == 0)) {
      uVar1 = 0;
    }
    else {
      uVar1 = 1;
    }
    *DAT_8c657220 = uVar1;
  }
  return uVar2 & 0xff | uVar3;
}

