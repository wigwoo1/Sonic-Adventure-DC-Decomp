/* Ghidra draft only; not buildable/verified source. */

void FUN_8c652520(undefined8 *param_1,undefined8 *param_2,uint param_3)

{
  undefined4 uVar1;
  undefined8 in_dr0;
  char in_FPSCR_SZ;
  
  if ((((uint)param_1 | (uint)param_2 | param_3) & 7) == 0) {
    param_3 = param_3 >> 3;
    do {
      if (in_FPSCR_SZ == '\0') {
        in_dr0 = CONCAT44(*(undefined4 *)param_2,(int)in_dr0);
        param_2 = (undefined8 *)((int)param_2 + 4);
      }
      else {
        in_dr0 = *param_2;
        param_2 = param_2 + 1;
      }
      param_3 = param_3 - 1;
      if (in_FPSCR_SZ == '\0') {
        *(int *)param_1 = (int)((ulonglong)in_dr0 >> 0x20);
      }
      else {
        *param_1 = in_dr0;
      }
      param_1 = param_1 + 1;
    } while (param_3 != 0);
    return;
  }
  param_3 = param_3 >> 2;
  do {
    uVar1 = *(undefined4 *)param_2;
    param_2 = (undefined8 *)((int)param_2 + 4);
    param_3 = param_3 - 1;
    *(undefined4 *)param_1 = uVar1;
    param_1 = (undefined8 *)((int)param_1 + 4);
  } while (param_3 != 0);
  return;
}

