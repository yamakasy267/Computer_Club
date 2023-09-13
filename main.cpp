#include <iostream>
#include <fstream>
#include <regex>
#include <string>
#include <vector>
#include <algorithm>
#include <stdlib.h>

using namespace std;
class ManagerComputerClub
{
private:
    vector<string> input_data;
    string line, user_name;
    int open_club, close_club, quantity_table, count_hour, event_id,event_time_now,number_of_table, last_event_time;
    
    void ReadForFile(string path_file)
    {
        ifstream input_file(path_file);
        if (!input_file.is_open())
        {
            cout<<"Could not open the file"<<endl;
        }
        input_file >> line;
        try
        {
            quantity_table = atoi(line.c_str());
            if (quantity_table < 0)
            {
                cout<<line;
                input_file.close();
                exit(1);
            }
        }
        catch(...)
        {
            cout<<line;
            input_file.close();
            exit(1);
        }


        input_file >> line;
        if(!IsCorectTime(line)){
            cout<<line;
            input_file.close();
            exit(1);
        }
        input_data.push_back(line);
        open_club = atoi(line.substr(0,2).c_str())*60 + atoi(line.substr(3,2).c_str());


        input_file >> line;
        if(!IsCorectTime(line)){
            cout<<line;
            input_file.close();
            exit(1);
        }
        input_data.push_back(line);
        close_club = atoi(line.substr(0,2).c_str())*60 + atoi(line.substr(3,2).c_str());

        input_file >> line;
        try
        {
            count_hour = atoi(line.c_str());
            if (count_hour < 0)
            {
                cout<<line;
                input_file.close();
                exit(1);
            }
        }
        catch(...)
        {
            cout<<line;
            input_file.close();
            exit(1);
        }
        getline(input_file, line);

        while (getline(input_file, line))
        {
            if(line =="")
                continue;
            try
            {
                event_id = atoi(line.substr(6,1).c_str());
                event_time_now = atoi(line.substr(0,2).c_str())*60 + atoi(line.substr(3,2).c_str());
            }
            catch(...)
            {
                cout<<line;
                input_file.close();
                exit(1);
            }

            if(event_id!=2)
            {
                user_name = line.substr(8,line.length());
            }
            else
            {
                int index_space = line.rfind(' ');
                user_name = line.substr(8,index_space-8);
                try
                {
                    number_of_table = atoi(line.substr(index_space+1,1).c_str());
                }
                catch(...)
                {
                    cout<<line;
                    input_file.close();
                    exit(1);
                }
            }

            if(!IsCorectTime(line.substr(0,5)) || !IsCorectName(user_name) || last_event_time>event_time_now ||  number_of_table> quantity_table)
            {
                cout<<line;
                input_file.close();
                exit(1);
            }
            input_data.push_back(line);
            last_event_time = event_time_now;
        }
        input_file.close();
    }
    
