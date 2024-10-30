#include <iostream>
#include <string>
using namespace std;

class CPUJob {
    public:
        int job_id; // Unique identifier for the job
        int priority; // Priority level of the job (1-10)
        int job_type; // Job type (1-10)
        int cpu_time_consumed; // Total CPU time consumed by the job
        int memory_consumed; // Total memory consumed thus far
        CPUJob(int job_id, int priority, int job_type, int cpu_time_consumed, int memory_consumed);
        void display() { // Display the job attributes
            cout << "Job ID: " << job_id << ", Priority: " << priority << ", Job Type: " << job_type << ", CPU Time Consumed: " << cpu_time_consumed << ", Memory Consumed: " << memory_consumed << endl;
        }
};

// default CPUJob constructor
CPUJob::CPUJob(int job_id, int priority, int job_type, int cpu_time_consumed, int memory_consumed) {
    this->job_id = job_id;
    this->priority = priority;
    this->job_type = job_type;
    this->cpu_time_consumed = cpu_time_consumed;
    this->memory_consumed = memory_consumed;
};

// templated Queue class
template <class DT>
class Queue {
    public:
        CPUJob* JobPointer; // Pointer to a job (e.g., CPUJob)
        Queue<DT>* next; // Pointer to the next node in the queue
        Queue(CPUJob* JobPointer); // parameterized constructor
        void add(Queue<DT>* node); // insert a node to the queue

};

// default Queue constructor
template <class DT>
Queue<DT>::Queue(CPUJob* JobPointer) {
    this->JobPointer = JobPointer; // set the JobPointer to the passed in JobPointer
    this->next = NULL;
}

// NovelQueue class
template <class DT>
class NovelQueue{
    public:
        Queue<DT>* front; // Pointer to the front of the queue
        Queue<DT>** NodePtrs; // Array of pointers to Queue nodes
        int size; // Number of elements in the queue
        NovelQueue(); // default constructor
        void enqueue(CPUJob* JobPointer); // Add a job to the queue
        CPUJob* dequeue(); // Remove a job from the front of the queue
        void dequeue(CPUJob* JobPointer); // Remove a specific job from the queue
        void modify(int job_id, int new_priority, int new_job_type, int new_cpu_time_consumed, int new_memory_consumed); // Modify a job in the queue
        CPUJob* findJob(int job_id); // Find a job in the queue
        void addNodePtr(CPUJob* JobPointer); // add a nodePtr and maintain sorted order
        void removeNodePtr(CPUJob* JopPointer); // remove a nodePtr and update the array
        void change(int job_id, int field_index, int new_value); // Change a job value
        void promote(int job_id, int positions); // Promote a job in the queue
        NovelQueue<CPUJob*>* reorder(int attribute_index); // Reorder the queue based on an attribute
        void display(); // Displays all elements in the queue
        int count(); // Returns the number of elements in the queue
        void listJobs(); // List all jobs in the queue
        int currentPosition(CPUJob* JobPointer); // Find the current position of a job in the queue
        void displayNodePtrs(); // Display the NodePtrs array
        void testAddNodePtr(); // Test the updateNodePtrs method
        void testRemoveNodePtr(); // Test the removeNodePtr method
        void testModify(); // Test the modify method
        void testPromote(); // Test the promote method
        void testReorder(); // Test the reorder method
        void testListJobs(); // Test the listJobs method
};

// default NovelQueue constructor
template <class DT>
NovelQueue<DT>::NovelQueue() {
    front = nullptr;
    size = 0;
    NodePtrs = new Queue<DT>*[size]; // initialize the NodePtrs array
}

template <class DT>
void NovelQueue<DT>::enqueue(CPUJob* JobPointer) {
    // make sure job does not already exist in the queue
    if (findJob(JobPointer->job_id) != nullptr) {
        cout << "Job ID " << JobPointer->job_id << " already exists!" << endl;
        return;
    }

    // create a new node
    Queue<DT>* queueNode = new Queue<DT>(JobPointer); 
    
    // update front pointer if queue is empty
    if (front == nullptr) {
        front = queueNode;
    } else { // queue not empty, add to end
        Queue<DT>* temp = front;
        // traverse to the end of the queue
        while (temp->next != nullptr) {
            temp = temp->next;
        }
        temp->next = queueNode; // add the new node to the end
    }

    // update the NodePtrs array
    addNodePtr(JobPointer);
}

