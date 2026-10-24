/* Ghidra draft only; not buildable/verified source. */

void FUN_8c6406ac(int param_1,undefined4 param_2,undefined4 param_3)

{
  undefined8 *puVar1;
  undefined4 uVar2;
  undefined4 *puVar3;
  int iVar4;
  char in_FPSCR_SZ;
  
  puVar1 = DAT_8c640778._4_4_;
  *DAT_8c640774 = DAT_8c640770;
  if (in_FPSCR_SZ == '\0') {
    *(undefined4 *)puVar1 = (undefined4)DAT_8c640778;
  }
  else {
    *puVar1 = CONCAT44(DAT_8c640778._4_4_,(undefined4)DAT_8c640778);
  }
  if (param_1 == 0) {
    (*(code *)PTR_FUN_8c640784)(DAT_8c640780,DAT_8c64076c + -0x80);
    (*(code *)PTR_FUN_8c64078c)(DAT_8c640780,PTR_DAT_8c640788,0);
  }
  else {
    (*(code *)PTR_FUN_8c640784)(DAT_8c640780,param_2,param_3);
    (*(code *)PTR_FUN_8c64078c)(DAT_8c640780,param_1,0);
  }
  puVar3 = DAT_8c640790;
  uVar2 = DAT_8c640780;
  *DAT_8c640790 = DAT_8c640794;
  puVar3[1] = 0;
  puVar3[2] = 2;
  puVar3[3] = 0;
  puVar3[8] = 2;
  puVar3[0x1b] = uVar2;
  puVar3[0x10] = 0;
  puVar3[0x11] = 1;
  (*(code *)PTR_FUN_8c640798)(puVar3);
  iVar4 = (int)DAT_8c64076e;
  *DAT_8c64079c = *(undefined4 *)(iVar4 + (int)puVar3);
  *DAT_8c6407a0 = *(undefined4 *)(iVar4 + 4 + (int)puVar3);
  *DAT_8c6407a4 = *(undefined4 *)(iVar4 + 8 + (int)puVar3);
  *DAT_8c6407a8 = *(undefined4 *)(iVar4 + 0xc + (int)puVar3);
  return;
}

