/* Ghidra draft only; not buildable/verified source. */

void FUN_8c64e720(undefined4 *param_1)

{
  undefined *puVar1;
  int *piVar2;
  int iVar3;
  undefined4 uVar4;
  
  puVar1 = PTR_DAT_8c64e810;
  *param_1 = 0xffffffff;
  param_1[1] = 0xffffffff;
  uVar4 = 0;
  param_1[5] = 0;
  param_1[6] = 0;
  param_1[7] = 0;
  param_1[8] = 0;
  param_1[9] = 0;
  param_1[10] = 0;
  param_1[0xb] = 0;
  param_1[0xc] = 0;
  param_1[0xd] = 0;
  param_1[4] = 0;
  *(undefined2 *)(param_1 + 0x10) = 0;
  iVar3 = (*(code *)PTR_FUN_8c64e820)((int)param_1 - *(int *)puVar1);
  piVar2 = DAT_8c64e81c;
  if ((0 < *DAT_8c64e81c) && (iVar3 == *DAT_8c64e81c + -1)) {
    while ((0 < *piVar2 && (*(int *)((*piVar2 + -1) * 0x44 + *(int *)PTR_DAT_8c64e810) == -1))) {
      *piVar2 = *piVar2 + -1;
    }
  }
  if (*piVar2 - 1U < *(uint *)PTR_DAT_8c64e818) {
    *(uint *)PTR_DAT_8c64e818 = *piVar2 - 1U;
  }
  else {
    *(undefined4 *)PTR_DAT_8c64e818 = uVar4;
  }
  return;
}

