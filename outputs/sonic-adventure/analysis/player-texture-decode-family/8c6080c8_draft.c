/* Ghidra draft only; not buildable/verified source. */

void FUN_8c6080c8(undefined4 *param_1)

{
  undefined *puVar1;
  int iVar2;
  int local_20;
  undefined4 local_1c;
  int local_18;
  int local_14;
  ushort local_10;
  ushort local_e;
  
  iVar2 = DAT_8c6081d0;
  do {
    (*(code *)PTR_FUN_8c6081c8)(&local_20,4);
    if (local_20 == DAT_8c6081d0) {
      (*(code *)PTR_FUN_8c6081c8)(&local_18,0xc);
      param_1[4] = (uint)local_10;
      param_1[5] = (uint)local_e;
      param_1[1] = local_14 << 0x10;
    }
    else {
      (*(code *)PTR_FUN_8c6081c8)(&local_1c,4);
      (*(code *)PTR_FUN_8c6081cc)(local_1c,1);
    }
    puVar1 = PTR_FUN_8c6081cc;
  } while (local_20 != iVar2);
  *param_1 = *DAT_8c6081d4;
  (*(code *)puVar1)(local_18 + -8,1);
  return;
}

