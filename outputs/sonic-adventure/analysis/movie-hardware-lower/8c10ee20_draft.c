/* Ghidra draft only; not buildable/verified source. */

undefined4 FUN_8c10ee20(void)

{
  undefined *puVar1;
  undefined *puVar2;
  uint uVar3;
  uint uVar4;
  code *pcVar5;
  
  puVar2 = PTR_DAT_8c10ee5c;
  puVar1 = PTR_FUN_8c10ee58;
  uVar4 = 0;
  pcVar5 = FUN_8c10eee0;
  uVar3 = (uint)DAT_8c10ee54;
  do {
    (*(code *)puVar1)(*(int *)puVar2 + 4 + uVar4,pcVar5);
    uVar4 = uVar4 + 0x34;
  } while (uVar4 < uVar3);
  *DAT_8c10ee60 = 0;
  return 0;
}

