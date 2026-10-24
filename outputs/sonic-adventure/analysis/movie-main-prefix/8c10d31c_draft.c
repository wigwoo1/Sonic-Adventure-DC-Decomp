/* Ghidra draft only; not buildable/verified source. */

undefined1 * FUN_8c10d31c(undefined1 *param_1,undefined1 param_2,uint param_3)

{
  undefined1 *puVar1;
  uint uVar2;
  
  uVar2 = 0;
  puVar1 = param_1;
  if (param_3 != 0) {
    do {
      uVar2 = uVar2 + 1;
      *puVar1 = param_2;
      puVar1 = puVar1 + 1;
    } while (uVar2 < param_3);
  }
  return param_1;
}

