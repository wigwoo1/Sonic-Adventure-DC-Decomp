/* Ghidra draft only; not buildable/verified source. */

void FUN_8c654520(int param_1,int param_2,undefined4 param_3,uint param_4)

{
  int iVar1;
  int iVar2;
  
  iVar1 = FUN_8c6541e0(param_3,&DAT_8c88f7a8);
  iVar1 = iVar1 * 4;
  if ((param_4 & 1) == 0) {
    iVar2 = *(int *)(PTR_DAT_8c6545cc + iVar1);
    iVar1 = *(int *)(PTR_DAT_8c6545d0 + iVar1);
  }
  else {
    iVar2 = *(int *)(PTR_DAT_8c6545c4 + iVar1);
    iVar1 = *(int *)(PTR_DAT_8c6545c8 + iVar1) + 10;
  }
  (*(code *)PTR_FUN_8c6545d4)(param_1,param_2,iVar2);
  (*(code *)PTR_FUN_8c6545d4)(param_1 + DAT_8c6545c0,param_2 + iVar2,iVar1);
  return;
}

