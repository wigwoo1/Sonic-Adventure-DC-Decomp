/* Ghidra draft only; not buildable/verified source. */

void FUN_8c10e304(int param_1,char *param_2,int param_3,byte *param_4)

{
  char cVar1;
  undefined1 uVar2;
  int iVar3;
  undefined1 *extraout_r2;
  char *pcVar4;
  int iVar5;
  int iVar6;
  char *pcVar7;
  char local_2c [16];
  
  *param_2 = '\0';
  iVar3 = param_1;
  if (((param_3 == 100) || (param_3 == 0x69)) && (param_1 < 0)) {
    iVar3 = -param_1;
  }
  iVar6 = 0;
  iVar5 = 0;
  pcVar4 = param_2;
  if ((iVar3 == 0) && (*(int *)(param_4 + 8) != 0)) {
    local_2c[0] = '0';
    iVar6 = 1;
  }
  else {
    while (iVar3 != 0) {
      pcVar7 = local_2c + iVar6;
      uVar2 = (*(code *)PTR_FUN_8c10e498)();
      *extraout_r2 = uVar2;
      if (*pcVar7 < '\n') {
        local_2c[iVar6] = local_2c[iVar6] + '0';
      }
      else {
        pcVar7 = local_2c + iVar6;
        if (param_3 == 0x78) {
          *pcVar7 = *pcVar7 + 'W';
        }
        else {
          *pcVar7 = *pcVar7 + '7';
        }
      }
      iVar3 = (*(code *)PTR_FUN_8c10e49c)(pcVar7);
      iVar6 = iVar6 + 1;
    }
  }
  if ((*param_4 & 4) != 0) {
    if (param_3 == 0x6f) {
      if (param_1 != 0) {
        local_2c[iVar6] = '0';
        iVar6 = iVar6 + 1;
      }
    }
    else if (((param_3 == 0x78) || (param_3 == 0x58)) && (param_1 != 0)) {
      *pcVar4 = '0';
      pcVar4[1] = (char)param_3;
      iVar5 = 2;
      pcVar4 = pcVar4 + 2;
    }
  }
  iVar5 = iVar5 + iVar6;
  if (((param_3 == 100) || (param_3 == 0x69)) && ((*(int *)(param_4 + 8) != 0 || (param_1 != 0)))) {
    if (param_1 < 0) {
      *pcVar4 = '-';
    }
    else {
      if ((*param_4 & 8) == 0) {
        if ((*param_4 & 0x10) != 0) {
          *pcVar4 = ' ';
          iVar5 = iVar5 + 1;
          pcVar4 = pcVar4 + 1;
        }
        goto LAB_8c10e446;
      }
      *pcVar4 = '+';
    }
    iVar5 = iVar5 + 1;
    pcVar4 = pcVar4 + 1;
  }
LAB_8c10e446:
  cVar1 = *param_2;
  if (((cVar1 == '+') || (cVar1 == '-')) || (cVar1 == ' ')) {
    *(char **)(param_4 + 0x14) = param_2 + 1;
  }
  else if ((*param_4 & 4) == 0) {
    *(char **)(param_4 + 0x14) = param_2;
  }
  else if ((param_3 == 0x58) || (param_3 == 0x78)) {
    *(char **)(param_4 + 0x14) = param_2 + 2;
  }
  else if (param_3 == 0x6f) {
    *(char **)(param_4 + 0x14) = param_2 + 1;
  }
  if (iVar5 < *(int *)(param_4 + 8)) {
    *(int *)(param_4 + 0x20) = *(int *)(param_4 + 8) - iVar5;
    iVar5 = *(int *)(param_4 + 8);
  }
  else {
    param_4[0x20] = 0;
    param_4[0x21] = 0;
    param_4[0x22] = 0;
    param_4[0x23] = 0;
  }
  cVar1 = *param_2;
  if (((cVar1 == '+') || (cVar1 == '-')) || (cVar1 == ' ')) {
    if (iVar5 < *(int *)(param_4 + 4)) {
      if (param_4[1] == 0x30) {
        *(int *)(param_4 + 0x20) = *(int *)(param_4 + 0x20) + (*(int *)(param_4 + 4) - iVar5);
        param_4[0x28] = 0;
        param_4[0x29] = 0;
        param_4[0x2a] = 0;
        param_4[0x2b] = 0;
        goto joined_r0x8c10e4e8;
      }
      goto LAB_8c10e4d2;
    }
  }
  else {
LAB_8c10e4d2:
    if (iVar5 < *(int *)(param_4 + 4)) {
      *(int *)(param_4 + 0x28) = *(int *)(param_4 + 4) - iVar5;
      goto joined_r0x8c10e4e8;
    }
  }
  param_4[0x28] = 0;
  param_4[0x29] = 0;
  param_4[0x2a] = 0;
  param_4[0x2b] = 0;
joined_r0x8c10e4e8:
  while (-1 < iVar6 + -1) {
    *pcVar4 = (&stack0xffffffd3)[iVar6];
    pcVar4 = pcVar4 + 1;
    iVar6 = iVar6 + -1;
  }
  *pcVar4 = '\0';
  return;
}

