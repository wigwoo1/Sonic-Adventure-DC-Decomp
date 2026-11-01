/* Ghidra draft only; not buildable/verified source. */

void FUN_8c02fe5c(int param_1,uint *param_2,uint *param_3)

{
  char in_FPSCR_SZ;
  
  if ((*param_2 & 0x20) == 0) {
    (*(code *)PTR_FUN_8c02ff58)(PTR_DAT_8c02ff54,&DAT_8c752bdc);
    (*(code *)PTR_FUN_8c02ff5c)(0,param_1 + 0x20);
    if ((*param_2 & 0x2000) == 0) {
      if ((*param_2 & 0x40) == 0) {
        if (*(int *)(param_1 + 0x1c) != 0) {
          (*(code *)PTR_FUN_8c02ff40)(0,*(int *)(param_1 + 0x1c));
        }
        if (*(int *)(param_1 + 0x14) != 0) {
          (*(code *)PTR_FUN_8c02ff44)(0,*(int *)(param_1 + 0x14));
        }
        if (*(int *)(param_1 + 0x18) != 0) {
          (*(code *)PTR_FUN_8c02ff48)(0,*(int *)(param_1 + 0x18));
        }
      }
      else {
        if (*(int *)(param_1 + 0x18) != 0) {
          (*(code *)PTR_FUN_8c02ff48)(0,*(int *)(param_1 + 0x18));
        }
        if (*(int *)(param_1 + 0x1c) != 0) {
          (*(code *)PTR_FUN_8c02ff40)(0,*(int *)(param_1 + 0x1c));
        }
        if (*(int *)(param_1 + 0x14) != 0) {
          (*(code *)PTR_FUN_8c02ff44)(0,*(int *)(param_1 + 0x14));
        }
      }
    }
    (*(code *)PTR_FUN_8c02ff4c)(0,param_2 + 2,param_3);
    (*(code *)PTR_FUN_8c02ff50)(1);
    return;
  }
  if (in_FPSCR_SZ == '\0') {
    *param_3 = param_2[2];
    param_3[1] = param_2[3];
    param_3[2] = param_2[4];
  }
  else {
    *(undefined8 *)param_3 = *(undefined8 *)(param_2 + 2);
    *(undefined8 *)(param_3 + 1) = *(undefined8 *)(param_2 + 3);
    *(undefined8 *)(param_3 + 2) = *(undefined8 *)(param_2 + 4);
  }
  return;
}

