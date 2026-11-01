/* Ghidra draft only; not buildable/verified source. */

undefined4 FUN_8c64f1a2(void)

{
  undefined *puVar1;
  undefined *puVar2;
  int iVar3;
  int local_24;
  undefined4 local_20;
  undefined1 auStack_1c [8];
  undefined4 local_14;
  
  do {
    puVar1 = PTR_FUN_8c64f238;
    iVar3 = (*(code *)PTR_FUN_8c64f238)(&local_24,4);
    puVar2 = PTR_FUN_8c64f23c;
    if (iVar3 < 0) {
      return 0xffffffff;
    }
    if (local_24 == DAT_8c64f240) {
      (*(code *)PTR_FUN_8c64f23c)(0xfffffffc,1);
      iVar3 = (*(code *)puVar1)(auStack_1c,0x10);
      if (iVar3 < 0) {
        return 0xffffffff;
      }
    }
    else {
      iVar3 = (*(code *)puVar1)(&local_20,4);
      if (iVar3 < 0) {
        return 0xffffffff;
      }
      (*(code *)puVar2)(local_20,1);
    }
  } while (local_24 != DAT_8c64f240);
  (*(code *)puVar2)(0);
  return local_14;
}

