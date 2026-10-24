/* Ghidra draft only; not buildable/verified source. */

undefined4 FUN_8c646b72(uint *param_1,undefined4 param_2)

{
  byte *pbVar1;
  
  pbVar1 = (byte *)FUN_8c646b42(param_2,1);
  if (pbVar1 == (byte *)0x0) {
    return 0xffffffff;
  }
  *param_1 = (uint)pbVar1[1] << 0x10 | (uint)pbVar1[2] << 8 | (uint)*pbVar1 << 0x18 |
             (uint)pbVar1[3];
  return 0;
}

