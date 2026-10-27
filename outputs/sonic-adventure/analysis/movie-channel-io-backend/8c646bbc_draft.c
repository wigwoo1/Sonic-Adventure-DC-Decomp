/* Ghidra draft only; not buildable/verified source. */

/* WARNING: Removing unreachable block (ram,0x8c646c10) */

undefined4 FUN_8c646bbc(short *param_1,undefined4 param_2)

{
  byte *pbVar1;
  
  pbVar1 = (byte *)FUN_8c646b42(param_2,2);
  if (pbVar1 != (byte *)0x0) {
    *param_1 = *pbVar1 + 1;
    param_1[1] = (pbVar1[1] + 1) * 0x20;
    param_1[2] = (ushort)(pbVar1[2] >> 4);
    param_1[3] = pbVar1[2] & 0xf;
    param_1[4] = (ushort)(pbVar1[3] >> 7);
    return 0;
  }
  return 0xffffffff;
}

