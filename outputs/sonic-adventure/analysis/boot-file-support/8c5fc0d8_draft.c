/* Ghidra draft only; not buildable/verified source. */

void FUN_8c5fc0d8(uint param_1,uint param_2)

{
  uint uVar1;
  uint uVar2;
  
  if (param_2 == 0) {
                    /* WARNING: Could not recover jumptable at 0x8c5fc0ea. Too many branches */
                    /* WARNING: Treating indirect jump as call */
    (*(code *)PTR_LAB_8c5fc118)();
    return;
  }
  uVar2 = DAT_8c5fc11c & param_1;
  if (DAT_8c5fc120 <= param_2) {
                    /* WARNING: Could not recover jumptable at 0x8c5fc0fa. Too many branches */
                    /* WARNING: Treating indirect jump as call */
    (*(code *)PTR_LAB_8c5fc124)();
    return;
  }
  uVar1 = (param_1 & 0x1f) + param_2 + 0x1f >> 5;
  do {
    CacheBlockInvalidate(uVar2);
    uVar1 = uVar1 - 1;
    uVar2 = uVar2 + 0x20;
  } while (uVar1 != 0);
                    /* WARNING: Could not recover jumptable at 0x8c5fc13a. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  (*(code *)PTR_LAB_8c5fc158)(uVar2);
  return;
}

