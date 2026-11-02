/* Ghidra draft only; not buildable/verified source. */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_8c63ac80(undefined4 param_1,uint param_2)

{
  undefined4 uVar1;
  undefined8 *puVar2;
  int iVar3;
  int iVar4;
  int iVar5;
  int iVar6;
  int iVar7;
  int iVar8;
  int *piVar9;
  undefined4 in_fr3;
  double dVar10;
  float fVar11;
  undefined4 uVar12;
  undefined8 in_xd2;
  undefined8 in_xd4;
  char in_FPSCR_PR;
  char in_FPSCR_SZ;
  
  iVar6 = DAT_8c63ad04;
  puVar2 = DAT_8c63ad00;
  uVar12 = DAT_8c63ad08._4_4_;
  uVar1 = (undefined4)DAT_8c63ad08;
  if (in_FPSCR_SZ != '\0') {
    in_xd2 = CONCAT44(DAT_8c63ad08._4_4_,(undefined4)DAT_8c63ad08);
    in_xd4 = CONCAT44(_DAT_8c63ad10,DAT_8c63ad08._4_4_);
    uVar12 = param_1;
    uVar1 = in_fr3;
  }
  iVar4 = (int)DAT_8c63acf4;
  if (in_FPSCR_SZ == '\0') {
    *(undefined4 *)(DAT_8c63ad00 + 6) = uVar1;
    *(undefined4 *)((int)puVar2 + 0x2c) = uVar12;
    *(undefined4 *)(puVar2 + 5) = uVar12;
    *(undefined4 *)((int)puVar2 + 0x24) = uVar12;
    *(undefined4 *)(puVar2 + 4) = 0x3f800000;
    *(undefined4 *)((int)puVar2 + 0x1c) = uVar12;
    *(undefined4 *)(puVar2 + 3) = uVar12;
    *(undefined4 *)((int)puVar2 + 0x14) = uVar12;
    *(undefined4 *)(puVar2 + 2) = 0x3f800000;
    *(undefined4 *)((int)puVar2 + 0xc) = 0;
    *(undefined4 *)(puVar2 + 1) = 0;
    *(undefined4 *)((int)puVar2 + 4) = 0;
    *(undefined4 *)puVar2 = 0x3f800000;
  }
  else {
    DAT_8c63ad00[6] = in_xd2;
    puVar2[5] = in_xd4;
    puVar2[4] = in_xd4;
    puVar2[3] = in_xd4;
    puVar2[2] = CONCAT44(0x3f800000,uVar12);
    puVar2[1] = in_xd4;
    *puVar2 = in_xd4;
    puVar2[-1] = in_xd4;
    puVar2[-2] = CONCAT44(0x3f800000,uVar12);
    puVar2[-3] = (ulonglong)param_2;
    puVar2[-4] = (ulonglong)param_2;
    puVar2[-5] = (ulonglong)param_2;
    puVar2[-6] = CONCAT44(0x3f800000,uVar12);
  }
  iVar3 = _DAT_8c63ad10;
  iVar4 = *(int *)(iVar4 + iVar6);
  if (iVar4 != 0) {
    piVar9 = (int *)(iVar6 + iVar4 * 4);
    iVar7 = (int)DAT_8c63acf6;
    iVar6 = (int)DAT_8c63acf8;
    do {
      piVar9 = piVar9 + -1;
      iVar8 = *(int *)(*piVar9 * 4 + iVar3);
      iVar5 = (int)DAT_8c63acfc;
      if ((*(int *)(DAT_8c63acfa + iVar8) == iVar7) || (*(int *)(DAT_8c63acfa + iVar8) == iVar6)) {
        if (in_FPSCR_SZ == '\0') {
          dVar10 = (double)CONCAT44(*(undefined4 *)(iVar5 + -4 + iVar8),
                                    *(undefined4 *)(iVar5 + iVar8));
        }
        else {
          dVar10 = *(double *)(iVar5 + -4 + iVar8);
        }
        if (in_FPSCR_PR == '\0') {
          fVar11 = (float)((ulonglong)dVar10 >> 0x20) - SUB84(dVar10,0);
          dVar10 = (double)CONCAT44(1.0 / SQRT(fVar11 * fVar11),SUB84(dVar10,0));
        }
        else {
          dVar10 = (dVar10 - dVar10) * (dVar10 - dVar10);
        }
        if (in_FPSCR_SZ == '\0') {
          *(int *)(iVar5 + 0xc + iVar8) = (int)((ulonglong)dVar10 >> 0x20);
        }
        else {
          *(double *)(iVar5 + 0xc + iVar8) = dVar10;
        }
      }
      iVar4 = iVar4 + -1;
    } while (iVar4 != 0);
  }
  return;
}

