/* Ghidra draft only; not buildable/verified source. */

/* WARNING: Removing unreachable block (ram,0x8c075762) */
/* WARNING: Removing unreachable block (ram,0x8c0757d8) */

undefined4 FUN_8c075686(undefined8 param_1,int param_2)

{
  undefined *puVar1;
  int iVar2;
  undefined4 uVar3;
  undefined4 *puVar4;
  int iVar5;
  int iVar6;
  int iVar7;
  undefined1 *puVar8;
  undefined4 uVar9;
  undefined8 uVar10;
  float fVar11;
  double in_dr2;
  double dVar12;
  double in_dr12;
  float in_fr15;
  undefined4 in_fr14;
  undefined8 in_xd2;
  bool bVar13;
  undefined4 in_PR;
  char in_FPSCR_PR;
  char in_FPSCR_SZ;
  undefined1 auStack_2c [8];
  undefined1 auStack_24 [8];
  
  if (in_FPSCR_SZ == '\0') {
    puVar8 = auStack_24;
  }
  else {
    puVar8 = auStack_2c;
  }
  *(undefined4 *)(puVar8 + -4) = in_PR;
  iVar6 = 0;
  *(undefined4 *)(puVar8 + -0x98) = 0;
  *(undefined4 *)(puVar8 + -0x94) = 0xffffffff;
  if (in_FPSCR_SZ == '\0') {
    in_fr15 = DAT_8c0756d4;
  }
  do {
    uVar3 = (undefined4)param_1;
    if ((int)(uint)*(ushort *)(param_2 + 0x28) <= iVar6) {
      if (0 < *(int *)(puVar8 + -0x98)) {
        uVar10 = (*(code *)PTR_FUN_8c075854)(param_2 + 0x10);
        fVar11 = (float)((ulonglong)uVar10 >> 0x20);
        uVar9 = (undefined4)uVar10;
        if (in_FPSCR_SZ == '\0') {
          uVar9 = uVar3;
        }
        iVar6 = *(int *)(puVar8 + -0x94) * 4;
        if (in_FPSCR_SZ == '\0') {
          in_dr2 = (double)CONCAT44((int)((ulonglong)in_dr2 >> 0x20),
                                    *(undefined4 *)(puVar8 + iVar6 + -0x44));
        }
        else {
          in_xd2 = *(undefined8 *)(puVar8 + iVar6 + -0x44);
        }
        if (in_FPSCR_PR == '\0') {
          bVar13 = SUB84(in_dr2,0) < fVar11;
        }
        else {
          bVar13 = in_dr2 < (double)CONCAT44(fVar11,uVar9);
        }
        if (bVar13) {
          fVar11 = (float)(*(code *)PTR_FUN_8c075854)(param_2 + 0x10);
          if (fVar11 == 0.0) {
            if (in_FPSCR_SZ == '\0') {
              *(undefined4 *)(param_2 + 0x18) = 0;
              *(undefined4 *)(param_2 + 0x14) = 0;
              *(undefined4 *)(param_2 + 0x10) = 0;
            }
            else {
              *(undefined8 *)(param_2 + 0x18) = in_xd2;
              *(undefined8 *)(param_2 + 0x14) = in_xd2;
              *(undefined8 *)(param_2 + 0x10) = in_xd2;
            }
          }
          else {
            (*(code *)PTR_FUN_8c075858)(param_2 + 0x10);
            puVar4 = (undefined4 *)(puVar8 + iVar6 + -0x44);
            if (in_FPSCR_SZ == '\0') {
              dVar12 = (double)CONCAT44(*(undefined4 *)(param_2 + 0x10),*puVar4);
            }
            else {
              dVar12 = *(double *)(param_2 + 0x10);
            }
            if (in_FPSCR_PR == '\0') {
              dVar12 = (double)CONCAT44((float)((ulonglong)dVar12 >> 0x20) * SUB84(dVar12,0),
                                        SUB84(dVar12,0));
            }
            else {
              dVar12 = dVar12 * dVar12;
            }
            if (in_FPSCR_SZ == '\0') {
              *(int *)(param_2 + 0x10) = (int)((ulonglong)dVar12 >> 0x20);
              dVar12 = (double)CONCAT44(*(undefined4 *)(param_2 + 0x14),*puVar4);
            }
            else {
              *(double *)(param_2 + 0x10) = dVar12;
              dVar12 = *(double *)(param_2 + 0x14);
            }
            if (in_FPSCR_PR == '\0') {
              dVar12 = (double)CONCAT44((float)((ulonglong)dVar12 >> 0x20) * SUB84(dVar12,0),
                                        SUB84(dVar12,0));
            }
            else {
              dVar12 = dVar12 * dVar12;
            }
            if (in_FPSCR_SZ == '\0') {
              *(int *)(param_2 + 0x14) = (int)((ulonglong)dVar12 >> 0x20);
              dVar12 = (double)CONCAT44(*(undefined4 *)(param_2 + 0x18),*puVar4);
            }
            else {
              *(double *)(param_2 + 0x14) = dVar12;
              dVar12 = *(double *)(param_2 + 0x18);
            }
            if (in_FPSCR_PR == '\0') {
              dVar12 = (double)CONCAT44((float)((ulonglong)dVar12 >> 0x20) * SUB84(dVar12,0),
                                        SUB84(dVar12,0));
            }
            else {
              dVar12 = dVar12 * dVar12;
            }
            if (in_FPSCR_SZ == '\0') {
              *(int *)(param_2 + 0x18) = (int)((ulonglong)dVar12 >> 0x20);
            }
            else {
              *(double *)(param_2 + 0x18) = dVar12;
            }
          }
          return 1;
        }
      }
      return 0;
    }
    iVar7 = iVar6 * 4;
    *(undefined1 **)(puVar8 + -0x9c) = puVar8 + iVar7 + -0x44;
    iVar5 = iVar6 * 0x58 + DAT_8c075840;
    uVar10 = (*(code *)PTR_FUN_8c075844)(iVar5 + 0x2c,param_2 + 4);
    puVar1 = PTR_FUN_8c075848;
    if (in_FPSCR_SZ == '\0') {
      *(int *)*(undefined8 **)(puVar8 + -0x9c) = (int)((ulonglong)uVar10 >> 0x20);
    }
    else {
      **(undefined8 **)(puVar8 + -0x9c) = uVar10;
    }
    *(undefined4 *)(puVar8 + iVar7 + -0x84) = 0;
    iVar2 = (*(code *)puVar1)(param_2 + 0x1c,iVar5 + 0x14,0);
    uVar3 = SUB84(in_dr12,0);
    if (iVar2 < 0x2c01) {
LAB_8c07573c:
      *(uint *)(puVar8 + iVar7 + -0x84) = *(uint *)(puVar8 + iVar7 + -0x84) | 2;
    }
    else {
      uVar10 = (*(code *)PTR_FUN_8c075850)(DAT_8c07584c,param_2 + 0x1c);
      uVar9 = (undefined4)uVar10;
      if (in_FPSCR_SZ == '\0') {
        uVar9 = uVar3;
      }
      in_dr12 = (double)CONCAT44((int)((ulonglong)uVar10 >> 0x20),uVar9);
      dVar12 = (double)(*(code *)PTR_FUN_8c075850)(DAT_8c07584c,iVar5 + 0x14);
      if (in_FPSCR_PR == '\0') {
        bVar13 = (float)((ulonglong)dVar12 >> 0x20) < (float)((ulonglong)in_dr12 >> 0x20);
      }
      else {
        bVar13 = dVar12 < in_dr12;
      }
      if (bVar13) goto LAB_8c07573c;
      uVar3 = *(undefined4 *)(param_2 + 0x14);
      *(undefined4 *)(puVar8 + -0x90) = *(undefined4 *)(param_2 + 0x10);
      *(undefined4 *)(puVar8 + -0x8c) = uVar3;
      *(undefined4 *)(puVar8 + -0x88) = *(undefined4 *)(param_2 + 0x18);
      fVar11 = (float)(*(code *)PTR_FUN_8c075854)(puVar8 + -0x90);
      in_dr2 = (double)((ulonglong)in_dr2 & 0xffffffff00000000);
      if (fVar11 != 0.0) {
        (*(code *)PTR_FUN_8c075858)(puVar8 + -0x90);
      }
      if (iVar2 < 0x7000) {
        dVar12 = (double)(*(code *)PTR_FUN_8c075850)(puVar8 + -0x90,iVar5 + 0x14);
        in_dr2 = (double)((ulonglong)in_dr2 & 0xffffffff00000000);
        if (in_FPSCR_PR == '\0') {
          bVar13 = (float)((ulonglong)dVar12 >> 0x20) < 0.0;
        }
        else {
          bVar13 = dVar12 < in_dr2;
        }
        if (bVar13) {
          *(uint *)(puVar8 + iVar7 + -0x84) = *(uint *)(puVar8 + iVar7 + -0x84) | 8;
          *(int *)(puVar8 + -0x98) = *(int *)(puVar8 + -0x98) + 1;
          if (in_FPSCR_SZ == '\0') {
            in_dr2 = (double)((ulonglong)*(uint *)(puVar8 + iVar7 + -0x44) << 0x20);
          }
          else {
            in_dr2 = *(double *)(puVar8 + iVar7 + -0x44);
          }
          if (in_FPSCR_PR == '\0') {
            bVar13 = (float)((ulonglong)in_dr2 >> 0x20) < in_fr15;
          }
          else {
            bVar13 = in_dr2 < (double)CONCAT44(in_fr14,in_fr15);
          }
          if (bVar13) {
            if (in_FPSCR_SZ == '\0') {
              in_fr15 = *(float *)(puVar8 + iVar7 + -0x44);
            }
            *(int *)(puVar8 + -0x94) = iVar6;
          }
        }
      }
    }
    iVar6 = iVar6 + 1;
  } while( true );
}

