/* Ghidra draft only; not buildable/verified source. */

void FUN_8c098998(int param_1)

{
  if (*(int *)(*(int *)(param_1 + 0x20) + 0x38) != 0) {
    (*(code *)PTR_FUN_8c098a60)(*(undefined4 *)(param_1 + 0x20));
  }
  if (*(int *)(*(int *)(param_1 + 0x20) + 0x3c) != 0) {
    (*(code *)PTR_FUN_8c098a64)(*(undefined4 *)(param_1 + 0x20));
  }
  FUN_8c098b42(*(undefined4 *)(param_1 + 0x20));
  *(undefined4 *)(param_1 + 0x20) = 0;
  return;
}

