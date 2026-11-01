/* Ghidra draft only; not buildable/verified source. */

void callback_8c0b3afc(int param_1)

{
  int iVar1;
  int iVar2;
  
  iVar2 = *(int *)(param_1 + 0x20);
  iVar1 = *(int *)(iVar2 + 0x10);
  while (iVar1 != 0) {
    FUN_8c0b33ce(iVar2,*(undefined4 *)(iVar2 + 0x10));
    iVar1 = *(int *)(iVar2 + 0x10);
  }
  return;
}

