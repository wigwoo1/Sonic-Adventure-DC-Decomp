/* Ghidra draft only; not buildable/verified source. */

int FUN_8c64919e(int param_1,int param_2)

{
  undefined *puVar1;
  int *piVar2;
  int iVar3;
  undefined1 *puVar4;
  undefined1 uVar5;
  int iVar6;
  undefined1 *puVar7;
  undefined1 *puVar8;
  int iVar9;
  undefined1 *puVar10;
  int iVar11;
  
  iVar6 = DAT_8c649264;
  if (param_1 == 0) {
    return (int)DAT_8c649254;
  }
  iVar9 = (int)DAT_8c64924e;
  iVar11 = 0;
  if (((*(char *)(*(int *)(param_1 + 0x28) * iVar9 + DAT_8c649264 + 2) != '\0') &&
      (iVar3 = (*(code *)PTR_FUN_8c649258)(*(undefined4 *)(param_1 + 0x28),0), iVar3 != 0)) &&
     (*(char *)(*(int *)(param_1 + 0x28) * iVar9 + iVar6 + 8) != '\0')) {
    iVar3 = (*(code *)PTR_FUN_8c649268)(*(undefined4 *)(param_1 + 0x28));
    if (iVar3 == 0) {
      FUN_8c64850a(*(undefined4 *)(param_1 + 0x28));
    }
    else {
      iVar3 = (*(code *)PTR_thunk_FUN_8c64a4b2_8c649260)(*(undefined4 *)(param_1 + 0x28));
      puVar1 = PTR_DAT_8c64926c;
      if (-1 < iVar3) {
        iVar3 = *(int *)(param_1 + 0x28);
        if (*(int *)(param_1 + 4) != 0) {
          if (param_2 != 0) {
            iVar6 = iVar3 * iVar9 + iVar6;
            *(undefined4 *)(iVar6 + 0x30) = 1;
            *(undefined4 *)(iVar6 + 0x34) = *(undefined4 *)(puVar1 + *(int *)(iVar6 + 4) * 0x50);
          }
          puVar4 = (undefined1 *)(*(code *)PTR_FUN_8c649270)(iVar3,*(undefined4 *)(param_1 + 0x2c));
          piVar2 = DAT_8c649364;
          if (*(int *)(param_1 + 8) == 1) {
            uVar5 = (undefined1)DAT_8c649256;
          }
          else {
            uVar5 = 0x33;
          }
          *puVar4 = uVar5;
          puVar4[1] = *(undefined1 *)(param_1 + 1);
          (**(code **)(*piVar2 + 0x10))(puVar4,2,*(undefined4 *)(param_1 + 0x38));
          (**(code **)(*piVar2 + 0x10))(puVar4,0x18,*(undefined4 *)(param_1 + 0x30));
          (**(code **)(*piVar2 + 0x10))(puVar4,0x1a,(int)*(short *)(param_1 + 2));
          puVar8 = (undefined1 *)(param_1 + 0x10);
          puVar10 = puVar4 + 0x10;
          for (puVar7 = puVar4 + 4; puVar7 < puVar10; puVar7 = puVar7 + 1) {
            uVar5 = *puVar8;
            puVar8 = puVar8 + 1;
            *puVar7 = uVar5;
          }
          puVar7 = (undefined1 *)(param_1 + 0x20);
          for (; puVar10 < puVar4 + 0x18; puVar10 = puVar10 + 1) {
            uVar5 = *puVar7;
            puVar7 = puVar7 + 1;
            *puVar10 = uVar5;
          }
          (*(code *)PTR_FUN_8c649368)(iVar3,*(undefined4 *)(param_1 + 0x2c));
          iVar11 = (*(code *)PTR_FUN_8c64936c)(iVar3);
          if (iVar11 < 0) goto LAB_8c6492ee;
          (*(code *)PTR_FUN_8c649370)(iVar3);
        }
        iVar11 = 0;
      }
    }
  }
LAB_8c6492ee:
  (*(code *)PTR_FUN_8c649374)(param_1);
  return iVar11;
}

