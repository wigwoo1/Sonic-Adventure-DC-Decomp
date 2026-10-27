/* Ghidra draft only; not buildable/verified source. */

int FUN_8c660bae(int param_1,undefined4 param_2,int param_3)

{
  undefined *puVar1;
  undefined *puVar2;
  undefined *puVar3;
  ushort uVar4;
  int iVar5;
  undefined4 uVar6;
  int iVar7;
  int iVar8;
  int iVar9;
  undefined4 uVar10;
  byte local_34 [4];
  undefined4 local_30;
  int local_2c;
  int local_28;
  int local_24;
  
  puVar3 = PTR_thunk_FUN_8c668e5c_8c660c0c;
  puVar2 = PTR_thunk_FUN_8c65f3f4_8c660bfc;
  puVar1 = PTR_thunk_FUN_8c65f40e_8c660bf8;
  local_2c = param_1 * DAT_8c660bf2 + DAT_8c660c10;
  iVar5 = FUN_8c660dec(param_1,param_3);
  if (iVar5 == 0) {
    (*(code *)puVar3)();
  }
  else {
    uVar10 = *(undefined4 *)(PTR_DAT_8c660d14 + param_1 * 4);
    (*(code *)PTR_FUN_8c660d18)(uVar10);
    iVar5 = *(int *)(*(int *)(local_2c + 0x1c) + 0x48);
    iVar9 = 0;
    for (local_24 = 0; local_24 < *(int *)(*(int *)(local_2c + 0x1c) + 0x40);
        local_24 = local_24 + 1) {
      while( true ) {
        while( true ) {
          iVar8 = (*(code *)puVar1)(uVar10,iVar5,param_3);
          if (iVar8 != 0) goto LAB_8c660d0a;
          uVar6 = (*(code *)PTR_FUN_8c660d1c)();
          iVar8 = (*(code *)PTR_FUN_8c660d20)(uVar10,2,0,iVar9,param_2,param_3,uVar6);
          if (-1 < iVar8) break;
          (*(code *)puVar3)();
        }
        while (iVar7 = (*(code *)puVar2)(uVar10,iVar8), iVar7 == 0) {
          (*(code *)puVar3)();
        }
        local_28 = (*(code *)PTR_FUN_8c660d18)(uVar10);
        iVar8 = (*(code *)puVar1)(uVar10);
        if (iVar8 != 0) goto LAB_8c660d0a;
        if (*(byte *)(local_28 + 4) == 7) break;
        if (*(byte *)(local_28 + 4) != DAT_8c660d10) goto LAB_8c660d76;
      }
      iVar9 = iVar9 + 1;
      param_3 = param_3 + iVar5;
    }
LAB_8c660cc2:
    do {
      iVar5 = (*(code *)puVar1)(uVar10);
      if (iVar5 != 0) {
LAB_8c660d0a:
        return (int)DAT_8c660d12;
      }
      iVar5 = (*(code *)PTR_FUN_8c660d24)(uVar10,2,0,iVar9,param_2);
      if (iVar5 < 0) {
        (*(code *)puVar3)();
        goto LAB_8c660cc2;
      }
      while (iVar8 = (*(code *)puVar2)(uVar10,iVar5), iVar8 == 0) {
        (*(code *)puVar3)();
      }
      iVar5 = (*(code *)PTR_FUN_8c660d18)(uVar10);
      iVar8 = (*(code *)puVar1)(uVar10);
      if (iVar8 != 0) goto LAB_8c660d0a;
      if ((*(code **)PTR_DAT_8c660e38 == (code *)0x0) ||
         (iVar8 = (**(code **)PTR_DAT_8c660e38)(param_1,param_2,local_34,&local_30), iVar8 == 0)) {
        local_34[0] = *(byte *)(iVar5 + 4);
        local_30 = *(undefined4 *)(iVar5 + 0xc);
      }
      uVar4 = (ushort)local_34[0];
      if (uVar4 == 7) {
        return 0;
      }
    } while (uVar4 == DAT_8c660e2e);
    if (uVar4 == DAT_8c660e30) {
      return (int)DAT_8c660e32;
    }
  }
LAB_8c660d76:
  return (int)DAT_8c660e34;
}

