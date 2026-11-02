/* Ghidra draft only; not buildable/verified source. */

void FUN_8c019adc(void)

{
  char cVar1;
  uint *puVar2;
  int iVar3;
  undefined *puVar4;
  int iVar5;
  
  puVar4 = PTR_DAT_8c019bc8;
  iVar3 = DAT_8c019bc4;
  puVar2 = DAT_8c019bc0;
  *DAT_8c019bc0 = *DAT_8c019bc0 + 1;
  iVar5 = *(int *)puVar4;
  if ((*(uint *)(iVar3 + 8) & 0xf0) == 0) {
    if ((*(char *)(iVar5 + 6) == '@') &&
       ((((*(uint *)(iVar3 + 8) & 0x606) != 0 || (*(short *)(iVar3 + 0x1c) != 0)) ||
        ((*(short *)(iVar3 + 0x1e) != 0 || (*DAT_8c019bcc == 0)))))) {
      (*(code *)PTR_FUN_8c019bd0)();
      (*(code *)PTR_FUN_8c019bd4)(5);
      return;
    }
  }
  else if (((*(char *)(iVar5 + 6) == '@') || (*(char *)(iVar5 + 8) == '\x02')) ||
          (*DAT_8c019bcc == 0)) {
    if ((*(char *)(iVar5 + 6) == '@') &&
       ((((*(uint *)(iVar3 + 8) & 0x606) != 0 || (*(short *)(iVar3 + 0x1c) != 0)) ||
        ((*(short *)(iVar3 + 0x1e) != 0 || (*DAT_8c019bcc == 0)))))) {
      (*(code *)PTR_FUN_8c019bd0)();
      (*(code *)PTR_FUN_8c019bd4)(5);
      return;
    }
  }
  else {
    cVar1 = *(char *)(iVar5 + 6);
    if (((cVar1 != '\x17') && (cVar1 != '\x18')) &&
       ((cVar1 != '\x19' &&
        ((((cVar1 != '\x1c' && (cVar1 != '\x1d')) && (cVar1 != '\x1e')) &&
         ((0x1e < *puVar2 && (*(char *)(iVar5 + 8) != '\x04')))))))) {
      (*(code *)PTR_FUN_8c019bd8)(0x40,5);
      return;
    }
  }
  return;
}

