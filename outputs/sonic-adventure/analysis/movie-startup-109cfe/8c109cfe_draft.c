/* Ghidra draft only; not buildable/verified source. */

void FUN_8c109cfe(void)

{
  int iVar1;
  undefined *puVar2;
  undefined4 uVar3;
  uint uVar4;
  char cVar5;
  undefined4 *puVar6;
  uint uVar7;
  code *pcVar8;
  
  puVar2 = PTR_FUN_8c109df0;
  iVar1 = DAT_8c109de8;
  pcVar8 = FUN_8c010b58;
  uVar4 = DAT_8c109de8 + 0x1900;
  cVar5 = '\0';
  puVar6 = (undefined4 *)PTR_DAT_8c109dec;
  for (uVar7 = DAT_8c109de8 + 0x10; uVar7 < uVar4; uVar7 = uVar7 + 0x63c) {
    (*(code *)puVar2)(uVar7,pcVar8);
    *(undefined1 *)(uVar7 + 0x603) = 0;
    uVar3 = *puVar6;
    puVar6 = puVar6 + 1;
    *(undefined4 *)(uVar7 + 0x630) = uVar3;
    *(char *)(uVar7 + 0x602) = cVar5;
    *(undefined4 *)(uVar7 + 0x634) = 0;
    *(undefined4 *)(uVar7 + 0x638) = 0;
    cVar5 = cVar5 + '\x01';
  }
  (*(code *)PTR_FUN_8c109df4)();
  *(undefined4 *)(iVar1 + 4) = 0;
  *(undefined4 *)(iVar1 + 8) = 0;
  *(undefined4 *)(iVar1 + 0xc) = 0;
  return;
}

