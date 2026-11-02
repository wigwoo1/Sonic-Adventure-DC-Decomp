/* Ghidra draft only; not buildable/verified source. */

void FUN_8c63ae90(undefined4 param_1,undefined4 param_2,undefined4 param_3,undefined4 param_4,
                 int param_5)

{
  undefined *puVar1;
  undefined8 in_xd4;
  char in_FPSCR_SZ;
  
  puVar1 = PTR_FUN_8c63aec0;
  if (in_FPSCR_SZ == '\0') {
    *(undefined4 *)(param_5 + 0x4c) = param_1;
    *(undefined4 *)(param_5 + 0x50) = param_2;
    *(undefined4 *)(param_5 + 0x54) = param_3;
  }
  else {
    *(ulonglong *)(param_5 + 0x4c) = CONCAT44(param_1,param_2);
    *(undefined8 *)(param_5 + 0x50) = in_xd4;
    *(ulonglong *)(param_5 + 0x54) = CONCAT44(param_3,param_4);
  }
  (*(code *)puVar1)(param_5 + 0x4c);
  (*(code *)PTR_FUN_8c63aec4)();
  return;
}

