/* Ghidra draft only; not buildable/verified source. */

undefined4 FUN_8c64e0b4(int param_1,uint param_2)

{
  undefined4 *puVar1;
  undefined *puVar2;
  undefined *puVar3;
  int iVar4;
  undefined *puVar5;
  undefined *puVar6;
  undefined *puVar7;
  int iVar8;
  undefined4 uVar9;
  uint uVar10;
  int iVar11;
  int iVar12;
  int iVar13;
  uint uVar14;
  
  puVar5 = PTR_DAT_8c64e164;
  iVar4 = DAT_8c64e160;
  puVar3 = PTR_DAT_8c64e14c;
  if ((param_2 != 0) &&
     (iVar11 = *(int *)PTR_DAT_8c64e14c, param_2 + iVar11 <= *(uint *)PTR_DAT_8c64e138)) {
    iVar12 = *(int *)PTR_DAT_8c64e164;
    iVar8 = (*(code *)PTR_FUN_8c64e168)(&DAT_8c88f7a8);
    puVar2 = PTR_DAT_8c64e148;
    iVar11 = iVar11 - iVar12 * iVar8;
    iVar13 = *(int *)PTR_DAT_8c64e16c;
    iVar12 = (*(code *)PTR_FUN_8c64e168)();
    puVar1 = DAT_8c64e134;
    if ((iVar8 != iVar12) || (*(int *)PTR_DAT_8c64e154 == 0)) {
      iVar12 = (*(code *)PTR_FUN_8c64e170)(*DAT_8c64e134,iVar8 * iVar13,0);
      if (iVar12 != 0) goto LAB_8c64e128;
      iVar12 = (*(code *)PTR_FUN_8c64e398)
                         (*puVar1,*(undefined4 *)PTR_DAT_8c64e394,*(undefined4 *)PTR_DAT_8c64e390);
      if (iVar12 != 0) {
        uVar9 = (*(code *)PTR_FUN_8c64e39c)(*puVar1);
        puVar3 = PTR_FUN_8c64e3a0;
        *(undefined4 *)(iVar4 + 0xc) = uVar9;
        uVar9 = (*(code *)puVar3)(*puVar1);
        *(undefined4 *)(iVar4 + 0x10) = uVar9;
        *(undefined4 *)(iVar4 + 8) = 5;
        return 0xffffffff;
      }
      *(int *)puVar2 = iVar8 * iVar13;
    }
    puVar7 = PTR_DAT_8c64e3a4;
    if (iVar11 != *(int *)PTR_DAT_8c64e3a4) {
      *(int *)PTR_DAT_8c64e3a4 = iVar11;
    }
    uVar10 = *(int *)puVar5 - *(int *)puVar7;
    *(uint *)PTR_DAT_8c64e3a8 = uVar10;
    if (uVar10 < param_2) {
      uVar14 = 0;
      if (uVar10 != 0) {
        (*(code *)PTR_FUN_8c64e3ac)(param_1,*(int *)PTR_DAT_8c64e390 + *(int *)puVar7);
        uVar14 = *(uint *)PTR_DAT_8c64e3a8;
        *(uint *)puVar3 = *(int *)puVar3 + uVar14;
      }
      for (; uVar14 < param_2; uVar14 = uVar14 + uVar10) {
        iVar11 = *(int *)PTR_DAT_8c64e394;
        uVar10 = param_2 - uVar14;
        if (*(uint *)puVar5 < uVar10) {
          iVar8 = (*(code *)PTR_FUN_8c64e3b0)();
          iVar11 = (*(code *)PTR_FUN_8c64e398)(iVar11 * iVar8,param_1 + uVar14);
          puVar6 = PTR_DAT_8c64e394;
          if (iVar11 != 0) {
            uVar9 = (*(code *)PTR_FUN_8c64e39c)(*puVar1);
            puVar3 = PTR_FUN_8c64e3a0;
            *(undefined4 *)(iVar4 + 0xc) = uVar9;
            uVar9 = (*(code *)puVar3)(*puVar1);
            *(undefined4 *)(iVar4 + 0x10) = uVar9;
            *(undefined4 *)(iVar4 + 8) = 5;
            return 0xffffffff;
          }
          uVar10 = *(int *)puVar5 * iVar8;
          *(uint *)puVar3 = *(int *)puVar3 + uVar10;
          *(int *)puVar2 = *(int *)puVar2 + *(int *)puVar6 * iVar8;
          *(undefined4 *)puVar7 = 0;
        }
        else {
          iVar11 = (*(code *)PTR_FUN_8c64e398)(*puVar1,iVar11,*(undefined4 *)PTR_DAT_8c64e390);
          if (iVar11 != 0) {
            uVar9 = (*(code *)PTR_FUN_8c64e39c)(*puVar1);
            puVar3 = PTR_FUN_8c64e3a0;
            *(undefined4 *)(iVar4 + 0xc) = uVar9;
            uVar9 = (*(code *)puVar3)(*puVar1);
            *(undefined4 *)(iVar4 + 0x10) = uVar9;
            *(undefined4 *)(iVar4 + 8) = 5;
            return 0xffffffff;
          }
          (*(code *)PTR_FUN_8c64e3ac)(param_1 + uVar14,*(undefined4 *)PTR_DAT_8c64e390,uVar10);
          *(uint *)puVar3 = *(int *)puVar3 + uVar10;
          *(int *)puVar2 = *(int *)puVar2 + *(int *)PTR_DAT_8c64e394;
          *(uint *)puVar7 = uVar10;
          *(uint *)PTR_DAT_8c64e3a8 = *(int *)puVar5 - uVar10;
        }
      }
    }
    else {
      (*(code *)PTR_FUN_8c64e3ac)(param_1,*(int *)PTR_DAT_8c64e390 + *(int *)puVar7,param_2);
      puVar2 = PTR_DAT_8c64e3a8;
      *(uint *)puVar7 = *(int *)puVar7 + param_2;
      *(uint *)puVar3 = *(int *)puVar3 + param_2;
      *(int *)puVar2 = *(int *)puVar5 - *(int *)puVar7;
    }
    return 1;
  }
LAB_8c64e128:
  *(undefined4 *)(iVar4 + 8) = 5;
  return 0xffffffff;
}

