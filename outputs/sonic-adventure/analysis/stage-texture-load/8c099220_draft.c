/* Ghidra draft only; not buildable/verified source. */

/* WARNING: Removing unreachable block (ram,0x8c099240) */

void FUN_8c099220(void)

{
  undefined4 *puVar1;
  undefined *puVar2;
  int *piVar3;
  uint uVar4;
  int iVar5;
  int iVar6;
  undefined4 *puVar7;
  undefined *puVar8;
  undefined *puVar9;
  undefined4 *puVar10;
  int *piVar11;
  int *piVar12;
  int *piVar13;
  int *piVar14;
  
  uVar4 = (*(code *)PTR_FUN_8c0992ec)(&DAT_8c7493c8);
  iVar5 = (int)(uVar4 & 0xffff & DAT_8c0992f0) >> 8;
  piVar13 = *(int **)(PTR_DAT_8c0992f4 + iVar5 * 4);
  if ((iVar5 == 0x1d) && (iVar5 = (*(code *)PTR_FUN_8c0992f8)(), iVar5 != 0)) {
    piVar13 = DAT_8c0992fc;
  }
  piVar3 = DAT_8c09930c;
  puVar2 = PTR_FUN_8c099308;
  puVar9 = PTR_s_OBJ_AL_RACE_E_8c099304;
  puVar1 = DAT_8c099300;
  if (piVar13 == (int *)0x0) {
    puVar10 = (undefined4 *)(DAT_8c0992e4 + 0x14);
    for (puVar7 = (undefined4 *)(DAT_8c0992e4 + 4); puVar7 < puVar10; puVar7 = puVar7 + 2) {
      *puVar7 = 0;
      puVar7[1] = 0;
    }
    *puVar1 = 0;
  }
  else {
    piVar11 = (int *)(DAT_8c0992e4 + 0x14);
    puVar8 = PTR_s_CHAO_HYOUJI_8c0994e0;
    for (piVar12 = (int *)(DAT_8c0992e4 + 4); PTR_s_CHAO_HYOUJI_8c0994e0 = puVar8, piVar12 < piVar11
        ; piVar12 = piVar12 + 4) {
      *piVar12 = piVar13[1];
      if (piVar13[1] != 0) {
        if (*piVar13 == 0) {
          FUN_8c098ee0(piVar13[1]);
        }
        else {
          iVar5 = (*(code *)puVar2)(*piVar13,PTR_s_OBJ_AL_RACE_8c099310);
          if (iVar5 == 0) {
            if (*piVar3 == 0) {
              iVar5 = piVar13[1];
              puVar8 = (undefined *)*piVar13;
            }
            else {
              iVar5 = piVar13[1];
              puVar8 = puVar9;
            }
            FUN_8c099750(puVar8,iVar5);
          }
          else {
            FUN_8c099750(*piVar13,piVar13[1]);
          }
        }
      }
      piVar14 = piVar13 + 2;
      piVar12[1] = piVar13[3];
      if (piVar13[3] != 0) {
        if (*piVar14 == 0) {
          FUN_8c098ee0(piVar13[3]);
        }
        else {
          iVar5 = (*(code *)puVar2)(*piVar14,PTR_s_OBJ_AL_RACE_8c099400);
          if (iVar5 == 0) {
            if (*piVar3 == 0) {
              iVar5 = piVar13[3];
              puVar8 = (undefined *)*piVar14;
            }
            else {
              iVar5 = piVar13[3];
              puVar8 = puVar9;
            }
            FUN_8c099750(puVar8,iVar5);
          }
          else {
            FUN_8c099750(*piVar14,piVar13[3]);
          }
        }
      }
      piVar14 = piVar13 + 4;
      piVar12[2] = piVar13[5];
      if (piVar13[5] != 0) {
        if (*piVar14 == 0) {
          FUN_8c098ee0(piVar13[5]);
        }
        else {
          iVar5 = (*(code *)puVar2)(*piVar14,PTR_s_OBJ_AL_RACE_8c099400);
          if (iVar5 == 0) {
            if (*piVar3 == 0) {
              iVar5 = piVar13[5];
              puVar8 = (undefined *)*piVar14;
            }
            else {
              iVar5 = piVar13[5];
              puVar8 = puVar9;
            }
            FUN_8c099750(puVar8,iVar5);
          }
          else {
            FUN_8c099750(*piVar14,piVar13[5]);
          }
        }
      }
      piVar14 = piVar13 + 6;
      piVar12[3] = piVar13[7];
      if (piVar13[7] != 0) {
        if (*piVar14 == 0) {
          FUN_8c098ee0(piVar13[7]);
        }
        else {
          iVar5 = (*(code *)puVar2)(*piVar14,PTR_s_OBJ_AL_RACE_8c099400);
          if (iVar5 == 0) {
            if (*piVar3 == 0) {
              iVar5 = piVar13[7];
              puVar8 = (undefined *)*piVar14;
            }
            else {
              iVar5 = piVar13[7];
              puVar8 = puVar9;
            }
            FUN_8c099750(puVar8,iVar5);
          }
          else {
            FUN_8c099750(*piVar14,piVar13[7]);
          }
        }
      }
      piVar13 = piVar13 + 8;
      puVar8 = PTR_s_CHAO_HYOUJI_8c0994e0;
    }
    iVar5 = 0;
    iVar6 = piVar13[1];
    while (iVar6 != 0) {
      puVar1[iVar5] = piVar13[1];
      if (*piVar13 == 0) {
        FUN_8c098ee0(piVar13[1]);
      }
      else {
        iVar6 = (*(code *)puVar2)(*piVar13,puVar8);
        if (iVar6 == 0) {
          iVar6 = *piVar3;
          if (iVar6 == 0) {
            iVar6 = piVar13[1];
            puVar9 = puVar8;
          }
          else if (iVar6 == 1) {
            iVar6 = piVar13[1];
            puVar9 = PTR_s_CHAO_HYOUJI_E_8c0994e4;
          }
          else if (iVar6 == 2) {
            iVar6 = piVar13[1];
            puVar9 = PTR_s_CHAO_HYOUJI_F_8c0994e8;
          }
          else if (iVar6 == 3) {
            iVar6 = piVar13[1];
            puVar9 = PTR_s_CHAO_HYOUJI_S_8c0994ec;
          }
          else {
            if (iVar6 != 4) goto LAB_8c0994aa;
            iVar6 = piVar13[1];
            puVar9 = PTR_s_CHAO_HYOUJI_G_8c0994f0;
          }
          FUN_8c099750(puVar9,iVar6);
        }
        else {
          FUN_8c099750(*piVar13,piVar13[1]);
        }
      }
LAB_8c0994aa:
      iVar5 = iVar5 + 1;
      iVar6 = piVar13[3];
      piVar13 = piVar13 + 2;
    }
    puVar1[iVar5] = 0;
  }
  return;
}

