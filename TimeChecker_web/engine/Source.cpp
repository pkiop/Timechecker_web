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
	times sleep_time;
	vector<time_doing> A_recodes;
	vector<time_doing> b_recodes;
public:
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
void init(mytimes& mytime, MYSQL& conn, MYSQL* connection);



int main() {
	mytimes mytime;
	MYSQL *connection = NULL, conn;
	init(mytime, conn, connection);

	return 0;
}

// 좋은 MYsql cpp 예제 블로그
// cpp로 안전하게 연결하게 : https://potatoggg.tistory.com/62
// Mysql 기초 : https://m.blog.naver.com/PostView.nhn?blogId=wlsdml1103&logNo=221160038986&proxyReferer=https%3A%2F%2Fwww.google.com%2F
void init(mytimes& mytime, MYSQL& conn, MYSQL* connection) {
	const char* DB_USER = "root";
	const char* DB_PASS = "q1w2e3r4!!";
	const char* DB_HOST = "127.0.0.1";
	const char* DB_NAME = "test";
	const int DB_PORT = 3306;
	int query_stat;

	mysql_init(&conn);
	connection = mysql_real_connect(&conn, DB_HOST, DB_USER, DB_PASS, DB_NAME, DB_PORT, NULL, 0);

	if (connection == NULL) {
		cout << stderr << "Mysql connection error : " << mysql_error(&conn) << endl;
		return;
	}
	// mysql_real_connect(&mysql, host, user, passwd, dbname, unix_socket, unit client_flag)
	/*query_stat = mysql_query(connection, "select * from login");
	if (query_stat != 0) {
		cout << stderr << "Mysql query error : " << mysql_error(&conn) << endl;
		return;
	}*/

	file_read(mytime);
}

void file_read(mytimes& mt) {
	string s;
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