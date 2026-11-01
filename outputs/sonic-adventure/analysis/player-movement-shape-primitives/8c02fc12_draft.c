/* Ghidra draft only; not buildable/verified source. */

void FUN_8c02fc12(undefined4 param_1,int param_2,uint *param_3,uint *param_4)

{
  double dVar1;
  double dVar2;
  double dVar3;
  undefined8 in_xd4;
  char in_FPSCR_PR;
  char in_FPSCR_SZ;
  
  if ((*param_3 & 0x20) == 0) {
    if ((*param_3 & 0x2000) == 0) {
      (*(code *)PTR_FUN_8c02fdc4)(PTR_DAT_8c02fdc0,0);
      (*(code *)PTR_FUN_8c02fdc8)(0,param_2 + 0x20);
      if ((*param_3 & 0x40) == 0) {
        if (*(int *)(param_2 + 0x1c) != 0) {
          (*(code *)PTR_FUN_8c02fdd0)(0,*(int *)(param_2 + 0x1c));
        }
        if (*(int *)(param_2 + 0x14) != 0) {
          (*(code *)PTR_FUN_8c02fdd4)(0,*(int *)(param_2 + 0x14));
        }
        if (*(int *)(param_2 + 0x18) != 0) {
          (*(code *)PTR_FUN_8c02fdcc)(0,*(int *)(param_2 + 0x18));
        }
      }
      else {
        if (*(int *)(param_2 + 0x18) != 0) {
          (*(code *)PTR_FUN_8c02fdcc)(0,*(int *)(param_2 + 0x18));
        }
        if (*(int *)(param_2 + 0x1c) != 0) {
          (*(code *)PTR_FUN_8c02fdd0)(0,*(int *)(param_2 + 0x1c));
        }
        if (*(int *)(param_2 + 0x14) != 0) {
          (*(code *)PTR_FUN_8c02fdd4)(0,*(int *)(param_2 + 0x14));
        }
      }
      (*(code *)PTR_FUN_8c02fdd8)(0,param_3 + 2,param_4);
      (*(code *)PTR_FUN_8c02fddc)(1);
      return;
    }
    if (in_FPSCR_SZ == '\0') {
      dVar1 = (double)CONCAT44(*(undefined4 *)(param_2 + 0x28),*(undefined4 *)(param_2 + 0x20));
      dVar2 = (double)CONCAT44(*(undefined4 *)(param_2 + 0x24),param_3[2]);
      dVar3 = (double)CONCAT44(param_3[3],param_1);
      param_3 = param_3 + 4;
    }
    else {
      in_xd4 = *(undefined8 *)(param_2 + 0x20);
      dVar2 = *(double *)(param_2 + 0x28);
      dVar1 = *(double *)(param_2 + 0x30);
      dVar3 = *(double *)(param_3 + 4);
      param_3 = param_3 + 6;
    }
    if (in_FPSCR_PR == '\0') {
      dVar1 = (double)CONCAT44((int)((ulonglong)dVar1 >> 0x20),SUB84(dVar1,0) + SUB84(dVar2,0));
    }
    else {
      dVar1 = dVar1 + dVar2;
    }
    if (in_FPSCR_SZ == '\0') {
      dVar3 = (double)CONCAT44((int)((ulonglong)dVar3 >> 0x20),*param_3);
    }
    if (in_FPSCR_PR == '\0') {
      dVar2 = (double)CONCAT44((float)((ulonglong)dVar2 >> 0x20) + (float)((ulonglong)dVar3 >> 0x20)
                               ,SUB84(dVar2,0));
      dVar1 = (double)CONCAT44((float)((ulonglong)dVar1 >> 0x20) + SUB84(dVar3,0),SUB84(dVar1,0));
    }
    else {
      dVar2 = dVar2 + dVar3;
      dVar1 = dVar1 + dVar3;
    }
    if (in_FPSCR_SZ == '\0') {
      param_4[2] = (uint)((ulonglong)dVar1 >> 0x20);
      param_4[1] = (uint)((ulonglong)dVar2 >> 0x20);
      *param_4 = SUB84(dVar1,0);
    }
    else {
      *(double *)(param_4 + 1) = dVar1;
      *(double *)(param_4 + -1) = dVar2;
      *(undefined8 *)(param_4 + -3) = in_xd4;
    }
  }
  else if (in_FPSCR_SZ == '\0') {
    *param_4 = param_3[2];
    param_4[1] = param_3[3];
    param_4[2] = param_3[4];
  }
  else {
    *(undefined8 *)param_4 = *(undefined8 *)(param_3 + 2);
    *(undefined8 *)(param_4 + 1) = *(undefined8 *)(param_3 + 3);
    *(undefined8 *)(param_4 + 2) = *(undefined8 *)(param_3 + 4);
  }
  return;
}