template <class DT>
CPUJob* NovelQueue<DT>::dequeue() {
    // check if the queue is empty
    if (front == nullptr) {
        // queue empty, return null
        return nullptr;
    }

    // get the job from the front of the queue
    CPUJob* job = front->JobPointer;

    // save the front node
    Queue<DT>* temp = front;

    // update the front pointer
    front = front->next;

    // update the NodePtrs array
    removeNodePtr(job);

    // delete the front node
    delete temp;

    // return the job
    return job;
}

template <class DT>
void NovelQueue<DT>::dequeue(CPUJob* JobPointer) {

    // check if the job is at the front of the queue
    if (front->JobPointer->job_id == JobPointer->job_id) {
        dequeue();
        return;
    }

    // find the job in the queue
    Queue<DT>* temp = front;
    Queue<DT>* prev = nullptr;
    while (temp != nullptr && temp->JobPointer != JobPointer) {
        prev = temp;
        temp = temp->next;
    }

    // special case if job is at the end of the queue
    if (temp->next == nullptr) {
        prev->next = nullptr;
    } else {
        // General case: Remove a node from the middle
        Queue<DT>* temp2 = temp->next;
        temp->next = temp2->next;
        temp->JobPointer = temp2->JobPointer;
        delete temp2;
    }
    
    // update the NodePtrs array
    removeNodePtr(JobPointer);

}

template <class DT>
void NovelQueue<DT>::addNodePtr(CPUJob* JobPointer) {
    // create a new array of pointers
    size++;
    Queue<DT>** newNodePtrs = new Queue<DT>*[size];

    // Merge the old node pointers array with the new node based on job_id using merge algorithm
    int i = 0, j = 0; // i for the old array, j for the new array
    bool inserted = false;

    while (i < size - 1) { // size - 1 because we are iterating over the old array only
        if (NodePtrs[i]->JobPointer->job_id < JobPointer->job_id) {
            // insert the old node pointer from the old array into the new array
            newNodePtrs[j] = NodePtrs[i];
            i++;
            j++;
        } else if (!inserted && NodePtrs[i]->JobPointer->job_id > JobPointer->job_id) {
            // insert the new node pointer before the current old one
            newNodePtrs[j] = new Queue<DT>(JobPointer);
            inserted = true; // ensure the new job is inserted only once
            j++; // only increment j for the new array
        } else {
            // insert the old node pointer from the old array
            newNodePtrs[j] = NodePtrs[i];
            i++;
            j++;
        }
    }

    // If the new job wasn't inserted, append it at the end
    if (!inserted) {
        newNodePtrs[j] = new Queue<DT>(JobPointer);
        j++;
    }

    // Update the NodePtrs array and delete the old array
    delete[] NodePtrs;
    this->NodePtrs = newNodePtrs;
}

template <class DT>
void NovelQueue<DT>::removeNodePtr(CPUJob* JobPointer) {
    // create a new array of pointers
    size--;
    Queue<DT>** newNodePtrs = new Queue<DT>*[size];

    // Merge the NodePtrs array with the new node based on job_id using merge algorithm
    int i = 0, j = 0; // i represents the index of the new array, j represents the index of the old array

    while (i < size) {
        if (NodePtrs[j]->JobPointer->job_id != JobPointer->job_id) {
            newNodePtrs[i] = NodePtrs[j];
            i++;
        }
        j++;
    }
    // Update the NodePtrs array and delete the old array
    delete[] NodePtrs;
    NodePtrs = newNodePtrs;
}

