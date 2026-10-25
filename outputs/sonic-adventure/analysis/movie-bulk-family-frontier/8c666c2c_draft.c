/* Ghidra draft only; not buildable/verified source. */

int FUN_8c666c2c(int param_1,undefined4 param_2)

{
  undefined *puVar1;
  undefined *puVar2;
  int iVar3;
  int iVar4;
  byte in_IMASK;
  byte in_S;
  byte in_T;
  int local_20;
  undefined4 local_1c;
  uint local_18;
  
  puVar1 = PTR_DAT_8c666ce8;
  local_18 = (((uint)in_T | (uint)in_S << 1 | (uint)in_IMASK << 4) & 0xf0) >> 4;
  iVar3 = DAT_8c666cec;
  local_1c = param_2;
  if (((*(int *)PTR_DAT_8c666ce8 != 0) && (iVar3 = DAT_8c666cf0, param_1 != 0)) &&
     ((*(code *)PTR_FUN_8c666cf4)(param_1,&local_20), puVar2 = PTR_FUN_8c666cfc,
     iVar3 = DAT_8c666cf8, local_20 != 2)) {
    *(undefined4 *)(param_1 + 0x18) = 0;
    iVar3 = (*(code *)puVar2)(param_1,2);
    puVar2 = PTR_LAB_8c666d00;
    if (iVar3 == 0) {
      iVar3 = (int)DAT_8c666ce4;
      *(int *)(iVar3 + *(int *)puVar1) = param_1;
      iVar3 = iVar3 + -8;
      iVar4 = *(int *)puVar1;
      *(int *)(iVar3 + iVar4) = *(int *)(iVar3 + iVar4) + 1;
      iVar3 = (*(code *)PTR_FUN_8c666d04)
                        (*(undefined4 *)(param_1 + 8),local_1c,*(undefined4 *)(param_1 + 0xc),puVar2
                         ,param_1);
    }
  }
  if (iVar3 != 0) {
    (*(code *)PTR_FUN_8c666d0c)(PTR_FUN_8c666d08,iVar3);
  }
  return iVar3;
}

