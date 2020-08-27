/*
 * Copyright (C) by Argonne National Laboratory
 *     See COPYRIGHT in top-level directory
 */

#include "mpiimpl.h"
#include "datatype.h"
#include "dataloop.h"
#include "mpir_typerep.h"

int MPIR_Typerep_create_vector(int count, int blocklength, int stride, MPI_Datatype oldtype,
                               MPIR_Datatype * newtype)
{
    int old_is_contig;
    MPI_Aint old_extent;

    if (HANDLE_IS_BUILTIN(oldtype)) {
        newtype->typerep.num_contig_blocks = count;
        old_is_contig = 1;
        old_extent = (MPI_Aint) MPIR_Datatype_get_basic_size(oldtype);
    } else {
        MPIR_Datatype *old_dtp;
        MPIR_Datatype_get_ptr(oldtype, old_dtp);
        newtype->typerep.num_contig_blocks =
            old_dtp->typerep.num_contig_blocks * count * blocklength;

        MPIR_Datatype_is_contig(oldtype, &old_is_contig);
        old_extent = old_dtp->extent;
    }

    if (old_is_contig && stride * old_extent == old_extent * blocklength) {
        newtype->typerep.num_contig_blocks = 1;
    }

    return MPI_SUCCESS;
}

int MPIR_Typerep_create_hvector(int count, int blocklength, MPI_Aint stride, MPI_Datatype oldtype,
                                MPIR_Datatype * newtype)
{
    int old_is_contig;
    MPI_Aint old_extent;

    if (HANDLE_IS_BUILTIN(oldtype)) {
        newtype->typerep.num_contig_blocks = count;
        old_is_contig = 1;
        old_extent = (MPI_Aint) MPIR_Datatype_get_basic_size(oldtype);
    } else {
        MPIR_Datatype *old_dtp;
        MPIR_Datatype_get_ptr(oldtype, old_dtp);
        newtype->typerep.num_contig_blocks =
            old_dtp->typerep.num_contig_blocks * count * blocklength;

        MPIR_Datatype_is_contig(oldtype, &old_is_contig);
        old_extent = old_dtp->extent;
    }

    if (old_is_contig && stride == old_extent * blocklength) {
        newtype->typerep.num_contig_blocks = 1;
    }

    return MPI_SUCCESS;
}

int MPIR_Typerep_create_contig(int count, MPI_Datatype oldtype, MPIR_Datatype * newtype)
{
    if (HANDLE_IS_BUILTIN(oldtype)) {
        newtype->typerep.num_contig_blocks = 1;
    } else if (newtype->is_contig) {
        newtype->typerep.num_contig_blocks = 1;
    } else {
        MPIR_Datatype *old_dtp;
        MPIR_Datatype_get_ptr(oldtype, old_dtp);
        newtype->typerep.num_contig_blocks = count * old_dtp->typerep.num_contig_blocks;
    }

    return MPI_SUCCESS;
}

int MPIR_Typerep_create_dup(MPI_Datatype oldtype, MPIR_Datatype * newtype)
{
    MPIR_Datatype *dtp;

    MPIR_Datatype_get_ptr(oldtype, dtp);
    if (dtp->is_committed)
        MPIR_Dataloop_dup(dtp->typerep.handle, &newtype->typerep.handle);

    newtype->is_contig = dtp->is_contig;
    newtype->size = dtp->size;
    newtype->extent = dtp->extent;
    newtype->ub = dtp->ub;
    newtype->lb = dtp->lb;
    newtype->true_ub = dtp->true_ub;
    newtype->true_lb = dtp->true_lb;
    newtype->alignsize = dtp->alignsize;

    newtype->n_builtin_elements = dtp->n_builtin_elements;
    newtype->builtin_element_size = dtp->builtin_element_size;
    newtype->basic_type = dtp->basic_type;

    newtype->typerep.num_contig_blocks = dtp->typerep.num_contig_blocks;

    return MPI_SUCCESS;
}

