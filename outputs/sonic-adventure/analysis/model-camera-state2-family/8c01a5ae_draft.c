/* Ghidra draft only; not buildable/verified source. */

void FUN_8c01a5ae(void)

{
  undefined *puVar1;
  int *piVar2;
  short *psVar3;
  int iVar4;
  
  psVar3 = DAT_8c01a674;
  piVar2 = DAT_8c01a670;
  puVar1 = PTR_DAT_8c01a664;
  if ((*(char *)(*(int *)PTR_DAT_8c01a664 + 8) == '\0') &&
     ((*(uint *)(DAT_8c01a668 + 0x10) & *(uint *)PTR_DAT_8c01a66c) != 0)) {
    do {
      *(char *)(*(int *)puVar1 + 6) = *(char *)(*(int *)puVar1 + 6) + '\x01';
      if ((int)*psVar3 <= (int)*(char *)(*(int *)puVar1 + 6)) {
        *(undefined1 *)(*(int *)puVar1 + 6) = 0;
      }
      iVar4 = *(int *)puVar1;
    } while ((*(int *)(*(char *)(iVar4 + 6) * 0x10 + *piVar2 + 8) != 1) ||
            (*(char *)(iVar4 + 8) != '\0'));
    FUN_8c01aae0((int)*(char *)(iVar4 + 6),0);
  }
  return;
}

