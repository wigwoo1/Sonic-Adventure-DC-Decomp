/* Ghidra draft only; not buildable/verified source. */

void FUN_8c075214(undefined4 param_1,int param_2,int param_3,int param_4,uint param_5)

{
  undefined *puVar1;
  uint uVar2;
  double in_dr0;
  float fVar3;
  double in_dr2;
  undefined4 uVar7;
  double dVar4;
  double dVar5;
  double dVar6;
  double dVar8;
  float fVar9;
  undefined8 in_xd0;
  undefined8 in_xd2;
  undefined8 in_xd4;
  char in_FPSCR_PR;
  char in_FPSCR_SZ;
  
  puVar1 = PTR_DAT_8c0753b0;
  *(uint *)(param_4 + 0x5c) = *(uint *)(param_4 + 0x5c) | param_5;
  if (in_FPSCR_SZ == '\0') {
    dVar4 = (double)CONCAT44(DAT_8c0753a0._4_4_,param_1);
    dVar8 = (double)CONCAT44(DAT_8c0753ac,DAT_8c0753a8);
  }
  else {
    dVar4 = (double)CONCAT44(DAT_8c0753a8,DAT_8c0753a0._4_4_);
    in_xd4 = CONCAT44(DAT_8c0753ac,DAT_8c0753a8);
    dVar8 = (double)CONCAT44(PTR_DAT_8c0753b0,DAT_8c0753ac);
  }
  fVar9 = (float)((ulonglong)dVar8 >> 0x20);
  fVar3 = (float)((ulonglong)dVar4 >> 0x20);
  if ((*(uint *)(param_4 + 0x1e4) & 2) == 0) {
    if ((*(uint *)(param_4 + 0x1e4) & DAT_8c0753b8) == 0) {
      if ((*(uint *)(param_4 + 0x1e4) & 2) == 0) {
        if (in_FPSCR_SZ == '\0') {
          in_dr2 = (double)(ulonglong)*(uint *)(puVar1 + (uint)*(byte *)(param_2 + 9) * 0x84 + 0x78)
          ;
          *(uint *)(param_4 + 0x114) =
               *(uint *)(puVar1 + (uint)*(byte *)(param_2 + 9) * 0x84 + 0x78);
        }
        else {
          in_xd2 = *(undefined8 *)(puVar1 + (uint)*(byte *)(param_2 + 9) * 0x84 + 0x78);
          *(undefined8 *)(param_4 + 0x114) = in_xd2;
        }
        if (in_FPSCR_SZ == '\0') {
          in_dr2 = (double)(ulonglong)*(uint *)(puVar1 + (uint)*(byte *)(param_2 + 9) * 0x84 + 0x58)
          ;
          *(uint *)(param_4 + 0xf4) = *(uint *)(puVar1 + (uint)*(byte *)(param_2 + 9) * 0x84 + 0x58)
          ;
        }
        else {
          in_xd2 = *(undefined8 *)(puVar1 + (uint)*(byte *)(param_2 + 9) * 0x84 + 0x58);
          *(undefined8 *)(param_4 + 0xf4) = in_xd2;
        }
        if (in_FPSCR_SZ == '\0') {
          in_dr2 = (double)(ulonglong)*(uint *)(puVar1 + (uint)*(byte *)(param_2 + 9) * 0x84 + 0x3c)
          ;
          *(uint *)(param_4 + 0xd8) = *(uint *)(puVar1 + (uint)*(byte *)(param_2 + 9) * 0x84 + 0x3c)
          ;
        }
        else {
          in_xd2 = *(undefined8 *)(puVar1 + (uint)*(byte *)(param_2 + 9) * 0x84 + 0x3c);
          *(undefined8 *)(param_4 + 0xd8) = in_xd2;
        }
      }
    }
    else {
      if (in_FPSCR_SZ == '\0') {
        dVar5 = (double)CONCAT44(*(undefined4 *)
                                  (puVar1 + (uint)*(byte *)(param_2 + 9) * 0x84 + 0x78),
                                 (undefined4)DAT_8c0753bc);
      }
      else {
        in_xd2 = CONCAT44(DAT_8c0753bc._4_4_,(undefined4)DAT_8c0753bc);
        dVar5 = *(double *)(puVar1 + (uint)*(byte *)(param_2 + 9) * 0x84 + 0x78);
      }
      if (in_FPSCR_PR == '\0') {
        dVar5 = (double)CONCAT44((float)((ulonglong)dVar5 >> 0x20) * SUB84(dVar5,0),SUB84(dVar5,0));
      }
      else {
        dVar5 = dVar5 * dVar5;
      }
      if (in_FPSCR_SZ == '\0') {
        *(int *)(param_4 + 0x114) = (int)((ulonglong)dVar5 >> 0x20);
      }
      else {
        *(double *)(param_4 + 0x114) = dVar5;
      }
      dVar6 = (double)CONCAT44(0x3f800000,SUB84(dVar5,0));
      if (in_FPSCR_PR == '\0') {
        in_dr2 = (double)CONCAT44(0x40000000,SUB84(dVar5,0));
      }
      else {
        in_dr2 = dVar6 + dVar6;
      }
      if (in_FPSCR_SZ == '\0') {
        in_dr0 = (double)CONCAT44((int)((ulonglong)in_dr0 >> 0x20),
                                  *(undefined4 *)
                                   (puVar1 + (uint)*(byte *)(param_2 + 9) * 0x84 + 0x58));
      }
      else {
        in_xd0 = *(undefined8 *)(puVar1 + (uint)*(byte *)(param_2 + 9) * 0x84 + 0x58);
      }
      if (in_FPSCR_PR == '\0') {
        dVar5 = (double)CONCAT44((int)((ulonglong)in_dr0 >> 0x20),
                                 SUB84(in_dr0,0) * (float)((ulonglong)in_dr2 >> 0x20));
      }
      else {
        dVar5 = in_dr0 * in_dr2;
      }
      if (in_FPSCR_SZ == '\0') {
        *(int *)(param_4 + 0xf4) = SUB84(dVar5,0);
      }
      else {
        *(undefined8 *)(param_4 + 0xf4) = in_xd0;
      }
      if (in_FPSCR_SZ == '\0') {
        dVar5 = (double)CONCAT44((int)((ulonglong)dVar5 >> 0x20),
                                 *(undefined4 *)
                                  (puVar1 + (uint)*(byte *)(param_2 + 9) * 0x84 + 0x3c));
      }
      else {
        in_xd0 = *(undefined8 *)(puVar1 + (uint)*(byte *)(param_2 + 9) * 0x84 + 0x3c);
      }
      if (in_FPSCR_PR == '\0') {
        fVar3 = SUB84(dVar5,0) * fVar3;
      }
      else {
        fVar3 = SUB84(dVar5 * dVar4,0);
      }
      if (in_FPSCR_SZ == '\0') {
        *(float *)(param_4 + 0xd8) = fVar3;
      }
      else {
        *(undefined8 *)(param_4 + 0xd8) = in_xd0;
      }
    }
  }
  else {
    uVar2 = (uint)*(byte *)(param_2 + 9);
    uVar7 = (undefined4)((ulonglong)in_dr2 >> 0x20);
    if (uVar2 == 5) {
      if (in_FPSCR_SZ == '\0') {
        in_dr2 = (double)CONCAT44(uVar7,*(undefined4 *)
                                         (puVar1 + (uint)*(byte *)(param_2 + 9) * 0x84 + 0x78));
      }
      else {
        in_xd2 = *(undefined8 *)(puVar1 + (uint)*(byte *)(param_2 + 9) * 0x84 + 0x78);
      }
      if (in_FPSCR_PR == '\0') {
        dVar4 = (double)CONCAT44((int)((ulonglong)in_dr2 >> 0x20),SUB84(in_dr2,0) * fVar9);
      }
      else {
        dVar4 = in_dr2 * dVar8;
      }
    }
    else {
      if (in_FPSCR_SZ == '\0') {
        in_dr2 = (double)CONCAT44(uVar7,*(undefined4 *)(puVar1 + uVar2 * 0x84 + 0x78));
      }
      else {
        in_xd2 = *(undefined8 *)(puVar1 + uVar2 * 0x84 + 0x78);
      }
      if (in_FPSCR_PR == '\0') {
        dVar4 = (double)CONCAT44((int)((ulonglong)in_dr2 >> 0x20),SUB84(in_dr2,0) * fVar3);
      }
      else {
        dVar4 = in_dr2 * dVar4;
      }
    }
    if (in_FPSCR_SZ == '\0') {
      *(int *)(param_4 + 0x114) = SUB84(dVar4,0);
    }
    else {
      *(undefined8 *)(param_4 + 0x114) = in_xd2;
    }
    if (in_FPSCR_SZ == '\0') {
      dVar4 = (double)CONCAT44((int)((ulonglong)dVar4 >> 0x20),
                               *(undefined4 *)(puVar1 + (uint)*(byte *)(param_2 + 9) * 0x84 + 0x58))
      ;
    }
    else {
      in_xd2 = *(undefined8 *)(puVar1 + (uint)*(byte *)(param_2 + 9) * 0x84 + 0x58);
    }
    if (in_FPSCR_PR == '\0') {
      fVar3 = SUB84(dVar4,0) * SUB84(dVar8,0);
    }
    else {
      fVar3 = SUB84(dVar4 * dVar8,0);
    }
    if (in_FPSCR_SZ == '\0') {
      *(float *)(param_4 + 0xf4) = fVar3;
    }
    else {
      *(undefined8 *)(param_4 + 0xf4) = in_xd2;
    }
    if (in_FPSCR_SZ == '\0') {
      dVar4 = (double)CONCAT44(*(undefined4 *)(puVar1 + (uint)*(byte *)(param_2 + 9) * 0x84 + 0x3c),
                               DAT_8c0753b4);
    }
    else {
      in_xd2 = CONCAT44(DAT_8c0753b8,DAT_8c0753b4);
      dVar4 = *(double *)(puVar1 + (uint)*(byte *)(param_2 + 9) * 0x84 + 0x3c);
    }
    if (in_FPSCR_PR == '\0') {
      in_dr2 = (double)CONCAT44((float)((ulonglong)dVar4 >> 0x20) * SUB84(dVar4,0),SUB84(dVar4,0));
    }
    else {
      in_dr2 = dVar4 * dVar4;
    }
    if (in_FPSCR_SZ == '\0') {
      *(int *)(param_4 + 0xd8) = (int)((ulonglong)in_dr2 >> 0x20);
    }
    else {
      *(double *)(param_4 + 0xd8) = in_dr2;
    }
  }
  if ((param_5 & 8) == 0) {
    if ((param_5 & 4) == 0) {
      if ((param_5 & 0x10) != 0) {
        if (in_FPSCR_SZ == '\0') {
          *(float *)(param_3 + 0x3c) = fVar9;
          return;
        }
        *(double *)(param_3 + 0x3c) = dVar8;
        return;
      }
      if ((param_5 & 0x80) == 0) {
        if (in_FPSCR_SZ == '\0') {
          *(undefined4 *)(param_3 + 0x3c) = 0x3f800000;
          return;
        }
        *(undefined8 *)(param_3 + 0x3c) = in_xd2;
        return;
      }
      if (in_FPSCR_SZ == '\0') {
        *(float *)(param_3 + 0x3c) = SUB84(dVar8,0);
        return;
      }
      *(undefined8 *)(param_3 + 0x3c) = in_xd4;
      return;
    }
    uVar7 = (undefined4)DAT_8c0754e0;
    if (in_FPSCR_SZ != '\0') {
      in_xd2 = CONCAT44(DAT_8c0754e0._4_4_,(undefined4)DAT_8c0754e0);
      uVar7 = SUB84(in_dr2,0);
    }
  }
  else {
    uVar7 = DAT_8c0753bc._4_4_;
    if (in_FPSCR_SZ != '\0') {
      in_xd2 = CONCAT44(uRam8c0753c4,DAT_8c0753bc._4_4_);
      uVar7 = SUB84(in_dr2,0);
    }
  }
  if (in_FPSCR_SZ == '\0') {
    *(undefined4 *)(param_3 + 0x3c) = uVar7;
  }
  else {
    *(undefined8 *)(param_3 + 0x3c) = in_xd2;
  }
  return;
}

