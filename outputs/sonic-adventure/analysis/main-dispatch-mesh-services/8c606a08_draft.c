/* Ghidra draft only; not buildable/verified source. */

void FUN_8c606a08(uint param_1)

{
  uint *puVar1;
  uint *puVar2;
  undefined4 *puVar3;
  uint uVar4;
  uint uVar5;
  uint uVar6;
  
  puVar3 = DAT_8c606b3c;
  puVar2 = DAT_8c606b20;
  uVar5 = *DAT_8c606b20 & DAT_8c606b24 | DAT_8c606b2c & param_1;
  uVar4 = DAT_8c606b30 & param_1;
  uVar6 = DAT_8c606b20[2] & DAT_8c606b28;
  puVar1 = DAT_8c606b20 + 1;
  if ((((int)DAT_8c606b14 & *DAT_8c606b34) == 0) &&
     (((*DAT_8c606b34 & (int)DAT_8c606b16) == 0 || ((DAT_8c606b38 & param_1) != 0)))) {
    uVar5 = uVar5 & (int)DAT_8c606b1a;
  }
  else {
    uVar5 = uVar5 | (int)DAT_8c606b18;
  }
  *DAT_8c606b20 = uVar5;
  puVar2[1] = *puVar1;
  puVar2[2] = uVar6 | uVar4;
  *(uint *)*puVar3 = param_1;
  return;
}

