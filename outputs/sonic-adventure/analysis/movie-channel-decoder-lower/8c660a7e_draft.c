/* Ghidra draft only; not buildable/verified source. */

int FUN_8c660a7e(int param_1,undefined4 param_2,int param_3)

{
  undefined *puVar1;
  undefined *puVar2;
  ushort uVar3;
  int iVar4;
  int iVar5;
  int iVar6;
  int iVar7;
  undefined4 uVar8;
  int local_2c;
  
  puVar1 = PTR_thunk_FUN_8c668e5c_8c660ae8;
  iVar7 = param_1 * DAT_8c660ad0 + DAT_8c660ae4;
  iVar4 = FUN_8c660dec(param_1,param_2,0);
  if (iVar4 == 0) {
    (*(code *)puVar1)();
LAB_8c660b68:
    iVar4 = (int)DAT_8c660bf0;
  }
  else {
    uVar8 = *(undefined4 *)(PTR_DAT_8c660aec + param_1 * 4);
    (*(code *)PTR_FUN_8c660af0)(uVar8);
    iVar4 = 0;
    for (local_2c = 0; local_2c < *(int *)(*(int *)(iVar7 + 0x1c) + 0x3c); local_2c = local_2c + 1)
    {
LAB_8c660af4:
      puVar2 = PTR_thunk_FUN_8c65f40e_8c660bf8;
      iVar5 = (*(code *)PTR_thunk_FUN_8c65f40e_8c660bf8)(uVar8);
      if (iVar5 != 0) {
LAB_8c660b42:
        return (int)DAT_8c660bea;
      }
      iVar5 = (*(code *)PTR_FUN_8c660bf4)(uVar8,2,0,iVar4,param_2);
      if (iVar5 < 0) {
        (*(code *)puVar1)();
        goto LAB_8c660af4;
      }
      while (iVar6 = (*(code *)PTR_thunk_FUN_8c65f3f4_8c660bfc)(uVar8,iVar5), iVar6 == 0) {
        (*(code *)puVar1)();
      }
      iVar5 = (*(code *)PTR_FUN_8c660c00)(uVar8);
      iVar6 = (*(code *)puVar2)(uVar8);
      if (iVar6 != 0) goto LAB_8c660b42;
      uVar3 = (ushort)*(byte *)(iVar5 + 4);
      if (uVar3 != 8) {
        if (uVar3 != DAT_8c660bec) {
          if (uVar3 == DAT_8c660bee) {
            return DAT_8c660c04;
          }
          goto LAB_8c660b68;
        }
        goto LAB_8c660af4;
      }
      (*(code *)PTR_FUN_8c660c08)
                (param_3,*(int *)(iVar5 + 8) + 0xc,*(undefined4 *)(*(int *)(iVar7 + 0x1c) + 0x44));
      iVar4 = iVar4 + 1;
      param_3 = param_3 + *(int *)(*(int *)(iVar7 + 0x1c) + 0x44);
    }
    iVar4 = 0;
  }
  return iVar4;
}

