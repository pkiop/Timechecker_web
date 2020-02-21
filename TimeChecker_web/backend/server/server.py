#main_server
'''
main server
'''

from flask import Flask, render_template, request
import os

app = Flask(__name__)


@app.route('/')
def hello():
    return "Hello World!"

@app.route('/login2', methods=['POST'])
def login2():
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
	if request.method == 'POST':
		return 'POST'

if __name__ == '__main__':
    app.run()

