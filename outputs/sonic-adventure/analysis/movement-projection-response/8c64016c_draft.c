/* Ghidra draft only; not buildable/verified source. */

/* WARNING: Removing unreachable block (ram,0x8c6402be) */
/* WARNING: Removing unreachable block (ram,0x8c640262) */
/* WARNING: Removing unreachable block (ram,0x8c64026c) */
/* WARNING: Removing unreachable block (ram,0x8c6402c6) */

void FUN_8c64016c(double param_1,double *param_2,double *param_3,double *param_4)

{
  int iVar1;
  undefined1 *puVar2;
  float fVar3;
  double extraout_dr0;
  undefined4 in_fr3;
  double dVar5;
  double dVar6;
  longlong lVar7;
  float fVar8;
  float fVar9;
  double dVar10;
  undefined4 uVar12;
  undefined4 uVar13;
  double in_xd2;
  double dVar14;
  double in_xd6;
  double dVar15;
  double in_xd14;
  undefined4 in_PR;
  char in_FPSCR_PR;
  char in_FPSCR_SZ;
  undefined1 local_14 [4];
  undefined1 local_10 [4];
  undefined1 *puStack_c;
  double dVar4;
  float fVar11;
  
  puStack_c = &DAT_8c754ef4;
  if (in_FPSCR_SZ == '\0') {
    puVar2 = local_10;
  }
  else {
    puVar2 = local_14;
  }
  *(undefined4 *)(puVar2 + -4) = in_PR;
  uVar13 = 0;
  iVar1 = (*(code *)PTR_FUN_8c640364)();
  if (iVar1 != 0) {
    if (in_FPSCR_SZ == '\0') {
      in_fr3 = *(undefined4 *)param_2;
    }
    else {
      in_xd2 = *param_2;
    }
    if (in_FPSCR_SZ == '\0') {
      *(undefined4 *)(puVar2 + -0x10) = in_fr3;
      *(undefined4 *)(puVar2 + -0xc) = *(undefined4 *)((int)param_2 + 4);
      *(undefined4 *)(puVar2 + -8) = *(undefined4 *)(param_2 + 1);
    }
    else {
      *(double *)(puVar2 + -0x10) = in_xd2;
      *(undefined8 *)(puVar2 + -0xc) = *(undefined8 *)((int)param_2 + 4);
      *(double *)(puVar2 + -8) = param_2[1];
    }
    if (param_4 != (double *)0x0) {
      if (in_FPSCR_SZ == '\0') {
        *(undefined4 *)param_4 = uVar13;
        *(undefined4 *)((int)param_4 + 4) = uVar13;
        *(undefined4 *)(param_4 + 1) = uVar13;
        *(undefined4 *)((int)param_4 + 0xc) = uVar13;
        *(undefined4 *)(param_4 + 2) = uVar13;
        *(undefined4 *)((int)param_4 + 0x14) = uVar13;
      }
      else {
        *param_4 = in_xd14;
        *(double *)((int)param_4 + 4) = in_xd14;
        param_4[1] = in_xd14;
        *(double *)((int)param_4 + 0xc) = in_xd14;
        param_4[2] = in_xd14;
        *(double *)((int)param_4 + 0x14) = in_xd14;
      }
    }
    (*(code *)PTR_FUN_8c640368)(puVar2 + -0x10,param_3,0);
    return;
  }
  if (param_4 == (double *)0x0) {
    return;
  }
  if (in_FPSCR_SZ == '\0') {
    dVar5 = (double)CONCAT44(*(undefined4 *)(param_2 + 2),*(undefined4 *)((int)param_3 + 0x14));
  }
  else {
    in_xd2 = *(double *)((int)param_3 + 0x14);
    dVar5 = param_2[2];
  }
  if (in_FPSCR_PR == '\0') {
    dVar5 = (double)CONCAT44((float)((ulonglong)dVar5 >> 0x20) * SUB84(dVar5,0),SUB84(dVar5,0));
  }
  else {
    dVar5 = dVar5 * dVar5;
  }
  if (in_FPSCR_SZ == '\0') {
    dVar5 = (double)CONCAT44((int)((ulonglong)dVar5 >> 0x20),*(undefined4 *)((int)param_2 + 0x14));
    dVar4 = (double)CONCAT44((int)((ulonglong)extraout_dr0 >> 0x20),*(undefined4 *)(param_3 + 2));
  }
  else {
    in_xd2 = *(double *)((int)param_2 + 0x14);
    dVar4 = extraout_dr0;
  }
  if (in_FPSCR_PR == '\0') {
    fVar3 = SUB84(dVar4,0) * SUB84(dVar5,0);
    dVar4 = (double)CONCAT44((int)((ulonglong)dVar4 >> 0x20),fVar3);
    dVar5 = (double)CONCAT44((float)((ulonglong)dVar5 >> 0x20) - fVar3,SUB84(dVar5,0));
  }
  else {
    dVar4 = dVar4 * dVar5;
    dVar5 = dVar5 - dVar4;
  }
  if (in_FPSCR_SZ == '\0') {
    *(int *)((int)param_4 + 0xc) = (int)((ulonglong)dVar5 >> 0x20);
    dVar5 = (double)CONCAT44(*(undefined4 *)((int)param_2 + 0x14),
                             *(undefined4 *)((int)param_3 + 0xc));
  }
  else {
    *(double *)((int)param_4 + 0xc) = dVar5;
    in_xd2 = *(double *)((int)param_3 + 0xc);
    dVar5 = *(double *)((int)param_2 + 0x14);
  }
  if (in_FPSCR_PR == '\0') {
    dVar5 = (double)CONCAT44((float)((ulonglong)dVar5 >> 0x20) * SUB84(dVar5,0),SUB84(dVar5,0));
  }
  else {
    dVar5 = dVar5 * dVar5;
  }
  if (in_FPSCR_SZ == '\0') {
    dVar5 = (double)CONCAT44((int)((ulonglong)dVar5 >> 0x20),*(undefined4 *)((int)param_2 + 0xc));
    dVar4 = (double)CONCAT44((int)((ulonglong)dVar4 >> 0x20),*(undefined4 *)((int)param_3 + 0x14));
  }
  else {
    in_xd2 = *(double *)((int)param_2 + 0xc);
  }
  if (in_FPSCR_PR == '\0') {
    fVar3 = SUB84(dVar4,0) * SUB84(dVar5,0);
    dVar4 = (double)CONCAT44((int)((ulonglong)dVar4 >> 0x20),fVar3);
    dVar5 = (double)CONCAT44((float)((ulonglong)dVar5 >> 0x20) - fVar3,SUB84(dVar5,0));
  }
  else {
    dVar4 = dVar4 * dVar5;
    dVar5 = dVar5 - dVar4;
  }
  if (in_FPSCR_SZ == '\0') {
    *(int *)(param_4 + 2) = (int)((ulonglong)dVar5 >> 0x20);
    dVar5 = (double)CONCAT44(*(undefined4 *)((int)param_2 + 0xc),*(undefined4 *)(param_3 + 2));
  }
  else {
    param_4[2] = dVar5;
    in_xd2 = param_3[2];
    dVar5 = *(double *)((int)param_2 + 0xc);
  }
  if (in_FPSCR_PR == '\0') {
    dVar5 = (double)CONCAT44((float)((ulonglong)dVar5 >> 0x20) * SUB84(dVar5,0),SUB84(dVar5,0));
  }
  else {
    dVar5 = dVar5 * dVar5;
  }
  if (in_FPSCR_SZ == '\0') {
    dVar5 = (double)CONCAT44((int)((ulonglong)dVar5 >> 0x20),*(undefined4 *)(param_2 + 2));
    dVar4 = (double)CONCAT44((int)((ulonglong)dVar4 >> 0x20),*(undefined4 *)((int)param_3 + 0xc));
  }
  else {
    in_xd2 = param_2[2];
  }
  if (in_FPSCR_PR == '\0') {
    fVar3 = SUB84(dVar4,0) * SUB84(dVar5,0);
    dVar4 = (double)CONCAT44((int)((ulonglong)dVar4 >> 0x20),fVar3);
    dVar5 = (double)CONCAT44((float)((ulonglong)dVar5 >> 0x20) - fVar3,SUB84(dVar5,0));
  }
  else {
    dVar4 = dVar4 * dVar5;
    dVar5 = dVar5 - dVar4;
  }
  if (in_FPSCR_SZ == '\0') {
    *(int *)((int)param_4 + 0x14) = (int)((ulonglong)dVar5 >> 0x20);
    dVar5 = (double)CONCAT44(*(undefined4 *)((int)param_2 + 0xc),*(undefined4 *)param_2);
  }
  else {
    *(double *)((int)param_4 + 0x14) = dVar5;
    dVar5 = *(double *)((int)param_2 + 0xc);
    in_xd2 = *param_2;
  }
  if (in_FPSCR_PR == '\0') {
    dVar5 = (double)CONCAT44((float)((ulonglong)dVar5 >> 0x20) * SUB84(dVar5,0),SUB84(dVar5,0));
  }
  else {
    dVar5 = dVar5 * dVar5;
  }
  if (in_FPSCR_SZ == '\0') {
    dVar5 = (double)CONCAT44((int)((ulonglong)dVar5 >> 0x20),*(undefined4 *)((int)param_2 + 4));
    dVar4 = (double)CONCAT44((int)((ulonglong)dVar4 >> 0x20),*(undefined4 *)(param_2 + 2));
  }
  else {
    in_xd2 = *(double *)((int)param_2 + 4);
  }
  if (in_FPSCR_PR == '\0') {
    dVar4 = (double)CONCAT44((int)((ulonglong)dVar4 >> 0x20),SUB84(dVar4,0) * SUB84(dVar5,0));
    dVar5 = (double)CONCAT44(-(float)((ulonglong)dVar5 >> 0x20),SUB84(dVar5,0));
  }
  else {
    dVar4 = dVar4 * dVar5;
    dVar5 = -dVar5;
  }
  if (in_FPSCR_SZ == '\0') {
    dVar5 = (double)CONCAT44((int)((ulonglong)dVar5 >> 0x20),*(undefined4 *)(param_2 + 1));
  }
  else {
    in_xd2 = param_2[1];
  }
  if (in_FPSCR_PR == '\0') {
    dVar5 = (double)CONCAT44((float)((ulonglong)dVar5 >> 0x20) - SUB84(dVar4,0),SUB84(dVar5,0));
  }
  else {
    dVar5 = dVar5 - dVar4;
  }
  if (in_FPSCR_SZ == '\0') {
    dVar4 = (double)CONCAT44((int)((ulonglong)dVar4 >> 0x20),*(undefined4 *)((int)param_2 + 0x14));
  }
  if (in_FPSCR_PR == '\0') {
    dVar4 = (double)CONCAT44((int)((ulonglong)dVar4 >> 0x20),SUB84(dVar4,0) * SUB84(dVar5,0));
  }
  else {
    dVar4 = dVar4 * dVar5;
  }
  if (in_FPSCR_SZ == '\0') {
    param_1 = (double)CONCAT44((int)((ulonglong)param_1 >> 0x20),(int)((ulonglong)dVar5 >> 0x20));
    dVar5 = (double)CONCAT44(*(undefined4 *)((int)param_3 + 0xc),*(undefined4 *)param_3);
  }
  else {
    in_xd2 = *param_3;
    dVar5 = *(double *)((int)param_3 + 0xc);
  }
  if (in_FPSCR_PR == '\0') {
    fVar3 = SUB84(param_1,0) - SUB84(dVar4,0);
    dVar5 = (double)CONCAT44((float)((ulonglong)dVar5 >> 0x20) * SUB84(dVar5,0),SUB84(dVar5,0));
  }
  else {
    fVar3 = SUB84(param_1 - dVar4,0);
    dVar5 = dVar5 * dVar5;
  }
  if (in_FPSCR_SZ == '\0') {
    dVar5 = (double)CONCAT44((int)((ulonglong)dVar5 >> 0x20),*(undefined4 *)((int)param_3 + 4));
    dVar4 = (double)CONCAT44((int)((ulonglong)dVar4 >> 0x20),*(undefined4 *)(param_3 + 2));
  }
  else {
    in_xd2 = *(double *)((int)param_3 + 4);
  }
  if (in_FPSCR_PR == '\0') {
    dVar6 = (double)CONCAT44(-(float)((ulonglong)dVar5 >> 0x20),SUB84(dVar5,0));
    dVar4 = (double)CONCAT44((int)((ulonglong)dVar4 >> 0x20),SUB84(dVar4,0) * SUB84(dVar5,0));
  }
  else {
    dVar6 = -dVar5;
    dVar4 = dVar4 * dVar6;
  }
  if (in_FPSCR_SZ == '\0') {
    dVar6 = (double)CONCAT44((int)((ulonglong)dVar6 >> 0x20),*(undefined4 *)(param_3 + 1));
  }
  else {
    in_xd2 = param_3[1];
  }
  if (in_FPSCR_PR == '\0') {
    dVar6 = (double)CONCAT44((float)((ulonglong)dVar6 >> 0x20) - SUB84(dVar4,0),SUB84(dVar6,0));
  }
  else {
    dVar6 = dVar6 - dVar4;
  }
  if (in_FPSCR_SZ == '\0') {
    dVar4 = (double)CONCAT44((int)((ulonglong)dVar4 >> 0x20),*(undefined4 *)((int)param_3 + 0x14));
  }
  if (in_FPSCR_PR == '\0') {
    dVar4 = (double)CONCAT44((int)((ulonglong)dVar4 >> 0x20),SUB84(dVar4,0) * SUB84(dVar6,0));
  }
  else {
    dVar4 = dVar4 * dVar6;
  }
  dVar5 = (double)((ulonglong)dVar6 & 0xffffffff00000000);
  if (in_FPSCR_SZ == '\0') {
    dVar5 = (double)CONCAT44((int)((ulonglong)dVar6 >> 0x20),fVar3);
    lVar7 = (ulonglong)*(uint *)((int)param_4 + 0xc) << 0x20;
  }
  else {
    lVar7 = *(longlong *)((int)param_4 + 0xc);
  }
  if (in_FPSCR_PR == '\0') {
    dVar5 = (double)CONCAT44((float)((ulonglong)dVar5 >> 0x20) - SUB84(dVar4,0),SUB84(dVar5,0));
  }
  else {
    dVar5 = dVar5 - dVar4;
  }
  uVar12 = (undefined4)((ulonglong)dVar4 >> 0x20);
  fVar9 = SUB84(dVar5,0);
  fVar3 = (float)((ulonglong)dVar5 >> 0x20);
  if ((float)((ulonglong)lVar7 >> 0x20) == (float)lVar7) {
    if (in_FPSCR_SZ == '\0') {
      dVar6 = (double)CONCAT44(*(undefined4 *)(param_4 + 2),(float)lVar7);
    }
    else {
      dVar6 = param_4[2];
    }
    if ((float)((ulonglong)dVar6 >> 0x20) == SUB84(dVar6,0)) {
      if (in_FPSCR_SZ == '\0') {
        fVar11 = *(float *)(param_3 + 2);
        dVar10 = (double)CONCAT44(fVar11,SUB84(dVar6,0));
        dVar6 = (double)CONCAT44(fVar11,*(undefined4 *)(param_2 + 2));
      }
      else {
        dVar10 = param_3[2];
        in_xd6 = param_2[2];
        fVar11 = (float)((ulonglong)dVar10 >> 0x20);
        dVar6 = dVar10;
      }
      if (in_FPSCR_PR == '\0') {
        dVar10 = (double)CONCAT44((float)((ulonglong)dVar10 >> 0x20) * fVar9,SUB84(dVar10,0));
      }
      else {
        dVar10 = dVar10 * dVar5;
      }
      dVar14 = in_xd6;
      if (in_FPSCR_SZ == '\0') {
        dVar10 = (double)CONCAT44((int)((ulonglong)dVar10 >> 0x20),SUB84(dVar6,0));
        dVar14 = in_xd2;
      }
      if (in_FPSCR_PR == '\0') {
        dVar10 = (double)CONCAT44((int)((ulonglong)dVar10 >> 0x20),SUB84(dVar10,0) * fVar3);
      }
      else {
        dVar10 = dVar10 * dVar5;
      }
      if (in_FPSCR_SZ == '\0') {
        dVar4 = (double)CONCAT44(uVar12,*(undefined4 *)((int)param_2 + 0xc));
      }
      if (in_FPSCR_PR == '\0') {
        dVar4 = (double)CONCAT44((int)((ulonglong)dVar4 >> 0x20),SUB84(dVar4,0) * fVar11);
        fVar11 = SUB84(dVar10,0) - (float)((ulonglong)dVar10 >> 0x20);
      }
      else {
        dVar4 = dVar4 * dVar6;
        fVar11 = SUB84(dVar10 - dVar10,0);
      }
      if (in_FPSCR_SZ == '\0') {
        dVar10 = (double)CONCAT44(*(undefined4 *)((int)param_3 + 0xc),fVar11);
      }
      else {
        dVar10 = *(double *)((int)param_3 + 0xc);
      }
      if (in_FPSCR_PR == '\0') {
        fVar11 = SUB84(dVar10,0) /
                 (SUB84(dVar4,0) - (float)((ulonglong)dVar10 >> 0x20) * SUB84(dVar6,0));
      }
      else {
        fVar11 = SUB84((dVar10 * dVar6) / (dVar4 - dVar10 * dVar6),0);
      }
      if (in_FPSCR_SZ == '\0') {
        *(float *)param_4 = fVar11;
        uVar12 = *(undefined4 *)((int)param_3 + 0xc);
        dVar6 = (double)CONCAT44(uVar12,fVar11);
        dVar4 = (double)CONCAT44(uVar12,*(undefined4 *)((int)param_2 + 0xc));
      }
      else {
        *param_4 = dVar14;
        dVar6 = *(double *)((int)param_3 + 0xc);
        in_xd6 = *(double *)((int)param_2 + 0xc);
        uVar12 = (undefined4)((ulonglong)dVar6 >> 0x20);
        dVar4 = dVar6;
      }
      if (in_FPSCR_PR == '\0') {
        dVar6 = (double)CONCAT44((float)((ulonglong)dVar6 >> 0x20) * fVar9,SUB84(dVar6,0));
      }
      else {
        dVar6 = dVar6 * dVar5;
      }
      dVar10 = in_xd6;
      if (in_FPSCR_SZ == '\0') {
        dVar6 = (double)CONCAT44((int)((ulonglong)dVar6 >> 0x20),SUB84(dVar4,0));
        dVar10 = dVar14;
      }
      if (in_FPSCR_PR == '\0') {
        dVar6 = (double)CONCAT44((int)((ulonglong)dVar6 >> 0x20),SUB84(dVar6,0) * fVar3);
      }
      else {
        dVar6 = dVar6 * dVar5;
      }
      if (in_FPSCR_SZ == '\0') {
        in_xd6 = (double)CONCAT44(SUB84(dVar4,0),uVar12);
      }
      if (in_FPSCR_PR == '\0') {
        fVar3 = SUB84(dVar6,0) - (float)((ulonglong)dVar6 >> 0x20);
      }
      else {
        fVar3 = SUB84(dVar6 - dVar6,0);
      }
      if (in_FPSCR_SZ == '\0') {
        dVar5 = (double)CONCAT44(*(undefined4 *)(param_2 + 2),fVar3);
      }
      else {
        dVar5 = param_2[2];
      }
      if (in_FPSCR_PR == '\0') {
        in_xd6 = (double)CONCAT44((int)((ulonglong)in_xd6 >> 0x20),
                                  SUB84(in_xd6,0) * (float)((ulonglong)dVar5 >> 0x20));
      }
      else {
        in_xd6 = in_xd6 * dVar5;
      }
      if (in_FPSCR_SZ == '\0') {
        dVar5 = (double)CONCAT44(*(undefined4 *)(param_3 + 2),SUB84(dVar5,0));
      }
      else {
        dVar5 = param_3[2];
      }
      if (in_FPSCR_PR == '\0') {
        fVar3 = SUB84(dVar5,0) /
                (SUB84(in_xd6,0) -
                (float)((ulonglong)in_xd6 >> 0x20) * (float)((ulonglong)dVar5 >> 0x20));
      }
      else {
        fVar3 = SUB84(dVar5 / (in_xd6 * dVar5 - in_xd6 * dVar5),0);
      }
      if (in_FPSCR_SZ == '\0') {
        *(float *)((int)param_4 + 4) = fVar3;
        *(undefined4 *)(param_4 + 1) = uVar13;
        return;
      }
      *(double *)((int)param_4 + 4) = dVar10;
      param_4[1] = in_xd14;
      return;
    }
    if (in_FPSCR_SZ == '\0') {
      lVar7 = (ulonglong)*(uint *)((int)param_4 + 0xc) << 0x20;
    }
    else {
      lVar7 = *(longlong *)((int)param_4 + 0xc);
    }
    if ((float)((ulonglong)lVar7 >> 0x20) == (float)lVar7) {
      if (in_FPSCR_SZ == '\0') {
        lVar7 = CONCAT44(*(undefined4 *)((int)param_4 + 0x14),(float)lVar7);
      }
      else {
        lVar7 = *(longlong *)((int)param_4 + 0x14);
      }
      if ((float)((ulonglong)lVar7 >> 0x20) == (float)lVar7) {
        if (in_FPSCR_SZ == '\0') {
          fVar11 = *(float *)((int)param_3 + 0x14);
          dVar10 = (double)CONCAT44(fVar11,(float)lVar7);
          dVar6 = (double)CONCAT44(fVar11,*(undefined4 *)((int)param_2 + 0x14));
        }
        else {
          dVar10 = *(double *)((int)param_3 + 0x14);
          in_xd6 = *(double *)((int)param_2 + 0x14);
          fVar11 = (float)((ulonglong)dVar10 >> 0x20);
          dVar6 = dVar10;
        }
        if (in_FPSCR_PR == '\0') {
          dVar10 = (double)CONCAT44((float)((ulonglong)dVar10 >> 0x20) * fVar9,SUB84(dVar10,0));
        }
        else {
          dVar10 = dVar10 * dVar5;
        }
        dVar15 = in_xd6;
        if (in_FPSCR_SZ == '\0') {
          dVar10 = (double)CONCAT44((int)((ulonglong)dVar10 >> 0x20),SUB84(dVar6,0));
          dVar15 = in_xd2;
        }
        if (in_FPSCR_PR == '\0') {
          dVar10 = (double)CONCAT44((int)((ulonglong)dVar10 >> 0x20),SUB84(dVar10,0) * fVar3);
        }
        else {
          dVar10 = dVar10 * dVar5;
        }
        if (in_FPSCR_SZ == '\0') {
          dVar4 = (double)CONCAT44(uVar12,*(undefined4 *)((int)param_2 + 0xc));
        }
        if (in_FPSCR_PR == '\0') {
          dVar4 = (double)CONCAT44((int)((ulonglong)dVar4 >> 0x20),SUB84(dVar4,0) * fVar11);
          fVar11 = SUB84(dVar10,0) - (float)((ulonglong)dVar10 >> 0x20);
        }
        else {
          dVar4 = dVar4 * dVar6;
          fVar11 = SUB84(dVar10 - dVar10,0);
        }
        if (in_FPSCR_SZ == '\0') {
          dVar10 = (double)CONCAT44(*(undefined4 *)((int)param_3 + 0xc),fVar11);
        }
        else {
          dVar10 = *(double *)((int)param_3 + 0xc);
        }
        if (in_FPSCR_PR == '\0') {
          fVar8 = (float)((ulonglong)dVar10 >> 0x20) * SUB84(dVar6,0);
          fVar11 = SUB84(dVar4,0) - fVar8;
          dVar4 = (double)CONCAT44((int)((ulonglong)dVar4 >> 0x20),fVar11);
          dVar10 = (double)CONCAT44(fVar8,SUB84(dVar10,0) / fVar11);
        }
        else {
          dVar4 = dVar4 - dVar10 * dVar6;
          dVar10 = (dVar10 * dVar6) / dVar4;
        }
        if (in_FPSCR_SZ == '\0') {
          *(int *)param_4 = SUB84(dVar10,0);
          *(undefined4 *)((int)param_4 + 4) = uVar13;
          dVar6 = (double)CONCAT44(*(undefined4 *)((int)param_3 + 0xc),
                                   *(undefined4 *)((int)param_2 + 0xc));
          dVar10 = (double)CONCAT44((int)((ulonglong)dVar10 >> 0x20),
                                    *(undefined4 *)((int)param_2 + 0xc));
        }
        else {
          *param_4 = dVar15;
          *(double *)((int)param_4 + 4) = in_xd14;
          dVar15 = *(double *)((int)param_2 + 0xc);
          dVar6 = *(double *)((int)param_3 + 0xc);
          in_xd6 = dVar15;
        }
        if (in_FPSCR_PR == '\0') {
          fVar3 = SUB84(dVar10,0) * fVar3;
        }
        else {
          fVar3 = SUB84(dVar10 * dVar5,0);
        }
        uVar13 = (undefined4)((ulonglong)dVar6 >> 0x20);
        dVar10 = dVar6;
        if (in_FPSCR_SZ == '\0') {
          dVar10 = (double)CONCAT44(uVar13,fVar3);
        }
        if (in_FPSCR_PR == '\0') {
          dVar10 = (double)CONCAT44((float)((ulonglong)dVar10 >> 0x20) * fVar9,SUB84(dVar10,0));
        }
        else {
          dVar10 = dVar10 * dVar5;
        }
        if (in_FPSCR_SZ == '\0') {
          dVar4 = (double)CONCAT44((int)((ulonglong)dVar4 >> 0x20),uVar13);
        }
        if (in_FPSCR_PR == '\0') {
          fVar3 = SUB84(dVar10,0) - (float)((ulonglong)dVar10 >> 0x20);
        }
        else {
          fVar3 = SUB84(dVar10 - dVar10,0);
        }
        if (in_FPSCR_SZ == '\0') {
          dVar5 = (double)CONCAT44(*(undefined4 *)((int)param_2 + 0x14),fVar3);
        }
        else {
          dVar5 = *(double *)((int)param_2 + 0x14);
        }
        if (in_FPSCR_PR == '\0') {
          fVar3 = SUB84(dVar4,0) * (float)((ulonglong)dVar5 >> 0x20);
        }
        else {
          fVar3 = SUB84(dVar4 * dVar5,0);
        }
        if (in_FPSCR_SZ == '\0') {
          dVar5 = (double)CONCAT44(*(undefined4 *)((int)param_3 + 0x14),SUB84(dVar5,0));
        }
        else {
          dVar5 = *(double *)((int)param_3 + 0x14);
        }
        goto LAB_8c640346;
      }
    }
  }
  if (in_FPSCR_SZ == '\0') {
    *(undefined4 *)param_4 = uVar13;
    fVar11 = *(float *)((int)param_2 + 0x14);
    dVar10 = (double)CONCAT44(fVar11,(int)lVar7);
    dVar6 = (double)CONCAT44(fVar11,*(undefined4 *)((int)param_3 + 0x14));
  }
  else {
    *param_4 = in_xd14;
    dVar10 = *(double *)((int)param_2 + 0x14);
    in_xd6 = *(double *)((int)param_3 + 0x14);
    fVar11 = (float)((ulonglong)dVar10 >> 0x20);
    dVar6 = dVar10;
  }
  if (in_FPSCR_PR == '\0') {
    dVar10 = (double)CONCAT44((float)((ulonglong)dVar10 >> 0x20) * fVar3,SUB84(dVar10,0));
  }
  else {
    dVar10 = dVar10 * dVar5;
  }
  dVar14 = in_xd6;
  if (in_FPSCR_SZ == '\0') {
    dVar10 = (double)CONCAT44((int)((ulonglong)dVar10 >> 0x20),SUB84(dVar6,0));
    dVar14 = in_xd2;
  }
  if (in_FPSCR_PR == '\0') {
    dVar10 = (double)CONCAT44((int)((ulonglong)dVar10 >> 0x20),SUB84(dVar10,0) * fVar9);
  }
  else {
    dVar10 = dVar10 * dVar5;
  }
  if (in_FPSCR_SZ == '\0') {
    dVar4 = (double)CONCAT44(uVar12,*(undefined4 *)(param_3 + 2));
  }
  if (in_FPSCR_PR == '\0') {
    dVar4 = (double)CONCAT44((int)((ulonglong)dVar4 >> 0x20),SUB84(dVar4,0) * fVar11);
    fVar11 = SUB84(dVar10,0) - (float)((ulonglong)dVar10 >> 0x20);
  }
  else {
    dVar4 = dVar4 * dVar6;
    fVar11 = SUB84(dVar10 - dVar10,0);
  }
  if (in_FPSCR_SZ == '\0') {
    dVar10 = (double)CONCAT44(*(undefined4 *)(param_2 + 2),fVar11);
  }
  else {
    dVar10 = param_2[2];
  }
  if (in_FPSCR_PR == '\0') {
    fVar11 = SUB84(dVar4,0) - (float)((ulonglong)dVar10 >> 0x20) * SUB84(dVar6,0);
    dVar4 = (double)CONCAT44((int)((ulonglong)dVar4 >> 0x20),fVar11);
    fVar11 = SUB84(dVar10,0) / fVar11;
  }
  else {
    dVar4 = dVar4 - dVar10 * dVar6;
    fVar11 = SUB84((dVar10 * dVar6) / dVar4,0);
  }
  if (in_FPSCR_SZ == '\0') {
    *(float *)((int)param_4 + 4) = fVar11;
    uVar13 = *(undefined4 *)(param_2 + 2);
    dVar10 = (double)CONCAT44(uVar13,fVar11);
    dVar6 = (double)CONCAT44(uVar13,*(undefined4 *)(param_3 + 2));
  }
  else {
    *(double *)((int)param_4 + 4) = dVar14;
    dVar10 = param_2[2];
    in_xd6 = param_3[2];
    uVar13 = (undefined4)((ulonglong)dVar10 >> 0x20);
    dVar6 = dVar10;
  }
  if (in_FPSCR_PR == '\0') {
    dVar10 = (double)CONCAT44((float)((ulonglong)dVar10 >> 0x20) * fVar3,SUB84(dVar10,0));
  }
  else {
    dVar10 = dVar10 * dVar5;
  }
  dVar15 = in_xd6;
  if (in_FPSCR_SZ == '\0') {
    dVar10 = (double)CONCAT44((int)((ulonglong)dVar10 >> 0x20),SUB84(dVar6,0));
    dVar15 = dVar14;
  }
  if (in_FPSCR_PR == '\0') {
    dVar10 = (double)CONCAT44((int)((ulonglong)dVar10 >> 0x20),SUB84(dVar10,0) * fVar9);
  }
  else {
    dVar10 = dVar10 * dVar5;
  }
  if (in_FPSCR_SZ == '\0') {
    dVar4 = (double)CONCAT44((int)((ulonglong)dVar4 >> 0x20),uVar13);
  }
  if (in_FPSCR_PR == '\0') {
    fVar3 = SUB84(dVar10,0) - (float)((ulonglong)dVar10 >> 0x20);
  }
  else {
    fVar3 = SUB84(dVar10 - dVar10,0);
  }
  if (in_FPSCR_SZ == '\0') {
    dVar5 = (double)CONCAT44(*(undefined4 *)((int)param_3 + 0x14),fVar3);
  }
  else {
    dVar5 = *(double *)((int)param_3 + 0x14);
  }
  if (in_FPSCR_PR == '\0') {
    fVar3 = SUB84(dVar4,0) * (float)((ulonglong)dVar5 >> 0x20);
  }
  else {
    fVar3 = SUB84(dVar4 * dVar5,0);
  }
  if (in_FPSCR_SZ == '\0') {
    dVar5 = (double)CONCAT44(*(undefined4 *)((int)param_2 + 0x14),SUB84(dVar5,0));
  }
  else {
    dVar5 = *(double *)((int)param_2 + 0x14);
  }
LAB_8c640346:
  if (in_FPSCR_SZ == '\0') {
    in_xd6 = (double)CONCAT44(SUB84(dVar6,0),fVar3);
  }
  if (in_FPSCR_PR == '\0') {
    fVar3 = SUB84(dVar5,0) /
            (SUB84(in_xd6,0) -
            (float)((ulonglong)in_xd6 >> 0x20) * (float)((ulonglong)dVar5 >> 0x20));
  }
  else {
    fVar3 = SUB84(dVar5 / (in_xd6 * dVar5 - in_xd6 * dVar5),0);
  }
  if (in_FPSCR_SZ == '\0') {
    *(float *)(param_4 + 1) = fVar3;
  }
  else {
    param_4[1] = dVar15;
  }
  return;
}

