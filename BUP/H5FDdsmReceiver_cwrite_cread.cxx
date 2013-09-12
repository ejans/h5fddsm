#include "H5FDdsmTest.h"
#include "H5FDdsmTools.h"
#include <hdf5.h>
#include "H5FDdsm.h"
#include <cstdlib>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//#include <sstream>
#include <iostream>

//----------------------------------------------------------------------------
int main(int argc, char *argv[]) {

	argc = 1;
	//char* fname = argv[1];
	char fullname[9] = "h5files/";
	//std::cout << argc << std::endl;
	//std::cout << argv[1] << std::endl;
        int number = 1;
        //char* num;
        char num[30];
        char num2[30];
        char name[30];
	//strcat(fullname, fname);

	//std::cout << fullname << std::endl;
	//std::cout << *fullname << std::endl;
	H5FDdsmManager *dsmManager = new H5FDdsmManager();
	MPI_Comm comm = MPI_COMM_WORLD;
	receiverInit(argc, argv, dsmManager, &comm);

	while (dsmManager->WaitForUnlock() != H5FD_DSM_FAIL) {
		//H5FD_dsm_dump();
                //num = "";
                sprintf(num, "%d",number);
                printf("%s\n", num);
                strcpy(num2, num);
                strcat(num2 ,".h5");
                strcpy(name, fullname);
                strcat(name, num2);
                H5FD_dsm_lock();

		//read the data from buffer by fapl_dsm
		hid_t fapl = H5Pcreate(H5P_FILE_ACCESS);
		H5Pset_fapl_dsm(fapl, comm, NULL, 0);
		hid_t hdf5Handle = H5Fopen("dsm", H5F_ACC_RDONLY, fapl);

		H5Pclose(fapl);

		hid_t ocpl = H5Pcreate(H5P_OBJECT_COPY);
		//hid_t desid = H5Fcreate("h5files/aaa.h5", H5F_ACC_TRUNC, H5P_DEFAULT,
				//H5P_DEFAULT);
		hid_t desid = H5Fcreate(name, H5F_ACC_TRUNC, H5P_DEFAULT,
				H5P_DEFAULT);
		hid_t lcpl = H5Pcreate(H5P_LINK_CREATE);
		H5Pset_create_intermediate_group(lcpl,true);
		// Close property list
		//H5Ocopy(hdf5Handle, "State/ArmJointPosition/arm_jnt_pos", desid, "abc/www", ocpl, lcpl);

		//H5Ocopy(hdf5Handle, "State/Timestamp/timestamp", desid, "State/Timestamp/timestamp", ocpl, lcpl);
		H5Ocopy(hdf5Handle, "State/TimeStamp", desid, "State/TimeStamp", ocpl, lcpl);
		//H5Ocopy(hdf5Handle, "State/ArmJointPosition/arm_jnt_pos", desid, "State/ArmJointPosition/arm_jnt_pos", ocpl, lcpl);
		H5Ocopy(hdf5Handle, "State/BaseCartesianPosition/Vector/x", desid, "State/BaseCartesianPosition/Vector/x", ocpl, lcpl);
		H5Ocopy(hdf5Handle, "State/BaseCartesianPosition/Vector/y", desid, "State/BaseCartesianPosition/Vector/y", ocpl, lcpl);
		H5Ocopy(hdf5Handle, "State/BaseCartesianPosition/Vector/z", desid, "State/BaseCartesianPosition/Vector/z", ocpl, lcpl);
		H5Ocopy(hdf5Handle, "State/BaseCartesianPosition/Rotation/rotation", desid, "State/BaseCartesianPosition/Rotation/rotation", ocpl, lcpl);

		H5Ocopy(hdf5Handle, "State/Twist/LinearVelocity/x", desid, "State/Twist/LinearVelocity/x", ocpl, lcpl);
		H5Ocopy(hdf5Handle, "State/Twist/LinearVelocity/y", desid, "State/Twist/LinearVelocity/y", ocpl, lcpl);
		H5Ocopy(hdf5Handle, "State/Twist/LinearVelocity/z", desid, "State/Twist/LinearVelocity/z", ocpl, lcpl);

		H5Ocopy(hdf5Handle, "State/Twist/RotationalVelocity/x", desid, "State/Twist/RotationalVelocity/x", ocpl, lcpl);
		H5Ocopy(hdf5Handle, "State/Twist/RotationalVelocity/y", desid, "State/Twist/RotationalVelocity/y", ocpl, lcpl);
		H5Ocopy(hdf5Handle, "State/Twist/RotationalVelocity/z", desid, "State/Twist/RotationalVelocity/z", ocpl, lcpl);

		H5Pclose(ocpl);
		H5Fclose(hdf5Handle);
		H5Fclose(desid);
                std::cout << "File dumped!" << std::endl;
                number++;

		// Sync here
		//MPI_Barrier(comm);
                H5FD_dsm_unlock(H5FD_DSM_NOTIFY_NONE);
	}

	receiverFinalize(dsmManager, &comm);
	delete dsmManager;
	return (EXIT_SUCCESS);

}
