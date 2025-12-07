template <typename Key, typename Val>
HashTableOpen<Key, Val>::HashTableOpen(int i)
: M(i), ht(nullptr) {
    // 1. Allocate the dynamic array of pointers (size M)
    ht = new LinkedList<Record>*[M];

    // 2. Initialize each slot with a new linked list object
    for (int idx = 0; idx < M; idx++) {
        ht[idx] = new LinkedList<Record>;
    }
}

template <typename Key, typename Val>
HashTableOpen<Key, Val>::HashTableOpen(const HashTableOpen<Key, Val>& copyObj)
: M(0), ht(nullptr) {
    copy(copyObj);
}

template <typename Key, typename Val>
HashTableOpen<Key, Val>& HashTableOpen<Key, Val>::operator=
(const HashTableOpen<Key, Val>& rightObj) {
    if (this != &rightObj) {
        clear();
        copy(rightObj);
    }
    return *this;
}

template <typename Key, typename Val>
HashTableOpen<Key, Val>::~HashTableOpen() {
    // 1. Delete each linked list object pointed to by ht[i]
    for (int idx = 0; idx < M; idx++) {
        delete ht[idx];
    }
    // 2. Delete the dynamic array of pointers
    delete[] ht;
}

template <typename Key, typename Val>
void HashTableOpen<Key, Val>::clear() {
    for (int i = 0; i < M; i++) {
        ht[i]->clear();
    }
}

template <typename Key, typename Val>
void HashTableOpen<Key, Val>::copy(const HashTableOpen<Key, Val>& copyObj) {
    if (M != copyObj.M) {
        LinkedList<Record>** tmp = new LinkedList<Record>*[copyObj.M];
        if (M < copyObj.M) {
            for (int i = 0; i < M; i++) {
                tmp[i] = ht[i];
            }
            for (int i = M; i < copyObj.M; i++) {
                tmp[i] = new LinkedList<Record>;
            }
        }
        else if (M > copyObj.M) {
            for (int i = 0; i < copyObj.M; i++) {
                tmp[i] = ht[i];
            }
            for (int i = copyObj.M; i < M; i++) {
                delete ht[i];
            }
        }

        M = copyObj.M;
        delete[] ht;
        ht = tmp;
    }

    LinkedList<Record>* myList      = nullptr;
    LinkedList<Record>* copyList    = nullptr;
    int                 myListLen   = 0;
    int                 copyListLen = 0;
    for (int i = 0; i < M; i++) {
        myList      = ht[i];
        myListLen   = myList->getLength();
        copyList    = copyObj.ht[i];
        copyListLen = copyList->getLength();
        if (myListLen < copyListLen) {
            for (int j = 0; j < myListLen; j++) {
                myList->replace(j, copyList->getElement(j));
            }
            for (int j = myListLen; j < copyListLen; j++) {
                if (myList->isEmpty()) {
                    myList->append(copyList->getElement(j));
                }
                else {
                    myList->insert(0, copyList->getElement(j));
                }
            }
        }
        else if (myListLen > copyListLen) {
            for (int j = 0; j < copyListLen; j++) {
                myList->replace(j, copyList->getElement(j));
            }
            for (int j = copyListLen; j < myListLen; j++) {
                myList->remove(j);
            }
        }
        else {
            for (int j = 0; j < myListLen; j++) {
                myList->replace(j, copyList->getElement(j));
            }
        }
    }
}

// ----------------------------------------------------------------------
// BRANCH MODIFIED: find (Search)
// ----------------------------------------------------------------------
template <typename Key, typename Val>
Val HashTableOpen<Key, Val>::find(const Key& k) const {
    // 1. Hash the key to get the slot index
    int slot = hash(k);

    // 2. Get the pointer to the linked list bucket
    LinkedList<Record>* bucket = ht[slot];

    // 3. Traverse the linked list (bucket)
    for (int i = 0; i < bucket->getLength(); i++) {
        Record currentRecord = bucket->getElement(i);
        // Compare the record's key (.k) with the target key (k)
        if (currentRecord.k == k) {
            // Record found, return its value (.v)
            return currentRecord.v;
        }
    }

    // 4. If loop completes without finding the key, throw an error
    throw string("find: error, unsuccessful search, target key not found");
}

template <typename Key, typename Val>
int HashTableOpen<Key, Val>::hash(const Key& k) const {
    // how long should each fold be
    // changing this means you should also change the reinterpeted data type
    const int FOLD_LEN = 4;
    // if the fold length is 4, then must treat the string as unsigned numbers
    unsigned* ikey = (unsigned*) k.c_str();
    // calculate how many folds there are
    int ilen = k.length() / FOLD_LEN;
    // accumulator
    unsigned sum = 0;

    // for each fold, now treated as a number, add it to the running total
    for (int i = 0; i < ilen; i++) {
        sum += ikey[i];
    }

    // calculate how many leftover characters there are
    int extra = k.length() - ilen * FOLD_LEN;
    // create a temporary array that will hold those extra characters
    char temp[FOLD_LEN];
    // clear out that array with a 0 value
    ikey    = (unsigned*) temp;
    ikey[0] = 0;

    // copy the extra characters over
    for (int i = 0; i < extra; i++) {
        temp[i] = k[ilen * FOLD_LEN + i];
    }

    // add these final characters as a number to the running total
    sum += ikey[0];
    // calculate the slot position
    int slot = sum % M;
    // display the hashing information
    cout << k << "\thashes to slot " << slot << endl;
    // return the valid slot position
    return slot;
}

// ----------------------------------------------------------------------
// BRANCH MODIFIED: insert (O(1) Insertion)
// ----------------------------------------------------------------------
template <typename Key, typename Val>
void HashTableOpen<Key, Val>::insert(const Key& k, const Val& v) {
    // 1. Hash the key to get the slot index
    int slot = hash(k);

    // 2. Create the new record
    Record newRecord(k, v);

    // 3. Insert the new record into the linked list at the calculated slot (O(1) operation).
    // We use append() as it is guaranteed O(1) for LinkedList (by traversing to the end),
    // or by using insert(0, ...) if the list provided supported fast front insertion.
    // Given the provided LinkedList.tpp, append is safest for O(1) insertion.
    LinkedList<Record>* bucket = ht[slot];
    bucket->append(newRecord);
}

// ----------------------------------------------------------------------
// BRANCH MODIFIED: remove (Deletion)
// ----------------------------------------------------------------------
template <typename Key, typename Val>
void HashTableOpen<Key, Val>::remove(const Key& k) {
    // 1. Hash the key to get the slot index
    int slot = hash(k);

    // 2. Get the pointer to the linked list bucket
    LinkedList<Record>* bucket = ht[slot];

    // 3. Traverse the linked list (bucket) to find the key
    for (int i = 0; i < bucket->getLength(); i++) {
        Record currentRecord = bucket->getElement(i);
        
        if (currentRecord.k == k) {
            // Record found at position i, remove it
            bucket->remove(i);
            return; // Exit after successful removal
        }
    }

    // 4. If loop completes without finding the key, throw an error
    throw string("remove: error, unable to find record with matching key to remove");
}

// ----------------------------------------------------------------------
// BRANCH MODIFIED: size (Total Elements)
// ----------------------------------------------------------------------
template <typename Key, typename Val>
int HashTableOpen<Key, Val>::size() const {
    int totalRecords = 0;
    
    // Iterate through all slots (M) in the dynamic array
    for (int i = 0; i < M; i++) {
        // Add the length of each linked list (bucket) to the total
        totalRecords += ht[i]->getLength();
    }

    return totalRecords;
}