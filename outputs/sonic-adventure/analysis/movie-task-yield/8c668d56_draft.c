/* Ghidra draft only; not buildable/verified source. */

/* WARNING: Removing unreachable block (ram,0x8c668e38) */

void callback_8c668d56(void)

{
  int *piVar1;
  undefined *puVar2;
  int iVar3;
  
  *(undefined4 *)(*(int *)((int)DAT_8c668d78 + *DAT_8c668d8c) + 0xc) = 3;
  puVar2 = PTR_FUN_8c668eac;
  piVar1 = DAT_8c668ea4;
  iVar3 = (int)DAT_8c668ea0;
  *(undefined4 *)(*(int *)((int)DAT_8c668ea0 + *DAT_8c668ea4) + 0x14) =
       *(undefined4 *)((int)DAT_8c668e9e + *DAT_8c668ea4);
  iVar3 = (*(code *)puVar2)(*(int *)(iVar3 + *piVar1) + 0x18);
  if (iVar3 == 0) {
    (*(code *)PTR_FUN_8c668eb0)(*piVar1 + 4,1);
  }
  return;
}

