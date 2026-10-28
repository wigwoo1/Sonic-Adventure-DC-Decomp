/* Ghidra draft only; not buildable/verified source. */

/* WARNING: Removing unreachable block (ram,0x8c6409bc) */
/* WARNING: Removing unreachable block (ram,0x8c6409ca) */

void FUN_8c640922(undefined4 param_1,char *param_2)

{
  char cVar1;
  uint *puVar2;
  double *pdVar3;
  uint *puVar4;
  int *piVar5;
  uint *puVar6;
  uint *puVar7;
  undefined4 *puVar8;
  undefined4 *puVar9;
  undefined4 *puVar10;
  short sVar11;
  uint uVar12;
  uint uVar13;
  undefined4 *puVar14;
  short sVar15;
  float fVar16;
  float fVar17;
  double dVar18;
  int iVar19;
  int iVar20;
  char in_FPSCR_PR;
  char in_FPSCR_SZ;
  
  piVar5 = DAT_8c640a50;
  puVar4 = DAT_8c640a4c;
  *(undefined4 *)PTR_DAT_8c640a48 = 0xfffffffe;
  puVar2 = DAT_8c640a58;
  uVar13 = DAT_8c640a54;
  uVar12 = *(uint *)(*piVar5 + 8);
  *puVar4 = uVar12;
  puVar7 = DAT_8c640a60;
  *puVar2 = uVar12 & uVar13;
  uVar12 = DAT_8c640a68;
  puVar6 = DAT_8c640a5c;
  puVar2 = DAT_8c640a34;
  uVar13 = *puVar4 >> 0x18 & 0x1c;
  *DAT_8c640a5c = uVar13;
  *puVar7 = uVar13;
  uVar13 = DAT_8c640a3c;
  *DAT_8c640a64 = *puVar6;
  puVar9 = DAT_8c640a70;
  puVar8 = DAT_8c640a6c;
  puVar14 = (undefined4 *)((*puVar4 & uVar12 | uVar13) + 0xc);
  *puVar2 = (uint)puVar14;
  puVar10 = DAT_8c640a74;
  *puVar14 = *puVar8;
  uVar13 = *puVar2;
  *puVar2 = uVar13 - 4;
  puVar8 = DAT_8c640a78;
  *(undefined4 *)(uVar13 - 4) = *puVar9;
  uVar13 = *puVar2;
  *puVar2 = uVar13 - 4;
  *(undefined4 *)(uVar13 - 4) = *puVar10;
  uVar13 = *puVar2;
  *puVar2 = uVar13 - 4;
  *(undefined4 *)(uVar13 - 4) = *puVar8;
  pdVar3 = DAT_8c640a40;
  *puVar2 = *puVar2 + 0x20;
  if (in_FPSCR_SZ == '\0') {
    dVar18 = (double)CONCAT44(*(float *)pdVar3,(float)(int)(short)((uint)param_1 >> 0x10));
  }
  else {
    dVar18 = *pdVar3;
  }
  if (in_FPSCR_PR == '\0') {
    fVar17 = (float)((ulonglong)dVar18 >> 0x20);
    fVar16 = SUB84(dVar18,0) * fVar17;
    dVar18 = (double)CONCAT44(fVar17,fVar16);
    iVar19 = (int)fVar16;
  }
  else {
    dVar18 = dVar18 * dVar18;
    iVar19 = (int)dVar18;
  }
  fVar16 = (float)((ulonglong)dVar18 >> 0x20);
  dVar18 = (double)CONCAT44(fVar16,(float)(int)(short)param_1);
  if (in_FPSCR_PR == '\0') {
    fVar17 = (float)(int)(short)param_1 * fVar16;
    dVar18 = (double)CONCAT44(fVar16,fVar17);
    iVar20 = (int)fVar17;
  }
  else {
    dVar18 = dVar18 * dVar18;
    iVar20 = (int)dVar18;
  }
  sVar11 = (*(code *)PTR_FUN_8c640a7c)(param_2,FUN_8c640f38);
  sVar15 = 0;
  if (0 < sVar11) {
    do {
      fVar16 = SUB84(dVar18,0);
      cVar1 = *param_2;
      param_2 = param_2 + 1;
      FUN_8c6407b0(iVar19,iVar20,(int)cVar1);
      if (in_FPSCR_SZ == '\0') {
        fVar16 = *(float *)pdVar3;
      }
      sVar15 = sVar15 + 1;
      if (in_FPSCR_PR == '\0') {
        fVar17 = (float)(int)(short)iVar19 + fVar16;
        dVar18 = (double)CONCAT44(fVar17,fVar16);
        iVar19 = (int)fVar17;
      }
      else {
        dVar18 = (double)(int)(short)iVar19;
        dVar18 = dVar18 + dVar18;
        iVar19 = (int)dVar18;
      }
    } while (sVar15 < sVar11);
  }
  *(uint *)(*DAT_8c640a50 + 8) = *puVar2 & DAT_8c640a68 | *DAT_8c640a58;
  return;
}

