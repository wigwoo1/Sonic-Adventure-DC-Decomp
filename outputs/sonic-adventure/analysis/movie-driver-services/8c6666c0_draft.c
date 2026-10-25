/* Ghidra draft only; not buildable/verified source. */

void FUN_8c6666c0(void)

{
  uint *puVar1;
  uint *puVar2;
  
  (*(code *)PTR_FUN_8c666880)(8,1);
  (*(code *)PTR_FUN_8c666880)(8,0);
  (*(code *)PTR_FUN_8c666884)();
  puVar1 = DAT_8c66688c;
  *DAT_8c66688c = DAT_8c666888;
  puVar2 = DAT_8c666894;
  *DAT_8c666894 = DAT_8c666890;
  puVar1[3] = *puVar2;
  puVar2[7] = *DAT_8c66688c >> 1;
  puVar2[0xf] = 0;
  *DAT_8c666898 = 1;
  return;
}

