/* Ghidra draft only; not buildable/verified source. */

undefined4 FUN_8c10faa4(int *param_1,int param_2)

{
  undefined *puVar1;
  int iVar2;
  uint uVar3;
  int *piVar4;
  undefined4 uVar5;
  int *piVar6;
  int local_18;
  int local_14;
  undefined4 *local_c;
  
  local_c = &DAT_8c67c1d0;
  piVar6 = (int *)(**(int **)PTR_DAT_8c10fb34 + *param_1 * 0x20);
  local_14 = param_2;
  (*(code *)PTR_FUN_8c10fb38)(&local_18);
  uVar5 = DAT_8c10fb3c;
  if (local_18 != 1) {
    (*(code *)PTR_FUN_8c10fb44)(param_1[2],param_1[3] & DAT_8c10fb40);
    piVar6[5] = 0;
    *piVar6 = param_1[1];
    piVar6[1] = param_1[2];
    piVar6[2] = param_1[3];
    piVar6[3] = param_1[4];
    puVar1 = PTR_DAT_8c10fb34;
    if (local_14 == 0) {
      uVar3 = 0;
    }
    else {
      uVar3 = 2;
    }
    iVar2 = (int)DAT_8c10fb32;
    piVar6[4] = uVar3 | param_1[5];
    piVar6[5] = 1;
    param_1[0xb] = 1;
    piVar4 = (int *)(*param_1 * 4 + iVar2 + *(int *)puVar1);
    *piVar4 = *piVar4 + 1;
    piVar6[6] = 1;
    uVar5 = 0;
  }
  return uVar5;
}

