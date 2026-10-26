/* Ghidra draft only; not buildable/verified source. */

void callback_8c10ebe8(int *param_1)

{
  undefined *puVar1;
  undefined *puVar2;
  uint uVar3;
  int iVar4;
  int *piVar5;
  
  puVar1 = PTR_DAT_8c10ec38;
  iVar4 = (int)DAT_8c10ec36;
  *(undefined4 *)(**(int **)PTR_DAT_8c10ec38 + *param_1 * 0x20 + 0x14) = 0;
  param_1[0xb] = 0;
  param_1[0xc] = 1;
  puVar2 = PTR_DAT_8c10ec3c;
  piVar5 = (int *)(*param_1 * 4 + iVar4 + *(int *)puVar1);
  *piVar5 = *piVar5 + 1;
  uVar3 = *param_1 + 0xf;
  if ((int)uVar3 < 0) {
    iVar4 = 1 >> (~uVar3 & 0x1f) + 1;
  }
  else {
    iVar4 = 1 << (uVar3 & 0x1f);
  }
  **(int **)puVar2 = iVar4;
  if (param_1[7] != 0) {
                    /* WARNING: Could not recover jumptable at 0x8c10ec2e. Too many branches */
                    /* WARNING: Treating indirect jump as call */
    (*(code *)param_1[7])(param_1[8]);
    return;
  }
  return;
}