int MPIR_Typerep_create_indexed_block(int count, int blocklength, const int *array_of_displacements,
                                      MPI_Datatype oldtype, MPIR_Datatype * newtype)
{
    int old_is_contig;
    MPI_Aint old_extent;

    if (HANDLE_IS_BUILTIN(oldtype)) {
        newtype->typerep.num_contig_blocks = count;
        old_is_contig = 1;
        old_extent = (MPI_Aint) MPIR_Datatype_get_basic_size(oldtype);
    } else {
        MPIR_Datatype *old_dtp;
        MPIR_Datatype_get_ptr(oldtype, old_dtp);
        newtype->typerep.num_contig_blocks =
            count * old_dtp->typerep.num_contig_blocks * blocklength;

        MPIR_Datatype_is_contig(oldtype, &old_is_contig);
        old_extent = old_dtp->extent;
    }

    if (old_is_contig) {
        newtype->typerep.num_contig_blocks =
            MPII_Datatype_blockindexed_count_contig(count, blocklength,
                                                    (const void *) array_of_displacements,
                                                    0, old_extent);
    }

    return MPI_SUCCESS;
}

int MPIR_Typerep_create_hindexed_block(int count, int blocklength,
                                       const MPI_Aint * array_of_displacements,
                                       MPI_Datatype oldtype, MPIR_Datatype * newtype)
{
    int old_is_contig;
    MPI_Aint old_extent;

    if (HANDLE_IS_BUILTIN(oldtype)) {
        newtype->typerep.num_contig_blocks = count;
        old_is_contig = 1;
        old_extent = (MPI_Aint) MPIR_Datatype_get_basic_size(oldtype);
    } else {
        MPIR_Datatype *old_dtp;
        MPIR_Datatype_get_ptr(oldtype, old_dtp);
        newtype->typerep.num_contig_blocks =
            count * old_dtp->typerep.num_contig_blocks * blocklength;

        MPIR_Datatype_is_contig(oldtype, &old_is_contig);
        old_extent = old_dtp->extent;
    }

    if (old_is_contig) {
        newtype->typerep.num_contig_blocks =
            MPII_Datatype_blockindexed_count_contig(count, blocklength,
                                                    (const void *) array_of_displacements,
                                                    1, old_extent);
    }

    return MPI_SUCCESS;
}

int MPIR_Typerep_create_indexed(int count, const int *array_of_blocklengths,
                                const int *array_of_displacements, MPI_Datatype oldtype,
                                MPIR_Datatype * newtype)
{
    int old_is_contig;
    MPI_Aint old_extent;

    if (HANDLE_IS_BUILTIN(oldtype)) {
        newtype->typerep.num_contig_blocks = count;
        old_is_contig = 1;
        old_extent = (MPI_Aint) MPIR_Datatype_get_basic_size(oldtype);
    } else {
        MPIR_Datatype *old_dtp;
        MPIR_Datatype_get_ptr(oldtype, old_dtp);

        newtype->typerep.num_contig_blocks = 0;
        for (int i = 0; i < count; i++)
            newtype->typerep.num_contig_blocks +=
                old_dtp->typerep.num_contig_blocks * array_of_blocklengths[i];

        MPIR_Datatype_is_contig(oldtype, &old_is_contig);
        old_extent = old_dtp->extent;
    }

    if (old_is_contig) {
        MPI_Aint *blklens = MPL_malloc(count * sizeof(MPI_Aint), MPL_MEM_DATATYPE);
        MPIR_Assert(blklens != NULL);
        for (int i = 0; i < count; i++)
            blklens[i] = (MPI_Aint) array_of_blocklengths[i];
        newtype->typerep.num_contig_blocks =
            MPII_Datatype_indexed_count_contig(count, blklens,
                                               (const void *) array_of_displacements, 0,
                                               old_extent);
        MPL_free(blklens);
    }

    return MPI_SUCCESS;
}

