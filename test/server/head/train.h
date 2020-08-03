#ifndef TRAIN_H
#define TRAIN_H

#include "includings.h"

typedef struct {
    size_t length;
    char buf[1 << 10];
} train_t, * pTrain_t;

#endif // TRAIN_H
