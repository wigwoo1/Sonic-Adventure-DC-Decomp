/* Ghidra draft only; not buildable/verified source. */

/* WARNING: Removing unreachable block (ram,0x8c052c68) */
/* WARNING: Removing unreachable block (ram,0x8c052c16) */
/* WARNING: Removing unreachable block (ram,0x8c052bc4) */
/* WARNING: Removing unreachable block (ram,0x8c052c28) */
/* WARNING: Removing unreachable block (ram,0x8c052c1e) */
/* WARNING: Removing unreachable block (ram,0x8c052c70) */
/* WARNING: Removing unreachable block (ram,0x8c052c7a) */

void FUN_8c052ba0(undefined8 param_1)

{
  short sVar1;
  undefined *puVar2;
  int *piVar3;
  undefined *puVar4;
  uint uVar5;
  short *psVar6;
  int iVar7;
  int iVar8;
  int iVar9;
  undefined8 *puVar10;
  longlong lVar11;
  undefined8 uVar12;
  double dVar13;
  undefined4 uVar14;
  double dVar15;
  undefined8 in_xd4;
  undefined4 in_PR;
  char in_FPSCR_PR;
  char in_FPSCR_SZ;
  short local_40;
  int local_38;
  undefined4 *local_30;
  undefined4 uStack_2c;
  undefined4 uStack_28;
  undefined4 uStack_24;
  undefined4 local_20;
  
  local_20 = in_PR;
  uVar5 = (*(code *)PTR_FUN_8c052da0)();
  piVar3 = DAT_8c052db0;
  puVar2 = PTR_DAT_8c052dac;
  local_38 = 0;
  local_30 = (undefined4 *)(PTR_DAT_8c052da8 + ((int)(uVar5 & 0xffff & DAT_8c052da4) >> 8) * 0x20);
  do {
    psVar6 = (short *)*local_30;
    *piVar3 = (int)psVar6;
    if (psVar6 != (short *)0x0) {
      sVar1 = *psVar6;
      iVar7 = 0;
      local_40 = 0;
      if (0 < sVar1) {
        do {
          uVar14 = (undefined4)param_1;
          puVar10 = (undefined8 *)(*(int *)(*piVar3 + 0xc) + iVar7);
          iVar8 = *(int *)(puVar10 + 3);
          iVar9 = *(int *)(iVar8 + 4);
          if (iVar9 != 0) {
            if (in_FPSCR_SZ == '\0') {
              lVar11 = (ulonglong)*(uint *)((int)puVar10 + 0xc) << 0x20;
            }
            else {
              lVar11 = *(longlong *)((int)puVar10 + 0xc);
            }
            if ((float)((ulonglong)lVar11 >> 0x20) != (float)lVar11) {
              if (in_FPSCR_SZ == '\0') {
                uVar12 = CONCAT44(*(undefined4 *)(puVar10 + 2),(float)lVar11);
              }
              else {
                uVar12 = puVar10[2];
              }
              if ((float)((ulonglong)uVar12 >> 0x20) == (float)uVar12) {
                if (in_FPSCR_SZ == '\0') {
                  lVar11 = (ulonglong)*(uint *)((int)puVar10 + 0x14) << 0x20;
                }
                else {
                  lVar11 = *(longlong *)((int)puVar10 + 0x14);
                }
                if ((float)((ulonglong)lVar11 >> 0x20) == (float)lVar11) goto LAB_8c052cfe;
              }
            }
            (*(code *)PTR_FUN_8c052db4)(puVar2);
            if (in_FPSCR_SZ == '\0') {
              param_1 = CONCAT44(*(undefined4 *)(iVar8 + 0x10),uVar14);
              uVar14 = *(undefined4 *)(iVar8 + 0xc);
            }
            else {
              param_1 = *(undefined8 *)(iVar8 + 0x10);
              uVar14 = (undefined4)*(undefined8 *)(iVar8 + 8);
              in_xd4 = *(undefined8 *)(iVar8 + 0xc);
            }
            (*(code *)PTR_FUN_8c052db8)(param_1,0);
            if (*(int *)(iVar8 + 0x14) != 0) {
              (*(code *)PTR_FUN_8c052dbc)(0,*(int *)(iVar8 + 0x14));
            }
            if (*(int *)(iVar8 + 0x18) != 0) {
              (*(code *)PTR_FUN_8c052dc0)(0,*(int *)(iVar8 + 0x18));
            }
            if (in_FPSCR_SZ == '\0') {
              dVar13 = (double)((ulonglong)*(uint *)((int)puVar10 + 0xc) << 0x20);
            }
            else {
              dVar13 = *(double *)((int)puVar10 + 0xc);
            }
            if ((float)((ulonglong)dVar13 >> 0x20) == SUB84(dVar13,0)) {
              if (in_FPSCR_SZ == '\0') {
                dVar13 = (double)CONCAT44(*(undefined4 *)(puVar10 + 2),SUB84(dVar13,0));
              }
              else {
                dVar13 = (double)puVar10[2];
              }
              if ((float)((ulonglong)dVar13 >> 0x20) != SUB84(dVar13,0)) goto LAB_8c052c9c;
              if (in_FPSCR_SZ == '\0') {
                dVar13 = (double)((ulonglong)*(uint *)((int)puVar10 + 0x14) << 0x20);
              }
              else {
                dVar13 = *(double *)((int)puVar10 + 0x14);
              }
              if ((float)((ulonglong)dVar13 >> 0x20) != SUB84(dVar13,0)) goto LAB_8c052c9c;
              if (in_FPSCR_SZ == '\0') {
                uStack_2c = *(undefined4 *)(iVar9 + 0x18);
                uStack_28 = *(undefined4 *)(iVar9 + 0x1c);
                uStack_24 = *(undefined4 *)(iVar9 + 0x20);
                uVar14 = *(undefined4 *)(iVar9 + 0x24);
              }
              else {
                uStack_2c = (undefined4)*(undefined8 *)(iVar9 + 0x18);
                uStack_28 = (undefined4)*(undefined8 *)(iVar9 + 0x1c);
                uStack_24 = (undefined4)*(undefined8 *)(iVar9 + 0x20);
                local_20 = (undefined4)((ulonglong)*(undefined8 *)(iVar9 + 0x20) >> 0x20);
                in_xd4 = *(undefined8 *)(iVar9 + 0x24);
              }
            }
            else {
LAB_8c052c9c:
              if (in_FPSCR_SZ == '\0') {
                uStack_2c = *(undefined4 *)puVar10;
                uStack_28 = *(undefined4 *)((int)puVar10 + 4);
                uStack_24 = *(undefined4 *)(puVar10 + 1);
                dVar15 = (double)CONCAT44(*(undefined4 *)((int)puVar10 + 0xc),
                                          *(undefined4 *)(puVar10 + 2));
                dVar13 = (double)CONCAT44((int)((ulonglong)dVar13 >> 0x20),
                                          *(undefined4 *)((int)puVar10 + 0xc));
              }
              else {
                uStack_2c = (undefined4)*puVar10;
                uStack_28 = (undefined4)*(undefined8 *)((int)puVar10 + 4);
                uStack_24 = (undefined4)puVar10[1];
                local_20 = (undefined4)((ulonglong)puVar10[1] >> 0x20);
                dVar15 = *(double *)((int)puVar10 + 0xc);
                in_xd4 = puVar10[2];
              }
              if (in_FPSCR_PR == '\0') {
                uVar14 = SUB84(dVar15,0);
              }
              else {
                uVar14 = SUB84(dVar15 * dVar13,0);
              }
              if (in_FPSCR_SZ == '\0') {
                uVar14 = *(undefined4 *)((int)puVar10 + 0x14);
                uVar12 = in_xd4;
              }
              else {
                uVar12 = *(undefined8 *)((int)puVar10 + 0x14);
              }
              in_xd4 = (*(code *)PTR_FUN_8c052dc4)();
              if (in_FPSCR_SZ == '\0') {
                uVar14 = (undefined4)((ulonglong)in_xd4 >> 0x20);
                in_xd4 = uVar12;
              }
            }
            puVar4 = PTR_FUN_8c052dc8;
            if (in_FPSCR_SZ == '\0') {
              *(undefined4 *)((int)puVar10 + 0xc) = uVar14;
            }
            else {
              *(undefined8 *)((int)puVar10 + 0xc) = in_xd4;
            }
            (*(code *)puVar4)(0,&uStack_2c,&uStack_2c);
            (*(code *)PTR_FUN_8c052dcc)(1);
            if (in_FPSCR_SZ == '\0') {
              *(undefined4 *)puVar10 = uStack_2c;
              *(undefined4 *)((int)puVar10 + 4) = uStack_28;
              *(undefined4 *)(puVar10 + 1) = uStack_24;
            }
            else {
              *puVar10 = CONCAT44(uStack_28,uStack_2c);
              *(ulonglong *)((int)puVar10 + 4) = CONCAT44(uStack_24,uStack_28);
              puVar10[1] = CONCAT44(local_20,uStack_24);
            }
          }
LAB_8c052cfe:
          local_40 = local_40 + 1;
          iVar7 = iVar7 + 0x24;
        } while (local_40 < sVar1);
      }
    }
    local_38 = local_38 + 1;
    local_30 = local_30 + 1;
    if (7 < local_38) {
      return;
    }
  } while( true );
}

