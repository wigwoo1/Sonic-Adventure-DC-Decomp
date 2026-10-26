/* Ghidra draft only; not buildable/verified source. */

undefined4 FUN_8c603c84(int param_1,undefined1 *param_2,uint param_3)

{
  int iVar1;
  int iVar2;
  
  for (iVar2 = param_3 << 6; 0 < iVar2; iVar2 = iVar2 + -1) {
    if (param_1 < 8) {
      if (param_1 < 1) {
        *param_2 = 0xff;
      }
      else {
        iVar1 = 8;
        do {
          param_3 = param_3 << 1;
          if (param_1 < 1) {
            param_3 = param_3 | 1;
          }
          else {
            param_1 = param_1 + -1;
          }
          iVar1 = iVar1 + -1;
        } while (iVar1 != 0);
        *param_2 = (char)param_3;
      }
    }
    else {
      *param_2 = 0;
      param_1 = param_1 + -8;
    }
    param_2 = param_2 + 1;
  }
  return 0;
}

