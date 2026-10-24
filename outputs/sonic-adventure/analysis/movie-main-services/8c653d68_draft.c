/* Ghidra draft only; not buildable/verified source. */

void FUN_8c653d68(uint *param_1)

{
  if ((*param_1 & 0xffff) == 3) {
    (*(code *)PTR_FUN_8c653dac)(param_1);
  }
  else {
    (*(code *)PTR_FUN_8c653db0)(param_1[7]);
  }
  return;
}

