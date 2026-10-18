/* Ghidra draft only; not buildable/verified source. */

/* WARNING: Removing unreachable block (ram,0x8c64f948) */
/* WARNING: Removing unreachable block (ram,0x8c64f974) */
/* WARNING: Removing unreachable block (ram,0x8c64f9a0) */
/* WARNING: Removing unreachable block (ram,0x8c64f9d4) */
/* WARNING: Removing unreachable block (ram,0x8c64f9fc) */
/* WARNING: Removing unreachable block (ram,0x8c64fa30) */

int FUN_8c64f840(double param_1,int *param_2,int param_3,int *param_4,uint param_5)

{
  short sVar1;
  byte bVar2;
  double *pdVar3;
  int iVar4;
  double *pdVar5;
  double *pdVar6;
  int iVar7;
  int iVar8;
  int iVar9;
  int iVar10;
  int iVar11;
  double in_dr0;
  double dVar12;
  undefined4 uVar13;
  double dVar14;
  float fVar16;
  float fVar17;
  ulonglong uVar15;
  double dVar18;
  double dVar19;
  uint uVar20;
  double in_xd0;
  double in_xd2;
  double in_xd4;
  undefined1 uVar21;
  undefined2 uVar22;
  char in_FPSCR_PR;
  char in_FPSCR_SZ;
  
  pdVar3 = DAT_8c64fae0;
  iVar10 = 0;
  param_5 = DAT_8c64fae4 & param_5;
  iVar9 = param_3 + -1;
  iVar11 = 0;
  if (0 < param_3) {
    do {
      iVar4 = iVar9 * 0xc + *param_2;
      if (in_FPSCR_SZ == '\0') {
        dVar14 = (double)CONCAT44(*(undefined4 *)(iVar4 + 8),*(undefined4 *)pdVar3);
      }
      else {
        in_xd2 = *pdVar3;
        dVar14 = *(double *)(iVar4 + 8);
      }
      if (in_FPSCR_PR == '\0') {
        dVar14 = (double)CONCAT44((float)((ulonglong)dVar14 >> 0x20) - SUB84(dVar14,0),
                                  SUB84(dVar14,0));
      }
      else {
        dVar14 = dVar14 - dVar14;
      }
      iVar4 = iVar11 * 0xc + *param_2;
      if (in_FPSCR_SZ == '\0') {
        in_dr0 = (double)CONCAT44((int)((ulonglong)in_dr0 >> 0x20),*(undefined4 *)(iVar4 + 8));
      }
      else {
        in_xd0 = *(double *)(iVar4 + 8);
      }
      if (in_FPSCR_PR == '\0') {
        in_dr0 = (double)CONCAT44((int)((ulonglong)in_dr0 >> 0x20),SUB84(in_dr0,0) - SUB84(dVar14,0)
                                 );
      }
      else {
        in_dr0 = in_dr0 - dVar14;
      }
      if (in_FPSCR_PR == '\0') {
        dVar14 = (double)((ulonglong)(uint)((float)((ulonglong)dVar14 >> 0x20) * SUB84(in_dr0,0)) <<
                         0x20);
      }
      else {
        dVar14 = (double)((ulonglong)dVar14 & 0xffffffff00000000) * in_dr0;
      }
      if (in_FPSCR_PR == '\0' && (float)((ulonglong)dVar14 >> 0x20) < SUB84(dVar14,0)) {
        pdVar5 = (double *)(*param_2 + iVar9 * 0xc);
        if (in_FPSCR_SZ == '\0') {
          dVar18 = (double)CONCAT44(*(undefined4 *)(pdVar5 + 1),SUB84(param_1,0));
        }
        else {
          dVar18 = pdVar5[1];
        }
        pdVar6 = (double *)(*param_2 + iVar11 * 0xc);
        if (in_FPSCR_SZ == '\0') {
          dVar14 = (double)CONCAT44(*(undefined4 *)pdVar3,SUB84(dVar14,0));
        }
        else {
          dVar14 = *pdVar3;
        }
        fVar16 = (float)((ulonglong)dVar18 >> 0x20);
        if (in_FPSCR_PR == '\0') {
          dVar14 = (double)CONCAT44((float)((ulonglong)dVar14 >> 0x20) - fVar16,SUB84(dVar14,0));
        }
        else {
          dVar14 = dVar14 - dVar18;
        }
        if (in_FPSCR_SZ == '\0') {
          in_dr0 = (double)CONCAT44((int)((ulonglong)in_dr0 >> 0x20),*(undefined4 *)(pdVar6 + 1));
        }
        else {
          in_xd0 = pdVar6[1];
        }
        iVar4 = iVar10 * 0xc;
        if (in_FPSCR_PR == '\0') {
          dVar12 = (double)CONCAT44((int)((ulonglong)in_dr0 >> 0x20),SUB84(in_dr0,0) - fVar16);
        }
        else {
          dVar12 = in_dr0 - dVar18;
        }
        dVar19 = dVar14;
        if (in_FPSCR_SZ == '\0') {
          dVar19 = (double)CONCAT44((int)((ulonglong)dVar14 >> 0x20),SUB84(dVar18,0));
        }
        if (in_FPSCR_PR == '\0') {
          dVar19 = (double)CONCAT44((float)((ulonglong)dVar19 >> 0x20) / SUB84(dVar12,0),
                                    SUB84(dVar19,0));
        }
        else {
          dVar19 = dVar19 / dVar12;
        }
        uVar13 = (undefined4)((ulonglong)dVar19 >> 0x20);
        if (in_FPSCR_SZ == '\0') {
          dVar14 = (double)CONCAT44(*(undefined4 *)pdVar6,SUB84(dVar14,0));
          dVar19 = (double)CONCAT44(uVar13,SUB84(dVar12,0));
          param_1 = (double)CONCAT44(uVar13,*(undefined4 *)pdVar5);
        }
        else {
          dVar14 = *pdVar6;
          in_xd4 = *pdVar5;
          param_1 = dVar19;
        }
        if (in_FPSCR_PR == '\0') {
          dVar14 = (double)CONCAT44((float)((ulonglong)dVar14 >> 0x20) - SUB84(param_1,0),
                                    SUB84(dVar14,0));
        }
        else {
          dVar14 = dVar14 - param_1;
        }
        dVar18 = in_xd4;
        if (in_FPSCR_SZ == '\0') {
          dVar19 = (double)CONCAT44((int)((ulonglong)dVar19 >> 0x20),SUB84(param_1,0));
          dVar18 = in_xd0;
        }
        if (in_FPSCR_PR == '\0') {
          fVar16 = (float)((ulonglong)dVar19 >> 0x20);
          dVar19 = (double)CONCAT44(fVar16,fVar16 * (float)((ulonglong)dVar14 >> 0x20) +
                                           SUB84(dVar19,0));
        }
        if (in_FPSCR_SZ == '\0') {
          *(int *)(*param_4 + iVar4) = SUB84(dVar19,0);
        }
        else {
          *(double *)(*param_4 + iVar4) = dVar18;
        }
        iVar7 = *param_2 + iVar9 * 0xc;
        if (in_FPSCR_SZ == '\0') {
          param_1 = (double)CONCAT44(uVar13,*(undefined4 *)(iVar7 + 4));
        }
        else {
          in_xd4 = *(double *)(iVar7 + 4);
        }
        iVar7 = iVar11 * 0xc + *param_2;
        if (in_FPSCR_SZ == '\0') {
          dVar14 = (double)CONCAT44(*(undefined4 *)(iVar7 + 4),SUB84(dVar14,0));
          dVar19 = (double)CONCAT44((int)((ulonglong)dVar19 >> 0x20),SUB84(param_1,0));
          in_xd0 = dVar18;
        }
        else {
          dVar14 = *(double *)(iVar7 + 4);
          in_xd0 = in_xd4;
        }
        if (in_FPSCR_PR == '\0') {
          uVar13 = SUB84(dVar14,0);
          dVar19 = (double)(ulonglong)
                           (uint)((float)((ulonglong)dVar19 >> 0x20) *
                                  ((float)((ulonglong)dVar14 >> 0x20) - SUB84(param_1,0)) +
                                 SUB84(dVar19,0));
        }
        else {
          uVar13 = SUB84(dVar14 - param_1,0);
        }
        fVar16 = SUB84(dVar19,0);
        if (in_FPSCR_SZ == '\0') {
          *(float *)(*param_4 + iVar4 + 4) = fVar16;
        }
        else {
          *(double *)(*param_4 + iVar4 + 4) = in_xd0;
        }
        if (in_FPSCR_SZ == '\0') {
          dVar14 = (double)CONCAT44(*(undefined4 *)pdVar3,uVar13);
        }
        else {
          dVar14 = *pdVar3;
        }
        if (in_FPSCR_SZ == '\0') {
          *(int *)(iVar4 + *param_4 + 8) = (int)((ulonglong)dVar14 >> 0x20);
        }
        else {
          *(double *)(iVar4 + *param_4 + 8) = dVar14;
        }
        iVar9 = iVar9 * 4;
        uVar20 = (uint)((ulonglong)param_1 >> 0x20);
        if (param_5 != 0) {
          dVar18 = param_1;
          if (in_FPSCR_SZ == '\0') {
            dVar18 = (double)((ulonglong)uVar20 << 0x20);
          }
          sVar1 = *(short *)(param_2[2] + iVar9);
          if (in_FPSCR_PR == '\0') {
            dVar14 = (double)CONCAT44((float)(int)sVar1,SUB84(dVar14,0));
          }
          else {
            dVar14 = (double)(int)sVar1;
          }
          fVar17 = (float)((ulonglong)dVar18 >> 0x20);
          if (in_FPSCR_PR == '\0') {
            fVar16 = fVar17 * (float)((int)*(short *)(param_2[2] + iVar11 * 4) - (int)sVar1) +
                     (float)((ulonglong)dVar14 >> 0x20);
            dVar14 = (double)CONCAT44(fVar16,SUB84(dVar14,0));
            uVar22 = (undefined2)(int)fVar16;
          }
          else {
            uVar22 = (undefined2)(int)dVar14;
          }
          *(undefined2 *)(param_4[2] + iVar10 * 4) = uVar22;
          sVar1 = *(short *)(param_2[2] + iVar9 + 2);
          if (in_FPSCR_PR == '\0') {
            dVar14 = (double)CONCAT44((float)(int)sVar1,SUB84(dVar14,0));
          }
          else {
            dVar14 = (double)(int)sVar1;
          }
          fVar16 = (float)((int)*(short *)(iVar11 * 4 + param_2[2] + 2) - (int)sVar1);
          if (in_FPSCR_PR == '\0') {
            uVar22 = (undefined2)(int)(fVar17 * fVar16 + (float)((ulonglong)dVar14 >> 0x20));
          }
          else {
            uVar22 = (undefined2)(int)dVar14;
          }
          *(undefined2 *)(iVar10 * 4 + param_4[2] + 2) = uVar22;
        }
        iVar4 = iVar11 * 4;
        in_dr0 = param_1;
        if (in_FPSCR_SZ == '\0') {
          in_dr0 = (double)CONCAT44(uVar20,fVar16);
        }
        iVar8 = iVar10 * 4;
        bVar2 = *(byte *)(param_2[1] + iVar9 + 3);
        iVar7 = (uint)*(byte *)(param_2[1] + iVar4 + 3) - (uint)bVar2;
        fVar16 = (float)((ulonglong)in_dr0 >> 0x20);
        if (in_FPSCR_PR == '\0') {
          uVar21 = (undefined1)(int)(fVar16 * (float)iVar7 + (float)bVar2);
        }
        else {
          uVar21 = (undefined1)(int)(double)iVar7;
        }
        *(undefined1 *)(param_4[1] + iVar8 + 3) = uVar21;
        bVar2 = *(byte *)(param_2[1] + iVar9 + 2);
        iVar7 = (uint)*(byte *)(param_2[1] + iVar4 + 2) - (uint)bVar2;
        if (in_FPSCR_PR == '\0') {
          uVar21 = (undefined1)(int)(fVar16 * (float)iVar7 + (float)bVar2);
        }
        else {
          uVar21 = (undefined1)(int)(double)iVar7;
        }
        *(undefined1 *)(param_4[1] + iVar8 + 2) = uVar21;
        bVar2 = *(byte *)(param_2[1] + iVar9 + 1);
        iVar7 = (uint)*(byte *)(param_2[1] + iVar4 + 1) - (uint)bVar2;
        if (in_FPSCR_PR == '\0') {
          uVar21 = (undefined1)(int)(fVar16 * (float)iVar7 + (float)bVar2);
        }
        else {
          uVar21 = (undefined1)(int)(double)iVar7;
        }
        *(undefined1 *)(param_4[1] + iVar8 + 1) = uVar21;
        bVar2 = *(byte *)(param_2[1] + iVar9);
        iVar9 = (uint)*(byte *)(iVar4 + param_2[1]) - (uint)bVar2;
        if (in_FPSCR_PR == '\0') {
          fVar17 = (float)iVar9;
          fVar16 = fVar16 * fVar17 + (float)bVar2;
          dVar14 = (double)CONCAT44(fVar17,fVar16);
          uVar21 = (undefined1)(int)fVar16;
        }
        else {
          dVar14 = (double)iVar9;
          uVar21 = (undefined1)(int)dVar14;
        }
        iVar10 = iVar10 + 1;
        *(undefined1 *)(iVar8 + param_4[1]) = uVar21;
      }
      uVar13 = SUB84(dVar14,0);
      if (in_FPSCR_SZ == '\0') {
        uVar13 = *(undefined4 *)pdVar3;
      }
      else {
        in_xd2 = *pdVar3;
      }
      iVar9 = iVar11 * 0xc;
      if (in_FPSCR_SZ == '\0') {
        uVar15 = CONCAT44(*(undefined4 *)(*param_2 + iVar9 + 8),uVar13);
      }
      else {
        uVar15 = *(ulonglong *)(*param_2 + iVar9 + 8);
      }
      if (in_FPSCR_PR != '\0' || (float)uVar15 <= (float)(uVar15 >> 0x20)) {
        iVar4 = iVar10 * 0xc;
        if (in_FPSCR_SZ == '\0') {
          uVar15 = (ulonglong)*(uint *)(*param_2 + iVar9);
        }
        else {
          in_xd2 = *(double *)(*param_2 + iVar9);
        }
        uVar13 = (undefined4)uVar15;
        if (in_FPSCR_SZ == '\0') {
          *(undefined4 *)(*param_4 + iVar4) = uVar13;
        }
        else {
          *(double *)(*param_4 + iVar4) = in_xd2;
        }
        if (in_FPSCR_SZ == '\0') {
          uVar13 = *(undefined4 *)(*param_2 + iVar9 + 4);
        }
        else {
          in_xd2 = *(double *)(*param_2 + iVar9 + 4);
        }
        if (in_FPSCR_SZ == '\0') {
          *(undefined4 *)(*param_4 + iVar4 + 4) = uVar13;
        }
        else {
          *(double *)(*param_4 + iVar4 + 4) = in_xd2;
        }
        if (in_FPSCR_SZ == '\0') {
          uVar13 = *(undefined4 *)(*param_2 + iVar9 + 8);
        }
        else {
          in_xd2 = *(double *)(*param_2 + iVar9 + 8);
        }
        iVar9 = iVar10 * 4;
        if (in_FPSCR_SZ == '\0') {
          *(undefined4 *)(iVar4 + *param_4 + 8) = uVar13;
        }
        else {
          *(double *)(iVar4 + *param_4 + 8) = in_xd2;
        }
        iVar4 = iVar11 * 4;
        if (param_5 != 0) {
          *(undefined2 *)(param_4[2] + iVar9) = *(undefined2 *)(param_2[2] + iVar4);
          *(undefined2 *)(param_4[2] + iVar9 + 2) = *(undefined2 *)(param_2[2] + iVar4 + 2);
        }
        iVar10 = iVar10 + 1;
        *(undefined4 *)(param_4[1] + iVar9) = *(undefined4 *)(param_2[1] + iVar4);
      }
      iVar4 = iVar11 + 1;
      iVar9 = iVar11;
      iVar11 = iVar4;
    } while (iVar4 < param_3);
  }
  return iVar10;
}

