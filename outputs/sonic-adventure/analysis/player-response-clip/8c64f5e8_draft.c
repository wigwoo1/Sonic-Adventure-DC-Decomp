/* Ghidra draft only; not buildable/verified source. */

/* WARNING: Removing unreachable block (ram,0x8c64f716) */
/* WARNING: Removing unreachable block (ram,0x8c64f6c2) */
/* WARNING: Removing unreachable block (ram,0x8c64f696) */
/* WARNING: Removing unreachable block (ram,0x8c64f676) */
/* WARNING: Removing unreachable block (ram,0x8c64f67a) */
/* WARNING: Removing unreachable block (ram,0x8c64f69e) */
/* WARNING: Removing unreachable block (ram,0x8c64f6e6) */
/* WARNING: Removing unreachable block (ram,0x8c64f740) */

undefined4 FUN_8c64f5e8(undefined4 param_1,int *param_2,int *param_3,uint param_4)

{
  short sVar1;
  byte bVar2;
  double *pdVar3;
  double *pdVar4;
  int iVar5;
  int iVar6;
  float in_fr1;
  double dVar7;
  float fVar8;
  undefined4 uVar9;
  double dVar10;
  ulonglong uVar11;
  float fVar12;
  double dVar13;
  uint uVar14;
  undefined8 in_xd0;
  double in_xd2;
  bool bVar15;
  undefined1 uVar16;
  undefined2 uVar17;
  char in_FPSCR_PR;
  char in_FPSCR_SZ;
  
  pdVar3 = DAT_8c64f838;
  iVar5 = *param_2;
  if (in_FPSCR_SZ == '\0') {
    dVar13 = (double)CONCAT44(*(undefined4 *)DAT_8c64f838,param_1);
    dVar10 = (double)CONCAT44(*(undefined4 *)(iVar5 + 8),*(undefined4 *)(iVar5 + 0x14));
  }
  else {
    dVar13 = *DAT_8c64f838;
    in_xd2 = *(double *)(iVar5 + 0x14);
    dVar10 = *(double *)(iVar5 + 8);
  }
  fVar12 = (float)((ulonglong)dVar13 >> 0x20);
  if (in_FPSCR_PR == '\0') {
    fVar8 = (SUB84(dVar10,0) - fVar12) * ((float)((ulonglong)dVar10 >> 0x20) - fVar12);
  }
  else {
    dVar10 = (dVar10 - dVar13) - dVar13;
    fVar8 = SUB84(dVar10 * dVar10,0);
  }
  iVar5 = 0;
  if (in_FPSCR_PR == '\0' && fVar8 < 0.0) {
    pdVar4 = (double *)*param_2;
    if (in_FPSCR_SZ == '\0') {
      dVar13 = (double)CONCAT44(fVar12,*(undefined4 *)((int)pdVar4 + 0x14));
      fVar8 = *(float *)(pdVar4 + 1);
    }
    else {
      in_xd2 = pdVar4[1];
    }
    if (in_FPSCR_PR == '\0') {
      dVar13 = (double)CONCAT44((float)((ulonglong)dVar13 >> 0x20) - SUB84(dVar13,0),SUB84(dVar13,0)
                               );
    }
    else {
      dVar13 = dVar13 - dVar13;
    }
    fVar12 = SUB84(dVar13,0);
    if (in_FPSCR_PR == '\0') {
      dVar10 = (double)(ulonglong)(uint)(fVar8 - fVar12);
    }
    else {
      dVar10 = (double)(ulonglong)(uint)fVar8 - dVar13;
    }
    if (in_FPSCR_SZ == '\0') {
      in_fr1 = *(float *)((int)pdVar4 + 0xc);
      dVar13 = (double)CONCAT44((int)((ulonglong)dVar13 >> 0x20),in_fr1);
      fVar12 = in_fr1;
    }
    else {
      in_xd0 = *(undefined8 *)((int)pdVar4 + 0xc);
    }
    if (in_FPSCR_PR == '\0') {
      dVar13 = (double)CONCAT44((float)((ulonglong)dVar13 >> 0x20) / SUB84(dVar10,0),fVar12);
    }
    else {
      dVar13 = dVar13 / dVar10;
    }
    if (in_FPSCR_SZ == '\0') {
      dVar10 = (double)CONCAT44((int)((ulonglong)dVar10 >> 0x20),*(undefined4 *)pdVar4);
    }
    else {
      in_xd2 = *pdVar4;
    }
    fVar12 = SUB84(dVar13,0);
    if (in_FPSCR_PR == '\0') {
      dVar10 = (double)CONCAT44((int)((ulonglong)dVar10 >> 0x20),SUB84(dVar10,0) - fVar12);
    }
    else {
      dVar10 = dVar10 - dVar13;
    }
    uVar9 = (undefined4)((ulonglong)dVar13 >> 0x20);
    dVar7 = dVar13;
    if (in_FPSCR_SZ == '\0') {
      dVar7 = (double)CONCAT44(uVar9,in_fr1);
    }
    if (in_FPSCR_PR == '\0') {
      fVar8 = (float)((ulonglong)dVar7 >> 0x20);
      dVar7 = (double)CONCAT44(fVar8,fVar8 * SUB84(dVar10,0) + SUB84(dVar7,0));
    }
    if (in_FPSCR_SZ == '\0') {
      *(int *)*param_3 = SUB84(dVar7,0);
    }
    else {
      *(undefined8 *)*param_3 = in_xd0;
    }
    iVar5 = *param_2;
    if (in_FPSCR_SZ == '\0') {
      fVar12 = *(float *)(iVar5 + 0x10);
      dVar13 = (double)CONCAT44(uVar9,fVar12);
      dVar10 = (double)CONCAT44((int)((ulonglong)dVar10 >> 0x20),*(undefined4 *)(iVar5 + 4));
      dVar7 = (double)CONCAT44((int)((ulonglong)dVar7 >> 0x20),fVar12);
    }
    else {
      in_xd0 = *(undefined8 *)(iVar5 + 0x10);
      in_xd2 = *(double *)(iVar5 + 4);
    }
    if (in_FPSCR_PR == '\0') {
      fVar12 = SUB84(dVar10,0) - fVar12;
      dVar10 = (double)CONCAT44((int)((ulonglong)dVar10 >> 0x20),fVar12);
      dVar7 = (double)(ulonglong)(uint)((float)((ulonglong)dVar7 >> 0x20) * fVar12 + SUB84(dVar7,0))
      ;
    }
    else {
      dVar10 = dVar10 - dVar13;
    }
    if (in_FPSCR_SZ == '\0') {
      *(int *)(*param_3 + 4) = SUB84(dVar7,0);
      dVar10 = (double)CONCAT44((int)((ulonglong)dVar10 >> 0x20),*(undefined4 *)pdVar3);
    }
    else {
      *(undefined8 *)(*param_3 + 4) = in_xd0;
      in_xd2 = *pdVar3;
    }
    if (in_FPSCR_SZ == '\0') {
      *(int *)(*param_3 + 8) = SUB84(dVar10,0);
    }
    else {
      *(double *)(*param_3 + 8) = in_xd2;
    }
    uVar14 = (uint)((ulonglong)dVar13 >> 0x20);
    if ((param_4 & DAT_8c64f83c) != 0) {
      dVar7 = dVar13;
      if (in_FPSCR_SZ == '\0') {
        dVar7 = (double)((ulonglong)uVar14 << 0x20);
      }
      sVar1 = ((short *)param_2[2])[2];
      uVar9 = (undefined4)((ulonglong)dVar10 >> 0x20);
      fVar8 = (float)(int)sVar1;
      dVar10 = (double)CONCAT44(uVar9,fVar8);
      fVar12 = (float)((ulonglong)dVar7 >> 0x20);
      if (in_FPSCR_PR == '\0') {
        fVar8 = fVar12 * (float)((int)*(short *)param_2[2] - (int)sVar1) + fVar8;
        dVar10 = (double)CONCAT44(uVar9,fVar8);
        uVar17 = (undefined2)(int)fVar8;
      }
      else {
        uVar17 = (undefined2)(int)dVar10;
      }
      *(undefined2 *)param_3[2] = uVar17;
      sVar1 = *(short *)(param_2[2] + 6);
      fVar8 = (float)(int)sVar1;
      if (in_FPSCR_PR == '\0') {
        uVar17 = (undefined2)
                 (int)(fVar12 * (float)((int)*(short *)(param_2[2] + 2) - (int)sVar1) + fVar8);
      }
      else {
        uVar17 = (undefined2)(int)(double)CONCAT44((int)((ulonglong)dVar10 >> 0x20),fVar8);
      }
      *(undefined2 *)(param_3[2] + 2) = uVar17;
    }
    dVar10 = dVar13;
    if (in_FPSCR_SZ == '\0') {
      dVar10 = (double)((ulonglong)uVar14 << 0x20);
    }
    bVar2 = *(byte *)(param_2[1] + 7);
    iVar5 = (uint)*(byte *)(param_2[1] + 3) - (uint)bVar2;
    fVar12 = (float)((ulonglong)dVar10 >> 0x20);
    if (in_FPSCR_PR == '\0') {
      uVar16 = (undefined1)(int)(fVar12 * (float)iVar5 + (float)bVar2);
    }
    else {
      uVar16 = (undefined1)(int)(double)iVar5;
    }
    *(undefined1 *)(param_3[1] + 3) = uVar16;
    bVar2 = *(byte *)(param_2[1] + 6);
    iVar5 = (uint)*(byte *)(param_2[1] + 2) - (uint)bVar2;
    if (in_FPSCR_PR == '\0') {
      uVar16 = (undefined1)(int)(fVar12 * (float)iVar5 + (float)bVar2);
    }
    else {
      uVar16 = (undefined1)(int)(double)iVar5;
    }
    *(undefined1 *)(param_3[1] + 2) = uVar16;
    bVar2 = *(byte *)(param_2[1] + 5);
    iVar5 = (uint)*(byte *)(param_2[1] + 1) - (uint)bVar2;
    if (in_FPSCR_PR == '\0') {
      uVar16 = (undefined1)(int)(fVar12 * (float)iVar5 + (float)bVar2);
    }
    else {
      uVar16 = (undefined1)(int)(double)iVar5;
    }
    *(undefined1 *)(param_3[1] + 1) = uVar16;
    bVar2 = ((byte *)param_2[1])[4];
    iVar6 = (uint)*(byte *)param_2[1] - (uint)bVar2;
    iVar5 = 1;
    if (in_FPSCR_PR == '\0') {
      uVar16 = (undefined1)(int)(fVar12 * (float)iVar6 + (float)bVar2);
    }
    else {
      uVar16 = (undefined1)(int)(double)iVar6;
    }
    *(undefined1 *)param_3[1] = uVar16;
  }
  if (in_FPSCR_SZ == '\0') {
    uVar11 = CONCAT44(*(undefined4 *)(*param_2 + 8),*(undefined4 *)pdVar3);
  }
  else {
    in_xd2 = *pdVar3;
    uVar11 = *(ulonglong *)(*param_2 + 8);
  }
  if (in_FPSCR_PR != '\0' || (float)uVar11 <= (float)(uVar11 >> 0x20)) {
    iVar6 = iVar5 * 0xc;
    if (in_FPSCR_SZ == '\0') {
      uVar11 = (ulonglong)*(uint *)*param_2;
    }
    else {
      in_xd2 = *(double *)*param_2;
    }
    uVar9 = (undefined4)uVar11;
    if (in_FPSCR_SZ == '\0') {
      *(undefined4 *)(*param_3 + iVar6) = uVar9;
    }
    else {
      *(double *)(*param_3 + iVar6) = in_xd2;
    }
    if (in_FPSCR_SZ == '\0') {
      uVar9 = *(undefined4 *)(*param_2 + 4);
    }
    else {
      in_xd2 = *(double *)(*param_2 + 4);
    }
    if (in_FPSCR_SZ == '\0') {
      *(undefined4 *)(*param_3 + iVar6 + 4) = uVar9;
    }
    else {
      *(double *)(*param_3 + iVar6 + 4) = in_xd2;
    }
    if (in_FPSCR_SZ == '\0') {
      *(undefined4 *)(iVar6 + *param_3 + 8) = *(undefined4 *)(*param_2 + 8);
    }
    else {
      *(undefined8 *)(iVar6 + *param_3 + 8) = *(undefined8 *)(*param_2 + 8);
    }
    iVar5 = iVar5 * 4;
    if (param_3[2] != 0) {
      *(undefined2 *)(param_3[2] + iVar5) = *(undefined2 *)param_2[2];
      *(undefined2 *)(param_3[2] + iVar5 + 2) = *(undefined2 *)(param_2[2] + 2);
    }
    *(undefined4 *)(param_3[1] + iVar5) = *(undefined4 *)param_2[1];
  }
  if (in_FPSCR_SZ == '\0') {
    dVar10 = (double)CONCAT44(*(undefined4 *)(*param_2 + 0x14),*(undefined4 *)pdVar3);
  }
  else {
    dVar10 = *(double *)(*param_2 + 0x14);
  }
  fVar12 = (float)((ulonglong)dVar10 >> 0x20);
  if (in_FPSCR_PR != '\0' || SUB84(dVar10,0) <= fVar12) {
    if (in_FPSCR_SZ == '\0') {
      uVar9 = *(undefined4 *)(*param_2 + 0xc);
      dVar10 = (double)CONCAT44(fVar12,uVar9);
      *(undefined4 *)(*param_3 + 0xc) = uVar9;
    }
    else {
      *(undefined8 *)(*param_3 + 0xc) = *(undefined8 *)(*param_2 + 0xc);
    }
    if (in_FPSCR_SZ == '\0') {
      uVar9 = *(undefined4 *)(*param_2 + 0x10);
      dVar10 = (double)CONCAT44((int)((ulonglong)dVar10 >> 0x20),uVar9);
      *(undefined4 *)(*param_3 + 0x10) = uVar9;
    }
    else {
      *(undefined8 *)(*param_3 + 0x10) = *(undefined8 *)(*param_2 + 0x10);
    }
    if (in_FPSCR_SZ == '\0') {
      uVar9 = *(undefined4 *)(*param_2 + 0x14);
      dVar10 = (double)CONCAT44((int)((ulonglong)dVar10 >> 0x20),uVar9);
      *(undefined4 *)(*param_3 + 0x14) = uVar9;
    }
    else {
      *(undefined8 *)(*param_3 + 0x14) = *(undefined8 *)(*param_2 + 0x14);
    }
    if (param_3[2] != 0) {
      *(undefined2 *)(param_3[2] + 4) = *(undefined2 *)(param_2[2] + 4);
      *(undefined2 *)(param_3[2] + 6) = *(undefined2 *)(param_2[2] + 6);
    }
    *(undefined4 *)(param_3[1] + 4) = *(undefined4 *)(param_2[1] + 4);
  }
  if (in_FPSCR_SZ == '\0') {
    dVar13 = (double)CONCAT44(*(undefined4 *)pdVar3,SUB84(dVar13,0));
    dVar10 = (double)CONCAT44((int)((ulonglong)dVar10 >> 0x20),*(undefined4 *)(*param_2 + 8));
  }
  else {
    dVar13 = *pdVar3;
  }
  fVar12 = (float)((ulonglong)dVar13 >> 0x20);
  if (in_FPSCR_PR == '\0') {
    bVar15 = SUB84(dVar10,0) < fVar12;
  }
  else {
    bVar15 = dVar10 < dVar13;
  }
  if (bVar15) {
    if (in_FPSCR_SZ == '\0') {
      dVar10 = (double)CONCAT44((int)((ulonglong)dVar10 >> 0x20),*(undefined4 *)(*param_2 + 0x14));
    }
    if (in_FPSCR_PR == '\0') {
      bVar15 = SUB84(dVar10,0) < fVar12;
    }
    else {
      bVar15 = dVar10 < dVar13;
    }
    if (bVar15) {
      return 0;
    }
  }
  return 2;
}

