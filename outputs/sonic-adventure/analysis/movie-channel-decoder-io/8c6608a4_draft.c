/* Ghidra draft only; not buildable/verified source. */

int FUN_8c6608a4(int param_1,uint *param_2)

{
  code *pcVar1;
  undefined *puVar2;
  undefined *puVar3;
  undefined *puVar4;
  int *piVar5;
  char *pcVar6;
  uint *puVar7;
  int iVar8;
  int iVar9;
  uint uVar10;
  uint uVar11;
  int iVar12;
  undefined4 uVar13;
  undefined1 auStack_2c [4];
  ushort local_28;
  ushort local_26;
  
  puVar2 = PTR_FUN_8c66095c;
  uVar13 = *(undefined4 *)(PTR_DAT_8c660958 + param_1 * 4);
  pcVar6 = (char *)(*(code *)PTR_FUN_8c66095c)(uVar13);
  puVar7 = (uint *)(*(code *)PTR_FUN_8c660960)(uVar13);
  puVar3 = PTR_thunk_FUN_8c668e5c_8c660964;
  if ((*pcVar6 == '\0') || ((*puVar7 & 2) == 0)) {
LAB_8c660a64:
    (*(code *)puVar3)();
  }
  else {
    (*(code *)PTR_thunk_FUN_8c65f41e_8c660968)(uVar13);
    pcVar1 = (code *)PTR_FUN_8c66096c;
    do {
      while( true ) {
        (*(code *)puVar2)(uVar13);
        iVar8 = (*pcVar1)(uVar13,2,0);
        puVar4 = PTR_thunk_FUN_8c65f3f4_8c660970;
        if (-1 < iVar8) break;
        (*(code *)puVar3)();
      }
      while (iVar9 = (*(code *)puVar4)(uVar13,iVar8), iVar9 == 0) {
        (*(code *)puVar3)();
      }
      iVar8 = (*(code *)puVar2)(uVar13);
      iVar9 = (*(code *)PTR_thunk_FUN_8c65f40e_8c660974)(uVar13);
      piVar5 = DAT_8c660ad4;
      if (iVar9 != 0) {
        (*(code *)puVar3)();
        return (int)DAT_8c66094e;
      }
      iVar9 = *(int *)(iVar8 + 8);
      iVar12 = iVar9 + 8;
      if (*(byte *)(iVar8 + 4) == 8) {
        uVar10 = (**(code **)(*DAT_8c660ad4 + 0xc))(iVar12,0);
        *param_2 = uVar10 & 0xffff;
        uVar10 = (**(code **)(*piVar5 + 0xc))(iVar12,2);
        param_2[1] = uVar10 & 0xffff;
        uVar10 = (**(code **)(*piVar5 + 0xc))(iVar12,4);
        param_2[2] = uVar10 & 0xffff;
        uVar10 = (**(code **)(*piVar5 + 0xc))(iVar12,6);
        param_2[3] = uVar10 & 0xffff;
        uVar10 = (**(code **)(*piVar5 + 0xc))(iVar12,8);
        param_2[4] = uVar10 & 0xffff;
        uVar10 = (**(code **)(*piVar5 + 0xc))(iVar12,10);
        param_2[5] = uVar10 & 0xffff;
        uVar10 = (**(code **)(*piVar5 + 0xc))(iVar12,0xc);
        param_2[6] = uVar10 & 0xffff;
        uVar10 = (**(code **)(*piVar5 + 0xc))(iVar12,0x10);
        param_2[7] = uVar10 & 0xffff;
        uVar10 = (**(code **)(*piVar5 + 0xc))(iVar12,0x12);
        param_2[8] = uVar10 & 0xffff;
        uVar10 = (**(code **)(*piVar5 + 0xc))(iVar12,0x14);
        uVar11 = (uint)DAT_8c660ac8;
        param_2[0xd] = uVar10 & 0xffff;
        uVar10 = (uint)DAT_8c660aca;
        param_2[0xe] = uVar11;
        puVar2 = PTR_FUN_8c660ad8;
        param_2[9] = (uint)*(byte *)(iVar9 + 0x16);
        param_2[10] = (uint)*(byte *)(iVar9 + 0x17);
        param_2[0xb] = uVar10;
        uVar13 = (*(code *)puVar2)(uVar13);
        iVar8 = (*(code *)PTR_FUN_8c660adc)(auStack_2c,uVar13);
        puVar2 = PTR_FUN_8c660ae0;
        if (-1 < iVar8) {
          iVar8 = (int)DAT_8c660acc;
          param_2[0xf] = (uint)local_26;
          param_2[0x10] = (uint)local_28;
          uVar10 = (*(code *)puVar2)(iVar8);
          puVar2 = PTR_FUN_8c660ae0;
          param_2[0x11] = uVar10;
          uVar10 = (*(code *)puVar2)();
          param_2[0x12] = uVar10;
          return 0;
        }
        goto LAB_8c660a64;
      }
      pcVar1 = (code *)PTR_FUN_8c66096c;
    } while (*(byte *)(iVar8 + 4) == DAT_8c660950);
  }
  return (int)DAT_8c660ace;
}

