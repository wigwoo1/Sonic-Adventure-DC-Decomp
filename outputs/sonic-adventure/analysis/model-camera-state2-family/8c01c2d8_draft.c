/* Ghidra draft only; not buildable/verified source. */

void FUN_8c01c2d8(void)

{
  int *piVar1;
  int *piVar2;
  double *pdVar3;
  int iVar4;
  int iVar5;
  undefined8 *puVar6;
  undefined8 *puVar7;
  double dVar8;
  double dVar9;
  undefined8 in_dr12;
  double dVar10;
  undefined8 in_xd2;
  bool bVar11;
  undefined4 in_PR;
  char in_FPSCR_PR;
  char in_FPSCR_SZ;
  undefined8 local_1c;
  
  piVar2 = DAT_8c01c464;
  iVar4 = DAT_8c01c460;
  piVar1 = DAT_8c01c348;
  if (in_FPSCR_SZ == '\0') {
    puVar7 = (undefined8 *)((int)&local_1c + 4);
    local_1c = CONCAT44((int)((ulonglong)in_dr12 >> 0x20),(undefined4)local_1c);
  }
  else {
    puVar7 = &local_1c;
    local_1c = in_dr12;
  }
  *(undefined4 *)((int)puVar7 + -4) = in_PR;
  if (*DAT_8c01c348 == 0) {
    iVar5 = 0x31;
  }
  else {
    iVar5 = *DAT_8c01c348 + -1;
  }
  puVar6 = (undefined8 *)(iVar5 * 0xc + DAT_8c01c460);
  if (in_FPSCR_SZ == '\0') {
    dVar9 = (double)CONCAT44(*(undefined4 *)(*DAT_8c01c464 + 0x20),*(undefined4 *)puVar6);
  }
  else {
    dVar9 = *(double *)(*DAT_8c01c464 + 0x20);
    in_xd2 = *puVar6;
  }
  if (in_FPSCR_PR == '\0') {
    dVar9 = (double)CONCAT44((float)((ulonglong)dVar9 >> 0x20) - SUB84(dVar9,0),SUB84(dVar9,0));
  }
  else {
    dVar9 = dVar9 - dVar9;
  }
  if (in_FPSCR_SZ == '\0') {
    *(int *)((int)puVar7 + -0x10) = (int)((ulonglong)dVar9 >> 0x20);
    dVar9 = (double)(ulonglong)*(uint *)((int)puVar6 + 4);
  }
  else {
    *(double *)((int)puVar7 + -0x10) = dVar9;
    in_xd2 = *(undefined8 *)((int)puVar6 + 4);
  }
  if (in_FPSCR_SZ == '\0') {
    dVar9 = (double)CONCAT44(*(undefined4 *)(*DAT_8c01c464 + 0x24),SUB84(dVar9,0));
  }
  else {
    dVar9 = *(double *)(*DAT_8c01c464 + 0x24);
  }
  if (in_FPSCR_PR == '\0') {
    dVar9 = (double)CONCAT44((float)((ulonglong)dVar9 >> 0x20) - SUB84(dVar9,0),SUB84(dVar9,0));
  }
  else {
    dVar9 = dVar9 - dVar9;
  }
  dVar10 = dVar9;
  if (in_FPSCR_SZ == '\0') {
    dVar10 = (double)CONCAT44((int)((ulonglong)dVar9 >> 0x20),(int)in_dr12);
  }
  iVar5 = (*(code *)PTR_FUN_8c01c468)(0);
  if (in_FPSCR_SZ == '\0') {
    dVar9 = (double)CONCAT44((int)((ulonglong)dVar9 >> 0x20),*(undefined4 *)(iVar5 + 0x118));
  }
  else {
    in_xd2 = *(undefined8 *)(iVar5 + 0x118);
  }
  if (in_FPSCR_PR == '\0') {
    dVar10 = (double)CONCAT44((float)((ulonglong)dVar10 >> 0x20) + SUB84(dVar9,0),SUB84(dVar10,0));
  }
  else {
    dVar10 = dVar10 + dVar9;
  }
  if (in_FPSCR_SZ == '\0') {
    *(int *)((int)puVar7 + -0xc) = (int)((ulonglong)dVar10 >> 0x20);
    dVar9 = (double)(ulonglong)*(uint *)(puVar6 + 1);
  }
  else {
    *(double *)((int)puVar7 + -0xc) = dVar10;
    in_xd2 = puVar6[1];
  }
  if (in_FPSCR_SZ == '\0') {
    dVar9 = (double)CONCAT44(*(undefined4 *)(*piVar2 + 0x28),SUB84(dVar9,0));
  }
  else {
    dVar9 = *(double *)(*piVar2 + 0x28);
  }
  if (in_FPSCR_PR == '\0') {
    dVar9 = (double)CONCAT44((float)((ulonglong)dVar9 >> 0x20) - SUB84(dVar9,0),SUB84(dVar9,0));
  }
  else {
    dVar9 = dVar9 - dVar9;
  }
  if (in_FPSCR_SZ == '\0') {
    *(int *)((int)puVar7 + -8) = (int)((ulonglong)dVar9 >> 0x20);
  }
  else {
    *(double *)((int)puVar7 + -8) = dVar9;
  }
  dVar8 = (double)(*(code *)PTR_FUN_8c01c46c)((undefined1 *)((int)puVar7 + -0x10));
  piVar2 = DAT_8c01c464;
  if (in_FPSCR_PR == '\0') {
    bVar11 = 1.0 < (float)((ulonglong)dVar8 >> 0x20);
  }
  else {
    bVar11 = (double)CONCAT44((int)((ulonglong)dVar9 >> 0x20),0x3f800000) < dVar8;
  }
  if (bVar11) {
    if (in_FPSCR_SZ == '\0') {
      dVar9 = (double)CONCAT44(*(undefined4 *)(*DAT_8c01c464 + 0x20),0x3f800000);
    }
    else {
      dVar9 = *(double *)(*DAT_8c01c464 + 0x20);
    }
    pdVar3 = (double *)(*piVar1 * 0xc + iVar4);
    if (in_FPSCR_SZ == '\0') {
      *(int *)pdVar3 = (int)((ulonglong)dVar9 >> 0x20);
    }
    else {
      *pdVar3 = dVar9;
    }
    iVar5 = *piVar2;
    if (in_FPSCR_SZ == '\0') {
      dVar10 = (double)CONCAT44(*(undefined4 *)(iVar5 + 0x24),SUB84(dVar10,0));
    }
    else {
      dVar10 = *(double *)(iVar5 + 0x24);
    }
    pdVar3 = (double *)(*piVar1 * 0xc + iVar4 + 4);
    iVar5 = (*(code *)PTR_FUN_8c01c468)(0);
    if (in_FPSCR_SZ == '\0') {
      dVar9 = (double)CONCAT44((int)((ulonglong)dVar9 >> 0x20),*(undefined4 *)(iVar5 + 0x118));
    }
    else {
      in_xd2 = *(undefined8 *)(iVar5 + 0x118);
    }
    if (in_FPSCR_PR == '\0') {
      dVar10 = (double)CONCAT44((float)((ulonglong)dVar10 >> 0x20) + SUB84(dVar9,0),SUB84(dVar10,0))
      ;
    }
    else {
      dVar10 = dVar10 + dVar9;
    }
    if (in_FPSCR_SZ == '\0') {
      *(int *)pdVar3 = (int)((ulonglong)dVar10 >> 0x20);
    }
    else {
      *pdVar3 = dVar10;
    }
    iVar5 = *piVar2;
    if (in_FPSCR_SZ == '\0') {
      dVar9 = (double)(ulonglong)*(uint *)(iVar5 + 0x28);
    }
    else {
      in_xd2 = *(undefined8 *)(iVar5 + 0x28);
    }
    iVar4 = *piVar1 * 0xc + iVar4;
    if (in_FPSCR_SZ == '\0') {
      *(int *)(iVar4 + 8) = SUB84(dVar9,0);
    }
    else {
      *(undefined8 *)(iVar4 + 8) = in_xd2;
    }
    iVar4 = *piVar1;
    *piVar1 = iVar4 + 1;
    if (0x31 < iVar4 + 1) {
      *piVar1 = 0;
    }
  }
  return;
}

