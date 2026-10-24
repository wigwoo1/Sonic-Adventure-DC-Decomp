/* Ghidra draft only; not buildable/verified source. */

undefined4 FUN_8c65262c(uint param_1)

{
  undefined *puVar1;
  undefined4 uVar2;
  
  (*(code *)PTR_FUN_8c652694)();
  if (((int)param_1 < 3) && (-1 < (int)param_1)) {
    uVar2 = (*(code *)PTR_FUN_8c652748)(PTR_s_KAMUI_8c652744,PTR_s_VideoMode_8c652740,param_1 & 3);
    puVar1 = PTR_FUN_8c652750;
    *DAT_8c65274c = uVar2;
    (*(code *)puVar1)();
    return 0;
  }
  return 2;
}

