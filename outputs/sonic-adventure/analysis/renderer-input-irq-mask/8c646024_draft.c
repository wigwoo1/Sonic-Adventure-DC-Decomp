/* Ghidra draft only; not buildable/verified source. */

void callback_8c646024(void)

{
  int iVar1;
  int *piVar2;
  undefined4 *puVar3;
  
  piVar2 = DAT_8c64614c;
  iVar1 = DAT_8c646148;
  *(int *)(DAT_8c646148 + 8) = *(int *)(DAT_8c646148 + 8) + 1;
  puVar3 = DAT_8c646150;
  if (*piVar2 != 0) {
    *(int *)(iVar1 + 0x14) = *(int *)(iVar1 + 0x14) + 1;
    if ((code *)*puVar3 != (code *)0x0) {
      (*(code *)*puVar3)(0,0xffffffff,*(undefined4 *)(iVar1 + 8));
    }
    return;
  }
  FUN_8c6463ac();
  return;
}

