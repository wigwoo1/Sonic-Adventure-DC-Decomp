/* Ghidra draft only; not buildable/verified source. */

void FUN_8c05e788(int param_1)

{
  short sVar1;
  undefined1 *puVar2;
  
  FUN_8c05e69e();
  puVar2 = *(undefined1 **)(*(int *)(*(int *)(*(int *)(param_1 + 0x38) + 0x18) + 0x10) + 0x20);
  sVar1 = *(short *)(puVar2 + 6);
  *puVar2 = 1;
  if ((sVar1 == 9) || (*(int *)((sVar1 + 1) * 4 + DAT_8c05e840) == 0)) {
    sVar1 = 0;
  }
  else {
    sVar1 = sVar1 + 1;
  }
  **(undefined1 **)(*(int *)(sVar1 * 4 + DAT_8c05e840) + 0x20) = 2;
  return;
}

