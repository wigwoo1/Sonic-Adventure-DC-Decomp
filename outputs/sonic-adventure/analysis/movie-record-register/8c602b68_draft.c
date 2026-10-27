/* Ghidra draft only; not buildable/verified source. */

int FUN_8c602b68(undefined4 param_1,undefined4 param_2,undefined4 param_3,code *param_4)

{
  undefined4 *puVar1;
  undefined4 *puVar2;
  undefined4 *puVar3;
  undefined *puVar4;
  int iVar5;
  byte in_IMASK;
  byte in_S;
  byte in_T;
  
  puVar1 = DAT_8c602c84;
  *DAT_8c602c80 = param_3;
  puVar2 = DAT_8c602c88;
  *puVar1 = 0;
  puVar3 = DAT_8c602c90;
  puVar1 = DAT_8c602c8c;
  *puVar2 = 0;
  puVar4 = PTR_FUN_8c602c94;
  *puVar1 = 0;
  *puVar3 = 0;
  (*(code *)puVar4)(param_2,(((uint)in_T | (uint)in_S << 1 | (uint)in_IMASK << 4) & 0xf0) >> 4);
  iVar5 = (*(code *)PTR_FUN_8c602c98)(param_1,param_2,param_3);
  (*(code *)PTR_FUN_8c602ca4)(*(undefined4 *)PTR_DAT_8c602ca0,PTR_FUN_8c602c9c);
  if ((iVar5 == 0) && (param_4 != (code *)0x0)) {
    (*param_4)();
  }
  *(undefined4 *)PTR_DAT_8c602ca8 = 1;
  return iVar5;
}

