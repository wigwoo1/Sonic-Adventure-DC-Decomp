/* Ghidra draft only; not buildable/verified source. */

undefined4 FUN_8c608134(undefined4 param_1,int *param_2)

{
  bool bVar1;
  undefined *puVar2;
  undefined *puVar3;
  undefined4 *puVar4;
  uint *puVar5;
  undefined4 uVar6;
  uint uVar7;
  int iVar8;
  int iVar9;
  uint uVar10;
  uint uVar11;
  int *piVar12;
  int local_74;
  uint local_68;
  int local_64;
  short local_60;
  int local_5c;
  int local_58;
  undefined4 local_54 [11];
  int iStack_28;
  short sStack_24;
  short sStack_22;
  
  puVar2 = PTR_DAT_8c6081d8;
  uVar10 = 0xffffffff;
  (*(code *)PTR_FUN_8c6081dc)();
  (*(code *)PTR_FUN_8c6081e0)(param_1);
  (*(code *)PTR_FUN_8c6081c8)(&local_64,4);
  if (local_64 == DAT_8c6081e4) {
    (*(code *)PTR_FUN_8c6081c8)(&iStack_28,8);
    local_60 = sStack_22;
    param_2[1] = (int)sStack_22;
    uVar6 = (*(code *)PTR_FUN_8c6081e8)((int)sStack_24);
    FUN_8c607ebc(param_2,uVar6,&iStack_28);
    (*(code *)PTR_FUN_8c6081cc)(0);
    (*(code *)PTR_FUN_8c6081cc)(iStack_28 + 8,1);
    FUN_8c607f90();
    param_2[1] = (int)sStack_22;
    for (local_68 = 0; (int)local_68 < (int)local_60; local_68 = local_68 + 1) {
      local_5c = FUN_8c607244(param_2,local_68);
      iVar9 = *param_2 + local_5c * 0xc;
      bVar1 = false;
      piVar12 = (int *)(iVar9 + 8);
      if ((*(uint *)(iVar9 + 4) & DAT_8c608434) == 0) {
        uVar11 = *(uint *)(iVar9 + 8);
        local_74 = (*(code *)PTR_FUN_8c60843c)(*(undefined4 *)puVar2,uVar11);
        if (local_74 < 0) {
          iVar8 = 2;
        }
        else {
          iVar8 = 1;
        }
      }
      else {
        uVar11 = *(uint *)(*param_2 + local_5c * 0xc + 8) & DAT_8c608440;
        uVar10 = (*(uint *)(iVar9 + 8) & DAT_8c608444) >> 0x1a;
        local_74 = (*(code *)PTR_FUN_8c60843c)
                             (*(undefined4 *)puVar2,*(undefined4 *)PTR_DAT_8c608438,uVar11);
        if (local_74 < 0) {
          iVar8 = 2;
        }
        else {
          iVar8 = (*(code *)PTR_FUN_8c608448)
                            (*(undefined4 *)puVar2,*(undefined4 *)PTR_DAT_8c608438,uVar11,uVar10);
          if (iVar8 < 0) {
            iVar8 = 0;
          }
          else {
            iVar8 = 1;
          }
        }
        bVar1 = true;
      }
      if (iVar8 == 0) {
        iVar9 = local_74 * 0x44 + *(int *)puVar2;
        uVar7 = *(uint *)(iVar9 + 0x10);
        FUN_8c6080c8(local_54);
        if ((uVar7 & 2) == 0) {
          uVar7 = *(uint *)(iVar9 + 4) & DAT_8c60844c;
          if (uVar7 == 0) {
LAB_8c608468:
            local_58 = (*(code *)PTR_FUN_8c6086b8)(iVar9,local_74 * 0x44 + *(int *)puVar2 + 0x14);
            if (local_58 < 0) {
              (*(code *)PTR_FUN_8c6086bc)
                        (piVar12,local_74 * 0x44 + *(int *)puVar2 + 0x14,uVar11,1,uVar10);
LAB_8c6085a8:
              *(short *)(*piVar12 + 0x42) = -(short)local_58;
              goto LAB_8c608770;
            }
            (*(code *)PTR_FUN_8c6086bc)
                      (piVar12,local_74 * 0x44 + *(int *)puVar2 + 0x14,uVar11,1,uVar10);
            *(undefined2 *)(*piVar12 + 0x42) = 0;
            *(undefined2 *)(*piVar12 + 0x40) = 1;
LAB_8c6085de:
            (*(code *)PTR_FUN_8c6086d4)(uVar11,2,*(undefined4 *)(*piVar12 + 0x14));
          }
          else if (uVar7 == DAT_8c608450) {
            (*(code *)PTR_FUN_8c60845c)
                      (piVar12,local_74 * 0x44 + *(int *)puVar2 + 0x14,uVar11,1,uVar10);
          }
          else if (uVar7 == DAT_8c608454) goto LAB_8c608468;
        }
        else {
          uVar7 = *(uint *)(iVar9 + 4) & DAT_8c60844c;
          if (uVar7 == 0) {
            uVar6 = 2;
LAB_8c6083c2:
            (*(code *)PTR_FUN_8c60845c)
                      (piVar12,local_74 * 0x44 + *(int *)puVar2 + 0x14,uVar11,uVar6,uVar10);
            *(undefined2 *)(*piVar12 + 0x42) = 0;
            *(undefined2 *)(*piVar12 + 0x40) = 1;
            (*(code *)PTR_FUN_8c608460)(*piVar12);
LAB_8c6087a0:
            (*(code *)PTR_FUN_8c6087e8)(*piVar12,uVar10);
          }
          else if ((uVar7 == DAT_8c608450) || (uVar7 == DAT_8c608454)) {
            if ((*(uint *)(iVar9 + 0x10) & 1) != 0) {
              uVar6 = 3;
              goto LAB_8c6083c2;
            }
            iVar9 = (*(code *)PTR_FUN_8c608458)(0,local_54);
            if (iVar9 == 0) {
              (*(code *)PTR_FUN_8c60845c)
                        (piVar12,local_74 * 0x44 + *(int *)puVar2 + 0x14,uVar11,2,uVar10);
              *(undefined2 *)(*piVar12 + 0x42) = 0;
              *(undefined2 *)(*piVar12 + 0x40) = 1;
              (*(code *)PTR_FUN_8c608460)(*piVar12);
              (*(code *)PTR_FUN_8c608464)(*piVar12,uVar10);
              goto LAB_8c608770;
            }
            (*(code *)PTR_FUN_8c60845c)
                      (piVar12,local_74 * 0x44 + *(int *)puVar2 + 0x14,uVar11,2,uVar10);
            *(undefined4 *)(*piVar12 + 0x14) = local_54[0];
            *(undefined2 *)(*piVar12 + 0x42) = 0;
            *(undefined2 *)(*piVar12 + 0x40) = 1;
            (*(code *)PTR_FUN_8c608460)(*piVar12);
            (*(code *)PTR_FUN_8c608464)(*piVar12,uVar10);
            for (local_68 = 0; local_68 < *(uint *)PTR_DAT_8c608438; local_68 = local_68 + 1) {
              iVar9 = local_68 * 0x44;
              if (*(uint *)(*(int *)puVar2 + iVar9) == uVar11) {
                *(uint *)(*(int *)puVar2 + iVar9 + 0x10) =
                     *(uint *)(*(int *)puVar2 + iVar9 + 0x10) | 1;
                *(undefined4 *)(*(int *)puVar2 + iVar9 + 0x14) = *(undefined4 *)(*piVar12 + 0x14);
              }
            }
          }
        }
      }
      else if (iVar8 == 1) {
        FUN_8c6080c8(local_54);
        *piVar12 = local_74 * 0x44 + *(int *)puVar2;
        uVar7 = *(uint *)(iVar9 + 4) & DAT_8c6086c0;
        if (uVar7 == 0) {
          if ((*(uint *)(*piVar12 + 0x10) & 2) == 0) {
            local_58 = (*(code *)PTR_FUN_8c6086b8)(iVar9,*piVar12 + 0x14);
            if (local_58 < 0) goto LAB_8c6085a8;
            *(undefined2 *)(*piVar12 + 0x42) = 0;
            *(uint *)(*piVar12 + 0x10) = *(uint *)(*piVar12 + 0x10) | 2;
            *(short *)(*piVar12 + 0x40) = *(short *)(*piVar12 + 0x40) + 1;
            (*(code *)PTR_FUN_8c6086cc)(*piVar12);
            if (!bVar1) goto LAB_8c6087a8;
            goto LAB_8c6085de;
          }
          *(short *)(*piVar12 + 0x40) = *(short *)(*piVar12 + 0x40) + 1;
        }
        else if (uVar7 == DAT_8c6086c4) {
LAB_8c60856a:
          if ((*(uint *)(*piVar12 + 0x10) & 1) == 0) {
            local_58 = (*(code *)PTR_FUN_8c6086d0)(0,local_54);
            if (local_58 == 0) goto LAB_8c6085a8;
            *(undefined4 *)(*piVar12 + 0x14) = local_54[0];
            *(uint *)(*piVar12 + 0x10) = *(uint *)(*piVar12 + 0x10) | 1;
          }
        }
        else if (uVar7 == DAT_8c6086c8) {
          if ((*(uint *)(*piVar12 + 0x10) & 2) != 0) {
            *(short *)(*piVar12 + 0x40) = *(short *)(*piVar12 + 0x40) + 1;
            goto LAB_8c60856a;
          }
          local_58 = (*(code *)PTR_FUN_8c6086b8)(iVar9,*piVar12 + 0x14);
          if (local_58 < 0) goto LAB_8c6085a8;
          *(undefined2 *)(*piVar12 + 0x42) = 0;
          *(uint *)(*piVar12 + 0x10) = *(uint *)(*piVar12 + 0x10) | 2;
          *(short *)(*piVar12 + 0x40) = *(short *)(*piVar12 + 0x40) + 1;
          (*(code *)PTR_FUN_8c6086cc)(*piVar12);
          if (bVar1) goto LAB_8c6085de;
        }
      }
      else if (iVar8 == 2) {
        FUN_8c6080c8(local_54);
        puVar4 = DAT_8c6086d8;
        uVar7 = *(uint *)(iVar9 + 4) & DAT_8c6086c0;
        if (uVar7 == 0) {
          uVar6 = (*(code *)PTR_FUN_8c6086dc)(uVar11);
          puVar3 = PTR_FUN_8c6086e0;
          *puVar4 = uVar6;
          (*(code *)puVar3)(uVar6,local_54,0x2c);
          local_74 = (*(code *)PTR_FUN_8c6086e4)(iVar9,*puVar4);
          if (-1 < local_74) {
            (*(code *)PTR_FUN_8c6086bc)(iVar9 + 8,*puVar4,uVar11,2,uVar10);
            puVar3 = PTR_FUN_8c6086cc;
            *(undefined2 *)(*piVar12 + 0x42) = 0;
            (*(code *)puVar3)(*piVar12);
            if (!bVar1) goto LAB_8c6087a8;
            goto LAB_8c6087a0;
          }
LAB_8c608682:
          (*(code *)PTR_FUN_8c6086bc)(piVar12,*puVar4,uVar11,0,uVar10);
          *(short *)(*piVar12 + 0x42) = -(short)local_74;
          *(undefined2 *)(*piVar12 + 0x40) = 0;
LAB_8c608770:
          puVar5 = DAT_8c6087f0;
          *DAT_8c6087ec = local_5c;
          *puVar5 = uVar11;
          goto LAB_8c60877a;
        }
        if (uVar7 == DAT_8c6086c4) {
          uVar6 = (*(code *)PTR_FUN_8c6086dc)(uVar11);
          puVar3 = PTR_FUN_8c6086e0;
          *puVar4 = uVar6;
          (*(code *)puVar3)(uVar6,local_54,0x2c);
          local_74 = (*(code *)PTR_FUN_8c6086d0)(0,*puVar4);
          if (local_74 == 0) goto LAB_8c608682;
          (*(code *)PTR_FUN_8c6086bc)(piVar12,*puVar4,uVar11,1,uVar10);
        }
        else if (uVar7 == DAT_8c6086c8) {
          uVar6 = (*(code *)PTR_FUN_8c6087d0)(uVar11);
          puVar3 = PTR_FUN_8c6087d4;
          *puVar4 = uVar6;
          (*(code *)puVar3)(uVar6,local_54,0x2c);
          iVar9 = (*(code *)PTR_FUN_8c6087d8)(iVar9,*puVar4);
          if (iVar9 < 0) {
            (*(code *)PTR_FUN_8c6087dc)(piVar12,*puVar4,uVar11,0,uVar10);
            *(short *)(*piVar12 + 0x42) = -(short)iVar9;
            *(undefined2 *)(*piVar12 + 0x40) = 0;
            goto LAB_8c60877a;
          }
          iVar9 = (*(code *)PTR_FUN_8c6087e0)(0,*puVar4);
          if (iVar9 == 0) {
            (*(code *)PTR_FUN_8c6087dc)(piVar12,*puVar4,uVar11,2,uVar10);
            *(undefined2 *)(*piVar12 + 0x42) = 0;
            *(undefined2 *)(*piVar12 + 0x40) = 0;
            (*(code *)PTR_FUN_8c6087e4)(*piVar12);
            if (bVar1) {
              (*(code *)PTR_FUN_8c6087e8)(*piVar12,uVar10);
            }
            goto LAB_8c608770;
          }
          (*(code *)PTR_FUN_8c6087dc)(piVar12,*puVar4,uVar11,3,uVar10);
          (*(code *)PTR_FUN_8c6087e4)(*piVar12);
          *(undefined2 *)(*piVar12 + 0x42) = 0;
          if (bVar1) goto LAB_8c6087a0;
        }
      }
LAB_8c6087a8:
    }
    uVar6 = 1;
  }
  else {
    *DAT_8c6081ec = 4;
LAB_8c60877a:
    uVar6 = 0xffffffff;
  }
  return uVar6;
}

