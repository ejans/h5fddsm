#include "H5FDdsmTest.h"
#include "H5FDdsmTools.h"
#include <hdf5.h>
#include "H5FDdsm.h"
#include <cstdlib>
#include <string.h>

#include "h5tools.h"
#include "h5tools_utils.h"
#include <string.h>
#include <stdlib.h>

//----------------------------------------------------------------------------
int main(int argc, char *argv[]) {

	char fname_src[4] = "dsm";
	char fname_dst[22] = "h5files/youbottest.h5";
	char oname_src[6] = "State";
	char oname_dst[6] = "State";
	char oname_dst2[6];
	//strcat(oname_dst, gname);
	int number = 1;
	char num[4];

	H5FDdsmManager *dsmManager = new H5FDdsmManager();
	MPI_Comm comm = MPI_COMM_WORLD;
	receiverInit(argc, argv, dsmManager, &comm);

	while (dsmManager->WaitForUnlock() != H5FD_DSM_FAIL) {

                sprintf(num, "%d",number);
		printf("%s\n", num);
		strcpy(oname_dst2, oname_dst);
		strcat(oname_dst2, num);
		//read the data from buffer by fapl_dsm
		hid_t fapl = H5Pcreate(H5P_FILE_ACCESS);
		H5Pset_fapl_dsm(fapl, comm, NULL, 0);

		hid_t ocpl_id = (-1); /* Object copy property list */
		hid_t lcpl_id = (-1); /* Link creation property list */

		/*-------------------------------------------------------------------------
		 * open input file
		 *-------------------------------------------------------------------------*/

		//  hid_t   fid_src = h5tools_fopen(fname_src, H5F_ACC_RDONLY, fapl, NULL, NULL, 0);
		hid_t fid_src = H5Fopen(fname_src, H5F_ACC_RDONLY, fapl);

		/*-------------------------------------------------------------------------
		 * open output file
		 *-------------------------------------------------------------------------*/

		/* Attempt to open an existing HDF5 file first */
		//	hid_t  fid_dst = h5tools_fopen(fname_dst, H5F_ACC_RDWR, H5P_DEFAULT, NULL, NULL, 0);
		hid_t fid_dst = H5Fopen(fname_dst, H5F_ACC_RDWR, H5P_DEFAULT);
		/* If we couldn't open an existing file, try creating file */
		/* (use "EXCL" instead of "TRUNC", so we don't blow away existing non-HDF5 file) */
		if (fid_dst < 0)
			fid_dst = H5Fcreate(fname_dst, H5F_ACC_EXCL, H5P_DEFAULT,
					H5P_DEFAULT);

		/*-------------------------------------------------------------------------
		 * create property lists for copy
		 *-------------------------------------------------------------------------*/

		/* create property to pass copy options */
		if ((ocpl_id = H5Pcreate(H5P_OBJECT_COPY)) < 0)
			goto error;

		/* Create link creation property list */
		if ((lcpl_id = H5Pcreate(H5P_LINK_CREATE)) < 0) {
			goto error;
		} /* end if */

		/*-------------------------------------------------------------------------
		 * do the copy
		 *-------------------------------------------------------------------------*/

		if (H5Ocopy(fid_src, /* Source file or group identifier */
		oname_src, /* Name of the source object to be copied */
		fid_dst, /* Destination file or group identifier  */
		oname_dst2, /* Name of the destination object  */
		ocpl_id, /* Object copy property list */
		lcpl_id) < 0) /* Link creation property list */
			goto error;

		/* close propertis */
		if (H5Pclose(fapl) < 0)
			goto error;
		if (H5Pclose(ocpl_id) < 0)
			goto error;
		if (H5Pclose(lcpl_id) < 0)
			goto error;

		/* close files */
		if (H5Fclose(fid_src) < 0)
			goto error;
		if (H5Fclose(fid_dst) < 0)
			goto error;

		error: printf("Error in copy...Exiting\n");
		// Sync here
		MPI_Barrier(comm);
		number++;
	}

	receiverFinalize(dsmManager, &comm);
	delete dsmManager;
	return (EXIT_SUCCESS);

}

