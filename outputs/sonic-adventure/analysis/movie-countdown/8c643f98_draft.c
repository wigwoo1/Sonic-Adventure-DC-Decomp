/* Ghidra draft only; not buildable/verified source. */

undefined4 FUN_8c643f98(void)

{
  if ((*(int *)PTR_DAT_8c643ff4 != 0) && (*(int *)PTR_DAT_8c643ff8 == 1)) {
    if (0 < *(int *)((int)DAT_8c643ff0 + *(int *)PTR_DAT_8c643ff4)) {
      (*(code *)PTR_FUN_8c643ffc)();
    }
    if (*(int *)((int)DAT_8c643ff2 + *(int *)PTR_DAT_8c643ff4) != 0) {
      (*(code *)PTR_FUN_8c644004)(0,DAT_8c644000);
      (*(code *)PTR_FUN_8c64400c)(0,DAT_8c644008);
      if (*DAT_8c644008 < *DAT_8c644000) {
        (*(code *)PTR_FUN_8c644018)(PTR_FUN_8c644014,DAT_8c644010);
      }
    }
  }
  return 0;
}

