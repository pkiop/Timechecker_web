temp = ""

for i in range(0, 24):
	temp += "<tr><th>"
	if i < 10 :
		temp += '0'
	temp += str(i)
	temp += "</tr></th>"

print(temp)