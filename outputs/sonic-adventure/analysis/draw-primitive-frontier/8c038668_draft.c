/* Ghidra draft only; not buildable/verified source. */

int FUN_8c038668(undefined4 param_1,float param_2,undefined4 param_3,undefined4 param_4)

{
  ushort uVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  uint uVar5;
  int in_r2;
  int extraout_r3;
  ushort *puVar6;
  int iVar7;
  undefined8 *puVar8;
  undefined8 *puVar9;
  undefined8 *puVar10;
  int unaff_r14;
  undefined4 uVar11;
  undefined4 uVar12;
  undefined8 uVar13;
  undefined8 uVar15;
  undefined4 uVar17;
  double in_dr12;
  float fVar18;
  float in_fr15;
  undefined4 in_fr14;
  undefined8 uVar19;
  int in_GBR;
  bool bVar20;
  char in_FPSCR_PR;
  char in_FPSCR_SZ;
  undefined4 uVar14;
  undefined4 uVar16;
  
  puVar6 = *(ushort **)(in_r2 + 4);
  iVar2 = *(int *)(in_GBR + 0x3c);
  *(int *)(in_GBR + 0x44) = (int)*(short *)(in_r2 + 2);
  iVar3 = *(int *)(in_GBR + 0x38);
  do {
    uVar12 = DAT_8c038758;
    iVar4 = *(int *)(in_GBR + 0x54);
    if (iVar4 == 0) {
      uVar1 = *puVar6;
      iVar4 = (int)(short)uVar1;
      puVar6 = puVar6 + 1;
      if (iVar4 < 0) {
        iVar4 = (int)(short)(uVar1 & DAT_8c038754);
      }
    }
    iVar4 = iVar4 + -2;
    iVar7 = 0;
    uVar1 = *puVar6;
    *(undefined4 *)(in_GBR + 0x48) = *(undefined4 *)((short)uVar1 * 4 + iVar3);
    puVar8 = (undefined8 *)((short)uVar1 * 0x10 + iVar2);
    uVar1 = puVar6[1];
    puVar6 = puVar6 + 2;
    *(undefined4 *)(in_GBR + 0x4c) = *(undefined4 *)((short)uVar1 * 4 + iVar3);
    puVar9 = (undefined8 *)((short)uVar1 * 0x10 + iVar2);
    if (in_FPSCR_SZ == '\0') {
      param_2 = *(float *)(puVar8 + 1);
      in_dr12 = (double)CONCAT44(*(undefined4 *)(puVar9 + 1),SUB84(in_dr12,0));
    }
    else {
      in_dr12 = (double)puVar9[1];
    }
    if (in_FPSCR_PR == '\0') {
      bVar20 = 0.0 < param_2;
    }
    else {
      bVar20 = 5.26354424712089e-315 < (double)CONCAT44(param_1,param_2);
    }
    uVar5 = (uint)!bVar20;
    if (in_FPSCR_PR == '\0') {
      bVar20 = 1.0 < param_2;
    }
    else {
      bVar20 = 5.26354424712089e-315 < (double)CONCAT44(param_1,param_2);
    }
    if (bVar20) {
      uVar5 = uVar5 | 0x10;
    }
    fVar18 = (float)((ulonglong)in_dr12 >> 0x20);
    if (in_FPSCR_PR == '\0') {
      bVar20 = 0.0 < fVar18;
    }
    else {
      bVar20 = 5.26354424712089e-315 < in_dr12;
    }
    if (!bVar20) {
      uVar5 = uVar5 | 2;
    }
    if (in_FPSCR_PR == '\0') {
      bVar20 = 1.0 < fVar18;
    }
    else {
      bVar20 = 5.26354424712089e-315 < in_dr12;
    }
    if (bVar20) {
      uVar5 = uVar5 | 0x20;
    }
    do {
      uVar1 = *puVar6;
      puVar6 = puVar6 + 1;
      *(undefined4 *)(in_GBR + 0x50) = *(undefined4 *)((short)uVar1 * 4 + iVar3);
      uVar11 = DAT_8c038788;
      puVar10 = (undefined8 *)((short)uVar1 * 0x10 + iVar2);
      if (in_FPSCR_SZ == '\0') {
        in_fr15 = *(float *)(puVar10 + 1);
      }
      if (in_FPSCR_PR == '\0') {
        bVar20 = 0.0 < in_fr15;
      }
      else {
        bVar20 = 5.26354424712089e-315 < (double)CONCAT44(in_fr14,in_fr15);
      }
      if (!bVar20) {
        uVar5 = uVar5 | 4;
      }
      if (in_FPSCR_PR == '\0') {
        bVar20 = 1.0 < in_fr15;
      }
      else {
        bVar20 = 5.26354424712089e-315 < (double)CONCAT44(in_fr14,in_fr15);
      }
      if (bVar20) {
        uVar5 = uVar5 | 0x40;
      }
      if (uVar5 == 0) {
        if (iVar7 == 0) {
          if (in_FPSCR_SZ == '\0') {
            uVar14 = *(undefined4 *)puVar8;
            uVar11 = *(undefined4 *)((int)puVar8 + 4);
            uVar16 = *(undefined4 *)(puVar8 + 1);
            *(undefined4 *)(unaff_r14 + 0x18) = *(undefined4 *)((int)puVar8 + 0xc);
            *(undefined4 *)(unaff_r14 + 0xc) = uVar16;
            *(undefined4 *)(unaff_r14 + 8) = uVar11;
            puVar8 = (undefined8 *)(unaff_r14 + 4);
            *(undefined4 *)puVar8 = uVar14;
          }
          else {
            uVar13 = *puVar8;
            uVar19 = puVar8[1];
            uVar15 = puVar8[2];
            *(undefined8 *)(unaff_r14 + 0x14) = puVar8[3];
            *(undefined8 *)(unaff_r14 + 4) = uVar15;
            *(undefined8 *)(unaff_r14 + -4) = uVar19;
            puVar8 = (undefined8 *)(unaff_r14 + -0xc);
            *puVar8 = uVar13;
          }
          *(undefined4 *)((int)puVar8 + -4) = uVar12;
          unaff_r14 = (int)puVar8 + 0x1c;
          iVar7 = 1;
        }
        if (in_FPSCR_SZ == '\0') {
          uVar14 = *(undefined4 *)puVar9;
          uVar11 = *(undefined4 *)((int)puVar9 + 4);
          uVar16 = *(undefined4 *)(puVar9 + 1);
          *(undefined4 *)(unaff_r14 + 0x18) = *(undefined4 *)((int)puVar9 + 0xc);
          *(undefined4 *)(unaff_r14 + 0xc) = uVar16;
          *(undefined4 *)(unaff_r14 + 8) = uVar11;
          puVar8 = (undefined8 *)(unaff_r14 + 4);
          *(undefined4 *)puVar8 = uVar14;
        }
        else {
          uVar13 = *puVar9;
          uVar19 = puVar9[1];
          uVar15 = puVar9[2];
          *(undefined8 *)(unaff_r14 + 0x14) = puVar9[3];
          *(undefined8 *)(unaff_r14 + 4) = uVar15;
          *(undefined8 *)(unaff_r14 + -4) = uVar19;
          puVar8 = (undefined8 *)(unaff_r14 + -0xc);
          *puVar8 = uVar13;
        }
        *(undefined4 *)((int)puVar8 + -4) = uVar12;
        unaff_r14 = (int)puVar8 + 0x1c;
        iVar7 = iVar7 + 1;
      }
      else {
        if (iVar7 != 0) {
          if (in_FPSCR_SZ == '\0') {
            uVar16 = *(undefined4 *)puVar9;
            uVar14 = *(undefined4 *)((int)puVar9 + 4);
            uVar17 = *(undefined4 *)(puVar9 + 1);
            *(undefined4 *)(unaff_r14 + 0x18) = *(undefined4 *)((int)puVar9 + 0xc);
            *(undefined4 *)(unaff_r14 + 0xc) = uVar17;
            *(undefined4 *)(unaff_r14 + 8) = uVar14;
            puVar8 = (undefined8 *)(unaff_r14 + 4);
            *(undefined4 *)puVar8 = uVar16;
          }
          else {
            uVar13 = *puVar9;
            uVar19 = puVar9[1];
            uVar15 = puVar9[2];
            *(undefined8 *)(unaff_r14 + 0x14) = puVar9[3];
            *(undefined8 *)(unaff_r14 + 4) = uVar15;
            *(undefined8 *)(unaff_r14 + -4) = uVar19;
            puVar8 = (undefined8 *)(unaff_r14 + -0xc);
            *puVar8 = uVar13;
          }
          *(undefined4 *)((int)puVar8 + -4) = uVar11;
          unaff_r14 = (int)puVar8 + 0x1c;
          iVar7 = 0;
        }
        if ((((uVar5 & 0x11) == 0) || ((uVar5 & 0x22) == 0)) || ((uVar5 & 0x44) == 0)) {
          FUN_8c038802(param_3,param_4);
          iVar3 = extraout_r3;
        }
      }
      *(undefined4 *)(in_GBR + 0x48) = *(undefined4 *)(in_GBR + 0x4c);
      *(undefined4 *)(in_GBR + 0x4c) = *(undefined4 *)(in_GBR + 0x50);
      uVar11 = DAT_8c0387e4;
      uVar5 = uVar5 >> 1 & 0x33;
      iVar4 = iVar4 + -1;
      puVar8 = puVar9;
      puVar9 = puVar10;
    } while (0 < iVar4);
    if (iVar7 != 0) {
      if (in_FPSCR_SZ == '\0') {
        uVar14 = *(undefined4 *)puVar10;
        uVar12 = *(undefined4 *)((int)puVar10 + 4);
        uVar16 = *(undefined4 *)(puVar10 + 1);
        *(undefined4 *)(unaff_r14 + 0x18) = *(undefined4 *)((int)puVar10 + 0xc);
        *(undefined4 *)(unaff_r14 + 0xc) = uVar16;
        *(undefined4 *)(unaff_r14 + 8) = uVar12;
        puVar8 = (undefined8 *)(unaff_r14 + 4);
        *(undefined4 *)puVar8 = uVar14;
      }
      else {
        uVar13 = *puVar10;
        uVar19 = puVar10[1];
        uVar15 = puVar10[2];
        *(undefined8 *)(unaff_r14 + 0x14) = puVar10[3];
        *(undefined8 *)(unaff_r14 + 4) = uVar15;
        *(undefined8 *)(unaff_r14 + -4) = uVar19;
        puVar8 = (undefined8 *)(unaff_r14 + -0xc);
        *puVar8 = uVar13;
      }
      *(undefined4 *)((int)puVar8 + -4) = uVar11;
      unaff_r14 = (int)puVar8 + 0x1c;
    }
    iVar4 = *(int *)(in_GBR + 0x44) + -1;
    *(int *)(in_GBR + 0x44) = iVar4;
  } while (iVar4 != 0);
  return unaff_r14;
}

