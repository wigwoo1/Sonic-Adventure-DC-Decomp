/* Ghidra draft only; not buildable/verified source. */

undefined4 FUN_8c665a80(undefined4 param_1,undefined4 param_2)

{
  int iVar1;
  
  iVar1 = (*(code *)PTR_FUN_8c665ac4)(PTR_s_HARDWARE_8c665ac0);
  if ((iVar1 == 0) &&
     (iVar1 = (*(code *)PTR_FUN_8c665ac4)(param_1,PTR_s_EnableTexture16MB_8c665ac8),
     param_2 = param_1, iVar1 == 0)) {
    param_2 = 0;
  }
  return param_2;
}

