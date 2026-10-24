/* Ghidra draft only; not buildable/verified source. */

int callback_8c6450ea(undefined4 *param_1)

{
  int iVar1;
  undefined4 uVar2;
  int iStack_18;
  undefined4 uStack_14;
  undefined4 uStack_10;
  undefined4 uStack_c;
  
  iVar1 = (*(code *)PTR_FUN_8c6451f4)(&iStack_18);
  if (iVar1 == 0) {
    return 0;
  }
  if (iVar1 == 1) {
    *param_1 = uStack_10;
    param_1[2] = uStack_c;
    return 4;
  }
  if (iVar1 == 2) {
    *param_1 = uStack_10;
    return 1;
  }
  if (iVar1 == 3) {
    return 0x41;
  }
  if (iVar1 == 4) {
    return 0x43;
  }
  if (iVar1 != -1) {
    return iVar1;
  }
  param_1[1] = uStack_14;
  if (iStack_18 == 0) {
    *param_1 = 0xffffffe5;
    return 5;
  }
  if (iStack_18 == 1) {
    uVar2 = 0xffffffe4;
LAB_8c645192:
    *param_1 = uVar2;
  }
  else {
    if (iStack_18 == 2) {
      uVar2 = 0xffffffe3;
    }
    else {
      if (iStack_18 == 3) {
        uVar2 = 0xffffffe2;
        goto LAB_8c645192;
      }
      if (iStack_18 == 4) {
        uVar2 = 0xffffffe1;
      }
      else {
        if (iStack_18 == 5) {
          uVar2 = 0xffffffe0;
          goto LAB_8c645192;
        }
        if (iStack_18 == 6) {
          uVar2 = 0xffffffdf;
        }
        else {
          if (iStack_18 == 7) {
            uVar2 = 0xffffffde;
            goto LAB_8c645192;
          }
          if (iStack_18 == 0xb) {
            uVar2 = 0xffffffdd;
          }
          else {
            if (iStack_18 == 0x10) {
              uVar2 = 0xffffffdc;
              goto LAB_8c645192;
            }
            uVar2 = 0xffffffe6;
          }
        }
      }
    }
    *param_1 = uVar2;
  }
  return 5;
}

