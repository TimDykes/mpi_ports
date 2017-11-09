//---------------------------------------------------------------
// MPI Demo
//---------------------------------------------------------------
#include <iostream>
#include <cstring>
#include <mpi.h>
#include <fstream>
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
    std::ofstream ofile("port.txt");
    ofile << port;
    ofile.close();
#endif

    int ret = MPI_Comm_accept(port, MPI_INFO_NULL, 0, MPI_COMM_WORLD, &intercomm);
    if(ret != MPI_SUCCESS){
        char error_string[1024];
        int length_of_error_string;
        MPI_Error_string(ret, error_string, &length_of_error_string);
        printf("%3d: %s\n", rank, error_string);
        MPI_Abort(MPI_COMM_WORLD, 1);
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
    std::ifstream ifile("port.txt");
    std::string p;
    if(ifile.is_open())
    {
        ifile >> p;
        ifile.close();
    }
    if(p.size() > MPI_MAX_PORT_NAME || p.size() == 0){
        std::cout << "Err, port name too long or empty\n";
        MPI_Abort(MPI_COMM_WORLD, 1);
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
	MPI_Abort(MPI_COMM_WORLD, 1);
    } else {
        std::cout << "Client: Connected to port: " << port << std::endl;
    }
#endif

    MPI_Finalize();

    return 0;
}

