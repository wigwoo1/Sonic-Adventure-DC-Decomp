/* Ghidra draft only; not buildable/verified source. */

void callback_8c04481a(undefined4 param_1,int param_2)

{
  char cVar1;
  ushort uVar2;
  int iVar3;
  undefined *puVar4;
  char *pcVar5;
  undefined8 in_dr2;
  undefined4 uVar7;
  double dVar6;
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
  
  puVar4 = PTR_callback_8c0448b4_8c04488c;
  uVar7 = (undefined4)((ulonglong)in_dr2 >> 0x20);
  pcVar5 = *(char **)(param_2 + 0x20);
  cVar1 = *pcVar5;
  if (cVar1 == '\0') {
    *pcVar5 = '\x01';
    *(undefined **)(param_2 + 0x14) = puVar4;
  }
  else {
    if (cVar1 == '\x01') {
      uStack_8 = in_PR;
      uStack_14 = in_xf2;
      iVar3 = (*(code *)PTR_FUN_8c0449e4)();
      if ((iVar3 == 0) &&
         (uVar2 = *(ushort *)(pcVar5 + 6), *(ushort *)(pcVar5 + 6) = uVar2 + 1, 9 < uVar2)) {
        *pcVar5 = '\x02';
        return;
      }
      (*(code *)PTR_FUN_8c0449e8)(0);
      (*(code *)PTR_FUN_8c0449ec)(0,pcVar5 + 0x20);
      if (*(int *)(*(int *)PTR_DAT_8c0449f0 + 0x18) != 0) {
        (*(code *)PTR_FUN_8c0449f4)(0,*(int *)(*(int *)PTR_DAT_8c0449f0 + 0x18));
      }
      if (*(int *)(pcVar5 + 0x1c) != 0) {
        (*(code *)PTR_FUN_8c0449f8)(0,*(int *)(pcVar5 + 0x1c));
      }
      puVar4 = PTR_DAT_8c044a00;
      if ((pcVar5[3] & 1U) != 0) {
        puVar4 = PTR_DAT_8c0449fc;
      }
      (*(code *)PTR_FUN_8c044a04)(puVar4);
      (*(code *)PTR_FUN_8c044a08)(1);
      if (in_FPSCR_PR == '\0') {
        uVar7 = 0x40000000;
      }
      else {
        uVar7 = SUB84((double)CONCAT44(uVar7,0x3f800000) + (double)CONCAT44(uVar7,0x3f800000),0);
      }
      if (in_FPSCR_SZ == '\0') {
        uStack_10 = 0;
        uStack_14 = uVar7;
      }
      else {
        uStack_8 = 0;
        uStack_10 = param_1;
      }
      uStack_c = 0;
      (*(code *)PTR_FUN_8c0449e8)(PTR_DAT_8c044a0c);
      if (*(int *)(pcVar5 + 0x14) != 0) {
        (*(code *)PTR_FUN_8c044a10)(0,*(int *)(pcVar5 + 0x14));
      }
      if (*(int *)(pcVar5 + 0x18) != 0) {
        (*(code *)PTR_FUN_8c0449f4)(0,*(int *)(pcVar5 + 0x18));
      }
      (*(code *)PTR_FUN_8c044a14)(0,&uStack_14,&uStack_20);
      (*(code *)PTR_FUN_8c044a08)(1);
      iVar3 = (*(code *)PTR_FUN_8c0449e4)();
      if (iVar3 == 0) {
        if (in_FPSCR_SZ == '\0') {
          dVar6 = (double)CONCAT44(*(undefined4 *)(pcVar5 + 0x20),uStack_20);
        }
        else {
          dVar6 = *(double *)(pcVar5 + 0x20);
        }
        if (in_FPSCR_PR == '\0') {
          dVar6 = (double)CONCAT44((float)((ulonglong)dVar6 >> 0x20) + SUB84(dVar6,0),SUB84(dVar6,0)
                                  );
        }
        else {
          dVar6 = dVar6 + dVar6;
        }
        if (in_FPSCR_SZ == '\0') {
          *(int *)(pcVar5 + 0x20) = (int)((ulonglong)dVar6 >> 0x20);
          dVar6 = (double)CONCAT44(*(undefined4 *)(pcVar5 + 0x24),uStack_1c);
        }
        else {
          *(double *)(pcVar5 + 0x20) = dVar6;
          dVar6 = *(double *)(pcVar5 + 0x24);
        }
        if (in_FPSCR_PR == '\0') {
          dVar6 = (double)CONCAT44((float)((ulonglong)dVar6 >> 0x20) + SUB84(dVar6,0),SUB84(dVar6,0)
                                  );
        }
        else {
          dVar6 = dVar6 + dVar6;
        }
        if (in_FPSCR_SZ == '\0') {
          *(int *)(pcVar5 + 0x24) = (int)((ulonglong)dVar6 >> 0x20);
          dVar6 = (double)CONCAT44(*(undefined4 *)(pcVar5 + 0x28),uStack_18);
        }
        else {
          *(double *)(pcVar5 + 0x24) = dVar6;
          dVar6 = *(double *)(pcVar5 + 0x28);
        }
        if (in_FPSCR_PR == '\0') {
          dVar6 = (double)CONCAT44((float)((ulonglong)dVar6 >> 0x20) + SUB84(dVar6,0),SUB84(dVar6,0)
                                  );
        }
        else {
          dVar6 = dVar6 + dVar6;
        }
        if (in_FPSCR_SZ == '\0') {
          *(int *)(pcVar5 + 0x28) = (int)((ulonglong)dVar6 >> 0x20);
        }
        else {
          *(double *)(pcVar5 + 0x28) = dVar6;
        }
        *(int *)(pcVar5 + 0x1c) = *(int *)(pcVar5 + 0x1c) + 0x1000;
      }
      return;
    }
    if (cVar1 == '\x02') {
      (*(code *)PTR_FUN_8c0449e0)();
    }
  }
  return;
}

