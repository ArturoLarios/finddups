typedef struct unordered_map 
{
    unordered_map* next;
    void* key;
    void* value;
} unordered_map;

int uomap_int_keys(void* key1, void* key2)
{
    if ( *( (int *) key1) == *( (int *) key2) )
        return 1;
    return 0;
}

// Simply returns NULL but allows for abstraction
unordered_map* uomap_create_dictionary(void)
{
    return NULL;
}

// If an entry doesn't exist for the key, insert the key-value pair at the head of the dictionary
unordered_map* uomap_insert_entry(unordered_map *map, void* key, void* value)
{
    unordered_map *newEntry = NULL;
    if (!uomap_contains(map, key)) {
        newEntry = (unordered_map*) malloc(sizeof(unordered_map));
        newEntry->next = map;
        newEntry->key = key;
        newEntry->value = value;
        return newEntry;
    }
    return map;
}

// Delete an entry and free the associated memory
// Takes an int (void*, void*) function as an argument to compare the keys that returns 0 if they're not equal
void uomap_delete_entry(unordered_map **map, void* key, int (*key_func)(void*, void*))
{
    unordered_map *previousEntry = NULL;
    // Iterate through the dictionary
    for (unordered_map *currentEntry = map; currentEntry != NULL; currentEntry = currentEntry->next) {
        if (key_func(currentEntry->key, key)) {         // If an entry with the key is found
            if (previousEntry)                                          // If we're not at the head of the dictionary
                previousEntry->next = currentEntry->next;                   // Update the previous entry to point to the next entry
            else                                                        // Otherwise,
                *map = currentEntry->next;                                  // Make the next entry the head of the dictionary
            free(currentEntry);                                         // Free the entry with the key
            return;
        }
        previousEntry = currentEntry;
    }
}

// Deletes dictionary and frees 
void uomap_delete_dictionary(unordered_map *map, void *(value_free) (void*))
{
    
}

// Find the entry with the corresponding key, if it exists
// Takes an int (void*, void*) function as an argument to compare the keys that returns 0 if they're not equal
unordered_map* uomap_contains(unordered_map *map, void* key, int (*key_func)(void*, void*))
{
    // Iterate through the dictionary
    for (unordered_map *currentEntry = map; currentEntry != NULL; currentEntry = currentEntry->next)
        if (key_func(currentEntry->key, key))    // If an entry with the key is found
            return currentEntry;                                // Return it
    return NULL;
}

// Return the value for an entry
void* uomap_get_value(unordered_map *entry)
{
    return entry->value;
}

// Update the value for an entry
void uomap_update_value(unordered_map *entry, void* value)
{
    entry->value = value;
}

