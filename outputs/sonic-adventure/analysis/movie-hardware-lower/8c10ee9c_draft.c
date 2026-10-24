/* Ghidra draft only; not buildable/verified source. */

undefined4 FUN_8c10ee9c(void)

{
  undefined *puVar1;
  undefined *puVar2;
  uint uVar3;
  uint uVar4;
  undefined1 *puVar5;
  
  puVar2 = PTR_DAT_8c10eed8;
  puVar1 = PTR_FUN_8c10eed4;
  uVar4 = 0;
  puVar5 = &DAT_8c88f644;
  uVar3 = (uint)DAT_8c10eed0;
  do {
    (*(code *)puVar1)(*(int *)puVar2 + 4 + uVar4,puVar5);
    uVar4 = uVar4 + 0x34;
  } while (uVar4 < uVar3);
  *DAT_8c10eedc = 1;
  return 0;
}

