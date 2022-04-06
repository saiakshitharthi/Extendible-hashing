#include <bits/stdc++.h>
using namespace std;

class Bucket
{
public:
    int capacity;
    vector<int> data;
    int local_depth;
    int no_of_keys;
    vector<int> overflow;
    bool overflow_accessed = 0;
    Bucket(int capacity, int local_depth);
    void insert(int data);
    bool isFull();
    int search(int data);
};

Bucket::Bucket(int cap, int local_dep)
{
    overflow_accessed = 0;
    capacity = cap;
    local_depth = local_dep;
    no_of_keys = 0;
    vector<int> temp_data(cap, 0);
    data = temp_data;
}

void Bucket::insert(int dat)
{

    if (isFull())
    {
        return;
    }
    data[no_of_keys] = dat;
    no_of_keys++;
}
bool Bucket::isFull()
{
    return (no_of_keys == capacity);
}
class Directory
{
public:
    Directory(int global_depth, int bucket_capacity, int M);
    vector<Bucket *> dir;
    int global_depth;
    int bucket_capacity;
    int M;
    void insert(int data);
    void display();
    void printDirectory();
};

Directory::Directory(int glob, int cap, int m)
{
    global_depth = glob;
    bucket_capacity = cap;
    M = m;
    int sz = (1 << glob);
    vector<Bucket *> temp_dir(sz, nullptr);
    dir = temp_dir;

    for (int i = 0; i < sz; i++)
    {
        auto b = new Bucket(cap, global_depth);
        dir[i] = b;
    }
}

void Directory::insert(int dat)
{
    int sz = (1 << global_depth);
    int index = dat % sz;
    if (dir[index]->isFull())
    {
        if (dir[index]->local_depth == global_depth)
        {
            dir[index]->overflow_accessed = 1;
            (dir[index]->overflow).push_back(dat);
            set<Bucket *> bucket_storage;
            for (auto x : dir)
            {
                bucket_storage.insert(x);
            }
            int P = 0, Q = bucket_storage.size();
            for (auto x : bucket_storage)
            {
                if (x->overflow_accessed)
                {
                    P++;
                }
            }
            if (P * 100 < M * Q)
            {
                return;
            }
            vector<int> all_values;
            vector<Bucket *> temp_dir(2 * sz);

            for (int i = 0; i < sz; i++)
            {
                if (!(dir[i]->overflow_accessed))
                {
                    temp_dir[i] = dir[i];
                    temp_dir[i + sz] = dir[i];
                }
                else{
                    for(auto x: dir[i]->data){
                        all_values.push_back(x);
                    }
                    for(auto x: dir[i]->overflow){
                        all_values.push_back(x);
                    }
                    temp_dir[i] = new Bucket(bucket_capacity,(dir[i]->local_depth)+1);
                    temp_dir[i + sz] = new Bucket(bucket_capacity,(dir[i]->local_depth)+1);
                }
            }
            dir = temp_dir;
            global_depth++;
            for(auto x: all_values){
                insert(x);
            }
        }
        else
        {
            vector<int> curvalues;
            for (auto x : dir[index]->data)
            {
                curvalues.push_back(x);
            }
            for (auto x : dir[index]->overflow)
            {
                curvalues.push_back(x);
            }
            curvalues.push_back(dat);
            int current_local_depth = dir[index]->local_depth;
            int first_index = index % (1 << current_local_depth);
            int second_index = first_index + (1 << current_local_depth);
            auto first_pointer = new Bucket(bucket_capacity, current_local_depth + 1);
            auto second_pointer = new Bucket(bucket_capacity, current_local_depth + 1);

            for (int i = 0; i < dir.size(); i++)
            {
                if (i % (1 << (current_local_depth + 1)) == first_index)
                {
                    dir[i] = first_pointer;
                }
                if (i % (1 << (current_local_depth + 1)) == second_index)
                {
                    dir[i] = second_pointer;
                }
            }
            for (auto x : curvalues)
            {
                insert(x);
            }
        }
    }
    else
    {
        dir[index]->insert(dat);
    }
}
void Directory::display()
{
    cout << global_depth << " ";
    set<Bucket *> s;
    for (auto x : dir)
    {
        s.insert(x);
    }
    cout << s.size() << endl;
}
void Directory::printDirectory()
{
    int sz = (1 << global_depth);
    for (int i = 0; i < sz; i++)
    {
        vector<int> binary_rep;
        for (int j = 0; j < 31; j++)
        {
            if ((1 << j) & i)
            {
                binary_rep.push_back(1);
            }
            else
            {
                binary_rep.push_back(0);
            }
        }
        for (int j = global_depth - 1; j >= 0; j--)
        {
            cout << binary_rep[j];
        }
        cout << " ";
        for (int j = 0; j < dir[i]->no_of_keys; j++)
        {
            cout << dir[i]->data[j] << " ";
        }
        cout << "overflow :";
        for (auto x : dir[i]->overflow)
        {
            cout << x << " ";
        }
        cout << endl;
    }
}
int main()
{

    int global_depth, bucket_capacity, M;
    cin >> global_depth >> bucket_capacity >> M;
    Directory d(global_depth, bucket_capacity, M);
    while (1)
    {
        int x;
        cin >> x;
        if (x == 2)
        {
            int y;
            cin >> y;
            d.insert(y);
        }
        else if (x == 5)
        {
            d.display();
            d.printDirectory();
        }
        else if (x == 6)
        {
            break;
        }
        else
        {
            break;
        }
    }
    return 0;
}