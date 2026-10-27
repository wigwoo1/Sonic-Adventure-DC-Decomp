/* Ghidra draft only; not buildable/verified source. */

void FUN_8c053ac0(void)

{
  undefined *puVar1;
  undefined *puVar2;
  int iVar3;
  
  (*(code *)PTR_FUN_8c053b20)();
  FUN_8c0546c0();
  puVar2 = PTR_FUN_8c053b28;
  puVar1 = PTR_DAT_8c053b24;
  do {
    iVar3 = (*(code *)puVar2)(*(undefined4 *)puVar1);
  } while (iVar3 == -1);
  (*(code *)PTR_FUN_8c053b2c)();
  (*(code *)PTR_FUN_8c053b2c)();
  *DAT_8c053b30 = 0;
  (*(code *)PTR_FUN_8c053b34)(0);
  (*(code *)PTR_FUN_8c053b38)();
  (*(code *)PTR_FUN_8c053b3c)();
  puVar1 = PTR_LAB_8c053b44;
  *DAT_8c053b40 = 0;
  (*(code *)PTR_FUN_8c053b48)(puVar1,0);
  return;
}

