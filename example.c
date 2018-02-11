/* TODO: Unfinished example...
 *
 */

#include "list.h"

typedef struct
{
	int number;
	char name[24];
} CoolObject;

int main()
{
	/* Create a new list object on the heap.
	 * If you were to allocate a new list object on the stack
	 * you would have to initialize it to zero afterwards.
	 * Example:
	 * List myLocalList = LIST_NULL;
	 */

	List *myDynamicList = list_new();

	/* Here we have an array of very cool objects -
	 * These can of course be of any datatype.
	 */

	CoolObject myCoolObjects[] =
	{
		{.number = 1, .name = "one"},
		{.number = 2, .name = "two"},
		{.number = 3, .name = "three"},
		{.number = 4, .name = "four"},
		{.number = 5, .name = "five"},
		{.number = 6, .name = "six"},
		{.number = 7, .name = "seven"},
		{.number = 8, .name = "eight"},
		{.number = 9, .name = "nine"}
	};
	#define NUM_OBJECTS (sizeof(myCoolObjects) / sizeof(myCoolObjects[0]))

	/* Now we'll just add every single object to the end
	 * of our list.
	 */

	size_t i;

	for(i = 0; i < NUM_OBJECTS; i++)
	{
		list_add_last(myDynamicList, myCoolObjects[0]);
	}

	/* Clear the list
	 * Note: If you have dynamically allocated elements within the list
	 * you'd have to free them first.
	 */

	list_clear(myDynamicList);

	/* Free the dynamically allocated list.
	 * You do not need to free local (stack) lists (obviously).
	 */

	list_free(myDynamicList);

	return(0);
}