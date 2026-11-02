/* Ghidra draft only; not buildable/verified source. */

void FUN_8c019be6(short param_1,char *param_2)

{
  char cVar1;
  int iVar2;
  
  iVar2 = 0;
  if ((*(short *)(*(int *)PTR_DAT_8c019ca0 + 4) != param_1) && (*DAT_8c019ca4 != 0)) {
    cVar1 = param_2[3];
    if (cVar1 == '\0') {
      iVar2 = FUN_8c01a1ac(param_2);
    }
    else if (cVar1 == '\x01') {
      iVar2 = FUN_8c01a240(param_2);
    }
    else if (cVar1 == '\x02') {
      iVar2 = FUN_8c01a480(param_2);
    }
    if (iVar2 != 0) {
      FUN_8c01a7d6((int)*param_2,(int)param_1);
      (*(code *)PTR_FUN_8c019cac)(8,6,PTR_LAB_8c019ca8);
      return;
    }
  }
  return;
}

