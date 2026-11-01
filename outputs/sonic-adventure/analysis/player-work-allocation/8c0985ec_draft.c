/* Ghidra draft only; not buildable/verified source. */

int * FUN_8c0985ec(int param_1,int param_2)

{
  int *piVar1;
  int iVar2;
  int *piVar3;
  
  piVar3 = (int *)*DAT_8c0986c0;
  if (piVar3 == (int *)0x0) {
    return (int *)0x0;
  }
  iVar2 = *piVar3;
  if (iVar2 != 0) {
    *(undefined4 *)(iVar2 + 4) = 0;
  }
  *DAT_8c0986c0 = iVar2;
  if (param_2 == 0) {
    piVar3[4] = (int)PTR_LAB_8c0986c4;
  }
  else {
    piVar3[4] = param_2;
  }
  if (param_1 != 7) {
    if (param_1 == 8) {
      param_1 = 7;
    }
    piVar1 = (int *)(param_1 * 4 + DAT_8c0986c8);
    if (*piVar1 == 0) {
      *piVar1 = (int)piVar3;
      *piVar3 = 0;
      piVar3[1] = 0;
    }
    else {
      iVar2 = *piVar1;
      piVar3[1] = 0;
      *piVar3 = iVar2;
      *(int **)(iVar2 + 4) = piVar3;
      *piVar1 = (int)piVar3;
    }
  }
  piVar3[7] = 0;
  piVar3[8] = 0;
  piVar3[9] = 0;
  piVar3[10] = 0;
  piVar3[0xb] = 0;
  piVar3[5] = 0;
  piVar3[6] = 0;
  piVar3[3] = 0;
  piVar3[2] = 0;
  return piVar3;
}

