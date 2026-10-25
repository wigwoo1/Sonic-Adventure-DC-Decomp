/* Ghidra draft only; not buildable/verified source. */

void FUN_8c6025e8(void)

{
  undefined1 *puVar1;
  byte *pbVar2;
  
  puVar1 = DAT_8c60270c;
  *DAT_8c60270c = 0;
  pbVar2 = DAT_8c602710;
  *DAT_8c602710 = *DAT_8c602710 & 0xfe;
  *DAT_8c602714 = 2;
  *DAT_8c602718 = 0xffffffff;
  *(undefined4 *)(puVar1 + 0xc) = 0xffffffff;
  *pbVar2 = *pbVar2 | 1;
  return;
}

