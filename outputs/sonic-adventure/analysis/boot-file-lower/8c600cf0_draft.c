/* Ghidra draft only; not buildable/verified source. */

int FUN_8c600cf0(int *param_1)

{
  undefined *puVar1;
  undefined *puVar2;
  uint uVar3;
  undefined4 uVar4;
  int iVar5;
  undefined4 uVar6;
  uint uVar7;
  int iVar8;
  code *pcVar9;
  
  puVar1 = PTR_FUN_8c600de4;
  pcVar9 = FUN_8c600d82;
  iVar8 = param_1[6];
  uVar4 = (*(code *)PTR_FUN_8c600de4)(FUN_8c600d82);
  uVar3 = DAT_8c600dec;
  puVar2 = PTR_FUN_8c600de8;
  do {
    (**(code **)(*(int *)(*param_1 + 0x3c) + 0x48))();
    iVar5 = FUN_8c6013a4(param_1);
    if (iVar5 == 0) {
      return -0x20;
    }
    if (iVar5 == 1) {
      return 0;
    }
    if (((iVar5 != 2) && (iVar5 != 3)) && (iVar5 != 4)) {
      if ((iVar5 != 5) && (iVar5 != 0x40)) {
        return -0x19;
      }
      return (int)*(short *)((int)param_1 + 0x52);
    }
    if (iVar8 != param_1[6]) {
      uVar4 = (*(code *)puVar1)(iVar5,pcVar9);
      iVar8 = param_1[6];
    }
    uVar6 = (*(code *)puVar1)();
    uVar7 = (*(code *)puVar2)(uVar4,uVar6);
  } while (uVar7 <= uVar3);
  return -0x15;
}