template <class DT>
void NovelQueue<DT>::modify(int job_id, int new_priority, int new_job_type, int new_cpu_time_consumed, int new_memory_consumed) {
    // find the job in the pointer array
    CPUJob* job = findJob(job_id);

    // check if the job is not found
    if (job == nullptr) {
        cout << "Job with ID " << job_id << " not found in the queue." << endl;
        return;
    }

    // find the job in the array, do not dequeue
    for (int i = 0; i < size; i++) {
        if (NodePtrs[i]->JobPointer->job_id == job_id) {
            job = NodePtrs[i]->JobPointer;
            break;
        }
    }

    // modify the job
    job->priority = new_priority;
    job->job_type = new_job_type;
    job->cpu_time_consumed = new_cpu_time_consumed;
    job->memory_consumed = new_memory_consumed;

    cout << "Modified Job ID " << job_id << ":" << endl;
    job->display();
}

template <class DT>
CPUJob* NovelQueue<DT>::findJob(int job_id) {
    // search sorted array of NodePtrs for the job_id using binary search
    int left = 0, right = size - 1;
    while (left <= right) {
        int mid = (left + right) / 2;
        if (NodePtrs[mid]->JobPointer->job_id == job_id) {
            return NodePtrs[mid]->JobPointer;
        } else if (NodePtrs[mid]->JobPointer->job_id < job_id) { 
            left = mid + 1; // search the right half
        } else {
            right = mid - 1; // search the left half
        }
    }
    return nullptr; // job not found
}

template <class DT>
void NovelQueue<DT>::change(int job_id, int field_index, int new_value) {
    
    // find the job in the pointer array
    CPUJob* job = findJob(job_id);

    // check if the job is not found
    if (job == nullptr) {
        cout << "Job with ID " << job_id << " not found in the queue." << endl;
        return;
    }

    // modify the job
    switch(field_index) {
        case 1:
            job->priority = new_value;
            break;
        case 2:
            job->job_type = new_value;
            break;
        case 3:
            job->cpu_time_consumed = new_value;
            break;
        case 4:
            job->memory_consumed = new_value;
            break;
    }
    cout << "Changed Job ID " << job_id << " field " << field_index << " to " << new_value << ":" << endl;
    job->display();
    cout << "Jobs after changing field:" << endl;
    display();
}

template <class DT>
void NovelQueue<DT>::displayNodePtrs() {
    // display the NodePtrs array
    for (int i = 0; i < size; i++) {
        if (NodePtrs[i]->JobPointer != nullptr) NodePtrs[i]->JobPointer->display();
    }
    cout << endl;
}

template <class DT>
void NovelQueue<DT>::testAddNodePtr() {
    // method to test the updateNodePtrs method
    NovelQueue<CPUJob*>* myNovelQueue = new NovelQueue<CPUJob*>();
    myNovelQueue->enqueue(new CPUJob(1, 1, 1, 1, 1));
    myNovelQueue->enqueue(new CPUJob(2, 2, 2, 2, 2));
    myNovelQueue->enqueue(new CPUJob(4, 4, 4, 4, 4));
    myNovelQueue->enqueue(new CPUJob(3, 3, 3, 3, 3));
    myNovelQueue->displayNodePtrs();
}

template <class DT>
void NovelQueue<DT>::testRemoveNodePtr() {
    // method to test the removeNodePtr method
    NovelQueue<CPUJob*>* myNovelQueue = new NovelQueue<CPUJob*>();
    myNovelQueue->enqueue(new CPUJob(1, 1, 1, 1, 1));
    myNovelQueue->enqueue(new CPUJob(2, 2, 2, 2, 2));
    myNovelQueue->enqueue(new CPUJob(3, 3, 3, 3, 3));
    myNovelQueue->enqueue(new CPUJob(4, 4, 4, 4, 4));
    myNovelQueue->dequeue();
    myNovelQueue->displayNodePtrs();
}


