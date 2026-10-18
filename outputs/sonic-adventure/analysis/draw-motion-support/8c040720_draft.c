/* Ghidra draft only; not buildable/verified source. */

void callback_8c040720(uint *param_1)

{
  if ((param_1[1] != 0) && ((*param_1 & 8) == 0)) {
    (*(code *)PTR_FUN_8c040854)(param_1[1]);
    return;
  }
  return;
}

