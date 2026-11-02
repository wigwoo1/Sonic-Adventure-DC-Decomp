/* Ghidra draft only; not buildable/verified source. */

/* WARNING: Removing unreachable block (ram,0x8c011dae) */

undefined4 FUN_8c011d46(uint param_1,uint param_2)

{
  int iVar1;
  undefined *puVar2;
  undefined *puVar3;
  int iVar4;
  uint uVar5;
  uint uVar6;
  undefined *puVar7;
  undefined1 *puVar8;
  undefined1 *puVar9;
  undefined1 local_2c;
  undefined1 local_2b;
  undefined1 local_2a;
  undefined1 local_29;
  undefined1 local_28;
  undefined1 local_27;
  undefined1 local_26;
  undefined1 local_25;
  undefined1 local_24;
  undefined1 local_23;
  char local_22;
  char local_21;
  undefined1 local_20;
  undefined1 local_1f;
  undefined1 local_1e;
  undefined1 local_1d;
  
  puVar2 = PTR_DAT_8c011f40;
  puVar7 = PTR_DAT_8c011de8;
  iVar1 = DAT_8c011de4;
  local_2c = *PTR_s_SONICADV_P00_8c011e18;
  local_2b = PTR_s_SONICADV_P00_8c011e18[1];
  local_2a = PTR_s_SONICADV_P00_8c011e18[2];
  local_29 = PTR_s_SONICADV_P00_8c011e18[3];
  local_28 = PTR_s_SONICADV_P00_8c011e18[4];
  local_27 = PTR_s_SONICADV_P00_8c011e18[5];
  local_26 = PTR_s_SONICADV_P00_8c011e18[6];
  local_25 = PTR_s_SONICADV_P00_8c011e18[7];
  local_24 = PTR_s_SONICADV_P00_8c011e18[8];
  local_23 = PTR_s_SONICADV_P00_8c011e18[9];
  local_20 = PTR_s_SONICADV_P00_8c011e18[0xc];
  local_1f = PTR_s_SONICADV_P00_8c011e18[0xd];
  local_1e = PTR_s_SONICADV_P00_8c011e18[0xe];
  local_1d = PTR_s_SONICADV_P00_8c011e18[0xf];
  uVar6 = (param_1 & 0xffff) << 3 | param_2 & 0xffff;
  uVar5 = (int)uVar6 >> 5 & 0x1f;
  local_22 = (char)uVar5;
  if (uVar5 < 10) {
    local_22 = local_22 + '0';
  }
  else {
    local_22 = local_22 + '7';
  }
  uVar6 = uVar6 & 0x1f;
  local_21 = (char)uVar6;
  if (uVar6 < 10) {
    local_21 = local_21 + '0';
  }
  else {
    local_21 = local_21 + '7';
  }
  iVar4 = FUN_8c010fcc(*(undefined4 *)PTR_DAT_8c011f40,&local_2c);
  if (iVar4 != 0) {
    local_28 = 0x30;
    iVar4 = FUN_8c010fcc(*(undefined4 *)puVar2,&local_2c);
    if (iVar4 != 0) {
      return 0xffffffff;
    }
  }
  (*(code *)PTR_FUN_8c011f44)(*(undefined4 *)puVar2,&local_2c,iVar1,0x12);
  puVar3 = PTR_FUN_8c011f48;
  do {
    iVar4 = (*(code *)puVar3)(*(undefined4 *)puVar2);
  } while (iVar4 == -1);
  iVar4 = 0x3000;
  puVar8 = (undefined1 *)(iVar1 + 0x280);
  do {
    iVar4 = iVar4 + -3;
    *puVar7 = *puVar8;
    puVar9 = puVar8 + 2;
    puVar7[1] = puVar8[1];
    puVar8 = puVar8 + 3;
    puVar7[2] = *puVar9;
    puVar7 = puVar7 + 3;
  } while (iVar4 != 0);
  return 0;
}

