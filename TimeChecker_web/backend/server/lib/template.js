var fs = require('fs');
var moment = require('moment');

module.exports = {
    HTML:function(title, body) {
        return `
        <!doctype html>
        <html>
            <head>
                <title>life manager</title>
                <meta charset="utf-8">

                <script type="text/javascript"> 
                    function numkeyCheck(e) { 
                        var keyValue = event.keyCode; 
                        if( ((keyValue >= 48) && (keyValue <= 57)) ) 
                            return true; 
                        else 
                            return false; 
                    } 
                </script>

            </head>
            <body>
                <h1><a href="/">life manager</a></h1>
                <ul>
                    <li><a href="/puttime">puttime</a></li>
                    <li><a href="/selecttable">selecttable</a></li>
                </ul>
                <hr>
                <p>
                    ${body}
                </p>
            </body>
        </html>
        `;
    },

    puttime:function(date) {
        var nowtimeh = moment().format("H");
        var nowtimem = moment().format("mm");
        console.log("date : ", date);
        return `<form action="/puttime_process" method="post">
                    
                    <h3> use time </h3>
                    <select name="category">
                        <option value="else">기타</option>
                        <option value="develop">개발</option>
                        <option value="study">공부</option>
                        <option value="book_reading">책</option>
                        <option value="exercise">운동</option>
                        <option value="work">회사</option>
                        <option value="english">영어</option>
                        <option value="chinese">중국어</option>
                        <option value="work">낮잠</option>
                    </select>
                    <input type="number" maxlength="2" max="23" min="0" onKeyPress="return numkeyCheck(event)" name="starth" value="${nowtimeh}">
                    <input type="number" maxlength="2" max="59" min="0" onKeyPress="return numkeyCheck(event)" name="startm" value="${nowtimem}">
                    :
                    <input type="number" maxlength="2" max="23" min="0" onKeyPress="return numkeyCheck(event)" name="endh" placeholder="종료h">
                    <input type="number" maxlength="2" max="59" min="0" onKeyPress="return numkeyCheck(event)" name="endm" placeholder="종류m">
                    <p>
                    <input type="checkbox" name="except_check">
                    <textarea name="doing" placeholder="description"></textarea>
                    <p>          
                    <input type="hidden" name="date" value="${date}">      
                    <input type="submit">

                </form>
                `
    },

    selecttable:function() {
        var nowdate = moment().format("YYYY-MM-DD");
        return `
        <h2>selecttable</h2>
        <form action="/showtable_process" method="post">
            <input type="date" name="date" value=${nowdate}>
            <p>
            <input type="submit">
        </form>
    
        `
    },

    showDBTable:function(inputs, date){
        var tag = '<table>';
        var i = 0;
        while(i < inputs.length) {
          tag += `
          <tr>
            <td>${inputs[i].id}</td>
            <td>${inputs[i].category}</td>
            <td>${inputs[i].doing}</td>
            <td>${inputs[i].starth}</td>
            <td>${inputs[i].startm}</td>
            <td>${inputs[i].endh}</td>
            <td>${inputs[i].endm}</td>
            <td>${inputs[i].consume}</td>
            <td>${inputs[i].except_check}</td>
            <td>
                <form action="/delete_process" method="post">
                    <input type="hidden" name="id" value="${inputs[i].id}">
                    <input type="hidden" name="date" value="${date}">
                    <input type="submit" value="delete">
                </form>
            </td>
          </tr>
          
          `
          i++;
        }
        tag += '</table>';
        tag += `
            <form action="/confirm_process" method="post">
                <input type="hidden" name="date" value="${date}">
                <input type="submit" value="confirm">
            </form>
        `
        return tag;
    },

    selectSleepTime:function(date) {
        console.log("selectst : ", date);
        var nowtimeh = moment().format("H");
        var nowtimem = moment().format("mm");
        return `
            <form action="/maketable_process" method="post">
                <h3> sleep time </h3>
                <input type="number" maxlength="2" max="23" min="0" onKeyPress="return numkeyCheck(event)" name="starth" value="23">
                <input type="number" maxlength="2" max="59" min="0" onKeyPress="return numkeyCheck(event)" name="startm" value="00">
                :
                <input type="number" maxlength="2" max="23" min="0" onKeyPress="return numkeyCheck(event)" name="endh" value="${nowtimeh}">
                <input type="number" maxlength="2" max="59" min="0" onKeyPress="return numkeyCheck(event)" name="endm" value="${nowtimem}">
                <input type="hidden" name="date" value="${date}">

                <input type="submit" value="submit">
            </form>
        `
    }
    
}