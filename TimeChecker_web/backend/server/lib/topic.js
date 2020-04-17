var template = require('./template');
var qs = require('querystring');
var db = require('./db');
const request_lib = require('request');
var tempdate = 'dump';
var dbgetdata = 'dump';
puttime_consume_calculation = function(ssh, ssm, eeh, eem) {
    var sh = Number(ssh);
    var sm = Number(ssm);
    var eh = Number(eeh);
    var em = Number(eem);
    if(eh < sh) {
        eh += 24; // 00시 넘었을 때 보정하기 위함.
    }
    if(eh > sh && sm > em) {
        em += 60;
        eh -= 1;  
    }
    
    var ret = (eh - sh) * 60;
    ret += (em - sm);
    return ret;
}

nicetime_calculate = function(inputs) {
    console.log('inputs : ', inputs);
    var total_consume = 0;
    var i = 0;
    while(i < inputs.length){
        if(inputs[i].except_check == 0) {
            total_consume += inputs[i].consume;
        }
        i += 1;
    }
    return total_consume;
}

module.exports = {
    home:function(request, response) {
        var title = '라매';
        var description = 'Hello, 여긴 main page';
        var html = template.HTML(title, description);
        response.writeHead(200);
        response.end(html);
    },
    
    page:function(request, response) {
        var title = 'Welcome';
        var description = 'Hello, Node.js - not undefined';
        var html = `
            <h1>${title}</h1>
            <h2>${description}</h2>
        `
        response.writeHead(200);
        response.end(html);
    },
    
  /*  puttime:function(request, response) {
        var title = 'Put time';
        var body = template.puttime();
        var html = template.HTML(title, body);
        response.writeHead(200);
        response.end(html);
    },
*/
    puttime_process:function(request, response) {
        var body = '';
        request.on('data', function(data) { // event driven programing 용도. 중간에 데이터 너무 길면 짤려서 이렇게.
            body = body + data;
        });
        request.on('end', function() { // 다 받았다.
            console.log("전형적인 body read : ", body);
            var post = qs.parse(body);
            console.log("전형적인 post read : ", post);

            var date_only_number = post.date;
            var except_check = '';

            if(post.except_check === 'on') { except_check = '1'; }
            else { except_check = '0'; }

            var temp_querystr = 'INSERT INTO day' + date_only_number;
            db.query(temp_querystr + ` (category, doing, starth, startm, endh, endm, consume, except_check) VALUES(?,?,?,?,?,?,?,?)`,[post.category, String(post.doing), String(post.starth), String(post.startm), String(post.endh), String(post.endm), String(puttime_consume_calculation(post.starth, post.startm, post.endh, post.endm)), except_check],
                function(error, result) {
                if(error) {
                    if(error.errno === 1146) { // table 없음.
                        var create_query_string = 'CREATE TABLE day' + date_only_number;
                        db.query(create_query_string + ` (
                            id INT PRIMARY KEY AUTO_INCREMENT,
                            category VARCHAR(32) NOT NULL,
                            doing text NOT NULL,
                            starth INT NOT NULL,
                            startm INT NOT NULL,
                            endh INT,
                            endm INT,
                            consume INT NOT NULL,
                            except_check INT NOT NULL
                            )`, function(error2, result2){
                            if(error2) {
                                throw error2;
                            }
                            db.query(temp_querystr + ` (category, doing, starth, startm, endh, endm, consume, except_check) VALUES(?,?,?,?,?,?,?,?)`,[post.category, String(post.doing), String(post.starth), String(post.startm), String(post.endh), String(post.endm), String(puttime_consume_calculation(post.starth, post.startm, post.endh, post.endm)), except_check],
                            function(error3, result3) {
                                if(error3) {
                                    throw error3;
                                }
                                response.writeHead(302, {Location: `/`});
                                response.end();    
                             });
                        });
                    }
                    else {
                        throw error;
                    }
                }
                console.log("puttime_process -> redirect");
                response.writeHead(302, {Location: `/showNupdatetable?date=${date_only_number}`});
                response.end();  
            });
        });
    },

    selecttable:function(request, response) {
        var title = 'show table';
        var body = template.selecttable();
        var html = template.HTML(title, body);
        response.writeHead(200);
        response.end(html);
    },
    
    showNupdatetable:function(request,response,queryData) {
        var title = 'show N update table';
        var body = template.puttime(queryData.date);
        body += '<hr>';
        db.query(`SELECT * FROM day` + queryData.date, function(error, results) {
            if(error) {
                throw error;
            }
            body += template.showDBTable(results, queryData.date);
            var html = template.HTML(title, body);
            response.writeHead(200);
            response.end(html);
        });
    },
    
    refresh_db:function(date) {
        console.log("refresh_db date = ", date);
        db.query(`SELECT * FROM day` + date, function(error, results) {
            if(error) {
                throw error;
            }
        });
        console.log("just end");
    }, 

 
    showtable_process:function(request, response) {
        console.log("showtable:process");
        var title = 'show table';
        var body = '';
        request.on('data', function(data) { // event driven programing 용도. 중간에 데이터 너무 길면 짤려서 이렇게.
            body = body + data;
        });
        request.on('end', function() {
            var post = qs.parse(body);
 
            var date_only_number = post.date[2] + post.date[3] + post.date[5] + post.date[6] + post.date[8] + post.date[9]; 
            console.log("showtable_process post : ", post.date); 
            db.query(`SELECT * FROM day` + date_only_number, function(error, results) {
                if(error) {
                    if(error.errno === 1146) {
                        console.log("redirection to selectSleeptime");
                        response.writeHead(302, {Location: `/selectSleepTime?date=${date_only_number}`});
                        response.end();
                        console.log("endendend");
                        return;
                    }
                    else {
                        throw error;
                    }
                }
                else {
                    dbgetdata = results;
                    console.log("redirection to showNupdatetable");
                    response.writeHead(302, {Location: `/showNupdatetable?date=${date_only_number}`});
                    response.end();    

                }
            });  
            
        });
    },

    delete_process:function(request, response) {
        var body = '';
        request.on('data', function(data){
            body = body + data;
        });
        request.on('end', function(){
            var post = qs.parse(body);
            var id = post.id;
            var date_only_number = post.date[2] + post.date[3] + post.date[5] + post.date[6] + post.date[8] + post.date[9]; 
            db.query(`DELETE from day` + date_only_number + ` where id=?`, [post.id], 
                function(error, results){
                if(error) {
                    throw error;
                }
                response.writeHead(302, {Location: `/`});
                response.end();
            });
        });
    },

    confirm_process:function(request, response) {
        var body = '';
        request.on('data', function(data){
            body = body + data;
        });
        request.on('end', function(){
            var post = qs.parse(body);
            var id = post.id;
            var date_only_number = post.date[2] + post.date[3] + post.date[5] + post.date[6] + post.date[8] + post.date[9]; 
            var date_only_number__year = post.date[2] + post.date[3];
            var date_only_number__month = post.date[5] + post.date[6];
            var date_only_number__day = post.date[8] + post.date[9];
            
            db.query(`SELECT * from day` + date_only_number, 
                function(error, results){
                if(error) {
                    throw error;
                }

                var nicetime = nicetime_calculate(results);
                var body = template.showDBTable(results);
                body += '<a href="/">confirm완료</a>';

                db.query(`INSERT INTO totalrecodes (daykey, year, month, day, sleep, nicetime) VALUES(?,?,?,?,?,?)`,[date_only_number, String(date_only_number__year), String(date_only_number__month), String(date_only_number__day), String(post.sleep), String(nicetime)], 
                function(error2, result2){
                    if(error2) {
                        throw error2;
                    }
                    
                    html = template.HTML('confirm', body);
                    response.writeHead(200);
                    response.end(html); 
                });
            });
        });
    },

    selectSleepTime:function(request, response, queryData) {

        var body = '';
        var title = 'show table';
        var body = template.selectSleepTime(queryData.date);
        var html = template.HTML(title, body);
        response.writeHead(200);
        response.end(html);
       
    },

    maketable_process:function(request, response) {
        var body = '';
        request.on('data', function(data){
            body = body + data;
        });
        request.on('end', function(){
            var post = qs.parse(body);
            console.log(post);
            var date_only_number = post.date;
            console.log("dateonlynumber : ", date_only_number);
            var create_query_string = 'CREATE TABLE day' + date_only_number;
            db.query(create_query_string + ` (
                id INT PRIMARY KEY AUTO_INCREMENT,
                category VARCHAR(32) NOT NULL,
                doing text NOT NULL,
                starth INT NOT NULL,
                startm INT NOT NULL,
                endh INT,
                endm INT,
                consume INT NOT NULL,
                except_check INT NOT NULL
                )`, function(error2, result2){
                if(error2) {
                    throw error2;
                }
                var temp_querystr = 'INSERT INTO day' + date_only_number;
                db.query(temp_querystr + ` (category, doing, starth, startm, endh, endm, consume, except_check) VALUES(?,?,?,?,?,?,?,?)`,['sleep', '수면', String(post.starth), String(post.startm), String(post.endh), String(post.endm), String(puttime_consume_calculation(post.starth, post.startm, post.endh, post.endm)), 1],
                function(error3, result3) {
                    if(error3) {
                        throw error3;
                    }
 
                });
                db.query('INSERT INTO totalrecodes' + ` (daykey, year, month, day, sleepstart, sleepend, sleep) VALUES(?,?,?,?,?,?,?)`,[String(date_only_number), date_only_number[0]+date_only_number[1], date_only_number[2]+date_only_number[3], date_only_number[4]+date_only_number[5], post.starth + ':' + post.startm, post.endh + ':' + post.endm, String(puttime_consume_calculation(post.starth, post.startm, post.endh, post.endm))],
                function(error3, result3) {
                    if(error3) {
                        throw error3;
                    }
                    response.writeHead(302, {Location: `/showNupdatetable?date=${date_only_number}`});
                    response.end();  
                });
            });
 
        });
    }
}
