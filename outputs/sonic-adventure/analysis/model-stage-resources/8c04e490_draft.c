/* Ghidra draft only; not buildable/verified source. */

/* WARNING: Removing unreachable block (ram,0x8c04e4ee) */
/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_8c04e490(void)

{
  short sVar1;
  short *psVar2;
  ushort *puVar3;
  short *psVar4;
  undefined *puVar5;
  int iVar6;
  undefined4 uVar7;
  undefined8 uVar8;
  char in_FPSCR_SZ;
  
  puVar3 = DAT_8c04e5cc;
  psVar2 = DAT_8c04e5c4;
  *DAT_8c04e5c8 = *DAT_8c04e5c4;
  *DAT_8c04e5d0 = *puVar3;
  (*(code *)PTR_FUN_8c04e5d4)(0,&DAT_8c19de24);
  if (in_FPSCR_SZ == '\0') {
    uVar8 = CONCAT44(DAT_8c04e5d8._4_4_,(undefined4)DAT_8c04e5d8);
  }
  else {
    uVar8 = CONCAT44(_DAT_8c04e5e0,DAT_8c04e5d8._4_4_);
  }
  (*(code *)PTR_FUN_8c04e5e4)(uVar8,_DAT_8c04e5e0);
  (*(code *)PTR_thunk_FUN_8c652dd0_8c04e5e8)(_DAT_8c04e5e0);
  (*(code *)PTR_FUN_8c04e5ec)();
  (*(code *)PTR_FUN_8c04e5f0)();
  (*(code *)PTR_FUN_8c04e5f4)();
  psVar4 = DAT_8c04e5fc;
  iVar6 = (int)((ushort)(*psVar2 << 8 | *puVar3) & DAT_8c04e5f8) >> 8;
  if ((iVar6 == 0x24) || (iVar6 == 0x25)) {
    (*(code *)PTR_FUN_8c04e600)();
    uVar7 = 2;
  }
  else {
    (*(code *)PTR_FUN_8c04e600)();
    (*(code *)PTR_FUN_8c04e604)();
    (*(code *)PTR_thunk_FUN_8c6087f4_8c04e608)();
    (*(code *)PTR_FUN_8c04e60c)(*psVar4);
    uVar7 = 0;
  }
  (*(code *)PTR_FUN_8c04e610)(uVar7);
  (*(code *)PTR_FUN_8c04e614)();
  (*(code *)PTR_FUN_8c04e618)((int)*psVar2 << 8 | (int)(short)*puVar3);
  (*(code *)PTR_FUN_8c04e61c)((int)*psVar2,(int)(short)*puVar3);
  (*(code *)PTR_FUN_8c04e624)(DAT_8c04e620,0x800);
  FUN_8c04e3aa();
  puVar5 = PTR_FUN_8c04e62c;
  *(undefined2 *)PTR_DAT_8c04e628 = 0;
  (*(code *)puVar5)();
  (*(code *)PTR_FUN_8c04e630)();
  sVar1 = *psVar4;
  if (sVar1 == 3) {
    (*(code *)PTR_FUN_8c04e634)();
  }
  else if (sVar1 == 7) {
    (*(code *)PTR_FUN_8c04e638)();
  }
  (*(code *)PTR_FUN_8c04e63c)();
  (*(code *)PTR_FUN_8c04e640)();
  (*(code *)PTR_FUN_8c04e648)(2,8,PTR_LAB_8c04e644);
  return;
}

