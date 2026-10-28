/* Ghidra draft only; not buildable/verified source. */

int FUN_8c64a360(int param_1,undefined4 param_2)

{
  int iVar1;
  undefined1 auStack_34 [40];
  int local_c;
  
  if (*(char *)(DAT_8c64a450 + param_1 * DAT_8c64a446) != '\0') {
    iVar1 = (*(code *)PTR_FUN_8c64a454)(param_1);
    if (iVar1 == 0) {
      (*(code *)PTR_FUN_8c64a458)(param_1);
    }
    else {
      iVar1 = (*(code *)PTR_FUN_8c64a45c)(param_1);
      if ((iVar1 != 0) && (*(char *)(DAT_8c64a460 + param_1 * DAT_8c64a446) != '\0')) {
        iVar1 = (*(code *)PTR_thunk_FUN_8c64a4b2_8c64a464)(param_1);
        if (iVar1 < 0) {
          return (int)DAT_8c64a44a;
        }
        iVar1 = FUN_8c64b2dc(param_1,param_2);
        if (iVar1 < 0) {
          return (int)DAT_8c64a44c;
        }
        iVar1 = FUN_8c64ad56(param_1,param_2,auStack_34);
        if (iVar1 < 0) {
          return (int)DAT_8c64a44e;
        }
        return local_c;
      }
    }
  }
  return (int)DAT_8c64a448;
}

