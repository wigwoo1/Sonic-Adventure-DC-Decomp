/* Ghidra draft only; not buildable/verified source. */

int FUN_8c663320(uint param_1,int param_2)

{
  undefined *puVar1;
  undefined *puVar2;
  undefined *puVar3;
  uint uVar4;
  int iVar5;
  undefined4 uVar6;
  
  puVar3 = PTR_FUN_8c663454;
  puVar2 = PTR_DAT_8c663444;
  puVar1 = PTR_DAT_8c663440;
  if ((int)param_1 < 0) {
    uVar4 = ~(~param_1 + 1 & 0x1f) + 1;
  }
  else {
    uVar4 = param_1 & 0x1f;
  }
  if (uVar4 != 0) {
    param_1 = param_1 + (0x20 - uVar4);
  }
  if (*(uint *)PTR_DAT_8c663440 == param_1) {
    *(uint *)(param_2 + 0x14) = param_1;
    uVar6 = (*(code *)puVar3)(*(int *)puVar2 + 4);
    *(undefined4 *)(param_2 + 0x1c) = uVar6;
    iVar5 = 0;
    *(undefined4 *)puVar1 = 0;
    *(undefined4 *)puVar2 = 0;
  }
  else {
    if ((*DAT_8c663448 & 4) == 0) {
      iVar5 = (*(code *)PTR_FUN_8c663450)(0,param_1 << 1,param_2,8);
    }
    else {
      iVar5 = (*(code *)PTR_FUN_8c663450)
                        (*(uint *)(PTR_DAT_8c66344c + 0x10) < *(uint *)(PTR_DAT_8c66344c + 0x24),
                         param_2,8);
    }
    puVar3 = PTR_FUN_8c663454;
    if (iVar5 == 0) {
      *(uint *)puVar1 = param_1;
      *(undefined4 *)puVar2 = *(undefined4 *)(param_2 + 0x1c);
      *(uint *)(param_2 + 0x14) = param_1;
      uVar6 = (*(code *)puVar3)(*(undefined4 *)(param_2 + 0x1c));
      *(undefined4 *)(param_2 + 0x1c) = uVar6;
    }
  }
  return iVar5;
}

