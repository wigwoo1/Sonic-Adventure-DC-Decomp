/* Ghidra draft only; not buildable/verified source. */

void FUN_8c64c320(char *param_1)

{
  undefined *puVar1;
  char cVar2;
  
  puVar1 = PTR_FUN_8c64c348;
  for (; *param_1 != '\0'; param_1 = param_1 + 1) {
    cVar2 = (*(code *)puVar1)((int)*param_1);
    *param_1 = cVar2;
  }
  *param_1 = '\0';
  return;
}

