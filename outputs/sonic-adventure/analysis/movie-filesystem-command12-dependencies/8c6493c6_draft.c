/* Ghidra draft only; not buildable/verified source. */

int FUN_8c6493c6(int param_1,undefined4 param_2)

{
  undefined *puVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  undefined4 uVar5;
  
  iVar3 = (*(code *)PTR_FUN_8c649470)(*(undefined4 *)(param_1 + 0x28),0);
  if (iVar3 == 0) {
    FUN_8c64850a(*(undefined4 *)(param_1 + 0x28));
  }
  else {
    iVar3 = (*(code *)PTR_FUN_8c649474)(*(undefined4 *)(param_1 + 0x28));
    if ((iVar3 != 0) &&
       (*(char *)(DAT_8c649478 + *(int *)(param_1 + 0x28) * (int)DAT_8c649468) != '\0')) {
      iVar3 = (*(code *)PTR_thunk_FUN_8c64a4b2_8c64947c)(*(undefined4 *)(param_1 + 0x28));
      if (iVar3 < 0) {
        return (int)DAT_8c64946c;
      }
      uVar5 = *(undefined4 *)(param_1 + 0x28);
      iVar3 = (*(code *)PTR_FUN_8c649480)(uVar5);
      puVar1 = PTR_FUN_8c649484;
      if (-1 < iVar3) {
        iVar4 = (*(code *)PTR_FUN_8c649484)(uVar5,iVar3,param_2);
        iVar2 = DAT_8c649488;
        if (iVar4 < 0) {
          if (iVar4 == DAT_8c64946a) {
            return iVar4;
          }
          do {
            (*(code *)PTR_FUN_8c64948c)(uVar5,iVar3,iVar2);
            iVar3 = (*(code *)PTR_FUN_8c649494)(uVar5,DAT_8c649490);
            if (iVar3 < 0) goto LAB_8c649460;
            iVar4 = (*(code *)puVar1)(uVar5,iVar3,param_2);
          } while (iVar4 != 0);
        }
        (*(code *)PTR_FUN_8c649588)(uVar5,iVar3,DAT_8c649584);
        if (*(int *)(param_1 + 0x38) == iVar2) {
          *(int *)(param_1 + 0x38) = (int)(short)iVar3;
        }
        else {
          (*(code *)PTR_FUN_8c649588)(uVar5,*(undefined4 *)(param_1 + 0x3c),iVar3);
        }
        *(int *)(param_1 + 0x3c) = (int)(short)iVar3;
        *(int *)(param_1 + 0x30) = *(int *)(param_1 + 0x30) + 1;
        return 0;
      }
LAB_8c649460:
      return (int)DAT_8c64946e;
    }
  }
  return (int)DAT_8c64946a;
}

