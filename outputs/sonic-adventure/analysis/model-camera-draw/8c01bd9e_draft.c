/* Ghidra draft only; not buildable/verified source. */

/* WARNING: Removing unreachable block (ram,0x8c01c0b8) */
/* WARNING: Removing unreachable block (ram,0x8c01c03a) */
/* WARNING: Removing unreachable block (ram,0x8c01be78) */
/* WARNING: Removing unreachable block (ram,0x8c01c048) */
/* WARNING: Removing unreachable block (ram,0x8c01c0f4) */
/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_8c01bd9e(void)

{
  undefined4 uVar1;
  int iVar2;
  uint *puVar3;
  undefined8 *puVar4;
  short sVar5;
  uint uVar6;
  uint *puVar7;
  uint *puVar8;
  int iVar9;
  uint uVar10;
  undefined1 *puVar11;
  undefined4 uVar12;
  float fVar13;
  undefined8 in_dr2;
  double dVar14;
  longlong lVar15;
  uint uVar16;
  undefined4 uVar17;
  double dVar18;
  float fVar20;
  undefined8 uVar19;
  undefined8 uVar21;
  float in_fr13;
  undefined8 in_dr14;
  undefined8 in_xd2;
  undefined8 in_xd12;
  undefined8 in_xd14;
  bool bVar22;
  undefined4 in_PR;
  char in_FPSCR_PR;
  char in_FPSCR_SZ;
  undefined1 auStack_34 [12];
  undefined1 local_28 [4];
  undefined8 uStack_24;
  
  uVar17 = (undefined4)((ulonglong)in_dr2 >> 0x20);
  if (in_FPSCR_SZ == '\0') {
    uStack_24 = CONCAT44((int)in_dr14,(undefined4)uStack_24);
                    /* WARNING: Ignoring partial resolution of indirect */
    uStack_24._0_4_ = (int)((ulonglong)in_dr14 >> 0x20);
    puVar11 = local_28;
  }
  else {
    puVar11 = auStack_34;
    uStack_24 = in_xd14;
  }
  *(undefined4 *)(puVar11 + -4) = in_PR;
  iVar9 = *(int *)PTR_DAT_8c01bef0;
  if (*DAT_8c01bef4 != 0) {
    return;
  }
  *(undefined4 *)(puVar11 + -0xf8) = *DAT_8c01bef8;
  iVar2 = (*(code *)PTR_FUN_8c01befc)();
  if (iVar2 == 0) {
    *DAT_8c01bef8 = 0;
  }
  if (*(int *)(puVar11 + -0xf8) == 1) {
    return;
  }
  uVar12 = 0;
  if (in_FPSCR_SZ == '\0') {
    *(undefined4 *)(puVar11 + -0x74) = 0;
    *(undefined4 *)(puVar11 + -0x70) = 0;
    *(undefined4 *)(puVar11 + -0x6c) = DAT_8c01bf00;
  }
  else {
    *(undefined8 *)(puVar11 + -0x74) = in_xd14;
    *(undefined8 *)(puVar11 + -0x70) = in_xd14;
    in_xd2 = CONCAT44(PTR_DAT_8c01bf04,DAT_8c01bf00);
    *(undefined8 *)(puVar11 + -0x6c) = in_xd2;
  }
  (*(code *)PTR_FUN_8c01bf08)(PTR_DAT_8c01bf04);
  (*(code *)PTR_FUN_8c01bf0c)(0,iVar9 + 0x20);
  iVar2 = *(int *)(iVar9 + 0x18);
  *(int *)(puVar11 + -0xf8) = iVar2;
  if (iVar2 != 0) {
    (*(code *)PTR_FUN_8c01bf10)(0,*(undefined4 *)(puVar11 + -0xf8));
  }
  iVar9 = *(int *)(iVar9 + 0x14);
  *(int *)(puVar11 + -0xf8) = iVar9;
  if (iVar9 != 0) {
    (*(code *)PTR_FUN_8c01bf14)(0,*(undefined4 *)(puVar11 + -0xf8));
  }
  (*(code *)PTR_FUN_8c01bee4)(0,puVar11 + -0x74,puVar11 + -0x74);
  if (in_FPSCR_SZ == '\0') {
    *(undefined4 *)(puVar11 + -0x94) = *(undefined4 *)(puVar11 + -0x74);
    *(undefined4 *)(puVar11 + -0x90) = *(undefined4 *)(puVar11 + -0x70);
    *(undefined4 *)(puVar11 + -0x8c) = *(undefined4 *)(puVar11 + -0x6c);
    *(undefined4 *)(puVar11 + -0x88) = uVar12;
    *(undefined4 *)(puVar11 + -0x84) = uVar12;
  }
  else {
    *(undefined8 *)(puVar11 + -0x94) = *(undefined8 *)(puVar11 + -0x74);
    *(undefined8 *)(puVar11 + -0x90) = *(undefined8 *)(puVar11 + -0x70);
    in_xd2 = *(undefined8 *)(puVar11 + -0x6c);
    *(undefined8 *)(puVar11 + -0x8c) = in_xd2;
    *(undefined8 *)(puVar11 + -0x88) = in_xd14;
    *(undefined8 *)(puVar11 + -0x84) = in_xd14;
  }
  dVar14 = (double)CONCAT44(uVar17,0xbf800000);
  if (in_FPSCR_SZ == '\0') {
    *(undefined4 *)(puVar11 + -0x80) = 0xbf800000;
  }
  else {
    *(undefined8 *)(puVar11 + -0x80) = in_xd2;
  }
  (*(code *)PTR_FUN_8c01bf18)(0,puVar11 + -0x88,puVar11 + -0x88);
  (*(code *)PTR_FUN_8c01bf1c)(1);
  *(undefined4 *)(puVar11 + -0x50) = *(undefined4 *)(puVar11 + -0x74);
  *(undefined4 *)(puVar11 + -0x4c) = *(undefined4 *)(puVar11 + -0x70);
  *(undefined4 *)(puVar11 + -0x48) = *(undefined4 *)(puVar11 + -0x6c);
  *(undefined4 *)(puVar11 + -0xf8) = DAT_8c01bf20;
  if (in_FPSCR_SZ == '\0') {
    uVar21 = CONCAT44(*(undefined4 *)(puVar11 + -0x6c),_DAT_8c01bf24);
    dVar18 = (double)(ulonglong)*(uint *)(puVar11 + -0x70);
  }
  else {
    uVar21 = *(undefined8 *)(puVar11 + -0x6c);
    dVar18 = *(double *)(puVar11 + -0x74);
  }
  (*(code *)PTR_FUN_8c01bf28)(uVar21);
  uVar16 = DAT_8c01bf34;
  fVar13 = DAT_8c01bf2c;
  if (in_FPSCR_SZ != '\0') {
    in_xd12 = CONCAT44(DAT_8c01bf30,DAT_8c01bf2c);
    fVar13 = in_fr13;
  }
  uVar6 = 0;
  if (in_FPSCR_SZ == '\0') {
    *(float *)(puVar11 + -0x58) = fVar13;
    uVar21 = CONCAT44(DAT_8c01bf30,uVar12);
    *(undefined4 *)(puVar11 + -100) = DAT_8c01bf30;
  }
  else {
    *(undefined8 *)(puVar11 + -0x58) = in_xd12;
    uVar21 = CONCAT44(DAT_8c01bf34,DAT_8c01bf30);
    *(undefined8 *)(puVar11 + -100) = uVar21;
  }
  puVar3 = *(uint **)(puVar11 + -0xf8);
  puVar7 = puVar3;
  puVar8 = puVar3;
  while( true ) {
    uVar17 = SUB84(dVar18,0);
    uVar12 = (undefined4)uVar21;
    if ((int)*DAT_8c01c128 <= (int)(uVar6 & 0xffff)) break;
    uVar10 = puVar8[1];
    if (puVar8[2] == 0) {
LAB_8c01bf9e:
      if (((*puVar3 & uVar16) != 0) &&
         (iVar9 = (*(code *)PTR_FUN_8c01c124)(puVar11 + -0x50,puVar7[1]), iVar9 != 0)) {
        if (*(int *)(puVar11 + -0x24) == 1) {
          if (in_FPSCR_SZ == '\0') {
            dVar14 = (double)CONCAT44(*(undefined4 *)(puVar11 + -0x4c),
                                      *(undefined4 *)(puVar11 + -0x14));
          }
          else {
            dVar14 = *(double *)(puVar11 + -0x4c);
          }
          if (in_FPSCR_PR == '\0' && (float)((ulonglong)dVar14 >> 0x20) < SUB84(dVar14,0)) {
            if (in_FPSCR_SZ == '\0') {
              dVar14 = (double)CONCAT44(*(undefined4 *)(puVar11 + -0x14),
                                        *(undefined4 *)(puVar11 + -0x58));
            }
            else {
              dVar14 = *(double *)(puVar11 + -0x14);
            }
            fVar20 = (float)((ulonglong)dVar14 >> 0x20);
            if (in_FPSCR_PR == '\0' && fVar20 < SUB84(dVar14,0)) {
              if (in_FPSCR_SZ == '\0') {
                dVar14 = (double)CONCAT44(fVar20,*(undefined4 *)(puVar11 + -0x14));
                *(undefined4 *)(puVar11 + -0x58) = *(undefined4 *)(puVar11 + -0x14);
              }
              else {
                *(undefined8 *)(puVar11 + -0x58) = *(undefined8 *)(puVar11 + -0x14);
              }
              goto LAB_8c01c022;
            }
          }
        }
        if (*(int *)(puVar11 + -0x44) == 1) {
          if (in_FPSCR_SZ == '\0') {
            dVar14 = (double)CONCAT44(*(undefined4 *)(puVar11 + -0x4c),
                                      *(undefined4 *)(puVar11 + -0x34));
          }
          else {
            dVar14 = *(double *)(puVar11 + -0x4c);
          }
          if (in_FPSCR_PR == '\0' && SUB84(dVar14,0) < (float)((ulonglong)dVar14 >> 0x20)) {
            if (in_FPSCR_SZ == '\0') {
              dVar14 = (double)CONCAT44(*(undefined4 *)(puVar11 + -0x34),
                                        *(undefined4 *)(puVar11 + -100));
            }
            else {
              dVar14 = *(double *)(puVar11 + -0x34);
            }
            fVar20 = (float)((ulonglong)dVar14 >> 0x20);
            if (in_FPSCR_PR == '\0' && fVar20 < SUB84(dVar14,0)) {
              if (in_FPSCR_SZ == '\0') {
                dVar14 = (double)CONCAT44(fVar20,*(undefined4 *)(puVar11 + -0x34));
                *(undefined4 *)(puVar11 + -100) = *(undefined4 *)(puVar11 + -0x34);
              }
              else {
                *(undefined8 *)(puVar11 + -100) = *(undefined8 *)(puVar11 + -0x34);
              }
            }
          }
        }
      }
    }
    else {
      if (in_FPSCR_SZ == '\0') {
        dVar14 = (double)CONCAT44(*(undefined4 *)(uVar10 + 8),*(undefined4 *)(puVar11 + -0x74));
      }
      else {
        dVar14 = *(double *)(uVar10 + 8);
      }
      if (in_FPSCR_PR == '\0') {
        dVar14 = (double)CONCAT44((float)((ulonglong)dVar14 >> 0x20) - SUB84(dVar14,0),
                                  SUB84(dVar14,0));
      }
      else {
        dVar14 = dVar14 - dVar14;
      }
      if (in_FPSCR_SZ == '\0') {
        *(int *)(puVar11 + -0xd0) = (int)((ulonglong)dVar14 >> 0x20);
        *(undefined4 *)(puVar11 + -0xcc) = uVar12;
        dVar14 = (double)CONCAT44(*(undefined4 *)(uVar10 + 0x10),*(undefined4 *)(puVar11 + -0x6c));
      }
      else {
        *(double *)(puVar11 + -0xd0) = dVar14;
        *(undefined8 *)(puVar11 + -0xcc) = in_xd14;
        dVar14 = *(double *)(uVar10 + 0x10);
      }
      if (in_FPSCR_PR == '\0') {
        dVar14 = (double)CONCAT44((float)((ulonglong)dVar14 >> 0x20) - SUB84(dVar14,0),
                                  SUB84(dVar14,0));
      }
      else {
        dVar14 = dVar14 - dVar14;
      }
      if (in_FPSCR_SZ == '\0') {
        *(int *)(puVar11 + -200) = (int)((ulonglong)dVar14 >> 0x20);
      }
      else {
        *(double *)(puVar11 + -200) = dVar14;
      }
      uVar12 = (*(code *)PTR_FUN_8c01c120)(puVar11 + -0xd0);
      if (in_FPSCR_SZ == '\0') {
        dVar14 = (double)CONCAT44((int)((ulonglong)dVar14 >> 0x20),*(undefined4 *)(uVar10 + 0x24));
        dVar18 = (double)CONCAT44(*(undefined4 *)(uVar10 + 0x20),uVar17);
      }
      else {
        dVar18 = *(double *)(uVar10 + 0x20);
      }
      fVar20 = (float)((ulonglong)dVar18 >> 0x20);
      if (in_FPSCR_PR == '\0') {
        bVar22 = fVar20 < SUB84(dVar14,0);
      }
      else {
        bVar22 = dVar18 < dVar14;
      }
      if (in_FPSCR_SZ == '\0') {
        dVar18 = (double)CONCAT44(fVar20,uVar12);
      }
      if (bVar22) {
        if (in_FPSCR_SZ == '\0') {
          dVar18 = (double)CONCAT44(*(undefined4 *)(uVar10 + 0x24),SUB84(dVar18,0));
        }
        else {
          dVar18 = *(double *)(uVar10 + 0x24);
        }
      }
      if (in_FPSCR_SZ == '\0') {
        dVar14 = (double)CONCAT44((int)((ulonglong)dVar14 >> 0x20),*(undefined4 *)(uVar10 + 0x28));
      }
      if (in_FPSCR_PR == '\0') {
        bVar22 = (float)((ulonglong)dVar18 >> 0x20) < SUB84(dVar14,0);
      }
      else {
        bVar22 = dVar18 < dVar14;
      }
      if (bVar22) {
        if (in_FPSCR_SZ == '\0') {
          dVar18 = (double)CONCAT44(*(undefined4 *)(uVar10 + 0x28),SUB84(dVar18,0));
        }
        else {
          dVar18 = *(double *)(uVar10 + 0x28);
        }
      }
      if (in_FPSCR_SZ == '\0') {
        dVar14 = (double)CONCAT44((int)((ulonglong)dVar14 >> 0x20),
                                  *(undefined4 *)(*(int *)(uVar10 + 4) + 0x24));
      }
      if (in_FPSCR_PR == '\0') {
        dVar18 = (double)CONCAT44((float)((ulonglong)dVar18 >> 0x20) * SUB84(dVar14,0),
                                  SUB84(dVar18,0));
      }
      else {
        dVar18 = dVar18 * dVar14;
      }
      if (in_FPSCR_PR != '\0' || SUB84(dVar18,0) <= (float)((ulonglong)dVar18 >> 0x20))
      goto LAB_8c01bf9e;
    }
LAB_8c01c022:
    puVar3 = puVar3 + 3;
    puVar8 = puVar8 + 3;
    uVar6 = uVar6 + 1;
    puVar7 = puVar7 + 3;
  }
  if (in_FPSCR_SZ == '\0') {
    dVar14 = (double)CONCAT44((int)((ulonglong)dVar14 >> 0x20),*(undefined4 *)(puVar11 + -0x58));
  }
  if (SUB84(dVar14,0) == fVar13) {
    if (in_FPSCR_SZ == '\0') {
      dVar14 = (double)CONCAT44(*(undefined4 *)(puVar11 + -100),DAT_8c01c12c);
    }
    else {
      dVar14 = *(double *)(puVar11 + -100);
    }
    if ((float)((ulonglong)dVar14 >> 0x20) == SUB84(dVar14,0)) {
      return;
    }
  }
  uVar16 = (uint)((ulonglong)dVar14 >> 0x20);
  uVar19 = CONCAT44(0x3f800000,uVar17);
  if (in_FPSCR_SZ == '\0') {
    *(undefined4 *)(puVar11 + -0xac) = *(undefined4 *)(puVar11 + -0x74);
    *(undefined4 *)(puVar11 + -0xa8) = *(undefined4 *)(puVar11 + -0x58);
    *(undefined4 *)(puVar11 + -0xa4) = *(undefined4 *)(puVar11 + -0x6c);
    *(undefined4 *)(puVar11 + -0xa0) = uVar12;
    *(undefined4 *)(puVar11 + -0x9c) = 0x3f800000;
    *(undefined4 *)(puVar11 + -0x98) = uVar12;
    *(undefined4 *)(puVar11 + -0xc4) = *(undefined4 *)(puVar11 + -0x74);
    *(undefined4 *)(puVar11 + -0xc0) = *(undefined4 *)(puVar11 + -0x58);
    *(undefined4 *)(puVar11 + -0xbc) = *(undefined4 *)(puVar11 + -0x6c);
    *(undefined4 *)(puVar11 + -0xb8) = uVar12;
    *(undefined4 *)(puVar11 + -0xb4) = 0x3f800000;
    *(undefined4 *)(puVar11 + -0xb0) = uVar12;
  }
  else {
    *(undefined8 *)(puVar11 + -0xac) = *(undefined8 *)(puVar11 + -0x74);
    *(undefined8 *)(puVar11 + -0xa8) = *(undefined8 *)(puVar11 + -0x58);
    *(undefined8 *)(puVar11 + -0xa4) = *(undefined8 *)(puVar11 + -0x6c);
    *(undefined8 *)(puVar11 + -0xa0) = in_xd14;
    *(undefined8 *)(puVar11 + -0x9c) = uVar19;
    *(undefined8 *)(puVar11 + -0x98) = in_xd14;
    *(undefined8 *)(puVar11 + -0xc4) = *(undefined8 *)(puVar11 + -0x74);
    *(undefined8 *)(puVar11 + -0xc0) = *(undefined8 *)(puVar11 + -0x58);
    *(undefined8 *)(puVar11 + -0xbc) = *(undefined8 *)(puVar11 + -0x6c);
    *(undefined8 *)(puVar11 + -0xb8) = in_xd14;
    *(undefined8 *)(puVar11 + -0xb4) = uVar19;
    *(undefined8 *)(puVar11 + -0xb0) = in_xd14;
  }
  fVar13 = (float)(*(code *)PTR_FUN_8c01c130)(puVar11 + -0x94,puVar11 + -0xac,puVar11 + -0xdc);
  uVar17 = (undefined4)uVar19;
  if (fVar13 == 0.0) {
    if (in_FPSCR_SZ == '\0') {
      *(undefined4 *)(puVar11 + -0x5c) = *(undefined4 *)(puVar11 + -0xdc);
      *(undefined4 *)(puVar11 + -0x58) = *(undefined4 *)(puVar11 + -0xd8);
      lVar15 = (ulonglong)*(uint *)(puVar11 + -0xd4) << 0x20;
      *(uint *)(puVar11 + -0x54) = *(uint *)(puVar11 + -0xd4);
    }
    else {
      *(undefined8 *)(puVar11 + -0x5c) = *(undefined8 *)(puVar11 + -0xdc);
      *(undefined8 *)(puVar11 + -0x58) = *(undefined8 *)(puVar11 + -0xd8);
      lVar15 = *(longlong *)(puVar11 + -0xd4);
      *(longlong *)(puVar11 + -0x54) = lVar15;
    }
    uVar16 = (uint)((ulonglong)lVar15 >> 0x20);
  }
  else if (in_FPSCR_SZ == '\0') {
    *(int *)(puVar11 + -0x58) = (int)((ulonglong)uVar21 >> 0x20);
  }
  else {
    *(undefined8 *)(puVar11 + -0x58) = uVar21;
  }
  fVar13 = (float)(*(code *)PTR_FUN_8c01c130)(puVar11 + -0x94,puVar11 + -0xc4,puVar11 + -0xdc);
  if (fVar13 == 0.0) {
    if (in_FPSCR_SZ == '\0') {
      *(undefined4 *)(puVar11 + -0x68) = *(undefined4 *)(puVar11 + -0xdc);
      *(undefined4 *)(puVar11 + -100) = *(undefined4 *)(puVar11 + -0xd8);
      lVar15 = (ulonglong)*(uint *)(puVar11 + -0xd4) << 0x20;
      *(uint *)(puVar11 + -0x60) = *(uint *)(puVar11 + -0xd4);
    }
    else {
      *(undefined8 *)(puVar11 + -0x68) = *(undefined8 *)(puVar11 + -0xdc);
      *(undefined8 *)(puVar11 + -100) = *(undefined8 *)(puVar11 + -0xd8);
      lVar15 = *(longlong *)(puVar11 + -0xd4);
      *(longlong *)(puVar11 + -0x60) = lVar15;
    }
    uVar16 = (uint)((ulonglong)lVar15 >> 0x20);
  }
  else if (in_FPSCR_SZ == '\0') {
    *(int *)(puVar11 + -0x58) = (int)((ulonglong)uVar21 >> 0x20);
  }
  else {
    *(undefined8 *)(puVar11 + -0x58) = uVar21;
  }
  (*(code *)PTR_FUN_8c01c300)(0);
  uVar12 = (undefined4)uVar21;
  (*(code *)PTR_FUN_8c01c304)(0,puVar11 + -0x5c,puVar11 + -0x7c);
  (*(code *)PTR_FUN_8c01c308)(1);
  dVar14 = (double)((ulonglong)uVar16 << 0x20);
  if (in_FPSCR_SZ == '\0') {
    dVar18 = (double)CONCAT44(*(undefined4 *)(puVar11 + -0x78),uVar17);
  }
  else {
    dVar18 = *(double *)(puVar11 + -0x78);
  }
  fVar13 = (float)((ulonglong)dVar18 >> 0x20);
  if (in_FPSCR_PR == '\0') {
    bVar22 = fVar13 < 0.0;
  }
  else {
    bVar22 = dVar18 < dVar14;
  }
  if (!bVar22) {
    if (in_FPSCR_SZ == '\0') {
      dVar14 = (double)((ulonglong)DAT_8c01c30c << 0x20);
    }
    else {
      dVar14 = (double)CONCAT44(_DAT_8c01c310,DAT_8c01c30c);
    }
    if (in_FPSCR_PR == '\0') {
      bVar22 = (float)((ulonglong)dVar14 >> 0x20) < fVar13;
    }
    else {
      bVar22 = dVar14 < dVar18;
    }
    if (!bVar22) goto LAB_8c01c192;
  }
  (*(code *)PTR_FUN_8c01c300)(0);
  (*(code *)PTR_FUN_8c01c304)(0,puVar11 + -0x68,puVar11 + -0x7c);
  (*(code *)PTR_FUN_8c01c308)(1);
LAB_8c01c192:
  uVar17 = DAT_8c01c318;
  *(undefined1 **)(puVar11 + -0xd4) = puVar11 + -0xe4;
  *(undefined1 **)(puVar11 + -0xd0) = puVar11 + -0xec;
  *(undefined4 *)(puVar11 + -0xcc) = 0;
  *(undefined4 *)(puVar11 + -200) = 2;
  if (in_FPSCR_SZ == '\0') {
    *(undefined4 *)*(undefined8 **)(puVar11 + -0xd4) = uVar12;
  }
  else {
    **(undefined8 **)(puVar11 + -0xd4) = in_xd14;
  }
  if (in_FPSCR_SZ == '\0') {
    uVar21 = CONCAT44(_DAT_8c01c310,uVar12);
    *(undefined4 *)(*(int *)(puVar11 + -0xd4) + 8) = _DAT_8c01c310;
  }
  else {
    uVar21 = CONCAT44(DAT_8c01c314,_DAT_8c01c310);
    *(undefined8 *)(*(int *)(puVar11 + -0xd4) + 8) = uVar21;
  }
  uVar12 = DAT_8c01c318;
  uVar1 = DAT_8c01c314;
  if (in_FPSCR_SZ == '\0') {
    uVar19 = CONCAT44(DAT_8c01c314,SUB84(dVar18,0));
    *(undefined4 *)(*(int *)(puVar11 + -0xd4) + 4) = DAT_8c01c314;
    uVar12 = uVar1;
  }
  else {
    uVar19 = CONCAT44(DAT_8c01c318,DAT_8c01c314);
    *(undefined8 *)(*(int *)(puVar11 + -0xd4) + 4) = uVar19;
  }
  if (in_FPSCR_SZ == '\0') {
    *(undefined4 *)(*(int *)(puVar11 + -0xd4) + 0xc) = uVar12;
  }
  else {
    *(undefined8 *)(*(int *)(puVar11 + -0xd4) + 0xc) = uVar19;
  }
  **(undefined4 **)(puVar11 + -0xd0) = uVar17;
  *(undefined4 *)(*(int *)(puVar11 + -0xd0) + 4) = uVar17;
  uVar17 = DAT_8c01c31c;
  if (in_FPSCR_SZ == '\0') {
    uVar17 = (int)uVar19;
  }
  (*(code *)PTR_FUN_8c01c320)(puVar11 + -0xd4,2,0x40);
  if (in_FPSCR_SZ == '\0') {
    dVar14 = (double)CONCAT44((int)((ulonglong)dVar14 >> 0x20),*(undefined4 *)(puVar11 + -0x78));
  }
  if (in_FPSCR_PR == '\0') {
    sVar5 = (short)(int)SUB84(dVar14,0);
  }
  else {
    sVar5 = (short)(int)dVar14;
  }
  if ((sVar5 < 0) || (0x1e0 < sVar5)) {
    sVar5 = 0;
  }
  puVar4 = (undefined8 *)(puVar11 + -0xf4);
  *(undefined8 **)(puVar11 + -0xd4) = puVar4;
  *(undefined **)(puVar11 + -0xd0) = PTR_DAT_8c01c324;
  *(undefined4 *)(puVar11 + -0xcc) = 0;
  *(undefined4 *)(puVar11 + -200) = 4;
  if (in_FPSCR_SZ == '\0') {
    *(int *)puVar4 = (int)uVar21;
  }
  else {
    *puVar4 = in_xd14;
  }
  if (in_FPSCR_PR == '\0') {
    dVar14 = (double)CONCAT44((float)(int)sVar5,uVar17);
  }
  else {
    dVar14 = (double)(int)sVar5;
  }
  uVar17 = (undefined4)((ulonglong)dVar14 >> 0x20);
  uVar12 = (undefined4)((ulonglong)uVar21 >> 0x20);
  if (in_FPSCR_SZ == '\0') {
    *(undefined4 *)(puVar11 + -0xf0) = uVar17;
    *(int *)(puVar11 + -0xec) = (int)uVar21;
    *(uint *)(puVar11 + -0xe8) = DAT_8c01c30c;
    *(undefined4 *)(puVar11 + -0xe4) = uVar12;
    *(uint *)(puVar11 + -0xe0) = DAT_8c01c30c;
    *(undefined4 *)(puVar11 + -0xdc) = uVar12;
    *(undefined4 *)(puVar11 + -0xd8) = uVar17;
  }
  else {
    *(double *)(puVar11 + -0xf0) = dVar14;
    *(undefined8 *)(puVar11 + -0xec) = in_xd14;
    *(ulonglong *)(puVar11 + -0xe8) = CONCAT44(_DAT_8c01c310,DAT_8c01c30c);
    *(undefined8 *)(puVar11 + -0xe4) = uVar21;
    *(ulonglong *)(puVar11 + -0xe0) = CONCAT44(_DAT_8c01c310,DAT_8c01c30c);
    *(undefined8 *)(puVar11 + -0xdc) = uVar21;
    *(double *)(puVar11 + -0xd8) = dVar14;
  }
  (*(code *)PTR_FUN_8c01c328)(0,8);
  uVar17 = (undefined4)uVar21;
  (*(code *)PTR_FUN_8c01c328)(1,10);
  uVar21 = in_xd14;
  if (in_FPSCR_SZ == '\0') {
    uVar21 = CONCAT44(uVar17,uVar17);
  }
  (*(code *)PTR_FUN_8c01c32c)(uVar21,uVar21);
  if (in_FPSCR_SZ == '\0') {
    in_xd14 = CONCAT44(uVar17,(int)uVar21);
  }
  (*(code *)PTR_FUN_8c01c330)(in_xd14,puVar11 + -0xd4,4,0x60);
  if (in_FPSCR_SZ == '\0') {
    uVar21 = CONCAT44((undefined4)DAT_8c01c334,(int)in_xd14);
  }
  else {
    uVar21 = CONCAT44(DAT_8c01c334._4_4_,(undefined4)DAT_8c01c334);
  }
  (*(code *)PTR_FUN_8c01c330)(uVar21,puVar11 + -0xd4,4,0x60);
  return;
}

