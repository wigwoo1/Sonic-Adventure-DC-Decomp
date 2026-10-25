/* Ghidra draft only; not buildable/verified source. */

byte FUN_8c645468(byte *param_1,int param_2)

{
  undefined1 uVar1;
  byte bVar2;
  uint uVar3;
  
  if (*param_1 < 0x22) {
    bVar2 = 0;
  }
  else {
    FUN_8c64535c(param_1 + 2,param_2,4);
    FUN_8c64535c(param_1 + 10,param_2 + 4,4);
    *(byte *)(param_2 + 8) = param_1[0x19];
    uVar3 = (uint)param_1[0x20];
    if ((*(byte *)(param_2 + 8) & 2) == 0) {
      uVar3 = uVar3 - 2;
    }
    FUN_8c64535c(param_1 + 0x21,param_2 + 10,uVar3);
    *(undefined1 *)(uVar3 + param_2 + 10) = 0;
    if ((*(byte *)(param_2 + 8) & 2) != 0) {
      if (*(char *)(param_2 + 10) == '\0') {
        *(undefined1 *)(param_2 + 10) = 0x2e;
      }
      else if (*(char *)(param_2 + 10) == '\x01') {
        *(undefined1 *)(param_2 + 10) = 0x2e;
        *(undefined1 *)(param_2 + 0xb) = 0x2e;
        *(undefined1 *)(param_2 + 0xc) = 0;
      }
    }
    uVar1 = FUN_8c645442(param_2 + 10);
    *(undefined1 *)(param_2 + 9) = uVar1;
    bVar2 = *param_1;
  }
  return bVar2;
}

