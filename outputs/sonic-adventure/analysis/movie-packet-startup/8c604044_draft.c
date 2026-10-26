/* Ghidra draft only; not buildable/verified source. */

/* WARNING: Removing unreachable block (ram,0x8c60408e) */
/* WARNING: Removing unreachable block (ram,0x8c60409a) */

int FUN_8c604044(int param_1,undefined4 param_2)

{
  undefined *puVar1;
  undefined *puVar2;
  int iVar3;
  int iVar4;
  undefined4 local_14;
  undefined4 local_10;
  
  puVar2 = PTR_FUN_8c604118;
  puVar1 = PTR_DAT_8c604110;
  if ((param_1 < 1) || (4 < param_1)) {
    iVar3 = -7;
  }
  else {
    *(undefined4 *)PTR_DAT_8c604110 = DAT_8c604114;
    (*(code *)puVar2)(param_1,&local_14);
    iVar3 = (int)DAT_8c604108;
    **(undefined2 **)puVar1 = (short)param_1;
    *(undefined4 *)(*(int *)puVar1 + 4) = local_14;
    *(undefined4 *)(*(int *)puVar1 + 8) = local_10;
    *(int *)(*(int *)puVar1 + 0x18) = *(int *)(*(int *)puVar1 + 8) + iVar3 >> 0xf;
    iVar3 = *(int *)puVar1;
    *(int *)(iVar3 + 0x14) = ((*(int *)(iVar3 + 8) >> 6) - *(int *)(iVar3 + 0x18)) + -1;
    *(undefined4 *)(*(int *)puVar1 + 0xc) = param_2;
    *(undefined4 *)(*(int *)puVar1 + 0x1c) = 0;
    *(undefined4 *)(*(int *)puVar1 + 0x20) = 0;
    *(undefined4 *)(*(int *)puVar1 + 0x2c) = 0;
    iVar3 = (*(code *)PTR_FUN_8c60411c)
                      (*(undefined4 *)(*(int *)puVar1 + 4),*(int *)puVar1 + 0x34,0x40);
    if (iVar3 == -1) {
      iVar3 = -1;
    }
    else {
      iVar4 = *(int *)puVar1;
      iVar3 = thunk_FUN_8c603bfc(iVar4 + 0x34,PTR_s_KATANA_FLASH_____8c604120,0xc);
      if ((iVar3 != 0) && ((int)**(short **)puVar1 == (int)*(char *)(iVar4 + 0x44))) {
        (*(short **)puVar1)[1] = (short)*(char *)(iVar4 + 0x45);
        if ((*(short *)(*(int *)puVar1 + 2) == 0) || (*(short *)(*(int *)puVar1 + 2) == 1)) {
          FUN_8c603c38(*(undefined4 *)(*(int *)puVar1 + 0xc),*(undefined4 *)(*(int *)puVar1 + 8));
          iVar3 = FUN_8c603d22(*(undefined4 *)puVar1);
          if (iVar3 != 0) {
            return iVar3;
          }
          return 0;
        }
      }
      iVar3 = -8;
    }
  }
  return iVar3;
}

