//---------------------------------------------------------------
// MPI Demo
//---------------------------------------------------------------
#include <iostream>
#include <mpi.h>

int main(int argc, char* argv[])
{
    int rank, size;
    MPI_Init(&argc, &argv);

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

#ifdef SERVER
    std::cout << "Server rank: " << rank << " of " << size << " ranks" << std::endl;
    
    char port[MPI_MAX_PORT_NAME];
    MPI_Comm intercomm;
    
    MPI_Open_port(MPI_INFO_NULL, port);

#ifdef PUBLISH_NAME
    MPI_Publish_name("my_service_name", MPI_INFO_NULL, port);
#else    
    printf("%s\n",port);
#endif

    int ret = MPI_Comm_accept(port, MPI_INFO_NULL, 0, MPI_COMM_WORLD, &intercomm);
    if(ret != MPI_SUCCESS){
        char error_string[1024];
        int length_of_error_string;

        MPI_Error_string(ret, error_string, &length_of_error_string);
        printf("%3d: %s\n", rank, error_string);
    } else {
        std::cout << "Server: Accepted comms on port: " << port << std::endl;
    }

    MPI_Close_port(port);
#endif

#ifdef CLIENT
    std::cout << "Client rank: " << rank << " of " << size << " processes" << std::endl;
    char port[MPI_MAX_PORT_NAME];
    MPI_Comm intercomm;

#ifdef PUBLISH_NAME
    MPI_Lookup_name("my_service_name", MPI_INFO_NULL, port);
#else
    std::cout << "Enter port:\n";
    std::string p;    
    std::cin >> p;
    if(p.size() > MPI_MAX_PORT_NAME){
        std::cout << "Err, port name too long\n";
        MPI_Finalize();
        return 0;
    } else {
        strcpy(port, p.c_str());
    } 
 #endif
    
    int ret = MPI_Comm_connect(port, MPI_INFO_NULL, 0, MPI_COMM_WORLD, &intercomm);
    if(ret != MPI_SUCCESS){
        char error_string[1024];
        int length_of_error_string;

        MPI_Error_string(ret, error_string, &length_of_error_string);
        printf("%3d: %s\n", rank, error_string);
    } else {
        std::cout << "Client connected to port: " << port;
    }
#endif

    MPI_Finalize();

    return 0;
}

