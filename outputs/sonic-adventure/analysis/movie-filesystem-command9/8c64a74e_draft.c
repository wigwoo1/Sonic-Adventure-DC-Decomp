/* Ghidra draft only; not buildable/verified source. */

char * FUN_8c64a74e(int param_1,uint param_2)

{
  int *piVar1;
  uint uVar2;
  int iVar3;
  int iVar4;
  char *pcVar5;
  
  piVar1 = DAT_8c64a774;
  iVar3 = param_1 * DAT_8c64a76e + DAT_8c64a770;
  pcVar5 = *(char **)(iVar3 + 0x28);
  iVar4 = 0;
  while( true ) {
    if (*(int *)(*(int *)(iVar3 + 0x1c) + 0x30) <= iVar4) {
      return (char *)0x0;
    }
    if ((*pcVar5 != '\0') &&
       (uVar2 = (**(code **)(*piVar1 + 0xc))(pcVar5,2), (uVar2 & 0xffff) == param_2)) break;
    pcVar5 = pcVar5 + 0x20;
    iVar4 = iVar4 + 1;
  }
  return pcVar5;
}

