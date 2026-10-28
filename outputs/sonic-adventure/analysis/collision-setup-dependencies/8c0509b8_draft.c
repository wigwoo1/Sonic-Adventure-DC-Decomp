/* Ghidra draft only; not buildable/verified source. */

void FUN_8c0509b8(byte param_1)

{
  undefined *puVar1;
  
  puVar1 = PTR_DAT_8c050a28;
  if ((param_1 != 0) && (param_1 != 1)) {
    *PTR_DAT_8c050a24 = 1;
    *puVar1 = 1;
    return;
  }
  PTR_DAT_8c050a24[param_1] = 1;
  return;
}

