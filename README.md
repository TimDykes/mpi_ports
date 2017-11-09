# Testing MPI Ports API for Dynamic Process Management (DPM)
-------
## Local machine / OpenMPI

Default build is for Cray MPICH, swap environment option in Makefile to build for OpenMPI.

### Without a nameserver

Default build is without a nameserver, the server passes the MPI port name by writing to a file (port.txt) and the client reads from this. To test:

    make 
    mpirun -np 1 ./server &
    mpirun -np 1 ./client

### With a name server: 

To use an intermediary service for name publishing, uncomment the NAMESERVER option in the Makefile, make clean && make;

You need to manually start a name service to handle the name publishing. In OpenMPI you can do this via:

mpirun -np 1  ompi-server --no-daemonize -r + &

Then get the process number via ps, e.g: 

    > ps
    < PID TTY          TIME CMD
      1 pts/0    00:00:00 bash
      43 pts/0    00:00:00 mpirun
      44 pts/0    00:00:00 ompi-server
      50 pts/0    00:00:00 ps 

Run server and client passing the ompi-server process id like so:

    mpirun -np 1 --ompi-server pid:43 ./server &
    mpirun -np 1 --ompi-server pid:43 ./client

------
## Cray XC / CRAY-MPICH
### Without a nameserver

Disable the mpicxx compiler option in the Makefile, enable the Cray compiler option. This uses the CC wrapper with an additional flag (-craympich-dpm) to ensure CC using the DPM enabled version of cray-mpich. Make clean && make.

#### Additional setup for Cray:

Grab a 2 node interactive job, e.g:

    qsub -I -l walltime=01:00:00,nodes=2

You must set up a protection domain and run each application in this domain (user pdomains must be enabled on the system...):

    > apmgr pdomain -c my_pdomain
    > apstat -P
    < PDomainID Type    Uid     Cookie    Cookie2
      my_pdomain user 210490 0xa92c0000 0xa92d0000

**After you are finished, remember to close the domain again:**

    apmgr pdomain -r my_pdomain 

Also you should export a directory for MPICH to use for DPM (that should be mounted for both applications):

    export MPICH_DPM_DIR=/home/users/tdykes/

Default build is without a nameserver, the server passes the MPI port name by writing to a file (port.txt) and the client reads from this. 

Test:

    > aprun -n 1 -p my_pdomain ./server &
    < Server rank: 0 of 1 ranks
    // If running interactively its worth checking port.txt has appeared before running client
    > aprun -n 1 -p my_pdomain ./client
    < Client rank: 0 of 1 processes
    < Server: Accepted comms on port: tag#0$nic_id#0100000000000000000000006D00000000000000$cdm_id#23899$
    < Client connected to port: tag#0$nic_id#0100000000000000000000006D00000000000000$cdm_id#23899$

### With a name server: 

Untested, but could use hydra name server? https://wiki.mpich.org/mpich/index.php/Using_the_Hydra_Process_Manager#Name_Publishing
