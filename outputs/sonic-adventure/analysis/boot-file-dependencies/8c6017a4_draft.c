/* Ghidra draft only; not buildable/verified source. */

undefined4 FUN_8c6017a4(void)

{
  undefined4 uVar1;
  
  if (*(int *)PTR_DAT_8c601818 != 0) {
                    /* WARNING: Could not recover jumptable at 0x8c6017b2. Too many branches */
                    /* WARNING: Treating indirect jump as call */
    uVar1 = (**(code **)(*(int *)PTR_DAT_8c601818 + 0x58))();
    return uVar1;
  }
  return 0;
}

