/* Ghidra draft only; not buildable/verified source. */

void FUN_8c664aec(ushort *param_1)

{
  undefined4 uVar1;
  ushort *puVar2;
  ushort *puVar3;
  ushort *puVar4;
  
  uVar1 = DAT_8c664b88;
  puVar2 = (ushort *)((int)DAT_8c664b80 + DAT_8c664b84);
  puVar3 = DAT_8c664b80;
  puVar4 = DAT_8c664b80;
  do {
    if (puVar3 == param_1) {
      *puVar4 = *puVar4 & (ushort)uVar1;
    }
    puVar3 = puVar3 + 0xc;
    puVar4 = puVar4 + 0xc;
  } while (puVar3 < puVar2);
  return;
}

