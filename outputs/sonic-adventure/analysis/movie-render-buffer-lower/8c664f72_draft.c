/* Ghidra draft only; not buildable/verified source. */

undefined4 FUN_8c664f72(short param_1,int param_2,uint param_3,int param_4,int param_5,int param_6)

{
  ushort *puVar1;
  undefined *puVar2;
  uint uVar3;
  int iVar4;
  ushort *puVar5;
  int iVar6;
  int iVar7;
  int iVar8;
  undefined *puVar9;
  
  puVar2 = PTR_DAT_8c665000;
  iVar8 = param_2 + param_3;
  if ((int)param_3 < 0) {
    uVar3 = ~(~param_3 + 1 & 0x1f) + 1;
  }
  else {
    uVar3 = param_3 & 0x1f;
  }
  if (uVar3 != 0) {
    param_3 = param_3 + (0x20 - uVar3);
  }
  iVar4 = param_1 * 0x14;
  puVar1 = (ushort *)0x0;
  puVar5 = *(ushort **)(PTR_DAT_8c665000 + iVar4 + 8);
  while (puVar5 != (ushort *)0x0) {
    iVar7 = *(int *)(puVar5 + 6);
    if ((((param_2 < iVar7) || (*(int *)(puVar5 + 8) + iVar7 <= param_2)) || (iVar8 < iVar7)) ||
       (*(int *)(puVar5 + 8) + iVar7 < iVar8)) {
      puVar5 = *(ushort **)(puVar5 + 4);
    }
    else {
      puVar1 = puVar5;
      puVar5 = (ushort *)0x0;
    }
  }
  if (puVar1 == (ushort *)0x0) {
    return 3;
  }
  if (*(uint *)(puVar1 + 8) == param_3) {
    puVar9 = PTR_DAT_8c665000 + iVar4;
    FUN_8c664b52(puVar9 + 8,puVar9 + 0xc,puVar1);
    FUN_8c664b12(puVar9,puVar9 + 4,puVar1);
    *(undefined4 *)(param_4 + 8) = *(undefined4 *)(puVar1 + 6);
    *(int *)(puVar1 + 10) = param_4;
    *puVar1 = 1;
    if (param_1 == 0) {
      *puVar1 = *puVar1 | DAT_8c6650a0;
    }
    else {
      *puVar1 = *puVar1 | DAT_8c6650a2;
    }
    if (param_5 != 0) {
      *puVar1 = *puVar1 | 2;
    }
    if (param_6 != 0) {
      *puVar1 = *puVar1 | 4;
    }
    iVar8 = *(int *)(puVar1 + 8);
  }
  else {
    if (*(int *)(puVar1 + 6) != param_2) {
      if (*(int *)(puVar1 + 6) + *(int *)(puVar1 + 8) == iVar8) {
        puVar5 = (ushort *)FUN_8c664ac0(1);
        if (puVar5 == (ushort *)0x0) {
          return 3;
        }
        *(uint *)(puVar1 + 8) = *(int *)(puVar1 + 8) - param_3;
        *(int *)(puVar5 + 6) = param_2;
        *(uint *)(puVar5 + 8) = param_3;
        FUN_8c664b12(puVar2 + iVar4,puVar2 + iVar4 + 4,puVar5);
        *(undefined4 *)(param_4 + 8) = *(undefined4 *)(puVar5 + 6);
        *(int *)(puVar5 + 10) = param_4;
        if (param_1 == 0) {
          *puVar5 = *puVar5 | DAT_8c6651e0;
        }
        else {
          *puVar5 = *puVar5 | DAT_8c6651e2;
        }
        if (param_5 != 0) {
          *puVar5 = *puVar5 | 2;
        }
        if (param_6 != 0) {
          *puVar5 = *puVar5 | 4;
        }
      }
      else {
        puVar5 = (ushort *)FUN_8c664ac0(1);
        if (puVar5 == (ushort *)0x0) {
          return 3;
        }
        iVar8 = FUN_8c664ac0();
        if (iVar8 == 0) {
          FUN_8c664aec(puVar5);
          return 3;
        }
        iVar7 = *(int *)(puVar1 + 8);
        iVar6 = *(int *)(puVar1 + 6);
        *(int *)(puVar1 + 8) = param_2 - iVar6;
        puVar9 = puVar2 + iVar4;
        *(int *)(puVar5 + 6) = param_2;
        *(uint *)(puVar5 + 8) = param_3;
        FUN_8c664b12(puVar9,puVar9 + 4,puVar5);
        *(uint *)(iVar8 + 0xc) = param_2 + param_3;
        *(uint *)(iVar8 + 0x10) = (iVar7 - param_3) - (param_2 - iVar6);
        FUN_8c664b36(puVar9 + 0xc,iVar8,*(undefined4 *)(puVar9 + 0xc));
        *(undefined4 *)(param_4 + 8) = *(undefined4 *)(puVar5 + 6);
        *(int *)(puVar5 + 10) = param_4;
        if (param_1 == 0) {
          *puVar5 = *puVar5 | DAT_8c6652a0;
        }
        else {
          *puVar5 = *puVar5 | DAT_8c6652a2;
        }
        if (param_5 != 0) {
          *puVar5 = *puVar5 | 2;
        }
        if (param_6 != 0) {
          *puVar5 = *puVar5 | 4;
        }
      }
      *(int *)(puVar2 + iVar4 + 0x10) = *(int *)(puVar2 + iVar4 + 0x10) - *(int *)(puVar5 + 8);
      return 0;
    }
    puVar5 = (ushort *)FUN_8c664ac0(1);
    if (puVar5 == (ushort *)0x0) {
      return 3;
    }
    *(undefined4 *)(puVar5 + 6) = *(undefined4 *)(puVar1 + 6);
    *(uint *)(puVar5 + 8) = param_3;
    *(uint *)(puVar1 + 8) = *(int *)(puVar1 + 8) - param_3;
    *(uint *)(puVar1 + 6) = *(int *)(puVar1 + 6) + param_3;
    FUN_8c664b12(puVar2 + iVar4,puVar2 + iVar4 + 4,puVar5);
    *(undefined4 *)(param_4 + 8) = *(undefined4 *)(puVar5 + 6);
    *(int *)(puVar5 + 10) = param_4;
    if (param_1 == 0) {
      *puVar5 = *puVar5 | DAT_8c665140;
    }
    else {
      *puVar5 = *puVar5 | DAT_8c665142;
    }
    if (param_5 != 0) {
      *puVar5 = *puVar5 | 2;
    }
    if (param_6 != 0) {
      *puVar5 = *puVar5 | 4;
    }
    iVar8 = *(int *)(puVar5 + 8);
  }
  *(int *)(puVar2 + iVar4 + 0x10) = *(int *)(puVar2 + iVar4 + 0x10) - iVar8;
  return 0;
}

