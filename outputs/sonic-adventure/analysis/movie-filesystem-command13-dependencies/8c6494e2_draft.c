/* Ghidra draft only; not buildable/verified source. */

int FUN_8c6494e2(int param_1,undefined4 param_2)

{
  int iVar1;
  uint uVar2;
  int iVar3;
  undefined4 uVar4;
  undefined4 uVar5;
  
  iVar3 = *(int *)(param_1 + 0x28) * (int)DAT_8c64957c + DAT_8c64958c;
  iVar1 = (*(code *)PTR_FUN_8c649590)(1);
  if (iVar1 == 0) {
    (*(code *)PTR_FUN_8c649594)(*(undefined4 *)(param_1 + 0x28));
  }
  else {
    iVar1 = (*(code *)PTR_FUN_8c649598)(*(undefined4 *)(param_1 + 0x28));
    if ((iVar1 != 0) &&
       (*(char *)(DAT_8c64959c + *(int *)(param_1 + 0x28) * (int)DAT_8c64957c) != '\0')) {
      iVar1 = (*(code *)PTR_thunk_FUN_8c64a4b2_8c6495a0)(*(undefined4 *)(param_1 + 0x28));
      if (iVar1 < 0) {
        return (int)DAT_8c649580;
      }
      uVar5 = *(undefined4 *)(param_1 + 0x30);
      uVar4 = *(undefined4 *)(param_1 + 0x28);
      uVar2 = (*(code *)PTR_FUN_8c6495a4)(uVar4,uVar5);
      if (((uVar2 & 0xffff) == DAT_8c6495a8) &&
         (*(int *)(param_1 + 0x30) < *(int *)(*(int *)(iVar3 + 0x1c) + 0x38))) {
        iVar1 = (*(code *)PTR_FUN_8c6495ac)(uVar4,uVar5,param_2);
        if (iVar1 < 0) {
          return iVar1;
        }
        (*(code *)PTR_FUN_8c6496ac)(uVar4,uVar5,DAT_8c6496a8);
        if (*(int *)(param_1 + 0x38) == DAT_8c6496b0) {
          *(int *)(param_1 + 0x38) = (int)(short)uVar5;
        }
        else {
          (*(code *)PTR_FUN_8c6496ac)(uVar4,*(undefined4 *)(param_1 + 0x3c),uVar5);
        }
        *(int *)(param_1 + 0x3c) = (int)(short)uVar5;
        *(int *)(param_1 + 0x30) = *(int *)(param_1 + 0x30) + 1;
        return 0;
      }
      return (int)DAT_8c649582;
    }
  }
  return (int)DAT_8c64957e;
}

