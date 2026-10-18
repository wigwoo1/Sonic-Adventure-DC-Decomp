/* Ghidra draft only; not buildable/verified source. */

void FUN_8c041a2e(ulonglong param_1,uint *param_2)

{
  undefined4 *puVar1;
  undefined2 *puVar2;
  undefined4 *puVar3;
  code *pcVar4;
  undefined4 uVar5;
  uint uVar6;
  undefined8 uVar7;
  uint uVar8;
  undefined8 uVar9;
  char in_FPSCR_SZ;
  undefined4 uVar10;
  
  puVar2 = DAT_8c041bb4;
  puVar1 = DAT_8c041bb0;
  uVar10 = 1;
  puVar3 = DAT_8c041bb0 + 1;
  do {
    uVar6 = *param_2;
    (*(code *)PTR_FUN_8c041bb8)(0,uVar10);
    if ((uVar6 & 0x40) == 0) {
      *DAT_8c041bbc = 0;
      *DAT_8c041bc0 = 0;
      *DAT_8c041bc4 = 0;
      *puVar2 = 0;
      (*(code *)*puVar1)(param_2);
      *puVar2 = 1;
      (*(code *)*puVar3)(param_2);
      FUN_8c0417c8();
    }
    else {
      uVar5 = *DAT_8c041bc8;
      if (in_FPSCR_SZ == '\0') {
        uVar7 = CONCAT44(param_2[2],param_2[3]);
        uVar9 = CONCAT44(param_2[4],(int)param_1);
      }
      else {
        uVar7 = *(undefined8 *)(param_2 + 2);
        uVar9 = *(undefined8 *)(param_2 + 6);
      }
      (*(code *)PTR_FUN_8c041b88)(uVar7,uVar9,uVar5);
      uVar8 = (uint)uVar9;
      pcVar4 = (code *)PTR_FUN_8c041bcc;
      if ((uVar6 & 0x20) != 0) {
        pcVar4 = (code *)PTR_FUN_8c041bd0;
      }
      (*pcVar4)(uVar5,param_2[5],param_2[6],param_2[7]);
      if (in_FPSCR_SZ == '\0') {
        param_1 = (ulonglong)uVar8;
      }
      else {
        param_1 = *(ulonglong *)(param_2 + 0xc);
      }
      (*(code *)PTR_FUN_8c041b90)(uVar5);
    }
    if ((code *)*DAT_8c041bd4 != (code *)0x0) {
      (*(code *)*DAT_8c041bd4)(param_2);
    }
    if ((param_2[1] != 0) && ((uVar6 & 8) == 0)) {
      (*(code *)PTR_FUN_8c041bac)(param_2[1]);
    }
    if (param_2[0xb] != 0) {
      FUN_8c041a2e(param_2[0xb]);
    }
    (*(code *)PTR_FUN_8c041bd8)(1);
    param_2 = (uint *)param_2[0xc];
  } while (param_2 != (uint *)0x0);
  return;
}

