/* Ghidra draft only; not buildable/verified source. */

/* WARNING: Restarted to delay deadcode elimination for space: stack */

undefined4 FUN_8c6574b8(int param_1,int param_2,int param_3,int param_4,int param_5,uint param_6)

{
  int *piVar1;
  int iVar2;
  uint uVar3;
  uint *puVar4;
  int iVar5;
  uint uVar6;
  uint uVar7;
  int local_38;
  int local_30;
  uint *local_2c;
  uint local_24 [2];
  
  *DAT_8c657604 = param_1 << 2;
  piVar1 = DAT_8c657608;
  *DAT_8c657608 = param_2 << 2;
  *DAT_8c65760c = param_3 << 2;
  *DAT_8c657610 = param_4 << 2;
  piVar1[3] = param_5 << 2;
  uVar3 = DAT_8c65761c;
  iVar2 = DAT_8c657618;
  uVar6 = 0;
  local_24[0] = *DAT_8c657614;
  local_38 = DAT_8c657618;
  local_30 = DAT_8c657618;
  local_2c = local_24;
  if (param_6 != 0) {
    do {
      uVar7 = 0;
      if ((local_24[uVar6] & 0x1f) != 0) {
        puVar4 = local_24 + uVar6;
        *puVar4 = *puVar4 + 0x1f & 0xffffffe0;
      }
      puVar4 = (uint *)(uVar6 * 0x14 + iVar2);
      if ((*DAT_8c657620 == 1) && (*DAT_8c657624 == 0x20)) {
        *puVar4 = uVar3;
      }
      else if (param_1 == 0) {
        *puVar4 = 0;
      }
      else {
        *puVar4 = local_24[uVar6];
        uVar7 = param_1 + 0x17U & 0xfffffff8;
      }
      iVar5 = uVar6 * 0x14 + iVar2;
      if ((*DAT_8c657620 == 1) && (*DAT_8c657624 == 0x21)) {
        *(uint *)(iVar5 + 4) = uVar3;
      }
      else if (param_2 == 0) {
        *(undefined4 *)(iVar5 + 4) = 0;
      }
      else {
        *(uint *)(iVar5 + 4) = local_24[uVar6] + uVar7 * 4;
        uVar7 = uVar7 + param_2 + 0x17 & 0xfffffff8;
      }
      iVar5 = uVar6 * 0x14 + iVar2;
      if ((*DAT_8c657620 == 1) && (*DAT_8c657624 == 0x22)) {
        *(uint *)(iVar5 + 8) = uVar3;
      }
      else if (param_3 == 0) {
        *(undefined4 *)(iVar5 + 8) = 0;
      }
      else {
        *(uint *)(iVar5 + 8) = local_24[uVar6] + uVar7 * 4;
        uVar7 = uVar7 + param_3 + 0x17 & 0xfffffff8;
      }
      iVar5 = uVar6 * 0x14 + iVar2;
      if ((*DAT_8c657740 == 1) && (*DAT_8c657744 == 0x23)) {
        *(uint *)(iVar5 + 0xc) = uVar3;
      }
      else if (param_4 == 0) {
        *(undefined4 *)(iVar5 + 0xc) = 0;
      }
      else {
        *(uint *)(iVar5 + 0xc) = local_24[uVar6] + uVar7 * 4;
        uVar7 = uVar7 + param_4 + 0x17 & 0xfffffff8;
      }
      if ((*DAT_8c657740 == 1) && (*DAT_8c657744 == 0x24)) {
        *(uint *)(uVar6 * 0x14 + iVar2 + 0x10) = uVar3;
      }
      else if (param_5 == 0) {
        *(undefined4 *)(local_30 + 0x10) = 0;
      }
      else {
        *(uint *)(local_38 + 0x10) = uVar7 * 4 + *local_2c;
        uVar7 = uVar7 + param_5 + 0x17 & 0xfffffff8;
      }
      uVar6 = uVar6 + 1;
      local_24[1] = uVar7 * 4 + local_24[0];
      local_30 = local_30 + 0x14;
      local_2c = local_2c + 1;
      local_38 = local_38 + 0x14;
    } while (uVar6 < param_6);
  }
  return 0;
}

