/* Ghidra draft only; not buildable/verified source. */

undefined4
FUN_8c10fedc(undefined8 param_1,int param_2,undefined1 *param_3,int *param_4,char *param_5)

{
  undefined *puVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  undefined1 *puVar5;
  undefined8 uVar6;
  undefined4 in_xf4;
  undefined4 in_PR;
  byte in_FPSCR_DN;
  byte bVar7;
  byte in_FPSCR_PR;
  byte bVar8;
  byte in_FPSCR_SZ;
  byte bVar9;
  short local_5c [2];
  int local_58;
  undefined4 local_54;
  int local_50;
  short local_4c;
  undefined1 auStack_48 [8];
  undefined1 local_40 [8];
  undefined1 local_38 [8];
  undefined1 auStack_30 [7];
  undefined1 local_29;
  undefined4 local_28;
  undefined2 uStack_24;
  byte bStack_22;
  byte bStack_21;
  undefined4 local_20;
  
  local_20 = (undefined4)((ulonglong)param_1 >> 0x20);
  if (in_FPSCR_SZ == 0) {
    local_28 = (undefined4)param_1;
    uStack_24 = (undefined2)((ulonglong)param_1 >> 0x20);
    bStack_22 = (byte)((ulonglong)param_1 >> 0x30);
    bStack_21 = (byte)((ulonglong)param_1 >> 0x38);
    local_20 = in_PR;
  }
  else {
    uStack_24 = (undefined2)param_1;
    bStack_22 = (byte)((ulonglong)param_1 >> 0x10);
    bStack_21 = (byte)((ulonglong)param_1 >> 0x18);
    local_28 = in_xf4;
  }
  local_54 = 8;
  local_4c = 0;
  if (((bStack_21 & 0x7f) == 0) && (iVar2 = (*(code *)PTR_FUN_8c10ffc0)(&local_28,7), iVar2 != 0)) {
    puVar5 = param_3 + 8;
    for (; param_3 < puVar5; param_3 = param_3 + 1) {
      *param_3 = 0;
    }
    *param_4 = 0;
    if ((bStack_21 & 0x80) == 0) {
      *param_5 = '\x01';
    }
    else {
      *param_5 = -1;
    }
  }
  else {
    if (in_FPSCR_SZ == 0) {
      uVar6 = CONCAT44(CONCAT13(bStack_21,CONCAT12(bStack_22,uStack_24)),local_28);
    }
    else {
      uVar6 = CONCAT44(local_20,CONCAT13(bStack_21,CONCAT12(bStack_22,uStack_24)));
    }
    bVar7 = 0;
    bVar8 = 0;
    bVar9 = (byte)((((uint)in_FPSCR_DN << 0x12 | (uint)in_FPSCR_PR << 0x13 |
                    (uint)in_FPSCR_SZ << 0x14) & DAT_8c10ffc4 & 0x100000) >> 0x14);
    (*(code *)PTR_FUN_8c10ffc8)(uVar6,param_5,local_5c,auStack_30);
    if (*param_5 == -1) {
      bStack_21 = bStack_21 & 0x7f;
    }
    if (local_5c[0] == DAT_8c10ffba) {
      if (bStack_22 != DAT_8c10ffbc) {
        return 2;
      }
      bVar9 = (byte)((((uint)bVar7 << 0x12 | (uint)bVar8 << 0x13 | (uint)bVar9 << 0x14) &
                      DAT_8c10ffc4 & 0x100000) >> 0x14);
      iVar2 = (*(code *)PTR_FUN_8c10ffc0)(&local_28,6);
      if (iVar2 == 0) {
        return 2;
      }
      if (*param_5 == '\x01') {
        return 1;
      }
      if (*param_5 == -1) {
        return 0xffffffff;
      }
    }
    local_5c[0] = local_5c[0] - DAT_8c10fff4;
    if (local_5c[0] < 0) {
      if (bVar9 == 0) {
        uVar6 = CONCAT44(CONCAT13(bStack_21,CONCAT12(bStack_22,uStack_24)),local_28);
      }
      else {
        uVar6 = CONCAT44(local_20,CONCAT13(bStack_21,CONCAT12(bStack_22,uStack_24)));
      }
      iVar2 = (*(code *)PTR_FUN_8c1101d4)(uVar6);
      iVar2 = iVar2 + -1;
    }
    else {
      if (bVar9 == 0) {
        uVar6 = CONCAT44(CONCAT13(bStack_21,CONCAT12(bStack_22,uStack_24)),local_28);
      }
      else {
        uVar6 = CONCAT44(local_20,CONCAT13(bStack_21,CONCAT12(bStack_22,uStack_24)));
      }
      iVar2 = (*(code *)PTR_FUN_8c10fffc)(uVar6);
    }
    puVar1 = PTR_FUN_8c1101d8;
    local_29 = 0;
    *param_4 = (iVar2 - param_2) + 1;
    (*(code *)puVar1)(param_2,local_38);
    iVar3 = (*(code *)PTR_FUN_8c1101dc)();
    iVar4 = 0;
    iVar2 = iVar4;
    if (0 < 8 - iVar3) {
      do {
        iVar4 = iVar2 + 1;
        local_38[iVar2] = local_38[iVar2 + iVar3];
        iVar2 = iVar4;
      } while (iVar4 < 8 - iVar3);
    }
    for (puVar5 = local_38 + iVar4; puVar5 < auStack_30; puVar5 = puVar5 + 1) {
      *puVar5 = 0;
    }
    (*(code *)PTR_FUN_8c1101e0)(local_38,8,param_2 + iVar3 * -8);
    local_50 = param_2 + -1;
    (*(code *)PTR_FUN_8c1101d8)(local_40);
    local_58 = (*(code *)PTR_FUN_8c1101dc)();
    iVar3 = 0;
    iVar2 = iVar3;
    if (0 < 8 - local_58) {
      do {
        iVar3 = iVar2 + 1;
        local_40[iVar2] = local_40[local_58 + iVar2];
        iVar2 = iVar3;
      } while (iVar3 < 8 - local_58);
    }
    for (puVar5 = local_40 + iVar3; puVar5 < local_38; puVar5 = puVar5 + 1) {
      *puVar5 = 0;
    }
    (*(code *)PTR_FUN_8c1101e0)(local_40,8,param_2 + local_58 * -8 + -1);
    while( true ) {
      while( true ) {
        (*(code *)PTR_FUN_8c1101e4)((int)local_5c[0],auStack_30,*param_4,param_3);
        (*(code *)PTR_FUN_8c1101e8)(auStack_48,param_3,8);
        puVar1 = PTR_FUN_8c1101ec;
        iVar2 = (*(code *)PTR_FUN_8c1101ec)(auStack_48,local_38,8);
        if (iVar2 < 1) break;
        local_4c = 1;
        *param_4 = *param_4 + 1;
      }
      (*(code *)PTR_FUN_8c1101e8)(auStack_48,param_3,8);
      iVar2 = (*(code *)puVar1)(auStack_48,local_38,8);
      if (iVar2 == 0) break;
      (*(code *)PTR_FUN_8c1101e8)(auStack_48,param_3,8);
      iVar2 = (*(code *)puVar1)(auStack_48,local_40,8);
      if (-1 < iVar2) {
        return 0;
      }
      if (local_4c != 0) {
        return 0;
      }
      *param_4 = *param_4 + -1;
    }
    iVar2 = *param_4;
    *param_4 = iVar2 + 1;
    (*(code *)PTR_FUN_8c1101e4)((int)local_5c[0],auStack_30,iVar2 + 1,param_3);
  }
  return 0;
}

