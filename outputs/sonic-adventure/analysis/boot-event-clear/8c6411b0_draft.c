/* Ghidra draft only; not buildable/verified source. */

void FUN_8c6411b0(int param_1,int param_2)

{
  int iVar1;
  undefined *puVar2;
  
  if (param_2 == 0) {
    *(undefined **)(DAT_8c641280 + param_1) = PTR_LAB_8c64128c;
    return;
  }
  if (param_2 == 1) {
    iVar1 = (int)DAT_8c641282;
    puVar2 = PTR_LAB_8c641290;
LAB_8c6412b4:
    *(undefined **)(iVar1 + param_1) = puVar2;
  }
  else {
    if (param_2 == 2) {
      iVar1 = (int)DAT_8c641284;
      puVar2 = PTR_LAB_8c641294;
    }
    else {
      if (param_2 == 3) {
        iVar1 = (int)DAT_8c641286;
        puVar2 = PTR_LAB_8c641298;
        goto LAB_8c6412b4;
      }
      if (param_2 == 4) {
        iVar1 = (int)DAT_8c641288;
        puVar2 = PTR_LAB_8c64129c;
      }
      else {
        if (param_2 == 5) {
          iVar1 = (int)DAT_8c641300;
          puVar2 = PTR_LAB_8c64130c;
          goto LAB_8c6412b4;
        }
        if (param_2 == 6) {
          iVar1 = (int)DAT_8c641302;
          puVar2 = PTR_LAB_8c641310;
        }
        else {
          if (param_2 == 7) {
            iVar1 = (int)DAT_8c641304;
            puVar2 = PTR_LAB_8c641314;
            goto LAB_8c6412b4;
          }
          if (param_2 != 8) {
            return;
          }
          iVar1 = (int)DAT_8c641306;
          puVar2 = PTR_DAT_8c641318;
        }
      }
    }
    *(undefined **)(iVar1 + param_1) = puVar2;
  }
  return;
}

