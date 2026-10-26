/* Ghidra draft only; not buildable/verified source. */

void FUN_8c654286(uint *param_1)

{
  uint uVar1;
  
  if ((*param_1 & 0xffff) == 3) {
    uVar1 = FUN_8c65441c(param_1);
  }
  else if ((param_1[6] & 8) == 0) {
    if ((param_1[6] & 0x20) == 0) {
      uVar1 = FUN_8c6542e0(param_1);
    }
    else {
      uVar1 = FUN_8c654392(param_1);
    }
  }
  else {
    uVar1 = FUN_8c654334(param_1);
  }
  param_1[5] = uVar1;
  return;
}

