#include <iostream>
#include <fstream>
#include <ctime>
#include <string>
using namespace std;

bool CompareTime(char* time1, char* time2)
{
    time_t tm1, tm2;
    struct tm *timeptr, t1, t2;
    if(strftime(time1, "%H, %M", &t1)== NULL)
        cout<<"strptime failed"<<endl;
    if(strftime(time2, "%H, %M", &t2)== NULL)
        cout<<"strptime failed"<<endl;
    
    tm1 = mktime(&t1);
    tm2 = mktime(&t2);
    return difftime(tm1,tm2) > 0.0 ? 1 : -1;
}


int main(int argc, char** argvs){

    string line;
    char* open_club;
    char* close_club;
    unsigned int quantity_table, count_hour;
    ifstream input_file("C:\\MyProject\\practics\\input.txt");
    //ifstream input_file(argvs[1])
    if (input_file.is_open())
    {
        input_file >> quantity_table;
        input_file.read(open_club, 5);
        input_file.read(close_club, 5);
        input_file >> count_hour;
        while (getline(input_file, line))
        {
            cout<< line<<endl;
        }
    }
    else
    {
        cout<<"Don`t open file"<<endl;
    }
    input_file.close(); 
    return 0;
}
