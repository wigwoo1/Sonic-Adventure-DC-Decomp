/* Ghidra draft only; not buildable/verified source. */

undefined4 FUN_8c660d8c(char *param_1,char *param_2)

{
  int iVar1;
  undefined4 uVar2;
  
  iVar1 = 0;
  uVar2 = 0;
  do {
    iVar1 = iVar1 + 1;
    if (*param_1 != *param_2) {
      uVar2 = 0xffffffff;
    }
    param_1 = param_1 + 1;
    param_2 = param_2 + 1;
  } while (iVar1 < DAT_8c660e36);
  return uVar2;
}

