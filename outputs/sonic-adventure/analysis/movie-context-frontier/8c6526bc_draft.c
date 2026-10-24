/* Ghidra draft only; not buildable/verified source. */

int FUN_8c6526bc(undefined4 param_1,int param_2,undefined4 param_3,undefined4 param_4)

{
  undefined *puVar1;
  undefined *puVar2;
  uint *puVar3;
  int *piVar4;
  uint uVar5;
  int iVar6;
  int iVar7;
  int iVar8;
  int iVar9;
  int iVar10;
  undefined4 local_2c;
  
  iVar10 = 0;
  (*(code *)PTR_FUN_8c652754)(0);
  puVar2 = PTR_FUN_8c652748;
  puVar1 = PTR_s_KAMUI_8c652744;
  uVar5 = (*(code *)PTR_FUN_8c652748)(PTR_s_KAMUI_8c652744,PTR_s_DisplayMode_8c652758,param_1);
  iVar6 = (*(code *)puVar2)(puVar1,PTR_s_AntiAlias_8c65275c,param_4);
  iVar7 = (*(code *)puVar2)(puVar1,PTR_s_Dither_8c652760,param_3);
  *DAT_8c65274c = uVar5 & 3;
  puVar3 = DAT_8c6527e4;
  if ((((param_2 != 5) && (param_2 != 4)) && (param_2 != 3)) && ((param_2 != 1 && (param_2 != 0))))
  {
    iVar10 = 1;
  }
  if (iVar10 == 0) {
    *DAT_8c6527e4 = uVar5;
    piVar4 = DAT_8c6527e8;
    *DAT_8c6527e8 = param_2;
    *DAT_8c6527ec = (uint)(iVar7 != 0);
    puVar3[3] = (uint)(iVar6 != 0);
    iVar8 = (int)DAT_8c6527e2;
    iVar10 = (int)DAT_8c6527e0;
    if (piVar4[-1] == 1) {
      local_2c = 0;
      *DAT_8c6527f0 = iVar10;
      iVar9 = iVar10;
    }
    else {
      if (((*DAT_8c6527e4 & 1) == 1) || ((*DAT_8c6527e4 & 0x2c) == 0)) {
        local_2c = 0;
      }
      else {
        local_2c = 1;
      }
      if ((*DAT_8c6527e4 & 0x10) == 0) {
        iVar8 = (int)DAT_8c652840;
      }
      iVar9 = (int)DAT_8c652842;
      if ((*DAT_8c652844 & 0x20) != 0) {
        iVar9 = iVar10;
      }
      *DAT_8c652848 = (int)DAT_8c652842;
    }
    iVar10 = *DAT_8c65284c;
    if (iVar10 == 1) {
      iVar10 = 0;
      (*(code *)PTR_FUN_8c652850)();
    }
    else if (iVar10 == 0) {
      iVar10 = 0;
      (*(code *)PTR_FUN_8c652854)();
    }
    else if (iVar10 == 2) {
      iVar10 = 0;
      (*(code *)PTR_FUN_8c6528e4)();
    }
    else {
      iVar10 = 1;
    }
    (*(code *)PTR_FUN_8c6528e8)(iVar8,iVar9,param_2,local_2c,(uint)(iVar7 != 0),(uint)(iVar6 != 0));
    FUN_8c6525c0();
  }
  return iVar10;
}

