/* Ghidra draft only; not buildable/verified source. */

int FUN_8c660e80(int param_1)

{
  undefined2 uVar1;
  int iVar2;
  undefined2 *puVar3;
  uint uVar4;
  
  uVar1 = DAT_8c660f36;
  uVar4 = param_1 + 0x3fU & 0xffffffe0;
  iVar2 = *DAT_8c660f40;
  while( true ) {
    if (iVar2 == DAT_8c660f38) {
      return 0;
    }
    if ((*(short *)(iVar2 + 2) == 0) && ((int)uVar4 <= *(int *)(iVar2 + 4))) break;
    iVar2 = *(int *)(iVar2 + 8);
  }
  if (0x40 < (int)(*(int *)(iVar2 + 4) - uVar4)) {
    puVar3 = (undefined2 *)(uVar4 + iVar2);
    *(uint *)(puVar3 + 2) = *(int *)(iVar2 + 4) - uVar4;
    *puVar3 = uVar1;
    puVar3[1] = 0;
    *(undefined2 **)(*(int *)(iVar2 + 8) + 0xc) = puVar3;
    *(undefined4 *)(puVar3 + 4) = *(undefined4 *)(iVar2 + 8);
    *(undefined2 **)(iVar2 + 8) = puVar3;
    *(int *)(puVar3 + 6) = iVar2;
    *(uint *)(iVar2 + 4) = uVar4;
  }
  *(undefined2 *)(iVar2 + 2) = 1;
  return iVar2 + 0x20;
}

