/* Ghidra draft only; not buildable/verified source. */

int FUN_8c666b34(undefined4 param_1)

{
  undefined *puVar1;
  undefined4 *puVar2;
  undefined *puVar3;
  int iVar4;
  
  puVar3 = PTR_FUN_8c666b98;
  puVar2 = DAT_8c666b94;
  puVar1 = PTR_DAT_8c666b8c;
  iVar4 = DAT_8c666b90;
  if (*(int *)PTR_DAT_8c666b8c != 0) {
    iVar4 = (int)DAT_8c666b86;
    *(undefined4 *)((int)DAT_8c666b84 + *(int *)PTR_DAT_8c666b8c) = param_1;
    *puVar2 = param_1;
    (*(code *)puVar3)(0,iVar4);
    *(undefined4 *)((int)DAT_8c666b88 + *(int *)puVar1) = 0;
    iVar4 = (*(code *)PTR_FUN_8c666b9c)(*(int *)puVar1 + (int)DAT_8c666b8a);
  }
  if (iVar4 != 0) {
    (*(code *)PTR_FUN_8c666ba4)(PTR_FUN_8c666ba0,iVar4);
  }
  return iVar4;
}

