/* Ghidra draft only; not buildable/verified source. */

void FUN_8c038bb8(undefined4 param_1,undefined4 param_2,undefined4 param_3,undefined4 param_4)

{
  int iVar1;
  undefined *puVar2;
  undefined8 in_xd4;
  char in_FPSCR_SZ;
  
  puVar2 = PTR_DAT_8c038bd0;
  iVar1 = DAT_8c038bcc;
  if (in_FPSCR_SZ == '\0') {
    *(undefined4 *)(DAT_8c038bcc + -4) = param_3;
    *(undefined4 *)(iVar1 + -8) = param_2;
    *(undefined4 *)(iVar1 + -0xc) = param_1;
    *(undefined4 *)(puVar2 + -4) = param_3;
    *(undefined4 *)(puVar2 + -8) = param_2;
    *(undefined4 *)(puVar2 + -0xc) = param_1;
  }
  else {
    *(ulonglong *)(DAT_8c038bcc + -8) = CONCAT44(param_3,param_4);
    *(undefined8 *)(iVar1 + -0x10) = in_xd4;
    *(ulonglong *)(iVar1 + -0x18) = CONCAT44(param_1,param_2);
    *(ulonglong *)(puVar2 + -8) = CONCAT44(param_3,param_4);
    *(undefined8 *)(puVar2 + -0x10) = in_xd4;
    *(ulonglong *)(puVar2 + -0x18) = CONCAT44(param_1,param_2);
  }
  return;
}

