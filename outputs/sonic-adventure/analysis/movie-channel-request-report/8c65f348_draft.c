/* Ghidra draft only; not buildable/verified source. */

/* WARNING: Removing unreachable block (ram,0x8c65f364) */

void FUN_8c65f348(undefined4 param_1)

{
  if (*(int *)((int)DAT_8c65f3c8 + *DAT_8c65f3d8) != 0) {
    (*(code *)PTR_FUN_8c65f3ec)(DAT_8c65f3e8,param_1,&stack0x00000004);
    if (*(code **)PTR_DAT_8c65f3f0 != (code *)0x0) {
      (**(code **)PTR_DAT_8c65f3f0)(DAT_8c65f3e8);
    }
  }
  return;
}

