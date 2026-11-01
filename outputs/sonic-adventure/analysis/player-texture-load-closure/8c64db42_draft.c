/* Ghidra draft only; not buildable/verified source. */

void FUN_8c64db42(int param_1)

{
  undefined4 *puVar1;
  undefined4 uVar2;
  
  puVar1 = DAT_8c64dbb4;
  *DAT_8c64dbb0 = param_1;
  if (param_1 == 0) {
    uVar2 = 0;
  }
  else if (param_1 == 1) {
    uVar2 = 1;
  }
  else if (param_1 == 2) {
    uVar2 = 2;
  }
  else {
    if (param_1 == 6) {
      uVar2 = 3;
      *puVar1 = 0x20;
      goto LAB_8c64db7a;
    }
    uVar2 = 0;
  }
  *puVar1 = 0x10;
LAB_8c64db7a:
  (*(code *)PTR_FUN_8c64dbb8)(uVar2);
  return;
}

