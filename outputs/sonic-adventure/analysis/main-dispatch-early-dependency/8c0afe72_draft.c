/* Ghidra draft only; not buildable/verified source. */

undefined4 FUN_8c0afe72(int param_1)

{
  char cVar1;
  int iVar2;
  
  iVar2 = *(int *)(param_1 + 0x3c);
  cVar1 = *(char *)(iVar2 + 0x24);
  if ((((cVar1 == '\x02') || (cVar1 == '\x03')) || (cVar1 == '\x04')) || (cVar1 == '\x01')) {
    *(ushort *)(param_1 + 4) = *(ushort *)(param_1 + 4) | 0x4000;
  }
  else {
    if (cVar1 == '\x06') {
      iVar2 = *(int *)(iVar2 + 0x2c);
    }
    else {
      if (cVar1 != '\x05') {
        *(ushort *)(param_1 + 4) = *(ushort *)(param_1 + 4) & 0xbfff;
        return 0;
      }
      iVar2 = (*(int *)(iVar2 + 0x2c) - *(int *)(param_1 + 0x18)) + 0x4000;
    }
    (*(code *)PTR_FUN_8c0afef8)((int)*(char *)(param_1 + 8),iVar2);
  }
  return 1;
}

