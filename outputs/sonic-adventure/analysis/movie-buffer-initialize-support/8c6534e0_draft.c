/* Ghidra draft only; not buildable/verified source. */

undefined4 FUN_8c6534e0(uint param_1,uint param_2,uint param_3)

{
  int iVar1;
  int iVar2;
  int iVar3;
  uint uVar4;
  byte in_IMASK;
  char in_S;
  byte in_T;
  undefined4 *local_24 [2];
  
  if ((((param_1 & 0x1f) == 0) && ((param_2 & 0x1f) == 0)) &&
     (((in_T | in_S << 1) & 0xf0) == 0 && (in_IMASK & 0xf) == 0)) {
    FUN_8c65344e(param_2,param_3);
    uVar4 = param_3 & 0xffffffe0;
    if (uVar4 == 0) {
      *DAT_8c65358c = 0;
    }
    else {
      iVar3 = (int)DAT_8c653580;
      iVar2 = DAT_8c653584 + param_1;
      do {
        iVar1 = (*(code *)PTR_FUN_8c653588)(iVar2,param_2,uVar4,0x18,local_24);
      } while (iVar1 == iVar3);
      *DAT_8c65358c = *local_24[0];
    }
    if ((param_3 & 0x1f) != 0) {
      (*(code *)PTR_FUN_8c653594)(DAT_8c653590 + param_1 + uVar4,param_2 + uVar4);
    }
    if (*DAT_8c653598 == 1) {
      (*(code *)PTR_FUN_8c65359c)(*DAT_8c65358c);
    }
  }
  else {
    (*(code *)PTR_FUN_8c653624)(DAT_8c653620 + param_1,param_2,param_3);
  }
  return 1;
}

