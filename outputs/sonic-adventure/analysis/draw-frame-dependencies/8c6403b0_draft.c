/* Ghidra draft only; not buildable/verified source. */

/* WARNING: Removing unreachable block (ram,0x8c6403b8) */

void FUN_8c6403b0(undefined4 param_1,undefined4 param_2,undefined4 param_3,undefined4 param_4,
                 int param_5)

{
  undefined8 *puVar1;
  undefined8 *puVar2;
  undefined8 uVar3;
  undefined4 in_fr3;
  undefined8 uVar5;
  undefined8 uVar7;
  undefined8 uVar9;
  undefined8 uVar11;
  undefined8 uVar13;
  undefined8 in_xd0;
  undefined8 in_xd2;
  undefined8 in_xd4;
  undefined8 in_xd6;
  undefined8 in_xd8;
  undefined8 in_xd10;
  char in_FPSCR_SZ;
  undefined4 uVar4;
  undefined4 uVar6;
  undefined4 uVar8;
  undefined4 uVar10;
  undefined4 uVar12;
  undefined4 uVar14;
  
  puVar1 = DAT_8c640414;
  if (in_FPSCR_SZ == '\0') {
    *(undefined4 *)(param_5 + 0x74) = 0;
    *(undefined4 *)(param_5 + 0x70) = 0xbf800000;
    *(undefined4 *)(param_5 + 0x6c) = 0;
    *(undefined4 *)(param_5 + 0x68) = 0;
    *(undefined4 *)(param_5 + 100) = 0;
    *(undefined4 *)(param_5 + 0x60) = 0;
    *(undefined4 *)(param_5 + 0x5c) = 0;
    *(undefined4 *)(param_5 + 0x58) = 0;
    *(undefined4 *)(param_5 + 0x54) = 0xbf800000;
    *(undefined4 *)(param_5 + 0x50) = 0;
    *(undefined4 *)(param_5 + 0x4c) = 0;
    *(undefined4 *)(param_5 + 0x48) = 0;
    *(undefined4 *)(param_5 + 0x44) = 0;
    puVar2 = (undefined8 *)(param_5 + 0x40);
    *(undefined4 *)puVar2 = 0;
  }
  else {
    *(undefined8 *)(param_5 + 0x70) = 0xbf800000;
    *(undefined8 *)(param_5 + 0x68) = in_xd0;
    *(undefined8 *)(param_5 + 0x60) = 0xbf800000;
    *(undefined8 *)(param_5 + 0x58) = 0xbf800000;
    *(undefined8 *)(param_5 + 0x50) = 0xbf800000;
    *(undefined8 *)(param_5 + 0x48) = 0xbf800000;
    *(undefined8 *)(param_5 + 0x40) = 0xbf800000;
    *(undefined8 *)(param_5 + 0x38) = 0xbf800000;
    *(undefined8 *)(param_5 + 0x30) = in_xd0;
    *(undefined8 *)(param_5 + 0x28) = 0xbf800000;
    *(undefined8 *)(param_5 + 0x20) = 0xbf800000;
    *(undefined8 *)(param_5 + 0x18) = 0xbf800000;
    *(undefined8 *)(param_5 + 0x10) = 0xbf800000;
    puVar2 = (undefined8 *)(param_5 + 8);
    *puVar2 = 0xbf800000;
  }
  if (in_FPSCR_SZ == '\0') {
    uVar4 = *(undefined4 *)(puVar1 + 4);
    uVar6 = *(undefined4 *)((int)puVar1 + 0x24);
    uVar8 = *(undefined4 *)(puVar1 + 5);
    uVar10 = *(undefined4 *)((int)puVar1 + 0x2c);
    uVar12 = *(undefined4 *)puVar1;
    uVar14 = *(undefined4 *)((int)puVar1 + 4);
    *(undefined4 *)((int)puVar2 + -4) = param_2;
    *(undefined4 *)(puVar2 + -1) = uVar10;
    *(undefined4 *)((int)puVar2 + -0xc) = param_1;
    *(undefined4 *)(puVar2 + -2) = uVar8;
    *(undefined4 *)((int)puVar2 + -0x14) = in_fr3;
    *(undefined4 *)(puVar2 + -3) = uVar6;
    *(undefined4 *)((int)puVar2 + -0x1c) = 0xbf800000;
    puVar2 = puVar2 + -4;
    *(undefined4 *)puVar2 = uVar4;
    uVar6 = *(undefined4 *)(puVar1 + 1);
    uVar4 = *(undefined4 *)((int)(puVar1 + 1) + 4);
    *(undefined4 *)((int)puVar2 + -4) = param_1;
    *(undefined4 *)(puVar2 + -1) = uVar4;
    *(undefined4 *)((int)puVar2 + -0xc) = param_2;
    *(undefined4 *)(puVar2 + -2) = uVar6;
    *(undefined4 *)((int)puVar2 + -0x14) = param_4;
    *(undefined4 *)(puVar2 + -3) = uVar14;
    *(undefined4 *)((int)puVar2 + -0x1c) = param_3;
    *(undefined4 *)(puVar2 + -4) = uVar12;
  }
  else {
    uVar3 = puVar1[4];
    uVar5 = puVar1[5];
    uVar7 = puVar1[6];
    uVar9 = puVar1[7];
    uVar11 = *puVar1;
    uVar13 = puVar1[1];
    puVar2[-1] = in_xd6;
    puVar2[-2] = uVar9;
    puVar2[-3] = in_xd4;
    puVar2[-4] = uVar7;
    puVar2[-5] = in_xd2;
    puVar2[-6] = uVar5;
    puVar2[-7] = in_xd0;
    puVar2 = puVar2 + -8;
    *puVar2 = uVar3;
    uVar5 = puVar1[2];
    uVar3 = (puVar1 + 2)[1];
    puVar2[-1] = in_xd4;
    puVar2[-2] = uVar3;
    puVar2[-3] = in_xd6;
    puVar2[-4] = uVar5;
    puVar2[-5] = in_xd10;
    puVar2[-6] = uVar13;
    puVar2[-7] = in_xd8;
    puVar2[-8] = uVar11;
  }
  return;
}

