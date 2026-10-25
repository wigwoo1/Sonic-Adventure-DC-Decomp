/* Ghidra draft only; not buildable/verified source. */

/* WARNING: Restarted to delay deadcode elimination for space: stack */

int FUN_8c600038(int param_1,uint param_2,undefined4 param_3,undefined4 param_4,undefined4 param_5)

{
  char cVar1;
  undefined *puVar2;
  undefined *puVar3;
  undefined *puVar4;
  short *psVar5;
  uint uVar6;
  int *piVar7;
  int iVar8;
  int iVar9;
  undefined4 *puVar10;
  undefined4 uVar11;
  char *pcVar12;
  char *pcVar13;
  char *pcVar14;
  int iVar15;
  byte in_IMASK;
  byte bVar16;
  byte extraout_S;
  byte extraout_S_00;
  byte extraout_T;
  byte extraout_T_00;
  undefined4 local_2c;
  uint local_28;
  int local_24;
  uint local_20;
  
  puVar2 = PTR_DAT_8c6000e4;
  if ((*(int *)PTR_DAT_8c6000e4 != 0) && (*(short *)(*(int *)PTR_DAT_8c6000e4 + 0x38) == 1)) {
    return 0;
  }
  if ((param_2 & 0x1f) != 0) {
    return -0xe;
  }
  local_28 = param_2;
  local_24 = param_1;
  puVar10 = (undefined4 *)(*(code *)PTR_FUN_8c6000e8)();
  (*(code *)*puVar10)();
  local_20 = (((uint)extraout_T | (uint)extraout_S << 1 | (uint)in_IMASK << 4) & 0xf0) >> 4;
  bVar16 = (byte)((((uint)extraout_T | (uint)extraout_S << 1 | (uint)in_IMASK << 4) & 0xf0) >> 4);
  *(uint *)puVar2 = local_28;
  *(undefined2 *)(local_28 + 0x38) = 0;
  *(undefined4 *)(*(int *)puVar2 + 0x20) = 0;
  **(int **)puVar2 = local_24;
  iVar15 = *(int *)puVar2;
  uVar11 = (*(code *)PTR_FUN_8c6000ec)(param_4,param_3);
  *(undefined4 *)(iVar15 + 0x10) = uVar11;
  *(undefined4 **)(*(int *)puVar2 + 0xc) = puVar10;
  *(undefined4 *)(*(int *)puVar2 + 0x1c) = 0;
  *(undefined2 *)(*(int *)puVar2 + 0x3a) = 0;
  *(undefined4 *)(*(int *)puVar2 + 0x24) = 0;
  *(undefined4 *)(*(int *)puVar2 + 0x34) = 0;
  *(undefined4 *)(*(int *)puVar2 + 0x3c) = param_5;
  puVar10 = (undefined4 *)(*(int *)puVar2 + (int)DAT_8c6000d8);
  for (iVar15 = 0; iVar15 < **(int **)puVar2 + 1; iVar15 = iVar15 + 1) {
    *(undefined2 *)(puVar10 + 0x13) = 0;
    *puVar10 = *(undefined4 *)puVar2;
    puVar10 = puVar10 + 0x15;
  }
  *(int *)(*(int *)puVar2 + 0x14) = (int)DAT_8c600234 + *(int *)puVar2;
  *(int *)(*(int *)puVar2 + 0x18) = (int)DAT_8c600234 + *(int *)puVar2 + 0x54;
  FUN_8c600464(*(undefined4 *)(*(int *)puVar2 + 0x14),0,0);
  puVar4 = PTR_DAT_8c5ffe38;
  puVar3 = PTR_FUN_8c5ffe34;
  puVar2 = PTR_DAT_8c5ffe30;
  iVar15 = DAT_8c5ffe2c;
  uVar6 = (((uint)extraout_T_00 | (uint)extraout_S_00 << 1 | (uint)bVar16 << 4) & 0xf0) >> 4;
  pcVar12 = (char *)(DAT_8c5ffe2c + 0xe);
  pcVar13 = (char *)(DAT_8c5ffe2c + 0xc);
  pcVar14 = (char *)(DAT_8c5ffe2c + 0xf);
  if (*(int *)PTR_DAT_8c5ffe30 == 0) {
    uVar11 = (*(code *)PTR_FUN_8c5ffe34)
                       ((int)DAT_8c5ffe24,PTR_LAB_8c5ffe3c,0x50,*(undefined4 *)PTR_DAT_8c5ffe38,
                        uVar6);
    *(undefined4 *)puVar2 = uVar11;
  }
  puVar2 = PTR_DAT_8c5ffe40;
  if (*(int *)PTR_DAT_8c5ffe40 == 0) {
    uVar11 = (*(code *)puVar3)((int)DAT_8c5ffe26,PTR_LAB_8c5ffe44,0x10,*(undefined4 *)puVar4,uVar6);
    *(undefined4 *)puVar2 = uVar11;
  }
  puVar2 = PTR_FUN_8c5ffe48;
  piVar7 = (int *)(*(code *)PTR_FUN_8c5ffe48)(*(undefined4 *)(*(int *)puVar4 + 0x14),0,0);
  if ((piVar7 != (int *)0x0) &&
     (iVar8 = (**(code **)(*(int *)(*(int *)puVar4 + 0xc) + 0x1c))(), iVar8 != 0)) {
    piVar7[0xf] = iVar8;
    *(undefined2 *)(piVar7 + 0x14) = 4;
    *(int **)(*piVar7 + 0x1c) = piVar7;
    iVar8 = (*(code *)PTR_FUN_8c5ffe4c)(piVar7);
    iVar9 = (**(code **)(*(int *)(*(int *)puVar4 + 0xc) + 0x20))(&local_28);
    if (iVar9 != 0) {
      if (iVar8 != 0) {
        if (iVar8 != -0x1d) {
          return iVar8;
        }
        if (local_28 == 6) {
          return -0x17;
        }
        return -0x1d;
      }
      if (local_24 != DAT_8c5ffe28) {
        return -5;
      }
      piVar7 = (int *)(*(code *)puVar2)(*(undefined4 *)(*(int *)puVar4 + 0x14),0,0);
      if (piVar7 != (int *)0x0) {
        *(undefined4 *)(*(int *)puVar4 + 0x2c) = 0;
        iVar15 = (**(code **)(*(int *)(*(int *)puVar4 + 0xc) + 0x4c))(iVar15);
        if (iVar15 != 0) {
          piVar7[0xf] = iVar15;
          *(undefined2 *)(piVar7 + 0x14) = 4;
          *(int **)(*piVar7 + 0x1c) = piVar7;
          iVar15 = (*(code *)PTR_FUN_8c5fff94)(piVar7);
          if (iVar15 == 0) {
            *(uint *)(*(int *)puVar4 + 0x2c) =
                 *(uint *)(*(int *)puVar4 + 0x2c) | (*pcVar13 + -0x30) * 0x10000;
            *(uint *)(*(int *)puVar4 + 0x2c) =
                 *(uint *)(*(int *)puVar4 + 0x2c) | (*pcVar12 + -0x30) * 0x100;
            *(uint *)(*(int *)puVar4 + 0x2c) =
                 *(uint *)(*(int *)puVar4 + 0x2c) | (int)*pcVar14 - 0x30U;
          }
        }
        iVar15 = FUN_8c5ffc98(*(undefined4 *)PTR_DAT_8c5fff98,1,*(int *)puVar4 + 0x40);
        if (iVar15 == 0) {
          iVar15 = FUN_8c5ff9e6(*(int *)puVar4 + 0x40);
          if (iVar15 == 0) {
            return -5;
          }
          iVar15 = *(int *)puVar4;
          iVar9 = (int)DAT_8c5fff8a;
          uVar11 = FUN_8c5ff9c0(*(int *)puVar4 + 0x40 + iVar9);
          iVar8 = (int)DAT_8c5fff8c;
          *(undefined4 *)(iVar15 + 4) = uVar11;
          *(int *)(*(int *)puVar4 + 4) = *(int *)(*(int *)puVar4 + 4) + iVar8;
          iVar15 = *(int *)puVar4;
          uVar11 = FUN_8c5ff9c0(*(int *)puVar4 + iVar9 + 0x38);
          pcVar12 = DAT_8c5fffa4;
          *(undefined4 *)(iVar15 + 8) = uVar11;
          psVar5 = DAT_8c5fffa0;
          piVar7 = DAT_8c5fff9c;
          *DAT_8c5fff9c = (int)DAT_8c5fffa0;
          *psVar5 = (short)pcVar12[0xc];
          if (*(short *)*piVar7 == 1) {
            iVar15 = 0;
            *(short *)(*piVar7 + 2) = (short)pcVar12[0xd];
            *(undefined4 *)(*piVar7 + 4) = 0;
            pcVar13 = (char *)(*piVar7 + 8);
            do {
              if (*pcVar12 == ' ') break;
              cVar1 = *pcVar12;
              pcVar12 = pcVar12 + 1;
              iVar15 = iVar15 + 1;
              *pcVar13 = cVar1;
              pcVar13 = pcVar13 + 1;
            } while (iVar15 < 0xc);
            *pcVar13 = '\0';
          }
          iVar15 = FUN_8c5ff9c0(*(int *)puVar4 + 0x40 + (int)DAT_8c5fff8e);
          uVar11 = FUN_8c5ff9c0(*(int *)puVar4 + 0x40 + (int)DAT_8c5fff90);
          *(undefined4 *)(*(int *)(*(int *)puVar4 + 0x10) + 8) = 0;
          iVar15 = (*(code *)PTR_FUN_8c5fffa8)
                             (*(int *)puVar4 + 0x40,iVar15 + iVar8,
                              *(undefined4 *)(*(int *)puVar4 + 0x10),uVar11);
          if (iVar15 == -0xc) {
            return -0xc;
          }
          if (iVar15 == 0) {
            if (**(int **)(*(int *)puVar4 + 0x10) == 0) {
              return -5;
            }
            if (*(short *)*piVar7 != 1) {
LAB_8c600000:
              *(undefined2 *)(*(int *)puVar4 + 0x38) = 1;
              return 0;
            }
            iVar15 = FUN_8c60041a((short *)*piVar7 + 4,&local_2c);
            if (iVar15 == 0) {
              *(undefined4 *)(*piVar7 + 4) = local_2c;
              iVar15 = (*(code *)PTR_FUN_8c6000dc)
                                 (*(int *)puVar4 + 0x40,local_2c,
                                  *(undefined4 *)(*(int *)puVar4 + 0x10),1);
              if (iVar15 == -0xc) {
                return -0xc;
              }
              if (iVar15 == 0) {
                if (**(int **)(*(int *)puVar4 + 0x10) == 0) {
                  return -5;
                }
                FUN_8c5ffcfe(*(undefined4 *)(*(int *)puVar4 + 0x10));
                goto LAB_8c600000;
              }
            }
          }
        }
        return -4;
      }
    }
  }
  return -2;
}

