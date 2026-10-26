/* Ghidra draft only; not buildable/verified source. */

/* WARNING: Removing unreachable block (ram,0x8c656eba) */
/* WARNING: Removing unreachable block (ram,0x8c656eae) */
/* WARNING: Removing unreachable block (ram,0x8c656eb4) */
/* WARNING: Removing unreachable block (ram,0x8c656ec0) */
/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

int FUN_8c656940(undefined4 param_1,int param_2)

{
  undefined4 *puVar1;
  undefined *puVar2;
  undefined4 *puVar3;
  undefined *puVar4;
  undefined4 uVar5;
  int iVar6;
  uint uVar7;
  uint uVar8;
  undefined8 *puVar9;
  uint uVar10;
  int *piVar11;
  undefined4 *puVar12;
  int *piVar13;
  undefined4 *puVar14;
  uint uVar15;
  uint *puVar16;
  uint *puVar17;
  undefined8 *puVar18;
  undefined4 uVar19;
  float fVar20;
  double in_dr2;
  double dVar21;
  double dVar22;
  float fVar23;
  ulonglong in_dr14;
  undefined8 in_xd2;
  undefined8 in_xd14;
  bool bVar24;
  undefined4 in_PR;
  char in_FPSCR_PR;
  char in_FPSCR_SZ;
  uint auStack_50 [11];
  undefined8 local_24;
  
  if (in_FPSCR_SZ == '\0') {
    puVar18 = (undefined8 *)((int)&local_24 + 4);
    local_24 = CONCAT44((int)in_dr14,(undefined4)local_24);
  }
  else {
    puVar18 = &local_24;
    local_24 = in_xd14;
  }
  *(undefined4 *)((int)puVar18 + -4) = in_PR;
  dVar21 = (double)(in_dr14 & 0xffffffff00000000);
  *(undefined4 *)((int)puVar18 + -0x68) = 2;
  (*(code *)PTR_FUN_8c6569d0)();
  (*(code *)PTR_FUN_8c6569d4)();
  puVar1 = DAT_8c6569d8;
  DAT_8c6569d8[0x15] = 0;
  if ((*(uint *)(param_2 + 4) & 1) == 0) {
    *(undefined4 *)((int)puVar18 + -0x5c) = 0;
  }
  else {
    *(undefined4 *)((int)puVar18 + -0x5c) = 1;
  }
  if ((*(uint *)(param_2 + 4) & 0x20) == 0) {
    *(undefined4 *)((int)puVar18 + -0x4c) = 1;
  }
  else {
    *(undefined4 *)((int)puVar18 + -0x4c) = 0;
  }
  if ((*(uint *)(param_2 + 4) & 2) == 0) {
    *(undefined4 *)((int)puVar18 + -0x60) = 0;
  }
  else {
    *(undefined4 *)((int)puVar18 + -0x60) = 1;
  }
  if ((*(uint *)(param_2 + 4) & DAT_8c656a84) == 0) {
    *DAT_8c656a88 = 1;
  }
  else {
    *DAT_8c656a88 = 0;
  }
  (*(code *)PTR_FUN_8c656a8c)
            ((int)DAT_8c656a80,*(undefined4 *)((int)puVar18 + -0x60),*(undefined4 *)(param_2 + 0x14)
            );
  puVar3 = DAT_8c656a94;
  puVar2 = PTR_DAT_8c656a90;
  if ((*(uint *)(param_2 + 4) & DAT_8c656a98) == 0) {
    *(undefined4 *)PTR_DAT_8c656a90 = 0;
    if ((*(uint *)(param_2 + 4) & 0x10) == 0) {
      *DAT_8c656a9c = 0;
    }
    else {
      *DAT_8c656a9c = 1;
      *DAT_8c656a88 = 1;
    }
    uVar5 = FUN_8c656860(*DAT_8c656a9c,*(undefined4 *)(param_2 + 0x5c));
    puVar16 = DAT_8c656aa0;
    uVar8 = *(uint *)(param_2 + 0x28);
    *DAT_8c656aa0 = *(uint *)(param_2 + 0x24);
    if ((*puVar16 & 0x1f) != 0) {
      uVar8 = uVar8 - (0x20 - (*DAT_8c656aa0 & 0x1f));
      *DAT_8c656aa0 = *DAT_8c656aa0 + 0x1f & 0xffffffe0;
    }
    if (*DAT_8c656a9c == 0) {
      uVar8 = uVar8 >> 1;
    }
    puVar17 = (uint *)((int)puVar18 + -0x2c);
    for (puVar16 = puVar17; puVar16 < (uint *)((int)puVar18 + -0x18); puVar16 = puVar16 + 1) {
      *puVar16 = 0;
    }
    puVar9 = (undefined8 *)(param_2 + 0x2c);
    while (fVar20 = SUB84(dVar21,0), puVar9 < (undefined8 *)(param_2 + 0x40U)) {
      if (in_FPSCR_SZ == '\0') {
        in_dr2 = (double)CONCAT44((int)((ulonglong)in_dr2 >> 0x20),*(undefined4 *)puVar9);
        puVar9 = (undefined8 *)((int)puVar9 + 4);
      }
      else {
        in_xd2 = *puVar9;
        puVar9 = puVar9 + 1;
      }
      if (in_FPSCR_PR == '\0') {
        dVar21 = (double)CONCAT44((int)((ulonglong)dVar21 >> 0x20),fVar20 + SUB84(in_dr2,0));
      }
      else {
        dVar21 = dVar21 + in_dr2;
      }
    }
    if (in_FPSCR_SZ == '\0') {
      dVar22 = (double)CONCAT44(DAT_8c656be0,param_1);
    }
    else {
      dVar22 = (double)CONCAT44(_DAT_8c656be4,DAT_8c656be0);
    }
    fVar23 = (float)((ulonglong)dVar22 >> 0x20);
    if (in_FPSCR_PR == '\0') {
      bVar24 = fVar23 < fVar20;
    }
    else {
      bVar24 = dVar22 < dVar21;
    }
    if (bVar24) {
      uVar10 = 0;
      while (uVar10 < 0x14) {
        puVar9 = (undefined8 *)(param_2 + 0x2c + uVar10);
        if (in_FPSCR_SZ == '\0') {
          in_dr2 = (double)CONCAT44((int)((ulonglong)in_dr2 >> 0x20),*(undefined4 *)puVar9);
        }
        else {
          in_xd2 = *puVar9;
        }
        if (in_FPSCR_PR == '\0') {
          in_dr2 = (double)CONCAT44((int)((ulonglong)in_dr2 >> 0x20),
                                    (SUB84(in_dr2,0) * fVar23) / fVar20);
        }
        else {
          in_dr2 = (in_dr2 * dVar22) / dVar21;
        }
        if (in_FPSCR_SZ == '\0') {
          *(int *)((int)puVar18 + (uVar10 - 0x18)) = SUB84(in_dr2,0);
          uVar10 = uVar10 + 4;
        }
        else {
          *(undefined8 *)((int)puVar18 + (uVar10 - 0x18)) = in_xd2;
          uVar10 = uVar10 + 4;
        }
      }
    }
    else {
      uVar10 = 0;
      while (uVar10 < 0x14) {
        puVar16 = (uint *)(param_2 + 0x2c + uVar10);
        if (in_FPSCR_SZ == '\0') {
          uVar15 = *puVar16;
          in_dr2 = (double)(ulonglong)uVar15;
          *(uint *)((int)puVar18 + (uVar10 - 0x18)) = uVar15;
          uVar10 = uVar10 + 4;
        }
        else {
          *(undefined8 *)((int)puVar18 + (uVar10 - 0x18)) = *(undefined8 *)puVar16;
          uVar10 = uVar10 + 4;
        }
      }
    }
    for (uVar10 = 0; uVar19 = SUB84(in_dr2,0), uVar10 < 0x14; uVar10 = uVar10 + 4) {
      if (in_FPSCR_SZ == '\0') {
        uVar19 = *(undefined4 *)((int)puVar18 + (uVar10 - 0x18));
      }
      if (in_FPSCR_PR == '\0') {
        dVar21 = (double)CONCAT44((float)(int)uVar8,uVar19);
      }
      else {
        dVar21 = (double)(int)uVar8;
      }
      if ((int)uVar8 < 0) {
        if (in_FPSCR_PR == '\0') {
          dVar21 = (double)CONCAT44((float)((ulonglong)dVar21 >> 0x20) + _DAT_8c656be4,
                                    SUB84(dVar21,0));
        }
        else {
          dVar21 = dVar21 + (double)CONCAT44(uVar5,_DAT_8c656be4);
        }
      }
      if (in_FPSCR_PR == '\0') {
        fVar20 = (SUB84(dVar21,0) * (float)((ulonglong)dVar21 >> 0x20)) / fVar23;
      }
      else {
        fVar20 = SUB84((dVar21 * dVar21) / dVar22,0);
      }
      in_dr2 = (double)CONCAT44(DAT_8c656be8,fVar20);
      if (in_FPSCR_PR != '\0' || DAT_8c656be8 <= fVar20) {
        if (in_FPSCR_PR == '\0') {
          in_dr2 = (double)CONCAT44(DAT_8c656be8 + DAT_8c656be8,
                                    fVar20 - (DAT_8c656be8 + DAT_8c656be8));
        }
        else {
          in_dr2 = (in_dr2 + in_dr2) - (in_dr2 + in_dr2);
        }
      }
      if (in_FPSCR_PR == '\0') {
        uVar15 = (uint)SUB84(in_dr2,0);
      }
      else {
        uVar15 = (uint)in_dr2;
      }
      *(uint *)(uVar10 + (int)puVar17) = uVar15 & 0xfffffff8;
    }
    if (((*DAT_8c656bec & 0x40) != 0) && (*puVar17 < 0x38)) {
      *puVar17 = *puVar17 + 0x38;
    }
    iVar6 = -1;
    uVar15 = 0;
    for (uVar10 = 0; uVar10 < 5; uVar10 = uVar10 + 1) {
      *(uint **)((int)puVar18 + -0x6c) = puVar17 + uVar10;
      uVar7 = puVar17[uVar10];
      uVar15 = uVar15 + uVar7;
      if (iVar6 < (int)uVar7) {
        *(uint *)((int)puVar18 + -100) = uVar10;
        iVar6 = **(int **)((int)puVar18 + -0x6c);
      }
    }
    if (uVar8 < uVar15) {
      puVar16 = puVar17 + *(int *)((int)puVar18 + -100);
      *puVar16 = *puVar16 - (uVar15 - uVar8);
      *puVar16 = *puVar16 & 0xffffffe0;
    }
    if (*puVar17 != 0) {
      *puVar17 = *puVar17 - 0x10;
    }
    piVar11 = (int *)((int)puVar18 + -0x28);
    piVar13 = piVar11;
    for (; piVar11 < (int *)((int)puVar18 + -0x18); piVar11 = piVar11 + 1) {
      if (*piVar11 != 0) {
        *piVar13 = *piVar13 + -0x10;
      }
      piVar13 = piVar13 + 1;
    }
    *puVar1 = *(undefined4 *)(param_2 + 0x20);
    *(uint **)((int)puVar18 + -0x6c) = puVar17;
    puVar1[1] = *puVar17;
    *(undefined4 **)((int)puVar18 + -100) = (undefined4 *)((int)puVar18 + -0x28);
    puVar1[2] = *(undefined4 *)((int)puVar18 + -0x28);
    *(undefined4 **)((int)puVar18 + -0x50) = (undefined4 *)((int)puVar18 + -0x24);
    puVar1[3] = *(undefined4 *)((int)puVar18 + -0x24);
    *(undefined4 **)((int)puVar18 + -0x54) = (undefined4 *)((int)puVar18 + -0x20);
    puVar1[4] = *(undefined4 *)((int)puVar18 + -0x20);
    *(undefined4 **)((int)puVar18 + -0x58) = (undefined4 *)((int)puVar18 + -0x1c);
    puVar1[5] = *(undefined4 *)((int)puVar18 + -0x1c);
    *(undefined4 *)((int)puVar18 + -0x70) = **(undefined4 **)((int)puVar18 + -0x58);
    FUN_8c656f50(**(undefined4 **)((int)puVar18 + -0x6c),**(undefined4 **)((int)puVar18 + -100),
                 **(undefined4 **)((int)puVar18 + -0x50),**(undefined4 **)((int)puVar18 + -0x54));
    puVar12 = DAT_8c656d44;
    *(undefined4 *)(*(int *)(param_2 + 0x20) + 0x10) = *DAT_8c656d40;
    puVar14 = puVar12 + 5;
    for (; puVar12 < puVar14; puVar12 = puVar12 + 1) {
      *puVar12 = 0;
    }
    *(undefined4 *)((int)puVar18 + -0x70) = 0;
    *(undefined4 *)((int)puVar18 + -0x74) = *(undefined4 *)((int)puVar18 + -0x1c);
    uVar5 = FUN_8c657096(*puVar17,*(undefined4 *)((int)puVar18 + -0x28),
                         *(undefined4 *)((int)puVar18 + -0x24),*(undefined4 *)((int)puVar18 + -0x20)
                        );
    *(undefined4 *)((int)puVar18 + -0x6c) = uVar5;
    FUN_8c6573c4(uVar5);
    if (*DAT_8c656d48 == 1) {
      *(undefined4 *)((int)puVar18 + -0x68) = 1;
    }
    *(undefined4 *)((int)puVar18 + -0x70) = *(undefined4 *)((int)puVar18 + -0x68);
    *(undefined4 *)((int)puVar18 + -0x74) = 0;
    *(undefined4 *)((int)puVar18 + -0x78) = *(undefined4 *)((int)puVar18 + -0x1c);
    FUN_8c6574b8(*puVar17,*(undefined4 *)((int)puVar18 + -0x28),
                 *(undefined4 *)((int)puVar18 + -0x24),*(undefined4 *)((int)puVar18 + -0x20));
    iVar6 = DAT_8c656d50;
    puVar12 = DAT_8c656d4c;
    *puVar3 = *DAT_8c656d4c;
    puVar3[1] = puVar12[1];
    puVar3[2] = puVar12[2];
    puVar3[3] = puVar12[3];
    puVar3[4] = puVar12[4];
    for (uVar8 = 0; piVar13 = DAT_8c656d58, puVar14 = DAT_8c656d54, uVar8 < 0x14; uVar8 = uVar8 + 4)
    {
      *(undefined4 *)(uVar8 + iVar6) = *(undefined4 *)(uVar8 + (int)puVar3);
    }
    *DAT_8c656d54 = 0;
    puVar14[1] = 0;
    puVar14[2] = 0;
    puVar14[3] = 0;
    *piVar13 = (int)puVar12;
    puVar14 = DAT_8c656d60;
    piVar13 = DAT_8c656d5c;
    *DAT_8c656d5c = (int)puVar3;
    uVar8 = 0;
    if (*(int *)((int)puVar18 + -0x68) != 0) {
      do {
        uVar8 = uVar8 + 1;
        *puVar14 = puVar12;
        puVar12 = puVar12 + 5;
        puVar14 = puVar14 + 1;
      } while (uVar8 < *(uint *)((int)puVar18 + -0x68));
    }
    FUN_8c657240(*(undefined4 *)((int)puVar18 + -0x68));
    for (uVar8 = 0; uVar8 < 0x14; uVar8 = uVar8 + 4) {
      *(int *)(*piVar13 + uVar8) =
           *(int *)(*piVar13 + uVar8) + *(int *)((int)DAT_8c656d44 + uVar8) * 4;
    }
  }
  else {
    *DAT_8c656f80 = 1;
    piVar13 = DAT_8c656f84;
    *(undefined4 *)puVar2 = 1;
    *DAT_8c656f88 = *piVar13;
    piVar11 = DAT_8c656f84;
    piVar13[*DAT_8c656f84 + -4] = 2;
    *DAT_8c656f8c = 0;
    piVar11[6] = 1;
    *(uint **)((int)puVar18 + -0x68) = (uint *)((int)puVar18 + -0x48);
    puVar16 = (uint *)((int)puVar18 + -0x44);
    *(uint *)((int)puVar18 + -0x48) = *(uint *)(*(int *)(param_2 + 0x20) + 0x10) & 1;
    *puVar16 = *(uint *)(*(int *)(param_2 + 0x20) + 0x10) & 2;
    puVar17 = (uint *)((int)puVar18 + -0x40);
    *puVar17 = *(uint *)(*(int *)(param_2 + 0x20) + 0x10) & 4;
    *(uint **)((int)puVar18 + -0x6c) = (uint *)((int)puVar18 + -0x3c);
    *(uint *)((int)puVar18 + -0x3c) = *(uint *)(*(int *)(param_2 + 0x20) + 0x10) & 8;
    *(uint **)((int)puVar18 + -100) = (uint *)((int)puVar18 + -0x38);
    *(uint *)((int)puVar18 + -0x38) = *(uint *)(*(int *)(param_2 + 0x20) + 0x10) & 0x10;
    *(undefined4 *)((int)puVar18 + -0x70) = **(undefined4 **)((int)puVar18 + -100);
    FUN_8c656f50(**(undefined4 **)((int)puVar18 + -0x68),*puVar16,*puVar17,
                 **(undefined4 **)((int)puVar18 + -0x6c));
    *(undefined4 *)((int)puVar18 + -0x74) = 1;
    *(undefined4 *)((int)puVar18 + -0x78) = **(undefined4 **)((int)puVar18 + -100);
    uVar5 = FUN_8c657096(**(undefined4 **)((int)puVar18 + -0x68),*puVar16,*puVar17,
                         **(undefined4 **)((int)puVar18 + -0x6c));
    *(undefined4 *)((int)puVar18 + -0x6c) = uVar5;
    FUN_8c6573c4(uVar5);
  }
  uVar8 = *(uint *)(param_2 + 0x1c);
  puVar1[0x17] = uVar8;
  if ((uVar8 & 7) != 0) {
    puVar1[0x17] = puVar1[0x17] + 7 & 0xfffffff8;
  }
  puVar4 = PTR_s_HARDWARE_8c656f98;
  puVar2 = PTR_s_EnableTexture16MB_8c656f94;
  puVar1[0x16] = (uint)(*DAT_8c656f90 + puVar1[0x17] * -4 + puVar1[0x18] * -2) >> 1;
  iVar6 = (*(code *)PTR_FUN_8c656f9c)(puVar4,puVar2,0);
  if (iVar6 == 1) {
    if (DAT_8c656fa0 < (uint)puVar1[0x16]) {
      puVar1[0x16] = DAT_8c656fa0;
    }
  }
  else if (DAT_8c656fa4 < (uint)puVar1[0x16]) {
    puVar1[0x16] = DAT_8c656fa4;
  }
  if ((uint)puVar1[0x16] < (uint)puVar1[0x19]) {
    iVar6 = 3;
  }
  else {
    iVar6 = (*(code *)PTR_FUN_8c656fa8)
                      (puVar1[0x16],(undefined1 *)((int)puVar18 + -0x34),
                       (undefined1 *)((int)puVar18 + -0x30));
    if (iVar6 == 0) {
      *DAT_8c656fac = puVar1[0x16];
      *(undefined4 *)((int)puVar18 + -0x70) = 0;
      *(uint *)((int)puVar18 + -0x74) =
           ~*(uint *)((int)puVar18 + -0x60) & *(uint *)((int)puVar18 + -0x6c);
      (*(code *)PTR_FUN_8c656fb4)
                (*DAT_8c656fb0 >> 7,DAT_8c656fb0[1] >> 7,DAT_8c656fb0[2] >> 7,DAT_8c656fb0[3] >> 7);
      if ((*(uint *)(param_2 + 4) & DAT_8c656fb8) != 0) {
        *DAT_8c656f8c = 1;
      }
      puVar12 = DAT_8c656fc8;
      **(int **)(param_2 + 0x20) = (int)puVar3;
      *(undefined **)(*(int *)(param_2 + 0x20) + 4) = PTR_DAT_8c656fbc;
      *(undefined4 *)(*(int *)(param_2 + 0x20) + 8) = DAT_8c656fc0;
      *(undefined4 *)(*(int *)(param_2 + 0x20) + 0xc) = DAT_8c656fc4;
      puVar1[6] = *(undefined4 *)((int)puVar18 + -0x60);
      *puVar12 = *(undefined4 *)((int)puVar18 + -0x60);
      uVar5 = DAT_8c656fcc;
      puVar1[7] = **(undefined4 **)(param_2 + 8);
      puVar1[8] = *(undefined4 *)(param_2 + 0x10);
      puVar1[9] = *(undefined4 *)(param_2 + 0x14);
      puVar1[10] = *(undefined4 *)((int)puVar18 + -0x5c);
      puVar1[0xb] = *(undefined4 *)(*(int *)(param_2 + 8) + 4);
      puVar1[0xc] = *(undefined4 *)(param_2 + 0x10);
      puVar1[0xd] = *(undefined4 *)(param_2 + 0x14);
      puVar1[0xe] = *(undefined4 *)((int)puVar18 + -0x5c);
      puVar1[0xf] = **(undefined4 **)(param_2 + 8);
      puVar1[0x10] = *(undefined4 *)(*(int *)(param_2 + 8) + 4);
      puVar2 = PTR_DAT_8c656fd0;
      puVar1[0x11] = *(undefined4 *)((int)puVar18 + -0x4c);
      *(undefined4 *)puVar2 = uVar5;
      iVar6 = FUN_8c657760(puVar1[0x15]);
    }
  }
  return iVar6;
}

