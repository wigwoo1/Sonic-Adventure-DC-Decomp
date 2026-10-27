/* Ghidra draft only; not buildable/verified source. */

void FUN_8c6461ae(void)

{
  undefined1 *puVar1;
  undefined1 *puVar2;
  int iVar3;
  
  iVar3 = 0;
  puVar1 = (undefined1 *)(*DAT_8c6462b4 + 0x28);
  puVar2 = DAT_8c6462b0;
  do {
    iVar3 = iVar3 + 1;
    *puVar2 = *puVar1;
    puVar2[1] = puVar1[1];
    puVar2[2] = puVar1[2];
    puVar2[3] = puVar1[3];
    *(undefined4 *)(puVar2 + 8) = *(undefined4 *)(puVar1 + 0x10);
    puVar2[4] = puVar1[8];
    puVar2[5] = puVar1[9];
    puVar2[6] = puVar1[10];
    puVar2[7] = puVar1[0xb];
    *(undefined4 *)(puVar2 + 0xc) = *(undefined4 *)(puVar1 + 0x14);
    puVar2 = puVar2 + 0x18;
    puVar1 = puVar1 + 0x2c;
  } while (iVar3 < 0x18);
  return;
}

