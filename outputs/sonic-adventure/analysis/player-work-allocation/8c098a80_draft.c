/* Ghidra draft only; not buildable/verified source. */

int * FUN_8c098a80(int param_1)

{
  int *piVar1;
  int iVar2;
  int *piVar3;
  undefined4 *puVar4;
  uint uVar5;
  int *piVar6;
  
  piVar1 = DAT_8c098b60;
  if ((((param_1 != 0) && (piVar6 = (int *)*DAT_8c098b60, piVar6 != (int *)0x0)) && (piVar6[2] != 0)
      ) && (uVar5 = param_1 + 0x1fU & 0xfffffff0, piVar6 != (int *)0x0)) {
    iVar2 = piVar6[2];
    while (iVar2 != 0) {
      if (piVar6[2] - (int)piVar6 == uVar5) {
        iVar2 = *piVar6;
        piVar3 = (int *)piVar6[1];
        if (iVar2 != 0) {
          *(int **)(iVar2 + 4) = piVar3;
        }
        if (piVar3 == (int *)0x0) {
          *piVar1 = iVar2;
        }
        else {
          *piVar3 = iVar2;
        }
LAB_8c098af0:
        *piVar6 = 0;
        piVar6[1] = 0;
        return piVar6 + 4;
      }
      if (uVar5 < (uint)(piVar6[2] - (int)piVar6)) {
        puVar4 = (undefined4 *)piVar6[1];
        piVar3 = (int *)(uVar5 + (int)piVar6);
        iVar2 = *piVar6;
        *piVar3 = iVar2;
        piVar3[1] = (int)puVar4;
        piVar3[2] = piVar6[2];
        piVar3[3] = (int)piVar6;
        *(int **)(piVar6[2] + 0xc) = piVar3;
        piVar6[2] = (int)piVar3;
        if (iVar2 != 0) {
          *(int **)(iVar2 + 4) = piVar3;
        }
        if (puVar4 == (undefined4 *)0x0) {
          *piVar1 = (int)piVar3;
        }
        else {
          *puVar4 = piVar3;
        }
        goto LAB_8c098af0;
      }
      piVar6 = (int *)*piVar6;
      if (piVar6 == (int *)0x0) {
        return (int *)0x0;
      }
      iVar2 = piVar6[2];
    }
  }
  return (int *)0x0;
}

