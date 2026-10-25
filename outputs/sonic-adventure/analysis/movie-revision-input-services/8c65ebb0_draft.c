/* Ghidra draft only; not buildable/verified source. */

undefined4 FUN_8c65ebb0(undefined4 param_1,undefined4 param_2)

{
  int *piVar1;
  uint uVar2;
  undefined *puVar3;
  undefined4 uVar4;
  int iVar5;
  int iVar6;
  int *piVar7;
  undefined4 *puVar8;
  int *piVar9;
  
  iVar6 = DAT_8c65ecb0;
  piVar1 = DAT_8c65ecac;
  iVar5 = (int)DAT_8c65ec9c;
  *DAT_8c65ecac = DAT_8c65ecb0;
  *(undefined4 *)(iVar5 + iVar6) = param_1;
  *(undefined4 *)(iVar5 + 4 + *piVar1) = param_2;
  if ((*(uint *)(iVar5 + *piVar1) & 0x1f) != 0) {
    iVar6 = (int)DAT_8c65ec9c;
    *(int *)(iVar6 + *piVar1) = *(int *)(iVar6 + *piVar1) + 0x20;
    *(uint *)(iVar6 + *piVar1) = *(uint *)(iVar6 + *piVar1) & 0xffffffe0;
  }
  iVar6 = (int)DAT_8c65ec9e;
  iVar5 = *piVar1;
  if ((*(uint *)(iVar6 + iVar5) & 0x1f) != 0) {
    *(int *)(iVar6 + iVar5) = *(int *)(iVar6 + iVar5) + 0x20;
    *(uint *)(iVar6 + *piVar1) = *(uint *)(iVar6 + *piVar1) & 0xffffffe0;
  }
  uVar2 = DAT_8c65ecb4;
  iVar6 = (int)DAT_8c65ec9c;
  *(uint *)(iVar6 + *piVar1) = *(uint *)(iVar6 + *piVar1) | DAT_8c65ecb4;
  *(uint *)(iVar6 + 4 + *piVar1) = *(uint *)(iVar6 + 4 + *piVar1) | uVar2;
  (*(code *)PTR_FUN_8c65ecb8)(0x8c88e9a4);
  puVar3 = PTR_FUN_8c65ecc0;
  piVar7 = (int *)(PTR_DAT_8c65ecbc + 0x10);
  puVar8 = (undefined4 *)PTR_DAT_8c65ecbc;
  piVar9 = (int *)PTR_DAT_8c65ecbc;
  do {
    if (*(char *)((int)DAT_8c65eca0 + *piVar1 + *piVar9) != '\0') {
      (*(code *)puVar3)(*puVar8);
    }
    piVar9 = piVar9 + 1;
    puVar8 = puVar8 + 1;
  } while (piVar9 < piVar7);
  (*(code *)PTR_FUN_8c65ecc4)();
  uVar4 = DAT_8c65ecc8;
  *(undefined4 *)((int)DAT_8c65eca2 + *piVar1) = 1;
  *(undefined4 *)((int)DAT_8c65eca6 + *(int *)((int)DAT_8c65eca4 + *piVar1)) = uVar4;
  iVar6 = (int)DAT_8c65eca4;
  *(undefined4 *)(*(int *)(iVar6 + *piVar1) + 0x10) = 0;
  iVar5 = *piVar1;
  *(uint *)((int)DAT_8c65eca8 + *(int *)(iVar6 + iVar5)) =
       *(uint *)(iVar6 + -0x34 + iVar5) | *(uint *)(iVar6 + -0x24 + iVar5);
  *(undefined4 *)(*(int *)((int)DAT_8c65eca4 + *piVar1) + 0x14) = 1;
  return 0;
}

