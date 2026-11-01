/* Ghidra draft only; not buildable/verified source. */

void FUN_8c04b440(int param_1)

{
  undefined1 *puVar1;
  
  if (*(int *)(param_1 + 0x3c) == 0) {
    puVar1 = (undefined1 *)(*(code *)PTR_FUN_8c04b5e0)(1,0x7c);
    *(undefined1 **)(param_1 + 0x3c) = puVar1;
    *puVar1 = 3;
  }
  return;
}

