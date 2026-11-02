/* Ghidra draft only; not buildable/verified source. */

void FUN_8c019c50(undefined4 param_1,char *param_2)

{
  undefined *puVar1;
  int iVar2;
  
  puVar1 = PTR_FUN_8c019cac;
  iVar2 = *(int *)PTR_DAT_8c019ca0;
  if (*(short *)(iVar2 + 4) == (short)param_1) {
    if (*DAT_8c019ca4 != 0) {
      if (param_2[3] == '\0') {
        iVar2 = FUN_8c01a1ac(param_2,&DAT_8c11204c);
      }
      else if (param_2[3] == '\x02') {
        iVar2 = FUN_8c01a480(param_2);
      }
      else {
        iVar2 = 0;
      }
      if (iVar2 == 0) {
        FUN_8c01a9b8();
        (*(code *)puVar1)(8,6,PTR_LAB_8c019d80);
      }
    }
  }
  else if (*(char *)(iVar2 + 8) < *(char *)(*param_2 * 0x10 + *DAT_8c019d84 + 4)) {
    if (param_2[3] == '\0') {
      iVar2 = FUN_8c01a1ac(param_2,&DAT_8c11204c);
    }
    else if (param_2[3] == '\x02') {
      iVar2 = FUN_8c01a480(param_2);
    }
    else {
      iVar2 = 0;
    }
    if (iVar2 != 0) {
      FUN_8c01a7d6((int)*param_2,param_1);
      (*(code *)puVar1)(8,6,PTR_LAB_8c019d80);
    }
  }
  else if (((*(char *)(*DAT_8c019d84 + *param_2 * 0x10 + 4) == *(char *)(iVar2 + 8)) &&
           (*DAT_8c019ca4 != 0)) &&
          (*(char *)(*(short *)(iVar2 + 4) * 0x40 + *DAT_8c019d88 + 1) < param_2[1])) {
    if (param_2[3] == '\0') {
      iVar2 = FUN_8c01a1ac(param_2,&DAT_8c11204c);
    }
    else if (param_2[3] == '\x02') {
      iVar2 = FUN_8c01a480(param_2);
    }
    else {
      iVar2 = 0;
    }
    if (iVar2 != 0) {
      FUN_8c01a9b8();
      FUN_8c01a7d6((int)*param_2,param_1);
      (*(code *)puVar1)(8,6,PTR_LAB_8c019f20);
    }
  }
  return;
}

