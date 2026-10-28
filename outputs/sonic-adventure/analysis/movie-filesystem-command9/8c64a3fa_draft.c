/* Ghidra draft only; not buildable/verified source. */

int FUN_8c64a3fa(int param_1,int param_2)

{
  int iVar1;
  undefined1 auStack_38 [16];
  undefined1 local_28 [32];
  
  if (*(char *)(DAT_8c64a450 + param_1 * DAT_8c64a446) != '\0') {
    iVar1 = (*(code *)PTR_FUN_8c64a454)(param_1);
    if (iVar1 == 0) {
      (*(code *)PTR_FUN_8c64a458)(param_1);
    }
    else {
      iVar1 = (*(code *)PTR_FUN_8c64a45c)(param_1);
      if ((iVar1 != 0) && (*(char *)(DAT_8c64a460 + param_1 * DAT_8c64a446) != '\0')) {
        iVar1 = (*(code *)PTR_thunk_FUN_8c64a4b2_8c64a544)(param_1);
        if (iVar1 < 0) {
          return (int)DAT_8c64a53a;
        }
        iVar1 = FUN_8c64adc6(param_1,auStack_38);
        if (iVar1 < 0) {
          return (int)DAT_8c64a53c;
        }
        for (iVar1 = 0; iVar1 < 0xc; iVar1 = iVar1 + 1) {
          *(undefined1 *)(iVar1 + param_2) = local_28[iVar1];
        }
        *(undefined1 *)(param_2 + 0xc) = 0;
        return 0;
      }
    }
  }
  return (int)DAT_8c64a448;
}

