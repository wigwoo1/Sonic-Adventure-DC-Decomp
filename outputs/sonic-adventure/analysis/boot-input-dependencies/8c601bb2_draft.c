/* Ghidra draft only; not buildable/verified source. */

void FUN_8c601bb2(undefined4 *param_1)

{
  int *piVar1;
  undefined *puVar2;
  
  piVar1 = DAT_8c601c2c;
  *param_1 = 0;
  param_1[1] = 0;
  if (*piVar1 == 0) {
    param_1[2] = 0;
    param_1[3] = 0xffffffff;
    param_1[4] = 0;
    param_1[5] = 0;
  }
  else {
    param_1[2] = 0xffffffff;
    param_1[3] = 0;
    param_1[4] = 0xffffffff;
    param_1[5] = 0xffffffff;
  }
  *(undefined2 *)(param_1 + 6) = 0;
  *(undefined2 *)((int)param_1 + 0x1a) = 0;
  *(undefined2 *)((int)param_1 + 0x22) = 0;
  *(undefined2 *)(param_1 + 8) = 0;
  puVar2 = PTR_s__no_device__8c601c60;
  *(undefined2 *)((int)param_1 + 0x1e) = 0;
  *(undefined2 *)(param_1 + 7) = 0;
  param_1[10] = 0;
  param_1[9] = puVar2;
  return;
}

