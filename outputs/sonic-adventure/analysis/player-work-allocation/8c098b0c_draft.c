/* Ghidra draft only; not buildable/verified source. */

undefined4 * FUN_8c098b0c(int param_1,int param_2)

{
  undefined4 *puVar1;
  undefined4 *puVar2;
  uint uVar3;
  
  uVar3 = (param_2 + 3U & 0xfffffffc) * param_1;
  puVar1 = (undefined4 *)FUN_8c098a80(uVar3);
  if (puVar1 != (undefined4 *)0x0) {
    puVar2 = puVar1;
    for (uVar3 = uVar3 >> 2; uVar3 != 0; uVar3 = uVar3 - 1) {
      *puVar2 = 0;
      puVar2 = puVar2 + 1;
    }
  }
  return puVar1;
}

