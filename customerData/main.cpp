//
//  main.cpp
//  customerData
//
//  Created by Allensim on 2018/7/5.
//  Copyright © 2018年 Allensim. All rights reserved.
//

#include <fstream>
#include <string>
#include <iostream>
#include <streambuf>
#include <vector>
#include <string.h>

#include "json/json.h"

using namespace std;

typedef struct {
    string year;
    string hour;
    string line_in;
    string station_in;
    string line_out;
    string station_out;
}customerData;

typedef struct {
    string departureTime;
    string destinationStation;
    string trainType;
    string isLast;
    string notes;
}stationMessage_json;

typedef struct{
    string type;
    string context;
    string sameAs;
    string station;
    string railway;
    string operator_s;
    string railDirection;
    string date;
    vector<vector<stationMessage_json>>  stationMs_jsons;
    string id;
} timeTable_json;

string Read_Str(string filepath)
{
    ifstream infile;
    infile.open(filepath);
    //打开失败，路径不正确
    if(!infile)
        cout << "Open File Fail!" << endl;
    //读取文本内容到字符串
    string readStr((istreambuf_iterator<char>(infile)),istreambuf_iterator<char>());
    
    return readStr;
}

//字符串分割函数
vector<string> split(string str,string pattern)
{
    string::size_type pos;
    vector<string> result;
    str+=pattern;//加分隔符扩展字符串以方便操作，不遗漏最后一组数据等
    int size=str.length();
    
    for(int i=0; i<size; i++)
    {
        pos=str.find(pattern,i);
        if(pos<size)
        {
            string s=str.substr(i,pos-i);
            result.push_back(s);
            i=pos+pattern.size()-1;
        }
    }
    return result;
}

