/* Ghidra draft only; not buildable/verified source. */

void FUN_8c050be4(void)

{
  int *piVar1;
  undefined *puVar2;
  undefined2 uVar3;
  int iVar4;
  undefined4 uVar5;
  undefined4 *puVar6;
  undefined4 *puVar7;
  byte bVar8;
  byte bVar9;
  undefined4 *puVar10;
  int *piVar11;
  uint uVar12;
  char *pcVar13;
  uint uVar14;
  bool bVar15;
  
  puVar10 = DAT_8c050d14;
  puVar2 = PTR_DAT_8c050d10;
  piVar1 = DAT_8c050d0c;
  DAT_8c050d0c[3] = (int)PTR_DAT_8c050d10;
  piVar1[2] = (int)puVar2;
  piVar1[1] = (int)puVar2;
  *piVar1 = (int)puVar2;
  puVar10[3] = 0xffffffff;
  puVar10[2] = 0xffffffff;
  puVar10[1] = 0xffffffff;
  *puVar10 = 0xffffffff;
  iVar4 = (*(code *)PTR_thunk_FUN_8c601f5a_8c050d18)(0,4);
  uVar12 = DAT_8c050d1c;
  *piVar1 = iVar4;
  bVar15 = (*(uint *)(*piVar1 + 4) & uVar12) != uVar12;
  if (bVar15) {
    *DAT_8c050d20 = 0;
  }
  else {
    *DAT_8c050d20 = 1;
    *puVar10 = 0;
  }
  uVar14 = (uint)!bVar15;
  iVar4 = (*(code *)PTR_thunk_FUN_8c601f5a_8c050d18)(6);
  puVar6 = DAT_8c050d24;
  piVar1[uVar14] = iVar4;
  if ((*(uint *)(piVar1[uVar14] + 4) & uVar12) == uVar12) {
    *puVar6 = 1;
    puVar10[uVar14] = 1;
    uVar14 = uVar14 + 1;
  }
  else {
    *puVar6 = 0;
  }
  iVar4 = (*(code *)PTR_thunk_FUN_8c601f5a_8c050d18)(0xc);
  puVar6 = DAT_8c050d28;
  piVar1[uVar14] = iVar4;
  if ((*(uint *)(piVar1[uVar14] + 4) & uVar12) == uVar12) {
    *puVar6 = 1;
    puVar10[uVar14] = 2;
    uVar14 = uVar14 + 1;
  }
  else {
    *puVar6 = 0;
  }
  iVar4 = (*(code *)PTR_thunk_FUN_8c601f5a_8c050d18)(0x12);
  puVar6 = DAT_8c050d2c;
  piVar1[uVar14] = iVar4;
  if ((*(uint *)(piVar1[uVar14] + 4) & uVar12) == uVar12) {
    *puVar6 = 1;
    puVar10[uVar14] = 3;
    uVar14 = uVar14 + 1;
  }
  else {
    *puVar6 = 0;
  }
  if (uVar14 == 0) {
    *piVar1 = (int)PTR_DAT_8c050ee0;
  }
  (*(code *)PTR_FUN_8c050ee4)();
  puVar2 = PTR_DAT_8c050ef4;
  puVar10 = DAT_8c050ee8;
  uVar3 = (undefined2)DAT_8c050eec;
  if (*PTR_DAT_8c050ef0 == '\x01') {
    piVar11 = piVar1 + 1;
    bVar8 = 0;
    pcVar13 = PTR_DAT_8c050ef4 + 1;
    puVar6 = DAT_8c050ee8 + 0xd;
    do {
      if ((bVar8 == 0) && (*puVar2 == '\x01')) {
        puVar7 = (undefined4 *)*piVar1;
        uVar5 = puVar7[1];
        *puVar10 = *puVar7;
        puVar10[1] = uVar5;
        uVar5 = puVar7[3];
        puVar10[2] = puVar7[2];
        puVar10[3] = uVar5;
        uVar5 = puVar7[5];
        puVar10[4] = puVar7[4];
        puVar10[5] = uVar5;
        uVar5 = puVar7[7];
        puVar10[6] = puVar7[6];
        puVar10[7] = uVar5;
        uVar5 = puVar7[9];
        puVar10[8] = puVar7[8];
        puVar10[9] = uVar5;
        uVar5 = puVar7[0xb];
        puVar10[10] = puVar7[10];
        puVar10[0xb] = uVar5;
        puVar10[0xc] = puVar7[0xc];
      }
      else if ((bVar8 == 1) && (*pcVar13 == '\x01')) {
        puVar7 = (undefined4 *)*piVar11;
        uVar5 = puVar7[1];
        *puVar6 = *puVar7;
        puVar10[0xe] = uVar5;
        uVar5 = puVar7[3];
        puVar10[0xf] = puVar7[2];
        puVar10[0x10] = uVar5;
        uVar5 = puVar7[5];
        puVar10[0x11] = puVar7[4];
        puVar10[0x12] = uVar5;
        uVar5 = puVar7[7];
        puVar10[0x13] = puVar7[6];
        puVar10[0x14] = uVar5;
        uVar5 = puVar7[9];
        puVar10[0x15] = puVar7[8];
        puVar10[0x16] = uVar5;
        uVar5 = puVar7[0xb];
        puVar10[0x17] = puVar7[10];
        puVar10[0x18] = uVar5;
        puVar10[0x19] = puVar7[0xc];
      }
      else {
        puVar10[(uint)bVar8 * 0xd + 0xb] = puVar10[(uint)bVar8 * 0xd + 2];
        puVar10[(uint)bVar8 * 0xd + 2] = 0;
        puVar10[(uint)bVar8 * 0xd + 3] = 0xffffffff;
        puVar10[(uint)bVar8 * 0xd + 4] = 0;
        puVar10[(uint)bVar8 * 0xd + 5] = 0;
        *(undefined2 *)(puVar10 + (uint)bVar8 * 0xd + 7) = 0;
        *(undefined2 *)((int)puVar10 + (uint)bVar8 * 0x34 + 0x1e) = 0;
        *(undefined2 *)((int)puVar10 + (uint)bVar8 * 0x34 + 0x1a) = uVar3;
        *(undefined2 *)(puVar10 + (uint)bVar8 * 0xd + 6) = uVar3;
        *(undefined2 *)(puVar10 + (uint)bVar8 * 0xd + 8) = 0x8000;
        *(undefined2 *)((int)puVar10 + (uint)bVar8 * 0x34 + 0x22) = 0x8000;
      }
      bVar9 = bVar8 + 1;
      if ((bVar9 == 0) && (*puVar2 == '\x01')) {
        puVar7 = (undefined4 *)*piVar1;
        uVar5 = puVar7[1];
        *puVar10 = *puVar7;
        puVar10[1] = uVar5;
        uVar5 = puVar7[3];
        puVar10[2] = puVar7[2];
        puVar10[3] = uVar5;
        uVar5 = puVar7[5];
        puVar10[4] = puVar7[4];
        puVar10[5] = uVar5;
        uVar5 = puVar7[7];
        puVar10[6] = puVar7[6];
        puVar10[7] = uVar5;
        uVar5 = puVar7[9];
        puVar10[8] = puVar7[8];
        puVar10[9] = uVar5;
        uVar5 = puVar7[0xb];
        puVar10[10] = puVar7[10];
        puVar10[0xb] = uVar5;
        puVar10[0xc] = puVar7[0xc];
      }
      else if ((bVar9 == 1) && (*pcVar13 == '\x01')) {
        puVar7 = (undefined4 *)*piVar11;
        uVar5 = puVar7[1];
        *puVar6 = *puVar7;
        puVar10[0xe] = uVar5;
        uVar5 = puVar7[3];
        puVar10[0xf] = puVar7[2];
        puVar10[0x10] = uVar5;
        uVar5 = puVar7[5];
        puVar10[0x11] = puVar7[4];
        puVar10[0x12] = uVar5;
        uVar5 = puVar7[7];
        puVar10[0x13] = puVar7[6];
        puVar10[0x14] = uVar5;
        uVar5 = puVar7[9];
        puVar10[0x15] = puVar7[8];
        puVar10[0x16] = uVar5;
        uVar5 = puVar7[0xb];
        puVar10[0x17] = puVar7[10];
        puVar10[0x18] = uVar5;
        puVar10[0x19] = puVar7[0xc];
      }
      else {
        puVar10[(uint)bVar9 * 0xd + 0xb] = puVar10[(uint)bVar9 * 0xd + 2];
        puVar10[(uint)bVar9 * 0xd + 2] = 0;
        puVar10[(uint)bVar9 * 0xd + 3] = 0xffffffff;
        puVar10[(uint)bVar9 * 0xd + 4] = 0;
        puVar10[(uint)bVar9 * 0xd + 5] = 0;
        *(undefined2 *)(puVar10 + (uint)bVar9 * 0xd + 7) = 0;
        *(undefined2 *)((int)puVar10 + (uint)bVar9 * 0x34 + 0x1e) = 0;
        *(undefined2 *)((int)puVar10 + (uint)bVar9 * 0x34 + 0x1a) = uVar3;
        *(undefined2 *)(puVar10 + (uint)bVar9 * 0xd + 6) = uVar3;
        *(undefined2 *)(puVar10 + (uint)bVar9 * 0xd + 8) = 0x8000;
        *(undefined2 *)((int)puVar10 + (uint)bVar9 * 0x34 + 0x22) = 0x8000;
      }
      bVar9 = bVar8 + 2;
      if ((bVar9 == 0) && (*puVar2 == '\x01')) {
        puVar7 = (undefined4 *)*piVar1;
        uVar5 = puVar7[1];
        *puVar10 = *puVar7;
        puVar10[1] = uVar5;
        uVar5 = puVar7[3];
        puVar10[2] = puVar7[2];
        puVar10[3] = uVar5;
        uVar5 = puVar7[5];
        puVar10[4] = puVar7[4];
        puVar10[5] = uVar5;
        uVar5 = puVar7[7];
        puVar10[6] = puVar7[6];
        puVar10[7] = uVar5;
        uVar5 = puVar7[9];
        puVar10[8] = puVar7[8];
        puVar10[9] = uVar5;
        uVar5 = puVar7[0xb];
        puVar10[10] = puVar7[10];
        puVar10[0xb] = uVar5;
        puVar10[0xc] = puVar7[0xc];
      }
      else if ((bVar9 == 1) && (*pcVar13 == '\x01')) {
        puVar7 = (undefined4 *)*piVar11;
        uVar5 = puVar7[1];
        *puVar6 = *puVar7;
        puVar10[0xe] = uVar5;
        uVar5 = puVar7[3];
        puVar10[0xf] = puVar7[2];
        puVar10[0x10] = uVar5;
        uVar5 = puVar7[5];
        puVar10[0x11] = puVar7[4];
        puVar10[0x12] = uVar5;
        uVar5 = puVar7[7];
        puVar10[0x13] = puVar7[6];
        puVar10[0x14] = uVar5;
        uVar5 = puVar7[9];
        puVar10[0x15] = puVar7[8];
        puVar10[0x16] = uVar5;
        uVar5 = puVar7[0xb];
        puVar10[0x17] = puVar7[10];
        puVar10[0x18] = uVar5;
        puVar10[0x19] = puVar7[0xc];
      }
      else {
        puVar10[(uint)bVar9 * 0xd + 0xb] = puVar10[(uint)bVar9 * 0xd + 2];
        puVar10[(uint)bVar9 * 0xd + 2] = 0;
        puVar10[(uint)bVar9 * 0xd + 3] = 0xffffffff;
        puVar10[(uint)bVar9 * 0xd + 4] = 0;
        puVar10[(uint)bVar9 * 0xd + 5] = 0;
        *(undefined2 *)(puVar10 + (uint)bVar9 * 0xd + 7) = 0;
        *(undefined2 *)((int)puVar10 + (uint)bVar9 * 0x34 + 0x1e) = 0;
        *(undefined2 *)((int)puVar10 + (uint)bVar9 * 0x34 + 0x1a) = uVar3;
        *(undefined2 *)(puVar10 + (uint)bVar9 * 0xd + 6) = uVar3;
        *(undefined2 *)(puVar10 + (uint)bVar9 * 0xd + 8) = 0x8000;
        *(undefined2 *)((int)puVar10 + (uint)bVar9 * 0x34 + 0x22) = 0x8000;
      }
      bVar9 = bVar8 + 3;
      if ((bVar9 == 0) && (*puVar2 == '\x01')) {
        puVar7 = (undefined4 *)*piVar1;
        uVar5 = puVar7[1];
        *puVar10 = *puVar7;
        puVar10[1] = uVar5;
        uVar5 = puVar7[3];
        puVar10[2] = puVar7[2];
        puVar10[3] = uVar5;
        uVar5 = puVar7[5];
        puVar10[4] = puVar7[4];
        puVar10[5] = uVar5;
        uVar5 = puVar7[7];
        puVar10[6] = puVar7[6];
        puVar10[7] = uVar5;
        uVar5 = puVar7[9];
        puVar10[8] = puVar7[8];
        puVar10[9] = uVar5;
        uVar5 = puVar7[0xb];
        puVar10[10] = puVar7[10];
        puVar10[0xb] = uVar5;
        puVar10[0xc] = puVar7[0xc];
      }
      else if ((bVar9 == 1) && (*pcVar13 == '\x01')) {
        puVar7 = (undefined4 *)*piVar11;
        uVar5 = puVar7[1];
        *puVar6 = *puVar7;
        puVar10[0xe] = uVar5;
        uVar5 = puVar7[3];
        puVar10[0xf] = puVar7[2];
        puVar10[0x10] = uVar5;
        uVar5 = puVar7[5];
        puVar10[0x11] = puVar7[4];
        puVar10[0x12] = uVar5;
        uVar5 = puVar7[7];
        puVar10[0x13] = puVar7[6];
        puVar10[0x14] = uVar5;
        uVar5 = puVar7[9];
        puVar10[0x15] = puVar7[8];
        puVar10[0x16] = uVar5;
        uVar5 = puVar7[0xb];
        puVar10[0x17] = puVar7[10];
        puVar10[0x18] = uVar5;
        puVar10[0x19] = puVar7[0xc];
      }
      else {
        puVar10[(uint)bVar9 * 0xd + 0xb] = puVar10[(uint)bVar9 * 0xd + 2];
        puVar10[(uint)bVar9 * 0xd + 2] = 0;
        puVar10[(uint)bVar9 * 0xd + 3] = 0xffffffff;
        puVar10[(uint)bVar9 * 0xd + 4] = 0;
        puVar10[(uint)bVar9 * 0xd + 5] = 0;
        *(undefined2 *)(puVar10 + (uint)bVar9 * 0xd + 7) = 0;
        *(undefined2 *)((int)puVar10 + (uint)bVar9 * 0x34 + 0x1e) = 0;
        *(undefined2 *)((int)puVar10 + (uint)bVar9 * 0x34 + 0x1a) = uVar3;
        *(undefined2 *)(puVar10 + (uint)bVar9 * 0xd + 6) = uVar3;
        *(undefined2 *)(puVar10 + (uint)bVar9 * 0xd + 8) = 0x8000;
        *(undefined2 *)((int)puVar10 + (uint)bVar9 * 0x34 + 0x22) = 0x8000;
      }
      bVar8 = bVar8 + 4;
    } while (bVar8 < 8);
  }
  else {
    uVar12 = 0;
    do {
      puVar10[0xb] = puVar10[2];
      puVar10[2] = 0;
      puVar10[3] = 0xffffffff;
      puVar10[4] = 0;
      puVar10[5] = 0;
      *(undefined2 *)(puVar10 + 7) = 0;
      *(undefined2 *)((int)puVar10 + 0x1e) = 0;
      *(undefined2 *)((int)puVar10 + 0x1a) = uVar3;
      *(undefined2 *)(puVar10 + 6) = uVar3;
      *(undefined2 *)(puVar10 + 8) = 0x8000;
      *(undefined2 *)((int)puVar10 + 0x22) = 0x8000;
      puVar10[0x18] = puVar10[0xf];
      puVar10[0xf] = 0;
      puVar10[0x10] = 0xffffffff;
      puVar10[0x11] = 0;
      puVar10[0x12] = 0;
      *(undefined2 *)(puVar10 + 0x14) = 0;
      *(undefined2 *)((int)puVar10 + 0x52) = 0;
      *(undefined2 *)((int)puVar10 + 0x4e) = uVar3;
      *(undefined2 *)(puVar10 + 0x13) = uVar3;
      *(undefined2 *)(puVar10 + 0x15) = 0x8000;
      *(undefined2 *)((int)puVar10 + 0x56) = 0x8000;
      puVar10[0x25] = puVar10[0x1c];
      puVar10[0x1c] = 0;
      puVar10[0x1d] = 0xffffffff;
      puVar10[0x1e] = 0;
      puVar10[0x1f] = 0;
      *(undefined2 *)(puVar10 + 0x21) = 0;
      *(undefined2 *)((int)puVar10 + 0x86) = 0;
      *(undefined2 *)((int)puVar10 + 0x82) = uVar3;
      *(undefined2 *)(puVar10 + 0x20) = uVar3;
      *(undefined2 *)(puVar10 + 0x22) = 0x8000;
      *(undefined2 *)((int)puVar10 + 0x8a) = 0x8000;
      puVar10[0x32] = puVar10[0x29];
      puVar10[0x29] = 0;
      puVar10[0x2a] = 0xffffffff;
      puVar10[0x2b] = 0;
      puVar10[0x2c] = 0;
      *(undefined2 *)(puVar10 + 0x2e) = 0;
      *(undefined2 *)((int)puVar10 + 0xba) = 0;
      *(undefined2 *)((int)puVar10 + 0xb6) = uVar3;
      *(undefined2 *)(puVar10 + 0x2d) = uVar3;
      uVar12 = uVar12 + 4;
      *(undefined2 *)(puVar10 + 0x2f) = 0x8000;
      *(undefined2 *)((int)puVar10 + 0xbe) = 0x8000;
      puVar10 = puVar10 + 0x34;
    } while ((uVar12 & 0xff) < 8);
  }
  FUN_8c050a40();
  FUN_8c05117e();
  iVar4 = 0;
  do {
    FUN_8c050ace(iVar4);
    FUN_8c050ace(iVar4 + 1);
    iVar4 = iVar4 + 2;
  } while (iVar4 < 4);
  return;
}

