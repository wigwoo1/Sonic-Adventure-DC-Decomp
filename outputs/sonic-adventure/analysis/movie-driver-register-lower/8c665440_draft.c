/* Ghidra draft only; not buildable/verified source. */

void FUN_8c665440(ushort *param_1)

{
  undefined4 uVar1;
  ushort *puVar2;
  ushort *puVar3;
  ushort *puVar4;
  
  uVar1 = DAT_8c6654c4;
  puVar2 = (ushort *)((int)DAT_8c6654c0 + DAT_8c6654c8);
  puVar3 = DAT_8c6654c0;
  puVar4 = DAT_8c6654c0;
  do {
    if (puVar3 == param_1) {
      *puVar4 = *puVar4 & (ushort)uVar1;
    }
    puVar3 = puVar3 + 0x26;
    puVar4 = puVar4 + 0x26;
  } while (puVar3 < puVar2);
  return;
}

