/* Ghidra draft only; not buildable/verified source. */

uint FUN_8c645442(char *param_1)

{
  uint uVar1;
  uint uVar2;
  
  uVar2 = (uint)DAT_8c64554e;
  for (; *param_1 != '\0'; param_1 = param_1 + 1) {
    uVar1 = FUN_8c645348((int)*param_1);
    uVar2 = uVar2 + (uVar1 & 0xff);
  }
  return uVar2 >> 2 & 0xff;
}

