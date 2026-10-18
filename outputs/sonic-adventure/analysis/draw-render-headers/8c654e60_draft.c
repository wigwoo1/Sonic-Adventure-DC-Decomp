/* Ghidra draft only; not buildable/verified source. */

/* WARNING: Removing unreachable block (ram,0x8c655390) */
/* WARNING: Removing unreachable block (ram,0x8c655362) */
/* WARNING: Removing unreachable block (ram,0x8c655238) */
/* WARNING: Removing unreachable block (ram,0x8c654f62) */
/* WARNING: Removing unreachable block (ram,0x8c654ec4) */
/* WARNING: Removing unreachable block (ram,0x8c654eb2) */
/* WARNING: Removing unreachable block (ram,0x8c654f4a) */
/* WARNING: Removing unreachable block (ram,0x8c655196) */
/* WARNING: Removing unreachable block (ram,0x8c65526e) */
/* WARNING: Removing unreachable block (ram,0x8c655400) */
/* WARNING: Removing unreachable block (ram,0x8c65550c) */
/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined4 FUN_8c654e60(uint *param_1)

{
  int iVar1;
  uint uVar2;
  uint uVar3;
  uint uVar4;
  uint uVar5;
  uint uVar6;
  uint uVar7;
  uint uVar8;
  uint uVar9;
  uint uVar10;
  uint uVar11;
  uint local_3c;
  int local_30 [5];
  
  uVar6 = DAT_8c654ee8;
  iVar1 = (int)DAT_8c654ee0;
  uVar9 = *(uint *)(iVar1 + (int)param_1);
  uVar11 = *(uint *)(iVar1 + 4 + (int)param_1);
  local_3c = *(uint *)(iVar1 + 8 + (int)param_1);
  uVar8 = *(uint *)((int)DAT_8c654ee2 + (int)param_1);
  local_30[0] = *(int *)PTR_DAT_8c654ee4;
  local_30[1] = *(undefined4 *)(PTR_DAT_8c654ee4 + 4);
  local_30[2] = *(undefined4 *)(PTR_DAT_8c654ee4 + 8);
  local_30[3] = *(undefined4 *)(PTR_DAT_8c654ee4 + 0xc);
  uVar10 = *param_1;
  if ((uVar10 & 1) != 0) {
    uVar9 = uVar9 & DAT_8c654ee8 | param_1[5] << 0x1d;
  }
  if ((uVar10 & 2) != 0) {
    uVar9 = DAT_8c654eec & uVar9 | param_1[6] << 0x1b;
  }
  if ((uVar10 & 0x20) != 0) {
    if (param_1[10] == 0) {
      uVar9 = uVar9 & DAT_8c654f84;
    }
    else {
      uVar9 = uVar9 | DAT_8c654ef0;
    }
  }
  uVar7 = uVar10 & 8;
  if (uVar7 != 0) {
    uVar9 = DAT_8c654f88 & uVar9 | (param_1[8] >> 1) << 0x19;
  }
  uVar3 = (uint)DAT_8c654f80;
  if ((uVar3 & uVar10) != 0) {
    uVar9 = uVar9 & DAT_8c654f8c | param_1[0x10] << 0x18;
  }
  if (uVar7 != 0) {
    uVar9 = DAT_8c654f90 & uVar9 | (param_1[8] & 1) << 0x17;
  }
  uVar2 = DAT_8c654f94 & uVar10;
  if (uVar2 != 0) {
    uVar9 = DAT_8c654f98 & uVar9 | param_1[4] << 0x16;
  }
  if ((uVar10 & DAT_8c654ef0) != 0) {
    if (*(int *)((int)DAT_8c654f82 + (int)param_1) == 0) {
      uVar9 = uVar9 & DAT_8c654fc4;
    }
    else {
      uVar9 = uVar9 | DAT_8c654f9c;
    }
  }
  uVar4 = uVar11 & DAT_8c654ee8;
  if ((uVar10 & 0x40) == 0) goto LAB_8c655082;
  switch(param_1[0xb]) {
  case 0:
    uVar11 = _DAT_8c655044;
    uVar5 = _DAT_8c655040;
    goto LAB_8c654ffa;
  case 1:
    uVar11 = _DAT_8c65504c;
    uVar5 = DAT_8c655048;
LAB_8c654ffa:
    uVar11 = (uVar4 | uVar5) & DAT_8c654fc8 | uVar11;
    goto LAB_8c655082;
  case 2:
    uVar11 = _DAT_8c655050;
    break;
  case 3:
    uVar11 = _DAT_8c655054;
    break;
  case 4:
    uVar11 = _DAT_8c655058;
    break;
  case 5:
    uVar11 = _DAT_8c65505c;
    break;
  case 6:
    uVar11 = _DAT_8c655040;
    break;
  default:
    goto LAB_8c655082;
  case 8:
    uVar11 = DAT_8c655048;
    break;
  case 10:
    uVar11 = _DAT_8c655060;
    break;
  case 0xb:
    uVar11 = uVar4;
    goto LAB_8c655082;
  }
  uVar11 = uVar11 | uVar4;
LAB_8c655082:
  if (((int)DAT_8c6550a6 & uVar10) == 0) goto LAB_8c65510a;
  uVar4 = uVar11 & DAT_8c654fc8;
  switch(param_1[0xc]) {
  case 0:
    uVar4 = _DAT_8c655144;
    uVar5 = _DAT_8c655140;
    goto LAB_8c6550da;
  case 1:
    uVar4 = DAT_8c65514c;
    uVar5 = _DAT_8c655148;
LAB_8c6550da:
    uVar11 = (uVar11 & DAT_8c654ee8 | uVar5) & DAT_8c654fc8 | uVar4;
    goto LAB_8c65510a;
  case 2:
    uVar11 = _DAT_8c655150;
    break;
  default:
    goto LAB_8c65510a;
  case 4:
    uVar11 = _DAT_8c655154;
    break;
  case 6:
    uVar11 = DAT_8c65514c;
    break;
  case 7:
    uVar11 = _DAT_8c655158;
    break;
  case 8:
    uVar11 = _DAT_8c655144;
    break;
  case 9:
    uVar11 = _DAT_8c65515c;
    break;
  case 10:
    uVar11 = uVar4 | DAT_8c654ef0;
    goto LAB_8c65510a;
  case 0xb:
    uVar11 = uVar4;
    goto LAB_8c65510a;
  }
  uVar11 = uVar11 | uVar4;
LAB_8c65510a:
  if ((uVar10 & DAT_8c655164) != 0) {
    if (param_1[0xd] == 0) {
      uVar11 = uVar11 & DAT_8c655168;
    }
    else {
      uVar11 = uVar11 | DAT_8c655160;
    }
  }
  if ((DAT_8c655160 & uVar10) != 0) {
    if (param_1[0xe] == 0) {
      uVar11 = uVar11 & DAT_8c654f8c;
    }
    else {
      uVar11 = uVar11 | DAT_8c655164;
    }
  }
  if (((int)DAT_8c6551e0 & uVar10) != 0) {
    uVar11 = DAT_8c6551e8 & uVar11 | (param_1[0xf] & 3) << 0x16;
  }
  if ((DAT_8c6551ec & uVar10) != 0) {
    if (param_1[0x19] == 0) {
      uVar11 = uVar11 & DAT_8c6551f4;
    }
    else {
      uVar11 = uVar11 | DAT_8c6551f0;
    }
  }
  if (((int)DAT_8c6551e2 & uVar10) != 0) {
    if (param_1[0x11] == 0) {
      uVar11 = uVar11 & DAT_8c6551fc;
    }
    else {
      uVar11 = uVar11 | DAT_8c6551f8;
    }
  }
  if (((int)DAT_8c6551e4 & uVar10) != 0) {
    if (param_1[0x12] == 0) {
      uVar11 = uVar11 & DAT_8c6552a8;
    }
    else {
      uVar11 = uVar11 | DAT_8c655200;
    }
  }
  if ((uVar10 & (int)DAT_8c6552a0) != 0) {
    uVar11 = DAT_8c6552ac & uVar11 | param_1[0x13] << 0xf;
  }
  if (((int)DAT_8c6552a2 & uVar10) != 0) {
    uVar11 = DAT_8c6552b0 & uVar11 | param_1[0x14] << 0x11;
  }
  if (((int)DAT_8c6552a4 & uVar10) != 0) {
    uVar11 = (int)DAT_8c6552a6 & uVar11 | local_30[param_1[0x15]] << 0xd;
  }
  if ((DAT_8c6552b4 & uVar10) != 0) {
    if (param_1[0x16] == 0) {
      uVar11 = uVar11 & (int)DAT_8c6553a0;
    }
    else {
      uVar11 = uVar11 | (int)DAT_8c6552a0;
    }
  }
  if ((DAT_8c6553a8 & uVar10) != 0) {
    uVar4 = param_1[0x17] & 0xf;
    if (uVar4 == 0) {
      uVar4 = 4;
    }
    uVar11 = (int)DAT_8c6553a2 & uVar11 | uVar4 << 8;
  }
  if ((DAT_8c6553ac & uVar10) != 0) {
    uVar11 = (int)DAT_8c6553a4 & uVar11 | (param_1[0x18] & 3) << 6;
  }
  if (((int)param_1[8] >> 1 == 1) && (param_1[0x1b] != 0)) {
    iVar1 = FUN_8c654b60(*(undefined4 *)(param_1[0x1b] + 0xc));
    uVar4 = FUN_8c654b60(*(undefined4 *)(param_1[0x1b] + 0x10),uVar11 & 0xffffffc7 | iVar1 << 3);
    uVar11 = uVar11 & 0xffffffc0 | iVar1 << 3 | uVar4;
  }
  uVar4 = DAT_8c655604;
  if (((int)param_1[8] >> 1 == 1) && (param_1[0x1b] != 0)) {
    uVar5 = *(uint *)(param_1[0x1b] + 0x18);
    uVar5 = ((local_3c & DAT_8c6553b0 | (uint)((uVar5 & 1) != 0) << 0x1f) & DAT_8c6553b4 |
            (uVar5 & 8) << 0x1b) & DAT_8c6553bc | *(uint *)(param_1[0x1b] + 8) & DAT_8c6553b8;
    if (((uVar5 & DAT_8c6553b8) == DAT_8c6553c0) || ((uVar5 & DAT_8c6553b8) == DAT_8c6553c4)) {
      uVar5 = DAT_8c6553c8 & uVar5 | (param_1[0x1a] & 0x3f) << 0x15;
    }
    else {
      uVar5 = DAT_8c655488 &
              (uVar5 & DAT_8c655484 | (~(*(uint *)(param_1[0x1b] + 0x18) & 4) & 4) << 0x18) |
              (*(uint *)(param_1[0x1b] + 0x18) & 0x10) << 0x15;
    }
    local_3c = uVar5 & DAT_8c65548c | *(uint *)(param_1[0x1b] + 0x1c) >> 3;
  }
  if ((DAT_8c655490 & uVar10) != 0) {
    uVar5 = param_1[1];
    if (uVar5 == 0) {
      uVar8 = DAT_8c655494 | uVar8 & uVar6;
    }
    if (uVar5 == 1) {
      uVar8 = DAT_8c655494 | uVar8 & uVar6;
    }
    if (uVar5 == 2) {
      uVar8 = DAT_8c655498 | uVar6 & uVar8;
    }
  }
  if ((DAT_8c65549c & uVar10) != 0) {
    uVar8 = DAT_8c6554a0 & uVar8 | param_1[2] << 0x18;
  }
  if ((DAT_8c6554ac & uVar10) == 0) {
    iVar1 = *DAT_8c655588;
  }
  else {
    iVar1 = *(int *)((int)DAT_8c655480 + (int)param_1);
  }
  uVar6 = iVar1 << 0x12 | DAT_8c6554a8 & (uVar8 | DAT_8c6554a4);
  if ((DAT_8c65558c & uVar10) != 0) {
    uVar6 = DAT_8c655590 & uVar6 | *(int *)((int)DAT_8c655580 + (int)param_1) << 0x10;
  }
  uVar6 = (int)DAT_8c655582 & uVar6;
  if (((*(uint *)((int)DAT_8c655584 + (int)param_1) & DAT_8c655594) != 0) && ((param_1[2] & 1) != 0)
     ) {
    uVar6 = uVar6 | 0x40;
  }
  if (((uVar10 & 0x10) != 0) && ((param_1[9] & 1) != 0)) {
    uVar6 = (int)DAT_8c655586 & uVar6 & 0xffffffbf | param_1[9] << 7 | *(int *)PTR_DAT_8c655598 << 6
    ;
  }
  if ((uVar10 & DAT_8c65559c) != 0) {
    uVar6 = uVar6 & 0xffffffcf | param_1[3] << 4;
  }
  uVar8 = uVar6;
  if (uVar7 != 0) {
    uVar5 = (param_1[8] >> 1) << 3;
    uVar8 = uVar6 & 0xfffffff7 | uVar5;
    if (uVar7 != 0) {
      uVar8 = uVar6 & 0xfffffff5 | uVar5 | (param_1[8] & 1) << 1;
    }
  }
  if ((uVar3 & uVar10) != 0) {
    if (param_1[0x10] == 0) {
      uVar8 = uVar8 & 0xfffffffb;
    }
    else {
      uVar8 = uVar8 | 4;
    }
  }
  if (uVar2 != 0) {
    uVar8 = param_1[4] | uVar8 & 0xfffffffe;
  }
  iVar1 = (int)DAT_8c655600;
  *(uint *)(iVar1 + (int)param_1) = uVar9 & DAT_8c655608;
  *(uint *)(iVar1 + 4 + (int)param_1) = uVar11;
  iVar1 = (int)DAT_8c655602;
  *(uint *)(iVar1 + (int)param_1) = local_3c;
  *(uint *)(iVar1 + -0xc + (int)param_1) = uVar8 & uVar4;
  return 0;
}

