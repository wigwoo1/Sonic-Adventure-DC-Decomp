/* Ghidra draft only; not buildable/verified source. */

undefined4
FUN_8c10d770(undefined4 *param_1,undefined4 *param_2,int param_3,int param_4,int param_5,int param_6
            )

{
  undefined *puVar1;
  undefined *puVar2;
  uint uVar3;
  int unaff_r9;
  int unaff_r10;
  undefined4 *local_30;
  undefined4 *local_2c;
  
  if (param_5 == 0) {
    unaff_r10 = 0;
  }
  else if (param_5 == 1) {
    unaff_r10 = 1;
  }
  else if (param_5 == 2) {
    unaff_r10 = -1;
  }
  if (param_6 == 0) {
    unaff_r9 = 0;
  }
  else if (param_6 == 1) {
    unaff_r9 = 1;
  }
  else if (param_6 == 2) {
    unaff_r9 = -1;
  }
  (*(code *)PTR_FUN_8c10d848)(&DAT_8c88f644);
  puVar1 = PTR_FUN_8c10d84c;
  (*(code *)PTR_FUN_8c10d84c)();
  puVar2 = PTR_FUN_8c10d850;
  local_30 = param_1;
  local_2c = param_2;
  if (param_3 == 1) {
    uVar3 = 0;
    if (0 < param_4) {
      do {
        if ((uVar3 & 7) == 0) {
          (*(code *)puVar1)();
          (*(code *)puVar2)();
        }
        uVar3 = uVar3 + 1;
        *(undefined1 *)local_2c = *(undefined1 *)local_30;
        local_30 = (undefined4 *)((int)local_30 + unaff_r10);
        local_2c = (undefined4 *)((int)local_2c + unaff_r9);
      } while ((int)uVar3 < param_4);
    }
  }
  else if (param_3 == 2) {
    uVar3 = 0;
    if (0 < param_4) {
      do {
        if ((uVar3 & 7) == 0) {
          (*(code *)puVar1)();
          (*(code *)puVar2)();
        }
        uVar3 = uVar3 + 1;
        *(undefined2 *)local_2c = *(undefined2 *)local_30;
        local_30 = (undefined4 *)((int)local_30 + unaff_r10 * 2);
        local_2c = (undefined4 *)((int)local_2c + unaff_r9 * 2);
      } while ((int)uVar3 < param_4);
    }
  }
  else if ((param_3 == 4) && (uVar3 = 0, 0 < param_4)) {
    do {
      if ((uVar3 & 7) == 0) {
        (*(code *)puVar1)();
        (*(code *)puVar2)();
      }
      uVar3 = uVar3 + 1;
      *local_2c = *local_30;
      local_30 = local_30 + unaff_r10;
      local_2c = local_2c + unaff_r9;
    } while ((int)uVar3 < param_4);
  }
  (*(code *)PTR_FUN_8c10d90c)();
  return 0;
}

