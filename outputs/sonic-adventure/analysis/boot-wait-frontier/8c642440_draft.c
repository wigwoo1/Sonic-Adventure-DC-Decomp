/* Ghidra draft only; not buildable/verified source. */

/* WARNING: Restarted to delay deadcode elimination for space: stack */

undefined4 FUN_8c642440(void)

{
  int *piVar1;
  undefined *puVar2;
  undefined *puVar3;
  uint uVar4;
  int iVar5;
  uint uVar6;
  uint uVar7;
  int iVar8;
  uint local_78;
  uint local_74;
  uint local_68;
  undefined4 local_64;
  undefined4 local_60;
  undefined4 local_5c;
  undefined4 local_58;
  undefined4 local_54;
  undefined4 local_50;
  undefined4 local_4c;
  int local_48 [5];
  undefined4 *local_34 [6];
  
  local_68 = *(uint *)PTR_DAT_8c642660;
  local_64 = *(undefined4 *)(PTR_DAT_8c642660 + 4);
  local_60 = *(undefined4 *)(PTR_DAT_8c642660 + 8);
  local_5c = *(undefined4 *)(PTR_DAT_8c642660 + 0xc);
  local_58 = *(undefined4 *)(PTR_DAT_8c642660 + 0x10);
  local_54 = *(undefined4 *)(PTR_DAT_8c642660 + 0x14);
  local_50 = *(undefined4 *)(PTR_DAT_8c642660 + 0x18);
  local_4c = *(undefined4 *)(PTR_DAT_8c642660 + 0x1c);
  if (((((*DAT_8c642664 & 0x40) != 0) && (*DAT_8c642668 != 0)) && (*DAT_8c64266c != 0)) &&
     (*DAT_8c642670 != 0)) {
    FUN_8c641bc0();
  }
  piVar1 = DAT_8c6426a0;
  iVar8 = *DAT_8c642674 * 4;
  local_34[0] = (undefined4 *)(DAT_8c642678 + iVar8);
  local_34[2] = (undefined4 *)(DAT_8c642680 + iVar8);
  local_34[1] = (undefined4 *)(DAT_8c64267c + iVar8);
  local_34[3] = (undefined4 *)(DAT_8c642684 + iVar8);
  local_34[4] = (undefined4 *)(DAT_8c642688 + iVar8);
  local_48[0] = DAT_8c64268c + iVar8;
  local_48[1] = DAT_8c642690 + iVar8;
  local_48[2] = DAT_8c642694 + iVar8;
  local_48[3] = DAT_8c642698 + iVar8;
  local_48[4] = iVar8 + DAT_8c64269c;
  *(undefined4 *)(DAT_8c642678 + iVar8) = *(undefined4 *)*DAT_8c6426a0;
  *local_34[1] = *(undefined4 *)(*piVar1 + 4);
  *local_34[2] = *(undefined4 *)(*piVar1 + 8);
  *local_34[3] = *(undefined4 *)(*piVar1 + 0xc);
  puVar2 = PTR_FUN_8c6426ac;
  *local_34[4] = *(undefined4 *)(*piVar1 + 0x10);
  piVar1 = DAT_8c6426a8;
  uVar4 = DAT_8c6426a4;
  if (*DAT_8c6426b0 == 0) {
    local_74 = 0;
    do {
      puVar3 = PTR_DAT_8c6426b8;
      if ((int)local_74 < 0) {
        uVar6 = 1 >> (~local_74 & 0x1f) + 1;
      }
      else {
        uVar6 = 1 << (local_74 & 0x1f);
      }
      if ((uVar6 & *DAT_8c6426b4) != 0) {
        iVar8 = local_74 * 4;
        iVar5 = *(int *)(iVar8 + *piVar1) - *local_34[local_74];
        if (iVar5 < 0) {
          iVar5 = iVar5 + 3;
        }
        *(int *)local_48[local_74] = (iVar5 >> 2) << 2;
        (*(code *)puVar2)(*(undefined4 *)(iVar8 + *piVar1),puVar3,4);
        *(int *)(iVar8 + *piVar1) = *(int *)(iVar8 + *piVar1) + 0x20;
      }
      local_74 = local_74 + 1;
    } while ((int)local_74 < 5);
    uVar6 = 0;
    do {
      if ((int)uVar6 < 0) {
        uVar7 = 1 >> (~uVar6 & 0x1f) + 1;
      }
      else {
        uVar7 = 1 << (uVar6 & 0x1f);
      }
      if ((uVar7 & *DAT_8c6426b4) == 0) {
        *(undefined4 *)local_48[uVar6] = 0;
      }
      uVar6 = uVar6 + 1;
    } while ((int)uVar6 < 5);
    uVar6 = 0;
LAB_8c6425ba:
    if ((int)uVar6 < 0) {
      uVar7 = 1 >> (~uVar6 & 0x1f) + 1;
    }
    else {
      uVar7 = 1 << (uVar6 & 0x1f);
    }
    if ((uVar7 & *DAT_8c6426b4) == 0) goto LAB_8c642654;
    if (*(uint *)(uVar6 * 4 + DAT_8c6426bc) < *(uint *)local_48[uVar6]) {
      *(undefined4 *)(DAT_8c6426bc + uVar6 * 4) = *(undefined4 *)local_48[uVar6];
    }
    if (*(uint *)local_48[uVar6] <= *(uint *)(uVar6 * 4 + DAT_8c6426c0)) goto LAB_8c642654;
    uVar6 = 0;
    do {
      if ((int)uVar6 < 0) {
        uVar7 = 1 >> (~uVar6 & 0x1f) + 1;
      }
      else {
        uVar7 = 1 << (uVar6 & 0x1f);
      }
      if ((uVar7 & *DAT_8c6426b4) != 0) {
        local_68 = local_68 & uVar4 | uVar6 << 0x18;
        (*(code *)puVar2)(*local_34[uVar6],&local_68,0x20);
        (*(code *)puVar2)(*local_34[uVar6] + 0x20,PTR_DAT_8c6426b8,4);
        *(undefined4 *)local_48[uVar6] = 0x20;
      }
      uVar6 = uVar6 + 1;
    } while ((int)uVar6 < 5);
    goto LAB_8c6429be;
  }
  if ((*DAT_8c642880 & 0x20) == 0) {
    (*(code *)PTR_FUN_8c642a40)();
    local_78 = 0;
    do {
      puVar3 = PTR_DAT_8c642a44;
      iVar8 = local_78 * 4;
      iVar5 = *(int *)(iVar8 + *piVar1) - *local_34[local_78];
      if (iVar5 < 0) {
        iVar5 = iVar5 + 3;
      }
      *(int *)local_48[local_78] = (iVar5 >> 2) << 2;
      (*(code *)puVar2)(*(undefined4 *)(iVar8 + *piVar1),puVar3,4);
      *(int *)(iVar8 + *piVar1) = *(int *)(iVar8 + *piVar1) + 0x20;
      if ((int)local_78 < 0) {
        uVar6 = 1 >> (~local_78 & 0x1f) + 1;
      }
      else {
        uVar6 = 1 << (local_78 & 0x1f);
      }
      if ((uVar6 & *DAT_8c642a48) == 0) {
        *(undefined4 *)local_48[local_78] = 0;
      }
      if ((int)local_78 < 0) {
        uVar6 = 1 >> (~local_78 & 0x1f) + 1;
      }
      else {
        uVar6 = 1 << (local_78 & 0x1f);
      }
      if ((uVar6 & *DAT_8c642a48) != 0) {
        iVar8 = local_78 * 4;
        if (*(uint *)(DAT_8c642a4c + iVar8) < *(uint *)local_48[local_78]) {
          *(undefined4 *)(DAT_8c642a4c + iVar8) = *(undefined4 *)local_48[local_78];
        }
        if (*(uint *)(DAT_8c642a50 + iVar8) < *(uint *)local_48[local_78]) {
          uVar6 = 0;
          do {
            if ((int)uVar6 < 0) {
              uVar7 = 1 >> (~uVar6 & 0x1f) + 1;
            }
            else {
              uVar7 = 1 << (uVar6 & 0x1f);
            }
            if ((uVar7 & *DAT_8c642a48) == 0) {
              *(undefined4 *)local_48[uVar6] = 0;
            }
            else {
              local_68 = local_68 & uVar4 | uVar6 << 0x18;
              (*(code *)puVar2)(*local_34[uVar6],&local_68,0x20);
              (*(code *)puVar2)(*local_34[uVar6] + 0x20,PTR_DAT_8c642a44,4);
              *(undefined4 *)local_48[uVar6] = 0x20;
            }
            uVar6 = uVar6 + 1;
          } while ((int)uVar6 < 5);
LAB_8c6429be:
          (*(code *)PTR_FUN_8c642a54)(2);
          break;
        }
      }
      local_78 = local_78 + 1;
    } while ((int)local_78 < 5);
    goto LAB_8c6429d6;
  }
  local_78 = 0;
  do {
    puVar3 = PTR_DAT_8c64288c;
    if (local_78 != *DAT_8c642884) {
      if ((int)local_78 < 0) {
        uVar6 = 1 >> (~local_78 & 0x1f) + 1;
      }
      else {
        uVar6 = 1 << (local_78 & 0x1f);
      }
      if ((uVar6 & *DAT_8c642888) == 0) {
        *(undefined4 *)local_48[local_78] = 0;
      }
      else {
        iVar8 = local_78 * 4;
        iVar5 = *(int *)(iVar8 + *piVar1) - *local_34[local_78];
        if (iVar5 < 0) {
          iVar5 = iVar5 + 3;
        }
        *(int *)local_48[local_78] = (iVar5 >> 2) << 2;
        (*(code *)puVar2)(*(undefined4 *)(iVar8 + *piVar1),puVar3,4);
        *(int *)(iVar8 + *piVar1) = *(int *)(iVar8 + *piVar1) + 0x20;
        if (*(uint *)(DAT_8c642890 + iVar8) < *(uint *)local_48[local_78]) {
          *(undefined4 *)(DAT_8c642890 + local_78 * 4) = *(undefined4 *)local_48[local_78];
        }
        if (*(uint *)(local_78 * 4 + DAT_8c642894) < *(uint *)local_48[local_78]) {
          uVar6 = 0;
          goto LAB_8c642784;
        }
      }
    }
    local_78 = local_78 + 1;
  } while ((int)local_78 < 5);
  goto LAB_8c64280c;
LAB_8c642654:
  uVar6 = uVar6 + 1;
  if (4 < (int)uVar6) goto LAB_8c6429d6;
  goto LAB_8c6425ba;
LAB_8c642784:
  do {
    if (uVar6 == *DAT_8c642884) {
LAB_8c6427de:
      *(undefined4 *)local_48[uVar6] = 0;
    }
    else {
      if ((int)uVar6 < 0) {
        uVar7 = 1 >> (~uVar6 & 0x1f) + 1;
      }
      else {
        uVar7 = 1 << (uVar6 & 0x1f);
      }
      if ((uVar7 & *DAT_8c642888) == 0) goto LAB_8c6427de;
      local_68 = local_68 & uVar4 | uVar6 << 0x18;
      (*(code *)puVar2)(*local_34[uVar6],&local_68,0x20);
      (*(code *)puVar2)(*local_34[uVar6] + 0x20,PTR_DAT_8c64288c,4);
      *(undefined4 *)local_48[uVar6] = 0x20;
    }
    uVar6 = uVar6 + 1;
  } while ((int)uVar6 < 5);
  (*(code *)PTR_FUN_8c642898)(2);
LAB_8c64280c:
  if (*(int *)(*piVar1 + *DAT_8c642884 * 4) == *local_34[*DAT_8c642884]) {
    *DAT_8c64289c = 0x10;
    *DAT_8c6428a0 = 0x10;
    local_68 = uVar4 & local_68 | (*DAT_8c642884 & 7) << 0x18;
    (*(code *)puVar2)(*(uint *)(*piVar1 + *DAT_8c642884 * 4) & DAT_8c6428a4 | DAT_8c6428a8,&local_68
                      ,0x20);
  }
LAB_8c6429d6:
  puVar3 = PTR_FUN_8c642a60;
  puVar2 = PTR_DAT_8c642a5c;
  *(undefined4 *)PTR_DAT_8c642a58 = 0;
  uVar4 = *(int *)puVar2 + 1U & 1;
  *(uint *)puVar2 = uVar4;
  (*(code *)puVar3)(uVar4);
  return 0;
}

