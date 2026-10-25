/* Ghidra draft only; not buildable/verified source. */

void FUN_8c5fc58a(uint param_1,uint param_2)

{
  uint uVar1;
  uint uVar2;
  
  if (param_2 == 0) {
                    /* WARNING: Could not recover jumptable at 0x8c5fc59c. Too many branches */
                    /* WARNING: Treating indirect jump as call */
    (*DAT_8c5fc5c0)();
    return;
  }
  uVar2 = DAT_8c5fc5c4 & param_1;
  if (DAT_8c5fc5c8 <= param_2) {
                    /* WARNING: Could not recover jumptable at 0x8c5fc5ac. Too many branches */
                    /* WARNING: Treating indirect jump as call */
    (*DAT_8c5fc5cc)();
    return;
  }
  uVar1 = (param_1 & 0x1f) + param_2 + 0x1f >> 5;
  do {
    CacheBlockWriteBack(uVar2);
    uVar1 = uVar1 - 1;
    uVar2 = uVar2 + 0x20;
  } while (uVar1 != 0);
                    /* WARNING: Could not recover jumptable at 0x8c5fc5e2. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  (*DAT_8c5fc600)(uVar2,&DAT_8c19de24);
  return;
}

