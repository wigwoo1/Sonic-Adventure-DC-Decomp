/* Ghidra draft only; not buildable/verified source. */

int FUN_8c64658a(void)

{
  byte bVar1;
  int iVar2;
  int iVar3;
  byte bVar4;
  int iVar5;
  uint *puVar6;
  uint uVar7;
  undefined1 *puVar8;
  uint uVar9;
  
  iVar3 = DAT_8c646630;
  iVar2 = DAT_8c64662c;
  bVar1 = *(byte *)((int)DAT_8c646616 + *DAT_8c646628);
  uVar9 = 0;
  do {
    if (3 < (int)uVar9) {
      return 0;
    }
    if ((int)uVar9 < 0) {
      uVar7 = 1 >> (~uVar9 & 0x1f) + 1;
    }
    else {
      uVar7 = 1 << (uVar9 & 0x1f);
    }
    if ((uVar7 & bVar1) != 0) {
      puVar8 = PTR_DAT_8c646634 + uVar9;
      iVar5 = FUN_8c646856(*puVar8);
      puVar6 = (uint *)FUN_8c646868(*puVar8);
      *(undefined1 *)(uVar9 + iVar3) = *(undefined1 *)(uVar9 + iVar2);
      if ((((*puVar6 & 0x80) != 0) && (*(char *)(iVar5 + 2) == '\0')) &&
         (*(char *)(iVar5 + 4) == '\b')) {
        bVar4 = *(byte *)(*(int *)(iVar5 + 8) + 8) & 4;
        *(byte *)(uVar9 + iVar2) = bVar4;
        if ((bVar4 == 0) && (*(char *)(uVar9 + iVar3) != '\0')) {
          return uVar9 + 1;
        }
      }
    }
    uVar9 = uVar9 + 1;
  } while( true );
}

