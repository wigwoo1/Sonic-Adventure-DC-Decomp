/* Ghidra draft only; not buildable/verified source. */

void FUN_8c5fc336(uint param_1,uint param_2)

{
  uint uVar1;
  uint uVar2;
  
  if (param_2 == 0) {
                    /* WARNING: Could not recover jumptable at 0x8c5fc348. Too many branches */
                    /* WARNING: Treating indirect jump as call */
    (*(code *)PTR_LAB_8c5fc36c)();
    return;
  }
  uVar2 = DAT_8c5fc370 & param_1;
  if (DAT_8c5fc374 <= param_2) {
                    /* WARNING: Could not recover jumptable at 0x8c5fc358. Too many branches */
                    /* WARNING: Treating indirect jump as call */
    (*(code *)PTR_LAB_8c5fc378)();
    return;
  }
  uVar1 = (param_1 & 0x1f) + param_2 + 0x1f >> 5;
  do {
    CacheBlockPurge(uVar2);
    uVar1 = uVar1 - 1;
    uVar2 = uVar2 + 0x20;
  } while (uVar1 != 0);
                    /* WARNING: Could not recover jumptable at 0x8c5fc38e. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  (*(code *)PTR_LAB_8c5fc3ac)(uVar2);
  return;
}

