/* Ghidra draft only; not buildable/verified source. */

void FUN_8c6050b0(void)

{
  undefined *puVar1;
  int *piVar2;
  undefined *puVar3;
  undefined *puVar4;
  undefined4 uVar5;
  undefined4 *puVar6;
  uint uVar7;
  undefined4 *puVar8;
  
  puVar1 = PTR_FUN_8c6051e4;
  puVar6 = DAT_8c6051e0;
  puVar8 = &DAT_8c19a8e8;
  uVar7 = 0;
  uVar5 = (*(code *)PTR_FUN_8c6051e4)(&DAT_8c19a8e8);
  piVar2 = DAT_8c6051e8;
  *puVar6 = uVar5;
  puVar4 = PTR_FUN_8c6051f0;
  puVar3 = PTR_FUN_8c6051ec;
  if (*piVar2 != 0) {
    while ((*DAT_8c6051f4 != 0 && (uVar7 < DAT_8c6051f8))) {
      uVar5 = (*(code *)puVar1)(puVar8);
      uVar5 = (*(code *)puVar4)(*puVar6,uVar5);
      uVar7 = (*(code *)puVar3)(uVar5);
    }
    if (uVar7 < DAT_8c6051f8) {
      FUN_8c60509a();
    }
  }
  puVar3 = PTR_FUN_8c6051fc;
  *DAT_8c6051f4 = 1;
  (*(code *)puVar3)();
  piVar2 = DAT_8c60520c;
  puVar8 = DAT_8c605208;
  puVar6 = DAT_8c605204;
  *DAT_8c605200 = 0;
  *puVar6 = 0;
  *puVar8 = 0;
  *piVar2 = *piVar2 + 1;
  (*(code *)PTR_FUN_8c605210)();
  (*(code *)PTR_FUN_8c605214)();
  uVar5 = (*(code *)puVar1)();
  puVar8 = DAT_8c605220;
  puVar6 = DAT_8c60521c;
  *DAT_8c605218 = uVar5;
  puVar6 = (undefined4 *)*puVar6;
  *puVar8 = *puVar6;
  *DAT_8c605224 = puVar6[1];
  *DAT_8c605228 = puVar6[2];
  *DAT_8c60522c = puVar6[3];
  *DAT_8c605230 = puVar6[4];
  return;
}

