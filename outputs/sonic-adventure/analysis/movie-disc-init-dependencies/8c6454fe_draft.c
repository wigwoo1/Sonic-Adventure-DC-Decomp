/* Ghidra draft only; not buildable/verified source. */

int FUN_8c6454fe(char *param_1,int param_2,int *param_3,int param_4)

{
  int iVar1;
  int iVar2;
  int iVar3;
  int *piVar4;
  char *unaff_r13;
  int iVar5;
  int local_2c;
  undefined1 *puVar6;
  
  puVar6 = &DAT_8c88e970;
  piVar4 = param_3 + 4;
  if (param_3[2] != param_2) {
    iVar5 = 0;
    iVar2 = (int)DAT_8c645550;
    param_3[2] = param_2;
    iVar3 = param_3[1];
    local_2c = param_2;
    for (; 0 < param_4; param_4 = param_4 - iVar2) {
      iVar1 = (*(code *)PTR_FUN_8c645554)(local_2c,1,param_1,puVar6);
      if (iVar1 != 0) {
        param_3[2] = 0;
        return iVar1;
      }
      unaff_r13 = param_1;
      if (iVar3 <= iVar5) break;
      while (iVar1 = FUN_8c645468(unaff_r13,piVar4), iVar1 != 0) {
        if ((iVar5 == 0) && (piVar4[1] != param_4)) {
          param_4 = piVar4[1];
        }
        unaff_r13 = unaff_r13 + iVar1;
        piVar4 = piVar4 + 0xb;
        iVar5 = iVar5 + 1;
        if ((param_1 + iVar2 <= unaff_r13) || (iVar3 <= iVar5)) break;
      }
      if (iVar3 <= iVar5) break;
      local_2c = local_2c + 1;
    }
    *param_3 = iVar5;
    if (((iVar5 == iVar3) && (param_4 != 0)) &&
       ((param_4 != iVar2 || ((unaff_r13 < param_1 + iVar2 && (*unaff_r13 != '\0')))))) {
      param_3[2] = 0;
      return -0xc;
    }
  }
  return 0;
}

