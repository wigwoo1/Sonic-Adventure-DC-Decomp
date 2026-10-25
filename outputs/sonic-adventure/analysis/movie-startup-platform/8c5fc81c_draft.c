/* Ghidra draft only; not buildable/verified source. */

void FUN_8c5fc81c(void)

{
  byte in_IMASK;
  byte in_S;
  byte in_T;
  
                    /* WARNING: Could not recover jumptable at 0x8c5fc83e. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  (*(code *)(DAT_8c5fc844 | 0x8c5fc848))
            ((short)((((uint)in_T | (uint)in_S << 1 | (uint)in_IMASK << 4) & 0xf0) >> 4));
  return;
}

