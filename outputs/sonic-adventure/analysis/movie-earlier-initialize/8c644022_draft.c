/* Ghidra draft only; not buildable/verified source. */

void FUN_8c644022(void)

{
  undefined *puVar1;
  undefined4 uVar2;
  
  puVar1 = PTR_FUN_8c6440f0;
  if (*DAT_8c6440ec < 1) {
    *DAT_8c6440ec = *DAT_8c6440ec + 1;
    (*(code *)puVar1)();
    (*(code *)PTR_thunk_FUN_8c65ca92_8c6440f4)();
    (*(code *)PTR_FUN_8c6440f8)();
    (*(code *)PTR_FUN_8c6440fc)();
    (*(code *)PTR_FUN_8c644100)();
    (*(code *)PTR_FUN_8c644104)();
    (*(code *)PTR_FUN_8c644108)();
    (*(code *)PTR_FUN_8c64410c)();
    (*(code *)PTR_FUN_8c644110)();
    (*(code *)PTR_FUN_8c644118)(PTR_DAT_8c644114,0,(int)DAT_8c6440e4);
    if (*(int *)PTR_DAT_8c6440e8 == 1) {
      uVar2 = (*(code *)PTR_FUN_8c644120)((int)DAT_8c6440e6,PTR_FUN_8c64411c,0x70,0);
      *(undefined4 *)PTR_DAT_8c644124 = uVar2;
    }
    (*(code *)PTR_FUN_8c644128)();
    return;
  }
  return;
}

