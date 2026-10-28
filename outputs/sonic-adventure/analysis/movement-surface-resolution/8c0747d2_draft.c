/* Ghidra draft only; not buildable/verified source. */

/* WARNING: Removing unreachable block (ram,0x8c07498e) */
/* WARNING: Removing unreachable block (ram,0x8c07494c) */
/* WARNING: Removing unreachable block (ram,0x8c074976) */
/* WARNING: Removing unreachable block (ram,0x8c07496c) */
/* WARNING: Removing unreachable block (ram,0x8c074980) */
/* WARNING: Removing unreachable block (ram,0x8c074860) */
/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined4 FUN_8c0747d2(undefined4 param_1,int param_2,int param_3,int param_4,int param_5)

{
  undefined *puVar1;
  undefined4 uVar2;
  int iVar3;
  double *pdVar4;
  undefined4 *puVar5;
  undefined4 uVar6;
  undefined1 *puVar7;
  float fVar9;
  undefined8 uVar8;
  undefined8 in_dr2;
  double dVar10;
  uint uVar12;
  double dVar11;
  float in_fr13;
  undefined4 in_fr12;
  undefined8 in_dr14;
  float fVar13;
  undefined8 in_xd2;
  undefined8 in_xd14;
  bool bVar14;
  undefined4 in_PR;
  char in_FPSCR_PR;
  char in_FPSCR_SZ;
  undefined1 auStack_34 [12];
  undefined1 local_28 [4];
  undefined8 uStack_24;
  
  if (in_FPSCR_SZ == '\0') {
    uStack_24 = CONCAT44((int)in_dr14,(undefined4)uStack_24);
                    /* WARNING: Ignoring partial resolution of indirect */
    uStack_24._0_4_ = (int)((ulonglong)in_dr14 >> 0x20);
    puVar7 = local_28;
  }
  else {
    puVar7 = auStack_34;
    uStack_24 = in_xd14;
  }
  *(undefined4 *)(puVar7 + -4) = in_PR;
  pdVar4 = *(double **)(puVar7 + 0x28);
  uVar6 = *(undefined4 *)(puVar7 + 0x30);
  uVar2 = *(undefined4 *)(param_5 + 0x4c);
  *(undefined4 *)(param_4 + 0x44) = *(undefined4 *)(param_5 + 0x48);
  *(undefined4 *)(param_4 + 0x48) = uVar2;
  uVar12 = DAT_8c0748f0._4_4_;
  *(undefined4 *)(param_4 + 0x4c) = *(undefined4 *)(param_5 + 0x50);
  if (in_FPSCR_SZ == '\0') {
    in_fr13 = (float)DAT_8c0748f0;
  }
  dVar11 = 0.0;
  *(uint *)(puVar7 + -0x14) = *(uint *)(puVar7 + 0x34) & uVar12;
  if ((*(ushort *)(param_2 + 4) & 1) == 0) {
    if (in_FPSCR_SZ == '\0') {
      dVar11 = (double)((ulonglong)_DAT_8c074a44 << 0x20);
    }
    else {
      dVar11 = (double)CONCAT44(PTR_DAT_8c074a48,_DAT_8c074a44);
    }
    uVar12 = (uint)((ulonglong)in_dr2 >> 0x20);
    if (*(int *)PTR_DAT_8c074a48 == 0) {
      if (in_FPSCR_SZ == '\0') {
        in_dr2 = CONCAT44(uVar12,*(undefined4 *)(param_3 + 4));
      }
      else {
        in_xd2 = *(undefined8 *)(param_3 + 4);
      }
      uVar12 = (uint)((ulonglong)in_dr2 >> 0x20);
      fVar9 = -(float)in_dr2;
      if (in_FPSCR_SZ == '\0') {
        *(float *)(puVar7 + -0x10) = fVar9;
        fVar9 = *(float *)(param_3 + 8);
      }
      else {
        *(undefined8 *)(puVar7 + -0x10) = in_xd2;
        in_xd2 = *(undefined8 *)(param_3 + 8);
      }
      fVar9 = -fVar9;
      if (in_FPSCR_SZ == '\0') {
        *(float *)(puVar7 + -0xc) = fVar9;
        fVar9 = *(float *)(param_3 + 0xc);
      }
      else {
        *(undefined8 *)(puVar7 + -0xc) = in_xd2;
        in_xd2 = *(undefined8 *)(param_3 + 0xc);
      }
      if (in_FPSCR_SZ == '\0') {
        *(float *)(puVar7 + -8) = -fVar9;
      }
      else {
        *(undefined8 *)(puVar7 + -8) = in_xd2;
      }
    }
    else {
      if (in_FPSCR_SZ == '\0') {
        *(undefined4 *)(puVar7 + -0x10) = 0xbf800000;
        *(int *)(puVar7 + -0xc) = SUB84(dVar11,0);
        *(int *)(puVar7 + -8) = SUB84(dVar11,0);
      }
      else {
        *(undefined8 *)(puVar7 + -0x10) = in_xd2;
        *(undefined8 *)(puVar7 + -0xc) = in_xd14;
        *(undefined8 *)(puVar7 + -8) = in_xd14;
      }
      (*(code *)PTR_FUN_8c074a4c)(param_2,puVar7 + -0x10);
    }
    fVar9 = (float)(*(code *)PTR_FUN_8c074a50)(puVar7 + -0x10);
    dVar10 = (double)((ulonglong)uVar12 << 0x20);
    if (fVar9 == 0.0) goto LAB_8c074a94;
    (*(code *)PTR_FUN_8c074a54)(puVar7 + -0x10);
    uVar2 = (*(code *)PTR_FUN_8c074a58)(puVar7 + -0x10,param_5 + 0x48,0);
    *(undefined4 *)(puVar7 + -0x18) = uVar2;
    uVar8 = (*(code *)PTR_FUN_8c074a5c)(puVar7 + -0x10,param_5 + 0x48);
    fVar9 = (float)((ulonglong)uVar8 >> 0x20);
    uVar2 = (undefined4)uVar8;
    if (in_FPSCR_SZ == '\0') {
      uVar2 = param_1;
    }
    if (*(int *)(puVar7 + -0x14) != 0) {
      if (0x4000 < *(int *)(puVar7 + -0x18)) {
        return uVar6;
      }
      uVar6 = 1;
      (*(code *)PTR_FUN_8c074a40)(param_2,param_4,param_5,puVar7 + -0x10);
      goto LAB_8c074a94;
    }
    if (in_FPSCR_SZ == '\0') {
      dVar10 = (double)CONCAT44((int)((ulonglong)dVar10 >> 0x20),*(undefined4 *)(param_4 + 0x38));
    }
    if (in_FPSCR_PR == '\0') {
      bVar14 = SUB84(dVar10,0) < in_fr13;
    }
    else {
      bVar14 = dVar10 < (double)CONCAT44(in_fr12,in_fr13);
    }
    fVar13 = (float)((ulonglong)dVar11 >> 0x20);
    if (bVar14) {
      if (in_FPSCR_PR == '\0') {
        bVar14 = fVar9 < fVar13;
      }
      else {
        bVar14 = (double)CONCAT44(fVar9,uVar2) < dVar11;
      }
      if (!bVar14) {
        if (in_FPSCR_SZ == '\0') {
          uVar8 = CONCAT44(*(undefined4 *)(param_4 + 0x38),*(undefined4 *)(param_4 + 0xcc));
        }
        else {
          uVar8 = *(undefined8 *)(param_4 + 0x38);
        }
        if (in_FPSCR_PR == '\0' && (float)((ulonglong)uVar8 >> 0x20) < (float)uVar8) {
          uVar6 = 1;
          goto LAB_8c074a94;
        }
        goto LAB_8c074a18;
      }
    }
    else {
      if (in_FPSCR_PR == '\0') {
        bVar14 = fVar9 < fVar13;
      }
      else {
        bVar14 = (double)CONCAT44(fVar9,uVar2) < dVar11;
      }
      if (!bVar14) goto LAB_8c0749ec;
    }
    if (((*(char *)(param_2 + 9) == '\x03') && (*(int *)(puVar7 + -0x18) < 0x3401)) &&
       ((*(uint *)(puVar7 + 0x34) & DAT_8c074a64) == 0)) {
      uVar6 = 1;
    }
    else {
      if (0x3fff < *(int *)(puVar7 + -0x18)) {
        return uVar6;
      }
      (*(code *)PTR_FUN_8c074c60)(param_2,param_4,param_5,puVar7 + -0x10);
    }
  }
  else {
    if (in_FPSCR_SZ == '\0') {
      dVar10 = (double)CONCAT44(*(undefined4 *)pdVar4,*(undefined4 *)(param_2 + 0x20));
    }
    else {
      dVar10 = *pdVar4;
    }
    if (in_FPSCR_PR == '\0') {
      dVar10 = (double)CONCAT44((float)((ulonglong)dVar10 >> 0x20) - SUB84(dVar10,0),SUB84(dVar10,0)
                               );
    }
    else {
      dVar10 = dVar10 - dVar10;
    }
    if (in_FPSCR_SZ == '\0') {
      *(int *)(puVar7 + -0x10) = (int)((ulonglong)dVar10 >> 0x20);
      dVar10 = (double)CONCAT44(*(undefined4 *)((int)pdVar4 + 4),*(undefined4 *)(param_2 + 0x24));
    }
    else {
      *(double *)(puVar7 + -0x10) = dVar10;
      dVar10 = *(double *)((int)pdVar4 + 4);
    }
    if (in_FPSCR_PR == '\0') {
      dVar10 = (double)CONCAT44((float)((ulonglong)dVar10 >> 0x20) - SUB84(dVar10,0),SUB84(dVar10,0)
                               );
    }
    else {
      dVar10 = dVar10 - dVar10;
    }
    if (in_FPSCR_SZ == '\0') {
      *(int *)(puVar7 + -0xc) = (int)((ulonglong)dVar10 >> 0x20);
      dVar10 = (double)CONCAT44(*(undefined4 *)(pdVar4 + 1),*(undefined4 *)(param_2 + 0x28));
    }
    else {
      *(double *)(puVar7 + -0xc) = dVar10;
      dVar10 = pdVar4[1];
    }
    if (in_FPSCR_PR == '\0') {
      dVar10 = (double)CONCAT44((float)((ulonglong)dVar10 >> 0x20) - SUB84(dVar10,0),SUB84(dVar10,0)
                               );
    }
    else {
      dVar10 = dVar10 - dVar10;
    }
    if (in_FPSCR_SZ == '\0') {
      *(int *)(puVar7 + -8) = (int)((ulonglong)dVar10 >> 0x20);
    }
    else {
      *(double *)(puVar7 + -8) = dVar10;
    }
    fVar9 = (float)(*(code *)PTR_FUN_8c0748f8)(puVar7 + -0x10);
    dVar10 = (double)((ulonglong)dVar10 & 0xffffffff00000000);
    if (fVar9 == 0.0) goto LAB_8c074a94;
    (*(code *)PTR_FUN_8c0748fc)(puVar7 + -0x10);
    iVar3 = (*(code *)PTR_FUN_8c0748e0)(puVar7 + -0x10,param_5 + 0x48,0);
    if (*(int *)(puVar7 + -0x14) != 0) {
      if (0x4000 < iVar3) {
        return uVar6;
      }
      uVar6 = 1;
      (*(code *)PTR_FUN_8c074900)(param_2,param_4,param_5,puVar7 + -0x10);
      goto LAB_8c074a94;
    }
    if (in_FPSCR_SZ == '\0') {
      dVar10 = (double)CONCAT44((int)((ulonglong)dVar10 >> 0x20),*(undefined4 *)(param_4 + 0x38));
    }
    if (in_FPSCR_PR == '\0') {
      bVar14 = SUB84(dVar10,0) < in_fr13;
    }
    else {
      bVar14 = dVar10 < (double)CONCAT44(in_fr12,in_fr13);
    }
    if (bVar14) {
      if (0x1000 < iVar3) {
        if (iVar3 < 0x4001) {
          uVar6 = 1;
          (*(code *)PTR_FUN_8c074a40)(param_2,param_4,param_5,puVar7 + -0x10);
        }
        goto LAB_8c074a94;
      }
      if (in_FPSCR_SZ == '\0') {
        uVar8 = CONCAT44(*(undefined4 *)(param_4 + 0x38),*(undefined4 *)(param_4 + 0xcc));
      }
      else {
        uVar8 = *(undefined8 *)(param_4 + 0x38);
      }
      if (in_FPSCR_PR == '\0' && (float)((ulonglong)uVar8 >> 0x20) < (float)uVar8) {
        uVar6 = 1;
        (*(code *)PTR_FUN_8c074900)(param_2,param_4,param_5,puVar7 + -0x10);
        goto LAB_8c074a94;
      }
LAB_8c074a18:
      uVar6 = 2;
      goto LAB_8c074a94;
    }
LAB_8c0749ec:
    puVar1 = PTR_FUN_8c074a60;
    uVar2 = SUB84(dVar11,0);
    if (in_FPSCR_SZ == '\0') {
      *(undefined4 *)(param_5 + 0x74) = uVar2;
      *(undefined4 *)(param_5 + 0x70) = uVar2;
    }
    else {
      *(undefined8 *)(param_5 + 0x74) = in_xd14;
      *(undefined8 *)(param_5 + 0x70) = in_xd14;
    }
    uVar6 = 2;
    if (in_FPSCR_SZ == '\0') {
      *(undefined4 *)(param_5 + 0x6c) = uVar2;
    }
    else {
      *(undefined8 *)(param_5 + 0x6c) = in_xd14;
    }
    (*(code *)puVar1)(0,param_4);
  }
LAB_8c074a94:
  *(undefined4 *)(param_5 + 4) = *(undefined4 *)(param_5 + 0x60);
  *(undefined4 *)(param_5 + 8) = *(undefined4 *)(param_5 + 100);
  *(undefined4 *)(param_5 + 0xc) = *(undefined4 *)(param_5 + 0x68);
  puVar5 = *(undefined4 **)(puVar7 + 0x2c);
  if (in_FPSCR_SZ == '\0') {
    dVar11 = (double)CONCAT44(*(undefined4 *)(param_5 + 0x60),*puVar5);
  }
  else {
    dVar11 = *(double *)(param_5 + 0x60);
  }
  if (in_FPSCR_PR == '\0') {
    dVar11 = (double)CONCAT44((float)((ulonglong)dVar11 >> 0x20) - SUB84(dVar11,0),SUB84(dVar11,0));
  }
  else {
    dVar11 = dVar11 - dVar11;
  }
  if (in_FPSCR_SZ == '\0') {
    *(int *)(param_2 + 0x20) = (int)((ulonglong)dVar11 >> 0x20);
    dVar11 = (double)CONCAT44(*(undefined4 *)(param_5 + 100),puVar5[1]);
  }
  else {
    *(double *)(param_2 + 0x20) = dVar11;
    dVar11 = *(double *)(param_5 + 100);
  }
  if (in_FPSCR_PR == '\0') {
    dVar11 = (double)CONCAT44((float)((ulonglong)dVar11 >> 0x20) - SUB84(dVar11,0),SUB84(dVar11,0));
  }
  else {
    dVar11 = dVar11 - dVar11;
  }
  if (in_FPSCR_SZ == '\0') {
    *(int *)(param_2 + 0x24) = (int)((ulonglong)dVar11 >> 0x20);
    dVar11 = (double)CONCAT44(*(undefined4 *)(param_5 + 0x68),puVar5[2]);
  }
  else {
    *(double *)(param_2 + 0x24) = dVar11;
    dVar11 = *(double *)(param_5 + 0x68);
  }
  if (in_FPSCR_PR == '\0') {
    dVar11 = (double)CONCAT44((float)((ulonglong)dVar11 >> 0x20) - SUB84(dVar11,0),SUB84(dVar11,0));
  }
  else {
    dVar11 = dVar11 - dVar11;
  }
  if (in_FPSCR_SZ == '\0') {
    *(int *)(param_2 + 0x28) = (int)((ulonglong)dVar11 >> 0x20);
  }
  else {
    *(double *)(param_2 + 0x28) = dVar11;
  }
  *(undefined4 *)(param_5 + 0x10) = *(undefined4 *)(param_5 + 0x6c);
  *(undefined4 *)(param_5 + 0x14) = *(undefined4 *)(param_5 + 0x70);
  *(undefined4 *)(param_5 + 0x18) = *(undefined4 *)(param_5 + 0x74);
  return uVar6;
}

