/* Ghidra-generated draft. Not buildable or behavior-verified. */
/* Address: 8c010000 */

void startup_8c010000(void)

{
  *DAT_8c010024 = *DAT_8c010024 & DAT_8c010028 | (int)DAT_8c010020;
  (*(code *)PTR_startup_8c0100a0_8c01002c)();
  return;
}


/* Address: 8c0100a0 */

void startup_8c0100a0(void)

{
  undefined4 uVar1;
  undefined4 *puVar2;
  undefined4 *puVar3;
  
  puVar2 = DAT_8c010140;
  uVar1 = DAT_8c01013c;
  puVar3 = DAT_8c010138;
  do {
    *puVar3 = uVar1;
    puVar3 = puVar3 + 1;
  } while (puVar3 < puVar2);
  startup_8c0100d8();
  startup_8c0100c8();
  (*(code *)PTR_startup_8c053940_8c010144)();
  startup_8c0100d0();
  return;
}


/* Address: 8c0100c8 */

void startup_8c0100c8(void)

{
  (*(code *)PTR_startup_8c5ff8b8_8c01014c)(*(undefined4 *)PTR_PTR_8c010148);
  return;
}


/* Address: 8c0100d0 */

void startup_8c0100d0(void)

{
  (*(code *)PTR_startup_8c5ff936_8c010150)(*(undefined4 *)PTR_PTR_8c010148);
  return;
}


/* Address: 8c0100d8 */

void startup_8c0100d8(void)

{
  undefined1 uVar1;
  undefined *puVar2;
  undefined *puVar3;
  undefined1 *puVar4;
  undefined1 *puVar5;
  
  puVar2 = PTR_DAT_8c010154;
  for (puVar4 = *(undefined1 **)PTR_DAT_8c010158; puVar3 = PTR_PTR_8c01015c,
      puVar4 < *(undefined1 **)puVar2; puVar4 = puVar4 + 1) {
    *puVar4 = 0;
  }
  puVar5 = *(undefined1 **)PTR_PTR_8c010164;
  for (puVar4 = *(undefined1 **)PTR_PTR_8c010160; puVar2 = PTR_DAT_8c010168,
      puVar4 < *(undefined1 **)puVar3; puVar4 = puVar4 + 1) {
    uVar1 = *puVar5;
    puVar5 = puVar5 + 1;
    *puVar4 = uVar1;
  }
  for (puVar4 = *(undefined1 **)PTR_DAT_8c01016c; puVar3 = PTR_DAT_8c010170,
      puVar4 < *(undefined1 **)puVar2; puVar4 = puVar4 + 1) {
    *puVar4 = 0;
  }
  puVar5 = *(undefined1 **)PTR_PTR_8c010178;
  for (puVar4 = *(undefined1 **)PTR_DAT_8c010174; puVar4 < *(undefined1 **)puVar3;
      puVar4 = puVar4 + 1) {
    uVar1 = *puVar5;
    puVar5 = puVar5 + 1;
    *puVar4 = uVar1;
  }
  return;
}


/* Address: 8c010180 */

void FUN_8c010180(undefined4 param_1,undefined4 param_2,undefined4 param_3)

{
  undefined *puVar1;
  undefined *puVar2;
  int iVar3;
  uint uVar4;
  uint uVar5;
  int iVar6;
  
  (*(code *)PTR_FUN_8c0102a0)(FUN_8c010b58);
  (*(code *)PTR_FUN_8c0102a4)();
  (*(code *)PTR_FUN_8c0102bc)
            ((*(uint *)PTR_DAT_8c0102b0 & DAT_8c0102ac | DAT_8c0102b4) + DAT_8c0102b8,DAT_8c0102a8);
  (*(code *)PTR_FUN_8c0102c0)(param_1,param_2,param_3);
  (*(code *)PTR_FUN_8c0102c4)();
  (*(code *)PTR_FUN_8c0102d0)(0,DAT_8c0102cc,DAT_8c0102c8);
  (*(code *)PTR_FUN_8c0102d4)();
  puVar2 = PTR_FUN_8c0102e4;
  puVar1 = PTR_thunk_FUN_8c601618_8c0102e0;
  uVar4 = DAT_8c0102d8 & 0xffffffe0;
  uVar5 = DAT_8c0102dc & 0xffffffe0;
  iVar6 = 8;
  do {
    iVar3 = (*(code *)puVar1)(8,uVar4 + 0x20,0x834,uVar5 + 0x20);
    if ((iVar3 == -0x17) || (iVar3 == -0x21)) {
      FUN_8c01025a();
      (*(code *)puVar2)();
    }
    else if (iVar3 == 0) break;
    iVar6 = iVar6 + -1;
  } while (0 < iVar6);
  if (iVar6 == 0) {
    FUN_8c01025a();
    (*(code *)puVar2)();
  }
  (*(code *)PTR_FUN_8c0102ec)(PTR_s_SONICADV_8c0102e8);
  return;
}


/* Address: 8c01025a */

uint FUN_8c01025a(void)

{
  byte in_MD;
  byte in_RB;
  byte in_BL;
  byte in_FD;
  byte extraout_M;
  byte extraout_Q;
  byte in_IMASK;
  byte extraout_S;
  byte extraout_T;
  
  (*(code *)PTR_FUN_8c0102f0)();
  (*(code *)PTR_FUN_8c0102f4)();
  (*(code *)PTR_FUN_8c0102f8)();
  (*(code *)PTR_FUN_8c0102fc)();
  (*(code *)PTR_FUN_8c010300)();
  (*(code *)PTR_FUN_8c010304)();
  return ((uint)extraout_T | (uint)extraout_S << 1 | (uint)in_IMASK << 4) & 0xffffff0f |
         (uint)extraout_Q << 8 | (uint)extraout_M << 9 | (uint)in_FD << 0xf | (uint)in_BL << 0x1c |
         (uint)in_RB << 0x1d | (uint)in_MD << 0x1e | 0xf0;
}


/* Address: 8c010640 */

void FUN_8c010640(void)

{
  undefined *puVar1;
  undefined8 *puVar2;
  undefined8 *puVar3;
  undefined *puVar4;
  int iVar5;
  int iVar6;
  int iVar7;
  int iVar8;
  int iVar9;
  char in_FPSCR_SZ;
  
  puVar3 = DAT_8c010888;
  puVar2 = DAT_8c010884;
  puVar1 = PTR_DAT_8c010880;
  if (in_FPSCR_SZ == '\0') {
    *(undefined4 *)(*(int *)PTR_DAT_8c010880 + 0x20) = *(undefined4 *)DAT_8c010884;
  }
  else {
    *(undefined8 *)(*(int *)PTR_DAT_8c010880 + 0x20) = *DAT_8c010884;
  }
  if (in_FPSCR_SZ == '\0') {
    *(undefined4 *)(*(int *)puVar1 + 0x24) = *(undefined4 *)puVar3;
  }
  else {
    *(undefined8 *)(*(int *)puVar1 + 0x24) = *puVar3;
  }
  if (in_FPSCR_SZ == '\0') {
    *(undefined4 *)(*(int *)puVar1 + 0x28) = *(undefined4 *)DAT_8c01088c;
  }
  else {
    *(undefined8 *)(*(int *)puVar1 + 0x28) = *DAT_8c01088c;
  }
  *(undefined4 *)(*(int *)puVar1 + 0x14) = *(undefined4 *)((int)puVar2 + 0xc);
  *(undefined4 *)(*(int *)puVar1 + 0x18) = *DAT_8c010890;
  puVar4 = PTR_FUN_8c010898;
  *(undefined4 *)(*(int *)puVar1 + 0x1c) = *DAT_8c010894;
  iVar9 = 0;
  do {
    iVar6 = (*(code *)puVar4)(iVar9);
    if (iVar6 != 0) {
      iVar6 = *(int *)(iVar6 + 0x20);
      iVar8 = iVar9 * 0x18;
      if (in_FPSCR_SZ == '\0') {
        *(undefined4 *)(iVar6 + 0x20) = *(undefined4 *)(DAT_8c01089c + iVar8);
        *(undefined4 *)(iVar6 + 0x24) = *(undefined4 *)(DAT_8c0108a0 + iVar8);
        *(undefined4 *)(iVar6 + 0x28) = *(undefined4 *)(DAT_8c0108a4 + iVar8);
      }
      else {
        *(undefined8 *)(iVar6 + 0x20) = *(undefined8 *)(DAT_8c01089c + iVar8);
        *(undefined8 *)(iVar6 + 0x24) = *(undefined8 *)(DAT_8c0108a0 + iVar8);
        *(undefined8 *)(iVar6 + 0x28) = *(undefined8 *)(DAT_8c0108a4 + iVar8);
      }
      iVar5 = DAT_8c0108a8;
      iVar7 = DAT_8c0108a8 + 4;
      *(undefined4 *)(iVar6 + 0x14) = *(undefined4 *)(DAT_8c0108a8 + iVar8);
      *(undefined4 *)(iVar6 + 0x18) = *(undefined4 *)(iVar7 + iVar8);
      *(undefined4 *)(iVar6 + 0x1c) = *(undefined4 *)(iVar5 + 8 + iVar8);
    }
    iVar6 = (*(code *)puVar4)(iVar9 + 1);
    if (iVar6 != 0) {
      iVar6 = *(int *)(iVar6 + 0x20);
      iVar8 = (iVar9 + 1) * 0x18;
      if (in_FPSCR_SZ == '\0') {
        *(undefined4 *)(iVar6 + 0x20) = *(undefined4 *)(DAT_8c01089c + iVar8);
        *(undefined4 *)(iVar6 + 0x24) = *(undefined4 *)(DAT_8c0108a0 + iVar8);
        *(undefined4 *)(iVar6 + 0x28) = *(undefined4 *)(DAT_8c0108a4 + iVar8);
      }
      else {
        *(undefined8 *)(iVar6 + 0x20) = *(undefined8 *)(DAT_8c01089c + iVar8);
        *(undefined8 *)(iVar6 + 0x24) = *(undefined8 *)(DAT_8c0108a0 + iVar8);
        *(undefined8 *)(iVar6 + 0x28) = *(undefined8 *)(DAT_8c0108a4 + iVar8);
      }
      iVar5 = DAT_8c0108a8;
      iVar7 = DAT_8c0108a8 + 4;
      *(undefined4 *)(iVar6 + 0x14) = *(undefined4 *)(DAT_8c0108a8 + iVar8);
      *(undefined4 *)(iVar6 + 0x18) = *(undefined4 *)(iVar7 + iVar8);
      *(undefined4 *)(iVar6 + 0x1c) = *(undefined4 *)(iVar5 + 8 + iVar8);
    }
    iVar6 = (*(code *)puVar4)(iVar9 + 2);
    if (iVar6 != 0) {
      iVar6 = *(int *)(iVar6 + 0x20);
      iVar8 = (iVar9 + 2) * 0x18;
      if (in_FPSCR_SZ == '\0') {
        *(undefined4 *)(iVar6 + 0x20) = *(undefined4 *)(DAT_8c01089c + iVar8);
        *(undefined4 *)(iVar6 + 0x24) = *(undefined4 *)(DAT_8c0108a0 + iVar8);
        *(undefined4 *)(iVar6 + 0x28) = *(undefined4 *)(DAT_8c0108a4 + iVar8);
      }
      else {
        *(undefined8 *)(iVar6 + 0x20) = *(undefined8 *)(DAT_8c01089c + iVar8);
        *(undefined8 *)(iVar6 + 0x24) = *(undefined8 *)(DAT_8c0108a0 + iVar8);
        *(undefined8 *)(iVar6 + 0x28) = *(undefined8 *)(DAT_8c0108a4 + iVar8);
      }
      iVar5 = DAT_8c0108a8;
      iVar7 = DAT_8c0108a8 + 4;
      *(undefined4 *)(iVar6 + 0x14) = *(undefined4 *)(DAT_8c0108a8 + iVar8);
      *(undefined4 *)(iVar6 + 0x18) = *(undefined4 *)(iVar7 + iVar8);
      *(undefined4 *)(iVar6 + 0x1c) = *(undefined4 *)(iVar5 + 8 + iVar8);
    }
    iVar6 = (*(code *)puVar4)(iVar9 + 3);
    if (iVar6 != 0) {
      iVar6 = *(int *)(iVar6 + 0x20);
      iVar8 = (iVar9 + 3) * 0x18;
      if (in_FPSCR_SZ == '\0') {
        *(undefined4 *)(iVar6 + 0x20) = *(undefined4 *)(DAT_8c01089c + iVar8);
        *(undefined4 *)(iVar6 + 0x24) = *(undefined4 *)(DAT_8c0108a0 + iVar8);
        *(undefined4 *)(iVar6 + 0x28) = *(undefined4 *)(DAT_8c0108a4 + iVar8);
      }
      else {
        *(undefined8 *)(iVar6 + 0x20) = *(undefined8 *)(DAT_8c01089c + iVar8);
        *(undefined8 *)(iVar6 + 0x24) = *(undefined8 *)(DAT_8c0108a0 + iVar8);
        *(undefined8 *)(iVar6 + 0x28) = *(undefined8 *)(DAT_8c0108a4 + iVar8);
      }
      iVar5 = DAT_8c0108a8;
      iVar7 = DAT_8c0108a8 + 4;
      *(undefined4 *)(iVar6 + 0x14) = *(undefined4 *)(DAT_8c0108a8 + iVar8);
      *(undefined4 *)(iVar6 + 0x18) = *(undefined4 *)(iVar7 + iVar8);
      *(undefined4 *)(iVar6 + 0x1c) = *(undefined4 *)(iVar5 + 8 + iVar8);
    }
    iVar9 = iVar9 + 4;
  } while (iVar9 < 8);
  return;
}


/* Address: 8c0107a6 */

void FUN_8c0107a6(void)

{
  int *piVar1;
  char in_FPSCR_SZ;
  
  if (*(short *)PTR_DAT_8c0108ac == 1) {
    *DAT_8c0108b4 = *(undefined2 *)PTR_DAT_8c0108b0;
    piVar1 = DAT_8c0108b8;
    *DAT_8c0108bc = (short)*(undefined4 *)(*DAT_8c0108b8 + 0x14);
    *DAT_8c0108c0 = (short)*(undefined4 *)(*piVar1 + 0x18);
    *DAT_8c0108c4 = (short)*(undefined4 *)(*piVar1 + 0x1c);
    if (in_FPSCR_SZ == '\0') {
      *(undefined4 *)DAT_8c0108c8 = *(undefined4 *)(*piVar1 + 0x20);
    }
    else {
      *DAT_8c0108c8 = *(undefined8 *)(*piVar1 + 0x20);
    }
    if (in_FPSCR_SZ == '\0') {
      *(undefined4 *)DAT_8c0108cc = *(undefined4 *)(*piVar1 + 0x24);
    }
    else {
      *DAT_8c0108cc = *(undefined8 *)(*piVar1 + 0x24);
    }
    if (in_FPSCR_SZ == '\0') {
      *(undefined4 *)DAT_8c0108d0 = *(undefined4 *)(*piVar1 + 0x28);
    }
    else {
      *DAT_8c0108d0 = *(undefined8 *)(*piVar1 + 0x28);
    }
    if (in_FPSCR_SZ == '\0') {
      *(undefined4 *)DAT_8c0108d4 = *(undefined4 *)(*piVar1 + 0x2c);
    }
    else {
      *DAT_8c0108d4 = *(undefined8 *)(*piVar1 + 0x2c);
    }
    if (in_FPSCR_SZ == '\0') {
      *(undefined4 *)DAT_8c0108d8 = *(undefined4 *)(*piVar1 + 0x30);
    }
    else {
      *DAT_8c0108d8 = *(undefined8 *)(*piVar1 + 0x30);
    }
    if (in_FPSCR_SZ == '\0') {
      *(undefined4 *)DAT_8c0108dc = *(undefined4 *)(*piVar1 + 0x34);
    }
    else {
      *DAT_8c0108dc = *(undefined8 *)(*piVar1 + 0x34);
    }
  }
  return;
}


/* Address: 8c010810 */

void FUN_8c010810(void)

{
  undefined2 *puVar1;
  int *piVar2;
  undefined8 *puVar3;
  short *psVar4;
  undefined8 *puVar5;
  char in_FPSCR_SZ;
  
  piVar2 = DAT_8c0108b8;
  puVar1 = DAT_8c0108b4;
  if (*(short *)PTR_DAT_8c0108ac == 1) {
    psVar4 = DAT_8c0108b4 + 1;
    *(undefined2 *)PTR_DAT_8c0108b0 = *DAT_8c0108b4;
    *(int *)(*piVar2 + 0x14) = (int)*psVar4;
    *(int *)(*piVar2 + 0x18) = (int)*DAT_8c0108c0;
    *(int *)(*piVar2 + 0x1c) = (int)(short)puVar1[3];
    puVar3 = DAT_8c0108c8;
    if (in_FPSCR_SZ == '\0') {
      *(undefined4 *)(*piVar2 + 0x20) = *(undefined4 *)DAT_8c0108c8;
    }
    else {
      *(undefined8 *)(*piVar2 + 0x20) = *DAT_8c0108c8;
    }
    if (in_FPSCR_SZ == '\0') {
      *(undefined4 *)(*piVar2 + 0x24) = *(undefined4 *)DAT_8c0108cc;
    }
    else {
      *(undefined8 *)(*piVar2 + 0x24) = *DAT_8c0108cc;
    }
    puVar5 = (undefined8 *)(puVar1 + 8);
    if (in_FPSCR_SZ == '\0') {
      *(undefined4 *)(*piVar2 + 0x28) = *(undefined4 *)puVar5;
    }
    else {
      *(undefined8 *)(*piVar2 + 0x28) = *puVar5;
    }
    if (in_FPSCR_SZ == '\0') {
      *(undefined4 *)(*piVar2 + 0x2c) = *(undefined4 *)DAT_8c0108d4;
    }
    else {
      *(undefined8 *)(*piVar2 + 0x2c) = *DAT_8c0108d4;
    }
    if (in_FPSCR_SZ == '\0') {
      *(undefined4 *)(*piVar2 + 0x30) = *(undefined4 *)(puVar3 + 2);
    }
    else {
      *(undefined8 *)(*piVar2 + 0x30) = puVar3[2];
    }
    if (in_FPSCR_SZ == '\0') {
      *(undefined4 *)(*piVar2 + 0x34) = *(undefined4 *)DAT_8c0108dc;
    }
    else {
      *(undefined8 *)(*piVar2 + 0x34) = *DAT_8c0108dc;
    }
  }
  return;
}


/* Address: 8c010a0e */

void FUN_8c010a0e(undefined4 param_1,undefined4 param_2,undefined4 param_3)

{
  undefined4 *puVar1;
  undefined *puVar2;
  
  puVar1 = DAT_8c010ac0;
  *DAT_8c010ac0 = 2;
  *DAT_8c010ac4 = param_1;
  *DAT_8c010ac8 = param_2;
  puVar1[3] = param_3;
  *DAT_8c010ad0 = 1;
  puVar2 = PTR_FUN_8c010acc;
  do {
    (*(code *)puVar2)();
  } while (*DAT_8c010ad0 != 0);
  return;
}


/* Address: 8c010a6e */

void FUN_8c010a6e(void)

{
  (*(code *)PTR_FUN_8c010adc)(DAT_8c010ad4,DAT_8c010ad8);
  (*(code *)PTR_FUN_8c010ae0)(1);
  return;
}


/* Address: 8c010b00 */

void FUN_8c010b00(void)

{
  undefined *puVar1;
  undefined *puVar2;
  uint uVar3;
  int iVar4;
  
  (*(code *)PTR_FUN_8c010c04)(DAT_8c010c00,0,0x260);
  uVar3 = *(uint *)PTR_DAT_8c010c10;
  if ((int)uVar3 < 0) {
    iVar4 = 1 >> (~uVar3 & 0x1f) + 1;
  }
  else {
    iVar4 = 1 << (uVar3 & 0x1f);
  }
  (*(code *)PTR_FUN_8c010c14)(0xf,iVar4,DAT_8c010c0c,PTR_LAB_8c010c08);
  puVar2 = PTR_DAT_8c010c20;
  puVar1 = PTR_DAT_8c010c1c;
  *(undefined4 *)PTR_DAT_8c010c18 = 0;
  *(undefined4 *)puVar1 = 0;
  *(undefined4 *)puVar2 = 1;
  return;
}


/* Address: 8c010b58 */

undefined4 FUN_8c010b58(void)

{
  if (((*(int *)PTR_DAT_8c010c20 != 0) && (1 < *(int *)PTR_DAT_8c010c1c)) &&
     (*(int *)PTR_DAT_8c010c1c < 4)) {
    return 0xffffffff;
  }
  return 0;
}


/* Address: 8c010b7a */

/* WARNING: Removing unreachable block (ram,0x8c010dbc) */
/* WARNING: Removing unreachable block (ram,0x8c010d44) */
/* WARNING: Removing unreachable block (ram,0x8c010cd0) */
/* WARNING: Removing unreachable block (ram,0x8c010d08) */
/* WARNING: Removing unreachable block (ram,0x8c010d80) */
/* WARNING: Removing unreachable block (ram,0x8c010e42) */
/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_8c010b7a(void)

{
  undefined *puVar1;
  undefined *puVar2;
  undefined *puVar3;
  undefined4 *puVar4;
  undefined *puVar5;
  int *piVar6;
  undefined4 uVar7;
  undefined4 uVar8;
  uint uVar9;
  int iVar10;
  short *psVar11;
  undefined8 *puVar12;
  float fVar13;
  double dVar14;
  double in_dr14;
  double dVar15;
  undefined8 in_xd14;
  bool bVar16;
  undefined4 in_PR;
  char in_FPSCR_PR;
  char in_FPSCR_SZ;
  undefined1 auStack_34 [16];
  undefined8 local_24;
  
  piVar6 = DAT_8c010de0;
  uVar7 = DAT_8c010c38;
  puVar5 = PTR_DAT_8c010c34;
  puVar4 = DAT_8c010c30;
  puVar3 = PTR_FUN_8c010c2c;
  puVar2 = PTR_FUN_8c010c28;
  puVar1 = PTR_DAT_8c010c1c;
  if (in_FPSCR_SZ == '\0') {
    puVar12 = (undefined8 *)((int)&local_24 + 4);
    local_24 = CONCAT44(SUB84(in_dr14,0),(undefined4)local_24);
  }
  else {
    puVar12 = &local_24;
    local_24 = in_xd14;
  }
  *(undefined4 *)((int)puVar12 + -4) = in_PR;
  iVar10 = *(int *)PTR_DAT_8c010c1c;
  psVar11 = (short *)(*(int *)PTR_DAT_8c010c10 * 0x4c + DAT_8c010c00);
  if ((iVar10 == 0) || (*(int *)PTR_DAT_8c010c20 == 0)) {
    uVar7 = (*(code *)PTR_FUN_8c010de4)();
    puVar1 = PTR_DAT_8c010dec;
    *DAT_8c010de8 = uVar7;
    iVar10 = *(int *)puVar1 + -1;
    *(int *)puVar1 = iVar10;
    if (iVar10 < 0) {
      *(undefined4 *)PTR_DAT_8c010dec = 0;
    }
    uVar7 = _DAT_8c010df8;
    if (*(int *)(psVar11 + 8) == 0xc) {
      iVar10 = 0;
      uVar8 = *(undefined4 *)(PTR_s_NOW_SAVING_8c010df0 + 4);
      if (in_FPSCR_PR == '\0') {
        dVar14 = (double)CONCAT44((float)*(int *)(psVar11 + 6),(float)*(int *)(psVar11 + 4));
      }
      else {
        dVar14 = (double)*(int *)(psVar11 + 6);
      }
      *(undefined4 *)((int)puVar12 + -0x10) = *(undefined4 *)PTR_s_NOW_SAVING_8c010df0;
      *(undefined4 *)((int)puVar12 + -0xc) = uVar8;
      *(undefined2 *)((int)puVar12 + -8) = *(undefined2 *)(PTR_s_NOW_SAVING_8c010df0 + 8);
      if (in_FPSCR_PR == '\0') {
        fVar13 = SUB84(dVar14,0) / (float)((ulonglong)dVar14 >> 0x20);
      }
      else {
        fVar13 = SUB84(dVar14 / dVar14,0);
      }
      *(undefined *)((int)puVar12 + -6) = PTR_s_NOW_SAVING_8c010df0[10];
      if (in_FPSCR_SZ == '\0') {
        dVar14 = (double)CONCAT44(DAT_8c010df4,fVar13);
      }
      else {
        dVar14 = (double)CONCAT44(_DAT_8c010df8,DAT_8c010df4);
      }
      *(undefined1 **)((int)puVar12 + -0x28) = (undefined1 *)((int)puVar12 + -0x20);
      *(undefined1 **)((int)puVar12 + -0x2c) = (undefined1 *)((int)puVar12 + -0x1f);
      if (in_FPSCR_SZ == '\0') {
        in_dr14 = (double)CONCAT44((int)((ulonglong)in_dr14 >> 0x20),SUB84(dVar14,0));
      }
      if (in_FPSCR_PR == '\0') {
        dVar15 = (double)CONCAT44((int)((ulonglong)in_dr14 >> 0x20),
                                  SUB84(in_dr14,0) * (float)((ulonglong)dVar14 >> 0x20));
      }
      else {
        dVar15 = in_dr14 * dVar14;
      }
      do {
        dVar14 = (double)CONCAT44((int)((ulonglong)dVar14 >> 0x20),(float)iVar10);
        if (in_FPSCR_PR == '\0') {
          bVar16 = (float)iVar10 < SUB84(dVar15,0);
        }
        else {
          bVar16 = dVar14 < dVar15;
        }
        uVar8 = DAT_8c010dfc;
        if (bVar16) {
          uVar8 = uVar7;
        }
        (*(code *)puVar3)(uVar8);
        uVar8 = (undefined4)((ulonglong)dVar14 >> 0x20);
        **(undefined1 **)((int)puVar12 + -0x28) = *(undefined1 *)((int)puVar12 + iVar10 + -0x10);
        **(undefined1 **)((int)puVar12 + -0x2c) = 0;
        *(undefined4 *)((int)puVar12 + -0x30) = *puVar4;
        *puVar4 = 0;
        (*(code *)puVar2)((iVar10 + 0x1b) * 0x10000 | 3,(undefined1 *)((int)puVar12 + -0x20));
        *puVar4 = *(undefined4 *)((int)puVar12 + -0x30);
        dVar14 = (double)CONCAT44(uVar8,(float)(iVar10 + 1));
        if (in_FPSCR_PR == '\0') {
          bVar16 = (float)(iVar10 + 1) < SUB84(dVar15,0);
        }
        else {
          bVar16 = dVar14 < dVar15;
        }
        uVar8 = DAT_8c010dfc;
        if (bVar16) {
          uVar8 = uVar7;
        }
        (*(code *)puVar3)(uVar8);
        uVar8 = (undefined4)((ulonglong)dVar14 >> 0x20);
        **(undefined1 **)((int)puVar12 + -0x28) = *(undefined1 *)((int)puVar12 + iVar10 + -0xf);
        **(undefined1 **)((int)puVar12 + -0x2c) = 0;
        *(undefined4 *)((int)puVar12 + -0x30) = *puVar4;
        *puVar4 = 0;
        (*(code *)puVar2)((iVar10 + 0x1c) * 0x10000 | 3,(undefined1 *)((int)puVar12 + -0x20));
        *puVar4 = *(undefined4 *)((int)puVar12 + -0x30);
        dVar14 = (double)CONCAT44(uVar8,(float)(iVar10 + 2));
        if (in_FPSCR_PR == '\0') {
          bVar16 = (float)(iVar10 + 2) < SUB84(dVar15,0);
        }
        else {
          bVar16 = dVar14 < dVar15;
        }
        uVar8 = DAT_8c010dfc;
        if (bVar16) {
          uVar8 = uVar7;
        }
        (*(code *)puVar3)(uVar8);
        uVar8 = (undefined4)((ulonglong)dVar14 >> 0x20);
        **(undefined1 **)((int)puVar12 + -0x28) = *(undefined1 *)((int)puVar12 + iVar10 + -0xe);
        **(undefined1 **)((int)puVar12 + -0x2c) = 0;
        *(undefined4 *)((int)puVar12 + -0x30) = *puVar4;
        *puVar4 = 0;
        (*(code *)puVar2)((iVar10 + 0x1d) * 0x10000 | 3,(undefined1 *)((int)puVar12 + -0x20));
        *puVar4 = *(undefined4 *)((int)puVar12 + -0x30);
        dVar14 = (double)CONCAT44(uVar8,(float)(iVar10 + 3));
        if (in_FPSCR_PR == '\0') {
          bVar16 = (float)(iVar10 + 3) < SUB84(dVar15,0);
        }
        else {
          bVar16 = dVar14 < dVar15;
        }
        uVar8 = DAT_8c010f40;
        if (bVar16) {
          uVar8 = uVar7;
        }
        (*(code *)puVar3)(uVar8);
        **(undefined1 **)((int)puVar12 + -0x28) = *(undefined1 *)((int)puVar12 + iVar10 + -0xd);
        **(undefined1 **)((int)puVar12 + -0x2c) = 0;
        *(undefined4 *)((int)puVar12 + -0x30) = *puVar4;
        *puVar4 = 0;
        (*(code *)puVar2)((iVar10 + 0x1e) * 0x10000 | 3,(undefined1 *)((int)puVar12 + -0x20));
        iVar10 = iVar10 + 4;
        *puVar4 = *(undefined4 *)((int)puVar12 + -0x30);
      } while (iVar10 < 6);
      for (; iVar10 < 10; iVar10 = iVar10 + 1) {
        dVar14 = (double)CONCAT44((int)((ulonglong)dVar14 >> 0x20),(float)iVar10);
        if (in_FPSCR_PR == '\0') {
          bVar16 = (float)iVar10 < SUB84(dVar15,0);
        }
        else {
          bVar16 = dVar14 < dVar15;
        }
        uVar8 = DAT_8c010f40;
        if (bVar16) {
          uVar8 = uVar7;
        }
        (*(code *)puVar3)(uVar8);
        **(undefined1 **)((int)puVar12 + -0x28) = *(undefined1 *)((int)puVar12 + iVar10 + -0x10);
        **(undefined1 **)((int)puVar12 + -0x2c) = 0;
        *(undefined4 *)((int)puVar12 + -0x30) = *puVar4;
        *puVar4 = 0;
        (*(code *)puVar2)((iVar10 + 0x1b) * 0x10000 | 3,(undefined1 *)((int)puVar12 + -0x20));
        *puVar4 = *(undefined4 *)((int)puVar12 + -0x30);
      }
      *(undefined4 *)PTR_DAT_8c010f44 = 4;
    }
  }
  else if (iVar10 == 4) {
    *DAT_8c010c30 = 0;
    (*(code *)puVar3)(uVar7);
    (*(code *)puVar2)(DAT_8c010c40,PTR_s___MEMORY_CARD___8c010c3c);
    (*(code *)puVar2)(DAT_8c010c48,PTR_s___NOT_FOUND___8c010c44);
    *(undefined4 *)puVar5 = 0;
    *(int *)puVar1 = *(int *)puVar1 + -1;
  }
  else if (iVar10 == 3) {
    *(int *)PTR_DAT_8c010c1c = *(int *)PTR_DAT_8c010c1c + -1;
  }
  else if (iVar10 == 2) {
    *(undefined4 *)PTR_DAT_8c010c34 = 1;
    iVar10 = *piVar6;
    *(int *)((int)puVar12 + -0x24) = iVar10;
    uVar9 = *(uint *)(iVar10 + 8);
    if (((uVar9 & 2) == 0) && ((uVar9 & 0x400) == 0)) {
      if (*psVar11 == 1) {
        FUN_8c0113da();
        *(undefined4 *)puVar1 = 1;
        *(undefined4 *)puVar5 = 0;
      }
    }
    else {
      *(undefined4 *)puVar1 = 1;
      *(undefined4 *)puVar5 = 0;
    }
  }
  else if (iVar10 == 1) {
    *(undefined4 *)PTR_DAT_8c010c1c = 0;
  }
  return;
}


/* Address: 8c010f9a */

void FUN_8c010f9a(undefined2 *param_1)

{
  undefined2 local_14;
  undefined1 local_12;
  undefined1 local_11;
  undefined1 local_10;
  undefined1 local_f;
  undefined1 local_e;
  undefined1 local_d;
  
  (*(code *)PTR_FUN_8c0110a8)(&local_14);
  *param_1 = local_14;
  *(undefined1 *)(param_1 + 1) = local_12;
  *(undefined1 *)((int)param_1 + 3) = local_11;
  *(undefined1 *)(param_1 + 2) = local_10;
  *(undefined1 *)((int)param_1 + 5) = local_f;
  *(undefined1 *)(param_1 + 3) = local_e;
  *(undefined1 *)((int)param_1 + 7) = local_d;
  return;
}


/* Address: 8c010fcc */

int FUN_8c010fcc(undefined4 param_1,undefined4 param_2)

{
  bool bVar1;
  undefined *puVar2;
  undefined *puVar3;
  int iVar4;
  int iVar5;
  undefined1 auStack_2c [20];
  
  if (*(int *)PTR_DAT_8c0110ac == 0) {
    iVar5 = (*(code *)PTR_FUN_8c0110b0)(param_1,auStack_2c);
    puVar2 = PTR_FUN_8c0110b4;
    iVar4 = (*(code *)PTR_FUN_8c0110b4)(auStack_2c,param_2);
    puVar3 = PTR_FUN_8c0110b8;
    while ((-1 < iVar5 && (bVar1 = iVar4 != 0, iVar4 = 0, bVar1))) {
      iVar5 = (*(code *)puVar3)(param_1,auStack_2c);
      iVar4 = (*(code *)puVar2)(auStack_2c,param_2);
    }
  }
  else {
    iVar4 = -1;
  }
  return iVar4;
}


/* Address: 8c011042 */

int FUN_8c011042(char param_1)

{
  int iVar1;
  
  iVar1 = 0;
  do {
    if (param_1 == PTR_s_0123456789ABCDEF_8c0110bc[iVar1]) {
      return iVar1;
    }
    iVar1 = iVar1 + 1;
  } while (iVar1 < 0x10);
  return -1;
}


/* Address: 8c011060 */

/* WARNING: Restarted to delay deadcode elimination for space: stack */

int FUN_8c011060(undefined4 param_1)

{
  int iVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  int iVar5;
  char local_30;
  char local_2f;
  char local_2e;
  char local_2d;
  char local_2c;
  char local_2b;
  char local_2a;
  char local_29;
  char local_28;
  char local_27;
  char local_26;
  char local_25;
  
  iVar5 = -1;
  if (*(int *)PTR_DAT_8c0110ac == 0) {
    iVar1 = (*(code *)PTR_FUN_8c0112a0)(param_1,&local_30);
    if (((((local_30 == 'S') && (local_2f == 'O')) && (local_2e == 'N')) &&
        (((local_2d == 'I' && (local_2c == 'C')) &&
         ((local_2b == 'A' && ((local_2a == 'D' && (local_29 == 'V')))))))) &&
       ((local_28 == '_' &&
        (((iVar2 = FUN_8c011042((int)local_27), -1 < iVar2 &&
          (iVar2 = FUN_8c011042((int)local_26), -1 < iVar2)) &&
         (iVar2 = FUN_8c011042((int)local_25), -1 < iVar2)))))) {
      iVar2 = FUN_8c011042((int)local_27);
      iVar3 = FUN_8c011042((int)local_26);
      iVar4 = FUN_8c011042((int)local_25);
      iVar4 = iVar3 * 0x10 + iVar2 * 0x100 + iVar4;
      if (-1 < iVar4) {
        iVar5 = iVar4;
      }
    }
    while (-1 < iVar1) {
      iVar1 = (*(code *)PTR_FUN_8c0112a4)(param_1,&local_30);
      if ((((local_30 == 'S') && (local_2f == 'O')) &&
          ((local_2e == 'N' && ((local_2d == 'I' && (local_2c == 'C')))))) &&
         ((local_2b == 'A' &&
          (((((local_2a == 'D' && (local_29 == 'V')) && (local_28 == '_')) &&
            ((iVar2 = FUN_8c011042((int)local_27), -1 < iVar2 &&
             (iVar2 = FUN_8c011042((int)local_26), -1 < iVar2)))) &&
           (iVar2 = FUN_8c011042((int)local_25), -1 < iVar2)))))) {
        iVar2 = FUN_8c011042((int)local_27);
        iVar3 = FUN_8c011042((int)local_26);
        iVar4 = FUN_8c011042((int)local_25);
        iVar4 = iVar3 * 0x10 + iVar2 * 0x100 + iVar4;
        if (iVar5 < iVar4) {
          iVar5 = iVar4;
        }
      }
    }
  }
  else {
    iVar5 = -1;
  }
  return iVar5;
}


/* Address: 8c011238 */

bool FUN_8c011238(void)

{
  int iVar1;
  undefined1 auStack_50 [56];
  undefined1 local_18;
  undefined1 local_17;
  undefined1 local_16;
  undefined1 local_15;
  undefined1 local_14;
  undefined1 local_13;
  undefined1 local_12;
  undefined1 local_11;
  undefined1 local_10;
  undefined1 local_f;
  undefined1 local_e;
  undefined1 local_d;
  undefined1 local_c;
  undefined1 local_b;
  undefined1 local_a;
  undefined1 local_9;
  
  local_18 = *PTR_s_SONICADV_INT_8c0112a8;
  local_17 = PTR_s_SONICADV_INT_8c0112a8[1];
  local_16 = PTR_s_SONICADV_INT_8c0112a8[2];
  local_15 = PTR_s_SONICADV_INT_8c0112a8[3];
  local_14 = PTR_s_SONICADV_INT_8c0112a8[4];
  local_13 = PTR_s_SONICADV_INT_8c0112a8[5];
  local_12 = PTR_s_SONICADV_INT_8c0112a8[6];
  local_11 = PTR_s_SONICADV_INT_8c0112a8[7];
  local_10 = PTR_s_SONICADV_INT_8c0112a8[8];
  local_f = PTR_s_SONICADV_INT_8c0112a8[9];
  local_e = PTR_s_SONICADV_INT_8c0112a8[10];
  local_d = PTR_s_SONICADV_INT_8c0112a8[0xb];
  local_c = PTR_s_SONICADV_INT_8c0112a8[0xc];
  local_b = PTR_s_SONICADV_INT_8c0112a8[0xd];
  local_a = PTR_s_SONICADV_INT_8c0112a8[0xe];
  local_9 = PTR_s_SONICADV_INT_8c0112a8[0xf];
  if (*(int *)PTR_DAT_8c0112ac != 0) {
    return false;
  }
  (*(code *)PTR_FUN_8c011444)(*(undefined4 *)PTR_DAT_8c011440,auStack_50);
  iVar1 = FUN_8c010fcc(*(undefined4 *)PTR_DAT_8c011440,&local_18);
  return iVar1 == 0;
}


/* Address: 8c0112e6 */

undefined4 FUN_8c0112e6(void)

{
  undefined *puVar1;
  int iVar2;
  undefined4 uVar3;
  short *psVar4;
  undefined4 uVar5;
  undefined1 auStack_a0 [56];
  undefined1 auStack_68 [18];
  undefined1 auStack_56 [34];
  undefined1 auStack_34 [16];
  undefined4 uStack_24;
  undefined4 uStack_20;
  undefined2 uStack_1c;
  undefined2 uStack_1a;
  undefined4 uStack_18;
  undefined2 uStack_14;
  undefined4 uStack_10;
  undefined4 uStack_c;
  
  uVar3 = DAT_8c011448;
  psVar4 = (short *)(*(int *)PTR_DAT_8c011440 * 0x4c + DAT_8c01144c);
  (*(code *)PTR_FUN_8c011444)(*(undefined4 *)PTR_DAT_8c011440,auStack_a0);
  uVar5 = DAT_8c011450;
  FUN_8c010f9a(DAT_8c011454,DAT_8c011450);
  puVar1 = PTR_FUN_8c011458;
  (*(code *)PTR_FUN_8c011458)(auStack_68,PTR_s_MAIN_SAVE_FILE_8c01145c);
  (*(code *)puVar1)(auStack_56,PTR_s_SONIC_ADVENTURE___Main_Save_File_8c011460);
  (*(code *)puVar1)(auStack_34,PTR_s_0000000000000000_8c011464);
  uStack_24 = *(undefined4 *)PTR_PTR_8c011468;
  uStack_20 = *(undefined4 *)PTR_PTR_8c01146c;
  uStack_1c = (undefined2)*(uint *)PTR_DAT_8c011470;
  uStack_1a = 0x14;
  uStack_18 = 0;
  uStack_14 = 0;
  uStack_c = 0xf80;
  uStack_10 = uVar5;
  iVar2 = (*(code *)PTR_FUN_8c011474)(*(uint *)PTR_DAT_8c011470 & 0xffff,0,0xf80);
  if (iVar2 == -0x100) {
    return 0xffffffff;
  }
  uVar3 = (*(code *)PTR_FUN_8c011478)(uVar3,auStack_68);
  *DAT_8c01147c = uVar3;
  if (*psVar4 == 0) {
    uVar3 = 4;
    *(undefined4 *)PTR_DAT_8c011480 = 4;
  }
  else {
    uVar3 = FUN_8c0113da();
  }
  return uVar3;
}


/* Address: 8c0113da */

undefined4 FUN_8c0113da(void)

{
  int iVar1;
  
  iVar1 = (*(code *)PTR_FUN_8c01148c)
                    (*(undefined4 *)PTR_DAT_8c011440,PTR_s_SONICADV_INT_8c011488,DAT_8c011448,
                     *DAT_8c01147c,DAT_8c011454,DAT_8c011484);
  if (iVar1 != 0) {
    return 0xffffffff;
  }
  return 0;
}


/* Address: 8c0114f0 */

undefined4 FUN_8c0114f0(void)

{
  undefined *puVar1;
  undefined *puVar2;
  int iVar3;
  undefined4 uVar4;
  
  (*(code *)PTR_FUN_8c011570)();
  puVar2 = PTR_FUN_8c011564;
  puVar1 = PTR_DAT_8c011560;
  if (*(int *)PTR_DAT_8c01156c == 0) {
    do {
      iVar3 = (*(code *)puVar2)(*(undefined4 *)puVar1);
    } while (iVar3 == -1);
    uVar4 = FUN_8c0112e6();
    return uVar4;
  }
  return 0xffffffff;
}


/* Address: 8c011524 */

undefined4 FUN_8c011524(void)

{
  undefined *puVar1;
  undefined *puVar2;
  int iVar3;
  
  (*(code *)PTR_FUN_8c011570)();
  puVar2 = PTR_FUN_8c011564;
  puVar1 = PTR_DAT_8c011560;
  if (*(int *)PTR_DAT_8c01156c == 0) {
    do {
      iVar3 = (*(code *)puVar2)(*(undefined4 *)puVar1);
    } while (iVar3 == -1);
    iVar3 = FUN_8c0112e6();
    if (iVar3 == 0) {
      do {
        iVar3 = (*(code *)puVar2)(*(undefined4 *)puVar1);
      } while (iVar3 == -1);
      return 0;
    }
  }
  return 0xffffffff;
}


/* Address: 8c011592 */

undefined4 FUN_8c011592(void)

{
  undefined *puVar1;
  undefined *puVar2;
  int iVar3;
  
  (*(code *)PTR_FUN_8c0116c0)();
  puVar2 = PTR_FUN_8c0116cc;
  puVar1 = PTR_DAT_8c0116c8;
  if (*(int *)PTR_DAT_8c0116c4 == 0) {
    do {
      iVar3 = (*(code *)puVar2)(*(undefined4 *)puVar1);
    } while (iVar3 == -1);
    iVar3 = FUN_8c0112e6();
    if (iVar3 == 0) {
      do {
        iVar3 = (*(code *)puVar2)(*(undefined4 *)puVar1);
      } while (iVar3 == -1);
      return 0;
    }
  }
  return 0xffffffff;
}


/* Address: 8c0115d0 */

undefined4 FUN_8c0115d0(void)

{
  undefined *puVar1;
  undefined *puVar2;
  int iVar3;
  
  (*(code *)PTR_FUN_8c0116c0)();
  puVar2 = PTR_FUN_8c0116cc;
  puVar1 = PTR_DAT_8c0116c8;
  if (*(int *)PTR_DAT_8c0116c4 == 0) {
    do {
      iVar3 = (*(code *)puVar2)(*(undefined4 *)puVar1);
    } while (iVar3 == -1);
    iVar3 = FUN_8c0112e6();
    if (iVar3 == 0) {
      return 0;
    }
  }
  return 0xffffffff;
}


/* Address: 8c011606 */

undefined4 FUN_8c011606(void)

{
  undefined *puVar1;
  undefined *puVar2;
  undefined4 uVar3;
  int iVar4;
  int iVar5;
  uint uVar6;
  undefined1 *puVar7;
  undefined1 *puVar8;
  undefined1 *puVar9;
  undefined1 auStack_c8 [152];
  undefined1 local_30;
  undefined1 local_2f;
  undefined1 local_2e;
  undefined1 local_2d;
  undefined1 local_2c;
  undefined1 local_2b;
  undefined1 local_2a;
  undefined1 local_29;
  undefined1 local_28;
  undefined1 local_27;
  undefined1 local_26;
  undefined1 local_25;
  undefined1 local_24;
  undefined1 local_23;
  undefined1 local_22;
  undefined1 local_21;
  
  iVar5 = DAT_8c0116d4;
  puVar2 = PTR_FUN_8c0116cc;
  puVar1 = PTR_DAT_8c0116c8;
  local_30 = *PTR_s_SONICADV_INT_8c0116d0;
  local_2f = PTR_s_SONICADV_INT_8c0116d0[1];
  local_2e = PTR_s_SONICADV_INT_8c0116d0[2];
  local_2d = PTR_s_SONICADV_INT_8c0116d0[3];
  local_2c = PTR_s_SONICADV_INT_8c0116d0[4];
  local_2b = PTR_s_SONICADV_INT_8c0116d0[5];
  local_2a = PTR_s_SONICADV_INT_8c0116d0[6];
  local_29 = PTR_s_SONICADV_INT_8c0116d0[7];
  local_28 = PTR_s_SONICADV_INT_8c0116d0[8];
  local_27 = PTR_s_SONICADV_INT_8c0116d0[9];
  local_26 = PTR_s_SONICADV_INT_8c0116d0[10];
  local_25 = PTR_s_SONICADV_INT_8c0116d0[0xb];
  local_24 = PTR_s_SONICADV_INT_8c0116d0[0xc];
  local_23 = PTR_s_SONICADV_INT_8c0116d0[0xd];
  local_22 = PTR_s_SONICADV_INT_8c0116d0[0xe];
  local_21 = PTR_s_SONICADV_INT_8c0116d0[0xf];
  if (*(int *)PTR_DAT_8c0116c4 == 0) {
    do {
      iVar4 = (*(code *)puVar2)(*(undefined4 *)puVar1);
    } while (iVar4 == -1);
    (*(code *)PTR_FUN_8c0116d8)(*(undefined4 *)puVar1,auStack_c8);
    puVar7 = DAT_8c0116dc;
    uVar3 = (*(code *)PTR_FUN_8c0116e0)(*(undefined4 *)puVar1,&local_30);
    iVar4 = FUN_8c010fcc(*(undefined4 *)puVar1,&local_30);
    if (iVar4 == 0) {
      (*(code *)PTR_FUN_8c011860)(*(undefined4 *)puVar1,&local_30,iVar5,uVar3);
      do {
        iVar4 = (*(code *)puVar2)(*(undefined4 *)puVar1);
      } while (iVar4 == -1);
      puVar8 = (undefined1 *)(iVar5 + 0x480);
      iVar5 = (*(code *)PTR_FUN_8c011864)(puVar8);
      if (iVar5 == 0) {
        uVar3 = 0xfffffffe;
      }
      else {
        uVar6 = 0;
        do {
          uVar6 = uVar6 + 3;
          *puVar7 = *puVar8;
          puVar9 = puVar8 + 2;
          puVar7[1] = puVar8[1];
          puVar8 = puVar8 + 3;
          puVar7[2] = *puVar9;
          puVar7 = puVar7 + 3;
        } while (uVar6 < 0xde0);
        (*(code *)PTR_FUN_8c011868)();
        uVar3 = 0;
      }
    }
    else {
      uVar3 = 0xffffffff;
    }
  }
  else {
    uVar3 = 0xfffffffd;
  }
  return uVar3;
}


/* Address: 8c011828 */

void FUN_8c011828(undefined4 param_1)

{
  undefined1 auStack_3c [60];
  
  (*(code *)PTR_FUN_8c01187c)(*(undefined4 *)PTR_DAT_8c011878,auStack_3c);
  FUN_8c010fcc(*(undefined4 *)PTR_DAT_8c011878,param_1);
  return;
}


/* Address: 8c0118a0 */

undefined4 FUN_8c0118a0(undefined4 param_1,undefined4 param_2,int param_3)

{
  undefined4 uVar1;
  undefined *puVar2;
  undefined *puVar3;
  undefined4 uVar4;
  int iVar5;
  undefined *puVar6;
  undefined1 auStack_b4 [56];
  undefined1 auStack_7c [18];
  undefined1 auStack_6a [34];
  undefined1 auStack_48 [15];
  char cStack_39;
  undefined4 local_38;
  undefined4 local_34;
  undefined2 local_30;
  undefined2 uStack_2e;
  undefined4 uStack_2c;
  undefined2 uStack_28;
  undefined4 uStack_24;
  undefined4 uStack_20;
  undefined1 auStack_1c [12];
  
  puVar2 = PTR_DAT_8c011a08;
  uVar1 = DAT_8c011a00;
  if (*(int *)PTR_DAT_8c011a04 == 0) {
    (*(code *)PTR_FUN_8c011a0c)(*(undefined4 *)PTR_DAT_8c011a08,auStack_b4);
    FUN_8c010f9a(auStack_1c);
    puVar3 = PTR_FUN_8c011a10;
    if (param_3 == 1) {
      (*(code *)PTR_FUN_8c011a10)(auStack_7c,PTR_s_CHAO_UPLOAD_8c011a14);
      puVar6 = PTR_s_SONIC_ADVENTURE___CHAO_Upload_8c011a18;
    }
    else if (param_3 == 2) {
      (*(code *)PTR_FUN_8c011a10)(auStack_7c,PTR_s_DATA_UPLOAD_8c011a1c);
      puVar6 = PTR_s_SONIC_ADVENTURE___Data_Upload_8c011a20;
    }
    else if (param_3 == 3) {
      (*(code *)PTR_FUN_8c011a10)(auStack_7c,PTR_s_CART_TIME_8c011a24);
      puVar6 = PTR_s_SONIC_ADVENTURE___CART_Time_Data_8c011a28;
    }
    else if (param_3 == 4) {
      (*(code *)PTR_FUN_8c011a10)(auStack_7c,PTR_s_EVENT_RESULT_8c011a2c);
      puVar6 = PTR_s_SONIC_ADVENTURE___Event_Result_8c011a30;
    }
    else {
      (*(code *)PTR_FUN_8c011a10)(auStack_7c,PTR_DAT_8c011a34);
      puVar6 = PTR_DAT_8c011a38;
    }
    (*(code *)puVar3)(auStack_6a,puVar6);
    (*(code *)puVar3)(auStack_48,PTR_DAT_8c011a3c);
    cStack_39 = (char)param_3 + 'T';
    iVar5 = param_3 * 4;
    local_38 = *(undefined4 *)(PTR_PTR_8c011a40 + iVar5);
    local_34 = *(undefined4 *)(PTR_PTR_8c011a44 + iVar5);
    local_30 = (undefined2)*(uint *)(PTR_DAT_8c011a48 + iVar5);
    uStack_2e = 0x14;
    uStack_2c = 0;
    uStack_28 = 0;
    uStack_24 = DAT_8c011a4c;
    uStack_20 = param_2;
    iVar5 = (*(code *)PTR_FUN_8c011a50)(*(uint *)(PTR_DAT_8c011a48 + iVar5) & 0xffff,0,param_2);
    if (iVar5 != -0x100) {
      uVar4 = (*(code *)PTR_FUN_8c011a54)(uVar1,auStack_7c);
      iVar5 = (*(code *)PTR_FUN_8c011a5c)
                        (*(undefined4 *)puVar2,param_1,uVar1,uVar4,auStack_1c,DAT_8c011a58,uVar4);
      puVar3 = PTR_FUN_8c011bc0;
      if (iVar5 == 0) {
        if (param_3 != 4) {
          do {
            iVar5 = (*(code *)puVar3)(*(undefined4 *)puVar2);
          } while (iVar5 == -1);
        }
        return 0;
      }
    }
  }
  return 0xffffffff;
}


/* Address: 8c011a84 */

int FUN_8c011a84(undefined4 param_1)

{
  undefined1 uVar1;
  undefined *puVar2;
  undefined4 uVar3;
  undefined *puVar4;
  int iVar5;
  int iVar6;
  undefined1 *puVar7;
  undefined1 *puVar8;
  undefined1 auStack_b8 [164];
  
  puVar4 = PTR_DAT_8c011bcc;
  uVar3 = DAT_8c011bc4;
  puVar2 = PTR_FUN_8c011bc0;
  if (*(int *)PTR_DAT_8c011bc8 == 0) {
    do {
      iVar5 = (*(code *)puVar2)(*(undefined4 *)puVar4);
    } while (iVar5 == -1);
    (*(code *)PTR_FUN_8c011bd0)(*(undefined4 *)puVar4,auStack_b8);
    puVar8 = DAT_8c011bd4;
    iVar5 = (*(code *)PTR_FUN_8c011bd8)(*(undefined4 *)puVar4,param_1);
    iVar6 = FUN_8c010fcc(*(undefined4 *)puVar4,param_1);
    if (iVar6 == 0) {
      (*(code *)PTR_FUN_8c011bdc)(*(undefined4 *)puVar4,param_1,uVar3,iVar5);
      do {
        iVar6 = (*(code *)puVar2)(*(undefined4 *)puVar4);
      } while (iVar6 == -1);
      iVar6 = iVar5 * 0x200 + -0x280;
      iVar5 = 0;
      puVar7 = DAT_8c011be0;
      if (iVar6 < 1) {
        return iVar6;
      }
      do {
        uVar1 = *puVar7;
        puVar7 = puVar7 + 1;
        iVar5 = iVar5 + 1;
        *puVar8 = uVar1;
        puVar8 = puVar8 + 1;
      } while (iVar5 < iVar6);
      return iVar6;
    }
  }
  return -1;
}


/* Address: 8c011d46 */

/* WARNING: Removing unreachable block (ram,0x8c011dae) */

undefined4 FUN_8c011d46(uint param_1,uint param_2)

{
  int iVar1;
  undefined *puVar2;
  undefined *puVar3;
  int iVar4;
  uint uVar5;
  uint uVar6;
  undefined *puVar7;
  undefined1 *puVar8;
  undefined1 *puVar9;
  undefined1 local_2c;
  undefined1 local_2b;
  undefined1 local_2a;
  undefined1 local_29;
  undefined1 local_28;
  undefined1 local_27;
  undefined1 local_26;
  undefined1 local_25;
  undefined1 local_24;
  undefined1 local_23;
  char local_22;
  char local_21;
  undefined1 local_20;
  undefined1 local_1f;
  undefined1 local_1e;
  undefined1 local_1d;
  
  puVar2 = PTR_DAT_8c011f40;
  puVar7 = PTR_DAT_8c011de8;
  iVar1 = DAT_8c011de4;
  local_2c = *PTR_s_SONICADV_P00_8c011e18;
  local_2b = PTR_s_SONICADV_P00_8c011e18[1];
  local_2a = PTR_s_SONICADV_P00_8c011e18[2];
  local_29 = PTR_s_SONICADV_P00_8c011e18[3];
  local_28 = PTR_s_SONICADV_P00_8c011e18[4];
  local_27 = PTR_s_SONICADV_P00_8c011e18[5];
  local_26 = PTR_s_SONICADV_P00_8c011e18[6];
  local_25 = PTR_s_SONICADV_P00_8c011e18[7];
  local_24 = PTR_s_SONICADV_P00_8c011e18[8];
  local_23 = PTR_s_SONICADV_P00_8c011e18[9];
  local_20 = PTR_s_SONICADV_P00_8c011e18[0xc];
  local_1f = PTR_s_SONICADV_P00_8c011e18[0xd];
  local_1e = PTR_s_SONICADV_P00_8c011e18[0xe];
  local_1d = PTR_s_SONICADV_P00_8c011e18[0xf];
  uVar6 = (param_1 & 0xffff) << 3 | param_2 & 0xffff;
  uVar5 = (int)uVar6 >> 5 & 0x1f;
  local_22 = (char)uVar5;
  if (uVar5 < 10) {
    local_22 = local_22 + '0';
  }
  else {
    local_22 = local_22 + '7';
  }
  uVar6 = uVar6 & 0x1f;
  local_21 = (char)uVar6;
  if (uVar6 < 10) {
    local_21 = local_21 + '0';
  }
  else {
    local_21 = local_21 + '7';
  }
  iVar4 = FUN_8c010fcc(*(undefined4 *)PTR_DAT_8c011f40,&local_2c);
  if (iVar4 != 0) {
    local_28 = 0x30;
    iVar4 = FUN_8c010fcc(*(undefined4 *)puVar2,&local_2c);
    if (iVar4 != 0) {
      return 0xffffffff;
    }
  }
  (*(code *)PTR_FUN_8c011f44)(*(undefined4 *)puVar2,&local_2c,iVar1,0x12);
  puVar3 = PTR_FUN_8c011f48;
  do {
    iVar4 = (*(code *)puVar3)(*(undefined4 *)puVar2);
  } while (iVar4 == -1);
  iVar4 = 0x3000;
  puVar8 = (undefined1 *)(iVar1 + 0x280);
  do {
    iVar4 = iVar4 + -3;
    *puVar7 = *puVar8;
    puVar9 = puVar8 + 2;
    puVar7[1] = puVar8[1];
    puVar8 = puVar8 + 3;
    puVar7[2] = *puVar9;
    puVar7 = puVar7 + 3;
  } while (iVar4 != 0);
  return 0;
}


/* Address: 8c012014 */

/* WARNING: Removing unreachable block (ram,0x8c01207c) */

undefined4 FUN_8c012014(uint param_1,uint param_2)

{
  undefined *puVar1;
  int iVar2;
  undefined *puVar3;
  int iVar4;
  uint uVar5;
  uint uVar6;
  undefined1 *puVar7;
  undefined1 *puVar8;
  undefined1 *puVar9;
  undefined1 local_2c;
  undefined1 local_2b;
  undefined1 local_2a;
  undefined1 local_29;
  undefined1 local_28;
  undefined1 local_27;
  undefined1 local_26;
  undefined1 local_25;
  undefined1 local_24;
  undefined1 local_23;
  char local_22;
  char local_21;
  undefined1 local_20;
  undefined1 local_1f;
  undefined1 local_1e;
  undefined1 local_1d;
  
  puVar7 = DAT_8c012114;
  iVar2 = DAT_8c012110;
  puVar1 = PTR_DAT_8c0120fc;
  local_2c = *PTR_s_SONICADV_S00_8c01210c;
  local_2b = PTR_s_SONICADV_S00_8c01210c[1];
  local_2a = PTR_s_SONICADV_S00_8c01210c[2];
  local_29 = PTR_s_SONICADV_S00_8c01210c[3];
  local_28 = PTR_s_SONICADV_S00_8c01210c[4];
  local_27 = PTR_s_SONICADV_S00_8c01210c[5];
  local_26 = PTR_s_SONICADV_S00_8c01210c[6];
  local_25 = PTR_s_SONICADV_S00_8c01210c[7];
  local_24 = PTR_s_SONICADV_S00_8c01210c[8];
  local_23 = PTR_s_SONICADV_S00_8c01210c[9];
  local_20 = PTR_s_SONICADV_S00_8c01210c[0xc];
  local_1f = PTR_s_SONICADV_S00_8c01210c[0xd];
  local_1e = PTR_s_SONICADV_S00_8c01210c[0xe];
  local_1d = PTR_s_SONICADV_S00_8c01210c[0xf];
  uVar6 = (param_1 & 0xffff) << 3 | param_2 & 0xffff;
  uVar5 = (int)uVar6 >> 5 & 0x1f;
  local_22 = (char)uVar5;
  if (uVar5 < 10) {
    local_22 = local_22 + '0';
  }
  else {
    local_22 = local_22 + '7';
  }
  uVar6 = uVar6 & 0x1f;
  local_21 = (char)uVar6;
  if (uVar6 < 10) {
    local_21 = local_21 + '0';
  }
  else {
    local_21 = local_21 + '7';
  }
  iVar4 = (*(code *)PTR_FUN_8c012118)(*(undefined4 *)PTR_DAT_8c0120fc,&local_2c);
  if (iVar4 != 0) {
    local_28 = 0x30;
    iVar4 = (*(code *)PTR_FUN_8c012118)(*(undefined4 *)puVar1,&local_2c);
    if (iVar4 != 0) {
      return 0xffffffff;
    }
  }
  (*(code *)PTR_FUN_8c012220)(*(undefined4 *)puVar1,&local_2c,iVar2,5);
  puVar3 = PTR_FUN_8c012224;
  do {
    iVar4 = (*(code *)puVar3)(*(undefined4 *)puVar1);
  } while (iVar4 == -1);
  iVar4 = 0x600;
  puVar8 = (undefined1 *)(iVar2 + 0x280);
  do {
    iVar4 = iVar4 + -3;
    *puVar7 = *puVar8;
    puVar9 = puVar8 + 2;
    puVar7[1] = puVar8[1];
    puVar8 = puVar8 + 3;
    puVar7[2] = *puVar9;
    puVar7 = puVar7 + 3;
  } while (iVar4 != 0);
  return 0;
}


/* Address: 8c012240 */

/* WARNING: Removing unreachable block (ram,0x8c01226e) */
/* WARNING: Removing unreachable block (ram,0x8c012352) */
/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_8c012240(uint param_1,undefined4 param_2)

{
  double *pdVar1;
  int *piVar2;
  int *piVar3;
  undefined *puVar4;
  undefined8 *puVar5;
  undefined8 *puVar6;
  ulonglong *puVar7;
  uint uVar8;
  int iVar9;
  undefined4 *puVar10;
  undefined4 *puVar11;
  uint uVar12;
  int iVar13;
  int iVar14;
  int iVar15;
  undefined8 uVar16;
  undefined4 in_fr3;
  double dVar17;
  float fVar18;
  float fVar19;
  undefined8 uVar20;
  undefined4 uVar21;
  double dVar22;
  double in_xd2;
  undefined4 in_PR;
  char in_FPSCR_PR;
  char in_FPSCR_SZ;
  undefined4 local_28;
  undefined4 uStack_24;
  undefined4 uStack_20;
  undefined4 local_1c;
  
  puVar4 = PTR_FUN_8c012464;
  *DAT_8c012460 = 0;
  local_1c = in_PR;
  uVar8 = (*(code *)puVar4)();
  iVar15 = -1;
  iVar14 = -1;
  uVar12 = uVar8 & 0xff;
  iVar13 = (int)(uVar8 & 0xffff & DAT_8c012468) >> 8;
  (*(code *)PTR_FUN_8c01246c)(0);
  iVar9 = (*(code *)PTR_thunk_FUN_8c64826e_8c012474)(*(undefined4 *)PTR_DAT_8c012470);
  if (iVar9 != 0) {
    iVar15 = FUN_8c012014(iVar13,uVar12);
    iVar14 = FUN_8c011d46(iVar13,uVar12);
  }
  if ((iVar15 != 0) || (iVar14 != 0)) {
    puVar11 = DAT_8c012478 + 0x168;
    pdVar1 = _DAT_8c012484;
    for (puVar10 = DAT_8c012478; _DAT_8c012484 = pdVar1, puVar10 < puVar11; puVar10 = puVar10 + 0x24
        ) {
      *puVar10 = 0;
      puVar10[0xc] = 0;
      puVar10[0x18] = 0;
      pdVar1 = _DAT_8c012484;
    }
    uVar20 = CONCAT44(0x3f800000,param_1);
    *DAT_8c01247c = 0;
    puVar5 = DAT_8c01248c;
    uVar21 = DAT_8c012480;
    if (in_FPSCR_SZ == '\0') {
      *(undefined4 *)pdVar1 = DAT_8c012480;
      puVar6 = DAT_8c012490;
      *(undefined4 *)DAT_8c012488 = 0x3f800000;
      *(undefined4 *)puVar5 = 0x3f800000;
      *(undefined4 *)puVar6 = 0x3f800000;
      piVar3 = DAT_8c01249c;
      piVar2 = DAT_8c012498;
      *(undefined4 *)DAT_8c012494 = 0x3f800000;
      in_fr3 = uVar21;
    }
    else {
      in_xd2 = (double)CONCAT44(_DAT_8c012484,DAT_8c012480);
      *pdVar1 = in_xd2;
      puVar6 = DAT_8c012490;
      *DAT_8c012488 = uVar20;
      *puVar5 = uVar20;
      *puVar6 = uVar20;
      piVar3 = DAT_8c01249c;
      piVar2 = DAT_8c012498;
      *DAT_8c012494 = uVar20;
    }
    *piVar2 = 0;
    *piVar3 = 0;
    puVar6 = DAT_8c0124a8;
    puVar5 = DAT_8c0124a4;
    if (in_FPSCR_SZ == '\0') {
      *(undefined4 *)DAT_8c0124a0 = 0x3f800000;
      *(undefined4 *)puVar5 = 0x3f800000;
      *(undefined4 *)puVar6 = 0x3f800000;
      puVar5 = DAT_8c0124b4;
      *(undefined4 *)DAT_8c0124b0 = DAT_8c0124ac;
      *(undefined4 *)puVar5 = 0x3f800000;
      in_fr3 = DAT_8c0124b8;
      uVar21 = DAT_8c0124c0;
    }
    else {
      *DAT_8c0124a0 = uVar20;
      *puVar5 = uVar20;
      *puVar6 = uVar20;
      puVar5 = DAT_8c0124b4;
      *DAT_8c0124b0 = CONCAT44(DAT_8c0124b0,DAT_8c0124ac);
      *puVar5 = uVar20;
      in_xd2 = (double)CONCAT44(DAT_8c0124bc,DAT_8c0124b8);
      uVar21 = DAT_8c0124c0;
    }
    iVar9 = 0;
    DAT_8c0124c0 = uVar21;
    if (in_FPSCR_SZ == '\0') {
      *(undefined4 *)DAT_8c0124bc = in_fr3;
    }
    else {
      *DAT_8c0124bc = in_xd2;
    }
    do {
      puVar4 = PTR_DAT_8c0124c4;
      uVar8 = 0;
      iVar15 = 4;
      do {
        *(undefined4 *)(puVar4 + uVar8 + iVar9 * 0x800) = 0xffffffff;
        *(undefined4 *)(puVar4 + iVar15 + iVar9 * 0x800) = uVar21;
        *(undefined4 *)(puVar4 + uVar8 + 8 + iVar9 * 0x800) = 0xffffffff;
        uVar8 = uVar8 + 0x10;
        *(undefined4 *)(puVar4 + iVar15 + 8 + iVar9 * 0x800) = uVar21;
        iVar15 = iVar15 + 0x10;
      } while (uVar8 < 0x800);
      iVar9 = iVar9 + 1;
    } while (iVar9 < 6);
    (*(code *)PTR_FUN_8c0124c8)(iVar13,uVar12);
    param_1 = (uint)uVar20;
    (*(code *)PTR_FUN_8c0124cc)(iVar13,uVar12);
  }
  puVar4 = PTR_FUN_8c0124d4;
  puVar7 = DAT_8c0124d0;
  if (in_FPSCR_SZ == '\0') {
    *(undefined4 *)DAT_8c0124d0 = 0;
    *(undefined4 *)((int)puVar7 + 4) = 0xbf800000;
    *(undefined4 *)(puVar7 + 1) = 0;
  }
  else {
    *DAT_8c0124d0 = (ulonglong)param_1;
    *(double *)((int)puVar7 + 4) = in_xd2;
    puVar7[1] = (ulonglong)param_1;
  }
  (*(code *)puVar4)(0);
  (*(code *)PTR_FUN_8c0124d8)(0);
  if (*DAT_8c012498 != 0) {
    (*(code *)PTR_FUN_8c0124dc)(0,*DAT_8c012498);
  }
  if (*DAT_8c01249c != 0) {
    (*(code *)PTR_FUN_8c0124e0)(0,*DAT_8c01249c);
  }
  (*(code *)PTR_FUN_8c0124e8)(0,DAT_8c0124d0,DAT_8c0124e4);
  (*(code *)PTR_FUN_8c0124ec)(1);
  (*(code *)PTR_FUN_8c0124d4)(0);
  (*(code *)PTR_FUN_8c0124e8)(0,DAT_8c0124e4,&local_28);
  (*(code *)PTR_FUN_8c0124ec)(1);
  if (in_FPSCR_SZ == '\0') {
    uVar20 = CONCAT44(uStack_20,param_2);
    uVar16 = CONCAT44(local_28,uStack_24);
  }
  else {
    uVar20 = CONCAT44(local_1c,uStack_20);
    uVar16 = CONCAT44(uStack_24,local_28);
  }
  (*(code *)PTR_FUN_8c0124f0)(uVar16,uVar20);
  if (in_FPSCR_SZ == '\0') {
    uVar20 = CONCAT44(uStack_20,(int)uVar20);
    dVar22 = (double)CONCAT44(local_28,uStack_24);
  }
  else {
    uVar20 = CONCAT44(local_1c,uStack_20);
    dVar22 = (double)CONCAT44(uStack_24,local_28);
  }
  uVar16 = (*(code *)PTR_FUN_8c0124f4)(dVar22,uVar20);
  fVar19 = (float)uVar16;
  uVar21 = (undefined4)uVar20;
  if (in_FPSCR_SZ == '\0') {
    dVar17 = (double)CONCAT44(*(undefined4 *)DAT_8c0124bc,DAT_8c0124f8);
    fVar19 = DAT_8c0124fc;
  }
  else {
    dVar17 = *DAT_8c0124bc;
  }
  if (in_FPSCR_PR == '\0') {
    fVar18 = (float)((ulonglong)dVar17 >> 0x20) + SUB84(dVar17,0);
  }
  else {
    fVar18 = (float)((ulonglong)(dVar17 + dVar17) >> 0x20);
  }
  if (in_FPSCR_SZ == '\0') {
    dVar22 = (double)CONCAT44((int)((ulonglong)dVar22 >> 0x20),fVar18);
  }
  if (in_FPSCR_PR == '\0') {
    fVar19 = SUB84(dVar22,0) * fVar19;
  }
  else {
    fVar19 = SUB84(dVar22 * (double)CONCAT44((int)((ulonglong)uVar16 >> 0x20),fVar19),0);
  }
  if (in_FPSCR_SZ == '\0') {
    uVar20 = CONCAT44(*(undefined4 *)DAT_8c0124b4,fVar19);
  }
  else {
    uVar20 = *DAT_8c0124b4;
  }
  (*(code *)PTR_FUN_8c012500)(uVar20);
  if (in_FPSCR_SZ == '\0') {
    uVar20 = CONCAT44(*(undefined4 *)DAT_8c0124a0,uVar21);
    uVar16 = CONCAT44(*(undefined4 *)DAT_8c0124a8,*(undefined4 *)DAT_8c0124a4);
  }
  else {
    uVar20 = *DAT_8c0124a0;
    uVar16 = *DAT_8c0124a8;
  }
  (*(code *)PTR_FUN_8c012504)(uVar16,uVar20);
  uVar21 = (undefined4)uVar20;
  (*(code *)PTR_FUN_8c01250c)(DAT_8c012508,0x13);
  if (in_FPSCR_SZ == '\0') {
    dVar22 = (double)CONCAT44(*(undefined4 *)DAT_8c0124bc,uVar21);
    uVar20 = CONCAT44(*(undefined4 *)DAT_8c0124b0,*(undefined4 *)DAT_8c0124b4);
  }
  else {
    dVar22 = *DAT_8c0124bc;
    uVar20 = *DAT_8c0124b0;
  }
  (*(code *)PTR_FUN_8c012510)(uVar20,dVar22,DAT_8c012508);
  if (in_FPSCR_SZ == '\0') {
    uVar20 = CONCAT44(*(undefined4 *)DAT_8c0124a0,SUB84(dVar22,0));
    uVar16 = CONCAT44(*(undefined4 *)DAT_8c0124a8,*(undefined4 *)DAT_8c0124a4);
  }
  else {
    uVar20 = *DAT_8c0124a0;
    uVar16 = *DAT_8c0124a8;
  }
  (*(code *)PTR_FUN_8c012514)(uVar16,uVar20,DAT_8c012508);
  uVar21 = (undefined4)uVar20;
  (*(code *)PTR_FUN_8c012518)(DAT_8c012508,0xaaa,0x31c7);
  if (in_FPSCR_SZ == '\0') {
    uVar20 = CONCAT44(*(undefined4 *)(DAT_8c0124e4 + 1),uVar21);
    uVar16 = CONCAT44(*(undefined4 *)DAT_8c0124e4,*(undefined4 *)((int)DAT_8c0124e4 + 4));
  }
  else {
    uVar20 = DAT_8c0124e4[1];
    uVar16 = *DAT_8c0124e4;
  }
  (*(code *)PTR_FUN_8c01251c)(uVar16,uVar20,DAT_8c012508);
  return;
}


/* Address: 8c01413a */

void FUN_8c01413a(undefined8 param_1,undefined4 param_2,undefined4 param_3,int param_4)

{
  undefined8 uVar1;
  undefined8 in_xd6;
  char in_FPSCR_SZ;
  undefined2 local_34;
  undefined2 local_32;
  undefined2 local_30;
  undefined2 local_2e;
  undefined4 local_2c;
  undefined4 local_28;
  undefined8 local_24;
  undefined4 local_1c;
  undefined4 uStack_18;
  undefined8 *local_14;
  undefined4 *local_10;
  undefined2 *local_c;
  undefined4 local_8;
  
  local_14 = &local_24;
  local_10 = &local_2c;
  local_c = &local_34;
  local_8 = 4;
  uVar1 = param_1;
  if (in_FPSCR_SZ == '\0') {
    local_24 = CONCAT44(local_24._4_4_,(int)((ulonglong)param_1 >> 0x20));
                    /* WARNING: Ignoring partial resolution of indirect */
    local_24._4_4_ = (int)param_1;
    uVar1 = local_24;
  }
  local_24 = uVar1;
  local_2c = *(undefined4 *)(param_4 + 4);
  local_34 = 0;
  local_32 = 0;
  local_1c = param_2;
  uStack_18 = param_3;
  if (in_FPSCR_SZ != '\0') {
    uStack_18 = (undefined4)in_xd6;
    local_14 = (undefined8 *)((ulonglong)in_xd6 >> 0x20);
    local_1c = param_3;
  }
  local_28 = *(undefined4 *)(param_4 + 4);
  local_30 = 0xff;
  local_2e = 0xff;
  if (in_FPSCR_SZ == '\0') {
    uVar1 = CONCAT44(DAT_8c0141d8,(int)param_1);
  }
  else {
    uVar1 = CONCAT44(PTR_FUN_8c0141dc,DAT_8c0141d8);
  }
  (*(code *)PTR_FUN_8c0141dc)(uVar1,&local_14,1,0x40);
  return;
}


/* Address: 8c0141e0 */

undefined4 FUN_8c0141e0(undefined4 param_1)

{
  int iVar1;
  int iVar2;
  uint uVar3;
  int local_10;
  undefined4 local_c;
  
  local_c = param_1;
  iVar1 = (*(code *)PTR_FUN_8c014380)(0);
  if (iVar1 == 0) {
    return 0;
  }
  (*(code *)PTR_FUN_8c014384)(iVar1,&local_10);
  iVar2 = (*(code *)PTR_FUN_8c014388)(0x800);
  uVar3 = (uint)(iVar2 != 0);
  local_10 = (*(code *)PTR_FUN_8c01438c)();
  local_10 = local_10 + uVar3;
  (*(code *)PTR_FUN_8c014390)(iVar1,local_10,local_c);
  (*(code *)PTR_FUN_8c014394)(iVar1);
  return 1;
}


/* Address: 8c014242 */

undefined4 FUN_8c014242(ushort param_1,char param_2)

{
  undefined4 uVar1;
  int iVar2;
  undefined4 uVar3;
  undefined1 local_1c;
  undefined1 local_1b;
  undefined1 local_1a;
  char local_19;
  char local_18;
  undefined1 local_17;
  undefined1 local_16;
  undefined1 local_15;
  undefined1 local_14;
  undefined1 local_13;
  undefined1 local_12;
  undefined1 local_11;
  undefined1 local_10;
  undefined1 local_f;
  undefined1 local_e;
  undefined1 local_d;
  
  uVar1 = DAT_8c01439c;
  local_1c = *PTR_DAT_8c014398;
  local_1b = PTR_DAT_8c014398[1];
  local_1a = PTR_DAT_8c014398[2];
  local_11 = PTR_DAT_8c014398[0xb];
  local_10 = PTR_DAT_8c014398[0xc];
  local_f = PTR_DAT_8c014398[0xd];
  local_e = PTR_DAT_8c014398[0xe];
  local_d = PTR_DAT_8c014398[0xf];
  if (param_1 < 10) {
    local_19 = (char)param_1 + '0';
  }
  else {
    local_19 = (char)param_1 + '7';
  }
  if ('Z' < local_19) {
    local_19 = local_19 + -0x1a;
    local_1a = 0x31;
  }
  local_18 = param_2 + '0';
  local_17 = 0x42;
  local_16 = 0x2e;
  local_15 = 0x62;
  local_14 = 0x69;
  local_13 = 0x6e;
  local_12 = 0;
  iVar2 = FUN_8c0141e0(&local_1c,DAT_8c01439c);
  uVar3 = 1;
  if (iVar2 != 1) {
    local_18 = 0x30;
    uVar3 = FUN_8c0141e0(&local_1c,uVar1);
  }
  return uVar3;
}


/* Address: 8c014308 */

undefined4 FUN_8c014308(ushort param_1,char param_2)

{
  undefined *puVar1;
  int iVar2;
  undefined4 uVar3;
  undefined1 local_1c;
  undefined1 local_1b;
  undefined1 local_1a;
  char local_19;
  char local_18;
  undefined1 local_17;
  undefined1 local_16;
  undefined1 local_15;
  undefined1 local_14;
  undefined1 local_13;
  undefined1 local_12;
  undefined1 local_11;
  undefined1 local_10;
  undefined1 local_f;
  undefined1 local_e;
  undefined1 local_d;
  
  puVar1 = PTR_DAT_8c0143a4;
  local_1c = *PTR_DAT_8c0143a0;
  local_1b = PTR_DAT_8c0143a0[1];
  local_1a = PTR_DAT_8c0143a0[2];
  local_11 = PTR_DAT_8c0143a0[0xb];
  local_10 = PTR_DAT_8c0143a0[0xc];
  local_f = PTR_DAT_8c0143a0[0xd];
  local_e = PTR_DAT_8c0143a0[0xe];
  local_d = PTR_DAT_8c0143a0[0xf];
  if (param_1 < 10) {
    local_19 = (char)param_1 + '0';
  }
  else {
    local_19 = (char)param_1 + '7';
  }
  if ('Z' < local_19) {
    local_19 = local_19 + -0x1a;
    local_1a = 0x31;
  }
  local_18 = param_2 + '0';
  local_17 = 0x42;
  local_16 = 0x2e;
  local_15 = 0x62;
  local_14 = 0x69;
  local_13 = 0x6e;
  local_12 = 0;
  iVar2 = FUN_8c0141e0(&local_1c,PTR_DAT_8c0143a4);
  uVar3 = 1;
  if (iVar2 != 1) {
    local_18 = 0x30;
    uVar3 = FUN_8c0141e0(&local_1c,puVar1);
  }
  return uVar3;
}


/* Address: 8c014422 */

void thunk_FUN_8c012240(void)

{
  (*(code *)PTR_FUN_8c014460)();
  return;
}


/* Address: 8c014480 */

undefined4 FUN_8c014480(int param_1,int param_2)

{
  undefined *puVar1;
  undefined *puVar2;
  int iVar3;
  int iVar4;
  undefined4 uVar5;
  
  puVar2 = PTR_FUN_8c014548;
  iVar4 = DAT_8c014544;
  puVar1 = PTR_DAT_8c014540;
  uVar5 = 1;
  iVar3 = (*(code *)PTR_FUN_8c014548)
                    (*(int *)PTR_DAT_8c014540 * 0x4a0 + DAT_8c014544,param_1,param_2,0,1);
  if (iVar3 != 0) {
    return 0xffffffff;
  }
  if (param_1 != 0) {
    if (param_1 == 2) {
      iVar3 = (*(code *)puVar2)(2,param_2,1,uVar5);
      if (iVar3 != 0) {
        if (param_2 == 4) {
          return 0x1e;
        }
        return 5;
      }
      iVar4 = (*(code *)puVar2)(*(int *)puVar1 * 0x4a0 + iVar4,2,param_2,2,uVar5);
      if (iVar4 != 0) {
        if (param_2 == 4) {
          return 0x1d;
        }
        return 4;
      }
      if (param_2 == 2) {
        return 0x19;
      }
      if (param_2 != 9) {
        if (param_2 == 8) {
          return 0x1b;
        }
        if (param_2 != 4) {
          return 3;
        }
        return 0x1c;
      }
      return 0x1a;
    }
    if (param_1 == 3) {
      iVar3 = (*(code *)puVar2)(3,param_2,1,uVar5);
      if (iVar3 == 0) {
        iVar4 = (*(code *)puVar2)(*(int *)puVar1 * 0x4a0 + iVar4,3,param_2,2,uVar5);
        if (iVar4 != 0) {
          return 7;
        }
        return 6;
      }
      if (param_2 == 6) {
        return 0x30;
      }
      return 8;
    }
    if (param_1 == 5) {
      iVar3 = (*(code *)puVar2)(5,param_2,1,uVar5);
      if (iVar3 == 0) {
        iVar4 = (*(code *)puVar2)(*(int *)puVar1 * 0x4a0 + iVar4,5,param_2,2,uVar5);
        if (iVar4 != 0) {
          return 10;
        }
        return 9;
      }
      if (param_2 != 0xc) {
        if (param_2 != 10) {
          return 0xb;
        }
        return 0x32;
      }
      return 0x31;
    }
    if (param_1 == 6) {
      iVar3 = (*(code *)puVar2)(6,param_2,1,uVar5);
      if (iVar3 != 0) {
        if (param_2 == 10) {
          return 0x21;
        }
        if (param_2 == 1) {
          return 0x24;
        }
        if (param_2 == 5) {
          return 0x27;
        }
        if (param_2 != 0xc) {
          return 0xe;
        }
        return 0x2a;
      }
      iVar4 = (*(code *)puVar2)(*(int *)puVar1 * 0x4a0 + iVar4,6,param_2,2,uVar5);
      if (iVar4 == 0) {
        if (param_2 == 10) {
          return 0x1f;
        }
        if (param_2 == 1) {
          return 0x22;
        }
        if (param_2 == 5) {
          return 0x25;
        }
        if (param_2 != 0xc) {
          return 0xc;
        }
        return 0x28;
      }
      if (param_2 == 10) {
        return 0x20;
      }
      if (param_2 == 1) {
        return 0x23;
      }
      if (param_2 == 5) {
        return 0x26;
      }
      if (param_2 != 0xc) {
        return 0xd;
      }
      return 0x29;
    }
    if (param_1 == 7) {
      iVar3 = (*(code *)puVar2)(7,param_2,1,uVar5);
      if (iVar3 != 0) {
        return 0x11;
      }
      iVar4 = (*(code *)puVar2)(*(int *)puVar1 * 0x4a0 + iVar4,7,param_2,2,uVar5);
      if (iVar4 != 0) {
        return 0x10;
      }
      return 0xf;
    }
  }
  iVar3 = (*(code *)puVar2)(*(int *)puVar1 * 0x4a0 + iVar4,param_1,param_2,1,uVar5);
  if (iVar3 == 0) {
    iVar4 = (*(code *)puVar2)(*(int *)puVar1 * 0x4a0 + iVar4,param_1,param_2,2,uVar5);
    if (iVar4 == 0) {
      if (param_2 == 1) {
        uVar5 = 0x15;
      }
      else if (param_2 == 2) {
        uVar5 = 0x12;
      }
      else if (param_2 == 5) {
        uVar5 = 0x16;
      }
      else if (param_2 == 7) {
        uVar5 = 0x17;
      }
      else if (param_2 == 8) {
        uVar5 = 0x14;
      }
      else if (param_2 == 9) {
        uVar5 = 0x13;
      }
      else if (param_2 == 10) {
        uVar5 = 0x18;
      }
      else {
        uVar5 = 0;
      }
    }
    else {
      uVar5 = 1;
    }
  }
  else if (param_2 == 1) {
    uVar5 = 0x2b;
  }
  else if ((param_2 == 9) || (param_2 == 6)) {
    uVar5 = 0x2f;
  }
  else if (param_2 == 8) {
    uVar5 = 0x2d;
  }
  else if (param_2 == 4) {
    uVar5 = 0x2c;
  }
  else if ((param_2 == 7) || (param_2 == 10)) {
    uVar5 = 0x2e;
  }
  else {
    uVar5 = 2;
  }
  return uVar5;
}


/* Address: 8c01493e */

/* WARNING: Removing unreachable block (ram,0x8c0149a0) */

void FUN_8c01493e(void)

{
  char cVar1;
  int *piVar2;
  undefined4 *puVar3;
  undefined *puVar4;
  uint uVar5;
  int iVar6;
  int iVar7;
  char *pcVar8;
  undefined4 *puVar9;
  undefined4 extraout_fr3;
  undefined4 uVar10;
  undefined8 in_xd2;
  char in_FPSCR_SZ;
  
  uVar5 = (*(code *)PTR_FUN_8c014988)();
  uVar5 = (uVar5 & 0xffff) >> 8;
  iVar6 = (*(code *)PTR_FUN_8c014ae0)();
  if (((((uVar5 != 0) && (uVar5 < 0xf)) &&
       ((uVar5 != 8 || ((*DAT_8c014ae4 != 9 || (*DAT_8c014ae8 != '\x01')))))) &&
      ((uVar5 != 5 || (iVar6 != 2)))) &&
     (((uVar5 != 1 || (iVar6 != 2)) &&
      (iVar7 = FUN_8c014480(iVar6,uVar5), puVar3 = DAT_8c014af0, piVar2 = DAT_8c014aec, -1 < iVar7))
     )) {
    if (*DAT_8c014aec == 0) {
      *DAT_8c014af0 = PTR_PTR_8c014af4 + iVar7 * 0xc;
    }
    else {
      puVar9 = (undefined4 *)(PTR_PTR_8c014af8 + iVar7 * 0xc);
      *DAT_8c014af0 = puVar9;
      pcVar8 = (char *)*puVar9;
      cVar1 = *pcVar8;
      while (cVar1 != '\0') {
        pcVar8 = pcVar8 + 1;
        cVar1 = *pcVar8;
      }
      pcVar8[-1] = PTR_s_jefsg_8c014afc[*piVar2];
    }
    puVar4 = PTR_FUN_8c014b00;
    puVar9 = DAT_8c014af0;
    puVar3[1] = 1;
    (*(code *)puVar4)(puVar9);
    iVar7 = (*(code *)PTR_FUN_8c014b08)(2,1,PTR_LAB_8c014b04);
    **(undefined1 **)(iVar7 + 0x20) = 0;
    *(char *)(*(int *)(iVar7 + 0x20) + 9) = (char)iVar6;
    *(undefined2 *)(*(int *)(iVar7 + 0x20) + 6) = 0x78;
    uVar10 = (undefined4)DAT_8c014b0c;
    if (in_FPSCR_SZ != '\0') {
      in_xd2 = CONCAT44(DAT_8c014b0c._4_4_,(undefined4)DAT_8c014b0c);
      uVar10 = extraout_fr3;
    }
    if (in_FPSCR_SZ == '\0') {
      *(undefined4 *)(*(int *)(iVar7 + 0x20) + 0x20) = uVar10;
      uVar10 = DAT_8c014b0c._4_4_;
    }
    else {
      *(undefined8 *)(*(int *)(iVar7 + 0x20) + 0x20) = in_xd2;
      in_xd2 = CONCAT44(PTR_LAB_8c014b14,DAT_8c014b0c._4_4_);
    }
    if (in_FPSCR_SZ == '\0') {
      *(undefined4 *)(*(int *)(iVar7 + 0x20) + 0x24) = uVar10;
    }
    else {
      *(undefined8 *)(*(int *)(iVar7 + 0x20) + 0x24) = in_xd2;
    }
    *(undefined **)(iVar7 + 0x18) = PTR_LAB_8c014b14;
  }
  return;
}


/* Address: 8c014a78 */

/* WARNING: Removing unreachable block (ram,0x8c014a8e) */

void FUN_8c014a78(void)

{
  char cVar1;
  int *piVar2;
  undefined4 *puVar3;
  undefined4 uVar4;
  undefined *puVar5;
  uint uVar6;
  undefined4 uVar7;
  int iVar8;
  undefined4 *puVar9;
  char *pcVar10;
  undefined4 extraout_fr3;
  undefined8 in_xd2;
  char in_FPSCR_SZ;
  
  uVar6 = (*(code *)PTR_FUN_8c014b18)();
  uVar6 = (uVar6 & 0xffff) >> 8;
  uVar7 = (*(code *)PTR_FUN_8c014ae0)();
  if (((uVar6 != 0) && (uVar6 < 0xf)) &&
     (iVar8 = FUN_8c014480(uVar7,uVar6), puVar3 = DAT_8c014af0, piVar2 = DAT_8c014aec, -1 < iVar8))
  {
    if (*DAT_8c014aec == 0) {
      *DAT_8c014af0 = PTR_PTR_8c014af4 + iVar8 * 0xc;
    }
    else {
      puVar9 = (undefined4 *)(PTR_PTR_8c014cc0 + iVar8 * 0xc);
      *DAT_8c014af0 = puVar9;
      pcVar10 = (char *)*puVar9;
      cVar1 = *pcVar10;
      while (cVar1 != '\0') {
        pcVar10 = pcVar10 + 1;
        cVar1 = *pcVar10;
      }
      pcVar10[-1] = PTR_s_jefsg_8c014cc4[*piVar2];
    }
    puVar5 = PTR_FUN_8c014ccc;
    uVar4 = DAT_8c014cc8;
    puVar3[1] = 1;
    (*(code *)puVar5)(uVar4);
    iVar8 = (*(code *)PTR_FUN_8c014cd4)(2,1,PTR_LAB_8c014cd0);
    **(undefined1 **)(iVar8 + 0x20) = 0;
    *(char *)(*(int *)(iVar8 + 0x20) + 9) = (char)uVar7;
    *(undefined2 *)(*(int *)(iVar8 + 0x20) + 6) = 0x3c;
    uVar7 = (undefined4)DAT_8c014cd8;
    if (in_FPSCR_SZ != '\0') {
      in_xd2 = CONCAT44(DAT_8c014cd8._4_4_,(undefined4)DAT_8c014cd8);
      uVar7 = extraout_fr3;
    }
    if (in_FPSCR_SZ == '\0') {
      *(undefined4 *)(*(int *)(iVar8 + 0x20) + 0x20) = uVar7;
      uVar7 = DAT_8c014cd8._4_4_;
    }
    else {
      *(undefined8 *)(*(int *)(iVar8 + 0x20) + 0x20) = in_xd2;
      in_xd2 = CONCAT44(PTR_LAB_8c014ce0,DAT_8c014cd8._4_4_);
    }
    if (in_FPSCR_SZ == '\0') {
      *(undefined4 *)(*(int *)(iVar8 + 0x20) + 0x24) = uVar7;
    }
    else {
      *(undefined8 *)(*(int *)(iVar8 + 0x20) + 0x24) = in_xd2;
    }
    *(undefined **)(iVar8 + 0x18) = PTR_LAB_8c014ce0;
  }
  return;
}


/* Address: 8c014b86 */

void FUN_8c014b86(void)

{
  return;
}


/* Address: 8c014c4a */

void FUN_8c014c4a(ulonglong param_1,int param_2)

{
  undefined1 *puVar1;
  ulonglong uVar2;
  ulonglong uVar3;
  undefined4 uVar4;
  ulonglong in_xd14;
  undefined4 in_PR;
  char in_FPSCR_SZ;
  undefined1 local_c [4];
  undefined1 local_8 [4];
  
  if (in_FPSCR_SZ == '\0') {
    puVar1 = local_8;
  }
  else {
    puVar1 = local_c;
  }
  *(undefined4 *)(puVar1 + -4) = in_PR;
  (*(code *)PTR_FUN_8c014d04)(0,8);
  (*(code *)PTR_FUN_8c014d04)(1,6);
  (*(code *)PTR_FUN_8c014d00)(PTR_PTR_8c014d18);
  FUN_8c015220();
  FUN_8c015566();
  uVar4 = 0;
  if (*DAT_8c014d1c == 0) {
    uVar2 = in_xd14;
    if (in_FPSCR_SZ == '\0') {
      uVar2 = param_1 & 0xffffffff;
    }
    FUN_8c015594(uVar2,5,0xffffffd0,param_2 == 0);
    uVar3 = in_xd14;
    if (in_FPSCR_SZ == '\0') {
      uVar3 = CONCAT44(uVar4,(int)uVar2);
    }
    FUN_8c015594(uVar3,4,0,param_2 == 1);
    if (in_FPSCR_SZ == '\0') {
      in_xd14 = CONCAT44(uVar4,(int)uVar3);
    }
    uVar4 = 6;
  }
  else {
    uVar2 = in_xd14;
    if (in_FPSCR_SZ == '\0') {
      uVar2 = param_1 & 0xffffffff;
    }
    FUN_8c015594(uVar2,0xc,0xffffffd0,param_2 == 0);
    uVar3 = in_xd14;
    if (in_FPSCR_SZ == '\0') {
      uVar3 = CONCAT44(uVar4,(int)uVar2);
    }
    FUN_8c015594(uVar3,0xb,0,param_2 == 1);
    if (in_FPSCR_SZ == '\0') {
      in_xd14 = CONCAT44(uVar4,(int)uVar3);
    }
    uVar4 = 0xd;
  }
  FUN_8c015594(in_xd14,uVar4,0x30,param_2 == 2);
  uVar4 = *(undefined4 *)(PTR_DAT_8c014ec0 + 4);
  *(undefined4 *)(puVar1 + -0x14) = *(undefined4 *)PTR_DAT_8c014ec0;
  *(undefined4 *)(puVar1 + -0x10) = uVar4;
  uVar4 = *(undefined4 *)(PTR_DAT_8c014ec0 + 0xc);
  *(undefined4 *)(puVar1 + -0xc) = *(undefined4 *)(PTR_DAT_8c014ec0 + 8);
  *(undefined4 *)(puVar1 + -8) = uVar4;
  (*(code *)PTR_FUN_8c014ec4)(puVar1 + -0x14);
  return;
}


/* Address: 8c014d6e */

void FUN_8c014d6e(ulonglong param_1,int param_2)

{
  undefined1 *puVar1;
  ulonglong uVar2;
  undefined4 uVar3;
  ulonglong in_xd14;
  undefined4 in_PR;
  char in_FPSCR_SZ;
  undefined1 local_c [4];
  undefined1 local_8 [4];
  
  if (in_FPSCR_SZ == '\0') {
    puVar1 = local_8;
  }
  else {
    puVar1 = local_c;
  }
  *(undefined4 *)(puVar1 + -4) = in_PR;
  (*(code *)PTR_FUN_8c014ec8)(0,8);
  (*(code *)PTR_FUN_8c014ec8)(1,6);
  (*(code *)PTR_FUN_8c014ed0)(PTR_PTR_8c014ecc);
  FUN_8c015220();
  FUN_8c015566();
  uVar3 = 0;
  if (*DAT_8c014ed4 == 0) {
    uVar2 = in_xd14;
    if (in_FPSCR_SZ == '\0') {
      uVar2 = param_1 & 0xffffffff;
    }
    FUN_8c015594(uVar2,5,0xffffffe0,param_2 == 0);
    if (in_FPSCR_SZ == '\0') {
      in_xd14 = CONCAT44(uVar3,(int)uVar2);
    }
    uVar3 = 6;
  }
  else {
    uVar2 = in_xd14;
    if (in_FPSCR_SZ == '\0') {
      uVar2 = param_1 & 0xffffffff;
    }
    FUN_8c015594(uVar2,0xc,0xffffffe0,param_2 == 0);
    if (in_FPSCR_SZ == '\0') {
      in_xd14 = CONCAT44(uVar3,(int)uVar2);
    }
    uVar3 = 0xd;
  }
  FUN_8c015594(in_xd14,uVar3,0x20,param_2 == 1);
  uVar3 = *(undefined4 *)(PTR_DAT_8c014ed8 + 4);
  *(undefined4 *)(puVar1 + -0x14) = *(undefined4 *)PTR_DAT_8c014ed8;
  *(undefined4 *)(puVar1 + -0x10) = uVar3;
  uVar3 = *(undefined4 *)(PTR_DAT_8c014ed8 + 0xc);
  *(undefined4 *)(puVar1 + -0xc) = *(undefined4 *)(PTR_DAT_8c014ed8 + 8);
  *(undefined4 *)(puVar1 + -8) = uVar3;
  (*(code *)PTR_FUN_8c014ec4)(puVar1 + -0x14);
  return;
}


/* Address: 8c014dfc */

void FUN_8c014dfc(void)

{
  undefined4 local_14;
  undefined4 local_10;
  undefined4 local_c;
  undefined4 local_8;
  
  (*(code *)PTR_FUN_8c014ec8)(0,8);
  (*(code *)PTR_FUN_8c014ec8)(1,6);
  (*(code *)PTR_FUN_8c014ed0)(PTR_PTR_8c014ecc);
  FUN_8c015220();
  FUN_8c015566();
  local_10 = *(undefined4 *)(PTR_DAT_8c014edc + 4);
  local_14 = *(undefined4 *)PTR_DAT_8c014edc;
  local_8 = *(undefined4 *)(PTR_DAT_8c014edc + 0xc);
  local_c = *(undefined4 *)(PTR_DAT_8c014edc + 8);
  (*(code *)PTR_FUN_8c014ec4)(&local_14);
  return;
}


/* Address: 8c014e42 */

/* WARNING: Removing unreachable block (ram,0x8c015134) */
/* WARNING: Removing unreachable block (ram,0x8c01516c) */
/* WARNING: Removing unreachable block (ram,0x8c01515c) */
/* WARNING: Removing unreachable block (ram,0x8c015152) */
/* WARNING: Removing unreachable block (ram,0x8c015156) */
/* WARNING: Removing unreachable block (ram,0x8c015158) */
/* WARNING: Removing unreachable block (ram,0x8c01515a) */
/* WARNING: Removing unreachable block (ram,0x8c01515e) */
/* WARNING: Removing unreachable block (ram,0x8c015160) */
/* WARNING: Removing unreachable block (ram,0x8c015162) */
/* WARNING: Removing unreachable block (ram,0x8c015164) */
/* WARNING: Removing unreachable block (ram,0x8c015166) */
/* WARNING: Removing unreachable block (ram,0x8c015168) */
/* WARNING: Removing unreachable block (ram,0x8c01516a) */
/* WARNING: Removing unreachable block (ram,0x8c01516e) */
/* WARNING: Removing unreachable block (ram,0x8c015170) */
/* WARNING: Removing unreachable block (ram,0x8c015172) */
/* WARNING: Removing unreachable block (ram,0x8c015174) */
/* WARNING: Removing unreachable block (ram,0x8c0151ac) */
/* WARNING: Removing unreachable block (ram,0x8c01506e) */
/* WARNING: Removing unreachable block (ram,0x8c0150f4) */
/* WARNING: Removing unreachable block (ram,0x8c0150e6) */
/* WARNING: Removing unreachable block (ram,0x8c01505c) */
/* WARNING: Removing unreachable block (ram,0x8c0150dc) */
/* WARNING: Removing unreachable block (ram,0x8c0150e0) */
/* WARNING: Removing unreachable block (ram,0x8c0150e2) */
/* WARNING: Removing unreachable block (ram,0x8c0150e4) */
/* WARNING: Removing unreachable block (ram,0x8c0150e8) */
/* WARNING: Removing unreachable block (ram,0x8c0150ea) */
/* WARNING: Removing unreachable block (ram,0x8c0150ec) */
/* WARNING: Removing unreachable block (ram,0x8c0150ee) */
/* WARNING: Removing unreachable block (ram,0x8c0150f0) */
/* WARNING: Removing unreachable block (ram,0x8c0150f2) */
/* WARNING: Removing unreachable block (ram,0x8c0150f6) */
/* WARNING: Removing unreachable block (ram,0x8c0150f8) */
/* WARNING: Removing unreachable block (ram,0x8c0150fa) */
/* WARNING: Removing unreachable block (ram,0x8c0150fc) */
/* WARNING: Removing unreachable block (ram,0x8c015060) */
/* WARNING: Removing unreachable block (ram,0x8c0150c8) */
/* WARNING: Removing unreachable block (ram,0x8c014fb6) */
/* WARNING: Removing unreachable block (ram,0x8c014f9e) */
/* WARNING: Removing unreachable block (ram,0x8c014fa2) */
/* WARNING: Removing unreachable block (ram,0x8c014fa4) */
/* WARNING: Removing unreachable block (ram,0x8c014fa6) */
/* WARNING: Removing unreachable block (ram,0x8c014fa8) */
/* WARNING: Removing unreachable block (ram,0x8c014fe0) */
/* WARNING: Removing unreachable block (ram,0x8c014fd0) */
/* WARNING: Removing unreachable block (ram,0x8c014f1a) */
/* WARNING: Removing unreachable block (ram,0x8c014f00) */
/* WARNING: Removing unreachable block (ram,0x8c014f04) */
/* WARNING: Removing unreachable block (ram,0x8c014f06) */
/* WARNING: Removing unreachable block (ram,0x8c014f08) */
/* WARNING: Removing unreachable block (ram,0x8c014f0a) */
/* WARNING: Removing unreachable block (ram,0x8c014e92) */
/* WARNING: Removing unreachable block (ram,0x8c014e84) */
/* WARNING: Removing unreachable block (ram,0x8c014f68) */
/* WARNING: Removing unreachable block (ram,0x8c014f50) */
/* WARNING: Removing unreachable block (ram,0x8c014f54) */
/* WARNING: Removing unreachable block (ram,0x8c014f56) */
/* WARNING: Removing unreachable block (ram,0x8c014f58) */
/* WARNING: Removing unreachable block (ram,0x8c014f5a) */
/* WARNING: Removing unreachable block (ram,0x8c014f5c) */
/* WARNING: Removing unreachable block (ram,0x8c014f5e) */
/* WARNING: Removing unreachable block (ram,0x8c014f60) */
/* WARNING: Removing unreachable block (ram,0x8c014f62) */
/* WARNING: Removing unreachable block (ram,0x8c014f64) */
/* WARNING: Removing unreachable block (ram,0x8c014f66) */
/* WARNING: Removing unreachable block (ram,0x8c014f6a) */
/* WARNING: Removing unreachable block (ram,0x8c014f6c) */
/* WARNING: Removing unreachable block (ram,0x8c014f6e) */
/* WARNING: Removing unreachable block (ram,0x8c014f70) */
/* WARNING: Removing unreachable block (ram,0x8c014e6a) */
/* WARNING: Removing unreachable block (ram,0x8c014e6e) */
/* WARNING: Removing unreachable block (ram,0x8c014e72) */
/* WARNING: Removing unreachable block (ram,0x8c014e76) */
/* WARNING: Removing unreachable block (ram,0x8c014f26) */
/* WARNING: Removing unreachable block (ram,0x8c014f2a) */
/* WARNING: Removing unreachable block (ram,0x8c014f2c) */
/* WARNING: Removing unreachable block (ram,0x8c014f2e) */
/* WARNING: Removing unreachable block (ram,0x8c014f30) */
/* WARNING: Removing unreachable block (ram,0x8c014f32) */
/* WARNING: Removing unreachable block (ram,0x8c014f34) */
/* WARNING: Removing unreachable block (ram,0x8c014f36) */
/* WARNING: Removing unreachable block (ram,0x8c014f38) */
/* WARNING: Removing unreachable block (ram,0x8c014f3a) */
/* WARNING: Removing unreachable block (ram,0x8c014f3c) */
/* WARNING: Removing unreachable block (ram,0x8c014f3e) */
/* WARNING: Removing unreachable block (ram,0x8c014f40) */
/* WARNING: Removing unreachable block (ram,0x8c014f42) */
/* WARNING: Removing unreachable block (ram,0x8c014f44) */
/* WARNING: Removing unreachable block (ram,0x8c014f46) */
/* WARNING: Removing unreachable block (ram,0x8c014f48) */
/* WARNING: Removing unreachable block (ram,0x8c014f0c) */
/* WARNING: Removing unreachable block (ram,0x8c014f0e) */
/* WARNING: Removing unreachable block (ram,0x8c014f10) */
/* WARNING: Removing unreachable block (ram,0x8c014f12) */
/* WARNING: Removing unreachable block (ram,0x8c014f14) */
/* WARNING: Removing unreachable block (ram,0x8c014f16) */
/* WARNING: Removing unreachable block (ram,0x8c014f18) */
/* WARNING: Removing unreachable block (ram,0x8c014f1c) */
/* WARNING: Removing unreachable block (ram,0x8c014f1e) */
/* WARNING: Removing unreachable block (ram,0x8c014f20) */
/* WARNING: Removing unreachable block (ram,0x8c014f22) */
/* WARNING: Removing unreachable block (ram,0x8c014f72) */
/* WARNING: Removing unreachable block (ram,0x8c014f74) */
/* WARNING: Removing unreachable block (ram,0x8c014f92) */
/* WARNING: Removing unreachable block (ram,0x8c014fec) */
/* WARNING: Removing unreachable block (ram,0x8c014ff0) */
/* WARNING: Removing unreachable block (ram,0x8c014ff2) */
/* WARNING: Removing unreachable block (ram,0x8c014ff4) */
/* WARNING: Removing unreachable block (ram,0x8c014ff6) */
/* WARNING: Removing unreachable block (ram,0x8c014ff8) */
/* WARNING: Removing unreachable block (ram,0x8c014ffa) */
/* WARNING: Removing unreachable block (ram,0x8c014ffc) */
/* WARNING: Removing unreachable block (ram,0x8c014ffe) */
/* WARNING: Removing unreachable block (ram,0x8c015000) */
/* WARNING: Removing unreachable block (ram,0x8c015002) */
/* WARNING: Removing unreachable block (ram,0x8c015004) */
/* WARNING: Removing unreachable block (ram,0x8c015006) */
/* WARNING: Removing unreachable block (ram,0x8c015008) */
/* WARNING: Removing unreachable block (ram,0x8c01500a) */
/* WARNING: Removing unreachable block (ram,0x8c01500c) */
/* WARNING: Removing unreachable block (ram,0x8c01500e) */
/* WARNING: Removing unreachable block (ram,0x8c014f96) */
/* WARNING: Removing unreachable block (ram,0x8c015016) */
/* WARNING: Removing unreachable block (ram,0x8c01501a) */
/* WARNING: Removing unreachable block (ram,0x8c01501c) */
/* WARNING: Removing unreachable block (ram,0x8c01501e) */
/* WARNING: Removing unreachable block (ram,0x8c015020) */
/* WARNING: Removing unreachable block (ram,0x8c014f9a) */
/* WARNING: Removing unreachable block (ram,0x8c015022) */
/* WARNING: Removing unreachable block (ram,0x8c015024) */
/* WARNING: Removing unreachable block (ram,0x8c015026) */
/* WARNING: Removing unreachable block (ram,0x8c015028) */
/* WARNING: Removing unreachable block (ram,0x8c01502a) */
/* WARNING: Removing unreachable block (ram,0x8c01502c) */
/* WARNING: Removing unreachable block (ram,0x8c01502e) */
/* WARNING: Removing unreachable block (ram,0x8c015030) */
/* WARNING: Removing unreachable block (ram,0x8c015032) */
/* WARNING: Removing unreachable block (ram,0x8c015034) */
/* WARNING: Removing unreachable block (ram,0x8c015036) */
/* WARNING: Removing unreachable block (ram,0x8c014faa) */
/* WARNING: Removing unreachable block (ram,0x8c014fac) */
/* WARNING: Removing unreachable block (ram,0x8c014fae) */
/* WARNING: Removing unreachable block (ram,0x8c014fb0) */
/* WARNING: Removing unreachable block (ram,0x8c014fb2) */
/* WARNING: Removing unreachable block (ram,0x8c014fb4) */
/* WARNING: Removing unreachable block (ram,0x8c014fb8) */
/* WARNING: Removing unreachable block (ram,0x8c014fba) */
/* WARNING: Removing unreachable block (ram,0x8c014fbc) */
/* WARNING: Removing unreachable block (ram,0x8c014fbe) */
/* WARNING: Removing unreachable block (ram,0x8c014fc0) */
/* WARNING: Removing unreachable block (ram,0x8c014fc2) */
/* WARNING: Removing unreachable block (ram,0x8c015010) */
/* WARNING: Removing unreachable block (ram,0x8c015012) */
/* WARNING: Removing unreachable block (ram,0x8c0150bc) */
/* WARNING: Removing unreachable block (ram,0x8c01508c) */
/* WARNING: Removing unreachable block (ram,0x8c015090) */
/* WARNING: Removing unreachable block (ram,0x8c015092) */
/* WARNING: Removing unreachable block (ram,0x8c015094) */
/* WARNING: Removing unreachable block (ram,0x8c015096) */
/* WARNING: Removing unreachable block (ram,0x8c015098) */
/* WARNING: Removing unreachable block (ram,0x8c01509a) */
/* WARNING: Removing unreachable block (ram,0x8c01509c) */
/* WARNING: Removing unreachable block (ram,0x8c01509e) */
/* WARNING: Removing unreachable block (ram,0x8c0150a0) */
/* WARNING: Removing unreachable block (ram,0x8c0150a2) */
/* WARNING: Removing unreachable block (ram,0x8c0150a4) */
/* WARNING: Removing unreachable block (ram,0x8c0150a6) */
/* WARNING: Removing unreachable block (ram,0x8c0150a8) */
/* WARNING: Removing unreachable block (ram,0x8c0150aa) */
/* WARNING: Removing unreachable block (ram,0x8c0150ac) */
/* WARNING: Removing unreachable block (ram,0x8c0150ae) */
/* WARNING: Removing unreachable block (ram,0x8c0150fe) */
/* WARNING: Removing unreachable block (ram,0x8c015100) */
/* WARNING: Removing unreachable block (ram,0x8c015064) */
/* WARNING: Removing unreachable block (ram,0x8c015068) */
/* WARNING: Removing unreachable block (ram,0x8c01506a) */
/* WARNING: Removing unreachable block (ram,0x8c01506c) */
/* WARNING: Removing unreachable block (ram,0x8c015070) */
/* WARNING: Removing unreachable block (ram,0x8c015072) */
/* WARNING: Removing unreachable block (ram,0x8c015074) */
/* WARNING: Removing unreachable block (ram,0x8c015076) */
/* WARNING: Removing unreachable block (ram,0x8c015078) */
/* WARNING: Removing unreachable block (ram,0x8c01507a) */
/* WARNING: Removing unreachable block (ram,0x8c01507c) */
/* WARNING: Removing unreachable block (ram,0x8c01507e) */
/* WARNING: Removing unreachable block (ram,0x8c015080) */
/* WARNING: Removing unreachable block (ram,0x8c015082) */
/* WARNING: Removing unreachable block (ram,0x8c015084) */
/* WARNING: Removing unreachable block (ram,0x8c015086) */
/* WARNING: Removing unreachable block (ram,0x8c015088) */
/* WARNING: Removing unreachable block (ram,0x8c0151ba) */
/* WARNING: Removing unreachable block (ram,0x8c015178) */
/* WARNING: Removing unreachable block (ram,0x8c01517c) */
/* WARNING: Removing unreachable block (ram,0x8c01517e) */
/* WARNING: Removing unreachable block (ram,0x8c015180) */
/* WARNING: Removing unreachable block (ram,0x8c015182) */
/* WARNING: Removing unreachable block (ram,0x8c015184) */
/* WARNING: Removing unreachable block (ram,0x8c015186) */
/* WARNING: Removing unreachable block (ram,0x8c015188) */
/* WARNING: Removing unreachable block (ram,0x8c01518a) */
/* WARNING: Removing unreachable block (ram,0x8c01518c) */
/* WARNING: Removing unreachable block (ram,0x8c01518e) */
/* WARNING: Removing unreachable block (ram,0x8c015190) */
/* WARNING: Removing unreachable block (ram,0x8c015192) */
/* WARNING: Removing unreachable block (ram,0x8c015194) */
/* WARNING: Removing unreachable block (ram,0x8c015196) */
/* WARNING: Removing unreachable block (ram,0x8c015198) */
/* WARNING: Removing unreachable block (ram,0x8c01519a) */
/* WARNING: Removing unreachable block (ram,0x8c01512a) */
/* WARNING: Removing unreachable block (ram,0x8c01512e) */
/* WARNING: Removing unreachable block (ram,0x8c015130) */
/* WARNING: Removing unreachable block (ram,0x8c015132) */
/* WARNING: Removing unreachable block (ram,0x8c015136) */
/* WARNING: Removing unreachable block (ram,0x8c015138) */
/* WARNING: Removing unreachable block (ram,0x8c01513a) */
/* WARNING: Removing unreachable block (ram,0x8c01513c) */
/* WARNING: Removing unreachable block (ram,0x8c01513e) */
/* WARNING: Removing unreachable block (ram,0x8c015140) */
/* WARNING: Removing unreachable block (ram,0x8c015142) */
/* WARNING: Removing unreachable block (ram,0x8c015144) */
/* WARNING: Removing unreachable block (ram,0x8c015146) */
/* WARNING: Removing unreachable block (ram,0x8c015148) */
/* WARNING: Removing unreachable block (ram,0x8c01514a) */
/* WARNING: Removing unreachable block (ram,0x8c01514c) */
/* WARNING: Removing unreachable block (ram,0x8c01514e) */
/* WARNING: Removing unreachable block (ram,0x8c01519c) */
/* WARNING: Removing unreachable block (ram,0x8c01519e) */
/* WARNING: Removing unreachable block (ram,0x8c015126) */

void FUN_8c014e42(int param_1,float *param_2,float *param_3)

{
  int *piVar1;
  double in_dr0;
  float fVar2;
  double dVar3;
  double dVar4;
  float fVar5;
  float in_xf0;
  undefined4 in_xf4;
  char in_FPSCR_PR;
  char in_FPSCR_SZ;
  float local_78;
  undefined4 uStack_74;
  float fStack_70;
  undefined4 uStack_6c;
  undefined4 uStack_68;
  int local_64;
  float local_60;
  float fStack_5c;
  float fStack_58;
  undefined4 uStack_54;
  undefined4 uStack_50;
  int local_4c;
  float local_48;
  undefined4 uStack_44;
  float fStack_40;
  undefined4 uStack_3c;
  undefined4 uStack_38;
  int local_34;
  float local_30;
  float fStack_2c;
  float fStack_28;
  undefined4 uStack_24;
  undefined4 uStack_20;
  int local_1c;
  
  param_1 = param_1 * 0x1c;
  piVar1 = (int *)(PTR_DAT_8c014ee0 + param_1);
  if (in_FPSCR_SZ == '\0') {
    fVar5 = *param_3;
    in_dr0 = (double)CONCAT44((int)((ulonglong)in_dr0 >> 0x20),*param_2);
    local_60 = in_xf0;
  }
  else {
    fVar5 = (float)((ulonglong)*(undefined8 *)param_3 >> 0x20);
    local_60 = (float)*(undefined8 *)param_2;
  }
  dVar4 = (double)CONCAT44(fVar5,(float)*piVar1);
  if (in_FPSCR_PR == '\0') {
    dVar4 = (double)CONCAT44((int)((ulonglong)in_dr0 >> 0x20),
                             SUB84(in_dr0,0) - (float)*piVar1 * fVar5);
  }
  else {
    dVar4 = in_dr0 - dVar4 * dVar4;
  }
  local_78 = local_60;
  if (in_FPSCR_SZ == '\0') {
    local_78 = SUB84(dVar4,0);
  }
  if (in_FPSCR_SZ == '\0') {
    fVar5 = param_3[1];
  }
  else {
    fVar5 = (float)((ulonglong)*(undefined8 *)(param_3 + 1) >> 0x20);
  }
  dVar3 = (double)CONCAT44(fVar5,(float)piVar1[1]);
  if (in_FPSCR_PR == '\0') {
    fVar5 = (float)piVar1[1] * fVar5;
  }
  else {
    fVar5 = SUB84(dVar3 * dVar3,0);
  }
  if (in_FPSCR_SZ == '\0') {
    dVar3 = (double)CONCAT44(param_2[1],fVar5);
  }
  else {
    dVar3 = *(double *)(param_2 + 1);
  }
  if (in_FPSCR_PR == '\0') {
    dVar3 = (double)CONCAT44((float)((ulonglong)dVar3 >> 0x20) - SUB84(dVar3,0),SUB84(dVar3,0));
  }
  else {
    dVar3 = dVar3 - dVar3;
  }
  if (in_FPSCR_SZ == '\0') {
    uStack_74 = (undefined4)((ulonglong)dVar3 >> 0x20);
    uStack_6c = 0;
    uStack_68 = 0;
    fStack_70 = param_2[2];
  }
  else {
    uStack_74 = SUB84(dVar3,0);
    uStack_68 = 0x3f800000;
    fStack_70 = (float)*(undefined8 *)(param_2 + 2);
    uStack_6c = (undefined4)((ulonglong)*(undefined8 *)(param_2 + 2) >> 0x20);
  }
  local_64 = piVar1[2];
  piVar1 = (int *)(PTR_DAT_8c014ee0 + param_1);
  if (in_FPSCR_SZ == '\0') {
    fVar5 = *param_3;
    dVar4 = (double)CONCAT44((int)((ulonglong)dVar4 >> 0x20),*param_2);
  }
  else {
    fVar5 = (float)((ulonglong)*(undefined8 *)param_3 >> 0x20);
    local_60 = (float)*(undefined8 *)param_2;
  }
  dVar3 = (double)CONCAT44(fVar5,(float)*piVar1);
  if (in_FPSCR_PR == '\0') {
    fVar5 = SUB84(dVar4,0) - (float)*piVar1 * fVar5;
  }
  else {
    fVar5 = SUB84(dVar4 - dVar3 * dVar3,0);
  }
  if (in_FPSCR_SZ == '\0') {
    local_60 = fVar5;
  }
  if (in_FPSCR_SZ == '\0') {
    fVar5 = param_3[1];
    fVar2 = param_2[1];
  }
  else {
    fVar5 = (float)((ulonglong)*(undefined8 *)(param_3 + 1) >> 0x20);
    fVar2 = (float)((ulonglong)*(undefined8 *)(param_2 + 1) >> 0x20);
  }
  fStack_5c = (float)piVar1[1];
  if (in_FPSCR_PR == '\0') {
    fVar2 = fVar5 * fStack_5c + fVar2;
  }
  if (in_FPSCR_SZ == '\0') {
    uStack_54 = 0;
    uStack_50 = 0x3f800000;
    fStack_58 = param_2[2];
    fStack_5c = fVar2;
  }
  else {
    fStack_58 = (float)*(undefined8 *)(param_2 + 2);
    uStack_54 = (undefined4)((ulonglong)*(undefined8 *)(param_2 + 2) >> 0x20);
    uStack_50 = in_xf4;
  }
  local_4c = piVar1[3];
  piVar1 = (int *)(PTR_DAT_8c014ee0 + param_1);
  if (in_FPSCR_SZ == '\0') {
    fVar5 = *param_2;
    fVar2 = *param_3;
  }
  else {
    fVar5 = (float)((ulonglong)*(undefined8 *)param_2 >> 0x20);
    fVar2 = (float)((ulonglong)*(undefined8 *)param_3 >> 0x20);
  }
  local_48 = (float)*piVar1;
  if (in_FPSCR_PR == '\0') {
    fVar5 = fVar2 * local_48 + fVar5;
  }
  if (in_FPSCR_SZ == '\0') {
    local_48 = fVar5;
  }
  if (in_FPSCR_SZ == '\0') {
    fVar5 = param_3[1];
  }
  else {
    fVar5 = (float)((ulonglong)*(undefined8 *)(param_3 + 1) >> 0x20);
  }
  dVar4 = (double)CONCAT44(fVar5,(float)piVar1[1]);
  if (in_FPSCR_PR == '\0') {
    fVar5 = (float)piVar1[1] * fVar5;
  }
  else {
    fVar5 = SUB84(dVar4 * dVar4,0);
  }
  if (in_FPSCR_SZ == '\0') {
    dVar4 = (double)CONCAT44(param_2[1],fVar5);
  }
  else {
    dVar4 = *(double *)(param_2 + 1);
  }
  if (in_FPSCR_PR == '\0') {
    dVar4 = (double)CONCAT44((float)((ulonglong)dVar4 >> 0x20) - SUB84(dVar4,0),SUB84(dVar4,0));
  }
  else {
    dVar4 = dVar4 - dVar4;
  }
  if (in_FPSCR_SZ == '\0') {
    uStack_44 = (undefined4)((ulonglong)dVar4 >> 0x20);
    uStack_3c = 0x3f800000;
    uStack_38 = 0;
    fStack_40 = param_2[2];
  }
  else {
    uStack_44 = SUB84(dVar4,0);
    uStack_38 = 0x3f800000;
    fStack_40 = (float)*(undefined8 *)(param_2 + 2);
    uStack_3c = (undefined4)((ulonglong)*(undefined8 *)(param_2 + 2) >> 0x20);
  }
  local_34 = piVar1[4];
  piVar1 = (int *)(PTR_DAT_8c014ee0 + param_1);
  if (in_FPSCR_SZ == '\0') {
    fVar5 = *param_2;
    fVar2 = *param_3;
  }
  else {
    fVar5 = (float)((ulonglong)*(undefined8 *)param_2 >> 0x20);
    fVar2 = (float)((ulonglong)*(undefined8 *)param_3 >> 0x20);
  }
  local_30 = (float)*piVar1;
  if (in_FPSCR_PR == '\0') {
    fVar5 = fVar2 * local_30 + fVar5;
  }
  if (in_FPSCR_SZ == '\0') {
    local_30 = fVar5;
  }
  if (in_FPSCR_SZ == '\0') {
    fVar5 = param_3[1];
    fVar2 = param_2[1];
  }
  else {
    fVar5 = (float)((ulonglong)*(undefined8 *)(param_3 + 1) >> 0x20);
    fVar2 = (float)((ulonglong)*(undefined8 *)(param_2 + 1) >> 0x20);
  }
  fStack_2c = (float)piVar1[1];
  if (in_FPSCR_PR == '\0') {
    fVar2 = fVar5 * fStack_2c + fVar2;
  }
  if (in_FPSCR_SZ == '\0') {
    uStack_20 = 0x3f800000;
    fStack_28 = param_2[2];
    uStack_24 = uStack_20;
    fStack_2c = fVar2;
  }
  else {
    fStack_28 = (float)*(undefined8 *)(param_2 + 2);
    uStack_24 = (undefined4)((ulonglong)*(undefined8 *)(param_2 + 2) >> 0x20);
    uStack_20 = in_xf4;
  }
  local_1c = piVar1[5];
  (*(code *)PTR_FUN_8c015200)(&local_78,4,*(undefined4 *)(PTR_DAT_8c014ee0 + param_1 + 0x18),1);
  return;
}


/* Address: 8c015220 */

/* WARNING: Removing unreachable block (ram,0x8c0153ce) */
/* WARNING: Removing unreachable block (ram,0x8c015322) */
/* WARNING: Removing unreachable block (ram,0x8c0152c6) */
/* WARNING: Removing unreachable block (ram,0x8c0152b4) */
/* WARNING: Removing unreachable block (ram,0x8c0152e6) */
/* WARNING: Removing unreachable block (ram,0x8c015368) */
/* WARNING: Removing unreachable block (ram,0x8c015306) */
/* WARNING: Removing unreachable block (ram,0x8c0153a6) */
/* WARNING: Removing unreachable block (ram,0x8c015338) */
/* WARNING: Removing unreachable block (ram,0x8c01534a) */
/* WARNING: Removing unreachable block (ram,0x8c01538a) */
/* WARNING: Removing unreachable block (ram,0x8c0153e0) */

void FUN_8c015220(double param_1)

{
  undefined *puVar1;
  undefined *puVar2;
  int iVar3;
  undefined4 uVar4;
  undefined1 *puVar5;
  byte *pbVar6;
  undefined1 *puVar7;
  short *psVar8;
  undefined8 *puVar9;
  double dVar10;
  double dVar12;
  double dVar13;
  undefined8 in_dr12;
  double dVar14;
  undefined8 in_dr14;
  undefined8 uVar15;
  undefined4 uVar16;
  double in_xd2;
  undefined8 in_xd14;
  undefined4 in_PR;
  undefined1 uVar17;
  char in_FPSCR_PR;
  char in_FPSCR_SZ;
  undefined1 auStack_3c [16];
  undefined8 uStack_2c;
  undefined8 uStack_24;
  float fVar11;
  
  puVar2 = PTR_DAT_8c0154a4;
  puVar1 = PTR_DAT_8c0154a0;
  if (in_FPSCR_SZ == '\0') {
    uStack_24 = CONCAT44((int)in_dr14,(undefined4)uStack_24);
                    /* WARNING: Ignoring partial resolution of indirect */
    uStack_24._0_4_ = (int)((ulonglong)in_dr14 >> 0x20);
    puVar9 = &uStack_2c;
    uStack_2c = CONCAT44((int)in_dr12,(int)((ulonglong)in_dr12 >> 0x20));
  }
  else {
    puVar9 = (undefined8 *)auStack_3c;
    uStack_24 = in_xd14;
    uStack_2c = in_dr14;
  }
  *(undefined4 *)((int)puVar9 + -4) = in_PR;
  puVar5 = (undefined1 *)((int)puVar9 + -0x18);
  psVar8 = (short *)((int)puVar9 + -8);
  *(undefined2 *)((int)puVar9 + -6) = 0;
  puVar7 = (undefined1 *)((int)puVar9 + -0x14);
  *(undefined1 **)((int)puVar9 + -0x5c) = puVar5;
  *(undefined1 **)((int)puVar9 + -0x7c) = puVar7;
  *(undefined1 **)((int)puVar9 + -0x6c) = puVar5;
  *(undefined1 **)((int)puVar9 + -0x30) = puVar7;
  *(undefined1 **)((int)puVar9 + -0x4c) = puVar7;
  *(undefined1 **)((int)puVar9 + -0x34) = puVar5;
  *(undefined1 **)((int)puVar9 + -0x74) = puVar7;
  *(undefined1 **)((int)puVar9 + -0x54) = puVar5;
  *(undefined1 **)((int)puVar9 + -0x40) = puVar5;
  *(undefined1 **)((int)puVar9 + -0x38) = puVar7;
  *(undefined1 **)((int)puVar9 + -0x3c) = puVar5;
  *(undefined1 **)((int)puVar9 + -0x44) = puVar7;
  *(undefined1 **)((int)puVar9 + -0x48) = puVar7;
  *(undefined1 **)((int)puVar9 + -0x50) = puVar5;
  *(undefined1 **)((int)puVar9 + -0x58) = puVar7;
  *(undefined1 **)((int)puVar9 + -0x60) = puVar5;
  *(undefined1 **)((int)puVar9 + -100) = puVar5;
  *(undefined1 **)((int)puVar9 + -0x68) = puVar7;
  *(undefined1 **)((int)puVar9 + -0x70) = puVar5;
  *(undefined1 **)((int)puVar9 + -0x78) = puVar7;
  pbVar6 = PTR_DAT_8c0154a4 + 4;
  if (*(short *)((int)puVar9 + -6) < 0xc) {
    if (in_FPSCR_SZ == '\0') {
      dVar14 = (double)CONCAT44((undefined4)DAT_8c0154a8,DAT_8c0154a8._4_4_);
    }
    else {
      dVar14 = (double)CONCAT44(DAT_8c0154a8._4_4_,(undefined4)DAT_8c0154a8);
    }
    if (in_FPSCR_PR == '\0') {
      uVar15 = 0xbf8000003f800000;
    }
    else {
      uVar15 = 0xbf8000003f800000;
    }
    do {
      iVar3 = (uint)(byte)puVar2[7] - (uint)(byte)puVar2[3];
      dVar13 = in_xd2;
      if (in_FPSCR_SZ == '\0') {
        dVar13 = (double)CONCAT44((float)(int)*(short *)((int)puVar9 + -6),SUB84(param_1,0));
      }
      if (in_FPSCR_PR == '\0') {
        dVar13 = (double)CONCAT44((float)((ulonglong)dVar13 >> 0x20) * SUB84(dVar14,0),
                                  SUB84(dVar13,0));
        dVar12 = (double)CONCAT44((float)iVar3,(float)(byte)puVar2[3]);
      }
      else {
        dVar13 = dVar13 * dVar14;
        dVar12 = (double)iVar3;
      }
      dVar10 = dVar13;
      if (in_FPSCR_SZ == '\0') {
        dVar10 = (double)((ulonglong)dVar13 & 0xffffffff00000000);
      }
      fVar11 = (float)((ulonglong)dVar10 >> 0x20);
      if (in_FPSCR_PR == '\0') {
        uVar17 = (undefined1)(int)(fVar11 * (float)((ulonglong)dVar12 >> 0x20) + SUB84(dVar12,0));
      }
      else {
        uVar17 = (undefined1)(int)dVar12;
      }
      *(undefined1 *)((int)puVar9 + -0x15) = uVar17;
      iVar3 = (uint)(byte)puVar2[6] - (uint)(byte)puVar2[2];
      if (in_FPSCR_PR == '\0') {
        uVar17 = (undefined1)(int)(fVar11 * (float)iVar3 + (float)(byte)puVar2[2]);
      }
      else {
        uVar17 = (undefined1)(int)(double)iVar3;
      }
      *(undefined1 *)((int)puVar9 + -0x16) = uVar17;
      iVar3 = (uint)(byte)puVar2[5] - (uint)(byte)puVar2[1];
      if (in_FPSCR_PR == '\0') {
        uVar17 = (undefined1)(int)(fVar11 * (float)iVar3 + (float)(byte)puVar2[1]);
      }
      else {
        uVar17 = (undefined1)(int)(double)iVar3;
      }
      *(undefined1 *)((int)puVar9 + -0x17) = uVar17;
      iVar3 = (uint)*pbVar6 - (uint)(byte)*puVar2;
      if (in_FPSCR_PR == '\0') {
        uVar17 = (undefined1)(int)(fVar11 * (float)iVar3 + (float)(byte)*puVar2);
      }
      else {
        uVar17 = (undefined1)(int)(double)iVar3;
      }
      *puVar5 = uVar17;
      iVar3 = (uint)(byte)puVar2[7] - (uint)(byte)puVar2[3];
      param_1 = in_xd2;
      if (in_FPSCR_SZ == '\0') {
        param_1 = (double)CONCAT44((float)(*(short *)((int)puVar9 + -6) + 1),SUB84(dVar13,0));
      }
      if (in_FPSCR_PR == '\0') {
        param_1 = (double)CONCAT44((float)((ulonglong)param_1 >> 0x20) * SUB84(dVar14,0),
                                   SUB84(param_1,0));
        dVar13 = (double)CONCAT44((float)iVar3,(float)(byte)puVar2[3]);
      }
      else {
        param_1 = param_1 * dVar14;
        dVar13 = (double)iVar3;
      }
      dVar12 = param_1;
      if (in_FPSCR_SZ == '\0') {
        dVar12 = (double)((ulonglong)param_1 & 0xffffffff00000000);
      }
      fVar11 = (float)((ulonglong)dVar12 >> 0x20);
      if (in_FPSCR_PR == '\0') {
        uVar17 = (undefined1)(int)(fVar11 * (float)((ulonglong)dVar13 >> 0x20) + SUB84(dVar13,0));
      }
      else {
        uVar17 = (undefined1)(int)dVar13;
      }
      *(undefined1 *)((int)puVar9 + -0x11) = uVar17;
      iVar3 = (uint)(byte)puVar2[6] - (uint)(byte)puVar2[2];
      if (in_FPSCR_PR == '\0') {
        uVar17 = (undefined1)(int)(fVar11 * (float)iVar3 + (float)(byte)puVar2[2]);
      }
      else {
        uVar17 = (undefined1)(int)(double)iVar3;
      }
      *(undefined1 *)((int)puVar9 + -0x12) = uVar17;
      iVar3 = (uint)(byte)puVar2[5] - (uint)(byte)puVar2[1];
      if (in_FPSCR_PR == '\0') {
        uVar17 = (undefined1)(int)(fVar11 * (float)iVar3 + (float)(byte)puVar2[1]);
      }
      else {
        uVar17 = (undefined1)(int)(double)iVar3;
      }
      *(undefined1 *)((int)puVar9 + -0x13) = uVar17;
      iVar3 = (uint)*pbVar6 - (uint)(byte)*puVar2;
      if (in_FPSCR_PR == '\0') {
        uVar17 = (undefined1)(int)(fVar11 * (float)iVar3 + (float)(byte)*puVar2);
      }
      else {
        uVar17 = (undefined1)(int)(double)iVar3;
      }
      *puVar7 = uVar17;
      *psVar8 = 0;
      do {
        if (in_FPSCR_SZ == '\0') {
          *(float *)((int)puVar9 + -0x24) = (float)((*psVar8 + -8) * 0x10 + 0x140);
        }
        else {
          *(double *)((int)puVar9 + -0x24) = in_xd2;
        }
        if (in_FPSCR_SZ == '\0') {
          *(float *)((int)puVar9 + -0x20) =
               (float)((*(short *)((int)puVar9 + -6) + -6) * 0x10 + 0xf0);
          *(int *)((int)puVar9 + -0x1c) = (int)((ulonglong)dVar14 >> 0x20);
        }
        else {
          *(double *)((int)puVar9 + -0x20) = in_xd2;
          *(double *)((int)puVar9 + -0x1c) = dVar14;
        }
        uVar4 = (undefined4)uVar15;
        if ((*psVar8 == 0) && (*(short *)((int)puVar9 + -6) == 0)) {
          if (in_FPSCR_SZ == '\0') {
            *(undefined4 *)((int)puVar9 + -0x2c) = uVar4;
            *(undefined4 *)((int)puVar9 + -0x28) = uVar4;
          }
          else {
            *(undefined8 *)((int)puVar9 + -0x2c) = in_xd14;
            *(undefined8 *)((int)puVar9 + -0x28) = in_xd14;
          }
          *(undefined4 *)(puVar1 + 0xcc) = **(undefined4 **)((int)puVar9 + -0x5c);
          *(undefined4 *)(puVar1 + 0xd0) = **(undefined4 **)((int)puVar9 + -0x7c);
          *(undefined4 *)(puVar1 + 0xd4) = **(undefined4 **)((int)puVar9 + -0x6c);
          *(undefined4 *)(puVar1 + 0xd8) = **(undefined4 **)((int)puVar9 + -0x30);
LAB_8c01547c:
          FUN_8c014e42(7,(undefined1 *)((int)puVar9 + -0x24),(undefined1 *)((int)puVar9 + -0x2c));
        }
        else {
          uVar16 = (undefined4)((ulonglong)uVar15 >> 0x20);
          if ((*psVar8 == 0) && (*(short *)((int)puVar9 + -6) == 0xb)) {
            if (in_FPSCR_SZ == '\0') {
              *(undefined4 *)((int)puVar9 + -0x2c) = uVar4;
              *(undefined4 *)((int)puVar9 + -0x28) = uVar16;
            }
            else {
              *(undefined8 *)((int)puVar9 + -0x2c) = in_xd14;
              *(undefined8 *)((int)puVar9 + -0x28) = uVar15;
            }
            *(undefined4 *)(puVar1 + 0xcc) = **(undefined4 **)((int)puVar9 + -0x4c);
            *(undefined4 *)(puVar1 + 0xd0) = **(undefined4 **)((int)puVar9 + -0x34);
            *(undefined4 *)(puVar1 + 0xd4) = **(undefined4 **)((int)puVar9 + -0x74);
            *(undefined4 *)(puVar1 + 0xd8) = **(undefined4 **)((int)puVar9 + -0x54);
            goto LAB_8c01547c;
          }
          if (*psVar8 == 0xf) {
            if (*(short *)((int)puVar9 + -6) == 0) {
              if (in_FPSCR_SZ == '\0') {
                *(undefined4 *)((int)puVar9 + -0x2c) = uVar16;
                *(undefined4 *)((int)puVar9 + -0x28) = uVar4;
              }
              else {
                *(undefined8 *)((int)puVar9 + -0x2c) = uVar15;
                *(undefined8 *)((int)puVar9 + -0x28) = in_xd14;
              }
              *(undefined4 *)(puVar1 + 0xcc) = **(undefined4 **)((int)puVar9 + -0x40);
              *(undefined4 *)(puVar1 + 0xd0) = **(undefined4 **)((int)puVar9 + -0x38);
              *(undefined4 *)(puVar1 + 0xd4) = **(undefined4 **)((int)puVar9 + -0x3c);
              *(undefined4 *)(puVar1 + 0xd8) = **(undefined4 **)((int)puVar9 + -0x44);
              goto LAB_8c01547c;
            }
            if ((*psVar8 != 0xf) || (*(short *)((int)puVar9 + -6) != 0xb)) goto LAB_8c0154f8;
            if (in_FPSCR_SZ == '\0') {
              *(undefined4 *)((int)puVar9 + -0x2c) = uVar16;
              *(undefined4 *)((int)puVar9 + -0x28) = uVar16;
            }
            else {
              *(undefined8 *)((int)puVar9 + -0x2c) = uVar15;
              *(undefined8 *)((int)puVar9 + -0x28) = uVar15;
            }
            *(undefined4 *)(puVar1 + 0xcc) = **(undefined4 **)((int)puVar9 + -0x48);
            *(undefined4 *)(puVar1 + 0xd0) = **(undefined4 **)((int)puVar9 + -0x50);
            *(undefined4 *)(puVar1 + 0xd4) = **(undefined4 **)((int)puVar9 + -0x58);
            *(undefined4 *)(puVar1 + 0xd8) = **(undefined4 **)((int)puVar9 + -0x60);
            uVar4 = 7;
          }
          else {
LAB_8c0154f8:
            if (in_FPSCR_SZ == '\0') {
              *(undefined4 *)((int)puVar9 + -0x2c) = uVar4;
              *(undefined4 *)((int)puVar9 + -0x28) = uVar4;
            }
            else {
              *(undefined8 *)((int)puVar9 + -0x2c) = in_xd14;
              *(undefined8 *)((int)puVar9 + -0x28) = in_xd14;
            }
            uVar4 = 8;
            *(undefined4 *)(puVar1 + 0xe8) = **(undefined4 **)((int)puVar9 + -100);
            *(undefined4 *)(puVar1 + 0xec) = **(undefined4 **)((int)puVar9 + -0x68);
            *(undefined4 *)(puVar1 + 0xf0) = **(undefined4 **)((int)puVar9 + -0x70);
            *(undefined4 *)(puVar1 + 0xf4) = **(undefined4 **)((int)puVar9 + -0x78);
          }
          FUN_8c014e42(uVar4,(undefined1 *)((int)puVar9 + -0x24),(undefined1 *)((int)puVar9 + -0x2c)
                      );
        }
        *psVar8 = *psVar8 + 1;
      } while (*psVar8 < 0x10);
      *(short *)((int)puVar9 + -6) = *(short *)((int)puVar9 + -6) + 1;
    } while (*(short *)((int)puVar9 + -6) < 0xc);
  }
  return;
}


/* Address: 8c015566 */

void FUN_8c015566(void)

{
  undefined4 local_14;
  undefined4 local_10;
  undefined4 local_c;
  undefined4 local_8;
  
  local_10 = *(undefined4 *)(PTR_DAT_8c015660 + 4);
  local_14 = *(undefined4 *)PTR_DAT_8c015660;
  local_8 = *(undefined4 *)(PTR_DAT_8c015660 + 0xc);
  local_c = *(undefined4 *)(PTR_DAT_8c015660 + 8);
  (*(code *)PTR_FUN_8c015664)(&local_14);
  FUN_8c014e42(3,PTR_DAT_8c01566c,PTR_DAT_8c015668);
  return;
}


/* Address: 8c015594 */

/* WARNING: Removing unreachable block (ram,0x8c015604) */
/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_8c015594(undefined4 param_1,undefined4 param_2,undefined4 param_3,undefined4 param_4,
                 int param_5)

{
  undefined4 uVar1;
  undefined8 *puVar2;
  double dVar3;
  float fVar4;
  double dVar5;
  longlong lVar6;
  double dVar7;
  undefined8 in_dr12;
  double dVar8;
  undefined4 uVar9;
  undefined8 in_dr14;
  undefined8 in_xd0;
  undefined8 in_xd2;
  double in_xd12;
  double in_xd14;
  undefined4 in_PR;
  char in_FPSCR_PR;
  char in_FPSCR_SZ;
  undefined1 auStack_24 [16];
  undefined8 uStack_14;
  undefined8 uStack_c;
  
  if (in_FPSCR_SZ == '\0') {
    uStack_c = (double)CONCAT44((int)in_dr14,(undefined4)uStack_c);
                    /* WARNING: Ignoring partial resolution of indirect */
    uStack_c._0_4_ = (int)((ulonglong)in_dr14 >> 0x20);
    puVar2 = &uStack_14;
    uStack_14 = CONCAT44((int)in_dr12,(int)((ulonglong)in_dr12 >> 0x20));
  }
  else {
    puVar2 = (undefined8 *)auStack_24;
    uStack_c = in_xd14;
    uStack_14 = in_dr14;
  }
  *(undefined4 *)((int)puVar2 + -4) = in_PR;
  dVar8 = (double)CONCAT44(0x3f800000,(int)in_dr12);
  *(undefined4 *)((int)puVar2 + -0x30) = param_3;
  *(undefined4 *)((int)puVar2 + -0x2c) = param_4;
  uVar1 = DAT_8c015670;
  if (in_FPSCR_SZ != '\0') {
    in_xd14 = (double)CONCAT44(_DAT_8c015674,DAT_8c015670);
    uVar1 = param_2;
  }
  dVar7 = (double)CONCAT44(param_1,uVar1);
  if (param_5 == 0) {
    if (in_FPSCR_SZ == '\0') {
      *(undefined4 *)((int)puVar2 + -0x18) = _DAT_8c015674;
      *(undefined4 *)((int)puVar2 + -0x14) = 0x3f800000;
    }
    else {
      in_xd2 = CONCAT44(PTR_DAT_8c015678,_DAT_8c015674);
      *(undefined8 *)((int)puVar2 + -0x18) = in_xd2;
      *(double *)((int)puVar2 + -0x14) = dVar8;
    }
  }
  else if (in_FPSCR_SZ == '\0') {
    *(undefined4 *)((int)puVar2 + -0x18) = uVar1;
    *(undefined4 *)((int)puVar2 + -0x14) = uVar1;
  }
  else {
    *(double *)((int)puVar2 + -0x18) = in_xd14;
    *(double *)((int)puVar2 + -0x14) = in_xd14;
  }
  uVar1 = *(undefined4 *)(PTR_DAT_8c015678 + 4);
  *(undefined4 *)((int)puVar2 + -0x28) = *(undefined4 *)PTR_DAT_8c015678;
  *(undefined4 *)((int)puVar2 + -0x24) = uVar1;
  uVar1 = *(undefined4 *)(PTR_DAT_8c015678 + 0xc);
  *(undefined4 *)((int)puVar2 + -0x20) = *(undefined4 *)(PTR_DAT_8c015678 + 8);
  *(undefined4 *)((int)puVar2 + -0x1c) = uVar1;
  dVar3 = (double)(*(code *)PTR_FUN_8c015664)((undefined1 *)((int)puVar2 + -0x28));
  if (in_FPSCR_SZ == '\0') {
    *(undefined4 *)((int)puVar2 + -0x10) = (undefined4)DAT_8c01567c;
  }
  else {
    in_xd2 = CONCAT44(DAT_8c01567c._4_4_,(undefined4)DAT_8c01567c);
    *(undefined8 *)((int)puVar2 + -0x10) = in_xd2;
  }
  fVar4 = (float)(*(int *)((int)puVar2 + -0x2c) + 0xf0);
  if (in_FPSCR_SZ == '\0') {
    dVar3 = (double)CONCAT44((int)((ulonglong)dVar3 >> 0x20),fVar4);
    *(float *)((int)puVar2 + -0x34) = fVar4;
    dVar5 = (double)CONCAT44(DAT_8c01567c._4_4_,fVar4);
    in_xd2 = in_xd0;
  }
  else {
    *(undefined8 *)((int)puVar2 + -0x34) = in_xd2;
    dVar5 = (double)CONCAT44(DAT_8c015684,DAT_8c01567c._4_4_);
  }
  if (in_FPSCR_PR == '\0') {
    fVar4 = SUB84(dVar3,0) + (float)((ulonglong)dVar5 >> 0x20);
  }
  else {
    fVar4 = SUB84(dVar3 + dVar5,0);
  }
  if (in_FPSCR_SZ == '\0') {
    *(float *)((int)puVar2 + -0xc) = fVar4;
    *(undefined4 *)((int)puVar2 + -8) = DAT_8c015684;
  }
  else {
    *(undefined8 *)((int)puVar2 + -0xc) = in_xd2;
    *(ulonglong *)((int)puVar2 + -8) = CONCAT44(_DAT_8c015688,DAT_8c015684);
  }
  FUN_8c014e42(1,(undefined1 *)((int)puVar2 + -0x10),(undefined1 *)((int)puVar2 + -0x18));
  fVar4 = (float)((ulonglong)dVar7 >> 0x20);
  dVar3 = dVar7;
  if (in_FPSCR_SZ == '\0') {
    dVar8 = (double)CONCAT44((int)((ulonglong)dVar8 >> 0x20),fVar4);
    dVar5 = (double)CONCAT44((int)((ulonglong)dVar5 >> 0x20),_DAT_8c015688);
    dVar3 = in_xd12;
  }
  if (in_FPSCR_PR == '\0') {
    dVar8 = (double)CONCAT44((int)((ulonglong)dVar8 >> 0x20),SUB84(dVar8,0) + SUB84(dVar5,0));
  }
  else {
    dVar8 = dVar8 + dVar5;
  }
  if (in_FPSCR_SZ == '\0') {
    *(int *)((int)puVar2 + -0x10) = SUB84(dVar8,0);
    dVar5 = (double)CONCAT44(*(undefined4 *)((int)puVar2 + -0x34),SUB84(dVar5,0));
  }
  else {
    *(double *)((int)puVar2 + -0x10) = dVar3;
    dVar5 = *(double *)((int)puVar2 + -0x34);
  }
  if (in_FPSCR_PR == '\0') {
    dVar5 = (double)CONCAT44((float)((ulonglong)dVar5 >> 0x20) + fVar4,SUB84(dVar5,0));
  }
  else {
    dVar5 = dVar5 + dVar7;
  }
  if (in_FPSCR_SZ == '\0') {
    *(int *)((int)puVar2 + -0xc) = (int)((ulonglong)dVar5 >> 0x20);
    lVar6 = (ulonglong)DAT_8c01568c << 0x20;
    *(uint *)((int)puVar2 + -8) = DAT_8c01568c;
  }
  else {
    *(double *)((int)puVar2 + -0xc) = dVar5;
    lVar6 = CONCAT44(uRam8c015690,DAT_8c01568c);
    *(longlong *)((int)puVar2 + -8) = lVar6;
  }
  uVar1 = (undefined4)((ulonglong)lVar6 >> 0x20);
  FUN_8c014e42(0,(undefined1 *)((int)puVar2 + -0x10),(undefined1 *)((int)puVar2 + -0x18));
  if (param_5 == 0) {
    uVar9 = (undefined4)((ulonglong)dVar8 >> 0x20);
    if (in_FPSCR_SZ == '\0') {
      *(undefined4 *)((int)puVar2 + -0x18) = uVar9;
      *(undefined4 *)((int)puVar2 + -0x14) = uVar9;
    }
    else {
      *(double *)((int)puVar2 + -0x18) = dVar8;
      *(double *)((int)puVar2 + -0x14) = dVar8;
    }
  }
  else if (in_FPSCR_SZ == '\0') {
    *(int *)((int)puVar2 + -0x18) = SUB84(dVar7,0);
    *(int *)((int)puVar2 + -0x14) = SUB84(dVar7,0);
  }
  else {
    *(double *)((int)puVar2 + -0x18) = in_xd14;
    *(double *)((int)puVar2 + -0x14) = in_xd14;
  }
  if (in_FPSCR_PR == '\0') {
    uVar1 = 0x40000000;
  }
  else {
    uVar1 = SUB84((double)CONCAT44(uVar1,0x3f800000) + (double)CONCAT44(uVar1,0x3f800000),0);
  }
  if (in_FPSCR_SZ == '\0') {
    *(int *)((int)puVar2 + -0x10) = SUB84(dVar8,0);
    dVar7 = (double)CONCAT44((int)((ulonglong)dVar7 >> 0x20),*(undefined4 *)((int)puVar2 + -0x34));
  }
  else {
    *(double *)((int)puVar2 + -0x10) = dVar3;
    in_xd14 = *(double *)((int)puVar2 + -0x34);
  }
  if (in_FPSCR_PR == '\0') {
    fVar4 = SUB84(dVar7,0) + (float)((ulonglong)dVar7 >> 0x20);
  }
  else {
    fVar4 = SUB84(dVar7 + dVar7,0);
  }
  dVar7 = in_xd14;
  if (in_FPSCR_SZ == '\0') {
    dVar7 = (double)CONCAT44(fVar4,uVar1);
  }
  if (in_FPSCR_PR == '\0') {
    dVar7 = (double)CONCAT44((float)((ulonglong)dVar7 >> 0x20) + SUB84(dVar7,0),SUB84(dVar7,0));
  }
  else {
    dVar7 = dVar7 + dVar7;
  }
  if (in_FPSCR_SZ == '\0') {
    *(int *)((int)puVar2 + -0xc) = (int)((ulonglong)dVar7 >> 0x20);
    *(undefined4 *)((int)puVar2 + -8) = DAT_8c015700;
  }
  else {
    *(double *)((int)puVar2 + -0xc) = dVar7;
    *(ulonglong *)((int)puVar2 + -8) = CONCAT44(_DAT_8c015704,DAT_8c015700);
  }
  FUN_8c014e42(*(undefined4 *)((int)puVar2 + -0x30),(undefined1 *)((int)puVar2 + -0x10),
               (undefined1 *)((int)puVar2 + -0x18));
  if (param_5 != 0) {
    if (in_FPSCR_SZ == '\0') {
      *(int *)((int)puVar2 + -0x10) = SUB84(dVar8,0);
      *(float *)((int)puVar2 + -0xc) = fVar4;
      *(undefined4 *)((int)puVar2 + -8) = _DAT_8c015704;
    }
    else {
      *(double *)((int)puVar2 + -0x10) = dVar3;
      *(double *)((int)puVar2 + -0xc) = in_xd14;
      *(ulonglong *)((int)puVar2 + -8) = CONCAT44(uRam8c015708,_DAT_8c015704);
    }
    FUN_8c014e42(2,(undefined1 *)((int)puVar2 + -0x10),(undefined1 *)((int)puVar2 + -0x18));
  }
  return;
}


/* Address: 8c015720 */

/* WARNING: Removing unreachable block (ram,0x8c01574a) */
/* WARNING: Removing unreachable block (ram,0x8c015736) */
/* WARNING: Removing unreachable block (ram,0x8c015762) */
/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_8c015720(undefined8 param_1,undefined8 param_2)

{
  float fVar1;
  ushort extraout_var;
  int iVar2;
  uint uVar3;
  float extraout_fr1;
  float fVar4;
  undefined4 extraout_fr0;
  double dVar5;
  undefined *puVar6;
  undefined4 uVar7;
  undefined8 uVar8;
  undefined4 uVar9;
  ulonglong uVar10;
  undefined8 in_xd4;
  bool bVar11;
  char in_FPSCR_PR;
  char in_FPSCR_SZ;
  undefined4 local_18;
  undefined4 uStack_14;
  undefined4 uStack_10;
  undefined2 uStack_a;
  
  if (in_FPSCR_SZ == '\0') {
    uStack_14 = (undefined4)((ulonglong)param_1 >> 0x20);
    local_18 = (undefined4)param_1;
    uStack_10 = (undefined4)((ulonglong)param_2 >> 0x20);
  }
  else {
    local_18 = (undefined4)in_xd4;
    uStack_14 = (undefined4)((ulonglong)in_xd4 >> 0x20);
    uStack_10 = (undefined4)param_2;
    uStack_a = (undefined2)((ulonglong)param_2 >> 0x30);
  }
  (*(code *)PTR_FUN_8c0157e0)();
  uVar7 = (undefined4)param_1;
  uVar9 = (undefined4)param_2;
  iVar2 = (*(code *)PTR_FUN_8c0157e4)();
  fVar4 = extraout_fr1;
  fVar1 = DAT_8c0157e8._4_4_;
  if (in_FPSCR_SZ == '\0') {
    fVar4 = DAT_8c0157e8._4_4_;
    fVar1 = (float)DAT_8c0157e8;
  }
  dVar5 = (double)CONCAT44(fVar1,(float)iVar2);
  if (in_FPSCR_PR == '\0') {
    bVar11 = (float)iVar2 * fVar1 < fVar4;
  }
  else {
    bVar11 = dVar5 * dVar5 < (double)CONCAT44(extraout_fr0,fVar4);
  }
  if (bVar11) {
    uVar3 = 4;
  }
  else {
    iVar2 = (*(code *)PTR_FUN_8c0157e4)();
    puVar6 = PTR_DAT_8c0157f4;
    if (in_FPSCR_SZ == '\0') {
      puVar6 = _DAT_8c0157f0;
    }
    dVar5 = (double)CONCAT44(puVar6,(float)iVar2);
    if (in_FPSCR_PR == '\0') {
      uVar3 = (uint)((float)iVar2 * (float)puVar6);
    }
    else {
      uVar3 = (uint)(dVar5 * dVar5);
    }
  }
  if (in_FPSCR_SZ == '\0') {
    uVar7 = local_18;
  }
  if (in_FPSCR_SZ == '\0') {
    uVar10 = CONCAT44(uStack_10,uVar9);
    uVar8 = CONCAT44(uStack_14,uVar7);
  }
  else {
    uVar10 = CONCAT26(uStack_a,CONCAT24(extraout_var,uStack_10)) & 0xffff00ffffffffff;
    uVar8 = CONCAT44(uStack_10,uStack_14);
  }
  (*(code *)PTR_FUN_8c0157fc)
            (uVar8,uVar10,
             *(undefined4 *)
              (PTR_PTR_8c0157f8 +
              (char)PTR_DAT_8c0157f4[(uVar3 & 0xffff) + (uint)(extraout_var & 0xff) * 5] * 4));
  return;
}


/* Address: 8c015798 */

/* WARNING: Removing unreachable block (ram,0x8c0157ac) */

void FUN_8c015798(undefined8 param_1,undefined8 param_2)

{
  int iVar1;
  uint uVar2;
  undefined8 uVar3;
  undefined8 uVar4;
  undefined4 in_xf4;
  char in_FPSCR_SZ;
  undefined4 local_14;
  undefined4 uStack_10;
  undefined4 uStack_c;
  undefined2 uStack_6;
  
  uStack_10 = (undefined4)param_1;
  if (in_FPSCR_SZ == '\0') {
    uStack_c = (undefined4)((ulonglong)param_2 >> 0x20);
    local_14 = (int)((ulonglong)param_1 >> 0x20);
  }
  else {
    uStack_c = (undefined4)param_2;
    uStack_6 = (undefined2)((ulonglong)param_2 >> 0x30);
    local_14 = uStack_10;
    uStack_10 = in_xf4;
  }
  iVar1 = (*(code *)PTR_FUN_8c0157e0)();
  uVar2 = iVar1 >> 8 & 0xff;
  if (in_FPSCR_SZ == '\0') {
    uVar3 = CONCAT44(local_14,(int)param_1);
  }
  else {
    uVar3 = CONCAT44(uStack_10,local_14);
  }
  if (in_FPSCR_SZ == '\0') {
    uVar4 = CONCAT44(uStack_c,(int)param_2);
    uVar3 = CONCAT44((int)((ulonglong)uVar3 >> 0x20),uStack_10);
  }
  else {
    uVar4 = CONCAT26(uStack_6,CONCAT24((short)uVar2,uStack_c));
  }
  (*(code *)PTR_FUN_8c0157fc)
            (uVar3,uVar4,*(undefined4 *)(PTR_PTR_8c0157f8 + (char)PTR_DAT_8c015800[uVar2 * 5] * 4));
  return;
}


/* Address: 8c015820 */

/* WARNING: Removing unreachable block (ram,0x8c015886) */

int FUN_8c015820(undefined4 param_1,undefined4 param_2,undefined4 param_3,undefined4 param_4,
                int param_5)

{
  undefined *puVar1;
  undefined4 uVar2;
  int iVar3;
  int iVar4;
  int iVar5;
  int iVar6;
  undefined4 in_fr3;
  float fVar8;
  double dVar7;
  undefined8 in_xd4;
  undefined4 in_PR;
  char in_FPSCR_PR;
  char in_FPSCR_SZ;
  undefined4 uStackY_1c;
  undefined4 uStackY_18;
  undefined4 uStackY_14;
  undefined4 local_10;
  
  uStackY_18 = param_2;
  uStackY_14 = param_1;
  local_10 = in_PR;
  if (in_FPSCR_SZ != '\0') {
    uStackY_18 = (undefined4)in_xd4;
    uStackY_14 = (undefined4)((ulonglong)in_xd4 >> 0x20);
    local_10 = param_1;
  }
  uStackY_1c = param_4;
  uVar2 = param_3;
  if (in_FPSCR_SZ == '\0') {
    uStackY_1c = param_3;
    uVar2 = uStackY_18;
  }
  uStackY_18 = uVar2;
  if (param_5 != 0) {
    iVar3 = (*(code *)PTR_FUN_8c015900)(3,4,param_5,0x482);
    iVar4 = (*(code *)PTR_FUN_8c015904)(1,0x6c);
    puVar1 = PTR_FUN_8c015908;
    if (iVar3 != 0) {
      if (iVar4 != 0) {
        iVar6 = *(int *)(iVar3 + 0x20);
        if (in_FPSCR_SZ == '\0') {
          *(undefined4 *)(iVar6 + 0x20) = uStackY_14;
          *(undefined4 *)(iVar6 + 0x24) = uStackY_18;
          *(undefined4 *)(iVar6 + 0x28) = uStackY_1c;
        }
        else {
          *(ulonglong *)(iVar6 + 0x20) = CONCAT44(local_10,uStackY_14);
          *(ulonglong *)(iVar6 + 0x24) = CONCAT44(uStackY_14,uStackY_18);
          *(ulonglong *)(iVar6 + 0x28) = CONCAT44(uStackY_18,uStackY_1c);
          uStackY_1c = in_fr3;
        }
        iVar5 = (*(code *)puVar1)();
        puVar1 = PTR_FUN_8c01590c;
        if (in_FPSCR_PR == '\0') {
          fVar8 = 2.0;
        }
        else {
          fVar8 = (float)((ulonglong)
                          ((double)CONCAT44(0x3f800000,uStackY_1c) +
                          (double)CONCAT44(0x3f800000,uStackY_1c)) >> 0x20);
        }
        dVar7 = (double)CONCAT44(fVar8,(float)iVar5);
        if (in_FPSCR_PR == '\0') {
          iVar5 = (int)((float)iVar5 * fVar8);
        }
        else {
          iVar5 = (int)(dVar7 * dVar7);
        }
        *(int *)(iVar6 + 0x18) = iVar5;
        (*(code *)puVar1)(0,iVar3,300);
        *(int *)(iVar6 + 0x10) = iVar4;
        *(undefined4 *)(iVar4 + 100) = 0;
        return iVar6;
      }
      (*(code *)PTR_FUN_8c015910)(iVar3);
    }
  }
  return 0;
}


/* Address: 8c0158b6 */

int FUN_8c0158b6(void)

{
  int iVar1;
  
  iVar1 = FUN_8c015820();
  if (iVar1 != 0) {
    *(ushort *)(iVar1 + 6) = *(ushort *)(iVar1 + 6) | 0x5000;
  }
  return iVar1;
}


/* Address: 8c015b80 */

/* WARNING: Removing unreachable block (ram,0x8c015d0a) */
/* WARNING: Removing unreachable block (ram,0x8c015ce2) */
/* WARNING: Removing unreachable block (ram,0x8c015d26) */
/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined4 FUN_8c015b80(undefined1 *param_1,int param_2,int param_3)

{
  undefined8 uVar1;
  undefined *puVar2;
  undefined *puVar3;
  undefined *puVar4;
  undefined *puVar5;
  undefined *puVar6;
  undefined4 uVar7;
  int iVar8;
  undefined *puVar9;
  int iVar10;
  undefined *puVar11;
  undefined1 *puVar12;
  double extraout_dr0;
  double dVar13;
  float fVar14;
  float fVar15;
  double dVar16;
  float fVar17;
  float fVar18;
  undefined8 in_xd2;
  undefined8 in_xd14;
  undefined4 in_PR;
  undefined1 uVar19;
  char in_FPSCR_PR;
  char in_FPSCR_SZ;
  undefined1 local_14 [4];
  undefined1 local_10 [4];
  
  if (in_FPSCR_SZ == '\0') {
    puVar12 = local_10;
  }
  else {
    puVar12 = local_14;
  }
  *(undefined4 *)(puVar12 + -4) = in_PR;
  *param_1 = (char)param_3;
  if ((*(ushort *)(param_2 + 6) & 0x2000) == 0) {
    *(undefined **)(param_1 + 0x68) = PTR_DAT_8c015bc8 + param_3 * 0x14;
  }
  puVar6 = PTR_LAB_8c015e14;
  puVar11 = PTR_LAB_8c015e0c;
  puVar5 = PTR_DAT_8c015e04;
  puVar4 = PTR_DAT_8c015c8c;
  puVar3 = PTR_LAB_8c015c84;
  puVar2 = PTR_FUN_8c015c70;
  puVar9 = PTR_LAB_8c015c64;
  switch(param_3) {
  case 0:
    *(undefined **)(param_2 + 8) = PTR_PTR_8c015c60;
    *(undefined **)(param_1 + 0x60) = puVar9;
    goto LAB_8c015cca;
  case 1:
    puVar9 = PTR_PTR_8c015c68;
    goto LAB_8c015cb2;
  case 2:
    *(undefined **)(param_2 + 8) = PTR_PTR_8c015e18;
    break;
  case 3:
    *(undefined **)(param_2 + 8) = PTR_PTR_8c015c74;
    puVar11 = puVar2;
    break;
  case 4:
    *(undefined **)(param_2 + 8) = PTR_PTR_8c015c6c;
    puVar11 = puVar2;
    break;
  case 5:
    *(undefined **)(param_2 + 8) = PTR_PTR_8c015c78;
    puVar11 = puVar2;
    break;
  case 6:
    *(undefined **)(param_2 + 8) = PTR_PTR_8c015c88;
    puVar11 = puVar4;
    break;
  case 7:
    *(undefined **)(param_2 + 8) = PTR_PTR_8c015c90;
    puVar11 = puVar3;
    break;
  case 8:
    *(undefined **)(param_2 + 8) = PTR_PTR_8c015c80;
    puVar11 = puVar3;
    break;
  case 9:
    *(undefined **)(param_2 + 8) = PTR_PTR_8c015e08;
    break;
  case 10:
    *(undefined **)(param_2 + 8) = PTR_PTR_8c015e00;
    puVar11 = puVar5;
    break;
  case 0xb:
    *(undefined **)(param_2 + 8) = PTR_PTR_8c015c98;
    puVar11 = puVar9;
    break;
  case 0xc:
    *(undefined **)(param_2 + 8) = PTR_PTR_8c015c94;
    puVar11 = puVar9;
    break;
  case 0xd:
    *(undefined **)(param_2 + 8) = PTR_PTR_8c015c7c;
    puVar11 = puVar9;
    break;
  case 0xe:
    puVar9 = PTR_PTR_8c015e10;
LAB_8c015cb2:
    *(undefined **)(param_2 + 8) = puVar9;
    puVar11 = puVar6;
    break;
  default:
    *(undefined **)(param_1 + 0x60) = PTR_FUN_8c015e1c;
    goto LAB_8c015cca;
  }
  *(undefined **)(param_1 + 0x60) = puVar11;
LAB_8c015cca:
  puVar9 = PTR_FUN_8c015e20;
  param_1[2] = 0;
  if (*(int *)(param_2 + 8) != 0) {
    iVar8 = (*(code *)puVar9)();
    iVar10 = *(int *)(*(int *)(*(int *)(param_2 + 8) + 4) + 4);
    if (in_FPSCR_PR == '\0') {
      dVar16 = (double)CONCAT44((float)iVar10,(float)iVar8);
    }
    else {
      dVar16 = (double)iVar10;
    }
    if (iVar10 < 0) {
      dVar13 = extraout_dr0;
      if (in_FPSCR_SZ == '\0') {
        dVar13 = (double)CONCAT44((int)((ulonglong)extraout_dr0 >> 0x20),DAT_8c015e24);
      }
      if (in_FPSCR_PR == '\0') {
        dVar16 = (double)CONCAT44((float)((ulonglong)dVar16 >> 0x20) + SUB84(dVar13,0),
                                  SUB84(dVar16,0));
      }
      else {
        dVar16 = dVar16 + dVar13;
      }
    }
    if (in_FPSCR_PR == '\0') {
      fVar14 = SUB84(dVar16,0) * (float)((ulonglong)dVar16 >> 0x20);
    }
    else {
      fVar14 = SUB84(dVar16 * dVar16,0);
    }
    if (in_FPSCR_SZ == '\0') {
      dVar16 = (double)CONCAT44(DAT_8c015e28,fVar14);
    }
    else {
      dVar16 = (double)CONCAT44(DAT_8c015e2c,DAT_8c015e28);
    }
    if (in_FPSCR_PR == '\0') {
      fVar14 = SUB84(dVar16,0) * (float)((ulonglong)dVar16 >> 0x20);
    }
    else {
      fVar14 = SUB84(dVar16 * dVar16,0);
    }
    if (in_FPSCR_SZ == '\0') {
      *(float *)(param_1 + 4) = fVar14;
    }
    else {
      *(undefined8 *)(param_1 + 4) = in_xd2;
    }
  }
  fVar18 = 0.0;
  iVar8 = (*(code *)puVar9)();
  fVar17 = fVar18;
  fVar14 = DAT_8c015e2c;
  if (in_FPSCR_SZ != '\0') {
    fVar17 = (float)((ulonglong)in_xd14 >> 0x20);
    fVar14 = DAT_8c015e30;
  }
  fVar15 = (float)iVar8;
  dVar16 = (double)CONCAT44(fVar17,fVar15);
  if (in_FPSCR_PR == '\0') {
    fVar17 = fVar14 * fVar15 + fVar17;
    dVar16 = (double)CONCAT44(fVar17,fVar15);
    uVar19 = (undefined1)(int)fVar17;
  }
  else {
    uVar19 = (undefined1)(int)dVar16;
  }
  param_1[1] = uVar19;
  fVar14 = DAT_8c015e30;
  if (in_FPSCR_SZ == '\0') {
    *(float *)(param_1 + 0x3c) = DAT_8c015e30;
  }
  else {
    fVar14 = SUB84(dVar16,0);
    *(ulonglong *)(param_1 + 0x3c) = CONCAT44(_DAT_8c015e34,DAT_8c015e30);
  }
  iVar8 = (*(code *)puVar9)();
  if (in_FPSCR_PR == '\0') {
    fVar14 = 2.0;
  }
  else {
    fVar14 = (float)((ulonglong)
                     ((double)CONCAT44(0x3f800000,fVar14) + (double)CONCAT44(0x3f800000,fVar14)) >>
                    0x20);
  }
  dVar16 = (double)CONCAT44(fVar14,(float)iVar8);
  if (in_FPSCR_PR == '\0') {
    iVar8 = (int)((float)iVar8 * fVar14);
  }
  else {
    iVar8 = (int)(dVar16 * dVar16);
  }
  *(int *)(param_1 + 0x38) = iVar8;
  if (in_FPSCR_SZ == '\0') {
    *(undefined4 *)(param_1 + 0x54) = *(undefined4 *)(param_2 + 0x20);
    *(undefined4 *)(param_1 + 0x58) = *(undefined4 *)(param_2 + 0x24);
    *(undefined4 *)(param_1 + 0x5c) = *(undefined4 *)(param_2 + 0x28);
  }
  else {
    *(undefined8 *)(param_1 + 0x54) = *(undefined8 *)(param_2 + 0x20);
    *(undefined8 *)(param_1 + 0x58) = *(undefined8 *)(param_2 + 0x24);
    *(undefined8 *)(param_1 + 0x5c) = *(undefined8 *)(param_2 + 0x28);
  }
  *(undefined2 *)(param_2 + 0xc) = 0;
  uVar7 = _DAT_8c015e34;
  if (in_FPSCR_SZ == '\0') {
    *(undefined4 *)(param_1 + 0x20) = _DAT_8c015e34;
    *(undefined4 *)(param_1 + 0x24) = DAT_8c015e38;
    *(undefined4 *)(param_1 + 0x28) = uVar7;
  }
  else {
    uVar1 = CONCAT44(DAT_8c015e38,_DAT_8c015e34);
    *(undefined8 *)(param_1 + 0x20) = uVar1;
    *(ulonglong *)(param_1 + 0x24) = CONCAT44(PTR_FUN_8c015e3c,DAT_8c015e38);
    *(undefined8 *)(param_1 + 0x28) = uVar1;
  }
  *(ushort *)(param_2 + 6) = *(ushort *)(param_2 + 6) | 0x40;
  if ((*(ushort *)(param_2 + 6) & 0x1000) != 0) {
    if (in_FPSCR_SZ == '\0') {
      *(undefined4 *)(param_1 + 0x48) = *(undefined4 *)(param_2 + 0x20);
      *(undefined4 *)(param_1 + 0x4c) = *(undefined4 *)(param_2 + 0x24);
      *(undefined4 *)(param_1 + 0x50) = *(undefined4 *)(param_2 + 0x28);
    }
    else {
      *(undefined8 *)(param_1 + 0x48) = *(undefined8 *)(param_2 + 0x20);
      *(undefined8 *)(param_1 + 0x4c) = *(undefined8 *)(param_2 + 0x24);
      *(undefined8 *)(param_1 + 0x50) = *(undefined8 *)(param_2 + 0x28);
    }
  }
  iVar8 = (*(code *)PTR_FUN_8c015e3c)(0);
  if (iVar8 == 6) {
    *(ushort *)(param_2 + 6) = *(ushort *)(param_2 + 6) | 0x100;
  }
  if (in_FPSCR_SZ == '\0') {
    *(float *)(param_1 + 0x14) = fVar18;
    *(float *)(param_1 + 0x18) = fVar18;
    *(float *)(param_1 + 0x1c) = fVar18;
    *(float *)(param_1 + 8) = fVar18;
    *(float *)(param_1 + 0xc) = fVar18;
    *(float *)(param_1 + 0x10) = fVar18;
  }
  else {
    *(undefined8 *)(param_1 + 0x14) = in_xd14;
    *(undefined8 *)(param_1 + 0x18) = in_xd14;
    *(undefined8 *)(param_1 + 0x1c) = in_xd14;
    *(undefined8 *)(param_1 + 8) = in_xd14;
    *(undefined8 *)(param_1 + 0xc) = in_xd14;
    *(undefined8 *)(param_1 + 0x10) = in_xd14;
  }
  return 1;
}


/* Address: 8c015dc4 */

/* WARNING: Removing unreachable block (ram,0x8c015e82) */
/* WARNING: Removing unreachable block (ram,0x8c015e9e) */
/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_8c015dc4(undefined4 param_1,int param_2,int param_3,undefined4 param_4)

{
  int iVar1;
  double in_dr0;
  float fVar2;
  double dVar3;
  undefined4 uVar4;
  undefined8 uVar5;
  undefined8 uVar6;
  undefined8 in_xd0;
  bool bVar7;
  char in_FPSCR_PR;
  char in_FPSCR_SZ;
  
  *(short *)(param_3 + 0xc) = *(short *)(param_3 + 0xc) + 1;
  if (*(short *)(param_3 + 0xc) < 0x4000) {
    if (*(short *)(param_3 + 0xc) == 0xf) {
      if (in_FPSCR_SZ == '\0') {
        uVar6 = CONCAT44(*(undefined4 *)(param_3 + 0x28),param_1);
        uVar5 = CONCAT44(*(undefined4 *)(param_3 + 0x20),*(undefined4 *)(param_3 + 0x24));
      }
      else {
        uVar6 = *(undefined8 *)(param_3 + 0x28);
        uVar5 = *(undefined8 *)(param_3 + 0x20);
      }
      in_dr0 = (double)(*(code *)PTR_FUN_8c015fa0)(uVar5,uVar6,0x18,0,0);
    }
  }
  else {
    *(undefined2 *)(param_3 + 0xc) = 0x3fff;
  }
  if (*(int *)(param_3 + 8) != 0) {
    if (in_FPSCR_SZ == '\0') {
      dVar3 = (double)CONCAT44(*(undefined4 *)(param_2 + 4),
                               *(undefined4 *)(*(int *)(param_2 + 0x68) + 0xc));
    }
    else {
      dVar3 = *(double *)(param_2 + 4);
    }
    if (in_FPSCR_PR == '\0') {
      dVar3 = (double)CONCAT44((float)((ulonglong)dVar3 >> 0x20) + SUB84(dVar3,0),SUB84(dVar3,0));
    }
    else {
      dVar3 = dVar3 + dVar3;
    }
    uVar4 = (undefined4)((ulonglong)dVar3 >> 0x20);
    if (in_FPSCR_SZ == '\0') {
      *(undefined4 *)(param_2 + 4) = uVar4;
    }
    else {
      *(double *)(param_2 + 4) = dVar3;
    }
    iVar1 = *(int *)(*(int *)(*(int *)(param_3 + 8) + 4) + 4);
    fVar2 = (float)iVar1;
    dVar3 = (double)CONCAT44(uVar4,fVar2);
    if (iVar1 < 0) {
      if (in_FPSCR_SZ == '\0') {
        dVar3 = (double)CONCAT44(DAT_8c015fa4,fVar2);
      }
      else {
        dVar3 = (double)CONCAT44(_DAT_8c015fa8,DAT_8c015fa4);
      }
      if (in_FPSCR_PR == '\0') {
        fVar2 = (float)((ulonglong)dVar3 >> 0x20);
        dVar3 = (double)CONCAT44(fVar2,SUB84(dVar3,0) + fVar2);
      }
      else {
        dVar3 = dVar3 + dVar3;
      }
    }
    if (in_FPSCR_SZ == '\0') {
      in_dr0 = (double)CONCAT44((int)((ulonglong)in_dr0 >> 0x20),*(undefined4 *)(param_2 + 4));
    }
    else {
      in_xd0 = *(undefined8 *)(param_2 + 4);
    }
    if (in_FPSCR_PR == '\0') {
      bVar7 = SUB84(dVar3,0) < SUB84(in_dr0,0);
    }
    else {
      bVar7 = dVar3 < in_dr0;
    }
    if (bVar7) {
      iVar1 = *(int *)(*(int *)(*(int *)(param_3 + 8) + 4) + 4);
      fVar2 = (float)iVar1;
      dVar3 = (double)CONCAT44((int)((ulonglong)dVar3 >> 0x20),fVar2);
      if (iVar1 < 0) {
        if (in_FPSCR_PR == '\0') {
          dVar3 = (double)CONCAT44(DAT_8c015fa4,fVar2 + DAT_8c015fa4);
        }
        else {
          dVar3 = (double)CONCAT44(DAT_8c015fa4,fVar2) + (double)CONCAT44(DAT_8c015fa4,fVar2);
        }
      }
      if (in_FPSCR_SZ == '\0') {
        in_dr0 = (double)CONCAT44((int)((ulonglong)in_dr0 >> 0x20),*(undefined4 *)(param_2 + 4));
      }
      else {
        in_xd0 = *(undefined8 *)(param_2 + 4);
      }
      if (in_FPSCR_PR == '\0') {
        fVar2 = SUB84(in_dr0,0) - SUB84(dVar3,0);
      }
      else {
        fVar2 = SUB84(in_dr0 - dVar3,0);
      }
      if (in_FPSCR_SZ == '\0') {
        *(float *)(param_2 + 4) = fVar2;
      }
      else {
        *(undefined8 *)(param_2 + 4) = in_xd0;
      }
    }
  }
  FUN_8c015ee0(param_2,param_3);
  FUN_8c015f1c(param_2,param_3,param_4);
  FUN_8c016006(param_2,param_3);
  if (*_DAT_8c015fa8 == 0) {
    FUN_8c016076(param_2,param_3);
  }
  (*(code *)PTR_FUN_8c015fac)(param_3);
  return;
}


/* Address: 8c015ee0 */

void FUN_8c015ee0(char *param_1)

{
  if (*param_1 == '\0') {
    if ((*(ushort *)(param_1 + 6) & 0x100) == 0) {
      if (600 < *(short *)(param_1 + 0xc)) {
        *param_1 = '\x01';
      }
    }
    else if ((*(ushort *)(param_1 + 6) & 0x40) == 0) {
      *param_1 = '\x02';
    }
  }
  return;
}


/* Address: 8c015f1c */

void FUN_8c015f1c(byte *param_1,int param_2)

{
  undefined4 uVar1;
  
  if ((0x1e < *(short *)(param_2 + 0xc)) && ((*(ushort *)(*(int *)(param_2 + 0x38) + 2) & 2) != 0))
  {
    if (**(short **)(*(int *)(param_2 + 0x38) + 0x18) != 0) {
      if ((*(ushort *)(*(int *)(param_2 + 0x38) + 2) & 8) == 0) {
        uVar1 = 6;
      }
      else {
        uVar1 = 1;
      }
      (*(code *)PTR_FUN_8c0160e0)(param_1 + 0x20,uVar1);
      return;
    }
    (*(code *)PTR_FUN_8c015fb0)(0x1c,0,0);
    if ((*(ushort *)(param_2 + 6) & 0x40) != 0) {
      (*(code *)PTR_FUN_8c015fb4)(param_2 + 0x20,**(undefined2 **)(param_1 + 0x68));
    }
    (*(code *)PTR_FUN_8c015fb8)
              (param_2 + 0x20,**(undefined2 **)(param_1 + 0x68),*(ushort *)(param_2 + 6) & 8);
    if (*param_1 < 0xf) {
      (*(code *)PTR_FUN_8c015fbc)((int)(char)*param_1);
    }
    *(ushort *)(param_2 + 6) = *(ushort *)(param_2 + 6) | (ushort)DAT_8c015fc0;
  }
  return;
}


/* Address: 8c016006 */

void FUN_8c016006(int param_1,int param_2)

{
  undefined *puVar1;
  
  puVar1 = PTR_FUN_8c0160e8;
  if ((*(ushort *)(param_2 + 6) & 0x4000) == 0) {
    if (*(short *)(param_2 + 0xc) == 0xb4) {
      *(ushort *)(param_2 + 6) = *(ushort *)(param_2 + 6) & (ushort)DAT_8c0160e4;
      *(ushort *)(param_2 + 6) = *(ushort *)(param_2 + 6) | 0x80;
      (*(code *)puVar1)(param_2 + 0x20,**(undefined2 **)(param_1 + 0x68));
    }
    if (*(short *)(param_2 + 0xc) == 0xb5) {
      *(ushort *)(param_2 + 6) = *(ushort *)(param_2 + 6) & (ushort)DAT_8c0160ec;
    }
  }
  if ((*(ushort *)(param_2 + 6) & 0x40) != 0) {
    (*(code *)PTR_FUN_8c0160f0)(param_1,param_2);
    return;
  }
  return;
}


/* Address: 8c016076 */

void FUN_8c016076(int param_1,int param_2)

{
  uint uVar1;
  double dVar2;
  float fVar3;
  double in_dr2;
  double dVar4;
  undefined4 uVar5;
  float fVar6;
  double dVar7;
  double in_xd2;
  bool bVar9;
  int iVar10;
  char in_FPSCR_PR;
  char in_FPSCR_SZ;
  float local_18;
  float fStack_14;
  float local_10;
  float fVar8;
  
  FUN_8c01622a();
  if (in_FPSCR_SZ == '\0') {
    fVar8 = *(float *)(param_1 + 8);
    fVar6 = *(float *)(param_1 + 0x10);
    dVar7 = (double)((ulonglong)(uint)fVar8 << 0x20);
    in_dr2 = (double)CONCAT44((int)((ulonglong)in_dr2 >> 0x20),fVar6);
    dVar4 = (double)CONCAT44(fVar8,fVar6);
  }
  else {
    dVar7 = *(double *)(param_1 + 8);
    in_xd2 = *(double *)(param_1 + 0x10);
    fVar8 = (float)((ulonglong)dVar7 >> 0x20);
    fVar6 = SUB84(dVar7,0);
    dVar4 = dVar7;
  }
  if (in_FPSCR_PR == '\0') {
    fVar3 = SUB84(in_dr2,0) * fVar6;
  }
  else {
    fVar3 = SUB84(in_dr2 * dVar4,0);
  }
  if (in_FPSCR_SZ == '\0') {
    dVar4 = (double)CONCAT44(DAT_8c0160f4,fVar3);
  }
  else {
    dVar4 = (double)CONCAT44(PTR_FUN_8c0160f8,DAT_8c0160f4);
  }
  if (in_FPSCR_PR == '\0') {
    dVar4 = (double)CONCAT44((int)((ulonglong)dVar4 >> 0x20),
                             (float)((ulonglong)dVar7 >> 0x20) * fVar8 + SUB84(dVar4,0));
  }
  dVar7 = in_xd2;
  if (in_FPSCR_SZ == '\0') {
    dVar7 = (double)CONCAT44(SUB84(dVar4,0),fVar6);
  }
  if (in_FPSCR_PR == '\0') {
    bVar9 = (float)((ulonglong)dVar4 >> 0x20) < (float)((ulonglong)dVar7 >> 0x20);
  }
  else {
    bVar9 = dVar4 < dVar7;
  }
  if (bVar9) {
    if (in_FPSCR_SZ == '\0') {
      dVar7 = (double)(ulonglong)*(uint *)(param_1 + 0x10);
    }
    else {
      dVar7 = *(double *)(param_1 + 8);
    }
    dVar2 = (double)(*(code *)PTR_FUN_8c0160f8)();
    if (in_FPSCR_SZ == '\0') {
      dVar4 = (double)CONCAT44((int)((ulonglong)dVar4 >> 0x20),DAT_8c0160fc);
    }
    else {
      in_xd2 = (double)CONCAT44(DAT_8c016100,DAT_8c0160fc);
    }
    if (in_FPSCR_PR == '\0') {
      iVar10 = (int)((float)((ulonglong)dVar2 >> 0x20) * SUB84(dVar4,0));
    }
    else {
      iVar10 = (int)(dVar2 * dVar4);
    }
    uVar1 = iVar10 - *(int *)(param_2 + 0x18);
    if ((DAT_8c016100 & uVar1) == 0) {
      uVar1 = uVar1 & 0xffff;
    }
    else {
      uVar1 = DAT_8c016104 | uVar1;
    }
    *(int *)(param_2 + 0x18) = *(int *)(param_2 + 0x18) + ((int)uVar1 >> 2);
  }
  fVar6 = SUB84(in_xd2,0);
  uVar1 = SUB84(dVar7,0);
  (**(code **)(param_1 + 0x60))(param_1,param_2);
  FUN_8c016c60(param_1,param_2);
  dVar7 = (double)(ulonglong)uVar1;
  if ((*(ushort *)(param_2 + 6) & 0x200) != 0) {
    if (in_FPSCR_SZ == '\0') {
      *(undefined4 *)(param_1 + 0x14) = 0;
      *(undefined4 *)(param_1 + 0x1c) = 0;
      *(undefined4 *)(param_1 + 8) = 0;
      *(undefined4 *)(param_1 + 0x10) = 0;
    }
    else {
      *(double *)(param_1 + 0x14) = dVar7;
      *(double *)(param_1 + 0x1c) = dVar7;
      *(double *)(param_1 + 8) = dVar7;
      *(double *)(param_1 + 0x10) = dVar7;
    }
  }
  *(ushort *)(param_2 + 6) = *(ushort *)(param_2 + 6) & (ushort)DAT_8c0162e0;
  if (in_FPSCR_SZ == '\0') {
    *(undefined4 *)(param_1 + 0x54) = *(undefined4 *)(param_2 + 0x20);
    *(undefined4 *)(param_1 + 0x58) = *(undefined4 *)(param_2 + 0x24);
    *(undefined4 *)(param_1 + 0x5c) = *(undefined4 *)(param_2 + 0x28);
    dVar4 = (double)CONCAT44(*(undefined4 *)(param_2 + 0x20),*(undefined4 *)(param_1 + 8));
  }
  else {
    *(undefined8 *)(param_1 + 0x54) = *(undefined8 *)(param_2 + 0x20);
    *(undefined8 *)(param_1 + 0x58) = *(undefined8 *)(param_2 + 0x24);
    *(undefined8 *)(param_1 + 0x5c) = *(undefined8 *)(param_2 + 0x28);
    dVar4 = *(double *)(param_2 + 0x20);
    fVar6 = (float)*(undefined8 *)(param_1 + 8);
  }
  if (in_FPSCR_PR == '\0') {
    dVar4 = (double)CONCAT44((float)((ulonglong)dVar4 >> 0x20) + SUB84(dVar4,0),SUB84(dVar4,0));
  }
  else {
    dVar4 = dVar4 + dVar4;
  }
  if (in_FPSCR_SZ == '\0') {
    *(int *)(param_2 + 0x20) = (int)((ulonglong)dVar4 >> 0x20);
    dVar4 = (double)CONCAT44(*(undefined4 *)(param_2 + 0x24),*(undefined4 *)(param_1 + 0xc));
  }
  else {
    *(double *)(param_2 + 0x20) = dVar4;
    dVar4 = *(double *)(param_2 + 0x24);
    fVar6 = (float)*(undefined8 *)(param_1 + 0xc);
  }
  if (in_FPSCR_PR == '\0') {
    dVar4 = (double)CONCAT44((float)((ulonglong)dVar4 >> 0x20) + SUB84(dVar4,0),SUB84(dVar4,0));
  }
  else {
    dVar4 = dVar4 + dVar4;
  }
  if (in_FPSCR_SZ == '\0') {
    *(int *)(param_2 + 0x24) = (int)((ulonglong)dVar4 >> 0x20);
    dVar4 = (double)CONCAT44(*(undefined4 *)(param_2 + 0x28),*(undefined4 *)(param_1 + 0x10));
  }
  else {
    *(double *)(param_2 + 0x24) = dVar4;
    dVar4 = *(double *)(param_2 + 0x28);
    fVar6 = (float)*(undefined8 *)(param_1 + 0x10);
  }
  if (in_FPSCR_PR == '\0') {
    dVar4 = (double)CONCAT44((float)((ulonglong)dVar4 >> 0x20) + SUB84(dVar4,0),SUB84(dVar4,0));
  }
  else {
    dVar4 = dVar4 + dVar4;
  }
  if (in_FPSCR_SZ == '\0') {
    *(int *)(param_2 + 0x28) = (int)((ulonglong)dVar4 >> 0x20);
    dVar4 = (double)CONCAT44(*(undefined4 *)(param_1 + 8),*(undefined4 *)(param_1 + 0x14));
  }
  else {
    *(double *)(param_2 + 0x28) = dVar4;
    dVar4 = *(double *)(param_1 + 8);
    fVar6 = (float)*(undefined8 *)(param_1 + 0x14);
  }
  if (in_FPSCR_PR == '\0') {
    dVar4 = (double)CONCAT44((float)((ulonglong)dVar4 >> 0x20) + SUB84(dVar4,0),SUB84(dVar4,0));
  }
  else {
    dVar4 = dVar4 + dVar4;
  }
  if (in_FPSCR_SZ == '\0') {
    *(int *)(param_1 + 8) = (int)((ulonglong)dVar4 >> 0x20);
    dVar4 = (double)CONCAT44(*(undefined4 *)(param_1 + 0xc),*(undefined4 *)(param_1 + 0x18));
  }
  else {
    *(double *)(param_1 + 8) = dVar4;
    dVar4 = *(double *)(param_1 + 0xc);
    fVar6 = (float)*(undefined8 *)(param_1 + 0x18);
  }
  if (in_FPSCR_PR == '\0') {
    dVar4 = (double)CONCAT44((float)((ulonglong)dVar4 >> 0x20) + SUB84(dVar4,0),SUB84(dVar4,0));
  }
  else {
    dVar4 = dVar4 + dVar4;
  }
  if (in_FPSCR_SZ == '\0') {
    *(int *)(param_1 + 0xc) = (int)((ulonglong)dVar4 >> 0x20);
    dVar4 = (double)CONCAT44(*(undefined4 *)(param_1 + 0x10),*(undefined4 *)(param_1 + 0x1c));
  }
  else {
    *(double *)(param_1 + 0xc) = dVar4;
    dVar4 = *(double *)(param_1 + 0x10);
    fVar6 = (float)*(undefined8 *)(param_1 + 0x1c);
  }
  if (in_FPSCR_PR == '\0') {
    dVar4 = (double)CONCAT44((float)((ulonglong)dVar4 >> 0x20) + SUB84(dVar4,0),SUB84(dVar4,0));
  }
  else {
    dVar4 = dVar4 + dVar4;
  }
  uVar5 = (undefined4)((ulonglong)dVar4 >> 0x20);
  if (in_FPSCR_SZ == '\0') {
    *(undefined4 *)(param_1 + 0x10) = uVar5;
    dVar7 = (double)(ulonglong)DAT_8c0162e4;
    dVar4 = (double)CONCAT44(uVar5,*(undefined4 *)(param_1 + 8));
  }
  else {
    *(double *)(param_1 + 0x10) = dVar4;
    fVar6 = (float)*(undefined8 *)(param_1 + 8);
  }
  fVar8 = SUB84(dVar7,0);
  if (in_FPSCR_PR == '\0') {
    dVar4 = (double)CONCAT44((int)((ulonglong)dVar4 >> 0x20),SUB84(dVar4,0) * fVar8);
  }
  else {
    dVar4 = dVar4 * dVar7;
  }
  if (in_FPSCR_SZ == '\0') {
    local_18 = SUB84(dVar4,0);
    dVar4 = (double)CONCAT44((int)((ulonglong)dVar4 >> 0x20),*(undefined4 *)(param_1 + 0xc));
    fVar3 = fVar6;
  }
  else {
    fVar3 = (float)*(undefined8 *)(param_1 + 0xc);
    local_18 = fVar6;
  }
  if (in_FPSCR_PR == '\0') {
    dVar4 = (double)CONCAT44((int)((ulonglong)dVar4 >> 0x20),SUB84(dVar4,0) * fVar8);
  }
  else {
    dVar4 = dVar4 * dVar7;
  }
  if (in_FPSCR_SZ == '\0') {
    fStack_14 = SUB84(dVar4,0);
    dVar4 = (double)CONCAT44((int)((ulonglong)dVar4 >> 0x20),*(undefined4 *)(param_1 + 0x10));
    local_10 = fVar3;
  }
  else {
    local_10 = (float)*(undefined8 *)(param_1 + 0x10);
    fStack_14 = fVar3;
  }
  if (in_FPSCR_PR == '\0') {
    fVar8 = SUB84(dVar4,0) * fVar8;
  }
  else {
    fVar8 = SUB84(dVar4 * dVar7,0);
  }
  if (in_FPSCR_SZ == '\0') {
    dVar4 = (double)CONCAT44(*(undefined4 *)(param_1 + 8),local_18);
    local_10 = fVar8;
  }
  else {
    dVar4 = *(double *)(param_1 + 8);
  }
  if (in_FPSCR_PR == '\0') {
    dVar4 = (double)CONCAT44((float)((ulonglong)dVar4 >> 0x20) - SUB84(dVar4,0),SUB84(dVar4,0));
  }
  else {
    dVar4 = dVar4 - dVar4;
  }
  if (in_FPSCR_SZ == '\0') {
    *(int *)(param_1 + 8) = (int)((ulonglong)dVar4 >> 0x20);
    dVar4 = (double)CONCAT44(*(undefined4 *)(param_1 + 0xc),fStack_14);
  }
  else {
    *(double *)(param_1 + 8) = dVar4;
    dVar4 = *(double *)(param_1 + 0xc);
  }
  if (in_FPSCR_PR == '\0') {
    dVar4 = (double)CONCAT44((float)((ulonglong)dVar4 >> 0x20) - SUB84(dVar4,0),SUB84(dVar4,0));
  }
  else {
    dVar4 = dVar4 - dVar4;
  }
  if (in_FPSCR_SZ == '\0') {
    *(int *)(param_1 + 0xc) = (int)((ulonglong)dVar4 >> 0x20);
    dVar4 = (double)CONCAT44(*(undefined4 *)(param_1 + 0x10),local_10);
  }
  else {
    *(double *)(param_1 + 0xc) = dVar4;
    dVar4 = *(double *)(param_1 + 0x10);
  }
  if (in_FPSCR_PR == '\0') {
    dVar4 = (double)CONCAT44((float)((ulonglong)dVar4 >> 0x20) - SUB84(dVar4,0),SUB84(dVar4,0));
  }
  else {
    dVar4 = dVar4 - dVar4;
  }
  if (in_FPSCR_SZ == '\0') {
    *(int *)(param_1 + 0x10) = (int)((ulonglong)dVar4 >> 0x20);
    *(undefined4 *)(param_1 + 0x14) = 0;
    *(undefined4 *)(param_1 + 0x18) = 0;
    *(undefined4 *)(param_1 + 0x1c) = 0;
  }
  else {
    *(double *)(param_1 + 0x10) = dVar4;
    *(double *)(param_1 + 0x14) = dVar7;
    *(double *)(param_1 + 0x18) = dVar7;
    *(double *)(param_1 + 0x1c) = dVar7;
  }
  return;
}


/* Address: 8c01622a */

/* WARNING: Removing unreachable block (ram,0x8c016360) */
/* WARNING: Removing unreachable block (ram,0x8c016350) */
/* WARNING: Removing unreachable block (ram,0x8c016414) */
/* WARNING: Removing unreachable block (ram,0x8c0164f2) */
/* WARNING: Removing unreachable block (ram,0x8c016394) */
/* WARNING: Removing unreachable block (ram,0x8c016380) */
/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_8c01622a(double param_1,int param_2,char *param_3)

{
  undefined *puVar1;
  int *piVar2;
  int iVar3;
  int iVar4;
  float *pfVar5;
  undefined1 *puVar6;
  float fVar10;
  double dVar7;
  longlong lVar8;
  double dVar9;
  float in_fr3;
  double dVar11;
  undefined4 uVar12;
  uint uVar13;
  float fVar14;
  float fVar15;
  undefined4 in_fr12;
  double in_dr14;
  longlong in_xd2;
  longlong lVar16;
  undefined8 in_xd12;
  longlong in_xd14;
  undefined4 in_PR;
  char cVar17;
  char in_FPSCR_PR;
  char in_FPSCR_SZ;
  undefined1 auStack_2c [12];
  undefined1 local_20 [4];
  undefined8 uStack_1c;
  
  piVar2 = _DAT_8c0162f0;
  puVar1 = PTR_FUN_8c0162e8;
  fVar14 = (float)((ulonglong)in_xd12 >> 0x20);
  uVar12 = (undefined4)((ulonglong)in_dr14 >> 0x20);
  if (in_FPSCR_SZ == '\0') {
    uStack_1c = CONCAT44(SUB84(in_dr14,0),(undefined4)uStack_1c);
                    /* WARNING: Ignoring partial resolution of indirect */
    uStack_1c._0_4_ = uVar12;
    puVar6 = local_20;
  }
  else {
    puVar6 = auStack_2c;
    uStack_1c = in_xd14;
  }
  *(undefined4 *)(puVar6 + -4) = in_PR;
  if (((*(ushort *)(param_3 + 6) & 0x1000) == 0) || ((*(ushort *)(param_3 + 6) & 0x40) == 0)) {
    if (in_FPSCR_SZ == '\0') {
      in_dr14 = (double)CONCAT44(uVar12,DAT_8c0162ec);
    }
    else {
      in_xd14 = CONCAT44(_DAT_8c0162f0,DAT_8c0162ec);
    }
    cVar17 = *param_3;
    if (cVar17 == '\0') {
      if (in_FPSCR_SZ == '\0') {
        *(undefined4 *)(param_2 + 0x48) = *(undefined4 *)(*_DAT_8c0162f0 + 0x20);
      }
      else {
        *(undefined8 *)(param_2 + 0x48) = *(undefined8 *)(*_DAT_8c0162f0 + 0x20);
      }
      if (in_FPSCR_SZ == '\0') {
        *(undefined4 *)(param_2 + 0x4c) = *(undefined4 *)(*piVar2 + 0x24);
      }
      else {
        *(undefined8 *)(param_2 + 0x4c) = *(undefined8 *)(*piVar2 + 0x24);
      }
      if (in_FPSCR_SZ == '\0') {
        *(undefined4 *)(param_2 + 0x50) = *(undefined4 *)(*piVar2 + 0x28);
      }
      else {
        *(undefined8 *)(param_2 + 0x50) = *(undefined8 *)(*piVar2 + 0x28);
      }
    }
    else if (cVar17 == '\x01') {
      if ((*(ushort *)(param_3 + 6) & 0x400) == 0) {
        if (*(undefined **)(param_2 + 0x60) == PTR_LAB_8c0162f4) {
          if (in_FPSCR_SZ == '\0') {
            *(undefined4 *)(param_2 + 0x48) = *(undefined4 *)(param_3 + 0x20);
            *(undefined4 *)(param_2 + 0x4c) = *(undefined4 *)(param_3 + 0x24);
            in_fr3 = *(float *)(param_3 + 0x28);
          }
          else {
            *(undefined8 *)(param_2 + 0x48) = *(undefined8 *)(param_3 + 0x20);
            *(undefined8 *)(param_2 + 0x4c) = *(undefined8 *)(param_3 + 0x24);
            in_xd2 = *(longlong *)(param_3 + 0x28);
          }
        }
        else {
          if (in_FPSCR_SZ == '\0') {
            in_fr3 = *(float *)(*_DAT_8c0162f0 + 0x20);
            *(float *)(param_2 + 0x48) = in_fr3;
          }
          else {
            in_xd2 = *(longlong *)(*_DAT_8c0162f0 + 0x20);
            *(longlong *)(param_2 + 0x48) = in_xd2;
          }
          if (in_FPSCR_SZ == '\0') {
            in_fr3 = *(float *)(*piVar2 + 0x24);
            *(float *)(param_2 + 0x4c) = in_fr3;
          }
          else {
            in_xd2 = *(longlong *)(*piVar2 + 0x24);
            *(longlong *)(param_2 + 0x4c) = in_xd2;
          }
          if (in_FPSCR_SZ == '\0') {
            in_fr3 = *(float *)(*piVar2 + 0x28);
          }
          else {
            in_xd2 = *(longlong *)(*piVar2 + 0x28);
          }
        }
        if (in_FPSCR_SZ == '\0') {
          *(float *)(param_2 + 0x50) = in_fr3;
          *(undefined4 *)(param_2 + 0x3c) = DAT_8c0163e0;
        }
        else {
          *(longlong *)(param_2 + 0x50) = in_xd2;
          in_xd2 = CONCAT44(_DAT_8c0163e4,DAT_8c0163e0);
          *(longlong *)(param_2 + 0x3c) = in_xd2;
        }
        *(ushort *)(param_3 + 6) = *(ushort *)(param_3 + 6) | 0x400;
      }
      if ((*(ushort *)(param_3 + 6) & 0x800) != 0) {
        if (in_FPSCR_SZ == '\0') {
          dVar11 = (double)CONCAT44(_DAT_8c0163e4,SUB84(in_dr14,0));
          lVar8 = (ulonglong)*(uint *)(param_2 + 8) << 0x20;
        }
        else {
          dVar11 = (double)CONCAT44(_DAT_8c0163e8,_DAT_8c0163e4);
          lVar8 = *(longlong *)(param_2 + 8);
        }
        fVar15 = 1.0;
        if (in_FPSCR_PR != '\0' || (float)((ulonglong)lVar8 >> 0x20) <= (float)lVar8) {
          iVar3 = (*(code *)puVar1)();
          fVar10 = (float)((ulonglong)in_xd14 >> 0x20);
          if (in_FPSCR_SZ == '\0') {
            fVar14 = fVar15;
          }
          dVar7 = (double)CONCAT44(fVar14,(float)iVar3);
        }
        else {
          iVar3 = (*(code *)puVar1)();
          fVar10 = (float)((ulonglong)in_xd14 >> 0x20);
          dVar7 = dVar11;
          if (in_FPSCR_SZ == '\0') {
            dVar7 = (double)CONCAT44((int)((ulonglong)dVar11 >> 0x20),(float)iVar3);
          }
        }
        pfVar5 = (float *)(param_2 + 0x48);
        fVar14 = fVar10;
        if (in_FPSCR_SZ == '\0') {
          fVar14 = SUB84(dVar11,0);
        }
        if (in_FPSCR_PR == '\0') {
          dVar7 = (double)CONCAT44(fVar14 * SUB84(dVar7,0) + (float)((ulonglong)dVar7 >> 0x20),
                                   SUB84(dVar7,0));
        }
        if (in_FPSCR_SZ == '\0') {
          dVar7 = (double)CONCAT44((int)((ulonglong)dVar7 >> 0x20),*pfVar5);
        }
        else {
          in_xd2 = *(longlong *)pfVar5;
        }
        if (in_FPSCR_PR == '\0') {
          fVar14 = SUB84(dVar7,0) + (float)((ulonglong)dVar7 >> 0x20);
        }
        else {
          fVar14 = SUB84(dVar7 + dVar7,0);
        }
        if (in_FPSCR_SZ == '\0') {
          *pfVar5 = fVar14;
          fVar14 = *(float *)(param_2 + 0x10);
        }
        else {
          *(longlong *)pfVar5 = in_xd2;
          in_xd2 = *(longlong *)(param_2 + 0x10);
        }
        uVar12 = 0;
        if (in_FPSCR_PR != '\0' || fVar14 <= 0.0) {
          iVar3 = (*(code *)puVar1)();
          if (in_FPSCR_SZ == '\0') {
            fVar10 = SUB84(dVar11,0);
          }
          fVar14 = (float)iVar3;
          if (in_FPSCR_PR == '\0') {
            fVar15 = fVar10 * fVar14 + fVar15;
          }
          if (in_FPSCR_SZ == '\0') {
            fVar14 = *(float *)(param_2 + 0x50);
          }
          else {
            in_xd2 = *(longlong *)(param_2 + 0x50);
          }
          if (in_FPSCR_PR == '\0') {
            fVar14 = fVar14 + fVar15;
          }
          else {
            fVar14 = SUB84((double)CONCAT44(uVar12,fVar14) + (double)CONCAT44(in_fr12,fVar15),0);
          }
        }
        else {
          iVar3 = (*(code *)puVar1)();
          if (in_FPSCR_SZ == '\0') {
            fVar10 = SUB84(dVar11,0);
          }
          fVar14 = (float)iVar3;
          if (in_FPSCR_PR == '\0') {
            dVar11 = (double)CONCAT44(fVar10 * fVar14 + (float)((ulonglong)dVar11 >> 0x20),
                                      SUB84(dVar11,0));
          }
          if (in_FPSCR_SZ == '\0') {
            fVar14 = *(float *)(param_2 + 0x50);
          }
          else {
            in_xd2 = *(longlong *)(param_2 + 0x50);
          }
          if (in_FPSCR_PR == '\0') {
            fVar14 = fVar14 + (float)((ulonglong)dVar11 >> 0x20);
          }
          else {
            fVar14 = SUB84((double)CONCAT44(uVar12,fVar14) + dVar11,0);
          }
        }
        uVar12 = _DAT_8c0163e8;
        if (in_FPSCR_SZ == '\0') {
          *(float *)(param_2 + 0x50) = fVar14;
        }
        else {
          *(longlong *)(param_2 + 0x50) = in_xd2;
        }
        *(ushort *)(param_3 + 6) = *(ushort *)(param_3 + 6) & (ushort)uVar12;
      }
    }
    else if (((cVar17 == '\x02') || (cVar17 == '\x03')) || (cVar17 == '\x04')) {
      if (*param_3 == '\x02') {
        iVar3 = 0;
      }
      else {
        iVar3 = DAT_8c016560;
        if (*param_3 == '\x03') {
          iVar3 = 0x4000;
        }
      }
      if ((*(ushort *)(param_3 + 6) & 0x800) != 0) {
        iVar4 = (*(code *)PTR_FUN_8c0162e8)();
        lVar8 = in_xd14;
        if (in_FPSCR_SZ == '\0') {
          lVar8 = (longlong)in_dr14 << 0x20;
        }
        in_fr3 = (float)iVar4;
        if (in_FPSCR_PR == '\0') {
          cVar17 = (char)(int)((float)((ulonglong)lVar8 >> 0x20) * in_fr3 + 0.0);
        }
        else {
          cVar17 = (char)(int)(double)(ulonglong)(uint)in_fr3;
        }
        *param_3 = cVar17;
        uVar12 = DAT_8c016564;
        *param_3 = *param_3 + '\x02';
        *(ushort *)(param_3 + 6) = *(ushort *)(param_3 + 6) & (ushort)uVar12;
      }
      if (in_FPSCR_SZ == '\0') {
        dVar11 = (double)CONCAT44(*(undefined4 *)(param_3 + 0x20),in_fr3);
      }
      else {
        dVar11 = *(double *)(param_3 + 0x20);
      }
      if (in_FPSCR_SZ == '\0') {
        dVar11 = (double)CONCAT44((int)((ulonglong)dVar11 >> 0x20),
                                  *(undefined4 *)(*DAT_8c016568 + 0x20));
      }
      else {
        in_xd2 = *(longlong *)(*DAT_8c016568 + 0x20);
      }
      if (in_FPSCR_PR == '\0') {
        dVar11 = (double)CONCAT44((float)((ulonglong)dVar11 >> 0x20) - SUB84(dVar11,0),
                                  SUB84(dVar11,0));
      }
      else {
        dVar11 = dVar11 - dVar11;
      }
      if (in_FPSCR_SZ == '\0') {
        *(int *)(puVar6 + -0x10) = (int)((ulonglong)dVar11 >> 0x20);
      }
      else {
        *(double *)(puVar6 + -0x10) = dVar11;
      }
      if (in_FPSCR_SZ == '\0') {
        dVar11 = (double)CONCAT44(*(undefined4 *)(param_3 + 0x24),
                                  *(undefined4 *)(*DAT_8c016568 + 0x24));
      }
      else {
        dVar11 = *(double *)(param_3 + 0x24);
        in_xd2 = *(longlong *)(*DAT_8c016568 + 0x24);
      }
      if (in_FPSCR_PR == '\0') {
        dVar11 = (double)CONCAT44((float)((ulonglong)dVar11 >> 0x20) - SUB84(dVar11,0),
                                  SUB84(dVar11,0));
      }
      else {
        dVar11 = dVar11 - dVar11;
      }
      if (in_FPSCR_SZ == '\0') {
        *(int *)(puVar6 + -0xc) = (int)((ulonglong)dVar11 >> 0x20);
      }
      else {
        *(double *)(puVar6 + -0xc) = dVar11;
      }
      if (in_FPSCR_SZ == '\0') {
        dVar11 = (double)CONCAT44(*(undefined4 *)(param_3 + 0x28),
                                  *(undefined4 *)(*DAT_8c016568 + 0x28));
      }
      else {
        dVar11 = *(double *)(param_3 + 0x28);
        in_xd2 = *(longlong *)(*DAT_8c016568 + 0x28);
      }
      if (in_FPSCR_PR == '\0') {
        dVar11 = (double)CONCAT44((float)((ulonglong)dVar11 >> 0x20) - SUB84(dVar11,0),
                                  SUB84(dVar11,0));
      }
      else {
        dVar11 = dVar11 - dVar11;
      }
      uVar12 = (undefined4)((ulonglong)dVar11 >> 0x20);
      if (in_FPSCR_SZ == '\0') {
        *(undefined4 *)(puVar6 + -8) = uVar12;
        in_dr14 = (double)CONCAT44((int)((ulonglong)in_dr14 >> 0x20),DAT_8c01656c);
        fVar15 = *(float *)(puVar6 + -0x10);
        fVar14 = *(float *)(puVar6 + -0xc);
        param_1 = (double)CONCAT44((int)((ulonglong)param_1 >> 0x20),fVar14);
        dVar11 = (double)CONCAT44(uVar12,fVar14);
      }
      else {
        *(double *)(puVar6 + -8) = dVar11;
        in_xd14 = CONCAT44(DAT_8c016570,DAT_8c01656c);
        fVar15 = (float)((ulonglong)*(undefined8 *)(puVar6 + -0x10) >> 0x20);
        in_xd2 = *(longlong *)(puVar6 + -0xc);
        fVar14 = SUB84(param_1,0);
      }
      if (in_FPSCR_PR == '\0') {
        dVar11 = (double)CONCAT44((int)((ulonglong)dVar11 >> 0x20),
                                  fVar15 * fVar15 + SUB84(dVar11,0) * fVar14);
      }
      else {
        dVar11 = dVar11 * param_1;
      }
      uVar13 = (uint)((ulonglong)dVar11 >> 0x20);
      dVar9 = dVar11;
      dVar7 = dVar11;
      if (in_FPSCR_SZ == '\0') {
        dVar9 = (double)CONCAT44(uVar13,fVar14);
        dVar7 = (double)((ulonglong)uVar13 << 0x20);
      }
      if (in_FPSCR_PR == '\0') {
        dVar11 = (double)(ulonglong)
                         (uint)((float)((ulonglong)dVar7 >> 0x20) *
                                (float)((ulonglong)dVar9 >> 0x20) + SUB84(dVar11,0));
      }
      if (in_FPSCR_SZ == '\0') {
        dVar11 = (double)CONCAT44(DAT_8c016570,SUB84(dVar11,0));
      }
      else {
        dVar11 = (double)CONCAT44(PTR_FUN_8c016574,DAT_8c016570);
      }
      if (in_FPSCR_PR != '\0' || SUB84(dVar11,0) <= (float)((ulonglong)dVar11 >> 0x20)) {
        dVar7 = (double)(*(code *)PTR_FUN_8c016584)(0);
        if (in_FPSCR_PR == '\0') {
          dVar7 = (double)CONCAT44((float)((ulonglong)dVar7 >> 0x20) * SUB84(in_dr14,0),
                                   SUB84(dVar7,0));
        }
        else {
          dVar7 = dVar7 * in_dr14;
        }
        if (in_FPSCR_SZ == '\0') {
          *(int *)(puVar6 + -0xc) = (int)((ulonglong)dVar7 >> 0x20);
        }
        else {
          *(double *)(puVar6 + -0xc) = dVar7;
        }
        lVar8 = (*(code *)PTR_FUN_8c016588)(0);
        if (in_FPSCR_SZ == '\0') {
          dVar11 = (double)CONCAT44((int)((ulonglong)dVar11 >> 0x20),SUB84(in_dr14,0));
          in_dr14 = (double)CONCAT44((int)((ulonglong)in_dr14 >> 0x20),
                                     (int)((ulonglong)lVar8 >> 0x20));
          lVar8 = in_xd14;
          in_xd14 = in_xd2;
        }
        if (in_FPSCR_PR == '\0') {
          dVar7 = (double)CONCAT44((int)((ulonglong)in_dr14 >> 0x20),
                                   SUB84(in_dr14,0) * SUB84(dVar11,0));
        }
        else {
          dVar7 = in_dr14 * dVar11;
        }
        dVar9 = (double)(*(code *)PTR_FUN_8c016588)(iVar3);
        lVar16 = lVar8;
        if (in_FPSCR_SZ == '\0') {
          dVar11 = (double)CONCAT44((int)((ulonglong)dVar11 >> 0x20),SUB84(dVar7,0));
          lVar16 = in_xd14;
        }
        if (in_FPSCR_PR == '\0') {
          fVar14 = SUB84(dVar11,0) * (float)((ulonglong)dVar9 >> 0x20);
        }
        else {
          fVar14 = SUB84(dVar11 * dVar9,0);
        }
        if (in_FPSCR_SZ == '\0') {
          *(float *)(puVar6 + -0x10) = fVar14;
        }
        else {
          *(longlong *)(puVar6 + -0x10) = lVar16;
        }
        dVar11 = (double)(*(code *)PTR_FUN_8c016584)(iVar3);
        if (in_FPSCR_PR == '\0') {
          fVar14 = SUB84(dVar7,0) * (float)((ulonglong)dVar11 >> 0x20);
        }
        else {
          fVar14 = SUB84(dVar7 * dVar11,0);
        }
        if (in_FPSCR_SZ == '\0') {
          *(float *)(puVar6 + -8) = -fVar14;
        }
        else {
          *(longlong *)(puVar6 + -8) = lVar8;
        }
      }
      else {
        (*(code *)PTR_FUN_8c016574)(puVar6 + -0x10);
        (*(code *)PTR_FUN_8c016578)(puVar6 + -0x50);
        if (iVar3 != 0) {
          (*(code *)PTR_FUN_8c01657c)(puVar6 + -0x50,iVar3);
        }
        (*(code *)PTR_FUN_8c016580)(puVar6 + -0x50,puVar6 + -0x10,puVar6 + -0x10);
        if (in_FPSCR_SZ == '\0') {
          dVar11 = (double)CONCAT44((int)((ulonglong)dVar11 >> 0x20),*(undefined4 *)(puVar6 + -0x10)
                                   );
        }
        else {
          in_xd2 = *(longlong *)(puVar6 + -0x10);
        }
        fVar14 = SUB84(in_dr14,0);
        if (in_FPSCR_PR == '\0') {
          dVar11 = (double)CONCAT44((int)((ulonglong)dVar11 >> 0x20),SUB84(dVar11,0) * fVar14);
        }
        else {
          dVar11 = dVar11 * in_dr14;
        }
        if (in_FPSCR_SZ == '\0') {
          *(int *)(puVar6 + -0x10) = SUB84(dVar11,0);
          dVar11 = (double)CONCAT44((int)((ulonglong)dVar11 >> 0x20),*(undefined4 *)(puVar6 + -0xc))
          ;
        }
        else {
          *(longlong *)(puVar6 + -0x10) = in_xd2;
          in_xd2 = *(longlong *)(puVar6 + -0xc);
        }
        if (in_FPSCR_PR == '\0') {
          dVar11 = (double)CONCAT44((int)((ulonglong)dVar11 >> 0x20),SUB84(dVar11,0) * fVar14);
        }
        else {
          dVar11 = dVar11 * in_dr14;
        }
        if (in_FPSCR_SZ == '\0') {
          *(int *)(puVar6 + -0xc) = SUB84(dVar11,0);
          dVar11 = (double)CONCAT44((int)((ulonglong)dVar11 >> 0x20),*(undefined4 *)(puVar6 + -8));
        }
        else {
          *(longlong *)(puVar6 + -0xc) = in_xd2;
          in_xd2 = *(longlong *)(puVar6 + -8);
        }
        if (in_FPSCR_PR == '\0') {
          fVar14 = SUB84(dVar11,0) * fVar14;
        }
        else {
          fVar14 = SUB84(dVar11 * in_dr14,0);
        }
        if (in_FPSCR_SZ == '\0') {
          *(float *)(puVar6 + -8) = fVar14;
        }
        else {
          *(longlong *)(puVar6 + -8) = in_xd2;
        }
      }
      if (in_FPSCR_SZ == '\0') {
        dVar11 = (double)CONCAT44(*(undefined4 *)(puVar6 + -0x10),*(undefined4 *)(param_3 + 0x20));
      }
      else {
        dVar11 = *(double *)(puVar6 + -0x10);
      }
      if (in_FPSCR_PR == '\0') {
        dVar11 = (double)CONCAT44((float)((ulonglong)dVar11 >> 0x20) + SUB84(dVar11,0),
                                  SUB84(dVar11,0));
      }
      else {
        dVar11 = dVar11 + dVar11;
      }
      if (in_FPSCR_SZ == '\0') {
        *(int *)(param_2 + 0x48) = (int)((ulonglong)dVar11 >> 0x20);
        dVar11 = (double)CONCAT44(*(undefined4 *)(puVar6 + -0xc),*(undefined4 *)(param_3 + 0x24));
      }
      else {
        *(double *)(param_2 + 0x48) = dVar11;
        dVar11 = *(double *)(puVar6 + -0xc);
      }
      if (in_FPSCR_PR == '\0') {
        dVar11 = (double)CONCAT44((float)((ulonglong)dVar11 >> 0x20) + SUB84(dVar11,0),
                                  SUB84(dVar11,0));
      }
      else {
        dVar11 = dVar11 + dVar11;
      }
      if (in_FPSCR_SZ == '\0') {
        *(int *)(param_2 + 0x4c) = (int)((ulonglong)dVar11 >> 0x20);
        dVar11 = (double)CONCAT44(*(undefined4 *)(puVar6 + -8),*(undefined4 *)(param_3 + 0x28));
      }
      else {
        *(double *)(param_2 + 0x4c) = dVar11;
        dVar11 = *(double *)(puVar6 + -8);
      }
      if (in_FPSCR_PR == '\0') {
        dVar11 = (double)CONCAT44((float)((ulonglong)dVar11 >> 0x20) + SUB84(dVar11,0),
                                  SUB84(dVar11,0));
      }
      else {
        dVar11 = dVar11 + dVar11;
      }
      if (in_FPSCR_SZ == '\0') {
        *(int *)(param_2 + 0x50) = (int)((ulonglong)dVar11 >> 0x20);
      }
      else {
        *(double *)(param_2 + 0x50) = dVar11;
      }
    }
  }
  return;
}


/* Address: 8c01669e */

/* WARNING: Removing unreachable block (ram,0x8c0166d4) */
/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined4 FUN_8c01669e(int param_1,int param_2,undefined8 *param_3)

{
  undefined *puVar1;
  undefined4 uVar2;
  undefined8 *puVar3;
  undefined4 uVar4;
  float fVar5;
  float fVar6;
  undefined8 uVar8;
  int iVar9;
  char in_FPSCR_PR;
  char in_FPSCR_SZ;
  double dVar7;
  
  if ((*(ushort *)(param_2 + 6) & 0x40) == 0) {
    fVar5 = _DAT_8c0167cc;
    if (in_FPSCR_SZ == '\0') {
      fVar5 = DAT_8c0167c8;
    }
    fVar6 = (float)((*(byte *)(param_1 + 1) & 3) + (uint)*(ushort *)(*(int *)(param_1 + 0x68) + 2));
    dVar7 = (double)CONCAT44(fVar5,fVar6);
    if (in_FPSCR_PR == '\0') {
      iVar9 = (int)(fVar6 * fVar5);
    }
    else {
      iVar9 = (int)(dVar7 * dVar7);
    }
    if ((*(byte *)(param_1 + 1) & 4) != 0) {
      iVar9 = -iVar9;
    }
    if ((*(ushort *)(param_2 + 6) & 0x20) != 0) {
      iVar9 = -iVar9;
    }
    *(int *)(param_1 + 0x38) = *(int *)(param_1 + 0x38) + iVar9;
    puVar1 = PTR_FUN_8c0167c0;
    if (in_FPSCR_SZ == '\0') {
      *(undefined4 *)param_3 = *(undefined4 *)(param_1 + 0x48);
      *(undefined4 *)((int)param_3 + 4) = *(undefined4 *)(param_1 + 0x4c);
      *(undefined4 *)(param_3 + 1) = *(undefined4 *)(param_1 + 0x50);
    }
    else {
      *param_3 = *(undefined8 *)(param_1 + 0x48);
      *(undefined8 *)((int)param_3 + 4) = *(undefined8 *)(param_1 + 0x4c);
      param_3[1] = *(undefined8 *)(param_1 + 0x50);
    }
    uVar4 = (*(code *)puVar1)(*(int *)(param_1 + 0x38) >> 2);
    puVar1 = PTR_FUN_8c0167c0;
    if (in_FPSCR_SZ == '\0') {
      uVar8 = CONCAT44(DAT_8c0167e4,uVar4);
      fVar5 = DAT_8c0167e8;
    }
    else {
      uVar8 = CONCAT44(DAT_8c0167e8,DAT_8c0167e4);
      fVar5 = DAT_8c0167ec;
    }
    if (in_FPSCR_PR == '\0') {
      uVar8 = CONCAT44(fVar5 * (float)uVar8 + (float)((ulonglong)uVar8 >> 0x20),(float)uVar8);
    }
    if (in_FPSCR_SZ == '\0') {
      *(int *)(param_1 + 0x3c) = (int)((ulonglong)uVar8 >> 0x20);
    }
    else {
      *(undefined8 *)(param_1 + 0x3c) = uVar8;
    }
    fVar5 = (float)(*(code *)puVar1)(*(undefined4 *)(param_1 + 0x38));
    puVar1 = PTR_FUN_8c0167c4;
    if (in_FPSCR_SZ == '\0') {
      uVar8 = CONCAT44(*(undefined4 *)param_3,*(undefined4 *)(param_1 + 0x3c));
    }
    else {
      uVar8 = *param_3;
    }
    if (in_FPSCR_PR == '\0') {
      uVar8 = CONCAT44(fVar5 * (float)uVar8 + (float)((ulonglong)uVar8 >> 0x20),(float)uVar8);
    }
    puVar3 = param_3 + 1;
    if (in_FPSCR_SZ == '\0') {
      *(int *)param_3 = (int)((ulonglong)uVar8 >> 0x20);
    }
    else {
      *param_3 = uVar8;
    }
    fVar5 = (float)(*(code *)puVar1)(*(undefined4 *)(param_1 + 0x38));
    uVar4 = (undefined4)uVar8;
    if (in_FPSCR_SZ == '\0') {
      uVar4 = *(undefined4 *)(param_1 + 0x3c);
    }
    uVar2 = 0;
    if (in_FPSCR_SZ == '\0') {
      uVar8 = CONCAT44(*(undefined4 *)puVar3,uVar4);
    }
    else {
      uVar8 = *puVar3;
    }
    if (in_FPSCR_PR == '\0') {
      uVar8 = CONCAT44(fVar5 * (float)uVar8 + (float)((ulonglong)uVar8 >> 0x20),(float)uVar8);
    }
    if (in_FPSCR_SZ == '\0') {
      *(int *)puVar3 = (int)((ulonglong)uVar8 >> 0x20);
    }
    else {
      *puVar3 = uVar8;
    }
  }
  else {
    FUN_8c016a70(param_1);
    uVar2 = 1;
  }
  return uVar2;
}


/* Address: 8c016a70 */

/* WARNING: Removing unreachable block (ram,0x8c016af4) */
/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined8 FUN_8c016a70(undefined4 param_1,char *param_2,int param_3)

{
  undefined *puVar1;
  float fVar2;
  int iVar3;
  double dVar4;
  undefined *puVar7;
  float fVar8;
  double dVar5;
  undefined8 uVar6;
  float fVar10;
  double dVar9;
  undefined4 uVar11;
  double in_xd2;
  float in_PR;
  char in_FPSCR_PR;
  char in_FPSCR_SZ;
  undefined4 local_24;
  undefined4 uStack_20;
  float local_1c;
  float fStack_18;
  float fStack_14;
  float fStack_10;
  float local_c;
  
  if (in_FPSCR_SZ == '\0') {
    dVar5 = (double)CONCAT44(DAT_8c016acc._4_4_,param_1);
  }
  else {
    dVar5 = (double)CONCAT44(uRam8c016ad4,DAT_8c016acc._4_4_);
  }
  if ((*(ushort *)(param_3 + 6) & 0x40) == 0) {
    iVar3 = ((byte)param_2[1] & 3) + (uint)*(ushort *)(*(int *)(param_2 + 0x68) + 2);
  }
  else {
    iVar3 = ((byte)param_2[1] & 3) + 1;
  }
  dVar4 = in_xd2;
  if (in_FPSCR_SZ == '\0') {
    dVar4 = (double)CONCAT44((float)iVar3,(float)iVar3);
  }
  if (in_FPSCR_PR == '\0') {
    iVar3 = (int)((float)((ulonglong)dVar4 >> 0x20) * (float)((ulonglong)dVar5 >> 0x20));
  }
  else {
    iVar3 = (int)(dVar4 * dVar5);
  }
  if ((param_2[1] & 8U) != 0) {
    iVar3 = -iVar3;
  }
  if ((*(ushort *)(param_3 + 6) & 0x20) != 0) {
    iVar3 = -iVar3;
  }
  *(int *)(param_2 + 0x38) = *(int *)(param_2 + 0x38) + iVar3;
  if (in_FPSCR_SZ == '\0') {
    fStack_18 = *(float *)(param_2 + 0x48);
    fStack_14 = *(float *)(param_2 + 0x4c);
    fStack_10 = *(float *)(param_2 + 0x50);
    local_c = in_PR;
  }
  else {
    fStack_18 = (float)*(undefined8 *)(param_2 + 0x48);
    fStack_14 = (float)*(undefined8 *)(param_2 + 0x4c);
    in_xd2 = *(double *)(param_2 + 0x50);
    fStack_10 = SUB84(in_xd2,0);
    local_c = (float)((ulonglong)in_xd2 >> 0x20);
  }
  dVar4 = (double)(*(code *)PTR_FUN_8c016d40)(*(int *)(param_2 + 0x38) >> 2);
  puVar1 = PTR_FUN_8c016d40;
  uVar11 = SUB84(dVar5,0);
  if (in_FPSCR_SZ == '\0') {
    uVar6 = CONCAT44(DAT_8c016d44,(int)((ulonglong)dVar4 >> 0x20));
    puVar7 = DAT_8c016d48;
    dVar4 = in_xd2;
  }
  else {
    uVar6 = CONCAT44(DAT_8c016d48,DAT_8c016d44);
    puVar7 = PTR_FUN_8c016d4c;
  }
  if (in_FPSCR_PR == '\0') {
    uVar6 = CONCAT44((float)puVar7 * (float)uVar6 + (float)((ulonglong)uVar6 >> 0x20),(float)uVar6);
  }
  if (in_FPSCR_SZ == '\0') {
    *(int *)(param_2 + 0x3c) = (int)((ulonglong)uVar6 >> 0x20);
  }
  else {
    *(undefined8 *)(param_2 + 0x3c) = uVar6;
  }
  fVar8 = (float)(*(code *)puVar1)(*(undefined4 *)(param_2 + 0x38));
  if (in_FPSCR_SZ == '\0') {
    fVar2 = *(float *)(param_2 + 0x3c);
    fVar10 = fStack_18;
  }
  else {
    dVar4 = *(double *)(param_2 + 0x3c);
    fVar2 = fStack_18;
    fVar10 = fStack_14;
  }
  fStack_18 = fVar2;
  if (in_FPSCR_PR == '\0') {
    fVar10 = fVar8 * fStack_18 + fVar10;
  }
  if (in_FPSCR_SZ == '\0') {
    fStack_18 = fVar10;
    fVar10 = fStack_14;
  }
  fStack_14 = fVar10;
  fVar8 = (float)(*(code *)PTR_FUN_8c016d4c)(*(undefined4 *)(param_2 + 0x38));
  if (in_FPSCR_SZ == '\0') {
    fVar2 = *(float *)(param_2 + 0x3c);
    fVar10 = fStack_10;
  }
  else {
    dVar4 = *(double *)(param_2 + 0x3c);
    fVar2 = fStack_10;
    fVar10 = local_c;
  }
  fStack_10 = fVar2;
  if (in_FPSCR_PR == '\0') {
    fVar10 = fVar8 * fStack_10 + fVar10;
  }
  if (in_FPSCR_SZ == '\0') {
    fStack_10 = fVar10;
    fVar10 = local_c;
  }
  local_c = fVar10;
  dVar5 = (double)(*(code *)PTR_FUN_8c016d4c)(*(int *)(param_2 + 0x38) << 1);
  if (in_FPSCR_SZ == '\0') {
    dVar9 = (double)CONCAT44(DAT_8c016d50,(int)((ulonglong)dVar5 >> 0x20));
    uVar6 = CONCAT44(_DAT_8c016d54,SUB84(dVar5,0));
    dVar5 = dVar4;
  }
  else {
    dVar9 = (double)CONCAT44(_DAT_8c016d54,DAT_8c016d50);
    uVar6 = CONCAT44(_DAT_8c016d58,_DAT_8c016d54);
  }
  if (in_FPSCR_PR == '\0') {
    dVar9 = (double)CONCAT44((float)((ulonglong)uVar6 >> 0x20) * SUB84(dVar9,0) +
                             (float)((ulonglong)dVar9 >> 0x20),SUB84(dVar9,0));
  }
  if (in_FPSCR_SZ == '\0') {
    dVar9 = (double)CONCAT44((int)((ulonglong)dVar9 >> 0x20),fStack_14);
  }
  else {
    dVar5 = (double)CONCAT44(fStack_10,fStack_14);
  }
  if (in_FPSCR_PR == '\0') {
    fVar8 = (float)((ulonglong)dVar9 >> 0x20);
    dVar9 = (double)CONCAT44(fVar8,SUB84(dVar9,0) + fVar8);
  }
  else {
    dVar9 = dVar9 + dVar9;
  }
  if (in_FPSCR_SZ == '\0') {
    fStack_14 = SUB84(dVar9,0);
  }
  else {
    fStack_14 = SUB84(dVar5,0);
    fStack_10 = (float)((ulonglong)dVar5 >> 0x20);
  }
  if (((*(ushort *)(param_3 + 6) & 0x40) == 0) && ((*(ushort *)(param_3 + 6) & 0x100) != 0)) {
    if (in_FPSCR_SZ == '\0') {
      fVar8 = *(float *)(param_2 + 0x4c);
    }
    else {
      fVar8 = (float)((ulonglong)*(undefined8 *)(param_2 + 0x4c) >> 0x20);
    }
    if (in_FPSCR_PR == '\0') {
      dVar9 = (double)CONCAT44(fVar8 + _DAT_8c016d58,_DAT_8c016d58);
    }
    else {
      dVar9 = (double)CONCAT44(fVar8,_DAT_8c016d58) + (double)CONCAT44(fVar8,_DAT_8c016d58);
    }
    if (in_FPSCR_SZ == '\0') {
      *(int *)(param_2 + 0x4c) = (int)((ulonglong)dVar9 >> 0x20);
    }
    else {
      *(double *)(param_2 + 0x4c) = dVar9;
    }
  }
  if (in_FPSCR_SZ == '\0') {
    dVar4 = (double)CONCAT44(fStack_18,uVar11);
  }
  else {
    dVar4 = (double)CONCAT44(fStack_14,fStack_18);
  }
  uVar11 = (undefined4)((ulonglong)dVar9 >> 0x20);
  fVar8 = (float)((ulonglong)dVar4 >> 0x20);
  if ((*param_2 == '\x04') && ((*(ushort *)(param_3 + 6) & 0x40) == 0)) {
    if (in_FPSCR_SZ == '\0') {
      dVar9 = (double)CONCAT44(uVar11,*(undefined4 *)(param_3 + 0x20));
    }
    else {
      dVar5 = *(double *)(param_3 + 0x20);
    }
    if (in_FPSCR_PR == '\0') {
      dVar4 = (double)CONCAT44(fVar8 - SUB84(dVar9,0),SUB84(dVar4,0));
    }
    else {
      dVar4 = dVar4 - dVar9;
    }
    if (in_FPSCR_SZ == '\0') {
      dVar9 = (double)CONCAT44(fStack_14,*(undefined4 *)(param_3 + 0x24));
      local_24 = (int)((ulonglong)dVar4 >> 0x20);
    }
    else {
      dVar5 = *(double *)(param_3 + 0x24);
      dVar9 = (double)CONCAT44(fStack_10,fStack_14);
      local_24 = SUB84(dVar4,0);
    }
    if (in_FPSCR_PR == '\0') {
      dVar9 = (double)CONCAT44((float)((ulonglong)dVar9 >> 0x20) - SUB84(dVar9,0),SUB84(dVar9,0));
    }
    else {
      dVar9 = dVar9 - dVar9;
    }
    if (in_FPSCR_SZ == '\0') {
      uStack_20 = (undefined4)((ulonglong)dVar9 >> 0x20);
      dVar9 = (double)CONCAT44(fStack_10,*(undefined4 *)(param_3 + 0x28));
    }
    else {
      uStack_20 = SUB84(dVar9,0);
      dVar5 = *(double *)(param_3 + 0x28);
      dVar9 = (double)CONCAT44(local_c,fStack_10);
    }
    if (in_FPSCR_PR == '\0') {
      dVar9 = (double)CONCAT44((float)((ulonglong)dVar9 >> 0x20) - SUB84(dVar9,0),SUB84(dVar9,0));
    }
    else {
      dVar9 = dVar9 - dVar9;
    }
    if (in_FPSCR_SZ == '\0') {
      local_1c = (float)((ulonglong)dVar9 >> 0x20);
      dVar4 = (double)CONCAT44(DAT_8c016d5c,SUB84(dVar4,0));
    }
    else {
      local_1c = SUB84(dVar9,0);
      dVar4 = (double)CONCAT44(DAT_8c016d60,DAT_8c016d5c);
    }
  }
  else {
    if (in_FPSCR_SZ == '\0') {
      dVar9 = (double)CONCAT44(uVar11,*(undefined4 *)(param_3 + 0x20));
    }
    else {
      dVar5 = *(double *)(param_3 + 0x20);
    }
    if (in_FPSCR_PR == '\0') {
      dVar4 = (double)CONCAT44(fVar8 - SUB84(dVar9,0),SUB84(dVar4,0));
    }
    else {
      dVar4 = dVar4 - dVar9;
    }
    if (in_FPSCR_SZ == '\0') {
      dVar9 = (double)CONCAT44(fStack_14,*(undefined4 *)(param_3 + 0x24));
      local_24 = (int)((ulonglong)dVar4 >> 0x20);
    }
    else {
      dVar5 = *(double *)(param_3 + 0x24);
      dVar9 = (double)CONCAT44(fStack_10,fStack_14);
      local_24 = SUB84(dVar4,0);
    }
    if (in_FPSCR_PR == '\0') {
      dVar9 = (double)CONCAT44((float)((ulonglong)dVar9 >> 0x20) - SUB84(dVar9,0),SUB84(dVar9,0));
    }
    else {
      dVar9 = dVar9 - dVar9;
    }
    if (in_FPSCR_SZ == '\0') {
      uStack_20 = (undefined4)((ulonglong)dVar9 >> 0x20);
      dVar9 = (double)CONCAT44(fStack_10,*(undefined4 *)(param_3 + 0x28));
    }
    else {
      uStack_20 = SUB84(dVar9,0);
      dVar5 = *(double *)(param_3 + 0x28);
      dVar9 = (double)CONCAT44(local_c,fStack_10);
    }
    if (in_FPSCR_PR == '\0') {
      dVar9 = (double)CONCAT44((float)((ulonglong)dVar9 >> 0x20) - SUB84(dVar9,0),SUB84(dVar9,0));
    }
    else {
      dVar9 = dVar9 - dVar9;
    }
    if (in_FPSCR_SZ == '\0') {
      local_1c = (float)((ulonglong)dVar9 >> 0x20);
      dVar4 = (double)CONCAT44(DAT_8c016d60,SUB84(dVar4,0));
    }
    else {
      local_1c = SUB84(dVar9,0);
      dVar4 = (double)CONCAT44(DAT_8c016d64,DAT_8c016d60);
    }
  }
  if (in_FPSCR_SZ == '\0') {
    dVar9 = (double)CONCAT44((int)((ulonglong)dVar9 >> 0x20),local_24);
  }
  else {
    dVar5 = (double)CONCAT44(uStack_20,local_24);
  }
  fVar8 = (float)((ulonglong)dVar4 >> 0x20);
  if (in_FPSCR_PR == '\0') {
    dVar9 = (double)CONCAT44((int)((ulonglong)dVar9 >> 0x20),SUB84(dVar9,0) * fVar8);
  }
  else {
    dVar9 = dVar9 * dVar4;
  }
  if (in_FPSCR_SZ == '\0') {
    local_24 = SUB84(dVar9,0);
    dVar9 = (double)CONCAT44((int)((ulonglong)dVar9 >> 0x20),uStack_20);
  }
  else {
    local_24 = SUB84(dVar5,0);
    uStack_20 = (undefined4)((ulonglong)dVar5 >> 0x20);
    dVar5 = (double)CONCAT44(local_1c,uStack_20);
  }
  if (in_FPSCR_PR == '\0') {
    dVar9 = (double)CONCAT44((int)((ulonglong)dVar9 >> 0x20),SUB84(dVar9,0) * fVar8);
  }
  else {
    dVar9 = dVar9 * dVar4;
  }
  if (in_FPSCR_SZ == '\0') {
    uStack_20 = SUB84(dVar9,0);
    dVar9 = (double)CONCAT44((int)((ulonglong)dVar9 >> 0x20),local_1c);
  }
  else {
    uStack_20 = SUB84(dVar5,0);
    dVar5 = (double)((ulonglong)dVar5 >> 0x20);
  }
  if (in_FPSCR_PR == '\0') {
    local_1c = SUB84(dVar9,0) * fVar8;
  }
  else {
    local_1c = SUB84(dVar9 * dVar4,0);
  }
  if (in_FPSCR_SZ == '\0') {
    dVar5 = (double)CONCAT44(*(undefined4 *)(param_2 + 0x14),local_24);
  }
  else {
    local_1c = SUB84(dVar5,0);
    dVar5 = *(double *)(param_2 + 0x14);
  }
  if (in_FPSCR_PR == '\0') {
    dVar5 = (double)CONCAT44((float)((ulonglong)dVar5 >> 0x20) + SUB84(dVar5,0),SUB84(dVar5,0));
  }
  else {
    dVar5 = dVar5 + dVar5;
  }
  if (in_FPSCR_SZ == '\0') {
    *(int *)(param_2 + 0x14) = (int)((ulonglong)dVar5 >> 0x20);
    dVar5 = (double)CONCAT44(*(undefined4 *)(param_2 + 0x18),uStack_20);
  }
  else {
    *(double *)(param_2 + 0x14) = dVar5;
    dVar5 = *(double *)(param_2 + 0x18);
  }
  if (in_FPSCR_PR == '\0') {
    dVar5 = (double)CONCAT44((float)((ulonglong)dVar5 >> 0x20) + SUB84(dVar5,0),SUB84(dVar5,0));
  }
  else {
    dVar5 = dVar5 + dVar5;
  }
  if (in_FPSCR_SZ == '\0') {
    *(int *)(param_2 + 0x18) = (int)((ulonglong)dVar5 >> 0x20);
    dVar5 = (double)CONCAT44(*(undefined4 *)(param_2 + 0x1c),local_1c);
  }
  else {
    *(double *)(param_2 + 0x18) = dVar5;
    dVar5 = *(double *)(param_2 + 0x1c);
  }
  if (in_FPSCR_PR == '\0') {
    dVar5 = (double)CONCAT44((float)((ulonglong)dVar5 >> 0x20) + SUB84(dVar5,0),SUB84(dVar5,0));
  }
  else {
    dVar5 = dVar5 + dVar5;
  }
  if (in_FPSCR_SZ == '\0') {
    *(int *)(param_2 + 0x1c) = (int)((ulonglong)dVar5 >> 0x20);
  }
  else {
    *(double *)(param_2 + 0x1c) = dVar5;
  }
  return uVar6;
}


/* Address: 8c016c60 */

/* WARNING: Removing unreachable block (ram,0x8c017244) */
/* WARNING: Removing unreachable block (ram,0x8c016e64) */
/* WARNING: Removing unreachable block (ram,0x8c016f1e) */
/* WARNING: Removing unreachable block (ram,0x8c0172fe) */
/* WARNING: Removing unreachable block (ram,0x8c017252) */
/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_8c016c60(int param_1,int param_2)

{
  undefined *puVar1;
  ushort uVar2;
  int iVar3;
  undefined4 *puVar4;
  undefined8 *puVar5;
  float *pfVar6;
  float *pfVar7;
  undefined4 extraout_fr1;
  double dVar8;
  double extraout_dr0;
  undefined8 in_dr2;
  double dVar9;
  float fVar11;
  longlong lVar10;
  undefined4 uVar12;
  float fVar13;
  double dVar14;
  undefined4 uVar15;
  float fVar16;
  undefined4 uVar17;
  undefined8 in_dr12;
  undefined8 in_dr14;
  double dVar18;
  undefined8 in_xd0;
  undefined8 in_xd2;
  undefined8 uVar19;
  undefined8 in_xd4;
  undefined8 in_xd14;
  bool bVar20;
  undefined4 in_PR;
  char in_FPSCR_PR;
  char in_FPSCR_SZ;
  undefined1 auStack_3c [16];
  undefined8 uStack_2c;
  undefined8 uStack_24;
  
  uVar15 = DAT_8c016d70;
  uVar17 = (undefined4)in_dr12;
  if (in_FPSCR_SZ == '\0') {
    uStack_24 = CONCAT44((int)in_dr14,(undefined4)uStack_24);
                    /* WARNING: Ignoring partial resolution of indirect */
    uStack_24._0_4_ = (int)((ulonglong)in_dr14 >> 0x20);
    puVar5 = &uStack_2c;
    uStack_2c = CONCAT44(uVar17,(int)((ulonglong)in_dr12 >> 0x20));
  }
  else {
    puVar5 = (undefined8 *)auStack_3c;
    uStack_24 = in_xd14;
    uStack_2c = in_dr14;
  }
  *(undefined4 *)((int)puVar5 + -4) = in_PR;
  pfVar6 = (float *)((int)puVar5 + -0x128);
  uVar12 = (undefined4)((ulonglong)in_dr2 >> 0x20);
  dVar9 = (double)CONCAT44(uVar12,0x3f800000);
  *(undefined4 *)((int)puVar5 + -0xfc) = DAT_8c016d64;
  *(undefined4 *)((int)puVar5 + -0xe8) = 0;
  *(undefined4 *)((int)puVar5 + -0xec) = 0;
  if (in_FPSCR_SZ == '\0') {
    *(undefined4 *)((int)puVar5 + -0xe0) = 0x3f800000;
    uVar17 = DAT_8c016d68;
  }
  else {
    *(undefined8 *)((int)puVar5 + -0xe0) = in_xd2;
  }
  *(uint *)((int)puVar5 + -0xe4) = *(ushort *)(param_2 + 6) & 8;
  *(ushort *)(param_2 + 6) = *(ushort *)(param_2 + 6) & (ushort)_DAT_8c016d6c;
  *(ushort *)(param_2 + 6) = *(ushort *)(param_2 + 6) & (ushort)uVar15;
  if (in_FPSCR_SZ == '\0') {
    *(undefined4 *)((int)puVar5 + -0xd0) = *(undefined4 *)(param_2 + 0x20);
    *(undefined4 *)((int)puVar5 + -0xcc) = *(undefined4 *)(param_2 + 0x24);
    dVar9 = (double)CONCAT44(uVar12,*(undefined4 *)(param_2 + 0x28));
    *(undefined4 *)((int)puVar5 + -200) = *(undefined4 *)(param_2 + 0x28);
    uVar12 = *(undefined4 *)(param_2 + 0x24);
    uVar15 = DAT_8c016d74;
  }
  else {
    *(undefined8 *)((int)puVar5 + -0xd0) = *(undefined8 *)(param_2 + 0x20);
    *(undefined8 *)((int)puVar5 + -0xcc) = *(undefined8 *)(param_2 + 0x24);
    in_xd2 = *(undefined8 *)(param_2 + 0x28);
    *(undefined8 *)((int)puVar5 + -200) = in_xd2;
    uVar15 = (undefined4)*(undefined8 *)(param_2 + 0x28);
    in_xd4 = *(undefined8 *)(param_2 + 0x24);
    uVar12 = (undefined4)*(undefined8 *)(param_2 + 0x20);
  }
  dVar8 = (double)(*(code *)PTR_FUN_8c016d78)();
  dVar18 = (double)CONCAT44(0x3f800000,(int)in_dr14);
  if (in_FPSCR_PR == '\0') {
    dVar18 = (double)CONCAT44(0x40000000,(int)in_dr14);
  }
  else {
    dVar18 = dVar18 + dVar18;
  }
  fVar11 = (float)((ulonglong)dVar18 >> 0x20);
  if ((*(ushort *)(param_2 + 6) & 0x40) == 0) {
    if (in_FPSCR_SZ == '\0') {
      fVar13 = *(float *)(param_2 + 0x20);
      dVar9 = (double)CONCAT44(fVar13,DAT_8c016fe0);
      dVar14 = (double)CONCAT44(fVar13,uVar12);
    }
    else {
      dVar9 = *(double *)(param_2 + 0x20);
      in_xd2 = CONCAT44(_DAT_8c016fe4,DAT_8c016fe0);
      fVar13 = (float)((ulonglong)dVar9 >> 0x20);
      dVar14 = dVar9;
    }
    if (in_FPSCR_PR == '\0') {
      dVar14 = (double)CONCAT44(fVar13 + fVar11,SUB84(dVar14,0));
      dVar9 = (double)CONCAT44((float)((ulonglong)dVar9 >> 0x20) + SUB84(dVar9,0),SUB84(dVar9,0));
    }
    else {
      dVar14 = dVar14 + dVar18;
      dVar9 = dVar9 + dVar9;
    }
    if (in_FPSCR_SZ == '\0') {
      *(int *)((int)puVar5 + -0x104) = (int)((ulonglong)dVar9 >> 0x20);
      *(int *)((int)puVar5 + -0x10c) = (int)((ulonglong)dVar14 >> 0x20);
      fVar13 = *(float *)(param_2 + 0x28);
      dVar9 = (double)CONCAT44(fVar13,SUB84(dVar9,0));
      dVar14 = (double)CONCAT44(fVar13,SUB84(dVar14,0));
    }
    else {
      *(double *)((int)puVar5 + -0x104) = dVar9;
      *(double *)((int)puVar5 + -0x10c) = dVar14;
      dVar9 = *(double *)(param_2 + 0x28);
      fVar13 = (float)((ulonglong)dVar9 >> 0x20);
      dVar14 = dVar9;
    }
    if (in_FPSCR_PR == '\0') {
      dVar9 = (double)CONCAT44((float)((ulonglong)dVar9 >> 0x20) + SUB84(dVar9,0),SUB84(dVar9,0));
      dVar14 = (double)CONCAT44(fVar13 + fVar11,SUB84(dVar14,0));
    }
    else {
      dVar9 = dVar9 + dVar9;
      dVar14 = dVar14 + dVar18;
    }
    if (in_FPSCR_SZ == '\0') {
      *(int *)((int)puVar5 + -0x114) = (int)((ulonglong)dVar9 >> 0x20);
      *(int *)((int)puVar5 + -0x110) = (int)((ulonglong)dVar14 >> 0x20);
    }
    else {
      *(double *)((int)puVar5 + -0x114) = dVar9;
      *(double *)((int)puVar5 + -0x110) = dVar14;
    }
  }
  else {
    if (in_FPSCR_SZ == '\0') {
      dVar18 = (double)CONCAT44(*(undefined4 *)(param_1 + 0x20),uVar15);
      dVar14 = (double)CONCAT44(DAT_8c016d50,*(undefined4 *)(param_2 + 0x20));
    }
    else {
      dVar14 = (double)CONCAT44(_DAT_8c016d54,DAT_8c016d50);
      dVar18 = *(double *)(param_1 + 0x20);
      in_xd4 = *(undefined8 *)(param_2 + 0x20);
    }
    fVar13 = (float)((ulonglong)dVar14 >> 0x20);
    if (in_FPSCR_PR == '\0') {
      dVar18 = (double)CONCAT44((float)((ulonglong)dVar18 >> 0x20) * fVar13,SUB84(dVar18,0));
    }
    else {
      dVar18 = dVar18 * dVar14;
    }
    uVar19 = in_xd4;
    if (in_FPSCR_SZ == '\0') {
      dVar9 = (double)CONCAT44((int)((ulonglong)dVar9 >> 0x20),SUB84(dVar14,0));
      uVar19 = in_xd2;
    }
    fVar16 = (float)((ulonglong)dVar18 >> 0x20);
    if (in_FPSCR_PR == '\0') {
      dVar9 = (double)CONCAT44((int)((ulonglong)dVar9 >> 0x20),SUB84(dVar9,0) - fVar16);
      dVar14 = (double)CONCAT44(fVar13,SUB84(dVar14,0) + fVar16);
    }
    else {
      dVar9 = dVar9 - dVar18;
      dVar14 = dVar14 + dVar18;
    }
    if (in_FPSCR_SZ == '\0') {
      *(int *)((int)puVar5 + -0x104) = SUB84(dVar9,0);
      *(int *)((int)puVar5 + -0x10c) = SUB84(dVar14,0);
      dVar18 = (double)CONCAT44(*(undefined4 *)(param_1 + 0x28),SUB84(dVar18,0));
      dVar14 = (double)CONCAT44((int)((ulonglong)dVar14 >> 0x20),*(undefined4 *)(param_2 + 0x28));
    }
    else {
      *(undefined8 *)((int)puVar5 + -0x104) = uVar19;
      *(undefined8 *)((int)puVar5 + -0x10c) = in_xd4;
      dVar18 = *(double *)(param_1 + 0x28);
      in_xd4 = *(undefined8 *)(param_2 + 0x28);
    }
    if (in_FPSCR_PR == '\0') {
      dVar18 = (double)CONCAT44((float)((ulonglong)dVar18 >> 0x20) *
                                (float)((ulonglong)dVar14 >> 0x20),SUB84(dVar18,0));
    }
    else {
      dVar18 = dVar18 * dVar14;
    }
    in_xd2 = in_xd4;
    if (in_FPSCR_SZ == '\0') {
      dVar9 = (double)CONCAT44((int)((ulonglong)dVar9 >> 0x20),SUB84(dVar14,0));
      in_xd2 = uVar19;
    }
    fVar13 = (float)((ulonglong)dVar18 >> 0x20);
    if (in_FPSCR_PR == '\0') {
      dVar9 = (double)CONCAT44((int)((ulonglong)dVar9 >> 0x20),SUB84(dVar9,0) - fVar13);
      fVar13 = SUB84(dVar14,0) + fVar13;
    }
    else {
      dVar9 = dVar9 - dVar18;
      fVar13 = SUB84(dVar14 + dVar18,0);
    }
    if (in_FPSCR_SZ == '\0') {
      *(int *)((int)puVar5 + -0x114) = SUB84(dVar9,0);
      *(float *)((int)puVar5 + -0x110) = fVar13;
    }
    else {
      *(undefined8 *)((int)puVar5 + -0x114) = in_xd2;
      *(undefined8 *)((int)puVar5 + -0x110) = in_xd4;
    }
  }
  dVar18 = (double)((ulonglong)(uint)fVar11 << 0x20);
  *(undefined4 *)((int)puVar5 + -0x118) = 0;
  *(undefined4 *)((int)puVar5 + -0x108) = *(undefined4 *)((int)puVar5 + -0xfc);
  if (in_FPSCR_SZ == '\0') {
    dVar14 = (double)CONCAT44(_DAT_8c016fe4,uVar17);
  }
  else {
    dVar14 = (double)CONCAT44(PTR_FUN_8c016fe8,_DAT_8c016fe4);
  }
  puVar4 = *(undefined4 **)((int)puVar5 + -0xfc);
  while ((int)pfVar6[4] < (int)*DAT_8c017400) {
    iVar3 = (*(code *)PTR_FUN_8c016fe8)(pfVar6 + 0x16,puVar4[1],*puVar4);
    dVar8 = extraout_dr0;
    if (iVar3 != 0) {
      fVar11 = SUB84(dVar18,0);
      if (in_FPSCR_SZ == '\0') {
        pfVar6[0x13] = fVar11;
        pfVar6[0x14] = fVar11;
        pfVar6[0x15] = fVar11;
      }
      else {
        *(undefined8 *)(pfVar6 + 0x13) = in_xd14;
        *(undefined8 *)(pfVar6 + 0x14) = in_xd14;
        *(undefined8 *)(pfVar6 + 0x15) = in_xd14;
      }
      if ((*(uint *)pfVar6[8] & 2) == 0) {
        if (in_FPSCR_SZ == '\0') {
          pfVar6[0xc] = pfVar6[0x1f];
          dVar9 = (double)CONCAT44((int)((ulonglong)dVar9 >> 0x20),pfVar6[0x1e]);
          *pfVar6 = pfVar6[0x1e];
        }
        else {
          *(undefined8 *)(pfVar6 + 0xc) = *(undefined8 *)(pfVar6 + 0x1f);
          in_xd2 = *(undefined8 *)(pfVar6 + 0x1e);
          *(undefined8 *)pfVar6 = in_xd2;
        }
        if (pfVar6[0x19] != 0.0) {
          dVar8 = (double)(*(code *)PTR_FUN_8c016fec)(0x11c7);
          if (in_FPSCR_SZ == '\0') {
            dVar9 = (double)CONCAT44((int)((ulonglong)dVar9 >> 0x20),pfVar6[0xc]);
          }
          else {
            in_xd2 = *(undefined8 *)(pfVar6 + 0xc);
          }
          fVar11 = (float)((ulonglong)dVar8 >> 0x20);
          if (in_FPSCR_PR == '\0') {
            bVar20 = SUB84(dVar9,0) < fVar11;
          }
          else {
            bVar20 = dVar9 < dVar8;
          }
          if (bVar20) {
            if (in_FPSCR_SZ == '\0') {
              fVar13 = *pfVar6;
            }
            else {
              fVar13 = (float)((ulonglong)*(undefined8 *)pfVar6 >> 0x20);
            }
            if (in_FPSCR_PR == '\0' && fVar13 < 0.0) {
LAB_8c016e36:
              if (in_FPSCR_SZ == '\0') {
                fVar11 = *pfVar6;
              }
              else {
                fVar11 = (float)((ulonglong)*(undefined8 *)pfVar6 >> 0x20);
              }
              dVar9 = (double)((ulonglong)(uint)fVar11 << 0x20);
              if (in_FPSCR_PR == '\0' && fVar11 < 0.0) {
                if (in_FPSCR_SZ == '\0') {
                  dVar9 = (double)((ulonglong)*(uint *)(param_1 + 8) << 0x20);
                  dVar8 = (double)CONCAT44((int)((ulonglong)dVar8 >> 0x20),pfVar6[7]);
                }
                else {
                  dVar9 = *(double *)(param_1 + 8);
                  in_xd0 = *(undefined8 *)(pfVar6 + 7);
                }
                if (in_FPSCR_PR == '\0') {
                  dVar8 = (double)CONCAT44((int)((ulonglong)dVar8 >> 0x20),
                                           SUB84(dVar8,0) + (float)((ulonglong)dVar9 >> 0x20));
                }
                else {
                  dVar8 = dVar8 + dVar9;
                }
                if (in_FPSCR_SZ == '\0') {
                  dVar9 = (double)CONCAT44(pfVar6[0x1d],SUB84(dVar9,0));
                }
                else {
                  dVar9 = *(double *)(pfVar6 + 0x1d);
                }
                if (in_FPSCR_PR == '\0') {
                  bVar20 = (float)((ulonglong)dVar9 >> 0x20) < SUB84(dVar8,0);
                }
                else {
                  bVar20 = dVar9 < dVar8;
                }
                if (bVar20) goto LAB_8c016e50;
              }
            }
            else {
              if (in_FPSCR_SZ == '\0') {
                dVar9 = (double)((ulonglong)*(uint *)(param_1 + 8) << 0x20);
                dVar8 = (double)CONCAT44(fVar11,pfVar6[9]);
              }
              else {
                dVar9 = *(double *)(param_1 + 8);
                in_xd0 = *(undefined8 *)(pfVar6 + 9);
              }
              if (in_FPSCR_PR == '\0') {
                dVar8 = (double)CONCAT44((int)((ulonglong)dVar8 >> 0x20),
                                         SUB84(dVar8,0) + (float)((ulonglong)dVar9 >> 0x20));
              }
              else {
                dVar8 = dVar8 + dVar9;
              }
              if (in_FPSCR_SZ == '\0') {
                dVar9 = (double)CONCAT44(pfVar6[0x1d],SUB84(dVar9,0));
              }
              else {
                dVar9 = *(double *)(pfVar6 + 0x1d);
              }
              if (in_FPSCR_PR == '\0') {
                bVar20 = SUB84(dVar8,0) < (float)((ulonglong)dVar9 >> 0x20);
              }
              else {
                bVar20 = dVar8 < dVar9;
              }
              if (!bVar20) goto LAB_8c016e36;
LAB_8c016e50:
              if (in_FPSCR_SZ == '\0') {
                lVar10 = (ulonglong)*(uint *)(param_1 + 8) << 0x20;
              }
              else {
                lVar10 = *(longlong *)(param_1 + 8);
              }
              if (in_FPSCR_PR == '\0' && (float)((ulonglong)lVar10 >> 0x20) < (float)lVar10) {
                if (in_FPSCR_SZ == '\0') {
                  dVar9 = (double)CONCAT44(DAT_8c016ff0,(float)lVar10);
                  dVar8 = (double)CONCAT44((int)((ulonglong)dVar8 >> 0x20),
                                           *(undefined4 *)(param_1 + 8));
                }
                else {
                  dVar9 = (double)CONCAT44(uRam8c016ff4,DAT_8c016ff0);
                  in_xd0 = *(undefined8 *)(param_1 + 8);
                }
                if (in_FPSCR_PR == '\0') {
                  dVar9 = (double)CONCAT44((int)((ulonglong)dVar8 >> 0x20),
                                           SUB84(dVar8,0) * (float)((ulonglong)dVar9 >> 0x20));
                }
                else {
                  dVar9 = dVar8 * dVar9;
                }
                dVar8 = (double)CONCAT44((int)((ulonglong)dVar9 >> 0x20),-SUB84(dVar9,0));
                if (in_FPSCR_SZ == '\0') {
                  *(float *)(param_1 + 8) = -SUB84(dVar9,0);
                }
                else {
                  *(undefined8 *)(param_1 + 8) = in_xd0;
                }
              }
              if (in_FPSCR_SZ == '\0') {
                dVar9 = (double)CONCAT44(pfVar6[0x13],pfVar6[0x1e]);
              }
              else {
                in_xd2 = *(undefined8 *)(pfVar6 + 0x1e);
                dVar9 = *(double *)(pfVar6 + 0x13);
              }
              if (in_FPSCR_PR == '\0') {
                dVar9 = (double)CONCAT44((float)((ulonglong)dVar9 >> 0x20) + SUB84(dVar9,0),
                                         SUB84(dVar9,0));
              }
              else {
                dVar9 = dVar9 + dVar9;
              }
              if (in_FPSCR_SZ == '\0') {
                pfVar6[0x13] = (float)((ulonglong)dVar9 >> 0x20);
                dVar9 = (double)CONCAT44(pfVar6[0x14],pfVar6[0x1f]);
              }
              else {
                *(double *)(pfVar6 + 0x13) = dVar9;
                in_xd2 = *(undefined8 *)(pfVar6 + 0x1f);
                dVar9 = *(double *)(pfVar6 + 0x14);
              }
              if (in_FPSCR_PR == '\0') {
                dVar9 = (double)CONCAT44((float)((ulonglong)dVar9 >> 0x20) + SUB84(dVar9,0),
                                         SUB84(dVar9,0));
              }
              else {
                dVar9 = dVar9 + dVar9;
              }
              if (in_FPSCR_SZ == '\0') {
                pfVar6[0x14] = (float)((ulonglong)dVar9 >> 0x20);
                dVar9 = (double)CONCAT44(pfVar6[0x15],pfVar6[0x20]);
              }
              else {
                *(double *)(pfVar6 + 0x14) = dVar9;
                in_xd2 = *(undefined8 *)(pfVar6 + 0x20);
                dVar9 = *(double *)(pfVar6 + 0x15);
              }
              if (in_FPSCR_PR == '\0') {
                dVar9 = (double)CONCAT44((float)((ulonglong)dVar9 >> 0x20) + SUB84(dVar9,0),
                                         SUB84(dVar9,0));
              }
              else {
                dVar9 = dVar9 + dVar9;
              }
              if (in_FPSCR_SZ == '\0') {
                pfVar6[0x15] = (float)((ulonglong)dVar9 >> 0x20);
              }
              else {
                *(double *)(pfVar6 + 0x15) = dVar9;
              }
              pfVar6[0x10] = 1.4013e-45;
              *(char *)(param_1 + 2) = *(char *)(param_1 + 2) + '\x01';
            }
          }
        }
        if (in_FPSCR_SZ == '\0') {
          pfVar6[0xd] = pfVar6[0x27];
          dVar9 = (double)CONCAT44((int)((ulonglong)dVar9 >> 0x20),pfVar6[0x26]);
          pfVar6[1] = pfVar6[0x26];
        }
        else {
          *(undefined8 *)(pfVar6 + 0xd) = *(undefined8 *)(pfVar6 + 0x27);
          in_xd2 = *(undefined8 *)(pfVar6 + 0x26);
          *(undefined8 *)(pfVar6 + 1) = in_xd2;
        }
        if (pfVar6[0x21] != 0.0) {
          dVar8 = (double)(*(code *)PTR_FUN_8c016fec)(0x11c7);
          if (in_FPSCR_SZ == '\0') {
            dVar9 = (double)CONCAT44((int)((ulonglong)dVar9 >> 0x20),pfVar6[0xd]);
          }
          else {
            in_xd2 = *(undefined8 *)(pfVar6 + 0xd);
          }
          fVar11 = (float)((ulonglong)dVar8 >> 0x20);
          if (in_FPSCR_PR == '\0') {
            bVar20 = SUB84(dVar9,0) < fVar11;
          }
          else {
            bVar20 = dVar9 < dVar8;
          }
          if (bVar20) {
            if (in_FPSCR_SZ == '\0') {
              lVar10 = (ulonglong)(uint)pfVar6[1] << 0x20;
            }
            else {
              lVar10 = *(longlong *)(pfVar6 + 1);
            }
            if (in_FPSCR_PR == '\0' && (float)lVar10 < (float)((ulonglong)lVar10 >> 0x20)) {
LAB_8c016eec:
              if (in_FPSCR_SZ == '\0') {
                dVar9 = (double)((ulonglong)(uint)pfVar6[1] << 0x20);
              }
              else {
                dVar9 = *(double *)(pfVar6 + 1);
              }
              if (in_FPSCR_PR == '\0' && SUB84(dVar9,0) < (float)((ulonglong)dVar9 >> 0x20)) {
                if (in_FPSCR_SZ == '\0') {
                  dVar9 = (double)CONCAT44(*(undefined4 *)(param_1 + 8),SUB84(dVar9,0));
                  dVar8 = (double)CONCAT44((int)((ulonglong)dVar8 >> 0x20),pfVar6[9]);
                }
                else {
                  dVar9 = *(double *)(param_1 + 8);
                  in_xd0 = *(undefined8 *)(pfVar6 + 9);
                }
                if (in_FPSCR_PR == '\0') {
                  dVar8 = (double)CONCAT44((int)((ulonglong)dVar8 >> 0x20),
                                           SUB84(dVar8,0) + (float)((ulonglong)dVar9 >> 0x20));
                }
                else {
                  dVar8 = dVar8 + dVar9;
                }
                if (in_FPSCR_SZ == '\0') {
                  dVar9 = (double)CONCAT44(pfVar6[0x25],SUB84(dVar9,0));
                }
                else {
                  dVar9 = *(double *)(pfVar6 + 0x25);
                }
                if (in_FPSCR_PR == '\0') {
                  bVar20 = SUB84(dVar8,0) < (float)((ulonglong)dVar9 >> 0x20);
                }
                else {
                  bVar20 = dVar8 < dVar9;
                }
                if (bVar20) goto LAB_8c016f0a;
              }
            }
            else {
              if (in_FPSCR_SZ == '\0') {
                dVar9 = (double)CONCAT44(*(undefined4 *)(param_1 + 8),(float)lVar10);
                dVar8 = (double)CONCAT44(fVar11,pfVar6[7]);
              }
              else {
                dVar9 = *(double *)(param_1 + 8);
                in_xd0 = *(undefined8 *)(pfVar6 + 7);
              }
              if (in_FPSCR_PR == '\0') {
                dVar8 = (double)CONCAT44((int)((ulonglong)dVar8 >> 0x20),
                                         SUB84(dVar8,0) + (float)((ulonglong)dVar9 >> 0x20));
              }
              else {
                dVar8 = dVar8 + dVar9;
              }
              if (in_FPSCR_SZ == '\0') {
                dVar9 = (double)CONCAT44(pfVar6[0x25],SUB84(dVar9,0));
              }
              else {
                dVar9 = *(double *)(pfVar6 + 0x25);
              }
              if (in_FPSCR_PR == '\0') {
                bVar20 = (float)((ulonglong)dVar9 >> 0x20) < SUB84(dVar8,0);
              }
              else {
                bVar20 = dVar9 < dVar8;
              }
              if (!bVar20) goto LAB_8c016eec;
LAB_8c016f0a:
              if (in_FPSCR_SZ == '\0') {
                lVar10 = (ulonglong)*(uint *)(param_1 + 8) << 0x20;
              }
              else {
                lVar10 = *(longlong *)(param_1 + 8);
              }
              if (in_FPSCR_PR == '\0' && (float)lVar10 < (float)((ulonglong)lVar10 >> 0x20)) {
                if (in_FPSCR_SZ == '\0') {
                  dVar9 = (double)CONCAT44(DAT_8c016ff0,(float)lVar10);
                  dVar8 = (double)CONCAT44((int)((ulonglong)dVar8 >> 0x20),
                                           *(undefined4 *)(param_1 + 8));
                }
                else {
                  dVar9 = (double)CONCAT44(uRam8c016ff4,DAT_8c016ff0);
                  in_xd0 = *(undefined8 *)(param_1 + 8);
                }
                if (in_FPSCR_PR == '\0') {
                  dVar9 = (double)CONCAT44((int)((ulonglong)dVar8 >> 0x20),
                                           SUB84(dVar8,0) * (float)((ulonglong)dVar9 >> 0x20));
                }
                else {
                  dVar9 = dVar8 * dVar9;
                }
                dVar8 = (double)CONCAT44((int)((ulonglong)dVar9 >> 0x20),-SUB84(dVar9,0));
                if (in_FPSCR_SZ == '\0') {
                  *(float *)(param_1 + 8) = -SUB84(dVar9,0);
                }
                else {
                  *(undefined8 *)(param_1 + 8) = in_xd0;
                }
              }
              if (in_FPSCR_SZ == '\0') {
                dVar9 = (double)CONCAT44(pfVar6[0x13],pfVar6[0x26]);
              }
              else {
                in_xd2 = *(undefined8 *)(pfVar6 + 0x26);
                dVar9 = *(double *)(pfVar6 + 0x13);
              }
              if (in_FPSCR_PR == '\0') {
                dVar9 = (double)CONCAT44((float)((ulonglong)dVar9 >> 0x20) + SUB84(dVar9,0),
                                         SUB84(dVar9,0));
              }
              else {
                dVar9 = dVar9 + dVar9;
              }
              if (in_FPSCR_SZ == '\0') {
                pfVar6[0x13] = (float)((ulonglong)dVar9 >> 0x20);
                dVar9 = (double)CONCAT44(pfVar6[0x14],pfVar6[0x27]);
              }
              else {
                *(double *)(pfVar6 + 0x13) = dVar9;
                in_xd2 = *(undefined8 *)(pfVar6 + 0x27);
                dVar9 = *(double *)(pfVar6 + 0x14);
              }
              if (in_FPSCR_PR == '\0') {
                dVar9 = (double)CONCAT44((float)((ulonglong)dVar9 >> 0x20) + SUB84(dVar9,0),
                                         SUB84(dVar9,0));
              }
              else {
                dVar9 = dVar9 + dVar9;
              }
              if (in_FPSCR_SZ == '\0') {
                pfVar6[0x14] = (float)((ulonglong)dVar9 >> 0x20);
                dVar9 = (double)CONCAT44(pfVar6[0x15],pfVar6[0x28]);
              }
              else {
                *(double *)(pfVar6 + 0x14) = dVar9;
                in_xd2 = *(undefined8 *)(pfVar6 + 0x28);
                dVar9 = *(double *)(pfVar6 + 0x15);
              }
              if (in_FPSCR_PR == '\0') {
                dVar9 = (double)CONCAT44((float)((ulonglong)dVar9 >> 0x20) + SUB84(dVar9,0),
                                         SUB84(dVar9,0));
              }
              else {
                dVar9 = dVar9 + dVar9;
              }
              if (in_FPSCR_SZ == '\0') {
                pfVar6[0x15] = (float)((ulonglong)dVar9 >> 0x20);
              }
              else {
                *(double *)(pfVar6 + 0x15) = dVar9;
              }
              pfVar6[0x10] = 1.4013e-45;
              *(char *)(param_1 + 2) = *(char *)(param_1 + 2) + '\x01';
            }
          }
        }
        uVar17 = (undefined4)((ulonglong)dVar14 >> 0x20);
        fVar11 = (float)((ulonglong)dVar18 >> 0x20);
        if (pfVar6[0x29] == 0.0) {
LAB_8c017000:
          if (pfVar6[0x31] != 0.0) {
            if (in_FPSCR_SZ == '\0') {
              dVar9 = (double)((ulonglong)(uint)pfVar6[0x37] << 0x20);
            }
            else {
              dVar9 = *(double *)(pfVar6 + 0x37);
            }
            fVar13 = SUB84(dVar9,0);
            if (in_FPSCR_PR == '\0' && fVar13 < (float)((ulonglong)dVar9 >> 0x20)) {
              if (in_FPSCR_PR == '\0') {
                dVar9 = (double)CONCAT44(0x40000000,fVar13);
              }
              else {
                dVar9 = (double)CONCAT44(0x3f800000,fVar13) + (double)CONCAT44(0x3f800000,fVar13);
              }
              if (in_FPSCR_SZ == '\0') {
                dVar8 = (double)CONCAT44(pfVar6[0x35],*(undefined4 *)(param_2 + 0x24));
              }
              else {
                in_xd0 = *(undefined8 *)(param_2 + 0x24);
                dVar8 = *(double *)(pfVar6 + 0x35);
              }
              fVar13 = (float)((ulonglong)dVar9 >> 0x20);
              if (in_FPSCR_PR == '\0') {
                dVar8 = (double)CONCAT44((int)((ulonglong)dVar8 >> 0x20),SUB84(dVar8,0) + fVar13);
              }
              else {
                dVar8 = dVar8 + dVar9;
              }
              if (in_FPSCR_PR == '\0' && (float)((ulonglong)dVar8 >> 0x20) < SUB84(dVar8,0)) {
                if (in_FPSCR_SZ == '\0') {
                  *(float *)(param_2 + 0x34) = pfVar6[0x35];
                  dVar14 = (double)CONCAT44(uVar17,pfVar6[0x35]);
                  dVar9 = (double)CONCAT44(fVar13,pfVar6[0x37]);
                  pfVar6[0x12] = pfVar6[0x37];
                }
                else {
                  *(undefined8 *)(param_2 + 0x34) = *(undefined8 *)(pfVar6 + 0x35);
                  in_xd2 = *(undefined8 *)(pfVar6 + 0x37);
                  *(undefined8 *)(pfVar6 + 0x12) = in_xd2;
                }
                *(float *)(param_1 + 0x40) = pfVar6[0x33];
                *(float *)(param_1 + 0x44) = pfVar6[0x34];
                *(ushort *)(param_2 + 6) = *(ushort *)(param_2 + 6) | 0x10;
                *(ushort *)(param_2 + 6) = *(ushort *)(param_2 + 6) | 8;
                if (in_FPSCR_SZ == '\0') {
                  *(int *)(param_1 + 0xc) = SUB84(dVar18,0);
                  dVar9 = (double)CONCAT44((int)((ulonglong)dVar9 >> 0x20),pfVar6[0x35]);
                }
                else {
                  *(undefined8 *)(param_1 + 0xc) = in_xd14;
                  in_xd2 = *(undefined8 *)(pfVar6 + 0x35);
                }
                if (in_FPSCR_PR == '\0') {
                  dVar9 = (double)CONCAT44((int)((ulonglong)dVar9 >> 0x20),SUB84(dVar9,0) + fVar11);
                }
                else {
                  dVar9 = dVar9 + dVar18;
                }
                if (in_FPSCR_SZ == '\0') {
                  *(int *)(param_2 + 0x24) = SUB84(dVar9,0);
                }
                else {
                  *(undefined8 *)(param_2 + 0x24) = in_xd2;
                }
                pfVar6[0xf] = 1.4013e-45;
              }
            }
          }
        }
        else {
          if (in_FPSCR_SZ == '\0') {
            dVar9 = (double)((ulonglong)(uint)pfVar6[0x2f] << 0x20);
          }
          else {
            dVar9 = *(double *)(pfVar6 + 0x2f);
          }
          fVar13 = (float)((ulonglong)dVar9 >> 0x20);
          if (in_FPSCR_PR != '\0' || fVar13 <= SUB84(dVar9,0)) goto LAB_8c017000;
          if (in_FPSCR_SZ == '\0') {
            dVar9 = (double)CONCAT44(fVar13,pfVar6[0x2d]);
          }
          else {
            in_xd2 = *(undefined8 *)(pfVar6 + 0x2d);
          }
          if (in_FPSCR_PR == '\0') {
            bVar20 = SUB84(dVar14,0) < SUB84(dVar9,0);
          }
          else {
            bVar20 = dVar14 < dVar9;
          }
          if (bVar20) {
            if (in_FPSCR_SZ == '\0') {
              dVar14 = (double)CONCAT44(uVar17,pfVar6[0x2d]);
              pfVar6[0x12] = pfVar6[0x2f];
            }
            else {
              in_xd2 = *(undefined8 *)(pfVar6 + 0x2f);
              *(undefined8 *)(pfVar6 + 0x12) = in_xd2;
            }
          }
          if (in_FPSCR_SZ == '\0') {
            *(float *)(param_2 + 0x34) = pfVar6[0x2d];
          }
          else {
            in_xd2 = *(undefined8 *)(pfVar6 + 0x2d);
            *(undefined8 *)(param_2 + 0x34) = in_xd2;
          }
          *(float *)(param_1 + 0x40) = pfVar6[0x2b];
          *(float *)(param_1 + 0x44) = pfVar6[0x2c];
          *(ushort *)(param_2 + 6) = *(ushort *)(param_2 + 6) | 0x10;
          if (in_FPSCR_SZ == '\0') {
            dVar9 = (double)CONCAT44(*(undefined4 *)(param_2 + 0x24),*(undefined4 *)(param_1 + 0xc))
            ;
          }
          else {
            in_xd2 = *(undefined8 *)(param_1 + 0xc);
            dVar9 = *(double *)(param_2 + 0x24);
          }
          if (in_FPSCR_PR == '\0') {
            dVar9 = (double)CONCAT44((float)((ulonglong)dVar9 >> 0x20) + SUB84(dVar9,0),
                                     SUB84(dVar9,0));
          }
          else {
            dVar9 = dVar9 + dVar9;
          }
          if (in_FPSCR_SZ == '\0') {
            dVar9 = (double)CONCAT44((int)((ulonglong)dVar9 >> 0x20),DAT_8c016fe0);
            dVar8 = (double)CONCAT44((int)((ulonglong)dVar8 >> 0x20),pfVar6[0x2d]);
          }
          else {
            in_xd2 = CONCAT44(_DAT_8c016fe4,DAT_8c016fe0);
            in_xd0 = *(undefined8 *)(pfVar6 + 0x2d);
          }
          if (in_FPSCR_PR == '\0') {
            fVar13 = (float)((ulonglong)dVar9 >> 0x20) + SUB84(dVar9,0);
            dVar9 = (double)CONCAT44(fVar13,SUB84(dVar9,0));
            bVar20 = fVar13 < SUB84(dVar8,0);
          }
          else {
            dVar9 = dVar9 + dVar9;
            bVar20 = dVar9 < dVar8;
          }
          if (bVar20) {
            if (in_FPSCR_SZ == '\0') {
              *(int *)(param_1 + 0xc) = SUB84(dVar18,0);
              dVar9 = (double)CONCAT44(pfVar6[0x2d],SUB84(dVar9,0));
            }
            else {
              *(undefined8 *)(param_1 + 0xc) = in_xd14;
              dVar9 = *(double *)(pfVar6 + 0x2d);
            }
            if (in_FPSCR_PR == '\0') {
              dVar9 = (double)CONCAT44((float)((ulonglong)dVar9 >> 0x20) + fVar11,SUB84(dVar9,0));
            }
            else {
              dVar9 = dVar9 + dVar18;
            }
            if (in_FPSCR_SZ == '\0') {
              *(int *)(param_2 + 0x24) = (int)((ulonglong)dVar9 >> 0x20);
            }
            else {
              *(double *)(param_2 + 0x24) = dVar9;
            }
            *(ushort *)(param_2 + 6) = *(ushort *)(param_2 + 6) | 8;
            pfVar6[0xf] = 1.4013e-45;
          }
        }
        if (in_FPSCR_SZ == '\0') {
          pfVar6[0xe] = pfVar6[0x3f];
          dVar9 = (double)CONCAT44((int)((ulonglong)dVar9 >> 0x20),pfVar6[0x40]);
          pfVar6[3] = pfVar6[0x40];
        }
        else {
          *(undefined8 *)(pfVar6 + 0xe) = *(undefined8 *)(pfVar6 + 0x3f);
          in_xd2 = *(undefined8 *)(pfVar6 + 0x40);
          *(undefined8 *)(pfVar6 + 3) = in_xd2;
        }
        if (pfVar6[0x39] != 0.0) {
          dVar8 = (double)(*(code *)PTR_FUN_8c017280)(0x11c7);
          if (in_FPSCR_SZ == '\0') {
            dVar9 = (double)CONCAT44((int)((ulonglong)dVar9 >> 0x20),pfVar6[0xe]);
          }
          else {
            in_xd2 = *(undefined8 *)(pfVar6 + 0xe);
          }
          fVar11 = (float)((ulonglong)dVar8 >> 0x20);
          if (in_FPSCR_PR == '\0') {
            bVar20 = SUB84(dVar9,0) < fVar11;
          }
          else {
            bVar20 = dVar9 < dVar8;
          }
          if (bVar20) {
            if (in_FPSCR_SZ == '\0') {
              lVar10 = (ulonglong)(uint)pfVar6[3] << 0x20;
            }
            else {
              lVar10 = *(longlong *)(pfVar6 + 3);
            }
            if (in_FPSCR_PR == '\0' && (float)((ulonglong)lVar10 >> 0x20) < (float)lVar10) {
LAB_8c0170bc:
              if (in_FPSCR_SZ == '\0') {
                dVar9 = (double)((ulonglong)(uint)pfVar6[3] << 0x20);
              }
              else {
                dVar9 = *(double *)(pfVar6 + 3);
              }
              if (in_FPSCR_PR == '\0' && (float)((ulonglong)dVar9 >> 0x20) < SUB84(dVar9,0)) {
                if (in_FPSCR_SZ == '\0') {
                  dVar9 = (double)CONCAT44(*(undefined4 *)(param_1 + 0x10),SUB84(dVar9,0));
                  dVar8 = (double)CONCAT44((int)((ulonglong)dVar8 >> 0x20),pfVar6[6]);
                }
                else {
                  dVar9 = *(double *)(param_1 + 0x10);
                  in_xd0 = *(undefined8 *)(pfVar6 + 6);
                }
                if (in_FPSCR_PR == '\0') {
                  dVar8 = (double)CONCAT44((int)((ulonglong)dVar8 >> 0x20),
                                           SUB84(dVar8,0) + (float)((ulonglong)dVar9 >> 0x20));
                }
                else {
                  dVar8 = dVar8 + dVar9;
                }
                if (in_FPSCR_SZ == '\0') {
                  dVar9 = (double)CONCAT44(pfVar6[0x3d],SUB84(dVar9,0));
                }
                else {
                  dVar9 = *(double *)(pfVar6 + 0x3d);
                }
                if (in_FPSCR_PR == '\0') {
                  bVar20 = (float)((ulonglong)dVar9 >> 0x20) < SUB84(dVar8,0);
                }
                else {
                  bVar20 = dVar9 < dVar8;
                }
                if (bVar20) goto LAB_8c0170da;
              }
            }
            else {
              if (in_FPSCR_SZ == '\0') {
                dVar9 = (double)CONCAT44(*(undefined4 *)(param_1 + 0x10),(float)lVar10);
                dVar8 = (double)CONCAT44(fVar11,pfVar6[5]);
              }
              else {
                dVar9 = *(double *)(param_1 + 0x10);
                in_xd0 = *(undefined8 *)(pfVar6 + 5);
              }
              if (in_FPSCR_PR == '\0') {
                dVar8 = (double)CONCAT44((int)((ulonglong)dVar8 >> 0x20),
                                         SUB84(dVar8,0) + (float)((ulonglong)dVar9 >> 0x20));
              }
              else {
                dVar8 = dVar8 + dVar9;
              }
              if (in_FPSCR_SZ == '\0') {
                dVar9 = (double)CONCAT44(pfVar6[0x3d],SUB84(dVar9,0));
              }
              else {
                dVar9 = *(double *)(pfVar6 + 0x3d);
              }
              if (in_FPSCR_PR == '\0') {
                bVar20 = SUB84(dVar8,0) < (float)((ulonglong)dVar9 >> 0x20);
              }
              else {
                bVar20 = dVar8 < dVar9;
              }
              if (!bVar20) goto LAB_8c0170bc;
LAB_8c0170da:
              if (in_FPSCR_SZ == '\0') {
                lVar10 = (ulonglong)*(uint *)(param_1 + 0x10) << 0x20;
              }
              else {
                lVar10 = *(longlong *)(param_1 + 0x10);
              }
              if (in_FPSCR_PR == '\0' && (float)((ulonglong)lVar10 >> 0x20) < (float)lVar10) {
                if (in_FPSCR_SZ == '\0') {
                  dVar9 = (double)CONCAT44(*(undefined4 *)(param_1 + 0x10),(float)lVar10);
                }
                else {
                  dVar9 = *(double *)(param_1 + 0x10);
                }
                if (in_FPSCR_PR == '\0') {
                  dVar9 = (double)CONCAT44(-((float)((ulonglong)dVar9 >> 0x20) *
                                            (float)((ulonglong)dVar14 >> 0x20)),SUB84(dVar9,0));
                }
                else {
                  dVar9 = -(dVar9 * dVar14);
                }
                if (in_FPSCR_SZ == '\0') {
                  *(int *)(param_1 + 0x10) = (int)((ulonglong)dVar9 >> 0x20);
                }
                else {
                  *(double *)(param_1 + 0x10) = dVar9;
                }
              }
              if (in_FPSCR_SZ == '\0') {
                dVar9 = (double)CONCAT44(pfVar6[0x13],pfVar6[0x3e]);
              }
              else {
                in_xd2 = *(undefined8 *)(pfVar6 + 0x3e);
                dVar9 = *(double *)(pfVar6 + 0x13);
              }
              if (in_FPSCR_PR == '\0') {
                dVar9 = (double)CONCAT44((float)((ulonglong)dVar9 >> 0x20) + SUB84(dVar9,0),
                                         SUB84(dVar9,0));
              }
              else {
                dVar9 = dVar9 + dVar9;
              }
              if (in_FPSCR_SZ == '\0') {
                pfVar6[0x13] = (float)((ulonglong)dVar9 >> 0x20);
                dVar9 = (double)CONCAT44(pfVar6[0x14],pfVar6[0x3f]);
              }
              else {
                *(double *)(pfVar6 + 0x13) = dVar9;
                in_xd2 = *(undefined8 *)(pfVar6 + 0x3f);
                dVar9 = *(double *)(pfVar6 + 0x14);
              }
              if (in_FPSCR_PR == '\0') {
                dVar9 = (double)CONCAT44((float)((ulonglong)dVar9 >> 0x20) + SUB84(dVar9,0),
                                         SUB84(dVar9,0));
              }
              else {
                dVar9 = dVar9 + dVar9;
              }
              if (in_FPSCR_SZ == '\0') {
                pfVar6[0x14] = (float)((ulonglong)dVar9 >> 0x20);
                dVar9 = (double)CONCAT44(pfVar6[0x15],pfVar6[0x40]);
              }
              else {
                *(double *)(pfVar6 + 0x14) = dVar9;
                in_xd2 = *(undefined8 *)(pfVar6 + 0x40);
                dVar9 = *(double *)(pfVar6 + 0x15);
              }
              if (in_FPSCR_PR == '\0') {
                dVar9 = (double)CONCAT44((float)((ulonglong)dVar9 >> 0x20) + SUB84(dVar9,0),
                                         SUB84(dVar9,0));
              }
              else {
                dVar9 = dVar9 + dVar9;
              }
              if (in_FPSCR_SZ == '\0') {
                pfVar6[0x15] = (float)((ulonglong)dVar9 >> 0x20);
              }
              else {
                *(double *)(pfVar6 + 0x15) = dVar9;
              }
              *(char *)(param_1 + 2) = *(char *)(param_1 + 2) + '\x01';
            }
          }
        }
        if (in_FPSCR_SZ == '\0') {
          pfVar6[10] = pfVar6[0x47];
          dVar9 = (double)CONCAT44((int)((ulonglong)dVar9 >> 0x20),pfVar6[0x48]);
          pfVar6[2] = pfVar6[0x48];
        }
        else {
          *(undefined8 *)(pfVar6 + 10) = *(undefined8 *)(pfVar6 + 0x47);
          in_xd2 = *(undefined8 *)(pfVar6 + 0x48);
          *(undefined8 *)(pfVar6 + 2) = in_xd2;
        }
        if (pfVar6[0x41] != 0.0) {
          dVar8 = (double)(*(code *)PTR_FUN_8c017280)(0x11c7);
          if (in_FPSCR_SZ == '\0') {
            dVar9 = (double)CONCAT44((int)((ulonglong)dVar9 >> 0x20),pfVar6[10]);
          }
          else {
            in_xd2 = *(undefined8 *)(pfVar6 + 10);
          }
          fVar11 = (float)((ulonglong)dVar8 >> 0x20);
          if (in_FPSCR_PR == '\0') {
            bVar20 = SUB84(dVar9,0) < fVar11;
          }
          else {
            bVar20 = dVar9 < dVar8;
          }
          if (bVar20) {
            if (in_FPSCR_SZ == '\0') {
              lVar10 = (ulonglong)(uint)pfVar6[2] << 0x20;
            }
            else {
              lVar10 = *(longlong *)(pfVar6 + 2);
            }
            if (in_FPSCR_PR == '\0' && (float)lVar10 < (float)((ulonglong)lVar10 >> 0x20)) {
LAB_8c01717c:
              if (in_FPSCR_SZ == '\0') {
                lVar10 = (ulonglong)(uint)pfVar6[2] << 0x20;
              }
              else {
                lVar10 = *(longlong *)(pfVar6 + 2);
              }
              if (in_FPSCR_PR == '\0' && (float)lVar10 < (float)((ulonglong)lVar10 >> 0x20)) {
                if (in_FPSCR_SZ == '\0') {
                  dVar9 = (double)CONCAT44(*(undefined4 *)(param_1 + 0x10),(float)lVar10);
                  dVar8 = (double)CONCAT44((int)((ulonglong)dVar8 >> 0x20),pfVar6[5]);
                }
                else {
                  dVar9 = *(double *)(param_1 + 0x10);
                  in_xd0 = *(undefined8 *)(pfVar6 + 5);
                }
                if (in_FPSCR_PR == '\0') {
                  dVar8 = (double)CONCAT44((int)((ulonglong)dVar8 >> 0x20),
                                           SUB84(dVar8,0) + (float)((ulonglong)dVar9 >> 0x20));
                }
                else {
                  dVar8 = dVar8 + dVar9;
                }
                if (in_FPSCR_SZ == '\0') {
                  dVar9 = (double)CONCAT44(pfVar6[0x45],SUB84(dVar9,0));
                }
                else {
                  dVar9 = *(double *)(pfVar6 + 0x45);
                }
                if (in_FPSCR_PR == '\0') {
                  bVar20 = SUB84(dVar8,0) < (float)((ulonglong)dVar9 >> 0x20);
                }
                else {
                  bVar20 = dVar8 < dVar9;
                }
                if (bVar20) goto LAB_8c01719a;
              }
            }
            else {
              if (in_FPSCR_SZ == '\0') {
                dVar9 = (double)CONCAT44(*(undefined4 *)(param_1 + 0x10),(float)lVar10);
                dVar8 = (double)CONCAT44(fVar11,pfVar6[6]);
              }
              else {
                dVar9 = *(double *)(param_1 + 0x10);
                in_xd0 = *(undefined8 *)(pfVar6 + 6);
              }
              if (in_FPSCR_PR == '\0') {
                dVar8 = (double)CONCAT44((int)((ulonglong)dVar8 >> 0x20),
                                         SUB84(dVar8,0) + (float)((ulonglong)dVar9 >> 0x20));
              }
              else {
                dVar8 = dVar8 + dVar9;
              }
              if (in_FPSCR_SZ == '\0') {
                dVar9 = (double)CONCAT44(pfVar6[0x45],SUB84(dVar9,0));
              }
              else {
                dVar9 = *(double *)(pfVar6 + 0x45);
              }
              if (in_FPSCR_PR == '\0') {
                bVar20 = (float)((ulonglong)dVar9 >> 0x20) < SUB84(dVar8,0);
              }
              else {
                bVar20 = dVar9 < dVar8;
              }
              if (!bVar20) goto LAB_8c01717c;
LAB_8c01719a:
              if (in_FPSCR_SZ == '\0') {
                lVar10 = (ulonglong)*(uint *)(param_1 + 0x10) << 0x20;
              }
              else {
                lVar10 = *(longlong *)(param_1 + 0x10);
              }
              if (in_FPSCR_PR == '\0' && (float)lVar10 < (float)((ulonglong)lVar10 >> 0x20)) {
                if (in_FPSCR_SZ == '\0') {
                  dVar9 = (double)CONCAT44(*(undefined4 *)(param_1 + 0x10),(float)lVar10);
                }
                else {
                  dVar9 = *(double *)(param_1 + 0x10);
                }
                if (in_FPSCR_PR == '\0') {
                  dVar9 = (double)CONCAT44(-((float)((ulonglong)dVar9 >> 0x20) *
                                            (float)((ulonglong)dVar14 >> 0x20)),SUB84(dVar9,0));
                }
                else {
                  dVar9 = -(dVar9 * dVar14);
                }
                if (in_FPSCR_SZ == '\0') {
                  *(int *)(param_1 + 0x10) = (int)((ulonglong)dVar9 >> 0x20);
                }
                else {
                  *(double *)(param_1 + 0x10) = dVar9;
                }
              }
              if (in_FPSCR_SZ == '\0') {
                dVar9 = (double)CONCAT44(pfVar6[0x13],pfVar6[0x46]);
              }
              else {
                in_xd2 = *(undefined8 *)(pfVar6 + 0x46);
                dVar9 = *(double *)(pfVar6 + 0x13);
              }
              if (in_FPSCR_PR == '\0') {
                dVar9 = (double)CONCAT44((float)((ulonglong)dVar9 >> 0x20) + SUB84(dVar9,0),
                                         SUB84(dVar9,0));
              }
              else {
                dVar9 = dVar9 + dVar9;
              }
              if (in_FPSCR_SZ == '\0') {
                pfVar6[0x13] = (float)((ulonglong)dVar9 >> 0x20);
                dVar9 = (double)CONCAT44(pfVar6[0x14],pfVar6[0x47]);
              }
              else {
                *(double *)(pfVar6 + 0x13) = dVar9;
                in_xd2 = *(undefined8 *)(pfVar6 + 0x47);
                dVar9 = *(double *)(pfVar6 + 0x14);
              }
              if (in_FPSCR_PR == '\0') {
                dVar9 = (double)CONCAT44((float)((ulonglong)dVar9 >> 0x20) + SUB84(dVar9,0),
                                         SUB84(dVar9,0));
              }
              else {
                dVar9 = dVar9 + dVar9;
              }
              if (in_FPSCR_SZ == '\0') {
                pfVar6[0x14] = (float)((ulonglong)dVar9 >> 0x20);
                dVar9 = (double)CONCAT44(pfVar6[0x15],pfVar6[0x48]);
              }
              else {
                *(double *)(pfVar6 + 0x14) = dVar9;
                in_xd2 = *(undefined8 *)(pfVar6 + 0x48);
                dVar9 = *(double *)(pfVar6 + 0x15);
              }
              if (in_FPSCR_PR == '\0') {
                dVar9 = (double)CONCAT44((float)((ulonglong)dVar9 >> 0x20) + SUB84(dVar9,0),
                                         SUB84(dVar9,0));
              }
              else {
                dVar9 = dVar9 + dVar9;
              }
              if (in_FPSCR_SZ == '\0') {
                pfVar6[0x15] = (float)((ulonglong)dVar9 >> 0x20);
              }
              else {
                *(double *)(pfVar6 + 0x15) = dVar9;
              }
              *(char *)(param_1 + 2) = *(char *)(param_1 + 2) + '\x01';
            }
          }
        }
        if ((*(ushort *)(param_2 + 6) & 0x40) != 0) {
          FUN_8c01798a(param_1 + 0x20,
                       ((uint)pfVar6[0xf] & 1) << 1 | ((uint)pfVar6[0xf] & 1) << 2 |
                       (uint)pfVar6[0x10] & 1);
        }
        puVar1 = PTR_FUN_8c017284;
        if (in_FPSCR_SZ == '\0') {
          dVar9 = (double)CONCAT44(*(undefined4 *)(param_1 + 0x14),pfVar6[0x13]);
        }
        else {
          in_xd2 = *(undefined8 *)(pfVar6 + 0x13);
          dVar9 = *(double *)(param_1 + 0x14);
        }
        if (in_FPSCR_PR == '\0') {
          dVar9 = (double)CONCAT44((float)((ulonglong)dVar9 >> 0x20) + SUB84(dVar9,0),SUB84(dVar9,0)
                                  );
        }
        else {
          dVar9 = dVar9 + dVar9;
        }
        if (in_FPSCR_SZ == '\0') {
          *(int *)(param_1 + 0x14) = (int)((ulonglong)dVar9 >> 0x20);
          dVar9 = (double)CONCAT44(*(undefined4 *)(param_1 + 0x18),pfVar6[0x14]);
        }
        else {
          *(double *)(param_1 + 0x14) = dVar9;
          dVar9 = *(double *)(param_1 + 0x18);
          in_xd2 = *(undefined8 *)(pfVar6 + 0x14);
        }
        if (in_FPSCR_PR == '\0') {
          dVar9 = (double)CONCAT44((float)((ulonglong)dVar9 >> 0x20) + SUB84(dVar9,0),SUB84(dVar9,0)
                                  );
        }
        else {
          dVar9 = dVar9 + dVar9;
        }
        if (in_FPSCR_SZ == '\0') {
          *(int *)(param_1 + 0x18) = (int)((ulonglong)dVar9 >> 0x20);
          dVar9 = (double)CONCAT44(*(undefined4 *)(param_1 + 0x1c),pfVar6[0x15]);
        }
        else {
          *(double *)(param_1 + 0x18) = dVar9;
          dVar9 = *(double *)(param_1 + 0x1c);
          in_xd2 = *(undefined8 *)(pfVar6 + 0x15);
        }
        if (in_FPSCR_PR == '\0') {
          dVar9 = (double)CONCAT44((float)((ulonglong)dVar9 >> 0x20) + SUB84(dVar9,0),SUB84(dVar9,0)
                                  );
        }
        else {
          dVar9 = dVar9 + dVar9;
        }
        if (in_FPSCR_SZ == '\0') {
          *(int *)(param_1 + 0x1c) = (int)((ulonglong)dVar9 >> 0x20);
        }
        else {
          *(double *)(param_1 + 0x1c) = dVar9;
        }
        if (in_FPSCR_SZ == '\0') {
          pfVar7 = pfVar6 + -1;
          pfVar6[-1] = (float)*(byte *)(param_1 + 2);
        }
        else {
          pfVar7 = pfVar6 + -2;
          *(undefined8 *)(pfVar6 + -2) = in_xd2;
        }
        iVar3 = (*(code *)puVar1)();
        if (in_FPSCR_SZ == '\0') {
          dVar8 = (double)CONCAT44(DAT_8c017288,extraout_fr1);
          fVar11 = SUB84(dVar18,0);
        }
        else {
          dVar8 = (double)CONCAT44(_DAT_8c01728c,DAT_8c017288);
          fVar11 = (float)((ulonglong)in_xd14 >> 0x20);
        }
        fVar13 = (float)iVar3;
        if (in_FPSCR_PR == '\0') {
          fVar11 = (float)((ulonglong)dVar8 >> 0x20) * fVar13 + fVar11;
        }
        if (in_FPSCR_SZ == '\0') {
          fVar13 = *pfVar7;
          pfVar6 = pfVar7 + 1;
        }
        else {
          in_xd2 = *(undefined8 *)pfVar7;
          pfVar6 = pfVar7 + 2;
        }
        dVar9 = (double)CONCAT44(fVar11,fVar13);
        if (in_FPSCR_PR == '\0' && fVar11 < fVar13) {
          *(undefined1 *)(param_1 + 2) = 0;
          if ((*(ushort *)(param_2 + 6) & 0x20) == 0) {
            uVar2 = *(ushort *)(param_2 + 6) | 0x20;
          }
          else {
            uVar2 = *(ushort *)(param_2 + 6) & (ushort)_DAT_8c01728c;
          }
          *(ushort *)(param_2 + 6) = uVar2;
        }
      }
    }
    puVar4 = puVar4 + 3;
    *(int *)((int)pfVar6 + 0x10) = *(int *)((int)pfVar6 + 0x10) + 1;
    *(int *)((int)pfVar6 + 0x20) = *(int *)((int)pfVar6 + 0x20) + 0xc;
  }
  if (*(undefined **)(param_1 + 0x60) == PTR_FUN_8c017404) {
    return;
  }
  if ((*(ushort *)(param_2 + 6) & 0x40) != 0) {
    return;
  }
  if ((pfVar6[0x11] == 0.0) && ((*(ushort *)(param_2 + 6) & 4) == 0)) {
    return;
  }
  if (in_FPSCR_SZ == '\0') {
    dVar9 = (double)CONCAT44((int)((ulonglong)dVar9 >> 0x20),DAT_8c017408);
  }
  fVar11 = SUB84(dVar14,0);
  if (in_FPSCR_PR == '\0') {
    bVar20 = SUB84(dVar9,0) < fVar11;
  }
  else {
    bVar20 = dVar9 < dVar14;
  }
  if (bVar20) {
    if (in_FPSCR_SZ == '\0') {
      dVar9 = (double)CONCAT44(*(undefined4 *)(param_1 + 0x58),SUB84(dVar9,0));
      dVar8 = (double)CONCAT44((int)((ulonglong)dVar8 >> 0x20),DAT_8c01740c);
    }
    else {
      dVar9 = *(double *)(param_1 + 0x58);
    }
    if (in_FPSCR_PR == '\0') {
      fVar13 = (float)((ulonglong)dVar9 >> 0x20) - fVar11;
      dVar9 = (double)CONCAT44(fVar13,SUB84(dVar9,0));
      bVar20 = SUB84(dVar8,0) < fVar13;
    }
    else {
      dVar9 = dVar9 - dVar14;
      bVar20 = dVar8 < dVar9;
    }
    if (bVar20) goto LAB_8c017316;
  }
  if (in_FPSCR_SZ == '\0') {
    dVar9 = (double)CONCAT44((int)((ulonglong)dVar9 >> 0x20),DAT_8c017408);
  }
  if (fVar11 != SUB84(dVar9,0)) {
    dVar8 = (double)(*(code *)PTR_FUN_8c017410)(0x238e);
    if (in_FPSCR_SZ == '\0') {
      dVar9 = (double)CONCAT44((int)((ulonglong)dVar9 >> 0x20),pfVar6[0x12]);
    }
    if (in_FPSCR_PR == '\0') {
      bVar20 = SUB84(dVar9,0) < (float)((ulonglong)dVar8 >> 0x20);
    }
    else {
      bVar20 = dVar9 < dVar8;
    }
    if (!bVar20) {
      return;
    }
  }
LAB_8c017316:
  *(char *)(param_1 + 2) = *(char *)(param_1 + 2) + '\x01';
  if (in_FPSCR_SZ == '\0') {
    *(undefined4 *)(param_2 + 0x20) = *(undefined4 *)(param_1 + 0x54);
    *(undefined4 *)(param_2 + 0x28) = *(undefined4 *)(param_1 + 0x5c);
  }
  else {
    *(undefined8 *)(param_2 + 0x20) = *(undefined8 *)(param_1 + 0x54);
    *(undefined8 *)(param_2 + 0x28) = *(undefined8 *)(param_1 + 0x5c);
  }
  *(ushort *)(param_2 + 6) = *(ushort *)(param_2 + 6) | 0x200;
  *(ushort *)(param_2 + 6) = *(ushort *)(param_2 + 6) | 0x800;
  return;
}


/* Address: 8c017360 */

/* WARNING: Removing unreachable block (ram,0x8c01739a) */
/* WARNING: Removing unreachable block (ram,0x8c0173b6) */
/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_8c017360(undefined4 param_1,int param_2,char *param_3,int param_4)

{
  char cVar1;
  ushort uVar2;
  undefined *puVar3;
  undefined8 *puVar4;
  int iVar5;
  float fVar6;
  double in_dr0;
  undefined4 uVar9;
  undefined4 uVar10;
  double dVar7;
  longlong lVar8;
  float fVar11;
  double dVar12;
  float fVar14;
  double dVar13;
  undefined8 in_xd0;
  undefined8 in_xd2;
  bool bVar15;
  char in_FPSCR_PR;
  char in_FPSCR_SZ;
  
  *(short *)(param_3 + 0xc) = *(short *)(param_3 + 0xc) + 1;
  if (0x3fff < *(short *)(param_3 + 0xc)) {
    param_3[0xc] = -1;
    param_3[0xd] = '?';
  }
  if (*(int *)(param_3 + 8) != 0) {
    puVar4 = (undefined8 *)(*(int *)(param_2 + 0x68) + 0xc);
    if (in_FPSCR_SZ == '\0') {
      dVar12 = (double)CONCAT44(*(undefined4 *)(param_2 + 4),*(undefined4 *)puVar4);
    }
    else {
      dVar12 = *(double *)(param_2 + 4);
      in_xd2 = *puVar4;
    }
    if (in_FPSCR_PR == '\0') {
      dVar12 = (double)CONCAT44((float)((ulonglong)dVar12 >> 0x20) + SUB84(dVar12,0),SUB84(dVar12,0)
                               );
    }
    else {
      dVar12 = dVar12 + dVar12;
    }
    uVar10 = (undefined4)((ulonglong)dVar12 >> 0x20);
    if (in_FPSCR_SZ == '\0') {
      *(undefined4 *)(param_2 + 4) = uVar10;
    }
    else {
      *(double *)(param_2 + 4) = dVar12;
    }
    iVar5 = *(int *)(*(int *)(*(int *)(param_3 + 8) + 4) + 4);
    fVar11 = (float)iVar5;
    dVar12 = (double)CONCAT44(uVar10,fVar11);
    if (iVar5 < 0) {
      if (in_FPSCR_SZ == '\0') {
        dVar12 = (double)CONCAT44(DAT_8c017414,fVar11);
      }
      else {
        dVar12 = (double)CONCAT44(uRam8c017418,DAT_8c017414);
      }
      if (in_FPSCR_PR == '\0') {
        fVar11 = (float)((ulonglong)dVar12 >> 0x20);
        dVar12 = (double)CONCAT44(fVar11,SUB84(dVar12,0) + fVar11);
      }
      else {
        dVar12 = dVar12 + dVar12;
      }
    }
    if (in_FPSCR_SZ == '\0') {
      in_dr0 = (double)CONCAT44((int)((ulonglong)in_dr0 >> 0x20),*(undefined4 *)(param_2 + 4));
    }
    else {
      in_xd0 = *(undefined8 *)(param_2 + 4);
    }
    if (in_FPSCR_PR == '\0') {
      bVar15 = SUB84(dVar12,0) < SUB84(in_dr0,0);
    }
    else {
      bVar15 = dVar12 < in_dr0;
    }
    if (bVar15) {
      iVar5 = *(int *)(*(int *)(*(int *)(param_3 + 8) + 4) + 4);
      fVar11 = (float)iVar5;
      dVar12 = (double)CONCAT44((int)((ulonglong)dVar12 >> 0x20),fVar11);
      if (iVar5 < 0) {
        if (in_FPSCR_PR == '\0') {
          dVar12 = (double)CONCAT44(DAT_8c017414,fVar11 + DAT_8c017414);
        }
        else {
          dVar12 = (double)CONCAT44(DAT_8c017414,fVar11) + (double)CONCAT44(DAT_8c017414,fVar11);
        }
      }
      if (in_FPSCR_SZ == '\0') {
        in_dr0 = (double)CONCAT44((int)((ulonglong)in_dr0 >> 0x20),*(undefined4 *)(param_2 + 4));
      }
      else {
        in_xd0 = *(undefined8 *)(param_2 + 4);
      }
      if (in_FPSCR_PR == '\0') {
        in_dr0 = (double)CONCAT44((int)((ulonglong)in_dr0 >> 0x20),SUB84(in_dr0,0) - SUB84(dVar12,0)
                                 );
      }
      else {
        in_dr0 = in_dr0 - dVar12;
      }
      if (in_FPSCR_SZ == '\0') {
        *(int *)(param_2 + 4) = SUB84(in_dr0,0);
      }
      else {
        *(undefined8 *)(param_2 + 4) = in_xd0;
      }
    }
  }
  cVar1 = *param_3;
  uVar10 = (undefined4)((ulonglong)in_dr0 >> 0x20);
  if (cVar1 == '\0') {
    iVar5 = *(int *)(param_4 + 0x24);
    if (in_FPSCR_SZ == '\0') {
      dVar12 = (double)CONCAT44(*(undefined4 *)(iVar5 + 8),DAT_8c0175e0);
    }
    else {
      dVar12 = *(double *)(iVar5 + 8);
    }
    if (in_FPSCR_PR == '\0') {
      dVar12 = (double)CONCAT44((float)((ulonglong)dVar12 >> 0x20) - SUB84(dVar12,0),SUB84(dVar12,0)
                               );
    }
    else {
      dVar12 = dVar12 - dVar12;
    }
    if (in_FPSCR_SZ == '\0') {
      *(int *)(iVar5 + 8) = (int)((ulonglong)dVar12 >> 0x20);
      dVar12 = (double)CONCAT44(*(undefined4 *)(iVar5 + 4),SUB84(dVar12,0));
      in_dr0 = (double)CONCAT44(uVar10,*(undefined4 *)(param_3 + 0x20));
    }
    else {
      *(double *)(iVar5 + 8) = dVar12;
      dVar12 = *(double *)(iVar5 + 4);
      in_xd0 = *(undefined8 *)(param_3 + 0x20);
    }
    if (in_FPSCR_PR == '\0') {
      dVar7 = (double)CONCAT44((int)((ulonglong)in_dr0 >> 0x20),
                               SUB84(in_dr0,0) + (float)((ulonglong)dVar12 >> 0x20));
    }
    else {
      dVar7 = in_dr0 + dVar12;
    }
    if (in_FPSCR_SZ == '\0') {
      *(int *)(param_3 + 0x20) = SUB84(dVar7,0);
      dVar12 = (double)CONCAT44(*(undefined4 *)(iVar5 + 8),SUB84(dVar12,0));
      dVar7 = (double)CONCAT44((int)((ulonglong)dVar7 >> 0x20),*(undefined4 *)(param_3 + 0x24));
    }
    else {
      *(undefined8 *)(param_3 + 0x20) = in_xd0;
      dVar12 = *(double *)(iVar5 + 8);
      in_xd0 = *(undefined8 *)(param_3 + 0x24);
    }
    if (in_FPSCR_PR == '\0') {
      dVar7 = (double)CONCAT44((int)((ulonglong)dVar7 >> 0x20),
                               SUB84(dVar7,0) + (float)((ulonglong)dVar12 >> 0x20));
    }
    else {
      dVar7 = dVar7 + dVar12;
    }
    if (in_FPSCR_SZ == '\0') {
      *(int *)(param_3 + 0x24) = SUB84(dVar7,0);
      dVar7 = (double)CONCAT44((int)((ulonglong)dVar7 >> 0x20),*(undefined4 *)(param_3 + 0x28));
      dVar12 = (double)CONCAT44(*(undefined4 *)(iVar5 + 0xc),SUB84(dVar12,0));
    }
    else {
      *(undefined8 *)(param_3 + 0x24) = in_xd0;
      in_xd0 = *(undefined8 *)(param_3 + 0x28);
      dVar12 = *(double *)(iVar5 + 0xc);
    }
    puVar3 = PTR_FUN_8c0175e8;
    if (in_FPSCR_PR == '\0') {
      fVar11 = SUB84(dVar7,0) + (float)((ulonglong)dVar12 >> 0x20);
    }
    else {
      fVar11 = SUB84(dVar7 + dVar12,0);
    }
    if (in_FPSCR_SZ == '\0') {
      *(float *)(param_3 + 0x28) = fVar11;
    }
    else {
      *(undefined8 *)(param_3 + 0x28) = in_xd0;
    }
    param_3[0x14] = '\0';
    param_3[0x15] = '\0';
    param_3[0x16] = '\0';
    param_3[0x17] = '\0';
    param_3[0x1c] = '\0';
    param_3[0x1d] = '\0';
    param_3[0x1e] = '\0';
    param_3[0x1f] = '\0';
    uVar10 = DAT_8c0175e4;
    if (in_FPSCR_SZ == '\0') {
      uVar10 = param_1;
    }
    dVar12 = (double)(*(code *)puVar3)(param_3);
    uVar9 = (undefined4)((ulonglong)dVar12 >> 0x20);
    if (in_FPSCR_SZ == '\0') {
      dVar7 = (double)CONCAT44(*(undefined4 *)(param_3 + 0x24),0x3f800000);
      dVar13 = (double)CONCAT44(uVar9,uVar10);
    }
    else {
      dVar7 = *(double *)(param_3 + 0x24);
      dVar13 = dVar12;
    }
    if (in_FPSCR_PR == '\0') {
      dVar7 = (double)CONCAT44((float)((ulonglong)dVar7 >> 0x20) - SUB84(dVar7,0),SUB84(dVar7,0));
    }
    else {
      dVar7 = dVar7 - dVar7;
    }
    if (in_FPSCR_SZ == '\0') {
      *(int *)(param_3 + 0x24) = (int)((ulonglong)dVar7 >> 0x20);
      dVar7 = (double)CONCAT44(DAT_8c0175ec,SUB84(dVar7,0));
      dVar12 = (double)CONCAT44(uVar9,*(undefined4 *)(param_3 + 0x24));
    }
    else {
      *(double *)(param_3 + 0x24) = dVar7;
      dVar7 = (double)CONCAT44(_DAT_8c0175f0,DAT_8c0175ec);
    }
    fVar11 = (float)((ulonglong)dVar7 >> 0x20);
    fVar14 = (float)((ulonglong)dVar13 >> 0x20);
    if (in_FPSCR_PR == '\0') {
      fVar6 = SUB84(dVar12,0) + fVar11;
      dVar12 = (double)CONCAT44((int)((ulonglong)dVar12 >> 0x20),fVar6);
      bVar15 = fVar6 < fVar14;
    }
    else {
      dVar12 = dVar12 + dVar7;
      bVar15 = dVar12 < dVar13;
    }
    if (bVar15) {
      uVar10 = (undefined4)((ulonglong)dVar12 >> 0x20);
      dVar12 = (double)CONCAT44(uVar10,0x3f800000);
      if (in_FPSCR_PR == '\0') {
        dVar12 = (double)CONCAT44(uVar10,0x40000000);
      }
      else {
        dVar12 = dVar12 + dVar12;
      }
      dVar7 = (double)((ulonglong)(uint)fVar11 << 0x20);
      if (in_FPSCR_PR == '\0') {
        dVar13 = (double)CONCAT44(fVar14 + SUB84(dVar12,0),SUB84(dVar13,0));
      }
      else {
        dVar13 = dVar13 + dVar12;
      }
      if (in_FPSCR_SZ == '\0') {
        *(int *)(param_3 + 0x24) = (int)((ulonglong)dVar13 >> 0x20);
        dVar12 = (double)CONCAT44(*(undefined4 *)(iVar5 + 8),SUB84(dVar12,0));
      }
      else {
        *(double *)(param_3 + 0x24) = dVar13;
        dVar12 = *(double *)(iVar5 + 8);
      }
      if (in_FPSCR_PR == '\0') {
        bVar15 = (float)((ulonglong)dVar12 >> 0x20) < 0.0;
      }
      else {
        bVar15 = dVar12 < dVar7;
      }
      if (bVar15) {
        if (in_FPSCR_SZ == '\0') {
          dVar12 = (double)CONCAT44(*(undefined4 *)(iVar5 + 8),SUB84(dVar12,0));
        }
        else {
          dVar12 = *(double *)(iVar5 + 8);
        }
        dVar7 = (double)((ulonglong)(uint)_DAT_8c0175f0 << 0x20);
        if (in_FPSCR_PR == '\0') {
          dVar12 = (double)CONCAT44((float)((ulonglong)dVar12 >> 0x20) * _DAT_8c0175f0,
                                    SUB84(dVar12,0));
        }
        else {
          dVar12 = dVar12 * dVar7;
        }
        if (in_FPSCR_SZ == '\0') {
          *(int *)(iVar5 + 8) = (int)((ulonglong)dVar12 >> 0x20);
        }
        else {
          *(double *)(iVar5 + 8) = dVar12;
        }
      }
      if (in_FPSCR_SZ == '\0') {
        dVar12 = (double)CONCAT44(DAT_8c0175f4,SUB84(dVar13,0));
        dVar7 = (double)CONCAT44((int)((ulonglong)dVar7 >> 0x20),*(undefined4 *)(iVar5 + 4));
      }
      else {
        dVar12 = (double)CONCAT44(PTR_FUN_8c0175f8,DAT_8c0175f4);
        in_xd2 = *(undefined8 *)(iVar5 + 4);
      }
      fVar11 = (float)((ulonglong)dVar12 >> 0x20);
      if (in_FPSCR_PR == '\0') {
        fVar14 = SUB84(dVar7,0) * fVar11;
      }
      else {
        fVar14 = SUB84(dVar7 * dVar12,0);
      }
      if (in_FPSCR_SZ == '\0') {
        *(float *)(iVar5 + 4) = fVar14;
        dVar7 = (double)CONCAT44(*(undefined4 *)(iVar5 + 0xc),fVar14);
      }
      else {
        *(undefined8 *)(iVar5 + 4) = in_xd2;
        dVar7 = *(double *)(iVar5 + 0xc);
      }
      if (in_FPSCR_PR == '\0') {
        dVar7 = (double)CONCAT44((float)((ulonglong)dVar7 >> 0x20) * fVar11,SUB84(dVar7,0));
      }
      else {
        dVar7 = dVar7 * dVar12;
      }
      if (in_FPSCR_SZ == '\0') {
        *(int *)(iVar5 + 0xc) = (int)((ulonglong)dVar7 >> 0x20);
      }
      else {
        *(double *)(iVar5 + 0xc) = dVar7;
      }
    }
    if ((*(ushort *)(param_3 + 4) & 0x1000) == 0) {
      (*(code *)PTR_FUN_8c0175f8)(param_3);
      return;
    }
    *param_3 = '\x02';
    param_3[6] = '\0';
    param_3[7] = '\0';
    return;
  }
  if (cVar1 == '\x01') {
    iVar5 = *(int *)(param_4 + 0x24);
    if (in_FPSCR_SZ == '\0') {
      dVar12 = (double)CONCAT44(*(undefined4 *)(iVar5 + 8),DAT_8c0175e0);
    }
    else {
      dVar12 = *(double *)(iVar5 + 8);
    }
    if (in_FPSCR_PR == '\0') {
      dVar12 = (double)CONCAT44((float)((ulonglong)dVar12 >> 0x20) - SUB84(dVar12,0),SUB84(dVar12,0)
                               );
    }
    else {
      dVar12 = dVar12 - dVar12;
    }
    if (in_FPSCR_SZ == '\0') {
      *(int *)(iVar5 + 8) = (int)((ulonglong)dVar12 >> 0x20);
      dVar12 = (double)CONCAT44(*(undefined4 *)(iVar5 + 4),SUB84(dVar12,0));
      in_dr0 = (double)CONCAT44(uVar10,*(undefined4 *)(param_3 + 0x20));
    }
    else {
      *(double *)(iVar5 + 8) = dVar12;
      dVar12 = *(double *)(iVar5 + 4);
      in_xd0 = *(undefined8 *)(param_3 + 0x20);
    }
    if (in_FPSCR_PR == '\0') {
      dVar7 = (double)CONCAT44((int)((ulonglong)in_dr0 >> 0x20),
                               SUB84(in_dr0,0) + (float)((ulonglong)dVar12 >> 0x20));
    }
    else {
      dVar7 = in_dr0 + dVar12;
    }
    if (in_FPSCR_SZ == '\0') {
      *(int *)(param_3 + 0x20) = SUB84(dVar7,0);
      dVar12 = (double)CONCAT44(*(undefined4 *)(iVar5 + 8),SUB84(dVar12,0));
      dVar7 = (double)CONCAT44((int)((ulonglong)dVar7 >> 0x20),*(undefined4 *)(param_3 + 0x24));
    }
    else {
      *(undefined8 *)(param_3 + 0x20) = in_xd0;
      dVar12 = *(double *)(iVar5 + 8);
      in_xd0 = *(undefined8 *)(param_3 + 0x24);
    }
    if (in_FPSCR_PR == '\0') {
      dVar7 = (double)CONCAT44((int)((ulonglong)dVar7 >> 0x20),
                               SUB84(dVar7,0) + (float)((ulonglong)dVar12 >> 0x20));
    }
    else {
      dVar7 = dVar7 + dVar12;
    }
    if (in_FPSCR_SZ == '\0') {
      *(int *)(param_3 + 0x24) = SUB84(dVar7,0);
      dVar7 = (double)CONCAT44((int)((ulonglong)dVar7 >> 0x20),*(undefined4 *)(param_3 + 0x28));
      dVar12 = (double)CONCAT44(*(undefined4 *)(iVar5 + 0xc),SUB84(dVar12,0));
    }
    else {
      *(undefined8 *)(param_3 + 0x24) = in_xd0;
      in_xd0 = *(undefined8 *)(param_3 + 0x28);
      dVar12 = *(double *)(iVar5 + 0xc);
    }
    puVar3 = PTR_FUN_8c0175e8;
    if (in_FPSCR_PR == '\0') {
      fVar11 = SUB84(dVar7,0) + (float)((ulonglong)dVar12 >> 0x20);
    }
    else {
      fVar11 = SUB84(dVar7 + dVar12,0);
    }
    if (in_FPSCR_SZ == '\0') {
      *(float *)(param_3 + 0x28) = fVar11;
    }
    else {
      *(undefined8 *)(param_3 + 0x28) = in_xd0;
    }
    param_3[0x14] = '\0';
    param_3[0x15] = '\0';
    param_3[0x16] = '\0';
    param_3[0x17] = '\0';
    param_3[0x1c] = '\0';
    param_3[0x1d] = '\0';
    param_3[0x1e] = '\0';
    param_3[0x1f] = '\0';
    uVar10 = DAT_8c0175e4;
    if (in_FPSCR_SZ == '\0') {
      uVar10 = param_1;
    }
    dVar12 = (double)(*(code *)puVar3)(param_3);
    if (in_FPSCR_SZ == '\0') {
      dVar12 = (double)CONCAT44((int)((ulonglong)dVar12 >> 0x20),uVar10);
      dVar7 = (double)CONCAT44(*(undefined4 *)(param_3 + 0x24),DAT_8c0175ec);
    }
    else {
      in_xd2 = CONCAT44(_DAT_8c0175f0,DAT_8c0175ec);
      dVar7 = *(double *)(param_3 + 0x24);
    }
    fVar11 = (float)((ulonglong)dVar12 >> 0x20);
    if (in_FPSCR_PR == '\0') {
      fVar14 = (float)((ulonglong)dVar7 >> 0x20) + SUB84(dVar7,0);
      dVar7 = (double)CONCAT44(fVar14,SUB84(dVar7,0));
      bVar15 = fVar14 < fVar11;
    }
    else {
      dVar7 = dVar7 + dVar7;
      bVar15 = dVar7 < dVar12;
    }
    if (bVar15) {
      dVar13 = (double)CONCAT44(0x3f800000,SUB84(dVar7,0));
      if (in_FPSCR_PR == '\0') {
        dVar13 = (double)CONCAT44(0x40000000,SUB84(dVar7,0));
        uVar10 = 0x40000000;
        dVar12 = (double)CONCAT44(fVar11 + 2.0,SUB84(dVar12,0));
      }
      else {
        dVar13 = dVar13 + dVar13;
        uVar10 = (undefined4)((ulonglong)dVar13 >> 0x20);
        dVar12 = dVar12 + dVar13;
      }
      if (in_FPSCR_SZ == '\0') {
        *(int *)(param_3 + 0x24) = (int)((ulonglong)dVar12 >> 0x20);
        lVar8 = (ulonglong)*(uint *)(iVar5 + 8) << 0x20;
      }
      else {
        *(double *)(param_3 + 0x24) = dVar12;
        lVar8 = *(longlong *)(iVar5 + 8);
      }
      if (in_FPSCR_PR == '\0' && (float)((ulonglong)lVar8 >> 0x20) < (float)lVar8) {
        if (in_FPSCR_SZ == '\0') {
          dVar7 = (double)CONCAT44(*(undefined4 *)(iVar5 + 8),(float)lVar8);
        }
        else {
          dVar7 = *(double *)(iVar5 + 8);
        }
        dVar13 = (double)CONCAT44(uVar10,_DAT_8c0175f0);
        if (in_FPSCR_PR == '\0') {
          dVar7 = (double)CONCAT44((float)((ulonglong)dVar7 >> 0x20) * _DAT_8c0175f0,SUB84(dVar7,0))
          ;
        }
        else {
          dVar7 = dVar7 * dVar13;
        }
        if (in_FPSCR_SZ == '\0') {
          *(int *)(iVar5 + 8) = (int)((ulonglong)dVar7 >> 0x20);
        }
        else {
          *(double *)(iVar5 + 8) = dVar7;
        }
      }
      if (in_FPSCR_SZ == '\0') {
        dVar12 = (double)CONCAT44(DAT_8c0175f4,SUB84(dVar12,0));
        dVar13 = (double)CONCAT44((int)((ulonglong)dVar13 >> 0x20),*(undefined4 *)(iVar5 + 4));
      }
      else {
        dVar12 = (double)CONCAT44(PTR_FUN_8c0175f8,DAT_8c0175f4);
        in_xd2 = *(undefined8 *)(iVar5 + 4);
      }
      fVar11 = (float)((ulonglong)dVar12 >> 0x20);
      if (in_FPSCR_PR == '\0') {
        fVar14 = SUB84(dVar13,0) * fVar11;
      }
      else {
        fVar14 = SUB84(dVar13 * dVar12,0);
      }
      if (in_FPSCR_SZ == '\0') {
        *(float *)(iVar5 + 4) = fVar14;
        dVar7 = (double)CONCAT44(*(undefined4 *)(iVar5 + 0xc),fVar14);
      }
      else {
        *(undefined8 *)(iVar5 + 4) = in_xd2;
        dVar7 = *(double *)(iVar5 + 0xc);
      }
      if (in_FPSCR_PR == '\0') {
        dVar7 = (double)CONCAT44((float)((ulonglong)dVar7 >> 0x20) * fVar11,SUB84(dVar7,0));
      }
      else {
        dVar7 = dVar7 * dVar12;
      }
      if (in_FPSCR_SZ == '\0') {
        *(int *)(iVar5 + 0xc) = (int)((ulonglong)dVar7 >> 0x20);
      }
      else {
        *(double *)(iVar5 + 0xc) = dVar7;
      }
    }
    uVar2 = *(ushort *)(param_3 + 6);
    *(ushort *)(param_3 + 6) = uVar2 + 1;
    if (200 < uVar2) {
      *param_3 = '\0';
      param_3[6] = '\0';
      param_3[7] = '\0';
    }
    (*(code *)PTR_FUN_8c0175f8)(param_3);
    return;
  }
  if (cVar1 == '\x02') {
    uVar2 = *(ushort *)(param_3 + 6);
    *(ushort *)(param_3 + 6) = uVar2 + 1;
    if (uVar2 < 0x1e) {
      iVar5 = (*(code *)PTR_FUN_8c017600)
                        (*(undefined4 *)(param_3 + 0x18),0x4000 - *(int *)(*DAT_8c0175fc + 0x18),
                         0x800);
    }
    else {
      iVar5 = 0x4000 - *(int *)(*DAT_8c0177a0 + 0x18);
    }
    *(int *)(param_3 + 0x18) = iVar5;
    if ((*(ushort *)(param_3 + 4) & 0x1000) == 0) {
      *param_3 = '\x01';
      param_3[6] = '\0';
      param_3[7] = '\0';
    }
    return;
  }
  return;
}


/* Address: 8c017698 */

void FUN_8c017698(double param_1,undefined4 param_2,int param_3,int param_4,undefined4 param_5)

{
  undefined1 *puVar1;
  float extraout_fr3;
  undefined4 extraout_fr2;
  float fVar2;
  undefined8 uVar3;
  undefined4 uVar4;
  undefined8 uVar5;
  undefined4 in_fr15;
  undefined8 in_xd14;
  undefined4 in_PR;
  char in_FPSCR_PR;
  char in_FPSCR_SZ;
  undefined1 local_10 [4];
  undefined1 local_c [4];
  
  if (in_FPSCR_SZ == '\0') {
    puVar1 = local_c;
  }
  else {
    puVar1 = local_10;
  }
  *(undefined4 *)(puVar1 + -4) = in_PR;
  *(undefined4 *)(puVar1 + -8) = param_5;
  if (*(int *)(param_4 + 8) == 0) {
    return;
  }
  (*(code *)PTR_FUN_8c0177ac)(*(undefined4 *)(*(int *)(param_3 + 0x68) + 0x10));
  (*(code *)PTR_FUN_8c0177b0)(0);
  if (in_FPSCR_SZ == '\0') {
    in_fr15 = *(undefined4 *)(*(int *)(param_3 + 0x68) + 8);
    uVar5 = CONCAT44(*(undefined4 *)(param_4 + 0x28),param_2);
    param_1 = (double)CONCAT44((int)((ulonglong)param_1 >> 0x20),*(undefined4 *)(param_4 + 0x24));
    fVar2 = DAT_8c0177b4;
  }
  else {
    in_xd14 = *(undefined8 *)(*(int *)(param_3 + 0x68) + 8);
    uVar5 = *(undefined8 *)(param_4 + 0x28);
    fVar2 = extraout_fr3;
  }
  if (in_FPSCR_PR == '\0') {
    fVar2 = SUB84(param_1,0) + fVar2;
  }
  else {
    fVar2 = SUB84(param_1 + (double)CONCAT44(extraout_fr2,fVar2),0);
  }
  if (in_FPSCR_SZ == '\0') {
    uVar3 = CONCAT44(*(undefined4 *)(param_4 + 0x20),fVar2);
  }
  else {
    uVar3 = *(undefined8 *)(param_4 + 0x20);
  }
  (*(code *)PTR_FUN_8c0177b8)(uVar3,uVar5,0);
  uVar4 = (undefined4)uVar5;
  if (*(int *)(puVar1 + -8) != 0) {
    (*(code *)PTR_FUN_8c0177bc)(0,*(undefined4 *)(puVar1 + -8));
  }
  uVar5 = in_xd14;
  if (in_FPSCR_SZ == '\0') {
    in_xd14 = CONCAT44(in_fr15,in_fr15);
    uVar5 = CONCAT44(in_fr15,uVar4);
  }
  (*(code *)PTR_FUN_8c0177c0)(in_xd14,uVar5,0);
  if (in_FPSCR_SZ == '\0') {
    uVar5 = CONCAT44(*(undefined4 *)(param_3 + 4),(int)in_xd14);
  }
  else {
    uVar5 = *(undefined8 *)(param_3 + 4);
  }
  (*(code *)PTR_FUN_8c0177c4)(uVar5,*(undefined4 *)(param_4 + 8));
  (*(code *)PTR_FUN_8c0177c8)(1);
  return;
}


/* Address: 8c01771e */

void FUN_8c01771e(undefined4 param_1,int param_2,int param_3,int param_4)

{
  float fVar1;
  float extraout_fr3;
  undefined4 extraout_fr2;
  double dVar2;
  undefined8 uVar3;
  undefined8 uVar4;
  char in_FPSCR_PR;
  char in_FPSCR_SZ;
  
  if ((*(ushort *)(param_3 + 6) & 0x40) == 0) {
    return;
  }
  (*(code *)PTR_FUN_8c0177ac)(PTR_PTR_8c0177cc);
  (*(code *)PTR_FUN_8c0177b0)(0);
  if (in_FPSCR_SZ == '\0') {
    uVar4 = CONCAT44(*(undefined4 *)(param_3 + 0x28),param_1);
    dVar2 = (double)CONCAT44(*(undefined4 *)(param_3 + 0x20),*(undefined4 *)(param_3 + 0x24));
    fVar1 = DAT_8c0177d0;
  }
  else {
    uVar4 = *(undefined8 *)(param_3 + 0x28);
    dVar2 = *(double *)(param_3 + 0x20);
    fVar1 = extraout_fr3;
  }
  if (in_FPSCR_PR == '\0') {
    dVar2 = (double)CONCAT44((int)((ulonglong)dVar2 >> 0x20),SUB84(dVar2,0) + fVar1);
  }
  else {
    dVar2 = dVar2 + (double)CONCAT44(extraout_fr2,fVar1);
  }
  (*(code *)PTR_FUN_8c0177b8)(dVar2,uVar4,0);
  if (in_FPSCR_SZ == '\0') {
    uVar4 = CONCAT44(*(undefined4 *)(param_2 + 0x28),(int)uVar4);
    uVar3 = CONCAT44(*(undefined4 *)(param_2 + 0x20),*(undefined4 *)(param_2 + 0x24));
  }
  else {
    uVar4 = *(undefined8 *)(param_2 + 0x28);
    uVar3 = *(undefined8 *)(param_2 + 0x20);
  }
  (*(code *)PTR_FUN_8c0177c0)(uVar3,uVar4,0);
  (*(code *)PTR_FUN_8c0177d4)(0,*(int *)(param_2 + 0x38),*(int *)(param_2 + 0x38) + param_4,param_4)
  ;
  (*(code *)PTR_FUN_8c0177dc)(PTR_DAT_8c0177d8);
  (*(code *)PTR_FUN_8c0177c8)(1);
  return;
}


/* Address: 8c0177e0 */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_8c0177e0(void)

{
  return;
}


/* Address: 8c017828 */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_8c017828(undefined4 param_1,int param_2)

{
  float fVar1;
  double dVar2;
  undefined4 uVar3;
  double dVar4;
  float fVar5;
  ulonglong in_xd2;
  undefined8 in_xd4;
  bool bVar6;
  char in_FPSCR_PR;
  char in_FPSCR_SZ;
  undefined4 local_24;
  undefined4 uStack_20;
  undefined4 uStack_1c;
  float local_18;
  float fStack_14;
  float local_10;
  
  if (in_FPSCR_SZ == '\0') {
    dVar4 = (double)CONCAT44(_DAT_8c0179ec,param_1);
    dVar2 = (double)CONCAT44(*(undefined4 *)(param_2 + 0x24),*(undefined4 *)(param_2 + 0x28));
  }
  else {
    dVar4 = (double)CONCAT44(_DAT_8c0179f0,_DAT_8c0179ec);
    in_xd2 = *(ulonglong *)(param_2 + 0x28);
    dVar2 = *(double *)(param_2 + 0x24);
  }
  fVar1 = (float)((ulonglong)dVar4 >> 0x20);
  if (in_FPSCR_PR == '\0') {
    dVar2 = (double)CONCAT44(((float)((ulonglong)dVar2 >> 0x20) + SUB84(dVar2,0)) * fVar1,
                             SUB84(dVar2,0));
  }
  else {
    dVar2 = (dVar2 + dVar2) * dVar4;
  }
  if (in_FPSCR_SZ == '\0') {
    local_24 = (undefined4)((ulonglong)dVar2 >> 0x20);
    dVar2 = (double)CONCAT44(*(undefined4 *)(param_2 + 0x28),*(undefined4 *)(param_2 + 0x20));
  }
  else {
    local_24 = SUB84(dVar2,0);
    in_xd2 = *(ulonglong *)(param_2 + 0x20);
    dVar2 = *(double *)(param_2 + 0x28);
  }
  if (in_FPSCR_PR == '\0') {
    dVar2 = (double)CONCAT44(((float)((ulonglong)dVar2 >> 0x20) + SUB84(dVar2,0)) * fVar1,
                             SUB84(dVar2,0));
  }
  else {
    dVar2 = (dVar2 + dVar2) * dVar4;
  }
  if (in_FPSCR_SZ == '\0') {
    uStack_20 = (undefined4)((ulonglong)dVar2 >> 0x20);
    dVar2 = (double)CONCAT44(*(undefined4 *)(param_2 + 0x20),*(undefined4 *)(param_2 + 0x24));
  }
  else {
    uStack_20 = SUB84(dVar2,0);
    in_xd2 = *(ulonglong *)(param_2 + 0x24);
    dVar2 = *(double *)(param_2 + 0x20);
  }
  if (in_FPSCR_PR == '\0') {
    dVar2 = (double)CONCAT44(((float)((ulonglong)dVar2 >> 0x20) + SUB84(dVar2,0)) * fVar1,
                             SUB84(dVar2,0));
  }
  else {
    dVar2 = (dVar2 + dVar2) * dVar4;
  }
  if (in_FPSCR_SZ == '\0') {
    uStack_1c = (undefined4)((ulonglong)dVar2 >> 0x20);
    dVar2 = (double)CONCAT44(local_24,*(undefined4 *)(param_2 + 0x20));
  }
  else {
    uStack_1c = SUB84(dVar2,0);
    dVar2 = (double)CONCAT44(uStack_20,local_24);
    in_xd2 = *(ulonglong *)(param_2 + 0x20);
  }
  if (in_FPSCR_PR == '\0') {
    dVar2 = (double)CONCAT44((float)((ulonglong)dVar2 >> 0x20) - SUB84(dVar2,0),SUB84(dVar2,0));
  }
  else {
    dVar2 = dVar2 - dVar2;
  }
  if (in_FPSCR_SZ == '\0') {
    local_18 = (float)((ulonglong)dVar2 >> 0x20);
    dVar2 = (double)CONCAT44(uStack_20,*(undefined4 *)(param_2 + 0x24));
  }
  else {
    local_18 = SUB84(dVar2,0);
    in_xd2 = *(ulonglong *)(param_2 + 0x24);
    dVar2 = (double)CONCAT44(uStack_1c,uStack_20);
  }
  if (in_FPSCR_PR == '\0') {
    dVar2 = (double)CONCAT44((float)((ulonglong)dVar2 >> 0x20) - SUB84(dVar2,0),SUB84(dVar2,0));
  }
  else {
    dVar2 = dVar2 - dVar2;
  }
  if (in_FPSCR_SZ == '\0') {
    fStack_14 = (float)((ulonglong)dVar2 >> 0x20);
    dVar2 = (double)CONCAT44(uStack_1c,*(undefined4 *)(param_2 + 0x28));
  }
  else {
    fStack_14 = SUB84(dVar2,0);
    in_xd2 = *(ulonglong *)(param_2 + 0x28);
    dVar2 = (double)CONCAT44(local_18,uStack_1c);
  }
  if (in_FPSCR_PR == '\0') {
    dVar2 = (double)CONCAT44((float)((ulonglong)dVar2 >> 0x20) - SUB84(dVar2,0),SUB84(dVar2,0));
  }
  else {
    dVar2 = dVar2 - dVar2;
  }
  local_10 = (float)((ulonglong)dVar2 >> 0x20);
  if (in_FPSCR_SZ == '\0') {
    dVar4 = (double)CONCAT44(fVar1,_DAT_8c0179f0);
    dVar2 = (double)CONCAT44(local_10,local_18);
  }
  else {
    local_10 = SUB84(dVar2,0);
    in_xd4 = CONCAT44(_DAT_8c0179f4,_DAT_8c0179f0);
    in_xd2 = CONCAT44(fStack_14,local_18);
  }
  fVar1 = SUB84(dVar4,0);
  if (in_FPSCR_PR == '\0') {
    dVar2 = (double)CONCAT44((int)((ulonglong)dVar2 >> 0x20),SUB84(dVar2,0) * fVar1);
  }
  else {
    dVar2 = dVar2 * dVar4;
  }
  if (in_FPSCR_SZ == '\0') {
    local_18 = SUB84(dVar2,0);
    dVar2 = (double)CONCAT44((int)((ulonglong)dVar2 >> 0x20),fStack_14);
  }
  else {
    local_18 = (float)in_xd2;
    fStack_14 = (float)(in_xd2 >> 0x20);
    in_xd2 = CONCAT44(local_10,fStack_14);
  }
  if (in_FPSCR_PR == '\0') {
    dVar2 = (double)CONCAT44((int)((ulonglong)dVar2 >> 0x20),SUB84(dVar2,0) * fVar1);
  }
  else {
    dVar2 = dVar2 * dVar4;
  }
  if (in_FPSCR_SZ == '\0') {
    fStack_14 = SUB84(dVar2,0);
    dVar2 = (double)CONCAT44((int)((ulonglong)dVar2 >> 0x20),local_10);
  }
  else {
    fStack_14 = (float)in_xd2;
    in_xd2 = in_xd2 >> 0x20;
  }
  if (in_FPSCR_PR == '\0') {
    local_10 = SUB84(dVar2,0) * fVar1;
  }
  else {
    local_10 = SUB84(dVar2 * dVar4,0);
  }
  if (in_FPSCR_SZ == '\0') {
    dVar2 = (double)CONCAT44(*(undefined4 *)(param_2 + 0x2c),local_18);
    local_18 = (float)in_xd2;
  }
  else {
    dVar2 = *(double *)(param_2 + 0x2c);
    local_10 = (float)in_xd2;
  }
  if (in_FPSCR_PR == '\0') {
    dVar2 = (double)CONCAT44((float)((ulonglong)dVar2 >> 0x20) + SUB84(dVar2,0),SUB84(dVar2,0));
  }
  else {
    dVar2 = dVar2 + dVar2;
  }
  if (in_FPSCR_SZ == '\0') {
    *(int *)(param_2 + 0x2c) = (int)((ulonglong)dVar2 >> 0x20);
    dVar2 = (double)CONCAT44(*(undefined4 *)(param_2 + 0x30),fStack_14);
    fStack_14 = local_18;
  }
  else {
    *(double *)(param_2 + 0x2c) = dVar2;
    dVar2 = *(double *)(param_2 + 0x30);
  }
  if (in_FPSCR_PR == '\0') {
    dVar2 = (double)CONCAT44((float)((ulonglong)dVar2 >> 0x20) + SUB84(dVar2,0),SUB84(dVar2,0));
  }
  else {
    dVar2 = dVar2 + dVar2;
  }
  if (in_FPSCR_SZ == '\0') {
    *(int *)(param_2 + 0x30) = (int)((ulonglong)dVar2 >> 0x20);
    dVar2 = (double)CONCAT44(*(undefined4 *)(param_2 + 0x34),local_10);
    local_10 = fStack_14;
  }
  else {
    *(double *)(param_2 + 0x30) = dVar2;
    dVar2 = *(double *)(param_2 + 0x34);
  }
  if (in_FPSCR_PR == '\0') {
    dVar2 = (double)CONCAT44((float)((ulonglong)dVar2 >> 0x20) + SUB84(dVar2,0),SUB84(dVar2,0));
  }
  else {
    dVar2 = dVar2 + dVar2;
  }
  uVar3 = (undefined4)((ulonglong)dVar2 >> 0x20);
  if (in_FPSCR_SZ == '\0') {
    *(undefined4 *)(param_2 + 0x34) = uVar3;
    dVar4 = (double)CONCAT44((int)((ulonglong)dVar4 >> 0x20),_DAT_8c0179f4);
    dVar2 = (double)CONCAT44(uVar3,*(undefined4 *)(param_2 + 0x2c));
  }
  else {
    *(double *)(param_2 + 0x34) = dVar2;
    in_xd4 = CONCAT44(_DAT_8c0179f8,_DAT_8c0179f4);
    local_10 = (float)*(undefined8 *)(param_2 + 0x2c);
  }
  fVar1 = SUB84(dVar4,0);
  if (in_FPSCR_PR == '\0') {
    dVar2 = (double)CONCAT44((int)((ulonglong)dVar2 >> 0x20),SUB84(dVar2,0) * fVar1);
  }
  else {
    dVar2 = dVar2 * dVar4;
  }
  if (in_FPSCR_SZ == '\0') {
    local_18 = SUB84(dVar2,0);
    dVar2 = (double)CONCAT44((int)((ulonglong)dVar2 >> 0x20),*(undefined4 *)(param_2 + 0x30));
    fVar5 = local_10;
  }
  else {
    fVar5 = (float)*(undefined8 *)(param_2 + 0x30);
    local_18 = local_10;
  }
  if (in_FPSCR_PR == '\0') {
    dVar2 = (double)CONCAT44((int)((ulonglong)dVar2 >> 0x20),SUB84(dVar2,0) * fVar1);
  }
  else {
    dVar2 = dVar2 * dVar4;
  }
  if (in_FPSCR_SZ == '\0') {
    fStack_14 = SUB84(dVar2,0);
    dVar2 = (double)CONCAT44((int)((ulonglong)dVar2 >> 0x20),*(undefined4 *)(param_2 + 0x34));
    local_10 = fVar5;
  }
  else {
    local_10 = (float)*(undefined8 *)(param_2 + 0x34);
    fStack_14 = fVar5;
  }
  if (in_FPSCR_PR == '\0') {
    fVar1 = SUB84(dVar2,0) * fVar1;
  }
  else {
    fVar1 = SUB84(dVar2 * dVar4,0);
  }
  if (in_FPSCR_SZ == '\0') {
    dVar2 = (double)CONCAT44(*(undefined4 *)(param_2 + 0x2c),local_18);
    local_10 = fVar1;
  }
  else {
    dVar2 = *(double *)(param_2 + 0x2c);
  }
  if (in_FPSCR_PR == '\0') {
    dVar2 = (double)CONCAT44((float)((ulonglong)dVar2 >> 0x20) - SUB84(dVar2,0),SUB84(dVar2,0));
  }
  else {
    dVar2 = dVar2 - dVar2;
  }
  if (in_FPSCR_SZ == '\0') {
    *(int *)(param_2 + 0x2c) = (int)((ulonglong)dVar2 >> 0x20);
    dVar2 = (double)CONCAT44(*(undefined4 *)(param_2 + 0x30),fStack_14);
  }
  else {
    *(double *)(param_2 + 0x2c) = dVar2;
    dVar2 = *(double *)(param_2 + 0x30);
  }
  if (in_FPSCR_PR == '\0') {
    dVar2 = (double)CONCAT44((float)((ulonglong)dVar2 >> 0x20) - SUB84(dVar2,0),SUB84(dVar2,0));
  }
  else {
    dVar2 = dVar2 - dVar2;
  }
  if (in_FPSCR_SZ == '\0') {
    *(int *)(param_2 + 0x30) = (int)((ulonglong)dVar2 >> 0x20);
    dVar2 = (double)CONCAT44(*(undefined4 *)(param_2 + 0x34),local_10);
  }
  else {
    *(double *)(param_2 + 0x30) = dVar2;
    dVar2 = *(double *)(param_2 + 0x34);
  }
  if (in_FPSCR_PR == '\0') {
    dVar2 = (double)CONCAT44((float)((ulonglong)dVar2 >> 0x20) - SUB84(dVar2,0),SUB84(dVar2,0));
  }
  else {
    dVar2 = dVar2 - dVar2;
  }
  if (in_FPSCR_SZ == '\0') {
    *(int *)(param_2 + 0x34) = (int)((ulonglong)dVar2 >> 0x20);
    dVar2 = (double)CONCAT44(*(undefined4 *)(param_2 + 0x20),*(undefined4 *)(param_2 + 0x2c));
  }
  else {
    *(double *)(param_2 + 0x34) = dVar2;
    dVar2 = *(double *)(param_2 + 0x20);
  }
  if (in_FPSCR_PR == '\0') {
    dVar2 = (double)CONCAT44((float)((ulonglong)dVar2 >> 0x20) + SUB84(dVar2,0),SUB84(dVar2,0));
  }
  else {
    dVar2 = dVar2 + dVar2;
  }
  if (in_FPSCR_SZ == '\0') {
    *(int *)(param_2 + 0x20) = (int)((ulonglong)dVar2 >> 0x20);
    dVar2 = (double)CONCAT44(*(undefined4 *)(param_2 + 0x24),*(undefined4 *)(param_2 + 0x30));
  }
  else {
    *(double *)(param_2 + 0x20) = dVar2;
    dVar2 = *(double *)(param_2 + 0x24);
  }
  if (in_FPSCR_PR == '\0') {
    dVar2 = (double)CONCAT44((float)((ulonglong)dVar2 >> 0x20) + SUB84(dVar2,0),SUB84(dVar2,0));
  }
  else {
    dVar2 = dVar2 + dVar2;
  }
  if (in_FPSCR_SZ == '\0') {
    *(int *)(param_2 + 0x24) = (int)((ulonglong)dVar2 >> 0x20);
    dVar2 = (double)CONCAT44(*(undefined4 *)(param_2 + 0x28),*(undefined4 *)(param_2 + 0x34));
  }
  else {
    *(double *)(param_2 + 0x24) = dVar2;
    dVar2 = *(double *)(param_2 + 0x28);
  }
  if (in_FPSCR_PR == '\0') {
    dVar2 = (double)CONCAT44((float)((ulonglong)dVar2 >> 0x20) + SUB84(dVar2,0),SUB84(dVar2,0));
  }
  else {
    dVar2 = dVar2 + dVar2;
  }
  uVar3 = (undefined4)((ulonglong)dVar2 >> 0x20);
  if (in_FPSCR_SZ == '\0') {
    *(undefined4 *)(param_2 + 0x28) = uVar3;
    dVar4 = (double)CONCAT44((int)((ulonglong)dVar4 >> 0x20),_DAT_8c0179f8);
    dVar2 = (double)CONCAT44(uVar3,*(undefined4 *)(param_2 + 0x20));
  }
  else {
    *(double *)(param_2 + 0x28) = dVar2;
    in_xd4 = CONCAT44(_DAT_8c0179fc,_DAT_8c0179f8);
  }
  fVar1 = SUB84(dVar4,0);
  if (in_FPSCR_PR == '\0') {
    bVar6 = fVar1 < SUB84(dVar2,0);
  }
  else {
    bVar6 = dVar4 < dVar2;
  }
  if (bVar6) {
    if (in_FPSCR_SZ == '\0') {
      *(float *)(param_2 + 0x20) = fVar1;
    }
    else {
      *(undefined8 *)(param_2 + 0x20) = in_xd4;
    }
  }
  if (in_FPSCR_SZ == '\0') {
    dVar2 = (double)CONCAT44((int)((ulonglong)dVar2 >> 0x20),*(undefined4 *)(param_2 + 0x24));
  }
  if (in_FPSCR_PR == '\0') {
    bVar6 = fVar1 < SUB84(dVar2,0);
  }
  else {
    bVar6 = dVar4 < dVar2;
  }
  if (bVar6) {
    if (in_FPSCR_SZ == '\0') {
      *(float *)(param_2 + 0x24) = fVar1;
    }
    else {
      *(undefined8 *)(param_2 + 0x24) = in_xd4;
    }
  }
  if (in_FPSCR_SZ == '\0') {
    dVar2 = (double)CONCAT44((int)((ulonglong)dVar2 >> 0x20),*(undefined4 *)(param_2 + 0x28));
  }
  if (in_FPSCR_PR == '\0') {
    bVar6 = fVar1 < SUB84(dVar2,0);
  }
  else {
    bVar6 = dVar4 < dVar2;
  }
  if (bVar6) {
    if (in_FPSCR_SZ == '\0') {
      *(float *)(param_2 + 0x28) = fVar1;
    }
    else {
      *(undefined8 *)(param_2 + 0x28) = in_xd4;
    }
  }
  if (in_FPSCR_SZ == '\0') {
    dVar2 = (double)CONCAT44((int)((ulonglong)dVar2 >> 0x20),*(undefined4 *)(param_2 + 0x20));
  }
  fVar1 = (float)((ulonglong)dVar4 >> 0x20);
  if (in_FPSCR_PR == '\0') {
    bVar6 = SUB84(dVar2,0) < fVar1;
  }
  else {
    bVar6 = dVar2 < dVar4;
  }
  if (bVar6) {
    if (in_FPSCR_SZ == '\0') {
      *(float *)(param_2 + 0x20) = fVar1;
    }
    else {
      *(double *)(param_2 + 0x20) = dVar4;
    }
  }
  if (in_FPSCR_SZ == '\0') {
    dVar2 = (double)CONCAT44((int)((ulonglong)dVar2 >> 0x20),*(undefined4 *)(param_2 + 0x24));
  }
  if (in_FPSCR_PR == '\0') {
    bVar6 = SUB84(dVar2,0) < fVar1;
  }
  else {
    bVar6 = dVar2 < dVar4;
  }
  if (bVar6) {
    if (in_FPSCR_SZ == '\0') {
      *(float *)(param_2 + 0x24) = fVar1;
    }
    else {
      *(double *)(param_2 + 0x24) = dVar4;
    }
  }
  if (in_FPSCR_SZ == '\0') {
    dVar2 = (double)CONCAT44((int)((ulonglong)dVar2 >> 0x20),*(undefined4 *)(param_2 + 0x28));
  }
  if (in_FPSCR_PR == '\0') {
    bVar6 = SUB84(dVar2,0) < fVar1;
  }
  else {
    bVar6 = dVar2 < dVar4;
  }
  if (bVar6) {
    if (in_FPSCR_SZ == '\0') {
      *(float *)(param_2 + 0x28) = fVar1;
    }
    else {
      *(double *)(param_2 + 0x28) = dVar4;
    }
  }
  return;
}


/* Address: 8c01798a */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_8c01798a(undefined4 param_1,float *param_2,uint param_3)

{
  float fVar1;
  float fVar2;
  double in_dr2;
  undefined4 uVar4;
  double dVar3;
  float fVar5;
  undefined8 in_xd2;
  char in_FPSCR_PR;
  char in_FPSCR_SZ;
  double dVar6;
  
  if (in_FPSCR_SZ == '\0') {
    param_1 = _DAT_8c0179fc;
  }
  if (in_FPSCR_SZ == '\0') {
    dVar6 = (double)CONCAT44(_DAT_8c017a00,param_1);
  }
  else {
    dVar6 = (double)CONCAT44(uRam8c017a04,_DAT_8c017a00);
  }
  fVar5 = SUB84(dVar6,0);
  fVar2 = (float)((ulonglong)dVar6 >> 0x20);
  uVar4 = (undefined4)((ulonglong)in_dr2 >> 0x20);
  if ((param_3 & 1) == 0) {
LAB_8c017a20:
    if ((param_3 & 4) != 0) {
      if (in_FPSCR_SZ == '\0') {
        in_dr2 = (double)CONCAT44(uVar4,*param_2);
      }
      else {
        in_xd2 = *(undefined8 *)param_2;
      }
      if (in_FPSCR_PR == '\0') {
        fVar1 = SUB84(in_dr2,0) + fVar2;
      }
      else {
        fVar1 = SUB84(in_dr2 + dVar6,0);
      }
      if (in_FPSCR_SZ == '\0') {
        *param_2 = fVar1;
        dVar3 = (double)CONCAT44(param_2[1],fVar1);
      }
      else {
        *(undefined8 *)param_2 = in_xd2;
        dVar3 = *(double *)(param_2 + 1);
      }
      if (in_FPSCR_PR == '\0') {
        dVar3 = (double)CONCAT44((float)((ulonglong)dVar3 >> 0x20) + fVar2,SUB84(dVar3,0));
      }
      else {
        dVar3 = dVar3 + dVar6;
      }
      fVar1 = (float)((ulonglong)dVar3 >> 0x20);
      if (in_FPSCR_SZ == '\0') {
        param_2[1] = fVar1;
        dVar3 = (double)CONCAT44(fVar1,param_2[2]);
      }
      else {
        *(double *)(param_2 + 1) = dVar3;
        in_xd2 = *(undefined8 *)(param_2 + 2);
      }
      if (in_FPSCR_PR == '\0') {
        in_dr2 = (double)CONCAT44((int)((ulonglong)dVar3 >> 0x20),SUB84(dVar3,0) - fVar5);
      }
      else {
        in_dr2 = dVar3 - dVar6;
      }
      if (in_FPSCR_SZ == '\0') {
        param_2[2] = SUB84(in_dr2,0);
      }
      else {
        *(undefined8 *)(param_2 + 2) = in_xd2;
      }
    }
  }
  else {
    if ((param_3 & 4) == 0) {
      if ((param_3 & 1) == 0) goto LAB_8c017a20;
      if (in_FPSCR_SZ == '\0') {
        in_dr2 = (double)CONCAT44(uVar4,*param_2);
      }
      else {
        in_xd2 = *(undefined8 *)param_2;
      }
      if (in_FPSCR_PR == '\0') {
        fVar1 = SUB84(in_dr2,0) - fVar5;
      }
      else {
        fVar1 = SUB84(in_dr2 - dVar6,0);
      }
      if (in_FPSCR_SZ == '\0') {
        *param_2 = fVar1;
        dVar3 = (double)CONCAT44(param_2[1],fVar1);
      }
      else {
        *(undefined8 *)param_2 = in_xd2;
        dVar3 = *(double *)(param_2 + 1);
      }
      if (in_FPSCR_PR == '\0') {
        dVar3 = (double)CONCAT44((float)((ulonglong)dVar3 >> 0x20) + fVar2,SUB84(dVar3,0));
      }
      else {
        dVar3 = dVar3 + dVar6;
      }
      fVar1 = (float)((ulonglong)dVar3 >> 0x20);
      if (in_FPSCR_SZ == '\0') {
        param_2[1] = fVar1;
        dVar3 = (double)CONCAT44(fVar1,param_2[2]);
      }
      else {
        *(double *)(param_2 + 1) = dVar3;
        in_xd2 = *(undefined8 *)(param_2 + 2);
      }
      if (in_FPSCR_PR == '\0') {
        in_dr2 = (double)CONCAT44((int)((ulonglong)dVar3 >> 0x20),SUB84(dVar3,0) + fVar2);
      }
      else {
        in_dr2 = dVar3 + dVar6;
      }
    }
    else {
      if (in_FPSCR_SZ == '\0') {
        in_dr2 = (double)CONCAT44(uVar4,*param_2);
      }
      else {
        in_xd2 = *(undefined8 *)param_2;
      }
      if (in_FPSCR_PR == '\0') {
        fVar1 = SUB84(in_dr2,0) - fVar2;
      }
      else {
        fVar1 = SUB84(in_dr2 - dVar6,0);
      }
      if (in_FPSCR_SZ == '\0') {
        *param_2 = fVar1;
        dVar3 = (double)CONCAT44(param_2[1],fVar1);
      }
      else {
        *(undefined8 *)param_2 = in_xd2;
        dVar3 = *(double *)(param_2 + 1);
      }
      if (in_FPSCR_PR == '\0') {
        dVar3 = (double)CONCAT44((float)((ulonglong)dVar3 >> 0x20) + fVar5,SUB84(dVar3,0));
      }
      else {
        dVar3 = dVar3 + dVar6;
      }
      fVar1 = (float)((ulonglong)dVar3 >> 0x20);
      if (in_FPSCR_SZ == '\0') {
        param_2[1] = fVar1;
        dVar3 = (double)CONCAT44(fVar1,param_2[2]);
      }
      else {
        *(double *)(param_2 + 1) = dVar3;
        in_xd2 = *(undefined8 *)(param_2 + 2);
      }
      if (in_FPSCR_PR == '\0') {
        in_dr2 = (double)CONCAT44((int)((ulonglong)dVar3 >> 0x20),SUB84(dVar3,0) - fVar2);
      }
      else {
        in_dr2 = dVar3 - dVar6;
      }
    }
    if (in_FPSCR_SZ == '\0') {
      param_2[2] = SUB84(in_dr2,0);
    }
    else {
      *(undefined8 *)(param_2 + 2) = in_xd2;
    }
  }
  if ((param_3 & 2) != 0) {
    if (in_FPSCR_SZ == '\0') {
      in_dr2 = (double)CONCAT44((int)((ulonglong)in_dr2 >> 0x20),*param_2);
    }
    else {
      in_xd2 = *(undefined8 *)param_2;
    }
    if (in_FPSCR_PR == '\0') {
      fVar1 = SUB84(in_dr2,0) + fVar2;
    }
    else {
      fVar1 = SUB84(in_dr2 + dVar6,0);
    }
    if (in_FPSCR_SZ == '\0') {
      *param_2 = fVar1;
      dVar3 = (double)CONCAT44(param_2[1],fVar1);
    }
    else {
      *(undefined8 *)param_2 = in_xd2;
      dVar3 = *(double *)(param_2 + 1);
    }
    if (in_FPSCR_PR == '\0') {
      dVar3 = (double)CONCAT44((float)((ulonglong)dVar3 >> 0x20) - fVar5,SUB84(dVar3,0));
    }
    else {
      dVar3 = dVar3 - dVar6;
    }
    fVar5 = (float)((ulonglong)dVar3 >> 0x20);
    if (in_FPSCR_SZ == '\0') {
      param_2[1] = fVar5;
      dVar3 = (double)CONCAT44(fVar5,param_2[2]);
    }
    else {
      *(double *)(param_2 + 1) = dVar3;
      in_xd2 = *(undefined8 *)(param_2 + 2);
    }
    if (in_FPSCR_PR == '\0') {
      fVar2 = SUB84(dVar3,0) + fVar2;
    }
    else {
      fVar2 = SUB84(dVar3 + dVar6,0);
    }
    if (in_FPSCR_SZ == '\0') {
      param_2[2] = fVar2;
    }
    else {
      *(undefined8 *)(param_2 + 2) = in_xd2;
    }
  }
  return;
}


/* Address: 8c017b20 */

/* WARNING: Removing unreachable block (ram,0x8c017b9a) */
/* WARNING: Removing unreachable block (ram,0x8c017b50) */
/* WARNING: Removing unreachable block (ram,0x8c017b82) */
/* WARNING: Removing unreachable block (ram,0x8c017bd0) */
/* WARNING: Removing unreachable block (ram,0x8c017c40) */
/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_8c017b20(undefined4 param_1,undefined4 param_2,undefined4 param_3)

{
  undefined *puVar1;
  undefined *puVar2;
  undefined *puVar3;
  undefined *puVar4;
  int iVar5;
  int iVar6;
  int iVar7;
  undefined1 *puVar8;
  undefined8 *puVar9;
  double dVar10;
  float fVar11;
  undefined4 uVar12;
  float fVar13;
  float fVar15;
  double dVar14;
  undefined8 in_dr12;
  double dVar16;
  undefined8 in_dr14;
  double dVar17;
  double in_xd2;
  undefined8 in_xd14;
  undefined4 in_PR;
  undefined2 uVar18;
  int iVar19;
  char in_FPSCR_PR;
  char in_FPSCR_SZ;
  undefined1 auStack_3c [16];
  undefined8 uStack_2c;
  undefined8 uStack_24;
  
  puVar2 = PTR_FUN_8c017d20;
  if (in_FPSCR_SZ == '\0') {
    uStack_24 = CONCAT44((int)in_dr14,(undefined4)uStack_24);
                    /* WARNING: Ignoring partial resolution of indirect */
    uStack_24._0_4_ = (int)((ulonglong)in_dr14 >> 0x20);
    puVar9 = &uStack_2c;
    uStack_2c = CONCAT44((int)in_dr12,(int)((ulonglong)in_dr12 >> 0x20));
  }
  else {
    puVar9 = (undefined8 *)auStack_3c;
    uStack_24 = in_xd14;
    uStack_2c = in_dr14;
  }
  *(undefined4 *)((int)puVar9 + -4) = in_PR;
  *(undefined4 *)((int)puVar9 + -0x20) = param_1;
  *(undefined4 *)((int)puVar9 + -0xc) = param_2;
  *(undefined4 *)((int)puVar9 + -8) = param_3;
  iVar5 = (*(code *)PTR_FUN_8c017d20)();
  puVar4 = PTR_FUN_8c017d38;
  puVar3 = PTR_FUN_8c017d34;
  fVar13 = DAT_8c017d2c;
  fVar15 = DAT_8c017d24._4_4_;
  if (in_FPSCR_SZ == '\0') {
    fVar13 = DAT_8c017d24._4_4_;
    fVar15 = (float)DAT_8c017d24;
  }
  fVar11 = (float)iVar5;
  dVar10 = (double)CONCAT44(fVar15,fVar11);
  dVar16 = (double)CONCAT44(0x3f800000,(int)in_dr12);
  if (in_FPSCR_PR == '\0') {
    dVar16 = (double)CONCAT44(0x40000000,(int)in_dr12);
  }
  else {
    dVar16 = dVar16 + dVar16;
  }
  iVar5 = 0;
  if (in_FPSCR_PR == '\0') {
    fVar15 = fVar13 * fVar11 + fVar15;
    dVar10 = (double)CONCAT44(fVar15,fVar11);
    iVar19 = (int)fVar15;
  }
  else {
    iVar19 = (int)dVar10;
  }
  *(int *)((int)puVar9 + -0x1c) = iVar19;
  if (in_FPSCR_SZ == '\0') {
    dVar16 = (double)CONCAT44((int)((ulonglong)dVar16 >> 0x20),DAT_8c017d2c);
    dVar17 = (double)((ulonglong)DAT_8c017d30 << 0x20);
  }
  else {
    dVar17 = (double)CONCAT44(PTR_FUN_8c017d34,DAT_8c017d30);
  }
  do {
    iVar19 = (*(code *)PTR_FUN_8c017d40)(2,6,PTR_FUN_8c017d3c);
    uVar12 = (undefined4)((ulonglong)dVar10 >> 0x20);
    if (iVar19 != 0) {
      puVar8 = *(undefined1 **)(iVar19 + 0x20);
      iVar6 = (*(code *)puVar2)();
      if (in_FPSCR_PR == '\0') {
        iVar6 = (int)((float)iVar6 * (float)((ulonglong)dVar16 >> 0x20));
      }
      else {
        iVar6 = (int)((double)CONCAT44(uVar12,(float)iVar6) * dVar16);
      }
      *(int *)((int)puVar9 + -0x14) = iVar6;
      iVar6 = (*(code *)puVar2)();
      if (in_FPSCR_SZ == '\0') {
        *(int *)(puVar8 + 8) = SUB84(dVar17,0);
      }
      else {
        *(undefined8 *)(puVar8 + 8) = in_xd14;
      }
      dVar14 = in_xd2;
      if (in_FPSCR_SZ == '\0') {
        dVar14 = (double)CONCAT44((float)iVar6,(float)iVar6);
      }
      if (in_FPSCR_PR == '\0') {
        dVar14 = (double)CONCAT44((float)((ulonglong)dVar14 >> 0x20) *
                                  (float)((ulonglong)dVar16 >> 0x20),SUB84(dVar14,0));
      }
      else {
        dVar14 = dVar14 * dVar16;
      }
      if (in_FPSCR_SZ == '\0') {
        uVar12 = *(undefined4 *)*(double **)((int)puVar9 + -0x20);
        dVar14 = (double)CONCAT44((int)((ulonglong)dVar14 >> 0x20),uVar12);
        *(undefined4 *)(puVar8 + 0x20) = uVar12;
      }
      else {
        in_xd2 = **(double **)((int)puVar9 + -0x20);
        *(double *)(puVar8 + 0x20) = in_xd2;
      }
      fVar15 = (float)((ulonglong)dVar14 >> 0x20);
      if (in_FPSCR_PR == '\0') {
        iVar6 = (int)fVar15;
      }
      else {
        iVar6 = (int)dVar14;
      }
      if (in_FPSCR_SZ == '\0') {
        uVar12 = *(undefined4 *)(*(int *)((int)puVar9 + -0x20) + 4);
        dVar14 = (double)CONCAT44(fVar15,uVar12);
        *(undefined4 *)(puVar8 + 0x24) = uVar12;
      }
      else {
        in_xd2 = *(double *)(*(int *)((int)puVar9 + -0x20) + 4);
        *(double *)(puVar8 + 0x24) = in_xd2;
      }
      if (in_FPSCR_SZ == '\0') {
        uVar12 = *(undefined4 *)(*(int *)((int)puVar9 + -0x20) + 8);
        dVar14 = (double)CONCAT44((int)((ulonglong)dVar14 >> 0x20),uVar12);
        *(undefined4 *)(puVar8 + 0x28) = uVar12;
      }
      else {
        in_xd2 = *(double *)(*(int *)((int)puVar9 + -0x20) + 8);
        *(double *)(puVar8 + 0x28) = in_xd2;
      }
      *puVar8 = (char)*(undefined4 *)((int)puVar9 + -0xc);
      if (iVar5 < 7) {
        iVar7 = (*(code *)puVar2)();
        if (in_FPSCR_SZ == '\0') {
          fVar15 = SUB84(dVar17,0);
          puVar1 = _DAT_8c017d44;
        }
        else {
          fVar15 = (float)((ulonglong)in_xd14 >> 0x20);
          puVar1 = PTR_FUN_8c017d48;
        }
        fVar13 = (float)iVar7;
        dVar14 = (double)CONCAT44(fVar15,fVar13);
        if (in_FPSCR_PR == '\0') {
          fVar15 = (float)puVar1 * fVar13 + fVar15;
          dVar14 = (double)CONCAT44(fVar15,fVar13);
          uVar18 = (undefined2)(int)fVar15;
        }
        else {
          uVar18 = (undefined2)(int)dVar14;
        }
        *(undefined2 *)(puVar8 + 0xe) = uVar18;
        *(undefined2 *)(puVar8 + 0xc) = 0;
        dVar10 = (double)(*(code *)puVar4)(*(undefined4 *)((int)puVar9 + -0x14));
        if (in_FPSCR_PR == '\0') {
          dVar10 = (double)CONCAT44((float)((ulonglong)dVar10 >> 0x20) *
                                    (float)((ulonglong)dVar17 >> 0x20),SUB84(dVar10,0));
        }
        else {
          dVar10 = dVar10 * dVar17;
        }
        if (in_FPSCR_SZ == '\0') {
          *(int *)(puVar8 + 0x30) = (int)((ulonglong)dVar10 >> 0x20);
        }
        else {
          *(double *)(puVar8 + 0x30) = dVar10;
        }
        dVar10 = (double)(*(code *)puVar3)(*(undefined4 *)((int)puVar9 + -0x14));
        if (in_FPSCR_PR == '\0') {
          dVar10 = (double)CONCAT44((float)((ulonglong)dVar10 >> 0x20) *
                                    (float)((ulonglong)dVar17 >> 0x20),SUB84(dVar10,0));
        }
        else {
          dVar10 = dVar10 * dVar17;
        }
        if (in_FPSCR_SZ == '\0') {
          *(int *)((int)puVar9 + -0x18) = (int)((ulonglong)dVar10 >> 0x20);
        }
        else {
          *(double *)((int)puVar9 + -0x18) = dVar10;
        }
        dVar10 = (double)(*(code *)puVar3)(iVar6);
        if (in_FPSCR_SZ == '\0') {
          dVar14 = (double)CONCAT44((int)((ulonglong)dVar14 >> 0x20),
                                    *(undefined4 *)((int)puVar9 + -0x18));
        }
        else {
          in_xd2 = *(double *)((int)puVar9 + -0x18);
        }
        if (in_FPSCR_PR == '\0') {
          dVar14 = (double)CONCAT44((int)((ulonglong)dVar14 >> 0x20),
                                    SUB84(dVar14,0) * (float)((ulonglong)dVar10 >> 0x20));
        }
        else {
          dVar14 = dVar14 * dVar10;
        }
        if (in_FPSCR_SZ == '\0') {
          *(int *)(puVar8 + 0x2c) = SUB84(dVar14,0);
        }
        else {
          *(double *)(puVar8 + 0x2c) = in_xd2;
        }
        dVar10 = (double)(*(code *)puVar4)(iVar6);
        if (in_FPSCR_SZ == '\0') {
          dVar14 = (double)CONCAT44((int)((ulonglong)dVar14 >> 0x20),
                                    *(undefined4 *)((int)puVar9 + -0x18));
        }
        else {
          in_xd2 = *(double *)((int)puVar9 + -0x18);
        }
      }
      else {
        *(undefined2 *)(puVar8 + 0xe) = 10;
        *(undefined2 *)(puVar8 + 0xc) = 1;
        dVar10 = (double)(*(code *)puVar4)(*(undefined4 *)((int)puVar9 + -0x1c));
        if (in_FPSCR_PR == '\0') {
          dVar10 = (double)CONCAT44((float)((ulonglong)dVar10 >> 0x20) * SUB84(dVar16,0),
                                    SUB84(dVar10,0));
        }
        else {
          dVar10 = dVar10 * dVar16;
        }
        if (in_FPSCR_SZ == '\0') {
          *(int *)(puVar8 + 0x30) = (int)((ulonglong)dVar10 >> 0x20);
        }
        else {
          *(double *)(puVar8 + 0x30) = dVar10;
        }
        dVar10 = (double)(*(code *)puVar3)(*(undefined4 *)((int)puVar9 + -0x1c));
        if (in_FPSCR_PR == '\0') {
          dVar10 = (double)CONCAT44((float)((ulonglong)dVar10 >> 0x20) * SUB84(dVar16,0),
                                    SUB84(dVar10,0));
        }
        else {
          dVar10 = dVar10 * dVar16;
        }
        if (in_FPSCR_SZ == '\0') {
          *(int *)((int)puVar9 + -0x10) = (int)((ulonglong)dVar10 >> 0x20);
        }
        else {
          *(double *)((int)puVar9 + -0x10) = dVar10;
        }
        dVar10 = (double)(*(code *)puVar3)(iVar6);
        if (in_FPSCR_SZ == '\0') {
          dVar14 = (double)CONCAT44((int)((ulonglong)dVar14 >> 0x20),
                                    *(undefined4 *)((int)puVar9 + -0x10));
        }
        else {
          in_xd2 = *(double *)((int)puVar9 + -0x10);
        }
        if (in_FPSCR_PR == '\0') {
          dVar14 = (double)CONCAT44((int)((ulonglong)dVar14 >> 0x20),
                                    SUB84(dVar14,0) * (float)((ulonglong)dVar10 >> 0x20));
        }
        else {
          dVar14 = dVar14 * dVar10;
        }
        if (in_FPSCR_SZ == '\0') {
          *(int *)(puVar8 + 0x2c) = SUB84(dVar14,0);
        }
        else {
          *(double *)(puVar8 + 0x2c) = in_xd2;
        }
        dVar10 = (double)(*(code *)puVar4)(iVar6);
        if (in_FPSCR_SZ == '\0') {
          dVar14 = (double)CONCAT44((int)((ulonglong)dVar14 >> 0x20),
                                    *(undefined4 *)((int)puVar9 + -0x10));
        }
        else {
          in_xd2 = *(double *)((int)puVar9 + -0x10);
        }
      }
      if (in_FPSCR_PR == '\0') {
        dVar14 = (double)CONCAT44((int)((ulonglong)dVar14 >> 0x20),
                                  SUB84(dVar14,0) * (float)((ulonglong)dVar10 >> 0x20));
      }
      else {
        dVar14 = dVar14 * dVar10;
      }
      dVar10 = (double)CONCAT44((int)((ulonglong)dVar14 >> 0x20),-SUB84(dVar14,0));
      if (in_FPSCR_SZ == '\0') {
        *(float *)(puVar8 + 0x34) = -SUB84(dVar14,0);
      }
      else {
        *(double *)(puVar8 + 0x34) = in_xd2;
      }
      if (*(int *)((int)puVar9 + -8) != 0) {
        if (in_FPSCR_SZ == '\0') {
          dVar10 = (double)((ulonglong)*(uint *)(puVar8 + 0x30) << 0x20);
        }
        else {
          dVar10 = *(double *)(puVar8 + 0x30);
        }
        if (in_FPSCR_PR == '\0' && (float)((ulonglong)dVar10 >> 0x20) < SUB84(dVar10,0)) {
          if (in_FPSCR_SZ == '\0') {
            dVar10 = (double)CONCAT44(*(undefined4 *)(puVar8 + 0x30),SUB84(dVar10,0));
          }
          else {
            dVar10 = *(double *)(puVar8 + 0x30);
          }
          if (in_FPSCR_PR == '\0') {
            dVar10 = (double)CONCAT44(-(float)((ulonglong)dVar10 >> 0x20),SUB84(dVar10,0));
          }
          else {
            dVar10 = -dVar10;
          }
          if (in_FPSCR_SZ == '\0') {
            *(int *)(puVar8 + 0x30) = (int)((ulonglong)dVar10 >> 0x20);
          }
          else {
            *(double *)(puVar8 + 0x30) = dVar10;
          }
        }
      }
      *(undefined **)(iVar19 + 0x14) = PTR_FUN_8c017d48;
    }
    iVar5 = iVar5 + 1;
  } while (iVar5 < 0xe);
  return;
}


/* Address: 8c017c82 */

void FUN_8c017c82(undefined8 *param_1,undefined1 param_2)

{
  undefined *puVar1;
  int iVar2;
  undefined1 *puVar3;
  undefined4 extraout_fr3;
  undefined4 uVar4;
  undefined8 in_xd2;
  char in_FPSCR_SZ;
  
  iVar2 = (*(code *)PTR_FUN_8c017d40)(2,6,PTR_LAB_8c017d4c);
  puVar1 = PTR_LAB_8c017d50;
  if (iVar2 != 0) {
    if (in_FPSCR_SZ == '\0') {
      uVar4 = *(undefined4 *)param_1;
    }
    else {
      in_xd2 = *param_1;
      uVar4 = extraout_fr3;
    }
    puVar3 = *(undefined1 **)(iVar2 + 0x20);
    if (in_FPSCR_SZ == '\0') {
      *(undefined4 *)(puVar3 + 0x20) = uVar4;
      *(undefined4 *)(puVar3 + 0x24) = *(undefined4 *)((int)param_1 + 4);
      *(undefined4 *)(puVar3 + 0x28) = *(undefined4 *)(param_1 + 1);
    }
    else {
      *(undefined8 *)(puVar3 + 0x20) = in_xd2;
      *(undefined8 *)(puVar3 + 0x24) = *(undefined8 *)((int)param_1 + 4);
      in_xd2 = param_1[1];
      *(undefined8 *)(puVar3 + 0x28) = in_xd2;
    }
    *(undefined2 *)(puVar3 + 0xc) = 0;
    *puVar3 = 0;
    puVar3[1] = param_2;
    if (in_FPSCR_SZ == '\0') {
      *(undefined4 *)(puVar3 + 8) = 0;
    }
    else {
      *(undefined8 *)(puVar3 + 8) = in_xd2;
    }
    *(undefined2 *)(puVar3 + 0xe) = 0;
    *(undefined **)(iVar2 + 0x14) = puVar1;
  }
  return;
}


/* Address: 8c017cd6 */

void FUN_8c017cd6(undefined4 param_1,int param_2)

{
  short sVar1;
  int iVar2;
  undefined8 in_dr0;
  undefined4 uVar4;
  double dVar3;
  float fVar7;
  double dVar5;
  double dVar6;
  double dVar8;
  undefined8 in_xd0;
  bool bVar9;
  undefined4 in_PR;
  char in_FPSCR_PR;
  char in_FPSCR_SZ;
  undefined4 local_10;
  undefined4 uStack_c;
  undefined4 local_8;
  undefined4 uStack_4;
  
  iVar2 = *(int *)(param_2 + 0x20);
  sVar1 = *(short *)(iVar2 + 0xe);
  *(short *)(iVar2 + 0xe) = sVar1 + -1;
  if (sVar1 < 1) {
    if (in_FPSCR_SZ == '\0') {
      fVar7 = *(float *)(iVar2 + 8);
    }
    else {
      fVar7 = (float)((ulonglong)*(undefined8 *)(iVar2 + 8) >> 0x20);
    }
    uVar4 = (undefined4)((ulonglong)in_dr0 >> 0x20);
    dVar3 = (double)CONCAT44(uVar4,0x3f800000);
    if (in_FPSCR_PR == '\0') {
      fVar7 = fVar7 + DAT_8c017d54;
      dVar5 = (double)CONCAT44(fVar7,DAT_8c017d54);
      bVar9 = fVar7 < 1.0;
    }
    else {
      dVar5 = (double)CONCAT44(fVar7,DAT_8c017d54) + (double)CONCAT44(fVar7,DAT_8c017d54);
      fVar7 = (float)((ulonglong)dVar5 >> 0x20);
      bVar9 = dVar5 < dVar3;
    }
    if (in_FPSCR_SZ == '\0') {
      *(float *)(iVar2 + 8) = fVar7;
    }
    else {
      *(double *)(iVar2 + 8) = dVar5;
    }
    if (!bVar9) {
      (*(code *)PTR_FUN_8c017d58)();
      return;
    }
    if (in_FPSCR_SZ == '\0') {
      fVar7 = *(float *)(iVar2 + 0x30);
    }
    else {
      fVar7 = (float)((ulonglong)*(undefined8 *)(iVar2 + 0x30) >> 0x20);
    }
    if (in_FPSCR_PR == '\0') {
      dVar5 = (double)CONCAT44(fVar7 - DAT_8c017ee0,DAT_8c017ee0);
    }
    else {
      dVar5 = (double)CONCAT44(fVar7,DAT_8c017ee0) - (double)CONCAT44(fVar7,DAT_8c017ee0);
    }
    if (in_FPSCR_SZ == '\0') {
      *(int *)(iVar2 + 0x30) = (int)((ulonglong)dVar5 >> 0x20);
      dVar8 = (double)CONCAT44(DAT_8c017ee4,param_1);
      dVar5 = (double)CONCAT44(*(undefined4 *)(iVar2 + 0x2c),SUB84(dVar5,0));
    }
    else {
      *(double *)(iVar2 + 0x30) = dVar5;
      dVar8 = (double)CONCAT44(DAT_8c017ee8,DAT_8c017ee4);
      dVar5 = *(double *)(iVar2 + 0x2c);
    }
    fVar7 = (float)((ulonglong)dVar8 >> 0x20);
    if (in_FPSCR_PR == '\0') {
      dVar5 = (double)CONCAT44((float)((ulonglong)dVar5 >> 0x20) * fVar7,SUB84(dVar5,0));
    }
    else {
      dVar5 = dVar5 * dVar8;
    }
    local_10 = SUB84(dVar5,0);
    if (in_FPSCR_SZ == '\0') {
      dVar6 = (double)CONCAT44(*(undefined4 *)(iVar2 + 0x30),local_10);
      local_10 = (int)((ulonglong)dVar5 >> 0x20);
    }
    else {
      dVar6 = *(double *)(iVar2 + 0x30);
    }
    if (in_FPSCR_PR == '\0') {
      dVar6 = (double)CONCAT44((float)((ulonglong)dVar6 >> 0x20) * fVar7,SUB84(dVar6,0));
    }
    else {
      dVar6 = dVar6 * dVar8;
    }
    uStack_c = SUB84(dVar6,0);
    if (in_FPSCR_SZ == '\0') {
      dVar5 = (double)CONCAT44(*(undefined4 *)(iVar2 + 0x34),uStack_c);
      uStack_c = (int)((ulonglong)dVar6 >> 0x20);
    }
    else {
      dVar5 = *(double *)(iVar2 + 0x34);
    }
    if (in_FPSCR_PR == '\0') {
      dVar5 = (double)CONCAT44((float)((ulonglong)dVar5 >> 0x20) * fVar7,SUB84(dVar5,0));
    }
    else {
      dVar5 = dVar5 * dVar8;
    }
    local_8 = SUB84(dVar5,0);
    uStack_4 = (undefined4)((ulonglong)dVar5 >> 0x20);
    if (in_FPSCR_SZ == '\0') {
      dVar3 = (double)CONCAT44(uVar4,*(undefined4 *)(iVar2 + 0x2c));
      dVar5 = (double)CONCAT44(local_10,local_8);
      local_8 = uStack_4;
      uStack_4 = in_PR;
    }
    else {
      in_xd0 = *(undefined8 *)(iVar2 + 0x2c);
      dVar5 = (double)CONCAT44(uStack_c,local_10);
    }
    if (in_FPSCR_PR == '\0') {
      dVar3 = (double)CONCAT44((int)((ulonglong)dVar3 >> 0x20),
                               SUB84(dVar3,0) - (float)((ulonglong)dVar5 >> 0x20));
    }
    else {
      dVar3 = dVar3 - dVar5;
    }
    if (in_FPSCR_SZ == '\0') {
      *(int *)(iVar2 + 0x2c) = SUB84(dVar3,0);
      dVar5 = (double)CONCAT44(uStack_c,SUB84(dVar5,0));
      dVar3 = (double)CONCAT44((int)((ulonglong)dVar3 >> 0x20),*(undefined4 *)(iVar2 + 0x30));
    }
    else {
      *(undefined8 *)(iVar2 + 0x2c) = in_xd0;
      dVar5 = (double)CONCAT44(local_8,uStack_c);
      in_xd0 = *(undefined8 *)(iVar2 + 0x30);
    }
    if (in_FPSCR_PR == '\0') {
      dVar3 = (double)CONCAT44((int)((ulonglong)dVar3 >> 0x20),
                               SUB84(dVar3,0) - (float)((ulonglong)dVar5 >> 0x20));
    }
    else {
      dVar3 = dVar3 - dVar5;
    }
    if (in_FPSCR_SZ == '\0') {
      *(int *)(iVar2 + 0x30) = SUB84(dVar3,0);
      dVar3 = (double)CONCAT44((int)((ulonglong)dVar3 >> 0x20),*(undefined4 *)(iVar2 + 0x34));
      dVar5 = (double)CONCAT44(local_8,SUB84(dVar5,0));
    }
    else {
      *(undefined8 *)(iVar2 + 0x30) = in_xd0;
      in_xd0 = *(undefined8 *)(iVar2 + 0x34);
      dVar5 = (double)CONCAT44(uStack_4,local_8);
    }
    if (in_FPSCR_PR == '\0') {
      dVar3 = (double)CONCAT44((int)((ulonglong)dVar3 >> 0x20),
                               SUB84(dVar3,0) - (float)((ulonglong)dVar5 >> 0x20));
    }
    else {
      dVar3 = dVar3 - dVar5;
    }
    if (in_FPSCR_SZ == '\0') {
      *(int *)(iVar2 + 0x34) = SUB84(dVar3,0);
      dVar3 = (double)CONCAT44((int)((ulonglong)dVar3 >> 0x20),*(undefined4 *)(iVar2 + 0x20));
      dVar5 = (double)CONCAT44(*(undefined4 *)(iVar2 + 0x2c),SUB84(dVar5,0));
    }
    else {
      *(undefined8 *)(iVar2 + 0x34) = in_xd0;
      in_xd0 = *(undefined8 *)(iVar2 + 0x20);
      dVar5 = *(double *)(iVar2 + 0x2c);
    }
    if (in_FPSCR_PR == '\0') {
      dVar3 = (double)CONCAT44((int)((ulonglong)dVar3 >> 0x20),
                               SUB84(dVar3,0) + (float)((ulonglong)dVar5 >> 0x20));
    }
    else {
      dVar3 = dVar3 + dVar5;
    }
    if (in_FPSCR_SZ == '\0') {
      *(int *)(iVar2 + 0x20) = SUB84(dVar3,0);
      dVar3 = (double)CONCAT44((int)((ulonglong)dVar3 >> 0x20),*(undefined4 *)(iVar2 + 0x24));
      dVar5 = (double)CONCAT44(*(undefined4 *)(iVar2 + 0x30),SUB84(dVar5,0));
    }
    else {
      *(undefined8 *)(iVar2 + 0x20) = in_xd0;
      in_xd0 = *(undefined8 *)(iVar2 + 0x24);
      dVar5 = *(double *)(iVar2 + 0x30);
    }
    if (in_FPSCR_PR == '\0') {
      dVar3 = (double)CONCAT44((int)((ulonglong)dVar3 >> 0x20),
                               SUB84(dVar3,0) + (float)((ulonglong)dVar5 >> 0x20));
    }
    else {
      dVar3 = dVar3 + dVar5;
    }
    if (in_FPSCR_SZ == '\0') {
      *(int *)(iVar2 + 0x24) = SUB84(dVar3,0);
      dVar3 = (double)CONCAT44((int)((ulonglong)dVar3 >> 0x20),*(undefined4 *)(iVar2 + 0x28));
      dVar5 = (double)CONCAT44(*(undefined4 *)(iVar2 + 0x34),SUB84(dVar5,0));
    }
    else {
      *(undefined8 *)(iVar2 + 0x24) = in_xd0;
      in_xd0 = *(undefined8 *)(iVar2 + 0x28);
      dVar5 = *(double *)(iVar2 + 0x34);
    }
    if (in_FPSCR_PR == '\0') {
      fVar7 = SUB84(dVar3,0) + (float)((ulonglong)dVar5 >> 0x20);
    }
    else {
      fVar7 = SUB84(dVar3 + dVar5,0);
    }
    if (in_FPSCR_SZ == '\0') {
      *(float *)(iVar2 + 0x28) = fVar7;
    }
    else {
      *(undefined8 *)(iVar2 + 0x28) = in_xd0;
    }
    FUN_8c017de8();
  }
  return;
}


/* Address: 8c017de8 */

/* WARNING: Removing unreachable block (ram,0x8c017e1a) */

void FUN_8c017de8(double param_1,int param_2)

{
  char cVar1;
  undefined *puVar2;
  undefined *puVar3;
  char *pcVar4;
  undefined1 *puVar5;
  double dVar6;
  undefined4 uVar7;
  undefined4 in_fr14;
  double in_xd2;
  undefined8 in_xd14;
  bool bVar8;
  undefined4 in_PR;
  char in_FPSCR_PR;
  char in_FPSCR_SZ;
  undefined1 local_14 [4];
  undefined1 local_10 [4];
  
  if (in_FPSCR_SZ == '\0') {
    puVar5 = local_10;
  }
  else {
    puVar5 = local_14;
  }
  *(undefined4 *)(puVar5 + -4) = in_PR;
  pcVar4 = *(char **)(param_2 + 0x20);
  if (*DAT_8c017ee8 == 0) {
    uVar7 = 0x3f800000;
    if (in_FPSCR_SZ == '\0') {
      dVar6 = (double)CONCAT44(*(undefined4 *)(pcVar4 + 8),DAT_8c017eec);
    }
    else {
      in_xd2 = (double)CONCAT44(PTR_DAT_8c017ef0,DAT_8c017eec);
      dVar6 = *(double *)(pcVar4 + 8);
    }
    if (in_FPSCR_PR == '\0') {
      dVar6 = (double)CONCAT44(-((float)((ulonglong)dVar6 >> 0x20) * SUB84(dVar6,0)),SUB84(dVar6,0))
      ;
    }
    else {
      dVar6 = -(dVar6 * dVar6);
    }
    if (in_FPSCR_SZ == '\0') {
      param_1 = (double)CONCAT44((int)((ulonglong)param_1 >> 0x20),(int)((ulonglong)dVar6 >> 0x20));
    }
    if (in_FPSCR_PR == '\0') {
      param_1 = (double)CONCAT44((int)((ulonglong)param_1 >> 0x20),SUB84(param_1,0) + 1.0);
    }
    else {
      param_1 = param_1 + (double)CONCAT44(in_fr14,0x3f800000);
    }
    dVar6 = (double)((ulonglong)dVar6 & 0xffffffff);
    if (in_FPSCR_PR == '\0') {
      bVar8 = SUB84(param_1,0) < 0.0;
    }
    else {
      bVar8 = param_1 < dVar6;
    }
    if (in_FPSCR_SZ == '\0') {
      dVar6 = (double)(ulonglong)*(uint *)(pcVar4 + 8);
    }
    else {
      in_xd2 = *(double *)(pcVar4 + 8);
    }
    if ((bVar8) && (in_FPSCR_SZ == '\0')) {
      param_1 = 0.0;
    }
    if (in_FPSCR_SZ == '\0') {
      in_xd2 = (double)CONCAT44(-SUB84(dVar6,0),SUB84(param_1,0));
    }
    if (in_FPSCR_PR == '\0') {
      dVar6 = (double)CONCAT44((float)((ulonglong)in_xd2 >> 0x20) + 1.0,SUB84(in_xd2,0));
    }
    else {
      dVar6 = in_xd2 + (double)CONCAT44(in_fr14,0x3f800000);
    }
    cVar1 = *pcVar4;
    *(undefined1 **)(puVar5 + -0x18) = puVar5 + -0x14;
    FUN_8c017ffa(dVar6,(int)cVar1);
    puVar3 = PTR_DAT_8c017ef0;
    if (*(short *)(pcVar4 + 0xc) != 0) {
      puVar3 = PTR_DAT_8c017ef4;
    }
    (*(code *)PTR_FUN_8c017ef8)(puVar5 + -0x14);
    puVar2 = PTR_FUN_8c017efc;
    (*(code *)PTR_FUN_8c017efc)(0,1);
    (*(code *)puVar2)(1,10);
    (*(code *)PTR_FUN_8c017f00)(0);
    (*(code *)PTR_FUN_8c017f04)(0,pcVar4 + 0x20);
    (*(code *)PTR_FUN_8c017f08)(puVar3,0,0x32);
    (*(code *)PTR_FUN_8c017f0c)(1);
    if (in_FPSCR_SZ == '\0') {
      *(undefined4 *)(puVar5 + -8) = uVar7;
      *(undefined4 *)(puVar5 + -0xc) = uVar7;
      *(undefined4 *)(puVar5 + -0x10) = uVar7;
      *(undefined4 *)(puVar5 + -0x14) = uVar7;
    }
    else {
      *(undefined8 *)(puVar5 + -8) = in_xd14;
      *(undefined8 *)(puVar5 + -0xc) = in_xd14;
      *(undefined8 *)(puVar5 + -0x10) = in_xd14;
      *(undefined8 *)(puVar5 + -0x14) = in_xd14;
    }
    (*(code *)PTR_FUN_8c017ef8)(puVar5 + -0x14);
    (*(code *)puVar2)(0,1);
    (*(code *)puVar2)(1);
  }
  return;
}


/* Address: 8c017ffa */

void FUN_8c017ffa(int *param_1,int *param_2,int param_3,int *param_4)

{
  int *piVar1;
  int *piVar2;
  undefined8 in_xd4;
  char in_FPSCR_SZ;
  int *local_10;
  int *piStack_c;
  int *local_8;
  int *local_4;
  
  piVar1 = param_4;
  local_10 = param_2;
  if (in_FPSCR_SZ == '\0') {
    local_10 = param_1;
  }
  local_4 = (int *)in_xd4;
  local_8 = local_4;
  param_4 = (int *)((ulonglong)in_xd4 >> 0x20);
  piVar2 = param_4;
  piStack_c = param_2;
  if (param_3 == 0) {
    if (in_FPSCR_SZ == '\0') {
      piStack_c = (int *)0x3f800000;
      param_4 = piVar1;
      local_4 = param_2;
    }
LAB_8c0180b2:
    if (in_FPSCR_SZ == '\0') {
      local_8 = param_2;
      piVar2 = local_4;
    }
  }
  else {
    if (param_3 == 1) {
      piStack_c = local_4;
      if (in_FPSCR_SZ == '\0') {
        piStack_c = param_2;
      }
LAB_8c0180a4:
      if (in_FPSCR_SZ == '\0') {
        param_4 = piVar1;
        local_4 = param_2;
      }
    }
    else {
      if (param_3 == 2) {
        if (in_FPSCR_SZ == '\0') {
          local_4 = (int *)0x3f800000;
          param_4 = piVar1;
        }
        else {
          param_4 = (int *)0x3f800000;
          piStack_c = local_4;
          local_4 = param_2;
        }
        goto LAB_8c0180b2;
      }
      if (param_3 == 3) {
        if (in_FPSCR_SZ == '\0') {
          local_4 = (int *)0x3f800000;
          param_4 = piVar1;
          piStack_c = local_4;
        }
        else {
          param_4 = (int *)0x3f800000;
          local_4 = param_2;
        }
      }
      else {
        if (param_3 == 4) {
          if (in_FPSCR_SZ == '\0') {
            piStack_c = (int *)0x3f800000;
          }
          goto LAB_8c0180a4;
        }
        if (param_3 == 5) {
          if (in_FPSCR_SZ == '\0') {
            piStack_c = (int *)0x3f800000;
            local_4 = (int *)0x3f800000;
            param_4 = piVar1;
          }
          else {
            param_4 = (int *)0x3f800000;
            local_4 = param_2;
          }
          goto LAB_8c0180b2;
        }
        if (param_3 == 6) {
          if (in_FPSCR_SZ == '\0') {
            local_4 = (int *)0x3f800000;
            param_4 = piVar1;
          }
          else {
            param_4 = (int *)0x3f800000;
            piStack_c = local_4;
            local_4 = param_2;
          }
        }
        else if (in_FPSCR_SZ == '\0') {
          piStack_c = DAT_8c018100;
          local_4 = DAT_8c018104;
          param_4 = piVar1;
        }
        else {
          piStack_c = DAT_8c018100;
          param_4 = piRam8c018108;
          local_4 = DAT_8c018104;
        }
      }
    }
    if (in_FPSCR_SZ == '\0') {
      local_8 = (int *)0x3f800000;
      piVar2 = local_4;
    }
    else {
      local_4 = (int *)0x3f800000;
      local_8 = param_2;
      piVar2 = local_4;
    }
  }
  local_4 = piVar2;
  *param_4 = (int)local_10;
  param_4[1] = (int)piStack_c;
  param_4[2] = (int)local_8;
  param_4[3] = (int)local_4;
  return;
}


/* Address: 8c019368 */

void FUN_8c019368(char param_1,char param_2)

{
  int *piVar1;
  
  piVar1 = DAT_8c0193e8;
  *DAT_8c0193e4 = (int)param_1;
  *piVar1 = (int)param_2;
  return;
}


/* Address: 8c019400 */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_8c019400(undefined4 param_1)

{
  undefined *puVar1;
  int *piVar2;
  undefined *puVar3;
  undefined4 *puVar4;
  int iVar5;
  undefined4 in_fr3;
  double dVar6;
  double dVar7;
  float fVar8;
  char in_FPSCR_PR;
  char in_FPSCR_SZ;
  undefined4 local_1c;
  undefined4 local_18;
  undefined4 local_14;
  
  piVar2 = DAT_8c019624;
  puVar1 = PTR_DAT_8c019620;
  if (in_FPSCR_SZ == '\0') {
    *(undefined4 *)(*(int *)PTR_DAT_8c019620 + 0x18) = *(undefined4 *)(*DAT_8c019624 + 0x20);
    in_fr3 = DAT_8c019628;
  }
  else {
    *(undefined8 *)(*(int *)PTR_DAT_8c019620 + 0x18) = *(undefined8 *)(*DAT_8c019624 + 0x20);
  }
  if (in_FPSCR_SZ == '\0') {
    dVar6 = (double)CONCAT44(*(undefined4 *)(*piVar2 + 0x24),in_fr3);
  }
  else {
    dVar6 = *(double *)(*piVar2 + 0x24);
  }
  if (in_FPSCR_PR == '\0') {
    dVar6 = (double)CONCAT44((float)((ulonglong)dVar6 >> 0x20) + SUB84(dVar6,0),SUB84(dVar6,0));
  }
  else {
    dVar6 = dVar6 + dVar6;
  }
  if (in_FPSCR_SZ == '\0') {
    *(int *)(*(int *)puVar1 + 0x1c) = (int)((ulonglong)dVar6 >> 0x20);
  }
  else {
    *(double *)(*(int *)puVar1 + 0x1c) = dVar6;
  }
  if (in_FPSCR_SZ == '\0') {
    *(undefined4 *)(*(int *)puVar1 + 0x20) = *(undefined4 *)(*piVar2 + 0x28);
  }
  else {
    *(undefined8 *)(*(int *)puVar1 + 0x20) = *(undefined8 *)(*piVar2 + 0x28);
  }
  *(undefined4 *)(*(int *)puVar1 + 0x24) = 0;
  *(undefined4 *)(*(int *)puVar1 + 0x28) = 0;
  *(undefined4 *)(*(int *)puVar1 + 0x2c) = 0;
  puVar3 = PTR_DAT_8c019630;
  if (in_FPSCR_SZ == '\0') {
    *(undefined4 *)(*(int *)puVar1 + 8) = _DAT_8c01962c;
  }
  else {
    *(ulonglong *)(*(int *)puVar1 + 8) = CONCAT44(PTR_DAT_8c019630,_DAT_8c01962c);
  }
  local_1c = *(undefined4 *)puVar3;
  local_18 = *(undefined4 *)(puVar3 + 4);
  local_14 = *(undefined4 *)(puVar3 + 8);
  (*(code *)PTR_FUN_8c019638)(PTR_DAT_8c019634);
  iVar5 = *(int *)(*piVar2 + 0x1c);
  if (iVar5 != 0) {
    (*(code *)PTR_FUN_8c01963c)(0,iVar5,*piVar2);
  }
  iVar5 = *(int *)(*piVar2 + 0x14);
  if (iVar5 != 0) {
    (*(code *)PTR_FUN_8c019640)(0,iVar5,*piVar2);
  }
  iVar5 = 0x4000 - *(int *)(*piVar2 + 0x18);
  if (iVar5 != 0) {
    (*(code *)PTR_FUN_8c019644)(0,iVar5,iVar5);
  }
  (*(code *)PTR_FUN_8c019648)(0,&local_1c,&local_1c);
  (*(code *)PTR_FUN_8c01964c)(1);
  iVar5 = *(int *)puVar1;
  if (in_FPSCR_SZ == '\0') {
    dVar6 = (double)CONCAT44(_DAT_8c019650,param_1);
    dVar7 = (double)CONCAT44(*(undefined4 *)(iVar5 + 0x18),local_1c);
  }
  else {
    dVar6 = (double)CONCAT44(PTR_DAT_8c019654,_DAT_8c019650);
    dVar7 = *(double *)(iVar5 + 0x18);
  }
  fVar8 = (float)((ulonglong)dVar6 >> 0x20);
  if (in_FPSCR_PR == '\0') {
    dVar7 = (double)CONCAT44((float)((ulonglong)dVar7 >> 0x20) + SUB84(dVar7,0) + fVar8,
                             SUB84(dVar7,0));
  }
  else {
    dVar7 = dVar7 + dVar7 + dVar6;
  }
  if (in_FPSCR_SZ == '\0') {
    *(int *)(iVar5 + 0xc) = (int)((ulonglong)dVar7 >> 0x20);
  }
  else {
    *(double *)(iVar5 + 0xc) = dVar7;
  }
  iVar5 = *(int *)puVar1;
  if (in_FPSCR_SZ == '\0') {
    dVar7 = (double)CONCAT44(*(undefined4 *)(iVar5 + 0x1c),local_18);
  }
  else {
    dVar7 = *(double *)(iVar5 + 0x1c);
  }
  if (in_FPSCR_PR == '\0') {
    dVar7 = (double)CONCAT44((float)((ulonglong)dVar7 >> 0x20) + SUB84(dVar7,0),SUB84(dVar7,0));
  }
  else {
    dVar7 = dVar7 + dVar7;
  }
  if (in_FPSCR_SZ == '\0') {
    *(int *)(iVar5 + 0x10) = (int)((ulonglong)dVar7 >> 0x20);
  }
  else {
    *(double *)(iVar5 + 0x10) = dVar7;
  }
  iVar5 = *(int *)puVar1;
  if (in_FPSCR_SZ == '\0') {
    dVar7 = (double)CONCAT44(*(undefined4 *)(iVar5 + 0x20),local_14);
  }
  else {
    dVar7 = *(double *)(iVar5 + 0x20);
  }
  if (in_FPSCR_PR == '\0') {
    dVar6 = (double)CONCAT44((float)((ulonglong)dVar7 >> 0x20) + SUB84(dVar7,0) + fVar8,
                             SUB84(dVar7,0));
  }
  else {
    dVar6 = dVar7 + dVar7 + dVar6;
  }
  if (in_FPSCR_SZ == '\0') {
    *(int *)(iVar5 + 0x14) = (int)((ulonglong)dVar6 >> 0x20);
  }
  else {
    *(double *)(iVar5 + 0x14) = dVar6;
  }
  (*(code *)PTR_FUN_8c019658)(*(undefined4 *)PTR_DAT_8c019654);
  puVar4 = DAT_8c019660;
  *DAT_8c01965c = 0;
  *puVar4 = 0;
  return;
}


/* Address: 8c01951e */

void FUN_8c01951e(void)

{
  undefined *puVar1;
  undefined4 *puVar2;
  int *piVar3;
  
  piVar3 = DAT_8c01966c;
  puVar2 = DAT_8c019668;
  puVar1 = PTR_DAT_8c019664;
  *(undefined1 *)(*(int *)PTR_DAT_8c019664 + 8) = 0;
  *(undefined2 *)(*(int *)puVar1 + 4) = 0;
  *(undefined1 *)(*(int *)puVar1 + 6) = *(undefined1 *)*puVar2;
  *(undefined4 *)(*(int *)puVar1 + 0xc) =
       *(undefined4 *)(*(char *)(*(int *)puVar1 + 6) * 0x10 + *piVar3);
  *(undefined1 *)(*(int *)puVar1 + 9) = 0;
  *(undefined4 *)(*(int *)puVar1 + 0x10) = *(undefined4 *)(*DAT_8c019670 + 4);
  *(undefined4 *)(*(int *)PTR_DAT_8c019674 + 0x14) = 0;
  *(undefined1 *)(*(int *)puVar1 + 7) =
       *(undefined1 *)(*(char *)(*(int *)puVar1 + 6) * 0x10 + *piVar3 + 5);
  **(undefined4 **)puVar1 = 1;
  *(undefined1 *)(*(int *)puVar1 + 0xb) = 2;
  return;
}


/* Address: 8c01957e */

/* WARNING: Removing unreachable block (ram,0x8c0195a8) */

void FUN_8c01957e(void)

{
  undefined4 *puVar1;
  byte *pbVar2;
  ushort *puVar3;
  undefined *puVar4;
  undefined2 *puVar5;
  undefined4 *puVar6;
  undefined2 uVar7;
  int iVar8;
  int *piVar9;
  
  iVar8 = (*(code *)PTR_FUN_8c01967c)(2,0,PTR_LAB_8c019678);
  pbVar2 = DAT_8c019680;
  uVar7 = (*(code *)PTR_FUN_8c019684)();
  puVar4 = PTR_DAT_8c01968c;
  puVar3 = DAT_8c019688;
  *(undefined2 *)pbVar2 = uVar7;
  puVar1 = DAT_8c01966c;
  *puVar3 = (ushort)*pbVar2;
  puVar5 = DAT_8c019690;
  *(short *)pbVar2 = (short)(char)((ushort)*(undefined2 *)pbVar2 >> 8);
  *puVar5 = *(undefined2 *)puVar4;
  puVar5 = DAT_8c019694;
  *puVar1 = *(undefined4 *)(puVar4 + 4);
  *puVar5 = *(undefined2 *)(puVar4 + 2);
  *DAT_8c019670 = *(undefined4 *)(puVar4 + 8);
  piVar9 = (int *)(PTR_DAT_8c019698 + (short)*puVar3 * 4 + *(short *)pbVar2 * 0x20);
  *DAT_8c01969c = *piVar9;
  *DAT_8c019668 = *piVar9 + 0x40;
  FUN_8c01951e();
  puVar4 = PTR_LAB_8c0196a4;
  *(undefined **)(iVar8 + 0x18) = PTR_LAB_8c0196a0;
  puVar1 = DAT_8c0196a8;
  *(undefined **)(iVar8 + 0x14) = puVar4;
  puVar6 = DAT_8c0196ac;
  *puVar1 = 0;
  puVar1 = DAT_8c0196b4;
  *puVar6 = 0xffffffff;
  puVar4 = PTR_FUN_8c0196b8;
  *DAT_8c0196b0 = 0xffffffff;
  *puVar1 = 0x31c7;
  (*(code *)puVar4)(0x31c7);
  return;
}


/* Address: 8c01976c */

void FUN_8c01976c(short param_1)

{
  int *piVar1;
  short *psVar2;
  short *psVar3;
  undefined4 *puVar4;
  int iVar5;
  int *piVar6;
  
  iVar5 = (*(code *)PTR_FUN_8c019844)();
  psVar3 = DAT_8c01984c;
  psVar2 = DAT_8c019848;
  piVar1 = DAT_8c019820;
  if (iVar5 != 0) {
    *DAT_8c019848 = *DAT_8c019848 + param_1;
    piVar6 = (int *)(PTR_DAT_8c019850 + *psVar2 * 4 + *psVar3 * 0x20);
    *piVar1 = *piVar6;
    *DAT_8c019824 = *piVar6 + 0x40;
    FUN_8c01951e();
    puVar4 = DAT_8c019858;
    *DAT_8c019854 = 0;
    *puVar4 = 0;
  }
  return;
}


/* Address: 8c01995e */

/* WARNING: Removing unreachable block (ram,0x8c019996) */

void FUN_8c01995e(void)

{
  char cVar1;
  char cVar2;
  undefined *puVar3;
  int *piVar4;
  uint uVar5;
  int iVar6;
  int iVar7;
  char *pcVar8;
  
  if (((*(int *)PTR_DAT_8c019a68 != 0) &&
      (FUN_8c019adc(&DAT_8c6b706c), puVar3 = PTR_DAT_8c019a44,
      *(char *)(*(int *)PTR_DAT_8c019a44 + 6) != '@')) &&
     (uVar5 = (*(code *)PTR_FUN_8c019a6c)(),
     *(int *)(PTR_DAT_8c019a70 + (uVar5 & 0xff) * 4 + ((int)(uVar5 & 0xffff) >> 8) * 0x20) != 0)) {
LAB_8c0199b2:
    piVar4 = DAT_8c019a7c;
    cVar1 = *(char *)(*(int *)puVar3 + 8);
    iVar7 = *(int *)*DAT_8c019a74 + -1;
    pcVar8 = (char *)((*(int *)*DAT_8c019a74 + -1) * 0x40 + *DAT_8c019a78);
    if (0 < (short)iVar7) {
      do {
        if (*(char *)(*(int *)puVar3 + 8) <= *(char *)(*pcVar8 * 0x10 + *piVar4 + 4)) {
          cVar2 = *(char *)(*(int *)puVar3 + 8);
          iVar6 = *piVar4;
          if (cVar2 == '\0') {
            if (*(char *)(*pcVar8 * 0x10 + iVar6 + 4) != '\x04') {
              FUN_8c019be6(iVar7,pcVar8);
            }
          }
          else if ((cVar2 == '\x01') || (cVar2 == '\x03')) {
            if (*(char *)(*pcVar8 * 0x10 + iVar6 + 4) != '\x04') {
              FUN_8c019c50(iVar7,pcVar8);
            }
          }
          else if (cVar2 == '\x02') {
            FUN_8c019dc4(iVar7,pcVar8);
          }
          if (*(char *)(*(int *)puVar3 + 8) < cVar1) goto LAB_8c0199b2;
        }
        iVar7 = iVar7 + -1;
        pcVar8 = pcVar8 + -0x40;
        if ((short)iVar7 < 1) {
          return;
        }
      } while( true );
    }
  }
  return;
}


/* Address: 8c019adc */

void FUN_8c019adc(void)

{
  char cVar1;
  uint *puVar2;
  int iVar3;
  undefined *puVar4;
  int iVar5;
  
  puVar4 = PTR_DAT_8c019bc8;
  iVar3 = DAT_8c019bc4;
  puVar2 = DAT_8c019bc0;
  *DAT_8c019bc0 = *DAT_8c019bc0 + 1;
  iVar5 = *(int *)puVar4;
  if ((*(uint *)(iVar3 + 8) & 0xf0) == 0) {
    if ((*(char *)(iVar5 + 6) == '@') &&
       ((((*(uint *)(iVar3 + 8) & 0x606) != 0 || (*(short *)(iVar3 + 0x1c) != 0)) ||
        ((*(short *)(iVar3 + 0x1e) != 0 || (*DAT_8c019bcc == 0)))))) {
      (*(code *)PTR_FUN_8c019bd0)();
      (*(code *)PTR_FUN_8c019bd4)(5);
      return;
    }
  }
  else if (((*(char *)(iVar5 + 6) == '@') || (*(char *)(iVar5 + 8) == '\x02')) ||
          (*DAT_8c019bcc == 0)) {
    if ((*(char *)(iVar5 + 6) == '@') &&
       ((((*(uint *)(iVar3 + 8) & 0x606) != 0 || (*(short *)(iVar3 + 0x1c) != 0)) ||
        ((*(short *)(iVar3 + 0x1e) != 0 || (*DAT_8c019bcc == 0)))))) {
      (*(code *)PTR_FUN_8c019bd0)();
      (*(code *)PTR_FUN_8c019bd4)(5);
      return;
    }
  }
  else {
    cVar1 = *(char *)(iVar5 + 6);
    if (((cVar1 != '\x17') && (cVar1 != '\x18')) &&
       ((cVar1 != '\x19' &&
        ((((cVar1 != '\x1c' && (cVar1 != '\x1d')) && (cVar1 != '\x1e')) &&
         ((0x1e < *puVar2 && (*(char *)(iVar5 + 8) != '\x04')))))))) {
      (*(code *)PTR_FUN_8c019bd8)(0x40,5);
      return;
    }
  }
  return;
}


/* Address: 8c019be6 */

void FUN_8c019be6(short param_1,char *param_2)

{
  char cVar1;
  int iVar2;
  
  iVar2 = 0;
  if ((*(short *)(*(int *)PTR_DAT_8c019ca0 + 4) != param_1) && (*DAT_8c019ca4 != 0)) {
    cVar1 = param_2[3];
    if (cVar1 == '\0') {
      iVar2 = FUN_8c01a1ac(param_2);
    }
    else if (cVar1 == '\x01') {
      iVar2 = FUN_8c01a240(param_2);
    }
    else if (cVar1 == '\x02') {
      iVar2 = FUN_8c01a480(param_2);
    }
    if (iVar2 != 0) {
      FUN_8c01a7d6((int)*param_2,(int)param_1);
      (*(code *)PTR_FUN_8c019cac)(8,6,PTR_LAB_8c019ca8);
      return;
    }
  }
  return;
}


/* Address: 8c019c50 */

void FUN_8c019c50(undefined4 param_1,char *param_2)

{
  undefined *puVar1;
  int iVar2;
  
  puVar1 = PTR_FUN_8c019cac;
  iVar2 = *(int *)PTR_DAT_8c019ca0;
  if (*(short *)(iVar2 + 4) == (short)param_1) {
    if (*DAT_8c019ca4 != 0) {
      if (param_2[3] == '\0') {
        iVar2 = FUN_8c01a1ac(param_2,&DAT_8c11204c);
      }
      else if (param_2[3] == '\x02') {
        iVar2 = FUN_8c01a480(param_2);
      }
      else {
        iVar2 = 0;
      }
      if (iVar2 == 0) {
        FUN_8c01a9b8();
        (*(code *)puVar1)(8,6,PTR_LAB_8c019d80);
      }
    }
  }
  else if (*(char *)(iVar2 + 8) < *(char *)(*param_2 * 0x10 + *DAT_8c019d84 + 4)) {
    if (param_2[3] == '\0') {
      iVar2 = FUN_8c01a1ac(param_2,&DAT_8c11204c);
    }
    else if (param_2[3] == '\x02') {
      iVar2 = FUN_8c01a480(param_2);
    }
    else {
      iVar2 = 0;
    }
    if (iVar2 != 0) {
      FUN_8c01a7d6((int)*param_2,param_1);
      (*(code *)puVar1)(8,6,PTR_LAB_8c019d80);
    }
  }
  else if (((*(char *)(*DAT_8c019d84 + *param_2 * 0x10 + 4) == *(char *)(iVar2 + 8)) &&
           (*DAT_8c019ca4 != 0)) &&
          (*(char *)(*(short *)(iVar2 + 4) * 0x40 + *DAT_8c019d88 + 1) < param_2[1])) {
    if (param_2[3] == '\0') {
      iVar2 = FUN_8c01a1ac(param_2,&DAT_8c11204c);
    }
    else if (param_2[3] == '\x02') {
      iVar2 = FUN_8c01a480(param_2);
    }
    else {
      iVar2 = 0;
    }
    if (iVar2 != 0) {
      FUN_8c01a9b8();
      FUN_8c01a7d6((int)*param_2,param_1);
      (*(code *)puVar1)(8,6,PTR_LAB_8c019f20);
    }
  }
  return;
}


/* Address: 8c019dc4 */

void FUN_8c019dc4(short param_1,char *param_2)

{
  undefined *puVar1;
  undefined *puVar2;
  int *piVar3;
  int iVar4;
  int iVar5;
  
  iVar5 = 0;
  if (*(char *)(*param_2 * 0x10 + *DAT_8c019f24 + 4) == '\x04') {
    if (param_2[3] == '\x01') {
      iVar5 = FUN_8c01a240(param_2);
    }
    piVar3 = DAT_8c019f2c;
    puVar2 = PTR_DAT_8c019f28;
    if (iVar5 != 0) {
      iVar5 = 2;
      iVar4 = *(int *)(*(int *)PTR_DAT_8c019f28 + 0x20);
      while (iVar4 != 0) {
        iVar5 = *(int *)(iVar5 * 4 + *(int *)PTR_DAT_8c019f28 + 0x18);
        iVar4 = *(int *)(iVar5 * 4 + *(int *)PTR_DAT_8c019f28 + 0x18);
      }
      iVar4 = param_1 * 0x40;
      if (*(char *)(*DAT_8c019f2c + iVar4) != '=') {
        *(undefined1 *)(*(int *)PTR_DAT_8c019f28 + 0x74 + iVar5) =
             *(undefined1 *)(*DAT_8c019f2c + iVar4);
      }
      *(undefined1 *)(*(int *)puVar2 + 0x7a + iVar5) = *(undefined1 *)(*piVar3 + iVar4 + 2);
      if (*(char *)(*piVar3 + iVar4) != '=') {
        *(undefined4 *)(*(int *)puVar2 + 0x44 + iVar5 * 4) =
             *(undefined4 *)(*param_2 * 0x10 + *DAT_8c019f24);
      }
      puVar1 = PTR_LAB_8c019f20;
      *(undefined4 *)(iVar5 * 4 + *(int *)puVar2 + 0x5c) =
           *(undefined4 *)((uint)*(byte *)(*piVar3 + iVar4 + 2) * 8 + *DAT_8c019f30 + 4);
      (*(code *)PTR_FUN_8c019f34)(8,6,puVar1);
      return;
    }
  }
  else if ((*DAT_8c019f38 != 0) &&
          (*(char *)(*(short *)(*(int *)PTR_DAT_8c019f28 + 4) * 0x40 + *DAT_8c019f2c + 1) <
           param_2[1])) {
    if (param_2[3] == '\0') {
      iVar5 = FUN_8c01a1ac(param_2);
    }
    else if (param_2[3] == '\x02') {
      iVar5 = FUN_8c01a480(param_2);
    }
    else {
      iVar5 = 0;
    }
    if (iVar5 != 0) {
      (*(code *)PTR_FUN_8c019f3c)();
      FUN_8c01a7d6((int)*param_2,(int)param_1);
      (*(code *)PTR_FUN_8c019f34)(8,6,PTR_LAB_8c019f20);
      return;
    }
  }
  return;
}


/* Address: 8c019f4a */

void FUN_8c019f4a(undefined4 *param_1)

{
  char cVar1;
  undefined *puVar2;
  undefined4 *puVar3;
  undefined *puVar4;
  undefined4 *puVar5;
  undefined4 uVar6;
  int iVar7;
  undefined4 in_fr3;
  undefined8 in_xd2;
  char in_FPSCR_SZ;
  
  puVar3 = DAT_8c01a0c4;
  puVar2 = PTR_DAT_8c01a0c0;
  if (*(int *)(*(int *)PTR_DAT_8c01a0c0 + 0xc) != 0) {
    uVar6 = param_1[1];
    *DAT_8c01a0c4 = *param_1;
    puVar3[1] = uVar6;
    uVar6 = param_1[3];
    puVar3[2] = param_1[2];
    puVar3[3] = uVar6;
    uVar6 = param_1[5];
    puVar3[4] = param_1[4];
    puVar3[5] = uVar6;
    uVar6 = param_1[7];
    puVar3[6] = param_1[6];
    puVar3[7] = uVar6;
    uVar6 = param_1[9];
    puVar3[8] = param_1[8];
    puVar3[9] = uVar6;
    uVar6 = param_1[0xb];
    puVar3[10] = param_1[10];
    puVar3[0xb] = uVar6;
    uVar6 = param_1[0xd];
    puVar3[0xc] = param_1[0xc];
    puVar3[0xd] = uVar6;
    uVar6 = param_1[0xf];
    puVar3[0xe] = param_1[0xe];
    puVar4 = PTR_DAT_8c01a0c8;
    puVar3[0xf] = uVar6;
    if (in_FPSCR_SZ == '\0') {
      in_fr3 = param_1[8];
    }
    else {
      in_xd2 = *(undefined8 *)(param_1 + 8);
    }
    if (in_FPSCR_SZ == '\0') {
      *(undefined4 *)(*(int *)puVar4 + 0xc) = in_fr3;
      in_fr3 = param_1[9];
    }
    else {
      *(undefined8 *)(*(int *)puVar4 + 0xc) = in_xd2;
      in_xd2 = *(undefined8 *)(param_1 + 9);
    }
    if (in_FPSCR_SZ == '\0') {
      *(undefined4 *)(*(int *)puVar4 + 0x10) = in_fr3;
      in_fr3 = param_1[10];
    }
    else {
      *(undefined8 *)(*(int *)puVar4 + 0x10) = in_xd2;
      in_xd2 = *(undefined8 *)(param_1 + 10);
    }
    if (in_FPSCR_SZ == '\0') {
      *(undefined4 *)(*(int *)puVar4 + 0x14) = in_fr3;
    }
    else {
      *(undefined8 *)(*(int *)puVar4 + 0x14) = in_xd2;
    }
    puVar5 = DAT_8c01a0d4;
    puVar3 = DAT_8c01a0cc;
    iVar7 = *(int *)puVar2;
    if (*(char *)(iVar7 + 8) == '\x05') {
      (*(code *)PTR_FUN_8c01a0d0)(DAT_8c01a0cc);
      (**(code **)(*(int *)puVar2 + 0xc))(DAT_8c01a0cc);
    }
    else if ((*(byte *)(iVar7 + 0xb) & 2) == 0) {
      (*(code *)PTR_FUN_8c01a0dc)(*DAT_8c01a0d8 + *(short *)(iVar7 + 4) * 0x40,&DAT_8c6b706c);
      (**(code **)(*(int *)puVar2 + 0xc))(DAT_8c01a0cc);
    }
    else {
      uVar6 = DAT_8c01a0d4[1];
      *DAT_8c01a0cc = *DAT_8c01a0d4;
      puVar3[1] = uVar6;
      uVar6 = puVar5[3];
      puVar3[2] = puVar5[2];
      puVar3[3] = uVar6;
      uVar6 = puVar5[5];
      puVar3[4] = puVar5[4];
      puVar3[5] = uVar6;
      uVar6 = puVar5[7];
      puVar3[6] = puVar5[6];
      puVar3[7] = uVar6;
      puVar3[8] = puVar5[8];
      puVar3[8] = *(undefined4 *)(*(int *)puVar2 + 0x14);
      (**(code **)(*(int *)puVar2 + 0xc))();
    }
    cVar1 = *(char *)(*(int *)puVar2 + 7);
    if (cVar1 == '\x03') {
      FUN_8c01a60c(param_1);
    }
    else if (cVar1 == '\x02') {
      FUN_8c01a680(param_1);
    }
    else if (cVar1 == '\x01') {
      FUN_8c01a718(param_1);
    }
    iVar7 = *(int *)puVar2;
    if (((*(int *)(iVar7 + 0x14) != 0) || ((*(byte *)(iVar7 + 0xb) & 1) == 0)) &&
       (0x3c < *DAT_8c01a0e0)) {
      (**(code **)(iVar7 + 0x10))
                (param_1,*(undefined4 *)PTR_DAT_8c01a0e8,*(undefined4 *)PTR_DAT_8c01a0e4);
      *(int *)(*(int *)PTR_DAT_8c01a0e4 + 0x14) = *(int *)(*(int *)PTR_DAT_8c01a0e4 + 0x14) + 1;
    }
  }
  return;
}


/* Address: 8c01a100 */

/* WARNING: Removing unreachable block (ram,0x8c01a12c) */

void FUN_8c01a100(double param_1,undefined4 param_2,int param_3)

{
  undefined *puVar1;
  uint uVar2;
  int iVar3;
  float extraout_fr3;
  float fVar4;
  undefined4 extraout_fr2;
  float fVar5;
  undefined8 uVar6;
  double dVar7;
  ulonglong uVar8;
  double dVar9;
  char in_FPSCR_PR;
  char in_FPSCR_SZ;
  
  if (*DAT_8c01a200 == 0) {
    (*(code *)PTR_FUN_8c01a21c)(DAT_8c01a218,*(undefined4 *)(param_3 + 0x1c));
    (*(code *)PTR_FUN_8c01a220)(DAT_8c01a218,*(undefined4 *)(param_3 + 0x14));
    (*(code *)PTR_FUN_8c01a224)(DAT_8c01a218,*(undefined4 *)(param_3 + 0x18));
    puVar1 = PTR_DAT_8c01a228;
    iVar3 = *(int *)PTR_DAT_8c01a228;
    if (in_FPSCR_SZ == '\0') {
      fVar4 = *(float *)(iVar3 + 0x9c);
      dVar9 = (double)CONCAT44(*(undefined4 *)(param_3 + 0x28),param_2);
    }
    else {
      dVar9 = *(double *)(param_3 + 0x28);
      fVar4 = extraout_fr3;
    }
    if (in_FPSCR_PR == '\0') {
      dVar9 = (double)CONCAT44((float)((ulonglong)dVar9 >> 0x20) + fVar4,SUB84(dVar9,0));
    }
    else {
      dVar9 = dVar9 + (double)CONCAT44(extraout_fr2,fVar4);
    }
    if (in_FPSCR_SZ == '\0') {
      fVar4 = *(float *)(iVar3 + 0x98);
      param_1 = (double)CONCAT44((int)((ulonglong)param_1 >> 0x20),*(undefined4 *)(param_3 + 0x24));
    }
    if (in_FPSCR_PR == '\0') {
      fVar5 = SUB84(param_1,0) + fVar4;
    }
    else {
      fVar5 = SUB84(param_1 + (double)CONCAT44(extraout_fr2,fVar4),0);
    }
    if (in_FPSCR_SZ == '\0') {
      fVar4 = *(float *)(iVar3 + 0x94);
      dVar7 = (double)CONCAT44(*(undefined4 *)(param_3 + 0x20),fVar5);
    }
    else {
      dVar7 = *(double *)(param_3 + 0x20);
    }
    if (in_FPSCR_PR == '\0') {
      uVar2 = SUB84(dVar7,0);
    }
    else {
      uVar2 = SUB84(dVar7 + (double)CONCAT44(extraout_fr2,fVar4),0);
    }
    (*(code *)PTR_FUN_8c01a22c)(dVar9,DAT_8c01a218);
    (*(code *)PTR_FUN_8c01a230)(DAT_8c01a218);
    uVar8 = (ulonglong)uVar2;
    if (in_FPSCR_SZ == '\0') {
      *(undefined4 *)(*(int *)puVar1 + 0x94) = 0;
    }
    else {
      *(ulonglong *)(*(int *)puVar1 + 0x94) = uVar8;
    }
    if (in_FPSCR_SZ == '\0') {
      *(undefined4 *)(*(int *)puVar1 + 0x98) = 0;
    }
    else {
      *(ulonglong *)(*(int *)puVar1 + 0x98) = uVar8;
    }
    if (in_FPSCR_SZ == '\0') {
      *(undefined4 *)(*(int *)puVar1 + 0x9c) = 0;
    }
    else {
      *(ulonglong *)(*(int *)puVar1 + 0x9c) = uVar8;
    }
  }
  else {
    if (in_FPSCR_SZ == '\0') {
      uVar6 = CONCAT44(*(undefined4 *)DAT_8c01a204,SUB84(param_1,0));
    }
    else {
      uVar6 = *DAT_8c01a204;
    }
    (*(code *)PTR_FUN_8c01a20c)(uVar6,*DAT_8c01a208);
    uVar2 = (*(code *)PTR_FUN_8c01a210)();
    if ((uVar2 & 0xffff) >> 8 == 0x20) {
      *(undefined4 *)(*(int *)PTR_DAT_8c01a214 + 0x18) = 0;
    }
  }
                    /* WARNING: Could not recover jumptable at 0x8c01a1a8. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  (*DAT_8c01a234)();
  return;
}


/* Address: 8c01a1ac */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

bool FUN_8c01a1ac(undefined4 param_1,undefined4 param_2,int param_3)

{
  int iVar1;
  undefined4 in_fr1;
  double dVar2;
  double in_dr2;
  double dVar3;
  undefined4 uVar5;
  double dVar4;
  float fVar6;
  double dVar7;
  undefined4 uVar8;
  double dVar9;
  float fVar10;
  double in_xd4;
  bool bVar11;
  char in_FPSCR_PR;
  char in_FPSCR_SZ;
  
  if (in_FPSCR_SZ == '\0') {
    in_dr2 = (double)CONCAT44((int)((ulonglong)in_dr2 >> 0x20),*(undefined4 *)(param_3 + 8));
  }
  iVar1 = *DAT_8c01a238;
  if (in_FPSCR_SZ == '\0') {
    dVar7 = (double)CONCAT44(*(undefined4 *)(iVar1 + 0x20),param_1);
  }
  else {
    dVar7 = *(double *)(iVar1 + 0x20);
  }
  if (in_FPSCR_PR == '\0') {
    dVar7 = (double)CONCAT44((float)((ulonglong)dVar7 >> 0x20) - SUB84(in_dr2,0),SUB84(dVar7,0));
  }
  else {
    dVar7 = dVar7 - in_dr2;
  }
  if (in_FPSCR_SZ == '\0') {
    dVar3 = (double)CONCAT44(*(undefined4 *)(iVar1 + 0x24),*(undefined4 *)(param_3 + 0xc));
  }
  else {
    dVar3 = *(double *)(iVar1 + 0x24);
  }
  if (in_FPSCR_PR == '\0') {
    dVar3 = (double)CONCAT44((float)((ulonglong)dVar3 >> 0x20) - SUB84(dVar3,0),SUB84(dVar3,0));
  }
  else {
    dVar3 = dVar3 - dVar3;
  }
  uVar8 = (undefined4)((ulonglong)dVar7 >> 0x20);
  dVar2 = dVar7;
  dVar9 = dVar3;
  if (in_FPSCR_SZ == '\0') {
    uVar5 = (undefined4)((ulonglong)dVar3 >> 0x20);
    dVar7 = (double)CONCAT44(uVar8,uVar5);
    dVar3 = in_xd4;
    dVar2 = (double)CONCAT44(uVar8,in_fr1);
    dVar9 = (double)CONCAT44(uVar5,DAT_8c01a23c);
  }
  if (in_FPSCR_PR == '\0') {
    dVar7 = (double)CONCAT44((int)((ulonglong)dVar7 >> 0x20),SUB84(dVar7,0) + SUB84(dVar9,0));
  }
  else {
    dVar7 = dVar7 + dVar9;
  }
  if (in_FPSCR_SZ == '\0') {
    dVar4 = (double)CONCAT44(*(undefined4 *)(param_3 + 0x10),SUB84(dVar9,0));
    dVar9 = (double)CONCAT44(*(undefined4 *)(iVar1 + 0x28),param_2);
    dVar2 = (double)CONCAT44((int)((ulonglong)dVar2 >> 0x20),*(undefined4 *)(param_3 + 0x1c));
  }
  else {
    dVar4 = *(double *)(param_3 + 0x10);
    dVar9 = *(double *)(iVar1 + 0x28);
  }
  if (in_FPSCR_PR == '\0') {
    dVar9 = (double)CONCAT44((float)((ulonglong)dVar9 >> 0x20) - (float)((ulonglong)dVar4 >> 0x20),
                             SUB84(dVar9,0));
  }
  else {
    dVar9 = dVar9 - dVar4;
  }
  fVar6 = SUB84(dVar7,0);
  if (in_FPSCR_SZ == '\0') {
    dVar3 = (double)CONCAT44(fVar6,SUB84(dVar4,0));
  }
  if (in_FPSCR_PR == '\0') {
    dVar3 = (double)CONCAT44((float)((ulonglong)dVar2 >> 0x20) * (float)((ulonglong)dVar7 >> 0x20) +
                             (float)((ulonglong)dVar3 >> 0x20) * fVar6,SUB84(dVar3,0));
  }
  else {
    dVar3 = dVar3 * dVar7;
  }
  fVar10 = (float)((ulonglong)dVar9 >> 0x20);
  if (in_FPSCR_SZ == '\0') {
    dVar9 = (double)CONCAT44(fVar10,SUB84(dVar2,0));
  }
  if (in_FPSCR_PR == '\0') {
    dVar3 = (double)CONCAT44((float)((ulonglong)dVar9 >> 0x20) * fVar10 +
                             (float)((ulonglong)dVar3 >> 0x20),SUB84(dVar3,0));
  }
  if (in_FPSCR_SZ == '\0') {
    dVar3 = (double)CONCAT44((int)((ulonglong)dVar3 >> 0x20),fVar6);
  }
  if (in_FPSCR_PR == '\0') {
    bVar11 = (float)((ulonglong)dVar3 >> 0x20) < SUB84(dVar9,0);
  }
  else {
    bVar11 = dVar3 < dVar9;
  }
  return bVar11;
}


/* Address: 8c01a240 */

/* WARNING: Removing unreachable block (ram,0x8c01a3f4) */

undefined4 FUN_8c01a240(undefined4 param_1,undefined4 param_2,int param_3)

{
  uint uVar1;
  int iVar2;
  ulonglong *puVar3;
  double in_dr2;
  double dVar4;
  double dVar5;
  float fVar8;
  longlong lVar6;
  undefined8 uVar7;
  double dVar9;
  double dVar10;
  float fVar11;
  undefined4 uVar12;
  undefined8 in_dr12;
  double dVar13;
  float fVar14;
  ulonglong in_dr14;
  double dVar15;
  float fVar16;
  double in_xd4;
  double in_xd12;
  undefined8 in_xd14;
  bool bVar17;
  undefined4 in_PR;
  char in_FPSCR_PR;
  char in_FPSCR_SZ;
  undefined1 auStack_24 [16];
  ulonglong uStack_14;
  undefined8 uStack_c;
  
  if (in_FPSCR_SZ == '\0') {
    uStack_c = CONCAT44((int)in_dr14,(undefined4)uStack_c);
                    /* WARNING: Ignoring partial resolution of indirect */
    uStack_c._0_4_ = (int)(in_dr14 >> 0x20);
    puVar3 = &uStack_14;
    uStack_14 = CONCAT44((int)in_dr12,(int)((ulonglong)in_dr12 >> 0x20));
  }
  else {
    puVar3 = (ulonglong *)auStack_24;
    uStack_c = in_xd14;
    uStack_14 = in_dr14;
  }
  *(undefined4 *)((int)puVar3 + -4) = in_PR;
  if (in_FPSCR_SZ == '\0') {
    uVar12 = *(undefined4 *)(param_3 + 8);
    in_dr2 = (double)CONCAT44((int)((ulonglong)in_dr2 >> 0x20),*(undefined4 *)(param_3 + 0xc));
    *(undefined4 *)((int)puVar3 + -0x90) = *(undefined4 *)(param_3 + 0xc);
    dVar13 = (double)CONCAT44(*(undefined4 *)(param_3 + 0x10),uVar12);
    in_dr14 = (ulonglong)*(uint *)(param_3 + 0x14);
  }
  else {
    in_xd12 = *(double *)(param_3 + 8);
    *(undefined8 *)((int)puVar3 + -0x90) = *(undefined8 *)(param_3 + 0xc);
    dVar13 = *(double *)(param_3 + 0x10);
    in_xd14 = *(undefined8 *)(param_3 + 0x14);
  }
  fVar8 = (float)((ulonglong)in_xd14 >> 0x20);
  iVar2 = *DAT_8c01a440;
  if (in_FPSCR_SZ == '\0') {
    dVar15 = (double)CONCAT44(*(undefined4 *)(param_3 + 0x18),(int)in_dr14);
    in_dr2 = (double)CONCAT44((int)((ulonglong)in_dr2 >> 0x20),*(undefined4 *)(iVar2 + 0x20));
    dVar9 = (double)CONCAT44(SUB84(dVar13,0),param_1);
  }
  else {
    dVar15 = *(double *)(param_3 + 0x18);
    dVar9 = in_xd12;
  }
  if (in_FPSCR_PR == '\0') {
    dVar9 = (double)CONCAT44((float)((ulonglong)dVar9 >> 0x20) - SUB84(in_dr2,0),SUB84(dVar9,0));
  }
  else {
    dVar9 = dVar9 - in_dr2;
  }
  if (in_FPSCR_SZ == '\0') {
    dVar4 = (double)CONCAT44(*(undefined4 *)((int)puVar3 + -0x90),*(undefined4 *)(iVar2 + 0x24));
    dVar10 = (double)CONCAT44((int)((ulonglong)dVar13 >> 0x20),param_2);
  }
  else {
    dVar4 = *(double *)((int)puVar3 + -0x90);
    dVar10 = dVar13;
  }
  if (in_FPSCR_PR == '\0') {
    dVar4 = (double)CONCAT44((float)((ulonglong)dVar4 >> 0x20) - SUB84(dVar4,0),SUB84(dVar4,0));
  }
  else {
    dVar4 = dVar4 - dVar4;
  }
  dVar5 = dVar4;
  if (in_FPSCR_SZ == '\0') {
    uVar12 = (undefined4)((ulonglong)dVar4 >> 0x20);
    dVar9 = (double)CONCAT44((int)((ulonglong)dVar9 >> 0x20),uVar12);
    dVar4 = in_xd4;
    dVar5 = (double)CONCAT44(uVar12,DAT_8c01a444);
  }
  if (in_FPSCR_PR == '\0') {
    dVar9 = (double)CONCAT44((int)((ulonglong)dVar9 >> 0x20),SUB84(dVar9,0) + SUB84(dVar5,0));
  }
  else {
    dVar9 = dVar9 + dVar5;
  }
  if (in_FPSCR_SZ == '\0') {
    dVar5 = (double)CONCAT44(*(undefined4 *)(iVar2 + 0x28),SUB84(dVar5,0));
  }
  else {
    dVar5 = *(double *)(iVar2 + 0x28);
  }
  if (in_FPSCR_PR == '\0') {
    dVar10 = (double)CONCAT44((float)((ulonglong)dVar10 >> 0x20) - (float)((ulonglong)dVar5 >> 0x20)
                              ,SUB84(dVar10,0));
  }
  else {
    dVar10 = dVar10 - dVar5;
  }
  fVar14 = SUB84(dVar9,0);
  if (in_FPSCR_SZ == '\0') {
    dVar4 = (double)CONCAT44(fVar14,SUB84(dVar5,0));
  }
  fVar16 = (float)((ulonglong)dVar9 >> 0x20);
  fVar11 = (float)((ulonglong)dVar10 >> 0x20);
  if (in_FPSCR_PR == '\0') {
    dVar4 = (double)CONCAT44(fVar11 * fVar11 +
                             fVar16 * fVar16 + (float)((ulonglong)dVar4 >> 0x20) * fVar14,
                             SUB84(dVar4,0));
  }
  else {
    dVar4 = dVar4 * dVar9;
  }
  fVar16 = (float)((ulonglong)dVar15 >> 0x20);
  dVar9 = dVar15;
  if (in_FPSCR_SZ == '\0') {
    dVar9 = (double)CONCAT44(fVar16,SUB84(dVar4,0));
    dVar4 = (double)CONCAT44((int)((ulonglong)dVar4 >> 0x20),fVar14);
    fVar8 = SUB84(dVar15,0);
  }
  if (in_FPSCR_PR == '\0') {
    fVar8 = fVar8 * SUB84(dVar15,0) + (float)((ulonglong)dVar9 >> 0x20) * fVar16;
  }
  else {
    fVar8 = (float)((ulonglong)(dVar9 * dVar15) >> 0x20);
  }
  if (in_FPSCR_SZ == '\0') {
    dVar4 = (double)CONCAT44((int)((ulonglong)dVar4 >> 0x20),fVar8);
  }
  if (in_FPSCR_PR != '\0' || (float)((ulonglong)dVar4 >> 0x20) <= SUB84(dVar4,0)) {
    (*(code *)PTR_FUN_8c01a448)(0,1,(undefined1 *)((int)puVar3 + -0x68));
    uVar12 = SUB84(in_xd12,0);
    uVar1 = SUB84(dVar10,0);
    (*(code *)PTR_FUN_8c01a448)(0,2,(undefined1 *)((int)puVar3 + -0x5c));
    if (in_FPSCR_SZ == '\0') {
      dVar9 = (double)CONCAT44(*(undefined4 *)((int)puVar3 + -100),DAT_8c01a444);
    }
    else {
      dVar9 = *(double *)((int)puVar3 + -100);
    }
    if (in_FPSCR_PR == '\0') {
      dVar9 = (double)CONCAT44((float)((ulonglong)dVar9 >> 0x20) + SUB84(dVar9,0),SUB84(dVar9,0));
    }
    else {
      dVar9 = dVar9 + dVar9;
    }
    if (in_FPSCR_SZ == '\0') {
      *(int *)((int)puVar3 + -100) = (int)((ulonglong)dVar9 >> 0x20);
      dVar9 = (double)CONCAT44(*(undefined4 *)((int)puVar3 + -0x58),SUB84(dVar9,0));
    }
    else {
      *(double *)((int)puVar3 + -100) = dVar9;
      dVar9 = *(double *)((int)puVar3 + -0x58);
    }
    if (in_FPSCR_PR == '\0') {
      dVar9 = (double)CONCAT44((float)((ulonglong)dVar9 >> 0x20) + SUB84(dVar9,0),SUB84(dVar9,0));
    }
    else {
      dVar9 = dVar9 + dVar9;
    }
    if (in_FPSCR_SZ == '\0') {
      *(int *)((int)puVar3 + -0x58) = (int)((ulonglong)dVar9 >> 0x20);
    }
    else {
      *(double *)((int)puVar3 + -0x58) = dVar9;
    }
    (*(code *)PTR_FUN_8c01a44c)((undefined1 *)((int)puVar3 + -0x44));
    if (in_FPSCR_SZ == '\0') {
      uVar12 = *(undefined4 *)((int)puVar3 + -0x90);
      dVar13 = (double)(ulonglong)uVar1;
    }
    (*(code *)PTR_FUN_8c01a450)(dVar13,(undefined1 *)((int)puVar3 + -0x44));
    uVar1 = (uint)*(ushort *)(param_3 + 4);
    *(uint *)((int)puVar3 + -0x94) = uVar1;
    if (uVar1 != 0) {
      (*(code *)PTR_FUN_8c01a454)((undefined1 *)((int)puVar3 + -0x44),uVar1);
    }
    uVar1 = (uint)*(ushort *)(param_3 + 6);
    *(uint *)((int)puVar3 + -0x94) = uVar1;
    if (uVar1 != 0) {
      (*(code *)PTR_FUN_8c01a458)((undefined1 *)((int)puVar3 + -0x44),uVar1);
    }
    (*(code *)PTR_FUN_8c01a45c)((undefined1 *)((int)puVar3 + -0x44));
    (*(code *)PTR_FUN_8c01a460)
              ((undefined1 *)((int)puVar3 + -0x44),(undefined1 *)((int)puVar3 + -0x5c),
               (undefined1 *)((int)puVar3 + -0x50));
    *(undefined4 *)((int)puVar3 + -0x5c) = *(undefined4 *)((int)puVar3 + -0x50);
    *(undefined4 *)((int)puVar3 + -0x58) = *(undefined4 *)((int)puVar3 + -0x4c);
    *(undefined4 *)((int)puVar3 + -0x54) = *(undefined4 *)((int)puVar3 + -0x48);
    (*(code *)PTR_FUN_8c01a460)
              ((undefined1 *)((int)puVar3 + -0x44),(undefined1 *)((int)puVar3 + -0x68),
               (undefined1 *)((int)puVar3 + -0x50));
    *(undefined4 *)((int)puVar3 + -0x68) = *(undefined4 *)((int)puVar3 + -0x50);
    *(undefined4 *)((int)puVar3 + -100) = *(undefined4 *)((int)puVar3 + -0x4c);
    *(undefined4 *)((int)puVar3 + -0x60) = *(undefined4 *)((int)puVar3 + -0x48);
    if (in_FPSCR_SZ == '\0') {
      lVar6 = (ulonglong)*(uint *)((int)puVar3 + -0x54) << 0x20;
    }
    else {
      lVar6 = *(longlong *)((int)puVar3 + -0x54);
    }
    if (in_FPSCR_PR == '\0' && (float)((ulonglong)lVar6 >> 0x20) < (float)lVar6) {
      if (in_FPSCR_SZ == '\0') {
        uVar7 = CONCAT44(*(undefined4 *)((int)puVar3 + -0x60),(float)lVar6);
      }
      else {
        uVar7 = *(undefined8 *)((int)puVar3 + -0x60);
      }
      if (in_FPSCR_PR == '\0' && (float)((ulonglong)uVar7 >> 0x20) < (float)uVar7) {
        return 0;
      }
    }
    if (in_FPSCR_SZ == '\0') {
      lVar6 = (ulonglong)*(uint *)((int)puVar3 + -0x54) << 0x20;
    }
    else {
      lVar6 = *(longlong *)((int)puVar3 + -0x54);
    }
    if (in_FPSCR_PR == '\0' && (float)lVar6 < (float)((ulonglong)lVar6 >> 0x20)) {
      if (in_FPSCR_SZ == '\0') {
        uVar7 = CONCAT44(*(undefined4 *)((int)puVar3 + -0x60),(float)lVar6);
      }
      else {
        uVar7 = *(undefined8 *)((int)puVar3 + -0x60);
      }
      if (in_FPSCR_PR == '\0' && (float)uVar7 < (float)((ulonglong)uVar7 >> 0x20)) {
        return 0;
      }
    }
    if (in_FPSCR_SZ == '\0') {
      *(undefined4 *)((int)puVar3 + -0x8c) = *(undefined4 *)((int)puVar3 + -0x5c);
      *(undefined4 *)((int)puVar3 + -0x88) = *(undefined4 *)((int)puVar3 + -0x58);
      *(undefined4 *)((int)puVar3 + -0x84) = *(undefined4 *)((int)puVar3 + -0x54);
      dVar13 = (double)CONCAT44(*(undefined4 *)((int)puVar3 + -0x68),
                                *(undefined4 *)((int)puVar3 + -0x5c));
    }
    else {
      *(undefined8 *)((int)puVar3 + -0x8c) = *(undefined8 *)((int)puVar3 + -0x5c);
      *(undefined8 *)((int)puVar3 + -0x88) = *(undefined8 *)((int)puVar3 + -0x58);
      *(undefined8 *)((int)puVar3 + -0x84) = *(undefined8 *)((int)puVar3 + -0x54);
      dVar13 = *(double *)((int)puVar3 + -0x68);
    }
    if (in_FPSCR_PR == '\0') {
      dVar13 = (double)CONCAT44((float)((ulonglong)dVar13 >> 0x20) - SUB84(dVar13,0),SUB84(dVar13,0)
                               );
    }
    else {
      dVar13 = dVar13 - dVar13;
    }
    if (in_FPSCR_SZ == '\0') {
      *(int *)((int)puVar3 + -0x80) = (int)((ulonglong)dVar13 >> 0x20);
      dVar13 = (double)CONCAT44(*(undefined4 *)((int)puVar3 + -100),
                                *(undefined4 *)((int)puVar3 + -0x58));
    }
    else {
      *(double *)((int)puVar3 + -0x80) = dVar13;
      dVar13 = *(double *)((int)puVar3 + -100);
    }
    if (in_FPSCR_PR == '\0') {
      dVar13 = (double)CONCAT44((float)((ulonglong)dVar13 >> 0x20) - SUB84(dVar13,0),SUB84(dVar13,0)
                               );
    }
    else {
      dVar13 = dVar13 - dVar13;
    }
    if (in_FPSCR_SZ == '\0') {
      *(int *)((int)puVar3 + -0x7c) = (int)((ulonglong)dVar13 >> 0x20);
      dVar13 = (double)CONCAT44(*(undefined4 *)((int)puVar3 + -0x60),
                                *(undefined4 *)((int)puVar3 + -0x54));
    }
    else {
      *(double *)((int)puVar3 + -0x7c) = dVar13;
      dVar13 = *(double *)((int)puVar3 + -0x60);
    }
    if (in_FPSCR_PR == '\0') {
      dVar13 = (double)CONCAT44((float)((ulonglong)dVar13 >> 0x20) - SUB84(dVar13,0),SUB84(dVar13,0)
                               );
    }
    else {
      dVar13 = dVar13 - dVar13;
    }
    if (in_FPSCR_SZ == '\0') {
      *(int *)((int)puVar3 + -0x78) = (int)((ulonglong)dVar13 >> 0x20);
    }
    else {
      *(double *)((int)puVar3 + -0x78) = dVar13;
    }
    (*(code *)PTR_FUN_8c01a468)
              ((undefined1 *)((int)puVar3 + -0x8c),PTR_DAT_8c01a464,
               (undefined1 *)((int)puVar3 + -0x74));
    if (in_FPSCR_SZ == '\0') {
      dVar13 = (double)CONCAT44(*(undefined4 *)((int)puVar3 + -0x74),uVar12);
    }
    else {
      dVar13 = *(double *)((int)puVar3 + -0x74);
    }
    fVar8 = (float)((ulonglong)dVar13 >> 0x20);
    if (in_FPSCR_PR == '\0') {
      bVar17 = SUB84(dVar15,0) < fVar8;
    }
    else {
      bVar17 = dVar15 < dVar13;
    }
    if (!bVar17) {
      fVar16 = (float)((ulonglong)dVar15 >> 0x20);
      fVar14 = -SUB84(dVar15,0);
      dVar15 = (double)CONCAT44(fVar16,fVar14);
      if (in_FPSCR_PR == '\0') {
        bVar17 = fVar8 < fVar14;
      }
      else {
        bVar17 = dVar13 < dVar15;
      }
      if (!bVar17) {
        if (in_FPSCR_SZ == '\0') {
          dVar13 = (double)CONCAT44(*(undefined4 *)((int)puVar3 + -0x70),SUB84(dVar13,0));
        }
        else {
          dVar13 = *(double *)((int)puVar3 + -0x70);
        }
        fVar8 = (float)((ulonglong)dVar13 >> 0x20);
        if (in_FPSCR_PR == '\0') {
          bVar17 = fVar16 < fVar8;
        }
        else {
          bVar17 = dVar15 < dVar13;
        }
        if (!bVar17) {
          if (in_FPSCR_PR == '\0') {
            bVar17 = fVar8 < -fVar16;
          }
          else {
            bVar17 = dVar13 < -dVar15;
          }
          if (!bVar17) {
            return 1;
          }
        }
      }
    }
  }
  return 0;
}


/* Address: 8c01a480 */

/* WARNING: Removing unreachable block (ram,0x8c01a576) */
/* WARNING: Removing unreachable block (ram,0x8c01a584) */

undefined4 FUN_8c01a480(undefined4 param_1,undefined4 param_2,int param_3)

{
  double dVar1;
  uint uVar2;
  int iVar3;
  ulonglong *puVar4;
  double in_dr2;
  double dVar5;
  double dVar6;
  float fVar7;
  double dVar8;
  double dVar9;
  float fVar11;
  undefined8 uVar10;
  undefined8 in_dr12;
  double dVar12;
  float fVar14;
  ulonglong in_dr14;
  double dVar15;
  float fVar16;
  double in_xd4;
  double in_xd12;
  undefined8 in_xd14;
  bool bVar17;
  undefined4 in_PR;
  char in_FPSCR_PR;
  char in_FPSCR_SZ;
  undefined1 auStack_24 [16];
  ulonglong uStack_14;
  undefined8 uStack_c;
  undefined4 uVar13;
  
  if (in_FPSCR_SZ == '\0') {
    uStack_c = CONCAT44((int)in_dr14,(undefined4)uStack_c);
                    /* WARNING: Ignoring partial resolution of indirect */
    uStack_c._0_4_ = (int)(in_dr14 >> 0x20);
    puVar4 = &uStack_14;
    uStack_14 = CONCAT44((int)in_dr12,(int)((ulonglong)in_dr12 >> 0x20));
  }
  else {
    puVar4 = (ulonglong *)auStack_24;
    uStack_c = in_xd14;
    uStack_14 = in_dr14;
  }
  *(undefined4 *)((int)puVar4 + -4) = in_PR;
  if (in_FPSCR_SZ == '\0') {
    uVar13 = *(undefined4 *)(param_3 + 8);
    *(undefined4 *)((int)puVar4 + -0x60) = *(undefined4 *)(param_3 + 0xc);
    in_dr2 = (double)CONCAT44((int)((ulonglong)in_dr2 >> 0x20),*(undefined4 *)(param_3 + 0x10));
    *(undefined4 *)((int)puVar4 + -100) = *(undefined4 *)(param_3 + 0x10);
    in_dr14 = (ulonglong)*(uint *)(param_3 + 0x14);
    fVar7 = (float)((ulonglong)in_xd14 >> 0x20);
    dVar12 = (double)CONCAT44(uVar13,*(undefined4 *)(param_3 + 0x18));
    dVar8 = (double)CONCAT44(uVar13,param_1);
  }
  else {
    dVar12 = *(double *)(param_3 + 8);
    *(undefined8 *)((int)puVar4 + -0x60) = *(undefined8 *)(param_3 + 0xc);
    *(undefined8 *)((int)puVar4 + -100) = *(undefined8 *)(param_3 + 0x10);
    fVar7 = (float)((ulonglong)*(undefined8 *)(param_3 + 0x14) >> 0x20);
    in_xd12 = *(double *)(param_3 + 0x18);
    dVar8 = dVar12;
  }
  iVar3 = *DAT_8c01a640;
  if (in_FPSCR_SZ == '\0') {
    dVar15 = (double)CONCAT44(*(undefined4 *)(param_3 + 0x1c),(int)in_dr14);
    in_dr2 = (double)CONCAT44((int)((ulonglong)in_dr2 >> 0x20),*(undefined4 *)(iVar3 + 0x20));
  }
  else {
    dVar15 = *(double *)(param_3 + 0x1c);
  }
  if (in_FPSCR_PR == '\0') {
    dVar8 = (double)CONCAT44((float)((ulonglong)dVar8 >> 0x20) - SUB84(in_dr2,0),SUB84(dVar8,0));
  }
  else {
    dVar8 = dVar8 - in_dr2;
  }
  if (in_FPSCR_SZ == '\0') {
    dVar5 = (double)CONCAT44(*(undefined4 *)((int)puVar4 + -0x60),*(undefined4 *)(iVar3 + 0x24));
  }
  else {
    dVar5 = *(double *)((int)puVar4 + -0x60);
  }
  if (in_FPSCR_PR == '\0') {
    dVar5 = (double)CONCAT44((float)((ulonglong)dVar5 >> 0x20) - SUB84(dVar5,0),SUB84(dVar5,0));
  }
  else {
    dVar5 = dVar5 - dVar5;
  }
  dVar6 = dVar5;
  if (in_FPSCR_SZ == '\0') {
    uVar13 = (undefined4)((ulonglong)dVar5 >> 0x20);
    dVar8 = (double)CONCAT44((int)((ulonglong)dVar8 >> 0x20),uVar13);
    dVar5 = in_xd4;
    dVar6 = (double)CONCAT44(uVar13,DAT_8c01a644);
  }
  if (in_FPSCR_PR == '\0') {
    dVar8 = (double)CONCAT44((int)((ulonglong)dVar8 >> 0x20),SUB84(dVar8,0) + SUB84(dVar6,0));
  }
  else {
    dVar8 = dVar8 + dVar6;
  }
  fVar14 = (float)((ulonglong)dVar8 >> 0x20);
  if (in_FPSCR_SZ == '\0') {
    dVar6 = (double)CONCAT44(*(undefined4 *)(iVar3 + 0x28),SUB84(dVar6,0));
    dVar9 = (double)CONCAT44(*(undefined4 *)((int)puVar4 + -100),param_2);
    dVar1 = (double)((ulonglong)(uint)fVar14 << 0x20);
  }
  else {
    dVar6 = *(double *)(iVar3 + 0x28);
    dVar9 = *(double *)((int)puVar4 + -100);
    dVar1 = dVar8;
  }
  if (in_FPSCR_PR == '\0') {
    dVar9 = (double)CONCAT44((float)((ulonglong)dVar9 >> 0x20) - (float)((ulonglong)dVar6 >> 0x20),
                             SUB84(dVar9,0));
  }
  else {
    dVar9 = dVar9 - dVar6;
  }
  fVar16 = SUB84(dVar8,0);
  if (in_FPSCR_SZ == '\0') {
    dVar5 = (double)CONCAT44(fVar16,SUB84(dVar6,0));
  }
  fVar11 = (float)((ulonglong)dVar9 >> 0x20);
  if (in_FPSCR_PR == '\0') {
    dVar5 = (double)CONCAT44(fVar11 * fVar11 +
                             (float)((ulonglong)dVar1 >> 0x20) * fVar14 +
                             (float)((ulonglong)dVar5 >> 0x20) * fVar16,SUB84(dVar5,0));
  }
  else {
    dVar5 = dVar5 * dVar8;
  }
  if (in_FPSCR_SZ == '\0') {
    in_xd12 = (double)CONCAT44(SUB84(dVar12,0),SUB84(dVar5,0));
    dVar5 = (double)CONCAT44((int)((ulonglong)dVar5 >> 0x20),fVar16);
    fVar7 = SUB84(dVar15,0);
  }
  fVar14 = (float)((ulonglong)dVar15 >> 0x20);
  if (in_FPSCR_PR == '\0') {
    fVar7 = fVar14 * fVar14 +
            fVar7 * SUB84(dVar15,0) + (float)((ulonglong)in_xd12 >> 0x20) * SUB84(dVar12,0);
  }
  else {
    fVar7 = (float)((ulonglong)(in_xd12 * dVar12) >> 0x20);
  }
  if (in_FPSCR_SZ == '\0') {
    dVar5 = (double)CONCAT44((int)((ulonglong)dVar5 >> 0x20),fVar7);
  }
  if (in_FPSCR_PR != '\0' || (float)((ulonglong)dVar5 >> 0x20) <= SUB84(dVar5,0)) {
    (*(code *)PTR_FUN_8c01a648)(0,1,(undefined1 *)((int)puVar4 + -0x5c));
    uVar13 = SUB84(dVar9,0);
    if (in_FPSCR_SZ == '\0') {
      dVar8 = (double)CONCAT44(*(undefined4 *)((int)puVar4 + -0x58),DAT_8c01a644);
    }
    else {
      dVar8 = *(double *)((int)puVar4 + -0x58);
    }
    if (in_FPSCR_PR == '\0') {
      dVar8 = (double)CONCAT44((float)((ulonglong)dVar8 >> 0x20) + SUB84(dVar8,0),SUB84(dVar8,0));
    }
    else {
      dVar8 = dVar8 + dVar8;
    }
    if (in_FPSCR_SZ == '\0') {
      *(int *)((int)puVar4 + -0x58) = (int)((ulonglong)dVar8 >> 0x20);
    }
    else {
      *(double *)((int)puVar4 + -0x58) = dVar8;
    }
    (*(code *)PTR_FUN_8c01a64c)((undefined1 *)((int)puVar4 + -0x44));
    if (in_FPSCR_SZ == '\0') {
      uVar10 = CONCAT44(*(undefined4 *)((int)puVar4 + -100),uVar13);
      dVar8 = (double)(ulonglong)*(uint *)((int)puVar4 + -0x60);
    }
    else {
      uVar10 = *(undefined8 *)((int)puVar4 + -100);
      dVar8 = dVar12;
    }
    uVar13 = SUB84(dVar8,0);
    (*(code *)PTR_FUN_8c01a650)(uVar10,(undefined1 *)((int)puVar4 + -0x44));
    uVar2 = (uint)*(ushort *)(param_3 + 4);
    *(uint *)((int)puVar4 + -0x68) = uVar2;
    if (uVar2 != 0) {
      (*(code *)PTR_FUN_8c01a654)((undefined1 *)((int)puVar4 + -0x44),uVar2);
    }
    uVar2 = (uint)*(ushort *)(param_3 + 6);
    *(uint *)((int)puVar4 + -0x68) = uVar2;
    if (uVar2 != 0) {
      (*(code *)PTR_FUN_8c01a658)((undefined1 *)((int)puVar4 + -0x44),uVar2);
    }
    (*(code *)PTR_FUN_8c01a65c)((undefined1 *)((int)puVar4 + -0x44));
    (*(code *)PTR_FUN_8c01a660)
              ((undefined1 *)((int)puVar4 + -0x44),(undefined1 *)((int)puVar4 + -0x5c),
               (undefined1 *)((int)puVar4 + -0x50));
    if (in_FPSCR_SZ == '\0') {
      dVar8 = (double)CONCAT44(*(undefined4 *)((int)puVar4 + -0x50),uVar13);
    }
    else {
      dVar8 = *(double *)((int)puVar4 + -0x50);
    }
    fVar7 = (float)((ulonglong)dVar8 >> 0x20);
    if (in_FPSCR_PR == '\0') {
      bVar17 = SUB84(dVar15,0) < fVar7;
    }
    else {
      bVar17 = dVar15 < dVar8;
    }
    if (!bVar17) {
      fVar16 = (float)((ulonglong)dVar15 >> 0x20);
      fVar14 = -SUB84(dVar15,0);
      dVar15 = (double)CONCAT44(fVar16,fVar14);
      if (in_FPSCR_PR == '\0') {
        bVar17 = fVar7 < fVar14;
      }
      else {
        bVar17 = dVar8 < dVar15;
      }
      if (!bVar17) {
        if (in_FPSCR_SZ == '\0') {
          dVar8 = (double)CONCAT44(*(undefined4 *)((int)puVar4 + -0x4c),SUB84(dVar8,0));
        }
        else {
          dVar8 = *(double *)((int)puVar4 + -0x4c);
        }
        fVar7 = (float)((ulonglong)dVar8 >> 0x20);
        if (in_FPSCR_PR == '\0') {
          bVar17 = SUB84(dVar12,0) < fVar7;
        }
        else {
          bVar17 = dVar12 < dVar8;
        }
        if (!bVar17) {
          fVar14 = -SUB84(dVar12,0);
          if (in_FPSCR_PR == '\0') {
            bVar17 = fVar7 < fVar14;
          }
          else {
            bVar17 = dVar8 < (double)CONCAT44((int)((ulonglong)dVar12 >> 0x20),fVar14);
          }
          if (!bVar17) {
            if (in_FPSCR_SZ == '\0') {
              dVar8 = (double)CONCAT44(*(undefined4 *)((int)puVar4 + -0x48),SUB84(dVar8,0));
            }
            else {
              dVar8 = *(double *)((int)puVar4 + -0x48);
            }
            fVar7 = (float)((ulonglong)dVar8 >> 0x20);
            if (in_FPSCR_PR == '\0') {
              bVar17 = fVar16 < fVar7;
            }
            else {
              bVar17 = dVar15 < dVar8;
            }
            if (!bVar17) {
              if (in_FPSCR_PR == '\0') {
                bVar17 = fVar7 < -fVar16;
              }
              else {
                bVar17 = dVar8 < -dVar15;
              }
              if (!bVar17) {
                return 1;
              }
            }
          }
        }
      }
    }
  }
  return 0;
}


/* Address: 8c01a5ae */

void FUN_8c01a5ae(void)

{
  undefined *puVar1;
  int *piVar2;
  short *psVar3;
  int iVar4;
  
  psVar3 = DAT_8c01a674;
  piVar2 = DAT_8c01a670;
  puVar1 = PTR_DAT_8c01a664;
  if ((*(char *)(*(int *)PTR_DAT_8c01a664 + 8) == '\0') &&
     ((*(uint *)(DAT_8c01a668 + 0x10) & *(uint *)PTR_DAT_8c01a66c) != 0)) {
    do {
      *(char *)(*(int *)puVar1 + 6) = *(char *)(*(int *)puVar1 + 6) + '\x01';
      if ((int)*psVar3 <= (int)*(char *)(*(int *)puVar1 + 6)) {
        *(undefined1 *)(*(int *)puVar1 + 6) = 0;
      }
      iVar4 = *(int *)puVar1;
    } while ((*(int *)(*(char *)(iVar4 + 6) * 0x10 + *piVar2 + 8) != 1) ||
            (*(char *)(iVar4 + 8) != '\0'));
    FUN_8c01aae0((int)*(char *)(iVar4 + 6),0);
  }
  return;
}


/* Address: 8c01a60c */

void FUN_8c01a60c(int param_1)

{
  undefined *puVar1;
  char in_FPSCR_SZ;
  
  puVar1 = PTR_DAT_8c01a678;
  if (in_FPSCR_SZ == '\0') {
    *(undefined4 *)(param_1 + 0x20) = *(undefined4 *)(*(int *)PTR_DAT_8c01a678 + 0xc);
  }
  else {
    *(undefined8 *)(param_1 + 0x20) = *(undefined8 *)(*(int *)PTR_DAT_8c01a678 + 0xc);
  }
  if (in_FPSCR_SZ == '\0') {
    *(undefined4 *)(param_1 + 0x24) = *(undefined4 *)(*(int *)puVar1 + 0x10);
  }
  else {
    *(undefined8 *)(param_1 + 0x24) = *(undefined8 *)(*(int *)puVar1 + 0x10);
  }
  if (in_FPSCR_SZ == '\0') {
    *(undefined4 *)(param_1 + 0x28) = *(undefined4 *)(*(int *)puVar1 + 0x14);
  }
  else {
    *(undefined8 *)(param_1 + 0x28) = *(undefined8 *)(*(int *)puVar1 + 0x14);
  }
  *(undefined4 *)(param_1 + 0x14) = *(undefined4 *)(*(int *)puVar1 + 0x24);
  *(undefined4 *)(param_1 + 0x18) = *(undefined4 *)(*(int *)puVar1 + 0x28);
  *(undefined4 *)(param_1 + 0x1c) = *(undefined4 *)(*(int *)puVar1 + 0x2c);
  return;
}


/* Address: 8c01a680 */

void FUN_8c01a680(int param_1)

{
  undefined *puVar1;
  undefined *puVar2;
  int iVar3;
  undefined1 *puVar4;
  double dVar5;
  undefined4 uVar6;
  undefined4 uVar7;
  double in_dr2;
  double dVar8;
  undefined8 uVar9;
  double dVar10;
  float in_fr13;
  undefined4 in_fr12;
  float fVar11;
  double in_dr14;
  double dVar12;
  double in_xd2;
  undefined8 in_xd14;
  undefined4 in_PR;
  char in_FPSCR_PR;
  char in_FPSCR_SZ;
  undefined1 auStack_20 [12];
  undefined1 local_14 [4];
  undefined8 uStack_10;
  
  puVar1 = PTR_DAT_8c01a8a0;
  uVar6 = (undefined4)((ulonglong)in_dr14 >> 0x20);
  if (in_FPSCR_SZ == '\0') {
    uStack_10 = CONCAT44(SUB84(in_dr14,0),(undefined4)uStack_10);
                    /* WARNING: Ignoring partial resolution of indirect */
    uStack_10._0_4_ = uVar6;
    puVar4 = local_14;
  }
  else {
    puVar4 = auStack_20;
    uStack_10 = in_xd14;
  }
  *(undefined4 *)(puVar4 + -4) = in_PR;
  if (in_FPSCR_SZ == '\0') {
    uVar7 = *(undefined4 *)(*(int *)PTR_DAT_8c01a8a0 + 0xc);
    in_dr2 = (double)CONCAT44((int)((ulonglong)in_dr2 >> 0x20),uVar7);
    *(undefined4 *)(param_1 + 0x20) = uVar7;
  }
  else {
    in_xd2 = *(double *)(*(int *)PTR_DAT_8c01a8a0 + 0xc);
    *(double *)(param_1 + 0x20) = in_xd2;
  }
  if (in_FPSCR_SZ == '\0') {
    uVar7 = *(undefined4 *)(*(int *)puVar1 + 0x10);
    in_dr2 = (double)CONCAT44((int)((ulonglong)in_dr2 >> 0x20),uVar7);
    *(undefined4 *)(param_1 + 0x24) = uVar7;
  }
  else {
    in_xd2 = *(double *)(*(int *)puVar1 + 0x10);
    *(double *)(param_1 + 0x24) = in_xd2;
  }
  if (in_FPSCR_SZ == '\0') {
    uVar7 = *(undefined4 *)(*(int *)puVar1 + 0x14);
    in_dr2 = (double)CONCAT44((int)((ulonglong)in_dr2 >> 0x20),uVar7);
    *(undefined4 *)(param_1 + 0x28) = uVar7;
  }
  else {
    in_xd2 = *(double *)(*(int *)puVar1 + 0x14);
    *(double *)(param_1 + 0x28) = in_xd2;
  }
  iVar3 = *(int *)puVar1;
  if (in_FPSCR_SZ == '\0') {
    in_dr2 = (double)CONCAT44((int)((ulonglong)in_dr2 >> 0x20),*(undefined4 *)(iVar3 + 0x18));
    in_dr14 = (double)CONCAT44(uVar6,*(undefined4 *)(iVar3 + 0xc));
  }
  else {
    in_xd2 = *(double *)(iVar3 + 0x18);
    in_xd14 = *(undefined8 *)(iVar3 + 0xc);
  }
  if (in_FPSCR_PR == '\0') {
    fVar11 = SUB84(in_dr14,0) - SUB84(in_dr2,0);
  }
  else {
    fVar11 = SUB84(in_dr14 - in_dr2,0);
  }
  if (in_FPSCR_SZ == '\0') {
    in_dr2 = (double)CONCAT44((int)((ulonglong)in_dr2 >> 0x20),*(undefined4 *)(iVar3 + 0x20));
    dVar12 = (double)CONCAT44(*(undefined4 *)(iVar3 + 0x14),fVar11);
  }
  else {
    in_xd2 = *(double *)(iVar3 + 0x20);
    dVar12 = *(double *)(iVar3 + 0x14);
  }
  if (in_FPSCR_PR == '\0') {
    dVar12 = (double)CONCAT44((float)((ulonglong)dVar12 >> 0x20) - SUB84(in_dr2,0),SUB84(dVar12,0));
  }
  else {
    dVar12 = dVar12 - in_dr2;
  }
  uVar9 = in_xd14;
  if (in_FPSCR_SZ == '\0') {
    uVar9 = CONCAT44(SUB84(dVar12,0),(int)((ulonglong)dVar12 >> 0x20));
    in_fr13 = DAT_8c01a8a4;
  }
  dVar5 = (double)(*(code *)PTR_FUN_8c01a8a8)(uVar9);
  puVar2 = PTR_FUN_8c01a8ac;
  if (in_FPSCR_PR == '\0') {
    dVar5 = (double)CONCAT44((float)((ulonglong)dVar5 >> 0x20) * in_fr13,SUB84(dVar5,0));
  }
  else {
    dVar5 = dVar5 * (double)CONCAT44(in_fr12,in_fr13);
  }
  fVar11 = (float)((ulonglong)dVar12 >> 0x20);
  dVar10 = dVar12;
  if (in_FPSCR_SZ == '\0') {
    in_dr2 = (double)CONCAT44((int)((ulonglong)in_dr2 >> 0x20),fVar11);
    dVar10 = in_xd2;
  }
  if (in_FPSCR_PR == '\0') {
    dVar8 = (double)CONCAT44((int)((ulonglong)in_dr2 >> 0x20),SUB84(in_dr2,0) * fVar11);
    iVar3 = (int)(float)((ulonglong)dVar5 >> 0x20);
  }
  else {
    dVar8 = in_dr2 * dVar12;
    iVar3 = (int)dVar5;
  }
  fVar11 = SUB84(dVar12,0);
  if (in_FPSCR_SZ != '\0') {
    fVar11 = (float)((ulonglong)in_xd14 >> 0x20);
  }
  if (in_FPSCR_PR == '\0') {
    dVar8 = (double)CONCAT44((int)((ulonglong)dVar8 >> 0x20),
                             fVar11 * SUB84(dVar12,0) + SUB84(dVar8,0));
  }
  *(int *)(param_1 + 0x18) = iVar3;
  if (in_FPSCR_SZ == '\0') {
    dVar10 = (double)CONCAT44(SUB84(dVar8,0),(int)uVar9);
  }
  uVar6 = (*(code *)puVar2)(dVar10);
  iVar3 = *(int *)puVar1;
  if (in_FPSCR_SZ == '\0') {
    dVar8 = (double)CONCAT44((int)((ulonglong)dVar8 >> 0x20),*(undefined4 *)(iVar3 + 0x10));
    dVar12 = (double)CONCAT44(*(undefined4 *)(iVar3 + 0x1c),uVar6);
  }
  else {
    dVar12 = *(double *)(iVar3 + 0x1c);
  }
  if (in_FPSCR_PR == '\0') {
    dVar12 = (double)CONCAT44((float)((ulonglong)dVar12 >> 0x20) - SUB84(dVar8,0),SUB84(dVar12,0));
  }
  else {
    dVar12 = dVar12 - dVar8;
  }
  dVar12 = (double)(*(code *)PTR_FUN_8c01a8a8)(dVar12);
  if (in_FPSCR_PR == '\0') {
    iVar3 = (int)((float)((ulonglong)dVar12 >> 0x20) * in_fr13);
  }
  else {
    iVar3 = (int)(dVar12 * (double)CONCAT44(in_fr12,in_fr13));
  }
  *(int *)(param_1 + 0x14) = iVar3;
  *(undefined4 *)(param_1 + 0x1c) = *(undefined4 *)(*(int *)puVar1 + 0x2c);
  return;
}


/* Address: 8c01a718 */

double FUN_8c01a718(int param_1)

{
  undefined *puVar1;
  int iVar2;
  undefined1 *puVar3;
  double dVar4;
  double dVar5;
  float fVar7;
  undefined8 uVar6;
  float extraout_fr3;
  float extraout_fr3_00;
  float fVar8;
  undefined8 in_dr12;
  double dVar9;
  undefined8 uVar10;
  float in_fr15;
  undefined8 in_xd12;
  double in_xd14;
  undefined4 in_PR;
  char in_FPSCR_PR;
  char in_FPSCR_SZ;
  undefined1 auStack_20 [8];
  undefined8 local_18;
  
  puVar1 = PTR_DAT_8c01a8a0;
  if (in_FPSCR_SZ == '\0') {
    puVar3 = (undefined1 *)((int)&local_18 + 4);
    local_18 = CONCAT44((int)((ulonglong)in_dr12 >> 0x20),(undefined4)local_18);
  }
  else {
    puVar3 = auStack_20;
    local_18 = in_xd12;
  }
  *(undefined4 *)(puVar3 + -4) = in_PR;
  iVar2 = *(int *)PTR_DAT_8c01a8a0;
  if (in_FPSCR_SZ == '\0') {
    dVar9 = (double)CONCAT44(*(undefined4 *)(iVar2 + 8),(int)in_dr12);
  }
  else {
    dVar9 = *(double *)(iVar2 + 8);
  }
  dVar4 = (double)(*(code *)PTR_FUN_8c01a8b0)(*(undefined4 *)(iVar2 + 0x24));
  if (in_FPSCR_PR == '\0') {
    dVar9 = (double)CONCAT44((float)((ulonglong)dVar9 >> 0x20) * (float)((ulonglong)dVar4 >> 0x20),
                             SUB84(dVar9,0));
  }
  else {
    dVar9 = dVar9 * dVar4;
  }
  iVar2 = *(int *)puVar1;
  if (in_FPSCR_SZ == '\0') {
    in_fr15 = (float)((ulonglong)dVar9 >> 0x20);
    dVar4 = (double)CONCAT44(*(undefined4 *)(iVar2 + 8),*(undefined4 *)(iVar2 + 0x1c));
    dVar9 = in_xd14;
  }
  else {
    in_xd12 = *(undefined8 *)(iVar2 + 0x1c);
    dVar4 = *(double *)(iVar2 + 8);
  }
  dVar5 = (double)(*(code *)PTR_FUN_8c01a8b4)(*(undefined4 *)(iVar2 + 0x24));
  if (in_FPSCR_PR == '\0') {
    fVar8 = SUB84(dVar4,0) - (float)((ulonglong)dVar4 >> 0x20) * (float)((ulonglong)dVar5 >> 0x20);
  }
  else {
    fVar8 = SUB84(dVar4 * dVar5 - dVar4 * dVar5,0);
  }
  if (in_FPSCR_SZ == '\0') {
    *(float *)(param_1 + 0x24) = fVar8;
  }
  else {
    *(undefined8 *)(param_1 + 0x24) = in_xd12;
  }
  iVar2 = *(int *)puVar1;
  if (in_FPSCR_SZ == '\0') {
    uVar10 = CONCAT44(*(undefined4 *)(iVar2 + 0x18),fVar8);
  }
  else {
    uVar10 = *(undefined8 *)(iVar2 + 0x18);
  }
  fVar8 = (float)(*(code *)PTR_FUN_8c01a8b4)(*(undefined4 *)(iVar2 + 0x28));
  fVar7 = in_fr15;
  if (in_FPSCR_SZ != '\0') {
    fVar7 = (float)((ulonglong)dVar9 >> 0x20);
    fVar8 = extraout_fr3;
  }
  if (in_FPSCR_PR == '\0') {
    uVar10 = CONCAT44(fVar7 * fVar8 + (float)((ulonglong)uVar10 >> 0x20),(int)uVar10);
  }
  if (in_FPSCR_SZ == '\0') {
    *(int *)(param_1 + 0x20) = (int)((ulonglong)uVar10 >> 0x20);
  }
  else {
    *(undefined8 *)(param_1 + 0x20) = uVar10;
  }
  iVar2 = *(int *)puVar1;
  if (in_FPSCR_SZ == '\0') {
    uVar10 = CONCAT44(*(undefined4 *)(iVar2 + 0x20),(int)uVar10);
  }
  else {
    uVar10 = *(undefined8 *)(iVar2 + 0x20);
  }
  uVar6 = (*(code *)PTR_FUN_8c01a8b0)(*(undefined4 *)(iVar2 + 0x28));
  fVar8 = extraout_fr3_00;
  if (in_FPSCR_SZ == '\0') {
    fVar8 = (float)((ulonglong)uVar6 >> 0x20);
    dVar9 = (double)CONCAT44(in_fr15,(int)uVar6);
  }
  if (in_FPSCR_PR == '\0') {
    uVar10 = CONCAT44((float)((ulonglong)dVar9 >> 0x20) * fVar8 + (float)((ulonglong)uVar10 >> 0x20)
                      ,(int)uVar10);
  }
  if (in_FPSCR_SZ == '\0') {
    *(int *)(param_1 + 0x28) = (int)((ulonglong)uVar10 >> 0x20);
  }
  else {
    *(undefined8 *)(param_1 + 0x28) = uVar10;
  }
  *(undefined4 *)(param_1 + 0x14) = *(undefined4 *)(*(int *)puVar1 + 0x24);
  *(undefined4 *)(param_1 + 0x18) = *(undefined4 *)(*(int *)puVar1 + 0x28);
  *(undefined4 *)(param_1 + 0x1c) = *(undefined4 *)(*(int *)puVar1 + 0x2c);
  return dVar9;
}


/* Address: 8c01a7d6 */

void FUN_8c01a7d6(char param_1,short param_2)

{
  char cVar1;
  char cVar2;
  char cVar3;
  undefined *puVar4;
  int *piVar5;
  int *piVar6;
  int *piVar7;
  undefined *puVar8;
  undefined1 uVar9;
  byte bVar10;
  int iVar11;
  int iVar12;
  int iVar13;
  
  piVar6 = DAT_8c01a8c0;
  piVar5 = DAT_8c01a8bc;
  puVar4 = PTR_DAT_8c01a8b8;
  cVar1 = *(char *)(param_1 * 0x10 + *DAT_8c01a8bc + 4);
  cVar2 = *(char *)(*(short *)(*(int *)PTR_DAT_8c01a8b8 + 4) * 0x40 + *DAT_8c01a8c0 + 1);
  iVar11 = cVar1 * 4;
  cVar3 = *(char *)(param_2 * 0x40 + *DAT_8c01a8c0 + 1);
  iVar13 = (int)cVar1;
  *(int *)(iVar11 + *(int *)PTR_DAT_8c01a8b8 + 0x18) = (int)*(char *)(*(int *)PTR_DAT_8c01a8b8 + 8);
  *(undefined2 *)(cVar1 * 2 + *(int *)puVar4 + 0x30) = *(undefined2 *)(*(int *)puVar4 + 4);
  *(undefined1 *)(*(int *)puVar4 + 0x3c + iVar13) = *(undefined1 *)(*(int *)puVar4 + 0xb);
  *(undefined1 *)(*(int *)puVar4 + 0x74 + iVar13) = *(undefined1 *)(*(int *)puVar4 + 6);
  *(undefined4 *)(*(int *)puVar4 + 0x44 + iVar11) = *(undefined4 *)(*(int *)puVar4 + 0xc);
  iVar12 = *(int *)puVar4;
  if (*(short *)(iVar12 + 4) == -1) {
    *(undefined1 *)(iVar12 + 0x7a + iVar13) = 0x15;
  }
  else {
    *(undefined1 *)(iVar12 + 0x7a + iVar13) =
         *(undefined1 *)(*(short *)(iVar12 + 4) * 0x40 + *piVar6 + 2);
  }
  piVar7 = DAT_8c01aa80;
  *(undefined4 *)(*(int *)puVar4 + 0x5c + iVar11) =
       *(undefined4 *)(*(char *)(*(int *)puVar4 + 0x7a + iVar13) * 8 + *DAT_8c01aa80 + 4);
  *(char *)(*(int *)puVar4 + 8) = cVar1;
  *(short *)(*(int *)puVar4 + 4) = param_2;
  *(char *)(*(int *)puVar4 + 6) = param_1;
  *(undefined4 *)(*(int *)puVar4 + 0xc) = *(undefined4 *)(param_1 * 0x10 + *piVar5);
  if (cVar2 < cVar3) {
    *DAT_8c01aa84 = 1;
  }
  if ((cVar2 < '\n') || (cVar3 < '\n')) {
    uVar9 = *(undefined1 *)(param_2 * 0x40 + *piVar6 + 2);
  }
  else {
    uVar9 = 0;
  }
  *(undefined1 *)(*(int *)puVar4 + 9) = uVar9;
  puVar8 = PTR_DAT_8c01aa88;
  *(undefined4 *)(*(int *)puVar4 + 0x10) =
       *(undefined4 *)(*(char *)(*(int *)puVar4 + 9) * 8 + *piVar7 + 4);
  *(undefined4 *)(*(int *)puVar8 + 0x14) = 0;
  iVar11 = *(int *)puVar4;
  if (*(int *)(*(char *)(iVar11 + 9) * 8 + *piVar7) == 0) {
    bVar10 = 0;
  }
  else {
    bVar10 = *(byte *)(iVar11 + 0xb) | 1;
  }
  *(byte *)(iVar11 + 0xb) = bVar10;
  *(undefined1 *)(*(int *)puVar4 + 7) =
       *(undefined1 *)(*(char *)(*(int *)puVar4 + 6) * 0x10 + *piVar5 + 5);
  puVar8 = PTR_LAB_8c01aa8c;
  **(undefined4 **)puVar4 = 1;
  (*(code *)PTR_FUN_8c01aa90)(8,6,puVar8);
  return;
}


/* Address: 8c01a9b8 */

void FUN_8c01a9b8(void)

{
  short sVar1;
  char cVar2;
  char cVar3;
  char cVar4;
  undefined *puVar5;
  undefined *puVar6;
  undefined1 uVar7;
  undefined4 uVar8;
  int iVar9;
  int iVar10;
  
  puVar5 = PTR_DAT_8c01aa94;
  cVar2 = *(char *)(*(int *)PTR_DAT_8c01aa94 + 8);
  cVar3 = *(char *)(*(short *)(*(int *)PTR_DAT_8c01aa94 + 4) * 0x40 + *DAT_8c01aa98 + 1);
  sVar1 = *(short *)(cVar2 * 2 + *(int *)PTR_DAT_8c01aa94 + 0x30);
  cVar4 = *(char *)(sVar1 * 0x40 + *DAT_8c01aa98 + 1);
  *(short *)(*(int *)PTR_DAT_8c01aa94 + 4) = sVar1;
  iVar9 = (int)cVar2;
  iVar10 = cVar2 * 4;
  *(undefined1 *)(*(int *)puVar5 + 0xb) = *(undefined1 *)(*(int *)puVar5 + 0x3c + iVar9);
  *(undefined1 *)(*(int *)puVar5 + 6) = *(undefined1 *)(*(int *)puVar5 + 0x74 + iVar9);
  *(undefined4 *)(*(int *)puVar5 + 0xc) = *(undefined4 *)(iVar10 + *(int *)puVar5 + 0x44);
  *(char *)(*(int *)puVar5 + 8) = (char)*(undefined4 *)(*(int *)puVar5 + 0x18 + iVar10);
  if ((cVar3 < '\n') || (cVar4 < '\n')) {
    uVar7 = *(undefined1 *)(*(int *)puVar5 + 0x7a + iVar9);
  }
  else {
    uVar7 = 0;
  }
  *(undefined1 *)(*(int *)puVar5 + 9) = uVar7;
  if (cVar3 < cVar4) {
    *DAT_8c01aa84 = 1;
  }
  if ((cVar3 < '\n') || (cVar4 < '\n')) {
    uVar8 = *(undefined4 *)(*(int *)puVar5 + 0x5c + iVar10);
  }
  else {
    uVar8 = *(undefined4 *)(*DAT_8c01aa80 + 4);
  }
  *(undefined4 *)(*(int *)puVar5 + 0x10) = uVar8;
  puVar6 = PTR_LAB_8c01ac68;
  *(undefined1 *)(*(int *)puVar5 + 7) =
       *(undefined1 *)(*(char *)(*(int *)puVar5 + 6) * 0x10 + *DAT_8c01ac60 + 5);
  **(undefined4 **)puVar5 = 1;
  *(undefined4 *)(*(int *)puVar5 + 0x14) = 0;
  *(undefined4 *)(*(int *)PTR_DAT_8c01ac64 + 0x14) = 0;
  (*(code *)PTR_FUN_8c01ac6c)(8,6,puVar6);
  return;
}


/* Address: 8c01aae0 */

void FUN_8c01aae0(short param_1,byte param_2)

{
  int *piVar1;
  undefined *puVar2;
  undefined *puVar3;
  int *piVar4;
  int iVar5;
  
  puVar3 = PTR_DAT_8c01ac70;
  iVar5 = *(int *)PTR_DAT_8c01ac70;
  if (*(char *)(iVar5 + 8) == '\0') {
    *(int *)(iVar5 + 0x18) = (int)*(char *)(iVar5 + 8);
    *(undefined2 *)(*(int *)puVar3 + 0x30) = *(undefined2 *)(*(int *)puVar3 + 4);
    *(undefined1 *)(*(int *)puVar3 + 0x3c) = *(undefined1 *)(*(int *)puVar3 + 0xb);
    *(undefined1 *)(*(int *)puVar3 + 0x74) = *(undefined1 *)(*(int *)puVar3 + 6);
    *(undefined4 *)(*(int *)puVar3 + 0x44) = *(undefined4 *)(*(int *)puVar3 + 0xc);
    iVar5 = *(int *)puVar3;
    if (*(short *)(iVar5 + 4) == -1) {
      *(undefined1 *)(iVar5 + 0x7a) = 0x15;
    }
    else {
      *(undefined1 *)(iVar5 + 0x7a) =
           *(undefined1 *)(*(short *)(iVar5 + 4) * 0x40 + *DAT_8c01ac74 + 2);
    }
    piVar4 = DAT_8c01ac78;
    piVar1 = DAT_8c01ac60;
    *(undefined4 *)(*(int *)puVar3 + 0x5c) =
         *(undefined4 *)(*(char *)(*(int *)puVar3 + 0x7a) * 8 + *DAT_8c01ac78 + 4);
    *(undefined2 *)(*(int *)puVar3 + 4) = 0xffff;
    *(undefined1 *)(*(int *)puVar3 + 8) = 0;
    *(char *)(*(int *)puVar3 + 6) = (char)param_1;
    *(undefined4 *)(*(int *)puVar3 + 0xc) = *(undefined4 *)(param_1 * 0x10 + *piVar1);
    *(byte *)(*(int *)puVar3 + 9) = param_2;
    puVar2 = PTR_DAT_8c01ac64;
    *(undefined4 *)(*(int *)puVar3 + 0x10) = *(undefined4 *)((uint)param_2 * 8 + *piVar4 + 4);
    *(undefined4 *)(*(int *)puVar2 + 0x14) = 0;
    if (*(int *)(*(char *)(*(int *)puVar3 + 9) * 8 + *piVar4) != 0) {
      *(byte *)(*(int *)puVar3 + 0xb) = *(byte *)(*(int *)puVar3 + 0xb) | 1;
    }
    *(undefined1 *)(*(int *)puVar3 + 7) =
         *(undefined1 *)(*(char *)(*(int *)puVar3 + 6) * 0x10 + *DAT_8c01ac60 + 5);
    puVar2 = PTR_LAB_8c01ac68;
    **(undefined4 **)puVar3 = 1;
    (*(code *)PTR_FUN_8c01ac6c)(8,6,puVar2);
    return;
  }
  return;
}


/* Address: 8c01abdc */

void FUN_8c01abdc(short param_1,byte param_2)

{
  undefined *puVar1;
  int *piVar2;
  int *piVar3;
  undefined *puVar4;
  int iVar5;
  
  piVar2 = DAT_8c01ac78;
  puVar1 = PTR_DAT_8c01ac70;
  iVar5 = *(int *)PTR_DAT_8c01ac70;
  if ('\x02' < *(char *)(iVar5 + 8)) {
    return;
  }
  if (*(char *)(iVar5 + 8) != '\x02') {
    *(int *)(iVar5 + 0x20) = (int)*(char *)(iVar5 + 8);
    *(undefined2 *)(*(int *)puVar1 + 0x34) = *(undefined2 *)(*(int *)puVar1 + 4);
    *(undefined1 *)(*(int *)puVar1 + 0x3e) = *(undefined1 *)(*(int *)puVar1 + 0xb);
    *(undefined1 *)(*(int *)puVar1 + 0x76) = *(undefined1 *)(*(int *)puVar1 + 6);
    *(undefined4 *)(*(int *)puVar1 + 0x4c) = *(undefined4 *)(*(int *)puVar1 + 0xc);
    if (*(short *)(*(int *)puVar1 + 4) == -1) {
      *(undefined1 *)(*(int *)puVar1 + 0x7c) = 0x15;
    }
    else {
      *(undefined1 *)(*(int *)puVar1 + 0x7c) =
           *(undefined1 *)(*(short *)(*(int *)puVar1 + 4) * 0x40 + *DAT_8c01ac74 + 2);
    }
    *(undefined4 *)(*(int *)puVar1 + 100) =
         *(undefined4 *)(*(char *)(*(int *)puVar1 + 0x7c) * 8 + *piVar2 + 4);
    *(undefined1 *)(*(int *)puVar1 + 8) = 2;
  }
  piVar3 = DAT_8c01ae40;
  *(undefined2 *)(*(int *)puVar1 + 4) = 0xffff;
  *(char *)(*(int *)puVar1 + 6) = (char)param_1;
  *(undefined4 *)(*(int *)puVar1 + 0xc) = *(undefined4 *)(param_1 * 0x10 + *piVar3);
  *(byte *)(*(int *)puVar1 + 9) = param_2;
  puVar4 = PTR_DAT_8c01ae44;
  *(undefined4 *)(*(int *)puVar1 + 0x10) = *(undefined4 *)((uint)param_2 * 8 + *piVar2 + 4);
  *(undefined4 *)(*(int *)puVar4 + 0x14) = 0;
  *(undefined1 *)(*(int *)puVar1 + 0xb) = 2;
  if (*(int *)(*(char *)(*(int *)puVar1 + 9) * 8 + *piVar2) != 0) {
    *(byte *)(*(int *)puVar1 + 0xb) = *(byte *)(*(int *)puVar1 + 0xb) | 1;
  }
  *(undefined1 *)(*(int *)puVar1 + 7) =
       *(undefined1 *)(*(char *)(*(int *)puVar1 + 6) * 0x10 + *piVar3 + 5);
  puVar4 = PTR_LAB_8c01ae48;
  **(undefined4 **)puVar1 = 1;
  (*(code *)PTR_FUN_8c01ae4c)(8,6,puVar4);
  return;
}


/* Address: 8c01ad2c */

void FUN_8c01ad2c(undefined4 param_1,undefined1 param_2,undefined1 param_3)

{
  undefined *puVar1;
  undefined *puVar2;
  int *piVar3;
  int iVar4;
  
  piVar3 = DAT_8c01ae54;
  puVar2 = PTR_DAT_8c01ae50;
  iVar4 = *(int *)PTR_DAT_8c01ae50;
  if (*(char *)(iVar4 + 8) < '\x03') {
    if (*(char *)(iVar4 + 8) != '\x02') {
      *(int *)(iVar4 + 0x20) = (int)*(char *)(iVar4 + 8);
      *(undefined2 *)(*(int *)puVar2 + 0x34) = *(undefined2 *)(*(int *)puVar2 + 4);
      *(undefined1 *)(*(int *)puVar2 + 0x3e) = *(undefined1 *)(*(int *)puVar2 + 0xb);
      *(undefined1 *)(*(int *)puVar2 + 0x76) = *(undefined1 *)(*(int *)puVar2 + 6);
      *(undefined4 *)(*(int *)puVar2 + 0x4c) = *(undefined4 *)(*(int *)puVar2 + 0xc);
      if (*(short *)(*(int *)puVar2 + 4) == -1) {
        *(undefined1 *)(*(int *)puVar2 + 0x7c) = 0x15;
      }
      else {
        *(undefined1 *)(*(int *)puVar2 + 0x7c) =
             *(undefined1 *)(*(short *)(*(int *)puVar2 + 4) * 0x40 + *DAT_8c01ae58 + 2);
      }
      *(undefined4 *)(*(int *)puVar2 + 100) =
           *(undefined4 *)(*(char *)(*(int *)puVar2 + 0x7c) * 8 + *piVar3 + 4);
      *(undefined1 *)(*(int *)puVar2 + 8) = 2;
    }
    *(undefined2 *)(*(int *)puVar2 + 4) = 0xffff;
    *(undefined1 *)(*(int *)puVar2 + 6) = 0xff;
    *(undefined4 *)(*(int *)puVar2 + 0xc) = param_1;
    *(undefined1 *)(*(int *)puVar2 + 9) = param_2;
    puVar1 = PTR_DAT_8c01ae44;
    *(undefined4 *)(*(int *)puVar2 + 0x10) =
         *(undefined4 *)(*(char *)(*(int *)puVar2 + 9) * 8 + *piVar3 + 4);
    *(undefined4 *)(*(int *)puVar1 + 0x14) = 0;
    *(undefined1 *)(*(int *)puVar2 + 0xb) = 2;
    if (*(int *)(*(char *)(*(int *)puVar2 + 9) * 8 + *piVar3) != 0) {
      *(byte *)(*(int *)puVar2 + 0xb) = *(byte *)(*(int *)puVar2 + 0xb) | 1;
    }
    puVar1 = PTR_LAB_8c01ae48;
    *(undefined1 *)(*(int *)puVar2 + 7) = param_3;
    **(undefined4 **)puVar2 = 1;
    (*(code *)PTR_FUN_8c01ae4c)(8,6,puVar1);
    return;
  }
  return;
}


/* Address: 8c01ae60 */

void FUN_8c01ae60(void)

{
  undefined *puVar1;
  undefined *puVar2;
  int iVar3;
  
  puVar1 = PTR_DAT_8c01b000;
  iVar3 = *(int *)PTR_DAT_8c01b000;
  if (*(char *)(iVar3 + 8) == '\x02') {
    *(undefined2 *)(iVar3 + 4) = *(undefined2 *)(iVar3 + 0x34);
    *(undefined1 *)(*(int *)puVar1 + 0xb) = *(undefined1 *)(*(int *)puVar1 + 0x3e);
    *(undefined1 *)(*(int *)puVar1 + 6) = *(undefined1 *)(*(int *)puVar1 + 0x76);
    *(undefined4 *)(*(int *)puVar1 + 0xc) = *(undefined4 *)(*(int *)puVar1 + 0x4c);
    *(undefined1 *)(*(int *)puVar1 + 9) = *(undefined1 *)(*(int *)puVar1 + 0x7c);
    *(undefined4 *)(*(int *)puVar1 + 0x10) = *(undefined4 *)(*(int *)puVar1 + 100);
    *(char *)(*(int *)puVar1 + 8) = (char)*(undefined4 *)(*(int *)puVar1 + 0x20);
    puVar2 = PTR_LAB_8c01b00c;
    *(undefined1 *)(*(int *)puVar1 + 7) =
         *(undefined1 *)(*(char *)(*(int *)puVar1 + 6) * 0x10 + *DAT_8c01b004 + 5);
    **(undefined4 **)puVar1 = 1;
    *(undefined4 *)(*(int *)puVar1 + 0x14) = 0;
    *(undefined4 *)(*(int *)PTR_DAT_8c01b008 + 0x14) = 0;
    (*(code *)PTR_FUN_8c01b010)(8,6,puVar2);
    if ((*(char *)(*(int *)puVar1 + 8) == '\0') && (*DAT_8c01b014 != -1)) {
      FUN_8c01aae0(*DAT_8c01b014,*DAT_8c01b018);
    }
  }
  *DAT_8c01b01c = 0;
  return;
}


/* Address: 8c01af06 */

void FUN_8c01af06(undefined4 param_1,undefined1 param_2,undefined1 param_3)

{
  undefined *puVar1;
  undefined *puVar2;
  int *piVar3;
  int iVar4;
  
  piVar3 = DAT_8c01b020;
  puVar1 = PTR_DAT_8c01b000;
  iVar4 = *(int *)PTR_DAT_8c01b000;
  if (*(char *)(iVar4 + 8) < '\x05') {
    if (*(char *)(iVar4 + 8) != '\x04') {
      *(int *)(iVar4 + 0x28) = (int)*(char *)(iVar4 + 8);
      *(undefined2 *)(*(int *)puVar1 + 0x38) = *(undefined2 *)(*(int *)puVar1 + 4);
      *(undefined1 *)(*(int *)puVar1 + 0x40) = *(undefined1 *)(*(int *)puVar1 + 0xb);
      *(undefined1 *)(*(int *)puVar1 + 0x78) = *(undefined1 *)(*(int *)puVar1 + 6);
      *(undefined4 *)(*(int *)puVar1 + 0x54) = *(undefined4 *)(*(int *)puVar1 + 0xc);
      if (*(short *)(*(int *)puVar1 + 4) == -1) {
        *(undefined1 *)(*(int *)puVar1 + 0x7e) = 0x15;
      }
      else {
        *(undefined1 *)(*(int *)puVar1 + 0x7e) =
             *(undefined1 *)(*(short *)(*(int *)puVar1 + 4) * 0x40 + *DAT_8c01b024 + 2);
      }
      *(undefined4 *)(*(int *)puVar1 + 0x6c) =
           *(undefined4 *)(*(char *)(*(int *)puVar1 + 0x7e) * 8 + *piVar3 + 4);
      *(undefined1 *)(*(int *)puVar1 + 8) = 4;
    }
    *(undefined2 *)(*(int *)puVar1 + 4) = 0xffff;
    *(undefined1 *)(*(int *)puVar1 + 6) = 0xff;
    *(undefined4 *)(*(int *)puVar1 + 0xc) = param_1;
    *(undefined1 *)(*(int *)puVar1 + 9) = param_2;
    puVar2 = PTR_DAT_8c01b008;
    *(undefined4 *)(*(int *)puVar1 + 0x10) =
         *(undefined4 *)(*(char *)(*(int *)puVar1 + 9) * 8 + *piVar3 + 4);
    *(undefined4 *)(*(int *)puVar2 + 0x14) = 0;
    *(undefined1 *)(*(int *)puVar1 + 0xb) = 2;
    if (*(int *)(*(char *)(*(int *)puVar1 + 9) * 8 + *piVar3) != 0) {
      *(byte *)(*(int *)puVar1 + 0xb) = *(byte *)(*(int *)puVar1 + 0xb) | 1;
    }
    puVar2 = PTR_LAB_8c01b00c;
    *(undefined1 *)(*(int *)puVar1 + 7) = param_3;
    **(undefined4 **)puVar1 = 1;
    (*(code *)PTR_FUN_8c01b010)(8,6,puVar2);
    return;
  }
  return;
}


/* Address: 8c01b040 */

void FUN_8c01b040(short param_1,byte param_2)

{
  undefined *puVar1;
  int *piVar2;
  int *piVar3;
  undefined *puVar4;
  int iVar5;
  
  piVar2 = DAT_8c01b204;
  puVar1 = PTR_DAT_8c01b200;
  iVar5 = *(int *)PTR_DAT_8c01b200;
  if ('\x04' < *(char *)(iVar5 + 8)) {
    return;
  }
  if (*(char *)(iVar5 + 8) != '\x04') {
    *(int *)(iVar5 + 0x28) = (int)*(char *)(iVar5 + 8);
    *(undefined2 *)(*(int *)puVar1 + 0x38) = *(undefined2 *)(*(int *)puVar1 + 4);
    *(undefined1 *)(*(int *)puVar1 + 0x40) = *(undefined1 *)(*(int *)puVar1 + 0xb);
    *(undefined1 *)(*(int *)puVar1 + 0x78) = *(undefined1 *)(*(int *)puVar1 + 6);
    *(undefined4 *)(*(int *)puVar1 + 0x54) = *(undefined4 *)(*(int *)puVar1 + 0xc);
    if (*(short *)(*(int *)puVar1 + 4) == -1) {
      *(undefined1 *)(*(int *)puVar1 + 0x7e) = 0x15;
    }
    else {
      *(undefined1 *)(*(int *)puVar1 + 0x7e) =
           *(undefined1 *)(*(short *)(*(int *)puVar1 + 4) * 0x40 + *DAT_8c01b208 + 2);
    }
    *(undefined4 *)(*(int *)puVar1 + 0x6c) =
         *(undefined4 *)(*(char *)(*(int *)puVar1 + 0x7e) * 8 + *piVar2 + 4);
    *(undefined1 *)(*(int *)puVar1 + 8) = 4;
  }
  piVar3 = DAT_8c01b20c;
  *(undefined2 *)(*(int *)puVar1 + 4) = 0xffff;
  *(char *)(*(int *)puVar1 + 6) = (char)param_1;
  *(undefined4 *)(*(int *)puVar1 + 0xc) = *(undefined4 *)(param_1 * 0x10 + *piVar3);
  *(byte *)(*(int *)puVar1 + 9) = param_2;
  puVar4 = PTR_DAT_8c01b210;
  *(undefined4 *)(*(int *)puVar1 + 0x10) = *(undefined4 *)((uint)param_2 * 8 + *piVar2 + 4);
  *(undefined4 *)(*(int *)puVar4 + 0x14) = 0;
  *(undefined1 *)(*(int *)puVar1 + 0xb) = 2;
  if (*(int *)(*(char *)(*(int *)puVar1 + 9) * 8 + *piVar2) != 0) {
    *(byte *)(*(int *)puVar1 + 0xb) = *(byte *)(*(int *)puVar1 + 0xb) | 1;
  }
  *(undefined1 *)(*(int *)puVar1 + 7) =
       *(undefined1 *)(*(char *)(*(int *)puVar1 + 6) * 0x10 + *piVar3 + 5);
  puVar4 = PTR_LAB_8c01b214;
  **(undefined4 **)puVar1 = 1;
  (*(code *)PTR_FUN_8c01b218)(8,6,puVar4);
  return;
}


/* Address: 8c01b158 */

void FUN_8c01b158(void)

{
  undefined *puVar1;
  undefined *puVar2;
  int iVar3;
  
  puVar1 = PTR_DAT_8c01b200;
  iVar3 = *(int *)PTR_DAT_8c01b200;
  if (*(char *)(iVar3 + 8) == '\x04') {
    *(undefined2 *)(iVar3 + 4) = *(undefined2 *)(iVar3 + 0x38);
    *(undefined1 *)(*(int *)puVar1 + 0xb) = *(undefined1 *)(*(int *)puVar1 + 0x40);
    *(undefined1 *)(*(int *)puVar1 + 6) = *(undefined1 *)(*(int *)puVar1 + 0x78);
    *(undefined4 *)(*(int *)puVar1 + 0xc) = *(undefined4 *)(*(int *)puVar1 + 0x54);
    *(undefined1 *)(*(int *)puVar1 + 9) = *(undefined1 *)(*(int *)puVar1 + 0x7e);
    *(undefined4 *)(*(int *)puVar1 + 0x10) = *(undefined4 *)(*(int *)puVar1 + 0x6c);
    *(char *)(*(int *)puVar1 + 8) = (char)*(undefined4 *)(*(int *)puVar1 + 0x28);
    puVar2 = PTR_LAB_8c01b214;
    *(undefined1 *)(*(int *)puVar1 + 7) =
         *(undefined1 *)(*(char *)(*(int *)puVar1 + 6) * 0x10 + *DAT_8c01b20c + 5);
    **(undefined4 **)puVar1 = 1;
    *(undefined4 *)(*(int *)puVar1 + 0x14) = 0;
    *(undefined4 *)(*(int *)PTR_DAT_8c01b210 + 0x14) = 0;
    (*(code *)PTR_FUN_8c01b218)(8,6,puVar2);
    if ((*(char *)(*(int *)puVar1 + 8) == '\0') && (*DAT_8c01b21c != -1)) {
      FUN_8c01aae0(*DAT_8c01b21c,*DAT_8c01b220);
      return;
    }
  }
  return;
}


/* Address: 8c01b308 */

void FUN_8c01b308(int param_1)

{
  undefined *puVar1;
  undefined2 *puVar2;
  char in_FPSCR_SZ;
  
  puVar2 = DAT_8c01b3b8;
  if (in_FPSCR_SZ == '\0') {
    *(undefined4 *)(DAT_8c01b3b8 + 2) = *(undefined4 *)(param_1 + 0x30);
    *(undefined4 *)(puVar2 + 4) = *(undefined4 *)(param_1 + 0x34);
    *(undefined4 *)(puVar2 + 6) = *(undefined4 *)(param_1 + 0x38);
    *(undefined4 *)(puVar2 + 8) = *(undefined4 *)(param_1 + 0x24);
    *(undefined4 *)(puVar2 + 10) = *(undefined4 *)(param_1 + 0x28);
    *(undefined4 *)(puVar2 + 0xc) = *(undefined4 *)(param_1 + 0x2c);
  }
  else {
    *(undefined8 *)(DAT_8c01b3b8 + 2) = *(undefined8 *)(param_1 + 0x30);
    *(undefined8 *)(puVar2 + 4) = *(undefined8 *)(param_1 + 0x34);
    *(undefined8 *)(puVar2 + 6) = *(undefined8 *)(param_1 + 0x38);
    *(undefined8 *)(puVar2 + 8) = *(undefined8 *)(param_1 + 0x24);
    *(undefined8 *)(puVar2 + 10) = *(undefined8 *)(param_1 + 0x28);
    *(undefined8 *)(puVar2 + 0xc) = *(undefined8 *)(param_1 + 0x2c);
  }
  *puVar2 = *(undefined2 *)(param_1 + 0x20);
  puVar2[1] = *(undefined2 *)(param_1 + 0x22);
  puVar1 = PTR_DAT_8c01b3a0;
  if (in_FPSCR_SZ == '\0') {
    *(undefined4 *)(puVar2 + 0xe) = *(undefined4 *)(param_1 + 0x3c);
  }
  else {
    *(undefined8 *)(puVar2 + 0xe) = *(undefined8 *)(param_1 + 0x3c);
  }
  *(undefined4 *)(puVar2 + 0x10) = *(undefined4 *)(*(int *)puVar1 + 0x14);
  return;
}


/* Address: 8c01b37a */

undefined4 FUN_8c01b37a(void)

{
  return *DAT_8c01b3c0;
}


/* Address: 8c01b3e0 */

void FUN_8c01b3e0(undefined1 param_1)

{
  undefined *puVar1;
  int *piVar2;
  undefined *puVar3;
  
  piVar2 = DAT_8c01b4e4;
  puVar1 = PTR_DAT_8c01b4e0;
  *(undefined1 *)(*(int *)PTR_DAT_8c01b4e0 + 9) = param_1;
  puVar3 = PTR_DAT_8c01b4e8;
  *(undefined4 *)(*(int *)puVar1 + 0x10) =
       *(undefined4 *)(*(char *)(*(int *)puVar1 + 9) * 8 + *piVar2 + 4);
  *(undefined4 *)(*(int *)puVar3 + 0x14) = 0;
  return;
}


/* Address: 8c01b406 */

int FUN_8c01b406(void)

{
  return (int)*(char *)(*(int *)PTR_DAT_8c01b4e0 + 9);
}


/* Address: 8c01b40e */

int FUN_8c01b40e(void)

{
  return (int)*(char *)(*(int *)PTR_DAT_8c01b4e0 + 6);
}


/* Address: 8c01b436 */

void FUN_8c01b436(undefined4 param_1,int param_2,int param_3)

{
  char cVar1;
  int iVar2;
  undefined4 uVar3;
  undefined1 *puVar4;
  double dVar5;
  double in_dr2;
  double dVar6;
  float fVar7;
  double dVar8;
  float fVar9;
  float in_fr15;
  undefined4 in_fr14;
  bool bVar10;
  undefined4 in_PR;
  char in_FPSCR_PR;
  char in_FPSCR_SZ;
  undefined1 local_10 [4];
  undefined1 local_c [4];
  
  if (in_FPSCR_SZ == '\0') {
    puVar4 = local_c;
  }
  else {
    puVar4 = local_10;
  }
  *(undefined4 *)(puVar4 + -4) = in_PR;
  if (in_FPSCR_SZ == '\0') {
    in_dr2 = (double)CONCAT44((int)((ulonglong)in_dr2 >> 0x20),*(undefined4 *)(param_3 + 0x20));
    dVar6 = (double)CONCAT44(*(undefined4 *)(param_2 + 0x20),*(undefined4 *)(param_2 + 0x24));
  }
  else {
    dVar6 = *(double *)(param_2 + 0x20);
  }
  if (in_FPSCR_PR == '\0') {
    dVar6 = (double)CONCAT44((float)((ulonglong)dVar6 >> 0x20) - SUB84(in_dr2,0),SUB84(dVar6,0));
  }
  else {
    dVar6 = dVar6 - in_dr2;
  }
  if (in_FPSCR_SZ == '\0') {
    in_dr2 = (double)CONCAT44((int)((ulonglong)in_dr2 >> 0x20),*(undefined4 *)(param_3 + 0x24));
  }
  if (in_FPSCR_PR == '\0') {
    dVar6 = (double)CONCAT44((int)((ulonglong)dVar6 >> 0x20),SUB84(dVar6,0) - SUB84(in_dr2,0));
  }
  else {
    dVar6 = dVar6 - in_dr2;
  }
  fVar7 = (float)((ulonglong)dVar6 >> 0x20);
  if (in_FPSCR_SZ == '\0') {
    in_dr2 = (double)CONCAT44((int)((ulonglong)in_dr2 >> 0x20),*(undefined4 *)(param_3 + 0x28));
    dVar8 = (double)CONCAT44(*(undefined4 *)(param_2 + 0x28),param_1);
    dVar5 = (double)((ulonglong)(uint)fVar7 << 0x20);
  }
  else {
    dVar8 = *(double *)(param_2 + 0x28);
    dVar5 = dVar6;
  }
  if (in_FPSCR_PR == '\0') {
    fVar9 = (float)((ulonglong)dVar8 >> 0x20) - SUB84(in_dr2,0);
  }
  else {
    fVar9 = (float)((ulonglong)(dVar8 - in_dr2) >> 0x20);
  }
  if (in_FPSCR_SZ == '\0') {
    dVar8 = (double)CONCAT44(DAT_8c01b4f0,SUB84(dVar6,0));
  }
  else {
    dVar8 = (double)CONCAT44(PTR_FUN_8c01b4f4,DAT_8c01b4f0);
  }
  if (in_FPSCR_PR == '\0') {
    dVar8 = (double)CONCAT44((int)((ulonglong)dVar8 >> 0x20),
                             fVar9 * fVar9 +
                             (float)((ulonglong)dVar5 >> 0x20) * fVar7 +
                             SUB84(dVar8,0) * SUB84(dVar6,0));
  }
  else {
    dVar8 = dVar8 * dVar6;
  }
  if (in_FPSCR_SZ == '\0') {
    in_fr15 = SUB84(dVar8,0);
  }
  if (in_FPSCR_PR == '\0') {
    bVar10 = in_fr15 < (float)((ulonglong)dVar8 >> 0x20);
  }
  else {
    bVar10 = (double)CONCAT44(in_fr14,in_fr15) < dVar8;
  }
  if ((((bVar10) &&
       (iVar2 = (*(code *)PTR_FUN_8c01b4f4)
                          (*(undefined4 *)(param_2 + 0x14),*(undefined4 *)(param_3 + 0x14)),
       iVar2 < 0x5b)) &&
      (iVar2 = (*(code *)PTR_FUN_8c01b4f4)
                         (*(undefined4 *)(param_2 + 0x18),*(undefined4 *)(param_3 + 0x18)),
      iVar2 < 0x5b)) &&
     (iVar2 = (*(code *)PTR_FUN_8c01b4f4)
                        (*(undefined4 *)(param_2 + 0x1c),*(undefined4 *)(param_3 + 0x1c)),
     iVar2 < 0x5b)) {
    cVar1 = *(char *)(*(int *)PTR_DAT_8c01b4e0 + 9);
    if (cVar1 == '\x01') {
      uVar3 = 3;
    }
    else {
      if (((cVar1 != '\0') && (cVar1 != '\x02')) && ((cVar1 != '\x03' && (cVar1 != '\x04')))) {
        return;
      }
      uVar3 = 0;
    }
    FUN_8c01b3e0(uVar3);
    return;
  }
  return;
}


/* Address: 8c01b500 */

void FUN_8c01b500(void)

{
  int *piVar1;
  undefined4 *puVar2;
  int iVar3;
  undefined4 uVar4;
  double dVar5;
  float extraout_fr3;
  float fVar6;
  undefined4 extraout_fr2;
  bool bVar7;
  char in_FPSCR_PR;
  char in_FPSCR_SZ;
  
  piVar1 = DAT_8c01b5a0;
  (*(code *)PTR_FUN_8c01b5a8)(DAT_8c01b5a4,*DAT_8c01b5a0 + 0x20);
  dVar5 = (double)(*(code *)PTR_FUN_8c01b5ac)(DAT_8c01b5a4);
  fVar6 = extraout_fr3;
  if (in_FPSCR_SZ == '\0') {
    fVar6 = (float)DAT_8c01b5b0;
  }
  if (in_FPSCR_PR == '\0') {
    bVar7 = (float)((ulonglong)dVar5 >> 0x20) < fVar6;
  }
  else {
    bVar7 = dVar5 < (double)CONCAT44(extraout_fr2,fVar6);
  }
  if ((bVar7) && ((*(ushort *)(*piVar1 + 4) & 1) != 0)) {
    *DAT_8c01b5b0._4_4_ = 1;
  }
  else {
    *DAT_8c01b5b0._4_4_ = 0;
  }
  puVar2 = DAT_8c01b5a4;
  iVar3 = *DAT_8c01b5a0;
  uVar4 = *(undefined4 *)(iVar3 + 0x24);
  *DAT_8c01b5a4 = *(undefined4 *)(iVar3 + 0x20);
  puVar2[1] = uVar4;
  puVar2[2] = *(undefined4 *)(iVar3 + 0x28);
  return;
}


/* Address: 8c01b554 */

void FUN_8c01b554(void)

{
  double *pdVar1;
  int *piVar2;
  undefined4 *puVar3;
  undefined4 uVar4;
  int iVar5;
  double dVar6;
  char in_FPSCR_PR;
  char in_FPSCR_SZ;
  
  puVar3 = DAT_8c01b5c0;
  piVar2 = DAT_8c01b5bc;
  pdVar1 = DAT_8c01b5b8;
  if (in_FPSCR_SZ == '\0') {
    dVar6 = (double)CONCAT44(*(undefined4 *)(*DAT_8c01b5bc + 0x20),*DAT_8c01b5c0);
  }
  else {
    dVar6 = *(double *)(*DAT_8c01b5bc + 0x20);
  }
  if (in_FPSCR_PR == '\0') {
    dVar6 = (double)CONCAT44((float)((ulonglong)dVar6 >> 0x20) - SUB84(dVar6,0),SUB84(dVar6,0));
  }
  else {
    dVar6 = dVar6 - dVar6;
  }
  if (in_FPSCR_SZ == '\0') {
    *(int *)DAT_8c01b5b8 = (int)((ulonglong)dVar6 >> 0x20);
    dVar6 = (double)(ulonglong)(uint)puVar3[1];
  }
  else {
    *DAT_8c01b5b8 = dVar6;
  }
  if (in_FPSCR_SZ == '\0') {
    dVar6 = (double)CONCAT44(*(undefined4 *)(*piVar2 + 0x24),SUB84(dVar6,0));
  }
  else {
    dVar6 = *(double *)(*piVar2 + 0x24);
  }
  if (in_FPSCR_PR == '\0') {
    dVar6 = (double)CONCAT44((float)((ulonglong)dVar6 >> 0x20) - SUB84(dVar6,0),SUB84(dVar6,0));
  }
  else {
    dVar6 = dVar6 - dVar6;
  }
  if (in_FPSCR_SZ == '\0') {
    *(int *)((int)pdVar1 + 4) = (int)((ulonglong)dVar6 >> 0x20);
    dVar6 = (double)(ulonglong)(uint)puVar3[2];
  }
  else {
    *(double *)((int)pdVar1 + 4) = dVar6;
  }
  if (in_FPSCR_SZ == '\0') {
    dVar6 = (double)CONCAT44(*(undefined4 *)(*piVar2 + 0x28),SUB84(dVar6,0));
  }
  else {
    dVar6 = *(double *)(*piVar2 + 0x28);
  }
  if (in_FPSCR_PR == '\0') {
    dVar6 = (double)CONCAT44((float)((ulonglong)dVar6 >> 0x20) - SUB84(dVar6,0),SUB84(dVar6,0));
  }
  else {
    dVar6 = dVar6 - dVar6;
  }
  if (in_FPSCR_SZ == '\0') {
    *(int *)(pdVar1 + 1) = (int)((ulonglong)dVar6 >> 0x20);
  }
  else {
    pdVar1[1] = dVar6;
  }
  iVar5 = *piVar2;
  uVar4 = *(undefined4 *)(iVar5 + 0x24);
  *puVar3 = *(undefined4 *)(iVar5 + 0x20);
  puVar3[1] = uVar4;
  puVar3[2] = *(undefined4 *)(iVar5 + 0x28);
  return;
}


/* Address: 8c01b5e0 */

undefined4 FUN_8c01b5e0(void)

{
  switch(*(undefined1 *)(*(int *)PTR_DAT_8c01b5fc + 9)) {
  case 5:
  case 7:
  case 9:
  case 0xb:
  case 0xd:
  case 0xf:
  case 0x11:
  case 0x13:
  case 0x15:
  case 0x17:
  case 0x19:
  case 0x1a:
    return 1;
  default:
    return 0;
  }
}


/* Address: 8c01b684 */

/* WARNING: Removing unreachable block (ram,0x8c01b6a0) */

int FUN_8c01b684(undefined4 param_1)

{
  int *piVar1;
  uint uVar2;
  int iVar3;
  short sVar4;
  int iVar5;
  int iVar6;
  char *pcVar7;
  
  iVar6 = 0;
  uVar2 = (*(code *)PTR_FUN_8c01b734)(&DAT_8c78c608);
  piVar1 = DAT_8c01b744;
  if (*(int *)(PTR_DAT_8c01b738 + (uVar2 & 0xff) * 4 + ((int)(uVar2 & 0xffff) >> 8) * 0x20) == 0) {
    iVar6 = 0;
  }
  else {
    iVar5 = *(int *)*DAT_8c01b73c + -1;
    pcVar7 = (char *)((*(int *)*DAT_8c01b73c + -1) * 0x40 + *DAT_8c01b740);
    sVar4 = (short)iVar5;
    while (0 < sVar4) {
      if (*(char *)(*pcVar7 * 0x10 + *piVar1 + 4) == '\x04') {
        if (pcVar7[3] == '\0') {
          iVar3 = FUN_8c01b84e(pcVar7,param_1);
        }
        else {
          if (pcVar7[3] != '\x02') goto LAB_8c01b768;
          iVar3 = FUN_8c01bba0(pcVar7,param_1);
        }
        iVar6 = iVar6 + iVar3;
      }
LAB_8c01b768:
      iVar5 = iVar5 + -1;
      pcVar7 = pcVar7 + -0x40;
      sVar4 = (short)iVar5;
    }
  }
  return iVar6;
}


/* Address: 8c01b782 */

/* WARNING: Removing unreachable block (ram,0x8c01b7a0) */

int FUN_8c01b782(undefined4 param_1,undefined4 param_2)

{
  int *piVar1;
  uint uVar2;
  int iVar3;
  int iVar4;
  short sVar5;
  int iVar6;
  char *pcVar7;
  
  iVar4 = 0;
  uVar2 = (*(code *)PTR_FUN_8c01b8e0)(&DAT_8c78c608);
  piVar1 = DAT_8c01b8f0;
  if (*(int *)(PTR_DAT_8c01b8e4 + (uVar2 & 0xff) * 4 + ((int)(uVar2 & 0xffff) >> 8) * 0x20) == 0) {
    iVar4 = 0;
  }
  else {
    iVar6 = *(int *)*DAT_8c01b8e8 + -1;
    pcVar7 = (char *)((*(int *)*DAT_8c01b8e8 + -1) * 0x40 + *DAT_8c01b8ec);
    sVar5 = (short)iVar6;
    while (0 < sVar5) {
      if ((*(char *)(*pcVar7 * 0x10 + *piVar1 + 4) == '\x04') && (pcVar7[3] == '\x01')) {
        iVar3 = FUN_8c01b900(pcVar7,param_1,param_2);
        iVar4 = iVar4 + iVar3;
      }
      iVar6 = iVar6 + -1;
      pcVar7 = pcVar7 + -0x40;
      sVar5 = (short)iVar6;
    }
  }
  return iVar4;
}


/* Address: 8c01b818 */

bool FUN_8c01b818(undefined4 param_1,undefined4 param_2,int param_3,double *param_4)

{
  double dVar1;
  double in_dr2;
  double dVar2;
  float fVar3;
  double dVar4;
  float fVar5;
  double dVar6;
  float fVar7;
  double in_xd2;
  double in_xd4;
  bool bVar8;
  char in_FPSCR_PR;
  char in_FPSCR_SZ;
  
  if (in_FPSCR_SZ == '\0') {
    dVar4 = (double)CONCAT44(*(undefined4 *)param_4,param_1);
    in_dr2 = (double)CONCAT44((int)((ulonglong)in_dr2 >> 0x20),*(undefined4 *)(param_3 + 8));
  }
  else {
    dVar4 = *param_4;
    in_xd2 = *(double *)(param_3 + 8);
  }
  if (in_FPSCR_PR == '\0') {
    dVar4 = (double)CONCAT44((float)((ulonglong)dVar4 >> 0x20) - SUB84(in_dr2,0),SUB84(dVar4,0));
  }
  else {
    dVar4 = dVar4 - in_dr2;
  }
  if (in_FPSCR_SZ == '\0') {
    in_dr2 = (double)CONCAT44((int)((ulonglong)in_dr2 >> 0x20),*(undefined4 *)(param_3 + 0xc));
    dVar4 = (double)CONCAT44((int)((ulonglong)dVar4 >> 0x20),*(undefined4 *)((int)param_4 + 4));
  }
  else {
    in_xd2 = *(double *)(param_3 + 0xc);
    in_xd4 = *(double *)((int)param_4 + 4);
  }
  if (in_FPSCR_PR == '\0') {
    dVar4 = (double)CONCAT44((int)((ulonglong)dVar4 >> 0x20),SUB84(dVar4,0) - SUB84(in_dr2,0));
  }
  else {
    dVar4 = dVar4 - in_dr2;
  }
  fVar5 = (float)((ulonglong)dVar4 >> 0x20);
  if (in_FPSCR_SZ == '\0') {
    dVar6 = (double)CONCAT44(*(undefined4 *)(param_4 + 1),param_2);
    dVar2 = (double)CONCAT44(*(undefined4 *)(param_3 + 0x1c),*(undefined4 *)(param_3 + 0x10));
    dVar1 = (double)((ulonglong)(uint)fVar5 << 0x20);
  }
  else {
    in_xd2 = *(double *)(param_3 + 0x10);
    dVar6 = param_4[1];
    dVar2 = *(double *)(param_3 + 0x1c);
    dVar1 = dVar4;
  }
  if (in_FPSCR_PR == '\0') {
    fVar7 = (float)((ulonglong)dVar6 >> 0x20) - SUB84(dVar2,0);
  }
  else {
    fVar7 = (float)((ulonglong)(dVar6 - dVar2) >> 0x20);
  }
  fVar3 = SUB84(dVar4,0);
  if (in_FPSCR_SZ == '\0') {
    dVar2 = (double)CONCAT44((int)((ulonglong)dVar2 >> 0x20),fVar3);
    in_xd4 = in_xd2;
  }
  if (in_FPSCR_PR == '\0') {
    dVar2 = (double)CONCAT44((int)((ulonglong)dVar2 >> 0x20),
                             fVar7 * fVar7 +
                             (float)((ulonglong)dVar1 >> 0x20) * fVar5 + SUB84(dVar2,0) * fVar3);
  }
  else {
    dVar2 = dVar2 * dVar4;
  }
  if (in_FPSCR_SZ == '\0') {
    in_xd4 = (double)CONCAT44(SUB84(dVar2,0),fVar3);
  }
  if (in_FPSCR_PR == '\0') {
    bVar8 = (float)((ulonglong)in_xd4 >> 0x20) < (float)((ulonglong)dVar2 >> 0x20);
  }
  else {
    bVar8 = in_xd4 < dVar2;
  }
  return bVar8;
}


/* Address: 8c01b84e */

undefined4 FUN_8c01b84e(int param_1,double *param_2)

{
  int iVar1;
  undefined4 uVar2;
  float fVar3;
  float fVar4;
  double dVar5;
  double in_xd2;
  float in_PR;
  char in_FPSCR_PR;
  char in_FPSCR_SZ;
  float local_18;
  float fStack_14;
  float fStack_10;
  float local_c;
  
  local_c = in_PR;
  iVar1 = FUN_8c01b818();
  if (iVar1 == 1) {
    if (in_FPSCR_SZ == '\0') {
      dVar5 = (double)CONCAT44(*(float *)param_2,*(undefined4 *)(param_1 + 8));
    }
    else {
      dVar5 = *param_2;
      in_xd2 = *(double *)(param_1 + 8);
    }
    if (in_FPSCR_PR == '\0') {
      dVar5 = (double)CONCAT44((float)((ulonglong)dVar5 >> 0x20) - SUB84(dVar5,0),SUB84(dVar5,0));
    }
    else {
      dVar5 = dVar5 - dVar5;
    }
    if (in_FPSCR_SZ == '\0') {
      local_18 = (float)((ulonglong)dVar5 >> 0x20);
      dVar5 = (double)CONCAT44(*(float *)((int)param_2 + 4),*(undefined4 *)(param_1 + 0xc));
    }
    else {
      local_18 = SUB84(dVar5,0);
      in_xd2 = *(double *)(param_1 + 0xc);
      dVar5 = *(double *)((int)param_2 + 4);
    }
    if (in_FPSCR_PR == '\0') {
      dVar5 = (double)CONCAT44((float)((ulonglong)dVar5 >> 0x20) - SUB84(dVar5,0),SUB84(dVar5,0));
    }
    else {
      dVar5 = dVar5 - dVar5;
    }
    if (in_FPSCR_SZ == '\0') {
      fStack_14 = (float)((ulonglong)dVar5 >> 0x20);
      dVar5 = (double)CONCAT44(*(float *)(param_2 + 1),*(undefined4 *)(param_1 + 0x10));
    }
    else {
      fStack_14 = SUB84(dVar5,0);
      in_xd2 = *(double *)(param_1 + 0x10);
      dVar5 = param_2[1];
    }
    if (in_FPSCR_PR == '\0') {
      dVar5 = (double)CONCAT44((float)((ulonglong)dVar5 >> 0x20) - SUB84(dVar5,0),SUB84(dVar5,0));
    }
    else {
      dVar5 = dVar5 - dVar5;
    }
    fVar4 = (float)((ulonglong)dVar5 >> 0x20);
    fStack_10 = fVar4;
    if (in_FPSCR_SZ != '\0') {
      fStack_10 = SUB84(dVar5,0);
      local_c = fVar4;
    }
    (*(code *)PTR_FUN_8c01b8f4)(&local_18);
    if (in_FPSCR_SZ == '\0') {
      fVar3 = *(float *)(param_1 + 0x14);
      fVar4 = *(float *)(param_1 + 8);
    }
    else {
      in_xd2 = *(double *)(param_1 + 8);
      fVar3 = (float)((ulonglong)*(undefined8 *)(param_1 + 0x14) >> 0x20);
      fVar4 = local_18;
      local_18 = fStack_14;
    }
    if (in_FPSCR_PR == '\0') {
      fVar4 = fVar3 * local_18 + fVar4;
    }
    if (in_FPSCR_SZ == '\0') {
      *(float *)param_2 = fVar4;
      fVar4 = *(float *)(param_1 + 0x14);
      fVar3 = *(float *)(param_1 + 0xc);
    }
    else {
      *param_2 = in_xd2;
      in_xd2 = *(double *)(param_1 + 0xc);
      fVar4 = (float)((ulonglong)*(undefined8 *)(param_1 + 0x14) >> 0x20);
      fVar3 = fStack_14;
      fStack_14 = fStack_10;
    }
    if (in_FPSCR_PR == '\0') {
      fVar3 = fVar4 * fStack_14 + fVar3;
    }
    if (in_FPSCR_SZ == '\0') {
      *(float *)((int)param_2 + 4) = fVar3;
      fVar4 = *(float *)(param_1 + 0x14);
      fVar3 = *(float *)(param_1 + 0x10);
    }
    else {
      *(double *)((int)param_2 + 4) = in_xd2;
      in_xd2 = *(double *)(param_1 + 0x10);
      fVar4 = (float)((ulonglong)*(undefined8 *)(param_1 + 0x14) >> 0x20);
      fVar3 = fStack_10;
      fStack_10 = local_c;
    }
    if (in_FPSCR_PR == '\0') {
      fVar3 = fVar4 * fStack_10 + fVar3;
    }
    if (in_FPSCR_SZ == '\0') {
      *(float *)(param_2 + 1) = fVar3;
    }
    else {
      param_2[1] = in_xd2;
    }
    uVar2 = 1;
  }
  else {
    uVar2 = 0;
  }
  return uVar2;
}


/* Address: 8c01b900 */

/* WARNING: Removing unreachable block (ram,0x8c01bb10) */
/* WARNING: Removing unreachable block (ram,0x8c01bb24) */

undefined4 FUN_8c01b900(undefined4 param_1,int param_2,double *param_3,double *param_4)

{
  longlong lVar1;
  uint uVar2;
  undefined4 uVar3;
  double *pdVar4;
  double *pdVar5;
  undefined8 *puVar6;
  double dVar7;
  uint uVar9;
  double dVar8;
  float fVar10;
  undefined4 uVar11;
  double dVar12;
  undefined8 uVar13;
  longlong lVar14;
  undefined8 in_dr12;
  double in_xd2;
  bool bVar16;
  undefined4 in_PR;
  char in_FPSCR_PR;
  char in_FPSCR_SZ;
  undefined8 local_1c;
  float fVar15;
  
  if (in_FPSCR_SZ == '\0') {
    puVar6 = (undefined8 *)((int)&local_1c + 4);
    local_1c = CONCAT44((int)((ulonglong)in_dr12 >> 0x20),(undefined4)local_1c);
  }
  else {
    puVar6 = &local_1c;
    local_1c = in_dr12;
  }
  *(undefined4 *)((int)puVar6 + -4) = in_PR;
  if (in_FPSCR_SZ == '\0') {
    dVar7 = (double)CONCAT44(*(undefined4 *)(param_2 + 8),*(undefined4 *)param_4);
  }
  else {
    in_xd2 = *param_4;
    dVar7 = *(double *)(param_2 + 8);
  }
  if (in_FPSCR_PR == '\0') {
    dVar7 = (double)CONCAT44((float)((ulonglong)dVar7 >> 0x20) - SUB84(dVar7,0),SUB84(dVar7,0));
  }
  else {
    dVar7 = dVar7 - dVar7;
  }
  pdVar4 = (double *)((int)puVar6 + -0x10);
  if (in_FPSCR_SZ == '\0') {
    *(int *)pdVar4 = (int)((ulonglong)dVar7 >> 0x20);
    dVar7 = (double)CONCAT44(*(undefined4 *)(param_2 + 0xc),*(undefined4 *)((int)param_4 + 4));
  }
  else {
    *pdVar4 = dVar7;
    in_xd2 = *(double *)((int)param_4 + 4);
    dVar7 = *(double *)(param_2 + 0xc);
  }
  if (in_FPSCR_PR == '\0') {
    dVar7 = (double)CONCAT44((float)((ulonglong)dVar7 >> 0x20) - SUB84(dVar7,0),SUB84(dVar7,0));
  }
  else {
    dVar7 = dVar7 - dVar7;
  }
  if (in_FPSCR_SZ == '\0') {
    *(int *)((int)puVar6 + -0xc) = (int)((ulonglong)dVar7 >> 0x20);
    dVar7 = (double)CONCAT44(*(undefined4 *)(param_2 + 0x10),*(undefined4 *)(param_4 + 1));
  }
  else {
    *(double *)((int)puVar6 + -0xc) = dVar7;
    in_xd2 = param_4[1];
    dVar7 = *(double *)(param_2 + 0x10);
  }
  if (in_FPSCR_PR == '\0') {
    dVar7 = (double)CONCAT44((float)((ulonglong)dVar7 >> 0x20) - SUB84(dVar7,0),SUB84(dVar7,0));
  }
  else {
    dVar7 = dVar7 - dVar7;
  }
  uVar11 = (undefined4)((ulonglong)dVar7 >> 0x20);
  if (in_FPSCR_SZ == '\0') {
    *(undefined4 *)((int)puVar6 + -8) = uVar11;
    fVar10 = *(float *)((int)puVar6 + -0xc);
    dVar12 = (double)CONCAT44(*(undefined4 *)pdVar4,fVar10);
    dVar7 = (double)CONCAT44(uVar11,fVar10);
  }
  else {
    *(double *)((int)puVar6 + -8) = dVar7;
    in_xd2 = *(double *)((int)puVar6 + -0xc);
    dVar12 = *pdVar4;
    fVar10 = SUB84(dVar12,0);
  }
  fVar15 = (float)((ulonglong)dVar12 >> 0x20);
  if (in_FPSCR_PR == '\0') {
    dVar7 = (double)CONCAT44((int)((ulonglong)dVar7 >> 0x20),
                             fVar15 * fVar15 + SUB84(dVar7,0) * fVar10);
  }
  else {
    dVar7 = dVar7 * dVar12;
  }
  if (in_FPSCR_SZ == '\0') {
    fVar10 = *(float *)((int)puVar6 + -8);
    uVar13 = CONCAT44(fVar10,param_1);
  }
  else {
    uVar13 = *(undefined8 *)((int)puVar6 + -8);
    fVar10 = (float)((ulonglong)uVar13 >> 0x20);
  }
  if (in_FPSCR_PR == '\0') {
    dVar7 = (double)CONCAT44((int)((ulonglong)dVar7 >> 0x20),fVar10 * fVar10 + SUB84(dVar7,0));
  }
  if (in_FPSCR_SZ == '\0') {
    fVar15 = *(float *)(param_2 + 0x14);
    fVar10 = *(float *)(param_2 + 0x18);
    lVar14 = (ulonglong)(uint)fVar15 << 0x20;
    in_xd2 = (double)CONCAT44(SUB84(dVar7,0),fVar10);
    dVar7 = (double)CONCAT44((int)((ulonglong)dVar7 >> 0x20),fVar10);
    lVar1 = CONCAT44(fVar15,(int)uVar13);
  }
  else {
    lVar14 = *(longlong *)(param_2 + 0x14);
    fVar15 = (float)((ulonglong)lVar14 >> 0x20);
    fVar10 = SUB84(in_xd2,0);
    lVar1 = lVar14;
  }
  uVar11 = (undefined4)lVar1;
  if (in_FPSCR_PR == '\0') {
    fVar10 = (float)((ulonglong)lVar14 >> 0x20) * fVar15 + SUB84(dVar7,0) * fVar10;
  }
  else {
    fVar10 = SUB84(dVar7 * in_xd2,0);
  }
  if (in_FPSCR_SZ == '\0') {
    in_xd2 = (double)CONCAT44((int)((ulonglong)in_xd2 >> 0x20),fVar10);
  }
  if (in_FPSCR_PR != '\0' || (float)((ulonglong)in_xd2 >> 0x20) <= SUB84(in_xd2,0)) {
    if (in_FPSCR_SZ == '\0') {
      dVar7 = (double)CONCAT44(*(undefined4 *)param_3,*(undefined4 *)(param_2 + 0x24));
    }
    else {
      dVar7 = *param_3;
    }
    if (in_FPSCR_PR == '\0') {
      dVar7 = (double)CONCAT44((float)((ulonglong)dVar7 >> 0x20) - SUB84(dVar7,0),SUB84(dVar7,0));
    }
    else {
      dVar7 = dVar7 - dVar7;
    }
    pdVar4 = (double *)((int)puVar6 + -0x1c);
    pdVar5 = (double *)((int)puVar6 + -0x10);
    if (in_FPSCR_SZ == '\0') {
      *(int *)pdVar4 = (int)((ulonglong)dVar7 >> 0x20);
      dVar7 = (double)CONCAT44(*(undefined4 *)((int)param_3 + 4),*(undefined4 *)(param_2 + 0x28));
    }
    else {
      *pdVar4 = dVar7;
      dVar7 = *(double *)((int)param_3 + 4);
    }
    if (in_FPSCR_PR == '\0') {
      dVar7 = (double)CONCAT44((float)((ulonglong)dVar7 >> 0x20) - SUB84(dVar7,0),SUB84(dVar7,0));
    }
    else {
      dVar7 = dVar7 - dVar7;
    }
    if (in_FPSCR_SZ == '\0') {
      *(int *)((int)puVar6 + -0x18) = (int)((ulonglong)dVar7 >> 0x20);
      dVar7 = (double)CONCAT44(*(undefined4 *)(param_3 + 1),*(undefined4 *)(param_2 + 0x2c));
    }
    else {
      *(double *)((int)puVar6 + -0x18) = dVar7;
      dVar7 = param_3[1];
    }
    if (in_FPSCR_PR == '\0') {
      dVar7 = (double)CONCAT44((float)((ulonglong)dVar7 >> 0x20) - SUB84(dVar7,0),SUB84(dVar7,0));
    }
    else {
      dVar7 = dVar7 - dVar7;
    }
    if (in_FPSCR_SZ == '\0') {
      *(int *)((int)puVar6 + -0x14) = (int)((ulonglong)dVar7 >> 0x20);
      dVar7 = (double)CONCAT44(*(undefined4 *)param_4,*(undefined4 *)(param_2 + 0x24));
    }
    else {
      *(double *)((int)puVar6 + -0x14) = dVar7;
      dVar7 = *param_4;
    }
    if (in_FPSCR_PR == '\0') {
      dVar7 = (double)CONCAT44((float)((ulonglong)dVar7 >> 0x20) - SUB84(dVar7,0),SUB84(dVar7,0));
    }
    else {
      dVar7 = dVar7 - dVar7;
    }
    if (in_FPSCR_SZ == '\0') {
      *(int *)pdVar5 = (int)((ulonglong)dVar7 >> 0x20);
      dVar7 = (double)CONCAT44(*(undefined4 *)((int)param_4 + 4),*(undefined4 *)(param_2 + 0x28));
    }
    else {
      *pdVar5 = dVar7;
      dVar7 = *(double *)((int)param_4 + 4);
    }
    if (in_FPSCR_PR == '\0') {
      dVar7 = (double)CONCAT44((float)((ulonglong)dVar7 >> 0x20) - SUB84(dVar7,0),SUB84(dVar7,0));
    }
    else {
      dVar7 = dVar7 - dVar7;
    }
    if (in_FPSCR_SZ == '\0') {
      *(int *)((int)puVar6 + -0xc) = (int)((ulonglong)dVar7 >> 0x20);
      dVar7 = (double)CONCAT44(*(undefined4 *)(param_4 + 1),*(undefined4 *)(param_2 + 0x2c));
    }
    else {
      *(double *)((int)puVar6 + -0xc) = dVar7;
      dVar7 = param_4[1];
    }
    if (in_FPSCR_PR == '\0') {
      dVar7 = (double)CONCAT44((float)((ulonglong)dVar7 >> 0x20) - SUB84(dVar7,0),SUB84(dVar7,0));
    }
    else {
      dVar7 = dVar7 - dVar7;
    }
    if (in_FPSCR_SZ == '\0') {
      *(int *)((int)puVar6 + -8) = (int)((ulonglong)dVar7 >> 0x20);
    }
    else {
      *(double *)((int)puVar6 + -8) = dVar7;
    }
    dVar12 = (double)(*(code *)PTR_FUN_8c01bb60)(pdVar4);
    uVar9 = (uint)((ulonglong)dVar7 >> 0x20);
    if (in_FPSCR_SZ == '\0') {
      dVar12 = (double)CONCAT44((int)((ulonglong)dVar12 >> 0x20),(int)in_dr12);
    }
    dVar7 = (double)(*(code *)PTR_FUN_8c01bb60)(pdVar5);
    if (in_FPSCR_PR == '\0') {
      bVar16 = (float)((ulonglong)dVar7 >> 0x20) < (float)((ulonglong)dVar12 >> 0x20);
    }
    else {
      bVar16 = dVar7 < dVar12;
    }
    if (!bVar16) {
      uVar3 = *(undefined4 *)((int)param_3 + 4);
      *(undefined4 *)((int)puVar6 + -0x68) = *(undefined4 *)param_3;
      *(undefined4 *)((int)puVar6 + -100) = uVar3;
      *(undefined4 *)((int)puVar6 + -0x60) = *(undefined4 *)(param_3 + 1);
      uVar3 = *(undefined4 *)((int)param_4 + 4);
      *(undefined4 *)((int)puVar6 + -0x74) = *(undefined4 *)param_4;
      *(undefined4 *)((int)puVar6 + -0x70) = uVar3;
      *(undefined4 *)((int)puVar6 + -0x6c) = *(undefined4 *)(param_4 + 1);
      (*(code *)PTR_FUN_8c01bb64)((undefined1 *)((int)puVar6 + -0x44));
      if (in_FPSCR_SZ == '\0') {
        uVar13 = CONCAT44(*(undefined4 *)(param_2 + 0x10),uVar11);
        uVar11 = *(undefined4 *)(param_2 + 0xc);
      }
      else {
        uVar13 = *(undefined8 *)(param_2 + 0x10);
        uVar11 = (undefined4)*(undefined8 *)(param_2 + 8);
      }
      (*(code *)PTR_FUN_8c01bb68)(uVar13,(undefined1 *)((int)puVar6 + -0x44));
      uVar2 = (uint)*(ushort *)(param_2 + 4);
      *(uint *)((int)puVar6 + -0x90) = uVar2;
      if (uVar2 != 0) {
        (*(code *)PTR_FUN_8c01bb6c)((undefined1 *)((int)puVar6 + -0x44),uVar2);
      }
      uVar2 = (uint)*(ushort *)(param_2 + 6);
      *(uint *)((int)puVar6 + -0x90) = uVar2;
      if (uVar2 != 0) {
        (*(code *)PTR_FUN_8c01bb70)((undefined1 *)((int)puVar6 + -0x44),uVar2);
      }
      (*(code *)PTR_FUN_8c01bb74)((undefined1 *)((int)puVar6 + -0x44));
      (*(code *)PTR_FUN_8c01bb78)
                ((undefined1 *)((int)puVar6 + -0x44),(undefined1 *)((int)puVar6 + -0x68),
                 (undefined1 *)((int)puVar6 + -0x80));
      *(undefined4 *)((int)puVar6 + -0x68) = *(undefined4 *)((int)puVar6 + -0x80);
      *(undefined4 *)((int)puVar6 + -100) = *(undefined4 *)((int)puVar6 + -0x7c);
      *(undefined4 *)((int)puVar6 + -0x60) = *(undefined4 *)((int)puVar6 + -0x78);
      (*(code *)PTR_FUN_8c01bb78)
                ((undefined1 *)((int)puVar6 + -0x44),(undefined1 *)((int)puVar6 + -0x74),
                 (undefined1 *)((int)puVar6 + -0x80));
      dVar7 = (double)((ulonglong)uVar9 << 0x20);
      *(undefined4 *)((int)puVar6 + -0x74) = *(undefined4 *)((int)puVar6 + -0x80);
      *(undefined4 *)((int)puVar6 + -0x70) = *(undefined4 *)((int)puVar6 + -0x7c);
      *(undefined4 *)((int)puVar6 + -0x6c) = *(undefined4 *)((int)puVar6 + -0x78);
      if (in_FPSCR_SZ == '\0') {
        dVar12 = (double)CONCAT44(*(undefined4 *)((int)puVar6 + -0x60),uVar11);
      }
      else {
        dVar12 = *(double *)((int)puVar6 + -0x60);
      }
      fVar10 = (float)((ulonglong)dVar12 >> 0x20);
      if (in_FPSCR_PR == '\0') {
        bVar16 = fVar10 < 0.0;
      }
      else {
        bVar16 = dVar12 < dVar7;
      }
      if (bVar16) {
        if (in_FPSCR_SZ == '\0') {
          dVar7 = (double)((ulonglong)*(uint *)((int)puVar6 + -0x6c) << 0x20);
        }
        else {
          dVar7 = *(double *)((int)puVar6 + -0x6c);
        }
        if (in_FPSCR_PR == '\0' && (float)((ulonglong)dVar7 >> 0x20) < SUB84(dVar7,0)) {
          return 0;
        }
      }
      if (in_FPSCR_PR == '\0') {
        bVar16 = 0.0 < fVar10;
      }
      else {
        bVar16 = (double)((ulonglong)dVar7 & 0xffffffff00000000) < dVar12;
      }
      if (bVar16) {
        if (in_FPSCR_SZ == '\0') {
          lVar14 = (ulonglong)*(uint *)((int)puVar6 + -0x6c) << 0x20;
        }
        else {
          lVar14 = *(longlong *)((int)puVar6 + -0x6c);
        }
        if (in_FPSCR_PR == '\0' && (float)lVar14 < (float)((ulonglong)lVar14 >> 0x20)) {
          return 0;
        }
      }
      if (in_FPSCR_SZ == '\0') {
        *(undefined4 *)((int)puVar6 + -0x5c) = *(undefined4 *)((int)puVar6 + -0x68);
        *(undefined4 *)((int)puVar6 + -0x58) = *(undefined4 *)((int)puVar6 + -100);
        *(undefined4 *)((int)puVar6 + -0x54) = *(undefined4 *)((int)puVar6 + -0x60);
        dVar7 = (double)CONCAT44(*(undefined4 *)((int)puVar6 + -0x74),
                                 *(undefined4 *)((int)puVar6 + -0x68));
      }
      else {
        *(undefined8 *)((int)puVar6 + -0x5c) = *(undefined8 *)((int)puVar6 + -0x68);
        *(undefined8 *)((int)puVar6 + -0x58) = *(undefined8 *)((int)puVar6 + -100);
        *(undefined8 *)((int)puVar6 + -0x54) = *(undefined8 *)((int)puVar6 + -0x60);
        dVar7 = *(double *)((int)puVar6 + -0x74);
      }
      if (in_FPSCR_PR == '\0') {
        dVar7 = (double)CONCAT44((float)((ulonglong)dVar7 >> 0x20) - SUB84(dVar7,0),SUB84(dVar7,0));
      }
      else {
        dVar7 = dVar7 - dVar7;
      }
      if (in_FPSCR_SZ == '\0') {
        *(int *)((int)puVar6 + -0x50) = (int)((ulonglong)dVar7 >> 0x20);
        dVar7 = (double)CONCAT44(*(undefined4 *)((int)puVar6 + -0x70),
                                 *(undefined4 *)((int)puVar6 + -100));
      }
      else {
        *(double *)((int)puVar6 + -0x50) = dVar7;
        dVar7 = *(double *)((int)puVar6 + -0x70);
      }
      if (in_FPSCR_PR == '\0') {
        dVar7 = (double)CONCAT44((float)((ulonglong)dVar7 >> 0x20) - SUB84(dVar7,0),SUB84(dVar7,0));
      }
      else {
        dVar7 = dVar7 - dVar7;
      }
      if (in_FPSCR_SZ == '\0') {
        *(int *)((int)puVar6 + -0x4c) = (int)((ulonglong)dVar7 >> 0x20);
        dVar7 = (double)CONCAT44(*(undefined4 *)((int)puVar6 + -0x6c),
                                 *(undefined4 *)((int)puVar6 + -0x60));
      }
      else {
        *(double *)((int)puVar6 + -0x4c) = dVar7;
        dVar7 = *(double *)((int)puVar6 + -0x6c);
      }
      if (in_FPSCR_PR == '\0') {
        dVar7 = (double)CONCAT44((float)((ulonglong)dVar7 >> 0x20) - SUB84(dVar7,0),SUB84(dVar7,0));
      }
      else {
        dVar7 = dVar7 - dVar7;
      }
      if (in_FPSCR_SZ == '\0') {
        *(int *)((int)puVar6 + -0x48) = (int)((ulonglong)dVar7 >> 0x20);
      }
      else {
        *(double *)((int)puVar6 + -0x48) = dVar7;
      }
      (*(code *)PTR_FUN_8c01bb80)
                ((undefined1 *)((int)puVar6 + -0x5c),PTR_DAT_8c01bb7c,
                 (undefined1 *)((int)puVar6 + -0x8c));
      if (in_FPSCR_SZ == '\0') {
        dVar12 = (double)CONCAT44(*(undefined4 *)((int)puVar6 + -0x8c),SUB84(dVar12,0));
        dVar7 = (double)CONCAT44((int)((ulonglong)dVar7 >> 0x20),*(undefined4 *)(param_2 + 0x14));
      }
      else {
        dVar12 = *(double *)((int)puVar6 + -0x8c);
      }
      fVar10 = (float)((ulonglong)dVar12 >> 0x20);
      if (in_FPSCR_PR == '\0') {
        bVar16 = SUB84(dVar7,0) < fVar10;
      }
      else {
        bVar16 = dVar7 < dVar12;
      }
      if (!bVar16) {
        if (in_FPSCR_SZ == '\0') {
          dVar7 = (double)CONCAT44((int)((ulonglong)dVar7 >> 0x20),*(undefined4 *)(param_2 + 0x14));
        }
        uVar11 = (undefined4)((ulonglong)dVar7 >> 0x20);
        dVar8 = (double)CONCAT44(uVar11,-SUB84(dVar7,0));
        if (in_FPSCR_PR == '\0') {
          bVar16 = fVar10 < -SUB84(dVar7,0);
        }
        else {
          bVar16 = dVar12 < dVar8;
        }
        if (!bVar16) {
          if (in_FPSCR_SZ == '\0') {
            dVar7 = (double)CONCAT44(*(undefined4 *)((int)puVar6 + -0x88),SUB84(dVar12,0));
            dVar8 = (double)CONCAT44(uVar11,*(undefined4 *)(param_2 + 0x18));
          }
          else {
            dVar7 = *(double *)((int)puVar6 + -0x88);
          }
          fVar10 = (float)((ulonglong)dVar7 >> 0x20);
          if (in_FPSCR_PR == '\0') {
            bVar16 = SUB84(dVar8,0) < fVar10;
          }
          else {
            bVar16 = dVar8 < dVar7;
          }
          if (!bVar16) {
            if (in_FPSCR_SZ == '\0') {
              dVar8 = (double)CONCAT44((int)((ulonglong)dVar8 >> 0x20),
                                       *(undefined4 *)(param_2 + 0x18));
            }
            if (in_FPSCR_PR == '\0') {
              bVar16 = fVar10 < -SUB84(dVar8,0);
            }
            else {
              bVar16 = dVar7 < (double)CONCAT44((int)((ulonglong)dVar8 >> 0x20),-SUB84(dVar8,0));
            }
            if (!bVar16) {
              uVar11 = *(undefined4 *)((int)param_3 + 4);
              *(undefined4 *)param_4 = *(undefined4 *)param_3;
              *(undefined4 *)((int)param_4 + 4) = uVar11;
              *(undefined4 *)(param_4 + 1) = *(undefined4 *)(param_3 + 1);
              FUN_8c01b3e0(0);
            }
          }
        }
      }
    }
  }
  return 0;
}


/* Address: 8c01bba0 */

undefined4 FUN_8c01bba0(undefined4 param_1,int param_2,undefined4 param_3)

{
  short sVar1;
  int iVar2;
  double in_dr2;
  double dVar3;
  float fVar4;
  float fVar5;
  float fVar6;
  undefined8 uVar7;
  undefined8 uVar8;
  undefined8 in_xd2;
  char in_FPSCR_PR;
  char in_FPSCR_SZ;
  float local_64;
  float fStack_60;
  float local_5c;
  float fStack_58;
  float fStack_54;
  float fStack_50;
  float afStack_4c [17];
  
  (*(code *)PTR_FUN_8c01bce0)(afStack_4c);
  if (in_FPSCR_SZ == '\0') {
    uVar8 = CONCAT44(*(undefined4 *)(param_2 + 0x10),param_1);
    uVar7 = CONCAT44(*(undefined4 *)(param_2 + 8),*(undefined4 *)(param_2 + 0xc));
  }
  else {
    uVar8 = *(undefined8 *)(param_2 + 0x10);
    uVar7 = *(undefined8 *)(param_2 + 8);
  }
  (*(code *)PTR_FUN_8c01bce4)(uVar7,uVar8,afStack_4c);
  sVar1 = *(short *)(param_2 + 4);
  if (sVar1 != 0) {
    (*(code *)PTR_FUN_8c01bce8)(afStack_4c,sVar1,sVar1);
  }
  sVar1 = *(short *)(param_2 + 6);
  if (sVar1 != 0) {
    (*(code *)PTR_FUN_8c01bcec)(afStack_4c,sVar1,sVar1);
  }
  (*(code *)PTR_FUN_8c01bcf0)(afStack_4c);
  (*(code *)PTR_FUN_8c01bcf4)(afStack_4c,param_3,&fStack_58);
  if (in_FPSCR_SZ == '\0') {
    in_dr2 = (double)CONCAT44((int)((ulonglong)in_dr2 >> 0x20),fStack_58);
  }
  else {
    in_xd2 = CONCAT44(fStack_54,fStack_58);
  }
  if (in_FPSCR_PR == '\0') {
    local_64 = ABS(SUB84(in_dr2,0));
  }
  else {
    local_64 = SUB84(ABS(in_dr2),0);
  }
  if (in_FPSCR_SZ == '\0') {
    dVar3 = (double)CONCAT44(*(undefined4 *)(param_2 + 0x14),local_64);
  }
  else {
    local_64 = (float)in_xd2;
    dVar3 = *(double *)(param_2 + 0x14);
  }
  fVar4 = (float)((ulonglong)dVar3 >> 0x20);
  if (in_FPSCR_PR != '\0' || SUB84(dVar3,0) <= fVar4) {
    if (in_FPSCR_SZ == '\0') {
      dVar3 = (double)CONCAT44(fVar4,fStack_54);
    }
    else {
      in_xd2 = CONCAT44(fStack_50,fStack_54);
    }
    if (in_FPSCR_PR == '\0') {
      fStack_60 = ABS(SUB84(dVar3,0));
    }
    else {
      fStack_60 = SUB84(ABS(dVar3),0);
    }
    if (in_FPSCR_SZ == '\0') {
      dVar3 = (double)CONCAT44(*(undefined4 *)(param_2 + 0x18),fStack_60);
    }
    else {
      fStack_60 = (float)in_xd2;
      dVar3 = *(double *)(param_2 + 0x18);
    }
    fVar4 = (float)((ulonglong)dVar3 >> 0x20);
    if (in_FPSCR_PR != '\0' || SUB84(dVar3,0) <= fVar4) {
      if (in_FPSCR_SZ == '\0') {
        dVar3 = (double)CONCAT44(fVar4,fStack_50);
      }
      else {
        in_xd2 = CONCAT44(afStack_4c[0],fStack_50);
      }
      if (in_FPSCR_PR == '\0') {
        local_5c = ABS(SUB84(dVar3,0));
      }
      else {
        local_5c = SUB84(ABS(dVar3),0);
      }
      if (in_FPSCR_SZ == '\0') {
        uVar8 = CONCAT44(*(undefined4 *)(param_2 + 0x1c),local_5c);
      }
      else {
        local_5c = (float)in_xd2;
        fStack_58 = (float)((ulonglong)in_xd2 >> 0x20);
        uVar8 = *(undefined8 *)(param_2 + 0x1c);
      }
      if (in_FPSCR_PR != '\0' || (float)uVar8 <= (float)((ulonglong)uVar8 >> 0x20)) {
        if (in_FPSCR_SZ == '\0') {
          dVar3 = (double)CONCAT44(*(undefined4 *)(param_2 + 0x14),local_64);
        }
        else {
          dVar3 = *(double *)(param_2 + 0x14);
        }
        if (in_FPSCR_PR == '\0') {
          dVar3 = (double)CONCAT44((float)((ulonglong)dVar3 >> 0x20) - SUB84(dVar3,0),SUB84(dVar3,0)
                                  );
        }
        else {
          dVar3 = dVar3 - dVar3;
        }
        iVar2 = 0;
        if (in_FPSCR_SZ == '\0') {
          local_64 = (float)((ulonglong)dVar3 >> 0x20);
          dVar3 = (double)CONCAT44(*(undefined4 *)(param_2 + 0x18),fStack_60);
        }
        else {
          local_64 = SUB84(dVar3,0);
          dVar3 = *(double *)(param_2 + 0x18);
        }
        if (in_FPSCR_PR == '\0') {
          dVar3 = (double)CONCAT44((float)((ulonglong)dVar3 >> 0x20) - SUB84(dVar3,0),SUB84(dVar3,0)
                                  );
        }
        else {
          dVar3 = dVar3 - dVar3;
        }
        if (in_FPSCR_SZ == '\0') {
          fStack_60 = (float)((ulonglong)dVar3 >> 0x20);
          dVar3 = (double)CONCAT44(*(undefined4 *)(param_2 + 0x1c),local_5c);
        }
        else {
          fStack_60 = SUB84(dVar3,0);
          dVar3 = *(double *)(param_2 + 0x1c);
        }
        if (in_FPSCR_PR == '\0') {
          dVar3 = (double)CONCAT44((float)((ulonglong)dVar3 >> 0x20) - SUB84(dVar3,0),SUB84(dVar3,0)
                                  );
        }
        else {
          dVar3 = dVar3 - dVar3;
        }
        fVar5 = (float)((ulonglong)dVar3 >> 0x20);
        local_5c = fVar5;
        fVar4 = local_64;
        fVar6 = fStack_60;
        if (in_FPSCR_SZ != '\0') {
          local_5c = SUB84(dVar3,0);
          fStack_58 = fVar5;
          fVar4 = fStack_60;
          fVar6 = local_5c;
        }
        if (in_FPSCR_PR != '\0' || fVar4 <= fVar6) {
          fVar4 = local_5c;
          fVar6 = fStack_58;
          if (in_FPSCR_SZ == '\0') {
            fVar4 = local_64;
            fVar6 = local_5c;
          }
          if (in_FPSCR_PR == '\0' && fVar6 < fVar4) {
            iVar2 = 2;
          }
        }
        else {
          fVar4 = local_5c;
          fVar6 = fStack_58;
          if (in_FPSCR_SZ == '\0') {
            fVar4 = fStack_60;
            fVar6 = local_5c;
          }
          iVar2 = 1;
          if (in_FPSCR_PR == '\0' && fVar6 < fVar4) {
            iVar2 = 2;
          }
        }
        if (iVar2 == 0) {
          fVar4 = fStack_58;
          fVar6 = fStack_54;
          if (in_FPSCR_SZ == '\0') {
            fVar4 = 0.0;
            fVar6 = fStack_58;
          }
          if (in_FPSCR_PR != '\0' || fVar6 <= fVar4) {
            if (in_FPSCR_SZ == '\0') {
              dVar3 = (double)CONCAT44(*(undefined4 *)(param_2 + 0x14),fVar4);
            }
            else {
              dVar3 = *(double *)(param_2 + 0x14);
            }
            if (in_FPSCR_PR == '\0') {
              dVar3 = (double)CONCAT44(-(float)((ulonglong)dVar3 >> 0x20),SUB84(dVar3,0));
            }
            else {
              dVar3 = -dVar3;
            }
          }
          else if (in_FPSCR_SZ == '\0') {
            dVar3 = (double)CONCAT44(*(undefined4 *)(param_2 + 0x14),fVar4);
          }
          else {
            dVar3 = *(double *)(param_2 + 0x14);
          }
          fVar4 = (float)((ulonglong)dVar3 >> 0x20);
          fStack_58 = fVar4;
          if (in_FPSCR_SZ != '\0') {
            fStack_58 = SUB84(dVar3,0);
            fStack_54 = fVar4;
          }
        }
        else if (iVar2 == 1) {
          fVar4 = fStack_54;
          fVar6 = fStack_50;
          if (in_FPSCR_SZ == '\0') {
            fVar4 = 0.0;
            fVar6 = fStack_54;
          }
          if (in_FPSCR_PR != '\0' || fVar6 <= fVar4) {
            if (in_FPSCR_SZ == '\0') {
              dVar3 = (double)CONCAT44(*(undefined4 *)(param_2 + 0x18),fVar4);
            }
            else {
              dVar3 = *(double *)(param_2 + 0x18);
            }
            if (in_FPSCR_PR == '\0') {
              dVar3 = (double)CONCAT44(-(float)((ulonglong)dVar3 >> 0x20),SUB84(dVar3,0));
            }
            else {
              dVar3 = -dVar3;
            }
          }
          else if (in_FPSCR_SZ == '\0') {
            dVar3 = (double)CONCAT44(*(undefined4 *)(param_2 + 0x18),fVar4);
          }
          else {
            dVar3 = *(double *)(param_2 + 0x18);
          }
          fVar4 = (float)((ulonglong)dVar3 >> 0x20);
          fStack_54 = fVar4;
          if (in_FPSCR_SZ != '\0') {
            fStack_54 = SUB84(dVar3,0);
            fStack_50 = fVar4;
          }
        }
        else if (iVar2 == 2) {
          fVar4 = fStack_50;
          fVar6 = afStack_4c[0];
          if (in_FPSCR_SZ == '\0') {
            fVar4 = 0.0;
            fVar6 = fStack_50;
          }
          if (in_FPSCR_PR != '\0' || fVar6 <= fVar4) {
            if (in_FPSCR_SZ == '\0') {
              dVar3 = (double)CONCAT44(*(undefined4 *)(param_2 + 0x1c),fVar4);
            }
            else {
              dVar3 = *(double *)(param_2 + 0x1c);
            }
            if (in_FPSCR_PR == '\0') {
              dVar3 = (double)CONCAT44(-(float)((ulonglong)dVar3 >> 0x20),SUB84(dVar3,0));
            }
            else {
              dVar3 = -dVar3;
            }
          }
          else if (in_FPSCR_SZ == '\0') {
            dVar3 = (double)CONCAT44(*(undefined4 *)(param_2 + 0x1c),fVar4);
          }
          else {
            dVar3 = *(double *)(param_2 + 0x1c);
          }
          fVar4 = (float)((ulonglong)dVar3 >> 0x20);
          fStack_50 = fVar4;
          if (in_FPSCR_SZ != '\0') {
            fStack_50 = SUB84(dVar3,0);
            afStack_4c[0] = fVar4;
          }
        }
        (*(code *)PTR_FUN_8c01bee0)(afStack_4c);
        (*(code *)PTR_FUN_8c01bee4)(afStack_4c,&fStack_58,param_3);
        return 1;
      }
    }
  }
  return 0;
}


/* Address: 8c01bd3c */

void FUN_8c01bd3c(undefined4 param_1,undefined8 *param_2,undefined8 *param_3)

{
  undefined *puVar1;
  int iVar2;
  float in_fr1;
  double dVar3;
  undefined4 in_fr3;
  float fVar4;
  float fVar5;
  double dVar6;
  undefined4 uVar8;
  double dVar7;
  undefined8 in_xd0;
  undefined8 uVar9;
  undefined8 uVar10;
  undefined8 in_xd4;
  char in_FPSCR_PR;
  char in_FPSCR_SZ;
  
  puVar1 = PTR_DAT_8c01bee8;
  if (*(int *)PTR_DAT_8c01bee8 != 0) {
    if (in_FPSCR_SZ == '\0') {
      param_1 = *(undefined4 *)param_2;
    }
    else {
      in_xd4 = *param_2;
    }
    iVar2 = 0x1e - *(int *)PTR_DAT_8c01bee8;
    if (in_FPSCR_SZ == '\0') {
      in_fr3 = *(undefined4 *)param_3;
      dVar7 = (double)CONCAT44(DAT_8c01beec,param_1);
    }
    else {
      dVar7 = (double)CONCAT44(PTR_DAT_8c01bef0,DAT_8c01beec);
    }
    if (in_FPSCR_PR == '\0') {
      dVar6 = (double)CONCAT44((float)iVar2,in_fr3);
    }
    else {
      dVar6 = (double)iVar2;
    }
    uVar9 = in_xd4;
    if (in_FPSCR_SZ == '\0') {
      uVar9 = in_xd0;
      in_fr1 = SUB84(dVar7,0);
    }
    if (in_FPSCR_PR == '\0') {
      dVar6 = (double)CONCAT44((int)((ulonglong)dVar6 >> 0x20),SUB84(dVar6,0) - SUB84(dVar7,0));
    }
    else {
      dVar6 = dVar6 - dVar7;
    }
    uVar8 = (undefined4)((ulonglong)dVar7 >> 0x20);
    dVar3 = dVar7;
    if (in_FPSCR_SZ == '\0') {
      dVar3 = (double)CONCAT44(uVar8,in_fr1);
    }
    if (in_FPSCR_PR == '\0') {
      fVar5 = (float)((ulonglong)dVar6 >> 0x20);
      fVar4 = SUB84(dVar6,0) * fVar5;
      dVar6 = (double)CONCAT44(fVar5,fVar4);
      fVar5 = (float)((ulonglong)dVar3 >> 0x20);
      dVar3 = (double)CONCAT44(fVar5,fVar5 * fVar4 + SUB84(dVar3,0));
    }
    else {
      dVar6 = dVar6 * dVar6;
    }
    if (in_FPSCR_SZ == '\0') {
      *(int *)param_3 = SUB84(dVar3,0);
    }
    else {
      *param_3 = uVar9;
    }
    if (in_FPSCR_SZ == '\0') {
      dVar6 = (double)CONCAT44((int)((ulonglong)dVar6 >> 0x20),*(undefined4 *)((int)param_3 + 4));
    }
    if (in_FPSCR_SZ == '\0') {
      dVar7 = (double)CONCAT44(uVar8,*(undefined4 *)((int)param_2 + 4));
    }
    else {
      in_xd4 = *(undefined8 *)((int)param_2 + 4);
    }
    if (in_FPSCR_PR == '\0') {
      fVar5 = SUB84(dVar6,0) - SUB84(dVar7,0);
    }
    else {
      fVar5 = SUB84(dVar6 - dVar7,0);
    }
    uVar10 = in_xd4;
    if (in_FPSCR_SZ == '\0') {
      dVar3 = (double)CONCAT44((int)((ulonglong)dVar3 >> 0x20),SUB84(dVar7,0));
      uVar10 = uVar9;
    }
    if (in_FPSCR_PR == '\0') {
      fVar4 = (float)(0x1e - *(int *)puVar1);
      fVar5 = fVar5 * fVar4;
      dVar6 = (double)CONCAT44(fVar4,fVar5);
      fVar4 = (float)((ulonglong)dVar3 >> 0x20);
      dVar3 = (double)CONCAT44(fVar4,fVar4 * fVar5 + SUB84(dVar3,0));
    }
    else {
      dVar6 = (double)(0x1e - *(int *)puVar1);
      dVar6 = dVar6 * dVar6;
    }
    if (in_FPSCR_SZ == '\0') {
      *(int *)((int)param_3 + 4) = SUB84(dVar3,0);
      dVar6 = (double)CONCAT44((int)((ulonglong)dVar6 >> 0x20),*(undefined4 *)(param_3 + 1));
      dVar7 = (double)CONCAT44((int)((ulonglong)dVar7 >> 0x20),*(undefined4 *)(param_2 + 1));
    }
    else {
      *(undefined8 *)((int)param_3 + 4) = uVar10;
      in_xd4 = param_2[1];
    }
    if (in_FPSCR_PR == '\0') {
      fVar5 = SUB84(dVar6,0) - SUB84(dVar7,0);
    }
    else {
      fVar5 = SUB84(dVar6 - dVar7,0);
    }
    if (in_FPSCR_SZ == '\0') {
      dVar3 = (double)CONCAT44((int)((ulonglong)dVar3 >> 0x20),SUB84(dVar7,0));
      in_xd4 = uVar10;
    }
    if (in_FPSCR_PR == '\0') {
      dVar3 = (double)(ulonglong)
                      (uint)((float)((ulonglong)dVar3 >> 0x20) *
                             fVar5 * (float)(0x1e - *(int *)puVar1) + SUB84(dVar3,0));
    }
    if (in_FPSCR_SZ == '\0') {
      *(int *)(param_3 + 1) = SUB84(dVar3,0);
    }
    else {
      param_3[1] = in_xd4;
    }
    *(int *)puVar1 = *(int *)puVar1 + -1;
  }
  return;
}


/* Address: 8c01bd9e */

/* WARNING: Removing unreachable block (ram,0x8c01c0b8) */
/* WARNING: Removing unreachable block (ram,0x8c01c03a) */
/* WARNING: Removing unreachable block (ram,0x8c01be78) */
/* WARNING: Removing unreachable block (ram,0x8c01c048) */
/* WARNING: Removing unreachable block (ram,0x8c01c0f4) */
/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_8c01bd9e(void)

{
  undefined4 uVar1;
  int iVar2;
  uint *puVar3;
  undefined8 *puVar4;
  short sVar5;
  uint uVar6;
  uint *puVar7;
  uint *puVar8;
  int iVar9;
  uint uVar10;
  undefined1 *puVar11;
  undefined4 uVar12;
  float fVar13;
  undefined8 in_dr2;
  double dVar14;
  longlong lVar15;
  uint uVar16;
  undefined4 uVar17;
  double dVar18;
  float fVar20;
  undefined8 uVar19;
  undefined8 uVar21;
  float in_fr13;
  undefined8 in_dr14;
  undefined8 in_xd2;
  undefined8 in_xd12;
  undefined8 in_xd14;
  bool bVar22;
  undefined4 in_PR;
  char in_FPSCR_PR;
  char in_FPSCR_SZ;
  undefined1 auStack_34 [12];
  undefined1 local_28 [4];
  undefined8 uStack_24;
  
  uVar17 = (undefined4)((ulonglong)in_dr2 >> 0x20);
  if (in_FPSCR_SZ == '\0') {
    uStack_24 = CONCAT44((int)in_dr14,(undefined4)uStack_24);
                    /* WARNING: Ignoring partial resolution of indirect */
    uStack_24._0_4_ = (int)((ulonglong)in_dr14 >> 0x20);
    puVar11 = local_28;
  }
  else {
    puVar11 = auStack_34;
    uStack_24 = in_xd14;
  }
  *(undefined4 *)(puVar11 + -4) = in_PR;
  iVar9 = *(int *)PTR_DAT_8c01bef0;
  if (*DAT_8c01bef4 != 0) {
    return;
  }
  *(undefined4 *)(puVar11 + -0xf8) = *DAT_8c01bef8;
  iVar2 = (*(code *)PTR_FUN_8c01befc)();
  if (iVar2 == 0) {
    *DAT_8c01bef8 = 0;
  }
  if (*(int *)(puVar11 + -0xf8) == 1) {
    return;
  }
  uVar12 = 0;
  if (in_FPSCR_SZ == '\0') {
    *(undefined4 *)(puVar11 + -0x74) = 0;
    *(undefined4 *)(puVar11 + -0x70) = 0;
    *(undefined4 *)(puVar11 + -0x6c) = DAT_8c01bf00;
  }
  else {
    *(undefined8 *)(puVar11 + -0x74) = in_xd14;
    *(undefined8 *)(puVar11 + -0x70) = in_xd14;
    in_xd2 = CONCAT44(PTR_DAT_8c01bf04,DAT_8c01bf00);
    *(undefined8 *)(puVar11 + -0x6c) = in_xd2;
  }
  (*(code *)PTR_FUN_8c01bf08)(PTR_DAT_8c01bf04);
  (*(code *)PTR_FUN_8c01bf0c)(0,iVar9 + 0x20);
  iVar2 = *(int *)(iVar9 + 0x18);
  *(int *)(puVar11 + -0xf8) = iVar2;
  if (iVar2 != 0) {
    (*(code *)PTR_FUN_8c01bf10)(0,*(undefined4 *)(puVar11 + -0xf8));
  }
  iVar9 = *(int *)(iVar9 + 0x14);
  *(int *)(puVar11 + -0xf8) = iVar9;
  if (iVar9 != 0) {
    (*(code *)PTR_FUN_8c01bf14)(0,*(undefined4 *)(puVar11 + -0xf8));
  }
  (*(code *)PTR_FUN_8c01bee4)(0,puVar11 + -0x74,puVar11 + -0x74);
  if (in_FPSCR_SZ == '\0') {
    *(undefined4 *)(puVar11 + -0x94) = *(undefined4 *)(puVar11 + -0x74);
    *(undefined4 *)(puVar11 + -0x90) = *(undefined4 *)(puVar11 + -0x70);
    *(undefined4 *)(puVar11 + -0x8c) = *(undefined4 *)(puVar11 + -0x6c);
    *(undefined4 *)(puVar11 + -0x88) = uVar12;
    *(undefined4 *)(puVar11 + -0x84) = uVar12;
  }
  else {
    *(undefined8 *)(puVar11 + -0x94) = *(undefined8 *)(puVar11 + -0x74);
    *(undefined8 *)(puVar11 + -0x90) = *(undefined8 *)(puVar11 + -0x70);
    in_xd2 = *(undefined8 *)(puVar11 + -0x6c);
    *(undefined8 *)(puVar11 + -0x8c) = in_xd2;
    *(undefined8 *)(puVar11 + -0x88) = in_xd14;
    *(undefined8 *)(puVar11 + -0x84) = in_xd14;
  }
  dVar14 = (double)CONCAT44(uVar17,0xbf800000);
  if (in_FPSCR_SZ == '\0') {
    *(undefined4 *)(puVar11 + -0x80) = 0xbf800000;
  }
  else {
    *(undefined8 *)(puVar11 + -0x80) = in_xd2;
  }
  (*(code *)PTR_FUN_8c01bf18)(0,puVar11 + -0x88,puVar11 + -0x88);
  (*(code *)PTR_FUN_8c01bf1c)(1);
  *(undefined4 *)(puVar11 + -0x50) = *(undefined4 *)(puVar11 + -0x74);
  *(undefined4 *)(puVar11 + -0x4c) = *(undefined4 *)(puVar11 + -0x70);
  *(undefined4 *)(puVar11 + -0x48) = *(undefined4 *)(puVar11 + -0x6c);
  *(undefined4 *)(puVar11 + -0xf8) = DAT_8c01bf20;
  if (in_FPSCR_SZ == '\0') {
    uVar21 = CONCAT44(*(undefined4 *)(puVar11 + -0x6c),_DAT_8c01bf24);
    dVar18 = (double)(ulonglong)*(uint *)(puVar11 + -0x70);
  }
  else {
    uVar21 = *(undefined8 *)(puVar11 + -0x6c);
    dVar18 = *(double *)(puVar11 + -0x74);
  }
  (*(code *)PTR_FUN_8c01bf28)(uVar21);
  uVar16 = DAT_8c01bf34;
  fVar13 = DAT_8c01bf2c;
  if (in_FPSCR_SZ != '\0') {
    in_xd12 = CONCAT44(DAT_8c01bf30,DAT_8c01bf2c);
    fVar13 = in_fr13;
  }
  uVar6 = 0;
  if (in_FPSCR_SZ == '\0') {
    *(float *)(puVar11 + -0x58) = fVar13;
    uVar21 = CONCAT44(DAT_8c01bf30,uVar12);
    *(undefined4 *)(puVar11 + -100) = DAT_8c01bf30;
  }
  else {
    *(undefined8 *)(puVar11 + -0x58) = in_xd12;
    uVar21 = CONCAT44(DAT_8c01bf34,DAT_8c01bf30);
    *(undefined8 *)(puVar11 + -100) = uVar21;
  }
  puVar3 = *(uint **)(puVar11 + -0xf8);
  puVar7 = puVar3;
  puVar8 = puVar3;
  while( true ) {
    uVar17 = SUB84(dVar18,0);
    uVar12 = (undefined4)uVar21;
    if ((int)*DAT_8c01c128 <= (int)(uVar6 & 0xffff)) break;
    uVar10 = puVar8[1];
    if (puVar8[2] == 0) {
LAB_8c01bf9e:
      if (((*puVar3 & uVar16) != 0) &&
         (iVar9 = (*(code *)PTR_FUN_8c01c124)(puVar11 + -0x50,puVar7[1]), iVar9 != 0)) {
        if (*(int *)(puVar11 + -0x24) == 1) {
          if (in_FPSCR_SZ == '\0') {
            dVar14 = (double)CONCAT44(*(undefined4 *)(puVar11 + -0x4c),
                                      *(undefined4 *)(puVar11 + -0x14));
          }
          else {
            dVar14 = *(double *)(puVar11 + -0x4c);
          }
          if (in_FPSCR_PR == '\0' && (float)((ulonglong)dVar14 >> 0x20) < SUB84(dVar14,0)) {
            if (in_FPSCR_SZ == '\0') {
              dVar14 = (double)CONCAT44(*(undefined4 *)(puVar11 + -0x14),
                                        *(undefined4 *)(puVar11 + -0x58));
            }
            else {
              dVar14 = *(double *)(puVar11 + -0x14);
            }
            fVar20 = (float)((ulonglong)dVar14 >> 0x20);
            if (in_FPSCR_PR == '\0' && fVar20 < SUB84(dVar14,0)) {
              if (in_FPSCR_SZ == '\0') {
                dVar14 = (double)CONCAT44(fVar20,*(undefined4 *)(puVar11 + -0x14));
                *(undefined4 *)(puVar11 + -0x58) = *(undefined4 *)(puVar11 + -0x14);
              }
              else {
                *(undefined8 *)(puVar11 + -0x58) = *(undefined8 *)(puVar11 + -0x14);
              }
              goto LAB_8c01c022;
            }
          }
        }
        if (*(int *)(puVar11 + -0x44) == 1) {
          if (in_FPSCR_SZ == '\0') {
            dVar14 = (double)CONCAT44(*(undefined4 *)(puVar11 + -0x4c),
                                      *(undefined4 *)(puVar11 + -0x34));
          }
          else {
            dVar14 = *(double *)(puVar11 + -0x4c);
          }
          if (in_FPSCR_PR == '\0' && SUB84(dVar14,0) < (float)((ulonglong)dVar14 >> 0x20)) {
            if (in_FPSCR_SZ == '\0') {
              dVar14 = (double)CONCAT44(*(undefined4 *)(puVar11 + -0x34),
                                        *(undefined4 *)(puVar11 + -100));
            }
            else {
              dVar14 = *(double *)(puVar11 + -0x34);
            }
            fVar20 = (float)((ulonglong)dVar14 >> 0x20);
            if (in_FPSCR_PR == '\0' && fVar20 < SUB84(dVar14,0)) {
              if (in_FPSCR_SZ == '\0') {
                dVar14 = (double)CONCAT44(fVar20,*(undefined4 *)(puVar11 + -0x34));
                *(undefined4 *)(puVar11 + -100) = *(undefined4 *)(puVar11 + -0x34);
              }
              else {
                *(undefined8 *)(puVar11 + -100) = *(undefined8 *)(puVar11 + -0x34);
              }
            }
          }
        }
      }
    }
    else {
      if (in_FPSCR_SZ == '\0') {
        dVar14 = (double)CONCAT44(*(undefined4 *)(uVar10 + 8),*(undefined4 *)(puVar11 + -0x74));
      }
      else {
        dVar14 = *(double *)(uVar10 + 8);
      }
      if (in_FPSCR_PR == '\0') {
        dVar14 = (double)CONCAT44((float)((ulonglong)dVar14 >> 0x20) - SUB84(dVar14,0),
                                  SUB84(dVar14,0));
      }
      else {
        dVar14 = dVar14 - dVar14;
      }
      if (in_FPSCR_SZ == '\0') {
        *(int *)(puVar11 + -0xd0) = (int)((ulonglong)dVar14 >> 0x20);
        *(undefined4 *)(puVar11 + -0xcc) = uVar12;
        dVar14 = (double)CONCAT44(*(undefined4 *)(uVar10 + 0x10),*(undefined4 *)(puVar11 + -0x6c));
      }
      else {
        *(double *)(puVar11 + -0xd0) = dVar14;
        *(undefined8 *)(puVar11 + -0xcc) = in_xd14;
        dVar14 = *(double *)(uVar10 + 0x10);
      }
      if (in_FPSCR_PR == '\0') {
        dVar14 = (double)CONCAT44((float)((ulonglong)dVar14 >> 0x20) - SUB84(dVar14,0),
                                  SUB84(dVar14,0));
      }
      else {
        dVar14 = dVar14 - dVar14;
      }
      if (in_FPSCR_SZ == '\0') {
        *(int *)(puVar11 + -200) = (int)((ulonglong)dVar14 >> 0x20);
      }
      else {
        *(double *)(puVar11 + -200) = dVar14;
      }
      uVar12 = (*(code *)PTR_FUN_8c01c120)(puVar11 + -0xd0);
      if (in_FPSCR_SZ == '\0') {
        dVar14 = (double)CONCAT44((int)((ulonglong)dVar14 >> 0x20),*(undefined4 *)(uVar10 + 0x24));
        dVar18 = (double)CONCAT44(*(undefined4 *)(uVar10 + 0x20),uVar17);
      }
      else {
        dVar18 = *(double *)(uVar10 + 0x20);
      }
      fVar20 = (float)((ulonglong)dVar18 >> 0x20);
      if (in_FPSCR_PR == '\0') {
        bVar22 = fVar20 < SUB84(dVar14,0);
      }
      else {
        bVar22 = dVar18 < dVar14;
      }
      if (in_FPSCR_SZ == '\0') {
        dVar18 = (double)CONCAT44(fVar20,uVar12);
      }
      if (bVar22) {
        if (in_FPSCR_SZ == '\0') {
          dVar18 = (double)CONCAT44(*(undefined4 *)(uVar10 + 0x24),SUB84(dVar18,0));
        }
        else {
          dVar18 = *(double *)(uVar10 + 0x24);
        }
      }
      if (in_FPSCR_SZ == '\0') {
        dVar14 = (double)CONCAT44((int)((ulonglong)dVar14 >> 0x20),*(undefined4 *)(uVar10 + 0x28));
      }
      if (in_FPSCR_PR == '\0') {
        bVar22 = (float)((ulonglong)dVar18 >> 0x20) < SUB84(dVar14,0);
      }
      else {
        bVar22 = dVar18 < dVar14;
      }
      if (bVar22) {
        if (in_FPSCR_SZ == '\0') {
          dVar18 = (double)CONCAT44(*(undefined4 *)(uVar10 + 0x28),SUB84(dVar18,0));
        }
        else {
          dVar18 = *(double *)(uVar10 + 0x28);
        }
      }
      if (in_FPSCR_SZ == '\0') {
        dVar14 = (double)CONCAT44((int)((ulonglong)dVar14 >> 0x20),
                                  *(undefined4 *)(*(int *)(uVar10 + 4) + 0x24));
      }
      if (in_FPSCR_PR == '\0') {
        dVar18 = (double)CONCAT44((float)((ulonglong)dVar18 >> 0x20) * SUB84(dVar14,0),
                                  SUB84(dVar18,0));
      }
      else {
        dVar18 = dVar18 * dVar14;
      }
      if (in_FPSCR_PR != '\0' || SUB84(dVar18,0) <= (float)((ulonglong)dVar18 >> 0x20))
      goto LAB_8c01bf9e;
    }
LAB_8c01c022:
    puVar3 = puVar3 + 3;
    puVar8 = puVar8 + 3;
    uVar6 = uVar6 + 1;
    puVar7 = puVar7 + 3;
  }
  if (in_FPSCR_SZ == '\0') {
    dVar14 = (double)CONCAT44((int)((ulonglong)dVar14 >> 0x20),*(undefined4 *)(puVar11 + -0x58));
  }
  if (SUB84(dVar14,0) == fVar13) {
    if (in_FPSCR_SZ == '\0') {
      dVar14 = (double)CONCAT44(*(undefined4 *)(puVar11 + -100),DAT_8c01c12c);
    }
    else {
      dVar14 = *(double *)(puVar11 + -100);
    }
    if ((float)((ulonglong)dVar14 >> 0x20) == SUB84(dVar14,0)) {
      return;
    }
  }
  uVar16 = (uint)((ulonglong)dVar14 >> 0x20);
  uVar19 = CONCAT44(0x3f800000,uVar17);
  if (in_FPSCR_SZ == '\0') {
    *(undefined4 *)(puVar11 + -0xac) = *(undefined4 *)(puVar11 + -0x74);
    *(undefined4 *)(puVar11 + -0xa8) = *(undefined4 *)(puVar11 + -0x58);
    *(undefined4 *)(puVar11 + -0xa4) = *(undefined4 *)(puVar11 + -0x6c);
    *(undefined4 *)(puVar11 + -0xa0) = uVar12;
    *(undefined4 *)(puVar11 + -0x9c) = 0x3f800000;
    *(undefined4 *)(puVar11 + -0x98) = uVar12;
    *(undefined4 *)(puVar11 + -0xc4) = *(undefined4 *)(puVar11 + -0x74);
    *(undefined4 *)(puVar11 + -0xc0) = *(undefined4 *)(puVar11 + -0x58);
    *(undefined4 *)(puVar11 + -0xbc) = *(undefined4 *)(puVar11 + -0x6c);
    *(undefined4 *)(puVar11 + -0xb8) = uVar12;
    *(undefined4 *)(puVar11 + -0xb4) = 0x3f800000;
    *(undefined4 *)(puVar11 + -0xb0) = uVar12;
  }
  else {
    *(undefined8 *)(puVar11 + -0xac) = *(undefined8 *)(puVar11 + -0x74);
    *(undefined8 *)(puVar11 + -0xa8) = *(undefined8 *)(puVar11 + -0x58);
    *(undefined8 *)(puVar11 + -0xa4) = *(undefined8 *)(puVar11 + -0x6c);
    *(undefined8 *)(puVar11 + -0xa0) = in_xd14;
    *(undefined8 *)(puVar11 + -0x9c) = uVar19;
    *(undefined8 *)(puVar11 + -0x98) = in_xd14;
    *(undefined8 *)(puVar11 + -0xc4) = *(undefined8 *)(puVar11 + -0x74);
    *(undefined8 *)(puVar11 + -0xc0) = *(undefined8 *)(puVar11 + -0x58);
    *(undefined8 *)(puVar11 + -0xbc) = *(undefined8 *)(puVar11 + -0x6c);
    *(undefined8 *)(puVar11 + -0xb8) = in_xd14;
    *(undefined8 *)(puVar11 + -0xb4) = uVar19;
    *(undefined8 *)(puVar11 + -0xb0) = in_xd14;
  }
  fVar13 = (float)(*(code *)PTR_FUN_8c01c130)(puVar11 + -0x94,puVar11 + -0xac,puVar11 + -0xdc);
  uVar17 = (undefined4)uVar19;
  if (fVar13 == 0.0) {
    if (in_FPSCR_SZ == '\0') {
      *(undefined4 *)(puVar11 + -0x5c) = *(undefined4 *)(puVar11 + -0xdc);
      *(undefined4 *)(puVar11 + -0x58) = *(undefined4 *)(puVar11 + -0xd8);
      lVar15 = (ulonglong)*(uint *)(puVar11 + -0xd4) << 0x20;
      *(uint *)(puVar11 + -0x54) = *(uint *)(puVar11 + -0xd4);
    }
    else {
      *(undefined8 *)(puVar11 + -0x5c) = *(undefined8 *)(puVar11 + -0xdc);
      *(undefined8 *)(puVar11 + -0x58) = *(undefined8 *)(puVar11 + -0xd8);
      lVar15 = *(longlong *)(puVar11 + -0xd4);
      *(longlong *)(puVar11 + -0x54) = lVar15;
    }
    uVar16 = (uint)((ulonglong)lVar15 >> 0x20);
  }
  else if (in_FPSCR_SZ == '\0') {
    *(int *)(puVar11 + -0x58) = (int)((ulonglong)uVar21 >> 0x20);
  }
  else {
    *(undefined8 *)(puVar11 + -0x58) = uVar21;
  }
  fVar13 = (float)(*(code *)PTR_FUN_8c01c130)(puVar11 + -0x94,puVar11 + -0xc4,puVar11 + -0xdc);
  if (fVar13 == 0.0) {
    if (in_FPSCR_SZ == '\0') {
      *(undefined4 *)(puVar11 + -0x68) = *(undefined4 *)(puVar11 + -0xdc);
      *(undefined4 *)(puVar11 + -100) = *(undefined4 *)(puVar11 + -0xd8);
      lVar15 = (ulonglong)*(uint *)(puVar11 + -0xd4) << 0x20;
      *(uint *)(puVar11 + -0x60) = *(uint *)(puVar11 + -0xd4);
    }
    else {
      *(undefined8 *)(puVar11 + -0x68) = *(undefined8 *)(puVar11 + -0xdc);
      *(undefined8 *)(puVar11 + -100) = *(undefined8 *)(puVar11 + -0xd8);
      lVar15 = *(longlong *)(puVar11 + -0xd4);
      *(longlong *)(puVar11 + -0x60) = lVar15;
    }
    uVar16 = (uint)((ulonglong)lVar15 >> 0x20);
  }
  else if (in_FPSCR_SZ == '\0') {
    *(int *)(puVar11 + -0x58) = (int)((ulonglong)uVar21 >> 0x20);
  }
  else {
    *(undefined8 *)(puVar11 + -0x58) = uVar21;
  }
  (*(code *)PTR_FUN_8c01c300)(0);
  uVar12 = (undefined4)uVar21;
  (*(code *)PTR_FUN_8c01c304)(0,puVar11 + -0x5c,puVar11 + -0x7c);
  (*(code *)PTR_FUN_8c01c308)(1);
  dVar14 = (double)((ulonglong)uVar16 << 0x20);
  if (in_FPSCR_SZ == '\0') {
    dVar18 = (double)CONCAT44(*(undefined4 *)(puVar11 + -0x78),uVar17);
  }
  else {
    dVar18 = *(double *)(puVar11 + -0x78);
  }
  fVar13 = (float)((ulonglong)dVar18 >> 0x20);
  if (in_FPSCR_PR == '\0') {
    bVar22 = fVar13 < 0.0;
  }
  else {
    bVar22 = dVar18 < dVar14;
  }
  if (!bVar22) {
    if (in_FPSCR_SZ == '\0') {
      dVar14 = (double)((ulonglong)DAT_8c01c30c << 0x20);
    }
    else {
      dVar14 = (double)CONCAT44(_DAT_8c01c310,DAT_8c01c30c);
    }
    if (in_FPSCR_PR == '\0') {
      bVar22 = (float)((ulonglong)dVar14 >> 0x20) < fVar13;
    }
    else {
      bVar22 = dVar14 < dVar18;
    }
    if (!bVar22) goto LAB_8c01c192;
  }
  (*(code *)PTR_FUN_8c01c300)(0);
  (*(code *)PTR_FUN_8c01c304)(0,puVar11 + -0x68,puVar11 + -0x7c);
  (*(code *)PTR_FUN_8c01c308)(1);
LAB_8c01c192:
  uVar17 = DAT_8c01c318;
  *(undefined1 **)(puVar11 + -0xd4) = puVar11 + -0xe4;
  *(undefined1 **)(puVar11 + -0xd0) = puVar11 + -0xec;
  *(undefined4 *)(puVar11 + -0xcc) = 0;
  *(undefined4 *)(puVar11 + -200) = 2;
  if (in_FPSCR_SZ == '\0') {
    *(undefined4 *)*(undefined8 **)(puVar11 + -0xd4) = uVar12;
  }
  else {
    **(undefined8 **)(puVar11 + -0xd4) = in_xd14;
  }
  if (in_FPSCR_SZ == '\0') {
    uVar21 = CONCAT44(_DAT_8c01c310,uVar12);
    *(undefined4 *)(*(int *)(puVar11 + -0xd4) + 8) = _DAT_8c01c310;
  }
  else {
    uVar21 = CONCAT44(DAT_8c01c314,_DAT_8c01c310);
    *(undefined8 *)(*(int *)(puVar11 + -0xd4) + 8) = uVar21;
  }
  uVar12 = DAT_8c01c318;
  uVar1 = DAT_8c01c314;
  if (in_FPSCR_SZ == '\0') {
    uVar19 = CONCAT44(DAT_8c01c314,SUB84(dVar18,0));
    *(undefined4 *)(*(int *)(puVar11 + -0xd4) + 4) = DAT_8c01c314;
    uVar12 = uVar1;
  }
  else {
    uVar19 = CONCAT44(DAT_8c01c318,DAT_8c01c314);
    *(undefined8 *)(*(int *)(puVar11 + -0xd4) + 4) = uVar19;
  }
  if (in_FPSCR_SZ == '\0') {
    *(undefined4 *)(*(int *)(puVar11 + -0xd4) + 0xc) = uVar12;
  }
  else {
    *(undefined8 *)(*(int *)(puVar11 + -0xd4) + 0xc) = uVar19;
  }
  **(undefined4 **)(puVar11 + -0xd0) = uVar17;
  *(undefined4 *)(*(int *)(puVar11 + -0xd0) + 4) = uVar17;
  uVar17 = DAT_8c01c31c;
  if (in_FPSCR_SZ == '\0') {
    uVar17 = (int)uVar19;
  }
  (*(code *)PTR_FUN_8c01c320)(puVar11 + -0xd4,2,0x40);
  if (in_FPSCR_SZ == '\0') {
    dVar14 = (double)CONCAT44((int)((ulonglong)dVar14 >> 0x20),*(undefined4 *)(puVar11 + -0x78));
  }
  if (in_FPSCR_PR == '\0') {
    sVar5 = (short)(int)SUB84(dVar14,0);
  }
  else {
    sVar5 = (short)(int)dVar14;
  }
  if ((sVar5 < 0) || (0x1e0 < sVar5)) {
    sVar5 = 0;
  }
  puVar4 = (undefined8 *)(puVar11 + -0xf4);
  *(undefined8 **)(puVar11 + -0xd4) = puVar4;
  *(undefined **)(puVar11 + -0xd0) = PTR_DAT_8c01c324;
  *(undefined4 *)(puVar11 + -0xcc) = 0;
  *(undefined4 *)(puVar11 + -200) = 4;
  if (in_FPSCR_SZ == '\0') {
    *(int *)puVar4 = (int)uVar21;
  }
  else {
    *puVar4 = in_xd14;
  }
  if (in_FPSCR_PR == '\0') {
    dVar14 = (double)CONCAT44((float)(int)sVar5,uVar17);
  }
  else {
    dVar14 = (double)(int)sVar5;
  }
  uVar17 = (undefined4)((ulonglong)dVar14 >> 0x20);
  uVar12 = (undefined4)((ulonglong)uVar21 >> 0x20);
  if (in_FPSCR_SZ == '\0') {
    *(undefined4 *)(puVar11 + -0xf0) = uVar17;
    *(int *)(puVar11 + -0xec) = (int)uVar21;
    *(uint *)(puVar11 + -0xe8) = DAT_8c01c30c;
    *(undefined4 *)(puVar11 + -0xe4) = uVar12;
    *(uint *)(puVar11 + -0xe0) = DAT_8c01c30c;
    *(undefined4 *)(puVar11 + -0xdc) = uVar12;
    *(undefined4 *)(puVar11 + -0xd8) = uVar17;
  }
  else {
    *(double *)(puVar11 + -0xf0) = dVar14;
    *(undefined8 *)(puVar11 + -0xec) = in_xd14;
    *(ulonglong *)(puVar11 + -0xe8) = CONCAT44(_DAT_8c01c310,DAT_8c01c30c);
    *(undefined8 *)(puVar11 + -0xe4) = uVar21;
    *(ulonglong *)(puVar11 + -0xe0) = CONCAT44(_DAT_8c01c310,DAT_8c01c30c);
    *(undefined8 *)(puVar11 + -0xdc) = uVar21;
    *(double *)(puVar11 + -0xd8) = dVar14;
  }
  (*(code *)PTR_FUN_8c01c328)(0,8);
  uVar17 = (undefined4)uVar21;
  (*(code *)PTR_FUN_8c01c328)(1,10);
  uVar21 = in_xd14;
  if (in_FPSCR_SZ == '\0') {
    uVar21 = CONCAT44(uVar17,uVar17);
  }
  (*(code *)PTR_FUN_8c01c32c)(uVar21,uVar21);
  if (in_FPSCR_SZ == '\0') {
    in_xd14 = CONCAT44(uVar17,(int)uVar21);
  }
  (*(code *)PTR_FUN_8c01c330)(in_xd14,puVar11 + -0xd4,4,0x60);
  if (in_FPSCR_SZ == '\0') {
    uVar21 = CONCAT44((undefined4)DAT_8c01c334,(int)in_xd14);
  }
  else {
    uVar21 = CONCAT44(DAT_8c01c334._4_4_,(undefined4)DAT_8c01c334);
  }
  (*(code *)PTR_FUN_8c01c330)(uVar21,puVar11 + -0xd4,4,0x60);
  return;
}


/* Address: 8c01c28e */

void FUN_8c01c28e(void)

{
  (*(code *)PTR_FUN_8c01c33c)(DAT_8c01c334._4_4_);
  return;
}


/* Address: 8c01c29a */

void FUN_8c01c29a(void)

{
  int *piVar1;
  undefined4 uVar2;
  int iVar3;
  undefined4 *puVar4;
  undefined4 *puVar5;
  
  piVar1 = DAT_8c01c344;
  puVar5 = DAT_8c01c340 + 0x96;
  for (puVar4 = DAT_8c01c340; puVar4 < puVar5; puVar4 = puVar4 + 6) {
    iVar3 = *piVar1;
    uVar2 = *(undefined4 *)(iVar3 + 0x24);
    *puVar4 = *(undefined4 *)(iVar3 + 0x20);
    puVar4[1] = uVar2;
    puVar4[2] = *(undefined4 *)(iVar3 + 0x28);
    iVar3 = *piVar1;
    uVar2 = *(undefined4 *)(iVar3 + 0x24);
    puVar4[3] = *(undefined4 *)(iVar3 + 0x20);
    puVar4[4] = uVar2;
    puVar4[5] = *(undefined4 *)(iVar3 + 0x28);
  }
  *DAT_8c01c348 = 0;
  return;
}


/* Address: 8c01c2d8 */

void FUN_8c01c2d8(void)

{
  int *piVar1;
  int *piVar2;
  double *pdVar3;
  int iVar4;
  int iVar5;
  undefined8 *puVar6;
  undefined8 *puVar7;
  double dVar8;
  double dVar9;
  undefined8 in_dr12;
  double dVar10;
  undefined8 in_xd2;
  bool bVar11;
  undefined4 in_PR;
  char in_FPSCR_PR;
  char in_FPSCR_SZ;
  undefined8 local_1c;
  
  piVar2 = DAT_8c01c464;
  iVar4 = DAT_8c01c460;
  piVar1 = DAT_8c01c348;
  if (in_FPSCR_SZ == '\0') {
    puVar7 = (undefined8 *)((int)&local_1c + 4);
    local_1c = CONCAT44((int)((ulonglong)in_dr12 >> 0x20),(undefined4)local_1c);
  }
  else {
    puVar7 = &local_1c;
    local_1c = in_dr12;
  }
  *(undefined4 *)((int)puVar7 + -4) = in_PR;
  if (*DAT_8c01c348 == 0) {
    iVar5 = 0x31;
  }
  else {
    iVar5 = *DAT_8c01c348 + -1;
  }
  puVar6 = (undefined8 *)(iVar5 * 0xc + DAT_8c01c460);
  if (in_FPSCR_SZ == '\0') {
    dVar9 = (double)CONCAT44(*(undefined4 *)(*DAT_8c01c464 + 0x20),*(undefined4 *)puVar6);
  }
  else {
    dVar9 = *(double *)(*DAT_8c01c464 + 0x20);
    in_xd2 = *puVar6;
  }
  if (in_FPSCR_PR == '\0') {
    dVar9 = (double)CONCAT44((float)((ulonglong)dVar9 >> 0x20) - SUB84(dVar9,0),SUB84(dVar9,0));
  }
  else {
    dVar9 = dVar9 - dVar9;
  }
  if (in_FPSCR_SZ == '\0') {
    *(int *)((int)puVar7 + -0x10) = (int)((ulonglong)dVar9 >> 0x20);
    dVar9 = (double)(ulonglong)*(uint *)((int)puVar6 + 4);
  }
  else {
    *(double *)((int)puVar7 + -0x10) = dVar9;
    in_xd2 = *(undefined8 *)((int)puVar6 + 4);
  }
  if (in_FPSCR_SZ == '\0') {
    dVar9 = (double)CONCAT44(*(undefined4 *)(*DAT_8c01c464 + 0x24),SUB84(dVar9,0));
  }
  else {
    dVar9 = *(double *)(*DAT_8c01c464 + 0x24);
  }
  if (in_FPSCR_PR == '\0') {
    dVar9 = (double)CONCAT44((float)((ulonglong)dVar9 >> 0x20) - SUB84(dVar9,0),SUB84(dVar9,0));
  }
  else {
    dVar9 = dVar9 - dVar9;
  }
  dVar10 = dVar9;
  if (in_FPSCR_SZ == '\0') {
    dVar10 = (double)CONCAT44((int)((ulonglong)dVar9 >> 0x20),(int)in_dr12);
  }
  iVar5 = (*(code *)PTR_FUN_8c01c468)(0);
  if (in_FPSCR_SZ == '\0') {
    dVar9 = (double)CONCAT44((int)((ulonglong)dVar9 >> 0x20),*(undefined4 *)(iVar5 + 0x118));
  }
  else {
    in_xd2 = *(undefined8 *)(iVar5 + 0x118);
  }
  if (in_FPSCR_PR == '\0') {
    dVar10 = (double)CONCAT44((float)((ulonglong)dVar10 >> 0x20) + SUB84(dVar9,0),SUB84(dVar10,0));
  }
  else {
    dVar10 = dVar10 + dVar9;
  }
  if (in_FPSCR_SZ == '\0') {
    *(int *)((int)puVar7 + -0xc) = (int)((ulonglong)dVar10 >> 0x20);
    dVar9 = (double)(ulonglong)*(uint *)(puVar6 + 1);
  }
  else {
    *(double *)((int)puVar7 + -0xc) = dVar10;
    in_xd2 = puVar6[1];
  }
  if (in_FPSCR_SZ == '\0') {
    dVar9 = (double)CONCAT44(*(undefined4 *)(*piVar2 + 0x28),SUB84(dVar9,0));
  }
  else {
    dVar9 = *(double *)(*piVar2 + 0x28);
  }
  if (in_FPSCR_PR == '\0') {
    dVar9 = (double)CONCAT44((float)((ulonglong)dVar9 >> 0x20) - SUB84(dVar9,0),SUB84(dVar9,0));
  }
  else {
    dVar9 = dVar9 - dVar9;
  }
  if (in_FPSCR_SZ == '\0') {
    *(int *)((int)puVar7 + -8) = (int)((ulonglong)dVar9 >> 0x20);
  }
  else {
    *(double *)((int)puVar7 + -8) = dVar9;
  }
  dVar8 = (double)(*(code *)PTR_FUN_8c01c46c)((undefined1 *)((int)puVar7 + -0x10));
  piVar2 = DAT_8c01c464;
  if (in_FPSCR_PR == '\0') {
    bVar11 = 1.0 < (float)((ulonglong)dVar8 >> 0x20);
  }
  else {
    bVar11 = (double)CONCAT44((int)((ulonglong)dVar9 >> 0x20),0x3f800000) < dVar8;
  }
  if (bVar11) {
    if (in_FPSCR_SZ == '\0') {
      dVar9 = (double)CONCAT44(*(undefined4 *)(*DAT_8c01c464 + 0x20),0x3f800000);
    }
    else {
      dVar9 = *(double *)(*DAT_8c01c464 + 0x20);
    }
    pdVar3 = (double *)(*piVar1 * 0xc + iVar4);
    if (in_FPSCR_SZ == '\0') {
      *(int *)pdVar3 = (int)((ulonglong)dVar9 >> 0x20);
    }
    else {
      *pdVar3 = dVar9;
    }
    iVar5 = *piVar2;
    if (in_FPSCR_SZ == '\0') {
      dVar10 = (double)CONCAT44(*(undefined4 *)(iVar5 + 0x24),SUB84(dVar10,0));
    }
    else {
      dVar10 = *(double *)(iVar5 + 0x24);
    }
    pdVar3 = (double *)(*piVar1 * 0xc + iVar4 + 4);
    iVar5 = (*(code *)PTR_FUN_8c01c468)(0);
    if (in_FPSCR_SZ == '\0') {
      dVar9 = (double)CONCAT44((int)((ulonglong)dVar9 >> 0x20),*(undefined4 *)(iVar5 + 0x118));
    }
    else {
      in_xd2 = *(undefined8 *)(iVar5 + 0x118);
    }
    if (in_FPSCR_PR == '\0') {
      dVar10 = (double)CONCAT44((float)((ulonglong)dVar10 >> 0x20) + SUB84(dVar9,0),SUB84(dVar10,0))
      ;
    }
    else {
      dVar10 = dVar10 + dVar9;
    }
    if (in_FPSCR_SZ == '\0') {
      *(int *)pdVar3 = (int)((ulonglong)dVar10 >> 0x20);
    }
    else {
      *pdVar3 = dVar10;
    }
    iVar5 = *piVar2;
    if (in_FPSCR_SZ == '\0') {
      dVar9 = (double)(ulonglong)*(uint *)(iVar5 + 0x28);
    }
    else {
      in_xd2 = *(undefined8 *)(iVar5 + 0x28);
    }
    iVar4 = *piVar1 * 0xc + iVar4;
    if (in_FPSCR_SZ == '\0') {
      *(int *)(iVar4 + 8) = SUB84(dVar9,0);
    }
    else {
      *(undefined8 *)(iVar4 + 8) = in_xd2;
    }
    iVar4 = *piVar1;
    *piVar1 = iVar4 + 1;
    if (0x31 < iVar4 + 1) {
      *piVar1 = 0;
    }
  }
  return;
}


/* Address: 8c01da3e */

/* WARNING: Removing unreachable block (ram,0x8c01dafc) */
/* WARNING: Removing unreachable block (ram,0x8c01dac0) */
/* WARNING: Removing unreachable block (ram,0x8c01dae8) */
/* WARNING: Removing unreachable block (ram,0x8c01db64) */
/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_8c01da3e(undefined8 param_1,int param_2,undefined2 param_3,undefined2 param_4)

{
  short sVar1;
  ushort uVar2;
  undefined8 *puVar3;
  float fVar4;
  float in_fr3;
  float fVar6;
  double dVar5;
  double dVar7;
  float fVar8;
  bool bVar9;
  char in_FPSCR_PR;
  char in_FPSCR_SZ;
  undefined8 local_18;
  undefined2 uStack_10;
  undefined2 local_c;
  
  if (in_FPSCR_SZ == '\0') {
    puVar3 = (undefined8 *)((int)&local_18 + 4);
    local_18 = CONCAT44((int)((ulonglong)param_1 >> 0x20),(undefined4)local_18);
  }
  else {
    puVar3 = &local_18;
    local_18 = param_1;
  }
  *(undefined **)((int)puVar3 + -4) = PTR_s__10_4f_8c01db20;
  uStack_10 = param_4;
  local_c = param_3;
  (*(code *)PTR_FUN_8c01db24)(param_2);
  sVar1 = *(short *)((int)puVar3 + 4);
  if (((((sVar1 == 0) && ((*(uint *)(*DAT_8c01db28 + 8) & 0x400) != 0)) ||
       ((sVar1 == 1 && ((*(uint *)(*DAT_8c01db28 + 8) & 0x200) != 0)))) ||
      ((sVar1 == 2 && ((*(uint *)(*DAT_8c01db28 + 8) & 2) != 0)))) &&
     ((*(short *)((int)puVar3 + 8) == *DAT_8c01db2c &&
      (uVar2 = *DAT_8c01db30, *DAT_8c01db30 = uVar2 + 1, (uVar2 & 8) != 0)))) {
    if (in_FPSCR_SZ == '\0') {
      dVar7 = (double)CONCAT44(*(undefined4 *)DAT_8c01db34,(int)param_1);
      in_fr3 = (float)DAT_8c01db38;
    }
    else {
      dVar7 = *DAT_8c01db34;
    }
    fVar8 = (float)((ulonglong)dVar7 >> 0x20);
    if (in_fr3 == fVar8) {
      *(undefined1 *)(param_2 + 7) = 0x20;
      return;
    }
    if (in_FPSCR_SZ == '\0') {
      dVar5 = (double)CONCAT44(DAT_8c01db38._4_4_,in_fr3);
    }
    else {
      dVar5 = (double)CONCAT44(_DAT_8c01db40,DAT_8c01db38._4_4_);
    }
    fVar6 = (float)((ulonglong)dVar5 >> 0x20);
    if (in_FPSCR_PR == '\0') {
      bVar9 = fVar6 == fVar8;
    }
    else {
      bVar9 = dVar5 == dVar7;
    }
    if (bVar9) {
      *(undefined1 *)(param_2 + 6) = 0x20;
      return;
    }
    fVar4 = 1.0;
    if (fVar8 == 1.0) {
      *(undefined1 *)(param_2 + 4) = 0x20;
      return;
    }
    if (in_FPSCR_SZ == '\0') {
      dVar5 = (double)CONCAT44(fVar6,_DAT_8c01db40);
    }
    if (SUB84(dVar5,0) == fVar8) {
      *(undefined1 *)(param_2 + 3) = 0x20;
      return;
    }
    if (in_FPSCR_SZ == '\0') {
      dVar5 = (double)CONCAT44(_DAT_8c01db44,SUB84(dVar5,0));
    }
    else {
      dVar5 = (double)CONCAT44(uRam8c01db48,_DAT_8c01db44);
    }
    if (in_FPSCR_PR == '\0') {
      bVar9 = (float)((ulonglong)dVar5 >> 0x20) == fVar8;
    }
    else {
      bVar9 = dVar5 == dVar7;
    }
    if (bVar9) {
      *(undefined1 *)(param_2 + 2) = 0x20;
      return;
    }
    if (in_FPSCR_SZ == '\0') {
      fVar4 = _DAT_8c01dc80;
    }
    if (fVar4 == fVar8) {
      *(undefined1 *)(param_2 + 1) = 0x20;
    }
  }
  return;
}


/* Address: 8c01db74 */

void FUN_8c01db74(int param_1,undefined4 param_2,short param_3,short param_4)

{
  ushort uVar1;
  int iVar2;
  
  (*(code *)PTR_FUN_8c01dc88)(param_1,PTR_DAT_8c01dc84,param_2);
  if ((((param_4 == 0) && ((*(uint *)(*DAT_8c01dc8c + 8) & 0x400) != 0)) ||
      ((param_4 == 1 && ((*(uint *)(*DAT_8c01dc8c + 8) & 0x200) != 0)))) &&
     ((param_3 == *DAT_8c01dc90 &&
      (uVar1 = *DAT_8c01dc94, *DAT_8c01dc94 = uVar1 + 1, (uVar1 & 8) != 0)))) {
    iVar2 = *DAT_8c01dc98;
    if (iVar2 == 1) {
      *(undefined1 *)(param_1 + 7) = 0x20;
      return;
    }
    if (iVar2 == 0x10) {
      *(undefined1 *)(param_1 + 6) = 0x20;
      return;
    }
    if (iVar2 == 0x100) {
      *(undefined1 *)(param_1 + 5) = 0x20;
      return;
    }
    if (iVar2 == 0x1000) {
      *(undefined1 *)(param_1 + 4) = 0x20;
      return;
    }
    if (iVar2 == DAT_8c01dc9c) {
      *(undefined1 *)(param_1 + 3) = 0x20;
    }
  }
  return;
}


/* Address: 8c01dc36 */

void FUN_8c01dc36(undefined2 *param_1)

{
  int iVar1;
  char in_FPSCR_SZ;
  
  iVar1 = DAT_8c01dca0;
  if (in_FPSCR_SZ == '\0') {
    *(undefined4 *)(param_1 + 2) = *(undefined4 *)(DAT_8c01dca0 + 0x30);
    *(undefined4 *)(param_1 + 4) = *(undefined4 *)(iVar1 + 0x34);
    *(undefined4 *)(param_1 + 6) = *(undefined4 *)(iVar1 + 0x38);
    *(undefined4 *)(param_1 + 8) = *(undefined4 *)(iVar1 + 0x24);
    *(undefined4 *)(param_1 + 10) = *(undefined4 *)(iVar1 + 0x28);
    *(undefined4 *)(param_1 + 0xc) = *(undefined4 *)(iVar1 + 0x2c);
  }
  else {
    *(undefined8 *)(param_1 + 2) = *(undefined8 *)(DAT_8c01dca0 + 0x30);
    *(undefined8 *)(param_1 + 4) = *(undefined8 *)(iVar1 + 0x34);
    *(undefined8 *)(param_1 + 6) = *(undefined8 *)(iVar1 + 0x38);
    *(undefined8 *)(param_1 + 8) = *(undefined8 *)(iVar1 + 0x24);
    *(undefined8 *)(param_1 + 10) = *(undefined8 *)(iVar1 + 0x28);
    *(undefined8 *)(param_1 + 0xc) = *(undefined8 *)(iVar1 + 0x2c);
  }
  *param_1 = *(undefined2 *)(iVar1 + 0x20);
  param_1[1] = *(undefined2 *)(iVar1 + 0x22);
  return;
}


/* Address: 8c01dde0 */

/* WARNING: Removing unreachable block (ram,0x8c01df0e) */
/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_8c01dde0(double param_1,undefined4 param_2)

{
  undefined *puVar1;
  undefined *puVar2;
  short sVar3;
  undefined4 uVar4;
  int iVar5;
  undefined8 *puVar6;
  float fVar7;
  double dVar8;
  double dVar9;
  float fVar10;
  float fVar11;
  float fVar13;
  double dVar12;
  undefined8 in_dr14;
  double in_xd2;
  double in_xd4;
  undefined8 in_xd14;
  bool bVar14;
  undefined4 in_PR;
  uint uVar15;
  char in_FPSCR_PR;
  char in_FPSCR_SZ;
  undefined1 auStack_1c [8];
  undefined8 uStack_14;
  undefined1 *puStack_c;
  
  puVar1 = PTR_DAT_8c01df44;
  puStack_c = &DAT_8c78c608;
  if (in_FPSCR_SZ == '\0') {
    uStack_14 = CONCAT44((int)in_dr14,(undefined4)uStack_14);
    puVar6 = &uStack_14;
                    /* WARNING: Ignoring partial resolution of indirect */
    uStack_14._0_4_ = (int)((ulonglong)in_dr14 >> 0x20);
  }
  else {
    puVar6 = (undefined8 *)auStack_1c;
    uStack_14 = in_xd14;
  }
  *(undefined4 *)((int)puVar6 + -4) = in_PR;
  uVar4 = *(undefined4 *)(PTR_DAT_8c01df40 + 4);
  *(undefined4 *)((int)puVar6 + -0x1c) = *(undefined4 *)PTR_DAT_8c01df40;
  *(undefined4 *)((int)puVar6 + -0x18) = uVar4;
  *(undefined4 *)((int)puVar6 + -0x14) = *(undefined4 *)(PTR_DAT_8c01df40 + 8);
  sVar3 = (*(code *)PTR_FUN_8c01df48)();
  puVar2 = PTR_FUN_8c01df4c;
  if (sVar3 == 0x602) {
    iVar5 = *(int *)puVar1;
    if (in_FPSCR_SZ == '\0') {
      dVar8 = (double)CONCAT44(*(undefined4 *)(iVar5 + 0xc),*(undefined4 *)(iVar5 + 0x18));
    }
    else {
      in_xd2 = *(double *)(iVar5 + 0x18);
      dVar8 = *(double *)(iVar5 + 0xc);
    }
    if (in_FPSCR_PR == '\0') {
      dVar8 = (double)CONCAT44((float)((ulonglong)dVar8 >> 0x20) - SUB84(dVar8,0),SUB84(dVar8,0));
    }
    else {
      dVar8 = dVar8 - dVar8;
    }
    if (in_FPSCR_SZ == '\0') {
      *(int *)((int)puVar6 + -0x10) = (int)((ulonglong)dVar8 >> 0x20);
    }
    else {
      *(double *)((int)puVar6 + -0x10) = dVar8;
    }
    iVar5 = *(int *)puVar1;
    if (in_FPSCR_SZ == '\0') {
      dVar8 = (double)CONCAT44(*(undefined4 *)(iVar5 + 0x10),*(undefined4 *)(iVar5 + 0x1c));
    }
    else {
      in_xd2 = *(double *)(iVar5 + 0x1c);
      dVar8 = *(double *)(iVar5 + 0x10);
    }
    if (in_FPSCR_PR == '\0') {
      dVar8 = (double)CONCAT44((float)((ulonglong)dVar8 >> 0x20) - SUB84(dVar8,0),SUB84(dVar8,0));
    }
    else {
      dVar8 = dVar8 - dVar8;
    }
    if (in_FPSCR_SZ == '\0') {
      *(int *)((int)puVar6 + -0xc) = (int)((ulonglong)dVar8 >> 0x20);
    }
    else {
      *(double *)((int)puVar6 + -0xc) = dVar8;
    }
    iVar5 = *(int *)puVar1;
    if (in_FPSCR_SZ == '\0') {
      dVar8 = (double)CONCAT44(*(undefined4 *)(iVar5 + 0x14),*(undefined4 *)(iVar5 + 0x20));
    }
    else {
      in_xd2 = *(double *)(iVar5 + 0x20);
      dVar8 = *(double *)(iVar5 + 0x14);
    }
    if (in_FPSCR_PR == '\0') {
      dVar8 = (double)CONCAT44((float)((ulonglong)dVar8 >> 0x20) - SUB84(dVar8,0),SUB84(dVar8,0));
    }
    else {
      dVar8 = dVar8 - dVar8;
    }
    if (in_FPSCR_SZ == '\0') {
      *(int *)((int)puVar6 + -8) = (int)((ulonglong)dVar8 >> 0x20);
    }
    else {
      *(double *)((int)puVar6 + -8) = dVar8;
    }
    (*(code *)PTR_FUN_8c01df4c)((undefined1 *)((int)puVar6 + -0x1c));
    (*(code *)PTR_FUN_8c01df50)(0);
    (*(code *)PTR_FUN_8c01df54)(0);
    if (*DAT_8c01df58 != 0) {
      (*(code *)PTR_FUN_8c01df5c)(0,*DAT_8c01df58);
    }
    if (*DAT_8c01df60 != 0) {
      (*(code *)PTR_FUN_8c01df64)(0,*DAT_8c01df60);
    }
    (*(code *)PTR_FUN_8c01df68)
              (0,(undefined1 *)((int)puVar6 + -0x1c),(undefined1 *)((int)puVar6 + -0x1c));
    (*(code *)PTR_FUN_8c01df6c)(1);
    if (in_FPSCR_SZ != '\0') {
      in_xd14 = *(undefined8 *)((int)puVar6 + -0x1c);
      in_xd2 = *(double *)((int)puVar6 + -0x14);
    }
    (*(code *)PTR_FUN_8c01df70)();
    dVar8 = (double)(*(code *)PTR_FUN_8c01df74)();
    if (in_FPSCR_SZ == '\0') {
      dVar8 = (double)CONCAT44((int)((ulonglong)dVar8 >> 0x20),DAT_8c01df78);
    }
    else {
      in_xd2 = (double)CONCAT44(_DAT_8c01df7c,DAT_8c01df78);
    }
    if (in_FPSCR_PR == '\0') {
      dVar8 = (double)CONCAT44((float)((ulonglong)dVar8 >> 0x20) * SUB84(dVar8,0),SUB84(dVar8,0));
    }
    else {
      dVar8 = dVar8 * dVar8;
    }
    if (in_FPSCR_SZ == '\0') {
      *(undefined4 *)((int)puVar6 + -0xc) = 0;
      *(undefined4 *)((int)puVar6 + -0x18) = 0;
    }
    else {
      *(undefined8 *)((int)puVar6 + -0xc) = in_xd14;
      *(undefined8 *)((int)puVar6 + -0x18) = in_xd14;
    }
    if (in_FPSCR_PR == '\0') {
      uVar15 = (uint)(float)((ulonglong)dVar8 >> 0x20);
    }
    else {
      uVar15 = (uint)dVar8;
    }
    (*(code *)puVar2)((undefined1 *)((int)puVar6 + -0x10));
    (*(code *)puVar2)((undefined1 *)((int)puVar6 + -0x1c));
    if (in_FPSCR_SZ == '\0') {
      fVar13 = *(float *)((int)puVar6 + -8);
      dVar9 = (double)CONCAT44(*(undefined4 *)((int)puVar6 + -0x1c),
                               *(undefined4 *)((int)puVar6 + -0x10));
      param_1 = (double)CONCAT44((int)((ulonglong)param_1 >> 0x20),
                                 *(undefined4 *)((int)puVar6 + -0x14));
      dVar8 = (double)CONCAT44((int)((ulonglong)dVar8 >> 0x20),fVar13);
      dVar12 = (double)CONCAT44(fVar13,param_2);
    }
    else {
      dVar9 = *(double *)((int)puVar6 + -0x1c);
      in_xd2 = *(double *)((int)puVar6 + -8);
      in_xd4 = *(double *)((int)puVar6 + -0x10);
      fVar13 = (float)((ulonglong)in_xd2 >> 0x20);
      dVar12 = in_xd2;
    }
    fVar10 = (float)((ulonglong)dVar9 >> 0x20);
    fVar11 = SUB84(param_1,0);
    if (in_FPSCR_PR == '\0') {
      fVar7 = SUB84(dVar8,0) * fVar10;
      dVar12 = (double)CONCAT44(fVar13 * fVar11,SUB84(dVar12,0));
    }
    else {
      fVar7 = SUB84(dVar8 * dVar9,0);
      dVar12 = dVar12 * param_1;
    }
    fVar13 = SUB84(dVar9,0);
    if (in_FPSCR_SZ == '\0') {
      dVar9 = (double)((ulonglong)(uint)fVar10 << 0x20);
      in_xd4 = (double)CONCAT44(fVar13,fVar7);
    }
    if (in_FPSCR_PR == '\0') {
      in_xd4 = (double)CONCAT44((float)((ulonglong)in_xd4 >> 0x20) * fVar11,SUB84(in_xd4,0));
    }
    else {
      in_xd4 = in_xd4 * param_1;
    }
    dVar8 = in_xd2;
    if (in_FPSCR_SZ == '\0') {
      dVar8 = (double)CONCAT44(SUB84(in_xd4,0),fVar11);
    }
    if (in_FPSCR_PR == '\0') {
      dVar12 = (double)CONCAT44((float)((ulonglong)dVar9 >> 0x20) * fVar13 +
                                (float)((ulonglong)dVar12 >> 0x20),SUB84(dVar12,0));
    }
    fVar13 = (float)((ulonglong)in_xd4 >> 0x20);
    if (in_FPSCR_PR == '\0') {
      dVar8 = (double)CONCAT44((float)((ulonglong)dVar8 >> 0x20) - fVar13,SUB84(dVar8,0));
    }
    else {
      dVar8 = dVar8 - in_xd4;
    }
    uVar4 = (undefined4)((ulonglong)dVar12 >> 0x20);
    dVar9 = dVar12;
    if (in_FPSCR_SZ == '\0') {
      in_xd4 = (double)CONCAT44(fVar13,uVar4);
      dVar12 = (double)CONCAT44(uVar4,0x3f800000);
      dVar9 = in_xd2;
    }
    if (in_FPSCR_PR == '\0') {
      fVar13 = SUB84(in_xd4,0) * (float)((ulonglong)dVar12 >> 0x20);
      in_xd4 = (double)(ulonglong)(uint)fVar13;
      fVar13 = SUB84(dVar12,0) - fVar13;
    }
    else {
      in_xd4 = in_xd4 * dVar12;
      fVar13 = SUB84(dVar12 - in_xd4,0);
    }
    if ((_DAT_8c01df7c & uVar15) == 0) {
      uVar15 = uVar15 & 0xffff;
    }
    else {
      uVar15 = DAT_8c01df80 | uVar15;
    }
    if (in_FPSCR_PR == '\0') {
      bVar14 = (float)((ulonglong)dVar8 >> 0x20) < 0.0;
    }
    else {
      bVar14 = dVar8 < (double)((ulonglong)in_xd4 & 0xffffffff);
    }
    if (in_FPSCR_SZ == '\0') {
      dVar9 = (double)CONCAT44((float)(int)uVar15,fVar13);
    }
    if (in_FPSCR_PR == '\0') {
      dVar9 = (double)CONCAT44((float)((ulonglong)dVar9 >> 0x20) * SUB84(dVar9,0),SUB84(dVar9,0));
    }
    else {
      dVar9 = dVar9 * dVar9;
    }
    if (bVar14) {
      if (in_FPSCR_PR == '\0') {
        dVar9 = (double)CONCAT44(-(float)((ulonglong)dVar9 >> 0x20),SUB84(dVar9,0));
      }
      else {
        dVar9 = -dVar9;
      }
    }
    if (in_FPSCR_PR == '\0') {
      iVar5 = (int)(float)((ulonglong)dVar9 >> 0x20);
    }
    else {
      iVar5 = (int)dVar9;
    }
    *(int *)(*(int *)puVar1 + 0x2c) = iVar5;
    *(uint *)(*(int *)puVar1 + 0x2c) = *(uint *)(*(int *)puVar1 + 0x2c) & DAT_8c01df84;
  }
  else {
    *(undefined4 *)(*(int *)puVar1 + 0x2c) = 0;
  }
  return;
}


/* Address: 8c01e32a */

void FUN_8c01e32a(double *param_1,double *param_2)

{
  bool bVar1;
  int iVar2;
  undefined4 uVar3;
  double *pdVar4;
  undefined8 *puVar5;
  double dVar6;
  double dVar7;
  float fVar8;
  float fVar9;
  double dVar10;
  uint uVar11;
  undefined8 in_dr12;
  double in_xd4;
  bool bVar12;
  undefined4 in_PR;
  char in_FPSCR_PR;
  char in_FPSCR_SZ;
  undefined8 local_1c;
  
  if (in_FPSCR_SZ == '\0') {
    puVar5 = (undefined8 *)((int)&local_1c + 4);
    local_1c = CONCAT44((int)((ulonglong)in_dr12 >> 0x20),(undefined4)local_1c);
  }
  else {
    puVar5 = &local_1c;
    local_1c = in_dr12;
  }
  *(undefined4 *)((int)puVar5 + -4) = in_PR;
  bVar1 = false;
  iVar2 = (*(code *)PTR_FUN_8c01e468)();
  uVar3 = (undefined4)in_dr12;
  if (iVar2 != 0) {
    uVar3 = *(undefined4 *)((int)param_2 + 4);
    *(undefined4 *)param_1 = *(undefined4 *)param_2;
    *(undefined4 *)((int)param_1 + 4) = uVar3;
    *(undefined4 *)(param_1 + 1) = *(undefined4 *)(param_2 + 1);
    return;
  }
  if (in_FPSCR_SZ == '\0') {
    dVar10 = (double)CONCAT44(*(undefined4 *)param_2,*(undefined4 *)param_1);
  }
  else {
    dVar10 = *param_2;
  }
  if (in_FPSCR_PR == '\0') {
    dVar10 = (double)CONCAT44((float)((ulonglong)dVar10 >> 0x20) - SUB84(dVar10,0),SUB84(dVar10,0));
  }
  else {
    dVar10 = dVar10 - dVar10;
  }
  if (in_FPSCR_SZ == '\0') {
    *(int *)((int)puVar5 + -0x10) = (int)((ulonglong)dVar10 >> 0x20);
    dVar10 = (double)CONCAT44(*(undefined4 *)((int)param_2 + 4),*(undefined4 *)((int)param_1 + 4));
  }
  else {
    *(double *)((int)puVar5 + -0x10) = dVar10;
    dVar10 = *(double *)((int)param_2 + 4);
  }
  if (in_FPSCR_PR == '\0') {
    dVar10 = (double)CONCAT44((float)((ulonglong)dVar10 >> 0x20) - SUB84(dVar10,0),SUB84(dVar10,0));
  }
  else {
    dVar10 = dVar10 - dVar10;
  }
  if (in_FPSCR_SZ == '\0') {
    *(int *)((int)puVar5 + -0xc) = (int)((ulonglong)dVar10 >> 0x20);
    dVar10 = (double)CONCAT44(*(undefined4 *)(param_2 + 1),*(undefined4 *)(param_1 + 1));
  }
  else {
    *(double *)((int)puVar5 + -0xc) = dVar10;
    dVar10 = param_2[1];
  }
  if (in_FPSCR_PR == '\0') {
    dVar10 = (double)CONCAT44((float)((ulonglong)dVar10 >> 0x20) - SUB84(dVar10,0),SUB84(dVar10,0));
  }
  else {
    dVar10 = dVar10 - dVar10;
  }
  if (in_FPSCR_SZ == '\0') {
    *(int *)((int)puVar5 + -8) = (int)((ulonglong)dVar10 >> 0x20);
  }
  else {
    *(double *)((int)puVar5 + -8) = dVar10;
  }
  if (in_FPSCR_SZ == '\0') {
    dVar10 = (double)CONCAT44(*(undefined4 *)(*DAT_8c01e46c + 0x20),*(undefined4 *)param_1);
  }
  else {
    dVar10 = *(double *)(*DAT_8c01e46c + 0x20);
  }
  if (in_FPSCR_PR == '\0') {
    dVar10 = (double)CONCAT44((float)((ulonglong)dVar10 >> 0x20) - SUB84(dVar10,0),SUB84(dVar10,0));
  }
  else {
    dVar10 = dVar10 - dVar10;
  }
  if (in_FPSCR_SZ == '\0') {
    *(int *)((int)puVar5 + -0x28) = (int)((ulonglong)dVar10 >> 0x20);
    dVar10 = (double)(ulonglong)*(uint *)((int)param_1 + 4);
  }
  else {
    *(double *)((int)puVar5 + -0x28) = dVar10;
  }
  if (in_FPSCR_SZ == '\0') {
    dVar10 = (double)CONCAT44(*(undefined4 *)(*DAT_8c01e46c + 0x24),SUB84(dVar10,0));
  }
  else {
    dVar10 = *(double *)(*DAT_8c01e46c + 0x24);
  }
  if (in_FPSCR_PR == '\0') {
    dVar10 = (double)CONCAT44((float)((ulonglong)dVar10 >> 0x20) - SUB84(dVar10,0),SUB84(dVar10,0));
  }
  else {
    dVar10 = dVar10 - dVar10;
  }
  if (in_FPSCR_SZ == '\0') {
    *(int *)((int)puVar5 + -0x24) = (int)((ulonglong)dVar10 >> 0x20);
    dVar10 = (double)(ulonglong)*(uint *)(param_1 + 1);
  }
  else {
    *(double *)((int)puVar5 + -0x24) = dVar10;
  }
  pdVar4 = (double *)((int)puVar5 + -0x1c);
  if (in_FPSCR_SZ == '\0') {
    dVar10 = (double)CONCAT44(*(undefined4 *)(*DAT_8c01e46c + 0x28),SUB84(dVar10,0));
  }
  else {
    dVar10 = *(double *)(*DAT_8c01e46c + 0x28);
  }
  if (in_FPSCR_PR == '\0') {
    dVar10 = (double)CONCAT44((float)((ulonglong)dVar10 >> 0x20) - SUB84(dVar10,0),SUB84(dVar10,0));
  }
  else {
    dVar10 = dVar10 - dVar10;
  }
  if (in_FPSCR_SZ == '\0') {
    *(int *)((int)puVar5 + -0x20) = (int)((ulonglong)dVar10 >> 0x20);
  }
  else {
    *(double *)((int)puVar5 + -0x20) = dVar10;
  }
  if (in_FPSCR_SZ == '\0') {
    dVar10 = (double)CONCAT44(*(undefined4 *)(*DAT_8c01e46c + 0x20),*(undefined4 *)param_2);
  }
  else {
    dVar10 = *(double *)(*DAT_8c01e46c + 0x20);
  }
  if (in_FPSCR_PR == '\0') {
    dVar10 = (double)CONCAT44((float)((ulonglong)dVar10 >> 0x20) - SUB84(dVar10,0),SUB84(dVar10,0));
  }
  else {
    dVar10 = dVar10 - dVar10;
  }
  if (in_FPSCR_SZ == '\0') {
    *(int *)pdVar4 = (int)((ulonglong)dVar10 >> 0x20);
  }
  else {
    *pdVar4 = dVar10;
  }
  if (in_FPSCR_SZ == '\0') {
    dVar10 = (double)CONCAT44(*(undefined4 *)(*DAT_8c01e46c + 0x24),
                              *(undefined4 *)((int)param_2 + 4));
  }
  else {
    dVar10 = *(double *)(*DAT_8c01e46c + 0x24);
  }
  if (in_FPSCR_PR == '\0') {
    dVar10 = (double)CONCAT44((float)((ulonglong)dVar10 >> 0x20) - SUB84(dVar10,0),SUB84(dVar10,0));
  }
  else {
    dVar10 = dVar10 - dVar10;
  }
  if (in_FPSCR_SZ == '\0') {
    *(int *)((int)puVar5 + -0x18) = (int)((ulonglong)dVar10 >> 0x20);
    dVar10 = (double)(ulonglong)*(uint *)(param_2 + 1);
  }
  else {
    *(double *)((int)puVar5 + -0x18) = dVar10;
  }
  if (in_FPSCR_SZ == '\0') {
    dVar10 = (double)CONCAT44(*(undefined4 *)(*DAT_8c01e46c + 0x28),SUB84(dVar10,0));
  }
  else {
    dVar10 = *(double *)(*DAT_8c01e46c + 0x28);
  }
  if (in_FPSCR_PR == '\0') {
    dVar10 = (double)CONCAT44((float)((ulonglong)dVar10 >> 0x20) - SUB84(dVar10,0),SUB84(dVar10,0));
  }
  else {
    dVar10 = dVar10 - dVar10;
  }
  if (in_FPSCR_SZ == '\0') {
    *(int *)((int)puVar5 + -0x14) = (int)((ulonglong)dVar10 >> 0x20);
  }
  else {
    *(double *)((int)puVar5 + -0x14) = dVar10;
  }
  dVar6 = (double)(*(code *)PTR_FUN_8c01e470)((undefined1 *)((int)puVar5 + -0x28));
  if (in_FPSCR_SZ == '\0') {
    dVar6 = (double)CONCAT44((int)((ulonglong)dVar6 >> 0x20),uVar3);
  }
  dVar7 = (double)(*(code *)PTR_FUN_8c01e470)(pdVar4);
  if (in_FPSCR_PR == '\0') {
    bVar12 = (float)((ulonglong)dVar6 >> 0x20) < (float)((ulonglong)dVar7 >> 0x20);
  }
  else {
    bVar12 = dVar6 < dVar7;
  }
  if (bVar12) {
    iVar2 = (*(code *)PTR_FUN_8c01e478)(param_1,(undefined1 *)((int)puVar5 + -0x10),0);
    if (iVar2 == 0) goto LAB_8c01e4c4;
    if ((*(uint *)(DAT_8c01e458 + 8) & DAT_8c01e47c) == 0) {
      if (in_FPSCR_SZ == '\0') {
        dVar10 = (double)CONCAT44(*(undefined4 *)param_1,*(undefined4 *)((int)puVar5 + -0x10));
      }
      else {
        dVar10 = *param_1;
      }
      if (in_FPSCR_PR == '\0') {
        dVar10 = (double)CONCAT44((float)((ulonglong)dVar10 >> 0x20) + SUB84(dVar10,0),
                                  SUB84(dVar10,0));
      }
      else {
        dVar10 = dVar10 + dVar10;
      }
      if (in_FPSCR_SZ == '\0') {
        *(int *)param_2 = (int)((ulonglong)dVar10 >> 0x20);
        dVar10 = (double)CONCAT44(*(undefined4 *)((int)param_1 + 4),
                                  *(undefined4 *)((int)puVar5 + -0xc));
      }
      else {
        *param_2 = dVar10;
        dVar10 = *(double *)((int)param_1 + 4);
      }
      if (in_FPSCR_PR == '\0') {
        dVar10 = (double)CONCAT44((float)((ulonglong)dVar10 >> 0x20) + SUB84(dVar10,0),
                                  SUB84(dVar10,0));
      }
      else {
        dVar10 = dVar10 + dVar10;
      }
      if (in_FPSCR_SZ == '\0') {
        *(int *)((int)param_2 + 4) = (int)((ulonglong)dVar10 >> 0x20);
        dVar10 = (double)CONCAT44(*(undefined4 *)(param_1 + 1),*(undefined4 *)((int)puVar5 + -8));
      }
      else {
        *(double *)((int)param_2 + 4) = dVar10;
        dVar10 = param_1[1];
      }
      if (in_FPSCR_PR == '\0') {
        dVar10 = (double)CONCAT44((float)((ulonglong)dVar10 >> 0x20) + SUB84(dVar10,0),
                                  SUB84(dVar10,0));
      }
      else {
        dVar10 = dVar10 + dVar10;
      }
      if (in_FPSCR_SZ == '\0') {
        *(int *)(param_2 + 1) = (int)((ulonglong)dVar10 >> 0x20);
      }
      else {
        param_2[1] = dVar10;
      }
      goto LAB_8c01e4c4;
    }
  }
  else {
    iVar2 = (*(code *)PTR_FUN_8c01e624)(param_1,(undefined1 *)((int)puVar5 + -0x10),0);
    if (iVar2 == 0) goto LAB_8c01e4c4;
    if ((*(uint *)(DAT_8c01e628 + 8) & DAT_8c01e62c) == 0) {
      if (in_FPSCR_SZ == '\0') {
        dVar10 = (double)CONCAT44(*(undefined4 *)param_1,*(undefined4 *)((int)puVar5 + -0x10));
      }
      else {
        dVar10 = *param_1;
      }
      if (in_FPSCR_PR == '\0') {
        dVar10 = (double)CONCAT44((float)((ulonglong)dVar10 >> 0x20) + SUB84(dVar10,0),
                                  SUB84(dVar10,0));
      }
      else {
        dVar10 = dVar10 + dVar10;
      }
      if (in_FPSCR_SZ == '\0') {
        *(int *)param_2 = (int)((ulonglong)dVar10 >> 0x20);
        dVar10 = (double)CONCAT44(*(undefined4 *)((int)param_1 + 4),
                                  *(undefined4 *)((int)puVar5 + -0xc));
      }
      else {
        *param_2 = dVar10;
        dVar10 = *(double *)((int)param_1 + 4);
      }
      if (in_FPSCR_PR == '\0') {
        dVar10 = (double)CONCAT44((float)((ulonglong)dVar10 >> 0x20) + SUB84(dVar10,0),
                                  SUB84(dVar10,0));
      }
      else {
        dVar10 = dVar10 + dVar10;
      }
      if (in_FPSCR_SZ == '\0') {
        *(int *)((int)param_2 + 4) = (int)((ulonglong)dVar10 >> 0x20);
        dVar10 = (double)CONCAT44(*(undefined4 *)(param_1 + 1),*(undefined4 *)((int)puVar5 + -8));
      }
      else {
        *(double *)((int)param_2 + 4) = dVar10;
        dVar10 = param_1[1];
      }
      if (in_FPSCR_PR == '\0') {
        dVar10 = (double)CONCAT44((float)((ulonglong)dVar10 >> 0x20) + SUB84(dVar10,0),
                                  SUB84(dVar10,0));
      }
      else {
        dVar10 = dVar10 + dVar10;
      }
      if (in_FPSCR_SZ == '\0') {
        *(int *)(param_2 + 1) = (int)((ulonglong)dVar10 >> 0x20);
      }
      else {
        param_2[1] = dVar10;
      }
      goto LAB_8c01e4c4;
    }
  }
  bVar1 = true;
LAB_8c01e4c4:
  if (bVar1) {
    if (in_FPSCR_SZ == '\0') {
      dVar10 = (double)CONCAT44((int)((ulonglong)dVar10 >> 0x20),*(undefined4 *)param_2);
      dVar6 = (double)CONCAT44(DAT_8c01e630,*(undefined4 *)param_1);
    }
    else {
      in_xd4 = *param_1;
      dVar6 = (double)CONCAT44(DAT_8c01e634,DAT_8c01e630);
    }
    if (in_FPSCR_PR == '\0') {
      fVar8 = SUB84(dVar10,0) - SUB84(dVar6,0);
    }
    else {
      fVar8 = SUB84(dVar10 - dVar6,0);
    }
    uVar11 = (uint)((ulonglong)dVar6 >> 0x20);
    dVar7 = dVar6;
    dVar10 = in_xd4;
    if (in_FPSCR_SZ == '\0') {
      dVar7 = (double)((ulonglong)uVar11 << 0x20);
      dVar10 = (double)CONCAT44(SUB84(dVar6,0),fVar8);
    }
    fVar8 = (float)((ulonglong)dVar7 >> 0x20);
    if (in_FPSCR_PR == '\0') {
      dVar10 = (double)CONCAT44(fVar8 * SUB84(dVar10,0) + (float)((ulonglong)dVar10 >> 0x20),
                                SUB84(dVar10,0));
    }
    uVar3 = (undefined4)((ulonglong)dVar10 >> 0x20);
    if (in_FPSCR_SZ == '\0') {
      *(undefined4 *)param_2 = uVar3;
      dVar10 = (double)CONCAT44(uVar3,*(undefined4 *)((int)param_2 + 4));
      dVar6 = (double)CONCAT44(uVar11,*(undefined4 *)((int)param_1 + 4));
    }
    else {
      *param_2 = dVar10;
      in_xd4 = *(double *)((int)param_1 + 4);
    }
    if (in_FPSCR_PR == '\0') {
      fVar9 = SUB84(dVar10,0) - SUB84(dVar6,0);
    }
    else {
      fVar9 = SUB84(dVar10 - dVar6,0);
    }
    dVar10 = in_xd4;
    if (in_FPSCR_SZ == '\0') {
      dVar10 = (double)CONCAT44(SUB84(dVar6,0),fVar9);
    }
    if (in_FPSCR_PR == '\0') {
      dVar10 = (double)CONCAT44(fVar8 * SUB84(dVar10,0) + (float)((ulonglong)dVar10 >> 0x20),
                                SUB84(dVar10,0));
    }
    uVar3 = (undefined4)((ulonglong)dVar10 >> 0x20);
    if (in_FPSCR_SZ == '\0') {
      *(undefined4 *)((int)param_2 + 4) = uVar3;
      dVar10 = (double)CONCAT44(uVar3,*(undefined4 *)(param_2 + 1));
      dVar6 = (double)CONCAT44((int)((ulonglong)dVar6 >> 0x20),*(undefined4 *)(param_1 + 1));
    }
    else {
      *(double *)((int)param_2 + 4) = dVar10;
      in_xd4 = param_1[1];
    }
    if (in_FPSCR_PR == '\0') {
      fVar9 = SUB84(dVar10,0) - SUB84(dVar6,0);
    }
    else {
      fVar9 = SUB84(dVar10 - dVar6,0);
    }
    if (in_FPSCR_SZ == '\0') {
      in_xd4 = (double)CONCAT44(SUB84(dVar6,0),fVar9);
    }
    if (in_FPSCR_PR == '\0') {
      in_xd4 = (double)CONCAT44(fVar8 * SUB84(in_xd4,0) + (float)((ulonglong)in_xd4 >> 0x20),
                                SUB84(in_xd4,0));
    }
    if (in_FPSCR_SZ == '\0') {
      *(int *)(param_2 + 1) = (int)((ulonglong)in_xd4 >> 0x20);
    }
    else {
      param_2[1] = in_xd4;
    }
  }
  return;
}


/* Address: 8c01f9c0 */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_8c01f9c0(undefined4 param_1)

{
  undefined *puVar1;
  int *piVar2;
  undefined4 uVar3;
  int iVar4;
  undefined4 *puVar5;
  double *pdVar6;
  undefined1 *puVar7;
  double dVar8;
  undefined *puVar12;
  double dVar9;
  double dVar10;
  undefined8 uVar11;
  float in_fr3;
  float fVar13;
  ulonglong uVar14;
  undefined4 in_fr13;
  undefined *in_fr15;
  undefined4 in_fr14;
  double in_xd2;
  undefined8 in_xd14;
  undefined *puVar15;
  undefined *puVar16;
  undefined4 in_PR;
  char in_FPSCR_PR;
  char in_FPSCR_SZ;
  undefined1 local_20 [8];
  undefined1 auStack_18 [8];
  
  puVar15 = (undefined *)((ulonglong)in_xd14 >> 0x20);
  if (in_FPSCR_SZ == '\0') {
    puVar7 = auStack_18;
  }
  else {
    puVar7 = local_20;
  }
  *(undefined4 *)(puVar7 + -4) = in_PR;
  puVar5 = (undefined4 *)(puVar7 + -0x10);
  iVar4 = *(int *)PTR_DAT_8c01fc00;
  uVar3 = *(undefined4 *)(iVar4 + 0x24);
  *puVar5 = *(undefined4 *)(iVar4 + 0x20);
  *(undefined4 *)(puVar7 + -0xc) = uVar3;
  *(undefined4 *)(puVar7 + -8) = *(undefined4 *)(iVar4 + 0x28);
  dVar8 = (double)(*(code *)PTR_FUN_8c01fc08)(*DAT_8c01fc04 + 4);
  puVar1 = PTR_FUN_8c01fc14;
  puVar12 = PTR_FUN_8c01fc14;
  puVar16 = DAT_8c01fc10;
  if (in_FPSCR_SZ == '\0') {
    in_fr3 = (float)((ulonglong)dVar8 >> 0x20);
    puVar12 = DAT_8c01fc10;
    in_fr15 = DAT_8c01fc0c;
    puVar16 = puVar15;
    dVar8 = in_xd2;
  }
  if (in_FPSCR_PR == '\0') {
    in_fr15 = (undefined *)((float)puVar12 * in_fr3 + (float)in_fr15);
  }
  pdVar6 = (double *)(puVar7 + -0x34);
  dVar9 = (double)(*(code *)PTR_FUN_8c01fc14)(0xfffff334);
  if (in_FPSCR_SZ == '\0') {
    dVar9 = (double)CONCAT44((int)((ulonglong)dVar9 >> 0x20),in_fr13);
  }
  dVar10 = (double)(*(code *)PTR_FUN_8c01fc18)(0xffffb8e4);
  if (in_FPSCR_PR == '\0') {
    dVar9 = (double)CONCAT44((float)((ulonglong)dVar9 >> 0x20) * (float)((ulonglong)dVar10 >> 0x20),
                             SUB84(dVar9,0));
  }
  else {
    dVar9 = dVar9 * dVar10;
  }
  if (in_FPSCR_SZ == '\0') {
    *(int *)pdVar6 = (int)((ulonglong)dVar9 >> 0x20);
  }
  else {
    *pdVar6 = dVar9;
  }
  uVar11 = (*(code *)PTR_FUN_8c01fc18)(0xfffff334);
  uVar3 = SUB84(dVar9,0);
  if (in_FPSCR_SZ == '\0') {
    *(int *)(puVar7 + -0x30) = (int)((ulonglong)uVar11 >> 0x20);
  }
  else {
    *(undefined8 *)(puVar7 + -0x30) = uVar11;
  }
  dVar9 = (double)(*(code *)puVar1)(0xfffff334);
  if (in_FPSCR_SZ == '\0') {
    dVar9 = (double)CONCAT44((int)((ulonglong)dVar9 >> 0x20),uVar3);
  }
  dVar10 = (double)(*(code *)puVar1)(0xffffb8e4);
  piVar2 = DAT_8c01fc20;
  puVar15 = PTR_DAT_8c01fc1c;
  if (in_FPSCR_PR == '\0') {
    dVar9 = (double)CONCAT44((float)((ulonglong)dVar9 >> 0x20) * (float)((ulonglong)dVar10 >> 0x20),
                             SUB84(dVar9,0));
  }
  else {
    dVar9 = dVar9 * dVar10;
  }
  if (in_FPSCR_SZ == '\0') {
    *(int *)(puVar7 + -0x2c) = (int)((ulonglong)dVar9 >> 0x20);
    puVar16 = in_fr15;
  }
  else {
    *(double *)(puVar7 + -0x2c) = dVar9;
  }
  if (in_FPSCR_SZ == '\0') {
    uVar11 = CONCAT44(*(undefined4 *)(*DAT_8c01fc20 + 0x20),*(undefined4 *)pdVar6);
  }
  else {
    dVar8 = *pdVar6;
    uVar11 = *(undefined8 *)(*DAT_8c01fc20 + 0x20);
  }
  if (in_FPSCR_PR == '\0') {
    uVar11 = CONCAT44((float)puVar16 * (float)uVar11 + (float)((ulonglong)uVar11 >> 0x20),
                      (float)uVar11);
  }
  if (in_FPSCR_SZ == '\0') {
    *(int *)(*(int *)PTR_DAT_8c01fc1c + 0xc) = (int)((ulonglong)uVar11 >> 0x20);
  }
  else {
    *(undefined8 *)(*(int *)PTR_DAT_8c01fc1c + 0xc) = uVar11;
  }
  if (in_FPSCR_SZ == '\0') {
    uVar14 = CONCAT44(*(undefined4 *)(puVar7 + -0x30),*(undefined4 *)(*piVar2 + 0x24));
  }
  else {
    dVar8 = *(double *)(*piVar2 + 0x24);
    uVar14 = *(ulonglong *)(puVar7 + -0x30);
  }
  if (in_FPSCR_PR == '\0') {
    uVar14 = (ulonglong)(uint)((float)puVar16 * (float)(uVar14 >> 0x20) + (float)uVar14);
  }
  if (in_FPSCR_SZ == '\0') {
    *(int *)(*(int *)puVar15 + 0x10) = (int)uVar14;
  }
  else {
    *(double *)(*(int *)puVar15 + 0x10) = dVar8;
  }
  if (in_FPSCR_SZ == '\0') {
    dVar9 = (double)CONCAT44(*(undefined4 *)(puVar7 + -0x2c),*(undefined4 *)(*piVar2 + 0x28));
  }
  else {
    dVar8 = *(double *)(*piVar2 + 0x28);
    dVar9 = *(double *)(puVar7 + -0x2c);
  }
  if (in_FPSCR_PR == '\0') {
    fVar13 = (float)((ulonglong)dVar9 >> 0x20);
    dVar9 = (double)CONCAT44(fVar13,(float)puVar16 * fVar13 + SUB84(dVar9,0));
  }
  if (in_FPSCR_SZ == '\0') {
    *(int *)(*(int *)puVar15 + 0x14) = SUB84(dVar9,0);
    dVar9 = (double)CONCAT44((int)((ulonglong)dVar9 >> 0x20),*(undefined4 *)pdVar6);
  }
  else {
    *(double *)(*(int *)puVar15 + 0x14) = dVar8;
    dVar8 = *pdVar6;
  }
  if (in_FPSCR_PR == '\0') {
    fVar13 = SUB84(dVar9,0) * (float)in_fr15;
  }
  else {
    fVar13 = SUB84(dVar9 * (double)CONCAT44(in_fr14,in_fr15),0);
  }
  if (in_FPSCR_SZ == '\0') {
    dVar9 = (double)CONCAT44(*(undefined4 *)(*piVar2 + 0x20),fVar13);
  }
  else {
    dVar9 = *(double *)(*piVar2 + 0x20);
  }
  if (in_FPSCR_PR == '\0') {
    dVar9 = (double)CONCAT44((float)((ulonglong)dVar9 >> 0x20) - SUB84(dVar9,0),SUB84(dVar9,0));
  }
  else {
    dVar9 = dVar9 - dVar9;
  }
  uVar3 = (undefined4)((ulonglong)dVar9 >> 0x20);
  if (in_FPSCR_SZ == '\0') {
    *(undefined4 *)(*(int *)puVar15 + 0x18) = uVar3;
    dVar9 = (double)CONCAT44(uVar3,*(undefined4 *)(puVar7 + -0x30));
  }
  else {
    *(double *)(*(int *)puVar15 + 0x18) = dVar9;
    dVar8 = *(double *)(puVar7 + -0x30);
  }
  if (in_FPSCR_PR == '\0') {
    fVar13 = SUB84(dVar9,0) * (float)in_fr15;
  }
  else {
    fVar13 = SUB84(dVar9 * (double)CONCAT44(in_fr14,in_fr15),0);
  }
  if (in_FPSCR_SZ == '\0') {
    dVar9 = (double)CONCAT44(*(undefined4 *)(*piVar2 + 0x24),fVar13);
  }
  else {
    dVar9 = *(double *)(*piVar2 + 0x24);
  }
  if (in_FPSCR_PR == '\0') {
    dVar9 = (double)CONCAT44((float)((ulonglong)dVar9 >> 0x20) - SUB84(dVar9,0),SUB84(dVar9,0));
  }
  else {
    dVar9 = dVar9 - dVar9;
  }
  uVar3 = (undefined4)((ulonglong)dVar9 >> 0x20);
  if (in_FPSCR_SZ == '\0') {
    *(undefined4 *)(*(int *)puVar15 + 0x1c) = uVar3;
    dVar9 = (double)CONCAT44(uVar3,*(undefined4 *)(puVar7 + -0x2c));
  }
  else {
    *(double *)(*(int *)puVar15 + 0x1c) = dVar9;
    dVar8 = *(double *)(puVar7 + -0x2c);
  }
  if (in_FPSCR_PR == '\0') {
    fVar13 = SUB84(dVar9,0) * (float)in_fr15;
  }
  else {
    fVar13 = SUB84(dVar9 * (double)CONCAT44(in_fr14,in_fr15),0);
  }
  if (in_FPSCR_SZ == '\0') {
    dVar9 = (double)CONCAT44(*(undefined4 *)(*piVar2 + 0x28),fVar13);
  }
  else {
    dVar9 = *(double *)(*piVar2 + 0x28);
  }
  if (in_FPSCR_PR == '\0') {
    dVar9 = (double)CONCAT44((float)((ulonglong)dVar9 >> 0x20) - SUB84(dVar9,0),SUB84(dVar9,0));
  }
  else {
    dVar9 = dVar9 - dVar9;
  }
  if (in_FPSCR_SZ == '\0') {
    *(int *)(*(int *)puVar15 + 0x20) = (int)((ulonglong)dVar9 >> 0x20);
  }
  else {
    *(double *)(*(int *)puVar15 + 0x20) = dVar9;
  }
  (*(code *)PTR_FUN_8c01fc24)(*(int *)puVar15 + 0xc);
  *(undefined4 *)(puVar7 + -0x40) = *puVar5;
  *(undefined4 *)(puVar7 + -0x3c) = *(undefined4 *)(puVar7 + -0xc);
  *(undefined4 *)(puVar7 + -0x38) = *(undefined4 *)(puVar7 + -8);
  iVar4 = (*(code *)PTR_FUN_8c01fc28)(0);
  uVar3 = *(undefined4 *)(iVar4 + 0x54);
  *(undefined4 *)(puVar7 + -0x58) = *(undefined4 *)(iVar4 + 0x50);
  *(undefined4 *)(puVar7 + -0x54) = uVar3;
  *(undefined4 *)(puVar7 + -0x50) = *(undefined4 *)(iVar4 + 0x58);
  if (in_FPSCR_SZ == '\0') {
    dVar10 = (double)CONCAT44(_DAT_8c01fc2c,param_1);
    dVar9 = (double)CONCAT44((int)((ulonglong)dVar9 >> 0x20),*(undefined4 *)(puVar7 + -0x58));
  }
  else {
    dVar10 = (double)CONCAT44(PTR_FUN_8c01fc30,_DAT_8c01fc2c);
    dVar8 = *(double *)(puVar7 + -0x58);
  }
  fVar13 = (float)((ulonglong)dVar10 >> 0x20);
  if (in_FPSCR_PR == '\0') {
    dVar9 = (double)CONCAT44((int)((ulonglong)dVar9 >> 0x20),SUB84(dVar9,0) * fVar13);
  }
  else {
    dVar9 = dVar9 * dVar10;
  }
  if (in_FPSCR_SZ == '\0') {
    *(int *)(puVar7 + -0x58) = SUB84(dVar9,0);
    dVar9 = (double)CONCAT44((int)((ulonglong)dVar9 >> 0x20),*(undefined4 *)(puVar7 + -0x54));
  }
  else {
    *(double *)(puVar7 + -0x58) = dVar8;
    dVar8 = *(double *)(puVar7 + -0x54);
  }
  if (in_FPSCR_PR == '\0') {
    dVar9 = (double)CONCAT44((int)((ulonglong)dVar9 >> 0x20),SUB84(dVar9,0) * fVar13);
  }
  else {
    dVar9 = dVar9 * dVar10;
  }
  if (in_FPSCR_SZ == '\0') {
    *(int *)(puVar7 + -0x54) = SUB84(dVar9,0);
    dVar9 = (double)CONCAT44((int)((ulonglong)dVar9 >> 0x20),*(undefined4 *)(puVar7 + -0x50));
  }
  else {
    *(double *)(puVar7 + -0x54) = dVar8;
    dVar8 = *(double *)(puVar7 + -0x50);
  }
  if (in_FPSCR_PR == '\0') {
    fVar13 = SUB84(dVar9,0) * fVar13;
  }
  else {
    fVar13 = SUB84(dVar9 * dVar10,0);
  }
  if (in_FPSCR_SZ == '\0') {
    *(float *)(puVar7 + -0x50) = fVar13;
  }
  else {
    *(double *)(puVar7 + -0x50) = dVar8;
  }
  (*(code *)PTR_FUN_8c01fc30)(puVar7 + -0x40,puVar7 + -0x58);
  if (in_FPSCR_SZ == '\0') {
    dVar8 = (double)CONCAT44(*(undefined4 *)(*(int *)puVar15 + 0xc),*puVar5);
  }
  else {
    dVar8 = *(double *)(*(int *)puVar15 + 0xc);
  }
  if (in_FPSCR_PR == '\0') {
    dVar8 = (double)CONCAT44((float)((ulonglong)dVar8 >> 0x20) - SUB84(dVar8,0),SUB84(dVar8,0));
  }
  else {
    dVar8 = dVar8 - dVar8;
  }
  if (in_FPSCR_SZ == '\0') {
    *(int *)(puVar7 + -0x4c) = (int)((ulonglong)dVar8 >> 0x20);
    dVar8 = (double)(ulonglong)*(uint *)(puVar7 + -0xc);
  }
  else {
    *(double *)(puVar7 + -0x4c) = dVar8;
  }
  if (in_FPSCR_SZ == '\0') {
    dVar8 = (double)CONCAT44(*(undefined4 *)(*(int *)puVar15 + 0x10),SUB84(dVar8,0));
  }
  else {
    dVar8 = *(double *)(*(int *)puVar15 + 0x10);
  }
  if (in_FPSCR_PR == '\0') {
    dVar8 = (double)CONCAT44((float)((ulonglong)dVar8 >> 0x20) - SUB84(dVar8,0),SUB84(dVar8,0));
  }
  else {
    dVar8 = dVar8 - dVar8;
  }
  if (in_FPSCR_SZ == '\0') {
    *(int *)(puVar7 + -0x48) = (int)((ulonglong)dVar8 >> 0x20);
    dVar8 = (double)(ulonglong)*(uint *)(puVar7 + -8);
  }
  else {
    *(double *)(puVar7 + -0x48) = dVar8;
  }
  if (in_FPSCR_SZ == '\0') {
    dVar8 = (double)CONCAT44(*(undefined4 *)(*(int *)puVar15 + 0x14),SUB84(dVar8,0));
  }
  else {
    dVar8 = *(double *)(*(int *)puVar15 + 0x14);
  }
  if (in_FPSCR_PR == '\0') {
    dVar8 = (double)CONCAT44((float)((ulonglong)dVar8 >> 0x20) - SUB84(dVar8,0),SUB84(dVar8,0));
  }
  else {
    dVar8 = dVar8 - dVar8;
  }
  if (in_FPSCR_SZ == '\0') {
    *(int *)(puVar7 + -0x44) = (int)((ulonglong)dVar8 >> 0x20);
    uVar11 = CONCAT44(DAT_8c01fc34,SUB84(dVar10,0));
  }
  else {
    *(double *)(puVar7 + -0x44) = dVar8;
    uVar11 = CONCAT44(PTR_FUN_8c01fc38,DAT_8c01fc34);
  }
  iVar4 = (*(code *)PTR_FUN_8c01fc38)(uVar11,puVar7 + -0x40,puVar7 + -0x4c,0);
  if (iVar4 != 0) {
    if (in_FPSCR_SZ == '\0') {
      dVar8 = (double)CONCAT44(*(undefined4 *)(puVar7 + -0x40),*(undefined4 *)(puVar7 + -0x4c));
    }
    else {
      dVar8 = *(double *)(puVar7 + -0x40);
    }
    if (in_FPSCR_PR == '\0') {
      dVar8 = (double)CONCAT44((float)((ulonglong)dVar8 >> 0x20) + SUB84(dVar8,0),SUB84(dVar8,0));
    }
    else {
      dVar8 = dVar8 + dVar8;
    }
    if (in_FPSCR_SZ == '\0') {
      dVar8 = (double)CONCAT44((int)((ulonglong)dVar8 >> 0x20),*(undefined4 *)(puVar7 + -0x58));
    }
    if (in_FPSCR_PR == '\0') {
      dVar8 = (double)CONCAT44((float)((ulonglong)dVar8 >> 0x20) - SUB84(dVar8,0),SUB84(dVar8,0));
    }
    else {
      dVar8 = dVar8 - dVar8;
    }
    if (in_FPSCR_SZ == '\0') {
      *(int *)(*(int *)puVar15 + 0xc) = (int)((ulonglong)dVar8 >> 0x20);
      dVar8 = (double)CONCAT44(*(undefined4 *)(puVar7 + -0x3c),*(undefined4 *)(puVar7 + -0x48));
    }
    else {
      *(double *)(*(int *)puVar15 + 0xc) = dVar8;
      dVar8 = *(double *)(puVar7 + -0x3c);
    }
    if (in_FPSCR_PR == '\0') {
      dVar8 = (double)CONCAT44((float)((ulonglong)dVar8 >> 0x20) + SUB84(dVar8,0),SUB84(dVar8,0));
    }
    else {
      dVar8 = dVar8 + dVar8;
    }
    if (in_FPSCR_SZ == '\0') {
      dVar8 = (double)CONCAT44((int)((ulonglong)dVar8 >> 0x20),*(undefined4 *)(puVar7 + -0x54));
    }
    if (in_FPSCR_PR == '\0') {
      dVar8 = (double)CONCAT44((float)((ulonglong)dVar8 >> 0x20) - SUB84(dVar8,0),SUB84(dVar8,0));
    }
    else {
      dVar8 = dVar8 - dVar8;
    }
    if (in_FPSCR_SZ == '\0') {
      *(int *)(*(int *)puVar15 + 0x10) = (int)((ulonglong)dVar8 >> 0x20);
      dVar8 = (double)CONCAT44(*(undefined4 *)(puVar7 + -0x38),*(undefined4 *)(puVar7 + -0x44));
    }
    else {
      *(double *)(*(int *)puVar15 + 0x10) = dVar8;
      dVar8 = *(double *)(puVar7 + -0x38);
    }
    if (in_FPSCR_PR == '\0') {
      dVar8 = (double)CONCAT44((float)((ulonglong)dVar8 >> 0x20) + SUB84(dVar8,0),SUB84(dVar8,0));
    }
    else {
      dVar8 = dVar8 + dVar8;
    }
    if (in_FPSCR_SZ == '\0') {
      dVar8 = (double)CONCAT44((int)((ulonglong)dVar8 >> 0x20),*(undefined4 *)(puVar7 + -0x50));
    }
    if (in_FPSCR_PR == '\0') {
      dVar8 = (double)CONCAT44((float)((ulonglong)dVar8 >> 0x20) - SUB84(dVar8,0),SUB84(dVar8,0));
    }
    else {
      dVar8 = dVar8 - dVar8;
    }
    if (in_FPSCR_SZ == '\0') {
      *(int *)(*(int *)puVar15 + 0x14) = (int)((ulonglong)dVar8 >> 0x20);
    }
    else {
      *(double *)(*(int *)puVar15 + 0x14) = dVar8;
    }
  }
  return;
}


/* Address: 8c0213e0 */

void FUN_8c0213e0(undefined4 param_1,undefined1 param_2,undefined4 param_3)

{
  undefined *puVar1;
  
  (*(code *)PTR_FUN_8c021524)(PTR_LAB_8c021520,0,2);
  puVar1 = PTR_DAT_8c021528;
  *(undefined4 *)(*(int *)PTR_DAT_8c021528 + 0x10) = param_1;
  *(undefined1 *)(*(int *)puVar1 + 8) = param_2;
  *(undefined4 *)(*(int *)puVar1 + 0xc) = param_3;
  *(undefined2 *)(*(int *)puVar1 + 10) = 0;
  return;
}


/* Address: 8c021846 */

double FUN_8c021846(double param_1,undefined4 param_2,undefined8 *param_3,double *param_4)

{
  double *pdVar1;
  undefined4 in_fr1;
  double dVar2;
  double in_dr2;
  float fVar3;
  double dVar4;
  float fVar5;
  double dVar6;
  double in_xd2;
  undefined8 in_xd4;
  char in_FPSCR_PR;
  char in_FPSCR_SZ;
  
  if (in_FPSCR_SZ == '\0') {
    param_1 = (double)CONCAT44((int)((ulonglong)param_1 >> 0x20),*(undefined4 *)param_3);
    in_dr2 = (double)CONCAT44((int)((ulonglong)in_dr2 >> 0x20),*(undefined4 *)param_4);
    param_4 = (double *)((int)param_4 + 4);
    dVar6 = (double)CONCAT44(*(undefined4 *)((int)param_3 + 4),param_2);
    pdVar1 = (double *)(param_3 + 1);
  }
  else {
    in_xd4 = *param_3;
    in_xd2 = *param_4;
    param_4 = param_4 + 1;
    dVar6 = (double)param_3[1];
    pdVar1 = (double *)(param_3 + 2);
  }
  if (in_FPSCR_PR == '\0') {
    fVar3 = SUB84(param_1,0) - SUB84(in_dr2,0);
  }
  else {
    fVar3 = SUB84(param_1 - in_dr2,0);
  }
  if (in_FPSCR_SZ == '\0') {
    in_dr2 = (double)CONCAT44((int)((ulonglong)in_dr2 >> 0x20),*(undefined4 *)param_4);
    param_4 = (double *)((int)param_4 + 4);
    dVar4 = (double)CONCAT44(*(undefined4 *)pdVar1,fVar3);
  }
  else {
    in_xd2 = *param_4;
    param_4 = param_4 + 1;
    dVar4 = *pdVar1;
  }
  if (in_FPSCR_PR == '\0') {
    dVar6 = (double)CONCAT44((float)((ulonglong)dVar6 >> 0x20) - SUB84(in_dr2,0),SUB84(dVar6,0));
  }
  else {
    dVar6 = dVar6 - in_dr2;
  }
  if (in_FPSCR_SZ == '\0') {
    in_dr2 = (double)CONCAT44((int)((ulonglong)in_dr2 >> 0x20),*(undefined4 *)param_4);
  }
  else {
    in_xd2 = *param_4;
  }
  if (in_FPSCR_PR == '\0') {
    dVar4 = (double)CONCAT44((float)((ulonglong)dVar4 >> 0x20) - SUB84(in_dr2,0),SUB84(dVar4,0));
  }
  else {
    dVar4 = dVar4 - in_dr2;
  }
  fVar3 = (float)((ulonglong)dVar6 >> 0x20);
  dVar2 = dVar6;
  if (in_FPSCR_SZ == '\0') {
    in_xd4 = CONCAT44(SUB84(dVar4,0),in_fr1);
    in_dr2 = (double)CONCAT44((int)((ulonglong)in_dr2 >> 0x20),fVar3);
    dVar2 = in_xd2;
  }
  if (in_FPSCR_PR == '\0') {
    fVar3 = (float)((ulonglong)in_xd4 >> 0x20) * SUB84(dVar4,0) + SUB84(in_dr2,0) * fVar3;
  }
  else {
    fVar3 = SUB84(in_dr2 * dVar6,0);
  }
  fVar5 = (float)((ulonglong)dVar4 >> 0x20);
  if (in_FPSCR_SZ == '\0') {
    dVar4 = (double)CONCAT44(fVar5,(int)in_xd4);
  }
  if (in_FPSCR_PR == '\0') {
    fVar3 = (float)((ulonglong)dVar4 >> 0x20) * fVar5 + fVar3;
  }
  if (in_FPSCR_SZ == '\0') {
    dVar2 = (double)CONCAT44(fVar3,SUB84(dVar4,0));
  }
  return dVar2;
}


/* Address: 8c021868 */

undefined4 FUN_8c021868(int param_1,int param_2)

{
  if ((param_1 < (int)(*(ushort *)(*(int *)(param_2 + 4) + 2) - 3)) && (param_1 != 0)) {
    return 0;
  }
  return 1;
}


/* Address: 8c0218e0 */

undefined4
FUN_8c0218e0(undefined4 param_1,int *param_2,undefined4 *param_3,undefined4 param_4,double *param_5)

{
  uint uVar1;
  undefined4 uVar2;
  int iVar3;
  int iVar4;
  int iVar5;
  int iVar6;
  int iVar7;
  undefined8 *puVar8;
  undefined8 uVar9;
  undefined4 uVar10;
  float fVar11;
  undefined4 in_fr3;
  double dVar12;
  undefined8 in_dr14;
  double dVar13;
  undefined8 in_xd14;
  bool bVar14;
  undefined4 in_PR;
  char in_FPSCR_PR;
  char in_FPSCR_SZ;
  undefined1 local_24 [8];
  undefined8 uStack_1c;
  
  if (in_FPSCR_SZ == '\0') {
    uStack_1c = CONCAT44((int)in_dr14,(undefined4)uStack_1c);
    puVar8 = &uStack_1c;
                    /* WARNING: Ignoring partial resolution of indirect */
    uStack_1c._0_4_ = (int)((ulonglong)in_dr14 >> 0x20);
  }
  else {
    puVar8 = (undefined8 *)local_24;
    uStack_1c = in_xd14;
  }
  *(undefined4 *)((int)puVar8 + -4) = in_PR;
  *(undefined4 *)((int)puVar8 + -0x44) = param_4;
  iVar4 = *(int *)((int)puVar8 + 0x1c);
  iVar7 = *param_2;
  uVar1 = (uint)*(ushort *)(*(int *)(iVar4 + 4) + 2);
  iVar6 = uVar1 - 1;
  if (iVar6 <= iVar7) {
    iVar7 = uVar1 - 2;
  }
  *(int *)((int)puVar8 + -0x48) = iVar7 + 1;
  if (in_FPSCR_SZ == '\0') {
    in_fr3 = *(undefined4 *)(iVar4 + 0x50);
  }
  iVar5 = iVar7 * 0x14;
  iVar3 = *(int *)(*(int *)(iVar4 + 4) + 8) + iVar5;
  if (in_FPSCR_SZ == '\0') {
    dVar12 = (double)CONCAT44(*(undefined4 *)(iVar3 + 8),in_fr3);
  }
  else {
    dVar12 = *(double *)(iVar3 + 8);
  }
  if (in_FPSCR_PR == '\0') {
    dVar12 = (double)CONCAT44((float)((ulonglong)dVar12 >> 0x20) + SUB84(dVar12,0),SUB84(dVar12,0));
  }
  else {
    dVar12 = dVar12 + dVar12;
  }
  if (in_FPSCR_SZ == '\0') {
    *(int *)((int)puVar8 + -0x10) = (int)((ulonglong)dVar12 >> 0x20);
  }
  else {
    *(double *)((int)puVar8 + -0x10) = dVar12;
  }
  if (in_FPSCR_SZ == '\0') {
    dVar12 = (double)(ulonglong)*(uint *)(iVar4 + 0x54);
  }
  iVar3 = *(int *)(*(int *)(iVar4 + 4) + 8) + iVar5;
  if (in_FPSCR_SZ == '\0') {
    dVar12 = (double)CONCAT44(*(undefined4 *)(iVar3 + 0xc),SUB84(dVar12,0));
  }
  else {
    dVar12 = *(double *)(iVar3 + 0xc);
  }
  if (in_FPSCR_PR == '\0') {
    dVar12 = (double)CONCAT44((float)((ulonglong)dVar12 >> 0x20) + SUB84(dVar12,0),SUB84(dVar12,0));
  }
  else {
    dVar12 = dVar12 + dVar12;
  }
  if (in_FPSCR_SZ == '\0') {
    *(int *)((int)puVar8 + -0xc) = (int)((ulonglong)dVar12 >> 0x20);
  }
  else {
    *(double *)((int)puVar8 + -0xc) = dVar12;
  }
  iVar5 = iVar5 + *(int *)(*(int *)(iVar4 + 4) + 8);
  if (in_FPSCR_SZ == '\0') {
    dVar12 = (double)CONCAT44(*(undefined4 *)(iVar5 + 0x10),*(undefined4 *)(iVar4 + 0x58));
  }
  else {
    dVar12 = *(double *)(iVar5 + 0x10);
  }
  if (in_FPSCR_PR == '\0') {
    dVar12 = (double)CONCAT44((float)((ulonglong)dVar12 >> 0x20) + SUB84(dVar12,0),SUB84(dVar12,0));
  }
  else {
    dVar12 = dVar12 + dVar12;
  }
  if (in_FPSCR_SZ == '\0') {
    *(int *)((int)puVar8 + -8) = (int)((ulonglong)dVar12 >> 0x20);
  }
  else {
    *(double *)((int)puVar8 + -8) = dVar12;
  }
  if (in_FPSCR_SZ == '\0') {
    dVar12 = (double)(ulonglong)*(uint *)(iVar4 + 0x50);
  }
  iVar5 = *(int *)((int)puVar8 + -0x48) * 0x14;
  iVar3 = *(int *)(*(int *)(iVar4 + 4) + 8) + iVar5;
  if (in_FPSCR_SZ == '\0') {
    dVar12 = (double)CONCAT44(*(undefined4 *)(iVar3 + 8),SUB84(dVar12,0));
  }
  else {
    dVar12 = *(double *)(iVar3 + 8);
  }
  if (in_FPSCR_PR == '\0') {
    dVar12 = (double)CONCAT44((float)((ulonglong)dVar12 >> 0x20) + SUB84(dVar12,0),SUB84(dVar12,0));
  }
  else {
    dVar12 = dVar12 + dVar12;
  }
  if (in_FPSCR_SZ == '\0') {
    *(int *)((int)puVar8 + -0x1c) = (int)((ulonglong)dVar12 >> 0x20);
  }
  else {
    *(double *)((int)puVar8 + -0x1c) = dVar12;
  }
  if (in_FPSCR_SZ == '\0') {
    dVar12 = (double)(ulonglong)*(uint *)(iVar4 + 0x54);
  }
  iVar3 = *(int *)(*(int *)(iVar4 + 4) + 8) + iVar5;
  if (in_FPSCR_SZ == '\0') {
    dVar12 = (double)CONCAT44(*(undefined4 *)(iVar3 + 0xc),SUB84(dVar12,0));
  }
  else {
    dVar12 = *(double *)(iVar3 + 0xc);
  }
  if (in_FPSCR_PR == '\0') {
    dVar12 = (double)CONCAT44((float)((ulonglong)dVar12 >> 0x20) + SUB84(dVar12,0),SUB84(dVar12,0));
  }
  else {
    dVar12 = dVar12 + dVar12;
  }
  if (in_FPSCR_SZ == '\0') {
    *(int *)((int)puVar8 + -0x18) = (int)((ulonglong)dVar12 >> 0x20);
  }
  else {
    *(double *)((int)puVar8 + -0x18) = dVar12;
  }
  if (in_FPSCR_SZ == '\0') {
    dVar12 = (double)(ulonglong)*(uint *)(iVar4 + 0x58);
  }
  iVar5 = iVar5 + *(int *)(*(int *)(iVar4 + 4) + 8);
  if (in_FPSCR_SZ == '\0') {
    dVar12 = (double)CONCAT44(*(undefined4 *)(iVar5 + 0x10),SUB84(dVar12,0));
  }
  else {
    dVar12 = *(double *)(iVar5 + 0x10);
  }
  if (in_FPSCR_PR == '\0') {
    dVar12 = (double)CONCAT44((float)((ulonglong)dVar12 >> 0x20) + SUB84(dVar12,0),SUB84(dVar12,0));
  }
  else {
    dVar12 = dVar12 + dVar12;
  }
  if (in_FPSCR_SZ == '\0') {
    *(int *)((int)puVar8 + -0x14) = (int)((ulonglong)dVar12 >> 0x20);
    *(undefined4 *)((int)puVar8 + -0x40) = *(undefined4 *)((int)puVar8 + -0x10);
    *(undefined4 *)((int)puVar8 + -0x3c) = *(undefined4 *)((int)puVar8 + -0xc);
    *(undefined4 *)((int)puVar8 + -0x38) = *(undefined4 *)((int)puVar8 + -8);
    dVar12 = (double)CONCAT44(*(undefined4 *)((int)puVar8 + -0x10),
                              *(undefined4 *)((int)puVar8 + -0x1c));
  }
  else {
    *(double *)((int)puVar8 + -0x14) = dVar12;
    *(undefined8 *)((int)puVar8 + -0x40) = *(undefined8 *)((int)puVar8 + -0x10);
    *(undefined8 *)((int)puVar8 + -0x3c) = *(undefined8 *)((int)puVar8 + -0xc);
    *(undefined8 *)((int)puVar8 + -0x38) = *(undefined8 *)((int)puVar8 + -8);
    dVar12 = *(double *)((int)puVar8 + -0x10);
  }
  if (in_FPSCR_PR == '\0') {
    dVar12 = (double)CONCAT44((float)((ulonglong)dVar12 >> 0x20) - SUB84(dVar12,0),SUB84(dVar12,0));
  }
  else {
    dVar12 = dVar12 - dVar12;
  }
  if (in_FPSCR_SZ == '\0') {
    *(int *)((int)puVar8 + -0x34) = (int)((ulonglong)dVar12 >> 0x20);
    dVar12 = (double)CONCAT44(*(undefined4 *)((int)puVar8 + -0xc),
                              *(undefined4 *)((int)puVar8 + -0x18));
  }
  else {
    *(double *)((int)puVar8 + -0x34) = dVar12;
    dVar12 = *(double *)((int)puVar8 + -0xc);
  }
  if (in_FPSCR_PR == '\0') {
    dVar12 = (double)CONCAT44((float)((ulonglong)dVar12 >> 0x20) - SUB84(dVar12,0),SUB84(dVar12,0));
  }
  else {
    dVar12 = dVar12 - dVar12;
  }
  if (in_FPSCR_SZ == '\0') {
    *(int *)((int)puVar8 + -0x30) = (int)((ulonglong)dVar12 >> 0x20);
    dVar12 = (double)CONCAT44(*(undefined4 *)((int)puVar8 + -8),*(undefined4 *)((int)puVar8 + -0x14)
                             );
  }
  else {
    *(double *)((int)puVar8 + -0x30) = dVar12;
    dVar12 = *(double *)((int)puVar8 + -8);
  }
  if (in_FPSCR_PR == '\0') {
    dVar12 = (double)CONCAT44((float)((ulonglong)dVar12 >> 0x20) - SUB84(dVar12,0),SUB84(dVar12,0));
  }
  else {
    dVar12 = dVar12 - dVar12;
  }
  if (in_FPSCR_SZ == '\0') {
    *(int *)((int)puVar8 + -0x2c) = (int)((ulonglong)dVar12 >> 0x20);
  }
  else {
    *(double *)((int)puVar8 + -0x2c) = dVar12;
  }
  (*(code *)PTR_FUN_8c021ac0)
            (*(undefined4 *)((int)puVar8 + -0x44),(undefined1 *)((int)puVar8 + -0x40),
             (undefined1 *)((int)puVar8 + -0x28));
  uVar2 = (undefined4)in_dr14;
  uVar9 = FUN_8c021846((undefined1 *)((int)puVar8 + -0x10),(undefined1 *)((int)puVar8 + -0x1c));
  uVar10 = (undefined4)uVar9;
  if (in_FPSCR_SZ == '\0') {
    uVar10 = uVar2;
  }
  dVar13 = (double)CONCAT44((int)((ulonglong)uVar9 >> 0x20),uVar10);
  uVar10 = FUN_8c021846((undefined1 *)((int)puVar8 + -0x10),(undefined1 *)((int)puVar8 + -0x28));
  if (in_FPSCR_SZ == '\0') {
    dVar13 = (double)CONCAT44((int)((ulonglong)dVar13 >> 0x20),uVar10);
  }
  uVar9 = FUN_8c021846((undefined1 *)((int)puVar8 + -0x1c),(undefined1 *)((int)puVar8 + -0x28));
  fVar11 = (float)((ulonglong)uVar9 >> 0x20);
  uVar10 = (undefined4)uVar9;
  if (in_FPSCR_SZ == '\0') {
    uVar10 = param_1;
  }
  if (param_5 != (double *)0x0) {
    if (in_FPSCR_SZ == '\0') {
      dVar12 = (double)CONCAT44(*(undefined4 *)((int)puVar8 + -0x10),
                                *(undefined4 *)((int)puVar8 + -0x1c));
    }
    else {
      dVar12 = *(double *)((int)puVar8 + -0x10);
    }
    if (in_FPSCR_PR == '\0') {
      dVar12 = (double)CONCAT44((float)((ulonglong)dVar12 >> 0x20) - SUB84(dVar12,0),SUB84(dVar12,0)
                               );
    }
    else {
      dVar12 = dVar12 - dVar12;
    }
    if (in_FPSCR_SZ == '\0') {
      *(int *)param_5 = (int)((ulonglong)dVar12 >> 0x20);
      dVar12 = (double)CONCAT44(*(undefined4 *)((int)puVar8 + -0xc),
                                *(undefined4 *)((int)puVar8 + -0x18));
    }
    else {
      *param_5 = dVar12;
      dVar12 = *(double *)((int)puVar8 + -0xc);
    }
    if (in_FPSCR_PR == '\0') {
      dVar12 = (double)CONCAT44((float)((ulonglong)dVar12 >> 0x20) - SUB84(dVar12,0),SUB84(dVar12,0)
                               );
    }
    else {
      dVar12 = dVar12 - dVar12;
    }
    if (in_FPSCR_SZ == '\0') {
      *(int *)((int)param_5 + 4) = (int)((ulonglong)dVar12 >> 0x20);
      dVar12 = (double)CONCAT44(*(undefined4 *)((int)puVar8 + -8),
                                *(undefined4 *)((int)puVar8 + -0x14));
    }
    else {
      *(double *)((int)param_5 + 4) = dVar12;
      dVar12 = *(double *)((int)puVar8 + -8);
    }
    if (in_FPSCR_PR == '\0') {
      dVar12 = (double)CONCAT44((float)((ulonglong)dVar12 >> 0x20) - SUB84(dVar12,0),SUB84(dVar12,0)
                               );
    }
    else {
      dVar12 = dVar12 - dVar12;
    }
    if (in_FPSCR_SZ == '\0') {
      *(int *)(param_5 + 1) = (int)((ulonglong)dVar12 >> 0x20);
    }
    else {
      param_5[1] = dVar12;
    }
  }
  if (in_FPSCR_SZ == '\0') {
    dVar12 = (double)CONCAT44((int)((ulonglong)dVar12 >> 0x20),SUB84(dVar13,0));
  }
  if (in_FPSCR_PR == '\0') {
    dVar12 = (double)CONCAT44((int)((ulonglong)dVar12 >> 0x20),SUB84(dVar12,0) + fVar11);
  }
  else {
    dVar12 = dVar12 + (double)CONCAT44(fVar11,uVar10);
  }
  *param_2 = iVar7;
  if (in_FPSCR_PR == '\0') {
    bVar14 = (float)((ulonglong)dVar13 >> 0x20) < SUB84(dVar12,0);
  }
  else {
    bVar14 = dVar13 < dVar12;
  }
  uVar2 = *(undefined4 *)((int)puVar8 + -0x24);
  *param_3 = *(undefined4 *)((int)puVar8 + -0x28);
  param_3[1] = uVar2;
  param_3[2] = *(undefined4 *)((int)puVar8 + -0x20);
  if (bVar14) {
    if (in_FPSCR_PR == '\0') {
      bVar14 = fVar11 < SUB84(dVar13,0);
    }
    else {
      bVar14 = (double)CONCAT44(fVar11,uVar10) < dVar13;
    }
    if (bVar14) {
      iVar7 = *(int *)((int)puVar8 + -0x48);
      *param_2 = iVar7;
      if (iVar6 <= iVar7) goto LAB_8c021aa6;
      uVar10 = *(undefined4 *)((int)puVar8 + -0x18);
      *param_3 = *(undefined4 *)((int)puVar8 + -0x1c);
      param_3[1] = uVar10;
      uVar10 = *(undefined4 *)((int)puVar8 + -0x14);
    }
    else {
      if (iVar7 < 1) {
        iVar7 = 0;
      }
      else {
        iVar7 = iVar7 + -1;
      }
      *param_2 = iVar7;
      if (iVar7 < 1) goto LAB_8c021aa6;
      uVar10 = *(undefined4 *)((int)puVar8 + -0xc);
      *param_3 = *(undefined4 *)((int)puVar8 + -0x10);
      param_3[1] = uVar10;
      uVar10 = *(undefined4 *)((int)puVar8 + -8);
    }
    param_3[2] = uVar10;
    uVar10 = 1;
  }
  else {
LAB_8c021aa6:
    uVar10 = 0;
  }
  return uVar10;
}


/* Address: 8c021ae0 */

int FUN_8c021ae0(double param_1,undefined4 param_2,int param_3)

{
  undefined4 *puVar1;
  int iVar2;
  double *pdVar3;
  double *pdVar4;
  int unaff_r11;
  uint uVar5;
  int iVar6;
  undefined1 *puVar7;
  undefined4 uVar8;
  float fVar10;
  double dVar11;
  undefined4 uVar12;
  float in_fr15;
  undefined4 in_fr14;
  bool bVar13;
  undefined4 in_PR;
  char in_FPSCR_PR;
  char in_FPSCR_SZ;
  undefined1 local_18 [4];
  undefined1 local_14 [4];
  undefined8 uVar9;
  
  if (in_FPSCR_SZ == '\0') {
    puVar7 = local_14;
  }
  else {
    puVar7 = local_18;
  }
  *(undefined4 *)(puVar7 + -4) = in_PR;
  *(undefined4 *)(puVar7 + -0x14) = param_2;
  uVar5 = (uint)*(ushort *)(*(int *)(param_3 + 4) + 2);
  iVar6 = 0;
  if (uVar5 != 0) {
    do {
      uVar12 = SUB84(param_1,0);
      iVar2 = *(int *)(*(int *)(param_3 + 4) + 8) + iVar6 * 0x14;
      if (in_FPSCR_SZ == '\0') {
        puVar1 = (undefined4 *)(param_3 + 0x54);
        dVar11 = (double)CONCAT44(*(undefined4 *)(iVar2 + 8),*(undefined4 *)(param_3 + 0x50));
        pdVar3 = (double *)(iVar2 + 0xc);
      }
      else {
        puVar1 = (undefined4 *)(param_3 + 0x58);
        dVar11 = *(double *)(iVar2 + 8);
        pdVar3 = (double *)(iVar2 + 0x10);
      }
      if (in_FPSCR_PR == '\0') {
        dVar11 = (double)CONCAT44((float)((ulonglong)dVar11 >> 0x20) + SUB84(dVar11,0),
                                  SUB84(dVar11,0));
      }
      else {
        dVar11 = dVar11 + dVar11;
      }
      if (in_FPSCR_SZ == '\0') {
        *(int *)(puVar7 + -0x10) = (int)((ulonglong)dVar11 >> 0x20);
        pdVar4 = (double *)((int)pdVar3 + 4);
        dVar11 = (double)CONCAT44(*(undefined4 *)pdVar3,*puVar1);
        puVar1 = puVar1 + 1;
      }
      else {
        *(double *)(puVar7 + -0x10) = dVar11;
        dVar11 = *pdVar3;
        pdVar4 = pdVar3 + 1;
        puVar1 = puVar1 + 2;
      }
      if (in_FPSCR_PR == '\0') {
        dVar11 = (double)CONCAT44((float)((ulonglong)dVar11 >> 0x20) + SUB84(dVar11,0),
                                  SUB84(dVar11,0));
      }
      else {
        dVar11 = dVar11 + dVar11;
      }
      if (in_FPSCR_SZ == '\0') {
        *(int *)(puVar7 + -0xc) = (int)((ulonglong)dVar11 >> 0x20);
        dVar11 = (double)CONCAT44(*(undefined4 *)pdVar4,*puVar1);
      }
      else {
        *(double *)(puVar7 + -0xc) = dVar11;
        dVar11 = *pdVar4;
      }
      if (in_FPSCR_PR == '\0') {
        dVar11 = (double)CONCAT44((float)((ulonglong)dVar11 >> 0x20) + SUB84(dVar11,0),
                                  SUB84(dVar11,0));
      }
      else {
        dVar11 = dVar11 + dVar11;
      }
      if (in_FPSCR_SZ == '\0') {
        *(int *)(puVar7 + -8) = (int)((ulonglong)dVar11 >> 0x20);
      }
      else {
        *(double *)(puVar7 + -8) = dVar11;
      }
      uVar9 = FUN_8c021846(puVar7 + -0x10,*(undefined4 *)(puVar7 + -0x14));
      fVar10 = (float)((ulonglong)uVar9 >> 0x20);
      uVar8 = (undefined4)uVar9;
      if (in_FPSCR_SZ == '\0') {
        uVar8 = uVar12;
      }
      param_1 = (double)CONCAT44(fVar10,uVar8);
      if (iVar6 == 0) {
LAB_8c021b44:
        unaff_r11 = iVar6;
        if (in_FPSCR_SZ == '\0') {
          in_fr15 = fVar10;
        }
      }
      else {
        if (in_FPSCR_PR == '\0') {
          bVar13 = fVar10 < in_fr15;
        }
        else {
          bVar13 = param_1 < (double)CONCAT44(in_fr14,in_fr15);
        }
        if (bVar13) goto LAB_8c021b44;
      }
      iVar6 = iVar6 + 1;
    } while (iVar6 < (int)uVar5);
  }
  return unaff_r11;
}


/* Address: 8c022802 */

void FUN_8c022802(int *param_1,undefined4 param_2)

{
  (*(code *)PTR_FUN_8c0229fc)(PTR_DAT_8c0229f8);
  if (param_1[2] != 0) {
    (*(code *)PTR_FUN_8c022a00)(0,param_1[2]);
  }
  if (*param_1 != 0) {
    (*(code *)PTR_FUN_8c022a04)(0,*param_1);
  }
  if (param_1[1] != 0) {
    (*(code *)PTR_FUN_8c022a08)(0,param_1[1]);
  }
  (*(code *)PTR_FUN_8c022a0c)(0,param_2);
  (*(code *)PTR_FUN_8c022a10)(1);
  return;
}


/* Address: 8c023960 */

void FUN_8c023960(void)

{
  undefined4 uVar1;
  
  uVar1 = (*(code *)PTR_FUN_8c02397c)();
  switch(uVar1) {
  case 1:
  case 2:
  case 3:
  case 4:
  case 5:
  case 8:
  case 9:
  case 10:
  case 0xb:
  case 0xc:
  case 0xd:
  case 0xe:
  case 0xf:
  case 0x12:
  case 0x13:
  case 0x14:
  case 0x15:
  case 0x16:
  case 0x17:
  case 0x18:
  case 0x19:
  case 0x1a:
    (*(code *)PTR_FUN_8c023be0)(0x11);
    return;
  default:
    return;
  }
}


/* Address: 8c026a40 */

void FUN_8c026a40(void)

{
  undefined1 *puVar1;
  undefined1 *puVar2;
  
  puVar2 = DAT_8c026ba0;
  *DAT_8c026ba4 = 1;
  (*(code *)PTR_FUN_8c026ba8)(puVar2,0,0x3400);
  (*(code *)PTR_FUN_8c026ba8)(puVar2,0,0x180);
  puVar1 = DAT_8c026bac;
  for (; puVar2 < puVar1; puVar2 = puVar2 + 0x400) {
    *puVar2 = 8;
    puVar2[0x200] = 8;
  }
  return;
}


/* Address: 8c026ce0 */

void FUN_8c026ce0(undefined4 param_1)

{
  undefined4 *puVar1;
  undefined1 *puVar2;
  undefined1 *puVar3;
  undefined1 *puVar4;
  char in_FPSCR_SZ;
  
  puVar3 = DAT_8c026e00 + 0x78;
  for (puVar2 = DAT_8c026e00; puVar2 < puVar3; puVar2 = puVar2 + 0x18) {
    *puVar2 = 0xff;
    if (in_FPSCR_SZ == '\0') {
      *(undefined4 *)(puVar2 + 4) = 0x3f800000;
    }
    else {
      *(ulonglong *)(puVar2 + 4) = CONCAT44(0x3f800000,param_1);
    }
    puVar2[0xc] = 0xff;
    if (in_FPSCR_SZ == '\0') {
      *(undefined4 *)(puVar2 + 0x10) = 0x3f800000;
    }
    else {
      *(ulonglong *)(puVar2 + 0x10) = CONCAT44(0x3f800000,param_1);
    }
  }
  puVar4 = DAT_8c026e04 + 10;
  puVar2 = DAT_8c026e0c;
  puVar1 = DAT_8c026e10;
  for (puVar3 = DAT_8c026e04; DAT_8c026e0c = puVar2, DAT_8c026e10 = puVar1, puVar3 < puVar4;
      puVar3 = puVar3 + 2) {
    *puVar3 = 0xff;
    puVar3[1] = 0xff;
    puVar2 = DAT_8c026e0c;
    puVar1 = DAT_8c026e10;
  }
  *DAT_8c026e08 = 0;
  *puVar2 = 0;
  *puVar1 = 0;
  FUN_8c026a40();
  puVar1 = DAT_8c026e18;
  *DAT_8c026e14 = 0xff;
  *puVar1 = 0;
  return;
}


/* Address: 8c026d40 */

void FUN_8c026d40(undefined4 param_1,undefined1 param_2)

{
  byte *pbVar1;
  int iVar2;
  undefined4 *puVar3;
  undefined4 *puVar4;
  undefined8 uVar5;
  undefined8 in_xd2;
  char in_FPSCR_SZ;
  undefined4 uVar6;
  
  iVar2 = DAT_8c026e1c._4_4_;
  pbVar1 = DAT_8c026e08;
  if (in_FPSCR_SZ == '\0') {
    uVar5 = CONCAT44((undefined4)DAT_8c026e1c,param_1);
  }
  else {
    uVar5 = CONCAT44(DAT_8c026e1c._4_4_,(undefined4)DAT_8c026e1c);
  }
  uVar6 = (undefined4)((ulonglong)uVar5 >> 0x20);
  if (*DAT_8c026e08 < 10) {
    *(undefined1 *)(DAT_8c026e00 + (uint)*DAT_8c026e08 * 3) = param_2;
    if (in_FPSCR_SZ == '\0') {
      *(undefined4 *)(iVar2 + (uint)*pbVar1 * 0xc) = uVar6;
    }
    else {
      *(undefined8 *)(iVar2 + (uint)*pbVar1 * 0xc) = uVar5;
    }
    *pbVar1 = *pbVar1 + 1;
  }
  else {
    puVar4 = DAT_8c026e00 + 0x1b;
    for (puVar3 = DAT_8c026e00; puVar3 < puVar4; puVar3 = puVar3 + 3) {
      *puVar3 = puVar3[3];
      puVar3[1] = puVar3[4];
      puVar3[2] = puVar3[5];
    }
    *(undefined1 *)puVar3 = param_2;
    if (in_FPSCR_SZ == '\0') {
      puVar3[1] = uVar6;
      puVar3[2] = 0;
    }
    else {
      *(undefined8 *)(puVar3 + 1) = uVar5;
      *(undefined8 *)(puVar3 + 2) = in_xd2;
    }
  }
  *DAT_8c026e10 = 1;
  return;
}


/* Address: 8c026e40 */

/* WARNING: Removing unreachable block (ram,0x8c027048) */
/* WARNING: Removing unreachable block (ram,0x8c026f26) */
/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_8c026e40(double param_1,int param_2)

{
  char cVar1;
  ushort uVar2;
  char *pcVar3;
  byte *pbVar4;
  undefined *puVar5;
  undefined *puVar6;
  int *piVar7;
  undefined *puVar8;
  int iVar9;
  int iVar10;
  undefined1 *puVar11;
  int iVar12;
  char *pcVar13;
  undefined8 *puVar14;
  double dVar15;
  float fVar16;
  double in_dr2;
  float fVar18;
  uint uVar19;
  undefined4 uVar20;
  undefined8 in_dr12;
  double dVar21;
  undefined8 in_dr14;
  double dVar22;
  undefined8 in_xd2;
  double in_xd4;
  undefined8 in_xd14;
  bool bVar23;
  undefined4 in_PR;
  char in_FPSCR_PR;
  char in_FPSCR_SZ;
  undefined1 auStack_3c [16];
  undefined8 uStack_2c;
  undefined8 uStack_24;
  double dVar17;
  
  puVar5 = PTR_DAT_8c026fb4;
  iVar10 = DAT_8c026fb0;
  pbVar4 = DAT_8c026fa8;
  pcVar3 = DAT_8c026fa0;
  if (in_FPSCR_SZ == '\0') {
    uStack_24 = CONCAT44((int)in_dr14,(undefined4)uStack_24);
                    /* WARNING: Ignoring partial resolution of indirect */
    uStack_24._0_4_ = (int)((ulonglong)in_dr14 >> 0x20);
    puVar14 = &uStack_2c;
    uStack_2c = CONCAT44((int)in_dr12,(int)((ulonglong)in_dr12 >> 0x20));
  }
  else {
    puVar14 = (undefined8 *)auStack_3c;
    uStack_24 = in_xd14;
    uStack_2c = in_dr14;
  }
  *(undefined4 *)((int)puVar14 + -4) = in_PR;
  pcVar13 = *(char **)(param_2 + 0x20);
  if (in_FPSCR_SZ == '\0') {
    dVar21 = (double)CONCAT44(DAT_8c026fa4,DAT_8c026fac);
  }
  else {
    dVar21 = (double)CONCAT44(DAT_8c026fa8,DAT_8c026fa4);
  }
  cVar1 = *pcVar13;
  if (in_FPSCR_PR == '\0') {
    uVar20 = 0xbf800000;
  }
  else {
    uVar20 = (undefined4)((ulonglong)-(double)CONCAT44(0x3f800000,(int)in_dr14) >> 0x20);
  }
  dVar22 = (double)CONCAT44(uVar20,0x3f800000);
  if (cVar1 == '\0') {
    if (*DAT_8c026fb8 != 0) {
      *pcVar13 = *pcVar13 + '\x01';
      pcVar13[6] = '\0';
      piVar7 = DAT_8c026fb8;
      pcVar13[7] = '\0';
      if (in_FPSCR_SZ == '\0') {
        *(undefined4 *)(pcVar13 + 0x24) = _DAT_8c026fbc;
      }
      else {
        in_xd2 = CONCAT44(PTR_FUN_8c026fc0,_DAT_8c026fbc);
        *(undefined8 *)(pcVar13 + 0x24) = in_xd2;
      }
      pcVar13[0x14] = '\0';
      pcVar13[0x15] = '\0';
      pcVar13[0x16] = '\0';
      pcVar13[0x17] = '\0';
      *piVar7 = 0;
      if (in_FPSCR_SZ == '\0') {
        pcVar13[0x28] = '\0';
        pcVar13[0x29] = '\0';
        pcVar13[0x2a] = '\0';
        pcVar13[0x2b] = '\0';
      }
      else {
        *(undefined8 *)(pcVar13 + 0x28) = in_xd2;
      }
    }
  }
  else if (cVar1 == '\x01') {
    dVar15 = (double)(*(code *)PTR_FUN_8c026fc0)(*(undefined4 *)(pcVar13 + 0x14));
    if (in_FPSCR_PR == '\0') {
      dVar15 = (double)CONCAT44(-(float)((ulonglong)dVar15 >> 0x20) + SUB84(dVar22,0),
                                SUB84(dVar15,0));
    }
    else {
      dVar15 = -dVar15 + dVar22;
    }
    if (in_FPSCR_SZ == '\0') {
      in_dr2 = (double)CONCAT44((int)((ulonglong)in_dr2 >> 0x20),*(undefined4 *)(pcVar13 + 0x24));
    }
    if (in_FPSCR_PR == '\0') {
      dVar15 = (double)CONCAT44((float)((ulonglong)dVar15 >> 0x20) * SUB84(in_dr2,0),SUB84(dVar15,0)
                               );
    }
    else {
      dVar15 = dVar15 * in_dr2;
    }
    if (in_FPSCR_SZ == '\0') {
      *(int *)(pcVar13 + 0x20) = (int)((ulonglong)dVar15 >> 0x20);
    }
    else {
      *(double *)(pcVar13 + 0x20) = dVar15;
    }
    iVar12 = 0;
    if (*pbVar4 != 0) {
      do {
        puVar11 = (undefined1 *)(iVar12 * 0xc + iVar10);
        *(undefined1 *)((int)puVar14 + -8) = *puVar11;
        if (in_FPSCR_SZ == '\0') {
          uVar20 = *(undefined4 *)(puVar11 + 4);
          dVar15 = (double)CONCAT44(uVar20,DAT_8c026fc4);
          param_1 = (double)CONCAT44(uVar20,SUB84(param_1,0));
        }
        else {
          dVar15 = *(double *)(puVar11 + 4);
          uVar20 = (undefined4)((ulonglong)dVar15 >> 0x20);
          param_1 = dVar15;
        }
        if (in_FPSCR_PR == '\0') {
          fVar18 = (float)((ulonglong)dVar15 >> 0x20) - SUB84(dVar15,0);
          dVar15 = (double)CONCAT44(fVar18,SUB84(dVar15,0));
          bVar23 = fVar18 < SUB84(dVar22,0);
        }
        else {
          dVar15 = dVar15 - dVar15;
          fVar18 = (float)((ulonglong)dVar15 >> 0x20);
          bVar23 = dVar15 < dVar22;
        }
        if (in_FPSCR_SZ == '\0') {
          *(float *)(puVar11 + 4) = fVar18;
        }
        else {
          *(double *)(puVar11 + 4) = dVar15;
        }
        if (bVar23) {
          iVar9 = iVar12 * 0xc + iVar10;
          if (in_FPSCR_SZ == '\0') {
            *(float *)(iVar9 + 4) = SUB84(dVar22,0);
          }
          else {
            *(undefined8 *)(iVar9 + 4) = in_xd14;
          }
        }
        puVar8 = PTR_FUN_8c026fd0;
        puVar6 = PTR_DAT_8c026fb4;
        if ((-1 < *pcVar3) && (*(char *)((int)puVar14 + -8) != -1)) {
          if (in_FPSCR_SZ == '\0') {
            dVar15 = (double)CONCAT44(*(undefined4 *)(pcVar13 + 0x20),
                                      *(undefined4 *)(pcVar13 + 0x28));
            param_1 = (double)CONCAT44(uVar20,uVar20);
          }
          else {
            dVar15 = *(double *)(pcVar13 + 0x20);
            in_xd4 = param_1;
          }
          if (in_FPSCR_PR == '\0') {
            fVar18 = SUB84(param_1,0) * SUB84(dVar21,0);
            fVar16 = (float)((ulonglong)dVar15 >> 0x20) + SUB84(dVar15,0);
          }
          else {
            fVar18 = SUB84(param_1 * dVar21,0);
            fVar16 = (float)((ulonglong)(dVar15 + dVar15) >> 0x20);
          }
          param_1 = dVar22;
          if (in_FPSCR_SZ == '\0') {
            param_1 = (double)(ulonglong)(uint)fVar18;
          }
          if (in_FPSCR_PR == '\0') {
            fVar18 = (float)iVar12 * (float)((ulonglong)dVar21 >> 0x20);
            dVar15 = (double)CONCAT44(fVar16 - fVar18,fVar18);
          }
          else {
            dVar15 = (double)CONCAT44(fVar16,(float)iVar12) * dVar21;
            dVar15 = dVar15 - dVar15;
          }
          if (in_FPSCR_SZ == '\0') {
            dVar15 = (double)CONCAT44((int)((ulonglong)dVar15 >> 0x20),DAT_8c026fc8);
          }
          if (in_FPSCR_PR == '\0') {
            dVar15 = (double)CONCAT44((float)((ulonglong)dVar15 >> 0x20) + SUB84(dVar15,0),
                                      SUB84(dVar15,0));
          }
          else {
            dVar15 = dVar15 + dVar15;
          }
          if (in_FPSCR_SZ == '\0') {
            *(int *)puVar5 = (int)((ulonglong)dVar15 >> 0x20);
            *(undefined4 *)(puVar5 + 4) = DAT_8c026fcc;
            *(int *)(puVar5 + 0xc) = SUB84(param_1,0);
            *(int *)(puVar5 + 0x10) = SUB84(param_1,0);
          }
          else {
            *(double *)puVar5 = dVar15;
            *(ulonglong *)(puVar5 + 4) = CONCAT44(PTR_FUN_8c026fd0,DAT_8c026fcc);
            *(double *)(puVar5 + 0xc) = in_xd4;
            *(double *)(puVar5 + 0x10) = in_xd4;
          }
          (*(code *)puVar8)(puVar6,(int)*(char *)((int)puVar14 + -8),0);
        }
        iVar12 = iVar12 + 1;
      } while (iVar12 < (int)(uint)*pbVar4);
    }
    if (*(int *)(pcVar13 + 0x14) < 0x4000) {
      *(int *)(pcVar13 + 0x14) = *(int *)(pcVar13 + 0x14) + 0x400;
    }
    uVar2 = *(ushort *)(pcVar13 + 6);
    *(ushort *)(pcVar13 + 6) = uVar2 + 1;
    if (uVar2 < 0x97) {
      if (*DAT_8c026fb8 != 0) {
        *DAT_8c026fb8 = 0;
        pcVar13[6] = '\0';
        pcVar13[7] = '\0';
      }
    }
    else {
      *pcVar13 = *pcVar13 + '\x01';
      pcVar13[6] = '\0';
      pcVar13[7] = '\0';
    }
  }
  else if (cVar1 == '\x02') {
    dVar15 = (double)(*(code *)PTR_FUN_8c027120)(*(undefined4 *)(pcVar13 + 0x14));
    if (in_FPSCR_PR == '\0') {
      dVar15 = (double)CONCAT44(-(float)((ulonglong)dVar15 >> 0x20) + SUB84(dVar22,0),
                                SUB84(dVar15,0));
    }
    else {
      dVar15 = -dVar15 + dVar22;
    }
    if (in_FPSCR_SZ == '\0') {
      in_dr2 = (double)CONCAT44((int)((ulonglong)in_dr2 >> 0x20),*(undefined4 *)(pcVar13 + 0x24));
    }
    if (in_FPSCR_PR == '\0') {
      dVar15 = (double)CONCAT44((float)((ulonglong)dVar15 >> 0x20) * SUB84(in_dr2,0),SUB84(dVar15,0)
                               );
    }
    else {
      dVar15 = dVar15 * in_dr2;
    }
    if (in_FPSCR_SZ == '\0') {
      *(int *)(pcVar13 + 0x20) = (int)((ulonglong)dVar15 >> 0x20);
    }
    else {
      *(double *)(pcVar13 + 0x20) = dVar15;
    }
    iVar12 = 0;
    if (*pbVar4 != 0) {
      do {
        puVar11 = (undefined1 *)(iVar12 * 0xc + iVar10);
        *(undefined1 *)((int)puVar14 + -8) = *puVar11;
        if (in_FPSCR_SZ == '\0') {
          uVar19 = *(uint *)(puVar11 + 4);
          param_1 = (double)(ulonglong)uVar19;
          dVar15 = (double)CONCAT44(uVar19,DAT_8c027124);
        }
        else {
          dVar15 = *(double *)(puVar11 + 4);
          uVar19 = SUB84(param_1,0);
          in_xd4 = dVar15;
        }
        if (in_FPSCR_PR == '\0') {
          fVar18 = (float)((ulonglong)dVar15 >> 0x20) - SUB84(dVar15,0);
          dVar15 = (double)CONCAT44(fVar18,SUB84(dVar15,0));
          bVar23 = fVar18 < SUB84(dVar22,0);
        }
        else {
          dVar15 = dVar15 - dVar15;
          fVar18 = (float)((ulonglong)dVar15 >> 0x20);
          bVar23 = dVar15 < dVar22;
        }
        if (in_FPSCR_SZ == '\0') {
          *(float *)(puVar11 + 4) = fVar18;
        }
        else {
          *(double *)(puVar11 + 4) = dVar15;
        }
        if (bVar23) {
          iVar9 = iVar12 * 0xc + iVar10;
          if (in_FPSCR_SZ == '\0') {
            *(float *)(iVar9 + 4) = SUB84(dVar22,0);
          }
          else {
            *(undefined8 *)(iVar9 + 4) = in_xd14;
          }
        }
        puVar8 = PTR_FUN_8c027134;
        puVar6 = PTR_DAT_8c027130;
        if ((-1 < *pcVar3) && (*(char *)((int)puVar14 + -8) != -1)) {
          if (in_FPSCR_SZ == '\0') {
            fVar18 = *(float *)(pcVar13 + 0x20);
            dVar15 = (double)CONCAT44(uVar19,uVar19);
          }
          else {
            fVar18 = (float)((ulonglong)*(undefined8 *)(pcVar13 + 0x20) >> 0x20);
            dVar15 = in_xd4;
          }
          if (in_FPSCR_PR == '\0') {
            dVar15 = (double)CONCAT44((float)((ulonglong)dVar15 >> 0x20) * SUB84(dVar21,0),
                                      SUB84(dVar15,0));
          }
          else {
            dVar15 = dVar15 * dVar21;
          }
          if (in_FPSCR_PR == '\0') {
            fVar16 = (float)iVar12 * (float)((ulonglong)dVar21 >> 0x20);
            dVar17 = (double)CONCAT44(fVar18 - fVar16,fVar16);
          }
          else {
            dVar17 = (double)CONCAT44(fVar18,(float)iVar12) * dVar21;
            dVar17 = dVar17 - dVar17;
          }
          if (in_FPSCR_SZ == '\0') {
            dVar17 = (double)CONCAT44((int)((ulonglong)dVar17 >> 0x20),DAT_8c027128);
          }
          if (in_FPSCR_PR == '\0') {
            dVar17 = (double)CONCAT44((float)((ulonglong)dVar17 >> 0x20) + SUB84(dVar17,0),
                                      SUB84(dVar17,0));
          }
          else {
            dVar17 = dVar17 + dVar17;
          }
          uVar20 = (undefined4)((ulonglong)dVar15 >> 0x20);
          if (in_FPSCR_SZ == '\0') {
            *(int *)puVar5 = (int)((ulonglong)dVar17 >> 0x20);
            *(undefined4 *)(puVar5 + 4) = DAT_8c02712c;
            *(undefined4 *)(puVar5 + 0xc) = uVar20;
            *(undefined4 *)(puVar5 + 0x10) = uVar20;
            param_1 = (double)((ulonglong)dVar15 & 0xffffffff);
          }
          else {
            *(double *)puVar5 = dVar17;
            *(ulonglong *)(puVar5 + 4) = CONCAT44(PTR_DAT_8c027130,DAT_8c02712c);
            *(double *)(puVar5 + 0xc) = dVar15;
            *(double *)(puVar5 + 0x10) = dVar15;
            param_1 = dVar22;
          }
          (*(code *)puVar8)(puVar6,(int)*(char *)((int)puVar14 + -8),0);
        }
        iVar12 = iVar12 + 1;
      } while (iVar12 < (int)(uint)*pbVar4);
    }
    iVar10 = *(int *)(pcVar13 + 0x14);
    *(int *)(pcVar13 + 0x14) = iVar10 + -0x200;
    if (iVar10 + -0x200 == 0) {
      *pcVar13 = '\0';
      pcVar13[6] = '\0';
      pcVar13[7] = '\0';
    }
  }
  return;
}


/* Address: 8c0272cc */

void FUN_8c0272cc(void)

{
  (*(code *)PTR_FUN_8c027344)(2,PTR_LAB_8c027340);
  return;
}


/* Address: 8c027360 */

void FUN_8c027360(undefined4 param_1,undefined4 param_2,double param_3,double *param_4,
                 double *param_5,undefined4 *param_6,double *param_7)

{
  float fVar1;
  double in_dr2;
  double dVar3;
  double dVar4;
  float fVar5;
  float fVar6;
  double dVar7;
  double in_xd6;
  double in_xd8;
  char in_FPSCR_PR;
  char in_FPSCR_SZ;
  double dVar2;
  
  if (in_FPSCR_SZ == '\0') {
    param_3 = (double)CONCAT44((int)((ulonglong)param_3 >> 0x20),*(undefined4 *)param_5);
    param_5 = (double *)((int)param_5 + 4);
    dVar3 = (double)CONCAT44(*(undefined4 *)param_4,param_1);
    param_4 = (double *)((int)param_4 + 4);
    in_dr2 = (double)CONCAT44((int)((ulonglong)in_dr2 >> 0x20),*param_6);
    param_6 = param_6 + 1;
  }
  else {
    in_xd8 = *param_5;
    param_5 = param_5 + 1;
    dVar3 = *param_4;
    param_4 = param_4 + 1;
    param_6 = param_6 + 2;
  }
  fVar6 = (float)((ulonglong)dVar3 >> 0x20);
  if (in_FPSCR_PR == '\0') {
    fVar5 = SUB84(param_3,0) - fVar6;
  }
  else {
    fVar5 = SUB84(param_3 - dVar3,0);
  }
  if (in_FPSCR_SZ == '\0') {
    dVar7 = (double)CONCAT44(*(undefined4 *)param_5,fVar5);
    param_5 = (double *)((int)param_5 + 4);
  }
  else {
    dVar7 = *param_5;
    param_5 = param_5 + 1;
  }
  if (in_FPSCR_PR == '\0') {
    dVar3 = (double)CONCAT44(fVar6 - SUB84(in_dr2,0),SUB84(dVar3,0));
  }
  else {
    dVar3 = dVar3 - in_dr2;
  }
  if (in_FPSCR_SZ == '\0') {
    dVar4 = (double)CONCAT44(*(undefined4 *)param_4,param_2);
    param_4 = (double *)((int)param_4 + 4);
    in_dr2 = (double)CONCAT44((int)((ulonglong)in_dr2 >> 0x20),*param_6);
    param_6 = param_6 + 1;
  }
  else {
    dVar4 = *param_4;
    param_4 = param_4 + 1;
    param_6 = param_6 + 2;
  }
  fVar6 = (float)((ulonglong)dVar4 >> 0x20);
  if (in_FPSCR_PR == '\0') {
    dVar7 = (double)CONCAT44((float)((ulonglong)dVar7 >> 0x20) - fVar6,SUB84(dVar7,0));
  }
  else {
    dVar7 = dVar7 - dVar4;
  }
  if (in_FPSCR_SZ == '\0') {
    dVar3 = (double)CONCAT44((int)((ulonglong)dVar3 >> 0x20),*(undefined4 *)param_4);
  }
  if (in_FPSCR_PR == '\0') {
    dVar4 = (double)CONCAT44(fVar6 - SUB84(in_dr2,0),SUB84(dVar4,0));
  }
  else {
    dVar4 = dVar4 - in_dr2;
  }
  if (in_FPSCR_SZ == '\0') {
    dVar4 = (double)CONCAT44((int)((ulonglong)dVar4 >> 0x20),*(undefined4 *)param_5);
    in_dr2 = (double)CONCAT44((int)((ulonglong)in_dr2 >> 0x20),*param_6);
  }
  else {
    in_xd6 = *param_5;
  }
  if (in_FPSCR_PR == '\0') {
    dVar4 = (double)CONCAT44((int)((ulonglong)dVar4 >> 0x20),SUB84(dVar4,0) - SUB84(dVar3,0));
    dVar3 = (double)CONCAT44((int)((ulonglong)dVar3 >> 0x20),SUB84(dVar3,0) - SUB84(in_dr2,0));
  }
  else {
    dVar4 = dVar4 - dVar3;
    dVar3 = dVar3 - in_dr2;
  }
  fVar6 = (float)((ulonglong)dVar7 >> 0x20);
  if (in_FPSCR_SZ == '\0') {
    in_dr2 = (double)(ulonglong)(uint)fVar6;
  }
  fVar5 = (float)((ulonglong)dVar3 >> 0x20);
  if (in_FPSCR_PR == '\0') {
    dVar7 = (double)CONCAT44(fVar6 * fVar5,SUB84(dVar7,0));
  }
  else {
    dVar7 = dVar7 * dVar3;
  }
  if (in_FPSCR_SZ == '\0') {
    in_xd6 = (double)CONCAT44(SUB84(dVar4,0),SUB84(in_dr2,0));
  }
  fVar6 = (float)((ulonglong)dVar4 >> 0x20);
  if (in_FPSCR_PR == '\0') {
    fVar1 = SUB84(in_xd6,0) * SUB84(dVar3,0);
    dVar4 = (double)CONCAT44(fVar6,SUB84(dVar4,0) * fVar5);
    dVar2 = (double)CONCAT44((float)((ulonglong)in_xd6 >> 0x20) * fVar6 - fVar1,fVar1);
  }
  else {
    dVar2 = in_xd6 * dVar4 * dVar3;
    dVar4 = dVar4 * dVar3;
    dVar2 = dVar2 - dVar2;
  }
  if (in_FPSCR_SZ == '\0') {
    *(int *)param_7 = (int)((ulonglong)dVar2 >> 0x20);
    in_xd8 = (double)CONCAT44(SUB84(dVar7,0),SUB84(dVar2,0));
  }
  else {
    *param_7 = dVar2;
  }
  if (in_FPSCR_PR == '\0') {
    fVar6 = SUB84(dVar7,0) * (float)((ulonglong)dVar4 >> 0x20);
    dVar3 = (double)CONCAT44((float)((ulonglong)in_xd8 >> 0x20) * SUB84(dVar3,0) - SUB84(dVar4,0),
                             SUB84(in_xd8,0));
    dVar7 = (double)CONCAT44((float)((ulonglong)dVar7 >> 0x20) - fVar6,fVar6);
  }
  else {
    dVar3 = in_xd8 * dVar3 - dVar4;
    dVar7 = dVar7 * dVar4 - dVar7 * dVar4;
  }
  if (in_FPSCR_SZ == '\0') {
    *(int *)((int)param_7 + 4) = (int)((ulonglong)dVar3 >> 0x20);
    *(int *)(param_7 + 1) = (int)((ulonglong)dVar7 >> 0x20);
  }
  else {
    *(double *)((int)param_7 + 4) = dVar3;
    param_7[1] = dVar7;
  }
  return;
}


/* Address: 8c0273a2 */

bool FUN_8c0273a2(int *param_1)

{
  short sVar1;
  undefined *puVar2;
  char *pcVar3;
  int iVar4;
  short *psVar5;
  int iVar6;
  int *piVar7;
  bool bVar8;
  undefined4 uVar9;
  
  puVar2 = PTR_FUN_8c027424;
  uVar9 = 1;
  iVar4 = *param_1;
  bVar8 = false;
  if ((iVar4 == 0) || (iVar4 == DAT_8c027420)) {
    iVar6 = *(int *)param_1[3];
    psVar5 = (short *)param_1[2];
    piVar7 = param_1 + 4;
    for (iVar4 = param_1[1]; 0 < iVar4; iVar4 = iVar4 + -1) {
      sVar1 = *psVar5;
      psVar5 = psVar5 + 1;
      (*(code *)puVar2)(0,sVar1 * 0xc + iVar6,piVar7,uVar9);
      piVar7 = piVar7 + 3;
    }
    param_1[2] = (int)psVar5;
  }
  else if ((iVar4 == DAT_8c027428) || (iVar4 == DAT_8c02742c)) {
    param_1[1] = (int)(short)(*(ushort *)param_1[2] & DAT_8c0274e0);
    pcVar3 = DAT_8c0274ec;
    bVar8 = ((int)*(short *)param_1[2] & DAT_8c0274e8) != 0;
    if (((int)DAT_8c0274e2 < param_1[1]) && (param_1[1] = (int)DAT_8c0274e2, -1 < *pcVar3)) {
      (*(code *)PTR_FUN_8c0274f8)(DAT_8c0274f4,PTR_s_WARNING__STRIP_buf_is_FULL_8c11246e_2_8c0274f0)
      ;
    }
    psVar5 = (short *)(param_1[2] + 2);
    param_1[2] = (int)psVar5;
    iVar6 = *(int *)param_1[3];
    piVar7 = param_1 + 4;
    for (iVar4 = param_1[1]; 0 < iVar4; iVar4 = iVar4 + -1) {
      sVar1 = *psVar5;
      psVar5 = psVar5 + 1;
      (*(code *)puVar2)(0,sVar1 * 0xc + iVar6,piVar7,uVar9);
      piVar7 = piVar7 + 3;
    }
    param_1[2] = (int)psVar5;
  }
  return bVar8;
}


/* Address: 8c0274b8 */

bool FUN_8c0274b8(int *param_1)

{
  short sVar1;
  char *pcVar2;
  int iVar3;
  int *piVar4;
  short *psVar5;
  int *piVar6;
  int iVar7;
  bool bVar8;
  
  iVar3 = *param_1;
  bVar8 = false;
  if ((iVar3 == 0) || (iVar3 == DAT_8c0274e4)) {
    iVar7 = *(int *)param_1[3];
    psVar5 = (short *)param_1[2];
    piVar6 = param_1 + 4;
    for (iVar3 = param_1[1]; 0 < iVar3; iVar3 = iVar3 + -1) {
      sVar1 = *psVar5;
      psVar5 = psVar5 + 1;
      piVar4 = (int *)(sVar1 * 0xc + iVar7);
      *piVar6 = *piVar4;
      piVar6[1] = piVar4[1];
      piVar6[2] = piVar4[2];
      piVar6 = piVar6 + 3;
    }
  }
  else {
    if ((iVar3 != DAT_8c0274e8) && (iVar3 != DAT_8c0274fc)) {
      return false;
    }
    param_1[1] = (int)(short)(*(ushort *)param_1[2] & DAT_8c0275c0);
    pcVar2 = DAT_8c0275c8;
    bVar8 = ((int)*(short *)param_1[2] & DAT_8c0275c4) != 0;
    if (((int)DAT_8c0275c2 < param_1[1]) && (param_1[1] = (int)DAT_8c0275c2, -1 < *pcVar2)) {
      (*(code *)PTR_FUN_8c0275d4)(DAT_8c0275d0,PTR_s_WARNING__STRIP_buf_is_FULL_8c11246e_2_8c0275cc)
      ;
    }
    psVar5 = (short *)(param_1[2] + 2);
    param_1[2] = (int)psVar5;
    iVar7 = *(int *)param_1[3];
    piVar6 = param_1 + 4;
    for (iVar3 = param_1[1]; 0 < iVar3; iVar3 = iVar3 + -1) {
      sVar1 = *psVar5;
      psVar5 = psVar5 + 1;
      piVar4 = (int *)(sVar1 * 0xc + iVar7);
      *piVar6 = *piVar4;
      piVar6[1] = piVar4[1];
      piVar6[2] = piVar4[2];
      piVar6 = piVar6 + 3;
    }
  }
  param_1[2] = (int)psVar5;
  return bVar8;
}


/* Address: 8c0275e0 */

bool FUN_8c0275e0(undefined4 param_1,undefined4 param_2,int *param_3,int param_4)

{
  short sVar1;
  char *pcVar2;
  int iVar3;
  int iVar4;
  short *psVar5;
  int *piVar6;
  undefined8 *puVar7;
  double in_dr2;
  double dVar8;
  double dVar9;
  double dVar10;
  undefined8 in_xd2;
  bool bVar11;
  char in_FPSCR_PR;
  char in_FPSCR_SZ;
  
  iVar3 = *param_3;
  bVar11 = false;
  if ((iVar3 == 0) || (iVar3 == DAT_8c027660)) {
    iVar3 = param_3[1];
    if (in_FPSCR_SZ == '\0') {
      param_1 = *(undefined4 *)(param_4 + 0xc);
    }
    iVar4 = *(int *)param_3[3];
    if (in_FPSCR_SZ == '\0') {
      dVar10 = (double)CONCAT44(*(undefined4 *)(param_4 + 8),param_2);
      dVar9 = (double)CONCAT44(*(undefined4 *)(param_4 + 0x10),param_1);
    }
    else {
      dVar10 = *(double *)(param_4 + 8);
      dVar9 = *(double *)(param_4 + 0x10);
    }
    psVar5 = (short *)param_3[2];
    piVar6 = param_3 + 4;
    while (0 < iVar3) {
      sVar1 = *psVar5;
      psVar5 = psVar5 + 1;
      iVar3 = iVar3 + -1;
      puVar7 = (undefined8 *)(sVar1 * 0xc + iVar4);
      if (in_FPSCR_SZ == '\0') {
        in_dr2 = (double)CONCAT44((int)((ulonglong)in_dr2 >> 0x20),*(undefined4 *)puVar7);
        puVar7 = (undefined8 *)((int)puVar7 + 4);
      }
      else {
        in_xd2 = *puVar7;
        puVar7 = puVar7 + 1;
      }
      if (in_FPSCR_PR == '\0') {
        dVar8 = (double)CONCAT44((int)((ulonglong)in_dr2 >> 0x20),
                                 SUB84(in_dr2,0) + (float)((ulonglong)dVar10 >> 0x20));
      }
      else {
        dVar8 = in_dr2 + dVar10;
      }
      if (in_FPSCR_SZ == '\0') {
        *piVar6 = SUB84(dVar8,0);
        dVar8 = (double)CONCAT44((int)((ulonglong)dVar8 >> 0x20),*(undefined4 *)puVar7);
        puVar7 = (undefined8 *)((int)puVar7 + 4);
      }
      else {
        *(undefined8 *)piVar6 = in_xd2;
        in_xd2 = *puVar7;
        puVar7 = puVar7 + 1;
      }
      if (in_FPSCR_PR == '\0') {
        dVar8 = (double)CONCAT44((int)((ulonglong)dVar8 >> 0x20),SUB84(dVar8,0) + SUB84(dVar9,0));
      }
      else {
        dVar8 = dVar8 + dVar9;
      }
      if (in_FPSCR_SZ == '\0') {
        piVar6[1] = SUB84(dVar8,0);
        dVar8 = (double)CONCAT44((int)((ulonglong)dVar8 >> 0x20),*(undefined4 *)puVar7);
      }
      else {
        *(undefined8 *)(piVar6 + 1) = in_xd2;
        in_xd2 = *puVar7;
      }
      if (in_FPSCR_PR == '\0') {
        in_dr2 = (double)CONCAT44((int)((ulonglong)dVar8 >> 0x20),
                                  SUB84(dVar8,0) + (float)((ulonglong)dVar9 >> 0x20));
      }
      else {
        in_dr2 = dVar8 + dVar9;
      }
      if (in_FPSCR_SZ == '\0') {
        piVar6[2] = SUB84(in_dr2,0);
      }
      else {
        *(undefined8 *)(piVar6 + 2) = in_xd2;
      }
      piVar6 = piVar6 + 3;
    }
    param_3[2] = (int)psVar5;
  }
  else if ((iVar3 == DAT_8c027664) || (iVar3 == DAT_8c027668)) {
    param_3[1] = (int)(short)(*(ushort *)param_3[2] & DAT_8c027780);
    pcVar2 = DAT_8c027790;
    bVar11 = ((int)*(short *)param_3[2] & DAT_8c02778c) != 0;
    if (((int)DAT_8c027782 < param_3[1]) && (param_3[1] = (int)DAT_8c027782, -1 < *pcVar2)) {
      (*(code *)PTR_FUN_8c02779c)(DAT_8c027798,PTR_s_WARNING__STRIP_buf_is_FULL_8c11246e_2_8c027794)
      ;
    }
    psVar5 = (short *)(param_3[2] + 2);
    param_3[2] = (int)psVar5;
    iVar3 = param_3[1];
    if (in_FPSCR_SZ == '\0') {
      param_1 = *(undefined4 *)(param_4 + 0xc);
    }
    iVar4 = *(int *)param_3[3];
    if (in_FPSCR_SZ == '\0') {
      dVar10 = (double)CONCAT44(*(undefined4 *)(param_4 + 0x10),param_2);
      dVar9 = (double)CONCAT44(*(undefined4 *)(param_4 + 8),param_1);
    }
    else {
      dVar10 = *(double *)(param_4 + 0x10);
      dVar9 = *(double *)(param_4 + 8);
    }
    piVar6 = param_3 + 4;
    while (0 < iVar3) {
      sVar1 = *psVar5;
      psVar5 = psVar5 + 1;
      iVar3 = iVar3 + -1;
      puVar7 = (undefined8 *)(sVar1 * 0xc + iVar4);
      if (in_FPSCR_SZ == '\0') {
        in_dr2 = (double)CONCAT44((int)((ulonglong)in_dr2 >> 0x20),*(undefined4 *)puVar7);
        puVar7 = (undefined8 *)((int)puVar7 + 4);
      }
      else {
        in_xd2 = *puVar7;
        puVar7 = puVar7 + 1;
      }
      if (in_FPSCR_PR == '\0') {
        dVar8 = (double)CONCAT44((int)((ulonglong)in_dr2 >> 0x20),
                                 SUB84(in_dr2,0) + (float)((ulonglong)dVar9 >> 0x20));
      }
      else {
        dVar8 = in_dr2 + dVar9;
      }
      if (in_FPSCR_SZ == '\0') {
        *piVar6 = SUB84(dVar8,0);
        dVar8 = (double)CONCAT44((int)((ulonglong)dVar8 >> 0x20),*(undefined4 *)puVar7);
        puVar7 = (undefined8 *)((int)puVar7 + 4);
      }
      else {
        *(undefined8 *)piVar6 = in_xd2;
        in_xd2 = *puVar7;
        puVar7 = puVar7 + 1;
      }
      if (in_FPSCR_PR == '\0') {
        dVar8 = (double)CONCAT44((int)((ulonglong)dVar8 >> 0x20),SUB84(dVar8,0) + SUB84(dVar9,0));
      }
      else {
        dVar8 = dVar8 + dVar9;
      }
      if (in_FPSCR_SZ == '\0') {
        piVar6[1] = SUB84(dVar8,0);
        dVar8 = (double)CONCAT44((int)((ulonglong)dVar8 >> 0x20),*(undefined4 *)puVar7);
      }
      else {
        *(undefined8 *)(piVar6 + 1) = in_xd2;
        in_xd2 = *puVar7;
      }
      if (in_FPSCR_PR == '\0') {
        in_dr2 = (double)CONCAT44((int)((ulonglong)dVar8 >> 0x20),
                                  SUB84(dVar8,0) + (float)((ulonglong)dVar10 >> 0x20));
      }
      else {
        in_dr2 = dVar8 + dVar10;
      }
      if (in_FPSCR_SZ == '\0') {
        piVar6[2] = SUB84(in_dr2,0);
      }
      else {
        *(undefined8 *)(piVar6 + 2) = in_xd2;
      }
      piVar6 = piVar6 + 3;
    }
    param_3[2] = (int)psVar5;
  }
  return bVar11;
}


/* Address: 8c0285d2 */

void FUN_8c0285d2(void)

{
  (*(code *)PTR_FUN_8c028614)(DAT_8c028610,(int)DAT_8c028606);
  (*(code *)PTR_FUN_8c02861c)(DAT_8c028618,(int)DAT_8c028600);
  return;
}


/* Address: 8c028666 */

bool FUN_8c028666(int *param_1)

{
  undefined *puVar1;
  char *pcVar2;
  int iVar3;
  int *piVar4;
  bool bVar5;
  undefined4 uVar6;
  
  puVar1 = PTR_FUN_8c0286ac;
  uVar6 = 0;
  iVar3 = *param_1;
  bVar5 = false;
  if ((iVar3 == 0) || (iVar3 == DAT_8c0286a2)) {
    piVar4 = param_1 + 4;
    for (iVar3 = param_1[1]; 0 < iVar3; iVar3 = iVar3 + -1) {
      (*(code *)puVar1)(0,*(int *)param_1[3] + *(short *)param_1[2] * 0xc,piVar4,uVar6);
      param_1[2] = param_1[2] + 2;
      piVar4 = piVar4 + 3;
    }
  }
  else if ((iVar3 == DAT_8c0286b0) || (iVar3 == DAT_8c0286b4)) {
    param_1[1] = (int)(short)(*(ushort *)param_1[2] & DAT_8c028780);
    pcVar2 = DAT_8c028788;
    bVar5 = ((int)*(short *)param_1[2] & DAT_8c028784) != 0;
    if (((int)DAT_8c028782 < param_1[1]) && (param_1[1] = (int)DAT_8c028782, -1 < *pcVar2)) {
      (*(code *)PTR_FUN_8c028794)(DAT_8c028790,PTR_s_WARNING__STRIP_buf_is_FULL_8c11246e_2_8c02878c)
      ;
    }
    param_1[2] = param_1[2] + 2;
    piVar4 = param_1 + 4;
    for (iVar3 = param_1[1]; 0 < iVar3; iVar3 = iVar3 + -1) {
      (*(code *)puVar1)(0,*(int *)param_1[3] + *(short *)param_1[2] * 0xc,piVar4,uVar6);
      param_1[2] = param_1[2] + 2;
      piVar4 = piVar4 + 3;
    }
  }
  return bVar5;
}


/* Address: 8c0287a0 */

undefined4 FUN_8c0287a0(double param_1,double param_2,int param_3)

{
  double *pdVar1;
  undefined *puVar2;
  int iVar3;
  uint uVar4;
  undefined4 uVar5;
  int iVar6;
  int iVar7;
  ushort *puVar8;
  undefined8 *puVar9;
  undefined8 *puVar10;
  float fVar11;
  float fVar12;
  double extraout_dr0;
  double dVar13;
  float fVar14;
  undefined4 uVar15;
  double dVar16;
  double dVar17;
  undefined4 uVar19;
  double dVar18;
  double dVar20;
  undefined8 in_dr12;
  double dVar21;
  undefined4 uVar22;
  undefined8 in_dr14;
  double dVar23;
  float fVar24;
  undefined8 in_xd0;
  double in_xd2;
  double in_xd4;
  double in_xd12;
  double in_xd14;
  bool bVar25;
  undefined4 in_PR;
  char in_FPSCR_PR;
  char in_FPSCR_SZ;
  undefined1 auStack_3c [16];
  undefined8 uStack_2c;
  undefined8 uStack_24;
  
  puVar2 = PTR_FUN_8c028808;
  pdVar1 = DAT_8c028804;
  if (in_FPSCR_SZ == '\0') {
    uStack_24 = (double)CONCAT44((int)in_dr14,(undefined4)uStack_24);
                    /* WARNING: Ignoring partial resolution of indirect */
    uStack_24._0_4_ = (int)((ulonglong)in_dr14 >> 0x20);
    puVar10 = &uStack_2c;
    uStack_2c = CONCAT44((int)in_dr12,(int)((ulonglong)in_dr12 >> 0x20));
  }
  else {
    puVar10 = (undefined8 *)auStack_3c;
    uStack_24 = in_xd14;
    uStack_2c = in_dr14;
  }
  *(undefined4 *)((int)puVar10 + -4) = in_PR;
  iVar3 = (int)DAT_8c028800;
  *(int *)((int)puVar10 + iVar3 + 0x20) = param_3;
  iVar7 = *(int *)(param_3 + 4);
  *(int *)((int)puVar10 + iVar3 + 0x38) = iVar7;
  *(uint *)((int)puVar10 + iVar3 + 0x24) = (uint)*(ushort *)(iVar7 + 0x14);
  puVar8 = *(ushort **)(iVar7 + 0xc);
  do {
    if (*(int *)((int)puVar10 + iVar3 + 0x24) < 1) {
      return 1;
    }
    uVar4 = *puVar8 & DAT_8c02880c;
    *(uint *)((int)puVar10 + iVar3 + 0x2c) = uVar4;
    if (uVar4 == 0) {
      uVar5 = 3;
LAB_8c028822:
      *(undefined4 *)((int)puVar10 + iVar3 + 0x30) = uVar5;
LAB_8c028824:
      *(uint *)((int)puVar10 + iVar3 + 0x1c) = (uint)puVar8[1];
      *(undefined4 *)((int)puVar10 + iVar3 + 0x34) = *(undefined4 *)(puVar8 + 2);
      iVar7 = *(int *)((int)puVar10 + iVar3 + 0x1c);
      while (0 < iVar7) {
        uVar5 = FUN_8c028666((undefined1 *)((int)puVar10 + iVar3 + 0x2c));
        *(undefined4 *)((int)puVar10 + iVar3 + 0x28) = uVar5;
        iVar7 = 0;
        puVar9 = (undefined8 *)((int)puVar10 + iVar3 + 0x3c);
        dVar13 = extraout_dr0;
        if (0 < *(int *)((int)puVar10 + iVar3 + 0x30) + -2) {
          do {
            uVar5 = SUB84(param_2,0);
            if (in_FPSCR_SZ == '\0') {
              *(undefined4 *)((int)puVar10 + iVar3 + 0x10) = *(undefined4 *)puVar9;
              *(undefined4 *)((int)puVar10 + iVar3 + 0xc) = *(undefined4 *)((int)puVar9 + 0xc);
              *(undefined4 *)((int)puVar10 + iVar3 + 8) = *(undefined4 *)(puVar9 + 3);
              *(undefined4 *)((int)puVar10 + iVar3) = *(undefined4 *)((int)puVar9 + 4);
              *(undefined4 *)((int)puVar10 + iVar3 + -4) = *(undefined4 *)(puVar9 + 2);
              dVar21 = (double)CONCAT44(*(undefined4 *)((int)puVar9 + 0x1c),
                                        *(undefined4 *)((int)puVar9 + 0x14));
              dVar23 = (double)CONCAT44(*(undefined4 *)(puVar9 + 1),*(undefined4 *)(puVar9 + 4));
              dVar18 = (double)CONCAT44(DAT_8c028a60,SUB84(param_1,0));
              dVar16 = (double)CONCAT44(*(undefined4 *)((int)puVar10 + iVar3 + 0x10),
                                        *(undefined4 *)((int)puVar10 + iVar3 + 0xc));
            }
            else {
              *(undefined8 *)((int)puVar10 + iVar3 + 0x10) = *puVar9;
              *(undefined8 *)((int)puVar10 + iVar3 + 0xc) = *(undefined8 *)((int)puVar9 + 0xc);
              *(undefined8 *)((int)puVar10 + iVar3 + 8) = puVar9[3];
              *(undefined8 *)((int)puVar10 + iVar3) = *(undefined8 *)((int)puVar9 + 4);
              *(undefined8 *)((int)puVar10 + iVar3 + -4) = puVar9[2];
              dVar21 = *(double *)((int)puVar9 + 0x1c);
              in_xd12 = *(double *)((int)puVar9 + 0x14);
              in_xd14 = (double)puVar9[4];
              dVar23 = (double)puVar9[1];
              dVar18 = (double)CONCAT44(DAT_8c028a64,DAT_8c028a60);
              in_xd2 = *(double *)((int)puVar10 + iVar3 + 0xc);
              dVar16 = *(double *)((int)puVar10 + iVar3 + 0x10);
            }
            if (in_FPSCR_PR == '\0') {
              dVar16 = (double)CONCAT44((float)((ulonglong)dVar16 >> 0x20) + SUB84(dVar16,0),
                                        SUB84(dVar16,0));
            }
            else {
              dVar16 = dVar16 + dVar16;
            }
            if (in_FPSCR_SZ == '\0') {
              dVar13 = (double)CONCAT44((int)((ulonglong)dVar13 >> 0x20),
                                        *(undefined4 *)((int)puVar10 + iVar3 + 8));
            }
            else {
              in_xd0 = *(undefined8 *)((int)puVar10 + iVar3 + 8);
            }
            fVar12 = (float)((ulonglong)dVar18 >> 0x20);
            if (in_FPSCR_PR == '\0') {
              dVar16 = (double)CONCAT44(((float)((ulonglong)dVar16 >> 0x20) + SUB84(dVar13,0)) *
                                        fVar12,SUB84(dVar16,0));
            }
            else {
              dVar16 = (dVar16 + dVar13) * dVar18;
            }
            if (in_FPSCR_SZ == '\0') {
              *(int *)((int)puVar10 + iVar3 + 0x14) = (int)((ulonglong)dVar16 >> 0x20);
              dVar13 = (double)CONCAT44(*(undefined4 *)((int)puVar10 + iVar3),SUB84(dVar13,0));
              dVar16 = (double)CONCAT44((int)((ulonglong)dVar23 >> 0x20),
                                        *(undefined4 *)((int)puVar10 + iVar3 + -4));
            }
            else {
              *(double *)((int)puVar10 + iVar3 + 0x14) = dVar16;
              dVar13 = *(double *)((int)puVar10 + iVar3);
              in_xd2 = *(double *)((int)puVar10 + iVar3 + -4);
              dVar16 = dVar23;
            }
            if (in_FPSCR_PR == '\0') {
              dVar17 = (double)CONCAT44(((float)((ulonglong)dVar16 >> 0x20) + SUB84(dVar21,0) +
                                        SUB84(dVar23,0)) * fVar12,SUB84(dVar16,0));
              dVar13 = (double)CONCAT44(((float)((ulonglong)dVar13 >> 0x20) + SUB84(dVar16,0) +
                                        (float)((ulonglong)dVar21 >> 0x20)) * fVar12,SUB84(dVar13,0)
                                       );
            }
            else {
              dVar17 = (dVar16 + dVar21 + dVar23) * dVar18;
              dVar13 = (dVar13 + dVar16 + dVar21 + dVar21) * dVar18;
            }
            uVar19 = (undefined4)((ulonglong)dVar17 >> 0x20);
            if (in_FPSCR_SZ == '\0') {
              *(int *)((int)puVar10 + iVar3 + 0x18) = (int)((ulonglong)dVar13 >> 0x20);
              *(undefined4 *)((int)puVar10 + iVar3 + 4) = uVar19;
              dVar17 = (double)CONCAT44(uVar19,*(undefined4 *)DAT_8c028a64);
              dVar13 = (double)CONCAT44(uVar19,SUB84(dVar18,0));
            }
            else {
              *(double *)((int)puVar10 + iVar3 + 0x18) = dVar13;
              *(double *)((int)puVar10 + iVar3 + 4) = dVar17;
              in_xd2 = *DAT_8c028a64;
              dVar13 = dVar17;
            }
            if (in_FPSCR_PR == '\0') {
              uVar19 = SUB84(dVar13,0);
            }
            else {
              uVar19 = SUB84(dVar13 - dVar17,0);
            }
            iVar6 = (*(code *)PTR_FUN_8c028a68)();
            if (iVar6 == 0) {
              return 0;
            }
            uVar15 = (undefined4)((ulonglong)dVar17 >> 0x20);
            fVar11 = SUB84(dVar23,0);
            fVar24 = (float)((ulonglong)dVar23 >> 0x20);
            fVar12 = SUB84(dVar21,0);
            uVar22 = (undefined4)((ulonglong)dVar21 >> 0x20);
            if ((*(int *)((int)puVar10 + iVar3 + 0x28) + iVar7 & 1U) == 0) {
              if (in_FPSCR_SZ == '\0') {
                *(undefined4 *)(iVar6 + 0x18) = *(undefined4 *)((int)puVar10 + iVar3 + 0x10);
                *(undefined4 *)(iVar6 + 0x1c) = *(undefined4 *)((int)puVar10 + iVar3);
                *(float *)(iVar6 + 0x20) = fVar24;
                *(undefined4 *)(iVar6 + 0x24) = *(undefined4 *)((int)puVar10 + iVar3 + 0xc);
                *(undefined4 *)(iVar6 + 0x28) = *(undefined4 *)((int)puVar10 + iVar3 + -4);
                *(float *)(iVar6 + 0x2c) = fVar12;
                dVar17 = (double)CONCAT44(uVar15,*(undefined4 *)((int)puVar10 + iVar3 + 8));
                *(undefined4 *)(iVar6 + 0x30) = *(undefined4 *)((int)puVar10 + iVar3 + 8);
                *(undefined4 *)(iVar6 + 0x34) = uVar22;
                *(float *)(iVar6 + 0x38) = fVar11;
              }
              else {
                *(undefined8 *)(iVar6 + 0x18) = *(undefined8 *)((int)puVar10 + iVar3 + 0x10);
                *(undefined8 *)(iVar6 + 0x1c) = *(undefined8 *)((int)puVar10 + iVar3);
                *(double *)(iVar6 + 0x20) = dVar23;
                *(undefined8 *)(iVar6 + 0x24) = *(undefined8 *)((int)puVar10 + iVar3 + 0xc);
                *(undefined8 *)(iVar6 + 0x28) = *(undefined8 *)((int)puVar10 + iVar3 + -4);
                *(double *)(iVar6 + 0x2c) = in_xd12;
                in_xd2 = *(double *)((int)puVar10 + iVar3 + 8);
                *(double *)(iVar6 + 0x30) = in_xd2;
                *(double *)(iVar6 + 0x34) = dVar21;
                *(double *)(iVar6 + 0x38) = in_xd14;
              }
            }
            else if (in_FPSCR_SZ == '\0') {
              *(undefined4 *)(iVar6 + 0x18) = *(undefined4 *)((int)puVar10 + iVar3 + 0x10);
              *(undefined4 *)(iVar6 + 0x1c) = *(undefined4 *)((int)puVar10 + iVar3);
              *(float *)(iVar6 + 0x20) = fVar24;
              *(undefined4 *)(iVar6 + 0x24) = *(undefined4 *)((int)puVar10 + iVar3 + 8);
              *(undefined4 *)(iVar6 + 0x28) = uVar22;
              *(float *)(iVar6 + 0x2c) = fVar11;
              *(undefined4 *)(iVar6 + 0x30) = *(undefined4 *)((int)puVar10 + iVar3 + 0xc);
              dVar17 = (double)CONCAT44(uVar15,*(undefined4 *)((int)puVar10 + iVar3 + -4));
              *(undefined4 *)(iVar6 + 0x34) = *(undefined4 *)((int)puVar10 + iVar3 + -4);
              *(float *)(iVar6 + 0x38) = fVar12;
            }
            else {
              *(undefined8 *)(iVar6 + 0x18) = *(undefined8 *)((int)puVar10 + iVar3 + 0x10);
              *(undefined8 *)(iVar6 + 0x1c) = *(undefined8 *)((int)puVar10 + iVar3);
              *(double *)(iVar6 + 0x20) = dVar23;
              *(undefined8 *)(iVar6 + 0x24) = *(undefined8 *)((int)puVar10 + iVar3 + 8);
              *(double *)(iVar6 + 0x28) = dVar21;
              *(double *)(iVar6 + 0x2c) = in_xd14;
              *(undefined8 *)(iVar6 + 0x30) = *(undefined8 *)((int)puVar10 + iVar3 + 0xc);
              in_xd2 = *(double *)((int)puVar10 + iVar3 + -4);
              *(double *)(iVar6 + 0x34) = in_xd2;
              *(double *)(iVar6 + 0x38) = in_xd12;
            }
            dVar13 = in_xd12;
            if (in_FPSCR_SZ == '\0') {
              dVar17 = (double)CONCAT44((int)((ulonglong)dVar17 >> 0x20),fVar12);
              dVar13 = in_xd2;
            }
            if (in_FPSCR_PR == '\0') {
              fVar14 = SUB84(dVar17,0) - fVar24;
            }
            else {
              fVar14 = SUB84(dVar17 - dVar23,0);
            }
            dVar18 = in_xd14;
            if (in_FPSCR_SZ == '\0') {
              dVar18 = (double)CONCAT44(fVar11,fVar14);
            }
            if (in_FPSCR_PR == '\0') {
              dVar18 = (double)CONCAT44((float)((ulonglong)dVar18 >> 0x20) - fVar12,
                                        ABS(SUB84(dVar18,0)));
            }
            else {
              dVar18 = ABS(dVar18 - dVar21);
            }
            in_xd2 = dVar23;
            if (in_FPSCR_SZ == '\0') {
              uVar15 = SUB84(dVar18,0);
              dVar18 = (double)CONCAT44((int)((ulonglong)dVar18 >> 0x20),fVar24);
              in_xd2 = dVar13;
              dVar13 = (double)CONCAT44(uVar15,uVar19);
            }
            if (in_FPSCR_PR == '\0') {
              dVar18 = (double)CONCAT44(ABS((float)((ulonglong)dVar18 >> 0x20)),
                                        SUB84(dVar18,0) - fVar11);
            }
            else {
              dVar18 = ABS(dVar18 - dVar23);
            }
            uVar19 = (undefined4)((ulonglong)dVar18 >> 0x20);
            dVar16 = dVar18;
            if (in_FPSCR_SZ == '\0') {
              dVar13 = (double)CONCAT44((int)((ulonglong)dVar13 >> 0x20),uVar19);
              dVar16 = in_xd4;
            }
            fVar14 = SUB84(dVar13,0);
            if (in_FPSCR_PR == '\0') {
              dVar18 = (double)CONCAT44(uVar19,ABS(SUB84(dVar18,0)));
            }
            else {
              dVar18 = ABS(dVar18);
            }
            dVar17 = in_xd2;
            if (in_FPSCR_SZ == '\0') {
              dVar17 = (double)CONCAT44(SUB84(dVar18,0),uVar5);
            }
            if ((in_FPSCR_PR == '\0' && (float)((ulonglong)dVar13 >> 0x20) < fVar14) &&
               (dVar13 = dVar16, in_FPSCR_SZ == '\0')) {
              dVar13 = (double)CONCAT44(fVar14,fVar14);
            }
            fVar14 = (float)((ulonglong)dVar17 >> 0x20);
            if (in_FPSCR_PR == '\0') {
              bVar25 = (float)((ulonglong)dVar13 >> 0x20) < fVar14;
            }
            else {
              bVar25 = dVar13 < dVar17;
            }
            if (in_FPSCR_SZ == '\0') {
              dVar18 = (double)CONCAT44((int)((ulonglong)dVar18 >> 0x20),*(undefined4 *)pdVar1);
            }
            else {
              in_xd2 = *pdVar1;
            }
            dVar20 = dVar13;
            if ((bVar25) && (dVar20 = dVar17, in_FPSCR_SZ == '\0')) {
              dVar20 = (double)CONCAT44(fVar14,SUB84(dVar13,0));
            }
            fVar14 = (float)((ulonglong)dVar20 >> 0x20);
            if (in_FPSCR_PR == '\0') {
              bVar25 = SUB84(dVar18,0) < fVar14;
            }
            else {
              bVar25 = dVar18 < dVar20;
            }
            if (bVar25) {
              if (in_FPSCR_SZ == '\0') {
                *(float *)pdVar1 = fVar14;
              }
              else {
                *pdVar1 = dVar20;
              }
            }
            if (in_FPSCR_SZ == '\0') {
              dVar13 = (double)CONCAT44(*(undefined4 *)((int)puVar10 + iVar3 + 0x10),
                                        *(undefined4 *)((int)puVar10 + iVar3 + 0x14));
            }
            else {
              in_xd2 = *(double *)((int)puVar10 + iVar3 + 0x14);
              dVar13 = *(double *)((int)puVar10 + iVar3 + 0x10);
            }
            if (in_FPSCR_PR == '\0') {
              dVar13 = (double)CONCAT44((float)((ulonglong)dVar13 >> 0x20) - SUB84(dVar13,0),
                                        SUB84(dVar13,0));
            }
            else {
              dVar13 = dVar13 - dVar13;
            }
            if (in_FPSCR_SZ == '\0') {
              *(int *)((int)puVar10 + iVar3 + 0x10) = (int)((ulonglong)dVar13 >> 0x20);
              dVar13 = (double)CONCAT44(*(undefined4 *)((int)puVar10 + iVar3),
                                        *(undefined4 *)((int)puVar10 + iVar3 + 0x18));
            }
            else {
              *(double *)((int)puVar10 + iVar3 + 0x10) = dVar13;
              in_xd0 = *(undefined8 *)((int)puVar10 + iVar3 + 0x18);
              dVar13 = *(double *)((int)puVar10 + iVar3);
            }
            if (in_FPSCR_PR == '\0') {
              dVar13 = (double)CONCAT44((float)((ulonglong)dVar13 >> 0x20) - SUB84(dVar13,0),
                                        SUB84(dVar13,0));
            }
            else {
              dVar13 = dVar13 - dVar13;
            }
            uVar5 = (undefined4)((ulonglong)dVar13 >> 0x20);
            if (in_FPSCR_SZ == '\0') {
              *(undefined4 *)((int)puVar10 + iVar3) = uVar5;
              dVar18 = (double)CONCAT44(*(undefined4 *)((int)puVar10 + iVar3 + 0x14),
                                        *(undefined4 *)((int)puVar10 + iVar3 + 4));
              dVar13 = (double)CONCAT44(uVar5,*(undefined4 *)((int)puVar10 + iVar3 + 0xc));
            }
            else {
              *(double *)((int)puVar10 + iVar3) = dVar13;
              in_xd2 = *(double *)((int)puVar10 + iVar3 + 4);
              dVar18 = *(double *)((int)puVar10 + iVar3 + 0x14);
              in_xd0 = *(undefined8 *)((int)puVar10 + iVar3 + 0xc);
            }
            fVar14 = (float)((ulonglong)dVar18 >> 0x20);
            if (in_FPSCR_PR == '\0') {
              dVar23 = (double)CONCAT44(fVar24 - SUB84(dVar18,0),fVar11);
              fVar11 = SUB84(dVar13,0) - fVar14;
            }
            else {
              dVar23 = dVar23 - dVar18;
              fVar11 = SUB84(dVar13 - dVar18,0);
            }
            if (in_FPSCR_SZ == '\0') {
              *(float *)((int)puVar10 + iVar3 + 0xc) = fVar11;
              dVar18 = (double)CONCAT44(fVar14,*(undefined4 *)((int)puVar10 + iVar3 + 0x18));
              dVar13 = (double)CONCAT44(*(undefined4 *)((int)puVar10 + iVar3 + -4),fVar11);
            }
            else {
              *(undefined8 *)((int)puVar10 + iVar3 + 0xc) = in_xd0;
              in_xd2 = *(double *)((int)puVar10 + iVar3 + 0x18);
              dVar13 = *(double *)((int)puVar10 + iVar3 + -4);
            }
            if (in_FPSCR_PR == '\0') {
              dVar13 = (double)CONCAT44((float)((ulonglong)dVar13 >> 0x20) - SUB84(dVar18,0),
                                        SUB84(dVar13,0));
            }
            else {
              dVar13 = dVar13 - dVar18;
            }
            uVar5 = (undefined4)((ulonglong)dVar13 >> 0x20);
            if (in_FPSCR_SZ == '\0') {
              *(undefined4 *)((int)puVar10 + iVar3 + -4) = uVar5;
              dVar18 = (double)CONCAT44(*(undefined4 *)((int)puVar10 + iVar3 + 4),SUB84(dVar18,0));
            }
            else {
              *(double *)((int)puVar10 + iVar3 + -4) = dVar13;
              dVar18 = *(double *)((int)puVar10 + iVar3 + 4);
            }
            fVar11 = (float)((ulonglong)dVar18 >> 0x20);
            if (in_FPSCR_PR == '\0') {
              dVar21 = (double)CONCAT44(uVar22,fVar12 - fVar11);
            }
            else {
              dVar21 = dVar21 - dVar18;
            }
            if (in_FPSCR_SZ == '\0') {
              dVar18 = (double)CONCAT44(fVar11,*(undefined4 *)((int)puVar10 + iVar3 + 0x14));
              dVar13 = (double)CONCAT44(uVar5,*(undefined4 *)((int)puVar10 + iVar3 + 8));
              dVar17 = (double)((ulonglong)dVar17 & 0xffffffff);
            }
            else {
              in_xd2 = *(double *)((int)puVar10 + iVar3 + 0x14);
              in_xd0 = *(undefined8 *)((int)puVar10 + iVar3 + 8);
              dVar17 = dVar23;
            }
            uVar4 = SUB84(dVar17,0);
            if (in_FPSCR_PR == '\0') {
              fVar12 = SUB84(dVar13,0) - SUB84(dVar18,0);
            }
            else {
              fVar12 = SUB84(dVar13 - dVar18,0);
            }
            if (in_FPSCR_SZ == '\0') {
              *(float *)((int)puVar10 + iVar3 + 8) = fVar12;
              dVar13 = (double)CONCAT44(*(undefined4 *)((int)puVar10 + iVar3 + 0x18),
                                        *(undefined4 *)((int)puVar10 + iVar3 + 4));
            }
            else {
              *(undefined8 *)((int)puVar10 + iVar3 + 8) = in_xd0;
              dVar13 = *(double *)((int)puVar10 + iVar3 + 0x18);
              in_xd2 = *(double *)((int)puVar10 + iVar3 + 4);
            }
            if (in_FPSCR_PR == '\0') {
              in_xd4 = (double)CONCAT44((float)((ulonglong)dVar21 >> 0x20) -
                                        (float)((ulonglong)dVar13 >> 0x20),SUB84(dVar21,0));
              fVar12 = SUB84(dVar23,0) - SUB84(dVar13,0);
            }
            else {
              in_xd4 = dVar21 - dVar13;
              fVar12 = SUB84(dVar23 - dVar13,0);
            }
            if (in_FPSCR_SZ == '\0') {
              *(undefined4 *)(iVar6 + 8) = *(undefined4 *)((int)puVar10 + iVar3 + 0x14);
              *(undefined4 *)(iVar6 + 0xc) = *(undefined4 *)((int)puVar10 + iVar3 + 0x18);
              *(undefined4 *)(iVar6 + 0x10) = *(undefined4 *)((int)puVar10 + iVar3 + 4);
            }
            else {
              *(undefined8 *)(iVar6 + 8) = *(undefined8 *)((int)puVar10 + iVar3 + 0x14);
              *(undefined8 *)(iVar6 + 0xc) = *(undefined8 *)((int)puVar10 + iVar3 + 0x18);
              in_xd2 = *(double *)((int)puVar10 + iVar3 + 4);
              *(double *)(iVar6 + 0x10) = in_xd2;
              dVar16 = *(double *)((int)puVar10 + iVar3);
            }
            dVar13 = (double)(*(code *)puVar2)();
            if (in_FPSCR_SZ == '\0') {
              uVar5 = *(undefined4 *)((int)puVar10 + iVar3 + -4);
              dVar13 = (double)CONCAT44((int)((ulonglong)dVar13 >> 0x20),fVar12);
              dVar18 = (double)(ulonglong)uVar4;
            }
            else {
              dVar16 = *(double *)((int)puVar10 + iVar3 + -4);
              uVar5 = (undefined4)*(undefined8 *)((int)puVar10 + iVar3 + 0xc);
              dVar18 = in_xd12;
            }
            uVar19 = SUB84(dVar18,0);
            dVar18 = (double)(*(code *)puVar2)();
            if (in_FPSCR_SZ == '\0') {
              dVar18 = (double)CONCAT44((int)((ulonglong)dVar18 >> 0x20),uVar5);
            }
            fVar12 = (float)((ulonglong)dVar18 >> 0x20);
            if (in_FPSCR_PR == '\0') {
              bVar25 = (float)((ulonglong)dVar13 >> 0x20) < fVar12;
            }
            else {
              bVar25 = dVar13 < dVar18;
            }
            param_2 = in_xd14;
            if (in_FPSCR_SZ == '\0') {
              param_2 = (double)CONCAT44(SUB84(dVar13,0),uVar19);
            }
            dVar23 = dVar13;
            if ((bVar25) && (dVar23 = dVar18, in_FPSCR_SZ == '\0')) {
              dVar23 = (double)CONCAT44(fVar12,SUB84(dVar13,0));
            }
            if (in_FPSCR_SZ == '\0') {
              uVar5 = (undefined4)((ulonglong)in_xd4 >> 0x20);
              in_xd4 = dVar16;
            }
            else {
              uVar5 = (undefined4)*(undefined8 *)((int)puVar10 + iVar3 + 8);
            }
            dVar13 = (double)(*(code *)puVar2)();
            param_1 = dVar13;
            if (in_FPSCR_SZ == '\0') {
              param_1 = (double)CONCAT44((int)((ulonglong)dVar13 >> 0x20),uVar5);
            }
            fVar12 = (float)((ulonglong)param_1 >> 0x20);
            if (in_FPSCR_PR == '\0') {
              bVar25 = (float)((ulonglong)dVar23 >> 0x20) < fVar12;
            }
            else {
              bVar25 = dVar23 < param_1;
            }
            dVar18 = dVar23;
            if ((bVar25) && (dVar18 = param_1, in_FPSCR_SZ == '\0')) {
              dVar18 = (double)CONCAT44(fVar12,SUB84(dVar23,0));
            }
            if (in_FPSCR_SZ == '\0') {
              *(int *)(iVar6 + 0x14) = (int)((ulonglong)dVar18 >> 0x20);
            }
            else {
              *(double *)(iVar6 + 0x14) = dVar18;
            }
            if (*(int *)(*(int *)(*(int *)((int)puVar10 + iVar3 + 0x20) + 4) + 0x10) == 0) {
              *(undefined4 *)(iVar6 + 0x3c) = 0;
            }
            else {
              *(undefined4 *)(iVar6 + 0x3c) =
                   *(undefined4 *)
                    (*(int *)(*(int *)(*(int *)((int)puVar10 + iVar3 + 0x20) + 4) + 0x10) +
                     (*puVar8 & DAT_8c028a6c) * 0x14 + 0x10);
            }
            iVar7 = iVar7 + 1;
            puVar9 = (undefined8 *)((int)puVar9 + 0xc);
          } while (iVar7 < *(int *)((int)puVar10 + iVar3 + 0x30) + -2);
        }
        iVar7 = *(int *)((int)puVar10 + iVar3 + 0x1c) + -1;
        *(int *)((int)puVar10 + iVar3 + 0x1c) = iVar7;
      }
    }
    else {
      if (uVar4 == (int)DAT_8c028802) {
        uVar5 = 4;
        goto LAB_8c028822;
      }
      if ((uVar4 == DAT_8c028810) || (uVar4 == DAT_8c02880c)) goto LAB_8c028824;
    }
    puVar8 = puVar8 + 0xc;
    *(int *)((int)puVar10 + iVar3 + 0x24) = *(int *)((int)puVar10 + iVar3 + 0x24) + -1;
  } while( true );
}


/* Address: 8c028b00 */

undefined4 FUN_8c028b00(int param_1,undefined4 param_2,undefined4 param_3)

{
  undefined4 uVar1;
  int iVar2;
  
  (*(code *)PTR_FUN_8c028ba0)(0,0xc0);
  (*(code *)PTR_FUN_8c028ba4)(0,param_1 + 8);
  if (*(int *)(param_1 + 0x1c) != 0) {
    (*(code *)PTR_FUN_8c028ba8)(0,*(int *)(param_1 + 0x1c));
  }
  if (*(int *)(param_1 + 0x18) != 0) {
    (*(code *)PTR_FUN_8c028bac)(0,*(int *)(param_1 + 0x18));
  }
  if (*(int *)(param_1 + 0x14) != 0) {
    (*(code *)PTR_FUN_8c028bb0)(0,*(int *)(param_1 + 0x14));
  }
  (*(code *)PTR_FUN_8c028bb4)(0,param_1 + 0x20);
  for (iVar2 = *(int *)(param_1 + 0x2c); iVar2 != 0; iVar2 = FUN_8c028b00(iVar2,param_2,param_3)) {
  }
  iVar2 = FUN_8c0287a0(param_1,param_2,param_3);
  if (iVar2 == 0) {
    (*(code *)PTR_FUN_8c028bb8)(1);
    uVar1 = 0;
  }
  else {
    (*(code *)PTR_FUN_8c028bb8)(1);
    uVar1 = *(undefined4 *)(param_1 + 0x30);
  }
  return uVar1;
}


/* Address: 8c028bfe */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

int FUN_8c028bfe(uint param_1,double param_2,double param_3,double *param_4)

{
  int *piVar1;
  undefined *puVar2;
  char *pcVar3;
  short *psVar4;
  int iVar5;
  int *piVar6;
  undefined4 *puVar7;
  int iVar8;
  double *pdVar9;
  double *pdVar10;
  int *piVar11;
  undefined8 *puVar12;
  double dVar13;
  float fVar14;
  double in_dr2;
  undefined4 uVar15;
  double dVar16;
  float fVar17;
  float fVar18;
  float fVar19;
  double dVar20;
  double dVar21;
  float fVar22;
  undefined8 in_dr14;
  undefined4 uVar23;
  double in_xd2;
  double in_xd4;
  double in_xd6;
  double dVar24;
  double in_xd14;
  bool bVar25;
  undefined4 in_PR;
  char in_FPSCR_PR;
  char in_FPSCR_SZ;
  undefined1 auStack_2c [8];
  undefined8 uStack_24;
  
  puVar2 = PTR_FUN_8c028d48;
  piVar11 = DAT_8c028d44;
  piVar1 = DAT_8c028d40;
  if (in_FPSCR_SZ == '\0') {
    uStack_24 = (double)CONCAT44((int)in_dr14,(undefined4)uStack_24);
    puVar12 = &uStack_24;
                    /* WARNING: Ignoring partial resolution of indirect */
    uStack_24._0_4_ = (int)((ulonglong)in_dr14 >> 0x20);
  }
  else {
    puVar12 = (undefined8 *)auStack_2c;
    uStack_24 = in_xd14;
  }
  *(undefined4 *)((int)puVar12 + -4) = in_PR;
  *DAT_8c028d40 = 0;
  dVar13 = (double)(*(code *)puVar2)(param_4 + 2);
  if (in_FPSCR_SZ == '\0') {
    param_2 = (double)CONCAT44((int)((ulonglong)param_2 >> 0x20),*(undefined4 *)param_4);
    dVar13 = (double)CONCAT44((int)((ulonglong)dVar13 >> 0x20),(int)in_dr14);
  }
  else {
    in_xd6 = *param_4;
  }
  fVar14 = SUB84(param_2,0);
  if (in_FPSCR_PR == '\0') {
    dVar13 = (double)CONCAT44((float)((ulonglong)dVar13 >> 0x20) + fVar14,SUB84(dVar13,0));
  }
  else {
    dVar13 = dVar13 + param_2;
  }
  if (in_FPSCR_SZ == '\0') {
    param_1 = *(uint *)(param_4 + 1);
    uVar23 = (undefined4)((ulonglong)dVar13 >> 0x20);
    dVar13 = (double)CONCAT44(uVar23,uVar23);
  }
  else {
    fVar14 = (float)*(undefined8 *)((int)param_4 + 0xc);
    in_xd4 = param_4[1];
    in_xd14 = dVar13;
  }
  if (in_FPSCR_PR == '\0') {
    fVar19 = (float)((ulonglong)dVar13 >> 0x20);
    dVar13 = (double)CONCAT44(fVar19,SUB84(dVar13,0) * fVar19);
  }
  else {
    dVar13 = dVar13 * dVar13;
  }
  if (in_FPSCR_SZ == '\0') {
    dVar16 = (double)(ulonglong)param_1;
  }
  else {
    dVar16 = *(double *)((int)param_4 + 4);
  }
  (*(code *)PTR_FUN_8c028d4c)();
  uVar23 = _DAT_8c028d60;
  pcVar3 = DAT_8c028d58;
  puVar2 = PTR_FUN_8c028d54;
  puVar7 = *(undefined4 **)PTR_DAT_8c028d50;
  if (in_FPSCR_SZ == '\0') {
    dVar20 = (double)CONCAT44(DAT_8c028d5c,fVar14);
  }
  else {
    dVar20 = (double)CONCAT44(_DAT_8c028d60,DAT_8c028d5c);
  }
  do {
    iVar8 = DAT_8c028d68;
    psVar4 = DAT_8c028d64;
    uVar15 = SUB84(dVar16,0);
    if (puVar7 == (undefined4 *)0x0) {
LAB_8c028e18:
      return *piVar1;
    }
    puVar7[7] = 0;
    puVar7[9] = 0;
    bVar25 = false;
    for (iVar5 = (int)*psVar4; 0 < iVar5; iVar5 = iVar5 + -1) {
      if (*(int *)(iVar8 + 4) == puVar7[2]) {
        bVar25 = true;
        break;
      }
      iVar8 = iVar8 + 0xc;
    }
    if (bVar25) {
      piVar6 = (int *)puVar7[4];
      if (in_FPSCR_SZ == '\0') {
        uVar15 = *(undefined4 *)((int)param_4 + 0xc);
        dVar20 = (double)CONCAT44((int)((ulonglong)dVar20 >> 0x20),puVar7[10]);
        in_dr2 = (double)CONCAT44((int)((ulonglong)in_dr2 >> 0x20),uVar15);
      }
      else {
        in_xd2 = *(double *)((int)param_4 + 0xc);
        in_xd6 = *(double *)(puVar7 + 10);
        in_xd4 = in_xd2;
      }
      if (in_FPSCR_PR == '\0') {
        fVar14 = SUB84(in_dr2,0) - SUB84(dVar20,0);
      }
      else {
        fVar14 = SUB84(in_dr2 - dVar20,0);
      }
      if (in_FPSCR_SZ == '\0') {
        in_dr2 = (double)CONCAT44(piVar6[4],fVar14);
      }
      else {
        in_dr2 = *(double *)(piVar6 + 4);
      }
      fVar14 = (float)((ulonglong)dVar13 >> 0x20);
      if (in_FPSCR_PR == '\0') {
        dVar20 = (double)CONCAT44((int)((ulonglong)dVar20 >> 0x20),SUB84(dVar20,0) + fVar14);
      }
      else {
        dVar20 = dVar20 + dVar13;
      }
      iVar8 = puVar7[5];
      dVar16 = in_xd2;
      if (in_FPSCR_SZ == '\0') {
        dVar16 = (double)CONCAT44(SUB84(in_dr2,0),uVar15);
      }
      if (in_FPSCR_PR == '\0') {
        dVar16 = (double)CONCAT44((float)((ulonglong)dVar16 >> 0x20) - fVar14,SUB84(dVar16,0));
      }
      else {
        dVar16 = dVar16 - dVar13;
      }
      dVar21 = in_xd6;
      if (in_FPSCR_SZ == '\0') {
        dVar21 = (double)CONCAT44(SUB84(dVar20,0),SUB84(param_3,0));
      }
      fVar14 = (float)((ulonglong)in_dr2 >> 0x20);
      fVar19 = (float)((ulonglong)dVar16 >> 0x20);
      if (in_FPSCR_PR == '\0') {
        bVar25 = fVar14 < fVar19;
        param_3 = (double)CONCAT44((float)((ulonglong)dVar21 >> 0x20) + SUB84(dVar16,0),
                                   SUB84(dVar21,0));
      }
      else {
        bVar25 = in_dr2 < dVar16;
        param_3 = dVar21 + dVar16;
      }
      fVar22 = (float)((ulonglong)param_3 >> 0x20);
      if (bVar25) {
        if (in_FPSCR_SZ == '\0') {
          in_dr2 = (double)CONCAT44(fVar14,*(undefined4 *)(iVar8 + 0x10));
        }
        else {
          in_xd2 = *(double *)(iVar8 + 0x10);
        }
        if (in_FPSCR_PR == '\0') {
          bVar25 = fVar22 < SUB84(in_dr2,0);
        }
        else {
          bVar25 = param_3 < in_dr2;
        }
        if (bVar25) {
          if (in_FPSCR_SZ == '\0') {
            in_dr2 = (double)CONCAT44((int)((ulonglong)in_dr2 >> 0x20),piVar6[4]);
          }
          else {
            in_xd2 = *(double *)(piVar6 + 4);
          }
          if (in_FPSCR_PR == '\0') {
            bVar25 = fVar19 < SUB84(in_dr2,0);
          }
          else {
            bVar25 = dVar16 < in_dr2;
          }
          while (!bVar25) {
            piVar6 = (int *)*piVar6;
            if (piVar6 == (int *)0x0) goto LAB_8c028e02;
            if (in_FPSCR_SZ == '\0') {
              in_dr2 = (double)CONCAT44((int)((ulonglong)in_dr2 >> 0x20),piVar6[4]);
            }
            else {
              in_xd2 = *(double *)(piVar6 + 4);
            }
            if (in_FPSCR_PR == '\0') {
              bVar25 = fVar19 < SUB84(in_dr2,0);
            }
            else {
              bVar25 = dVar16 < in_dr2;
            }
          }
        }
      }
      if (piVar6 == (int *)0x0) {
LAB_8c028e02:
        do {
          if (in_FPSCR_SZ == '\0') {
            in_dr2 = (double)CONCAT44((int)((ulonglong)in_dr2 >> 0x20),*(undefined4 *)(iVar8 + 0x10)
                                     );
          }
          else {
            in_xd2 = *(double *)(iVar8 + 0x10);
          }
          fVar14 = (float)((ulonglong)dVar16 >> 0x20);
          if (in_FPSCR_PR == '\0') {
            bVar25 = fVar14 < SUB84(in_dr2,0);
          }
          else {
            bVar25 = dVar16 < in_dr2;
          }
          if ((!bVar25) || (iVar8 == 0)) goto LAB_8c028e0e;
          if (in_FPSCR_SZ == '\0') {
            in_dr2 = (double)CONCAT44(SUB84(dVar13,0),*(undefined4 *)(iVar8 + 0x14));
          }
          else {
            in_xd2 = *(double *)(iVar8 + 0x14);
            in_dr2 = in_xd14;
          }
          if (in_FPSCR_PR == '\0') {
            in_dr2 = (double)CONCAT44((float)((ulonglong)in_dr2 >> 0x20) + SUB84(in_dr2,0),
                                      SUB84(in_dr2,0));
          }
          else {
            in_dr2 = in_dr2 + in_dr2;
          }
          if (in_FPSCR_SZ == '\0') {
            in_dr2 = (double)CONCAT44((int)((ulonglong)in_dr2 >> 0x20),
                                      *(undefined4 *)((int)param_4 + 4));
            pdVar9 = param_4 + 1;
            param_3 = (double)CONCAT44(*(undefined4 *)(iVar8 + 8),SUB84(param_3,0));
            pdVar10 = (double *)(iVar8 + 0xc);
          }
          else {
            in_xd2 = *(double *)((int)param_4 + 4);
            pdVar9 = (double *)((int)param_4 + 0xc);
            param_3 = *(double *)(iVar8 + 8);
            pdVar10 = (double *)(iVar8 + 0x10);
          }
          if (in_FPSCR_PR == '\0') {
            param_3 = (double)CONCAT44((float)((ulonglong)param_3 >> 0x20) - SUB84(in_dr2,0),
                                       SUB84(param_3,0));
          }
          else {
            param_3 = param_3 - in_dr2;
          }
          uVar15 = (undefined4)((ulonglong)in_dr2 >> 0x20);
          dVar21 = in_dr2;
          if (in_FPSCR_SZ == '\0') {
            dVar16 = (double)CONCAT44(fVar14,uVar15);
            dVar21 = in_xd4;
          }
          fVar14 = (float)((ulonglong)dVar20 >> 0x20);
          if (in_FPSCR_PR == '\0') {
            dVar16 = (double)CONCAT44((int)((ulonglong)dVar16 >> 0x20),SUB84(dVar16,0) * fVar14);
          }
          else {
            dVar16 = dVar16 * dVar20;
          }
          fVar19 = (float)((ulonglong)param_3 >> 0x20);
          dVar24 = param_3;
          if (in_FPSCR_SZ == '\0') {
            dVar20 = (double)CONCAT44(fVar14,fVar19);
            dVar24 = in_xd6;
          }
          fVar14 = SUB84(dVar16,0);
          if (in_FPSCR_PR == '\0') {
            fVar19 = SUB84(dVar20,0) * fVar19;
            dVar20 = (double)CONCAT44((int)((ulonglong)dVar20 >> 0x20),fVar19);
            bVar25 = fVar14 < fVar19;
          }
          else {
            dVar20 = dVar20 * param_3;
            fVar19 = SUB84(dVar20,0);
            bVar25 = dVar16 < dVar20;
          }
          if (!bVar25) {
            if (in_FPSCR_SZ == '\0') {
              param_3 = (double)CONCAT44(*(undefined4 *)pdVar10,SUB84(param_3,0));
              pdVar10 = (double *)((int)pdVar10 + 4);
              in_dr2 = (double)CONCAT44(uVar15,*(undefined4 *)pdVar9);
              pdVar9 = (double *)((int)pdVar9 + 4);
            }
            else {
              param_3 = *pdVar10;
              pdVar10 = pdVar10 + 1;
              in_xd2 = *pdVar9;
              pdVar9 = pdVar9 + 1;
            }
            fVar22 = SUB84(dVar20,0);
            if (in_FPSCR_PR == '\0') {
              fVar22 = (float)((ulonglong)param_3 >> 0x20) - SUB84(in_dr2,0);
              param_3 = (double)CONCAT44(fVar22,SUB84(param_3,0));
              fVar22 = fVar22 * fVar22 + fVar19;
              dVar20 = (double)CONCAT44((int)((ulonglong)dVar20 >> 0x20),fVar22);
              bVar25 = fVar14 < fVar22;
            }
            else {
              param_3 = param_3 - in_dr2;
              bVar25 = dVar16 < dVar20;
            }
            if (!bVar25) {
              if (in_FPSCR_SZ == '\0') {
                param_3 = (double)CONCAT44(*(undefined4 *)pdVar10,SUB84(param_3,0));
                in_dr2 = (double)CONCAT44((int)((ulonglong)in_dr2 >> 0x20),*(undefined4 *)pdVar9);
              }
              else {
                param_3 = *pdVar10;
                in_xd2 = *pdVar9;
              }
              if (in_FPSCR_PR == '\0') {
                fVar19 = (float)((ulonglong)param_3 >> 0x20) - SUB84(in_dr2,0);
                param_3 = (double)CONCAT44(fVar19,SUB84(param_3,0));
                fVar22 = fVar19 * fVar19 + fVar22;
                dVar20 = (double)CONCAT44((int)((ulonglong)dVar20 >> 0x20),fVar22);
                bVar25 = fVar14 < fVar22;
              }
              else {
                param_3 = param_3 - in_dr2;
                bVar25 = dVar16 < dVar20;
              }
              if (!bVar25) {
                if (puVar7[7] == 0) {
                  puVar7[7] = piVar11;
                }
                puVar7[9] = puVar7[9] + 1;
                *piVar11 = iVar8;
                piVar11 = piVar11 + 1;
                iVar5 = *piVar1;
                *piVar1 = iVar5 + 1;
                if (iVar5 + 1 == 0x60) {
                  if (-1 < *pcVar3) {
                    (*(code *)puVar2)(uVar23,PTR_s_WARNING__NEAR_poly_is_FULL_8c028e40);
                  }
                  goto LAB_8c028e18;
                }
              }
            }
          }
          iVar8 = *(int *)(iVar8 + 4);
          in_xd4 = dVar21;
          in_xd6 = dVar24;
        } while( true );
      }
      if (in_FPSCR_SZ == '\0') {
        in_dr2 = (double)CONCAT44((int)((ulonglong)in_dr2 >> 0x20),piVar6[4]);
      }
      else {
        in_xd2 = *(double *)(piVar6 + 4);
      }
      if (in_FPSCR_PR == '\0') {
        bVar25 = fVar19 < SUB84(in_dr2,0);
      }
      else {
        bVar25 = dVar16 < in_dr2;
      }
      if (!bVar25) goto LAB_8c028e02;
      for (; piVar6 != (int *)0x0; piVar6 = (int *)*piVar6) {
        if (in_FPSCR_SZ == '\0') {
          in_dr2 = (double)CONCAT44((int)((ulonglong)in_dr2 >> 0x20),piVar6[4]);
        }
        else {
          in_xd2 = *(double *)(piVar6 + 4);
        }
        if (in_FPSCR_PR == '\0') {
          bVar25 = SUB84(in_dr2,0) < fVar22;
        }
        else {
          bVar25 = in_dr2 < param_3;
        }
        if (!bVar25) break;
        if (in_FPSCR_SZ == '\0') {
          in_dr2 = (double)CONCAT44(SUB84(dVar13,0),piVar6[5]);
        }
        else {
          in_xd2 = *(double *)(piVar6 + 5);
          in_dr2 = in_xd14;
        }
        if (in_FPSCR_PR == '\0') {
          in_dr2 = (double)CONCAT44((float)((ulonglong)in_dr2 >> 0x20) + SUB84(in_dr2,0),
                                    SUB84(in_dr2,0));
        }
        else {
          in_dr2 = in_dr2 + in_dr2;
        }
        if (in_FPSCR_SZ == '\0') {
          in_dr2 = (double)CONCAT44((int)((ulonglong)in_dr2 >> 0x20),
                                    *(undefined4 *)((int)param_4 + 4));
          pdVar9 = param_4 + 1;
          dVar16 = (double)CONCAT44((int)((ulonglong)dVar16 >> 0x20),piVar6[2]);
          pdVar10 = (double *)(piVar6 + 3);
        }
        else {
          in_xd2 = *(double *)((int)param_4 + 4);
          pdVar9 = (double *)((int)param_4 + 0xc);
          in_xd4 = *(double *)(piVar6 + 2);
          pdVar10 = (double *)(piVar6 + 4);
        }
        if (in_FPSCR_PR == '\0') {
          fVar14 = SUB84(dVar16,0) - SUB84(in_dr2,0);
        }
        else {
          fVar14 = SUB84(dVar16 - in_dr2,0);
        }
        uVar15 = (undefined4)((ulonglong)in_dr2 >> 0x20);
        dVar16 = in_dr2;
        if (in_FPSCR_SZ == '\0') {
          dVar16 = (double)CONCAT44(uVar15,fVar14);
        }
        fVar14 = (float)((ulonglong)dVar20 >> 0x20);
        if (in_FPSCR_PR == '\0') {
          dVar16 = (double)CONCAT44((float)((ulonglong)dVar16 >> 0x20) * fVar14,SUB84(dVar16,0));
        }
        else {
          dVar16 = dVar16 * dVar20;
        }
        dVar21 = in_xd4;
        if (in_FPSCR_SZ == '\0') {
          dVar20 = (double)CONCAT44(fVar14,SUB84(dVar16,0));
          dVar21 = in_xd6;
        }
        fVar14 = (float)((ulonglong)dVar16 >> 0x20);
        if (in_FPSCR_PR == '\0') {
          fVar19 = SUB84(dVar20,0) * SUB84(dVar16,0);
          dVar20 = (double)CONCAT44((int)((ulonglong)dVar20 >> 0x20),fVar19);
          bVar25 = fVar14 < fVar19;
        }
        else {
          dVar20 = dVar20 * dVar16;
          fVar19 = SUB84(dVar20,0);
          bVar25 = dVar16 < dVar20;
        }
        if (!bVar25) {
          if (in_FPSCR_SZ == '\0') {
            dVar16 = (double)CONCAT44(fVar14,*(undefined4 *)pdVar10);
            pdVar10 = (double *)((int)pdVar10 + 4);
            in_dr2 = (double)CONCAT44(uVar15,*(undefined4 *)pdVar9);
            pdVar9 = (double *)((int)pdVar9 + 4);
          }
          else {
            in_xd4 = *pdVar10;
            pdVar10 = pdVar10 + 1;
            in_xd2 = *pdVar9;
            pdVar9 = pdVar9 + 1;
          }
          if (in_FPSCR_PR == '\0') {
            dVar16 = (double)CONCAT44((int)((ulonglong)dVar16 >> 0x20),
                                      SUB84(dVar16,0) - SUB84(in_dr2,0));
          }
          else {
            dVar16 = dVar16 - in_dr2;
          }
          fVar14 = SUB84(dVar16,0);
          if (in_FPSCR_SZ != '\0') {
            fVar14 = (float)((ulonglong)in_xd4 >> 0x20);
          }
          fVar18 = SUB84(dVar20,0);
          fVar17 = (float)((ulonglong)dVar16 >> 0x20);
          if (in_FPSCR_PR == '\0') {
            fVar18 = fVar14 * SUB84(dVar16,0) + fVar19;
            dVar20 = (double)CONCAT44((int)((ulonglong)dVar20 >> 0x20),fVar18);
            bVar25 = fVar17 < fVar18;
          }
          else {
            bVar25 = dVar16 < dVar20;
          }
          if (!bVar25) {
            if (in_FPSCR_SZ == '\0') {
              dVar16 = (double)CONCAT44(fVar17,*(undefined4 *)pdVar10);
              in_dr2 = (double)CONCAT44((int)((ulonglong)in_dr2 >> 0x20),*(undefined4 *)pdVar9);
            }
            else {
              in_xd4 = *pdVar10;
              in_xd2 = *pdVar9;
            }
            if (in_FPSCR_PR == '\0') {
              dVar16 = (double)CONCAT44((int)((ulonglong)dVar16 >> 0x20),
                                        SUB84(dVar16,0) - SUB84(in_dr2,0));
            }
            else {
              dVar16 = dVar16 - in_dr2;
            }
            fVar14 = SUB84(dVar16,0);
            if (in_FPSCR_SZ != '\0') {
              fVar14 = (float)((ulonglong)in_xd4 >> 0x20);
            }
            if (in_FPSCR_PR == '\0') {
              fVar18 = fVar14 * SUB84(dVar16,0) + fVar18;
              dVar20 = (double)CONCAT44((int)((ulonglong)dVar20 >> 0x20),fVar18);
              bVar25 = (float)((ulonglong)dVar16 >> 0x20) < fVar18;
            }
            else {
              bVar25 = dVar16 < dVar20;
            }
            if (!bVar25) {
              if (puVar7[7] == 0) {
                puVar7[7] = piVar11;
              }
              puVar7[9] = puVar7[9] + 1;
              *piVar11 = (int)piVar6;
              piVar11 = piVar11 + 1;
              iVar8 = *piVar1;
              *piVar1 = iVar8 + 1;
              if (iVar8 + 1 == 0x60) {
                if (-1 < *pcVar3) {
                  (*(code *)puVar2)(uVar23,PTR_s_WARNING__NEAR_poly_is_FULL_8c028d6c);
                }
                goto LAB_8c028e18;
              }
            }
          }
        }
        in_xd6 = dVar21;
      }
    }
LAB_8c028e0e:
    puVar7 = (undefined4 *)*puVar7;
  } while( true );
}


/* Address: 8c028e60 */

undefined4 FUN_8c028e60(uint param_1)

{
  undefined *puVar1;
  undefined4 *puVar2;
  int iVar3;
  int iVar4;
  char in_FPSCR_SZ;
  
  puVar1 = PTR_DAT_8c028f10;
  *DAT_8c028f08 = 0;
  iVar3 = DAT_8c028f18;
  puVar2 = DAT_8c028f14;
  iVar4 = (int)DAT_8c028f00;
  *DAT_8c028f0c = 0;
  *(undefined4 *)puVar1 = 0;
  *puVar2 = 0;
  do {
    *(undefined4 *)(iVar3 + 8) = 0;
    *(undefined4 *)(iVar3 + 0x48) = 0;
    iVar4 = iVar4 + -3;
    *(undefined4 *)(iVar3 + 0x88) = 0;
    iVar3 = iVar3 + 0xc0;
  } while (0 < iVar4);
  iVar4 = (int)DAT_8c028f02;
  iVar3 = DAT_8c028f1c;
  do {
    if (in_FPSCR_SZ == '\0') {
      *(undefined4 *)(iVar3 + 0x14) = 0;
    }
    else {
      *(ulonglong *)(iVar3 + 0x14) = (ulonglong)param_1;
    }
    iVar4 = iVar4 + -2;
    if (in_FPSCR_SZ == '\0') {
      *(undefined4 *)(iVar3 + 0x54) = 0;
    }
    else {
      *(ulonglong *)(iVar3 + 0x54) = (ulonglong)param_1;
    }
    iVar3 = iVar3 + 0x80;
  } while (0 < iVar4);
  (*(code *)PTR_FUN_8c028f20)(DAT_8c028f1c,(int)DAT_8c028f02);
  (*(code *)PTR_FUN_8c028f24)(DAT_8c028f18,(int)DAT_8c028f00);
  return 1;
}


