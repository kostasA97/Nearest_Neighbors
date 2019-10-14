#include "lib/Library.h"
#include "lib/LSH_Functions.h"
#include "lib/Helper_Functions.h"
#include "lib/HashTable.h"

using namespace std;

int main(int argc, char* argv[]){
    if(argc != 3) {
      cout << "We need input_file AND query_file!" << endl;
      return -1;
    }
    /* variable declaration | k = 4 default value */
    int error_code, k = 4, L = 5;
    /* vectors for the data and query points */
    vector<vector<int>> dataset;
    vector<vector<int>> searchset;

    /* read data set and query set and load them in vectors */
    error_code = Read_input_files(&dataset, &searchset, argv[1], argv[2]);
    if (error_code == -1) return -1;

    /* do brute force to find actual NNs */
#ifdef BRUTE_FORCE
    brute_force(&dataset, &searchset);
#endif

    /* d-dimensional vectors */
    int d = dataset[0].size();
    /* compute window for all hash tables */
    //int w = 4*compute_window(dataset);
    int w = 36;
    /* Size of Hash Table */
    int TableSize = dataset.size() / 8;
    HashTable *MyHashTable[L];
    /* TODO: loop for L */
    /* vector containing (k,d) shifts */
    vector<vector<int>> s;
    /* H of size (k, dataset.size()) */
    vector<vector<int>> hash_functions;
    /* projections of data */
    vector<vector<int>> a_projects;
    /* internal vector H for pushing */
    vector<int> H;
    /* amplified hash */
    vector <int> amplified_g;

    /* loop for L, to create L hash tables */
    for (int l = 0; l < L; l++) {
        /* generate the random shifts */
        generate_shifts(&s, w, d, k);

        /* ----------------------- DATA SET -------------------------------*/
        /* loop for K */
        for (int i = 0; i < k; i++) {
            a_projects.clear();
            projections(&a_projects, dataset, &(s[i]), w, d);

            H.clear();
            compute_hash(&H, a_projects, d, k, w);
            hash_functions.push_back(H);
        } /* end for */
        /* compute the amplified hashes for every item */
        amplify_hash(&amplified_g, &hash_functions, k);

        /* Now that we have the hash codes, lets put them in the hash table
        *  Insert all items inside the Hash Table */
        MyHashTable[l] = new HashTable(TableSize);
        for (int i = 0; i < dataset.size(); i++) {
            cout << "Insert" << amplified_g[i] << endl;
            MyHashTable[l]->Insert(amplified_g[i], dataset[i]);
        }

        /* ------------------------ SEARCH SET ----------------------------*/
        /* do the same for the queries, and put them inside the hash table */
        /* loop for K */
        for (int i = 0; i < k; i++) {
            a_projects.clear();
            projections(&a_projects, searchset, &(s[i]), w, d);

            H.clear();
            compute_hash(&H, a_projects, d, k, w);
            hash_functions.push_back(H);
        } /* end for */
        /* compute the amplified hashes for every item */
        amplified_g.clear();
        amplify_hash(&amplified_g, &hash_functions, k);

        /* calculate approximate nearest neighbors */
        vector<vector<int>>** ANN = new vector<vector<int>>* [searchset.size()];
        for (int i = 0; i < searchset.size(); i++) {
            ANN[i] = MyHashTable[l]->Search_Neighbors(amplified_g[i]);
        }

        for (int i = 0; i < searchset.size(); i++) {
            cout << "Query " << i << ": \t Neighbors: " << ANN[i]->size() << endl;
        }
        getchar();
    }

    return 0;
}