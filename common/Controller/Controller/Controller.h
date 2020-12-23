#pragma once

/*  ref      err   ____________  cout  _____________    pout
     r  +     e   |            |  u   |             |     y
   ----->( )----->| Controller |----->|    Plant    |-----+----->
        - ^       |____________|      |_____________|     |
          |                                               |
          +-----------------------------------------------+

    Param | Member |
    r     | ref    | Reference
    e     | err    | Error
    u     | cout   | Controller output / Plant input
    y     | pout   | Plant output
*/

class Controller
{
protected:
	float ref  = 0;
	float cout = 0;

public:
    virtual void Process(float y) = 0;

    float GetOutput() {return cout;}
    void  SetRef(float r) {ref = r;}
};

