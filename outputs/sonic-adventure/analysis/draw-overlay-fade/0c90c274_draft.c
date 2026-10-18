/* Ghidra draft only; not buildable/verified source. */

/* WARNING: Removing unreachable block (ram,0x0c90c308) */
/* WARNING: Removing unreachable block (ram,0x0c90c31e) */

void FUN_0c90c274(ulonglong param_1,ulonglong param_2,int param_3,undefined4 param_4,
                 undefined4 param_5)

{
  char cVar1;
  ulonglong uVar2;
  code *pcVar3;
  code *pcVar4;
  code *pcVar5;
  undefined4 uVar6;
  char *pcVar7;
  int iVar8;
  int iVar9;
  ulonglong *puVar10;
  float fVar11;
  undefined8 uVar12;
  undefined4 in_fr3;
  float fVar14;
  double dVar13;
  double dVar15;
  double dVar16;
  undefined4 uVar17;
  undefined8 in_dr12;
  double dVar18;
  uint uVar19;
  ulonglong in_dr14;
  double dVar20;
  double in_xd2;
  double in_xd12;
  double dVar21;
  ulonglong in_xd14;
  undefined4 in_PR;
  char in_FPSCR_PR;
  char in_FPSCR_SZ;
  undefined1 local_3c [16];
  ulonglong uStack_2c;
  undefined8 uStack_24;
  
  if (in_FPSCR_SZ == '\0') {
    uStack_24 = CONCAT44((int)in_dr14,(undefined4)uStack_24);
                    /* WARNING: Ignoring partial resolution of indirect */
    uStack_24._0_4_ = (int)(in_dr14 >> 0x20);
    puVar10 = &uStack_2c;
    uStack_2c = CONCAT44((int)in_dr12,(int)((ulonglong)in_dr12 >> 0x20));
  }
  else {
    puVar10 = (ulonglong *)local_3c;
    uStack_24 = in_xd14;
    uStack_2c = in_dr14;
  }
  *(undefined4 *)((int)puVar10 + -4) = in_PR;
  *(undefined4 *)((int)puVar10 + -0x20) = param_4;
  uVar6 = *(undefined4 *)(PTR_DAT_0c90c3dc + 4);
  pcVar7 = (char *)(param_3 + 4);
  *(undefined4 *)((int)puVar10 + -0x10) = *(undefined4 *)PTR_DAT_0c90c3dc;
  *(undefined4 *)((int)puVar10 + -0xc) = uVar6;
  iVar8 = 0;
  if (in_FPSCR_SZ == '\0') {
    in_dr14 = param_1 >> 0x20;
    param_1 = in_xd14;
  }
  uVar19 = (uint)in_dr14;
  *(undefined4 *)((int)puVar10 + -8) = *(undefined4 *)(PTR_DAT_0c90c3dc + 8);
  iVar9 = 0;
  if (*pcVar7 != '\0') {
    *(undefined1 *)(param_3 + 0xb) = 0;
    pcVar3 = DAT_0c90c3e0;
    cVar1 = *pcVar7;
    while (cVar1 != '\0') {
      pcVar7 = pcVar7 + 1;
      iVar8 = iVar8 + 1;
      cVar1 = *pcVar7;
    }
    pcVar7 = (char *)(param_3 + 4);
    (*DAT_0c90c3e0)(0);
    uVar17 = (undefined4)in_dr12;
    (*DAT_0c90c3e4)(0,*(undefined4 *)((int)puVar10 + -0x20));
    (*DAT_0c90c3e8)(0,(undefined1 *)((int)puVar10 + -0x10),(undefined1 *)((int)puVar10 + -0x1c));
    pcVar4 = DAT_0c90c3ec;
    (*DAT_0c90c3ec)(1);
    (*pcVar3)(DAT_0c90c3f0);
    uVar12 = (*DAT_0c90c3e4)(0,(undefined1 *)((int)puVar10 + -0x1c));
    pcVar5 = DAT_0c90c3f8;
    fVar11 = (float)uVar12;
    uVar6 = (undefined4)param_2;
    if (in_FPSCR_PR == '\0') {
      fVar14 = -1.0;
    }
    else {
      fVar14 = (float)((ulonglong)-(double)CONCAT44(0x3f800000,in_fr3) >> 0x20);
    }
    dVar13 = (double)CONCAT44(fVar14,(float)iVar8);
    if (in_FPSCR_SZ == '\0') {
      fVar11 = DAT_0c90c3f4;
    }
    cVar1 = *pcVar7;
    if (in_FPSCR_PR == '\0') {
      dVar13 = (double)CONCAT44(fVar14,(float)iVar8 + fVar14);
    }
    else {
      dVar13 = dVar13 + dVar13;
    }
    dVar18 = in_xd2;
    if (in_FPSCR_SZ == '\0') {
      dVar18 = (double)CONCAT44(SUB84(dVar13,0),uVar17);
    }
    if (in_FPSCR_PR == '\0') {
      dVar18 = (double)CONCAT44((float)((ulonglong)dVar18 >> 0x20) * fVar11,SUB84(dVar18,0));
    }
    else {
      dVar18 = dVar18 * (double)CONCAT44((int)((ulonglong)uVar12 >> 0x20),fVar11);
    }
    dVar20 = (double)(ulonglong)uVar19;
    while (cVar1 != '\0') {
      uVar17 = (undefined4)param_2;
      uVar6 = (undefined4)((ulonglong)dVar13 >> 0x20);
      fVar11 = (float)((ulonglong)dVar18 >> 0x20);
      if (in_FPSCR_PR == '\0') {
        dVar13 = (double)CONCAT44(uVar6,(float)iVar9 - fVar11);
      }
      else {
        dVar13 = (double)CONCAT44(uVar6,(float)iVar9) - dVar18;
      }
      dVar21 = in_xd2;
      if (in_FPSCR_SZ == '\0') {
        dVar18 = (double)CONCAT44(fVar11,SUB84(dVar13,0));
        dVar21 = in_xd12;
      }
      if (in_FPSCR_PR == '\0') {
        dVar18 = (double)CONCAT44((int)((ulonglong)dVar18 >> 0x20),SUB84(dVar18,0) * SUB84(dVar20,0)
                                 );
      }
      else {
        dVar18 = dVar18 * dVar20;
      }
      (*pcVar3)(0);
      uVar6 = (undefined4)((ulonglong)dVar20 >> 0x20);
      dVar15 = dVar21;
      dVar16 = dVar20;
      if (in_FPSCR_SZ == '\0') {
        dVar15 = (double)CONCAT44(SUB84(dVar18,0),uVar6);
        dVar16 = (double)CONCAT44(uVar6,uVar17);
      }
      (*DAT_0c90c3fc)(dVar15,dVar16,0);
      param_2 = param_1;
      if (in_FPSCR_SZ == '\0') {
        param_2 = (ulonglong)dVar16 & 0xffffffff;
      }
      (*pcVar5)(0);
      cVar1 = *pcVar7;
      pcVar7 = pcVar7 + 1;
      FUN_0c90bfc0((int)cVar1,param_5);
      (*pcVar4)(1);
      uVar6 = (undefined4)param_2;
      iVar9 = iVar9 + 1;
      if (iVar9 == iVar8) break;
      in_xd12 = dVar21;
      cVar1 = *pcVar7;
    }
    uVar17 = SUB84(dVar20,0);
    uVar2 = param_1;
    if (in_FPSCR_SZ == '\0') {
      param_1 = CONCAT44(uVar17,uVar6);
      uVar2 = CONCAT44(uVar17,uVar17);
    }
    (*pcVar5)(uVar2,param_1,0);
    FUN_0c90c160(param_5);
    (*pcVar4)(1);
  }
  return;
}

