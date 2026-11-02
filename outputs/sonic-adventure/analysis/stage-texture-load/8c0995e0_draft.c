/* Ghidra draft only; not buildable/verified source. */

undefined4 FUN_8c0995e0(short param_1)

{
  int iVar1;
  int *piVar2;
  undefined4 *puVar3;
  short *psVar4;
  undefined1 *puVar5;
  
  puVar5 = &DAT_8c7493c8;
  iVar1 = *(int *)PTR_PTR_8c0996e0;
  piVar2 = (int *)PTR_PTR_8c0996e0;
  while( true ) {
    if (iVar1 == 0) {
      return 0;
    }
    psVar4 = (short *)*piVar2;
    if (*psVar4 == param_1) break;
    piVar2 = piVar2 + 1;
    iVar1 = *piVar2;
  }
  if (*(char *)((int)psVar4 + 3) == '\0') {
    puVar3 = *(undefined4 **)(psVar4 + 2);
    for (iVar1 = (int)(char)psVar4[1]; 0 < iVar1; iVar1 = iVar1 + -1) {
      FUN_8c099750(*puVar3,puVar3[1],puVar5);
      puVar3 = puVar3 + 2;
    }
  }
  *(char *)((int)psVar4 + 3) = *(char *)((int)psVar4 + 3) + '\x01';
  return 1;
}

