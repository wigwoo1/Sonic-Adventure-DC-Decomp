/* Ghidra draft only; not buildable/verified source. */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined4
FUN_8c0742d4(undefined4 param_1,undefined4 param_2,int param_3,double *param_4,uint *param_5)

{
  undefined *puVar1;
  undefined *puVar2;
  undefined4 uVar3;
  undefined4 uVar4;
  int iVar5;
  int iVar6;
  undefined1 *puVar7;
  double dVar8;
  float fVar9;
  double in_dr2;
  double dVar10;
  uint uVar11;
  double dVar12;
  undefined8 uVar13;
  undefined8 uVar14;
  undefined8 uVar15;
  undefined8 in_dr12;
  undefined4 uVar16;
  undefined8 in_dr14;
  double in_xd2;
  double in_xd14;
  bool bVar17;
  undefined4 in_PR;
  char in_FPSCR_PR;
  char in_FPSCR_SZ;
  undefined1 auStack_30 [8];
  undefined8 local_28;
  undefined8 uStack_20;
  
  if (in_FPSCR_SZ == '\0') {
    uStack_20 = (double)CONCAT44((int)in_dr14,(undefined4)uStack_20);
                    /* WARNING: Ignoring partial resolution of indirect */
    uStack_20._0_4_ = (int)((ulonglong)in_dr14 >> 0x20);
    puVar7 = (undefined1 *)((int)&local_28 + 4);
    local_28 = CONCAT44((int)((ulonglong)in_dr12 >> 0x20),(undefined4)local_28);
  }
  else {
    puVar7 = auStack_30;
    uStack_20 = in_xd14;
    local_28 = in_dr14;
  }
  *(undefined4 *)(puVar7 + -4) = in_PR;
  iVar5 = (uint)*(byte *)(param_3 + 8) * 4;
  iVar6 = *(int *)(DAT_8c074394 + iVar5);
  (*(code *)PTR_FUN_8c074398)(*(undefined4 *)(DAT_8c074390 + iVar5));
  puVar1 = PTR_DAT_8c0743a0;
  uVar3 = (undefined4)in_dr12;
  uVar16 = 0;
  if (in_FPSCR_SZ == '\0') {
    *(undefined4 *)(puVar7 + -0x98) = 0;
    *(undefined4 *)(puVar7 + -0xa0) = 0;
    in_dr2 = (double)CONCAT44((int)((ulonglong)in_dr2 >> 0x20),DAT_8c07439c);
    *(undefined4 *)(puVar7 + -0x9c) = DAT_8c07439c;
  }
  else {
    *(double *)(puVar7 + -0x98) = in_xd14;
    *(double *)(puVar7 + -0xa0) = in_xd14;
    in_xd2 = (double)CONCAT44(PTR_DAT_8c0743a0,DAT_8c07439c);
    *(double *)(puVar7 + -0x9c) = in_xd2;
  }
  if ((*(int *)PTR_DAT_8c0743a0 != 0) && (*(char *)(param_3 + 8) == '\x05')) {
    if (in_FPSCR_SZ == '\0') {
      in_dr2 = (double)CONCAT44((int)((ulonglong)in_dr2 >> 0x20),*(undefined4 *)PTR_DAT_8c0743a4);
      *(undefined4 *)(puVar7 + -0x9c) = *(undefined4 *)PTR_DAT_8c0743a4;
    }
    else {
      in_xd2 = *(double *)PTR_DAT_8c0743a4;
      *(double *)(puVar7 + -0x9c) = in_xd2;
    }
  }
  FUN_8c074cf0(iVar6,param_4,puVar7 + -0xa0);
  if ((*(int *)puVar1 == 0) || ((*(ushort *)(param_3 + 4) & 0x2000) != 0)) {
    (*(code *)PTR_FUN_8c0743a8)(param_3,puVar7 + -0xa0);
  }
  else {
    (*(code *)PTR_FUN_8c074520)(param_4 + 9,puVar7 + -0xa0,*(undefined4 *)(param_3 + 0x18));
  }
  if (in_FPSCR_SZ == '\0') {
    *(undefined4 *)(puVar7 + -0x8c) = uVar16;
    *(undefined4 *)(puVar7 + -0x94) = uVar16;
    in_dr2 = (double)CONCAT44((int)((ulonglong)in_dr2 >> 0x20),*(undefined4 *)param_4);
    *(undefined4 *)(puVar7 + -0x90) = *(undefined4 *)param_4;
  }
  else {
    *(double *)(puVar7 + -0x8c) = in_xd14;
    *(double *)(puVar7 + -0x94) = in_xd14;
    in_xd2 = *param_4;
    *(double *)(puVar7 + -0x90) = in_xd2;
  }
  if ((*(int *)puVar1 != 0) && (*(char *)(param_3 + 8) == '\x05')) {
    if (in_FPSCR_SZ == '\0') {
      in_dr2 = (double)CONCAT44((int)((ulonglong)in_dr2 >> 0x20),*(undefined4 *)PTR_DAT_8c074524);
      *(undefined4 *)(puVar7 + -0x90) = *(undefined4 *)PTR_DAT_8c074524;
    }
    else {
      in_xd2 = *(double *)PTR_DAT_8c074524;
      *(double *)(puVar7 + -0x90) = in_xd2;
    }
  }
  if ((*(int *)puVar1 == 0) || ((*(ushort *)(param_3 + 4) & 0x2000) != 0)) {
    (*(code *)PTR_FUN_8c074528)(param_3,puVar7 + -0x94);
  }
  else {
    (*(code *)PTR_FUN_8c074520)(param_4 + 9,puVar7 + -0x94,*(undefined4 *)(param_3 + 0x18));
  }
  puVar2 = PTR_FUN_8c0746e0;
  dVar8 = (double)CONCAT44(0x3f800000,uVar16);
  if (in_FPSCR_SZ == '\0') {
    dVar12 = (double)CONCAT44(*(undefined4 *)(puVar7 + -0xa0),param_1);
  }
  else {
    dVar12 = *(double *)(puVar7 + -0xa0);
  }
  uVar4 = (undefined4)((ulonglong)in_dr2 >> 0x20);
  fVar9 = (float)((ulonglong)dVar12 >> 0x20);
  if ((*(int *)puVar1 == 0) || ((*(ushort *)(param_3 + 4) & 0x2000) != 0)) {
    if (*(char *)(param_3 + 8) == '\a') {
      if (in_FPSCR_SZ == '\0') {
        in_dr2 = (double)CONCAT44(uVar4,*(undefined4 *)((int)param_4 + 0x54));
      }
      else {
        in_xd2 = *(double *)((int)param_4 + 0x54);
      }
      if (in_FPSCR_PR == '\0') {
        fVar9 = SUB84(in_dr2,0) + fVar9;
      }
      else {
        fVar9 = SUB84(in_dr2 + dVar12,0);
      }
      if (in_FPSCR_SZ == '\0') {
        *(float *)((int)param_4 + 4) = fVar9;
        dVar12 = (double)CONCAT44(*(undefined4 *)(param_4 + 0xb),*(undefined4 *)(puVar7 + -0x9c));
      }
      else {
        *(double *)((int)param_4 + 4) = in_xd2;
        in_xd2 = *(double *)(puVar7 + -0x9c);
        dVar12 = param_4[0xb];
      }
      if (in_FPSCR_PR == '\0') {
        dVar12 = (double)CONCAT44((float)((ulonglong)dVar12 >> 0x20) + SUB84(dVar12,0),
                                  SUB84(dVar12,0));
      }
      else {
        dVar12 = dVar12 + dVar12;
      }
      if (in_FPSCR_SZ == '\0') {
        *(int *)(param_4 + 1) = (int)((ulonglong)dVar12 >> 0x20);
        dVar12 = (double)CONCAT44(*(undefined4 *)((int)param_4 + 0x5c),
                                  *(undefined4 *)(puVar7 + -0x98));
      }
      else {
        param_4[1] = dVar12;
        in_xd2 = *(double *)(puVar7 + -0x98);
        dVar12 = *(double *)((int)param_4 + 0x5c);
      }
    }
    else {
      if (in_FPSCR_SZ == '\0') {
        dVar10 = (double)CONCAT44(*(undefined4 *)(param_4 + 0xc),*(undefined4 *)((int)param_4 + 4));
      }
      else {
        in_xd2 = *(double *)((int)param_4 + 4);
        dVar10 = param_4[0xc];
      }
      if (in_FPSCR_PR == '\0') {
        dVar10 = (double)CONCAT44((float)((ulonglong)dVar10 >> 0x20) - SUB84(dVar10,0),
                                  SUB84(dVar10,0));
      }
      else {
        dVar10 = dVar10 - dVar10;
      }
      if (in_FPSCR_SZ == '\0') {
        dVar10 = (double)CONCAT44((int)((ulonglong)dVar10 >> 0x20),*(undefined4 *)(param_3 + 0x20));
      }
      else {
        in_xd2 = *(double *)(param_3 + 0x20);
      }
      if (in_FPSCR_PR == '\0') {
        dVar12 = (double)CONCAT44((float)((ulonglong)dVar10 >> 0x20) + SUB84(dVar10,0) + fVar9,
                                  SUB84(dVar10,0));
      }
      else {
        dVar12 = dVar10 + dVar10 + dVar12;
      }
      if (in_FPSCR_SZ == '\0') {
        *(int *)((int)param_4 + 4) = (int)((ulonglong)dVar12 >> 0x20);
        dVar12 = (double)CONCAT44(*(undefined4 *)((int)param_4 + 100),*(undefined4 *)(param_4 + 1));
      }
      else {
        *(double *)((int)param_4 + 4) = dVar12;
        in_xd2 = param_4[1];
        dVar12 = *(double *)((int)param_4 + 100);
      }
      if (in_FPSCR_PR == '\0') {
        dVar12 = (double)CONCAT44((float)((ulonglong)dVar12 >> 0x20) - SUB84(dVar12,0),
                                  SUB84(dVar12,0));
      }
      else {
        dVar12 = dVar12 - dVar12;
      }
      if (in_FPSCR_SZ == '\0') {
        dVar12 = (double)CONCAT44((int)((ulonglong)dVar12 >> 0x20),*(undefined4 *)(param_3 + 0x24));
      }
      else {
        in_xd2 = *(double *)(param_3 + 0x24);
      }
      if (in_FPSCR_PR == '\0') {
        dVar12 = (double)CONCAT44((float)((ulonglong)dVar12 >> 0x20) + SUB84(dVar12,0),
                                  SUB84(dVar12,0));
      }
      else {
        dVar12 = dVar12 + dVar12;
      }
      if (in_FPSCR_SZ == '\0') {
        dVar12 = (double)CONCAT44((int)((ulonglong)dVar12 >> 0x20),*(undefined4 *)(puVar7 + -0x9c));
      }
      else {
        in_xd2 = *(double *)(puVar7 + -0x9c);
      }
      if (in_FPSCR_PR == '\0') {
        dVar12 = (double)CONCAT44((float)((ulonglong)dVar12 >> 0x20) + SUB84(dVar12,0),
                                  SUB84(dVar12,0));
      }
      else {
        dVar12 = dVar12 + dVar12;
      }
      if (in_FPSCR_SZ == '\0') {
        *(int *)(param_4 + 1) = (int)((ulonglong)dVar12 >> 0x20);
        dVar12 = (double)CONCAT44(*(undefined4 *)(param_4 + 0xd),*(undefined4 *)((int)param_4 + 0xc)
                                 );
      }
      else {
        param_4[1] = dVar12;
        in_xd2 = *(double *)((int)param_4 + 0xc);
        dVar12 = param_4[0xd];
      }
      if (in_FPSCR_PR == '\0') {
        dVar12 = (double)CONCAT44((float)((ulonglong)dVar12 >> 0x20) - SUB84(dVar12,0),
                                  SUB84(dVar12,0));
      }
      else {
        dVar12 = dVar12 - dVar12;
      }
      if (in_FPSCR_SZ == '\0') {
        dVar12 = (double)CONCAT44((int)((ulonglong)dVar12 >> 0x20),*(undefined4 *)(param_3 + 0x28));
      }
      else {
        in_xd2 = *(double *)(param_3 + 0x28);
      }
      if (in_FPSCR_PR == '\0') {
        dVar12 = (double)CONCAT44((float)((ulonglong)dVar12 >> 0x20) + SUB84(dVar12,0),
                                  SUB84(dVar12,0));
      }
      else {
        dVar12 = dVar12 + dVar12;
      }
      if (in_FPSCR_SZ == '\0') {
        dVar12 = (double)CONCAT44((int)((ulonglong)dVar12 >> 0x20),*(undefined4 *)(puVar7 + -0x98));
      }
      else {
        in_xd2 = *(double *)(puVar7 + -0x98);
      }
    }
    if (in_FPSCR_PR == '\0') {
      dVar12 = (double)CONCAT44((float)((ulonglong)dVar12 >> 0x20) + SUB84(dVar12,0),SUB84(dVar12,0)
                               );
    }
    else {
      dVar12 = dVar12 + dVar12;
    }
    if (in_FPSCR_SZ == '\0') {
      *(int *)((int)param_4 + 0xc) = (int)((ulonglong)dVar12 >> 0x20);
    }
    else {
      *(double *)((int)param_4 + 0xc) = dVar12;
    }
  }
  else {
    if (in_FPSCR_SZ == '\0') {
      dVar12 = (double)CONCAT44(fVar9,*(undefined4 *)(puVar7 + -0x94));
    }
    fVar9 = (float)((ulonglong)dVar12 >> 0x20);
    if ((*param_5 & 0x4000) == 0) {
      if (in_FPSCR_SZ == '\0') {
        in_dr2 = (double)CONCAT44(uVar4,*(undefined4 *)(param_4 + 0xc));
      }
      else {
        in_xd2 = param_4[0xc];
      }
      if (in_FPSCR_PR == '\0') {
        fVar9 = (SUB84(in_dr2,0) - SUB84(dVar12,0)) + fVar9;
      }
      else {
        fVar9 = SUB84((in_dr2 - dVar12) + dVar12,0);
      }
      if (in_FPSCR_SZ == '\0') {
        *(float *)((int)param_4 + 4) = fVar9;
        dVar10 = (double)CONCAT44(*(undefined4 *)((int)param_4 + 100),
                                  *(undefined4 *)(puVar7 + -0x90));
      }
      else {
        *(double *)((int)param_4 + 4) = in_xd2;
        in_xd2 = *(double *)(puVar7 + -0x90);
        dVar10 = *(double *)((int)param_4 + 100);
      }
      if (in_FPSCR_PR == '\0') {
        dVar10 = (double)CONCAT44((float)((ulonglong)dVar10 >> 0x20) - SUB84(dVar10,0),
                                  SUB84(dVar10,0));
      }
      else {
        dVar10 = dVar10 - dVar10;
      }
      if (in_FPSCR_SZ == '\0') {
        dVar10 = (double)CONCAT44((int)((ulonglong)dVar10 >> 0x20),*(undefined4 *)(puVar7 + -0x9c));
      }
      else {
        in_xd2 = *(double *)(puVar7 + -0x9c);
      }
      if (in_FPSCR_PR == '\0') {
        dVar10 = (double)CONCAT44((float)((ulonglong)dVar10 >> 0x20) + SUB84(dVar10,0),
                                  SUB84(dVar10,0));
      }
      else {
        dVar10 = dVar10 + dVar10;
      }
      if (in_FPSCR_SZ == '\0') {
        *(int *)(param_4 + 1) = (int)((ulonglong)dVar10 >> 0x20);
        dVar10 = (double)CONCAT44(*(undefined4 *)(param_4 + 0xd),*(undefined4 *)(puVar7 + -0x8c));
      }
      else {
        param_4[1] = dVar10;
        in_xd2 = *(double *)(puVar7 + -0x8c);
        dVar10 = param_4[0xd];
      }
      if (in_FPSCR_PR == '\0') {
        dVar10 = (double)CONCAT44((float)((ulonglong)dVar10 >> 0x20) - SUB84(dVar10,0),
                                  SUB84(dVar10,0));
      }
      else {
        dVar10 = dVar10 - dVar10;
      }
      if (in_FPSCR_SZ == '\0') {
        dVar10 = (double)CONCAT44((int)((ulonglong)dVar10 >> 0x20),*(undefined4 *)(puVar7 + -0x98));
      }
      else {
        in_xd2 = *(double *)(puVar7 + -0x98);
      }
      if (in_FPSCR_PR == '\0') {
        dVar10 = (double)CONCAT44((float)((ulonglong)dVar10 >> 0x20) + SUB84(dVar10,0),
                                  SUB84(dVar10,0));
      }
      else {
        dVar10 = dVar10 + dVar10;
      }
      if (in_FPSCR_SZ == '\0') {
        *(int *)((int)param_4 + 0xc) = (int)((ulonglong)dVar10 >> 0x20);
      }
      else {
        *(double *)((int)param_4 + 0xc) = dVar10;
      }
      iVar5 = (*(code *)puVar2)((int)param_4 + 0x1c,param_4 + 9,0);
      uVar16 = SUB84(dVar12,0);
      if (0x2bff < iVar5) {
        uVar4 = *(undefined4 *)(param_4 + 0xe);
        *(undefined4 *)(puVar7 + -0xac) = *(undefined4 *)((int)param_4 + 0x6c);
        *(undefined4 *)(puVar7 + -0xa8) = uVar4;
        *(undefined4 *)(puVar7 + -0xa4) = *(undefined4 *)((int)param_4 + 0x74);
        dVar12 = (double)(*(code *)PTR_FUN_8c0746e4)(puVar7 + -0xac);
        if (in_FPSCR_PR == '\0') {
          bVar17 = (float)((ulonglong)dVar12 >> 0x20) < (float)((ulonglong)dVar8 >> 0x20);
        }
        else {
          bVar17 = dVar12 < dVar8;
        }
        if (!bVar17) {
          (*(code *)PTR_FUN_8c0746e8)(puVar7 + -0xac);
          uVar15 = (*(code *)PTR_FUN_8c0746e4)(param_4 + 2);
          fVar9 = (float)((ulonglong)uVar15 >> 0x20);
          uVar4 = (undefined4)uVar15;
          if (in_FPSCR_SZ == '\0') {
            uVar4 = uVar16;
          }
          dVar12 = (double)CONCAT44(fVar9,uVar4);
          if (in_FPSCR_SZ == '\0') {
            dVar10 = (double)CONCAT44((int)((ulonglong)dVar10 >> 0x20),
                                      *(undefined4 *)(puVar7 + -0xac));
          }
          else {
            in_xd2 = *(double *)(puVar7 + -0xac);
          }
          if (in_FPSCR_PR == '\0') {
            dVar10 = (double)CONCAT44((int)((ulonglong)dVar10 >> 0x20),SUB84(dVar10,0) * fVar9);
          }
          else {
            dVar10 = dVar10 * dVar12;
          }
          if (in_FPSCR_SZ == '\0') {
            *(int *)(param_4 + 2) = SUB84(dVar10,0);
            dVar10 = (double)CONCAT44((int)((ulonglong)dVar10 >> 0x20),
                                      *(undefined4 *)(puVar7 + -0xa8));
          }
          else {
            param_4[2] = in_xd2;
            in_xd2 = *(double *)(puVar7 + -0xa8);
          }
          if (in_FPSCR_PR == '\0') {
            dVar10 = (double)CONCAT44((int)((ulonglong)dVar10 >> 0x20),SUB84(dVar10,0) * fVar9);
          }
          else {
            dVar10 = dVar10 * dVar12;
          }
          if (in_FPSCR_SZ == '\0') {
            *(int *)((int)param_4 + 0x14) = SUB84(dVar10,0);
            dVar10 = (double)CONCAT44((int)((ulonglong)dVar10 >> 0x20),
                                      *(undefined4 *)(puVar7 + -0xa4));
          }
          else {
            *(double *)((int)param_4 + 0x14) = in_xd2;
            in_xd2 = *(double *)(puVar7 + -0xa4);
          }
          if (in_FPSCR_PR == '\0') {
            fVar9 = SUB84(dVar10,0) * fVar9;
          }
          else {
            fVar9 = SUB84(dVar10 * dVar12,0);
          }
          if (in_FPSCR_SZ == '\0') {
            *(float *)(param_4 + 3) = fVar9;
          }
          else {
            param_4[3] = in_xd2;
          }
        }
      }
    }
    else {
      if (in_FPSCR_SZ == '\0') {
        dVar10 = (double)CONCAT44(*(undefined4 *)((int)param_4 + 0x6c),
                                  *(undefined4 *)(param_4 + 0xc));
      }
      else {
        in_xd2 = param_4[0xc];
        dVar10 = *(double *)((int)param_4 + 0x6c);
      }
      if (in_FPSCR_PR == '\0') {
        dVar12 = (double)CONCAT44((((float)((ulonglong)dVar10 >> 0x20) + SUB84(dVar10,0)) -
                                  SUB84(dVar12,0)) + fVar9,SUB84(dVar10,0));
      }
      else {
        dVar12 = ((dVar10 + dVar10) - dVar12) + dVar12;
      }
      if (in_FPSCR_SZ == '\0') {
        *(int *)((int)param_4 + 4) = (int)((ulonglong)dVar12 >> 0x20);
        dVar12 = (double)CONCAT44(*(undefined4 *)(param_4 + 0xe),*(undefined4 *)((int)param_4 + 100)
                                 );
      }
      else {
        *(double *)((int)param_4 + 4) = dVar12;
        in_xd2 = *(double *)((int)param_4 + 100);
        dVar12 = param_4[0xe];
      }
      if (in_FPSCR_PR == '\0') {
        dVar12 = (double)CONCAT44((float)((ulonglong)dVar12 >> 0x20) + SUB84(dVar12,0),
                                  SUB84(dVar12,0));
      }
      else {
        dVar12 = dVar12 + dVar12;
      }
      if (in_FPSCR_SZ == '\0') {
        dVar12 = (double)CONCAT44((int)((ulonglong)dVar12 >> 0x20),*(undefined4 *)(puVar7 + -0x90));
      }
      else {
        in_xd2 = *(double *)(puVar7 + -0x90);
      }
      if (in_FPSCR_PR == '\0') {
        dVar12 = (double)CONCAT44((float)((ulonglong)dVar12 >> 0x20) - SUB84(dVar12,0),
                                  SUB84(dVar12,0));
      }
      else {
        dVar12 = dVar12 - dVar12;
      }
      if (in_FPSCR_SZ == '\0') {
        dVar12 = (double)CONCAT44((int)((ulonglong)dVar12 >> 0x20),*(undefined4 *)(puVar7 + -0x9c));
      }
      else {
        in_xd2 = *(double *)(puVar7 + -0x9c);
      }
      if (in_FPSCR_PR == '\0') {
        dVar12 = (double)CONCAT44((float)((ulonglong)dVar12 >> 0x20) + SUB84(dVar12,0),
                                  SUB84(dVar12,0));
      }
      else {
        dVar12 = dVar12 + dVar12;
      }
      if (in_FPSCR_SZ == '\0') {
        *(int *)(param_4 + 1) = (int)((ulonglong)dVar12 >> 0x20);
        dVar12 = (double)CONCAT44(*(undefined4 *)((int)param_4 + 0x74),
                                  *(undefined4 *)(param_4 + 0xd));
      }
      else {
        param_4[1] = dVar12;
        in_xd2 = param_4[0xd];
        dVar12 = *(double *)((int)param_4 + 0x74);
      }
      if (in_FPSCR_PR == '\0') {
        dVar12 = (double)CONCAT44((float)((ulonglong)dVar12 >> 0x20) + SUB84(dVar12,0),
                                  SUB84(dVar12,0));
      }
      else {
        dVar12 = dVar12 + dVar12;
      }
      if (in_FPSCR_SZ == '\0') {
        dVar12 = (double)CONCAT44((int)((ulonglong)dVar12 >> 0x20),*(undefined4 *)(puVar7 + -0x8c));
      }
      else {
        in_xd2 = *(double *)(puVar7 + -0x8c);
      }
      if (in_FPSCR_PR == '\0') {
        dVar12 = (double)CONCAT44((float)((ulonglong)dVar12 >> 0x20) - SUB84(dVar12,0),
                                  SUB84(dVar12,0));
      }
      else {
        dVar12 = dVar12 - dVar12;
      }
      if (in_FPSCR_SZ == '\0') {
        dVar12 = (double)CONCAT44((int)((ulonglong)dVar12 >> 0x20),*(undefined4 *)(puVar7 + -0x98));
      }
      else {
        in_xd2 = *(double *)(puVar7 + -0x98);
      }
      if (in_FPSCR_PR == '\0') {
        dVar12 = (double)CONCAT44((float)((ulonglong)dVar12 >> 0x20) + SUB84(dVar12,0),
                                  SUB84(dVar12,0));
      }
      else {
        dVar12 = dVar12 + dVar12;
      }
      if (in_FPSCR_SZ == '\0') {
        *(int *)((int)param_4 + 0xc) = (int)((ulonglong)dVar12 >> 0x20);
        *(undefined4 *)(param_4 + 3) = uVar16;
        *(undefined4 *)((int)param_4 + 0x14) = uVar16;
        *(undefined4 *)(param_4 + 2) = uVar16;
      }
      else {
        *(double *)((int)param_4 + 0xc) = dVar12;
        param_4[3] = in_xd14;
        *(double *)((int)param_4 + 0x14) = in_xd14;
        param_4[2] = in_xd14;
      }
    }
  }
  if (in_FPSCR_SZ == '\0') {
    dVar12 = (double)CONCAT44(*(undefined4 *)(iVar6 + 0x38),*(undefined4 *)(iVar6 + 0xd0));
  }
  else {
    in_xd2 = *(double *)(iVar6 + 0xd0);
    dVar12 = *(double *)(iVar6 + 0x38);
  }
  if ((in_FPSCR_PR != '\0' || SUB84(dVar12,0) <= (float)((ulonglong)dVar12 >> 0x20)) ||
     ((*(uint *)(iVar6 + 0x60) & 0x4000) != 0)) {
LAB_8c074606:
    if (in_FPSCR_SZ == '\0') {
      dVar12 = (double)CONCAT44((int)((ulonglong)dVar12 >> 0x20),DAT_8c0746ec);
    }
    else {
      in_xd2 = (double)CONCAT44(_DAT_8c0746f0,DAT_8c0746ec);
    }
LAB_8c074640:
    if (in_FPSCR_SZ == '\0') {
      *(int *)param_4 = SUB84(dVar12,0);
    }
    else {
      *param_4 = in_xd2;
    }
  }
  else {
    if (in_FPSCR_SZ == '\0') {
      dVar12 = (double)((ulonglong)*(uint *)(iVar6 + 0x3c) << 0x20);
    }
    else {
      dVar12 = *(double *)(iVar6 + 0x3c);
    }
    fVar9 = (float)((ulonglong)dVar12 >> 0x20);
    if ((in_FPSCR_PR != '\0' || fVar9 <= SUB84(dVar12,0)) && ((*param_5 & 0x4000) != 0))
    goto LAB_8c074606;
    if (in_FPSCR_SZ == '\0') {
      dVar10 = (double)CONCAT44(_DAT_8c0746f0,*(undefined4 *)(iVar6 + 0x38));
      dVar12 = (double)CONCAT44(fVar9,*(undefined4 *)(iVar6 + 0xc0));
    }
    else {
      dVar10 = (double)CONCAT44(DAT_8c0746f4,_DAT_8c0746f0);
      in_xd2 = *(double *)(iVar6 + 0xc0);
    }
    if (in_FPSCR_PR == '\0') {
      bVar17 = SUB84(dVar10,0) < SUB84(dVar12,0);
    }
    else {
      bVar17 = dVar10 < dVar12;
    }
    uVar16 = (undefined4)((ulonglong)dVar10 >> 0x20);
    if (!bVar17) {
      if (in_FPSCR_SZ == '\0') {
        dVar12 = (double)CONCAT44((int)((ulonglong)dVar12 >> 0x20),*(undefined4 *)(iVar6 + 0xd0));
      }
      else {
        in_xd2 = *(double *)(iVar6 + 0xd0);
      }
      if (in_FPSCR_PR == '\0') {
        bVar17 = SUB84(dVar12,0) < SUB84(dVar10,0);
      }
      else {
        bVar17 = dVar12 < dVar10;
      }
      if (bVar17) goto LAB_8c074644;
      if (in_FPSCR_SZ == '\0') {
        dVar10 = (double)CONCAT44(uVar16,*(undefined4 *)(iVar6 + 0xc0));
        dVar12 = (double)CONCAT44(*(undefined4 *)(iVar6 + 0xd0),*(undefined4 *)(iVar6 + 0x38));
      }
      else {
        in_xd2 = *(double *)(iVar6 + 0x38);
        dVar12 = *(double *)(iVar6 + 0xd0);
      }
      if (in_FPSCR_PR == '\0') {
        fVar9 = (float)((ulonglong)dVar12 >> 0x20) - SUB84(dVar10,0);
        dVar12 = (double)CONCAT44(fVar9,(SUB84(dVar12,0) - SUB84(dVar10,0)) / fVar9 +
                                        (float)((ulonglong)dVar10 >> 0x20));
      }
      else {
        dVar12 = (dVar12 - dVar10) - dVar10;
        dVar12 = dVar12 / dVar12 + dVar10;
      }
      goto LAB_8c074640;
    }
LAB_8c074644:
    if (in_FPSCR_SZ == '\0') {
      dVar12 = (double)((ulonglong)*(uint *)(iVar6 + 0x38) << 0x20);
      *(undefined4 *)param_4 = uVar16;
    }
    else {
      dVar12 = *(double *)(iVar6 + 0x38);
      *param_4 = dVar10;
    }
  }
  if (*(char *)(param_3 + 8) == '\a') {
    if (in_FPSCR_SZ == '\0') {
      dVar12 = (double)CONCAT44(*(undefined4 *)param_4,DAT_8c0746f4);
    }
    else {
      dVar12 = *param_4;
    }
    if (in_FPSCR_PR == '\0') {
      dVar12 = (double)CONCAT44((float)((ulonglong)dVar12 >> 0x20) + SUB84(dVar12,0),SUB84(dVar12,0)
                               );
    }
    else {
      dVar12 = dVar12 + dVar12;
    }
    if (in_FPSCR_SZ == '\0') {
      *(int *)param_4 = (int)((ulonglong)dVar12 >> 0x20);
    }
    else {
      *param_4 = dVar12;
    }
  }
  uVar11 = (uint)((ulonglong)dVar12 >> 0x20);
  if (-1 < *DAT_8c0746f8) {
    (*(code *)PTR_FUN_8c0746fc)(0);
    if (in_FPSCR_SZ == '\0') {
      uVar15 = CONCAT44(*(undefined4 *)((int)param_4 + 0xc),param_2);
      uVar13 = CONCAT44(*(undefined4 *)((int)param_4 + 4),*(undefined4 *)(param_4 + 1));
    }
    else {
      uVar15 = *(undefined8 *)((int)param_4 + 0xc);
      uVar13 = *(undefined8 *)((int)param_4 + 4);
    }
    (*(code *)PTR_FUN_8c074700)(uVar13,uVar15,0);
    if (in_FPSCR_SZ == '\0') {
      uVar15 = CONCAT44(DAT_8c074704._4_4_,(undefined4)DAT_8c074704);
      uVar13 = CONCAT44(DAT_8c07470c,(int)((ulonglong)dVar8 >> 0x20));
    }
    else {
      uVar15 = CONCAT44(DAT_8c07470c,DAT_8c074704._4_4_);
      uVar13 = CONCAT44(PTR_FUN_8c074710,DAT_8c07470c);
    }
    (*(code *)PTR_FUN_8c074710)(uVar13,uVar15);
    if (in_FPSCR_SZ == '\0') {
      uVar14 = CONCAT44(DAT_8c074714,DAT_8c074714);
      uVar15 = CONCAT44(DAT_8c074714,(int)uVar15);
      uVar13 = CONCAT44(DAT_8c074714,SUB84(dVar8,0));
    }
    else {
      uVar14 = CONCAT44(PTR_FUN_8c074718,DAT_8c074714);
      uVar15 = uVar14;
      uVar13 = uVar14;
    }
    uVar16 = (undefined4)uVar13;
    (*(code *)PTR_FUN_8c074718)(uVar14,uVar15,0);
    (*(code *)PTR_FUN_8c074720)(PTR_DAT_8c07471c);
    if (in_FPSCR_SZ == '\0') {
      in_xd14 = (double)CONCAT44(uVar16,uVar16);
    }
    (*(code *)PTR_FUN_8c074710)(in_xd14,in_xd14);
    (*(code *)PTR_FUN_8c074724)(1);
  }
  iVar5 = (*(code *)PTR_FUN_8c074728)(param_4,param_5[1],*param_5);
  if (iVar5 == 0) {
    (*(code *)PTR_FUN_8c07472c)();
    return 0;
  }
  iVar5 = (*(code *)PTR_FUN_8c0748e0)((int)param_4 + 0x1c,param_4 + 9,0);
  *(int *)(puVar7 + -0xb0) = iVar5;
  if (0x2c00 < iVar5) {
    uVar15 = (*(code *)PTR_FUN_8c0748e8)(DAT_8c0748e4,(int)param_4 + 0x1c);
    uVar16 = (undefined4)uVar15;
    if (in_FPSCR_SZ == '\0') {
      uVar16 = uVar3;
    }
    dVar12 = (double)CONCAT44((int)((ulonglong)uVar15 >> 0x20),uVar16);
    dVar8 = (double)(*(code *)PTR_FUN_8c0748e8)(DAT_8c0748e4,param_4 + 9);
    if (in_FPSCR_PR == '\0') {
      bVar17 = (float)((ulonglong)dVar8 >> 0x20) < (float)((ulonglong)dVar12 >> 0x20);
    }
    else {
      bVar17 = dVar8 < dVar12;
    }
    if ((!bVar17) && ((*param_5 & 0x4000) == 0)) goto LAB_8c0747a4;
  }
  dVar8 = (double)(*(code *)PTR_FUN_8c0748e8)(DAT_8c0748e4,param_4 + 9);
  if (in_FPSCR_PR == '\0') {
    bVar17 = 0.0 < (float)((ulonglong)dVar8 >> 0x20);
  }
  else {
    bVar17 = (double)((ulonglong)uVar11 << 0x20) < dVar8;
  }
  if ((!bVar17) || ((*(ushort *)(iVar6 + 0x10) & 3) != 0)) {
    if (in_FPSCR_SZ == '\0') {
      *(undefined4 *)param_4 = *(undefined4 *)(puVar7 + -0x88);
    }
    else {
      *param_4 = *(double *)(puVar7 + -0x88);
    }
    uVar3 = *(undefined4 *)(puVar7 + -0x74);
    *(undefined4 *)(param_4 + 2) = *(undefined4 *)(puVar7 + -0x78);
    *(undefined4 *)((int)param_4 + 0x14) = uVar3;
    *(undefined4 *)(param_4 + 3) = *(undefined4 *)(puVar7 + -0x70);
    return 1;
  }
LAB_8c0747a4:
  (*(code *)PTR_FUN_8c0748ec)();
  return 0;
}