    void WriteInConsole()
    {

        vector<string> queue;
        vector<int>time_the_table(quantity_table);
        vector<int>profit_table(quantity_table);
        vector<string> clients_in_club(quantity_table);
        int index_space;

        cout<<input_data[0]<<endl;
        for (auto iter(input_data.begin()+2); iter!=input_data.end();iter++)
        {
            line = *iter;
            cout<<line<<endl;
            event_id = atoi(line.substr(6,1).c_str());
            event_time_now = atoi(line.substr(0,2).c_str())*60 + atoi(line.substr(3,2).c_str());
            
            if(event_id!=2)
            {
                user_name = line.substr(8,line.length());
            }
            else
            {
                index_space = line.rfind(' ');
                user_name = line.substr(8,index_space-8);
            }
            switch (event_id)
            {
                case 1:
                {
                    if(find(clients_in_club.begin(), clients_in_club.end(), user_name) != clients_in_club.end())
                    {
                        cout<<line.substr(0,6)<< "13 YouShallNotPass "<<endl;
                    }
                    else if(event_time_now> close_club || event_time_now< open_club)
                    {
                        cout<<line.substr(0,6)<< "13 NotOpenYet "<<endl; 
                    }
                    else
                    {
                        queue.push_back(user_name);
                    }
                    break;
                }
                case 2:
                {
                    number_of_table = atoi(line.substr(index_space+1,1).c_str());
                    if(clients_in_club[number_of_table-1] != "")
                    {
                        cout<<line.substr(0,6)<< "13 PlaceIsBusy "<<endl;
                        break; 
                    }
                    auto index_table = find(clients_in_club.begin(), clients_in_club.end(), user_name);
                    if(index_table == end(clients_in_club))
                    {
                        auto index_queue = find(queue.begin(), queue.end(), user_name);
                        if(index_queue == end(queue))
                        {
                            cout<<line.substr(0,6)<< "13 ClientUnknown "<<endl;
                            break;
                        }
                        queue.erase(index_queue);
                        clients_in_club[number_of_table-1] = user_name;
                        time_the_table[number_of_table-1] -= event_time_now;
                        profit_table[number_of_table-1] -= event_time_now;
                        break;      
                    }
                    *index_table="";
                    time_the_table[index_table-begin(clients_in_club)]+=event_time_now;
                    profit_table[index_table-begin(clients_in_club)] += event_time_now;
                    if (profit_table[index_table-begin(clients_in_club)]%60 != 0)
                        profit_table[index_table-begin(clients_in_club)] += 60-abs(profit_table[index_table-begin(clients_in_club)]%60);
                    clients_in_club[number_of_table-1] = user_name;
                    time_the_table[number_of_table-1] -= event_time_now;
                    profit_table[number_of_table-1] -= event_time_now;
                    break;
                }
                case 3:
                {
                    if(find(clients_in_club.begin(), clients_in_club.end(), "") != clients_in_club.end())
                    {
                        cout<<line.substr(0,6)<< "13 ICanWaitNoLonger! "<<endl;
                    }
                    else if(queue.size()>quantity_table)
                    {
                        queue.erase(find(queue.begin(), queue.end(), user_name)); // нужно решить что - то с очередью
                        cout<<line.substr(0,6)<< "11 " << user_name<<endl;
                    }
                    break;
                }
                case 4:
                {
                    auto index_table = find(clients_in_club.begin(), clients_in_club.end(), user_name); // - begin(clients_in_club);
                    if(index_table == end(clients_in_club))
                    {
                        auto index_queue = find(queue.begin(), queue.end(), user_name);
                        if(index_queue == end(queue))
                        {
                            cout<<line.substr(0,6)<< "13 ClientUnknown " << user_name<<endl;
                            break;
                        }
                        queue.erase(index_queue);
                        break;      
                    }
                    if(!queue.empty())
                    {
                        *index_table=queue[0];
                        queue.erase(begin(queue));
                        cout<<line.substr(0,6)<< "12 " << *index_table<<" "<<index_table-begin(clients_in_club)+1<<endl; 
                        time_the_table[index_table-begin(clients_in_club)] -= event_time_now;
                        profit_table[index_table-begin(clients_in_club)] -= event_time_now;
                    }
                    else{
                        *index_table="";
                    }
                    time_the_table[index_table-begin(clients_in_club)] += event_time_now;
                    profit_table[index_table-begin(clients_in_club)] += event_time_now;
                    if (profit_table[index_table-begin(clients_in_club)]%60 != 0)
                        profit_table[index_table-begin(clients_in_club)] += 60-abs(profit_table[index_table-begin(clients_in_club)]%60);
                    break;
                }
                default:
                    break;
            }

        }
        
        if(!clients_in_club.empty())
        {
            sort(begin(clients_in_club), end(clients_in_club));
            for (vector<string>::iterator client = clients_in_club.begin(); client!=clients_in_club.end();++client)
            {
                if (*client == "")
                    continue;
                cout<<input_data[1]<< " 11 " << *client<<endl;
                time_the_table[client - begin(clients_in_club)] += close_club;
                profit_table[client-begin(clients_in_club)] += close_club;
                    if (profit_table[client-begin(clients_in_club)]%60 != 0)
                        profit_table[client-begin(clients_in_club)] += 60-abs(profit_table[client-begin(clients_in_club)]%60);
            }
        }
        if(!queue.empty())
        {
            sort(begin(queue), end(queue));
            for (const auto& client : queue)
            {
                cout<<input_data[1]<< " 11 " << client<<endl;
            }
        }
        cout<<input_data[1]<<endl;
        int profit;
        for (int i =0 ;i<quantity_table;i++)
            {
                
                cout<<i+1<<" "<<profit_table[i]/60*count_hour<<" "<<ConvertIntToTime(time_the_table[i])<<endl;
            }

    }
    
    bool IsCorectTime(string str)
    {
        bool flag = regex_match(str,regex("([01][0-9]|2[0-3]):([0-5][0-9])"));
        return flag;
    }

    bool IsCorectName(string str)
    {
        bool flag = regex_match(str, regex("^[a-z0-9_-]+$"));
        return flag;
    }

    string ConvertIntToTime(int time)
    {
        int hour = time/60;
        int minuts = time%60;
        string Times;
        if (hour < 10)
            Times = "0" + to_string(hour) + ":";
        else
            Times = to_string(hour)+":";
        
        if (minuts < 10)
            Times += "0" + to_string(minuts);
        else
            Times += to_string(minuts);
        
        return Times;
    }

public:
    ManagerComputerClub(string path_file)
    {
        number_of_table = 0;
        last_event_time=0;
        ReadForFile(path_file);
        WriteInConsole();
    }

};

int main(int argc, char** argvs)
{

    ManagerComputerClub manager(argvs[1]);
    return 0;
}
