/* Ghidra draft only; not buildable/verified source. */

int FUN_8c64b0e4(undefined4 param_1)

{
  int iVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  
  iVar1 = DAT_8c64b1e4;
  iVar4 = DAT_8c64b1e4 + -2;
  while( true ) {
    iVar2 = FUN_8c64b080(param_1,iVar1);
    if (iVar2 < 0) {
      return -1;
    }
    iVar3 = FUN_8c64b12a(param_1,iVar2);
    if (-1 < iVar3) break;
    FUN_8c64b18e(param_1,iVar2,iVar4);
  }
  return iVar2;
}

