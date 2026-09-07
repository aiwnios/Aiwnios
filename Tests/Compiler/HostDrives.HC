I64 drive;
for (drive='A';drive<='Z';drive++)
  if (!!Let2Drv(drive,FALSE)!=VFsDriveMounted(drive))
    throw('HostDrv');
if (!VFsDriveMounted('T') || !VFsDriveMounted('t') ||
    VFsDriveMounted(0) || VFsDriveMounted('?'))
  throw('HostDrv');
"PASS: HostDrives\n";
Exit;
