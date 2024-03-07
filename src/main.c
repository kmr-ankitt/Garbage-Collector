#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#define STACK_MAX 256
#define INITIAL_GC_THRESHOLD 8

typedef enum
{
    OBJ_INT,
    OBJ_PAIR
} ObjectType;

// creating a tagged union
typedef struct sObject
{
    /** we’re only storing pointers to objects in variables and the pair fields.
     *  As soon as an object is no longer pointed to by one of those,
     * the VM has lost it entirely and actually leaked memory.
     * So, we are using the concept of Linked List here. **/
    struct sObject *next;

    // it identifies the type of it
    ObjectType type;

    // marking bit
    unsigned char marked;

    union
    {
        int value;

        struct
        {
            struct sObject *head;
            struct sObject *tail;
        };
    };

} Object;

// creating a virtual machine
typedef struct
{
    // the total no of currently allocated objects
    int numObjects;

    // The number of objects required to trigger a GC.
    int maxObjects;

    // first object is the head
    Object *firstObject;

    Object *stack[STACK_MAX];
    int stackSize;
} VM;

// creates and initialises a Virtual machine
VM *newVM()
{
    VM *vm = malloc(sizeof(VM));
    vm->stackSize = 0;

    vm->firstObject = NULL;

    vm->numObjects = 0;
    vm->maxObjects = INITIAL_GC_THRESHOLD;
    return vm;
}

// pushing operation in stack virtual machine
void push(VM *vm, Object *value)
{
    // assert macro will check the condition inside it. If condition is false the program will terminate and an error message will be displayed
    assert(vm->stackSize < STACK_MAX);
    vm->stack[vm->stackSize++] = value;
}

// popping operation in the virtual machine
Object *pop(VM *vm)
{
    assert(vm->stackSize > 0);
    return vm->stack[--vm->stackSize];
}

void gc(VM *vm);

// That does the actual memory allocation and sets the type tag.
Object *newObject(VM *vm, ObjectType type)
{
    if (vm->numObjects == vm->maxObjects)
        gc(vm);

    Object *object = malloc(sizeof(Object));
    object->type = type;
    object->marked = 0;

    // we will insert this new object into the list of allocated objects
    object->next = vm->firstObject;
    vm->firstObject = object;

    vm->numObjects++;
    return object;
}

// pushing interger
void pushInt(VM *vm, int intValue)
{
    Object *object = newObject(vm, OBJ_INT);
    object->value = intValue;
    push(vm, object);
}

// pushing pair
Object* pushPair(VM *vm)
{
    Object *object = newObject(vm, OBJ_PAIR);
    object->tail = pop(vm);
    object->head = pop(vm);

    push(vm, object);
    return object;
}
 
void mark(Object *object);

void markAll(VM *vm)
{
    for (int i = 0; i < vm->stackSize; i++)
    {
        mark(vm->stack[i]);
    }
}

void mark(Object *object)
{
    // checking if already marked
    // base case
    if (object==NULL && object->marked)
        return;

    // marking this object to be 1
    object->marked = 1;

    // handling pairs here
    if (object->type == OBJ_PAIR)
    {
        // marking using recursion
        mark(object->head);
        mark(object->tail);
    }
}


// sweeping through the entire code and deleting the unmark object
void sweep(VM *vm)
{
    Object **object = &vm->firstObject;
    while (*object)
    {
        if (!(*object)->marked)
        {
            // this object wasnt reached. So it will be removed from the list and freed.
            Object *unreached = *object;
            *object = unreached->next;
            free(unreached);
            vm->numObjects--;
        }
        else
        {
            // this object was reached, so umarked it (next gc) and move to the next
            (*object)->marked = 0;
            object = &(*object)->next;
        }
    }
}

void gc(VM *vm)
{
    int numObjects = vm->numObjects;

    markAll(vm);
    sweep(vm);
    
    vm->maxObjects = vm->numObjects * 2;
}


int main()
{
    VM *vm = newVM();

    // Push some integer values onto the stack
    pushInt(vm, 10);
    pushInt(vm, 20);
    
    // Push a pair onto the stack
    pushPair(vm);
    
    // Trigger garbage collection
    // Assume that the number of allocated objects exceeds INITIAL_GC_THRESHOLD
    gc(vm);

    // Clean up: free the virtual machine
    free(vm);

    return 0;
}
