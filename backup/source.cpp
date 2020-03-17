#include <iostream>
#include <string>
#include <algorithm>
#include <fstream>
#include <vector>

#include <mysql_connection.h>

using namespace std;
#include "times_data_class.h"

#pragma comment(lib, "libmySQL.lib")
fstream in("temp.txt");

void file_read(mytimes& mt);
MYSQL* init(mytimes& mytime, MYSQL& conn, MYSQL* connection);

//recode tables
MYSQL* make_db_recode_table(mytimes& mytime, MYSQL& conn, MYSQL* connection);
string* query_st_CREATE_recode_table(string table_name);
string query_st_USE_DATABASE(string database_name);
string* query_st_INSERT_RECODE_INTO(time_doing&, string, char);
MYSQL* insert_recodes_data(mytimes& mytime, MYSQL& conn, MYSQL* connection);

MYSQL* insert_total_recodes_data(mytimes& mt, MYSQL& conn, MYSQL* connection);
string* query_st_INSERT_TOTAL_RECODE_INTO(string table_name, int A_sum, int B_sum, string Achievements, string sleeptime, string cal_sleeptime);


int main() {
	cout << "ENGINE start ! " << endl;
	mytimes mytime;
	MYSQL conn;
	MYSQL* connection = NULL;
	connection = init(mytime, conn, connection);
	if (connection == NULL) {
		cout << "connection fail" << endl;
		return 0;
	}
	connection = make_db_recode_table(mytime, conn, connection);
	if (connection == NULL) {
		cout << "connection fail" << endl;
		return 0;
	}
	connection = insert_recodes_data(mytime, conn, connection);
	if (connection == NULL) {
		cout << "connection fail" << endl;
		return 0;
	}
	connection = insert_total_recodes_data(mytime, conn, connection);
	if (connection == NULL) {
		cout << "connection fail" << endl;
		return 0;
	}
	mysql_close(connection);
	return 0;
}

// 좋은 MYsql cpp 예제 블로그
// cpp로 안전하게 연결하게 : https://potatoggg.tistory.com/62
// Mysql 기초 : https://m.blog.naver.com/postview.nhn?blogid=wlsdml1103&logno=221160038986&proxyreferer=https%3a%2f%2fwww.google.com%2f
MYSQL* init(mytimes& mytime, MYSQL& conn, MYSQL* connection) {
	const char* DB_USER = "root";
	const char* DB_PASS = "q1w2e3r4!!";
	const char* DB_HOST = "127.0.0.1";
	const char* DB_NAME = "test";
	const int DB_PORT = 3306;

	mysql_init(&conn);
	connection = mysql_real_connect(&conn, DB_HOST, DB_USER, DB_PASS, DB_NAME, DB_PORT, NULL, 0);
	if (connection == NULL) {
		cout << stderr << "Mysql connection error : " << mysql_error(&conn) << endl;
		return connection;
	}
	mysql_query(connection, "set session character_set_connection=euckr");
	mysql_query(connection, "set session character_set_results=euckr");
	mysql_query(connection, "set session character_set_client=euckr");
	file_read(mytime);
	return connection;
}

void file_read(mytimes& mt) {
	string s;
	getline(in, s); // 오늘날짜
	mt.set_today(s);

	getline(in, s); // 수면 시간
	mt.set_sleep_time(stoi(s.substr(0, 4)), stoi(s.substr(5, 9)));

	while (s != "end") {
		getline(in, s); // 기록들
		if (s == "") {
			continue;
		}
		cout << "s[0] is " << s[0] << endl;
		switch (s[0]) {
		case 'A':
			mt.set_recodes(stoi(s.substr(2, 6)), stoi(s.substr(7, 11)), s.substr(12, s.size()), 'A');
			break;
		case 'b':
			mt.set_recodes(stoi(s.substr(2, 6)), stoi(s.substr(7, 11)), s.substr(12, s.size()), 'b');
			break;
		case 'e':
			cout << "finish" << endl;
			break;
		default:
			cout << "???" << endl;
			break;
		}
	}
	mt.DB_All_time_data_read();
}
MYSQL* make_db_recode_table(mytimes& mt, MYSQL& conn, MYSQL* connection) {
	string* query_string = query_st_CREATE_recode_table(mt.get_today());
	cout << *query_string;
	int query_stat = mysql_query(connection, (*query_string).c_str());
	if (query_stat != 0) {
		cout << stderr << "Mysql query error : " << mysql_error(&conn) << endl;
		return connection;
	}
	cout << "CREATE TABLE ^_^" << endl;
	delete query_string;
	return connection;
}

////////////recodes table //////////////////
string* query_st_CREATE_recode_table(string table_name) {
	string* make_query_string = new string;
	*make_query_string += "CREATE TABLE RECODE";
	*make_query_string += table_name;
	*make_query_string += "(";
	*make_query_string += "id INT NOT NULL AUTO_INCREMENT PRIMARY KEY, ";
	*make_query_string += "workname VARCHAR(50) NOT NULL, ";
	*make_query_string += "start_time_h int NOT NULL, ";
	*make_query_string += "start_time_m int NOT NULL, ";
	*make_query_string += "end_time_h int NOT NULL, ";
	*make_query_string += "end_time_m int NOT NULL, ";
	*make_query_string += "property VARCHAR(2) NOT NULL, ";
	*make_query_string += "time_consume int NOT NULL ";
	*make_query_string += ") DEFAULT CHARSET=utf8;";
	return make_query_string;
}

string query_st_USE_DATABASE(string database_name) {
	string make_query_string;
	make_query_string += "USE ";
	make_query_string += database_name;
	make_query_string += ";";
	return make_query_string;
}

