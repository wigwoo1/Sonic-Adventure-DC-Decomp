/* Ghidra draft only; not buildable/verified source. */

void FUN_8c093c74(void)

{
  undefined *puVar1;
  uint uVar2;
  uint uVar3;
  code *pcVar4;
  
  puVar1 = PTR_FUN_8c093cd0;
  pcVar4 = FUN_8c010b58;
  uVar2 = DAT_8c093cc0 + 0x20;
  for (uVar3 = DAT_8c093cc0; uVar3 < uVar2; uVar3 = uVar3 + 8) {
    (*(code *)puVar1)(uVar3,pcVar4);
    (*(code *)puVar1)(uVar3 + 4);
  }
  return;
}