template <class DT>
void NovelQueue<DT>::testModify() {
    // test the modify method
    NovelQueue<CPUJob*>* myNovelQueue = new NovelQueue<CPUJob*>();
    myNovelQueue->enqueue(new CPUJob(1, 1, 1, 1, 1));
    myNovelQueue->enqueue(new CPUJob(2, 2, 2, 2, 2));
    myNovelQueue->enqueue(new CPUJob(3, 3, 3, 3, 3));
    myNovelQueue->modify(3, 4, 4, 4, 4);
    // print queue
    for (int i = 0; i < myNovelQueue->size; i++) {
        myNovelQueue->NodePtrs[i]->JobPointer->display();
    }
}

template <class DT>
void NovelQueue<DT>::promote(int job_id, int positions) {

    // find the job in the queue
    Queue<DT>* temp = front;

    // store the current position
    int current_position = 0;
    while (temp->JobPointer->job_id != job_id) {
        current_position++;
        temp = temp->next;
    }

    // make sure front is not being promoted
    if (current_position == 0) {
        cout << "Cannot promote the front of the queue" << endl;
        return;
    }

    // save the job and remove it from the queue
    CPUJob* job = temp->JobPointer;
    dequeue(job);
    
    // calculate the new position
    int new_position = max(0, current_position - positions);

    // Insert the node at the new position
    if (new_position == 0) {
        // Insert at the front of the queue
        Queue<DT>* new_node = new Queue<DT>(job);
        new_node->next = front;
        front = new_node;
    } else {
        // Find position to insert node after
        temp = front;
        for (int i = 0; i < new_position - 1; i++) {
            temp = temp->next;
        }

        // Create a new node and add it after the temp node
        Queue<DT>* new_node = new Queue<DT>(job);
        new_node->next = temp->next;
        temp->next = new_node;
    }
    
    // update the NodePtrs array
    addNodePtr(job);

    cout << "Promoted Job ID " << job_id << " by " << positions << " Position(s):" << endl;
    job->display();
}

template <class DT>
NovelQueue<CPUJob*>* NovelQueue<DT>::reorder(int attribute_index) {
    // create a new queue
    NovelQueue<CPUJob*>* reorderedQueue = new NovelQueue<CPUJob*>();

    // create a copy of the NodePtrs array
    Queue<DT>** copyNodePtrs = new Queue<DT>*[size];
    for (int i = 0; i < size; i++) {
        copyNodePtrs[i] = NodePtrs[i];
    }

    // sort the copyNodePtrs array based on the attribute_index and insertion sort
    switch(attribute_index) {
        case 2: {
            cout << "Reordered Queue by attribute 2:" << endl;
            // sort by priority
            for (int i = 1; i < size; i++) {
                Queue<DT>* key = copyNodePtrs[i];
                int j = i - 1;
                // move elements greater than key to one position ahead of their current position
                while (j >= 0 && copyNodePtrs[j]->JobPointer->priority > key->JobPointer->priority) {
                    copyNodePtrs[j + 1] = copyNodePtrs[j];
                    j--;
                }
                copyNodePtrs[j + 1] = key;
            }
            break;
        }
        case 3: {
            // sort by job_type
            cout << "Reordered Queue by attribute 3:" << endl;
            for (int i = 1; i < size; i++) {
                Queue<DT>* key = copyNodePtrs[i];
                int j = i - 1;
                // move elements greater than key to one position ahead of their current position
                while (j >= 0 && copyNodePtrs[j]->JobPointer->job_type > key->JobPointer->job_type) {
                    copyNodePtrs[j + 1] = copyNodePtrs[j];
                    j--;
                }
                copyNodePtrs[j + 1] = key;
            }
            break;
        }
        case 4: {
            // sort by cpu_time_consumed
            cout << "Reordered Queue by attribute 4:" << endl;
            for (int i = 1; i < size; i++) {
                Queue<DT>* key = copyNodePtrs[i];
                int j = i - 1;
                // move elements greater than key to one position ahead of their current position
                while (j >= 0 && copyNodePtrs[j]->JobPointer->cpu_time_consumed > key->JobPointer->cpu_time_consumed) {
                    copyNodePtrs[j + 1] = copyNodePtrs[j];
                    j--;
                }
                copyNodePtrs[j + 1] = key;
            }
            break;
        }
        case 5: {
            // sort by memory_consumed
            cout << "Reordered Queue by attribute 5:" << endl;
            for (int i = 1; i < size; i++) {
                Queue<DT>* key = copyNodePtrs[i];
                int j = i - 1;
                // move elements greater than key to one position ahead of their current position
                while (j >= 0 && copyNodePtrs[j]->JobPointer->memory_consumed > key->JobPointer->memory_consumed) {
                    copyNodePtrs[j + 1] = copyNodePtrs[j];
                    j--;
                }
                copyNodePtrs[j + 1] = key;
            }
            break;
        }
        case 1 : {
            // sort by job_id
            cout << "Reordered Queue by attribute 1:" << endl;
            for (int i = 1; i < size; i++) {
                Queue<DT>* key = copyNodePtrs[i];
                int j = i - 1;
                // move elements greater than key to one position ahead of their current position
                while (j >= 0 && copyNodePtrs[j]->JobPointer->job_id > key->JobPointer->job_id) {
                    copyNodePtrs[j + 1] = copyNodePtrs[j];
                    j--;
                }
                copyNodePtrs[j + 1] = key;
            }
            break;
        }
    }

    // enqueue the jobs in the reorderedQueue
    for (int i = 0; i < size; i++) {
        reorderedQueue->enqueue(copyNodePtrs[i]->JobPointer);
    }

    // resort the NodePtrs array based on job_id using insertion sort
    for (int i = 1; i < size; i++) {
        Queue<DT>* key = copyNodePtrs[i];
        int j = i - 1;

        while (j >= 0 && copyNodePtrs[j]->JobPointer->job_id > key->JobPointer->job_id) {
            copyNodePtrs[j + 1] = copyNodePtrs[j];
            j--;
        }
        copyNodePtrs[j + 1] = key;
    }

    // delete and reassign the copyNodePtrs array
    delete[] reorderedQueue->NodePtrs;
    reorderedQueue->NodePtrs = copyNodePtrs;

    // return the reorderedQueue
    return reorderedQueue;
}

