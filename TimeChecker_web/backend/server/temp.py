temp = ""

for j in range(6):
	for i in range(0, 10):
		temp += "<th>"
		temp += "&nbsp"
		temp += str(i)
		temp += "</th>"
print(temp)
# for i in range(0, 24):
# 	temp += """<tr>
# 				<th>"""
# 	if i < 10 :
# 		temp += '0'
# 	temp += str(i)
# 	temp += """</th>
# 			</tr>
# 			"""

# print(temp)