/* Ghidra draft only; not buildable/verified source. */

int FUN_8c09d9c6(undefined4 param_1)

{
  int iVar1;
  int iVar2;
  
  iVar1 = FUN_8c09da74();
  if ((iVar1 != -1) && (iVar1 = (*(code *)PTR_FUN_8c09da38)(iVar1), iVar1 != 0)) {
    iVar2 = FUN_8c09cc80(param_1,iVar1);
    if (iVar2 != -1) {
      return iVar1;
    }
    (*(code *)PTR_FUN_8c09da3c)(iVar1,0xffffffff);
  }
  return 0;
}

