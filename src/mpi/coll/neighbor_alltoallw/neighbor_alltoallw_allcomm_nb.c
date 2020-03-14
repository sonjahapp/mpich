/*
 * Copyright (C) by Argonne National Laboratory
 *     See COPYRIGHT in top-level directory
 */

#include "mpiimpl.h"

int MPIR_Neighbor_alltoallw_allcomm_nb(const void *sendbuf, const int sendcounts[],
                                       const MPI_Aint sdispls[], const MPI_Datatype sendtypes[],
                                       void *recvbuf, const int recvcounts[],
                                       const MPI_Aint rdispls[], const MPI_Datatype recvtypes[],
                                       MPIR_Comm * comm_ptr)
{
    int mpi_errno = MPI_SUCCESS;
    MPI_Request req = MPI_REQUEST_NULL;
    MPIR_Request *req_ptr = NULL;

    /* just call the nonblocking version and wait on it */
    mpi_errno =
        MPIR_Ineighbor_alltoallw(sendbuf, sendcounts, sdispls, sendtypes, recvbuf, recvcounts,
                                 rdispls, recvtypes, comm_ptr, &req_ptr);
    MPIR_ERR_CHECK(mpi_errno);
    if (req_ptr)
        req = req_ptr->handle;

    mpi_errno = MPIR_Wait(&req, MPI_STATUS_IGNORE);
    MPIR_ERR_CHECK(mpi_errno);

  fn_exit:
    return mpi_errno;
  fn_fail:
    goto fn_exit;
}