int main()
{
    
    
    //时刻表的json的读取
    string read_string=Read_Str("/Users/allensim/Desktop/timetable_2017_02_01.json");

//    int level_kako=0;
//    vector<int>list_index;
//    for (int i=0; i<read_string.length(); i++) {
//        if (read_string[i] == '[' ) {
//            level_kako++;
////            printf(" [ 位于字符串的第 %d 位,为等 %d 级括号\n",i,level_kako);
//        }
//        if (read_string[i] == ']' ) {
////            printf(" ] 位于字符串的第 %d 位,为等 %d 级括号\n",i,level_kako);
//            if (level_kako == 1) {
//                //不同的车站的分界该处插入分割线
//                list_index.push_back(i);
//            }
//            if (level_kako == 2) {
//                //3分时间表的分割线
//                list_index.push_back(i);
//            }
//            level_kako--;
//        }
//    }
//    for (int  i =0; i<list_index.size(); i++) {
//        printf("%d   ",list_index[i]);
//        read_string.insert(list_index[i]+i+1,"\n");
//    }


    //添加元素，使其成为json数据来解析
    read_string.insert(0, "{\"stationTimeTableList\":\n");
    read_string.append("}");


    Json::Reader reader;
    Json::Value root;

    if (!reader.parse(read_string, root, false))
    {
        std::cout << "failed!\n";
        return -1;
    }



    vector<timeTable_json> jsonList;
    jsonList.clear();
    int count_list=root["stationTimeTableList"].size();
    for (int i = 0 ; i < count_list ; i++) {
        timeTable_json temp_table_json;
        //解析json構造体
        temp_table_json.type.assign(root["stationTimeTableList"][i]["@type"].asString());
        temp_table_json.context.assign(root["stationTimeTableList"][i]["@context"].asString());
        temp_table_json.sameAs.assign(root["stationTimeTableList"][i]["owl:sameAs"].asString());
        temp_table_json.station.assign(root["stationTimeTableList"][i]["odpt:station"].asString());
        temp_table_json.railway.assign(root["stationTimeTableList"][i]["odpt:railway"].asString());
        temp_table_json.operator_s.assign(root["stationTimeTableList"][i]["odpt:operator"].asString());
        temp_table_json.railDirection.assign(root["stationTimeTableList"][i]["odpt:railDirection"].asString());
        temp_table_json.date.assign(root["stationTimeTableList"][i]["dc:date"].asString());
        temp_table_json.id.assign(root["stationTimeTableList"][i]["@id"].asString());

        string arr[3] = {"odpt:weekdays", "odpt:saturdays","odpt:holidays"};
        //json中日付毎に数字を入れる
        for (int j=0; j<3; j++) {
            vector<stationMessage_json> temp_list;
            temp_list.clear();
            Json:: Value timeValue = root["stationTimeTableList"][i][arr[j]];
            int count= timeValue.size();
            for (int k=0; k<count; k++) {
                stationMessage_json temp;
                temp.departureTime.assign(timeValue[k]["odpt:departureTime"].asString());
                temp.destinationStation.assign(timeValue[k]["odpt:destinationStation"].asString());
                temp.trainType.assign(timeValue[k]["odpt:trainType"].asString());
                if (!timeValue[k]["odpt:isLast"].isNull() ) {
                    temp.isLast.assign(timeValue[k]["odpt:isLast"].asString());
                }
                if (!timeValue[k]["odpt:notes"].isNull() ) {
                    temp.notes.assign(timeValue[k]["odpt:notes"].asString());
                }
                temp_list.push_back(temp);
            }
            temp_table_json.stationMs_jsons.push_back(temp_list) ;
        }



        jsonList.push_back(temp_table_json);
    }

    vector<string> dataVec;
    dataVec.clear();
    int countmmm=jsonList.size();
    for (int i=0; i<countmmm; i++) {
//        int t=0;
//        for (int j=0; j<dataVec.size(); j++) {
//            if (jsonList[i].railway.compare(dataVec[j]) == 0) {
//                t=1;
//                break;
//            }
//        }
//        if (t==0) {
//            dataVec.push_back(jsonList[i].railway);
//        }
        
        
        printf("%d  %s \n",i,jsonList[i].railway.c_str());
//        printf("%s \n",jsonList[i].railDirection.c_str());
//        printf("%d %s\n",i,jsonList[i].station.c_str());
    }
    for (int i=0; i<dataVec.size(); i++) {
                printf("%s \n",dataVec[i].c_str());
    }


    return 0;
//
    string retureString;
    string startStation;
    string direction;
    string typeOfDay;
    //Ginza TokyoMetro.Shibuya  TokyoMetro.Asakusa
    //Hibigaya   TokyoMetro.KitaSenju  TokyoMetro.NakaMeguro
    //Tozai  TokyoMetro.NishiFunabashi   TokyoMetro.Nakano
    //Chiyoda   TokyoMetro.Ayase  TokyoMetro.YoyogiUehara
    //Yurakucho  TokyoMetro.ShinKiba  TokyoMetro.Wakoshi
    //Hanzomon  TokyoMetro.Shibuya  TokyoMetro.Oshiage
    //Namboku  TokyoMetro.AkabaneIwabuchi     TokyoMetro.Meguro



    // AoyamaItchome  Shibuya   KitaSenju  NakaMeguro
    startStation.append("odpt.Station:TokyoMetro.Ginza.");
    startStation.append("Ueno");
    direction.append("odpt.RailDirection:TokyoMetro.");
    direction.append("Asakusa");
    //    startStation.append(languageChange(end));


    typeOfDay.assign("odpt:weekdays");


    int count=jsonList.size();
    for (int i=0; i<count; i++) {
        if (startStation.compare(jsonList[i].station) == 0 && direction.compare(jsonList[i].railDirection) == 0) {
            //找到该车站
            vector<stationMessage_json> temp;
            if (typeOfDay.compare("odpt:weekdays") == 0) {
                //找到当天r该站的时间表
                temp=jsonList[i].stationMs_jsons[0];
                for (int j=0; j<temp.size(); j++) {
                    //确认出发时间
                    printf("%s \n",temp[j].departureTime.c_str());
                }
            }
        }
    }



    return 0;
    
    
    
    
    
    /*客户信息的文件处理*/
    string customerDataString;
    vector<string> customerVector;
    vector<customerData> csData;
    vector<customerData> finalData;
    
    finalData.clear();
    
    customerDataString=Read_Str("/Users/allensim/Desktop/sample_od_20170201.txt");
    customerVector=split(customerDataString, "\n");
    vector<string> temp;
    for (int i=0; i<customerVector.size(); i++) {
        temp.clear();
        customerData tempData;
        temp=split(customerVector[i], ",");
        tempData.year.assign(temp[0]);
        tempData.hour.assign(temp[1]);
        tempData.line_in.assign(temp[2]);
        tempData.station_in.assign(temp[3]);
        tempData.line_out.assign(temp[4]);
        tempData.station_out.assign(temp[5]);
        
        finalData.push_back(tempData);
    }
    
    printf("");
    
    return 0;
}
