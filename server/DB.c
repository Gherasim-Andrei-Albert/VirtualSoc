//#include <string>
#include <iostream>
#include <my_global.h>
#include <mysql.h>
#undef min
#undef max
#include <map>
#include <vector>
#include <stdio.h>
using namespace std;

typedef map<string, string> DBrow;
typedef vector<DBrow> DBrows;
typedef map<string, string> DBselection;

typedef vector<string> DBfieldProps;
typedef map<string, DBfieldProps> DBfields;
typedef map<string, DBfields> DBschema;

typedef map<string,string> DBupdateSet;


MYSQL* conn;

DBschema schema;
void DBdefSchema(DBschema schemaDef){
    schema = schemaDef;
}

void DBcreate(string DBname){

    if(mysql_query(conn,("CREATE DATABASE " + DBname).c_str())){
        fprintf(stderr,"%s\n",mysql_error(conn));
    }
    mysql_query(conn,("USE " + DBname).c_str());

    string query = "";
    for(auto tableIt = schema.begin(); tableIt != schema.end();++tableIt){
        query += "CREATE TABLE " + tableIt->first
            + "(\n\tID INT NOT NULL AUTO_INCREMENT,\n";
        auto fields = tableIt->second;
        for(auto field = fields.begin(); field != fields.end(); ++field){
            query += "\t";
            query += field->first + " ";
            auto props = field->second;
            bool nullable = false;
            for(auto fieldProp = props.begin(); fieldProp!= props.end(); ++fieldProp){
                if(*fieldProp == "nullable")
                    nullable = true;
                else
                    query += *fieldProp + " ";
            }
            if(!nullable){
                query +="NOT NULL";
            }
            query+=",\n";
        }
        query+="PRIMARY KEY(ID))";
        if(mysql_query(conn,query.c_str())){
            fprintf(stderr,"%s\n",mysql_error(conn));
        }
        query = "";
    }
}

void DBconn(char* host, char* user, char* pass, char* DB){

    conn = mysql_init(NULL);
    if(conn==NULL){
        fprintf(stderr,"%s\n",mysql_error(conn));
    }
    
    if( mysql_real_connect(conn, host, 
        user, pass, DB, 0, NULL, 0)){
            fprintf(stderr,"%s\n",mysql_error(conn));
    }
}


vector<string> DBfind(string table, DBselection selection, string column){

    string query = "SELECT " + column
        + " FROM " + table + " WHERE ";
    for (auto it = selection.begin(); it != selection.end(); ++it)
    {
        //cout<<it->first<<endl;
        //cout<<table<<endl;
        if(it->second == "NULL")
            query += string(it->first) + " IS NULL AND ";
        else
        if(it->first == "ID" || schema[table][it->first][0] == "INT")
            query += string(it->first) + "=" 
                + string(it->second) + " AND ";
        else
            query += string(it->first) + "='" 
                + string(it->second) + "' AND ";
    }

    for(int i=1;i<=5;++i)
        query.pop_back();
    cout<<query<<endl;
        
    if(mysql_query(conn, query.c_str())){
        fprintf(stderr,"%s\n",mysql_error(conn));
    }

    MYSQL_RES* result = mysql_store_result(conn);
    if(mysql_num_rows(result) != 0){
        vector<string> rows = {};
        while(MYSQL_ROW row = mysql_fetch_row(result)){
            if(row[0]==NULL)
                rows.push_back("NULL");
            else
                rows.push_back(string(row[0]));
        }
        return rows;
    }
    return {};
}

DBrows DBfind(string table, DBselection selection, vector<string> columns){

    string query = "SELECT ";
    for(auto it=columns.begin();it!=columns.end();++it){
        query+=(*it)+',';
    }
    query.pop_back();
    query += " FROM " + table + " WHERE ";
    for (auto it = selection.begin(); it != selection.end(); ++it)
    {
        //cout<<it->first<<endl;
        //cout<<table<<endl;
        if(it->second == "NULL")
            query += string(it->first) + " IS NULL AND ";
        else
        if(it->first == "ID" || schema[table][it->first][0] == "INT")
            query += string(it->first) + "=" 
                + string(it->second) + " AND ";
        else
            query += string(it->first) + "='" 
                + string(it->second) + "' AND ";
    }

    for(int i=1;i<=5;++i)
        query.pop_back();
    cout<<query<<endl;
        
    if(mysql_query(conn, query.c_str())){
        fprintf(stderr,"%s\n",mysql_error(conn));
    }

    MYSQL_RES* result = mysql_store_result(conn);
    if(mysql_num_rows(result) != 0){
        DBrows rows = {};
        while(MYSQL_ROW row = mysql_fetch_row(result)){
            DBrow dbRow;
            for(int i=0;i<columns.size();++i){
                if(row[i]==NULL)
                    dbRow.insert({columns[i], "NULL"});
                else
                    dbRow.insert({columns[i], string(row[i])});
            }
            rows.push_back(dbRow);
        }
        return rows;
    }
    return {};
}

