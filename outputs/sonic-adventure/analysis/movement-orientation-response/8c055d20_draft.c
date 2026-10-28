/* Ghidra draft only; not buildable/verified source. */

uint FUN_8c055d20(uint param_1,ushort param_2,int param_3)

{
  uint uVar1;
  
  uVar1 = (uint)(short)(param_2 - (short)param_1);
  if (((int)uVar1 <= param_3) && (-param_3 <= (int)uVar1)) {
    return (uint)param_2;
  }
  if ((DAT_8c055e20 & uVar1) != 0) {
    param_3 = -param_3;
  }
  return (param_1 & 0xffff) + param_3 & 0xffff;
}

