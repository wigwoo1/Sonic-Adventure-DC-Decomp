/* Ghidra draft only; not buildable/verified source. */

uint FUN_8c645de0(int param_1,undefined4 param_2)

{
  uint uVar1;
  uint uVar2;
  uint uVar3;
  uint uVar4;
  uint uVar5;
  undefined *puVar6;
  undefined *puVar7;
  undefined *puVar8;
  undefined *puVar9;
  undefined *puVar10;
  int iVar11;
  int iVar12;
  undefined4 *puVar13;
  int iVar14;
  int iVar15;
  undefined4 *puVar16;
  int in_VBR;
  byte in_MD;
  byte in_RB;
  byte in_BL;
  byte in_FD;
  byte in_M;
  byte in_Q;
  byte in_IMASK;
  byte in_S;
  byte in_T;
  
  puVar6 = PTR_PTR_8c645e64;
  uVar1 = (uint)in_T | (uint)in_S << 1;
  uVar2 = uVar1 | (uint)in_IMASK << 4;
  uVar3 = uVar2 | (uint)in_Q << 8;
  uVar4 = uVar3 | (uint)in_M << 9;
  uVar5 = (uint)in_BL << 0x1c | (uint)in_RB << 0x1d;
  *DAT_8c645e60 = *DAT_8c645e60 | (int)DAT_8c645e58;
  puVar10 = PTR_LAB_8c645e7c;
  puVar9 = PTR_DAT_8c645e78;
  puVar8 = PTR_DAT_8c645e74;
  puVar7 = PTR_LAB_8c645e6c;
  if (param_1 == DAT_8c645e68) {
    *(undefined **)PTR_PTR_8c645e70 = PTR_LAB_8c645e6c;
    *(undefined **)puVar6 = puVar7;
    *(undefined4 *)puVar8 = 0;
    *(undefined4 *)puVar9 = param_2;
  }
  else {
    *(undefined **)PTR_PTR_8c645e70 = PTR_LAB_8c645e7c;
    *(undefined **)puVar6 = puVar10;
    *(undefined4 *)puVar8 = 0;
    *(undefined4 *)puVar9 = 0;
  }
  puVar16 = (undefined4 *)(in_VBR + DAT_8c645e5a);
  for (puVar13 = (undefined4 *)PTR_DAT_8c645e80; puVar13 <= PTR_DAT_8c645e84; puVar13 = puVar13 + 1)
  {
    *puVar16 = *puVar13;
    puVar16 = puVar16 + 1;
  }
  iVar12 = in_VBR + DAT_8c645e5e;
  iVar14 = ((uint)((int)puVar16 + (DAT_8c645e5c - in_VBR)) >> 5) + 1;
  do {
    CacheBlockPurge(iVar12);
    iVar14 = iVar14 + -1;
    iVar12 = iVar12 + 0x20;
  } while (iVar14 != 0);
  puVar16 = (undefined4 *)(in_VBR + DAT_8c645ec0);
  for (puVar13 = (undefined4 *)PTR_DAT_8c645ec8; puVar13 <= PTR_DAT_8c645ecc; puVar13 = puVar13 + 1)
  {
    *puVar16 = *puVar13;
    puVar16 = puVar16 + 1;
  }
  iVar12 = in_VBR + DAT_8c645ec4;
  iVar14 = ((uint)((int)puVar16 + (DAT_8c645ec2 - in_VBR)) >> 5) + 1;
  do {
    CacheBlockPurge(iVar12);
    iVar14 = iVar14 + -1;
    iVar12 = iVar12 + 0x20;
  } while (iVar14 != 0);
  puVar16 = (undefined4 *)(in_VBR + DAT_8c645f08);
  for (puVar13 = (undefined4 *)PTR_DAT_8c645f10; puVar6 = PTR_LAB_8c645f64,
      puVar13 <= PTR_LAB_8c645f14; puVar13 = puVar13 + 1) {
    *puVar16 = *puVar13;
    puVar16 = puVar16 + 1;
  }
  iVar12 = in_VBR + DAT_8c645f0c;
  iVar14 = ((uint)((int)puVar16 + (DAT_8c645f0a - in_VBR)) >> 5) + 1;
  do {
    CacheBlockPurge(iVar12);
    iVar14 = iVar14 + -1;
    iVar12 = iVar12 + 0x20;
  } while (iVar14 != 0);
  iVar14 = (int)DAT_8c645f60;
  iVar12 = (int)DAT_8c645f62;
  iVar15 = (int)DAT_8c645f5e;
  do {
    iVar11 = iVar12 >> 3;
    iVar12 = iVar12 + 0x20;
    *(undefined **)(in_VBR + iVar11 + iVar14) = puVar6;
  } while (iVar15 > iVar12);
  return ((uint)(iVar15 <= iVar12) | uVar1 & 2 | uVar2 & 0xf0 | uVar3 & 0x100 | uVar4 & 0x200 |
          (uVar4 | (uint)in_FD << 0xf) & 0x8000 | (uint)in_BL << 0x1c & 0x10000000 |
          uVar5 & 0x20000000 |
         (uint)((((byte)(uVar5 >> 0x18) | (byte)(((uint)in_MD << 0x1e) >> 0x18)) & 0x40) >> 6) <<
         0x1e) & DAT_8c645f74;
}

