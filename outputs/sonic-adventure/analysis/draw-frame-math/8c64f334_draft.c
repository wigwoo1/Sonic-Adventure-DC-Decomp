/* Ghidra draft only; not buildable/verified source. */

void FUN_8c64f334(void)

{
  undefined8 *puVar1;
  undefined *puVar2;
  undefined *puVar3;
  undefined4 *puVar4;
  undefined8 *puVar5;
  undefined8 uVar6;
  undefined4 uVar7;
  undefined8 uVar8;
  undefined4 uVar9;
  undefined8 uVar10;
  undefined4 uVar11;
  undefined4 uVar12;
  undefined4 uVar13;
  undefined4 uVar14;
  undefined4 uVar15;
  undefined8 uVar16;
  undefined8 uVar17;
  undefined8 uVar18;
  undefined8 uVar19;
  char in_FPSCR_SZ;
  
  puVar3 = PTR_DAT_8c64f3d0;
  puVar4 = (undefined4 *)*DAT_8c64f3cc;
  *DAT_8c64f3c8 = 1;
  if (in_FPSCR_SZ == '\0') {
    uVar6 = CONCAT44(*(undefined4 *)(puVar3 + 0x20),*puVar4);
    uVar8 = CONCAT44(*(undefined4 *)(puVar3 + 0x24),puVar4[1]);
    uVar10 = CONCAT44(*(undefined4 *)(puVar3 + 0x28),puVar4[2]);
    puVar2 = puVar3 + 0x30;
    puVar5 = (undefined8 *)(puVar4 + 7);
    *(undefined4 *)puVar5 = *(undefined4 *)(puVar3 + 0x2c);
  }
  else {
    uVar6 = *(undefined8 *)(puVar3 + 0x20);
    uVar8 = *(undefined8 *)(puVar3 + 0x28);
    uVar10 = *(undefined8 *)(puVar3 + 0x30);
    puVar2 = puVar3 + 0x40;
    puVar5 = (undefined8 *)(puVar4 + 0xe);
    *puVar5 = *(undefined8 *)(puVar3 + 0x38);
  }
  if (in_FPSCR_SZ == '\0') {
    *(int *)((int)puVar5 + -4) = (int)((ulonglong)uVar10 >> 0x20);
    *(int *)(puVar5 + -1) = (int)((ulonglong)uVar8 >> 0x20);
    *(int *)((int)puVar5 + -0xc) = (int)((ulonglong)uVar6 >> 0x20);
    uVar7 = *(undefined4 *)(puVar2 + -0x40);
    uVar9 = *(undefined4 *)(puVar2 + -0x3c);
    uVar11 = *(undefined4 *)(puVar2 + -0x38);
    puVar3 = puVar2 + -0x30;
    *(undefined4 *)(puVar5 + -2) = *(undefined4 *)(puVar2 + -0x34);
    *(undefined4 *)((int)puVar5 + -0x14) = uVar11;
    *(undefined4 *)(puVar5 + -3) = uVar9;
    puVar5 = (undefined8 *)((int)puVar5 + -0x1c);
    *(undefined4 *)puVar5 = uVar7;
  }
  else {
    puVar5[-1] = uVar10;
    puVar5[-2] = uVar8;
    puVar5[-3] = uVar6;
    uVar10 = *(undefined8 *)(puVar2 + -0x40);
    uVar8 = *(undefined8 *)(puVar2 + -0x38);
    uVar6 = *(undefined8 *)(puVar2 + -0x30);
    puVar3 = puVar2 + -0x20;
    puVar5[-4] = *(undefined8 *)(puVar2 + -0x28);
    puVar5[-5] = uVar6;
    puVar5[-6] = uVar8;
    puVar5 = puVar5 + -7;
    *puVar5 = uVar10;
  }
  *DAT_8c64f3d4 = (int)puVar5;
  (*(code *)PTR_FUN_8c64f3d8)(puVar5,puVar3);
  (*(code *)PTR_FUN_8c64f3e0)(puVar5,*DAT_8c64f3dc);
  puVar1 = DAT_8c64f3e4;
  if (in_FPSCR_SZ == '\0') {
    uVar7 = *(undefined4 *)puVar5;
    uVar9 = *(undefined4 *)((int)puVar5 + 4);
    uVar11 = *(undefined4 *)(puVar5 + 1);
    uVar12 = *(undefined4 *)((int)puVar5 + 0xc);
    uVar13 = *(undefined4 *)(puVar5 + 2);
    uVar14 = *(undefined4 *)((int)puVar5 + 0x14);
    uVar15 = *(undefined4 *)(puVar5 + 3);
    *(undefined4 *)((int)DAT_8c64f3e4 + 0x3c) = *(undefined4 *)((int)puVar5 + 0x1c);
    *(undefined4 *)(puVar1 + 7) = uVar15;
    *(undefined4 *)((int)puVar1 + 0x34) = uVar14;
    *(undefined4 *)(puVar1 + 6) = uVar13;
    *(undefined4 *)((int)puVar1 + 0x2c) = uVar12;
    *(undefined4 *)(puVar1 + 5) = uVar11;
    *(undefined4 *)((int)puVar1 + 0x24) = uVar9;
    *(undefined4 *)(puVar1 + 4) = uVar7;
  }
  else {
    uVar10 = *puVar5;
    uVar8 = puVar5[1];
    uVar6 = puVar5[2];
    uVar16 = puVar5[3];
    uVar17 = puVar5[4];
    uVar18 = puVar5[5];
    uVar19 = puVar5[6];
    DAT_8c64f3e4[7] = puVar5[7];
    puVar1[6] = uVar19;
    puVar1[5] = uVar18;
    puVar1[4] = uVar17;
    puVar1[3] = uVar16;
    puVar1[2] = uVar6;
    puVar1[1] = uVar8;
    *puVar1 = uVar10;
  }
  return;
}

