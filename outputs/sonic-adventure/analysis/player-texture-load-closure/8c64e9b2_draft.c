/* Ghidra draft only; not buildable/verified source. */

undefined4 FUN_8c64e9b2(int *param_1,int param_2,int param_3,uint param_4)

{
  undefined *puVar1;
  uint uVar2;
  uint uVar3;
  int iVar4;
  int iVar5;
  int iVar6;
  int iVar7;
  
  iVar7 = DAT_8c64eb38;
  iVar5 = DAT_8c64ea90;
  iVar6 = DAT_8c64ea8c;
  uVar2 = param_4 & 0xff;
  uVar3 = DAT_8c64ea88 & param_4;
  if ((uVar3 == (int)DAT_8c64ea6e) || (uVar3 == (int)DAT_8c64ea70)) {
    param_1[6] = 4;
LAB_8c64eaa8:
    iVar6 = 0;
  }
  else if (uVar3 == (int)DAT_8c64ea72) {
    param_1[6] = 5;
    iVar6 = 1;
  }
  else if (uVar3 == (int)DAT_8c64ea74) {
    param_1[6] = 8;
    iVar6 = 2;
  }
  else if (uVar3 == (int)DAT_8c64ea76) {
    param_1[6] = 8;
    iVar6 = 3;
  }
  else if (uVar3 == (int)DAT_8c64ea78) {
    param_1[6] = 9;
    iVar6 = 4;
  }
  else if (uVar3 == (int)DAT_8c64ea7a) {
    param_1[6] = 9;
    iVar6 = 5;
  }
  else if (uVar3 == (int)DAT_8c64ea7c) {
    param_1[6] = 0x20;
    param_1[2] = iVar6;
    iVar6 = 6;
  }
  else if (uVar3 == (int)DAT_8c64ea7e) {
    param_1[6] = 0x21;
    param_1[2] = iVar6;
    iVar6 = 7;
  }
  else if (uVar3 == (int)DAT_8c64ea80) {
    param_1[6] = 0x20;
    param_1[2] = iVar5;
    iVar6 = 8;
  }
  else {
    if (uVar3 != (int)DAT_8c64ea82) {
      if (uVar3 == (int)DAT_8c64ea84) {
        param_1[6] = 0;
      }
      else {
        if (uVar3 != (int)DAT_8c64ea86) {
          return 0xffffffff;
        }
        param_1[6] = 0x10;
      }
      goto LAB_8c64eaa8;
    }
    param_1[6] = 0x21;
    param_1[2] = iVar7;
    iVar6 = 9;
  }
  if ((param_1[6] & 0x20U) != 0) goto LAB_8c64eae4;
  if (uVar2 == 0) {
    param_1[2] = 0;
    goto LAB_8c64eae4;
  }
  iVar5 = DAT_8c64eb3c;
  if (uVar2 == 1) {
LAB_8c64eadc:
    param_1[2] = iVar5;
  }
  else {
    iVar5 = DAT_8c64eb40;
    if (uVar2 != 2) {
      iVar5 = DAT_8c64eb44;
      if (uVar2 == 3) goto LAB_8c64eadc;
      iVar5 = DAT_8c64eb48;
      if (uVar2 != 4) {
        return 0xffffffff;
      }
    }
    param_1[2] = iVar5;
  }
LAB_8c64eae4:
  puVar1 = PTR_DAT_8c64ec1c;
  if (param_2 == 8) {
    iVar5 = 7;
  }
  else if (param_2 == 0x10) {
    iVar5 = 6;
  }
  else if (param_2 == 0x20) {
    iVar5 = 5;
  }
  else if (param_2 == 0x40) {
    iVar5 = 4;
  }
  else if (param_2 == DAT_8c64eb2e) {
    iVar5 = 3;
  }
  else if (param_2 == DAT_8c64eb30) {
    iVar5 = 2;
  }
  else if (param_2 == DAT_8c64eb32) {
    iVar5 = 1;
  }
  else {
    if (param_2 != DAT_8c64eb34) {
      return 0xffffffff;
    }
    iVar5 = 0;
  }
  if (((((param_3 != 8) && (param_3 != 0x10)) && (param_3 != 0x20)) &&
      ((param_3 != 0x40 && (param_3 != DAT_8c64ec12)))) &&
     ((param_3 != DAT_8c64ec14 && ((param_3 != DAT_8c64ec16 && (param_3 != DAT_8c64ec18)))))) {
    return 0xffffffff;
  }
  iVar7 = param_2 * param_3;
  *param_1 = param_4 << 0x10;
  param_1[3] = param_2;
  param_1[4] = param_3;
  iVar5 = iVar5 * 4;
  if (iVar6 == 0) {
    iVar4 = iVar7 * 2;
  }
  else {
    if (iVar6 == 1) {
      param_1[5] = *(int *)(puVar1 + iVar5);
      return 1;
    }
    iVar4 = iVar7 >> 1;
    if (iVar6 == 2) {
      iVar4 = (iVar7 >> 2) + (int)DAT_8c64ec1a;
    }
    else {
      if (iVar6 == 3) {
        param_1[5] = *(int *)(PTR_DAT_8c64ec20 + iVar5) + (iVar7 >> 2);
        return 1;
      }
      if (iVar6 == 4) {
        param_1[5] = (*(int *)(puVar1 + iVar5) + 4U >> 3) + (int)DAT_8c64ec1a;
        return 1;
      }
      if (iVar6 == 5) {
        param_1[5] = (*(int *)(puVar1 + iVar5) + 4U >> 3) + *(int *)(PTR_DAT_8c64ec24 + iVar5);
        return 1;
      }
      if (iVar6 != 6) {
        if (iVar6 == 7) {
          param_1[5] = *(int *)(puVar1 + iVar5) + 4U >> 2;
          return 1;
        }
        iVar4 = iVar7;
        if (iVar6 != 8) {
          if (iVar6 != 9) {
            return 1;
          }
          param_1[5] = *(int *)(puVar1 + iVar5) + 4U >> 1;
          return 1;
        }
      }
    }
  }
  param_1[5] = iVar4;
  return 1;
}

