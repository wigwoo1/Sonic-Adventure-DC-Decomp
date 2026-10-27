/* Ghidra draft only; not buildable/verified source. */

void FUN_8c647de8(uint param_1)

{
  int *piVar1;
  undefined *puVar2;
  
  piVar1 = DAT_8c647e9c;
  *(uint *)*DAT_8c647e9c = param_1 & 1;
  puVar2 = PTR_LAB_8c647eac;
  if ((param_1 & 1) == 1) {
    *(undefined **)(*piVar1 + 0xc) = PTR_LAB_8c647ea0;
    *(undefined **)(*piVar1 + 0x10) = PTR_LAB_8c647ea4;
  }
  else {
    *(undefined **)(*piVar1 + 0xc) = PTR_LAB_8c647ea8;
    *(undefined **)(*piVar1 + 0x10) = puVar2;
  }
  return;
}

