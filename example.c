#include "list.h"

int main()
{
	/* Create a new list object on the heap.
	 * If you were to allocate a new list object on the stack
	 * you would have to initialize it to zero afterwards.
	 * Example:
	 * List myLocalList = LIST_NULL;
	 */

	List *myDynamicList = list_new();

	/* Free the dynamically allocated list.
	 * You do not need to free local (stack) lists (obviously).
	 */

	list_free(myDynamicList);

	return(0);
}