/* Ghidra draft only; not buildable/verified source. */

undefined4 FUN_8c06c880(int param_1)

{
  char cVar1;
  char cVar2;
  ushort uVar3;
  int iVar4;
  int iVar5;
  
  if ((*(ushort *)(param_1 + 4) & 0x800) == 0) {
    uVar3 = *(ushort *)(*(int *)(param_1 + 0x38) + 2);
    if ((uVar3 & 2) == 0) {
      return 0;
    }
    if ((*(ushort *)(param_1 + 4) & 4) != 0) {
      return 0;
    }
    if ((uVar3 & 8) == 0) {
      return 0;
    }
    iVar4 = *(int *)(*(int *)(param_1 + 0x38) + 0x18);
    if (iVar4 == 0) {
      return 0;
    }
    if ((*(ushort *)(*(int *)(*(int *)(iVar4 + 0x10) + 0x20) + 4) & 0x40) == 0) {
      return 0;
    }
    iVar4 = *(int *)(*(int *)(iVar4 + 0x10) + 0x20);
    cVar1 = *(char *)(iVar4 + 2);
    if ((*(ushort *)(iVar4 + 4) & 0x1000) != 0) {
      return 0;
    }
    cVar2 = *(char *)(param_1 + 9);
    if ((((cVar2 == '\0') || (cVar2 == '\x02')) || (cVar2 == '\x03')) ||
       ((cVar2 == '\x05' || (cVar2 == '\x06')))) {
      if (cVar1 == '\x04') {
        return 0x200;
      }
      if (cVar1 == '\x05') {
        return 0x400;
      }
      if (cVar1 == '\x06') {
        return 0;
      }
      if (cVar1 == '\a') {
        return 0x800;
      }
      return 0;
    }
    if (cVar2 != '\a') {
      return 0;
    }
    if (cVar1 == '\x04') {
      return 0x200;
    }
    if ((cVar1 != '\x05') && (cVar1 != '\x06')) {
      if (cVar1 == '\a') {
        return 0x800;
      }
      return 0;
    }
  }
  else {
    iVar4 = *(int *)(*(int *)(DAT_8c06c9c0 + (uint)*(byte *)(param_1 + 8) * 4) + 0x68);
    iVar5 = *(int *)(iVar4 + 0x20);
    if (((*(int *)(iVar4 + 0x24) == 0) || (iVar5 == 0)) || (*(int *)(iVar5 + 0x38) == 0)) {
      return 0;
    }
    cVar1 = *(char *)(iVar5 + 2);
    cVar2 = *(char *)(param_1 + 9);
    if (((cVar2 == '\0') || (cVar2 == '\x02')) ||
       ((cVar2 == '\x03' || ((cVar2 == '\x05' || (cVar2 == '\x06')))))) {
      if (cVar1 == '\x04') {
        return 0x200;
      }
      if (cVar1 == '\x05') {
        return 0x400;
      }
      if (cVar1 == '\x06') {
        return 0;
      }
      if (cVar1 == '\a') {
        return 0x800;
      }
      return 0;
    }
    if (cVar2 != '\a') {
      return 0;
    }
    if (cVar1 == '\x04') {
      return 0x200;
    }
    if ((cVar1 != '\x05') && (cVar1 != '\x06')) {
      if (cVar1 == '\a') {
        return 0x800;
      }
      return 0;
    }
  }
  return 0x600;
}

