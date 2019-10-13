// Created by Christian Pizarro on 10/6/19.
//
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <sys/time.h>
#include <pthread.h>

using namespace std;
                                                    // global variables
vector <vector<string> > max_min_data;              // holds max and min from each thread
vector <map<string, int> > common_data;             // holds maps from each thread
vector <string> merged_results;                     // holds results in a single vector
static int number_of_cores = 4;                     // amount of threads used in terms of cores

void* details(void *pointer)                        // calculates vector with max and min word
{                                                   // pushes it global vector of vectors

    vector <string> *ref =                          // has ref point to where the pointer is pointing to
            (vector <string> *)pointer;
    string max, min, temp;                          // variables created
    int max_length = 0;
    int min_length = INT_MAX;
    vector <string> data;
    map<string, int> common_map;

    for (int i=0; i< ref->size(); i++)              // traverses vector
    {
        if (ref->at(i).length() > max_length)       // updates max_length when it finds a larger word
        {
            max = ref->at(i);
            max_length = ref->at(i).length();
        }
        if (ref->at(i).length() < min_length)       // updates min_length when it finds a smaller word
        {
            min = ref->at(i);
            min_length = ref->at(i).length();
        }
        temp = ref->at(i);
        common_map[temp]++;                         // adds word to map if not already and aggregates its int value
    }

    data.push_back(max);                            // push to local vector
    data.push_back(min);                            // push to local vector
    common_data.push_back(common_map);              // push to global vector of maps
    max_min_data.push_back(data);                   // push to global vector of vectors

}

void merge()                                        // use global variables vectors and maps to get single max
{                                                   // min and common words into a single vector

    string max, min, common, temp;                  // variables created
    int max_length = 0;
    int min_length = INT_MAX;
    int occurrences = 0;
    vector <string> data;
    map<string, int> common_map;
    map<string, int>::iterator it1;                 // cursor for map

    for (int i=0; i < number_of_cores; i++)         // traverses global vector of vectors only at location 0
    {                                               // which holds the largest word

        if                                          // updates max_length word when it finds a larger word
         (max_min_data.at(i).at(0).length() > max_length)
        {
            max = max_min_data.at(i).at(0);
            max_length = max_min_data.at(i).at(0).length();
        }
    }
    for (int i=0; i < number_of_cores; i++)         // traverses global vector of vectors only at location 1
    {                                               // which holds the smallest word

        if                                          // updates max_length word when it finds a larger word
         (max_min_data.at(i).at(1).length() < min_length)
        {
            min = max_min_data.at(i).at(1);
            min_length = max_min_data.at(i).at(1).length();
        }
    }

    for (int i=0; i < number_of_cores; i++)         // number of cores = number of maps = number of threads
    {
        for                                         // combines vector of maps into a single map
         (it1 = common_data.at(i).begin(); it1!=common_data.at(i).end(); ++it1)
        {
            common_map[it1->first] += it1->second;  // adds word if not there already and aggregates corresponding int
        }
    }

    for                                             // traverse map
            (it1 = common_map.begin(); it1!=common_map.end(); ++it1)
    {
        if(it1->second > occurrences)               // updates whenever int value(occurrence) is larger than previous
        {
            common = it1->first;
            occurrences = it1->second;
        }
    }

    merged_results.push_back(max);                  // push to global vector
    merged_results.push_back(min);                  // push to global vector
    merged_results.push_back(common);               // push to global vector
}



int main() {
    double time_taken_read;
    double time_taken_after;
    struct timeval start, end;                      // time variables that hold values in seconds and microseconds.
    fstream file;                                   // file variable that will allow to manipulate files
    vector <string> read_into;                      // vector that will take in words from list.
    string filename = "file.txt";                   // filename of the file
    string word;                                    // temp string variable
    file.open(filename.c_str());                    // opens file, must be within same directory
    vector <pthread_t> threads;                     // vector of threads
    vector <vector<string> > divided_list;          // vector of vectors

    gettimeofday(&start, NULL);                     // sends seconds after linux EPOCH time to start variable

    while (file >> word)                            // extracting words from the file into word variable until
    {                                               // there is no more
        read_into.push_back(word);                  // sends word read from file into vector
    }
    gettimeofday(&end, NULL);                       // sends seconds after linux EPOCH time to end variable

    time_taken_read =                               // takes difference in seconds and converts to micro secs
            (end.tv_sec - start.tv_sec) * 1e6;
    time_taken_read =                               // takes difference in micro secs and converts to secs
            (time_taken_read + (end.tv_usec - start.tv_usec)) * 1e-6;


    gettimeofday(&start, NULL);
    for (int i=0; i < number_of_cores; i++)         // splits read_into into as many vectors as cores
    {
        if (i != number_of_cores-1)
        {
            vector <string> temp(read_into.begin() + (i * read_into.size() / number_of_cores),
                                 read_into.begin() + ((i + 1) * read_into.size() / number_of_cores));
            divided_list.push_back(temp);
        }
        else                                        // may not always divide perfectly this ensure last piece gets
        {                                           // all remaining values via end()
            vector <string> temp(read_into.begin() + (i * read_into.size() / number_of_cores),
                                 read_into.end());
            divided_list.push_back(temp);           // pushes vector to local vectors or vectors
        }
    }

    for (int i=0; i < number_of_cores; i++)         // creates as many threads as cores
    {
        pthread_t tid;                              // thread id
        threads.push_back(tid);                     // push it to vector of threads
        pthread_create                              // creates thread at id specified and runs details function
                                                    // with its list specified
                (&threads.at(i), NULL, details, (void *)&divided_list.at(i));
    }

    for (int i=0; i < number_of_cores; i++)
    {
        pthread_join(threads.at(i), NULL);          // waits for all threads to be completed
    }

    merge();                                        // see merge function


    for (int i=0; i < number_of_cores; i++)         // just to show all longest and shortest words collected
    {                                               // as proof
        cout << "One of the longest words found in a thread is: " <<max_min_data.at(i).at(0)<< endl;
        cout << "One of the shortest words found in a thread is: " <<max_min_data.at(i).at(1)<< endl;
    }


    cout << "The longest word from all threads is: " <<merged_results.at(0)<< endl;
    cout << "The shortest word from all threads is: " <<merged_results.at(1)<< endl;
    cout << "The common word from all threads is: " <<merged_results.at(2)<< endl;


    gettimeofday(&end, NULL); // ends timer after all is complete

    time_taken_after = (end.tv_sec - start.tv_sec) * 1e6;
    time_taken_after = (time_taken_after + (end.tv_usec - start.tv_usec)) * 1e-6;

    cout << "Reading from the text file took: "<<time_taken_read;
    cout<< " seconds!" << endl;
    cout << "The rest of the code took: "<<time_taken_after;
    cout<< " seconds!" << endl;

    return 0;
}

