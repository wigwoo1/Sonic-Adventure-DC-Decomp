/* Ghidra draft only; not buildable/verified source. */

void FUN_8c074ee4(undefined4 param_1,int param_2,int param_3,int param_4)

{
  undefined *puVar1;
  undefined4 uVar2;
  undefined1 *puVar3;
  double dVar4;
  double dVar5;
  undefined8 uVar6;
  double in_dr2;
  undefined8 in_dr12;
  float fVar7;
  undefined8 in_dr14;
  float fVar8;
  undefined8 in_xd2;
  undefined8 in_xd14;
  bool bVar9;
  undefined4 in_PR;
  uint uVar10;
  char in_FPSCR_PR;
  char in_FPSCR_SZ;
  undefined1 auStack_28 [8];
  undefined8 local_20;
  undefined8 uStack_18;
  
  if (in_FPSCR_SZ == '\0') {
    uStack_18 = CONCAT44((int)in_dr14,(undefined4)uStack_18);
                    /* WARNING: Ignoring partial resolution of indirect */
    uStack_18._0_4_ = (int)((ulonglong)in_dr14 >> 0x20);
    puVar3 = (undefined1 *)((int)&local_20 + 4);
    local_20 = CONCAT44((int)((ulonglong)in_dr12 >> 0x20),(undefined4)local_20);
  }
  else {
    puVar3 = auStack_28;
    uStack_18 = in_xd14;
    local_20 = in_dr14;
  }
  *(undefined4 *)(puVar3 + -4) = in_PR;
  dVar4 = (double)(*(code *)PTR_FUN_8c074f60)(DAT_8c074f5c,param_4 + 0x1c);
  if (in_FPSCR_SZ == '\0') {
    dVar4 = (double)CONCAT44((int)((ulonglong)dVar4 >> 0x20),(int)in_dr12);
  }
  dVar5 = (double)(*(code *)PTR_FUN_8c074f60)(DAT_8c074f5c,param_4 + 0x48);
  if (in_FPSCR_PR == '\0') {
    bVar9 = (float)((ulonglong)dVar5 >> 0x20) < (float)((ulonglong)dVar4 >> 0x20);
  }
  else {
    bVar9 = dVar5 < dVar4;
  }
  if (((bVar9) || ((*(ushort *)(param_3 + 0x10) & 1) == 0)) ||
     ((*(ushort *)(param_3 + 0x10) & 0x2000) != 0)) {
    *(undefined4 *)(param_2 + 0x14) = *(undefined4 *)(param_4 + 0x34);
    *(undefined4 *)(param_2 + 0x1c) = *(undefined4 *)(param_4 + 0x38);
  }
  else {
    uVar2 = *(undefined4 *)(param_4 + 0x4c);
    *(undefined4 *)(puVar3 + -0x10) = *(undefined4 *)(param_4 + 0x48);
    *(undefined4 *)(puVar3 + -0xc) = uVar2;
    *(undefined4 *)(puVar3 + -8) = *(undefined4 *)(param_4 + 0x50);
    (*(code *)PTR_FUN_8c075180)(param_2,puVar3 + -0x10);
    if (in_FPSCR_SZ == '\0') {
      fVar7 = *(float *)(puVar3 + -0x10);
      dVar4 = (double)CONCAT44(*(undefined4 *)(puVar3 + -8),fVar7);
      in_dr2 = (double)CONCAT44((int)((ulonglong)in_dr2 >> 0x20),fVar7);
    }
    else {
      in_xd2 = *(undefined8 *)(puVar3 + -0x10);
      dVar4 = *(double *)(puVar3 + -8);
      fVar7 = SUB84(dVar4,0);
    }
    if (in_FPSCR_PR == '\0') {
      fVar7 = SUB84(in_dr2,0) * fVar7;
    }
    else {
      fVar7 = SUB84(in_dr2 * dVar4,0);
    }
    fVar8 = (float)((ulonglong)dVar4 >> 0x20);
    if (in_FPSCR_SZ == '\0') {
      dVar4 = (double)((ulonglong)(uint)fVar8 << 0x20);
    }
    if (in_FPSCR_PR == '\0') {
      fVar7 = (float)((ulonglong)dVar4 >> 0x20) * fVar8 + fVar7;
    }
    uVar6 = in_xd2;
    if (in_FPSCR_SZ == '\0') {
      uVar6 = CONCAT44(fVar7,param_1);
    }
    uVar6 = (*(code *)PTR_FUN_8c075184)(uVar6);
    if (in_FPSCR_SZ == '\0') {
      *(int *)(puVar3 + -0x10) = (int)((ulonglong)uVar6 >> 0x20);
      *(undefined4 *)(puVar3 + -8) = 0;
      uVar6 = CONCAT44(*(undefined4 *)(puVar3 + -0x10),*(undefined4 *)(puVar3 + -0xc));
    }
    else {
      *(undefined8 *)(puVar3 + -0x10) = uVar6;
      *(undefined8 *)(puVar3 + -8) = in_xd2;
      uVar6 = *(undefined8 *)(puVar3 + -0x10);
    }
    dVar4 = (double)(*(code *)PTR_FUN_8c075188)(uVar6);
    if (in_FPSCR_SZ == '\0') {
      dVar4 = (double)CONCAT44((int)((ulonglong)dVar4 >> 0x20),DAT_8c07518c);
    }
    if (in_FPSCR_PR == '\0') {
      uVar10 = (uint)((float)((ulonglong)dVar4 >> 0x20) * SUB84(dVar4,0));
    }
    else {
      uVar10 = (uint)(dVar4 * dVar4);
    }
    uVar10 = uVar10 & DAT_8c075190;
    if (DAT_8c075194 < (int)uVar10) {
      uVar10 = DAT_8c075198 - uVar10;
    }
    if ((*(ushort *)(param_2 + 4) & 0x2000) == 0) {
      if (in_FPSCR_SZ == '\0') {
        uVar6 = CONCAT44(*(undefined4 *)(param_3 + 0x38),*(undefined4 *)(param_3 + 0xd0));
      }
      else {
        uVar6 = *(undefined8 *)(param_3 + 0x38);
      }
      if (in_FPSCR_PR == '\0' && (float)((ulonglong)uVar6 >> 0x20) < (float)uVar6) {
        if (in_FPSCR_SZ == '\0') {
          uVar6 = CONCAT44(*(undefined4 *)(param_3 + 0x38),*(undefined4 *)(param_3 + 200));
        }
        else {
          uVar6 = *(undefined8 *)(param_3 + 0x38);
        }
        if (in_FPSCR_PR == '\0' && (float)((ulonglong)uVar6 >> 0x20) < (float)uVar6) {
          if (in_FPSCR_SZ == '\0') {
            uVar6 = CONCAT44(*(undefined4 *)(param_3 + 0x38),*(undefined4 *)(param_3 + 0xc4));
          }
          else {
            uVar6 = *(undefined8 *)(param_3 + 0x38);
          }
          if (in_FPSCR_PR == '\0' && (float)((ulonglong)uVar6 >> 0x20) < (float)uVar6) {
            if (in_FPSCR_SZ == '\0') {
              uVar6 = CONCAT44(*(undefined4 *)(param_3 + 0x38),*(undefined4 *)(param_3 + 0xc0));
            }
            else {
              uVar6 = *(undefined8 *)(param_3 + 0x38);
            }
            if (in_FPSCR_PR == '\0' && (float)((ulonglong)uVar6 >> 0x20) < (float)uVar6) {
              uVar10 = 0x400;
            }
            else if (0x1000 < (int)uVar10) {
              uVar10 = 0x1000;
            }
          }
          else if (0x1500 < (int)uVar10) {
            uVar10 = 0x1500;
          }
        }
        else if (0x2000 < (int)uVar10) {
          uVar10 = 0x2000;
        }
      }
    }
    uVar2 = (*(code *)PTR_FUN_8c07519c)
                      (*(undefined4 *)(param_2 + 0x14),*(undefined4 *)(param_4 + 0x34),uVar10);
    puVar1 = PTR_FUN_8c07519c;
    *(undefined4 *)(param_2 + 0x14) = uVar2;
    uVar2 = (*(code *)puVar1)(*(undefined4 *)(param_2 + 0x1c),*(undefined4 *)(param_4 + 0x38),uVar10
                             );
    *(undefined4 *)(param_2 + 0x1c) = uVar2;
  }
  return;
}

