/* Ghidra draft only; not buildable/verified source. */

/* WARNING: Removing unreachable block (ram,0x8c051836) */

void FUN_8c051810(void)

{
  undefined4 uVar1;
  int iVar2;
  
  iVar2 = *DAT_8c051884;
  *DAT_8c051884 = iVar2 + -1;
  if (iVar2 + -1 == 0) {
    uVar1 = (*(code *)PTR_FUN_8c0518d4)();
    iVar2 = *DAT_8c0518dc;
    if (((iVar2 == 4) || (iVar2 == 5)) || (iVar2 == 9)) {
      (**(code **)(PTR_PTR_8c0518e0 +
                  (uint)((ushort)uVar1 & 0xff) * 4 +
                  (uint)((ushort)((uint)uVar1 >> 8) & 0xff & (ushort)((uint)DAT_8c0518d8 >> 8)) *
                  0x20))();
    }
    (*(code *)PTR_FUN_8c0518e4)();
  }
  (*(code *)PTR_FUN_8c0518e8)();
  (*(code *)PTR_FUN_8c0518ec)();
  return;
}

