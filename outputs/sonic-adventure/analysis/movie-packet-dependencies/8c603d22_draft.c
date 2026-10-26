/* Ghidra draft only; not buildable/verified source. */

undefined4 FUN_8c603d22(int param_1)

{
  undefined *puVar1;
  short sVar2;
  int iVar3;
  int iVar4;
  int iVar5;
  int iVar6;
  ushort *puVar7;
  code *pcVar8;
  
  puVar1 = PTR_FUN_8c603da4;
  pcVar8 = FUN_8c098b42;
  iVar4 = *(int *)(param_1 + 0xc);
  iVar6 = *(int *)(param_1 + 4);
  iVar3 = (*(code *)PTR_FUN_8c603da4)(iVar6,iVar4,0x40,FUN_8c098b42);
  if (iVar3 != -1) {
    iVar5 = (*(int *)(param_1 + 0x14) + 1) * 0x40;
    iVar3 = (*(code *)puVar1)(iVar6 + iVar5,*(int *)(param_1 + 0x18) << 6,pcVar8);
    if (iVar3 != -1) {
      iVar3 = FUN_8c603c4c(iVar5 + iVar4,*(undefined4 *)(param_1 + 0x18));
      *(int *)(param_1 + 0x10) = iVar3;
      if (*(short *)(param_1 + 2) != 0) {
        return 0xfffffffa;
      }
      puVar7 = (ushort *)(param_1 + 0x34);
      while( true ) {
        if (iVar3 < 1) {
          return 0;
        }
        iVar6 = iVar6 + 0x40;
        iVar5 = (*(code *)puVar1)(iVar6,puVar7,0x40,pcVar8);
        if (iVar5 == -1) break;
        if (*puVar7 == DAT_8c603ffc) {
          return 0;
        }
        sVar2 = (*(code *)PTR_FUN_8c604000)(0x3e,puVar7);
        if ((*(short *)(param_1 + 0x72) == sVar2) && ((int)(uint)*puVar7 < *(int *)(param_1 + 0x14))
           ) {
          FUN_8c603c08(puVar7,(*puVar7 + 1) * 0x40 + iVar4,0x40);
        }
        iVar3 = iVar3 + -1;
      }
      return 0xffffffff;
    }
  }
  return 0xffffffff;
}

