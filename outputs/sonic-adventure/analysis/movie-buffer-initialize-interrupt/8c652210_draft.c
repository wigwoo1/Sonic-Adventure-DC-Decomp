/* Ghidra draft only; not buildable/verified source. */

undefined4 callback_8c652210(void)

{
  undefined *puVar1;
  undefined4 *puVar2;
  uint *puVar3;
  int *piVar4;
  undefined4 *puVar5;
  undefined *puVar6;
  uint *puVar7;
  int iVar8;
  uint uVar9;
  uint uVar10;
  
  puVar3 = DAT_8c6522dc;
  puVar1 = PTR_FUN_8c6522c0;
  uVar10 = *DAT_8c6522dc;
  if (((uVar10 & 8) != 0) && (iVar8 = FUN_8c651bc0(), iVar8 != 0)) {
    (*(code *)puVar1)();
  }
  if ((uVar10 & 0x20) != 0) {
    *puVar3 = 0x20;
    *DAT_8c6522d8 = *DAT_8c6522d8 | 4;
  }
  if ((uVar10 & 4) != 0) {
    *puVar3 = 7;
    *DAT_8c6522e0 = 0;
    piVar4 = DAT_8c6522e4;
    puVar2 = DAT_8c6522c8;
    *DAT_8c6522c8 = 0;
    puVar2[0x11] = puVar2[0x10] - *piVar4;
    if (*DAT_8c6522e8 == 0) {
      *(undefined4 *)(DAT_8c652428 + *(int *)(DAT_8c652428 + 0x14) * 4) = 5;
    }
    else {
      *DAT_8c6522e8 = 0;
      puVar5 = DAT_8c6522ec;
      puVar2[6] = 0;
      puVar2 = DAT_8c6522f0;
      *DAT_8c6522f0 = *puVar5;
      puVar2[1] = *DAT_8c6522f4;
      *DAT_8c6522fc = *DAT_8c6522f8;
      *DAT_8c652304 = *DAT_8c652300;
    }
    *DAT_8c65242c = *DAT_8c65242c | 1;
    *DAT_8c652430 = 0;
  }
  uVar9 = DAT_8c652434;
  if ((uVar10 & DAT_8c652434) != 0) {
    *DAT_8c652438 = *DAT_8c652438 & 0xfffffffc;
    puVar6 = PTR_FUN_8c65243c;
    *puVar3 = uVar9;
    (*(code *)puVar6)();
  }
  if ((uVar10 & 0x40) != 0) {
    *puVar3 = 0x40;
    *DAT_8c65242c = *DAT_8c65242c | 0x40;
  }
  if ((uVar10 & (int)DAT_8c652420) != 0) {
    *puVar3 = (int)DAT_8c652420;
    puVar7 = DAT_8c652440;
    *DAT_8c652440 = *DAT_8c652440 | 1;
    if (*DAT_8c652444 == *puVar7) {
      FUN_8c651f1e();
    }
  }
  if ((uVar10 & (int)DAT_8c652422) != 0) {
    *puVar3 = (int)DAT_8c652422;
    puVar7 = DAT_8c652440;
    *DAT_8c652440 = *DAT_8c652440 | 2;
    if (*DAT_8c652444 == *puVar7) {
      FUN_8c651f1e();
    }
  }
  if ((uVar10 & (int)DAT_8c652424) != 0) {
    *puVar3 = (int)DAT_8c652424;
    puVar7 = DAT_8c652440;
    *DAT_8c652440 = *DAT_8c652440 | 4;
    if (*DAT_8c652444 == *puVar7) {
      FUN_8c651f1e();
    }
  }
  if ((uVar10 & (int)DAT_8c652426) != 0) {
    *puVar3 = (int)DAT_8c652426;
    puVar7 = DAT_8c652440;
    *DAT_8c652440 = *DAT_8c652440 | 8;
    if (*DAT_8c652444 == *puVar7) {
      FUN_8c651f1e();
    }
  }
  if ((uVar10 & DAT_8c652448) != 0) {
    *puVar3 = DAT_8c652448;
    puVar3 = DAT_8c652440;
    *DAT_8c652440 = *DAT_8c652440 | 0x10;
    if (*DAT_8c652444 == *puVar3) {
      FUN_8c651f1e();
    }
  }
  uVar10 = *DAT_8c65244c;
  if (((uVar10 & 4) != 0) && (iVar8 = FUN_8c652196(), iVar8 != 0)) {
    (*(code *)puVar1)();
  }
  if (((uVar10 & 8) != 0) && (iVar8 = FUN_8c6521ca(), iVar8 != 0)) {
    (*(code *)puVar1)();
  }
  if (((uVar10 & 2) != 0) && (iVar8 = FUN_8c6521f2(), iVar8 != 0)) {
    (*(code *)puVar1)();
  }
  puVar1 = PTR_FUN_8c652500;
  for (uVar10 = 0; (int)uVar10 < 9; uVar10 = uVar10 + 1) {
    if ((int)uVar10 < 0) {
      uVar9 = 1 >> (~uVar10 & 0x1f) + 1;
    }
    else {
      uVar9 = 1 << (uVar10 & 0x1f);
    }
    if ((*DAT_8c652504 & uVar9) != 0) {
      *DAT_8c652504 = *DAT_8c652504 & ~uVar9;
      (*(code *)puVar1)(*(undefined4 *)(DAT_8c652508 + -4 + uVar10 * 8),
                        *(undefined4 *)(DAT_8c652508 + uVar10 * 8));
    }
  }
  return 0;
}

