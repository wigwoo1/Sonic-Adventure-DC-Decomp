/* Ghidra draft only; not buildable/verified source. */

/* WARNING: Removing unreachable block (ram,0x8c076684) */
/* WARNING: Removing unreachable block (ram,0x8c0766b6) */

void FUN_8c076640(int param_1,int param_2,int param_3,undefined4 param_4)

{
  undefined *puVar1;
  undefined *puVar2;
  int iVar3;
  undefined4 uVar4;
  int iVar5;
  undefined8 *puVar6;
  double dVar7;
  float fVar9;
  double dVar8;
  double in_dr2;
  uint uVar11;
  ulonglong uVar10;
  double in_dr14;
  undefined8 in_xd2;
  undefined8 in_xd14;
  bool bVar12;
  undefined4 in_PR;
  char in_FPSCR_PR;
  char in_FPSCR_SZ;
  undefined8 local_1c;
  
  if (in_FPSCR_SZ == '\0') {
    puVar6 = (undefined8 *)((int)&local_1c + 4);
    local_1c = CONCAT44(SUB84(in_dr14,0),(undefined4)local_1c);
  }
  else {
    puVar6 = &local_1c;
    local_1c = in_xd14;
  }
  *(undefined4 *)((int)puVar6 + -4) = in_PR;
  *(undefined4 *)((int)puVar6 + -0x2c) = param_4;
  if ((*(ushort *)(param_2 + 0x10) & 3) == 0) {
    dVar7 = (double)(*(code *)PTR_FUN_8c076880)(param_2 + 0x38);
    if (in_FPSCR_SZ == '\0') {
      in_dr2 = (double)CONCAT44((int)((ulonglong)in_dr2 >> 0x20),*(undefined4 *)(param_2 + 0xc0));
    }
    else {
      in_xd2 = *(undefined8 *)(param_2 + 0xc0);
    }
    if (in_FPSCR_PR == '\0') {
      bVar12 = SUB84(in_dr2,0) < (float)((ulonglong)dVar7 >> 0x20);
    }
    else {
      bVar12 = in_dr2 < dVar7;
    }
    if (!bVar12) {
      return;
    }
  }
  uVar11 = (uint)((ulonglong)in_dr2 >> 0x20);
  fVar9 = (float)(*(code *)PTR_FUN_8c076884)
                           (*(undefined4 *)((int)puVar6 + -0x2c),param_3 + 0x48,
                            (undefined1 *)((int)puVar6 + -0x10));
  puVar1 = PTR_FUN_8c076880;
  uVar10 = (ulonglong)uVar11 << 0x20;
  if (fVar9 != 0.0) {
    uVar4 = *(undefined4 *)(param_3 + 0x14);
    *(undefined4 *)((int)puVar6 + -0x1c) = *(undefined4 *)(param_3 + 0x10);
    *(undefined4 *)((int)puVar6 + -0x18) = uVar4;
    *(undefined4 *)((int)puVar6 + -0x14) = *(undefined4 *)(param_3 + 0x18);
    uVar4 = *(undefined4 *)(param_3 + 0x70);
    *(undefined4 *)((int)puVar6 + -0x28) = *(undefined4 *)(param_3 + 0x6c);
    *(undefined4 *)((int)puVar6 + -0x24) = uVar4;
    *(undefined4 *)((int)puVar6 + -0x20) = *(undefined4 *)(param_3 + 0x74);
    fVar9 = (float)(*(code *)PTR_FUN_8c076880)((undefined1 *)((int)puVar6 + -0x28));
    puVar2 = PTR_FUN_8c076888;
    dVar7 = (double)(uVar10 & 0xffffffff00000000);
    if (fVar9 != 0.0) {
      (*(code *)PTR_FUN_8c076888)((undefined1 *)((int)puVar6 + -0x1c));
      (*(code *)puVar2)((undefined1 *)((int)puVar6 + -0x28));
      iVar3 = (*(code *)PTR_FUN_8c07688c)
                        ((undefined1 *)((int)puVar6 + -0x1c),(undefined1 *)((int)puVar6 + -0x28),
                         param_3 + 0x1c);
      iVar5 = DAT_8c076890;
      iVar3 = *(int *)(param_1 + 0x18) - iVar3;
      *(int *)(param_1 + 0x18) = iVar3;
      iVar3 = (*(code *)PTR_FUN_8c076894)
                        (*(undefined4 *)(*(int *)((uint)*(byte *)(param_1 + 8) * 4 + iVar5) + 0x20),
                         iVar3);
      if (0x6000 < iVar3) {
        *(int *)(param_1 + 0x18) = *(int *)(param_1 + 0x18) + DAT_8c076898;
      }
      *(undefined4 *)(*(int *)((uint)*(byte *)(param_1 + 8) * 4 + iVar5) + 0x20) =
           *(undefined4 *)(param_1 + 0x18);
      uVar4 = *(undefined4 *)(param_3 + 0x70);
      *(undefined4 *)((int)puVar6 + -0x10) = *(undefined4 *)(param_3 + 0x6c);
      *(undefined4 *)((int)puVar6 + -0xc) = uVar4;
      *(undefined4 *)((int)puVar6 + -8) = *(undefined4 *)(param_3 + 0x74);
      dVar8 = (double)(*(code *)puVar1)((undefined1 *)((int)puVar6 + -0x10));
      if (in_FPSCR_SZ == '\0') {
        dVar7 = (double)CONCAT44((int)((ulonglong)dVar7 >> 0x20),(undefined4)DAT_8c07689c);
      }
      else {
        in_xd2 = CONCAT44(DAT_8c07689c._4_4_,(undefined4)DAT_8c07689c);
      }
      if (in_FPSCR_PR == '\0') {
        bVar12 = (float)((ulonglong)dVar8 >> 0x20) < SUB84(dVar7,0);
      }
      else {
        bVar12 = dVar8 < dVar7;
      }
      if (!bVar12) {
        iVar5 = *(int *)((uint)*(byte *)(param_1 + 8) * 4 + iVar5);
        uVar4 = (*(code *)puVar1)(iVar5 + 4);
        if (in_FPSCR_SZ == '\0') {
          in_dr14 = (double)CONCAT44((int)((ulonglong)in_dr14 >> 0x20),uVar4);
          dVar7 = (double)CONCAT44((int)((ulonglong)dVar7 >> 0x20),DAT_8c07689c._4_4_);
        }
        else {
          in_xd2 = CONCAT44(DAT_8c0768a4,DAT_8c07689c._4_4_);
        }
        if (in_FPSCR_PR == '\0') {
          bVar12 = SUB84(dVar7,0) < SUB84(in_dr14,0);
        }
        else {
          bVar12 = dVar7 < in_dr14;
        }
        if (bVar12) {
          if (in_FPSCR_SZ == '\0') {
            dVar7 = (double)CONCAT44(DAT_8c0768a4,SUB84(dVar7,0));
          }
          else {
            dVar7 = (double)CONCAT44(PTR_FUN_8c0768a8,DAT_8c0768a4);
          }
          if (in_FPSCR_PR == '\0') {
            in_dr14 = (double)CONCAT44((int)((ulonglong)in_dr14 >> 0x20),
                                       SUB84(in_dr14,0) * (float)((ulonglong)dVar7 >> 0x20));
          }
          else {
            in_dr14 = in_dr14 * dVar7;
          }
        }
        (*(code *)puVar2)((undefined1 *)((int)puVar6 + -0x10));
        if (in_FPSCR_SZ == '\0') {
          dVar7 = (double)CONCAT44((int)((ulonglong)dVar7 >> 0x20),
                                   *(undefined4 *)((int)puVar6 + -0x10));
        }
        else {
          in_xd2 = *(undefined8 *)((int)puVar6 + -0x10);
        }
        fVar9 = SUB84(in_dr14,0);
        if (in_FPSCR_PR == '\0') {
          dVar7 = (double)CONCAT44((int)((ulonglong)dVar7 >> 0x20),SUB84(dVar7,0) * fVar9);
        }
        else {
          dVar7 = dVar7 * in_dr14;
        }
        if (in_FPSCR_SZ == '\0') {
          *(int *)(iVar5 + 4) = SUB84(dVar7,0);
          dVar7 = (double)CONCAT44((int)((ulonglong)dVar7 >> 0x20),
                                   *(undefined4 *)((int)puVar6 + -0xc));
        }
        else {
          *(undefined8 *)(iVar5 + 4) = in_xd2;
          in_xd2 = *(undefined8 *)((int)puVar6 + -0xc);
        }
        if (in_FPSCR_PR == '\0') {
          dVar7 = (double)CONCAT44((int)((ulonglong)dVar7 >> 0x20),SUB84(dVar7,0) * fVar9);
        }
        else {
          dVar7 = dVar7 * in_dr14;
        }
        if (in_FPSCR_SZ == '\0') {
          *(int *)(iVar5 + 8) = SUB84(dVar7,0);
          dVar7 = (double)CONCAT44((int)((ulonglong)dVar7 >> 0x20),*(undefined4 *)((int)puVar6 + -8)
                                  );
        }
        else {
          *(undefined8 *)(iVar5 + 8) = in_xd2;
          in_xd2 = *(undefined8 *)((int)puVar6 + -8);
        }
        if (in_FPSCR_PR == '\0') {
          fVar9 = SUB84(dVar7,0) * fVar9;
        }
        else {
          fVar9 = SUB84(dVar7 * in_dr14,0);
        }
        if (in_FPSCR_SZ == '\0') {
          *(float *)(iVar5 + 0xc) = fVar9;
        }
        else {
          *(undefined8 *)(iVar5 + 0xc) = in_xd2;
        }
      }
    }
  }
  return;
}

