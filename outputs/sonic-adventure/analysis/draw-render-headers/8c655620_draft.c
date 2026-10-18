/* Ghidra draft only; not buildable/verified source. */

undefined4 FUN_8c655620(undefined4 param_1,int param_2)

{
  uint uVar1;
  uint *puVar2;
  undefined *puVar3;
  int *piVar4;
  uint *puVar5;
  int iVar6;
  uint *puVar7;
  uint uVar8;
  uint *puVar9;
  uint uVar10;
  float fVar11;
  double in_dr2;
  double dVar13;
  float fVar14;
  int iVar15;
  int iVar16;
  char in_FPSCR_PR;
  char in_FPSCR_SZ;
  double dVar12;
  
  uVar8 = DAT_8c655748;
  puVar2 = DAT_8c655744;
  iVar6 = (int)DAT_8c655740;
  *DAT_8c655744 = *(uint *)(iVar6 + param_2);
  puVar2[4] = *(uint *)(iVar6 + 4 + param_2);
  puVar2[5] = *(uint *)(iVar6 + 8 + param_2);
  if ((*puVar2 >> 0x18 & 7) == 0) {
    puVar2[5] = puVar2[5] & DAT_8c65574c | uVar8;
  }
  puVar2[6] = *(uint *)(DAT_8c655742 + param_2);
  if (in_FPSCR_SZ == '\0') {
    puVar2[8] = *(uint *)(param_2 + 0x70);
    puVar2[9] = *(uint *)(param_2 + 0x74);
    puVar2[10] = *(uint *)(param_2 + 0x78);
    puVar2[0xb] = *(uint *)(param_2 + 0x7c);
    puVar2[0xc] = *(uint *)(param_2 + 0x80);
    puVar2[0xd] = *(uint *)(param_2 + 0x84);
    puVar2[0xe] = *(uint *)(param_2 + 0x88);
    uVar1 = DAT_8c655754;
    in_dr2 = (double)CONCAT44((int)((ulonglong)in_dr2 >> 0x20),*(uint *)(param_2 + 0x8c));
    puVar2[0xf] = *(uint *)(param_2 + 0x8c);
  }
  else {
    *(undefined8 *)(puVar2 + 8) = *(undefined8 *)(param_2 + 0x70);
    *(undefined8 *)(puVar2 + 9) = *(undefined8 *)(param_2 + 0x74);
    *(undefined8 *)(puVar2 + 10) = *(undefined8 *)(param_2 + 0x78);
    *(undefined8 *)(puVar2 + 0xb) = *(undefined8 *)(param_2 + 0x7c);
    *(undefined8 *)(puVar2 + 0xc) = *(undefined8 *)(param_2 + 0x80);
    *(undefined8 *)(puVar2 + 0xd) = *(undefined8 *)(param_2 + 0x84);
    *(undefined8 *)(puVar2 + 0xe) = *(undefined8 *)(param_2 + 0x88);
    uVar1 = DAT_8c655754;
    *(undefined8 *)(puVar2 + 0xf) = *(undefined8 *)(param_2 + 0x8c);
  }
  puVar3 = PTR_DAT_8c655750;
  puVar2[0x10] = *(uint *)(param_2 + 0xa0);
  puVar5 = DAT_8c655760;
  uVar10 = *puVar2;
  *DAT_8c65575c =
       *(int *)(PTR_DAT_8c655758 +
               ((uVar8 & uVar10) >> 0x18 | (uVar1 & uVar10) >> 0x14 | (uVar10 & 0x40) >> 3 |
                (uVar10 & 0x20) >> 3 | (uVar10 & 0x10) >> 3 | (uVar10 & 4) >> 2) * 4);
  *(uint *)puVar3 = *puVar2;
  uVar8 = (*puVar2 & DAT_8c655764) >> 0x18;
  *puVar5 = uVar8;
  piVar4 = DAT_8c65575c;
  if ((uVar8 & 1) != 0) {
    *(uint *)(puVar3 + 4) = puVar2[0x10];
    puVar9 = (uint *)(puVar3 + 0x20);
    goto LAB_8c6558ca;
  }
  *(uint *)(puVar3 + 4) = puVar2[4];
  *(uint *)(puVar3 + 8) = puVar2[5];
  *(uint *)(puVar3 + 0xc) = puVar2[6];
  iVar6 = *piVar4;
  puVar9 = (uint *)(puVar3 + 0x10);
  if (iVar6 == 0) {
    puVar9 = (uint *)(puVar3 + 0x20);
    goto LAB_8c6558ca;
  }
  if (iVar6 == 1) {
    *puVar9 = puVar2[8];
    *(uint *)(puVar3 + 0x14) = puVar2[9];
    puVar9 = (uint *)(puVar3 + 0x18);
    *puVar9 = puVar2[10];
    puVar7 = puVar2 + 0xb;
LAB_8c6557d2:
    puVar9 = puVar9 + 1;
    *puVar9 = *puVar7;
  }
  else {
    if (iVar6 == 2) {
      *(uint *)(puVar3 + 0x20) = puVar2[8];
      *(uint *)(puVar3 + 0x24) = puVar2[9];
      *(uint *)(puVar3 + 0x28) = puVar2[10];
      *(uint *)(puVar3 + 0x2c) = puVar2[0xb];
      *(uint *)(puVar3 + 0x30) = puVar2[0xc];
      *(uint *)(puVar3 + 0x34) = puVar2[0xd];
      puVar9 = (uint *)(puVar3 + 0x38);
      puVar7 = puVar2 + 0xf;
      *puVar9 = puVar2[0xe];
      goto LAB_8c6557d2;
    }
    if (iVar6 == 3) {
      *puVar9 = puVar2[0x11];
      *(uint *)(puVar3 + 0x14) = puVar2[0x12];
      puVar9 = (uint *)(puVar3 + 0x20);
      goto LAB_8c6558ca;
    }
    if (iVar6 != 4) {
      if (iVar6 == 0x10) {
        if (in_FPSCR_SZ == '\0') {
          dVar13 = (double)CONCAT44((undefined4)DAT_8c6558e0,param_1);
          in_dr2 = (double)CONCAT44((int)((ulonglong)in_dr2 >> 0x20),puVar2[8]);
        }
        else {
          dVar13 = (double)CONCAT44(DAT_8c6558e0._4_4_,(undefined4)DAT_8c6558e0);
        }
        fVar14 = (float)((ulonglong)dVar13 >> 0x20);
        if (in_FPSCR_PR == '\0') {
          fVar11 = SUB84(in_dr2,0) * fVar14;
          dVar12 = (double)CONCAT44((int)((ulonglong)in_dr2 >> 0x20),fVar11);
          iVar6 = (int)fVar11;
        }
        else {
          dVar12 = in_dr2 * dVar13;
          iVar6 = (int)dVar12;
        }
        if (in_FPSCR_SZ == '\0') {
          dVar12 = (double)CONCAT44((int)((ulonglong)dVar12 >> 0x20),puVar2[9]);
        }
        if (in_FPSCR_PR == '\0') {
          fVar11 = SUB84(dVar12,0) * fVar14;
          dVar12 = (double)CONCAT44((int)((ulonglong)dVar12 >> 0x20),fVar11);
          iVar15 = (int)fVar11;
        }
        else {
          dVar12 = dVar12 * dVar13;
          iVar15 = (int)dVar12;
        }
        if (in_FPSCR_SZ == '\0') {
          dVar12 = (double)CONCAT44((int)((ulonglong)dVar12 >> 0x20),puVar2[10]);
        }
        if (in_FPSCR_PR == '\0') {
          fVar11 = SUB84(dVar12,0) * fVar14;
          dVar12 = (double)CONCAT44((int)((ulonglong)dVar12 >> 0x20),fVar11);
          iVar16 = (int)fVar11;
        }
        else {
          dVar12 = dVar12 * dVar13;
          iVar16 = (int)dVar12;
        }
        if (in_FPSCR_SZ == '\0') {
          dVar12 = (double)CONCAT44((int)((ulonglong)dVar12 >> 0x20),puVar2[0xb]);
        }
        if (in_FPSCR_PR == '\0') {
          fVar11 = SUB84(dVar12,0) * fVar14;
          dVar12 = (double)CONCAT44((int)((ulonglong)dVar12 >> 0x20),fVar11);
          uVar8 = (uint)fVar11;
        }
        else {
          dVar12 = dVar12 * dVar13;
          uVar8 = (uint)dVar12;
        }
        *puVar9 = iVar6 << 0x18 | iVar15 << 0x10 | iVar16 << 8 | uVar8;
        if (in_FPSCR_SZ == '\0') {
          dVar12 = (double)CONCAT44((int)((ulonglong)dVar12 >> 0x20),puVar2[0xc]);
        }
        if (in_FPSCR_PR == '\0') {
          fVar11 = SUB84(dVar12,0) * fVar14;
          dVar12 = (double)CONCAT44((int)((ulonglong)dVar12 >> 0x20),fVar11);
          iVar6 = (int)fVar11;
        }
        else {
          dVar12 = dVar12 * dVar13;
          iVar6 = (int)dVar12;
        }
        if (in_FPSCR_SZ == '\0') {
          dVar12 = (double)CONCAT44((int)((ulonglong)dVar12 >> 0x20),puVar2[0xd]);
        }
        if (in_FPSCR_PR == '\0') {
          fVar11 = SUB84(dVar12,0) * fVar14;
          dVar12 = (double)CONCAT44((int)((ulonglong)dVar12 >> 0x20),fVar11);
          iVar15 = (int)fVar11;
        }
        else {
          dVar12 = dVar12 * dVar13;
          iVar15 = (int)dVar12;
        }
        if (in_FPSCR_SZ == '\0') {
          dVar12 = (double)CONCAT44((int)((ulonglong)dVar12 >> 0x20),puVar2[0xe]);
        }
        if (in_FPSCR_PR == '\0') {
          fVar11 = SUB84(dVar12,0) * fVar14;
          dVar12 = (double)CONCAT44((int)((ulonglong)dVar12 >> 0x20),fVar11);
          iVar16 = (int)fVar11;
        }
        else {
          dVar12 = dVar12 * dVar13;
          iVar16 = (int)dVar12;
        }
        if (in_FPSCR_SZ == '\0') {
          dVar12 = (double)CONCAT44((int)((ulonglong)dVar12 >> 0x20),puVar2[0xf]);
        }
        if (in_FPSCR_PR == '\0') {
          uVar8 = (uint)(SUB84(dVar12,0) * fVar14);
        }
        else {
          uVar8 = (uint)(dVar12 * dVar13);
        }
        *(uint *)(puVar3 + 0x14) = iVar6 << 0x18 | iVar15 << 0x10 | iVar16 << 8 | uVar8;
        puVar9 = (uint *)(puVar3 + 0x20);
      }
      goto LAB_8c6558ca;
    }
    *puVar9 = puVar2[0x11];
    *(uint *)(puVar3 + 0x14) = puVar2[0x12];
    *(uint *)(puVar3 + 0x20) = puVar2[8];
    *(uint *)(puVar3 + 0x24) = puVar2[9];
    *(uint *)(puVar3 + 0x28) = puVar2[10];
    *(uint *)(puVar3 + 0x2c) = puVar2[0xb];
    *(uint *)(puVar3 + 0x30) = puVar2[0x14];
    *(uint *)(puVar3 + 0x34) = puVar2[0x15];
    *(uint *)(puVar3 + 0x38) = puVar2[0x16];
    puVar9 = (uint *)(puVar3 + 0x3c);
    *puVar9 = puVar2[0x17];
  }
  puVar9 = puVar9 + 1;
LAB_8c6558ca:
  puVar2 = DAT_8c6558e0._4_4_;
  *DAT_8c6558e0._4_4_ = *puVar5;
  puVar2[1] = (int)puVar9 - (int)puVar3;
  return 0;
}

