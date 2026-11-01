/* Ghidra draft only; not buildable/verified source. */

void callback_8c0b3a2e(int param_1)

{
  undefined *puVar1;
  int iVar2;
  uint uVar3;
  int *piVar4;
  undefined *puVar5;
  int iVar6;
  
  iVar6 = *(int *)(param_1 + 0x20);
  *(undefined **)(param_1 + 0x14) = PTR_FUN_8c0b3ac0;
  puVar1 = PTR_LAB_8c0b3ac8;
  *(undefined **)(param_1 + 0x10) = PTR_LAB_8c0b3ac4;
  *(undefined **)(param_1 + 0x18) = puVar1;
  puVar1 = PTR_FUN_8c0b3acc;
  *(undefined4 *)(iVar6 + 0x10) = 0;
  *(undefined1 *)(iVar6 + 1) = 0;
  *(undefined2 *)(iVar6 + 6) = 0;
  iVar2 = (*(code *)puVar1)();
  puVar1 = PTR_FUN_8c0b3ad4;
  piVar4 = (int *)PTR_DAT_8c0b3ad0;
  if (iVar2 == 0) {
    *(undefined1 *)(iVar6 + 3) = 0;
    *(undefined4 *)(iVar6 + 8) = 0;
    uVar3 = 0;
    puVar5 = (undefined *)piVar4;
    do {
      iVar2 = (*(code *)puVar1)(0);
      if (iVar2 == *piVar4) {
        *(undefined **)(iVar6 + 8) = puVar5;
        return;
      }
      uVar3 = uVar3 + 3;
      puVar5 = puVar5 + 0xd8;
      piVar4 = piVar4 + 0x36;
    } while (uVar3 < 0xf);
  }
  return;
}

