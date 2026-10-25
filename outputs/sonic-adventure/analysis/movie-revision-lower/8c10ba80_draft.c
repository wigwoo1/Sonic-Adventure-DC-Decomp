/* Ghidra draft only; not buildable/verified source. */

void FUN_8c10ba80(void)

{
  undefined4 uVar1;
  
  if ((*DAT_8c10bafc & 1) == 0) {
    uVar1 = FUN_8c10bd96(*DAT_8c10baf8);
    *DAT_8c10baf8 = uVar1;
  }
  if ((*DAT_8c10bafc & 2) != 0) {
    uVar1 = FUN_8c10be0c();
    *DAT_8c10baf8 = uVar1;
  }
  return;
}

