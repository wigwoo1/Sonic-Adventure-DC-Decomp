/* Ghidra draft only; not buildable/verified source. */

void FUN_8c640528(undefined8 *param_1)

{
  undefined8 *puVar1;
  char in_FPSCR_SZ;
  
  puVar1 = DAT_8c64053c;
  if (in_FPSCR_SZ == '\0') {
    *(undefined4 *)DAT_8c64053c = *(undefined4 *)param_1;
    *(undefined4 *)((int)puVar1 + 0xc) = *(undefined4 *)((int)param_1 + 0xc);
    *(undefined4 *)(puVar1 + 2) = *(undefined4 *)(param_1 + 2);
  }
  else {
    *DAT_8c64053c = *param_1;
    *(undefined8 *)((int)puVar1 + 0xc) = *(undefined8 *)((int)param_1 + 0xc);
    puVar1[2] = param_1[2];
  }
  return;
}

