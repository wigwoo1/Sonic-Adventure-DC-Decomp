/* Ghidra draft only; not buildable/verified source. */

undefined4 FUN_8c603e08(int param_1)

{
  undefined *puVar1;
  int iVar2;
  int iVar3;
  undefined4 uVar4;
  int iVar5;
  int iVar6;
  int iVar7;
  ushort *puVar8;
  
  puVar1 = PTR_FUN_8c604004;
  iVar5 = *(int *)(param_1 + 4);
  if (*(short *)(param_1 + 2) == 0) {
    puVar8 = *(ushort **)(param_1 + 0xc);
    iVar2 = (*(code *)PTR_FUN_8c604004)(iVar5,puVar8,0x40);
    if (iVar2 == 0x40) {
      iVar7 = iVar5 + 0x40;
      iVar6 = 0;
      for (iVar2 = *(int *)(param_1 + 0x14); puVar8 = puVar8 + 0x20, 0 < iVar2; iVar2 = iVar2 + -1)
      {
        if ((*puVar8 != DAT_8c603ffc) && (iVar3 = FUN_8c603cd2(param_1,puVar8), iVar3 == 0)) {
          iVar3 = (*(code *)puVar1)(iVar7,puVar8,0x40);
          if (iVar3 != 0x40) goto LAB_8c603ea4;
          iVar6 = iVar6 + 1;
          iVar7 = iVar7 + 0x40;
        }
      }
      iVar2 = *(int *)(param_1 + 0x14);
      FUN_8c603c84(iVar6,puVar8,*(undefined4 *)(param_1 + 0x18));
      iVar5 = (*(code *)puVar1)((iVar2 + 1) * 0x40 + iVar5,puVar8,*(int *)(param_1 + 0x18) << 6);
      if (iVar5 == *(int *)(param_1 + 0x18) << 6) {
        *(int *)(param_1 + 0x10) = iVar6;
        return 0;
      }
    }
LAB_8c603ea4:
    uVar4 = 0xfffffffe;
  }
  else {
    uVar4 = 0xfffffffa;
  }
  return uVar4;
}

