/* -*- Mode: C; c-basic-offset:4 ; -*- */
/*
 *  (C) 2008 by Argonne National Laboratory.
 *      See COPYRIGHT in top-level directory.
 */

#ifndef BSCI_H_INCLUDED
#define BSCI_H_INCLUDED

#include "hydra.h"

HYD_Status HYD_BSCI_Launch_procs(void);
HYD_Status HYD_BSCI_Get_universe_size(int *size);
HYD_Status HYD_BSCI_Wait_for_completion(void);
HYD_Status HYD_BSCI_Finalize(void);

#endif /* BSCI_H_INCLUDED */
