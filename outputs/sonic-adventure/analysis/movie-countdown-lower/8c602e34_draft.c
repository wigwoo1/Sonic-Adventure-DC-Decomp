/* Ghidra draft only; not buildable/verified source. */

undefined4 callback_8c602e34(undefined4 param_1)

{
  int iVar1;
  undefined4 uVar2;
  
  iVar1 = (*(code *)PTR_FUN_8c602f40)();
  if (iVar1 == 0) {
    (*(code *)PTR_FUN_8c602f44)(param_1,0x12);
    (*(code *)PTR_FUN_8c602f48)(param_1,param_1,0,0,0,0,0);
    uVar2 = 0;
  }
  else {
    uVar2 = 0xffffffff;
  }
  return uVar2;
}