template <class DT>
void NovelQueue<DT>::testPromote() {
    // test the promote method
    NovelQueue<CPUJob*>* myNovelQueue = new NovelQueue<CPUJob*>();
    myNovelQueue->enqueue(new CPUJob(1, 1, 1, 1, 1));
    myNovelQueue->enqueue(new CPUJob(2, 2, 2, 2, 2));
    myNovelQueue->enqueue(new CPUJob(3, 3, 3, 3, 3));
    myNovelQueue->enqueue(new CPUJob(4, 4, 4, 4, 4));
    myNovelQueue->promote(1, 2);
    // print queue
    Queue<DT>* temp = myNovelQueue->front;
    while (temp != nullptr) {
        temp->JobPointer->display();
        temp = temp->next;
    }
}

template <class DT>
void Queue<DT>::add(Queue<DT>* node) {
    // insert node after the current node
    node->next = next;
    next = node;
}

template <class DT>
void NovelQueue<DT>::testReorder() {
    // test the reorder method
    NovelQueue<CPUJob*>* myNovelQueue = new NovelQueue<CPUJob*>();
    // enqueue with random attributes
    myNovelQueue->enqueue(new CPUJob(1, 3, 15, 6, 2));
    myNovelQueue->enqueue(new CPUJob(2, 1, 10, 3, 1));
    myNovelQueue->enqueue(new CPUJob(3, 2, 5, 4, 3));
    myNovelQueue->enqueue(new CPUJob(4, 4, 20, 8, 4));
    NovelQueue<CPUJob*>* reorderedQueue = myNovelQueue->reorder(4);
    // print queue
    Queue<DT>* temp = reorderedQueue->front;
    while (temp != nullptr) {
        temp->JobPointer->display();
        temp = temp->next;
    }
}

template <class DT>
void NovelQueue<DT>::display() {
    // display all elements in the queue
    Queue<DT>* temp = front;
    while (temp != nullptr) {
        temp->JobPointer->display();
        temp = temp->next;
    }
}

