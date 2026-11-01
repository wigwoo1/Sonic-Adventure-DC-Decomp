/* Ghidra draft only; not buildable/verified source. */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void callback_8c6416f0(int param_1)

{
  undefined4 uVar1;
  undefined4 uStack_4;
  
  if (param_1 == 1) {
    uStack_4 = _DAT_8c641740;
    goto LAB_8c641720;
  }
  uVar1 = _DAT_8c641744;
  if (param_1 != 2) {
    if (param_1 == 3) {
      uStack_4 = _DAT_8c641748;
      goto LAB_8c641720;
    }
    uVar1 = _DAT_8c64174c;
    if (param_1 != 4) goto LAB_8c641720;
  }
  uStack_4 = uVar1;
LAB_8c641720:
  (*_DAT_8c641750)(uStack_4);
  do {
                    /* WARNING: Do nothing block with infinite loop */
  } while( true );
}

