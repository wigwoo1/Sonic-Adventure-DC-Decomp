/* Ghidra draft only; not buildable/verified source. */

void FUN_8c051e56(undefined8 param_1)

{
  undefined *puVar1;
  undefined *puVar2;
  undefined *puVar3;
  undefined8 *puVar4;
  int iVar5;
  short sVar6;
  int *piVar7;
  undefined8 uVar8;
  char in_FPSCR_SZ;
  
  (*(code *)PTR_FUN_8c051fdc)();
  (*(code *)PTR_FUN_8c051fe0)(0);
  puVar3 = PTR_FUN_8c051fec;
  puVar2 = PTR_FUN_8c051fe8;
  puVar1 = PTR_FUN_8c051fe4;
  if ((*(ushort *)(*DAT_8c051ff0 + 4) & 10) == 0) {
    sVar6 = *DAT_8c051ff8 + -1;
    piVar7 = (int *)(sVar6 * 4 + DAT_8c051ffc);
    for (; -1 < sVar6; sVar6 = sVar6 + -1) {
      iVar5 = *(int *)(*piVar7 + 0x18);
      puVar4 = (undefined8 *)(iVar5 + 8);
      if (in_FPSCR_SZ == '\0') {
        param_1 = CONCAT44(*(undefined4 *)(iVar5 + 0x10),(int)param_1);
        uVar8 = CONCAT44(*(undefined4 *)puVar4,*(undefined4 *)(iVar5 + 0xc));
      }
      else {
        param_1 = *(undefined8 *)(iVar5 + 0x10);
        uVar8 = *puVar4;
      }
      (*(code *)puVar3)(uVar8,param_1,0,puVar4);
      (*(code *)puVar1)(0,*(undefined4 *)(iVar5 + 0x14),*(undefined4 *)(iVar5 + 0x18),
                        *(undefined4 *)(iVar5 + 0x1c));
      if (*(int *)(iVar5 + 4) != 0x195) {
        (*(code *)PTR_FUN_8c052000)();
      }
      (*(code *)puVar2)();
      piVar7 = piVar7 + -1;
    }
  }
  else {
    (*(code *)PTR_FUN_8c051ff4)(*(undefined4 *)(*DAT_8c051ff0 + 0x18));
    sVar6 = *DAT_8c051ff8 + -1;
    piVar7 = (int *)(sVar6 * 4 + DAT_8c051ffc);
    for (; -1 < sVar6; sVar6 = sVar6 + -1) {
      iVar5 = *(int *)(*piVar7 + 0x18);
      puVar4 = (undefined8 *)(iVar5 + 8);
      if (in_FPSCR_SZ == '\0') {
        param_1 = CONCAT44(*(undefined4 *)(iVar5 + 0x10),(int)param_1);
        uVar8 = CONCAT44(*(undefined4 *)puVar4,*(undefined4 *)(iVar5 + 0xc));
      }
      else {
        param_1 = *(undefined8 *)(iVar5 + 0x10);
        uVar8 = *puVar4;
      }
      (*(code *)puVar3)(uVar8,param_1,0,puVar4);
      (*(code *)puVar1)(0,*(undefined4 *)(iVar5 + 0x14),*(undefined4 *)(iVar5 + 0x18),
                        *(undefined4 *)(iVar5 + 0x1c));
      if (*(int *)(iVar5 + 4) != 0x195) {
        (*(code *)PTR_FUN_8c052000)();
      }
      (*(code *)puVar2)();
      piVar7 = piVar7 + -1;
    }
  }
  (*(code *)PTR_FUN_8c052004)(1);
  (*(code *)PTR_FUN_8c052008)();
  return;
}

