/* Ghidra draft only; not buildable/verified source. */

undefined4 FUN_8c664352(short param_1,uint param_2,undefined4 *param_3,ushort param_4)

{
  ushort *puVar1;
  ushort *puVar2;
  undefined *puVar3;
  ushort uVar4;
  uint uVar5;
  ushort *puVar6;
  undefined4 uVar7;
  int iVar8;
  int iVar9;
  undefined *puVar10;
  
  puVar3 = PTR_DAT_8c664444;
  if ((int)param_2 < 0) {
    uVar5 = ~(~param_2 + 1 & 0x1f) + 1;
  }
  else {
    uVar5 = param_2 & 0x1f;
  }
  if (uVar5 != 0) {
    param_2 = param_2 + (0x20 - uVar5);
  }
  iVar9 = param_1 * 0x14;
  puVar6 = *(ushort **)(PTR_DAT_8c664444 + iVar9 + 8);
  puVar2 = (ushort *)0x0;
  while (puVar1 = puVar6, puVar1 != (ushort *)0x0) {
    if (*(int *)(puVar1 + 8) < (int)param_2) {
      puVar6 = *(ushort **)(puVar1 + 4);
    }
    else {
      puVar6 = (ushort *)0x0;
      puVar2 = puVar1;
    }
  }
  if (puVar2 == (ushort *)0x0) {
LAB_8c664500:
    uVar7 = 3;
  }
  else {
    if (*(uint *)(puVar2 + 8) == param_2) {
      puVar10 = PTR_DAT_8c664444 + iVar9;
      FUN_8c664b52(puVar10 + 8,puVar10 + 0xc,puVar2);
      FUN_8c664b12(puVar10,puVar10 + 4,puVar2);
      *puVar2 = 1;
      uVar4 = DAT_8c6644e0;
      if (param_4 == 8) {
        param_3[7] = *(undefined4 *)(puVar2 + 6);
        *(undefined4 **)(puVar2 + 10) = param_3;
        *puVar2 = *puVar2 | 8;
      }
      else {
        if (param_4 != DAT_8c664440) goto LAB_8c664500;
        *param_3 = *(undefined4 *)(puVar2 + 6);
        *(undefined4 **)(puVar2 + 10) = param_3;
        *puVar2 = *puVar2 | uVar4;
      }
      *puVar2 = *puVar2 | 4;
      iVar8 = *(int *)(puVar2 + 8);
    }
    else {
      puVar6 = (ushort *)FUN_8c664ac0(0);
      if (puVar6 == (ushort *)0x0) goto LAB_8c664500;
      *(undefined4 *)(puVar6 + 6) = *(undefined4 *)(puVar2 + 6);
      *(uint *)(puVar6 + 8) = param_2;
      *(uint *)(puVar2 + 8) = *(int *)(puVar2 + 8) - param_2;
      *(uint *)(puVar2 + 6) = param_2 + *(int *)(puVar2 + 6);
      FUN_8c664b12(puVar3 + iVar9,puVar3 + iVar9 + 4,puVar6);
      uVar4 = DAT_8c6644e0;
      if (param_4 == 8) {
        param_3[7] = *(undefined4 *)(puVar6 + 6);
        *(undefined4 **)(puVar6 + 10) = param_3;
        *puVar6 = *puVar6 | 8;
      }
      else {
        if (param_4 != DAT_8c6644e0) goto LAB_8c664500;
        *param_3 = *(undefined4 *)(puVar6 + 6);
        *(undefined4 **)(puVar6 + 10) = param_3;
        *puVar6 = *puVar6 | uVar4;
      }
      *puVar6 = *puVar6 | 4;
      iVar8 = *(int *)(puVar6 + 8);
    }
    uVar7 = 0;
    *(int *)(puVar3 + iVar9 + 0x10) = *(int *)(puVar3 + iVar9 + 0x10) - iVar8;
  }
  return uVar7;
}

