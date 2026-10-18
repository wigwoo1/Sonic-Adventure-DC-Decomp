/* Ghidra draft only; not buildable/verified source. */

void FUN_8c09865c(int *param_1)

{
  int *piVar1;
  int *piVar2;
  int iVar3;
  
  if (param_1[6] != 0) {
    (*(code *)param_1[6])(param_1,&DAT_8c4bfa6c);
  }
  param_1[4] = 0;
  param_1[5] = 0;
  param_1[6] = 0;
  iVar3 = *param_1;
  piVar2 = (int *)param_1[1];
  if (param_1[3] != 0) {
    FUN_8c098e60(param_1);
  }
  if ((iVar3 == 0) || (piVar2 == (int *)0x0)) {
    if ((iVar3 == 0) && (piVar2 != (int *)0x0)) {
      *piVar2 = 0;
    }
    else if (param_1[2] == 0) {
      if (iVar3 != 0) {
        *(undefined4 *)(iVar3 + 4) = 0;
      }
      piVar1 = DAT_8c0987e0;
      for (piVar2 = DAT_8c0987e0; piVar2 < DAT_8c0987e0 + 8; piVar2 = piVar2 + 1) {
        if (param_1 == (int *)*piVar2) {
          *piVar1 = iVar3;
          break;
        }
        piVar1 = piVar1 + 1;
      }
    }
    else {
      if (iVar3 != 0) {
        *(undefined4 *)(iVar3 + 4) = 0;
      }
      *(int *)(param_1[2] + 0xc) = iVar3;
    }
  }
  else {
    *(int **)(iVar3 + 4) = piVar2;
    *piVar2 = iVar3;
  }
  if (param_1[8] != 0) {
    FUN_8c098998(param_1);
  }
  if (param_1[10] != 0) {
    FUN_8c098b42(param_1[10]);
    param_1[10] = 0;
  }
  if (param_1[9] != 0) {
    FUN_8c098b42(param_1[9]);
    param_1[9] = 0;
  }
  if (param_1[0xb] != 0) {
    FUN_8c098b42(param_1[0xb]);
    param_1[0xb] = 0;
  }
  if (param_1[7] != 0) {
    *(ushort *)(param_1[7] + 2) = *(ushort *)(param_1[7] + 2) & 0xfffe;
    param_1[7] = 0;
  }
  piVar2 = DAT_8c0987e4;
  iVar3 = *DAT_8c0987e4;
  if (iVar3 == 0) {
    *DAT_8c0987e4 = (int)param_1;
    param_1[1] = 0;
    *param_1 = 0;
  }
  else {
    *(int **)(iVar3 + 4) = param_1;
    *param_1 = iVar3;
    param_1[1] = 0;
    *piVar2 = (int)param_1;
  }
  param_1[3] = 0;
  param_1[2] = 0;
  return;
}

