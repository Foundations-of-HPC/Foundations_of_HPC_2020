# A guided tour on Orfeo storage system

A short tour to explore all the levels of ORFEO's storage and start measuring performance. 

## local disks on both login and computing nodes 

The login node is a small VM hosted on Orfeo's OpenStack infrastructure. 
Let us explore storage available:

``` 

[cozzini@login Foundations_of_HPC_2020]$ df -h
Filesystem                                                                 Size  Used Avail Use% Mounted on
devtmpfs                                                                   1.9G     0  1.9G   0% /dev
tmpfs                                                                      1.9G  514M  1.4G  28% /dev/shm
tmpfs                                                                      1.9G   82M  1.8G   5% /run
tmpfs                                                                      1.9G     0  1.9G   0% /sys/fs/cgroup
/dev/vda1                                                                   40G   11G   30G  28% /
10.128.6.211:6789,10.128.6.212:6789,10.128.6.213:6789,10.128.6.214:6789:/   88T  5.8T   82T   7% /fast
10.128.2.231:/storage                                                       37T   19T   19T  51% /storage
10.128.6.211:6789,10.128.6.213:6789,10.128.6.212:6789,10.128.6.214:6789:/  598T   93T  506T  16% /large
10.128.4.201:/opt/area                                                     381G  181G  201G  48% /opt/area
...
```

We noted that the local disk ```/dev/sda1``` is a virtual disks.
Let us now estimate basic performance by means of the simple  `dd ` command 




## CEPH filesystem 


## Long term storage

