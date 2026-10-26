/* Ghidra draft only; not buildable/verified source. */

int FUN_8c643b9c(int param_1,undefined1 *param_2)

{
  int *piVar1;
  int *piVar2;
  undefined1 *puVar3;
  undefined *puVar4;
  int iVar5;
  
  puVar3 = DAT_8c643c04;
  piVar1 = DAT_8c643c00;
  iVar5 = DAT_8c643bf8;
  if ((*DAT_8c643bf4 != 0) && (iVar5 = DAT_8c643bfc, param_1 != 0)) {
    *DAT_8c643c00 = (int)DAT_8c643bf2;
    piVar2 = DAT_8c643c00;
    *puVar3 = *param_2;
    puVar4 = PTR_FUN_8c643c0c;
    puVar3 = DAT_8c643c08;
    *DAT_8c643c08 = param_2[1];
    puVar3[1] = param_2[2];
    *(undefined1 *)((int)piVar1 + 9) = param_2[3];
    iVar5 = (*(code *)puVar4)(piVar2);
  }
  if (iVar5 != 0) {
    (*(code *)PTR_FUN_8c643c14)(PTR_FUN_8c643c10,iVar5);
  }
  return iVar5;
}

