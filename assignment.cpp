    #include <bits/stdc++.h>
    using namespace std;

    class Bucket
    {
    public:
        int capacity;
        vector<int> data;
        int local_depth;
        int no_of_keys;

        Bucket(int capacity, int local_depth);
        void insert(int data);
        bool isFull();
        int search(int data);
    };

    Bucket::Bucket(int cap, int local_dep)
    {

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
    int Bucket::search(int dat)
    {

        for (int i = 0; i < no_of_keys; i++)
        {
            if (data[i] == dat)
            {
                return i;
            }
        }
        return -1;
    }
    class Directory
    {
    public:
        Directory(int global_depth, int bucket_capacity);
        vector<Bucket *> dir;
        int global_depth;
        int bucket_capacity;
        vector<Bucket *> ascending;

        bool isPresent(int data);
        void insert(int data);
        void delete_elem(int data);
        void print_bucket(int index);
        void display();
        void printDirectory();
    };

    Directory::Directory(int glob, int cap)
    {
        global_depth = glob;
        bucket_capacity = cap;
        int sz = (1 << glob);
        vector<Bucket *> temp_dir(sz, nullptr);
        dir = temp_dir;

        for (int i = 0; i < sz; i++)
        {
            auto b = new Bucket(cap, global_depth);
            ascending.push_back(b);
            dir[i] = b;
        }
    }

    void Directory::insert(int dat)
    {
        if (global_depth == 20)
        {
            return;
        }
        int sz = (1 << global_depth);
        int index = dat % sz;
        if (dir[index]->isFull())
        {
            if (dir[index]->local_depth == global_depth)
            {

                vector<Bucket *> temp_dir(2 * sz);

                for (int i = 0; i < sz; i++)
                {
                    temp_dir[i] = dir[i];

                    temp_dir[i + sz] = dir[i];
                }
                dir = temp_dir;
                global_depth++;
                insert(dat);
            }
            else
            {
                vector<int> curvalues;
                for (auto x : dir[index]->data)
                {
                    curvalues.push_back(x);
                }
                curvalues.push_back(dat);
                int current_local_depth = dir[index]->local_depth;
                int first_index = index % (1 << current_local_depth);
                int second_index = first_index + (1 << current_local_depth);
                dir[index]->no_of_keys = 0;
                dir[index]->local_depth++;
                auto first_pointer = dir[index];
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
                ascending.push_back(second_pointer);
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
    void Directory::print_bucket(int index)
    {

        for (int i = 0; i < dir[index]->no_of_keys; i++)
        {
            cout << (dir[index]->data)[i] << " ";
        }
        cout << endl;
    }

    bool Directory::isPresent(int dat)
    {
        bool yes = 0;
        int sz = (1 << global_depth);
        yes = ((dir[dat % sz]->search(dat)) != -1) ? 1 : 0;

        return yes;
    }

    void Directory::delete_elem(int dat)
    {
        if (isPresent(dat))
        {
            int sz = (1 << global_depth);
            int ind = dir[dat % sz]->search(dat);

            auto it = (dir[dat % sz]->data).begin() + ind;

            (dir[dat % sz]->data).erase(it);
            dir[dat % sz]->no_of_keys--;
            dir[dat % sz]->data.push_back(0);
        }
        int old_global_depth = 0;

        while ((old_global_depth != global_depth) && global_depth>1)
        {
            int half_sz = (1 << global_depth) / 2;

            for (int i = 0; i < half_sz; i++)
            {
                if (dir[i] != dir[i + half_sz])
                {
                    if (dir[i]->no_of_keys + dir[i + half_sz]->no_of_keys <= bucket_capacity)
                    {

                        for (int j = 0; j < dir[i + half_sz]->no_of_keys; j++)
                        {
                            dir[i]->insert(dir[i + half_sz]->data[j]);
                        }
                        auto it = find(ascending.begin(), ascending.end(), dir[i + half_sz]);
                        ascending.erase(it);
                        dir[i + half_sz] = dir[i];
                        dir[i]->local_depth--;
                    }
                }
            }
            int sz = (1 << global_depth);

            old_global_depth = global_depth;
            sz = (1 << global_depth);
            bool yes = 1;
            for (int i = 0; i < sz / 2; i++)
            {
                if (dir[i] != dir[i + sz / 2])
                {
                    yes = 0;
                }
            }
            if (yes)
            {
                for (int i = 0; i < sz / 2; i++)
                {
                    dir.pop_back();
                }
                global_depth--;
            }
        }
    }

    void Directory::display()
    {
        int noofbuckets = 0;
        int directory_size = (1 << global_depth);
        if (global_depth == 20)
        {
            cout << "Some error" << endl;
            return;
        }
        cout << global_depth << endl;

        set<Bucket *> s;
        for (auto x : dir)
        {
            s.insert(x);
        }
        cout << s.size() << endl;

        for (auto x : ascending)
        {
            cout << (x->no_of_keys) << " " << (x->local_depth) << endl;
        }
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
            cout << endl;
        }
    }

    int main()
    {

        int global_depth, bucket_capacity;

        cin >> global_depth >> bucket_capacity;
        Directory d(global_depth, bucket_capacity);
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
            else if (x == 3)
            {
                int y;
                cin >> y;
                if (d.isPresent(y))
                {
                    cout << "Present" << endl;
                }
                else
                {
                    cout << "Not present" << endl;
                }
            }
            else if (x == 4)
            {
                int y;
                cin >> y;
                d.delete_elem(y);
            }
            else if (x == 5)
            {
                d.display();
            }
            else if (x == 6)
            {
                break;
            }
        }
        d.printDirectory();
        return 0;
    }