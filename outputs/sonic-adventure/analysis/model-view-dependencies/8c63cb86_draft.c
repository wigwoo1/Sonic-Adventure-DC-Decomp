/* Ghidra draft only; not buildable/verified source. */

void FUN_8c63cb86(undefined8 param_1)

{
  int iVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  uint uVar5;
  int iVar6;
  int iVar7;
  undefined1 *puVar8;
  double dVar9;
  float fVar10;
  double dVar11;
  undefined4 uVar12;
  undefined4 uVar13;
  undefined8 in_xd2;
  undefined8 uVar14;
  undefined8 in_xd14;
  undefined4 in_PR;
  char in_FPSCR_PR;
  char in_FPSCR_SZ;
  undefined1 local_20 [4];
  undefined1 local_1c [4];
  
  iVar3 = DAT_8c63ccb0;
  iVar2 = DAT_8c63ccac;
  iVar1 = DAT_8c63cca8;
  if (in_FPSCR_SZ == '\0') {
    puVar8 = local_1c;
  }
  else {
    puVar8 = local_20;
  }
  *(undefined4 *)(puVar8 + -4) = in_PR;
  iVar6 = *(int *)(DAT_8c63cc8c + DAT_8c63cca8);
  uVar13 = 0x3f800000;
  while (iVar6 != 0) {
    iVar6 = iVar6 + -1;
    iVar7 = *(int *)(*(int *)(iVar6 * 4 + iVar1) * 4 + iVar2);
    if (in_FPSCR_SZ == '\0') {
      dVar11 = (double)CONCAT44(*(undefined4 *)(iVar3 + 4),*(undefined4 *)(DAT_8c63cc8e + iVar7));
    }
    else {
      in_xd2 = *(undefined8 *)(DAT_8c63cc8e + iVar7);
      dVar11 = *(double *)(iVar3 + 4);
    }
    iVar4 = (int)DAT_8c63cc90;
    if (in_FPSCR_PR == '\0') {
      dVar11 = (double)CONCAT44((float)((ulonglong)dVar11 >> 0x20) * SUB84(dVar11,0),SUB84(dVar11,0)
                               );
    }
    else {
      dVar11 = dVar11 * dVar11;
    }
    if (in_FPSCR_SZ == '\0') {
      *(int *)(iVar4 + iVar7) = (int)((ulonglong)dVar11 >> 0x20);
      dVar11 = (double)CONCAT44(*(undefined4 *)(iVar3 + 8),*(undefined4 *)(iVar4 + -0x2c + iVar7));
    }
    else {
      *(double *)(iVar4 + iVar7) = dVar11;
      in_xd2 = *(undefined8 *)(iVar4 + -0x2c + iVar7);
      dVar11 = *(double *)(iVar3 + 8);
    }
    iVar4 = (int)DAT_8c63cc92;
    if (in_FPSCR_PR == '\0') {
      dVar11 = (double)CONCAT44((float)((ulonglong)dVar11 >> 0x20) * SUB84(dVar11,0),SUB84(dVar11,0)
                               );
    }
    else {
      dVar11 = dVar11 * dVar11;
    }
    if (in_FPSCR_SZ == '\0') {
      *(int *)(iVar4 + iVar7) = (int)((ulonglong)dVar11 >> 0x20);
      dVar11 = (double)CONCAT44(*(undefined4 *)(iVar3 + 0xc),*(undefined4 *)(iVar4 + -0x2c + iVar7))
      ;
    }
    else {
      *(double *)(iVar4 + iVar7) = dVar11;
      in_xd2 = *(undefined8 *)(iVar4 + -0x2c + iVar7);
      dVar11 = *(double *)(iVar3 + 0xc);
    }
    iVar4 = (int)DAT_8c63cc94;
    if (in_FPSCR_PR == '\0') {
      dVar11 = (double)CONCAT44((float)((ulonglong)dVar11 >> 0x20) * SUB84(dVar11,0),SUB84(dVar11,0)
                               );
    }
    else {
      dVar11 = dVar11 * dVar11;
    }
    if (in_FPSCR_SZ == '\0') {
      *(int *)(iVar4 + iVar7) = (int)((ulonglong)dVar11 >> 0x20);
      dVar11 = (double)CONCAT44(*(undefined4 *)(iVar3 + 0x14),*(undefined4 *)(iVar4 + -0x28 + iVar7)
                               );
    }
    else {
      *(double *)(iVar4 + iVar7) = dVar11;
      in_xd2 = *(undefined8 *)(iVar4 + -0x28 + iVar7);
      dVar11 = *(double *)(iVar3 + 0x14);
    }
    iVar4 = (int)DAT_8c63cc96;
    if (in_FPSCR_PR == '\0') {
      dVar11 = (double)CONCAT44((float)((ulonglong)dVar11 >> 0x20) * SUB84(dVar11,0),SUB84(dVar11,0)
                               );
    }
    else {
      dVar11 = dVar11 * dVar11;
    }
    if (in_FPSCR_SZ == '\0') {
      *(int *)(iVar4 + iVar7) = (int)((ulonglong)dVar11 >> 0x20);
      dVar11 = (double)CONCAT44(*(undefined4 *)(iVar3 + 0x18),*(undefined4 *)(iVar4 + -0x2c + iVar7)
                               );
    }
    else {
      *(double *)(iVar4 + iVar7) = dVar11;
      in_xd2 = *(undefined8 *)(iVar4 + -0x2c + iVar7);
      dVar11 = *(double *)(iVar3 + 0x18);
    }
    iVar4 = (int)DAT_8c63cc98;
    if (in_FPSCR_PR == '\0') {
      dVar11 = (double)CONCAT44((float)((ulonglong)dVar11 >> 0x20) * SUB84(dVar11,0),SUB84(dVar11,0)
                               );
    }
    else {
      dVar11 = dVar11 * dVar11;
    }
    if (in_FPSCR_SZ == '\0') {
      *(int *)(iVar4 + iVar7) = (int)((ulonglong)dVar11 >> 0x20);
      dVar11 = (double)CONCAT44(*(undefined4 *)(iVar3 + 0x1c),*(undefined4 *)(iVar4 + -0x2c + iVar7)
                               );
    }
    else {
      *(double *)(iVar4 + iVar7) = dVar11;
      in_xd2 = *(undefined8 *)(iVar4 + -0x2c + iVar7);
      dVar11 = *(double *)(iVar3 + 0x1c);
    }
    iVar4 = (int)DAT_8c63cc9a;
    if (in_FPSCR_PR == '\0') {
      dVar11 = (double)CONCAT44((float)((ulonglong)dVar11 >> 0x20) * SUB84(dVar11,0),SUB84(dVar11,0)
                               );
    }
    else {
      dVar11 = dVar11 * dVar11;
    }
    if (in_FPSCR_SZ == '\0') {
      *(int *)(iVar4 + iVar7) = (int)((ulonglong)dVar11 >> 0x20);
      dVar11 = (double)CONCAT44(*(undefined4 *)(iVar3 + 0x24),*(undefined4 *)(iVar4 + -0x28 + iVar7)
                               );
    }
    else {
      *(double *)(iVar4 + iVar7) = dVar11;
      in_xd2 = *(undefined8 *)(iVar4 + -0x28 + iVar7);
      dVar11 = *(double *)(iVar3 + 0x24);
    }
    iVar4 = (int)DAT_8c63cc9c;
    if (in_FPSCR_PR == '\0') {
      dVar11 = (double)CONCAT44((float)((ulonglong)dVar11 >> 0x20) * SUB84(dVar11,0),SUB84(dVar11,0)
                               );
    }
    else {
      dVar11 = dVar11 * dVar11;
    }
    if (in_FPSCR_SZ == '\0') {
      *(int *)(iVar4 + iVar7) = (int)((ulonglong)dVar11 >> 0x20);
      dVar11 = (double)CONCAT44(*(undefined4 *)(iVar3 + 0x28),*(undefined4 *)(iVar4 + -0x2c + iVar7)
                               );
    }
    else {
      *(double *)(iVar4 + iVar7) = dVar11;
      in_xd2 = *(undefined8 *)(iVar4 + -0x2c + iVar7);
      dVar11 = *(double *)(iVar3 + 0x28);
    }
    if (in_FPSCR_PR == '\0') {
      dVar11 = (double)CONCAT44((float)((ulonglong)dVar11 >> 0x20) * SUB84(dVar11,0),SUB84(dVar11,0)
                               );
    }
    else {
      dVar11 = dVar11 * dVar11;
    }
    iVar4 = (int)DAT_8c63cc9e;
    if (in_FPSCR_SZ == '\0') {
      *(int *)(iVar4 + iVar7) = (int)((ulonglong)dVar11 >> 0x20);
      dVar11 = (double)CONCAT44(*(undefined4 *)(iVar3 + 0x2c),*(undefined4 *)(iVar4 + -0x2c + iVar7)
                               );
      uVar14 = in_xd2;
    }
    else {
      *(double *)(iVar4 + iVar7) = dVar11;
      uVar14 = *(undefined8 *)(iVar4 + -0x2c + iVar7);
      dVar11 = *(double *)(iVar3 + 0x2c);
    }
    iVar4 = (int)DAT_8c63cca0;
    if (in_FPSCR_PR == '\0') {
      dVar11 = (double)CONCAT44((float)((ulonglong)dVar11 >> 0x20) * SUB84(dVar11,0),SUB84(dVar11,0)
                               );
    }
    else {
      dVar11 = dVar11 * dVar11;
    }
    uVar12 = (undefined4)((ulonglong)dVar11 >> 0x20);
    if (in_FPSCR_SZ == '\0') {
      *(undefined4 *)(iVar4 + iVar7) = uVar12;
      *(undefined4 *)(iVar4 + -0xc + iVar7) = *(undefined4 *)(iVar4 + -0x3c + iVar7);
      dVar11 = (double)CONCAT44(uVar12,*(undefined4 *)(iVar3 + 0x30));
    }
    else {
      *(double *)(iVar4 + iVar7) = dVar11;
      *(undefined8 *)(iVar4 + -0xc + iVar7) = *(undefined8 *)(iVar4 + -0x3c + iVar7);
      uVar14 = *(undefined8 *)(iVar3 + 0x30);
    }
    if (in_FPSCR_PR == '\0') {
      iVar4 = (int)SUB84(dVar11,0);
    }
    else {
      iVar4 = (int)dVar11;
    }
    *(int *)(iVar7 + 0x88) = iVar4;
    uVar5 = *(uint *)(DAT_8c63cca2 + iVar7);
    in_xd2 = uVar14;
    if (((uVar5 & 0x38) != 0) && ((uVar5 != (int)DAT_8c63cca4 || (uVar5 != (int)DAT_8c63cc8e)))) {
      if (in_FPSCR_SZ == '\0') {
        param_1 = CONCAT44(*(undefined4 *)(iVar7 + 0x70),(int)param_1);
      }
      else {
        param_1 = *(undefined8 *)(iVar7 + 0x70);
      }
      dVar9 = (double)(*(code *)PTR_FUN_8c63ccb4)(param_1,*(undefined4 *)(DAT_8c63cca4 + iVar7));
      in_xd2 = in_xd14;
      if (in_FPSCR_SZ == '\0') {
        dVar11 = (double)CONCAT44((int)((ulonglong)dVar11 >> 0x20),uVar13);
        in_xd2 = uVar14;
      }
      if (in_FPSCR_PR == '\0') {
        fVar10 = SUB84(dVar11,0) / (float)((ulonglong)dVar9 >> 0x20);
      }
      else {
        fVar10 = SUB84(dVar11 / dVar9,0);
      }
      if (in_FPSCR_SZ == '\0') {
        *(float *)(DAT_8c63cc8c + iVar7) = fVar10;
      }
      else {
        *(undefined8 *)(DAT_8c63cc8c + iVar7) = in_xd2;
      }
    }
  }
  return;
}

