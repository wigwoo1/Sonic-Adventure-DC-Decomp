/* Ghidra draft only; not buildable/verified source. */

undefined4 FUN_8c65393a(uint *param_1,int param_2,int param_3)

{
  uint uVar1;
  
  if ((param_2 != 1) && (param_3 != 1)) {
    if ((*param_1 & 0xffff) == 3) {
      FUN_8c654520(param_1[7],param_1[3],param_1[6]);
    }
    else {
      uVar1 = param_1[6];
      if (((((uVar1 & 4) == 0) || ((uVar1 & 1) == 0)) || ((uVar1 & 8) != 0)) ||
         ((uVar1 & 0x20) != 0)) {
        (*(code *)PTR_FUN_8c6539cc)(param_1[7],param_1[5]);
      }
      else {
        FUN_8c6544a0(param_1[7],param_1[3]);
      }
    }
    return 0;
  }
  return 4;
}

