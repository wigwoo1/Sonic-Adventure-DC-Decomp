/* Ghidra draft only; not buildable/verified source. */

void FUN_8c09dab2(void)

{
  undefined4 *puVar1;
  undefined4 *puVar2;
  
  puVar1 = DAT_8c09db64;
  puVar2 = DAT_8c09db64 + 0xa0;
  *DAT_8c09db60 = 0;
  for (; puVar1 < puVar2; puVar1 = puVar1 + 0x14) {
    *puVar1 = 0;
    puVar1[10] = 0;
  }
  return;
}

