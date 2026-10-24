/* Ghidra draft only; not buildable/verified source. */

undefined4 FUN_8c10eee0(void)

{
  undefined *puVar1;
  undefined *puVar2;
  int local_10 [2];
  
  puVar2 = PTR_FUN_8c10ef18;
  puVar1 = PTR_DAT_8c10ef14;
  do {
    (*(code *)puVar2)(local_10);
  } while (local_10[0] != 1);
  *(int *)((int)DAT_8c10ef12 + *(int *)puVar1) = *(int *)((int)DAT_8c10ef12 + *(int *)puVar1) + 1;
  return 0;
}

