/* Ghidra draft only; not buildable/verified source. */

char * FUN_8c10d3a8(char *param_1,char *param_2)

{
  char cVar1;
  char *pcVar2;
  
  for (pcVar2 = param_1; *pcVar2 != '\0'; pcVar2 = pcVar2 + 1) {
  }
  do {
    cVar1 = *param_2;
    param_2 = param_2 + 1;
    *pcVar2 = cVar1;
    pcVar2 = pcVar2 + 1;
  } while (cVar1 != '\0');
  return param_1;
}

