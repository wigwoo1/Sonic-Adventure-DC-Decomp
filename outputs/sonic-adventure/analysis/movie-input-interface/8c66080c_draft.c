/* Ghidra draft only; not buildable/verified source. */

uint callback_8c66080c(int param_1,int param_2)

{
  return ((int)*(short *)(param_1 + param_2) & 0xffU) << 8 |
         ((int)*(short *)(param_1 + param_2) & 0xff00U) >> 8;
}