string DBfindOne(string table, DBselection selection, string column){
    vector<string> rows = DBfind(table, selection, column);
    if(rows.size() == 0){
        return {};
    }
    return rows[0];
}

string DBfindOneValue(string table, DBselection selection, string column, string def){
    string row = DBfindOne(table, selection, column);
    if(row == ""){
        return def;
    }
    return row;
}

string DBfindOneValue(string table, DBselection selection, string column){
    string row = DBfindOne(table, selection, column);
    return row;
}

DBrow DBfindOne(string table, DBselection selection, vector<string> columns){
    DBrows rows = DBfind(table, selection, columns);
    if(rows.size() == 0){
        return {};
    }
    return rows[0];
}

int DBfindID(string table,DBselection selection){
    return atoi(
        DBfindOneValue(
            table,selection,"ID","0"
            ).c_str());
}

bool DBexists(string table, DBselection selection){
    string row = DBfindOne(table, selection, "'selected'");
    if(row == ""){
        return false;
    }
    return true;
}

void DBadd(string table, DBselection selection){
    string query = "INSERT INTO " + table + "(";
    for (auto it = selection.begin(); it != selection.end(); ++it)
    {//cout<<string(it->first)<<endl;
        query += string(it->first) + ",";
    }
query.pop_back();
    query += ") VALUES( ";
    for (auto it = selection.begin(); it != selection.end(); ++it)
    {//cout<<string(it->second)<<endl;
        if(it->first == "ID" || schema[table][it->first][0] == "INT" 
            || it->second == "NULL")
            query += string(it->second) + ",";
        else
        if(schema[table][it->first][0] == "DATETIME" 
            && it->second == "NOW")
            query += "NOW(),";
        else{
            query += "'" + string(it->second) + "',";
        }
    }
    query.pop_back();
    query+=")";
    //cout<<query<<endl;

    if(mysql_query(conn, query.c_str())){
        fprintf(stderr,"%s\n",mysql_error(conn));
    }
}

MYSQL* GetDBconn(){
    return conn;
}

void DBupdate(string table, DBselection selection, DBupdateSet updateSet){
    string query = "UPDATE "+table+" SET ";
    for (auto it = updateSet.begin(); it != updateSet.end(); ++it)
    {
        if(it->first == "ID" || schema[table][it->first][0] == "INT")
            query+= it->first + "=" + it->second + ",";
        else
            query+= it->first + "='" + it->second + "',";
    }
    query.pop_back();
    query+= " WHERE ";
    for (auto it = selection.begin(); it != selection.end(); ++it)
    {
        if(it->first == "ID" || schema[table][it->first][0] == "INT")
            query += string(it->first) + "=" 
                + string(it->second) + " AND ";
        else
            query += string(it->first) + "='" 
                + string(it->second) + "' AND ";
    }

    for(int i=1;i<=5;++i)
        query.pop_back();
        
    if(mysql_query(conn, query.c_str())){
        fprintf(stderr,"%s\n",mysql_error(conn));
    }
}

int DBnextID(string table){
    string query = "SELECT IFNULL(max(ID),0) AS maxID FROM "+table;
    if(mysql_query(conn, query.c_str())){
        fprintf(stderr,"%s\n",mysql_error(conn));
    }
    return atoi(mysql_fetch_row((mysql_store_result(conn)))[0])+1;
}

int DBnextValue(string table, string column){
    string query = "SELECT IFNULL(max("+column+"),0) AS maxVal FROM "+table;
    if(mysql_query(conn, query.c_str())){
        fprintf(stderr,"%s\n",mysql_error(conn));
    }
    return atoi(mysql_fetch_row((mysql_store_result(conn)))[0])+1;
}

void DBdelete(string table, DBselection selection){
    string query = "DELETE FROM "+ table + " WHERE ";
    for (auto it = selection.begin(); it != selection.end(); ++it)
    {
        //cout<<it->first<<endl;
        //cout<<table<<endl;
        if(it->first == "ID" || schema[table][it->first][0] == "INT")
            query += string(it->first) + "=" 
                + string(it->second) + " AND ";
        else
            query += string(it->first) + "='" 
                + string(it->second) + "' AND ";
    }

    for(int i=1;i<=5;++i)
        query.pop_back();
    cout<<query<<endl;
        
    if(mysql_query(conn, query.c_str())){
        fprintf(stderr,"%s\n",mysql_error(conn));
    }

}

/*
sql injection
*/