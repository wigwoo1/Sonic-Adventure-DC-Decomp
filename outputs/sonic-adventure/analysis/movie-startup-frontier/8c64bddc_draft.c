/* Ghidra draft only; not buildable/verified source. */

undefined4 FUN_8c64bddc(uint param_1)

{
  undefined1 uVar1;
  undefined *puVar2;
  int iVar3;
  undefined1 *puVar4;
  undefined1 *puVar5;
  undefined1 *puVar6;
  undefined1 local_50 [7];
  undefined1 local_49 [57];
  
  puVar6 = local_50;
  iVar3 = (*(code *)PTR_FUN_8c64bec0)(2);
  if (iVar3 == -8) {
    iVar3 = (*(code *)PTR_FUN_8c64bec4)(0);
  }
  if ((iVar3 == 0) &&
     (iVar3 = (*(code *)PTR_FUN_8c64bec8)(5,local_50), puVar2 = PTR_FUN_8c64becc, iVar3 == 0)) {
    do {
      iVar3 = (*(code *)puVar2)();
    } while (iVar3 == 2);
    if (iVar3 == 1) {
      iVar3 = (*(code *)PTR_FUN_8c64bed0)();
      puVar4 = (undefined1 *)(iVar3 + 0x10);
      if (param_1 == 0) {
        for (iVar3 = 8; iVar3 != 0; iVar3 = iVar3 + -1) {
          uVar1 = *puVar6;
          puVar6 = puVar6 + 1;
          *puVar4 = uVar1;
          puVar4 = puVar4 + 1;
        }
        return 0;
      }
      puVar4 = local_49;
      puVar6 = (undefined1 *)(iVar3 + 0x17);
      for (iVar3 = 0; iVar3 < 5; iVar3 = iVar3 + 1) {
        if (((param_1 & 1) != 0) && (*puVar4 = *puVar6, iVar3 == 4)) {
          puVar5 = puVar6 + -2;
          puVar4[-1] = puVar6[-1];
          puVar6 = puVar6 + -3;
          puVar4[-2] = *puVar5;
          puVar4 = puVar4 + -3;
          *puVar4 = *puVar6;
        }
        puVar6 = puVar6 + -1;
        param_1 = (int)param_1 >> 1;
        puVar4 = puVar4 + -1;
      }
      iVar3 = (*(code *)PTR_FUN_8c64bed4)(5,local_50);
      if (iVar3 == 0) {
        do {
          iVar3 = (*(code *)puVar2)();
        } while (iVar3 == 2);
        if (iVar3 == 1) {
          return 0;
        }
      }
    }
  }
  return 0xffffffff;
}