template <class DT>
int NovelQueue<DT>::count() {
    // return the number of elements in the queue
    return size;
}

template <class DT>
int NovelQueue<DT>::currentPosition(CPUJob* JobPointer) {
    // find the current position of a job in the queue
    int current_position = 0;
    Queue<DT>* temp = front;
    // traverse the queue to find the job
    while (temp->JobPointer->job_id != JobPointer->job_id) {
        current_position++;
        temp = temp->next;
    }

    return current_position;
}

template <class DT>
void NovelQueue<DT>::listJobs() {
    // list all jobs in the queue
    for (int i = 0; i < size; i++) {
        NodePtrs[i]->JobPointer->display();
    }
}

template <class DT>
void NovelQueue<DT>::testListJobs() {
    // test the listJobs method
    NovelQueue<CPUJob*>* myNovelQueue = new NovelQueue<CPUJob*>();
    myNovelQueue->enqueue(new CPUJob(1, 1, 1, 1, 1));
    myNovelQueue->enqueue(new CPUJob(2, 2, 2, 2, 2));
    myNovelQueue->enqueue(new CPUJob(3, 3, 3, 3, 3));
    myNovelQueue->enqueue(new CPUJob(4, 4, 4, 4, 4));
    myNovelQueue->listJobs();
}

int main() {

    int n; // number of commands
    std::cin >> n;

    // Instantiate a NovelQueue object for CPUJob Pointers
    NovelQueue<CPUJob*>* myNovelQueue = new NovelQueue<CPUJob*>();

    char command; // Variable to store the command type

    // Variables for job attributes
    int job_id, priority, job_type, cpu_time_consumed, memory_consumed;

    // Variables for modifying a job
    int new_priority, new_job_type, new_cpu_time_consumed, new_memory_consumed;
    int field_index, new_value;
    
    // Variable for the number of positions in the 'Promote' command
    int positions;

    // Variable for the 'Reorder' command
    int attribute_index;

    // Read each command process

    for (int i = 0; i < n; i++) {
        cin >> command;

        switch(command) {
            case 'A': { // Add (Enqueue)
                cin >> job_id >> priority >> job_type >> cpu_time_consumed >> memory_consumed;
                CPUJob* newJob = new CPUJob(job_id, priority, job_type, cpu_time_consumed, memory_consumed);
                // make sure job does not already exist in the queue
                if (myNovelQueue->findJob(job_id) != nullptr) {
                    cout << "Job ID " << job_id << " already exists!" << endl;
                    break;
                } else {
                    myNovelQueue->enqueue(newJob);
                    cout << "Enqueued Job: " << endl;
                    newJob->display();
                    cout << "Jobs after enqueue:" << endl;
                    myNovelQueue->display();
                }
                break;
            } 
            case 'R': { // Remove (Dequeue)
                CPUJob* removedJob = myNovelQueue->dequeue();
                if (removedJob) {
                    cout << "Dequeued Job: " << endl;
                    removedJob->display();
                    delete removedJob;
                    cout << "Jobs after dequeue:" << endl;
                    myNovelQueue->display();
                } else {
                    cout << "Queue is empty" << endl;
                }
                break;
            }
            case 'M': { // Modify
                cin >> job_id >> new_priority >> new_job_type;
                cin >> new_cpu_time_consumed >> new_memory_consumed;
                myNovelQueue->modify(job_id, new_priority, new_job_type, new_cpu_time_consumed, new_memory_consumed);
                cout << "Jobs after modification:" << endl;
                myNovelQueue->display();
                break;
            }
            case 'C': { // Change Job Values
                cin >> job_id >> field_index >> new_value;
                myNovelQueue->change(job_id, field_index, new_value);
                break;
            }
            case 'P': { // Promote 
                cin >> job_id >> positions;
                myNovelQueue->promote(job_id, positions);
                cout << "Jobs after promotion:" << endl;
                myNovelQueue->display();
                break;
            }
            case 'O': { // Reorder
                cin >> attribute_index;
                NovelQueue<CPUJob*>* reorderedQueue = myNovelQueue->reorder(attribute_index);
                reorderedQueue->display();
                NovelQueue<CPUJob*>* temp = myNovelQueue;
                myNovelQueue = reorderedQueue;
                delete temp;
                break;
            }
            case 'D': { // Displays all elements in the queue
                cout << "Displaying all jobs in the queue:" << endl;
                myNovelQueue->display();
                break;
            }
            case 'N': { // Count
                cout << "Number of elements in the queue: " << myNovelQueue->count() << endl;
                break;
            }
            case 'L': { // List Jobs
                cout << "List of jobs sorted by job IDs:" << endl;
                myNovelQueue->listJobs();
                break;
            }
            default: {
                cout << "Invalid command" << endl;
            }
        }
        
    }
    
   delete myNovelQueue;
   return 0;
}

