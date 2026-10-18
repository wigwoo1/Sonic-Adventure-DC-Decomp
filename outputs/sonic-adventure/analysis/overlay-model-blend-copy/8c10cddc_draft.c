/* Ghidra draft only; not buildable/verified source. */

void FUN_8c10cddc(void)

{
  uint in_r0;
  undefined4 *in_r1;
  undefined4 *in_r2;
  undefined4 *puVar1;
  undefined4 *puVar2;
  
  if (in_r0 < 0x41) {
                    /* WARNING: Could not recover jumptable at 0x8c10cde8. Too many branches */
                    /* WARNING: Treating indirect jump as call */
    (**(code **)(PTR_PTR_8c10cdec + in_r0))();
    return;
  }
  puVar2 = (undefined4 *)((int)in_r2 + in_r0);
  while( true ) {
    puVar1 = in_r2 + 1;
    if (puVar2 < puVar1) {
      return;
    }
    *in_r1 = *in_r2;
    in_r2 = in_r2 + 2;
    if (puVar2 < in_r2) break;
    in_r1[1] = *puVar1;
    in_r1 = in_r1 + 2;
  }
  return;
}

