/* Ghidra draft only; not buildable/verified source. */

void FUN_8c05e69e(void)

{
  int *piVar1;
  int *piVar2;
  int *piVar3;
  
  piVar3 = DAT_8c05e760 + 10;
  piVar2 = DAT_8c05e760;
  for (piVar1 = DAT_8c05e760; piVar1 < piVar3; piVar1 = piVar1 + 1) {
    if (*piVar1 != 0) {
      **(undefined1 **)(*piVar2 + 0x20) = 0;
    }
    piVar2 = piVar2 + 1;
  }
  return;
}

