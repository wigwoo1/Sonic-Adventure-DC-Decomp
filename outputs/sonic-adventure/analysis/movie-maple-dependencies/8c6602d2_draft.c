/* Ghidra draft only; not buildable/verified source. */

undefined4 FUN_8c6602d2(int param_1)

{
  undefined1 *puVar1;
  
  FUN_8c6607a8(0,0x1e);
  *(undefined1 *)(param_1 + 0x14) = 0x28;
  *(undefined1 *)(param_1 + 0x15) = 0x6e;
  *(undefined1 *)(param_1 + 0x16) = 0x6f;
  *(undefined1 *)(param_1 + 0x17) = 0x6e;
  *(undefined1 *)(param_1 + 0x18) = 0x65;
  puVar1 = (undefined1 *)(param_1 + 0x1a);
  *(undefined1 *)(param_1 + 0x19) = 0x29;
  do {
    *puVar1 = 0x20;
    puVar1 = puVar1 + 1;
  } while (puVar1 < (undefined1 *)(param_1 + 0x33));
  *(undefined1 *)(param_1 + 0x33) = 0;
  return 0;
}

