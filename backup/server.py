#sample servertest

from flask import Flask, render_template, request
app = Flask(__name__)
@app.route('/')
def main():
	
	return '안뇽'

if __name__ == '__main__':
    app.run(host='0.0.0.0')