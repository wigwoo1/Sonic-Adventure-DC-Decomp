/* Ghidra draft only; not buildable/verified source. */

/* WARNING: Removing unreachable block (ram,0x8c65e912) */
/* WARNING: Removing unreachable block (ram,0x8c65e90e) */
/* WARNING: Removing unreachable block (ram,0x8c65e90a) */
/* WARNING: Removing unreachable block (ram,0x8c65e906) */
/* WARNING: Removing unreachable block (ram,0x8c65e902) */
/* WARNING: Removing unreachable block (ram,0x8c65e8fe) */
/* WARNING: Removing unreachable block (ram,0x8c65e8fa) */
/* WARNING: Removing unreachable block (ram,0x8c65e8f6) */
/* WARNING: Removing unreachable block (ram,0x8c65e8ee) */
/* WARNING: Removing unreachable block (ram,0x8c65e8ea) */
/* WARNING: Removing unreachable block (ram,0x8c65e8e6) */
/* WARNING: Removing unreachable block (ram,0x8c65e8e2) */
/* WARNING: Removing unreachable block (ram,0x8c65e8de) */
/* WARNING: Removing unreachable block (ram,0x8c65e8da) */
/* WARNING: Removing unreachable block (ram,0x8c65e8d6) */
/* WARNING: Removing unreachable block (ram,0x8c65e88c) */
/* WARNING: Removing unreachable block (ram,0x8c65e888) */
/* WARNING: Removing unreachable block (ram,0x8c65e884) */
/* WARNING: Removing unreachable block (ram,0x8c65e880) */
/* WARNING: Removing unreachable block (ram,0x8c65e87c) */
/* WARNING: Removing unreachable block (ram,0x8c65e878) */
/* WARNING: Removing unreachable block (ram,0x8c65e874) */
/* WARNING: Removing unreachable block (ram,0x8c65e870) */
/* WARNING: Removing unreachable block (ram,0x8c65e86a) */
/* WARNING: Removing unreachable block (ram,0x8c65e868) */
/* WARNING: Removing unreachable block (ram,0x8c65e864) */
/* WARNING: Removing unreachable block (ram,0x8c65e860) */
/* WARNING: Removing unreachable block (ram,0x8c65e85c) */
/* WARNING: Removing unreachable block (ram,0x8c65e858) */
/* WARNING: Removing unreachable block (ram,0x8c65e854) */
/* WARNING: Removing unreachable block (ram,0x8c65e850) */
/* WARNING: Removing unreachable block (ram,0x8c65e84c) */
/* WARNING: Removing unreachable block (ram,0x8c65e84e) */
/* WARNING: Removing unreachable block (ram,0x8c65e852) */
/* WARNING: Removing unreachable block (ram,0x8c65e856) */
/* WARNING: Removing unreachable block (ram,0x8c65e85a) */
/* WARNING: Removing unreachable block (ram,0x8c65e85e) */
/* WARNING: Removing unreachable block (ram,0x8c65e862) */
/* WARNING: Removing unreachable block (ram,0x8c65e866) */
/* WARNING: Removing unreachable block (ram,0x8c65e86e) */
/* WARNING: Removing unreachable block (ram,0x8c65e872) */
/* WARNING: Removing unreachable block (ram,0x8c65e876) */
/* WARNING: Removing unreachable block (ram,0x8c65e87a) */
/* WARNING: Removing unreachable block (ram,0x8c65e87e) */
/* WARNING: Removing unreachable block (ram,0x8c65e882) */
/* WARNING: Removing unreachable block (ram,0x8c65e886) */
/* WARNING: Removing unreachable block (ram,0x8c65e88a) */
/* WARNING: Removing unreachable block (ram,0x8c65e8d4) */
/* WARNING: Removing unreachable block (ram,0x8c65e8d8) */
/* WARNING: Removing unreachable block (ram,0x8c65e8dc) */
/* WARNING: Removing unreachable block (ram,0x8c65e8e0) */
/* WARNING: Removing unreachable block (ram,0x8c65e8e4) */
/* WARNING: Removing unreachable block (ram,0x8c65e8e8) */
/* WARNING: Removing unreachable block (ram,0x8c65e8ec) */
/* WARNING: Removing unreachable block (ram,0x8c65e8f0) */
/* WARNING: Removing unreachable block (ram,0x8c65e8f2) */
/* WARNING: Removing unreachable block (ram,0x8c65e8f8) */
/* WARNING: Removing unreachable block (ram,0x8c65e8fc) */
/* WARNING: Removing unreachable block (ram,0x8c65e900) */
/* WARNING: Removing unreachable block (ram,0x8c65e904) */
/* WARNING: Removing unreachable block (ram,0x8c65e908) */
/* WARNING: Removing unreachable block (ram,0x8c65e90c) */
/* WARNING: Removing unreachable block (ram,0x8c65e910) */
/* WARNING: Removing unreachable block (ram,0x8c65e914) */
/* WARNING: Restarted to delay deadcode elimination for space: stack */

undefined8 callback_8c65e80c(undefined4 param_1)

{
  undefined4 in_r0;
  code *pcVar1;
  int in_VBR;
  
  pcVar1 = *(code **)(in_VBR + (*(int *)PTR_DAT_8c65e954 + -0x200 >> 3) + 0x200);
  if (pcVar1 != (code *)0x0) {
    (*pcVar1)();
  }
  return CONCAT44(param_1,in_r0);
}

