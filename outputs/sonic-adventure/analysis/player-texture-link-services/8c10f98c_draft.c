/* Ghidra draft only; not buildable/verified source. */

uint FUN_8c10f98c(uint param_1)

{
  if ((PTR_DAT_8c10f9a4[param_1 & 0xff] & 2) != 0) {
    return param_1 - 0x20;
  }
  return param_1;
}

