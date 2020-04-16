#main_server
'''
main server
'''

from flask import Flask, render_template, request, redirect
import os
import pymysql
import sys

In_host = ""
In_user = ""
In_password = ""
In_db = ""
app = Flask(__name__)

def min_to_hour_minutes(minutes):
	ret = ""
    ret += str(minutes // 60)
	ret += ':'
	temp_ret = str(minutes % 60)
	if minutes % 60 < 10 :
		ret += '0'
	ret += temp_ret
	return ret

def make_time_consume(start_time_h ,start_time_m, end_time_h, end_time_m):
	temp = int(start_time_h[0])
	print(type(temp))
	print(temp)
	temp = int('0')
	print(type(temp))
	print(temp)
	start_cal_h = 60 * (10 * (int(start_time_h[0]) - int('0')) + (int(start_time_h[1]) - int('0')))
	print(start_cal_h) 

	start_cal_m = 10 * (int(start_time_m[0]) - int('0')) + (int(start_time_m[1]) - int('0'))
	print(start_cal_m)
	end_cal_h = 60 * (10 * (int(end_time_h[0]) - int('0')) + (int(end_time_h[1]) - int('0')))
	end_cal_m = 10 * (int(end_time_m[0]) - int('0')) + (int(end_time_m[1]) - int('0'))

	ret = end_cal_h + end_cal_m - start_cal_h - start_cal_m 
	return ret

@app.route('/')
def main():
	rt = render_template('header.html')
	rt += render_template('index.html')
	return rt

@app.route('/input')
def input():
	rt = render_template('header.html')
	rt += render_template('input.html')
	return rt

@app.route('/many_put', methods=['POST'])
def many_put():
	print("server.py : many_put")
	day = request.form.get('day')
	v = request.form.get('time_text')
	html = day + '<p>' + v
	print(day)
	print(v)
	data = []
	temp_data = ""
	for x in v:
		temp_data += x
		if x == '\n':
			data.append(temp_data)
			temp_data = ""

	for x in data:
		print(x)
	return html


@app.route('/show_data')
def show_data():
	rt = render_template('header.html')
	rt += render_template('show_data.html')
	return rt

@app.route('/get_recode_data', methods=['POST'])
def get_recode_data():
	print("get_recode_DATA")
	html = "<h1>get_recode_DATA</h1>";
	conn = pymysql.connect(host = In_host, user = In_user, password = In_password, db = In_db)
	curs = conn.cursor()

	day = request.form.get('day')

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
		conn.close()
		return day + "는 없습니다. "
	
@app.route('/get_total_recode_data', methods=['POST'])
def get_total_recode_data():
	print("get_total_recode_DATA")
	conn = pymysql.connect(host = In_host, user = In_user, password = In_password, db = In_db)
	curs = conn.cursor()

	sql = "select * from total_recode"
	try:
		curs_ret = curs.execute(sql)
		rows = curs.fetchall()
		html = render_template('header.html')
		html += "<h1>get_total_recode_DATA</h1>";

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
				html += "<td>"
				html += str(j) + '\t'
				html += "</td>"
				if t_cnt == 2 or t_cnt == 3 or t_cnt == 6:
					html += "<td>"
					html += str(min_to_hour_minutes(j))
					html += "</td>"
				t_cnt += 1
			html += "</tr>"
		html += "</table></body></html>"
		conn.close()
		return html
	except:
		conn.close()
		return "total_recode에서 에러가 났습니다."

@app.route('/show_time_table', methods=['POST'])
def show_time_table():
	conn = pymysql.connect(host = In_host, user = In_user, password = In_password, db = In_db)
	curs = conn.cursor()
	day = request.form.get('day')
	html = render_template('header.html')
	html += render_template('time_table_front.html')

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

@app.route('/new_show_time')
def new_show_time():
	rt = render_template('header.html')
	rt += render_template('seperate_data_input.html')
	return rt

@app.route('/new_show_time_table', methods=['POST'])
def new_show_time_table():
	print("new_show")
	conn = pymysql.connect(host = In_host, user = In_user, password = In_password, db = In_db)
	curs = conn.cursor()
	
	day = '200304' #로그인 시에 이미 정보를 안다고 가정 
	state = request.form.get('state')
	if state == "공부":
		state = 'A'
	if state == "그 외":
		state = 'b'
	start_time_h = request.form.get('start_time_h')
	start_time_m = request.form.get('start_time_m')
	end_time_h = request.form.get('end_time_h')
	end_time_m = request.form.get('end_time_m')
	doing = request.form.get('doing')
	html = state + ' ' + start_time_h + ' ' + start_time_m + ' ' + end_time_h + ' ' + end_time_m + ' ' + doing
	consume_time = make_time_consume(start_time_h, start_time_m, end_time_h, end_time_m)

	sql = "INSERT INTO RECODE" + day
	sql += "(workname, start_time_h, start_time_m, end_time_h, end_time_m, property, time_consume)"
	sql += " values('"
	sql += doing
	sql += "', "
	sql += start_time_h
	sql += ", "
	sql += start_time_m
	sql += ", "
	sql += end_time_h
	sql += ", "
	sql += end_time_m
	sql += ", '"
	sql += state
	sql += "', "
	sql += str(consume_time)
	sql += ");"

	sql2 = "SELECT A_SUM, B_SUM, Achievements from total_recode"
	sql2 += " where recode_table_num = '"
	sql2 += day
	sql2 += "';"

	sql3_A_SUM = "UPDATE total_recode set A_SUM = '"
	sql3_B_SUM = "UPDATE total_recode set B_SUM = '"
	sql3_Achievements = "UPDATE total_recode set Achievements = '"
	sql3_last = "' where recode_table_num = "
	sql3_last += day

	try:
		curs.execute(sql)
		conn.commit()
		print("execute 성공 ")
		print(sql2)
		curs.execute(sql2)
		rows = curs.fetchall()
		print(type(rows))
		print(type(rows[0]))
		print(type(rows[0][0]))
		print(rows)
		conn.commit()
		print("STate" + state)
		if state == 'A':
			sql3_A_SUM += str(rows[0][0] + consume_time)
			sql3_A_SUM += sql3_last
			curs.execute(sql3_A_SUM)
			temp_str = rows[0][2]
			print(temp_str)
			for x in temp_str:
				print(ord(x))

			if ord(temp_str[-1]) == 13 or ord(temp_str[-1]) == 10 :
				temp_str = temp_str[0:-1]
			if ord(temp_str[-1]) == 13 or ord(temp_str[-1]) == 10 :
				temp_str = temp_str[0:-1]

			print(temp_str)
			sql3_Achievements += str(temp_str + ' ' + doing)
			sql3_Achievements += sql3_last
			curs.execute(sql3_Achievements)

		elif state == 'b':
			sql3_B_SUM += str(rows[0][1] + consume_time)
			sql3_B_SUM += sql3_last
			curs.execute(sql3_B_SUM)
		else:
			print("A도 아니고 B 도 아니고")
		conn.commit()

	except:
		conn.close()
		return day + "table 이 없습니다. 먼저 만드세요"

	conn.close()
	return redirect('/') 


if __name__ == '__main__':
    f = open("DBaccessInfo.txt", "r")
    xx = f.readline()
    print(xx)
    xx = f.readline()
    xxx = xx.split()
    In_host = xxx[0]
    In_user = xxx[1]
    In_password = xxx[2]
    In_db = xxx[3]
    app.run(host='0.0.0.0')

