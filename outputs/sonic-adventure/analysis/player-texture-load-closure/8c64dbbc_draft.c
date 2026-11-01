/* Ghidra draft only; not buildable/verified source. */

void FUN_8c64dbbc(int param_1,int param_2,ushort *param_3)

{
  ushort uVar1;
  uint uVar2;
  uint uVar3;
  
  uVar2 = DAT_8c64dcf4;
  if (*DAT_8c64dcf8 == 0x10) {
    param_2 = param_2 + param_1;
    uVar3 = param_1 * 4 + DAT_8c64dcfc;
    for (; param_1 < param_2; param_1 = param_1 + 1) {
      uVar1 = *param_3;
      param_3 = param_3 + 1;
      *(uint *)(uVar3 | uVar2) = (uint)uVar1;
      uVar3 = uVar3 + 4;
    }
    return;
  }
  (*(code *)PTR_FUN_8c64dd00)(param_1 * 4 + DAT_8c64dcfc | DAT_8c64dcf4,param_3,param_2);
  return;
}

