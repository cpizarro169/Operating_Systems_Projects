// Created by Christian Pizarro on 10/6/19.
//
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <sys/time.h>

using namespace std;

vector <string> details             // return vector with max word, min word and common word from input vector
        (vector <string> ref)
{
    string max, min, common, temp;
    int max_length = 0;
    int min_length = INT_MAX;
    int occurrences = 0;
    map<string, int> mymap;
    map<string, int>::iterator it1; // cursor for map
    vector <string> b;
    for
      (int i=0; i<ref.size(); i++)  // traverse through vector
    {
        if                          // updates max_length when it finds a larger word
         (ref.at(i).length() > max_length)
        {
            max = ref.at(i);
            max_length = ref.at(i).length();
        }
        if                          // updates min_length when it finds a smaller word
         (ref.at(i).length() < min_length)
        {
            min = ref.at(i);
            min_length = ref.at(i).length();
        }
        mymap[ref.at(i)]++;         // adds word to map if not already and aggregates its int value
    }

    for                             // traverse map
     (it1 = mymap.begin(); it1!=mymap.end(); ++it1)
    {
        if                          // updates whenever int value(occurrence) is larger than previous
         (it1->second > occurrences)
        {
            common = it1->first;
            occurrences = it1->second;
        }
    }
    b.push_back(max);
    b.push_back(min);
    b.push_back(common);
    return b;                       // pushed values and returns vector
}

int main() {
    double time_taken_read;
    double time_taken_after;
    struct timeval start, end;      // time variables that hold values in seconds and microseconds.
    fstream file;                   // file variable that will allow to manipulate files
    vector <string> read_into;      // vector that will take in words from list.
    string filename = "file.txt";   // filename of the file
    string word;                    // temp string variable
    file.open(filename.c_str());    // opens file, must be within same directory

    gettimeofday(&start, NULL);     // sends seconds after linux EPOCH time to start variable

    while (file >> word)            // extracting words from the file into word variable until there is no more
    {
        read_into.push_back(word);  // sends word read from file into vector
    }
    gettimeofday(&end, NULL);       // sends seconds after linux EPOCH time to end variable

    time_taken_read =               // takes difference in seconds and converts to micro secs
            (end.tv_sec - start.tv_sec) * 1e6;
    time_taken_read =               // takes difference in micro secs and converts to secs
            (time_taken_read + (end.tv_usec - start.tv_usec)) * 1e-6;


    gettimeofday(&start, NULL);
    vector <string> answers =       // see details function
            details(read_into);


    cout <<                         // display vector data
         "The longest word in the list is: " <<answers.at(0)<< endl;
    cout <<
         "The shortest word in the list is: " <<answers.at(1)<< endl;
    cout <<
         "The most common word in the list is: " <<answers.at(2)<< endl;

    gettimeofday(&end, NULL);

    time_taken_after =
            (end.tv_sec - start.tv_sec) * 1e6;
    time_taken_after = (
            time_taken_after + (end.tv_usec - start.tv_usec)) * 1e-6;

    cout <<                         // display time collected
         "Reading from the text file took: "<<time_taken_read
         << " seconds!" << endl;
    cout << "The rest of the code after took: "<<time_taken_after
         << " seconds!" << endl;

    return 0;
}
