/* Ghidra draft only; not buildable/verified source. */

/* WARNING: Removing unreachable block (ram,0x8c037e18) */
/* WARNING: Removing unreachable block (ram,0x8c037d9c) */
/* WARNING: Removing unreachable block (ram,0x8c037dd4) */
/* WARNING: Removing unreachable block (ram,0x8c037e98) */

undefined8 *
FUN_8c037ccc(double param_1,undefined4 param_2,float param_3,undefined4 param_4,float param_5,
            undefined4 param_6,uint param_7,undefined4 param_8)

{
  ushort uVar1;
  short sVar2;
  undefined4 uVar3;
  undefined8 *puVar4;
  int iVar5;
  int iVar6;
  int iVar7;
  uint uVar8;
  short *psVar9;
  short *psVar10;
  short *extraout_r1;
  int in_r2;
  int extraout_r3;
  ushort *puVar11;
  int iVar12;
  undefined8 *puVar13;
  undefined8 *puVar14;
  undefined4 uVar15;
  undefined8 *unaff_r14;
  undefined8 uVar16;
  undefined8 uVar17;
  double dVar18;
  double in_dr12;
  float fVar19;
  float in_fr15;
  undefined4 in_fr14;
  undefined8 in_xd0;
  undefined8 in_xd2;
  undefined8 in_xd4;
  int in_GBR;
  bool bVar20;
  char in_FPSCR_PR;
  char in_FPSCR_SZ;
  
  if ((param_7 & DAT_8c037cd8) == 0) {
    puVar11 = *(ushort **)(in_r2 + 4);
    iVar5 = *(int *)(in_GBR + 0x3c);
    *(int *)(in_GBR + 0x44) = (int)*(short *)(in_r2 + 2);
    psVar9 = *(short **)(in_r2 + 0x14);
    if (in_FPSCR_SZ == '\0') {
      param_5 = *(float *)PTR_DAT_8c037d04;
    }
    iVar6 = *(int *)(in_GBR + 0x38);
    do {
      uVar15 = DAT_8c037dfc;
      iVar7 = *(int *)(in_GBR + 0x54);
      if (iVar7 == 0) {
        uVar1 = *puVar11;
        iVar7 = (int)(short)uVar1;
        puVar11 = puVar11 + 1;
        if (iVar7 < 0) {
          iVar7 = (int)(short)(uVar1 & DAT_8c037dfa);
        }
      }
      iVar7 = iVar7 + -2;
      iVar12 = 0;
      uVar1 = *puVar11;
      *(undefined4 *)(in_GBR + 0x48) = *(undefined4 *)((short)uVar1 * 4 + iVar6);
      puVar4 = (undefined8 *)((short)uVar1 * 0x10 + iVar5);
      uVar1 = puVar11[1];
      puVar11 = puVar11 + 2;
      *(undefined4 *)(in_GBR + 0x4c) = *(undefined4 *)((short)uVar1 * 4 + iVar6);
      puVar13 = (undefined8 *)((short)uVar1 * 0x10 + iVar5);
      if (in_FPSCR_SZ == '\0') {
        param_3 = *(float *)(puVar4 + 1);
        in_dr12 = (double)CONCAT44(*(undefined4 *)(puVar13 + 1),SUB84(in_dr12,0));
      }
      else {
        in_dr12 = (double)puVar13[1];
      }
      if (in_FPSCR_PR == '\0') {
        bVar20 = 0.0 < param_3;
      }
      else {
        bVar20 = 5.26354424712089e-315 < (double)CONCAT44(param_2,param_3);
      }
      uVar8 = (uint)!bVar20;
      if (in_FPSCR_PR == '\0') {
        bVar20 = 1.0 < param_3;
      }
      else {
        bVar20 = 5.26354424712089e-315 < (double)CONCAT44(param_2,param_3);
      }
      if (bVar20) {
        uVar8 = uVar8 | 0x10;
      }
      fVar19 = (float)((ulonglong)in_dr12 >> 0x20);
      if (in_FPSCR_PR == '\0') {
        bVar20 = 0.0 < fVar19;
      }
      else {
        bVar20 = 5.26354424712089e-315 < in_dr12;
      }
      if (!bVar20) {
        uVar8 = uVar8 | 2;
      }
      if (in_FPSCR_PR == '\0') {
        bVar20 = 1.0 < fVar19;
      }
      else {
        bVar20 = 5.26354424712089e-315 < in_dr12;
      }
      if (bVar20) {
        uVar8 = uVar8 | 0x20;
      }
      do {
        uVar1 = *puVar11;
        puVar11 = puVar11 + 1;
        *(undefined4 *)(in_GBR + 0x50) = *(undefined4 *)((short)uVar1 * 4 + iVar6);
        uVar3 = DAT_8c037e44;
        puVar14 = (undefined8 *)((short)uVar1 * 0x10 + iVar5);
        if (in_FPSCR_SZ == '\0') {
          in_fr15 = *(float *)(puVar14 + 1);
        }
        if (in_FPSCR_PR == '\0') {
          bVar20 = 0.0 < in_fr15;
        }
        else {
          bVar20 = 5.26354424712089e-315 < (double)CONCAT44(in_fr14,in_fr15);
        }
        if (!bVar20) {
          uVar8 = uVar8 | 4;
        }
        if (in_FPSCR_PR == '\0') {
          bVar20 = 1.0 < in_fr15;
        }
        else {
          bVar20 = 5.26354424712089e-315 < (double)CONCAT44(in_fr14,in_fr15);
        }
        if (bVar20) {
          uVar8 = uVar8 | 0x40;
        }
        if (uVar8 == 0) {
          if (iVar12 == 0) {
            if (in_FPSCR_SZ == '\0') {
              uVar16 = CONCAT44(*(undefined4 *)puVar4,*(undefined4 *)((int)puVar4 + 4));
              uVar17 = CONCAT44(*(undefined4 *)(puVar4 + 1),*(undefined4 *)((int)puVar4 + 0xc));
            }
            else {
              uVar16 = *puVar4;
              in_xd0 = puVar4[1];
              uVar17 = puVar4[2];
              in_xd2 = puVar4[3];
            }
            sVar2 = *psVar9;
            psVar10 = psVar9 + 1;
            psVar9 = psVar9 + 2;
            if (in_FPSCR_PR == '\0') {
              dVar18 = (double)CONCAT44(*(undefined4 *)(in_GBR + 0x48),(float)(int)sVar2 * param_5);
              param_1 = (double)CONCAT44((float)(int)*psVar10 * param_5,SUB84(param_1,0));
            }
            else {
              dVar18 = (double)CONCAT44(*(undefined4 *)(in_GBR + 0x48),(float)(int)sVar2) *
                       (double)CONCAT44(param_4,param_5);
              param_1 = (double)(int)*psVar10 * (double)CONCAT44(param_4,param_5);
            }
            if (in_FPSCR_SZ == '\0') {
              *(int *)((int)unaff_r14 + 0x1c) = (int)((ulonglong)dVar18 >> 0x20);
              *(int *)(unaff_r14 + 3) = (int)uVar17;
              *(int *)((int)unaff_r14 + 0x14) = (int)((ulonglong)param_1 >> 0x20);
              *(int *)(unaff_r14 + 2) = SUB84(dVar18,0);
              *(int *)((int)unaff_r14 + 0xc) = (int)((ulonglong)uVar17 >> 0x20);
              *(int *)(unaff_r14 + 1) = (int)uVar16;
              puVar4 = (undefined8 *)((int)unaff_r14 + 4);
              *(int *)puVar4 = (int)((ulonglong)uVar16 >> 0x20);
            }
            else {
              unaff_r14[3] = dVar18;
              unaff_r14[2] = in_xd2;
              unaff_r14[1] = param_1;
              *unaff_r14 = in_xd4;
              unaff_r14[-1] = uVar17;
              unaff_r14[-2] = in_xd0;
              puVar4 = unaff_r14 + -3;
              *puVar4 = uVar16;
            }
            *(undefined4 *)((int)puVar4 + -4) = uVar15;
            unaff_r14 = (undefined8 *)((int)puVar4 + 0x1c);
            iVar12 = 1;
          }
          if (in_FPSCR_SZ == '\0') {
            uVar16 = CONCAT44(*(undefined4 *)puVar13,*(undefined4 *)((int)puVar13 + 4));
            uVar17 = CONCAT44(*(undefined4 *)(puVar13 + 1),*(undefined4 *)((int)puVar13 + 0xc));
          }
          else {
            uVar16 = *puVar13;
            in_xd0 = puVar13[1];
            uVar17 = puVar13[2];
            in_xd2 = puVar13[3];
          }
          sVar2 = *psVar9;
          psVar10 = psVar9 + 1;
          psVar9 = psVar9 + 2;
          if (in_FPSCR_PR == '\0') {
            dVar18 = (double)CONCAT44(*(undefined4 *)(in_GBR + 0x4c),(float)(int)sVar2 * param_5);
            param_1 = (double)CONCAT44((float)(int)*psVar10 * param_5,SUB84(param_1,0));
          }
          else {
            dVar18 = (double)CONCAT44(*(undefined4 *)(in_GBR + 0x4c),(float)(int)sVar2) *
                     (double)CONCAT44(param_4,param_5);
            param_1 = (double)(int)*psVar10 * (double)CONCAT44(param_4,param_5);
          }
          if (in_FPSCR_SZ == '\0') {
            *(int *)((int)unaff_r14 + 0x1c) = (int)((ulonglong)dVar18 >> 0x20);
            *(int *)(unaff_r14 + 3) = (int)uVar17;
            *(int *)((int)unaff_r14 + 0x14) = (int)((ulonglong)param_1 >> 0x20);
            *(int *)(unaff_r14 + 2) = SUB84(dVar18,0);
            *(int *)((int)unaff_r14 + 0xc) = (int)((ulonglong)uVar17 >> 0x20);
            *(int *)(unaff_r14 + 1) = (int)uVar16;
            puVar4 = (undefined8 *)((int)unaff_r14 + 4);
            *(int *)puVar4 = (int)((ulonglong)uVar16 >> 0x20);
          }
          else {
            unaff_r14[3] = dVar18;
            unaff_r14[2] = in_xd2;
            unaff_r14[1] = param_1;
            *unaff_r14 = in_xd4;
            unaff_r14[-1] = uVar17;
            unaff_r14[-2] = in_xd0;
            puVar4 = unaff_r14 + -3;
            *puVar4 = uVar16;
          }
          *(undefined4 *)((int)puVar4 + -4) = uVar15;
          unaff_r14 = (undefined8 *)((int)puVar4 + 0x1c);
          iVar12 = iVar12 + 1;
        }
        else {
          if (iVar12 == 0) {
            psVar9 = psVar9 + 2;
          }
          else {
            if (in_FPSCR_SZ == '\0') {
              uVar16 = CONCAT44(*(undefined4 *)puVar13,*(undefined4 *)((int)puVar13 + 4));
              uVar17 = CONCAT44(*(undefined4 *)(puVar13 + 1),*(undefined4 *)((int)puVar13 + 0xc));
            }
            else {
              uVar16 = *puVar13;
              in_xd0 = puVar13[1];
              uVar17 = puVar13[2];
              in_xd2 = puVar13[3];
            }
            if (in_FPSCR_PR == '\0') {
              dVar18 = (double)CONCAT44(*(undefined4 *)(in_GBR + 0x4c),(float)(int)*psVar9 * param_5
                                       );
              param_1 = (double)CONCAT44((float)(int)psVar9[1] * param_5,SUB84(param_1,0));
            }
            else {
              dVar18 = (double)CONCAT44(*(undefined4 *)(in_GBR + 0x4c),(float)(int)*psVar9) *
                       (double)CONCAT44(param_4,param_5);
              param_1 = (double)(int)psVar9[1] * (double)CONCAT44(param_4,param_5);
            }
            if (in_FPSCR_SZ == '\0') {
              *(int *)((int)unaff_r14 + 0x1c) = (int)((ulonglong)dVar18 >> 0x20);
              *(int *)(unaff_r14 + 3) = (int)uVar17;
              *(int *)((int)unaff_r14 + 0x14) = (int)((ulonglong)param_1 >> 0x20);
              *(int *)(unaff_r14 + 2) = SUB84(dVar18,0);
              *(int *)((int)unaff_r14 + 0xc) = (int)((ulonglong)uVar17 >> 0x20);
              *(int *)(unaff_r14 + 1) = (int)uVar16;
              puVar4 = (undefined8 *)((int)unaff_r14 + 4);
              *(int *)puVar4 = (int)((ulonglong)uVar16 >> 0x20);
            }
            else {
              unaff_r14[3] = dVar18;
              unaff_r14[2] = in_xd2;
              unaff_r14[1] = param_1;
              *unaff_r14 = in_xd4;
              unaff_r14[-1] = uVar17;
              unaff_r14[-2] = in_xd0;
              puVar4 = unaff_r14 + -3;
              *puVar4 = uVar16;
            }
            *(undefined4 *)((int)puVar4 + -4) = uVar3;
            unaff_r14 = (undefined8 *)((int)puVar4 + 0x1c);
            iVar12 = 0;
          }
          if ((((uVar8 & 0x11) == 0) || ((uVar8 & 0x22) == 0)) || ((uVar8 & 0x44) == 0)) {
            FUN_8c037ee0(param_6,param_8);
            psVar9 = extraout_r1;
            iVar6 = extraout_r3;
            uVar15 = DAT_8c037e7c;
          }
        }
        *(undefined4 *)(in_GBR + 0x48) = *(undefined4 *)(in_GBR + 0x4c);
        *(undefined4 *)(in_GBR + 0x4c) = *(undefined4 *)(in_GBR + 0x50);
        uVar3 = DAT_8c037ec0;
        uVar8 = uVar8 >> 1 & 0x33;
        iVar7 = iVar7 + -1;
        puVar4 = puVar13;
        puVar13 = puVar14;
      } while (iVar7 != 0);
      if (iVar12 == 0) {
        psVar9 = psVar9 + 4;
      }
      else {
        if (in_FPSCR_SZ == '\0') {
          uVar16 = CONCAT44(*(undefined4 *)puVar14,*(undefined4 *)((int)puVar14 + 4));
          uVar17 = CONCAT44(*(undefined4 *)(puVar14 + 1),*(undefined4 *)((int)puVar14 + 0xc));
        }
        else {
          uVar16 = *puVar14;
          in_xd0 = puVar14[1];
          uVar17 = puVar14[2];
          in_xd2 = puVar14[3];
        }
        sVar2 = *psVar9;
        psVar10 = psVar9 + 1;
        psVar9 = psVar9 + 2;
        if (in_FPSCR_PR == '\0') {
          dVar18 = (double)CONCAT44(*(undefined4 *)(in_GBR + 0x50),(float)(int)sVar2 * param_5);
          param_1 = (double)CONCAT44((float)(int)*psVar10 * param_5,SUB84(param_1,0));
        }
        else {
          dVar18 = (double)CONCAT44(*(undefined4 *)(in_GBR + 0x50),(float)(int)sVar2) *
                   (double)CONCAT44(param_4,param_5);
          param_1 = (double)(int)*psVar10 * (double)CONCAT44(param_4,param_5);
        }
        if (in_FPSCR_SZ == '\0') {
          *(int *)((int)unaff_r14 + 0x1c) = (int)((ulonglong)dVar18 >> 0x20);
          *(int *)(unaff_r14 + 3) = (int)uVar17;
          *(int *)((int)unaff_r14 + 0x14) = (int)((ulonglong)param_1 >> 0x20);
          *(int *)(unaff_r14 + 2) = SUB84(dVar18,0);
          *(int *)((int)unaff_r14 + 0xc) = (int)((ulonglong)uVar17 >> 0x20);
          *(int *)(unaff_r14 + 1) = (int)uVar16;
          puVar4 = (undefined8 *)((int)unaff_r14 + 4);
          *(int *)puVar4 = (int)((ulonglong)uVar16 >> 0x20);
        }
        else {
          unaff_r14[3] = dVar18;
          unaff_r14[2] = in_xd2;
          unaff_r14[1] = param_1;
          *unaff_r14 = in_xd4;
          unaff_r14[-1] = uVar17;
          unaff_r14[-2] = in_xd0;
          puVar4 = unaff_r14 + -3;
          *puVar4 = uVar16;
        }
        *(undefined4 *)((int)puVar4 + -4) = uVar3;
        unaff_r14 = (undefined8 *)((int)puVar4 + 0x1c);
      }
      iVar7 = *(int *)(in_GBR + 0x44) + -1;
      *(int *)(in_GBR + 0x44) = iVar7;
    } while (iVar7 != 0);
    return unaff_r14;
  }
  puVar4 = (undefined8 *)FUN_8c0382ac();
  return puVar4;
}

