/* Ghidra draft only; not buildable/verified source. */

/* WARNING: Removing unreachable block (ram,0x8c60265e) */
/* WARNING: Removing unreachable block (ram,0x8c602664) */

int FUN_8c602652(uint param_1)

{
  return ((param_1 & 0x7f) * 100 >> 7) + (param_1 >> 7) * 100;
}

