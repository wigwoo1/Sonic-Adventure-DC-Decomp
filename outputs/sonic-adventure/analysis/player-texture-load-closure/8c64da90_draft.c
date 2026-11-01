/* Ghidra draft only; not buildable/verified source. */

void FUN_8c64da90(undefined4 param_1)

{
  int *piVar1;
  
  piVar1 = DAT_8c64db90;
  *(undefined4 *)(*DAT_8c64db90 + 0x68) = param_1;
  (*(code *)PTR_FUN_8c64db94)(*piVar1);
  return;
}