MYSQL* insert_recodes_data(mytimes& mt, MYSQL& conn, MYSQL* connection) {
	vector<time_doing> A_data = mt.get_recodes('A');
	vector<time_doing> B_data = mt.get_recodes('b');
	int query_stat = mysql_query(connection, "set names euckr;"); // 한글이 안될때
	if (query_stat != 0) {
		cout << stderr << "Mysql query error : " << mysql_error(&conn) << endl;
		return connection;
	}
	for (auto x : A_data) {
		string* query_string;
		query_string = query_st_INSERT_RECODE_INTO(x, mt.get_today(), 'A');
		cout << "\n\n하나하나확인 " << endl;
		for (int i = 0; i < (*query_string).size(); ++i) {
			cout << (*query_string)[i];
		}
		cout << endl;
		cout << "INSERT : " << *query_string << endl;
		int query_stat = mysql_query(connection, (*query_string).c_str());
		if (query_stat != 0) {
			cout << stderr << "Mysql query error : " << mysql_error(&conn) << endl;
			return connection;
		}
		delete query_string;
	}
	for (auto x : B_data) {
		string* query_string = new string;
		query_string = query_st_INSERT_RECODE_INTO(x, mt.get_today(), 'b');
		cout << "INSERT : " << *query_string << endl;
		int query_stat = mysql_query(connection, (*query_string).c_str());
		if (query_stat != 0) {
			cout << stderr << "Mysql query error : " << mysql_error(&conn) << endl;
			return connection;
		}
		delete query_string;
	}
	return connection;
}

string* query_st_INSERT_RECODE_INTO(time_doing& x, string table_name, char type) {

	string* make_query_string = new string("INSERT INTO RECODE");
	const char* temp = x.doing.c_str();
	cout << *make_query_string << "   check::!!!:!:!:\n\n!" << endl;

	//*make_query_string = "INSERT INTO A";
	*make_query_string += table_name;
	*make_query_string += "(workname, start_time_h, \nstart_time_m, \nend_time_h, \nend_time_m,\n property,\n time_consume)";
	*make_query_string += " values\n(";
	*make_query_string += "\n\"";
	*make_query_string += temp;
	*make_query_string += "\n\"\n , "; // new line 안하면 자꾸 짤려서 어쭐수 없음 server에서 긁어서 html로 띄울땐 \n 안나오는것 확인
	*make_query_string += ('0' + x.time.start_hours / 10);
	*make_query_string += ('0' + x.time.start_hours % 10);
	*make_query_string += ", ";
	*make_query_string += '0' + x.time.start_minutes / 10;
	*make_query_string += '0' + x.time.start_minutes % 10;
	*make_query_string += ",\n ";
	*make_query_string += '0' + x.time.end_hours / 10;
	*make_query_string += '0' + x.time.end_hours % 10;
	*make_query_string += ",\n";
	*make_query_string += '0' + x.time.end_minutes / 10;
	*make_query_string += '0' + x.time.end_minutes % 10;
	*make_query_string += ",\n'";
	*make_query_string += type;
	*make_query_string += "', '";
	*make_query_string += to_string(x.time_consume).c_str();
	*make_query_string += "');";
	cout << "LAST : " << (*make_query_string) << endl;

	return make_query_string;
}

///////////recodes table end/////////////////



MYSQL* insert_total_recodes_data(mytimes& mt, MYSQL& conn, MYSQL* connection) {
	vector<time_doing> A_data = mt.get_recodes('A');
	vector<time_doing> B_data = mt.get_recodes('b');
	int query_stat = mysql_query(connection, "set names euckr;"); // 한글이 안될때
	if (query_stat != 0) {
		cout << stderr << "Mysql query error : " << mysql_error(&conn) << endl;
		return connection;
	}

	int A_sum = 0;
	string achievements;
	for (auto x : A_data) {
		A_sum += x.time_consume;
		achievements += x.doing;
		achievements += ' ';
	}

	int B_sum = 0;
	for (auto x : B_data) {
		B_sum += x.time_consume;
	}

	string table_name = mt.get_today();
	string sleeptime = mt.get_sleep_time().get_like_real_time();
	string cal_sleeptime = to_string(mt.get_sleep_time().get_time_consume());

	string* query_string;
	query_string = query_st_INSERT_TOTAL_RECODE_INTO(table_name, A_sum, B_sum, achievements, sleeptime, cal_sleeptime);
	query_stat = mysql_query(connection, (*query_string).c_str());
	if (query_stat != 0) {
		cout << stderr << "Mysql query error : " << mysql_error(&conn) << endl;
		return connection;
	}
	delete query_string;
	return connection;
}

string* query_st_INSERT_TOTAL_RECODE_INTO(string tablename, int A_sum, int B_sum, string Achievements, string sleeptime, string cal_sleeptime) {

	string* make_query_string = new string("INSERT INTO TOTAL_RECODE");
	*make_query_string += " (recode_table_num, \nA_sum, \nB_sum, \nAchievements,\n sleeptime,\n cal_sleeptime)";
	*make_query_string += " values\n(";
	*make_query_string += "\n\"";
	*make_query_string += tablename;
	*make_query_string += "\n\"\n , '"; // new line 안하면 자꾸 짤려서 어쭐수 없음 server에서 긁어서 html로 띄울땐 \n 안나오는것 확인
	*make_query_string += to_string(A_sum);
	*make_query_string += "', '";
	*make_query_string += to_string(B_sum);
	*make_query_string += "', \"\n";
	*make_query_string += Achievements;
	*make_query_string += "\n\"\n, '";
	*make_query_string += sleeptime;
	*make_query_string += "', '";
	*make_query_string += cal_sleeptime;
	*make_query_string += "');";
	cout << "LAST : " << (*make_query_string) << endl;

	return make_query_string;
}
