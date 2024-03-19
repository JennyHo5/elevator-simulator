#ifndef CARREQUEST_H
#define CARREQUEST_H

#include "Elevator.h"

struct CarRequest {
    Floor* floor;
    Elevator* elevator;
};
#endif // CARREQUEST_H
