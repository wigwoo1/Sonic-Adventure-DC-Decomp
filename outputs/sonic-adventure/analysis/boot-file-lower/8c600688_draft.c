/* Ghidra draft only; not buildable/verified source. */

undefined4 FUN_8c600688(void)

{
  int iVar1;
  undefined4 local_c [3];
  
  iVar1 = (**(code **)(*(int *)(*(int *)PTR_DAT_8c6006b4 + 0xc) + 0x20))(local_c);
  if (iVar1 == 0) {
    return 0xffffffff;
  }
  return local_c[0];
}

