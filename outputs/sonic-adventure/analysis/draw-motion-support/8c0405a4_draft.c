/* Ghidra draft only; not buildable/verified source. */

void callback_8c0405a4(undefined4 param_1,int param_2)

{
  undefined8 uVar1;
  undefined8 uVar2;
  char in_FPSCR_SZ;
  
  if (in_FPSCR_SZ == '\0') {
    uVar1 = CONCAT44(*(undefined4 *)(param_2 + 0x20),*(undefined4 *)(param_2 + 0x24));
    uVar2 = CONCAT44(*(undefined4 *)(param_2 + 0x28),param_1);
  }
  else {
    uVar1 = *(undefined8 *)(param_2 + 0x20);
    uVar2 = *(undefined8 *)(param_2 + 0x30);
  }
  (*(code *)PTR_FUN_8c040680)(uVar1,uVar2,0);
  return;
}

