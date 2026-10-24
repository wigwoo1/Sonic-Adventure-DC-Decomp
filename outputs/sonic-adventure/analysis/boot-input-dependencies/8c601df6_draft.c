/* Ghidra draft only; not buildable/verified source. */

undefined4 FUN_8c601df6(int param_1)

{
  ushort uVar1;
  
  uVar1 = (ushort)*(byte *)(param_1 + 4);
  if (uVar1 == DAT_8c601e38) {
    return DAT_8c601e50;
  }
  if ((((uVar1 != DAT_8c601e3a) && (uVar1 != 8)) && (uVar1 != 5)) && ((uVar1 != 6 && (uVar1 != 0))))
  {
    return DAT_8c601e54;
  }
  return 0;
}

