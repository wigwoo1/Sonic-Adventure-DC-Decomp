/* Ghidra draft only; not buildable/verified source. */

void FUN_8c0446a2(undefined1 *param_1)

{
  int iVar1;
  int iVar2;
  double dVar3;
  undefined8 in_xd2;
  undefined4 in_PR;
  char in_FPSCR_PR;
  char in_FPSCR_SZ;
  undefined4 uStack_24;
  undefined4 uStack_20;
  undefined4 uStack_1c;
  undefined *puStack_18;
  undefined4 uStack_14;
  undefined *puStack_10;
  undefined4 local_c;
  
  *param_1 = 1;
  iVar2 = *(int *)(DAT_8c044844 + (uint)(byte)param_1[8] * 4);
  if (iVar2 != 0) {
    if (in_FPSCR_SZ == '\0') {
      puStack_10 = (undefined *)0x0;
      puStack_18 = puStack_10;
      local_c = in_PR;
    }
    else {
      puStack_10 = (undefined *)in_xd2;
      puStack_18 = puStack_10;
      local_c = (undefined4)((ulonglong)in_xd2 >> 0x20);
      puStack_10 = PTR_DAT_8c04484c;
    }
    uStack_14 = DAT_8c044848;
    (*(code *)PTR_FUN_8c044850)(PTR_DAT_8c04484c);
    if (*(int *)(iVar2 + 0x1c) != 0) {
      (*(code *)PTR_FUN_8c044854)(0,*(int *)(iVar2 + 0x1c));
    }
    if (*(int *)(iVar2 + 0x14) != 0) {
      (*(code *)PTR_FUN_8c044858)(0,*(int *)(iVar2 + 0x14));
    }
    iVar1 = -*(int *)(iVar2 + 0x18);
    if (iVar1 != 0) {
      (*(code *)PTR_FUN_8c04485c)(0,iVar1,iVar1);
    }
    (*(code *)PTR_FUN_8c044860)(0,&puStack_18,&uStack_24);
    (*(code *)PTR_FUN_8c044864)(1);
    if (in_FPSCR_SZ == '\0') {
      dVar3 = (double)CONCAT44(*(undefined4 *)(iVar2 + 0x20),uStack_24);
    }
    else {
      dVar3 = *(double *)(iVar2 + 0x20);
    }
    if (in_FPSCR_PR == '\0') {
      dVar3 = (double)CONCAT44((float)((ulonglong)dVar3 >> 0x20) + SUB84(dVar3,0),SUB84(dVar3,0));
    }
    else {
      dVar3 = dVar3 + dVar3;
    }
    if (in_FPSCR_SZ == '\0') {
      *(int *)(param_1 + 0x20) = (int)((ulonglong)dVar3 >> 0x20);
      dVar3 = (double)CONCAT44(*(undefined4 *)(iVar2 + 0x24),uStack_20);
    }
    else {
      *(double *)(param_1 + 0x20) = dVar3;
      dVar3 = *(double *)(iVar2 + 0x24);
    }
    if (in_FPSCR_PR == '\0') {
      dVar3 = (double)CONCAT44((float)((ulonglong)dVar3 >> 0x20) + SUB84(dVar3,0),SUB84(dVar3,0));
    }
    else {
      dVar3 = dVar3 + dVar3;
    }
    if (in_FPSCR_SZ == '\0') {
      *(int *)(param_1 + 0x24) = (int)((ulonglong)dVar3 >> 0x20);
      dVar3 = (double)CONCAT44(*(undefined4 *)(iVar2 + 0x28),uStack_1c);
    }
    else {
      *(double *)(param_1 + 0x24) = dVar3;
      dVar3 = *(double *)(iVar2 + 0x28);
    }
    if (in_FPSCR_PR == '\0') {
      dVar3 = (double)CONCAT44((float)((ulonglong)dVar3 >> 0x20) + SUB84(dVar3,0),SUB84(dVar3,0));
    }
    else {
      dVar3 = dVar3 + dVar3;
    }
    if (in_FPSCR_SZ == '\0') {
      *(int *)(param_1 + 0x28) = (int)((ulonglong)dVar3 >> 0x20);
    }
    else {
      *(double *)(param_1 + 0x28) = dVar3;
    }
  }
  return;
}

