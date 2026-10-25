/* Ghidra draft only; not buildable/verified source. */

uint FUN_8c5ff9c0(byte *param_1)

{
  return (uint)param_1[1] << 8 | (uint)param_1[2] << 0x10 | (uint)param_1[3] << 0x18 |
         (uint)*param_1;
}

