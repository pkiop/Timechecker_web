#include <mysql.h>
#include <iostream>
#include <string>
#include <algorithm>
#include <fstream>
#include <vector>
using namespace std;

#pragma comment(lib, "libmySQL.lib")
fstream in("temp.txt");

struct times {
	int start_minutes;
	int start_seconds;
	int end_minutes;
	int end_seconds;

	void DB_print_times() {
		cout << start_minutes << ':' << start_seconds << '~';
		cout << end_minutes << ':' << end_seconds;
	}
};
struct time_doing {
	times time;
	string doing;

	time_doing(times t, string l_doing) : time(t), doing(l_doing) {}
	
};

class mytimes {
	string today;
	times sleep_time;
	vector<time_doing> A_recodes;
	vector<time_doing> b_recodes;
public:
	void set_today(string today_in) {
		today = today_in;
		return;
	}

	string get_today() {
		return today;
	}

	vector<time_doing>& get_A_recodes() {
		return A_recodes;
	}

	void set_sleep_time(int start_MS_number, int end_MS_number) { // MS : minute second 합친 숫자 네자리
		cout << start_MS_number << ' ' << end_MS_number << endl;
		sleep_time.start_minutes = start_MS_number / 100;
		sleep_time.start_seconds = start_MS_number % 100;
		sleep_time.end_minutes = end_MS_number / 100;
		sleep_time.end_seconds = end_MS_number % 100;
		return;
	}

	void set_A_recodes(int start_MS_number, int end_MS_number, string doing) { // MS : minute second 합친 숫자 네자리
		times temp;
		temp.start_minutes = start_MS_number / 100;
		temp.start_seconds = start_MS_number % 100;
		temp.end_minutes = end_MS_number / 100;
		temp.end_seconds = end_MS_number % 100;

		time_doing td_temp(temp, doing);
		A_recodes.push_back(td_temp);
		return;
	}

	void set_b_recodes(int start_MS_number, int end_MS_number, string doing) { // MS : minute second 합친 숫자 네자리
		times temp;
		temp.start_minutes = start_MS_number / 100;
		temp.start_seconds = start_MS_number % 100;
		temp.end_minutes = end_MS_number / 100;
		temp.end_seconds = end_MS_number % 100;
		time_doing td_temp(temp, doing);
		b_recodes.push_back(td_temp);
		return;
	}

	void DB_print_sleep_time() {
		cout << __FUNCTION__ << "  line : " << __LINE__ << endl;
		sleep_time.DB_print_times();
		cout << endl;
	}
	void DB_print_A_recodes() {
		cout << __FUNCTION__ << "  line : " << __LINE__ << endl;
		for (auto x : A_recodes) {
			x.time.DB_print_times();
			cout << ' ' << x.doing << endl;
		}
	}
	void DB_print_B_recodes() {
		cout << __FUNCTION__ << "  line : " << __LINE__ << endl;
		for (auto x : b_recodes) {
			x.time.DB_print_times();
			cout << ' ' << x.doing << endl;
		}
	}
	void DB_All_time_data_read() {
		DB_print_sleep_time();
		DB_print_A_recodes();
		DB_print_B_recodes();
	}


};

void file_read(mytimes& mt);
MYSQL* init(mytimes& mytime, MYSQL& conn, MYSQL* connection);
MYSQL* make_db_table(mytimes& mytime, MYSQL& conn, MYSQL* connection);
string query_st_CREATE_TABLE(string table_name);
string query_st_USE_DATABASE(string database_name);
string query_st_INSERT_INTO(time_doing&, string);
MYSQL* insert_A_recodes_data(mytimes& mytime, MYSQL& conn, MYSQL* connection);

