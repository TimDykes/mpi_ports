## Testing MPI Ports API

### Without a nameserver

To avoid useing a name server and pass the port from client to server via intermediary text file, comment out –DPUBLISH_NAME in Makefile, build, and launch like so:

mpirun -np 1 ./server &

mpirun -np 1 ./client

### To use a name server: 

Start a name service to handle name publishing. In OpenMPI you can do this via:

mpirun -np 1  ompi-server --no-daemonize -r + &

Then get the process number via ps, e.g:
ps
  PID TTY          TIME CMD
    1 pts/0    00:00:00 bash
   43 pts/0    00:00:00 mpirun
   44 pts/0    00:00:00 ompi-server
   50 pts/0    00:00:00 ps 

Run server

mpirun -np 1 --ompi-server pid:43 ./server

Run client: 

mpirun -np 1 --ompi-server pid:43 ./client


----- Notes

MVAPICH could use hydra name server? https://wiki.mpich.org/mpich/index.php/Using_the_Hydra_Process_Manager#Name_Publishing
