/* Ghidra draft only; not buildable/verified source. */

/* WARNING: Removing unreachable block (ram,0x8c65f87a) */
/* WARNING: Removing unreachable block (ram,0x8c65f9ac) */

undefined4
FUN_8c65f810(int param_1,byte param_2,uint *param_3,int param_4,uint *param_5,int param_6)

{
  byte bVar1;
  int *piVar2;
  uint uVar3;
  uint uVar4;
  undefined4 uVar5;
  uint uVar6;
  int iVar7;
  int iVar8;
  int iVar9;
  uint uVar10;
  uint uVar11;
  uint *puVar12;
  int iVar13;
  
  piVar2 = DAT_8c65f850;
  iVar13 = param_1 * 0x2c;
  iVar9 = *DAT_8c65f850 + 0x28 + iVar13;
  if (*(char *)(iVar9 + 6) == '\0') {
    uVar10 = (uint)DAT_8c65f950;
    *(byte *)(iVar9 + 6) = param_2;
    uVar11 = param_4 + param_6;
    bVar1 = PTR_DAT_8c65f960[param_1];
    uVar10 = uVar10 & (int)(char)bVar1 & 0xff;
    iVar9 = *piVar2;
    if (((0 < *(int *)(DAT_8c65f952 + iVar9)) && (0 < *(int *)(DAT_8c65f954 + iVar9))) &&
       ((int)uVar10 >> 6 ==
        ((int)(short)((uint)**(undefined4 **)(DAT_8c65f956 + iVar9) >> 0x10) & 3U))) {
      FUN_8c65f7cc(*(undefined4 *)(DAT_8c65f952 + iVar9),FUN_8c6469c8);
    }
    iVar9 = (int)DAT_8c65f956;
    iVar7 = *piVar2;
    *(int *)(iVar9 + iVar7) = *(int *)(iVar9 + iVar7) + *(int *)(iVar9 + 0xc + iVar7) * 4;
    puVar12 = *(uint **)(iVar9 + *piVar2);
    iVar9 = (int)DAT_8c65f958;
    *puVar12 = ((int)uVar10 >> 6) << 0x10 | uVar11 & 0xff;
    uVar6 = FUN_8c6607be(*(undefined4 *)
                          (*piVar2 + (int)DAT_8c65f95a + *(int *)(iVar9 + *piVar2) * 0x60 +
                          param_1 * 4));
    iVar9 = (int)DAT_8c65f95c;
    puVar12[1] = uVar6;
    uVar4 = DAT_8c65f96c;
    uVar3 = DAT_8c65f968;
    uVar6 = DAT_8c65f964;
    if (*(int *)(*piVar2 + iVar9) == 1) {
      puVar12[2] = (uint)param_2 << 0x18 & DAT_8c65f968 | (uint)bVar1 << 0x10 & DAT_8c65f96c |
                   uVar10 << 8 & DAT_8c65f964 | uVar11 & 0xff;
      puVar12 = puVar12 + 3;
      iVar9 = 0;
      if (0 < param_4) {
        do {
          uVar10 = *param_3;
          param_3 = param_3 + 1;
          iVar9 = iVar9 + 1;
          *puVar12 = uVar10;
          puVar12 = puVar12 + 1;
        } while (iVar9 < param_4);
      }
    }
    else {
      puVar12[2] = uVar10 << 0x10 & DAT_8c65f96c | (uint)bVar1 << 8 & DAT_8c65f964 |
                   uVar11 * 0x1000000 & DAT_8c65f968 | (uint)param_2;
      puVar12 = puVar12 + 3;
      iVar9 = 0;
      if (0 < param_4) {
        do {
          uVar10 = *param_3;
          param_3 = param_3 + 1;
          iVar9 = iVar9 + 1;
          *puVar12 = uVar10 << 0x18 & uVar3 | uVar10 << 8 & uVar4 | (int)uVar10 >> 8 & uVar6 |
                     uVar10 >> 0x18;
          puVar12 = puVar12 + 1;
        } while (iVar9 < param_4);
      }
    }
    if (*(int *)(*piVar2 + (int)DAT_8c65fa7a) == 1) {
      iVar9 = 0;
      if (0 < param_6) {
        do {
          iVar9 = iVar9 + 1;
          *puVar12 = (int)*(char *)((int)param_5 + 1) << 0x10 & uVar4 |
                     (int)*(char *)((int)param_5 + 2) << 8 & uVar6 |
                     (int)(char)*param_5 << 0x18 & uVar3 | (uint)*(byte *)((int)param_5 + 3);
          puVar12 = puVar12 + 1;
          param_5 = param_5 + 1;
        } while (iVar9 < param_6);
      }
    }
    else {
      iVar9 = 0;
      if (0 < param_6) {
        do {
          uVar10 = *param_5;
          param_5 = param_5 + 1;
          iVar9 = iVar9 + 1;
          *puVar12 = uVar10;
          puVar12 = puVar12 + 1;
        } while (iVar9 < param_6);
      }
    }
    iVar9 = (int)DAT_8c65fa7c;
    *(uint *)(iVar9 + *piVar2) = uVar11 + 3;
    *(uint *)(iVar9 + 4 + *piVar2) = *(int *)(iVar9 + 4 + *piVar2) + (uVar11 + 3) * 4;
    iVar9 = iVar9 + 8;
    iVar8 = *piVar2;
    iVar7 = *(int *)(iVar9 + *piVar2);
    *(int *)(iVar9 + *piVar2) = iVar7 + 1;
    *(int *)(iVar8 + iVar13 + 0x40) = iVar7;
    *(uint *)(iVar9 + *piVar2) = *(uint *)(iVar9 + *piVar2) & DAT_8c65fa80;
    if (*(int *)(iVar9 + *piVar2) == 0) {
      *(undefined4 *)((int)DAT_8c65fa7e + *piVar2) = 1;
    }
    uVar5 = *(undefined4 *)(*piVar2 + iVar13 + 0x40);
  }
  else {
    uVar5 = 0xffffffff;
  }
  return uVar5;
}

