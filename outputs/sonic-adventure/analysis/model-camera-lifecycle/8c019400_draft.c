/* Ghidra draft only; not buildable/verified source. */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_8c019400(undefined4 param_1)

{
  undefined *puVar1;
  int *piVar2;
  undefined *puVar3;
  undefined4 *puVar4;
  int iVar5;
  undefined4 in_fr3;
  double dVar6;
  double dVar7;
  float fVar8;
  char in_FPSCR_PR;
  char in_FPSCR_SZ;
  undefined4 local_1c;
  undefined4 local_18;
  undefined4 local_14;
  
  piVar2 = DAT_8c019624;
  puVar1 = PTR_DAT_8c019620;
  if (in_FPSCR_SZ == '\0') {
    *(undefined4 *)(*(int *)PTR_DAT_8c019620 + 0x18) = *(undefined4 *)(*DAT_8c019624 + 0x20);
    in_fr3 = DAT_8c019628;
  }
  else {
    *(undefined8 *)(*(int *)PTR_DAT_8c019620 + 0x18) = *(undefined8 *)(*DAT_8c019624 + 0x20);
  }
  if (in_FPSCR_SZ == '\0') {
    dVar6 = (double)CONCAT44(*(undefined4 *)(*piVar2 + 0x24),in_fr3);
  }
  else {
    dVar6 = *(double *)(*piVar2 + 0x24);
  }
  if (in_FPSCR_PR == '\0') {
    dVar6 = (double)CONCAT44((float)((ulonglong)dVar6 >> 0x20) + SUB84(dVar6,0),SUB84(dVar6,0));
  }
  else {
    dVar6 = dVar6 + dVar6;
  }
  if (in_FPSCR_SZ == '\0') {
    *(int *)(*(int *)puVar1 + 0x1c) = (int)((ulonglong)dVar6 >> 0x20);
  }
  else {
    *(double *)(*(int *)puVar1 + 0x1c) = dVar6;
  }
  if (in_FPSCR_SZ == '\0') {
    *(undefined4 *)(*(int *)puVar1 + 0x20) = *(undefined4 *)(*piVar2 + 0x28);
  }
  else {
    *(undefined8 *)(*(int *)puVar1 + 0x20) = *(undefined8 *)(*piVar2 + 0x28);
  }
  *(undefined4 *)(*(int *)puVar1 + 0x24) = 0;
  *(undefined4 *)(*(int *)puVar1 + 0x28) = 0;
  *(undefined4 *)(*(int *)puVar1 + 0x2c) = 0;
  puVar3 = PTR_DAT_8c019630;
  if (in_FPSCR_SZ == '\0') {
    *(undefined4 *)(*(int *)puVar1 + 8) = _DAT_8c01962c;
  }
  else {
    *(ulonglong *)(*(int *)puVar1 + 8) = CONCAT44(PTR_DAT_8c019630,_DAT_8c01962c);
  }
  local_1c = *(undefined4 *)puVar3;
  local_18 = *(undefined4 *)(puVar3 + 4);
  local_14 = *(undefined4 *)(puVar3 + 8);
  (*(code *)PTR_FUN_8c019638)(PTR_DAT_8c019634);
  iVar5 = *(int *)(*piVar2 + 0x1c);
  if (iVar5 != 0) {
    (*(code *)PTR_FUN_8c01963c)(0,iVar5,*piVar2);
  }
  iVar5 = *(int *)(*piVar2 + 0x14);
  if (iVar5 != 0) {
    (*(code *)PTR_FUN_8c019640)(0,iVar5,*piVar2);
  }
  iVar5 = 0x4000 - *(int *)(*piVar2 + 0x18);
  if (iVar5 != 0) {
    (*(code *)PTR_FUN_8c019644)(0,iVar5,iVar5);
  }
  (*(code *)PTR_FUN_8c019648)(0,&local_1c,&local_1c);
  (*(code *)PTR_FUN_8c01964c)(1);
  iVar5 = *(int *)puVar1;
  if (in_FPSCR_SZ == '\0') {
    dVar6 = (double)CONCAT44(_DAT_8c019650,param_1);
    dVar7 = (double)CONCAT44(*(undefined4 *)(iVar5 + 0x18),local_1c);
  }
  else {
    dVar6 = (double)CONCAT44(PTR_DAT_8c019654,_DAT_8c019650);
    dVar7 = *(double *)(iVar5 + 0x18);
  }
  fVar8 = (float)((ulonglong)dVar6 >> 0x20);
  if (in_FPSCR_PR == '\0') {
    dVar7 = (double)CONCAT44((float)((ulonglong)dVar7 >> 0x20) + SUB84(dVar7,0) + fVar8,
                             SUB84(dVar7,0));
  }
  else {
    dVar7 = dVar7 + dVar7 + dVar6;
  }
  if (in_FPSCR_SZ == '\0') {
    *(int *)(iVar5 + 0xc) = (int)((ulonglong)dVar7 >> 0x20);
  }
  else {
    *(double *)(iVar5 + 0xc) = dVar7;
  }
  iVar5 = *(int *)puVar1;
  if (in_FPSCR_SZ == '\0') {
    dVar7 = (double)CONCAT44(*(undefined4 *)(iVar5 + 0x1c),local_18);
  }
  else {
    dVar7 = *(double *)(iVar5 + 0x1c);
  }
  if (in_FPSCR_PR == '\0') {
    dVar7 = (double)CONCAT44((float)((ulonglong)dVar7 >> 0x20) + SUB84(dVar7,0),SUB84(dVar7,0));
  }
  else {
    dVar7 = dVar7 + dVar7;
  }
  if (in_FPSCR_SZ == '\0') {
    *(int *)(iVar5 + 0x10) = (int)((ulonglong)dVar7 >> 0x20);
  }
  else {
    *(double *)(iVar5 + 0x10) = dVar7;
  }
  iVar5 = *(int *)puVar1;
  if (in_FPSCR_SZ == '\0') {
    dVar7 = (double)CONCAT44(*(undefined4 *)(iVar5 + 0x20),local_14);
  }
  else {
    dVar7 = *(double *)(iVar5 + 0x20);
  }
  if (in_FPSCR_PR == '\0') {
    dVar6 = (double)CONCAT44((float)((ulonglong)dVar7 >> 0x20) + SUB84(dVar7,0) + fVar8,
                             SUB84(dVar7,0));
  }
  else {
    dVar6 = dVar7 + dVar7 + dVar6;
  }
  if (in_FPSCR_SZ == '\0') {
    *(int *)(iVar5 + 0x14) = (int)((ulonglong)dVar6 >> 0x20);
  }
  else {
    *(double *)(iVar5 + 0x14) = dVar6;
  }
  (*(code *)PTR_FUN_8c019658)(*(undefined4 *)PTR_DAT_8c019654);
  puVar4 = DAT_8c019660;
  *DAT_8c01965c = 0;
  *puVar4 = 0;
  return;
}

