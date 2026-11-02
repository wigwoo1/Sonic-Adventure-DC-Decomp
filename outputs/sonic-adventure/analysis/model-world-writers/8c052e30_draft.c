/* Ghidra draft only; not buildable/verified source. */

void FUN_8c052e30(void)

{
  short *psVar1;
  undefined *puVar2;
  undefined4 *puVar3;
  short *psVar4;
  undefined *puVar5;
  uint uVar6;
  undefined4 uVar7;
  undefined4 *puVar8;
  undefined4 *puVar9;
  short sVar10;
  undefined4 *puVar11;
  
  psVar4 = DAT_8c052f74;
  puVar3 = DAT_8c052f70;
  puVar9 = DAT_8c052f6c;
  puVar2 = PTR_DAT_8c052f68;
  psVar1 = DAT_8c052f64;
  puVar8 = DAT_8c052f60;
  *DAT_8c052f64 = 0;
  *(undefined4 *)puVar2 = 0;
  for (puVar11 = puVar3 + (*psVar4 + -1) * 3; puVar3 <= puVar11; puVar11 = puVar11 + -3) {
    uVar7 = puVar11[1];
    *puVar9 = *puVar11;
    puVar9[1] = uVar7;
    puVar9[2] = puVar11[2];
    puVar9 = puVar9 + 3;
    *(int *)puVar2 = *(int *)puVar2 + 1;
    puVar8[1] = uVar7;
    puVar8[2] = puVar11[2];
    *puVar8 = *puVar11;
    *psVar1 = *psVar1 + 1;
    puVar8 = puVar8 + 3;
    if ((0x7f < *(int *)puVar2) || (0x3ff < *psVar1)) break;
  }
  puVar5 = PTR_DAT_8c052f80;
  puVar11 = DAT_8c052f7c;
  puVar2 = PTR_DAT_8c052f78;
  *(undefined4 *)PTR_DAT_8c052f78 = 0;
  uVar6 = DAT_8c052f88;
  if ((*(int *)puVar5 == 1) && (*DAT_8c052f84 != 0)) {
    puVar9 = *(undefined4 **)((short *)*DAT_8c052f84 + 6);
    for (sVar10 = *(short *)*DAT_8c052f84; 0 < sVar10; sVar10 = sVar10 + -1) {
      if ((puVar9[8] & uVar6) != 0) {
        uVar7 = puVar9[1];
        *puVar11 = *puVar9;
        puVar11[1] = uVar7;
        uVar7 = puVar9[3];
        puVar11[2] = puVar9[2];
        puVar11[3] = uVar7;
        uVar7 = puVar9[5];
        puVar11[4] = puVar9[4];
        puVar11[5] = uVar7;
        uVar7 = puVar9[7];
        puVar11[6] = puVar9[6];
        puVar11[7] = uVar7;
        puVar11[8] = puVar9[8];
        puVar11 = puVar11 + 9;
        *(int *)puVar2 = *(int *)puVar2 + 1;
        *puVar8 = puVar9[8];
        puVar8[2] = 0;
        puVar8[1] = puVar9[6];
        *psVar1 = *psVar1 + 1;
        puVar8 = puVar8 + 3;
        if (0x7f < *(int *)puVar2) {
          return;
        }
        if (0x3ff < *psVar1) {
          return;
        }
      }
      puVar9 = puVar9 + 9;
    }
  }
  return;
}

