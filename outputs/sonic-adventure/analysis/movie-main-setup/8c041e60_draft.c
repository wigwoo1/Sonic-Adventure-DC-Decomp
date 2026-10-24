/* Ghidra draft only; not buildable/verified source. */

void FUN_8c041e60(void)

{
  int iVar1;
  undefined4 *puVar2;
  char in_FPSCR_SZ;
  
  iVar1 = DAT_8c041ed0;
  *DAT_8c041ec4 = DAT_8c041ec0;
  puVar2 = DAT_8c041ed4;
  if (in_FPSCR_SZ == '\0') {
    *(undefined4 *)DAT_8c041ec8._4_4_ = (undefined4)DAT_8c041ec8;
  }
  else {
    *DAT_8c041ec8._4_4_ = CONCAT44(DAT_8c041ec8._4_4_,(undefined4)DAT_8c041ec8);
  }
  *puVar2 = *(undefined4 *)(iVar1 + 0x90);
  *DAT_8c041ed8 = *(undefined4 *)(iVar1 + 0x94);
  *DAT_8c041edc = *(undefined4 *)(iVar1 + 0x98);
  *DAT_8c041ee0 = *(undefined4 *)(iVar1 + 0x9c);
  return;
}

