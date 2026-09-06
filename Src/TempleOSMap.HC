class CTempleOSDbgInfo
{
  U32 min_line,max_line;
  U32 body[1];
};

CTempleOSDbgInfo *TempleOSMapDebugInfo(CHashFun *fun)
{
  CDbgInfo *info=fun->dbg_info;
  CTempleOSDbgInfo *out;
  I64 count,i,address;

  if (!info)
    return NULL;
  if (MSize(info)<offset(CDbgInfo.body) || info->min_line>info->max_line)
    return NULL;
  count=info->max_line;
  count+=2-info->min_line;
  if (count>(MSize(info)-offset(CDbgInfo.body))/sizeof(U8 *))
    return NULL;
  out=CAlloc(offset(CTempleOSDbgInfo.body)+count*sizeof(U32));
  out->min_line=info->min_line;
  out->max_line=info->max_line;
  for (i=0;i<count;i++) {
    if (info->body[i]) {
      address=fun->exe_addr+info->body[i]-info->body[0];
      if (!(0<=address<=U32_MAX)) {
        Free(out);
        return NULL;
      }
      out->body[i]=address;
    }
  }
  return out;
}

U0 TempleOSMapFileWrite(CHashTable *h,U8 *map_name,U8 drv_let)
{
  CHashSrcSym *tmph;
  I64 i,size;
  U8 *src_link;
  CDoc *doc;
  CDocBin *tmpb;
  CTempleOSDbgInfo *dbg_info;

  doc=DocNew(map_name);
  doc->flags|=DOCF_NO_CURSOR;
  for (i=0;i<=h->mask;i++) {
    tmph=h->body[i];
    while (tmph) {
      if (tmph->src_link && !(tmph->type & (HTF_IMPORT | HTF_PRIVATE))) {
        src_link=StrNew(tmph->src_link);
        if (drv_let && StrLen(src_link)>=4)
          src_link[3]=drv_let;
        dbg_info=NULL;
        if (tmph->type&HTT_FUN && tmph->dbg_info) {
          if (dbg_info=TempleOSMapDebugInfo(tmph)) {
            size=dbg_info->max_line;
            size=offset(CTempleOSDbgInfo.body)+
                  sizeof(U32)*(size+2-dbg_info->min_line);
            tmpb=CAlloc(sizeof(CDocBin));
            tmpb->size=size;
            tmpb->data=dbg_info;
            tmpb->num=doc->cur_bin_num++;
            tmpb->use_cnt=1;
            QueIns(tmpb,doc->bin_head.last);
          } else
            PrintWarn("Unrepresentable TempleOS debug map for %s\n",tmph->str);
        }
        if (dbg_info)
          DocPrint(doc,"$$LK,\"%s\",A=\"%s\",BI=%d$$\n",
                tmph->str,src_link,tmpb->num);
        else
          DocPrint(doc,"$$LK,\"%s\",A=\"%s\"$$\n",tmph->str,src_link);
        Free(src_link);
      }
      tmph=tmph->next;
    }
  }
  DocBinsValidate(doc);
  DocWrite(doc);
  DocDel(doc);
}
