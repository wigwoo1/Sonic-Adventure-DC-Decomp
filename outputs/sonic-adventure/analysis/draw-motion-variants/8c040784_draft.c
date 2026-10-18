/* Ghidra draft only; not buildable/verified source. */

void FUN_8c040784(ulonglong param_1,uint *param_2)

{
  undefined *puVar1;
  undefined *puVar2;
  undefined4 unaff_r8;
  undefined4 unaff_r9;
  code *pcVar3;
  uint uVar4;
  undefined8 uVar5;
  uint uVar6;
  undefined8 uVar7;
  undefined4 in_PR;
  char in_FPSCR_SZ;
  code *pcVar8;
  
  puVar2 = PTR_FUN_8c040860;
  puVar1 = PTR_FUN_8c04085c;
  pcVar8 = FUN_8c608ccc;
  do {
    uVar4 = *param_2;
    (*(code *)puVar2)(0,pcVar8);
    if ((uVar4 & 0x40) == 0) {
      (*(code *)*DAT_8c040864)(param_2);
    }
    else {
      if (in_FPSCR_SZ == '\0') {
        uVar5 = CONCAT44(param_2[2],param_2[3]);
        uVar7 = CONCAT44(param_2[4],(int)param_1);
      }
      else {
        uVar5 = *(undefined8 *)(param_2 + 2);
        uVar7 = *(undefined8 *)(param_2 + 6);
      }
      (*(code *)PTR_FUN_8c040868)(uVar5,uVar7,0);
      uVar6 = (uint)uVar7;
      pcVar3 = (code *)puVar1;
      if ((uVar4 & 0x20) != 0) {
        pcVar3 = (code *)PTR_FUN_8c04086c;
      }
      (*pcVar3)(0,in_PR,unaff_r8,unaff_r9);
      if (in_FPSCR_SZ == '\0') {
        param_1 = (ulonglong)uVar6;
      }
      else {
        param_1 = *(ulonglong *)(param_2 + 0xc);
      }
      (*(code *)PTR_FUN_8c040870)(0);
    }
    if ((code *)*DAT_8c040874 != (code *)0x0) {
      (*(code *)*DAT_8c040874)(param_2);
    }
    if ((param_2[1] != 0) && ((uVar4 & 8) == 0)) {
      (*(code *)PTR_FUN_8c040854)(param_2[1]);
    }
    if (param_2[0xb] != 0) {
      FUN_8c040784(param_2[0xb]);
    }
    (*(code *)PTR_FUN_8c040878)(1);
    param_2 = (uint *)param_2[0xc];
  } while (param_2 != (uint *)0x0);
  return;
}

