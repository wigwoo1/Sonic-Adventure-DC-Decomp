/* Ghidra draft only; not buildable/verified source. */

void FUN_8c041c92(float param_1,undefined4 param_2,undefined4 *param_3)

{
  int iVar1;
  undefined4 uVar2;
  undefined4 uVar3;
  undefined4 in_fr2;
  bool bVar4;
  char in_FPSCR_PR;
  char in_FPSCR_SZ;
  
  if (in_FPSCR_PR == '\0') {
    bVar4 = 1.0 < param_1;
  }
  else {
    bVar4 = (double)CONCAT44(in_fr2,0x3f800000) < (double)CONCAT44(param_1,param_2);
  }
  if (!bVar4) {
    uVar2 = param_3[1];
    uVar3 = *param_3;
    *DAT_8c041d30 = 0;
    if (in_FPSCR_SZ == '\0') {
      *DAT_8c041d34 = param_1;
    }
    else {
      *(ulonglong *)DAT_8c041d34 = CONCAT44(param_1,param_2);
    }
    iVar1 = (*(code *)PTR_FUN_8c041d38)(uVar2,0);
    if ((iVar1 == 0) && (iVar1 = (*(code *)PTR_FUN_8c041d38)(uVar2,1), iVar1 == 0)) {
      (*(code *)PTR_FUN_8c041d3c)(uVar3,0);
      (*(code *)PTR_FUN_8c041d3c)(uVar3,1);
      (*(code *)PTR_FUN_8c041d40)();
      FUN_8c041a2e(uVar3);
      (*(code *)PTR_FUN_8c041d44)();
      return;
    }
  }
  return;
}

