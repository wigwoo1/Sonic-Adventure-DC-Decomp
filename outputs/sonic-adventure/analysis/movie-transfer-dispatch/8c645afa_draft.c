/* Ghidra draft only; not buildable/verified source. */

/* WARNING: Removing unreachable block (ram,0x8c645b04) */

void FUN_8c645afa(short param_1)

{
  undefined4 *puVar1;
  
  for (puVar1 = *(undefined4 **)(DAT_8c645b60 + ((int)param_1 >> 4) * 4);
      puVar1 != (undefined4 *)0x0; puVar1 = (undefined4 *)puVar1[1]) {
    (*(code *)*puVar1)(puVar1[2]);
  }
  return;
}

