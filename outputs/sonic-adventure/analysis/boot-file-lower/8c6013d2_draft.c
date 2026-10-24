/* Ghidra draft only; not buildable/verified source. */

undefined4 FUN_8c6013d2(int *param_1)

{
  undefined *puVar1;
  undefined *puVar2;
  undefined4 uVar3;
  undefined4 uVar4;
  uint uVar5;
  uint uVar6;
  undefined1 *puVar7;
  
  puVar1 = PTR_FUN_8c60151c;
  puVar7 = &DAT_8c7a0a48;
  if ((param_1 == (int *)0x0) || ((short)param_1[0x13] == 0)) {
    uVar3 = 0xfffffff6;
  }
  else if (((short)param_1[0x14] == 2) || ((short)param_1[0x14] == 4)) {
    if (*(int **)(*param_1 + 0x1c) == param_1) {
      uVar3 = (*(code *)PTR_FUN_8c60151c)(&DAT_8c7a0a48);
      while (puVar2 = PTR_FUN_8c601524, param_1[0x10] != 1) {
        if (((short)param_1[0x14] != 2) && ((short)param_1[0x14] != 4)) goto LAB_8c60142c;
        uVar4 = (*(code *)puVar1)(puVar7);
        uVar6 = (*(code *)puVar2)(uVar3,uVar4);
        if (DAT_8c601520 < uVar6) {
          return 0xffffffeb;
        }
        (**(code **)(*(int *)(*param_1 + 0x3c) + 0x48))();
      }
      uVar3 = (*(code *)puVar1)(puVar7);
      uVar6 = (uint)DAT_8c601518;
      do {
        uVar4 = (*(code *)puVar1)(puVar7);
        uVar5 = (*(code *)puVar2)(uVar3,uVar4);
      } while (uVar5 < uVar6);
      if (((short)param_1[0x14] == 2) || ((short)param_1[0x14] == 4)) {
        if (*(int **)(*param_1 + 0x1c) == param_1) {
          (**(code **)(*(int *)(*param_1 + 0xc) + 0x14))(param_1[0xf]);
          *(undefined2 *)(param_1 + 0x12) = 3;
          uVar3 = FUN_8c600cf0(param_1);
        }
        else {
          uVar3 = 0xfffffff3;
        }
      }
      else {
        *(undefined2 *)(param_1 + 0x14) = 0;
        uVar3 = 0;
      }
    }
    else {
      uVar3 = 0xfffffff3;
    }
  }
  else {
LAB_8c60142c:
    *(undefined2 *)(param_1 + 0x14) = 0;
    uVar3 = 0;
  }
  return uVar3;
}

