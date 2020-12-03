#pragma once

#include "Transmitter.h"
#include "Receiver.h"

class Transciever : public Transmitter,
                    public Receiver
{

};
