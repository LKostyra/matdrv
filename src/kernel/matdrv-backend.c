#include "matdrv-backend.h"

struct matBackendFunc* gBackends = NULL;
int* gBackendInitResults = NULL;
int activeBackend = -1;

int matBackendAdd(struct matBackendFunc backend)
{
    // TODO fill
    backend;

    return 0;
}

int matBackendActivate(unsigned int backendNum)
{
   // TODO fill
    backendNum;

    return 0;
}

void matBackendCleanup(void)
{
    // TODO fill
}

// from this point on it is assumed, that:
//   * gAvailableBackend points to last added working backend
//   * "working backend" means that there is no function pointer in backend struct which is NULL
void matCommSetOp(enum matOps op)
{
    gBackends[activeBackend].setOp(op);
}

void matCommSendMatrix(int* mat)
{
    gBackends[activeBackend].sendMatrix(mat);
}

void matCommGetResultMatrix(int* mat)
{
    gBackends[activeBackend].getResultMatrix(mat);
}
