#include "stackh.h"

QUEIS::QUEIS(int m):STACK::elems(new int[m]), STACK::max(STACK::elems?m:0),STACK::pos(0)，s.elems(new int[m]), s.max(s.elems?m:0), s.pos(0)  
{
}

QUEIS::QUEIS(const QUEIS &q)
{

}

QUEIS::operator int() const
{
	return STACK::pos + s.pos;
}


