#main_server
'''
main server
'''

from flask import Flask, render_template, request
import os
import pymysql

app = Flask(__name__)

def min_to_hour_minutes(minutes):
	ret = ""
	ret += str(minutes // 60);
	print(ret)
	ret += ':'
	temp_ret = str(minutes % 60)
	if minutes % 60 < 10 :
		ret += '0'
	ret += temp_ret
	return ret

@app.route('/')
def main():
	rt = render_template('index.html')
	print(type(rt))
	print(rt)
	return render_template('index.html')

@app.route('/input')
def input():
    return render_template('input.html')

@app.route('/abc')
def abc():
	return "ABC"

@app.route('/to_engine', methods=['POST'])
def to_engine():
	print("here OK")
	#v = request.form['main2']
	day = request.form.get('day')
	print(day)
	v = request.form.get('time_text')
	print(v)

	f = open("temp.txt", "w")
	f.write(day + '\n')
	f.write(v)
	f.write("\nend")
	f.close()
	os.system("real_web_time_db.exe")
	
	#os.system("timechecker_processing.exe " + day)
	if request.method == 'POST':
		return 'POST, to_engine'

@app.route('/show_data')
def show_data():
	print("in show_data")
	return render_template('show_data.html')

@app.route('/get_recode_data', methods=['POST'])
def get_recode_data():
	print("get_recode_DATA")
	html = "<h1>get_recode_DATA</h1>";
	conn = pymysql.connect(host = '127.0.0.1', user = 'root', password = 'q1w2e3r4!!', db = 'test')
	curs = conn.cursor()

	day = request.form.get('day')
	print(day)

	sql = "select * from RECODE" + day
	try:
		curs_ret = curs.execute(sql)
		print(curs_ret)

		rows = curs.fetchall()
		html += """
		<table>
			<th>id</th>
			<th>work</th>
			<th>start_h</th>
			<th>start_m</th>
			<th>end_h</th>
			<th>end_m</th>
			<th>property</th>
			<th>consume</th>

"""

		for i in rows:
			html += "<tr>"
			for j in i:
				html += "<td>"
				html += str(j) + '\t'
				html += "</td>"
			html += "</tr>"
		html += "</table>"
		conn.close()
		return html
	except:
		return day + "는 없습니다. "
	
@app.route('/get_total_recode_data', methods=['POST'])
def get_total_recode_data():
	print("get_total_recode_DATA")
	html = "<h1>get_total_recode_DATA</h1>";
	conn = pymysql.connect(host = '127.0.0.1', user = 'root', password = 'q1w2e3r4!!', db = 'test')
	curs = conn.cursor()

	sql = "select * from total_recode"
	try:
		curs_ret = curs.execute(sql)
		rows = curs.fetchall()
		html += """
		<table>
			<th>id</th>
			<th>recode_table_num</th>
			<th>A_SUM</th>
			<th>cal_A_SUM</th>
			<th>B_SUM</th>
			<th>cal_B_SUM</th>
			<th>Achievements</th>
			<th>sleeptime</th>
			<th>cal_sleeptime</th>
			<th>cal_hs_sleeptime</th>

"""

		for i in rows:
			html += "<tr>"
			t_cnt = 0
			for j in i:
				print(j)
				html += "<td>"
				html += str(j) + '\t'
				html += "</td>"
				if t_cnt == 2 or t_cnt == 3 or t_cnt == 6:
					html += "<td>"
					html += str(min_to_hour_minutes(j))
					html += "</td>"
				t_cnt += 1
			html += "</tr>"
		html += "</table>"
		conn.close()
		return html
	except:
		return "total_recode에서 에러가 났습니다."

@app.route('/show_time_table', methods=['POST'])
def show_time_table():
	conn = pymysql.connect(host='127.0.0.1', user='root', password="q1w2e3r4!!", db='test')
	curs = conn.cursor()
	day = request.form.get('day')

	html = render_template('time_table_front.html')

	html += """
        <div class="form-group" style="margin:20px;">
            <div>
    """
	html += "<h1>" + str(day) + "의 time table 입니다 </h1>"
	html += """
        </div>
    </div>
	"""
	html += render_template('time_table_middle.html')
	
	sql = "select * from RECODE" + day
	compact_list = []
	#try:
	curs.execute(sql)
	rows = curs.fetchall()

	A_ascii_int = 65
	b_ascii_int = 97
	for x in rows:
		print(x)
		in_com_list = []
		
		temp_str = x[1]
		if ord(temp_str[-1]) == 13 or ord(temp_str[-1]) == 10 :
			temp_str = temp_str[0:-1]
		if ord(temp_str[-1]) == 13 or ord(temp_str[-1]) == 10 :
			temp_str = temp_str[0:-1]
		in_com_list.append(temp_str)		

		start_minute = x[2] * 60 + x[3] - 240
		if start_minute < 0 :
			start_minute += 1440

		in_com_list.append(start_minute)
		in_com_list.append(x[7] + start_minute)
		if x[6] == 'A':
			in_com_list.append(A_ascii_int)
			A_ascii_int += 1
		elif x[6] == 'b':
			in_com_list.append(b_ascii_int)
			b_ascii_int += 1
		else :
			print('A, b만 지원합니다.')
		compact_list.append(in_com_list)
	#except:
	#	return "show_time_table에서 sql에러가 남"
	for x in compact_list:
		for y in x:
			print(y, end=" ")
		print()

	m_html = ""
	m_html += "<tr>"

	for i in range(1440):
		if(i%60 == 0):
			m_html += "<th>"
			m_html += str((i//60 + 4) % 24)
			m_html += "</th>"

		get_success = False
		for x in compact_list:
			if x[1] <= i and i < x[2]:
				get_success = True
				m_html += "<th class="
				m_html += chr(x[3])
				m_html += ">"
				if len(x[0]) > i - x[1]:
					m_html += (x[0][i - x[1]])
				#m_html += chr(x[3])
				m_html += "</th>\n"
				break
		if get_success == False:
			m_html += "<th class=empty"
			m_html += ">"
			m_html += "</th>\n"

		if (i+1)%60 == 0:
			m_html += "</tr>\n<tr>"

	html += m_html
	html += render_template('time_table_back.html')
	return html


if __name__ == '__main__':
    app.run()

