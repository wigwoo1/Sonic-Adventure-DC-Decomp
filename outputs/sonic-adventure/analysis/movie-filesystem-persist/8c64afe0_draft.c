/* Ghidra draft only; not buildable/verified source. */

int FUN_8c64afe0(int param_1)

{
  bool bVar1;
  int *piVar2;
  undefined4 uVar3;
  int iVar4;
  int iVar5;
  undefined4 uVar6;
  
  uVar3 = DAT_8c64b0e0;
  piVar2 = DAT_8c64b0dc;
  uVar6 = 0xfffc;
  iVar5 = param_1 * DAT_8c64b0d2 + DAT_8c64b0d8;
  bVar1 = false;
  while( true ) {
    iVar4 = FUN_8c64aecc(param_1,uVar6);
    if (iVar4 == DAT_8c64b0d4) {
      return (int)DAT_8c64b0d4;
    }
    if (iVar4 != DAT_8c64b0d6) break;
    iVar4 = FUN_8c64af4a(param_1);
    if (iVar4 < 0) {
      return (int)DAT_8c64b0d6;
    }
    *(int *)(*(int *)(iVar5 + 0x1c) + 0xc) = iVar4;
    (**(code **)(*piVar2 + 0x10))(*(undefined4 *)(iVar5 + 0x20),0x46,iVar4);
    for (iVar4 = 0; bVar1 = true, iVar4 < *(int *)(*(int *)(iVar5 + 0x1c) + 0x10); iVar4 = iVar4 + 1
        ) {
      FUN_8c64b18e(param_1,*(int *)(*(int *)(iVar5 + 0x1c) + 0xc) + iVar4,uVar3);
    }
  }
  if (bVar1) {
    FUN_8c64ae4a(param_1);
  }
  return 0;
}

