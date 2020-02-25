#main_server
'''
main server
'''

from flask import Flask, render_template, request
import os
import pymysql

app = Flask(__name__)

@app.route('/')
def main():
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
	f.close()
	os.system("real_web_time_db.exe")
	os.system("timechecker_processing.exe " + day)
	if request.method == 'POST':
		return 'POST, to_engine'

@app.route('/show_data')
def show_data():
	print("in show_data")
	return render_template('show_data.html')

@app.route('/get_data', methods=['POST'])
def get_data():
	print("get_DATA")
	html = "<h1>get_DATA</h1>";
	conn = pymysql.connect(host = '127.0.0.1', user = 'root', password = 'q1w2e3r4!!', db = 'test')
	curs = conn.cursor()

	day = request.form.get('day')
	print(day)

	sql = "select * from a" + day
	try:
		curs_ret = curs.execute(sql)
		print(curs_ret)

		rows = curs.fetchall()

		for i in rows:
			for j in i:
				html += str(j) + '\t'
			html += '<br>'

		conn.close()
		return html
	except:
		return day + "는 없습니다. "
	
	


if __name__ == '__main__':
    app.run()

