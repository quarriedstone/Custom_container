# Custom_container
Container created for the course TotalVirtualization2019

Steps to do:
1) Create directory Container in /root
2) Create directory Mnt in /root/Container
3) Create empty file check.txt in /root/Container/Mnt/check.txt
4) Compile with: gcc cont.c -o cont.out
5) Run ./cont.out
6) See that container works


Procedure of *sysbench* performance testing:

1) start the container ./cont.out
2) **CPU testig:** sysbench --test=cpu --cpu-max-prime=20000 run
3) **File I/O write:** 
sysbench --num-threads=16 --test=fileio --file-total-size=3G --file-test-mode=rndwr prepare

sysbench --num-threads=16 --test=fileio --file-total-size=3G --file-test-mode=rndwr run

sysbench --num-threads=16 --test=fileio --file-total-size=3G --file-test-mode=rndwr cleanup

4) **File I/O write:**
sysbench --num-threads=16 --test=fileio --file-total-size=10G --file-test-mode=rndrd prepare

sysbench --num-threads=16 --test=fileio --file-total-size=10G --file-test-mode=rndrd run

sysbench --num-threads=16 --test=fileio --file-total-size=10G --file-test-mode=rndrd cleanup

5) **Memory test:**
sysbench --test=memory --memory-total-size=10G run

6) **Thread test:**
sysbench --num-threads=64 --test=threads --thread-yields=1000 --thread-locks=8 run
