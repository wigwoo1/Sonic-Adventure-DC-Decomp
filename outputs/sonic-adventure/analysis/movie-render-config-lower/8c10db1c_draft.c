/* Ghidra draft only; not buildable/verified source. */

int FUN_8c10db1c(uint *param_1,uint *param_2)

{
  int iVar1;
  uint uVar2;
  int iVar3;
  uint uVar4;
  uint uVar5;
  uint *puVar6;
  uint *puVar7;
  
  if ((((uint)param_1 | (uint)param_2) & 3) != 0) {
    while( true ) {
      iVar1 = (int)(char)*param_1;
      iVar3 = (int)(char)*param_2;
      if ((iVar1 == 0) || (iVar3 != iVar1)) break;
      iVar1 = (int)*(char *)((int)param_1 + 1);
      iVar3 = (int)*(char *)((int)param_2 + 1);
      if ((iVar1 == 0) || (iVar3 != iVar1)) break;
      iVar1 = (int)*(char *)((int)param_1 + 2);
      iVar3 = (int)*(char *)((int)param_2 + 2);
      if ((iVar1 == 0) || (iVar3 != iVar1)) break;
      iVar1 = (int)*(char *)((int)param_1 + 3);
      param_1 = param_1 + 1;
      iVar3 = (int)*(char *)((int)param_2 + 3);
      param_2 = param_2 + 1;
      if ((iVar1 == 0) || (iVar3 != iVar1)) break;
    }
    return iVar1 - iVar3;
  }
  uVar4 = *param_2;
  param_2 = param_2 + 1;
  do {
    puVar6 = param_1 + 1;
    puVar7 = param_2;
    if (((uVar4 >> 0x18 == 0 || (uVar4 & 0xff0000) == 0) || (uVar4 & 0xff00) == 0) ||
        (uVar4 & 0xff) == 0) goto LAB_8c10db5a;
    uVar2 = *param_2;
    puVar7 = param_2 + 1;
    if (uVar4 != *param_1) break;
    uVar4 = *puVar6;
    puVar6 = param_1 + 2;
    if (((uVar2 >> 0x18 == 0 || (uVar2 & 0xff0000) == 0) || (uVar2 & 0xff00) == 0) ||
        (uVar2 & 0xff) == 0) goto LAB_8c10db5a;
    uVar5 = *puVar7;
    puVar7 = param_2 + 2;
    if (uVar2 != uVar4) break;
    uVar4 = *puVar6;
    puVar6 = param_1 + 3;
    if (((uVar5 >> 0x18 == 0 || (uVar5 & 0xff0000) == 0) || (uVar5 & 0xff00) == 0) ||
        (uVar5 & 0xff) == 0) goto LAB_8c10db5a;
    uVar2 = *puVar7;
    puVar7 = param_2 + 3;
    if (uVar5 != uVar4) break;
    uVar5 = *puVar6;
    puVar6 = param_1 + 4;
    if (((uVar2 >> 0x18 == 0 || (uVar2 & 0xff0000) == 0) || (uVar2 & 0xff00) == 0) ||
        (uVar2 & 0xff) == 0) goto LAB_8c10db5a;
    uVar4 = *puVar7;
    param_2 = param_2 + 4;
    param_1 = puVar6;
    puVar7 = param_2;
  } while (uVar2 == uVar5);
  puVar7 = puVar7 + -1;
LAB_8c10db5a:
  iVar3 = (int)(char)puVar7[-1];
  iVar1 = (int)(char)puVar6[-1];
  if ((iVar3 != 0) && (iVar3 == iVar1)) {
    iVar3 = (int)*(char *)((int)puVar7 + -3);
    iVar1 = (int)*(char *)((int)puVar6 + -3);
    if ((iVar3 != 0) && (iVar3 == iVar1)) {
      iVar3 = (int)*(char *)((int)puVar7 + -2);
      iVar1 = (int)*(char *)((int)puVar6 + -2);
      if ((iVar3 != 0) && (iVar3 == iVar1)) {
        iVar1 = (int)*(char *)((int)puVar6 + -1);
        iVar3 = (int)*(char *)((int)puVar7 + -1);
      }
    }
  }
  return iVar1 - iVar3;
}

