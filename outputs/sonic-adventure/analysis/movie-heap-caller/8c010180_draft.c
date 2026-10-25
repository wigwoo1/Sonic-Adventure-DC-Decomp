/* Ghidra draft only; not buildable/verified source. */

void FUN_8c010180(undefined4 param_1,undefined4 param_2,undefined4 param_3)

{
  undefined *puVar1;
  undefined *puVar2;
  int iVar3;
  uint uVar4;
  uint uVar5;
  int iVar6;
  
  (*(code *)PTR_FUN_8c0102a0)(FUN_8c010b58);
  (*(code *)PTR_FUN_8c0102a4)();
  (*(code *)PTR_FUN_8c0102bc)
            ((*(uint *)PTR_DAT_8c0102b0 & DAT_8c0102ac | DAT_8c0102b4) + DAT_8c0102b8,DAT_8c0102a8);
  (*(code *)PTR_FUN_8c0102c0)(param_1,param_2,param_3);
  (*(code *)PTR_FUN_8c0102c4)();
  (*(code *)PTR_FUN_8c0102d0)(0,DAT_8c0102cc,DAT_8c0102c8);
  (*(code *)PTR_FUN_8c0102d4)();
  puVar2 = PTR_FUN_8c0102e4;
  puVar1 = PTR_thunk_FUN_8c601618_8c0102e0;
  uVar4 = DAT_8c0102d8 & 0xffffffe0;
  uVar5 = DAT_8c0102dc & 0xffffffe0;
  iVar6 = 8;
  do {
    iVar3 = (*(code *)puVar1)(8,uVar4 + 0x20,0x834,uVar5 + 0x20);
    if ((iVar3 == -0x17) || (iVar3 == -0x21)) {
      FUN_8c01025a();
      (*(code *)puVar2)();
    }
    else if (iVar3 == 0) break;
    iVar6 = iVar6 + -1;
  } while (0 < iVar6);
  if (iVar6 == 0) {
    FUN_8c01025a();
    (*(code *)puVar2)();
  }
  (*(code *)PTR_FUN_8c0102ec)(PTR_s_SONICADV_8c0102e8);
  return;
}

