#include "divide.h"

/**
* @brief	交换元素的值
* @arg		x:		int *型，代交换元素x地址
* @arg		y:		int *型，待交换元素y地址
* @retval	void
*/
void interchange(int *x, int *y)
{
	int temp;
	temp = *x;
	*x = *y;
	*y = temp;
}
