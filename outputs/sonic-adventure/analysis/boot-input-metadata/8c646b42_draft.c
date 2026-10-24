/* Ghidra draft only; not buildable/verified source. */

uint * FUN_8c646b42(uint *param_1,uint param_2)

{
  int iVar1;
  uint *puVar2;
  uint uVar3;
  uint *puVar4;
  
  puVar2 = param_1 + 1;
  puVar4 = param_1 + 1;
  iVar1 = 0;
  uVar3 = DAT_8c646c3c;
  do {
    if ((uVar3 & *param_1) != 0) {
      if (param_2 == uVar3) {
        return puVar4;
      }
      puVar2 = puVar2 + 1;
      puVar4 = puVar2;
    }
    iVar1 = iVar1 + 1;
    uVar3 = uVar3 >> 1;
  } while (iVar1 < 0x20);
  return (uint *)0x0;
}

