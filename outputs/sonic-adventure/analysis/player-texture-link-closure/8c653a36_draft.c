/* Ghidra draft only; not buildable/verified source. */

undefined4 FUN_8c653a36(uint *param_1,uint param_2,uint param_3)

{
  uint uVar1;
  int iVar2;
  uint uVar3;
  
  param_3 = param_3 & 0xfffffffc;
  if (param_3 != 0) {
    if ((*param_1 & 0xffff) == 3) {
      return 4;
    }
    uVar1 = param_1[6];
    if (((((uVar1 & 4) == 0) || ((uVar1 & 1) == 0)) || ((uVar1 & 8) != 0)) || ((uVar1 & 0x20) != 0))
    {
      iVar2 = 0;
    }
    else {
      iVar2 = 4;
    }
    uVar3 = (param_2 & 0xfffffffc) + iVar2;
    uVar1 = param_1[5] - uVar3;
    if (param_1[5] <= uVar3) {
      return 7;
    }
    if (param_3 < uVar1) {
      uVar1 = param_3;
    }
    (*(code *)PTR_FUN_8c653ae0)(uVar3 + param_1[7],uVar1);
  }
  return 0;
}

