/* Ghidra draft only; not buildable/verified source. */

undefined4 FUN_8c660dac(int param_1)

{
  char *pcVar1;
  uint *puVar2;
  undefined4 uVar3;
  
  uVar3 = *(undefined4 *)(PTR_DAT_8c660e3c + param_1 * 4);
  pcVar1 = (char *)(*(code *)PTR_FUN_8c660e40)(uVar3);
  puVar2 = (uint *)(*(code *)PTR_FUN_8c660e44)(uVar3);
  if ((*pcVar1 != '\0') && ((*puVar2 & 2) != 0)) {
    return 1;
  }
  return 0;
}

