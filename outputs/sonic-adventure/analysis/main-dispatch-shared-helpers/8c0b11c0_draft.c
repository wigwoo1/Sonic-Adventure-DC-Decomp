/* Ghidra draft only; not buildable/verified source. */

undefined4 FUN_8c0b11c0(undefined1 *param_1,undefined4 param_2,int param_3)

{
  uint uVar1;
  
  if ((*(ushort *)(param_1 + 4) & 3) != 0) {
    return 0;
  }
  if ((*(ushort *)(param_1 + 4) & 0x100) == 0) {
    if ((*(ushort *)(param_1 + 4) & 0x800) != 0) {
      uVar1 = (*(code *)PTR_FUN_8c0b12c0)(param_1);
      if ((uVar1 & 0x200) != 0) {
        *param_1 = 0x1c;
        *(undefined2 *)(param_3 + 0x124) = 0x39;
        return 1;
      }
      FUN_8c0b1d60(param_1,param_2,param_3);
    }
    *param_1 = 0xc;
    *(undefined2 *)(param_3 + 0x124) = 0x12;
  }
  else {
    *param_1 = 8;
  }
  return 1;
}

