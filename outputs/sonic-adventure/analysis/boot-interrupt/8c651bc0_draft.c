/* Ghidra draft only; not buildable/verified source. */

undefined4 FUN_8c651bc0(void)

{
  bool bVar1;
  uint *puVar2;
  int *piVar3;
  uint *puVar4;
  int *piVar5;
  undefined4 *puVar6;
  int iVar7;
  uint *puVar8;
  undefined *puVar9;
  uint uVar10;
  int iVar11;
  uint uVar12;
  int iVar13;
  uint *puVar14;
  int local_34;
  int local_30;
  int local_2c;
  int local_28;
  undefined1 *puVar15;
  
  puVar15 = &DAT_a05f6900;
  *DAT_8c651c64 = 8;
  *DAT_8c651c68 = *DAT_8c651c68 | 2;
  *DAT_8c651c6c = *DAT_8c651c6c + 1;
  puVar2 = DAT_8c651c70;
  if ((*DAT_8c651c74 & 2) == 0) {
    uVar10 = *(int *)PTR_DAT_8c651c7c + 1U & 1;
    *(uint *)PTR_DAT_8c651c7c = uVar10;
  }
  else {
    uVar10 = (*DAT_8c651c78 & (int)DAT_8c651c60) >> 10;
  }
  puVar4 = DAT_8c651c84;
  piVar3 = DAT_8c651c80;
  *puVar2 = uVar10;
  piVar5 = DAT_8c651c88;
  if (((*puVar4 & 4) == 4) && (*puVar2 == 1)) {
    bVar1 = true;
  }
  else {
    *DAT_8c651c88 = *DAT_8c651c88 + -1;
    if (*piVar5 < 0) {
      *DAT_8c651da4 = *DAT_8c651da0;
      bVar1 = false;
    }
    else if (*piVar3 == 0) {
      bVar1 = true;
    }
    else {
      bVar1 = false;
      *piVar3 = 0;
    }
  }
  puVar4 = DAT_8c651dcc;
  puVar2 = DAT_8c651dac;
  if (!bVar1) {
    uVar10 = 0;
    if (*DAT_8c651da8 == 0) {
      if (*DAT_8c651db0 == 0) {
        local_28 = 0;
        local_30 = 0;
        local_2c = 0;
        puVar14 = DAT_8c651dcc + -0xf;
        for (local_34 = 0; iVar7 = DAT_8c651f60, iVar11 = DAT_8c651dd0, local_34 < 4;
            local_34 = local_34 + 1) {
          iVar13 = local_34 * 4;
          if ((-1 < *(int *)(DAT_8c651dd0 + iVar13)) &&
             ((*(uint *)(DAT_8c651dd0 + iVar13) & 1) == 0)) {
            if (*DAT_8c651dd4 == 0) {
              if (*(int *)(DAT_8c651dd0 + iVar13) == 4) {
                *(undefined4 *)(DAT_8c651dd0 + iVar13) = 5;
                if (*DAT_8c651dc8 != 0) {
                  puVar2[2] = puVar2[2] + 1;
                }
                *DAT_8c651dc8 = 0;
                *puVar14 = 2;
                *puVar14 = 0;
              }
              else {
                *piVar3 = 1;
                uVar10 = 1;
                if ((*(int *)(iVar11 + iVar13) == 2) &&
                   (puVar2[1] = puVar2[1] + 1, *DAT_8c651dd8 < puVar2[1])) {
                  puVar2[3] = puVar2[3] + 1;
                  (*(code *)PTR_FUN_8c651ddc)();
                  *(undefined4 *)(DAT_8c651dd0 + iVar13) = 0xffffffff;
                  *DAT_8c651de0 = 0;
                  puVar2[1] = 0;
                }
              }
            }
            else {
              *piVar3 = 1;
              uVar10 = 1;
              if ((*(int *)(iVar7 + iVar13) == 4) &&
                 (*puVar2 = *puVar2 + 1, puVar8 = DAT_8c651f68, *DAT_8c651f64 < *puVar2)) {
                puVar2[2] = puVar2[2] + 1;
                *puVar4 = *puVar8 & 0xfffffffe;
                *puVar14 = 6;
                *puVar14 = 0;
                *puVar4 = *puVar8;
                (*(code *)PTR_FUN_8c651f78)
                          (*DAT_8c651f74,*DAT_8c651f70,0,1,0,~*DAT_8c651f6c & 1,puVar15);
                (*(code *)PTR_FUN_8c651f78)(*DAT_8c651f74,*DAT_8c651f70,0,1,1,~*DAT_8c651f6c & 1);
                *(undefined4 *)(DAT_8c651f60 + local_2c) = 5;
                *DAT_8c651f7c = 0;
                *DAT_8c651f80 = *DAT_8c651f80 + 1;
                *puVar2 = 0;
              }
              puVar8 = DAT_8c651f84;
              if ((*(int *)(DAT_8c651f60 + local_30) == 2) &&
                 (uVar12 = puVar2[1], puVar2[1] = uVar12 + 1, puVar9 = PTR_FUN_8c651f88,
                 *puVar8 < uVar12 + 1)) {
                puVar2[3] = puVar2[3] + 1;
                (*(code *)puVar9)();
                *(undefined4 *)(DAT_8c651f60 + local_28) = 0xffffffff;
                *DAT_8c651f8c = 0;
                puVar2[1] = 0;
              }
            }
          }
          local_28 = local_28 + 4;
          local_30 = local_30 + 4;
          local_2c = local_2c + 4;
        }
        uVar12 = uVar10 | *DAT_8c651f7c;
        uVar10 = uVar12;
      }
      else {
        uVar12 = DAT_8c651dac[6];
        DAT_8c651dac[6] = uVar12 + 1;
        puVar6 = DAT_8c651db8;
        if (0x10 < uVar12 + 1) {
          *DAT_8c651db8 = *DAT_8c651db4;
          puVar6[1] = *DAT_8c651dbc;
          puVar6[8] = *DAT_8c651dc0;
          puVar6[9] = *DAT_8c651dc4;
          *DAT_8c651dc8 = 0;
          puVar2[6] = 0;
        }
        uVar12 = 1;
      }
    }
    else {
      uVar12 = 1;
    }
    if (uVar12 == 0) {
      FUN_8c651570(uVar10);
      iVar11 = FUN_8c6516a0();
      if (iVar11 == 0) {
        return 1;
      }
      FUN_8c65194a();
      *DAT_8c651f90 = 0;
    }
  }
  return 0;
}

