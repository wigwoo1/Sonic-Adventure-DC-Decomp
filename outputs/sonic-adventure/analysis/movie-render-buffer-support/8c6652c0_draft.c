/* Ghidra draft only; not buildable/verified source. */

undefined4 FUN_8c6652c0(int *param_1,int param_2)

{
  short sVar1;
  int iVar2;
  int iVar3;
  
  sVar1 = DAT_8c6653a2;
  iVar2 = *(int *)(param_2 + 0xc);
  iVar3 = (int)DAT_8c665320;
  if (iVar2 == 8) {
    if ((*(uint *)(param_2 + 0x18) & 1) == 0) {
      iVar2 = (int)DAT_8c665326;
      *param_1 = iVar3;
      param_1[1] = 0x80;
      param_1[2] = 0x20;
      param_1[3] = iVar2;
    }
    else {
      iVar2 = (int)DAT_8c665324;
      *param_1 = (int)DAT_8c665322;
      param_1[1] = 0x80;
      param_1[2] = 0x20;
      param_1[3] = iVar2;
    }
  }
  else if (iVar2 == 0x10) {
    if ((*(uint *)(param_2 + 0x18) & 1) == 0) {
      *param_1 = (int)DAT_8c6653a0;
      param_1[1] = 0x80;
      param_1[2] = 0x40;
    }
    else {
      *param_1 = (int)DAT_8c665328;
      sVar1 = DAT_8c66532a;
      param_1[1] = 0x80;
      param_1[2] = 0x60;
    }
    param_1[3] = (int)sVar1;
  }
  else {
    if (iVar2 == 0x20) {
      if ((*(uint *)(param_2 + 0x18) & 1) == 0) {
        iVar2 = (int)DAT_8c6653ae;
        *param_1 = (int)DAT_8c6653ac;
        param_1[1] = iVar2;
        param_1[2] = iVar2;
        param_1[3] = iVar3;
        goto LAB_8c6653d6;
      }
      iVar3 = (int)DAT_8c6653a6;
      *param_1 = (int)DAT_8c6653a4;
      iVar2 = (int)DAT_8c6653a8;
      param_1[1] = iVar3;
      sVar1 = DAT_8c6653aa;
      param_1[2] = iVar2;
    }
    else {
      if (iVar2 != 0x40) {
        return 0;
      }
      if ((*(uint *)(param_2 + 0x18) & 1) == 0) {
        iVar2 = (int)DAT_8c665422;
        *param_1 = (int)DAT_8c665420;
        param_1[1] = iVar2;
        param_1[2] = iVar2;
        param_1[3] = iVar3;
        goto LAB_8c6653d6;
      }
      iVar3 = (int)DAT_8c6653b0;
      iVar2 = (int)DAT_8c6653b2;
      *param_1 = DAT_8c6653b8;
      param_1[1] = iVar3;
      sVar1 = DAT_8c6653b4;
      param_1[2] = iVar2;
    }
    param_1[3] = (int)sVar1;
  }
LAB_8c6653d6:
  iVar2 = (*(code *)PTR_FUN_8c665428)();
  param_1[4] = iVar2;
  return 1;
}

