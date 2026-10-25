/* Ghidra draft only; not buildable/verified source. */

int FUN_8c643e6c(int param_1)

{
  undefined *puVar1;
  int iVar2;
  int *piVar3;
  
  iVar2 = DAT_8c643f24;
  if ((((param_1 != 0) &&
       (piVar3 = *(int **)(param_1 + 8), iVar2 = DAT_8c643f2c, *piVar3 == DAT_8c643f28)) &&
      (iVar2 = DAT_8c643f30, (char)piVar3[1] == '\x01')) && (*(char *)((int)piVar3 + 5) < '\x02')) {
    (*(code *)PTR_FUN_8c643f34)();
    (*(code *)PTR_FUN_8c643f38)();
    (*(code *)PTR_FUN_8c643f40)(PTR_DAT_8c643f3c);
    (*(code *)PTR_FUN_8c643f44)(*(undefined4 *)PTR_DAT_8c643f3c,piVar3 + 8,piVar3[2],0xffffffff,0);
    (*(code *)PTR_FUN_8c643f48)(*(undefined4 *)PTR_DAT_8c643f3c,0);
    (*(code *)PTR_FUN_8c643f4c)();
    puVar1 = PTR_FUN_8c643f58;
    for (iVar2 = DAT_8c643f50; iVar2 != 0; iVar2 = iVar2 + -1) {
    }
    *(undefined4 *)PTR_DAT_8c643f54 = 1;
    (*(code *)puVar1)();
    (*(code *)PTR_FUN_8c643f5c)(param_1);
    iVar2 = 0;
  }
  if (*(int *)PTR_DAT_8c643f3c != 0) {
    (*(code *)PTR_FUN_8c643f60)(*(undefined4 *)PTR_DAT_8c643f3c);
  }
  if (iVar2 != 0) {
    (*(code *)PTR_FUN_8c643f68)(PTR_FUN_8c643f64,iVar2);
  }
  return iVar2;
}

