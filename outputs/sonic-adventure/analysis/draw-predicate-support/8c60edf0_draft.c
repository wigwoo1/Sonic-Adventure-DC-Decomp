/* Ghidra draft only; not buildable/verified source. */

void FUN_8c60edf0(undefined8 *param_1)

{
  undefined8 *puVar1;
  char in_FPSCR_SZ;
  
  puVar1 = DAT_8c60ee40;
  if (in_FPSCR_SZ == '\0') {
    *(undefined4 *)DAT_8c60ee40 = *(undefined4 *)param_1;
    *(undefined4 *)((int)puVar1 + 4) = *(undefined4 *)((int)param_1 + 4);
    *(undefined4 *)(puVar1 + 1) = *(undefined4 *)(param_1 + 1);
    *(undefined4 *)((int)puVar1 + 0xc) = *(undefined4 *)((int)param_1 + 0xc);
  }
  else {
    *DAT_8c60ee40 = *param_1;
    *(undefined8 *)((int)puVar1 + 4) = *(undefined8 *)((int)param_1 + 4);
    puVar1[1] = param_1[1];
    *(undefined8 *)((int)puVar1 + 0xc) = *(undefined8 *)((int)param_1 + 0xc);
  }
  return;
}

