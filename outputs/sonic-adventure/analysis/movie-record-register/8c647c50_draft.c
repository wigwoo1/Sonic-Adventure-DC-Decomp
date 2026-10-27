/* Ghidra draft only; not buildable/verified source. */

void FUN_8c647c50(undefined4 param_1)

{
  uint uVar1;
  int iVar2;
  uint uVar3;
  
  iVar2 = (int)DAT_8c647d0c;
  uVar3 = DAT_8c647d10 + (int)DAT_8c647d0e;
  uVar1 = DAT_8c647d10;
  do {
    *(undefined4 *)(uVar1 + 0x74) = param_1;
    uVar1 = uVar1 + iVar2;
  } while (uVar1 < uVar3);
  return;
}

