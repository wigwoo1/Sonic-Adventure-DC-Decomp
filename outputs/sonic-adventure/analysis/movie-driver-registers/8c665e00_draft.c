/* Ghidra draft only; not buildable/verified source. */

int FUN_8c665e00(int param_1)

{
  return (*(int *)(PTR_DAT_8c665eec + *DAT_8c665ef0 * 4) +
          *(int *)(PTR_DAT_8c665eec + *DAT_8c665ef4 * 4) +
          *(int *)(PTR_DAT_8c665eec + DAT_8c665ef0[2] * 4) +
          *(int *)(PTR_DAT_8c665eec + DAT_8c665ef0[3] * 4) +
         *(int *)(PTR_DAT_8c665eec + DAT_8c665ef0[4] * 4)) * param_1 * 4;
}

