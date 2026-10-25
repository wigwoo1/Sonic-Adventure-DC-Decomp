/* Ghidra draft only; not buildable/verified source. */

undefined4 FUN_8c5ffc98(undefined4 param_1,int param_2,undefined4 param_3)

{
  int iVar1;
  undefined4 uVar2;
  
  iVar1 = (*(code *)PTR_FUN_8c5ffd1c)
                    (*(undefined4 *)(*(int *)PTR_DAT_8c5ffd18 + 0x14),0,param_1,param_2 << 0xb);
  if (iVar1 == 0) {
    return 0xfffffff3;
  }
  uVar2 = (*(code *)PTR_FUN_8c5ffd20)(param_2,param_3);
  return uVar2;
}

