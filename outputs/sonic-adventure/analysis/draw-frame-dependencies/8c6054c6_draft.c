/* Ghidra draft only; not buildable/verified source. */

void FUN_8c6054c6(int param_1)

{
  int *piVar1;
  
  piVar1 = DAT_8c6054ec;
  *(undefined4 *)*DAT_8c6054ec = DAT_8c6054fc;
  if (param_1 == 1) {
    *(undefined4 *)(*piVar1 + 0x58) = 1;
  }
  else if (param_1 == 0) {
    *(undefined4 *)(*piVar1 + 0x58) = 0;
  }
  (*(code *)PTR_FUN_8c6055c4)(*piVar1);
  (*(code *)PTR_FUN_8c6055c8)(*piVar1);
  return;
}

