/* Ghidra draft only; not buildable/verified source. */

ushort * FUN_8c6653e8(void)

{
  ushort *puVar1;
  ushort *puVar2;
  
  puVar1 = DAT_8c66542c;
  puVar2 = DAT_8c66542c;
  while( true ) {
    if ((ushort *)(DAT_8c665430 + (int)DAT_8c66542c) <= puVar1) {
      return (ushort *)0x0;
    }
    if ((*puVar1 & 1) == 0) break;
    puVar1 = puVar1 + 0x26;
    puVar2 = puVar2 + 0x26;
  }
  *puVar2 = 1;
  return puVar2;
}

