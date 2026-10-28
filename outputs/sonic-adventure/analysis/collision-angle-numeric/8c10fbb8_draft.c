/* Ghidra draft only; not buildable/verified source. */

/* WARNING: Removing unreachable block (ram,0x8c10fc46) */
/* WARNING: Removing unreachable block (ram,0x8c10fbec) */
/* WARNING: Removing unreachable block (ram,0x8c10fc76) */
/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined8 FUN_8c10fbb8(undefined8 param_1,int *param_2)

{
  undefined *puVar1;
  uint uVar2;
  uint uVar3;
  uint unaff_r14;
  float in_fr1;
  float fVar4;
  float fVar6;
  double dVar5;
  double dVar7;
  undefined8 in_xd0;
  undefined8 in_xd2;
  undefined8 uVar8;
  undefined8 in_xd4;
  bool bVar9;
  int iVar10;
  char in_FPSCR_PR;
  char in_FPSCR_SZ;
  float local_14;
  float local_10;
  uint uStack_c;
  uint local_8;
  
  puVar1 = PTR_DAT_8c10fca4;
  uVar3 = (uint)((ulonglong)param_1 >> 0x20);
  uVar2 = (uint)param_1;
  uStack_c = uVar2;
  local_8 = uVar3;
  if (in_FPSCR_SZ != '\0') {
    uStack_c = (uint)in_xd4;
    local_8 = (uint)((ulonglong)in_xd4 >> 0x20);
    unaff_r14 = uVar3;
  }
  if (((local_8 & DAT_8c10fc9c) != DAT_8c10fc9c) &&
     (((uStack_c & DAT_8c10fc9c) != DAT_8c10fc9c || ((uStack_c & DAT_8c10fca0) == 0)))) {
    if (in_FPSCR_SZ == '\0') {
      dVar5 = (double)((ulonglong)uStack_c << 0x20);
    }
    else {
      dVar5 = (double)CONCAT44(local_8,uStack_c);
    }
    fVar4 = (float)((ulonglong)dVar5 >> 0x20);
    if (fVar4 != SUB84(dVar5,0)) {
      if ((uStack_c & DAT_8c10fc9c) == DAT_8c10fc9c) {
        *param_2 = 0;
        local_10 = (float)uVar2;
        if (in_FPSCR_SZ == '\0') {
          local_10 = (float)uVar3;
          uVar3 = uStack_c;
        }
      }
      else {
        uVar3 = local_8 & DAT_8c10fca8;
        local_8 = local_8 & DAT_8c10fcac;
        uStack_c = uStack_c & DAT_8c10fcac;
        if (in_FPSCR_SZ == '\0') {
          dVar5 = (double)CONCAT44(fVar4,uStack_c);
          dVar7 = (double)CONCAT44(local_8,uVar2);
        }
        else {
          in_xd2 = CONCAT44(local_8,uStack_c);
          dVar7 = (double)CONCAT44(unaff_r14,local_8);
        }
        if (in_FPSCR_PR == '\0') {
          dVar7 = (double)CONCAT44((float)((ulonglong)dVar7 >> 0x20) / SUB84(dVar5,0),SUB84(dVar7,0)
                                  );
        }
        else {
          dVar7 = dVar7 / dVar5;
        }
        fVar4 = (float)((ulonglong)dVar7 >> 0x20);
        local_14 = fVar4;
        if (in_FPSCR_SZ != '\0') {
          local_14 = SUB84(dVar7,0);
          local_10 = fVar4;
        }
        uVar2 = (uint)local_14 & DAT_8c10fca8;
        if ((int)(DAT_8c10fc9c & (uint)local_14) < (int)DAT_8c10fcb0) {
          if (in_FPSCR_PR == '\0') {
            iVar10 = (int)fVar4;
          }
          else {
            iVar10 = (int)dVar7;
          }
          local_14 = (float)iVar10;
          dVar5 = (double)(ulonglong)(uint)local_14;
          if (in_FPSCR_SZ != '\0') {
            local_14 = (float)in_xd2;
            local_10 = (float)((ulonglong)in_xd2 >> 0x20);
          }
        }
        uVar2 = (uint)local_14 | uVar2;
        if (in_FPSCR_SZ == '\0') {
          dVar7 = (double)CONCAT44(uVar2,in_fr1);
          dVar5 = (double)CONCAT44(DAT_8c10fcb4,SUB84(dVar5,0));
        }
        else {
          dVar7 = (double)CONCAT44(local_10,uVar2);
          dVar5 = (double)CONCAT44(DAT_8c10fcb8,DAT_8c10fcb4);
        }
        fVar4 = (float)((ulonglong)dVar7 >> 0x20);
        fVar6 = (float)((ulonglong)dVar5 >> 0x20);
        if (in_FPSCR_PR == '\0') {
          bVar9 = fVar6 < fVar4;
        }
        else {
          bVar9 = dVar5 < dVar7;
        }
        if (bVar9) {
          if (in_FPSCR_SZ == '\0') {
            dVar5 = (double)CONCAT44(fVar6,fVar4);
            dVar7 = (double)CONCAT44(fVar4,DAT_8c10fcb8);
          }
          else {
            in_xd0 = CONCAT44(_FUN_8c10fcbc,DAT_8c10fcb8);
          }
          if (in_FPSCR_PR == '\0') {
            dVar5 = (double)CONCAT44((int)((ulonglong)dVar5 >> 0x20),SUB84(dVar5,0) + SUB84(dVar7,0)
                                    );
          }
          else {
            dVar5 = dVar5 + dVar7;
          }
        }
        else if (in_FPSCR_SZ == '\0') {
          dVar5 = (double)CONCAT44(fVar6,fVar4);
        }
        if (in_FPSCR_PR == '\0') {
          iVar10 = (int)SUB84(dVar5,0);
        }
        else {
          iVar10 = (int)dVar5;
        }
        *param_2 = iVar10;
        fVar4 = (float)iVar10;
        if (iVar10 < 0) {
          if (in_FPSCR_PR == '\0') {
            fVar4 = fVar4 + DAT_8c10fcb0;
          }
          else {
            fVar4 = SUB84((double)CONCAT44(DAT_8c10fcb0,fVar4) +
                          (double)CONCAT44(DAT_8c10fcb0,fVar4),0);
          }
        }
        if (in_FPSCR_SZ == '\0') {
          dVar5 = (double)CONCAT44(uStack_c,fVar4);
          dVar7 = (double)CONCAT44((int)((ulonglong)dVar7 >> 0x20),local_8);
        }
        else {
          dVar5 = (double)CONCAT44(local_8,uStack_c);
          in_xd0 = CONCAT44(unaff_r14,local_8);
        }
        if (in_FPSCR_PR == '\0') {
          in_fr1 = SUB84(dVar7,0) - SUB84(dVar5,0) * (float)((ulonglong)dVar5 >> 0x20);
        }
        else {
          in_fr1 = SUB84(dVar7 - dVar5 * dVar5,0);
        }
        local_10 = in_fr1;
        if (in_FPSCR_SZ != '\0') {
          local_10 = (float)in_xd0;
          uStack_c = (uint)((ulonglong)in_xd0 >> 0x20);
        }
        local_10 = (float)((uint)local_10 | uVar3);
        uVar3 = uStack_c;
      }
      goto LAB_8c10fc94;
    }
  }
  *param_2 = 0;
  if (in_FPSCR_SZ == '\0') {
    local_10 = *(float *)puVar1;
    uVar3 = uStack_c;
  }
  else {
    uVar8 = *(undefined8 *)puVar1;
    local_10 = (float)uVar8;
    uStack_c = (uint)((ulonglong)uVar8 >> 0x20);
    uVar3 = uStack_c;
  }
LAB_8c10fc94:
  uStack_c = uVar3;
  if (in_FPSCR_SZ == '\0') {
    uVar8 = CONCAT44(local_10,in_fr1);
  }
  else {
    uVar8 = CONCAT44(uStack_c,local_10);
  }
  return uVar8;
}

