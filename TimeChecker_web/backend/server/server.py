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

	f = open(day + ".txt", "w")
	f.write(v)
	f.close()
	os.system("time_checker_engine.exe " + day)
	if request.method == 'POST':
		return 'POST'

if __name__ == '__main__':
    app.run()