/*
Prompts and Suggestions:
1. How can I implement a sorting algorithm to make sure that my nodes is always sorted by job_id?
Rationale: I asked this because I wanted more ideas on how to properly implement a sorting algorithm for my NodePtrs array. I needed
to make sure that it was always sorted by job_id, and would do so automatically whenever I added or removed a node.
2. Help me implement a merge method to merge a new node with the existing NodePtrs array.
Rationale: I asked this because I needed help implementing a merge method to merge a new node with the existing NodePtrs array. 
I wanted to make sure that the NodePtrs array was always sorted by job_id, but I needed to make sure my program stayed optimized. Writing
a merge method helped me keep the array sorted in O(n) time.
3. Generate a test method for my promote, addNodeptr, removeNodeptr, and reorder methods.
Rationale: I asked this because I wanted to make sure each small piece of my program was working individually as I built it. I needed to build unit tests
to test each piece individually because each part of the program tends to rely on another piece to work properly. These tests helped me ensure that everything
was working properly as I built it.
4. Explain why my dequeue might be causing a segmentation fault/going out of bounds.
Rationale: I had a lot of issues to debug with boundaries and segmentation faults. I needed help understanding why my program was going out of bounds and causing
other things to break, so I had Copilot try to identify areas in my program that could cause these issues. It helped me understand that my logic inside my removeNodePtr
method was wrong and was decrementing the size of the array twice, instead of only once, causing the array to go out of bounds.
5. Implement a helper method to perform a binary search on my nodePtrs array to find the specified job_id.
Rationale: I had Copilot help me build a helper method to perform a binary search on my nodePtrs array to find the specified job_id. Because there were quite a few areas
in my program that I had to reference that array and search the for a speficic job by id, I imeplemnted a helper method that I could use over again that
used binary search to optimize the search time.
6. Implement a helper method to perform an insertion sort on my nodePtrs array to reorder the queue based on a specific attribute.
Rationale: I had Copilot help me build a helper method to perform an insertion sort on my nodePtrs array that reordered the queue based on a specific attribute. Though it doesn't provide
the best overall time complexity, the code and logic for insertion sort is simple and easy to read, which is why I chose to use it over some of the longer algorithms. This made my program concise and easy
for anybody to understand.
7. Help me write a method to display all the elements currently in the queue
Rationale: I asked this because I needed help writing a method to display not the elements in the array, but in the queue itself. Copilot was able to efficiently help me
write a method that did this, which was also helpful for debugging my program.
8. Prompt: How can I implement a method to merge all the elements currently in my nodePtrs array into a new one without the job that was removed?
Rationale: I asked this because I needed help implementing a method to merge all the elements currently in my nodePtrs array into a new one excluding the one job that was dequeued. This was important because
the nodePtrs array needed to stay sorted by job_id, and the element dequeued wasn't always necessarily the one at the front of the array. I had Copilot help me implement almost an inverse merge to complete this.
9. Prompt: How can I implement an algorithm to promote an element in my queue by a certain number of positions?
Rationale: While I was familiar with the algorithms from the linked list class, I was unsure if the exact same logic would carry over to queues because I was having issues with how my logic worked. I had Copilot
help me write similar logic that hepled me maintain the important properties of a queue while promoting an element by a certain number of positions.
10. Prompt: Help me write an overloaded dequeue method that removes a specific job from the queue.
Rationale: I asked Copilot this because I quickly realized that while my dequeue functioned properly, I needed a way to dequeue a specific element from the queue. Copilot helped me realize
the logic for this is very similar to the logic for removeAt in the LinkedList class, and helped me implement it in my program.
11. Prompt: How can I implement a method to reorder the queue based on a specific attribute?
Rationale: I asked this because I needed help implementing a method to reorder the queue based on a specific attribute. I needed to make sure that the queue was always sorted by a specific attribute, and that


Incremental Development: I used Copilot to help me design test cases and test methods that allowed me to test each individual smaller part of my program as I went
before combining multiple parts into one larger method. I started by working my way down the method chart, building enqueue and dequeue and then testing both of them
before building methods to modify the associated array sorted by job_id. I then continued to build methods, such as promote, reorder, and modify. Because I ensured that
my enqueue and dequeue methods worked properly before implementing some of these more complicated methods, the debugging process and testing process for them was a lot easier.
I didn't have to worry about whether or not the issue was with some of the simpler enqueue or dequeue methods, because I had already thoroughly tested them and ensured that they worked properly.
Lastly, I ran all of the given inputs and made sure that the corresponding outputs were correct, and rephrased or restructured my code to make sure that the outputs were matching
as I was running the given input test cases.

Debugging and Testing Plan: For this project, I made sure to test everything at the bottom of the testing pyramid at a unit level before working my way up
and combining multiple methods together. I started by structuring my program with different helper methods, such as findJob, that could be used by multiple different
methods in the future. I then built the methods and utilized Copilot to thoroughly test each one individually using differeng edge cases and individual instances of a new novelQueue
object so that none of the progres would carry over. After I made sure that some of the smaller methods worked together properly, I went on to implement the larger ones.
I really focused on making sure that I was testing things as I went since this program and project were a lot larger than the previous ones, and consequently involved a lot more 
small moving pieces and processes that needed to work together properly. At each level, I used print statements and debug mode to help me identify where different parts of my program were breaking, 
and often I used Copilot to help me refine my design process and make sure that I was implementing everything properly. 

Issues and Resolutions: Because I so thorougly tested my program as I went, I didn't run into many major issues with my program. While that's not to say I didn't have smaller issues,
making sure that everything was working properly as I went helped me identify and fix these issues before they became larger problems. I did have some issues with my dequeue method, most of which involved
boundary errors that I had to resolve by making sure that I was using proper logic when merging or removing elements from the nodePtrs array. I resolved many of these by tracking the size of
the array as I was adding and removing elements, and also by tracking what exactly I was adding or removing from the array as I was working through the program. I encountered a lot of issues with
trying to figure out the logic of promoting an element in the queue. I had to refer back to the original linked list class of removing an element at a specific index and inserting it back at a specific index
because the logic for that was really similar to what needed to be implemented here. I also had to make sure that my add and remove nodePtrs methods were working properly, as everytime that I enqueued or dequeued an element
to promote it or reorder the queue, they would be called and modify the contents of the array. The hardest part in paticular for me was tracking down
where exactly specific issues were happening, because I would get far into the test cases and then something would break. I ended up having to do a lot of work with my modify and reorder methods, as
there were small issues with the logic on both involving enqueueing and dequeueing from the queue itself that was difficult to assess and properly implement.

Verification: The way that I verified that everything in my program was working correctly with the main program was by building small pieces, thoroughly testing them
and then implementing them with other small pieces to make sure that they worked together. I created lots of unit tests as you can see above to make sure that, at the bottom
of the testing pyramid, my program was working correctly so that when I went on to use these pieces in some of the more complicated methods, I could rule out that there were any
issues with them. Along the way, I also made sure to use display methods and print statements to track what was going in and out of my queue, and so that I could make sure
I was able to follow the execution of my methods as they were called. I tried to make everything robust and transparent by implementing modular helper methods
that allowed by code to be pieced together in a way that was efficient and made sense.




*/