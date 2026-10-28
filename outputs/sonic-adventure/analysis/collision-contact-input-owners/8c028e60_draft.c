/* Ghidra draft only; not buildable/verified source. */

undefined4 FUN_8c028e60(uint param_1)

{
  undefined *puVar1;
  undefined4 *puVar2;
  int iVar3;
  int iVar4;
  char in_FPSCR_SZ;
  
  puVar1 = PTR_DAT_8c028f10;
  *DAT_8c028f08 = 0;
  iVar3 = DAT_8c028f18;
  puVar2 = DAT_8c028f14;
  iVar4 = (int)DAT_8c028f00;
  *DAT_8c028f0c = 0;
  *(undefined4 *)puVar1 = 0;
  *puVar2 = 0;
  do {
    *(undefined4 *)(iVar3 + 8) = 0;
    *(undefined4 *)(iVar3 + 0x48) = 0;
    iVar4 = iVar4 + -3;
    *(undefined4 *)(iVar3 + 0x88) = 0;
    iVar3 = iVar3 + 0xc0;
  } while (0 < iVar4);
  iVar4 = (int)DAT_8c028f02;
  iVar3 = DAT_8c028f1c;
  do {
    if (in_FPSCR_SZ == '\0') {
      *(undefined4 *)(iVar3 + 0x14) = 0;
    }
    else {
      *(ulonglong *)(iVar3 + 0x14) = (ulonglong)param_1;
    }
    iVar4 = iVar4 + -2;
    if (in_FPSCR_SZ == '\0') {
      *(undefined4 *)(iVar3 + 0x54) = 0;
    }
    else {
      *(ulonglong *)(iVar3 + 0x54) = (ulonglong)param_1;
    }
    iVar3 = iVar3 + 0x80;
  } while (0 < iVar4);
  (*(code *)PTR_FUN_8c028f20)(DAT_8c028f1c,(int)DAT_8c028f02);
  (*(code *)PTR_FUN_8c028f24)(DAT_8c028f18,(int)DAT_8c028f00);
  return 1;
}

