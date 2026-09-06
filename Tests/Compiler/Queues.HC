public _intern IC_QUE_INIT U0 TestInit(CQue *q);
public _intern IC_QUE_INS U0 TestInsert(CQue *a,CQue *b);
public _intern IC_QUE_INS_REV U0 TestInsertRev(CQue *a,CQue *b);
public _intern IC_QUE_REM U0 TestRemove(CQue *q);
CQue head,one,two;
CQue *TestOne() {return &one;}
CQue *TestHead() {return &head;}
TestInit(&head);
TestInsert(TestOne(),TestHead());
if (head.next!=&one || one.last!=&head ||
      one.next!=&head || head.last!=&one) throw('QueueIns');
TestInsertRev(&two,TestHead());
if (head.last!=&two || two.next!=&head ||
      two.last!=&one || one.next!=&two) throw('QueueRev');
TestRemove(TestOne());
if (head.next!=&two || two.last!=&head) throw('QueueRem');
TestRemove(&two);
if (head.next!=&head || head.last!=&head) throw('QueueEnd');
"PASS: queues\n";
Exit;
