/* Ghidra draft only; not buildable/verified source. */

undefined4 FUN_8c660dec(int param_1)

{
  char *pcVar1;
  uint *puVar2;
  int iVar3;
  undefined4 uVar4;
  
  uVar4 = *(undefined4 *)(PTR_DAT_8c660e3c + param_1 * 4);
  pcVar1 = (char *)(*(code *)PTR_FUN_8c660e40)(uVar4);
  puVar2 = (uint *)(*(code *)PTR_FUN_8c660e44)(uVar4);
  if (((*pcVar1 != '\0') && (iVar3 = (*(code *)PTR_thunk_FUN_8c65f40e_8c660e48)(uVar4), iVar3 == 0))
     && ((*puVar2 & 2) != 0)) {
    return 1;
  }
  return 0;
}

