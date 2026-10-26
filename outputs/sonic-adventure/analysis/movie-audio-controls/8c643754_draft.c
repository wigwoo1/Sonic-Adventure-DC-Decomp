/* Ghidra draft only; not buildable/verified source. */

int FUN_8c643754(char param_1,char param_2,char param_3)

{
  char cVar1;
  int *piVar2;
  int *piVar3;
  char *pcVar4;
  undefined *puVar5;
  int iVar6;
  int iVar7;
  
  pcVar4 = DAT_8c6437dc;
  piVar2 = DAT_8c6437d8;
  iVar6 = DAT_8c6437d0;
  if (((*(int *)PTR_DAT_8c6437cc != 0) && (iVar7 = (int)param_1, iVar6 = DAT_8c6437d4, -1 < iVar7))
     && (iVar7 < 0x10)) {
    if (*(char *)((int)DAT_8c6437c6 + *(int *)PTR_DAT_8c6437cc) < '\'') {
      param_3 = param_3 >> 3;
    }
    *DAT_8c6437d8 = (int)DAT_8c6437c8;
    *pcVar4 = param_1;
    puVar5 = PTR_FUN_8c6437e4;
    piVar3 = DAT_8c6437d8;
    cVar1 = (char)DAT_8c6437ca;
    *DAT_8c6437e0 = param_3 + cVar1;
    *(char *)((int)piVar2 + 6) = param_2 + cVar1;
    iVar6 = (*(code *)puVar5)(piVar3,iVar7);
  }
  if (iVar6 != 0) {
    (*(code *)PTR_FUN_8c6437ec)(PTR_FUN_8c6437e8,iVar6);
  }
  return iVar6;
}

