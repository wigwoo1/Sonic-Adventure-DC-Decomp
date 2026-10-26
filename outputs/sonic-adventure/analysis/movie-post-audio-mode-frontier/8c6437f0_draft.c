/* Ghidra draft only; not buildable/verified source. */

int FUN_8c6437f0(char param_1,char param_2)

{
  int *piVar1;
  char *pcVar2;
  undefined *puVar3;
  int iVar4;
  
  pcVar2 = DAT_8c64386c;
  piVar1 = DAT_8c643868;
  iVar4 = DAT_8c643860;
  if (((*(int *)PTR_DAT_8c64385c != 0) && (iVar4 = DAT_8c643864, -1 < param_1)) && (-1 < param_2)) {
    *DAT_8c643868 = (int)DAT_8c643858;
    puVar3 = PTR_FUN_8c643870;
    *pcVar2 = param_1;
    iVar4 = (*(code *)puVar3)(piVar1);
    pcVar2 = DAT_8c64386c;
    piVar1 = DAT_8c643868;
    if (iVar4 == 0) {
      *DAT_8c643868 = (int)DAT_8c64385a;
      *pcVar2 = param_2;
      iVar4 = (*(code *)PTR_FUN_8c643870)(piVar1);
    }
  }
  if (iVar4 != 0) {
    (*(code *)PTR_FUN_8c643878)(PTR_FUN_8c643874,iVar4);
  }
  return iVar4;
}

