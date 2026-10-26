/* Ghidra draft only; not buildable/verified source. */

int FUN_8c665ae0(undefined4 *param_1,undefined4 param_2,undefined4 param_3,int param_4)

{
  undefined *puVar1;
  int *piVar2;
  int iVar3;
  undefined4 uVar4;
  int iVar5;
  undefined4 *puVar6;
  uint uVar7;
  int iVar8;
  int local_18;
  undefined4 local_c;
  
  iVar8 = DAT_8c665bf0;
  puVar1 = PTR_DAT_8c665bec;
  local_c = 0;
  iVar3 = (int)DAT_8c665be0;
  *(undefined4 *)((int)&local_18 + iVar3) = param_3;
  uVar4 = *(undefined4 *)(PTR_DAT_8c665be8 + 4);
  *(undefined4 *)(&stack0xffffffec + iVar3) = *(undefined4 *)PTR_DAT_8c665be8;
  *(undefined4 *)(&stack0xfffffff0 + iVar3) = uVar4;
  uVar4 = *(undefined4 *)(PTR_DAT_8c665be8 + 0xc);
  *(undefined4 *)((int)&stack0xfffffff4 + iVar3) = *(undefined4 *)(PTR_DAT_8c665be8 + 8);
  *(undefined4 *)(&stack0xfffffff8 + iVar3) = uVar4;
  uVar4 = *(undefined4 *)(PTR_DAT_8c665be8 + 0x14);
  *(undefined4 *)(&stack0xfffffffc + iVar3) = *(undefined4 *)(PTR_DAT_8c665be8 + 0x10);
  *(undefined4 *)(&stack0x00000000 + iVar3) = uVar4;
  *(undefined4 **)(*(int *)PTR_DAT_8c665bec * 0x10 + DAT_8c665bf0 + 0x1c) = param_1;
  *(undefined4 *)(*(int *)puVar1 * 0x10 + iVar8 + 0x20) = param_2;
  *(undefined4 *)(*(int *)puVar1 * 0x10 + iVar8 + 0x24) = *(undefined4 *)((int)&local_18 + iVar3);
  iVar5 = (int)DAT_8c665be2;
  *(int *)(iVar8 + 0x18) = param_4;
  *(undefined4 *)(*(int *)puVar1 * 0x10 + iVar8 + 0x28) =
       *(undefined4 *)((int)&local_18 + iVar5 + iVar3);
  piVar2 = DAT_8c665bf4;
  *(uint *)puVar1 = *(int *)puVar1 + 1U & 1;
  *piVar2 = param_4;
  if (*(int *)(iVar8 + 0x54) != 0) {
    FUN_8c665cc0((int)DAT_8c665be4,param_4,*(undefined4 *)((int)&local_18 + iVar3));
  }
  iVar5 = (*(code *)PTR_FUN_8c665bf8)();
  if (iVar5 == DAT_8c665bfc) {
    iVar5 = (*(code *)PTR_FUN_8c665c00)(*(undefined4 *)(iVar8 + 0x60),param_1);
    piVar2 = DAT_8c665c04;
    if (iVar5 != 0) {
      return iVar5;
    }
    *param_1 = 1;
    param_1[1] = *(undefined4 *)(&stack0xffffffec + *piVar2 * 4 + iVar3);
    param_1[2] = *piVar2;
    param_1[3] = *(undefined4 *)(iVar8 + 0x48);
    param_1[4] = *(undefined4 *)(iVar8 + 0x4c);
    if (param_4 == 1) {
      param_1[6] = 1;
    }
    else {
      param_1[6] = 0;
    }
    puVar1 = PTR_FUN_8c665ca8;
    if (*(int *)((int)&local_18 + DAT_8c665ca0 + iVar3) == 1) {
      iVar8 = (int)DAT_8c665ca2;
      for (puVar6 = (undefined4 *)(&stack0x00000004 + iVar3);
          puVar6 < &stack0x00000004 + iVar8 + iVar3; puVar6 = puVar6 + 1) {
        *puVar6 = 0;
      }
      uVar7 = 0;
      if (param_1[5] != 0) {
        do {
          (*(code *)puVar1)(param_1[7] + uVar7,&stack0x00000004 + iVar3,iVar8);
          uVar7 = uVar7 + iVar8;
        } while (uVar7 < (uint)param_1[5]);
      }
    }
  }
  else {
    (*(code *)PTR_FUN_8c665cac)((int)DAT_8c665ca4);
  }
  return 0;
}

