/* Ghidra draft only; not buildable/verified source. */

void FUN_8c01b554(void)

{
  double *pdVar1;
  int *piVar2;
  undefined4 *puVar3;
  undefined4 uVar4;
  int iVar5;
  double dVar6;
  char in_FPSCR_PR;
  char in_FPSCR_SZ;
  
  puVar3 = DAT_8c01b5c0;
  piVar2 = DAT_8c01b5bc;
  pdVar1 = DAT_8c01b5b8;
  if (in_FPSCR_SZ == '\0') {
    dVar6 = (double)CONCAT44(*(undefined4 *)(*DAT_8c01b5bc + 0x20),*DAT_8c01b5c0);
  }
  else {
    dVar6 = *(double *)(*DAT_8c01b5bc + 0x20);
  }
  if (in_FPSCR_PR == '\0') {
    dVar6 = (double)CONCAT44((float)((ulonglong)dVar6 >> 0x20) - SUB84(dVar6,0),SUB84(dVar6,0));
  }
  else {
    dVar6 = dVar6 - dVar6;
  }
  if (in_FPSCR_SZ == '\0') {
    *(int *)DAT_8c01b5b8 = (int)((ulonglong)dVar6 >> 0x20);
    dVar6 = (double)(ulonglong)(uint)puVar3[1];
  }
  else {
    *DAT_8c01b5b8 = dVar6;
  }
  if (in_FPSCR_SZ == '\0') {
    dVar6 = (double)CONCAT44(*(undefined4 *)(*piVar2 + 0x24),SUB84(dVar6,0));
  }
  else {
    dVar6 = *(double *)(*piVar2 + 0x24);
  }
  if (in_FPSCR_PR == '\0') {
    dVar6 = (double)CONCAT44((float)((ulonglong)dVar6 >> 0x20) - SUB84(dVar6,0),SUB84(dVar6,0));
  }
  else {
    dVar6 = dVar6 - dVar6;
  }
  if (in_FPSCR_SZ == '\0') {
    *(int *)((int)pdVar1 + 4) = (int)((ulonglong)dVar6 >> 0x20);
    dVar6 = (double)(ulonglong)(uint)puVar3[2];
  }
  else {
    *(double *)((int)pdVar1 + 4) = dVar6;
  }
  if (in_FPSCR_SZ == '\0') {
    dVar6 = (double)CONCAT44(*(undefined4 *)(*piVar2 + 0x28),SUB84(dVar6,0));
  }
  else {
    dVar6 = *(double *)(*piVar2 + 0x28);
  }
  if (in_FPSCR_PR == '\0') {
    dVar6 = (double)CONCAT44((float)((ulonglong)dVar6 >> 0x20) - SUB84(dVar6,0),SUB84(dVar6,0));
  }
  else {
    dVar6 = dVar6 - dVar6;
  }
  if (in_FPSCR_SZ == '\0') {
    *(int *)(pdVar1 + 1) = (int)((ulonglong)dVar6 >> 0x20);
  }
  else {
    pdVar1[1] = dVar6;
  }
  iVar5 = *piVar2;
  uVar4 = *(undefined4 *)(iVar5 + 0x24);
  *puVar3 = *(undefined4 *)(iVar5 + 0x20);
  puVar3[1] = uVar4;
  puVar3[2] = *(undefined4 *)(iVar5 + 0x28);
  return;
}

