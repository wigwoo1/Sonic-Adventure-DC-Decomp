/* Ghidra draft only; not buildable/verified source. */

undefined4 FUN_8c646c8a(short *param_1,undefined4 param_2)

{
  byte *pbVar1;
  
  pbVar1 = (byte *)FUN_8c646b42(param_2,4);
  if (pbVar1 == (byte *)0x0) {
    return 0xffffffff;
  }
  *param_1 = *pbVar1 + 1;
  param_1[1] = (pbVar1[1] + 1) * 0x20;
  param_1[2] = (ushort)(pbVar1[2] >> 4);
  *(byte *)(param_1 + 3) = pbVar1[3] >> 6;
  *(byte *)((int)param_1 + 7) = pbVar1[3] >> 5 & 1;
  return 0;
}

