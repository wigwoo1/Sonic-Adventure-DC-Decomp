/* Ghidra draft only; not buildable/verified source. */

void FUN_8c63fb18(void)

{
  int *piVar1;
  
  piVar1 = DAT_8c63fb3c;
  *(int *)*DAT_8c63fb3c = (int)DAT_8c63fb3a;
  *(undefined4 *)(*piVar1 + 0x3c) = 0;
  (*(code *)PTR_FUN_8c63fb40)(*piVar1);
  (*(code *)PTR_FUN_8c63fb44)(*piVar1);
  return;
}

