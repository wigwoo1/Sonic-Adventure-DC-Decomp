/* Ghidra draft only; not buildable/verified source. */

/* WARNING: Removing unreachable block (ram,0x0c904ef6) */
/* WARNING: Removing unreachable block (ram,0x0c904eba) */
/* WARNING: Removing unreachable block (ram,0x0c904edc) */
/* WARNING: Removing unreachable block (ram,0x0c905256) */
/* WARNING: Removing unreachable block (ram,0x0c905272) */
/* WARNING: Removing unreachable block (ram,0x0c905290) */
/* WARNING: Removing unreachable block (ram,0x0c9052aa) */
/* WARNING: Removing unreachable block (ram,0x0c9052c0) */
/* WARNING: Removing unreachable block (ram,0x0c90533a) */
/* WARNING: Removing unreachable block (ram,0x0c90534a) */
/* WARNING: Removing unreachable block (ram,0x0c90535e) */
/* WARNING: Removing unreachable block (ram,0x0c905386) */
/* WARNING: Removing unreachable block (ram,0x0c905392) */
/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void callback_candidate_c904e80
               (float param_1,undefined4 param_2,undefined4 param_3,uint param_4,int param_5)

{
  byte bVar1;
  undefined4 uVar2;
  undefined4 uVar3;
  int iVar4;
  int iVar5;
  undefined4 *puVar6;
  undefined1 *puVar7;
  int *piVar8;
  int iVar9;
  double *pdVar10;
  double *pdVar11;
  int iVar12;
  int iVar13;
  double *pdVar14;
  byte *pbVar15;
  int iVar16;
  double *pdVar17;
  int iVar18;
  byte *pbVar19;
  int iVar20;
  byte *pbVar21;
  undefined8 *puVar22;
  byte *pbVar23;
  uint uVar24;
  int iVar25;
  double *pdVar26;
  undefined8 *puVar27;
  float fVar28;
  float fVar29;
  double in_dr0;
  undefined8 uVar30;
  double dVar31;
  double dVar32;
  float fVar33;
  float fVar34;
  float fVar35;
  undefined4 uVar36;
  ulonglong in_dr2;
  double dVar37;
  double dVar38;
  double dVar39;
  double dVar40;
  float fVar41;
  double dVar42;
  double dVar43;
  float fVar44;
  double dVar45;
  float fVar46;
  double in_dr12;
  double dVar47;
  undefined8 in_dr14;
  double dVar48;
  double in_xd0;
  double in_xd2;
  double dVar49;
  double dVar50;
  double dVar51;
  double dVar52;
  double in_xd4;
  double dVar53;
  double in_xd6;
  double in_xd8;
  double in_xd10;
  double in_xd12;
  double in_xd14;
  bool bVar54;
  undefined4 in_PR;
  undefined1 uVar55;
  undefined1 uVar56;
  uint uVar57;
  char in_FPSCR_PR;
  char in_FPSCR_SZ;
  undefined1 local_3c [16];
  undefined8 uStack_2c;
  undefined8 uStack_24;
  
  uVar3 = (undefined4)((ulonglong)in_dr12 >> 0x20);
  if (in_FPSCR_SZ == '\0') {
    uStack_24 = (double)CONCAT44((int)in_dr14,(undefined4)uStack_24);
                    /* WARNING: Ignoring partial resolution of indirect */
    uStack_24._0_4_ = (int)((ulonglong)in_dr14 >> 0x20);
    puVar27 = &uStack_2c;
    uStack_2c = CONCAT44(SUB84(in_dr12,0),uVar3);
  }
  else {
    puVar27 = (undefined8 *)local_3c;
    uStack_24 = in_xd14;
    uStack_2c = in_dr14;
  }
  *(undefined4 *)((int)puVar27 + -4) = in_PR;
  iVar5 = *(int *)(param_5 + 0xec);
  if (in_FPSCR_SZ == '\0') {
    param_1 = *(float *)(iVar5 + 0x20);
    dVar42 = (double)CONCAT44(*(undefined4 *)(iVar5 + 0x24),param_2);
    param_3 = *(undefined4 *)(iVar5 + 0x28);
  }
  else {
    in_xd4 = *(double *)(iVar5 + 0x20);
    dVar42 = *(double *)(iVar5 + 0x24);
    in_xd8 = *(double *)(iVar5 + 0x28);
  }
  *(int *)((int)puVar27 + -0x84) = param_5 + 0x314;
  iVar5 = param_5 + 0x24c;
  *(int *)((int)puVar27 + -0x80) = param_5 + 0x3dc;
  if (in_FPSCR_SZ == '\0') {
    dVar38 = (double)CONCAT44(DAT_0c904f20,param_1);
  }
  else {
    dVar38 = (double)CONCAT44(uRam0c904f24,DAT_0c904f20);
  }
  dVar53 = in_xd4;
  if ((param_1 == 0.0) && (dVar53 = dVar38, in_FPSCR_SZ == '\0')) {
    uVar2 = (undefined4)((ulonglong)dVar38 >> 0x20);
    dVar38 = (double)CONCAT44(uVar2,uVar2);
    dVar53 = in_xd4;
  }
  uVar2 = (undefined4)((ulonglong)dVar38 >> 0x20);
  dVar43 = dVar42;
  if (((float)((ulonglong)dVar42 >> 0x20) == 0.0) && (dVar43 = dVar38, in_FPSCR_SZ == '\0')) {
    dVar43 = (double)CONCAT44(uVar2,SUB84(dVar42,0));
  }
  if (in_FPSCR_PR == '\0') {
    bVar54 = 0.0 < SUB84(dVar38,0);
  }
  else {
    bVar54 = (double)((in_dr2 >> 0x20) << 0x20) < dVar38;
  }
  dVar42 = (double)((in_dr2 >> 0x20) << 0x20);
  if (bVar54) {
    iVar18 = 0x324;
  }
  else {
    dVar38 = (double)CONCAT44(uVar2,-SUB84(dVar38,0));
    iVar18 = 0x3ec;
  }
  iVar18 = iVar18 + param_5 + 0x248;
  fVar33 = (float)((ulonglong)dVar43 >> 0x20);
  if (in_FPSCR_PR == '\0') {
    bVar54 = 0.0 < fVar33;
  }
  else {
    bVar54 = dVar42 < dVar43;
  }
  dVar48 = (double)CONCAT44(0x3f800000,(int)in_dr14);
  if (bVar54) {
    iVar20 = 0x25c;
  }
  else {
    if (in_FPSCR_PR == '\0') {
      dVar43 = (double)CONCAT44(-fVar33,SUB84(dVar43,0));
    }
    else {
      dVar43 = -dVar43;
    }
    iVar20 = 0x4b4;
  }
  iVar20 = iVar20 + param_5 + 0x248;
  dVar32 = dVar48;
  if (in_FPSCR_SZ == '\0') {
    dVar43 = (double)CONCAT44((int)((ulonglong)dVar43 >> 0x20),0x3f800000);
    dVar32 = in_xd6;
  }
  uVar2 = *(undefined4 *)(PTR_DAT_0c905220 + 4);
  if (in_FPSCR_PR == '\0') {
    fVar33 = (float)((ulonglong)dVar43 >> 0x20);
    dVar43 = (double)CONCAT44(fVar33,SUB84(dVar43,0) - fVar33);
  }
  else {
    dVar43 = dVar43 - dVar43;
  }
  dVar45 = dVar48;
  if (in_FPSCR_SZ == '\0') {
    dVar45 = (double)CONCAT44(0x3f800000,param_3);
  }
  fVar33 = SUB84(dVar38,0);
  if (in_FPSCR_PR == '\0') {
    dVar45 = (double)CONCAT44((float)((ulonglong)dVar45 >> 0x20) - fVar33,SUB84(dVar45,0));
  }
  else {
    dVar45 = dVar45 - dVar38;
  }
  *(undefined4 *)((int)puVar27 + -0x3c) = *(undefined4 *)PTR_DAT_0c905220;
  *(undefined4 *)((int)puVar27 + -0x38) = uVar2;
  uVar2 = *(undefined4 *)(PTR_DAT_0c905220 + 0xc);
  dVar38 = dVar48;
  if (in_FPSCR_SZ == '\0') {
    in_dr12 = (double)CONCAT44(uVar3,0x3f800000);
    dVar38 = in_xd12;
  }
  *(undefined4 *)((int)puVar27 + -0x34) = *(undefined4 *)(PTR_DAT_0c905220 + 8);
  *(undefined4 *)((int)puVar27 + -0x30) = uVar2;
  uVar3 = *(undefined4 *)(PTR_DAT_0c905220 + 0x14);
  *(undefined4 *)((int)puVar27 + -0x2c) = *(undefined4 *)(PTR_DAT_0c905220 + 0x10);
  *(undefined4 *)((int)puVar27 + -0x28) = uVar3;
  uVar3 = *(undefined4 *)(PTR_DAT_0c905220 + 0x1c);
  *(undefined4 *)((int)puVar27 + -0x24) = *(undefined4 *)(PTR_DAT_0c905220 + 0x18);
  *(undefined4 *)((int)puVar27 + -0x20) = uVar3;
  uVar3 = *(undefined4 *)(PTR_DAT_0c905220 + 0x24);
  *(undefined4 *)((int)puVar27 + -0x1c) = *(undefined4 *)(PTR_DAT_0c905220 + 0x20);
  *(undefined4 *)((int)puVar27 + -0x18) = uVar3;
  uVar3 = *(undefined4 *)(PTR_DAT_0c905220 + 0x2c);
  *(undefined4 *)((int)puVar27 + -0x14) = *(undefined4 *)(PTR_DAT_0c905220 + 0x28);
  *(undefined4 *)((int)puVar27 + -0x10) = uVar3;
  uVar3 = *(undefined4 *)(PTR_DAT_0c905220 + 0x34);
  *(undefined4 *)((int)puVar27 + -0xc) = *(undefined4 *)(PTR_DAT_0c905220 + 0x30);
  *(undefined4 *)((int)puVar27 + -8) = uVar3;
  *(undefined4 *)((int)puVar27 + -0x6c) = 0xe;
  fVar34 = SUB84(dVar45,0);
  if (in_FPSCR_PR == '\0') {
    dVar47 = (double)(ulonglong)(uint)(SUB84(in_dr12,0) - fVar34);
  }
  else {
    dVar47 = in_dr12 - dVar45;
  }
  uVar24 = SUB84(dVar47,0);
  if (**(char **)(param_5 + 0xec) == '\0') {
    *(undefined4 *)((int)puVar27 + -0x6c) = 0xc;
  }
  dVar50 = dVar53;
  if (in_FPSCR_SZ == '\0') {
    dVar42 = (double)CONCAT44((int)(in_dr2 >> 0x20),fVar33);
    dVar50 = in_xd2;
  }
  fVar44 = (float)((ulonglong)dVar43 >> 0x20);
  if (in_FPSCR_PR == '\0') {
    dVar42 = (double)(ulonglong)(uint)(SUB84(dVar42,0) + fVar44);
  }
  else {
    dVar42 = dVar42 + dVar43;
  }
  iVar12 = 0;
  *(undefined4 *)((int)puVar27 + -0x88) = 0;
  if (in_FPSCR_SZ == '\0') {
    *(int *)((int)puVar27 + -0x78) = SUB84(dVar42,0);
  }
  else {
    *(double *)((int)puVar27 + -0x78) = dVar50;
  }
  if (in_FPSCR_SZ == '\0') {
    dVar39 = (double)CONCAT44(DAT_0c905224,fVar33);
  }
  else {
    dVar39 = (double)CONCAT44(_DAT_0c905228,DAT_0c905224);
  }
  dVar40 = (double)(ulonglong)param_4;
  if (0 < *(int *)((int)puVar27 + -0x6c)) {
    do {
      fVar33 = SUB84(dVar42,0);
      uVar3 = (undefined4)((ulonglong)dVar48 >> 0x20);
      if (in_FPSCR_SZ == '\0') {
        fVar33 = *(float *)((int)puVar27 + -0x78);
        dVar47 = (double)CONCAT44(uVar3,SUB84(dVar47,0));
      }
      else {
        dVar50 = *(double *)((int)puVar27 + -0x78);
        dVar47 = dVar48;
      }
      dVar42 = (double)(ulonglong)(uint)fVar33;
      if (in_FPSCR_PR == '\0') {
        dVar47 = (double)CONCAT44((float)((ulonglong)dVar47 >> 0x20) / fVar33,SUB84(dVar47,0));
      }
      else {
        dVar47 = dVar47 / dVar42;
      }
      fVar33 = SUB84(dVar39,0);
      dVar51 = dVar43;
      dVar52 = dVar53;
      if (in_FPSCR_SZ == '\0') {
        dVar40 = (double)CONCAT44((int)((ulonglong)dVar40 >> 0x20),fVar33);
        dVar48 = (double)CONCAT44(uVar3,fVar44);
        dVar51 = in_xd14;
        dVar52 = in_xd10;
      }
      in_xd10 = dVar52;
      fVar41 = (float)((ulonglong)dVar47 >> 0x20);
      if (in_FPSCR_PR == '\0') {
        bVar54 = fVar41 == 0.0;
        dVar40 = (double)CONCAT44((int)((ulonglong)dVar40 >> 0x20),SUB84(dVar40,0) * fVar41);
        dVar48 = (double)CONCAT44((int)((ulonglong)dVar48 >> 0x20),SUB84(dVar48,0) * fVar41);
      }
      else {
        bVar54 = dVar47 == dVar42;
        dVar40 = dVar40 * dVar47;
        dVar48 = dVar48 * dVar47;
      }
      if (!bVar54) {
        iVar4 = *(int *)((int)puVar27 + *(int *)((int)puVar27 + -0x88) * 4 + -0x3c) * 4;
        *(int *)((int)puVar27 + -0x7c) =
             *(int *)((int)puVar27 + *(int *)((int)puVar27 + -0x88) * 4 + -0x3c);
        if (*(int *)(*(int *)(iVar4 + iVar5) + 4) == 0) {
          return;
        }
        iVar9 = *(int *)(iVar4 + iVar5);
        *(int *)((int)puVar27 + -0x60) = iVar9;
        puVar6 = *(undefined4 **)(iVar9 + 4);
        *(undefined4 *)((int)puVar27 + -0xa0) = puVar6[2];
        pdVar10 = (double *)*puVar6;
        pdVar17 = (double *)**(int **)(*(int *)(*(int *)((int)puVar27 + -0x84) + iVar4) + 4);
        puVar22 = (undefined8 *)**(int **)(*(int *)(*(int *)((int)puVar27 + -0x80) + iVar4) + 4);
        pdVar14 = (double *)**(undefined4 **)(*(int *)(iVar4 + iVar20) + 4);
        pdVar26 = (double *)**(undefined4 **)(*(int *)(iVar4 + iVar18) + 4);
        *(undefined4 *)((int)puVar27 + -0xa8) = 0;
        fVar46 = (float)((ulonglong)dVar45 >> 0x20);
        fVar41 = SUB84(dVar43,0);
        uVar2 = SUB84(dVar48,0);
        uVar3 = SUB84(dVar40,0);
        if (*(int *)((int)puVar27 + -0xa0) != 0) {
          do {
            if (in_FPSCR_SZ == '\0') {
              dVar42 = (double)CONCAT44(*(undefined4 *)pdVar14,SUB84(dVar42,0));
              uVar30 = CONCAT44(*(undefined4 *)puVar22,SUB84(in_dr0,0));
            }
            else {
              dVar42 = *pdVar14;
              uVar30 = *puVar22;
            }
            if (in_FPSCR_PR == '\0') {
              dVar42 = (double)CONCAT44((float)((ulonglong)dVar42 >> 0x20) * fVar44,SUB84(dVar42,0))
              ;
            }
            else {
              dVar42 = dVar42 * dVar43;
            }
            if (in_FPSCR_SZ == '\0') {
              dVar42 = (double)CONCAT44((int)((ulonglong)dVar42 >> 0x20),*(undefined4 *)pdVar26);
              uVar30 = CONCAT44((int)((ulonglong)uVar30 >> 0x20),uVar2);
            }
            else {
              dVar50 = *pdVar26;
              in_xd0 = dVar51;
            }
            fVar29 = (float)((ulonglong)uVar30 >> 0x20);
            if (in_FPSCR_PR == '\0') {
              dVar42 = (double)CONCAT44(fVar29 * fVar41 + (float)((ulonglong)dVar42 >> 0x20),
                                        fVar29 * fVar46 + SUB84(dVar42,0) * fVar33);
            }
            else {
              dVar42 = dVar42 * dVar39;
            }
            dVar52 = in_xd10;
            if (in_FPSCR_SZ == '\0') {
              dVar52 = (double)CONCAT44(uVar3,(int)uVar30);
            }
            if (in_FPSCR_PR == '\0') {
              fVar29 = (float)((ulonglong)dVar52 >> 0x20) * SUB84(dVar42,0) +
                       SUB84(dVar52,0) * (float)((ulonglong)dVar42 >> 0x20);
            }
            else {
              fVar29 = SUB84(dVar52 * dVar42,0);
            }
            if (in_FPSCR_SZ == '\0') {
              *(float *)pdVar10 = fVar29;
              dVar52 = (double)CONCAT44(*(undefined4 *)((int)pdVar14 + 4),
                                        *(undefined4 *)((int)pdVar26 + 4));
              dVar42 = (double)CONCAT44(fVar46,fVar29);
            }
            else {
              *pdVar10 = in_xd0;
              dVar50 = *(double *)((int)pdVar26 + 4);
              dVar52 = *(double *)((int)pdVar14 + 4);
              dVar42 = dVar45;
            }
            if (in_FPSCR_PR == '\0') {
              dVar52 = (double)CONCAT44((int)((ulonglong)dVar52 >> 0x20),SUB84(dVar52,0) * fVar33);
            }
            else {
              dVar52 = dVar52 * dVar39;
            }
            if (in_FPSCR_SZ == '\0') {
              uVar30 = CONCAT44(*(undefined4 *)((int)puVar22 + 4),SUB84(dVar47,0));
            }
            else {
              uVar30 = *(undefined8 *)((int)puVar22 + 4);
            }
            if (in_FPSCR_PR == '\0') {
              dVar52 = (double)CONCAT44((float)((ulonglong)dVar52 >> 0x20) * fVar44,SUB84(dVar52,0))
              ;
            }
            else {
              dVar52 = dVar52 * dVar43;
            }
            dVar37 = dVar51;
            if (in_FPSCR_SZ == '\0') {
              dVar42 = (double)CONCAT44((int)((ulonglong)dVar42 >> 0x20),uVar2);
              dVar37 = in_xd0;
            }
            fVar29 = (float)((ulonglong)uVar30 >> 0x20);
            if (in_FPSCR_PR == '\0') {
              dVar52 = (double)CONCAT44((int)((ulonglong)dVar52 >> 0x20),
                                        (float)((ulonglong)dVar42 >> 0x20) * fVar29 +
                                        SUB84(dVar52,0));
            }
            dVar47 = dVar32;
            if (in_FPSCR_SZ == '\0') {
              dVar47 = (double)CONCAT44(fVar41,SUB84(dVar42,0));
            }
            if (in_FPSCR_PR == '\0') {
              dVar52 = (double)CONCAT44((float)((ulonglong)dVar47 >> 0x20) * fVar29 +
                                        (float)((ulonglong)dVar52 >> 0x20),SUB84(dVar52,0));
            }
            dVar42 = in_xd10;
            if (in_FPSCR_SZ == '\0') {
              dVar42 = (double)CONCAT44(uVar3,SUB84(dVar47,0));
            }
            fVar29 = (float)((ulonglong)dVar52 >> 0x20);
            if (in_FPSCR_PR == '\0') {
              fVar35 = (float)((ulonglong)dVar42 >> 0x20) * SUB84(dVar52,0) +
                       SUB84(dVar42,0) * fVar29;
            }
            else {
              fVar35 = SUB84(dVar42 * dVar52,0);
            }
            if (in_FPSCR_SZ == '\0') {
              *(float *)((int)pdVar10 + 4) = fVar35;
              dVar52 = (double)CONCAT44(fVar29,*(undefined4 *)(pdVar26 + 1));
              dVar47 = (double)CONCAT44(*(undefined4 *)(puVar22 + 1),(int)uVar30);
              dVar42 = (double)CONCAT44(fVar46,fVar35);
            }
            else {
              *(double *)((int)pdVar10 + 4) = dVar37;
              dVar50 = pdVar26[1];
              dVar47 = (double)puVar22[1];
              fVar35 = fVar34;
              dVar42 = dVar45;
            }
            if (in_FPSCR_PR == '\0') {
              fVar29 = SUB84(dVar52,0) * fVar33;
            }
            else {
              fVar29 = SUB84(dVar52 * dVar39,0);
            }
            if (in_FPSCR_SZ == '\0') {
              dVar52 = (double)CONCAT44(*(undefined4 *)(pdVar14 + 1),fVar29);
            }
            else {
              dVar52 = pdVar14[1];
            }
            fVar29 = (float)((ulonglong)dVar47 >> 0x20);
            if (in_FPSCR_PR == '\0') {
              dVar52 = (double)CONCAT44((int)((ulonglong)dVar52 >> 0x20),
                                        (float)((ulonglong)dVar42 >> 0x20) * fVar29 +
                                        SUB84(dVar52,0));
            }
            dVar42 = dVar32;
            if (in_FPSCR_SZ == '\0') {
              dVar42 = (double)CONCAT44(fVar41,fVar35);
            }
            if (in_FPSCR_PR == '\0') {
              dVar52 = (double)CONCAT44((float)((ulonglong)dVar52 >> 0x20) * fVar44,SUB84(dVar52,0))
              ;
            }
            else {
              dVar52 = dVar52 * dVar43;
            }
            in_xd0 = dVar51;
            if (in_FPSCR_SZ == '\0') {
              dVar42 = (double)CONCAT44((int)((ulonglong)dVar42 >> 0x20),uVar2);
              in_xd0 = dVar37;
            }
            if (in_FPSCR_PR == '\0') {
              dVar52 = (double)CONCAT44((float)((ulonglong)dVar42 >> 0x20) * fVar29 +
                                        (float)((ulonglong)dVar52 >> 0x20),SUB84(dVar52,0));
            }
            dVar37 = in_xd10;
            if (in_FPSCR_SZ == '\0') {
              dVar37 = (double)CONCAT44(uVar3,SUB84(dVar42,0));
            }
            if (in_FPSCR_PR == '\0') {
              fVar29 = (float)((ulonglong)dVar37 >> 0x20) * SUB84(dVar52,0) +
                       SUB84(dVar37,0) * (float)((ulonglong)dVar52 >> 0x20);
            }
            else {
              fVar29 = SUB84(dVar37 * dVar52,0);
            }
            if (in_FPSCR_SZ == '\0') {
              *(float *)(pdVar10 + 1) = fVar29;
              dVar42 = (double)CONCAT44(*(undefined4 *)pdVar17,*(undefined4 *)pdVar10);
              in_dr0 = (double)CONCAT44(SUB84(dVar47,0),fVar29);
            }
            else {
              pdVar10[1] = in_xd0;
              dVar50 = *pdVar10;
              dVar42 = *pdVar17;
              in_dr0 = dVar38;
            }
            fVar29 = (float)((ulonglong)in_dr0 >> 0x20);
            if (in_FPSCR_PR == '\0') {
              fVar35 = fVar29 * (float)((ulonglong)dVar42 >> 0x20) + SUB84(dVar42,0) * fVar34;
            }
            else {
              fVar35 = SUB84(dVar42 * dVar45,0);
            }
            if (in_FPSCR_SZ == '\0') {
              *(float *)pdVar10 = fVar35;
              dVar42 = (double)CONCAT44(*(undefined4 *)((int)pdVar17 + 4),
                                        *(undefined4 *)((int)pdVar10 + 4));
            }
            else {
              *pdVar10 = dVar50;
              dVar50 = *(double *)((int)pdVar10 + 4);
              dVar42 = *(double *)((int)pdVar17 + 4);
            }
            if (in_FPSCR_PR == '\0') {
              fVar35 = fVar29 * (float)((ulonglong)dVar42 >> 0x20) + SUB84(dVar42,0) * fVar34;
            }
            else {
              fVar35 = SUB84(dVar42 * dVar45,0);
            }
            if (in_FPSCR_SZ == '\0') {
              *(float *)((int)pdVar10 + 4) = fVar35;
              dVar42 = (double)CONCAT44(*(undefined4 *)(pdVar17 + 1),*(undefined4 *)(pdVar10 + 1));
            }
            else {
              *(double *)((int)pdVar10 + 4) = dVar50;
              dVar50 = pdVar10[1];
              dVar42 = pdVar17[1];
            }
            if (in_FPSCR_PR == '\0') {
              fVar35 = (float)((ulonglong)dVar42 >> 0x20);
              dVar42 = (double)CONCAT44(fVar35,fVar29 * fVar35 + SUB84(dVar42,0) * fVar34);
            }
            else {
              dVar42 = dVar42 * dVar45;
            }
            if (in_FPSCR_SZ == '\0') {
              *(int *)(pdVar10 + 1) = SUB84(dVar42,0);
            }
            else {
              pdVar10[1] = dVar50;
            }
            pdVar10 = (double *)((int)pdVar10 + 0xc);
            iVar9 = *(int *)((int)puVar27 + -0xa8);
            *(uint *)((int)puVar27 + -0xa8) = iVar9 + 1U;
            pdVar14 = (double *)((int)pdVar14 + 0xc);
            puVar22 = (undefined8 *)((int)puVar22 + 0xc);
            pdVar26 = (double *)((int)pdVar26 + 0xc);
            pdVar17 = (double *)((int)pdVar17 + 0xc);
          } while (iVar9 + 1U < *(uint *)((int)puVar27 + -0xa0));
        }
        iVar9 = *(int *)(iVar4 + iVar5);
        *(int *)((int)puVar27 + -100) = iVar9;
        iVar9 = *(int *)(iVar9 + 4);
        *(undefined4 *)((int)puVar27 + -0xa0) = *(undefined4 *)(iVar9 + 8);
        pdVar10 = *(double **)(iVar9 + 4);
        pdVar17 = *(double **)(*(int *)(*(int *)(*(int *)((int)puVar27 + -0x84) + iVar4) + 4) + 4);
        puVar22 = *(undefined8 **)
                   (*(int *)(*(int *)(*(int *)((int)puVar27 + -0x80) + iVar4) + 4) + 4);
        pdVar14 = *(double **)(*(int *)(*(int *)(iVar4 + iVar20) + 4) + 4);
        pdVar26 = *(double **)(*(int *)(*(int *)(iVar4 + iVar18) + 4) + 4);
        *(undefined4 *)((int)puVar27 + -0xa8) = 0;
        if (*(int *)((int)puVar27 + -0xa0) != 0) {
          do {
            if (in_FPSCR_SZ == '\0') {
              dVar42 = (double)CONCAT44(*(undefined4 *)pdVar14,SUB84(dVar42,0));
              uVar30 = CONCAT44(*(undefined4 *)puVar22,SUB84(in_dr0,0));
            }
            else {
              dVar42 = *pdVar14;
              uVar30 = *puVar22;
            }
            if (in_FPSCR_PR == '\0') {
              dVar42 = (double)CONCAT44((float)((ulonglong)dVar42 >> 0x20) * fVar44,SUB84(dVar42,0))
              ;
            }
            else {
              dVar42 = dVar42 * dVar43;
            }
            if (in_FPSCR_SZ == '\0') {
              dVar42 = (double)CONCAT44((int)((ulonglong)dVar42 >> 0x20),*(undefined4 *)pdVar26);
              uVar30 = CONCAT44((int)((ulonglong)uVar30 >> 0x20),uVar2);
            }
            else {
              dVar50 = *pdVar26;
              in_xd0 = dVar51;
            }
            fVar29 = (float)((ulonglong)uVar30 >> 0x20);
            if (in_FPSCR_PR == '\0') {
              dVar42 = (double)CONCAT44(fVar29 * fVar41 + (float)((ulonglong)dVar42 >> 0x20),
                                        fVar29 * fVar46 + SUB84(dVar42,0) * fVar33);
            }
            else {
              dVar42 = dVar42 * dVar39;
            }
            dVar52 = in_xd10;
            if (in_FPSCR_SZ == '\0') {
              dVar52 = (double)CONCAT44(uVar3,(int)uVar30);
            }
            if (in_FPSCR_PR == '\0') {
              fVar29 = (float)((ulonglong)dVar52 >> 0x20) * SUB84(dVar42,0) +
                       SUB84(dVar52,0) * (float)((ulonglong)dVar42 >> 0x20);
            }
            else {
              fVar29 = SUB84(dVar52 * dVar42,0);
            }
            if (in_FPSCR_SZ == '\0') {
              *(float *)pdVar10 = fVar29;
              dVar52 = (double)CONCAT44(*(undefined4 *)((int)pdVar14 + 4),
                                        *(undefined4 *)((int)pdVar26 + 4));
              dVar42 = (double)CONCAT44(fVar46,fVar29);
            }
            else {
              *pdVar10 = in_xd0;
              dVar50 = *(double *)((int)pdVar26 + 4);
              dVar52 = *(double *)((int)pdVar14 + 4);
              dVar42 = dVar45;
            }
            if (in_FPSCR_PR == '\0') {
              dVar52 = (double)CONCAT44((int)((ulonglong)dVar52 >> 0x20),SUB84(dVar52,0) * fVar33);
            }
            else {
              dVar52 = dVar52 * dVar39;
            }
            if (in_FPSCR_SZ == '\0') {
              uVar30 = CONCAT44(*(undefined4 *)((int)puVar22 + 4),SUB84(dVar47,0));
            }
            else {
              uVar30 = *(undefined8 *)((int)puVar22 + 4);
            }
            if (in_FPSCR_PR == '\0') {
              dVar52 = (double)CONCAT44((float)((ulonglong)dVar52 >> 0x20) * fVar44,SUB84(dVar52,0))
              ;
            }
            else {
              dVar52 = dVar52 * dVar43;
            }
            dVar37 = dVar51;
            if (in_FPSCR_SZ == '\0') {
              dVar42 = (double)CONCAT44((int)((ulonglong)dVar42 >> 0x20),uVar2);
              dVar37 = in_xd0;
            }
            fVar29 = (float)((ulonglong)uVar30 >> 0x20);
            if (in_FPSCR_PR == '\0') {
              dVar52 = (double)CONCAT44((int)((ulonglong)dVar52 >> 0x20),
                                        (float)((ulonglong)dVar42 >> 0x20) * fVar29 +
                                        SUB84(dVar52,0));
            }
            dVar47 = dVar32;
            if (in_FPSCR_SZ == '\0') {
              dVar47 = (double)CONCAT44(fVar41,SUB84(dVar42,0));
            }
            if (in_FPSCR_PR == '\0') {
              dVar52 = (double)CONCAT44((float)((ulonglong)dVar47 >> 0x20) * fVar29 +
                                        (float)((ulonglong)dVar52 >> 0x20),SUB84(dVar52,0));
            }
            dVar42 = in_xd10;
            if (in_FPSCR_SZ == '\0') {
              dVar42 = (double)CONCAT44(uVar3,SUB84(dVar47,0));
            }
            fVar29 = (float)((ulonglong)dVar52 >> 0x20);
            if (in_FPSCR_PR == '\0') {
              fVar35 = (float)((ulonglong)dVar42 >> 0x20) * SUB84(dVar52,0) +
                       SUB84(dVar42,0) * fVar29;
            }
            else {
              fVar35 = SUB84(dVar42 * dVar52,0);
            }
            if (in_FPSCR_SZ == '\0') {
              *(float *)((int)pdVar10 + 4) = fVar35;
              dVar52 = (double)CONCAT44(fVar29,*(undefined4 *)(pdVar26 + 1));
              dVar47 = (double)CONCAT44(*(undefined4 *)(puVar22 + 1),(int)uVar30);
              dVar42 = (double)CONCAT44(fVar46,fVar35);
            }
            else {
              *(double *)((int)pdVar10 + 4) = dVar37;
              dVar50 = pdVar26[1];
              dVar47 = (double)puVar22[1];
              fVar35 = fVar34;
              dVar42 = dVar45;
            }
            if (in_FPSCR_PR == '\0') {
              fVar29 = SUB84(dVar52,0) * fVar33;
            }
            else {
              fVar29 = SUB84(dVar52 * dVar39,0);
            }
            if (in_FPSCR_SZ == '\0') {
              dVar52 = (double)CONCAT44(*(undefined4 *)(pdVar14 + 1),fVar29);
            }
            else {
              dVar52 = pdVar14[1];
            }
            fVar29 = (float)((ulonglong)dVar47 >> 0x20);
            if (in_FPSCR_PR == '\0') {
              dVar52 = (double)CONCAT44((int)((ulonglong)dVar52 >> 0x20),
                                        (float)((ulonglong)dVar42 >> 0x20) * fVar29 +
                                        SUB84(dVar52,0));
            }
            dVar42 = dVar32;
            if (in_FPSCR_SZ == '\0') {
              dVar42 = (double)CONCAT44(fVar41,fVar35);
            }
            if (in_FPSCR_PR == '\0') {
              dVar52 = (double)CONCAT44((float)((ulonglong)dVar52 >> 0x20) * fVar44,SUB84(dVar52,0))
              ;
            }
            else {
              dVar52 = dVar52 * dVar43;
            }
            in_xd0 = dVar51;
            if (in_FPSCR_SZ == '\0') {
              dVar42 = (double)CONCAT44((int)((ulonglong)dVar42 >> 0x20),uVar2);
              in_xd0 = dVar37;
            }
            if (in_FPSCR_PR == '\0') {
              dVar52 = (double)CONCAT44((float)((ulonglong)dVar42 >> 0x20) * fVar29 +
                                        (float)((ulonglong)dVar52 >> 0x20),SUB84(dVar52,0));
            }
            dVar37 = in_xd10;
            if (in_FPSCR_SZ == '\0') {
              dVar37 = (double)CONCAT44(uVar3,SUB84(dVar42,0));
            }
            if (in_FPSCR_PR == '\0') {
              fVar29 = (float)((ulonglong)dVar37 >> 0x20) * SUB84(dVar52,0) +
                       SUB84(dVar37,0) * (float)((ulonglong)dVar52 >> 0x20);
            }
            else {
              fVar29 = SUB84(dVar37 * dVar52,0);
            }
            if (in_FPSCR_SZ == '\0') {
              *(float *)(pdVar10 + 1) = fVar29;
              dVar42 = (double)CONCAT44(*(undefined4 *)pdVar17,*(undefined4 *)pdVar10);
              in_dr0 = (double)CONCAT44(SUB84(dVar47,0),fVar29);
            }
            else {
              pdVar10[1] = in_xd0;
              dVar50 = *pdVar10;
              dVar42 = *pdVar17;
              in_dr0 = dVar38;
            }
            fVar29 = (float)((ulonglong)in_dr0 >> 0x20);
            if (in_FPSCR_PR == '\0') {
              fVar35 = fVar29 * (float)((ulonglong)dVar42 >> 0x20) + SUB84(dVar42,0) * fVar34;
            }
            else {
              fVar35 = SUB84(dVar42 * dVar45,0);
            }
            if (in_FPSCR_SZ == '\0') {
              *(float *)pdVar10 = fVar35;
              dVar42 = (double)CONCAT44(*(undefined4 *)((int)pdVar17 + 4),
                                        *(undefined4 *)((int)pdVar10 + 4));
            }
            else {
              *pdVar10 = dVar50;
              dVar50 = *(double *)((int)pdVar10 + 4);
              dVar42 = *(double *)((int)pdVar17 + 4);
            }
            if (in_FPSCR_PR == '\0') {
              fVar35 = fVar29 * (float)((ulonglong)dVar42 >> 0x20) + SUB84(dVar42,0) * fVar34;
            }
            else {
              fVar35 = SUB84(dVar42 * dVar45,0);
            }
            if (in_FPSCR_SZ == '\0') {
              *(float *)((int)pdVar10 + 4) = fVar35;
              dVar42 = (double)CONCAT44(*(undefined4 *)(pdVar17 + 1),*(undefined4 *)(pdVar10 + 1));
            }
            else {
              *(double *)((int)pdVar10 + 4) = dVar50;
              dVar50 = pdVar10[1];
              dVar42 = pdVar17[1];
            }
            if (in_FPSCR_PR == '\0') {
              fVar35 = (float)((ulonglong)dVar42 >> 0x20);
              dVar42 = (double)CONCAT44(fVar35,fVar29 * fVar35 + SUB84(dVar42,0) * fVar34);
            }
            else {
              dVar42 = dVar42 * dVar45;
            }
            if (in_FPSCR_SZ == '\0') {
              *(int *)(pdVar10 + 1) = SUB84(dVar42,0);
            }
            else {
              pdVar10[1] = dVar50;
            }
            pdVar10 = (double *)((int)pdVar10 + 0xc);
            iVar9 = *(int *)((int)puVar27 + -0xa8);
            *(uint *)((int)puVar27 + -0xa8) = iVar9 + 1U;
            pdVar14 = (double *)((int)pdVar14 + 0xc);
            puVar22 = (undefined8 *)((int)puVar22 + 0xc);
            pdVar26 = (double *)((int)pdVar26 + 0xc);
            pdVar17 = (double *)((int)pdVar17 + 0xc);
          } while (iVar9 + 1U < *(uint *)((int)puVar27 + -0xa0));
        }
        if (*(int *)((int)puVar27 + -0x7c) != 0x1b) {
          iVar9 = *(int *)(iVar4 + iVar5);
          *(int *)((int)puVar27 + -0x68) = iVar9;
          iVar9 = *(int *)(iVar9 + 4);
          *(uint *)((int)puVar27 + -0xa8) = (uint)*(ushort *)(iVar9 + 0x16);
          puVar7 = *(undefined1 **)(iVar9 + 0x10);
          pbVar15 = *(byte **)(*(int *)(*(int *)(*(int *)((int)puVar27 + -0x84) + iVar4) + 4) + 0x10
                              );
          pbVar19 = *(byte **)(*(int *)(*(int *)(*(int *)((int)puVar27 + -0x80) + iVar4) + 4) + 0x10
                              );
          pbVar21 = *(byte **)(*(int *)(*(int *)(iVar4 + iVar20) + 4) + 0x10);
          pbVar23 = *(byte **)(*(int *)(*(int *)(iVar4 + iVar18) + 4) + 0x10);
          *(uint *)((int)puVar27 + -0x7c) = (uint)(*(int *)((int)puVar27 + -0x88) == 0);
          uVar24 = 0;
          if (*(int *)((int)puVar27 + -0xa8) != 0) {
            do {
              uVar36 = SUB84(in_dr0,0);
              if ((*(ushort *)(*(int *)(param_5 + 0xec) + 0x36) & 2) == 0) {
                dVar52 = dVar51;
                if (in_FPSCR_SZ == '\0') {
                  dVar52 = in_xd0;
                  uVar36 = uVar2;
                }
                if (in_FPSCR_PR == '\0') {
                  dVar42 = (double)CONCAT44((float)pbVar19[2],SUB84(dVar47,0));
                }
                else {
                  dVar42 = (double)pbVar19[2];
                }
                bVar1 = pbVar19[1];
                dVar47 = dVar42;
                if (in_FPSCR_SZ == '\0') {
                  dVar47 = (double)CONCAT44((int)((ulonglong)dVar42 >> 0x20),uVar36);
                }
                fVar29 = (float)((ulonglong)dVar47 >> 0x20);
                if (in_FPSCR_PR == '\0') {
                  dVar37 = (double)CONCAT44(fVar29 * fVar41 + (float)pbVar21[2] * fVar44,
                                            fVar29 * fVar46 + (float)pbVar23[2] * fVar33);
                }
                else {
                  dVar37 = (double)pbVar21[2] * dVar39 * dVar43;
                }
                dVar49 = in_xd10;
                if (in_FPSCR_SZ == '\0') {
                  dVar49 = (double)CONCAT44(uVar3,SUB84(dVar47,0));
                }
                if (in_FPSCR_PR == '\0') {
                  fVar29 = (float)((ulonglong)dVar49 >> 0x20) * SUB84(dVar37,0) +
                           SUB84(dVar49,0) * (float)((ulonglong)dVar37 >> 0x20);
                }
                else {
                  fVar29 = SUB84(dVar49 * dVar37,0);
                }
                if (in_FPSCR_SZ == '\0') {
                  *(float *)((int)puVar27 + -0x74) = (float)bVar1;
                  dVar42 = (double)CONCAT44(fVar29,SUB84(dVar42,0));
                }
                else {
                  *(double *)((int)puVar27 + -0x74) = dVar50;
                  dVar42 = dVar52;
                }
                if (in_FPSCR_PR == '\0') {
                  dVar47 = (double)CONCAT44((float)pbVar23[1],(float)bVar1);
                }
                else {
                  dVar47 = (double)pbVar23[1];
                }
                if (in_FPSCR_PR == '\0') {
                  dVar47 = (double)CONCAT44((float)((ulonglong)dVar47 >> 0x20) * fVar33,
                                            SUB84(dVar47,0));
                }
                else {
                  dVar47 = dVar47 * dVar39;
                }
                dVar37 = dVar45;
                if (in_FPSCR_SZ == '\0') {
                  dVar37 = (double)((ulonglong)(uint)fVar46 << 0x20);
                }
                if (in_FPSCR_PR == '\0') {
                  dVar47 = (double)CONCAT44((float)((ulonglong)dVar37 >> 0x20) * SUB84(dVar47,0) +
                                            (float)((ulonglong)dVar47 >> 0x20),SUB84(dVar47,0));
                }
                fVar29 = fVar41;
                if (in_FPSCR_SZ != '\0') {
                  fVar29 = (float)((ulonglong)dVar32 >> 0x20);
                }
                if (in_FPSCR_PR == '\0') {
                  fVar29 = fVar29 * SUB84(dVar47,0) + (float)pbVar21[1] * fVar44;
                }
                else {
                  fVar29 = SUB84((double)CONCAT44(fVar29,(float)pbVar21[1]) * dVar43,0);
                }
                dVar49 = in_xd10;
                dVar37 = dVar51;
                if (in_FPSCR_SZ == '\0') {
                  dVar47 = (double)CONCAT44((int)((ulonglong)dVar47 >> 0x20),uVar2);
                  dVar49 = (double)CONCAT44(uVar3,fVar29);
                  dVar37 = dVar50;
                }
                if (in_FPSCR_PR == '\0') {
                  fVar29 = (float)((ulonglong)dVar49 >> 0x20) * (float)((ulonglong)dVar47 >> 0x20) +
                           SUB84(dVar47,0) * SUB84(dVar49,0);
                }
                else {
                  fVar29 = SUB84(dVar47 * dVar49,0);
                }
                if (in_FPSCR_SZ == '\0') {
                  *(float *)((int)puVar27 + -0x94) = fVar29;
                  dVar47 = (double)((ulonglong)(uint)fVar46 << 0x20);
                }
                else {
                  *(double *)((int)puVar27 + -0x94) = dVar37;
                  dVar47 = dVar45;
                }
                fVar29 = (float)*pbVar19;
                if (in_FPSCR_SZ == '\0') {
                  *(float *)((int)puVar27 + -0x70) = fVar29;
                }
                else {
                  *(double *)((int)puVar27 + -0x70) = dVar37;
                }
                if (in_FPSCR_PR == '\0') {
                  dVar47 = (double)CONCAT44((float)((ulonglong)dVar47 >> 0x20) * fVar29 +
                                            (float)*pbVar23 * fVar33,fVar29);
                }
                else {
                  dVar47 = (double)*pbVar23 * dVar39;
                }
                dVar50 = dVar32;
                if (in_FPSCR_SZ == '\0') {
                  dVar50 = (double)CONCAT44(fVar41,(float)*pbVar21);
                }
                fVar29 = (float)((ulonglong)dVar39 >> 0x20);
                fVar35 = (float)((ulonglong)dVar42 >> 0x20);
                if (in_FPSCR_PR == '\0') {
                  bVar54 = fVar29 < fVar35;
                  fVar28 = (float)((ulonglong)dVar50 >> 0x20) * SUB84(dVar47,0) +
                           SUB84(dVar50,0) * fVar44;
                }
                else {
                  bVar54 = dVar39 < dVar42;
                  fVar28 = SUB84(dVar50 * dVar43,0);
                }
                dVar31 = in_xd10;
                dVar49 = dVar51;
                if (in_FPSCR_SZ == '\0') {
                  dVar47 = (double)CONCAT44((int)((ulonglong)dVar47 >> 0x20),uVar2);
                  dVar31 = (double)CONCAT44(uVar3,fVar28);
                  dVar49 = dVar37;
                }
                if (in_FPSCR_PR == '\0') {
                  fVar28 = (float)((ulonglong)dVar47 >> 0x20);
                  dVar47 = (double)CONCAT44(fVar28,(float)((ulonglong)dVar31 >> 0x20) * fVar28 +
                                                   SUB84(dVar47,0) * SUB84(dVar31,0));
                }
                else {
                  dVar47 = dVar47 * dVar31;
                }
                if (in_FPSCR_SZ == '\0') {
                  *(int *)((int)puVar27 + -0x98) = SUB84(dVar47,0);
                }
                else {
                  *(double *)((int)puVar27 + -0x98) = dVar49;
                }
                uVar36 = (undefined4)((ulonglong)dVar40 >> 0x20);
                if (bVar54) {
                  dVar37 = dVar39;
                  if (in_FPSCR_SZ == '\0') {
                    dVar37 = (double)CONCAT44(fVar29,SUB84(dVar42,0));
                  }
                }
                else {
                  dVar47 = (double)((ulonglong)dVar47 & 0xffffffff);
                  if (in_FPSCR_PR == '\0') {
                    bVar54 = fVar35 < 0.0;
                  }
                  else {
                    bVar54 = dVar42 < dVar47;
                  }
                  dVar37 = dVar42;
                  if ((bVar54) && (dVar37 = dVar40, in_FPSCR_SZ == '\0')) {
                    dVar37 = (double)CONCAT44(uVar36,SUB84(dVar42,0));
                  }
                }
                if (in_FPSCR_SZ == '\0') {
                  dVar47 = (double)CONCAT44((int)((ulonglong)dVar47 >> 0x20),
                                            *(undefined4 *)((int)puVar27 + -0x94));
                }
                else {
                  dVar49 = *(double *)((int)puVar27 + -0x94);
                }
                fVar35 = SUB84(dVar47,0);
                if (in_FPSCR_PR == '\0') {
                  bVar54 = fVar29 < fVar35;
                }
                else {
                  bVar54 = dVar39 < dVar47;
                }
                if (bVar54) {
                  if (in_FPSCR_SZ == '\0') {
                    *(float *)((int)puVar27 + -0x94) = fVar29;
                  }
                  else {
                    *(double *)((int)puVar27 + -0x94) = dVar39;
                  }
                }
                else {
                  dVar47 = (double)((ulonglong)dVar47 & 0xffffffff);
                  if (in_FPSCR_PR == '\0' && fVar35 < 0.0) {
                    if (in_FPSCR_SZ == '\0') {
                      *(undefined4 *)((int)puVar27 + -0x94) = uVar36;
                    }
                    else {
                      *(double *)((int)puVar27 + -0x94) = dVar40;
                    }
                  }
                }
                if (in_FPSCR_SZ == '\0') {
                  dVar47 = (double)CONCAT44((int)((ulonglong)dVar47 >> 0x20),
                                            *(undefined4 *)((int)puVar27 + -0x98));
                }
                else {
                  dVar49 = *(double *)((int)puVar27 + -0x98);
                }
                fVar35 = SUB84(dVar47,0);
                if (in_FPSCR_PR == '\0') {
                  bVar54 = fVar29 < fVar35;
                }
                else {
                  bVar54 = dVar39 < dVar47;
                }
                if (bVar54) {
                  if (in_FPSCR_SZ == '\0') {
                    *(float *)((int)puVar27 + -0x98) = fVar29;
                  }
                  else {
                    *(double *)((int)puVar27 + -0x98) = dVar39;
                  }
                }
                else {
                  dVar47 = (double)((ulonglong)dVar47 & 0xffffffff);
                  if (in_FPSCR_PR == '\0' && fVar35 < 0.0) {
                    if (in_FPSCR_SZ == '\0') {
                      *(undefined4 *)((int)puVar27 + -0x98) = uVar36;
                    }
                    else {
                      *(double *)((int)puVar27 + -0x98) = dVar40;
                    }
                  }
                }
                dVar50 = in_xd8;
                if (in_FPSCR_SZ == '\0') {
                  dVar47 = (double)CONCAT44((int)((ulonglong)dVar47 >> 0x20),fVar34);
                  dVar50 = dVar49;
                }
                fVar35 = (float)((ulonglong)dVar48 >> 0x20);
                if (in_FPSCR_PR == '\0') {
                  bVar54 = fVar35 < SUB84(dVar47,0);
                }
                else {
                  bVar54 = dVar48 < dVar47;
                }
                if (in_FPSCR_SZ == '\0') {
                  *(float *)((int)puVar27 + -0xa4) = fVar34;
                }
                else {
                  *(double *)((int)puVar27 + -0xa4) = in_xd8;
                }
                if (bVar54) {
                  if (in_FPSCR_SZ == '\0') {
                    *(float *)((int)puVar27 + -0xa4) = fVar35;
                  }
                  else {
                    *(double *)((int)puVar27 + -0xa4) = dVar48;
                  }
                }
                if (in_FPSCR_SZ == '\0') {
                  dVar42 = (double)CONCAT44(fVar35,*(undefined4 *)((int)puVar27 + -0xa4));
                }
                else {
                  dVar50 = *(double *)((int)puVar27 + -0xa4);
                  dVar42 = dVar48;
                }
                if (in_FPSCR_PR == '\0') {
                  dVar42 = (double)CONCAT44((float)((ulonglong)dVar42 >> 0x20) - SUB84(dVar42,0),
                                            SUB84(dVar42,0));
                }
                else {
                  dVar42 = dVar42 - dVar42;
                }
                fVar35 = (float)((ulonglong)dVar42 >> 0x20);
                if (in_FPSCR_SZ == '\0') {
                  *(float *)((int)puVar27 + -0x9c) = fVar35;
                }
                else {
                  *(double *)((int)puVar27 + -0x9c) = dVar42;
                }
                if (in_FPSCR_SZ == '\0') {
                  dVar42 = (double)CONCAT44(*(undefined4 *)((int)puVar27 + -0xa4),SUB84(dVar31,0));
                }
                else {
                  dVar42 = *(double *)((int)puVar27 + -0xa4);
                }
                if (in_FPSCR_PR == '\0') {
                  dVar37 = (double)CONCAT44((float)((ulonglong)dVar37 >> 0x20) *
                                            (float)((ulonglong)dVar42 >> 0x20),SUB84(dVar37,0));
                }
                else {
                  dVar37 = dVar37 * dVar42;
                }
                fVar28 = (float)pbVar15[2];
                if (in_FPSCR_SZ == '\0') {
                  fVar28 = SUB84(dVar42,0);
                }
                if (in_FPSCR_PR == '\0') {
                  fVar35 = fVar35 * (float)pbVar15[2] + (float)((ulonglong)dVar37 >> 0x20);
                  dVar37 = (double)CONCAT44(fVar35,SUB84(dVar37,0));
                  uVar57 = (uint)fVar35;
                }
                else {
                  uVar57 = (uint)dVar37;
                }
                if (in_FPSCR_SZ == '\0') {
                  *(float *)((int)puVar27 + -0x90) = (float)(uVar57 & 0xff);
                }
                else {
                  *(double *)((int)puVar27 + -0x90) = dVar50;
                }
                if (in_FPSCR_SZ == '\0') {
                  dVar42 = (double)CONCAT44(*(undefined4 *)((int)puVar27 + -0xa4),fVar28);
                  fVar35 = *(float *)((int)puVar27 + -0x94);
                }
                else {
                  dVar42 = *(double *)((int)puVar27 + -0xa4);
                  fVar35 = (float)((ulonglong)*(undefined8 *)((int)puVar27 + -0x94) >> 0x20);
                }
                if (in_FPSCR_PR == '\0') {
                  dVar47 = (double)CONCAT44(fVar35 * (float)((ulonglong)dVar42 >> 0x20),
                                            (float)pbVar15[1]);
                }
                else {
                  dVar47 = (double)CONCAT44(fVar35,(float)pbVar15[1]) * dVar42;
                }
                if (in_FPSCR_SZ == '\0') {
                  uVar30 = CONCAT44(*(undefined4 *)((int)puVar27 + -0x9c),SUB84(dVar42,0));
                }
                else {
                  uVar30 = *(undefined8 *)((int)puVar27 + -0x9c);
                }
                if (in_FPSCR_PR == '\0') {
                  dVar47 = (double)CONCAT44((float)((ulonglong)uVar30 >> 0x20) * SUB84(dVar47,0) +
                                            (float)((ulonglong)dVar47 >> 0x20),SUB84(dVar47,0));
                }
                if (in_FPSCR_SZ == '\0') {
                  dVar42 = (double)CONCAT44(*(undefined4 *)((int)puVar27 + -0xa4),(int)uVar30);
                }
                else {
                  dVar42 = *(double *)((int)puVar27 + -0xa4);
                }
                if (in_FPSCR_PR == '\0') {
                  uVar57 = (uint)(float)((ulonglong)dVar47 >> 0x20);
                }
                else {
                  uVar57 = (uint)dVar47;
                }
                if (in_FPSCR_SZ == '\0') {
                  dVar47 = (double)CONCAT44(*(undefined4 *)((int)puVar27 + -0x98),SUB84(dVar47,0));
                }
                else {
                  dVar47 = *(double *)((int)puVar27 + -0x98);
                }
                if (in_FPSCR_PR == '\0') {
                  fVar35 = (float)((ulonglong)dVar47 >> 0x20) * (float)((ulonglong)dVar42 >> 0x20);
                }
                else {
                  fVar35 = (float)((ulonglong)(dVar47 * dVar42) >> 0x20);
                }
                if (in_FPSCR_SZ == '\0') {
                  in_dr0 = (double)CONCAT44(*(undefined4 *)((int)puVar27 + -0x9c),SUB84(dVar42,0));
                }
                else {
                  in_dr0 = *(double *)((int)puVar27 + -0x9c);
                }
                if (in_FPSCR_PR == '\0') {
                  dVar37 = (double)CONCAT44((float)(uVar57 & 0xff),SUB84(dVar37,0));
                }
                else {
                  dVar37 = (double)(uVar57 & 0xff);
                }
                fVar28 = (float)((ulonglong)in_dr0 >> 0x20);
                if (in_FPSCR_PR == '\0') {
                  uVar57 = (uint)(fVar28 * (float)*pbVar15 + fVar35);
                }
                else {
                  uVar57 = (uint)(double)CONCAT44(fVar35,(float)*pbVar15);
                }
                if (in_FPSCR_SZ == '\0') {
                  *(float *)((int)puVar27 + -0x8c) = (float)(uVar57 & 0xff);
                  dVar42 = (double)CONCAT44(*(undefined4 *)((int)puVar27 + -0x90),
                                            (float)(uVar57 & 0xff));
                }
                else {
                  *(double *)((int)puVar27 + -0x8c) = dVar50;
                  dVar42 = *(double *)((int)puVar27 + -0x90);
                }
                fVar35 = (float)((ulonglong)dVar42 >> 0x20);
                if (in_FPSCR_PR == '\0') {
                  bVar54 = fVar29 < fVar35;
                }
                else {
                  bVar54 = dVar39 < dVar42;
                }
                if (bVar54) {
                  if (in_FPSCR_SZ == '\0') {
                    *(float *)((int)puVar27 + -0x90) = fVar29;
                  }
                  else {
                    *(double *)((int)puVar27 + -0x90) = dVar39;
                  }
                }
                else {
                  in_dr0 = (double)((ulonglong)(uint)fVar28 << 0x20);
                  if (in_FPSCR_PR == '\0') {
                    bVar54 = fVar35 < 0.0;
                  }
                  else {
                    bVar54 = dVar42 < in_dr0;
                  }
                  if (bVar54) {
                    if (in_FPSCR_SZ == '\0') {
                      *(undefined4 *)((int)puVar27 + -0x90) = uVar36;
                    }
                    else {
                      *(double *)((int)puVar27 + -0x90) = dVar40;
                    }
                  }
                }
                fVar28 = (float)((ulonglong)dVar37 >> 0x20);
                if (in_FPSCR_PR == '\0') {
                  bVar54 = fVar29 < fVar28;
                }
                else {
                  bVar54 = dVar39 < dVar37;
                }
                if (bVar54) {
                  dVar47 = dVar39;
                  if (in_FPSCR_SZ == '\0') {
                    dVar47 = (double)CONCAT44(fVar29,SUB84(dVar37,0));
                  }
                }
                else {
                  dVar42 = (double)((ulonglong)(uint)fVar35 << 0x20);
                  if (in_FPSCR_PR == '\0') {
                    bVar54 = fVar28 < 0.0;
                  }
                  else {
                    bVar54 = dVar37 < dVar42;
                  }
                  dVar47 = dVar37;
                  if ((bVar54) && (dVar47 = dVar40, in_FPSCR_SZ == '\0')) {
                    dVar47 = (double)CONCAT44(uVar36,SUB84(dVar37,0));
                  }
                }
                if (in_FPSCR_SZ == '\0') {
                  dVar42 = (double)CONCAT44((int)((ulonglong)dVar42 >> 0x20),
                                            *(undefined4 *)((int)puVar27 + -0x8c));
                }
                else {
                  dVar50 = *(double *)((int)puVar27 + -0x8c);
                }
                fVar35 = SUB84(dVar42,0);
                if (in_FPSCR_PR == '\0') {
                  bVar54 = fVar29 < fVar35;
                }
                else {
                  bVar54 = dVar39 < dVar42;
                }
                in_xd0 = dVar52;
                if (bVar54) {
                  if (in_FPSCR_SZ == '\0') {
                    *(float *)((int)puVar27 + -0x8c) = fVar29;
                  }
                  else {
                    *(double *)((int)puVar27 + -0x8c) = dVar39;
                  }
                }
                else {
                  dVar42 = (double)((ulonglong)dVar42 & 0xffffffff);
                  if (in_FPSCR_PR == '\0' && fVar35 < 0.0) {
                    if (in_FPSCR_SZ == '\0') {
                      *(undefined4 *)((int)puVar27 + -0x8c) = uVar36;
                    }
                    else {
                      *(double *)((int)puVar27 + -0x8c) = dVar40;
                    }
                  }
                }
              }
              else if (in_FPSCR_SZ == '\0') {
                dVar42 = (double)CONCAT44((int)((ulonglong)dVar42 >> 0x20),_DAT_0c905228);
                *(undefined4 *)((int)puVar27 + -0x90) = _DAT_0c905228;
                dVar47 = (double)CONCAT44(_DAT_0c905228,SUB84(dVar47,0));
                *(undefined4 *)((int)puVar27 + -0x8c) = _DAT_0c905228;
              }
              else {
                dVar47 = (double)CONCAT44(uRam0c90522c,_DAT_0c905228);
                *(double *)((int)puVar27 + -0x90) = dVar47;
                *(double *)((int)puVar27 + -0x8c) = dVar47;
                dVar50 = dVar47;
              }
              if (in_FPSCR_SZ == '\0') {
                dVar42 = (double)CONCAT44((int)((ulonglong)dVar42 >> 0x20),
                                          *(undefined4 *)((int)puVar27 + -0x90));
              }
              else {
                dVar50 = *(double *)((int)puVar27 + -0x90);
              }
              if (in_FPSCR_PR == '\0') {
                uVar55 = (undefined1)(int)SUB84(dVar42,0);
                uVar56 = (undefined1)(int)(float)((ulonglong)dVar47 >> 0x20);
              }
              else {
                uVar55 = (undefined1)(int)dVar42;
                uVar56 = (undefined1)(int)dVar47;
              }
              puVar7[2] = uVar55;
              puVar7[1] = uVar56;
              if (in_FPSCR_SZ == '\0') {
                dVar42 = (double)CONCAT44((int)((ulonglong)dVar42 >> 0x20),
                                          *(undefined4 *)((int)puVar27 + -0x8c));
              }
              else {
                dVar50 = *(double *)((int)puVar27 + -0x8c);
              }
              if (in_FPSCR_PR == '\0') {
                uVar55 = (undefined1)(int)SUB84(dVar42,0);
              }
              else {
                uVar55 = (undefined1)(int)dVar42;
              }
              *puVar7 = uVar55;
              if ((uVar24 == 0) && (*(int *)((int)puVar27 + -0x7c) != 0)) {
                *(undefined1 *)(param_5 + 0x8c6) = puVar7[2];
                *(undefined1 *)(param_5 + 0x8c5) = puVar7[1];
                *(undefined1 *)(param_5 + 0x8c4) = *puVar7;
                *(undefined1 *)(param_5 + 0x8c7) = 0xff;
              }
              uVar24 = uVar24 + 1;
              pbVar21 = pbVar21 + 0x14;
              puVar7 = puVar7 + 0x14;
              pbVar19 = pbVar19 + 0x14;
              pbVar23 = pbVar23 + 0x14;
              pbVar15 = pbVar15 + 0x14;
            } while (uVar24 < *(uint *)((int)puVar27 + -0xa8));
          }
        }
      }
      uVar24 = SUB84(dVar47,0);
      iVar4 = *(int *)((int)puVar27 + -0x88);
      *(int *)((int)puVar27 + -0x88) = iVar4 + 1;
      in_xd14 = dVar51;
    } while (iVar4 + 1 < *(int *)((int)puVar27 + -0x6c));
  }
  piVar8 = (int *)((int)puVar27 + -0x5c);
  dVar32 = (double)(*DAT_0c9055e4)();
  do {
    fVar33 = SUB84(dVar42,0);
    if (in_FPSCR_SZ == '\0') {
      fVar33 = *(float *)((int)puVar27 + -0x78);
      dVar39 = (double)CONCAT44((int)((ulonglong)dVar48 >> 0x20),SUB84(dVar39,0));
    }
    else {
      dVar50 = *(double *)((int)puVar27 + -0x78);
      dVar39 = dVar48;
    }
    dVar42 = (double)(ulonglong)(uint)fVar33;
    if (in_FPSCR_PR == '\0') {
      dVar39 = (double)CONCAT44((float)((ulonglong)dVar39 >> 0x20) / fVar33,SUB84(dVar39,0));
    }
    else {
      dVar39 = dVar39 / dVar42;
    }
    uVar3 = (undefined4)((ulonglong)dVar43 >> 0x20);
    dVar47 = dVar43;
    dVar40 = dVar53;
    if (in_FPSCR_SZ == '\0') {
      dVar47 = (double)CONCAT44(uVar3,SUB84(dVar39,0));
      dVar40 = in_xd10;
    }
    in_xd10 = dVar40;
    fVar33 = (float)((ulonglong)dVar39 >> 0x20);
    if (in_FPSCR_PR == '\0') {
      bVar54 = fVar33 == 0.0;
      dVar47 = (double)CONCAT44((float)((ulonglong)dVar47 >> 0x20) * fVar33,SUB84(dVar47,0) * fVar33
                               );
    }
    else {
      bVar54 = dVar39 == dVar42;
      dVar47 = dVar47 * dVar39 * dVar39;
    }
    if (!bVar54) {
      iVar9 = *piVar8 * 4;
      iVar4 = *(int *)(iVar9 + iVar5);
      pdVar10 = (double *)(iVar4 + 8);
      iVar13 = *(int *)(*(int *)((int)puVar27 + -0x80) + iVar9);
      iVar16 = *(int *)(iVar9 + iVar18);
      iVar25 = *(int *)(iVar9 + iVar20);
      if (in_FPSCR_SZ == '\0') {
        uVar2 = *(undefined4 *)(iVar13 + 8);
        pdVar14 = (double *)(iVar13 + 0xc);
        dVar42 = (double)(ulonglong)*(uint *)(iVar16 + 8);
        pdVar17 = (double *)(iVar16 + 0xc);
        dVar40 = (double)CONCAT44(uVar2,SUB84(dVar32,0));
        dVar32 = (double)CONCAT44(uVar2,SUB84(dVar39,0));
      }
      else {
        dVar40 = *(double *)(iVar13 + 8);
        pdVar14 = (double *)(iVar13 + 0x10);
        dVar50 = *(double *)(iVar16 + 8);
        pdVar17 = (double *)(iVar16 + 0x10);
        uVar2 = (undefined4)((ulonglong)dVar40 >> 0x20);
        dVar32 = dVar40;
      }
      fVar33 = SUB84(dVar32,0);
      if (in_FPSCR_PR == '\0') {
        fVar34 = SUB84(dVar42,0) * fVar33;
      }
      else {
        fVar34 = SUB84(dVar42 * dVar32,0);
      }
      if (in_FPSCR_SZ == '\0') {
        dVar40 = (double)CONCAT44((int)((ulonglong)dVar40 >> 0x20),uVar2);
        dVar32 = in_xd0;
      }
      fVar44 = SUB84(dVar43,0);
      if (in_FPSCR_PR == '\0') {
        dVar40 = (double)CONCAT44((int)((ulonglong)dVar40 >> 0x20),SUB84(dVar40,0) * fVar44);
      }
      else {
        dVar40 = dVar40 * dVar43;
      }
      iVar9 = *(int *)(*(int *)((int)puVar27 + -0x84) + iVar9);
      fVar41 = (float)((ulonglong)dVar45 >> 0x20);
      if (in_FPSCR_PR == '\0') {
        fVar34 = (float)((ulonglong)dVar40 >> 0x20) * fVar41 + fVar34;
      }
      if (in_FPSCR_SZ == '\0') {
        pdVar26 = (double *)(iVar25 + 0xc);
        dVar39 = (double)CONCAT44(fVar34,*(undefined4 *)(iVar25 + 8));
        dVar51 = dVar50;
        dVar42 = (double)CONCAT44(uVar3,SUB84(dVar40,0));
      }
      else {
        dVar51 = *(double *)(iVar25 + 8);
        pdVar26 = (double *)(iVar25 + 0x10);
        dVar39 = dVar50;
        dVar42 = dVar43;
      }
      if (in_FPSCR_PR == '\0') {
        dVar42 = (double)(ulonglong)
                         (uint)((float)((ulonglong)dVar42 >> 0x20) * SUB84(dVar39,0) +
                               SUB84(dVar42,0));
      }
      uVar2 = SUB84(dVar47,0);
      dVar50 = in_xd10;
      if (in_FPSCR_SZ == '\0') {
        dVar50 = (double)CONCAT44(uVar2,SUB84(dVar42,0));
      }
      fVar46 = (float)((ulonglong)dVar47 >> 0x20);
      fVar34 = (float)((ulonglong)dVar39 >> 0x20);
      if (in_FPSCR_PR == '\0') {
        fVar29 = (float)((ulonglong)dVar50 >> 0x20) * fVar34 + SUB84(dVar50,0) * fVar46;
      }
      else {
        fVar29 = SUB84(dVar50 * dVar47,0);
      }
      if (in_FPSCR_SZ == '\0') {
        dVar39 = (double)CONCAT44(fVar34,fVar29);
        dVar42 = (double)CONCAT44(uVar24,fVar29);
      }
      else {
        fVar29 = SUB84(dVar38,0);
        dVar51 = dVar32;
        dVar42 = dVar38;
      }
      fVar34 = SUB84(dVar45,0);
      if (in_FPSCR_PR == '\0') {
        fVar35 = SUB84(dVar39,0) * fVar34;
      }
      else {
        fVar35 = SUB84(dVar39 * dVar45,0);
      }
      if (in_FPSCR_SZ == '\0') {
        *(float *)pdVar10 = fVar29;
        dVar50 = (double)CONCAT44(*(undefined4 *)(iVar9 + 8),fVar35);
        pdVar11 = (double *)(iVar9 + 0xc);
      }
      else {
        *pdVar10 = dVar32;
        dVar50 = *(double *)(iVar9 + 8);
        pdVar11 = (double *)(iVar9 + 0x10);
      }
      if (in_FPSCR_PR == '\0') {
        fVar35 = (float)((ulonglong)dVar50 >> 0x20);
        dVar50 = (double)CONCAT44(fVar35,(float)((ulonglong)dVar42 >> 0x20) * fVar35 +
                                         SUB84(dVar50,0));
      }
      if (in_FPSCR_SZ == '\0') {
        *(int *)pdVar10 = SUB84(dVar50,0);
        dVar50 = (double)CONCAT44((int)((ulonglong)dVar50 >> 0x20),*(undefined4 *)pdVar17);
        pdVar17 = (double *)((int)pdVar17 + 4);
        dVar39 = (double)CONCAT44(*(undefined4 *)pdVar14,fVar33);
        pdVar14 = (double *)((int)pdVar14 + 4);
        dVar42 = (double)CONCAT44(fVar41,fVar29);
      }
      else {
        *pdVar10 = dVar51;
        dVar51 = *pdVar17;
        pdVar17 = pdVar17 + 1;
        dVar39 = *pdVar14;
        pdVar14 = pdVar14 + 1;
        dVar42 = dVar45;
      }
      pdVar10 = (double *)(iVar4 + 0xc);
      fVar33 = SUB84(dVar39,0);
      if (in_FPSCR_PR == '\0') {
        fVar29 = SUB84(dVar50,0) * fVar33;
      }
      else {
        fVar29 = SUB84(dVar50 * dVar39,0);
      }
      fVar35 = (float)((ulonglong)dVar39 >> 0x20);
      if (in_FPSCR_SZ == '\0') {
        dVar39 = (double)CONCAT44(fVar35,fVar29);
      }
      if (in_FPSCR_PR == '\0') {
        dVar39 = (double)CONCAT44((float)((ulonglong)dVar39 >> 0x20) * fVar44,
                                  (float)((ulonglong)dVar42 >> 0x20) * fVar35 + SUB84(dVar39,0));
      }
      else {
        dVar39 = dVar39 * dVar43;
      }
      if (in_FPSCR_SZ == '\0') {
        uVar36 = SUB84(dVar39,0);
        dVar39 = (double)CONCAT44((int)((ulonglong)dVar39 >> 0x20),*(undefined4 *)pdVar26);
        pdVar26 = (double *)((int)pdVar26 + 4);
        dVar50 = dVar51;
        dVar42 = (double)CONCAT44(uVar3,uVar36);
        dVar51 = dVar32;
      }
      else {
        dVar50 = *pdVar26;
        pdVar26 = pdVar26 + 1;
        dVar42 = dVar43;
      }
      if (in_FPSCR_PR == '\0') {
        dVar39 = (double)CONCAT44((float)((ulonglong)dVar42 >> 0x20) * SUB84(dVar39,0) +
                                  (float)((ulonglong)dVar39 >> 0x20),SUB84(dVar39,0));
      }
      dVar32 = in_xd10;
      if (in_FPSCR_SZ == '\0') {
        dVar32 = (double)CONCAT44(uVar2,SUB84(dVar42,0));
      }
      if (in_FPSCR_PR == '\0') {
        dVar39 = (double)CONCAT44((float)((ulonglong)dVar32 >> 0x20) * SUB84(dVar32,0) +
                                  (float)((ulonglong)dVar39 >> 0x20) * fVar46,SUB84(dVar39,0));
      }
      else {
        dVar39 = dVar39 * dVar47;
      }
      in_xd0 = dVar39;
      dVar42 = dVar38;
      if (in_FPSCR_SZ == '\0') {
        uVar36 = (undefined4)((ulonglong)dVar39 >> 0x20);
        dVar39 = (double)CONCAT44(uVar36,uVar36);
        in_xd0 = dVar50;
        dVar42 = (double)((ulonglong)uVar24 << 0x20);
      }
      if (in_FPSCR_PR == '\0') {
        dVar39 = (double)CONCAT44((int)((ulonglong)dVar39 >> 0x20),SUB84(dVar39,0) * fVar34);
      }
      else {
        dVar39 = dVar39 * dVar45;
      }
      if (in_FPSCR_SZ == '\0') {
        *(int *)pdVar10 = (int)((ulonglong)dVar39 >> 0x20);
        dVar32 = (double)CONCAT44(*(undefined4 *)pdVar11,SUB84(dVar39,0));
        pdVar11 = (double *)((int)pdVar11 + 4);
      }
      else {
        *pdVar10 = dVar39;
        dVar32 = *pdVar11;
        pdVar11 = pdVar11 + 1;
      }
      if (in_FPSCR_PR == '\0') {
        fVar29 = (float)((ulonglong)dVar32 >> 0x20);
        dVar32 = (double)CONCAT44(fVar29,(float)((ulonglong)dVar42 >> 0x20) * fVar29 +
                                         SUB84(dVar32,0));
      }
      if (in_FPSCR_SZ == '\0') {
        *(int *)pdVar10 = SUB84(dVar32,0);
        dVar39 = (double)CONCAT44(*(undefined4 *)pdVar14,fVar33);
        dVar32 = (double)CONCAT44((int)((ulonglong)dVar32 >> 0x20),*(undefined4 *)pdVar17);
        dVar42 = (double)((ulonglong)(uint)fVar41 << 0x20);
      }
      else {
        *pdVar10 = in_xd0;
        dVar39 = *pdVar14;
        in_xd0 = *pdVar17;
        dVar42 = dVar45;
      }
      pdVar10 = (double *)(iVar4 + 0x10);
      if (in_FPSCR_PR == '\0') {
        fVar33 = SUB84(dVar32,0) * SUB84(dVar39,0);
      }
      else {
        fVar33 = SUB84(dVar32 * dVar39,0);
      }
      fVar41 = (float)((ulonglong)dVar39 >> 0x20);
      dVar40 = dVar39;
      if (in_FPSCR_SZ == '\0') {
        dVar40 = (double)CONCAT44(fVar41,fVar33);
      }
      if (in_FPSCR_PR == '\0') {
        dVar40 = (double)CONCAT44((float)((ulonglong)dVar40 >> 0x20) * fVar44,
                                  (float)((ulonglong)dVar42 >> 0x20) * fVar41 + SUB84(dVar40,0));
      }
      else {
        dVar40 = dVar40 * dVar43;
      }
      if (in_FPSCR_SZ == '\0') {
        uVar36 = SUB84(dVar40,0);
        dVar40 = (double)CONCAT44((int)((ulonglong)dVar40 >> 0x20),*(undefined4 *)pdVar26);
        dVar52 = in_xd0;
        dVar42 = (double)CONCAT44(uVar3,uVar36);
        in_xd0 = dVar51;
      }
      else {
        dVar52 = *pdVar26;
        dVar42 = dVar43;
      }
      if (in_FPSCR_PR == '\0') {
        dVar40 = (double)CONCAT44((float)((ulonglong)dVar42 >> 0x20) * SUB84(dVar40,0) +
                                  (float)((ulonglong)dVar40 >> 0x20),SUB84(dVar40,0));
      }
      dVar51 = in_xd10;
      if (in_FPSCR_SZ == '\0') {
        dVar51 = (double)CONCAT44(uVar2,SUB84(dVar42,0));
      }
      if (in_FPSCR_PR == '\0') {
        dVar40 = (double)CONCAT44((float)((ulonglong)dVar51 >> 0x20) * SUB84(dVar51,0) +
                                  (float)((ulonglong)dVar40 >> 0x20) * fVar46,SUB84(dVar40,0));
      }
      else {
        dVar40 = dVar40 * dVar47;
      }
      dVar50 = dVar40;
      dVar32 = dVar38;
      if (in_FPSCR_SZ == '\0') {
        uVar3 = (undefined4)((ulonglong)dVar40 >> 0x20);
        dVar40 = (double)CONCAT44(uVar3,uVar3);
        dVar50 = dVar52;
        dVar32 = (double)CONCAT44(uVar24,SUB84(dVar51,0));
      }
      if (in_FPSCR_PR == '\0') {
        dVar40 = (double)CONCAT44((int)((ulonglong)dVar40 >> 0x20),SUB84(dVar40,0) * fVar34);
      }
      else {
        dVar40 = dVar40 * dVar45;
      }
      if (in_FPSCR_SZ == '\0') {
        *(int *)pdVar10 = (int)((ulonglong)dVar40 >> 0x20);
        dVar42 = (double)CONCAT44(*(undefined4 *)pdVar11,SUB84(dVar40,0));
      }
      else {
        *pdVar10 = dVar40;
        dVar42 = *pdVar11;
      }
      if (in_FPSCR_PR == '\0') {
        dVar42 = (double)(ulonglong)
                         (uint)((float)((ulonglong)dVar32 >> 0x20) *
                                (float)((ulonglong)dVar42 >> 0x20) + SUB84(dVar42,0));
      }
      if (in_FPSCR_SZ == '\0') {
        *(int *)pdVar10 = SUB84(dVar42,0);
      }
      else {
        *pdVar10 = dVar50;
      }
    }
    iVar12 = iVar12 + 1;
    piVar8 = piVar8 + 1;
  } while (iVar12 < 0x16);
  return;
}

