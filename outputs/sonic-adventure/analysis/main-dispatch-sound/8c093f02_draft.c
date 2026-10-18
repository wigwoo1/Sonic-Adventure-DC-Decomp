/* Ghidra draft only; not buildable/verified source. */

int FUN_8c093f02(int param_1,int param_2)

{
  int *piVar1;
  int *piVar2;
  int iVar3;
  int iVar4;
  int iVar5;
  
  piVar2 = DAT_8c094220;
  piVar1 = DAT_8c09407c;
  iVar5 = -1;
  if (((((((((param_2 == 0) || (param_2 == 0x42)) || (param_2 == 0x43)) ||
          ((param_2 == 0x7c || (param_2 == 0xa1)))) || (param_2 == 0xa0)) ||
        (((param_2 == 0xef || (param_2 == 0x229)) ||
         ((param_2 == 0x22a || (((param_2 == 0x423 || (param_2 == 0x40f)) || (param_2 == 0x438))))))
        )) || ((param_2 == 0x29b || (param_2 == 0x403)))) ||
      ((param_2 == 0x405 ||
       (((0x308 < param_2 && (param_2 < 0x30e)) || ((0x4be < param_2 && (param_2 < 0x4c7)))))))) ||
     (((0x4ef < param_2 && (param_2 < 0x4f8)) || (param_2 == 0x4f9)))) {
    iVar4 = *DAT_8c09407c;
    iVar3 = 0x25;
    do {
      if (*(int *)(iVar4 * 0x34 + DAT_8c094078 + 4) == 0) {
        iVar5 = iVar4;
      }
      if (*(int *)(iVar4 * 0x34 + DAT_8c094078) <= param_1) {
        iVar5 = iVar4;
      }
      iVar4 = iVar4 + 1;
      iVar3 = iVar3 + -1;
      if (0x24 < iVar4) {
        iVar4 = 0x20;
      }
    } while ((iVar5 < 0) && (0x20 < iVar3));
    *DAT_8c09407c = iVar5 + 1;
    if (0x24 < iVar5 + 1) {
      *piVar1 = 0x20;
    }
  }
  else {
    iVar3 = 0x20;
    iVar4 = *DAT_8c094220;
    do {
      if (*(int *)(iVar4 * 0x34 + DAT_8c094078 + 4) == 0) {
        iVar5 = iVar4;
      }
      if (*(int *)(iVar4 * 0x34 + DAT_8c094078) <= param_1) {
        iVar5 = iVar4;
      }
      iVar4 = iVar4 + 1;
      iVar3 = iVar3 + -1;
      if (0x1f < iVar4) {
        iVar4 = 0;
      }
    } while ((iVar5 < 0) && (0 < iVar3));
    *DAT_8c094220 = iVar5 + 1;
    if (0x1f < iVar5 + 1) {
      *piVar2 = 0;
    }
  }
  return iVar5;
}

