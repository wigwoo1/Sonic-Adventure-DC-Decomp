/* Ghidra draft only; not buildable/verified source. */

undefined4 FUN_8c6477cc(uint param_1)

{
  undefined4 *puVar1;
  undefined *puVar2;
  int iVar3;
  undefined4 uVar4;
  int iVar5;
  
  puVar1 = DAT_8c647848;
  *DAT_8c647848 = 0;
  puVar1[1] = 0;
  puVar1[2] = 0;
  puVar1[3] = 0;
  puVar1[4] = 0;
  puVar1[5] = 0;
  puVar1[6] = 0;
  puVar1[7] = 0;
  puVar2 = PTR_FUN_8c64784c;
  (*(code *)PTR_FUN_8c64784c)(DAT_8c647850,0,(int)DAT_8c647844);
  iVar5 = (int)DAT_8c647846;
  (*(code *)puVar2)(DAT_8c647854,0x30,iVar5);
  (*(code *)puVar2)(DAT_8c647858,0x31,iVar5);
  (*(code *)puVar2)(DAT_8c64785c,0x32,iVar5);
  (*(code *)puVar2)(DAT_8c647860,0x33,iVar5);
  (*(code *)puVar2)(DAT_8c647864,0x34,iVar5);
  (*(code *)puVar2)(DAT_8c647868,0x35,iVar5);
  (*(code *)puVar2)(DAT_8c64786c,0x36,iVar5);
  (*(code *)puVar2)(DAT_8c647870,0x37,iVar5);
  iVar3 = (*(code *)PTR_FUN_8c647874)();
  puVar2 = PTR_FUN_8c647930;
  if (iVar3 == 0) {
    uVar4 = 0xffffffff;
  }
  else {
    if ((param_1 & 1) != 0) {
      (*(code *)PTR_FUN_8c647930)(PTR_LAB_8c647938,DAT_8c647934,iVar5);
    }
    if ((param_1 & 2) != 0) {
      (*(code *)puVar2)(PTR_LAB_8c647940,DAT_8c64793c,iVar5);
    }
    if ((param_1 & 4) != 0) {
      (*(code *)puVar2)(PTR_LAB_8c647948,DAT_8c647944,iVar5);
    }
    if ((param_1 & 8) != 0) {
      (*(code *)puVar2)(PTR_LAB_8c647950,DAT_8c64794c,iVar5);
    }
    if ((param_1 & 0x10) != 0) {
      (*(code *)puVar2)(PTR_LAB_8c647958,DAT_8c647954,iVar5);
    }
    if ((param_1 & 0x20) != 0) {
      (*(code *)puVar2)(PTR_LAB_8c647960,DAT_8c64795c,iVar5);
    }
    if ((param_1 & 0x40) != 0) {
      (*(code *)puVar2)(PTR_LAB_8c647968,DAT_8c647964,iVar5);
    }
    if ((param_1 & (int)DAT_8c64792a) != 0) {
      (*(code *)puVar2)(PTR_LAB_8c647970,DAT_8c64796c,iVar5);
    }
    uVar4 = 0;
  }
  return uVar4;
}

