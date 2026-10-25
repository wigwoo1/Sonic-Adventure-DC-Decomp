/* Ghidra draft only; not buildable/verified source. */

void startup_8c010000(void)

{
  *DAT_8c010024 = *DAT_8c010024 & DAT_8c010028 | (int)DAT_8c010020;
  (*(code *)PTR_startup_8c0100a0_8c01002c)();
  return;
}

