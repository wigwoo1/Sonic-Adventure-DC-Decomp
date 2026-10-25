/* Ghidra draft only; not buildable/verified source. */

undefined4 * FUN_8c10d25c(undefined4 *param_1,undefined4 *param_2,uint param_3)

{
  undefined1 uVar1;
  undefined2 uVar2;
  undefined4 *puVar3;
  undefined4 uVar4;
  uint uVar5;
  uint uVar6;
  uint uVar7;
  
  if ((param_1 != param_2) && (param_3 != 0)) {
    uVar6 = (param_3 & 0xffff) >> 1;
    uVar5 = (uint)param_1 | (uint)param_2 | param_3;
    uVar7 = param_3 >> 2;
    if (param_1 < param_2) {
      puVar3 = param_1;
      if ((uVar5 & 1) == 0) {
        if ((uVar5 & 3) == 0) {
          do {
            uVar4 = *param_2;
            param_2 = param_2 + 1;
            uVar7 = uVar7 - 1;
            *puVar3 = uVar4;
            puVar3 = puVar3 + 1;
          } while (uVar7 != 0);
        }
        else {
          do {
            uVar2 = *(undefined2 *)param_2;
            param_2 = (undefined4 *)((int)param_2 + 2);
            uVar6 = uVar6 - 1;
            *(undefined2 *)puVar3 = uVar2;
            puVar3 = (undefined4 *)((int)puVar3 + 2);
          } while (uVar6 != 0);
        }
      }
      else {
        do {
          uVar1 = *(undefined1 *)param_2;
          param_2 = (undefined4 *)((int)param_2 + 1);
          param_3 = param_3 - 1;
          *(undefined1 *)puVar3 = uVar1;
          puVar3 = (undefined4 *)((int)puVar3 + 1);
        } while (param_3 != 0);
      }
    }
    else {
      puVar3 = (undefined4 *)((int)param_1 + param_3);
      param_2 = (undefined4 *)((int)param_2 + param_3);
      if ((uVar5 & 1) == 0) {
        if ((uVar5 & 3) == 0) {
          do {
            param_2 = param_2 + -1;
            uVar7 = uVar7 - 1;
            puVar3 = puVar3 + -1;
            *puVar3 = *param_2;
          } while (uVar7 != 0);
        }
        else {
          do {
            param_2 = (undefined4 *)((int)param_2 + -2);
            uVar6 = uVar6 - 1;
            puVar3 = (undefined4 *)((int)puVar3 + -2);
            *(undefined2 *)puVar3 = *(undefined2 *)param_2;
          } while (uVar6 != 0);
        }
      }
      else {
        do {
          param_2 = (undefined4 *)((int)param_2 + -1);
          param_3 = param_3 - 1;
          puVar3 = (undefined4 *)((int)puVar3 + -1);
          *(undefined1 *)puVar3 = *(undefined1 *)param_2;
        } while (param_3 != 0);
      }
    }
  }
  return param_1;
}

