/* Ghidra draft only; not buildable/verified source. */

undefined4 FUN_8c653f08(undefined4 *param_1,int param_2,int param_3,uint param_4)

{
  undefined4 uVar1;
  uint uVar2;
  undefined4 uVar3;
  uint uVar4;
  
  uVar3 = 0;
  uVar4 = DAT_8c653f50 & param_4;
  param_4 = param_4 & 0xff;
  if ((uVar4 == (int)DAT_8c653f40) || (uVar4 == (int)DAT_8c653f42)) {
    *param_1 = 3;
  }
  else {
    *param_1 = 2;
  }
  if ((uVar4 == (int)DAT_8c653fc0) || (uVar4 == (int)DAT_8c653fc2)) {
    if (*(int *)(*(int *)PTR_DAT_8c653fcc + (int)DAT_8c653fc4) == 3) {
      param_1[1] = 3;
      uVar1 = DAT_8c653fd0;
    }
    else {
      param_1[1] = 1;
      uVar1 = DAT_8c653fd0;
    }
  }
  else {
    if ((uVar4 != (int)DAT_8c653fc6) && (uVar4 != (int)DAT_8c653fc8)) {
      param_1[1] = 1;
      if (param_4 == 0) {
        param_1[2] = 0;
        goto LAB_8c654042;
      }
      uVar1 = DAT_8c654020;
      if (param_4 == 1) {
LAB_8c65400e:
        param_1[2] = uVar1;
      }
      else {
        uVar1 = DAT_8c654024;
        if (param_4 != 2) {
          uVar1 = DAT_8c654028;
          if (param_4 == 3) goto LAB_8c65400e;
          uVar1 = DAT_8c65402c;
          if (param_4 != 4) {
            uVar3 = 4;
            goto LAB_8c654042;
          }
        }
        param_1[2] = uVar1;
      }
      goto LAB_8c654042;
    }
    if (*(int *)(*(int *)PTR_DAT_8c653fcc + (int)DAT_8c653fc4) == 3) {
      param_1[1] = 3;
      uVar1 = DAT_8c653fd4;
    }
    else {
      param_1[1] = 1;
      uVar1 = DAT_8c653fd4;
    }
  }
  param_1[2] = uVar1;
LAB_8c654042:
  param_1[3] = param_2;
  param_1[4] = param_3;
  if ((((param_2 != 8) && (param_2 != 0x10)) && (param_2 != 0x20)) &&
     (((param_2 != 0x40 && (param_2 != DAT_8c654100)) &&
      ((param_2 != DAT_8c654102 && ((param_2 != DAT_8c654104 && (param_2 != DAT_8c654106)))))))) {
    uVar3 = 4;
  }
  if (((((param_3 != 8) && (param_3 != 0x10)) && (param_3 != 0x20)) &&
      ((param_3 != 0x40 && (param_3 != DAT_8c654100)))) &&
     ((param_3 != DAT_8c654102 && ((param_3 != DAT_8c654104 && (param_3 != DAT_8c654106)))))) {
    uVar3 = 4;
  }
  if (((((uVar4 != (int)DAT_8c654108) && (uVar4 != (int)DAT_8c65410a)) &&
       (uVar4 != (int)DAT_8c65410c)) &&
      ((uVar4 != (int)DAT_8c65410e && (uVar4 != (int)DAT_8c654110)))) && (param_2 != param_3)) {
    uVar3 = 4;
  }
  uVar2 = (uint)DAT_8c654112;
  param_1[6] = 0;
  if (((uVar4 == uVar2) || (uVar4 == (int)DAT_8c654104)) ||
     ((uVar4 == (int)DAT_8c654106 ||
      (((uVar4 == (int)DAT_8c654114 || (uVar4 == (int)DAT_8c654116)) || (uVar4 == (int)DAT_8c654118)
       ))))) {
    param_1[6] = param_1[6] | 1;
  }
  if (((uVar4 == (int)DAT_8c65411a) || (uVar4 == (int)DAT_8c654112)) ||
     ((uVar4 == (int)DAT_8c6541c0 || ((uVar4 == (int)DAT_8c6541c2 || (uVar4 == (int)DAT_8c6541c4))))
     )) {
    param_1[6] = param_1[6] | 4;
  }
  if ((((uVar4 == (int)DAT_8c6541c6) || (uVar4 == (int)DAT_8c6541c8)) ||
      (uVar4 == (int)DAT_8c6541ca)) || (uVar4 == (int)DAT_8c6541cc)) {
    param_1[6] = param_1[6] | 0xc;
  }
  if (uVar4 == (int)DAT_8c6541ce) {
    param_1[6] = param_1[6] | 0x10;
  }
  if (((uVar4 == (int)DAT_8c6541d0) || (uVar4 == (int)DAT_8c6541d2)) ||
     ((uVar4 == (int)DAT_8c6541d4 || (uVar4 == (int)DAT_8c6541d6)))) {
    param_1[6] = param_1[6] | 0x24;
  }
  FUN_8c654286(&DAT_8c88f7a8);
  return uVar3;
}

