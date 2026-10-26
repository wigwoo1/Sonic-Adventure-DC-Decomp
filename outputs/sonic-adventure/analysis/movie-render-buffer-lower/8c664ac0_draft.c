/* Ghidra draft only; not buildable/verified source. */

ushort * FUN_8c664ac0(void)

{
  ushort *puVar1;
  ushort *puVar2;
  
  puVar1 = DAT_8c664b80;
  puVar2 = DAT_8c664b80;
  while( true ) {
    if ((ushort *)(DAT_8c664b84 + (int)DAT_8c664b80) <= puVar1) {
      return (ushort *)0x0;
    }
    if ((*puVar1 & 1) == 0) break;
    puVar1 = puVar1 + 0xc;
    puVar2 = puVar2 + 0xc;
  }
  *puVar2 = 1;
  return puVar2;
}

