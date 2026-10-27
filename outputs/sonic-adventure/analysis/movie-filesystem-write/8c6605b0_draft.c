/* Ghidra draft only; not buildable/verified source. */

undefined4 FUN_8c6605b0(undefined4 param_1)

{
  byte bVar1;
  undefined4 uVar2;
  
  bVar1 = **(byte **)PTR_PTR_8c660610;
  **(byte **)PTR_PTR_8c660610 = bVar1 | 0x80;
  if (bVar1 != 0) {
    (*(code *)PTR_FUN_8c660618)(PTR_s____FATAL_ERROR_Cannot_get_semaph_8c660614);
    return 0xfffffffe;
  }
  uVar2 = FUN_8c6605e2(param_1);
  **(undefined1 **)PTR_PTR_8c660610 = 0;
  return uVar2;
}

