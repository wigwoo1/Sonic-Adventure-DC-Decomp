/* Ghidra draft only; not buildable/verified source. */

int FUN_8c64ab36(int param_1)

{
  bool bVar1;
  undefined2 uVar2;
  int iVar3;
  int iVar4;
  uint uVar5;
  uint unaff_r8;
  int iVar6;
  int iVar7;
  uint uVar8;
  undefined4 uVar9;
  
  uVar9 = 0xfffc;
  iVar6 = param_1 * DAT_8c64aba8 + DAT_8c64abbc;
  iVar4 = *(int *)(*(int *)(iVar6 + 0x1c) + 0x18);
  uVar5 = *(uint *)(*(int *)(iVar6 + 0x1c) + 0x14);
  iVar7 = 0;
  bVar1 = false;
  do {
    uVar8 = uVar5 & 0xffff;
    if (iVar4 <= iVar7) {
      if ((!bVar1) ||
         ((iVar4 = FUN_8c64afe0(param_1), -1 < iVar4 && (iVar4 = FUN_8c64ae4a(param_1), -1 < iVar4))
         )) {
        iVar4 = 0;
      }
      return iVar4;
    }
    if (*(char *)(iVar6 + 0x48 + iVar7) == '\0') {
      uVar5 = FUN_8c64b1c0(param_1,uVar8,uVar9);
    }
    else {
      while (iVar3 = (*(code *)PTR_FUN_8c64abac)
                               (param_1,uVar8,*(int *)(iVar6 + 0x28) + iVar7 * 0x200), iVar3 != 0) {
        if (iVar3 == DAT_8c64ac7c) {
          return iVar3;
        }
        bVar1 = true;
        uVar2 = FUN_8c64b1c0(param_1,uVar8);
        FUN_8c64b18e(param_1,uVar8,DAT_8c64ac80);
        uVar5 = (*(code *)PTR_FUN_8c64ac88)(param_1,DAT_8c64ac84);
        if ((int)uVar5 < 0) {
          return (int)DAT_8c64ac7e;
        }
        FUN_8c64b18e(param_1,uVar5,uVar2);
        if (iVar7 == 0) {
          (**(code **)(*DAT_8c64ac8c + 0x10))(*(undefined4 *)(iVar6 + 0x20),0x4a,uVar5);
          unaff_r8 = uVar8;
          uVar8 = uVar5;
        }
        else {
          FUN_8c64b18e(param_1,unaff_r8,uVar5);
          unaff_r8 = uVar8;
          uVar8 = uVar5;
        }
      }
      uVar5 = FUN_8c64b1c0(param_1,uVar8);
      *(undefined1 *)(iVar6 + 0x48 + iVar7) = 0;
    }
    iVar7 = iVar7 + 1;
    unaff_r8 = uVar8;
  } while( true );
}

