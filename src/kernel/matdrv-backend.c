#include "matdrv-backend.h"
#include "matdrv-log.h"

struct matBackendFunc** gBackends = NULL;
int activeBackend = -1;
unsigned int backendSize = 0, backendCurrentSize = 0;

int matBackendAdd(struct matBackendFunc* backend)
{
    int ret;

    // a small optimization to avoid often allocations during initialization
    // preallocate 4 spaces for backends
    if (backendCurrentSize == backendSize)
    {
        // pre-allocate gBackends and gBackendInitResults
        backendSize += 4;

        if (!gBackends)
        {
            gBackends = (struct matBackendFunc**)kmalloc(sizeof(struct matBackendFunc) * backendSize,
                        GFP_KERNEL);
        }
        else
        {
            gBackends = (struct matBackendFunc**)krealloc(gBackends,
                        sizeof(struct matBackendFunc) * backendSize,
                        GFP_KERNEL);
        }
        if (!gBackends)
        {
            return -ENOMEM;
        }
    }

    // assign backend
    gBackends[backendCurrentSize] = backend;

    // verify that all functions are defined
    if (gBackends[backendCurrentSize]->init == NULL ||
        gBackends[backendCurrentSize]->release == NULL ||
        gBackends[backendCurrentSize]->setOp == NULL ||
        gBackends[backendCurrentSize]->sendMatrix == NULL ||
        gBackends[backendCurrentSize]->getResultMatrix == NULL)
    {
        LOGE("Incomplete backend provided!");
        return -ENOENT;
    }

    // initialize backend
    ret = gBackends[backendCurrentSize]->init();
    if (ret != 0)
    {
        return ret;
    }

    LOGI("Added backend with ID = %u", backendCurrentSize);
    backendCurrentSize++;

    return 0;
}

int matBackendActivate(unsigned int backendNum)
{
    if (backendNum > backendCurrentSize)
    {
        return -ENOENT;
    }

    activeBackend = backendNum;

    return 0;
}

void matBackendCleanup(void)
{
    // call release() functions
    unsigned int i;
    for (i=0; i<backendCurrentSize; ++i)
    {
        gBackends[i]->release();
    }

    if (gBackends)
    {
        kfree(gBackends);
        gBackends = NULL;
    }

    activeBackend = -1;
}

// from this point on it is assumed, that:
//   * gAvailableBackend points to last added working backend
//   * "working backend" means that there is no function pointer in backend struct which is NULL
void matCommSetOp(enum matOps op)
{
    gBackends[activeBackend]->setOp(op);
}

void matCommSendMatrix(int* mat)
{
    gBackends[activeBackend]->sendMatrix(mat);
}

void matCommGetResultMatrix(int* mat)
{
    gBackends[activeBackend]->getResultMatrix(mat);
}
