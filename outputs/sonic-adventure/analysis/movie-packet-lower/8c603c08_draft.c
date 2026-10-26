/* Ghidra draft only; not buildable/verified source. */

void FUN_8c603c08(undefined4 *param_1,undefined4 *param_2,int param_3)

{
  undefined4 uVar1;
  bool bVar2;
  
  if (param_3 < 0) {
    param_3 = param_3 + 3;
  }
  param_3 = param_3 >> 2;
  while (bVar2 = 0 < param_3, param_3 = param_3 + -1, bVar2) {
    uVar1 = *param_1;
    param_1 = param_1 + 1;
    *param_2 = uVar1;
    param_2 = param_2 + 1;
  }
  return;
}

