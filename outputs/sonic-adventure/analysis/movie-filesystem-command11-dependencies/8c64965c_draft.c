/* Ghidra draft only; not buildable/verified source. */

int FUN_8c64965c(int param_1,undefined4 param_2)

{
  int iVar1;
  uint uVar2;
  undefined4 uVar3;
  undefined4 uVar4;
  
  iVar1 = (*(code *)PTR_FUN_8c6496b8)(*(undefined4 *)(param_1 + 0x28),1);
  if (iVar1 == 0) {
    (*(code *)PTR_FUN_8c6496bc)(*(undefined4 *)(param_1 + 0x28));
  }
  else {
    iVar1 = (*(code *)PTR_FUN_8c6496c0)(*(undefined4 *)(param_1 + 0x28));
    if ((iVar1 != 0) &&
       (*(char *)(DAT_8c6496c4 + *(int *)(param_1 + 0x28) * (int)DAT_8c6496a0) != '\0')) {
      iVar1 = (*(code *)PTR_thunk_FUN_8c64a4b2_8c6497a0)(*(undefined4 *)(param_1 + 0x28));
      if (iVar1 < 0) {
        return (int)DAT_8c649798;
      }
      uVar4 = *(undefined4 *)(param_1 + 0x28);
      if (*(int *)(param_1 + 0x40) == DAT_8c6497a4) {
        if (*(int *)(param_1 + 0x34) != 0) {
          return DAT_8c6497a8;
        }
      }
      else if (*(int *)(param_1 + 0x34) != 0) {
        if ((*(int *)(param_1 + 0x40) != DAT_8c6497ac) && (*(int *)(param_1 + 0x40) != DAT_8c6497b0)
           ) {
          uVar3 = *(undefined4 *)(param_1 + 0x40);
          iVar1 = (*(code *)PTR_FUN_8c6497b4)(uVar4,uVar3,param_2);
          if (iVar1 < 0) {
            return iVar1;
          }
          uVar2 = (*(code *)PTR_FUN_8c6497b8)(uVar4,uVar3);
          *(uint *)(param_1 + 0x40) = uVar2 & 0xffff;
          iVar1 = *(int *)(param_1 + 0x34) + -1;
          *(int *)(param_1 + 0x34) = iVar1;
          return iVar1;
        }
        return (int)DAT_8c64979a;
      }
      return 0;
    }
  }
  return (int)DAT_8c6496a2;
}

