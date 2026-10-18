/* Ghidra draft only; not buildable/verified source. */

void FUN_8c036f60(undefined4 param_1)

{
  undefined8 *puVar1;
  undefined8 *puVar2;
  double *pdVar4;
  double *pdVar6;
  undefined8 *puVar8;
  undefined4 in_fr1;
  double dVar10;
  double dVar11;
  char in_FPSCR_PR;
  char in_FPSCR_SZ;
  undefined4 *puVar3;
  float *pfVar5;
  undefined4 *puVar7;
  undefined8 *puVar9;
  float fVar12;
  
  puVar1 = DAT_8c036fb4;
  puVar2 = (undefined8 *)((int)DAT_8c036fb4 + 4);
  puVar3 = (undefined4 *)((int)DAT_8c036fb4 + 4);
  pdVar4 = (double *)(DAT_8c036fb4 + 1);
  pfVar5 = (float *)(DAT_8c036fb4 + 1);
  pdVar6 = (double *)((int)DAT_8c036fb4 + 0xc);
  puVar7 = (undefined4 *)((int)DAT_8c036fb4 + 0xc);
  puVar8 = DAT_8c036fb4 + 2;
  puVar9 = DAT_8c036fb4 + 2;
  if (in_FPSCR_SZ == '\0') {
    *(undefined4 *)DAT_8c036fb4 = *(undefined4 *)DAT_8c036fb8;
    *puVar3 = *(undefined4 *)DAT_8c036fbc;
    fVar12 = *(float *)DAT_8c036fc0;
    dVar11 = (double)CONCAT44(fVar12,param_1);
    *pfVar5 = fVar12;
    *puVar7 = *(undefined4 *)DAT_8c036fc4;
    *(undefined4 *)puVar9 = *(undefined4 *)DAT_8c036fc8;
    dVar10 = (double)CONCAT44(*(undefined4 *)DAT_8c036fcc,in_fr1);
  }
  else {
    *DAT_8c036fb4 = *DAT_8c036fb8;
    *puVar2 = *DAT_8c036fbc;
    dVar11 = *DAT_8c036fc0;
    fVar12 = (float)((ulonglong)dVar11 >> 0x20);
    *pdVar4 = dVar11;
    *pdVar6 = *DAT_8c036fc4;
    *puVar8 = *DAT_8c036fc8;
    dVar10 = *DAT_8c036fcc;
  }
  if (in_FPSCR_PR == '\0') {
    dVar10 = (double)CONCAT44((float)((ulonglong)dVar10 >> 0x20) * fVar12,SUB84(dVar10,0));
  }
  else {
    dVar10 = dVar10 * dVar11;
  }
  if (in_FPSCR_SZ == '\0') {
    *(int *)((int)puVar1 + 0x14) = (int)((ulonglong)dVar10 >> 0x20);
    dVar10 = (double)CONCAT44(*(undefined4 *)DAT_8c036fc4,SUB84(dVar10,0));
  }
  else {
    *(double *)((int)puVar1 + 0x14) = dVar10;
    dVar10 = *DAT_8c036fc4;
  }
  if (in_FPSCR_PR == '\0') {
    dVar10 = (double)CONCAT44((float)((ulonglong)dVar10 >> 0x20) * fVar12,SUB84(dVar10,0));
  }
  else {
    dVar10 = dVar10 * dVar11;
  }
  if (in_FPSCR_SZ == '\0') {
    *(int *)(puVar1 + 3) = (int)((ulonglong)dVar10 >> 0x20);
  }
  else {
    puVar1[3] = dVar10;
  }
  *(undefined **)(puVar1 + 0xb) = PTR_DAT_8c036fd0;
  *(undefined **)(puVar1 + 7) = PTR_DAT_8c036fd4;
  return;
}

