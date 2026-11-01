/* Ghidra draft only; not buildable/verified source. */

undefined4 FUN_8c64efcc(undefined4 param_1,undefined4 param_2)

{
  undefined *puVar1;
  undefined *puVar2;
  undefined *puVar3;
  undefined4 uVar4;
  int iVar5;
  int iVar6;
  char local_28 [12];
  
  uVar4 = (*(code *)PTR_FUN_8c64f12c)(param_2);
  (*(code *)PTR_FUN_8c64f130)(uVar4);
  (*(code *)PTR_FUN_8c64f134)(local_28);
  puVar3 = PTR_DAT_8c64f13c;
  puVar2 = PTR_FUN_8c64f138;
  if (local_28[0] == '.') {
    iVar6 = 0;
    do {
      iVar5 = (*(code *)PTR_FUN_8c64f140)(local_28,puVar3 + iVar6 * 5);
      puVar1 = PTR_FUN_8c64f130;
      if (iVar5 == 0) break;
      iVar6 = iVar6 + 1;
    } while (iVar6 < 3);
    if (iVar6 != 3) {
      *DAT_8c64f144 = iVar6;
      (*(code *)puVar1)();
      (*(code *)puVar2)(param_1,param_2);
      return 1;
    }
  }
  (*(code *)PTR_FUN_8c64f130)();
  (*(code *)puVar2)(param_1,param_2);
  (*(code *)puVar2)(param_1,PTR_DAT_8c64f14c);
  *DAT_8c64f144 = 0;
  return 1;
}

