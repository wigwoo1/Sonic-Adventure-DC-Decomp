/* Ghidra draft only; not buildable/verified source. */

/* WARNING: Removing unreachable block (ram,0x8c074e62) */
/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_8c074cf0(float param_1,int param_2,int param_3,undefined4 param_4,undefined8 *param_5)

{
  undefined4 uVar1;
  undefined1 *puVar2;
  float fVar3;
  undefined8 uVar4;
  float fVar6;
  float fVar7;
  double in_dr2;
  double dVar8;
  undefined4 uVar9;
  float fVar10;
  double in_dr14;
  double dVar11;
  undefined8 in_xd2;
  undefined8 in_xd12;
  undefined8 in_xd14;
  bool bVar12;
  undefined4 in_PR;
  char in_FPSCR_PR;
  char in_FPSCR_SZ;
  undefined1 local_24 [12];
  undefined1 local_18 [4];
  undefined8 uStack_14;
  float fVar5;
  
  uVar1 = (undefined4)((ulonglong)in_dr14 >> 0x20);
  if (in_FPSCR_SZ == '\0') {
    uStack_14 = CONCAT44(SUB84(in_dr14,0),(undefined4)uStack_14);
                    /* WARNING: Ignoring partial resolution of indirect */
    uStack_14._0_4_ = uVar1;
    puVar2 = local_18;
  }
  else {
    puVar2 = local_24;
    uStack_14 = in_xd14;
  }
  *(undefined4 *)(puVar2 + -4) = in_PR;
  uVar9 = 0;
  *(undefined4 *)(puVar2 + -0x14) = param_4;
  if (in_FPSCR_SZ == '\0') {
    in_dr14 = (double)CONCAT44(uVar1,*(undefined4 *)(param_3 + 0xa0));
    *(undefined4 *)(param_5 + 1) = 0;
    *(undefined4 *)param_5 = 0;
  }
  else {
    param_5[1] = in_xd12;
    *param_5 = in_xd12;
  }
  fVar7 = SUB84(in_dr14,0);
  if ((*(uint *)(param_3 + 0x60) & 0x4000) != 0) {
    if (in_FPSCR_SZ == '\0') {
      in_dr2 = (double)((ulonglong)*(uint *)(param_2 + 8) << 0x20);
    }
    else {
      in_dr2 = *(double *)(param_2 + 8);
    }
    fVar3 = (float)((ulonglong)in_dr2 >> 0x20);
    if (in_FPSCR_PR != '\0' || fVar3 <= SUB84(in_dr2,0)) {
      dVar8 = (double)CONCAT44(fVar3,DAT_8c074de4);
      goto LAB_8c074d50;
    }
  }
  if ((*(uint *)(param_3 + 0x60) & 0x4000) != 0) {
    if (in_FPSCR_SZ == '\0') {
      in_dr2 = (double)((ulonglong)*(uint *)(param_2 + 8) << 0x20);
    }
    else {
      in_dr2 = *(double *)(param_2 + 8);
    }
    fVar3 = (float)((ulonglong)in_dr2 >> 0x20);
    if (in_FPSCR_PR == '\0' && SUB84(in_dr2,0) < fVar3) {
      dVar8 = (double)CONCAT44(fVar3,DAT_8c074de8);
LAB_8c074d50:
      if (in_FPSCR_PR == '\0') {
        dVar11 = (double)CONCAT44((int)((ulonglong)in_dr14 >> 0x20),fVar7 * SUB84(dVar8,0));
      }
      else {
        dVar11 = in_dr14 * dVar8;
      }
      if (in_FPSCR_SZ == '\0') {
        dVar8 = (double)CONCAT44(*(undefined4 *)((int)param_5 + 4),SUB84(dVar8,0));
      }
      else {
        dVar8 = *(double *)((int)param_5 + 4);
      }
      if (in_FPSCR_PR == '\0') {
        dVar8 = (double)CONCAT44((float)((ulonglong)dVar8 >> 0x20) - SUB84(dVar11,0),SUB84(dVar8,0))
        ;
      }
      else {
        dVar8 = dVar8 - dVar11;
      }
      if (in_FPSCR_SZ == '\0') {
        *(int *)((int)param_5 + 4) = (int)((ulonglong)dVar8 >> 0x20);
        return;
      }
      *(double *)((int)param_5 + 4) = dVar8;
      return;
    }
  }
  if ((*(uint *)(param_3 + 0x60) & DAT_8c074dec) != 0) goto LAB_8c074eb6;
  if (in_FPSCR_SZ == '\0') {
    in_dr2 = (double)((ulonglong)*(uint *)(param_3 + 0x3c) << 0x20);
  }
  else {
    in_dr2 = *(double *)(param_3 + 0x3c);
  }
  if (in_FPSCR_PR == '\0' && SUB84(in_dr2,0) < (float)((ulonglong)in_dr2 >> 0x20)) {
    if ((*(ushort *)(param_3 + 0x10) & 0x2000) == 0) {
      return;
    }
    if (in_FPSCR_SZ == '\0') {
      dVar8 = (double)CONCAT44(*(undefined4 *)((int)param_5 + 4),SUB84(in_dr2,0));
    }
    else {
      dVar8 = *(double *)((int)param_5 + 4);
    }
    if (in_FPSCR_PR == '\0') {
      dVar8 = (double)CONCAT44((float)((ulonglong)dVar8 >> 0x20) - fVar7,SUB84(dVar8,0));
    }
    else {
      dVar8 = dVar8 - in_dr14;
    }
    if (in_FPSCR_SZ == '\0') {
      *(int *)((int)param_5 + 4) = (int)((ulonglong)dVar8 >> 0x20);
      return;
    }
    *(double *)((int)param_5 + 4) = dVar8;
    return;
  }
  if (in_FPSCR_SZ == '\0') {
    in_dr14 = (double)CONCAT44((undefined4)DAT_8c074df0,fVar7);
  }
  else {
    in_dr14 = (double)CONCAT44(DAT_8c074df0._4_4_,(undefined4)DAT_8c074df0);
  }
  if ((*(uint *)(param_3 + 0x60) & 8) != 0) {
    if (in_FPSCR_PR == '\0') {
      fVar7 = (float)((ulonglong)in_dr14 >> 0x20);
      in_dr14 = (double)CONCAT44(fVar7,SUB84(in_dr14,0) * fVar7);
    }
    else {
      in_dr14 = in_dr14 * in_dr14;
    }
    goto LAB_8c074eb6;
  }
  uVar4 = (*(code *)PTR_FUN_8c074df8)(DAT_8c074df0._4_4_,*(int *)(puVar2 + -0x14) + 0x1c);
  fVar5 = (float)((ulonglong)uVar4 >> 0x20);
  fVar3 = (float)uVar4;
  fVar7 = fVar3;
  if (in_FPSCR_SZ == '\0') {
    fVar7 = param_1;
  }
  if (in_FPSCR_PR == '\0') {
    dVar8 = (double)CONCAT44(-fVar5,fVar7);
  }
  else {
    dVar8 = -(double)CONCAT44(fVar5,fVar7);
  }
  fVar7 = (float)((ulonglong)dVar8 >> 0x20);
  fVar10 = SUB84(in_dr14,0);
  uVar1 = (undefined4)((ulonglong)in_dr14 >> 0x20);
  if ((*(ushort *)(param_3 + 0x10) & 0x2000) != 0) {
    if (in_FPSCR_SZ == '\0') {
      in_dr2 = (double)CONCAT44((int)((ulonglong)in_dr2 >> 0x20),DAT_8c074dfc);
    }
    else {
      in_xd2 = CONCAT44(DAT_8c074e00,DAT_8c074dfc);
    }
    fVar6 = SUB84(in_dr2,0);
    if (in_FPSCR_PR == '\0') {
      bVar12 = fVar6 < fVar7;
    }
    else {
      bVar12 = in_dr2 < dVar8;
    }
    if (bVar12) {
      if (in_FPSCR_SZ == '\0') {
        in_dr2 = (double)CONCAT44(_DAT_8c074e04,fVar6);
      }
      else {
        in_dr2 = (double)CONCAT44(DAT_8c074e08,_DAT_8c074e04);
      }
      if (in_FPSCR_PR == '\0') {
        bVar12 = (float)((ulonglong)in_dr2 >> 0x20) < fVar7;
      }
      else {
        bVar12 = in_dr2 < dVar8;
      }
      if (bVar12) {
        if (in_FPSCR_SZ == '\0') {
          fVar3 = _DAT_8c074e0c;
        }
      }
      else if (in_FPSCR_SZ == '\0') {
        fVar3 = DAT_8c074e08;
      }
      if (in_FPSCR_PR == '\0') {
        in_dr14 = (double)CONCAT44(uVar1,fVar10 * fVar3);
      }
      else {
        in_dr14 = in_dr14 * (double)CONCAT44(fVar5,fVar3);
      }
    }
    else {
      if (in_FPSCR_SZ == '\0') {
        in_dr2 = (double)CONCAT44(DAT_8c074e00,fVar6);
      }
      else {
        in_dr2 = (double)CONCAT44(_DAT_8c074e04,DAT_8c074e00);
      }
      if (in_FPSCR_PR == '\0') {
        in_dr14 = (double)CONCAT44(uVar1,fVar10 * (float)((ulonglong)in_dr2 >> 0x20));
      }
      else {
        in_dr14 = in_dr14 * in_dr2;
      }
    }
    goto LAB_8c074eb6;
  }
  if (in_FPSCR_SZ == '\0') {
    in_dr2 = (double)CONCAT44(*(undefined4 *)(param_3 + 0x38),*(undefined4 *)(param_3 + 0xd0));
  }
  else {
    in_xd2 = *(undefined8 *)(param_3 + 0xd0);
    in_dr2 = *(double *)(param_3 + 0x38);
  }
  fVar3 = (float)((ulonglong)in_dr2 >> 0x20);
  if ((in_FPSCR_PR != '\0' || SUB84(in_dr2,0) <= fVar3) && ((*(ushort *)(param_3 + 0x10) & 3) != 0))
  goto LAB_8c074eb6;
  if (in_FPSCR_SZ == '\0') {
    dVar8 = (double)CONCAT44(fVar7,DAT_8c074f40);
  }
  fVar7 = (float)((ulonglong)dVar8 >> 0x20);
  if (in_FPSCR_PR != '\0' || fVar7 <= SUB84(dVar8,0)) goto LAB_8c074eb6;
  if (in_FPSCR_PR != '\0' || fVar7 <= SUB84(dVar8,0)) {
LAB_8c074e8c:
    if (in_FPSCR_SZ == '\0') {
      in_dr2 = (double)CONCAT44(*(undefined4 *)(param_3 + 0x38),*(undefined4 *)(param_3 + 0xc4));
    }
    else {
      in_xd2 = *(undefined8 *)(param_3 + 0xc4);
      in_dr2 = *(double *)(param_3 + 0x38);
    }
    fVar7 = (float)((ulonglong)in_dr2 >> 0x20);
    if (in_FPSCR_PR == '\0' && SUB84(in_dr2,0) < fVar7) {
      if ((*(ushort *)(param_3 + 0x10) & 3) != 0) {
        if (in_FPSCR_SZ == '\0') {
          dVar8 = (double)CONCAT44(*(undefined4 *)(param_3 + 0x38),*(undefined4 *)(param_3 + 0xd0));
        }
        else {
          in_xd2 = *(undefined8 *)(param_3 + 0xd0);
          dVar8 = *(double *)(param_3 + 0x38);
        }
        if (in_FPSCR_PR == '\0') {
          fVar7 = (float)((ulonglong)dVar8 >> 0x20) / SUB84(dVar8,0);
          in_dr2 = (double)CONCAT44(fVar7,SUB84(dVar8,0));
          in_dr14 = (double)CONCAT44(uVar1,fVar10 * fVar7);
        }
        else {
          in_dr2 = dVar8 / dVar8;
          in_dr14 = in_dr14 * in_dr2;
        }
      }
      goto LAB_8c074eb6;
    }
    if (in_FPSCR_SZ == '\0') {
      in_dr2 = (double)CONCAT44(fVar7,DAT_8c074f58);
    }
    else {
      in_xd2 = CONCAT44(DAT_8c074f5c,DAT_8c074f58);
    }
  }
  else {
    if (in_FPSCR_SZ == '\0') {
      in_dr2 = (double)CONCAT44(fVar3,DAT_8c074f44);
    }
    else {
      in_xd2 = CONCAT44(PTR_FUN_8c074f48,DAT_8c074f44);
    }
    if (in_FPSCR_PR == '\0') {
      bVar12 = fVar7 < SUB84(in_dr2,0);
    }
    else {
      bVar12 = dVar8 < in_dr2;
    }
    if (!bVar12) goto LAB_8c074e8c;
    uVar1 = *(undefined4 *)(param_2 + 8);
    *(undefined4 *)(puVar2 + -0x10) = *(undefined4 *)(param_2 + 4);
    *(undefined4 *)(puVar2 + -0xc) = uVar1;
    *(undefined4 *)(puVar2 + -8) = *(undefined4 *)(param_2 + 0xc);
    fVar7 = (float)(*(code *)PTR_FUN_8c074f48)(puVar2 + -0x10);
    if (fVar7 == 0.0) {
      if (in_FPSCR_SZ == '\0') {
        *(undefined4 *)(puVar2 + -0xc) = uVar9;
      }
      else {
        *(undefined8 *)(puVar2 + -0xc) = in_xd12;
      }
    }
    else {
      (*(code *)PTR_FUN_8c074f4c)(puVar2 + -0x10);
    }
    if (in_FPSCR_SZ == '\0') {
      dVar8 = (double)CONCAT44(DAT_8c074f50,*(undefined4 *)(puVar2 + -0xc));
    }
    else {
      in_xd2 = *(undefined8 *)(puVar2 + -0xc);
      dVar8 = (double)CONCAT44(DAT_8c074f54,DAT_8c074f50);
    }
    if (in_FPSCR_PR == '\0') {
      in_dr2 = (double)CONCAT44((int)((ulonglong)dVar8 >> 0x20),ABS(SUB84(dVar8,0)));
    }
    else {
      in_dr2 = ABS(dVar8);
    }
    fVar7 = (float)((ulonglong)in_dr2 >> 0x20);
    if (in_FPSCR_PR != '\0' || fVar7 <= SUB84(in_dr2,0)) {
      if (in_FPSCR_PR == '\0') {
        fVar7 = (float)((ulonglong)in_dr14 >> 0x20);
        in_dr14 = (double)CONCAT44(fVar7,SUB84(in_dr14,0) * fVar7);
      }
      else {
        in_dr14 = in_dr14 * in_dr14;
      }
      goto LAB_8c074eb6;
    }
    if (in_FPSCR_SZ == '\0') {
      in_dr2 = (double)CONCAT44(fVar7,DAT_8c074f54);
    }
    else {
      in_xd2 = CONCAT44(DAT_8c074f58,DAT_8c074f54);
    }
  }
  if (in_FPSCR_PR == '\0') {
    in_dr14 = (double)CONCAT44((int)((ulonglong)in_dr14 >> 0x20),SUB84(in_dr14,0) * SUB84(in_dr2,0))
    ;
  }
  else {
    in_dr14 = in_dr14 * in_dr2;
  }
LAB_8c074eb6:
  if (in_FPSCR_SZ == '\0') {
    in_dr2 = (double)CONCAT44((int)((ulonglong)in_dr2 >> 0x20),*(undefined4 *)((int)param_5 + 4));
  }
  else {
    in_xd2 = *(undefined8 *)((int)param_5 + 4);
  }
  if (in_FPSCR_PR == '\0') {
    fVar7 = SUB84(in_dr2,0) - SUB84(in_dr14,0);
  }
  else {
    fVar7 = SUB84(in_dr2 - in_dr14,0);
  }
  if (in_FPSCR_SZ == '\0') {
    *(float *)((int)param_5 + 4) = fVar7;
  }
  else {
    *(undefined8 *)((int)param_5 + 4) = in_xd2;
  }
  return;
}

