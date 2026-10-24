/* Ghidra draft only; not buildable/verified source. */

/* WARNING: Removing unreachable block (ram,0x8c645044) */

int callback_8c644ff6(undefined4 param_1,int param_2,uint param_3,int param_4)

{
  int iVar1;
  undefined4 uVar2;
  undefined4 uStack_1c;
  int iStack_18;
  uint uStack_14;
  undefined4 uStack_10;
  
  uStack_10 = 0;
  uStack_14 = DAT_8c6451dc & param_3;
  if (param_4 == 1) {
    uVar2 = 0x11;
  }
  else {
    if (param_4 != 0) {
      return -0x12;
    }
    uVar2 = 0x10;
    uStack_14 = uStack_14 | DAT_8c6451e0;
  }
  uStack_1c = param_1;
  iStack_18 = param_2;
  iVar1 = (*(code *)PTR_FUN_8c6451e4)(uVar2,&uStack_1c);
  if ((iVar1 != 0) && (((int)param_3 >> 0x1c & 0xeU) != 10)) {
    if ((param_3 & 0x1f) == 0) {
      (*(code *)PTR_FUN_8c6451e8)(param_3);
    }
    else {
      (*(code *)PTR_FUN_8c6451ec)(param_3,param_2 << 0xb);
    }
  }
  return iVar1;
}

