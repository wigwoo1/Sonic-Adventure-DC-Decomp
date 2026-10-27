/* Ghidra draft only; not buildable/verified source. */

undefined4 FUN_8c64b4e4(int param_1)

{
  int iVar1;
  int *piVar2;
  
  piVar2 = *(int **)(param_1 + 0x1c);
  if ((((((0 < *piVar2) && (*piVar2 <= (int)DAT_8c64b5be)) && (*piVar2 == piVar2[2])) &&
       ((-1 < piVar2[3] && (piVar2[3] < *piVar2)))) &&
      (((-1 < piVar2[5] && ((piVar2[5] < *piVar2 && (-1 < piVar2[0xd])))) &&
       (piVar2[0xd] <= *piVar2)))) &&
     (((-1 < piVar2[0xe] && (piVar2[0xe] <= *piVar2)) &&
      ((*(int *)(*DAT_8c64b5cc + 8) != 0 ||
       ((piVar2[4] <= (int)*(short *)(param_1 + 0x10) &&
        (piVar2[6] <= (int)*(short *)(param_1 + 0x12))))))))) {
    iVar1 = *piVar2;
    if (iVar1 == DAT_8c64b5c0) {
      *(undefined4 *)(param_1 + 4) = 0;
    }
    else if (iVar1 == DAT_8c64b5c2) {
      *(undefined4 *)(param_1 + 4) = 1;
    }
    else if (iVar1 == DAT_8c64b5c4) {
      *(undefined4 *)(param_1 + 4) = 2;
    }
    else {
      if (iVar1 != DAT_8c64b5be) {
        return 0xffffffff;
      }
      *(undefined4 *)(param_1 + 4) = 3;
    }
    return 0;
  }
  return 0xffffffff;
}

