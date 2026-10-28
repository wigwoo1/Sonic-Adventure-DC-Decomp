/* Ghidra draft only; not buildable/verified source. */

void FUN_8c036bc0(ulonglong param_1,uint *param_2)

{
  undefined *puVar1;
  undefined *puVar2;
  undefined *puVar3;
  uint uVar4;
  uint *puVar5;
  undefined8 uVar6;
  uint uVar7;
  undefined8 uVar8;
  char in_FPSCR_SZ;
  code *pcVar9;
  
  puVar3 = PTR_FUN_8c036d48;
  puVar2 = PTR_FUN_8c036d44;
  puVar1 = PTR_FUN_8c036d40;
  pcVar9 = FUN_8c037098;
  do {
    uVar7 = (uint)param_1;
    uVar4 = *param_2 & 7;
    if (uVar4 == 7) {
      if ((*param_2 & 8) == 0) {
        (*(code *)puVar3)(param_2[1],pcVar9);
      }
      if ((*param_2 & 0x10) == 0) {
        FUN_8c036bc0(param_2[0xb]);
      }
    }
    else {
      if (uVar4 == 0) {
        (*(code *)puVar2)(0);
        puVar5 = param_2 + 2;
        if (in_FPSCR_SZ == '\0') {
          uVar8 = CONCAT44(param_2[4],uVar7);
          uVar6 = CONCAT44(*puVar5,param_2[3]);
        }
        else {
          uVar8 = *(undefined8 *)(param_2 + 4);
          uVar6 = *(undefined8 *)puVar5;
        }
        (*(code *)PTR_FUN_8c036d4c)(uVar6,uVar8,0,puVar5);
        uVar7 = (uint)uVar8;
        if ((*param_2 & 0x20) == 0) {
          (*(code *)PTR_FUN_8c036d54)(0,param_2[5],param_2[6],param_2[7]);
        }
        else {
          (*(code *)PTR_FUN_8c036d50)(0,param_2[5],param_2[6],param_2[7]);
        }
        if (in_FPSCR_SZ == '\0') {
          param_1 = (ulonglong)uVar7;
        }
        else {
          param_1 = *(ulonglong *)(param_2 + 10);
        }
        (*(code *)PTR_FUN_8c036d58)(0,param_2 + 8);
        if ((*param_2 & 8) == 0) {
          (*(code *)puVar3)(param_2[1]);
        }
        if ((*param_2 & 0x10) == 0) {
          FUN_8c036bc0(param_2[0xb]);
        }
      }
      else if (uVar4 == 1) {
        (*(code *)puVar2)(0);
        if ((*param_2 & 0x20) == 0) {
          (*(code *)PTR_FUN_8c036d54)(0,param_2[5],param_2[6],param_2[7]);
        }
        else {
          (*(code *)PTR_FUN_8c036d50)(0,param_2[5],param_2[6],param_2[7]);
        }
        if (in_FPSCR_SZ == '\0') {
          param_1 = (ulonglong)uVar7;
        }
        else {
          param_1 = *(ulonglong *)(param_2 + 10);
        }
        (*(code *)PTR_FUN_8c036d58)(0,param_2 + 8);
        if ((*param_2 & 8) == 0) {
          (*(code *)puVar3)(param_2[1]);
        }
        if ((*param_2 & 0x10) == 0) {
          FUN_8c036bc0(param_2[0xb]);
        }
      }
      else if (uVar4 == 2) {
        (*(code *)puVar2)(0);
        if (in_FPSCR_SZ != '\0') {
          uVar7 = (uint)*(undefined8 *)(param_2 + 4);
        }
        (*(code *)PTR_FUN_8c036ea0)(0,param_2 + 2);
        if (in_FPSCR_SZ == '\0') {
          param_1 = (ulonglong)uVar7;
        }
        else {
          param_1 = *(ulonglong *)(param_2 + 10);
        }
        (*(code *)PTR_FUN_8c036ea4)(0,param_2 + 8);
        if ((*param_2 & 8) == 0) {
          (*(code *)puVar3)(param_2[1],pcVar9);
        }
        if ((*param_2 & 0x10) == 0) {
          FUN_8c036bc0(param_2[0xb]);
        }
      }
      else if (uVar4 == 3) {
        (*(code *)puVar2)(0);
        if (in_FPSCR_SZ == '\0') {
          param_1 = (ulonglong)uVar7;
        }
        else {
          param_1 = *(ulonglong *)(param_2 + 10);
        }
        (*(code *)PTR_FUN_8c036ea4)(0,param_2 + 8);
        if ((*param_2 & 8) == 0) {
          (*(code *)puVar3)(param_2[1]);
        }
        if ((*param_2 & 0x10) == 0) {
          FUN_8c036bc0(param_2[0xb]);
        }
      }
      else if (uVar4 == 4) {
        (*(code *)puVar2)(0);
        if (in_FPSCR_SZ == '\0') {
          param_1 = (ulonglong)uVar7;
        }
        else {
          param_1 = *(ulonglong *)(param_2 + 4);
        }
        (*(code *)PTR_FUN_8c036ea0)(0,param_2 + 2);
        if ((*param_2 & 0x20) == 0) {
          (*(code *)PTR_FUN_8c036eac)(0,param_2[5],param_2[6],param_2[7]);
        }
        else {
          (*(code *)PTR_FUN_8c036ea8)(0,param_2[5],param_2[6],param_2[7]);
        }
        if ((*param_2 & 8) == 0) {
          (*(code *)puVar3)(param_2[1]);
        }
        if ((*param_2 & 0x10) == 0) {
          FUN_8c036bc0(param_2[0xb]);
        }
      }
      else if (uVar4 == 5) {
        (*(code *)puVar2)(0);
        if ((*param_2 & 0x20) == 0) {
          (*(code *)PTR_FUN_8c036eac)(0,param_2[5],param_2[6],param_2[7]);
        }
        else {
          (*(code *)PTR_FUN_8c036ea8)(0,param_2[5],param_2[6],param_2[7]);
        }
        if ((*param_2 & 8) == 0) {
          (*(code *)puVar3)(param_2[1]);
        }
        if ((*param_2 & 0x10) == 0) {
          FUN_8c036bc0(param_2[0xb]);
        }
      }
      else {
        if (uVar4 != 6) goto LAB_8c036ef4;
        (*(code *)puVar2)(0);
        if (in_FPSCR_SZ == '\0') {
          param_1 = (ulonglong)uVar7;
        }
        else {
          param_1 = *(ulonglong *)(param_2 + 4);
        }
        (*(code *)PTR_FUN_8c036f40)(0,param_2 + 2);
        if ((*param_2 & 8) == 0) {
          (*(code *)puVar3)(param_2[1]);
        }
        if ((*param_2 & 0x10) == 0) {
          FUN_8c036bc0(param_2[0xb]);
        }
      }
      (*(code *)puVar1)(1);
    }
LAB_8c036ef4:
    param_2 = (uint *)param_2[0xc];
    if (param_2 == (uint *)0x0) {
      return;
    }
  } while( true );
}

