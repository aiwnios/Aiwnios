U8 *name="/Src/KernelA.HH";
CFile *file=FOpen(name,"r");
if (!file)
  throw('FileSize');
I64 size=FSize(file);
FClose(file);
if (size<=0 || size!=VFsFSize(name) || FSize(NULL)!=0)
  throw('FileSize');
"PASS: FileSize\n";
Exit;
