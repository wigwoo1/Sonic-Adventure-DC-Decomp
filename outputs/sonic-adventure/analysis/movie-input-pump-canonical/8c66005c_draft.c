/* Ghidra draft only; not buildable/verified source. */

undefined4 FUN_8c66005c(byte param_1,byte *param_2,int param_3,char *param_4,uint *param_5)

{
  int *piVar1;
  char cVar2;
  int iVar3;
  
  FUN_8c65ff5a(param_4,param_5,0);
  param_4[2] = '\0';
  param_4[3] = '\0';
  if ((param_2[2] & param_1) != 0) {
    param_4[4] = '\x01';
    *param_4 = '\0';
    *(int *)(param_4 + 0x24) = *(int *)(param_4 + 0x24) + 1;
  }
  if ((param_2[3] & param_1) != 0) {
    param_4[4] = '\x04';
    *param_4 = '\0';
    param_4[3] = '\x01';
    param_4[0x24] = '\0';
    param_4[0x25] = '\0';
    param_4[0x26] = '\0';
    param_4[0x27] = '\0';
  }
  piVar1 = DAT_8c660120;
  cVar2 = (char)DAT_8c66011c;
  if ((*param_2 & param_1) == 0) {
    param_4[4] = cVar2;
    *param_4 = '\0';
    goto LAB_8c66012a;
  }
  if (param_4[7] == '\0') {
    cVar2 = *param_4;
joined_r0x8c660114:
    if (cVar2 == '\0') goto LAB_8c66012a;
  }
  else {
    if (param_4[7] != '\x01') {
      if (param_4[7] == '\x04') {
        param_4[3] = '\x01';
        goto LAB_8c660106;
      }
      cVar2 = param_4[7];
      goto joined_r0x8c660114;
    }
    if (param_4[8] != '\x05') {
      iVar3 = (int)DAT_8c66011e;
      *(int *)(param_4 + 0x24) = *(int *)(param_4 + 0x24) + 1;
      if (*(uint *)(param_4 + 0x24) <= *(uint *)(iVar3 + *piVar1)) {
        param_4[4] = '\x01';
        goto LAB_8c66012a;
      }
LAB_8c660106:
      *param_4 = '\0';
      param_4[4] = cVar2;
      param_4[0x24] = '\0';
      param_4[0x25] = '\0';
      param_4[0x26] = '\0';
      param_4[0x27] = '\0';
      goto LAB_8c66012a;
    }
    param_4[2] = '\x01';
    *param_4 = '\x01';
  }
  param_4[4] = '\0';
LAB_8c66012a:
  if ((((*param_4 != '\0') && (param_4[7] != '\0')) &&
      (*(int *)(*DAT_8c66022c + (int)DAT_8c660228) == 1)) &&
     ((*param_5 != 0xffffffff && (*param_5 != (int)DAT_8c66022a)))) {
    (*(code *)PTR_FUN_8c660230)(param_5,(*param_5 & 0xff) + 1);
  }
  if (param_4[2] != '\0') {
    *(undefined1 *)(param_3 + 0x10) = 0;
    FUN_8c66018e(param_3,param_4,param_5);
  }
  if (param_4[3] != '\0') {
    *(undefined1 *)(param_3 + 0x10) = 0;
    FUN_8c6602d2(param_3);
  }
  return 0;
}

