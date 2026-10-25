/* Ghidra draft only; not buildable/verified source. */

void FUN_8c09536a(void)

{
  undefined4 *puVar1;
  undefined1 *puVar2;
  undefined4 *puVar3;
  undefined1 *puVar4;
  undefined4 *puVar5;
  undefined1 *puVar6;
  int iVar7;
  
  FUN_8c094df8(0);
  puVar1 = DAT_8c095464 + 0x8c;
  for (puVar5 = DAT_8c095464; puVar5 < puVar1; puVar5 = puVar5 + 4) {
    iVar7 = 0;
    puVar3 = puVar5;
    do {
      *puVar3 = 0;
      iVar7 = iVar7 + 2;
      puVar3[1] = 0;
      puVar3 = puVar3 + 2;
    } while (iVar7 < 4);
  }
  puVar2 = DAT_8c09546c + 8;
  puVar6 = DAT_8c095468;
  for (puVar4 = DAT_8c09546c; puVar4 < puVar2; puVar4 = puVar4 + 2) {
    *puVar4 = 0xf;
    *puVar6 = 0;
    puVar4[1] = 0xf;
    puVar6[1] = 0;
    puVar6 = puVar6 + 2;
  }
  return;
}

