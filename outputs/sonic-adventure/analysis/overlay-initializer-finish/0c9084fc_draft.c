/* Ghidra draft only; not buildable/verified source. */

void FUN_0c9084fc(int param_1,int param_2)

{
  byte *pbVar1;
  
  pbVar1 = PTR_DAT_0c9085a4 + param_2 * 2;
  *(ushort *)(param_1 + 0x808) = (ushort)*pbVar1;
  *(ushort *)(param_1 + 0x80a) = (ushort)pbVar1[1];
  return;
}

