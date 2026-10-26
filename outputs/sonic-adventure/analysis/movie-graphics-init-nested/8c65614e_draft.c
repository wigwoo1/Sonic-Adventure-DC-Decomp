/* Ghidra draft only; not buildable/verified source. */

/* WARNING: Removing unreachable block (ram,0x8c6563b0) */
/* WARNING: Removing unreachable block (ram,0x8c6563a4) */
/* WARNING: Removing unreachable block (ram,0x8c6563aa) */
/* WARNING: Removing unreachable block (ram,0x8c6563b6) */
/* WARNING: Restarted to delay deadcode elimination for space: stack */

int FUN_8c65614e(undefined4 *param_1,int param_2,int param_3,int param_4,int param_5)

{
  uint *puVar1;
  undefined *puVar2;
  undefined *puVar3;
  undefined4 uVar4;
  undefined4 uVar5;
  int iVar6;
  uint uVar7;
  undefined4 uVar8;
  undefined4 uVar9;
  int *piVar10;
  undefined4 *puVar11;
  undefined4 *puVar12;
  int *piVar13;
  undefined4 *puVar14;
  uint uVar15;
  int iVar16;
  uint uVar17;
  int local_30;
  int local_2c [4];
  
  puVar1 = DAT_8c6561f0;
  puVar12 = DAT_8c6561ec;
  iVar16 = 0;
  uVar4 = *(undefined4 *)(PTR_DAT_8c6561e8 + 4);
  uVar8 = *(undefined4 *)PTR_DAT_8c6561e8;
  uVar9 = *(undefined4 *)(PTR_DAT_8c6561e8 + 8);
  uVar5 = *(undefined4 *)(PTR_DAT_8c6561e8 + 0xc);
  DAT_8c6561ec[0x15] = 1;
  iVar6 = DAT_8c6561f8;
  puVar2 = PTR_DAT_8c6561f4;
  uVar17 = 2;
  if ((*puVar1 & 0x40) != 0) {
    param_2 = param_2 + 0x20;
  }
  *puVar12 = param_1;
  puVar12[1] = param_2;
  puVar12[2] = param_3;
  puVar12[3] = param_4;
  puVar12[4] = param_5;
  if (*(int *)puVar2 == iVar6) {
    iVar6 = FUN_8c656f50(param_2,param_3,param_4,param_5,0,uVar8,uVar4,uVar9,uVar5);
    puVar11 = DAT_8c656204;
    puVar2 = PTR_DAT_8c656200;
    param_1[4] = *DAT_8c6561fc;
    *(undefined4 *)puVar2 = 0;
    puVar14 = puVar11 + 4;
    for (; puVar11 < puVar14; puVar11 = puVar11 + 1) {
      *puVar11 = 0;
    }
    uVar7 = (*(code *)PTR_FUN_8c656324)(param_2,param_3,param_4,param_5,0,0,iVar6);
    (*(code *)PTR_FUN_8c656328)(uVar7,uVar7);
    iVar16 = (int)DAT_8c656320;
    uVar15 = param_2 + param_3 + param_4 + param_5;
    local_2c[0] = param_3;
    local_2c[1] = param_4;
    local_2c[2] = param_5;
    local_30 = param_2;
    if (param_2 != 0) {
      local_30 = param_2 + -0x10;
    }
    piVar13 = local_2c;
    piVar10 = piVar13;
    for (; piVar13 < local_2c + 3; piVar13 = piVar13 + 1) {
      if (*piVar13 != 0) {
        *piVar10 = *piVar10 + -0x10;
      }
      piVar10 = piVar10 + 1;
    }
    puVar12[1] = local_30;
    puVar12[2] = local_2c[0];
    puVar12[3] = local_2c[1];
    puVar12[4] = local_2c[2];
    if (*DAT_8c65632c == 1) {
      uVar17 = 1;
    }
    iVar16 = (*(code *)PTR_FUN_8c656330)
                       (local_30,local_2c[0],local_2c[1],local_2c[2],0,iVar6 * 8 + uVar15 + iVar16,
                        uVar17);
    if (iVar16 == 0) {
      (*(code *)PTR_FUN_8c6563e4)((int)DAT_8c6563e0,puVar12[6],puVar12[9]);
      iVar6 = DAT_8c6563ec;
      if (uVar15 < DAT_8c6563e8) {
        uVar15 = DAT_8c6563e8;
      }
      puVar12[0x16] = uVar15 * 4 + puVar12[0x19];
      if ((uint)(iVar6 - puVar12[0x18]) < (uint)puVar12[0x16]) {
        puVar12[0x16] = iVar6 - puVar12[0x18];
      }
      iVar16 = (*(code *)PTR_FUN_8c6563f4)(puVar12[0x16],DAT_8c6563f0 + -4);
      if (iVar16 == 0) {
        *DAT_8c6563f8 = puVar12[0x16];
        (*(code *)PTR_FUN_8c656400)
                  (*DAT_8c6563fc >> 7,DAT_8c6563fc[1] >> 7,DAT_8c6563fc[2] >> 7,DAT_8c6563fc[3] >> 7
                   ,~puVar12[6] & uVar7,0);
        iVar6 = DAT_8c65640c;
        puVar11 = DAT_8c656408;
        puVar12 = DAT_8c656404;
        *DAT_8c656408 = *DAT_8c656404;
        puVar11[1] = puVar12[1];
        puVar11[2] = puVar12[2];
        puVar11[3] = puVar12[3];
        for (uVar15 = 0; piVar10 = DAT_8c6564c8, puVar14 = DAT_8c6564c4, uVar15 < 0x10;
            uVar15 = uVar15 + 4) {
          *(undefined4 *)(uVar15 + iVar6) = *(undefined4 *)(uVar15 + (int)puVar11);
        }
        *DAT_8c6564c4 = 0;
        puVar14[1] = 0;
        puVar14[2] = 0;
        puVar14[3] = 0;
        piVar13 = DAT_8c6564cc;
        *piVar10 = (int)puVar12;
        puVar14 = DAT_8c6564d0;
        *piVar13 = (int)puVar11;
        uVar15 = 0;
        if (uVar17 != 0) {
          do {
            uVar15 = uVar15 + 1;
            *puVar14 = puVar12;
            puVar12 = puVar12 + 5;
            puVar14 = puVar14 + 1;
          } while (uVar15 < uVar17);
        }
        FUN_8c657240(uVar17);
        for (uVar17 = 0; puVar2 = PTR_DAT_8c6564d8, puVar12 = DAT_8c6564c4, uVar17 < 0x10;
            uVar17 = uVar17 + 4) {
          piVar10 = (int *)(*piVar13 + uVar17);
          *piVar10 = *piVar10 + *(int *)(DAT_8c6564d4 + uVar17) * 4;
        }
        *param_1 = puVar11;
        param_1[1] = puVar2;
        puVar11 = DAT_8c6564d0;
        param_1[2] = puVar12;
        param_1[3] = puVar11;
      }
    }
  }
  else {
    uVar17 = *(uint *)puVar2;
    uVar15 = (uint)DAT_8c6564c0;
    *(uint *)puVar2 = uVar17 | uVar15;
    puVar3 = PTR_FUN_8c6564dc;
    if ((uVar17 | uVar15) == (int)DAT_8c6564c2) {
      *(int *)puVar2 = iVar6;
      iVar16 = (*(code *)puVar3)(puVar12[0x15]);
    }
  }
  return iVar16;
}

