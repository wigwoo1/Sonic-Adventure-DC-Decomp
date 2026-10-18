/* Ghidra draft only; not buildable/verified source. */

void callback_8c0448b4(undefined4 param_1,int param_2)

{
  ushort uVar1;
  int iVar2;
  undefined1 *puVar3;
  undefined *puVar4;
  undefined8 in_dr2;
  undefined4 uVar6;
  double dVar5;
  undefined4 in_xf2;
  undefined4 in_PR;
  char in_FPSCR_PR;
  char in_FPSCR_SZ;
  undefined4 uStack_20;
  undefined4 uStack_1c;
  undefined4 uStack_18;
  undefined4 uStack_14;
  undefined4 uStack_10;
  undefined4 uStack_c;
  undefined4 uStack_8;
  
  uVar6 = (undefined4)((ulonglong)in_dr2 >> 0x20);
  puVar3 = *(undefined1 **)(param_2 + 0x20);
  uStack_8 = in_PR;
  uStack_14 = in_xf2;
  iVar2 = (*(code *)PTR_FUN_8c0449e4)();
  if ((iVar2 == 0) &&
     (uVar1 = *(ushort *)(puVar3 + 6), *(ushort *)(puVar3 + 6) = uVar1 + 1, 9 < uVar1)) {
    *puVar3 = 2;
    return;
  }
  (*(code *)PTR_FUN_8c0449e8)(0);
  (*(code *)PTR_FUN_8c0449ec)(0,puVar3 + 0x20);
  if (*(int *)(*(int *)PTR_DAT_8c0449f0 + 0x18) != 0) {
    (*(code *)PTR_FUN_8c0449f4)(0,*(int *)(*(int *)PTR_DAT_8c0449f0 + 0x18));
  }
  if (*(int *)(puVar3 + 0x1c) != 0) {
    (*(code *)PTR_FUN_8c0449f8)(0,*(int *)(puVar3 + 0x1c));
  }
  puVar4 = PTR_DAT_8c044a00;
  if ((puVar3[3] & 1) != 0) {
    puVar4 = PTR_DAT_8c0449fc;
  }
  (*(code *)PTR_FUN_8c044a04)(puVar4);
  (*(code *)PTR_FUN_8c044a08)(1);
  if (in_FPSCR_PR == '\0') {
    uVar6 = 0x40000000;
  }
  else {
    uVar6 = SUB84((double)CONCAT44(uVar6,0x3f800000) + (double)CONCAT44(uVar6,0x3f800000),0);
  }
  if (in_FPSCR_SZ == '\0') {
    uStack_10 = 0;
    uStack_14 = uVar6;
  }
  else {
    uStack_8 = 0;
    uStack_10 = param_1;
  }
  uStack_c = 0;
  (*(code *)PTR_FUN_8c0449e8)(PTR_DAT_8c044a0c);
  if (*(int *)(puVar3 + 0x14) != 0) {
    (*(code *)PTR_FUN_8c044a10)(0,*(int *)(puVar3 + 0x14));
  }
  if (*(int *)(puVar3 + 0x18) != 0) {
    (*(code *)PTR_FUN_8c0449f4)(0,*(int *)(puVar3 + 0x18));
  }
  (*(code *)PTR_FUN_8c044a14)(0,&uStack_14,&uStack_20);
  (*(code *)PTR_FUN_8c044a08)(1);
  iVar2 = (*(code *)PTR_FUN_8c0449e4)();
  if (iVar2 == 0) {
    if (in_FPSCR_SZ == '\0') {
      dVar5 = (double)CONCAT44(*(undefined4 *)(puVar3 + 0x20),uStack_20);
    }
    else {
      dVar5 = *(double *)(puVar3 + 0x20);
    }
    if (in_FPSCR_PR == '\0') {
      dVar5 = (double)CONCAT44((float)((ulonglong)dVar5 >> 0x20) + SUB84(dVar5,0),SUB84(dVar5,0));
    }
    else {
      dVar5 = dVar5 + dVar5;
    }
    if (in_FPSCR_SZ == '\0') {
      *(int *)(puVar3 + 0x20) = (int)((ulonglong)dVar5 >> 0x20);
      dVar5 = (double)CONCAT44(*(undefined4 *)(puVar3 + 0x24),uStack_1c);
    }
    else {
      *(double *)(puVar3 + 0x20) = dVar5;
      dVar5 = *(double *)(puVar3 + 0x24);
    }
    if (in_FPSCR_PR == '\0') {
      dVar5 = (double)CONCAT44((float)((ulonglong)dVar5 >> 0x20) + SUB84(dVar5,0),SUB84(dVar5,0));
    }
    else {
      dVar5 = dVar5 + dVar5;
    }
    if (in_FPSCR_SZ == '\0') {
      *(int *)(puVar3 + 0x24) = (int)((ulonglong)dVar5 >> 0x20);
      dVar5 = (double)CONCAT44(*(undefined4 *)(puVar3 + 0x28),uStack_18);
    }
    else {
      *(double *)(puVar3 + 0x24) = dVar5;
      dVar5 = *(double *)(puVar3 + 0x28);
    }
    if (in_FPSCR_PR == '\0') {
      dVar5 = (double)CONCAT44((float)((ulonglong)dVar5 >> 0x20) + SUB84(dVar5,0),SUB84(dVar5,0));
    }
    else {
      dVar5 = dVar5 + dVar5;
    }
    if (in_FPSCR_SZ == '\0') {
      *(int *)(puVar3 + 0x28) = (int)((ulonglong)dVar5 >> 0x20);
    }
    else {
      *(double *)(puVar3 + 0x28) = dVar5;
    }
    *(int *)(puVar3 + 0x1c) = *(int *)(puVar3 + 0x1c) + 0x1000;
  }
  return;
}

