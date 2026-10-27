/* Ghidra draft only; not buildable/verified source. */

void callback_8c66081e(int param_1,int param_2,uint param_3)

{
  *(ushort *)(param_1 + param_2) = (ushort)((param_3 & 0xff) << 8) | (ushort)(param_3 >> 8) & 0xff;
  return;
}

