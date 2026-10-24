/* Ghidra draft only; not buildable/verified source. */

void FUN_8c65194a(void)

{
  undefined4 *puVar1;
  int iVar2;
  uint *puVar3;
  uint uVar4;
  undefined4 uVar5;
  undefined4 *puVar6;
  uint uVar7;
  uint uVar8;
  
  puVar1 = DAT_8c651a64;
  uVar8 = 0;
  puVar6 = DAT_8c651a64 + 0x16;
  while ((*(int *)(uVar8 * 4 + DAT_8c651a68) != 3 || (*DAT_8c651a6c != 0))) {
    uVar8 = uVar8 + 1;
    if (3 < (int)uVar8) {
      return;
    }
  }
  uVar7 = uVar8 & 1;
  if ((uVar7 + 1 & *DAT_8c651a70) != 0) {
    (*(code *)PTR_FUN_8c651a80)(*DAT_8c651a7c,*DAT_8c651a78,0,1,uVar7,~*DAT_8c651a74 & 1,1);
    *DAT_8c651a70 = ~(uVar7 + 1) & *DAT_8c651a70 & 3;
  }
  *DAT_8c651a6c = 1;
  *(undefined4 *)(DAT_8c651a68 + uVar8 * 4) = 4;
  if ((*DAT_8c651a84 & 0x10) != 0) {
    *puVar1 = 2;
    *puVar1 = 0;
  }
  iVar2 = DAT_8c651a88;
  *DAT_8c651a8c = *(undefined4 *)(DAT_8c651a88 + uVar7 * 0x20);
  *DAT_8c651a90 = *(undefined4 *)(iVar2 + 0x14 + uVar7 * 0x20);
  if (*DAT_8c651a94 != 1) {
    if (((*DAT_8c651a98 & 1) == 1) || ((*DAT_8c651a98 & 0x2c) != 0x20)) {
      uVar5 = *(undefined4 *)(DAT_8c651b60 + uVar7 * 4);
    }
    else {
      uVar5 = *(undefined4 *)(DAT_8c651a9c + *DAT_8c651aa0 * 4);
    }
    *puVar6 = uVar5;
  }
  puVar6 = DAT_8c651b68;
  puVar1 = DAT_8c651b64;
  *DAT_8c651b68 = *DAT_8c651b64;
  puVar6[-1] = puVar1[-1] & 0xfffffff0;
  puVar1 = DAT_8c651b70;
  *DAT_8c651b70 = *DAT_8c651b6c;
  puVar1[1] = *DAT_8c651b74;
  puVar1[-7] = *DAT_8c651b78;
  puVar1[-8] = *DAT_8c651b7c;
  puVar3 = DAT_8c651b84;
  *DAT_8c651b80 = 0;
  if ((*puVar3 & 0xffffffed) == 0x20) {
    if (*DAT_8c651b88 == 0) {
      uVar4 = *DAT_8c651b90;
      uVar7 = DAT_8c651b8c;
    }
    else {
      uVar4 = *DAT_8c651b90;
      uVar7 = DAT_8c651b94;
    }
    *DAT_8c651b98 = uVar4 | uVar7;
  }
  puVar3 = DAT_8c651ba4;
  *DAT_8c651ba0 = *DAT_8c651b9c;
  *puVar3 = uVar8;
  puVar1 = DAT_8c651bac;
  *DAT_8c651ba8 = 1;
  *puVar1 = 0xffffffff;
  return;
}

