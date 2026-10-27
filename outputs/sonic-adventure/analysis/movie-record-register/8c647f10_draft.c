/* Ghidra draft only; not buildable/verified source. */

int FUN_8c647f10(int param_1)

{
  undefined4 uVar1;
  int iVar2;
  
  iVar2 = *DAT_8c647fc8;
  if (param_1 == 0xf) {
    *(undefined4 *)(iVar2 + 4) = 0;
  }
  else {
    if (param_1 == 0x21) {
      uVar1 = 1;
    }
    else {
      if (param_1 == 0x41) {
        *(undefined4 *)(iVar2 + 4) = 2;
        return 0;
      }
      if (param_1 != 0x7a) {
        return (int)DAT_8c647fb2;
      }
      uVar1 = 3;
    }
    *(undefined4 *)(iVar2 + 4) = uVar1;
  }
  return 0;
}

