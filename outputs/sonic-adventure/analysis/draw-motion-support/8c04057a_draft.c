/* Ghidra draft only; not buildable/verified source. */

void callback_8c04057a(undefined4 param_1,int param_2)

{
  undefined8 uVar1;
  undefined8 uVar2;
  char in_FPSCR_SZ;
  
  if (in_FPSCR_SZ == '\0') {
    uVar1 = CONCAT44(*(undefined4 *)(param_2 + 8),*(undefined4 *)(param_2 + 0xc));
    uVar2 = CONCAT44(*(undefined4 *)(param_2 + 0x10),param_1);
  }
  else {
    uVar1 = *(undefined8 *)(param_2 + 8);
    uVar2 = *(undefined8 *)(param_2 + 0x18);
  }
  (*(code *)PTR_FUN_8c040674)(uVar1,uVar2,0);
  return;
}

