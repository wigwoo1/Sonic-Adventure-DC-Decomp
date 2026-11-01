/* Ghidra draft only; not buildable/verified source. */

/* WARNING: Removing unreachable block (ram,0x8c099828) */

undefined4 FUN_8c0997bc(undefined4 param_1,undefined4 param_2)

{
  undefined *puVar1;
  int iVar2;
  int iVar3;
  undefined4 uVar4;
  int iVar5;
  int iVar6;
  int iVar7;
  int local_120;
  undefined4 local_11c;
  undefined1 auStack_118 [260];
  
  local_11c = param_2;
  (*(code *)PTR_FUN_8c0998d4)(auStack_118,PTR_s__s_PRS_8c0998d0,param_1);
  puVar1 = PTR_FUN_8c0998d8;
  iVar6 = 3;
  while (iVar2 = (*(code *)puVar1)(auStack_118,0), iVar5 = 1000, iVar2 == 0) {
    do {
      iVar5 = iVar5 + -2;
    } while (iVar5 != 0);
    iVar6 = iVar6 + -1;
    if (iVar6 < 1) {
      return 0xffffffff;
    }
  }
  (*(code *)PTR_FUN_8c0998dc)(iVar2,&local_120);
  puVar1 = PTR_FUN_8c0998e4;
  iVar6 = local_120 + 0x7ff >> 0xb;
  iVar7 = 3;
  iVar5 = DAT_8c0998e0 + iVar6 * -0x800;
  do {
    iVar3 = (*(code *)puVar1)(iVar2,iVar6,iVar5);
    if (iVar3 == 0) break;
    iVar7 = iVar7 + -1;
  } while (0 < iVar7);
  (*(code *)PTR_FUN_8c0998e8)(iVar2);
  (*(code *)PTR_FUN_8c0998f0)(iVar5,DAT_8c0998ec);
  uVar4 = (*(code *)PTR_FUN_8c0998f4)(DAT_8c0998ec,local_11c);
  return uVar4;
}

