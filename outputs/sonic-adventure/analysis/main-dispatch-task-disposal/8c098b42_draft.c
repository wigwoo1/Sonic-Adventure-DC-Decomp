/* Ghidra draft only; not buildable/verified source. */

void FUN_8c098b42(int param_1)

{
  int *piVar1;
  int *piVar2;
  int *piVar3;
  int iVar4;
  int *piVar5;
  int iVar6;
  
  piVar1 = DAT_8c098c80;
  if (param_1 == 0) {
    return;
  }
  piVar3 = (int *)(param_1 + -0x10);
  if ((*piVar3 != 0) && (*(int *)(param_1 + -0xc) != 0)) {
    do {
                    /* WARNING: Do nothing block with infinite loop */
    } while( true );
  }
  piVar5 = *(int **)(param_1 + -4);
  piVar2 = *(int **)(param_1 + -8);
  if (piVar5 != (int *)0x0) {
    if ((((*piVar5 != 0) || (piVar5[1] != 0)) && ((*piVar2 != 0 || (piVar2[1] != 0)))) &&
       (piVar2[2] != 0)) {
      piVar2[3] = (int)piVar5;
      piVar5[2] = (int)piVar2;
      iVar4 = *piVar2;
      piVar3 = (int *)piVar2[1];
      if (iVar4 != 0) {
        *(int **)(iVar4 + 4) = piVar3;
      }
      if (piVar3 == (int *)0x0) {
        *piVar1 = iVar4;
      }
      else {
        *piVar3 = iVar4;
      }
      *(int **)(piVar2[2] + 0xc) = piVar5;
      piVar5[2] = piVar2[2];
      return;
    }
    if ((piVar5 != (int *)0x0) && ((*piVar5 != 0 || (piVar5[1] != 0)))) {
      piVar2[3] = (int)piVar5;
      piVar5[2] = (int)piVar2;
      return;
    }
  }
  if (((*piVar2 == 0) && (piVar2[1] == 0)) || (piVar2[2] == 0)) {
    iVar4 = *DAT_8c098c80;
    *DAT_8c098c80 = (int)piVar3;
    if (iVar4 != 0) {
      *(int **)(iVar4 + 4) = piVar3;
    }
    *piVar3 = iVar4;
    *(undefined4 *)(param_1 + -0xc) = 0;
  }
  else {
    iVar6 = *piVar2;
    iVar4 = piVar2[1];
    *piVar3 = iVar6;
    *(int *)(param_1 + -0xc) = iVar4;
    if (iVar6 != 0) {
      *(int **)(*piVar2 + 4) = piVar3;
    }
    if (iVar4 == 0) {
      *piVar1 = (int)piVar3;
    }
    else {
      *(int **)piVar2[1] = piVar3;
    }
    *(int *)(param_1 + -8) = piVar2[2];
    *(int **)(piVar2[2] + 0xc) = piVar3;
  }
  return;
}

