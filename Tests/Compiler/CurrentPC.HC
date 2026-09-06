U8 *CurrentPCDirect()
{
  return $$;
}

U8 *CurrentPCStored()
{
  U8 *pc=$$;
  return pc;
}

U8 *CurrentPCOffset()
{
  return $$+3;
}

U0 CheckCurrentPC(U8 *pc,U8 *start)
{
  if (pc<=start || pc>=start+4096)
    throw('CurPC');
}

CheckCurrentPC(CurrentPCDirect(),&CurrentPCDirect);
CheckCurrentPC(CurrentPCStored(),&CurrentPCStored);
CheckCurrentPC(CurrentPCOffset()-3,&CurrentPCOffset);
U8 *direct1=CurrentPCDirect(),*direct2=CurrentPCDirect();
U8 *stored1=CurrentPCStored(),*stored2=CurrentPCStored();
if (direct1!=direct2 || stored1!=stored2)
  throw('CurPC');
"PASS: CurrentPC\n";
Exit;