int main() {
	mytimes mytime;
	MYSQL conn;
	MYSQL* connection = NULL;
	connection = init(mytime, conn, connection);
	if (connection == NULL) {
		cout << "connection fail" << endl;  
		return 0;
	}
	connection = make_db_table(mytime, conn, connection);
	connection = insert_A_recodes_data(mytime, conn, connection);
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
	
	int query_stat;
	string query_string;
	mysql_init(&conn);
	connection = mysql_real_connect(&conn, DB_HOST, DB_USER, DB_PASS, DB_NAME, DB_PORT, NULL, 0);
	if (connection == NULL) {
		cout << stderr << "Mysql connection error : " << mysql_error(&conn) << endl;
		return connection;
	}

	query_string = query_st_USE_DATABASE(DB_NAME); 
	query_stat = mysql_query(connection, query_string.c_str());
	if (query_stat != 0) {
		cout << stderr << "Mysql query error : " << mysql_error(&conn) << endl;
		return connection;
	}

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
		cout << "s[0] is " << s[0] << endl;
		switch (s[0]) {
		case 'A':
			mt.set_A_recodes(stoi(s.substr(2, 6)), stoi(s.substr(7, 11)), s.substr(12, s.size()));
			break;
		case 'b':
			mt.set_b_recodes(stoi(s.substr(2, 6)), stoi(s.substr(7, 11)), s.substr(12, s.size()));
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

MYSQL* make_db_table(mytimes& mt, MYSQL& conn, MYSQL* connection) {
	string query_string = query_st_CREATE_TABLE(mt.get_today());
	cout << query_string;
	int query_stat = mysql_query(connection, query_string.c_str());
	if (query_stat != 0) {
		cout << stderr << "Mysql query error : " << mysql_error(&conn) << endl;
		return connection;
	}
	cout << "CREATE TABLE ^_^" << endl;
	return connection;
}

string query_st_CREATE_TABLE(string table_name) {
	string make_query_string;
	make_query_string += "CREATE TABLE A";
	make_query_string += table_name;
	make_query_string += "(";
	make_query_string += "id INT NOT NULL AUTO_INCREMENT PRIMARY KEY, ";
	make_query_string += "workname VARCHAR(50) NOT NULL, ";
	make_query_string += "start_time_min int NOT NULL, ";
	make_query_string += "start_time_sec int NOT NULL, ";
	make_query_string += "end_time_min int NOT NULL, ";
	make_query_string += "end_time_sec int NOT NULL, ";
	make_query_string += "property VARCHAR(2) NOT NULL";
	make_query_string += ") DEFAULT CHARSET=utf8;";
	return make_query_string;
}

string query_st_USE_DATABASE(string database_name) {
	string make_query_string;
	make_query_string += "USE ";
	make_query_string += database_name;
	make_query_string += ";";
	return make_query_string;
}

MYSQL* insert_A_recodes_data(mytimes& mt, MYSQL& conn, MYSQL* connection) {
	vector<time_doing> A_data = mt.get_A_recodes();
	for (auto x : A_data) {
		string query_string = query_st_INSERT_INTO(x, mt.get_today());
		int query_stat = mysql_query(connection, query_string.c_str());
		if (query_stat != 0) {
			cout << stderr << "Mysql query error : " << mysql_error(&conn) << endl;
			return connection;
		}
	}
	return connection;
}

string query_st_INSERT_INTO(time_doing& x, string table_name) {
	string make_query_string;
	make_query_string += "INSERT INTO A";
	make_query_string += table_name;
	make_query_string += "(workname, start_time_min, start_time_sec, end_time_min, end_time_sec, property)";
	make_query_string += " VALUES(\"";
	make_query_string += x.doing;
	make_query_string += "\", ";
	make_query_string += '0' + x.time.start_minutes / 10;
	make_query_string += '0' + x.time.start_minutes % 10;
	make_query_string += ", ";
	make_query_string += '0' + x.time.start_seconds / 10;
	make_query_string += '0' + x.time.start_seconds % 10;
	make_query_string += ", ";
	make_query_string += '0' + x.time.end_minutes / 10;
	make_query_string += '0' + x.time.end_minutes % 10;
	make_query_string += ", ";
	make_query_string += '0' + x.time.end_seconds / 10;
	make_query_string += '0' + x.time.end_seconds % 10;

	make_query_string += ", 'A');";
	cout << "INSERT : " << make_query_string << endl;
	return make_query_string;
}
