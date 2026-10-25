/* Ghidra draft only; not buildable/verified source. */

int FUN_8c666d10(int param_1,int param_2)

{
  undefined *puVar1;
  int *piVar2;
  undefined *puVar3;
  int iVar4;
  int iVar5;
  byte in_IMASK;
  byte in_S;
  byte in_T;
  int local_24;
  uint local_20;
  int local_1c;
  
  puVar1 = PTR_DAT_8c666e18;
  local_20 = (((uint)in_T | (uint)in_S << 1 | (uint)in_IMASK << 4) & 0xf0) >> 4;
  iVar4 = DAT_8c666e1c;
  local_1c = param_2;
  if (((*(int *)PTR_DAT_8c666e18 != 0) && (iVar4 = DAT_8c666e20, param_1 != 0)) &&
     ((*(code *)PTR_FUN_8c666e24)(param_1,&local_24), iVar5 = DAT_8c666e34,
     puVar3 = PTR_LAB_8c666e30, piVar2 = DAT_8c666e2c, iVar4 = DAT_8c666e28, local_24 != 2)) {
    iVar4 = (int)DAT_8c666e12;
    *DAT_8c666e2c = 0;
    *(undefined4 *)(param_1 + 0x18) = 0;
    iVar4 = (*(code *)PTR_FUN_8c666e38)
                      (*(undefined4 *)(iVar4 + *(int *)puVar1),local_1c + iVar5,
                       *(undefined4 *)(param_1 + 8),*(undefined4 *)(param_1 + 0xc),0,1,1,puVar3,
                       param_1);
    *piVar2 = iVar4;
    if (iVar4 != 0) {
      FUN_8c666e06();
    }
    iVar4 = (*(code *)PTR_FUN_8c666e3c)(param_1,2);
    if (iVar4 == 0) {
      iVar5 = (int)DAT_8c666e14;
      *(int *)(iVar5 + *(int *)puVar1) = param_1;
      *(int *)(iVar5 + -8 + *(int *)puVar1) = *(int *)(iVar5 + -8 + *(int *)puVar1) + 1;
      iVar5 = (*(code *)PTR_FUN_8c666e40)(*(undefined4 *)(iVar5 + 0x1c + *(int *)puVar1),0);
      *piVar2 = iVar5;
      if (iVar5 != 0) {
        FUN_8c666e06();
      }
    }
  }
  if (iVar4 != 0) {
    (*(code *)PTR_FUN_8c666e48)(PTR_FUN_8c666e44,iVar4);
  }
  return iVar4;
}

