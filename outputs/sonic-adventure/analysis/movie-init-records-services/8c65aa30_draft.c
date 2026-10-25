/* Ghidra draft only; not buildable/verified source. */

int FUN_8c65aa30(int *param_1,int param_2)

{
  int iVar1;
  int iVar2;
  int iVar3;
  
  iVar1 = DAT_8c65aae8;
  iVar3 = *(int *)PTR_DAT_8c65aae0;
  if (param_2 == 1) {
    iVar2 = 0;
    iVar3 = iVar3 + 8;
    do {
      if (*(int *)(iVar3 + 4) != 1) break;
      iVar2 = iVar2 + 1;
      iVar3 = iVar3 + 8;
    } while (iVar2 < 8);
joined_r0x8c65aa6a:
    if (iVar2 == 8) {
LAB_8c65aaaa:
      iVar3 = 0;
    }
  }
  else {
    if (param_2 == 2) {
      iVar2 = 0;
      iVar3 = iVar3 + 0x48;
      do {
        if (*(int *)(iVar3 + 4) != 1) break;
        iVar2 = iVar2 + 1;
        iVar3 = iVar3 + 8;
      } while (iVar2 < 8);
      goto joined_r0x8c65aa6a;
    }
    iVar2 = DAT_8c65aae4;
    if (param_2 != 3) goto LAB_8c65aac8;
    iVar2 = 0;
    iVar3 = DAT_8c65aade + iVar3;
    do {
      if (*(int *)(iVar3 + 4) != 1) break;
      iVar2 = iVar2 + 1;
      iVar3 = iVar3 + 8;
    } while (iVar2 < 4);
    if (iVar2 == 4) goto LAB_8c65aaaa;
  }
  if (iVar3 == 0) {
    *param_1 = 0;
    iVar2 = iVar1;
  }
  else {
    *param_1 = iVar3;
    *(undefined4 *)(iVar3 + 4) = 1;
    iVar2 = 0;
  }
LAB_8c65aac8:
  if (iVar2 != 0) {
    (*(code *)PTR_FUN_8c65aaf0)(PTR_FUN_8c65aaec,iVar2);
  }
  return iVar2;
}

