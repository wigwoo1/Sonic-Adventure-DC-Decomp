/* Ghidra draft only; not buildable/verified source. */

undefined8 FUN_8c650bc8(int param_1,uint param_2,uint param_3,uint param_4,undefined4 *param_5)

{
  uint uVar1;
  uint uVar2;
  uint uVar3;
  uint uVar4;
  byte bVar5;
  byte bVar8;
  ushort uVar9;
  int iVar10;
  int iVar11;
  int *piVar12;
  int iVar13;
  int iVar14;
  int iVar15;
  int iVar16;
  uint uVar17;
  int iVar18;
  int *piVar19;
  byte in_MD;
  byte in_RB;
  byte in_BL;
  byte in_FD;
  byte in_M;
  byte in_Q;
  byte in_IMASK;
  byte in_S;
  byte in_T;
  byte bVar6;
  byte bVar7;
  
  piVar12 = DAT_8c650d0c;
  iVar10 = DAT_8c650d08;
  uVar1 = (uint)in_T | (uint)in_S << 1;
  uVar2 = uVar1 | (uint)in_IMASK << 4;
  uVar3 = uVar2 | (uint)in_Q << 8;
  uVar4 = uVar3 | (uint)in_M << 9;
  bVar5 = (byte)(((uint)in_BL << 0x1c) >> 0x18);
  bVar8 = bVar5 | (byte)(((uint)in_RB << 0x1d) >> 0x18);
  uVar1 = (uVar1 & 2) >> 1;
  uVar2 = (uVar2 & 0xf0) >> 4;
  bVar7 = (byte)(uVar3 >> 8) & 1;
  uVar3 = (uVar4 & 0x200) >> 9;
  uVar4 = ((uVar4 | (uint)in_FD << 0xf) & 0x8000) >> 0xf;
  bVar5 = (bVar5 & 0x10) >> 4;
  bVar6 = (bVar8 & 0x20) >> 5;
  bVar8 = ((bVar8 | (byte)(((uint)in_MD << 0x1e) >> 0x18)) & 0x40) >> 6;
  if (param_3 == 0) {
    iVar11 = (int)DAT_8c650c24;
    piVar12 = (int *)((uVar1 << 1 | 1 | uVar2 << 4 | (uint)bVar7 << 8 | uVar3 << 9 | uVar4 << 0xf |
                       (uint)bVar5 << 0x1c | (uint)bVar6 << 0x1d | (uint)bVar8 << 0x1e) &
                     (int)DAT_8c650c22);
  }
  else {
    uVar17 = (param_4 & 8) >> 3;
    iVar11 = (int)DAT_8c650d00;
    iVar13 = uVar17 * 4;
    iVar15 = uVar17 * 0x100;
    iVar14 = *(int *)(DAT_8c650d04 + iVar13);
    iVar18 = iVar14 * 0x20;
    if ((*(uint *)(iVar15 + DAT_8c650d08 + iVar18 + 0x18) & 3) == 0) {
      if ((param_2 & 0x1f) != 0) {
        param_2 = param_2 + 0x1f & 0xffffffe0;
      }
      if ((param_3 & 0x1f) != 0) {
        param_3 = param_3 + 0x1f & 0xffffffe0;
      }
      piVar19 = (int *)(iVar15 + DAT_8c650d08 + iVar18);
      *piVar19 = *DAT_8c650d0c;
      iVar16 = DAT_8c650d04;
      piVar19[1] = param_2;
      piVar19[3] = param_1;
      piVar19[5] = param_3;
      piVar19[6] = param_4 | 1;
      *param_5 = piVar19;
      iVar11 = 0;
      *(uint *)(iVar13 + iVar16) = *(uint *)(iVar13 + iVar16) + 1 & 7;
      *piVar12 = *piVar12 + 1;
    }
    if (iVar11 == DAT_8c650d00) {
      piVar12 = (int *)(((uint)(iVar11 == DAT_8c650d00) | uVar1 << 1 | uVar2 << 4 | (uint)bVar7 << 8
                         | uVar3 << 9 | uVar4 << 0xf | (uint)bVar5 << 0x1c | (uint)bVar6 << 0x1d |
                        (uint)bVar8 << 0x1e) & (int)DAT_8c650d02);
    }
    else {
      piVar12 = DAT_8c650e0c;
      if (((*(uint *)(*DAT_8c650e08 * 0x100 + iVar10 + *DAT_8c650e0c * 0x20 + 0x18) & 3) == 0) &&
         ((*(uint *)(iVar15 + iVar10 + iVar18 + 0x18) & 3) == 1)) {
        iVar16 = iVar15 + iVar10 + iVar18;
        *(uint *)(iVar16 + 0x18) = *(uint *)(iVar16 + 0x18) & 0xfffffffc | 2;
        *DAT_8c650e10 = *(undefined4 *)(iVar16 + 4);
        *DAT_8c650e18 = (*(int *)(iVar16 + 0xc) + DAT_8c650e14) * 2;
        *DAT_8c650e1c = param_3 >> 5;
        uVar9 = DAT_8c650e00;
        if ((*(uint *)(iVar16 + 0x18) & 4) == 0) {
          uVar9 = DAT_8c650e02;
        }
        if ((*(uint *)(iVar15 + iVar10 + iVar18 + 0x18) & 0x10) != 0) {
          uVar9 = uVar9 | DAT_8c650e04;
        }
        *DAT_8c650e20 = (int)(short)uVar9;
        iVar18 = iVar15 + iVar10 + iVar18;
        *DAT_8c650e24 = *(undefined4 *)(iVar18 + 0xc);
        *DAT_8c650e28 = *(undefined4 *)(iVar18 + 0x14);
        *DAT_8c650e2c = 1;
        iVar10 = DAT_8c650e30;
        piVar12 = DAT_8c650e0c;
        *DAT_8c650e0c = iVar14;
        *DAT_8c650e08 = uVar17;
        *(uint *)(iVar13 + iVar10) = *(uint *)(iVar13 + iVar10) + 1 & 7;
      }
    }
  }
  return CONCAT44(piVar12,iVar11);
}

