/* Ghidra draft only; not buildable/verified source. */

void FUN_8c010b00(void)

{
  undefined *puVar1;
  undefined *puVar2;
  uint uVar3;
  int iVar4;
  
  (*(code *)PTR_FUN_8c010c04)(DAT_8c010c00,0,0x260);
  uVar3 = *(uint *)PTR_DAT_8c010c10;
  if ((int)uVar3 < 0) {
    iVar4 = 1 >> (~uVar3 & 0x1f) + 1;
  }
  else {
    iVar4 = 1 << (uVar3 & 0x1f);
  }
  (*(code *)PTR_FUN_8c010c14)(0xf,iVar4,DAT_8c010c0c,PTR_callback_8c010f22_8c010c08);
  puVar2 = PTR_DAT_8c010c20;
  puVar1 = PTR_DAT_8c010c1c;
  *(undefined4 *)PTR_DAT_8c010c18 = 0;
  *(undefined4 *)puVar1 = 0;
  *(undefined4 *)puVar2 = 1;
  return;
}

