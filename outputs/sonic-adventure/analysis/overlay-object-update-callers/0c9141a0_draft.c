/* Ghidra draft only; not buildable/verified source. */

/* WARNING: Removing unreachable block (ram,0x0c914428) */
/* WARNING: Removing unreachable block (ram,0x0c91429a) */
/* WARNING: Removing unreachable block (ram,0x0c914434) */
/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void callback_candidate_c9141a0(ulonglong param_1,undefined1 *param_2,int param_3)

{
  char cVar1;
  undefined *puVar2;
  uint *puVar3;
  double dVar4;
  float fVar5;
  double in_dr2;
  undefined8 uVar6;
  float fVar7;
  double dVar8;
  undefined4 uVar9;
  undefined8 in_xd2;
  undefined8 in_xd4;
  bool bVar10;
  char in_FPSCR_PR;
  char in_FPSCR_SZ;
  
  dVar4 = (double)(*DAT_0c914340)(*(undefined4 *)(param_3 + 0x8cc));
  puVar2 = PTR_callback_candidate_c904e26_0c914354;
  if (in_FPSCR_SZ == '\0') {
    dVar8 = (double)CONCAT44((undefined4)DAT_0c914344,0x3f800000);
  }
  else {
    dVar8 = (double)CONCAT44(DAT_0c914344._4_4_,(undefined4)DAT_0c914344);
  }
  cVar1 = param_2[1];
  if (cVar1 == '\0') {
    *(ushort *)(param_3 + 0x11a) = *(ushort *)(param_3 + 0x11a) | 0x10;
    *(undefined2 *)(param_3 + 0x122) = 7;
    param_2[1] = param_2[1] + '\x01';
  }
  else {
    fVar7 = (float)((ulonglong)dVar8 >> 0x20);
    if (cVar1 == '\x01') {
      if (300 < *(ushort *)(param_2 + 6)) {
        param_2[1] = param_2[1] + '\x01';
        *(ushort *)(param_2 + 4) = *(ushort *)(param_2 + 4) & 0xefbf;
        **(uint **)(*(int *)(param_2 + 0x38) + 0xc) =
             **(uint **)(*(int *)(param_2 + 0x38) + 0xc) | 0x10;
        puVar3 = (uint *)(*(int *)(*(int *)(param_2 + 0x38) + 0xc) + 0x34);
        *puVar3 = *puVar3 | 0x10;
        puVar3 = (uint *)(*(int *)(*(int *)(param_2 + 0x38) + 0xc) + 0x68);
        *puVar3 = *puVar3 & 0xffffffef;
        *(byte *)(*(int *)(*(int *)(param_2 + 0x38) + 0xc) + 4) =
             *(byte *)(*(int *)(*(int *)(param_2 + 0x38) + 0xc) + 4) & 0xf3 | 0xc;
        *(byte *)(*(int *)(*(int *)(param_2 + 0x38) + 0xc) + 0x38) =
             *(byte *)(*(int *)(*(int *)(param_2 + 0x38) + 0xc) + 0x38) & 0xf3 | 0xc;
        *(uint *)(param_3 + 0x60) = *(uint *)(param_3 + 0x60) | 4;
        if (in_FPSCR_SZ == '\0') {
          *(undefined4 *)(param_3 + 0x6c) = 0;
          *(float *)(param_3 + 0x70) = fVar7;
        }
        else {
          *(ulonglong *)(param_3 + 0x6c) = param_1 & 0xffffffff;
          *(double *)(param_3 + 0x70) = dVar8;
        }
      }
    }
    else {
      fVar5 = SUB84(dVar8,0);
      if (cVar1 == '\x02') {
        if (in_FPSCR_SZ == '\0') {
          uVar9 = *(undefined4 *)(param_3 + 0x6c);
          in_dr2 = (double)CONCAT44((int)((ulonglong)in_dr2 >> 0x20),uVar9);
          uVar6 = CONCAT44(uVar9,(int)param_1);
          dVar4 = (double)((ulonglong)(uint)fVar7 << 0x20);
        }
        else {
          in_xd2 = *(undefined8 *)(param_3 + 0x6c);
          uVar9 = (undefined4)((ulonglong)in_xd2 >> 0x20);
          uVar6 = in_xd2;
          dVar4 = dVar8;
        }
        if (in_FPSCR_PR == '\0') {
          fVar7 = -SUB84(in_dr2,0) + fVar5;
        }
        else {
          fVar7 = SUB84((double)CONCAT44((int)((ulonglong)in_dr2 >> 0x20),-SUB84(in_dr2,0)) + dVar8,
                        0);
        }
        if (in_FPSCR_SZ == '\0') {
          uVar6 = CONCAT44(uVar9,fVar7);
        }
        if (in_FPSCR_PR == '\0') {
          uVar6 = CONCAT44((float)((ulonglong)dVar4 >> 0x20) * (float)uVar6 +
                           (float)((ulonglong)uVar6 >> 0x20),(float)uVar6);
        }
        if (in_FPSCR_SZ == '\0') {
          *(int *)(param_3 + 0x6c) = (int)((ulonglong)uVar6 >> 0x20);
          in_dr2 = (double)CONCAT44(*(undefined4 *)(param_3 + 0x6c),DAT_0c914344._4_4_);
        }
        else {
          *(undefined8 *)(param_3 + 0x6c) = uVar6;
          in_xd2 = CONCAT44(_DAT_0c91434c,DAT_0c914344._4_4_);
          in_dr2 = *(double *)(param_3 + 0x6c);
        }
        if (in_FPSCR_PR == '\0' && SUB84(in_dr2,0) < (float)((ulonglong)in_dr2 >> 0x20)) {
          if (in_FPSCR_SZ == '\0') {
            *(float *)(param_3 + 0x6c) = fVar5;
          }
          else {
            *(undefined8 *)(param_3 + 0x6c) = in_xd4;
          }
          param_2[1] = param_2[1] + '\x01';
        }
      }
      else if (cVar1 == '\x03') {
        if (in_FPSCR_SZ == '\0') {
          fVar7 = *(float *)(param_3 + 0x70);
        }
        else {
          fVar7 = (float)((ulonglong)*(undefined8 *)(param_3 + 0x70) >> 0x20);
        }
        if (in_FPSCR_PR == '\0') {
          fVar7 = fVar7 + _DAT_0c91434c;
          in_dr2 = (double)CONCAT44(fVar7,_DAT_0c91434c);
          bVar10 = fVar5 < fVar7;
        }
        else {
          in_dr2 = (double)CONCAT44(fVar7,_DAT_0c91434c) + (double)CONCAT44(fVar7,_DAT_0c91434c);
          fVar7 = (float)((ulonglong)in_dr2 >> 0x20);
          bVar10 = dVar8 < in_dr2;
        }
        if (in_FPSCR_SZ == '\0') {
          *(float *)(param_3 + 0x70) = fVar7;
        }
        else {
          *(double *)(param_3 + 0x70) = in_dr2;
        }
        if (bVar10) {
          if (in_FPSCR_SZ == '\0') {
            *(float *)(param_3 + 0x70) = fVar5;
          }
          else {
            *(undefined8 *)(param_3 + 0x70) = in_xd4;
          }
          puVar2 = PTR_FUN_0c914350;
          param_2[1] = param_2[1] + '\x01';
          (*(code *)puVar2)(param_2,param_3);
          *(undefined2 *)(param_2 + 6) = 0;
        }
      }
      else if (cVar1 == '\x04') {
        if (300 < *(ushort *)(param_2 + 6)) {
          param_2[1] = param_2[1] + '\x01';
          (*(code *)puVar2)(param_2,param_3);
          (*(code *)PTR_FUN_0c914358)(param_2,param_3);
          (*(code *)PTR_callback_candidate_c904e80_0c91435c)(param_2,param_3);
          (*(code *)PTR_callback_candidate_c9048ec_0c914360)(param_2,param_3);
          (*(code *)PTR_FUN_0c914364)(param_2,param_3);
        }
      }
      else if (cVar1 == '\x05') {
        if (in_FPSCR_SZ == '\0') {
          fVar5 = *(float *)(param_3 + 0x70);
        }
        else {
          fVar5 = (float)((ulonglong)*(undefined8 *)(param_3 + 0x70) >> 0x20);
        }
        if (in_FPSCR_PR == '\0') {
          fVar5 = fVar5 - DAT_0c9144e0;
          in_dr2 = (double)CONCAT44(fVar5,DAT_0c9144e0);
          bVar10 = fVar5 < fVar7;
        }
        else {
          in_dr2 = (double)CONCAT44(fVar5,DAT_0c9144e0) - (double)CONCAT44(fVar5,DAT_0c9144e0);
          fVar5 = (float)((ulonglong)in_dr2 >> 0x20);
          bVar10 = in_dr2 < dVar8;
        }
        if (in_FPSCR_SZ == '\0') {
          *(float *)(param_3 + 0x70) = fVar5;
        }
        else {
          *(double *)(param_3 + 0x70) = in_dr2;
        }
        if (bVar10) {
          if (in_FPSCR_SZ == '\0') {
            *(float *)(param_3 + 0x70) = fVar7;
          }
          else {
            *(double *)(param_3 + 0x70) = dVar8;
          }
          param_2[1] = param_2[1] + '\x01';
        }
      }
      else if (cVar1 == '\x06') {
        if (in_FPSCR_SZ == '\0') {
          fVar7 = *(float *)(param_3 + 0x6c);
        }
        else {
          fVar7 = (float)((ulonglong)*(undefined8 *)(param_3 + 0x6c) >> 0x20);
        }
        if (in_FPSCR_PR == '\0') {
          in_dr2 = (double)CONCAT44(fVar7 - DAT_0c9144e4,DAT_0c9144e4);
        }
        else {
          in_dr2 = (double)CONCAT44(fVar7,DAT_0c9144e4) - (double)CONCAT44(fVar7,DAT_0c9144e4);
        }
        if (in_FPSCR_SZ == '\0') {
          *(int *)(param_3 + 0x6c) = (int)((ulonglong)in_dr2 >> 0x20);
          dVar4 = (double)CONCAT44((int)((ulonglong)dVar4 >> 0x20),*(undefined4 *)(param_3 + 0x6c));
        }
        else {
          *(double *)(param_3 + 0x6c) = in_dr2;
        }
        in_dr2 = (double)((ulonglong)in_dr2 & 0xffffffff);
        if (in_FPSCR_PR == '\0') {
          bVar10 = SUB84(dVar4,0) < 0.0;
        }
        else {
          bVar10 = dVar4 < in_dr2;
        }
        if (bVar10) {
          if (in_FPSCR_SZ == '\0') {
            *(undefined4 *)(param_3 + 0x6c) = 0;
          }
          else {
            *(ulonglong *)(param_3 + 0x6c) = param_1 & 0xffffffff;
          }
          *(ushort *)(param_2 + 4) = *(ushort *)(param_2 + 4) | 0x40;
          **(uint **)(*(int *)(param_2 + 0x38) + 0xc) =
               **(uint **)(*(int *)(param_2 + 0x38) + 0xc) & 0xffffffef;
          puVar3 = (uint *)(*(int *)(*(int *)(param_2 + 0x38) + 0xc) + 0x34);
          *puVar3 = *puVar3 & 0xffffffef;
          puVar3 = (uint *)(*(int *)(*(int *)(param_2 + 0x38) + 0xc) + 0x68);
          *puVar3 = *puVar3 | 0x10;
          *(byte *)(*(int *)(*(int *)(param_2 + 0x38) + 0xc) + 4) =
               *(byte *)(*(int *)(*(int *)(param_2 + 0x38) + 0xc) + 4) & 0xf3;
          *(byte *)(*(int *)(*(int *)(param_2 + 0x38) + 0xc) + 0x38) =
               *(byte *)(*(int *)(*(int *)(param_2 + 0x38) + 0xc) + 0x38) & 0xf3;
          *(uint *)(param_3 + 0x60) = *(uint *)(param_3 + 0x60) & 0xfffffffb;
          param_2[1] = param_2[1] + '\x01';
        }
      }
      else if (cVar1 == '\a') {
        *(undefined2 *)(param_3 + 0x5c) = 0;
        *param_2 = 0;
        param_2[1] = 0;
        *(undefined2 *)(param_2 + 6) = 0;
      }
    }
  }
  if ((*(ushort *)(param_2 + 4) & 1) != 0) {
    if (in_FPSCR_SZ == '\0') {
      dVar4 = (double)CONCAT44(DAT_0c9144e8,SUB84(dVar8,0));
      in_dr2 = (double)CONCAT44((int)((ulonglong)in_dr2 >> 0x20),*(undefined4 *)(param_3 + 4));
    }
    else {
      dVar4 = (double)CONCAT44(_DAT_0c9144ec,DAT_0c9144e8);
      in_xd2 = *(undefined8 *)(param_3 + 4);
    }
    fVar7 = (float)((ulonglong)dVar4 >> 0x20);
    if (in_FPSCR_PR == '\0') {
      in_dr2 = (double)CONCAT44((int)((ulonglong)in_dr2 >> 0x20),SUB84(in_dr2,0) * fVar7);
    }
    else {
      in_dr2 = in_dr2 * dVar4;
    }
    fVar5 = -SUB84(in_dr2,0);
    if (in_FPSCR_SZ == '\0') {
      *(float *)(param_3 + 0x10) = fVar5;
      fVar5 = *(float *)(param_3 + 0xc);
    }
    else {
      *(undefined8 *)(param_3 + 0x10) = in_xd2;
      in_xd2 = *(undefined8 *)(param_3 + 0xc);
    }
    if (in_FPSCR_PR == '\0') {
      fVar5 = fVar5 * fVar7;
    }
    else {
      fVar5 = SUB84((double)CONCAT44((int)((ulonglong)in_dr2 >> 0x20),fVar5) * dVar4,0);
    }
    if (in_FPSCR_SZ == '\0') {
      *(float *)(param_3 + 0x18) = -fVar5;
    }
    else {
      *(undefined8 *)(param_3 + 0x18) = in_xd2;
    }
  }
  return;
}

