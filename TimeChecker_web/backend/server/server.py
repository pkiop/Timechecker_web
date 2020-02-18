#main_server
'''
main server
'''

from flask import Flask, render_template, request
app = Flask(__name__)


@app.route('/')
def hello():
    return "Hello World!"


@app.route('/login', methods=['GET'])
def login():
	print("here OK")
	#v = request.form['main2']
	v = request.args.get('main2')
	print(v)
	if request.method == 'GET':
		return 'asd'

@app.route('/login2', methods=['POST'])
def login2():
	print("here OK")
	#v = request.form['main2']
	v = request.args.get('main2')
	print(v)
	if request.method == 'POST':
		return 'POST'

if __name__ == '__main__':
    app.run()

