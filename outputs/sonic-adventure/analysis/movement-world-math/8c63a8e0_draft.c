/* Ghidra draft only; not buildable/verified source. */

void FUN_8c63a8e0(undefined8 *param_1)

{
  undefined *puVar1;
  undefined4 *puVar2;
  undefined4 uVar4;
  undefined8 uVar5;
  undefined4 uVar7;
  undefined8 uVar8;
  undefined4 uVar10;
  undefined8 uVar11;
  undefined8 uVar13;
  undefined8 uVar15;
  undefined8 uVar16;
  undefined8 uVar17;
  char in_FPSCR_SZ;
  undefined8 *puVar3;
  undefined4 uVar6;
  undefined4 uVar9;
  undefined4 uVar12;
  undefined4 uVar14;
  
  puVar1 = PTR_DAT_8c63a8fc;
  if (param_1 != (undefined8 *)0x0) {
    if (in_FPSCR_SZ == '\0') {
      uVar6 = *(undefined4 *)PTR_DAT_8c63a8fc;
      uVar4 = *(undefined4 *)(PTR_DAT_8c63a8fc + 4);
      uVar9 = *(undefined4 *)(PTR_DAT_8c63a8fc + 8);
      uVar7 = *(undefined4 *)(PTR_DAT_8c63a8fc + 0xc);
      uVar12 = *(undefined4 *)(PTR_DAT_8c63a8fc + 0x10);
      uVar10 = *(undefined4 *)(PTR_DAT_8c63a8fc + 0x14);
      uVar14 = *(undefined4 *)(PTR_DAT_8c63a8fc + 0x18);
      puVar2 = (undefined4 *)(PTR_DAT_8c63a8fc + 0x20);
      *(undefined4 *)((int)param_1 + 0x1c) = *(undefined4 *)(PTR_DAT_8c63a8fc + 0x1c);
      *(undefined4 *)(param_1 + 3) = uVar14;
      *(undefined4 *)((int)param_1 + 0x14) = uVar10;
      *(undefined4 *)(param_1 + 2) = uVar12;
      *(undefined4 *)((int)param_1 + 0xc) = uVar7;
      *(undefined4 *)(param_1 + 1) = uVar9;
      *(undefined4 *)((int)param_1 + 4) = uVar4;
      *(undefined4 *)param_1 = uVar6;
      uVar6 = *puVar2;
      uVar4 = *(undefined4 *)(puVar1 + 0x24);
      uVar9 = *(undefined4 *)(puVar1 + 0x28);
      uVar7 = *(undefined4 *)(puVar1 + 0x2c);
      uVar12 = *(undefined4 *)(puVar1 + 0x30);
      uVar10 = *(undefined4 *)(puVar1 + 0x34);
      uVar14 = *(undefined4 *)(puVar1 + 0x38);
      *(undefined4 *)((int)param_1 + 0x3c) = *(undefined4 *)(puVar1 + 0x3c);
      *(undefined4 *)(param_1 + 7) = uVar14;
      *(undefined4 *)((int)param_1 + 0x34) = uVar10;
      *(undefined4 *)(param_1 + 6) = uVar12;
      *(undefined4 *)((int)param_1 + 0x2c) = uVar7;
      *(undefined4 *)(param_1 + 5) = uVar9;
      *(undefined4 *)((int)param_1 + 0x24) = uVar4;
      *(undefined4 *)(param_1 + 4) = uVar6;
    }
    else {
      uVar5 = *(undefined8 *)PTR_DAT_8c63a8fc;
      uVar15 = *(undefined8 *)(PTR_DAT_8c63a8fc + 8);
      uVar8 = *(undefined8 *)(PTR_DAT_8c63a8fc + 0x10);
      uVar16 = *(undefined8 *)(PTR_DAT_8c63a8fc + 0x18);
      uVar11 = *(undefined8 *)(PTR_DAT_8c63a8fc + 0x20);
      uVar17 = *(undefined8 *)(PTR_DAT_8c63a8fc + 0x28);
      uVar13 = *(undefined8 *)(PTR_DAT_8c63a8fc + 0x30);
      puVar3 = (undefined8 *)(PTR_DAT_8c63a8fc + 0x40);
      param_1[3] = *(undefined8 *)(PTR_DAT_8c63a8fc + 0x38);
      param_1[2] = uVar13;
      param_1[1] = uVar17;
      *param_1 = uVar11;
      param_1[-1] = uVar16;
      param_1[-2] = uVar8;
      param_1[-3] = uVar15;
      param_1[-4] = uVar5;
      uVar5 = *puVar3;
      uVar15 = *(undefined8 *)(puVar1 + 0x48);
      uVar8 = *(undefined8 *)(puVar1 + 0x50);
      uVar16 = *(undefined8 *)(puVar1 + 0x58);
      uVar11 = *(undefined8 *)(puVar1 + 0x60);
      uVar17 = *(undefined8 *)(puVar1 + 0x68);
      uVar13 = *(undefined8 *)(puVar1 + 0x70);
      param_1[3] = *(undefined8 *)(puVar1 + 0x78);
      param_1[2] = uVar13;
      param_1[1] = uVar17;
      *param_1 = uVar11;
      param_1[-1] = uVar16;
      param_1[-2] = uVar8;
      param_1[-3] = uVar15;
      param_1[-4] = uVar5;
    }
    return;
  }
  return;
}

