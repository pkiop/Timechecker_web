#pragma once

struct times {
	int start_hours;
	int start_minutes;
	int end_hours;
	int end_minutes;

	void DB_print_times() {
		cout << start_hours << ':' << start_minutes << '~';
		cout << end_hours << ':' << end_minutes;
	}

	string get_like_real_time() {
		string ret;
		if (start_hours < 10) {
			ret += '0';
		}
		ret += to_string(start_hours);
		ret += ':';
		if (start_minutes < 10) {
			ret += '0';
		}
		ret += to_string(start_minutes);
		ret += '~';
		if (end_hours < 10) {
			ret += '0';
		}
		ret += to_string(end_hours);
		ret += ':';
		if (end_minutes < 10) {
			ret += '0';
		}
		ret += to_string(end_minutes);
		return ret;
	}

	int get_time_consume() {
		int ret = 0;
		ret = end_minutes - start_minutes;
		ret += (end_hours - start_hours) * 60;
		return ret;
	}
};

struct time_doing {
	times time;
	string doing;
	int time_consume;

	time_doing(times t, string l_doing, int tc) : time(t), doing(l_doing), time_consume(tc) {}
	string get_like_real_time() {
		return time.get_like_real_time();
	}
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

	times get_sleep_time() {
		return sleep_time;
	}

	vector<time_doing>& get_recodes(char type) {
		if (type == 'A') {
			return A_recodes;
		}
		if (type == 'b') {
			return b_recodes;
		}
		cerr << "error : A or b 만 가능합니다" << endl;
		vector<time_doing> null;
		return null;
	}

	void set_sleep_time(int start_MS_number, int end_MS_number) { // MS : minute second 합친 숫자 네자리
		cout << start_MS_number << ' ' << end_MS_number << endl;
		sleep_time.start_hours = start_MS_number / 100;
		sleep_time.start_minutes = start_MS_number % 100;
		sleep_time.end_hours = end_MS_number / 100;
		sleep_time.end_minutes = end_MS_number % 100;
		return;
	}

	void set_recodes(int start_MS_number, int end_MS_number, string doing, char types) { // MS : minute second 합친 숫자 네자리
		times temp;
		int time_consume = 0;
		temp.start_hours = start_MS_number / 100;
		temp.start_minutes = start_MS_number % 100;
		temp.end_hours = end_MS_number / 100;
		temp.end_minutes = end_MS_number % 100;
		time_consume = temp.get_time_consume();
		time_doing td_temp(temp, doing, time_consume);
		if (types == 'A') {
			A_recodes.push_back(td_temp);
		}
		else if (types == 'b') {
			b_recodes.push_back(td_temp);
		}
		else {
			cerr << "지원하지 않는 type 입니다. " << endl;
			return;
		}

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

