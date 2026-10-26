/* Ghidra draft only; not buildable/verified source. */

undefined4 FUN_8c66466c(short param_1,uint param_2,int param_3)

{
  undefined *puVar1;
  uint uVar2;
  int iVar3;
  int iVar4;
  ushort *puVar5;
  ushort *puVar6;
  int iVar7;
  int iVar8;
  ushort *puVar9;
  uint uVar10;
  undefined *puVar11;
  ushort *puVar12;
  undefined4 uVar13;
  
  puVar1 = PTR_DAT_8c664724;
  uVar13 = 0;
  iVar7 = 0;
  if ((int)param_2 < 0) {
    uVar2 = ~(~param_2 + 1 & 0x1f) + 1;
  }
  else {
    uVar2 = param_2 & 0x1f;
  }
  puVar9 = (ushort *)0x0;
  uVar10 = param_2;
  if (uVar2 != 0) {
    uVar10 = param_2 + (0x20 - uVar2);
  }
  puVar12 = (ushort *)0x0;
  iVar3 = param_1 * 0x14;
  if (*(ushort **)(PTR_DAT_8c664724 + iVar3 + 8) != (ushort *)0x0) {
    iVar8 = (int)DAT_8c664720;
    puVar5 = *(ushort **)(PTR_DAT_8c664724 + iVar3 + 8);
    do {
      if (*(int *)(puVar5 + 8) < (int)uVar10) {
        puVar6 = *(ushort **)(puVar5 + 4);
      }
      else {
        if (puVar9 == (ushort *)0x0) {
          puVar9 = puVar5;
        }
        iVar4 = (*(code *)PTR_FUN_8c664728)(param_2,uVar13);
        if (iVar4 == 0) {
          param_2 = *(uint *)(puVar5 + 6);
LAB_8c664714:
          iVar7 = 1;
        }
        else {
          param_2 = *(int *)(puVar5 + 6) + iVar8 & (int)DAT_8c664722;
          iVar4 = *(int *)(puVar5 + 6) + *(int *)(puVar5 + 8);
          if ((iVar4 <= (int)param_2) || ((int)uVar10 <= (int)(param_2 - *(int *)(puVar5 + 6))))
          goto LAB_8c664714;
          if ((int)(iVar4 - param_2) < (int)uVar10) {
            puVar6 = *(ushort **)(puVar5 + 4);
            goto LAB_8c664742;
          }
          iVar7 = 2;
        }
        puVar6 = (ushort *)0x0;
        puVar12 = puVar5;
      }
LAB_8c664742:
      puVar5 = puVar6;
    } while (puVar6 != (ushort *)0x0);
  }
  if ((iVar7 == 0) && (puVar9 != (ushort *)0x0)) {
    iVar7 = 1;
    puVar12 = puVar9;
  }
  if (puVar12 == (ushort *)0x0) {
LAB_8c6647ea:
    uVar13 = 3;
  }
  else {
    if (iVar7 == 2) {
      uVar13 = FUN_8c664f72((int)param_1,uVar10,param_3,0,1);
      return uVar13;
    }
    if (*(uint *)(puVar12 + 8) == uVar10) {
      puVar11 = puVar1 + iVar3;
      FUN_8c664b52(puVar11 + 8,puVar11 + 0xc,puVar12);
      FUN_8c664b12(puVar11,puVar11 + 4,puVar12);
      *(undefined4 *)(param_3 + 8) = *(undefined4 *)(puVar12 + 6);
      *(int *)(puVar12 + 10) = param_3;
      *puVar12 = 1;
      if (param_1 == 0) {
        *puVar12 = *puVar12 | DAT_8c6647c0;
      }
      else {
        *puVar12 = *puVar12 | DAT_8c6647c2;
      }
      *puVar12 = *puVar12 | 4;
      iVar7 = *(int *)(puVar12 + 8);
    }
    else {
      puVar9 = (ushort *)FUN_8c664ac0();
      if (puVar9 == (ushort *)0x0) goto LAB_8c6647ea;
      *(undefined4 *)(puVar9 + 6) = *(undefined4 *)(puVar12 + 6);
      *(uint *)(puVar9 + 8) = uVar10;
      *(uint *)(puVar12 + 8) = *(int *)(puVar12 + 8) - uVar10;
      *(uint *)(puVar12 + 6) = *(int *)(puVar12 + 6) + uVar10;
      FUN_8c664b12(puVar1 + iVar3,puVar1 + iVar3 + 4,puVar9);
      *(undefined4 *)(param_3 + 8) = *(undefined4 *)(puVar9 + 6);
      *(int *)(puVar9 + 10) = param_3;
      if (param_1 == 0) {
        *puVar9 = *puVar9 | DAT_8c664860;
      }
      else {
        *puVar9 = *puVar9 | DAT_8c664862;
      }
      *puVar9 = *puVar9 | 4;
      iVar7 = *(int *)(puVar9 + 8);
    }
    uVar13 = 0;
    *(int *)(puVar1 + iVar3 + 0x10) = *(int *)(puVar1 + iVar3 + 0x10) - iVar7;
  }
  return uVar13;
}

