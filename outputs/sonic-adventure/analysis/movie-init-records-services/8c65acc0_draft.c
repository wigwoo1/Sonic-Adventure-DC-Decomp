/* Ghidra draft only; not buildable/verified source. */

undefined4 FUN_8c65acc0(void)

{
  undefined *puVar1;
  int iVar2;
  uint uVar3;
  
  puVar1 = PTR_DAT_8c65ad2c;
  iVar2 = (int)DAT_8c65ad26;
  *(uint *)PTR_DAT_8c65ad2c = DAT_8c65ad30 & 0xffffffe0;
  uVar3 = DAT_8c65ad34 & 0xffffffe0;
  (*(code *)PTR_FUN_8c65ad3c)(DAT_8c65ad38,0x40);
  (*(code *)PTR_FUN_8c65ad44)(DAT_8c65ad40,0x40);
  (*(code *)PTR_FUN_8c65ad48)(uVar3);
  (*(code *)PTR_FUN_8c65ad50)(DAT_8c65ad4c,iVar2);
  (*(code *)PTR_FUN_8c65ad54)();
  *(undefined4 *)((int)DAT_8c65ad28 + *(int *)puVar1) = 0;
  (*(code *)PTR_FUN_8c65ad58)(*(int *)puVar1 + (int)DAT_8c65ad2a,0);
  *DAT_8c65ad5c = *(undefined4 *)((int)DAT_8c65ad2a + *(int *)puVar1);
  return 0;
}

