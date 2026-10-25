/* Ghidra draft only; not buildable/verified source. */

undefined4 FUN_8c5ff31e(undefined4 *param_1)

{
  undefined *puVar1;
  int iVar2;
  
  puVar1 = PTR_DAT_8c5ff4b4;
  if (((param_1 != (undefined4 *)0x0) && (*param_1 = 0, *(int *)puVar1 != 0)) &&
     (iVar2 = (*(code *)PTR_FUN_8c5ff4bc)(0), iVar2 != -1)) {
    if (*DAT_8c5ff4b8 == '\0') {
      *param_1 = 0;
    }
    else {
      if (*DAT_8c5ff4b8 != '\x01') {
        return 0xffffffff;
      }
      *param_1 = 1;
    }
    return 0;
  }
  return 0xffffffff;
}

