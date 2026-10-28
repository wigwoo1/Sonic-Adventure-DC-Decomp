/* Ghidra draft only; not buildable/verified source. */

/* WARNING: Removing unreachable block (ram,0x8c07593e) */
/* WARNING: Removing unreachable block (ram,0x8c075994) */

undefined4 FUN_8c075860(undefined8 param_1,int param_2)

{
  undefined *puVar1;
  int iVar2;
  undefined4 uVar3;
  undefined4 *puVar4;
  int iVar5;
  int iVar6;
  undefined1 *puVar7;
  undefined4 uVar8;
  undefined8 uVar9;
  double dVar10;
  float fVar11;
  double in_dr2;
  double dVar12;
  double in_dr12;
  undefined8 in_dr14;
  undefined8 in_xd2;
  undefined8 in_xd14;
  bool bVar13;
  undefined4 in_PR;
  char in_FPSCR_PR;
  char in_FPSCR_SZ;
  undefined1 auStack_34 [8];
  undefined8 local_2c;
  undefined8 uStack_24;
  
  puVar1 = PTR_FUN_8c075a08;
  uVar3 = (undefined4)param_1;
  if (in_FPSCR_SZ == '\0') {
    uStack_24 = CONCAT44((int)in_dr14,(undefined4)uStack_24);
                    /* WARNING: Ignoring partial resolution of indirect */
    uStack_24._0_4_ = (int)((ulonglong)in_dr14 >> 0x20);
    puVar7 = (undefined1 *)((int)&local_2c + 4);
    local_2c = CONCAT44((int)((ulonglong)in_dr12 >> 0x20),(undefined4)local_2c);
  }
  else {
    puVar7 = auStack_34;
    uStack_24 = in_xd14;
    local_2c = in_dr14;
  }
  *(undefined4 *)(puVar7 + -4) = in_PR;
  *(undefined4 *)(puVar7 + -0x98) = 0;
  *(undefined4 *)(puVar7 + -0x94) = 0xffffffff;
  if (in_FPSCR_SZ == '\0') {
    dVar12 = (double)CONCAT44(DAT_8c075a04,DAT_8c075a00);
  }
  else {
    dVar12 = (double)CONCAT44(PTR_FUN_8c075a08,DAT_8c075a04);
  }
  iVar6 = 0;
  if (*(short *)(param_2 + 0x28) != 0) {
    do {
      *(int *)(puVar7 + -0x9c) = iVar6 * 4;
      *(undefined1 **)(puVar7 + -0xa0) = puVar7 + iVar6 * 4 + -0x44;
      iVar5 = iVar6 * 0x58 + DAT_8c075a0c;
      uVar9 = (*(code *)PTR_FUN_8c075a10)(iVar5 + 0x2c,param_2 + 4);
      if (in_FPSCR_SZ == '\0') {
        *(int *)*(undefined8 **)(puVar7 + -0xa0) = (int)((ulonglong)uVar9 >> 0x20);
      }
      else {
        **(undefined8 **)(puVar7 + -0xa0) = uVar9;
      }
      *(undefined4 *)(puVar7 + *(int *)(puVar7 + -0x9c) + -0x84) = 0;
      iVar2 = (*(code *)PTR_FUN_8c075a14)(param_2 + 0x1c,iVar5 + 0x14,0);
      uVar3 = SUB84(in_dr12,0);
      *(int *)(puVar7 + -0x9c) = iVar2;
      if (iVar2 < 0x2c01) {
LAB_8c075900:
        iVar5 = iVar6 * 4;
        *(uint *)(puVar7 + iVar5 + -0x84) = *(uint *)(puVar7 + iVar5 + -0x84) | 2;
        *(int *)(puVar7 + -0x98) = *(int *)(puVar7 + -0x98) + 1;
        if (in_FPSCR_SZ == '\0') {
          in_dr2 = (double)CONCAT44((int)((ulonglong)in_dr2 >> 0x20),
                                    *(undefined4 *)(puVar7 + iVar5 + -0x44));
        }
        else {
          in_xd2 = *(undefined8 *)(puVar7 + iVar5 + -0x44);
        }
        if (in_FPSCR_PR == '\0') {
          bVar13 = SUB84(in_dr2,0) < SUB84(dVar12,0);
        }
        else {
          bVar13 = in_dr2 < dVar12;
        }
        if (bVar13) {
          if (in_FPSCR_SZ == '\0') {
            dVar12 = (double)CONCAT44((int)((ulonglong)dVar12 >> 0x20),
                                      *(undefined4 *)(puVar7 + iVar6 * 4 + -0x44));
          }
          *(int *)(puVar7 + -0x94) = iVar6;
        }
      }
      else {
        uVar9 = (*(code *)PTR_FUN_8c075a1c)(DAT_8c075a18,param_2 + 0x1c);
        uVar8 = (undefined4)uVar9;
        if (in_FPSCR_SZ == '\0') {
          uVar8 = uVar3;
        }
        in_dr12 = (double)CONCAT44((int)((ulonglong)uVar9 >> 0x20),uVar8);
        dVar10 = (double)(*(code *)PTR_FUN_8c075a1c)(DAT_8c075a18,iVar5 + 0x14);
        if (in_FPSCR_PR == '\0') {
          bVar13 = (float)((ulonglong)dVar10 >> 0x20) < (float)((ulonglong)in_dr12 >> 0x20);
        }
        else {
          bVar13 = dVar10 < in_dr12;
        }
        if (bVar13) goto LAB_8c075900;
        uVar3 = *(undefined4 *)(param_2 + 0x14);
        *(undefined4 *)(puVar7 + -0x90) = *(undefined4 *)(param_2 + 0x10);
        *(undefined4 *)(puVar7 + -0x8c) = uVar3;
        *(undefined4 *)(puVar7 + -0x88) = *(undefined4 *)(param_2 + 0x18);
        fVar11 = (float)(*(code *)puVar1)(puVar7 + -0x90);
        in_dr2 = (double)((ulonglong)in_dr2 & 0xffffffff00000000);
        if (fVar11 != 0.0) {
          (*(code *)PTR_FUN_8c075a20)(puVar7 + -0x90);
        }
        dVar10 = (double)(*(code *)PTR_FUN_8c075a1c)(puVar7 + -0x90,iVar5 + 0x14);
        if (in_FPSCR_PR == '\0') {
          bVar13 = (float)((ulonglong)dVar10 >> 0x20) < (float)((ulonglong)dVar12 >> 0x20);
        }
        else {
          bVar13 = dVar10 < dVar12;
        }
        if (bVar13) {
          *(uint *)(puVar7 + iVar6 * 4 + -0x84) = *(uint *)(puVar7 + iVar6 * 4 + -0x84) | 8;
        }
      }
      uVar3 = (undefined4)param_1;
      iVar6 = iVar6 + 1;
    } while (iVar6 < (int)(uint)*(ushort *)(param_2 + 0x28));
  }
  if (0 < *(int *)(puVar7 + -0x98)) {
    uVar9 = (*(code *)puVar1)(param_2 + 0x10);
    fVar11 = (float)((ulonglong)uVar9 >> 0x20);
    uVar8 = (undefined4)uVar9;
    if (in_FPSCR_SZ == '\0') {
      uVar8 = uVar3;
    }
    iVar6 = *(int *)(puVar7 + -0x94) * 4;
    if (in_FPSCR_SZ == '\0') {
      in_dr2 = (double)CONCAT44((int)((ulonglong)in_dr2 >> 0x20),
                                *(undefined4 *)(puVar7 + iVar6 + -0x44));
    }
    else {
      in_xd2 = *(undefined8 *)(puVar7 + iVar6 + -0x44);
    }
    if (in_FPSCR_PR == '\0') {
      bVar13 = SUB84(in_dr2,0) < fVar11;
    }
    else {
      bVar13 = in_dr2 < (double)CONCAT44(fVar11,uVar8);
    }
    if (bVar13) {
      fVar11 = (float)(*(code *)puVar1)(param_2 + 0x10);
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
        (*(code *)PTR_FUN_8c075a20)(param_2 + 0x10);
        puVar4 = (undefined4 *)(puVar7 + iVar6 + -0x44);
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