int MPIR_Typerep_create_hindexed(int count, const int *array_of_blocklengths,
                                 const MPI_Aint * array_of_displacements, MPI_Datatype oldtype,
                                 MPIR_Datatype * newtype)
{
    int old_is_contig;
    MPI_Aint old_extent;

    if (HANDLE_IS_BUILTIN(oldtype)) {
        newtype->typerep.num_contig_blocks = count;
        old_is_contig = 1;
        old_extent = (MPI_Aint) MPIR_Datatype_get_basic_size(oldtype);
    } else {
        MPIR_Datatype *old_dtp;
        MPIR_Datatype_get_ptr(oldtype, old_dtp);

        newtype->typerep.num_contig_blocks = 0;
        for (int i = 0; i < count; i++)
            newtype->typerep.num_contig_blocks +=
                old_dtp->typerep.num_contig_blocks * array_of_blocklengths[i];

        MPIR_Datatype_is_contig(oldtype, &old_is_contig);
        old_extent = old_dtp->extent;
    }

    if (old_is_contig) {
        MPI_Aint *blklens = MPL_malloc(count * sizeof(MPI_Aint), MPL_MEM_DATATYPE);
        MPIR_Assert(blklens != NULL);
        for (int i = 0; i < count; i++)
            blklens[i] = (MPI_Aint) array_of_blocklengths[i];
        newtype->typerep.num_contig_blocks =
            MPII_Datatype_indexed_count_contig(count, blklens,
                                               (const void *) array_of_displacements, 1,
                                               old_extent);
        MPL_free(blklens);
    }

    return MPI_SUCCESS;
}

int MPIR_Typerep_create_resized(MPI_Datatype oldtype, MPI_Aint lb, MPI_Aint extent,
                                MPIR_Datatype * newtype)
{
    if (HANDLE_IS_BUILTIN(oldtype)) {
        int oldsize = MPIR_Datatype_get_basic_size(oldtype);

        newtype->size = oldsize;
        newtype->true_lb = 0;
        newtype->lb = lb;
        newtype->true_ub = oldsize;
        newtype->ub = lb + extent;
        newtype->extent = extent;
        newtype->alignsize = oldsize;   /* FIXME ??? */
        newtype->n_builtin_elements = 1;
        newtype->builtin_element_size = oldsize;
        newtype->is_contig = (extent == oldsize) ? 1 : 0;
        newtype->basic_type = oldtype;
        newtype->typerep.num_contig_blocks = 3; /* lb, data, ub */
    } else {
        MPIR_Datatype *old_dtp;
        MPIR_Datatype_get_ptr(oldtype, old_dtp);

        newtype->size = old_dtp->size;
        newtype->true_lb = old_dtp->true_lb;
        newtype->lb = lb;
        newtype->true_ub = old_dtp->true_ub;
        newtype->ub = lb + extent;
        newtype->extent = extent;
        newtype->alignsize = old_dtp->alignsize;
        newtype->n_builtin_elements = old_dtp->n_builtin_elements;
        newtype->builtin_element_size = old_dtp->builtin_element_size;
        newtype->basic_type = old_dtp->basic_type;

        if (extent == old_dtp->size)
            MPIR_Datatype_is_contig(oldtype, &newtype->is_contig);
        else
            newtype->is_contig = 0;
        newtype->typerep.num_contig_blocks = old_dtp->typerep.num_contig_blocks;
    }

    return MPI_SUCCESS;
}

int MPIR_Typerep_create_struct(int count, const int *array_of_blocklengths,
                               const MPI_Aint * array_of_displacements,
                               const MPI_Datatype * array_of_types, MPIR_Datatype * newtype)
{
    newtype->typerep.num_contig_blocks = 0;
    for (int i = 0; i < count; i++) {
        if (HANDLE_IS_BUILTIN(array_of_types[i])) {
            newtype->typerep.num_contig_blocks++;
        } else {
            MPIR_Datatype *old_dtp;
            MPIR_Datatype_get_ptr(array_of_types[i], old_dtp);
            newtype->typerep.num_contig_blocks +=
                old_dtp->typerep.num_contig_blocks * array_of_blocklengths[i];
        }
    }

    return MPI_SUCCESS;
}

int MPIR_Typerep_create_subarray(int ndims, const int *array_of_sizes, const int *array_of_subsizes,
                                 const int *array_of_starts, int order,
                                 MPI_Datatype oldtype, MPIR_Datatype * newtype)
{
    return MPI_SUCCESS;
}

int MPIR_Typerep_create_darray(int size, int rank, int ndims, const int *array_of_gsizes,
                               const int *array_of_distribs, const int *array_of_dargs,
                               const int *array_of_psizes, int order, MPI_Datatype oldtype,
                               MPIR_Datatype * newtype)
{
    return MPI_SUCCESS;
}
