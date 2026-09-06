#include "/Src/TempleOSMap.HC"
CHashFun *fun=CAlloc(sizeof(CHashFun));
CDbgInfo *info=CAlloc(offset(CDbgInfo.body)+4*sizeof(U8 *));
info->min_line=10;
info->max_line=12;
info->body[0]=0x100000000;
info->body[1]=0x100000010;
info->body[2]=NULL;
info->body[3]=0x100000030;
fun->exe_addr=0x120;
fun->dbg_info=info;
CTempleOSDbgInfo *out=TempleOSMapDebugInfo(fun);
if (!out || out->min_line!=10 || out->max_line!=12 ||
      out->body[0]!=0x120 || out->body[1]!=0x130 ||
      out->body[2]!=0 || out->body[3]!=0x150) throw('MapAddr');
Free(out);
info->max_line=100;
if (TempleOSMapDebugInfo(fun)) throw('MapCount');
info->max_line=9;
if (TempleOSMapDebugInfo(fun)) throw('MapRange');
info->max_line=12;
fun->exe_addr=0x100000000;
if (TempleOSMapDebugInfo(fun)) throw('MapWidth');
Free(info);
Free(fun);
"PASS: TempleOS map\n";
Exit;
