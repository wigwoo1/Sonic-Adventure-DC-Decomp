/* Ghidra draft only; not buildable/verified source. */

/* WARNING: Removing unreachable block (ram,0x8c60b930) */
/* WARNING: Removing unreachable block (ram,0x8c60b910) */
/* WARNING: Removing unreachable block (ram,0x8c60b950) */

void callback_8c60b81c(undefined4 param_1)

{
  uint uVar1;
  undefined8 *puVar2;
  ushort *puVar3;
  undefined8 *puVar4;
  int *piVar5;
  int iVar6;
  double *pdVar7;
  undefined8 *puVar8;
  undefined8 *puVar9;
  undefined1 *puVar10;
  float fVar11;
  double in_dr2;
  double dVar12;
  undefined4 uVar13;
  undefined8 uVar14;
  float in_fr15;
  undefined4 in_fr14;
  undefined4 in_PR;
  char in_FPSCR_PR;
  char in_FPSCR_SZ;
  undefined1 auStack_20 [4];
  undefined1 auStack_1c [4];
  
  puVar4 = DAT_8c60ba48._4_4_;
  puVar3 = DAT_8c60ba44;
  puVar2 = DAT_8c60ba40;
  if (in_FPSCR_SZ == '\0') {
    puVar10 = auStack_1c;
  }
  else {
    puVar10 = auStack_20;
  }
  *(undefined4 *)(puVar10 + -4) = in_PR;
  puVar8 = (undefined8 *)(puVar10 + -0x10);
  uVar1 = (uint)*DAT_8c60ba3c * 3;
  if (in_FPSCR_SZ == '\0') {
    in_fr15 = (float)DAT_8c60ba48;
  }
  if (*DAT_8c60ba44 == 0) {
    if (in_FPSCR_SZ == '\0') {
      *(undefined4 *)puVar8 = *(undefined4 *)DAT_8c60ba48._4_4_;
      uVar13 = *(undefined4 *)((int)DAT_8c60ba48._4_4_ + 4);
      *(undefined4 *)(puVar10 + -0xc) = uVar13;
      in_dr2 = (double)CONCAT44(uVar13,*(undefined4 *)(DAT_8c60ba48._4_4_ + 1));
      *(undefined4 *)(puVar10 + -8) = *(undefined4 *)(DAT_8c60ba48._4_4_ + 1);
    }
    else {
      *puVar8 = *DAT_8c60ba48._4_4_;
      in_dr2 = *(double *)((int)DAT_8c60ba48._4_4_ + 4);
      *(double *)(puVar10 + -0xc) = in_dr2;
      *(undefined8 *)(puVar10 + -8) = DAT_8c60ba48._4_4_[1];
    }
  }
  else {
    if (*DAT_8c60ba44 < uVar1) {
      pdVar7 = (double *)((int)DAT_8c60ba48._4_4_ + 0xc);
      *DAT_8c60ba44 = *DAT_8c60ba44 - 1;
      *puVar3 = *puVar3 ^ 1;
      if (in_FPSCR_SZ == '\0') {
        dVar12 = (double)CONCAT44(*(undefined4 *)pdVar7,*(undefined4 *)puVar4);
      }
      else {
        dVar12 = *pdVar7;
      }
      if (in_FPSCR_PR == '\0') {
        dVar12 = (double)CONCAT44((float)((ulonglong)dVar12 >> 0x20) - SUB84(dVar12,0),
                                  SUB84(dVar12,0));
      }
      else {
        dVar12 = dVar12 - dVar12;
      }
      if (in_FPSCR_PR == '\0') {
        dVar12 = (double)CONCAT44((float)((ulonglong)dVar12 >> 0x20) * in_fr15,SUB84(dVar12,0));
      }
      else {
        dVar12 = dVar12 * (double)CONCAT44(in_fr14,in_fr15);
      }
      iVar6 = ((uint)*puVar3 * 3 + 6) * 4;
      if (in_FPSCR_SZ == '\0') {
        *(int *)(iVar6 + (int)puVar4) = (int)((ulonglong)dVar12 >> 0x20);
      }
      else {
        *(double *)(iVar6 + (int)puVar4) = dVar12;
      }
      if (in_FPSCR_SZ == '\0') {
        dVar12 = (double)(ulonglong)*(uint *)((int)puVar4 + 4);
      }
      iVar6 = ((uint)*puVar3 * 3 + 7) * 4;
      if (in_FPSCR_SZ == '\0') {
        dVar12 = (double)CONCAT44(*(undefined4 *)(puVar4 + 2),SUB84(dVar12,0));
      }
      else {
        dVar12 = (double)puVar4[2];
      }
      if (in_FPSCR_PR == '\0') {
        dVar12 = (double)CONCAT44(((float)((ulonglong)dVar12 >> 0x20) - SUB84(dVar12,0)) * in_fr15,
                                  SUB84(dVar12,0));
      }
      else {
        dVar12 = (dVar12 - dVar12) * (double)CONCAT44(in_fr14,in_fr15);
      }
      if (in_FPSCR_SZ == '\0') {
        *(int *)(iVar6 + (int)puVar4) = (int)((ulonglong)dVar12 >> 0x20);
        dVar12 = (double)(ulonglong)*(uint *)(puVar4 + 1);
      }
      else {
        *(double *)(iVar6 + (int)puVar4) = dVar12;
      }
      if (in_FPSCR_SZ == '\0') {
        in_dr2 = (double)CONCAT44(*(undefined4 *)((int)puVar4 + 0x14),SUB84(dVar12,0));
      }
      else {
        in_dr2 = *(double *)((int)puVar4 + 0x14);
      }
      if (in_FPSCR_PR == '\0') {
        in_dr2 = (double)CONCAT44((float)((ulonglong)in_dr2 >> 0x20) - SUB84(in_dr2,0),
                                  SUB84(in_dr2,0));
      }
      else {
        in_dr2 = in_dr2 - in_dr2;
      }
      if (in_FPSCR_PR == '\0') {
        in_dr2 = (double)CONCAT44((float)((ulonglong)in_dr2 >> 0x20) * in_fr15,SUB84(in_dr2,0));
      }
      else {
        in_dr2 = in_dr2 * (double)CONCAT44(in_fr14,in_fr15);
      }
      iVar6 = ((uint)*puVar3 * 3 + 8) * 4;
      if (in_FPSCR_SZ == '\0') {
        *(int *)(iVar6 + (int)puVar4) = (int)((ulonglong)in_dr2 >> 0x20);
      }
      else {
        *(double *)(iVar6 + (int)puVar4) = in_dr2;
      }
    }
    if (in_FPSCR_SZ == '\0') {
      uVar14 = CONCAT44(*(undefined4 *)puVar2,param_1);
    }
    else {
      uVar14 = *puVar2;
    }
    (*(code *)*DAT_8c60ba50)(uVar14,puVar4,puVar8);
    param_1 = (undefined4)uVar14;
  }
  piVar5 = DAT_8c60ba58;
  puVar3 = DAT_8c60ba54;
  if (*DAT_8c60ba54 == 0) {
    *(int *)(puVar10 + -0x28) = *DAT_8c60ba58;
    *(int *)(puVar10 + -0x24) = DAT_8c60ba58[1];
    *(int *)(puVar10 + -0x20) = DAT_8c60ba58[2];
  }
  else {
    if (*DAT_8c60ba54 < uVar1) {
      *DAT_8c60ba54 = *DAT_8c60ba54 - 1;
      *puVar3 = *puVar3 ^ 1;
      uVar13 = (undefined4)((ulonglong)in_dr2 >> 0x20);
      if (in_FPSCR_PR == '\0') {
        fVar11 = (float)(piVar5[3] - *piVar5) * in_fr15;
        dVar12 = (double)CONCAT44(uVar13,fVar11);
        iVar6 = (int)fVar11;
      }
      else {
        dVar12 = (double)CONCAT44(uVar13,(float)(piVar5[3] - *piVar5)) *
                 (double)CONCAT44(in_fr14,in_fr15);
        iVar6 = (int)dVar12;
      }
      piVar5[(uint)*puVar3 * 3 + 6] = iVar6;
      uVar13 = (undefined4)((ulonglong)dVar12 >> 0x20);
      if (in_FPSCR_PR == '\0') {
        fVar11 = (float)(piVar5[4] - piVar5[1]) * in_fr15;
        dVar12 = (double)CONCAT44(uVar13,fVar11);
        iVar6 = (int)fVar11;
      }
      else {
        dVar12 = (double)CONCAT44(uVar13,(float)(piVar5[4] - piVar5[1])) *
                 (double)CONCAT44(in_fr14,in_fr15);
        iVar6 = (int)dVar12;
      }
      piVar5[(uint)*puVar3 * 3 + 7] = iVar6;
      if (in_FPSCR_PR == '\0') {
        iVar6 = (int)((float)(piVar5[5] - piVar5[2]) * in_fr15);
      }
      else {
        iVar6 = (int)((double)CONCAT44((int)((ulonglong)dVar12 >> 0x20),
                                       (float)(piVar5[5] - piVar5[2])) *
                     (double)CONCAT44(in_fr14,in_fr15));
      }
      piVar5[(uint)*puVar3 * 3 + 8] = iVar6;
    }
    if (in_FPSCR_SZ == '\0') {
      uVar14 = CONCAT44(*(undefined4 *)puVar2,param_1);
    }
    else {
      uVar14 = *puVar2;
    }
    (*(code *)*DAT_8c60ba5c)(uVar14,piVar5,puVar10 + -0x28);
    param_1 = (undefined4)uVar14;
  }
  puVar4 = DAT_8c60ba64;
  puVar3 = DAT_8c60ba60;
  puVar9 = (undefined8 *)(puVar10 + -0x1c);
  if (*DAT_8c60ba60 == 0) {
    if (in_FPSCR_SZ == '\0') {
      *(undefined4 *)puVar9 = *(undefined4 *)DAT_8c60ba64;
      *(undefined4 *)(puVar10 + -0x18) = *(undefined4 *)((int)DAT_8c60ba64 + 4);
      *(undefined4 *)(puVar10 + -0x14) = *(undefined4 *)(DAT_8c60ba64 + 1);
    }
    else {
      *puVar9 = *DAT_8c60ba64;
      *(undefined8 *)(puVar10 + -0x18) = *(undefined8 *)((int)DAT_8c60ba64 + 4);
      *(undefined8 *)(puVar10 + -0x14) = DAT_8c60ba64[1];
    }
  }
  else {
    if (*DAT_8c60ba60 < uVar1) {
      pdVar7 = (double *)((int)DAT_8c60ba64 + 0xc);
      *DAT_8c60ba60 = *DAT_8c60ba60 - 1;
      *puVar3 = *puVar3 ^ 1;
      if (in_FPSCR_SZ == '\0') {
        dVar12 = (double)CONCAT44(*(undefined4 *)pdVar7,*(undefined4 *)puVar4);
      }
      else {
        dVar12 = *pdVar7;
      }
      if (in_FPSCR_PR == '\0') {
        dVar12 = (double)CONCAT44((float)((ulonglong)dVar12 >> 0x20) - SUB84(dVar12,0),
                                  SUB84(dVar12,0));
      }
      else {
        dVar12 = dVar12 - dVar12;
      }
      if (in_FPSCR_PR == '\0') {
        dVar12 = (double)CONCAT44((float)((ulonglong)dVar12 >> 0x20) * in_fr15,SUB84(dVar12,0));
      }
      else {
        dVar12 = dVar12 * (double)CONCAT44(in_fr14,in_fr15);
      }
      iVar6 = ((uint)*puVar3 * 3 + 6) * 4;
      if (in_FPSCR_SZ == '\0') {
        *(int *)(iVar6 + (int)puVar4) = (int)((ulonglong)dVar12 >> 0x20);
      }
      else {
        *(double *)(iVar6 + (int)puVar4) = dVar12;
      }
      if (in_FPSCR_SZ == '\0') {
        dVar12 = (double)(ulonglong)*(uint *)((int)puVar4 + 4);
      }
      iVar6 = ((uint)*puVar3 * 3 + 7) * 4;
      if (in_FPSCR_SZ == '\0') {
        dVar12 = (double)CONCAT44(*(undefined4 *)(puVar4 + 2),SUB84(dVar12,0));
      }
      else {
        dVar12 = (double)puVar4[2];
      }
      if (in_FPSCR_PR == '\0') {
        dVar12 = (double)CONCAT44(((float)((ulonglong)dVar12 >> 0x20) - SUB84(dVar12,0)) * in_fr15,
                                  SUB84(dVar12,0));
      }
      else {
        dVar12 = (dVar12 - dVar12) * (double)CONCAT44(in_fr14,in_fr15);
      }
      if (in_FPSCR_SZ == '\0') {
        *(int *)(iVar6 + (int)puVar4) = (int)((ulonglong)dVar12 >> 0x20);
        dVar12 = (double)(ulonglong)*(uint *)(puVar4 + 1);
      }
      else {
        *(double *)(iVar6 + (int)puVar4) = dVar12;
      }
      if (in_FPSCR_SZ == '\0') {
        dVar12 = (double)CONCAT44(*(undefined4 *)((int)puVar4 + 0x14),SUB84(dVar12,0));
      }
      else {
        dVar12 = *(double *)((int)puVar4 + 0x14);
      }
      if (in_FPSCR_PR == '\0') {
        dVar12 = (double)CONCAT44((float)((ulonglong)dVar12 >> 0x20) - SUB84(dVar12,0),
                                  SUB84(dVar12,0));
      }
      else {
        dVar12 = dVar12 - dVar12;
      }
      if (in_FPSCR_PR == '\0') {
        dVar12 = (double)CONCAT44((float)((ulonglong)dVar12 >> 0x20) * in_fr15,SUB84(dVar12,0));
      }
      else {
        dVar12 = dVar12 * (double)CONCAT44(in_fr14,in_fr15);
      }
      iVar6 = ((uint)*puVar3 * 3 + 8) * 4;
      if (in_FPSCR_SZ == '\0') {
        *(int *)(iVar6 + (int)puVar4) = (int)((ulonglong)dVar12 >> 0x20);
      }
      else {
        *(double *)(iVar6 + (int)puVar4) = dVar12;
      }
    }
    if (in_FPSCR_SZ == '\0') {
      uVar14 = CONCAT44(*(undefined4 *)puVar2,param_1);
    }
    else {
      uVar14 = *puVar2;
    }
    (*(code *)*DAT_8c60ba50)(uVar14,puVar4,puVar9);
  }
  (*(code *)PTR_FUN_8c60ba68)(puVar8);
  (*(code *)PTR_FUN_8c60ba70)(puVar10 + -0x28,*(undefined4 *)PTR_DAT_8c60ba6c);
  (*(code *)PTR_FUN_8c60ba74)(puVar9);
  return;
}

