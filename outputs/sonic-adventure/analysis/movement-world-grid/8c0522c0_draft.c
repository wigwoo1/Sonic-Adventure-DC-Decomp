/* Ghidra draft only; not buildable/verified source. */

void FUN_8c0522c0(double param_1,double param_2)

{
  short *psVar1;
  undefined *puVar2;
  uint *puVar3;
  int iVar4;
  uint *puVar5;
  uint uVar6;
  int iVar7;
  double *pdVar8;
  double dVar9;
  double dVar10;
  undefined4 uVar12;
  undefined4 uVar13;
  double dVar11;
  float fVar14;
  float fVar15;
  undefined8 in_dr12;
  double in_dr14;
  double in_xd4;
  undefined8 in_xd6;
  undefined8 in_xd14;
  undefined4 in_PR;
  char in_FPSCR_PR;
  char in_FPSCR_SZ;
  undefined1 auStack_3c [16];
  double dStack_2c;
  undefined8 uStack_24;
  
  puVar2 = PTR_FUN_8c052448;
  psVar1 = DAT_8c052444;
  puVar5 = DAT_8c052440;
  if (in_FPSCR_SZ == '\0') {
    uStack_24 = CONCAT44(SUB84(in_dr14,0),(undefined4)uStack_24);
                    /* WARNING: Ignoring partial resolution of indirect */
    uStack_24._0_4_ = (int)((ulonglong)in_dr14 >> 0x20);
    pdVar8 = &dStack_2c;
    dStack_2c = (double)CONCAT44((int)in_dr12,(int)((ulonglong)in_dr12 >> 0x20));
  }
  else {
    pdVar8 = (double *)auStack_3c;
    uStack_24 = in_xd14;
    dStack_2c = in_dr14;
  }
  *(undefined4 *)((int)pdVar8 + -4) = in_PR;
  if (in_FPSCR_SZ == '\0') {
    *(int *)((int)pdVar8 + -0x1c) = (int)((ulonglong)param_1 >> 0x20);
    *(int *)((int)pdVar8 + -0x18) = (int)((ulonglong)param_2 >> 0x20);
    in_xd4 = (double)CONCAT44(SUB84(param_1,0),(int)in_dr12);
    *(int *)((int)pdVar8 + -0x14) = SUB84(param_2,0);
  }
  else {
    *(double *)((int)pdVar8 + -0x1c) = param_1;
    *(double *)((int)pdVar8 + -0x18) = param_2;
    *(undefined8 *)((int)pdVar8 + -0x14) = in_xd6;
  }
  *DAT_8c052444 = 0;
  (*(code *)puVar2)(0);
  puVar3 = DAT_8c052450 + (*(int *)PTR_DAT_8c05244c + -1) * 3;
  if (in_FPSCR_SZ == '\0') {
    in_dr14 = (double)CONCAT44((int)((ulonglong)in_dr14 >> 0x20),(undefined4)DAT_8c052454);
  }
  if (DAT_8c052450 <= puVar3) {
    do {
      uVar13 = SUB84(param_2,0);
      uVar6 = puVar3[1];
      if ((*puVar3 & DAT_8c052454._4_4_) == 0) {
        if (in_FPSCR_SZ == '\0') {
          param_2 = (double)CONCAT44(*(undefined4 *)((int)pdVar8 + -0x1c),*(undefined4 *)(uVar6 + 8)
                                    );
        }
        else {
          param_2 = *(double *)((int)pdVar8 + -0x1c);
        }
        iVar7 = *(int *)(uVar6 + 4);
        if (in_FPSCR_PR == '\0') {
          param_2 = (double)CONCAT44((float)((ulonglong)param_2 >> 0x20) - SUB84(param_2,0),
                                     SUB84(param_2,0));
        }
        else {
          param_2 = param_2 - param_2;
        }
        dVar11 = param_2;
        if (in_FPSCR_SZ == '\0') {
          uVar12 = (undefined4)((ulonglong)param_2 >> 0x20);
          param_2 = (double)CONCAT44(uVar12,uVar13);
          dVar11 = (double)CONCAT44(uVar12,*(undefined4 *)(iVar7 + 0x18));
        }
        if (in_FPSCR_PR == '\0') {
          param_2 = (double)CONCAT44((float)((ulonglong)param_2 >> 0x20) - SUB84(dVar11,0),
                                     SUB84(param_2,0));
        }
        else {
          param_2 = param_2 - dVar11;
        }
        dVar11 = in_xd4;
        if (in_FPSCR_SZ == '\0') {
          dVar11 = (double)CONCAT44((int)((ulonglong)in_xd4 >> 0x20),*(undefined4 *)(uVar6 + 0xc));
        }
        if (in_FPSCR_PR == '\0') {
          dVar11 = (double)CONCAT44((float)((ulonglong)dVar11 >> 0x20) - SUB84(dVar11,0),
                                    SUB84(dVar11,0));
        }
        else {
          dVar11 = dVar11 - dVar11;
        }
        if (in_FPSCR_SZ == '\0') {
          uVar13 = (undefined4)((ulonglong)dVar11 >> 0x20);
          dVar11 = (double)CONCAT44(uVar13,*(undefined4 *)(iVar7 + 0x1c));
          param_1 = (double)CONCAT44((int)((ulonglong)param_1 >> 0x20),uVar13);
        }
        if (in_FPSCR_PR == '\0') {
          fVar14 = SUB84(param_1,0) - SUB84(dVar11,0);
        }
        else {
          fVar14 = SUB84(param_1 - dVar11,0);
        }
        if (in_FPSCR_SZ == '\0') {
          dVar11 = (double)CONCAT44(*(undefined4 *)((int)pdVar8 + -0x18),
                                    *(undefined4 *)(uVar6 + 0x10));
        }
        else {
          dVar11 = *(double *)((int)pdVar8 + -0x18);
        }
        if (in_FPSCR_PR == '\0') {
          dVar11 = (double)CONCAT44((float)((ulonglong)dVar11 >> 0x20) - SUB84(dVar11,0),
                                    SUB84(dVar11,0));
        }
        else {
          dVar11 = dVar11 - dVar11;
        }
        if (in_FPSCR_SZ == '\0') {
          dVar11 = (double)CONCAT44((int)((ulonglong)dVar11 >> 0x20),*(undefined4 *)(iVar7 + 0x20));
        }
      }
      else {
        (*(code *)PTR_FUN_8c05245c)(0);
        if (*(int *)(uVar6 + 0x1c) != 0) {
          (*(code *)PTR_FUN_8c052460)(0,*(int *)(uVar6 + 0x1c));
        }
        if (*(int *)(uVar6 + 0x14) != 0) {
          (*(code *)PTR_FUN_8c052464)(0,*(int *)(uVar6 + 0x14));
        }
        if (*(int *)(uVar6 + 0x18) != 0) {
          (*(code *)PTR_FUN_8c052468)(0,*(int *)(uVar6 + 0x18));
        }
        (*(code *)PTR_FUN_8c05246c)
                  (0,*(int *)(uVar6 + 4) + 0x18,(undefined1 *)((int)pdVar8 + -0x10));
        if (in_FPSCR_SZ == '\0') {
          param_2 = (double)CONCAT44(*(undefined4 *)((int)pdVar8 + -0x1c),*(undefined4 *)(uVar6 + 8)
                                    );
        }
        else {
          param_2 = *(double *)((int)pdVar8 + -0x1c);
        }
        if (in_FPSCR_PR == '\0') {
          param_2 = (double)CONCAT44((float)((ulonglong)param_2 >> 0x20) - SUB84(param_2,0),
                                     SUB84(param_2,0));
        }
        else {
          param_2 = param_2 - param_2;
        }
        dVar11 = param_2;
        if (in_FPSCR_SZ == '\0') {
          uVar12 = (undefined4)((ulonglong)param_2 >> 0x20);
          param_2 = (double)CONCAT44(uVar12,uVar13);
          dVar11 = (double)CONCAT44(uVar12,*(undefined4 *)((int)pdVar8 + -0x10));
        }
        if (in_FPSCR_PR == '\0') {
          param_2 = (double)CONCAT44((float)((ulonglong)param_2 >> 0x20) - SUB84(dVar11,0),
                                     SUB84(param_2,0));
        }
        else {
          param_2 = param_2 - dVar11;
        }
        dVar11 = in_xd4;
        if (in_FPSCR_SZ == '\0') {
          dVar11 = (double)CONCAT44((int)((ulonglong)in_xd4 >> 0x20),*(undefined4 *)(uVar6 + 0xc));
        }
        if (in_FPSCR_PR == '\0') {
          dVar11 = (double)CONCAT44((float)((ulonglong)dVar11 >> 0x20) - SUB84(dVar11,0),
                                    SUB84(dVar11,0));
        }
        else {
          dVar11 = dVar11 - dVar11;
        }
        if (in_FPSCR_SZ == '\0') {
          uVar13 = (undefined4)((ulonglong)dVar11 >> 0x20);
          dVar11 = (double)CONCAT44(uVar13,*(undefined4 *)((int)pdVar8 + -0xc));
          param_1 = (double)CONCAT44((int)((ulonglong)param_1 >> 0x20),uVar13);
        }
        if (in_FPSCR_PR == '\0') {
          fVar14 = SUB84(param_1,0) - SUB84(dVar11,0);
        }
        else {
          fVar14 = SUB84(param_1 - dVar11,0);
        }
        if (in_FPSCR_SZ == '\0') {
          dVar11 = (double)CONCAT44(*(undefined4 *)((int)pdVar8 + -0x18),
                                    *(undefined4 *)(uVar6 + 0x10));
        }
        else {
          dVar11 = *(double *)((int)pdVar8 + -0x18);
        }
        if (in_FPSCR_PR == '\0') {
          dVar11 = (double)CONCAT44((float)((ulonglong)dVar11 >> 0x20) - SUB84(dVar11,0),
                                    SUB84(dVar11,0));
        }
        else {
          dVar11 = dVar11 - dVar11;
        }
        if (in_FPSCR_SZ == '\0') {
          dVar11 = (double)CONCAT44((int)((ulonglong)dVar11 >> 0x20),
                                    *(undefined4 *)((int)pdVar8 + -8));
        }
      }
      uVar13 = (undefined4)((ulonglong)dVar11 >> 0x20);
      dVar10 = dVar11;
      if (in_FPSCR_SZ == '\0') {
        dVar10 = (double)CONCAT44(uVar13,fVar14);
      }
      if (in_FPSCR_PR == '\0') {
        dVar10 = (double)CONCAT44((float)((ulonglong)dVar10 >> 0x20) - SUB84(dVar11,0),
                                  SUB84(dVar10,0));
      }
      else {
        dVar10 = dVar10 - dVar11;
      }
      if (in_FPSCR_SZ == '\0') {
        dVar11 = (double)CONCAT44(uVar13,SUB84(dVar10,0));
      }
      if (in_FPSCR_PR == '\0') {
        dVar11 = (double)(ulonglong)(uint)(SUB84(dVar11,0) * SUB84(dVar10,0));
      }
      else {
        dVar11 = dVar11 * dVar10;
      }
      fVar14 = (float)((ulonglong)param_2 >> 0x20);
      dVar9 = param_2;
      if (in_FPSCR_SZ == '\0') {
        dVar9 = (double)((ulonglong)(uint)fVar14 << 0x20);
      }
      fVar15 = (float)((ulonglong)dVar10 >> 0x20);
      if (in_FPSCR_PR == '\0') {
        dVar11 = (double)(ulonglong)
                         (uint)(fVar15 * fVar15 +
                               (float)((ulonglong)dVar9 >> 0x20) * fVar14 + SUB84(dVar11,0));
      }
      if (in_FPSCR_SZ == '\0') {
        dVar10 = (double)((ulonglong)dVar11 & 0xffffffff);
        dVar11 = (double)CONCAT44(*(undefined4 *)(*(int *)(uVar6 + 4) + 0x24),
                                  *(undefined4 *)((int)pdVar8 + -0x14));
      }
      else {
        dVar11 = *(double *)(*(int *)(uVar6 + 4) + 0x24);
      }
      if (in_FPSCR_PR == '\0') {
        dVar11 = (double)CONCAT44((float)((ulonglong)dVar11 >> 0x20) + SUB84(dVar11,0),
                                  SUB84(dVar11,0));
      }
      else {
        dVar11 = dVar11 + dVar11;
      }
      if (in_FPSCR_SZ == '\0') {
        dVar11 = (double)CONCAT44((int)((ulonglong)dVar11 >> 0x20),SUB84(dVar10,0));
      }
      if (in_FPSCR_PR == '\0') {
        fVar14 = (float)((ulonglong)dVar11 >> 0x20) + SUB84(in_dr14,0);
        param_1 = (double)CONCAT44(fVar14 * fVar14,SUB84(dVar11,0));
      }
      else {
        param_1 = (dVar11 + in_dr14) * (dVar11 + in_dr14);
      }
      if (in_FPSCR_PR == '\0' && SUB84(param_1,0) < (float)((ulonglong)param_1 >> 0x20)) {
        puVar5[1] = uVar6;
        puVar5[2] = puVar3[2];
        *puVar5 = *puVar3;
        *psVar1 = *psVar1 + 1;
        puVar5 = puVar5 + 3;
        if (0x3ff < *psVar1) {
          (*(code *)PTR_FUN_8c052470)(1);
          return;
        }
      }
      puVar3 = puVar3 + -3;
    } while (DAT_8c052450 <= puVar3);
  }
  (*(code *)PTR_FUN_8c052470)(1);
  uVar6 = DAT_8c052484;
  if ((*(int *)PTR_DAT_8c052474 == 1) && (*DAT_8c052478 != 0)) {
    iVar7 = DAT_8c05247c;
    for (iVar4 = *(int *)PTR_DAT_8c052480; 0 < (short)iVar4; iVar4 = iVar4 + -1) {
      if ((*(uint *)(iVar7 + 0x20) & uVar6) != 0) {
        if (in_FPSCR_SZ == '\0') {
          dVar11 = (double)CONCAT44(*(undefined4 *)((int)pdVar8 + -0x18),
                                    *(undefined4 *)((int)pdVar8 + -0x1c));
        }
        else {
          dVar11 = *(double *)((int)pdVar8 + -0x18);
        }
        dVar10 = (double)(*(code *)PTR_FUN_8c052600)();
        if (in_FPSCR_SZ == '\0') {
          dVar11 = (double)CONCAT44((int)((ulonglong)dVar11 >> 0x20),
                                    *(undefined4 *)((int)pdVar8 + -0x14));
          dVar10 = (double)CONCAT44((int)((ulonglong)dVar10 >> 0x20),*(undefined4 *)(iVar7 + 0xc));
        }
        if (in_FPSCR_PR == '\0') {
          dVar10 = (double)CONCAT44((float)((ulonglong)dVar10 >> 0x20) - SUB84(dVar11,0),
                                    SUB84(dVar10,0));
        }
        else {
          dVar10 = dVar10 - dVar11;
        }
        if (in_FPSCR_PR == '\0' && (float)((ulonglong)dVar10 >> 0x20) < SUB84(dVar10,0)) {
          *puVar5 = *(uint *)(iVar7 + 0x20);
          puVar5[2] = 0;
          puVar5[1] = *(uint *)(iVar7 + 0x18);
          *psVar1 = *psVar1 + 1;
          puVar5 = puVar5 + 3;
          if (0x3ff < *psVar1) {
            return;
          }
        }
      }
      iVar7 = iVar7 + 0x24;
    }
  }
  return;
}

