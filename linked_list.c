typedef struct linked_list
{
    linked_list* next;
    void *data;
} linked_list;

// Initialize the linked list
linked_list* llist_create_list()
{
    return NULL;
}

// Add a node to the front of the list
linked_list* llist_prepend(linked_list* list, void* data)
{
    linked_list *newNode = (linked_list*) malloc(sizeof(linked_list));
    newNode->next = list;
    newNode->data = data;
    return newNode;
}

// Delete a node in the list
// Takes an int (void*, void*) function as an argument to compare node data that returns 0 if they're not equal
linked_list* llist_delete(linked_list** list, void* data, int (*data_func) (void*, void*), void (*data_free) (void*, void*))
{
    linked_list *previousNode = NULL;
    for (linked_list *currentNode = list; currentNode; currentNode = currentNode->next) {
        if (data_func(currentNode->data, data)) {   // If the current node contains the data being deleted
            if (previousNode)                           // If we're not at the head of the list
                previousNode->next = currentNode->next;     // Update the previous node to point to the next node
            else                                        // Otherwise,
                *list = currentNode->next;                  // Make the next node the head of the list
            data_free(currentNode->data);               // Free node's data using custom function
            free(currentNode);                          // Free node
            return;
        }
        previousNode = currentNode;
    }
}

void llist_free(linked_list* list, void (*data_free) (void*))
{
    linked_list *previousNode = list;
    linked_list *nextNode = list;
    while (nextNode) {
        data_free(nextNode->data);
        free(nextNode);
    }
}