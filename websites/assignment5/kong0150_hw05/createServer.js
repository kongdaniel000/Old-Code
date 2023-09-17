const http = require('http');
const url = require('url');
const fs = require('fs');
const qs = require('querystring');
const { decode } = require('punycode');
const { unescape } = require('querystring');

const port = 9001;
http.createServer(function(req, res) {
  var q = url.parse(req.url, true);
  if (q.pathname === '/') {
    indexPage(req, res);
  }
  else if (q.pathname === '/index.html') {
    indexPage(req, res);
  } 
  else if (q.pathname === '/schedule.html') {
    schedulePage(req, res);
  }
  else if (q.pathname === '/addEvent.html') {
    addEventPage(req, res);
  }
  else if (q.pathname === '/getSchedule') {
    day = q.query['day'];
    getSchedule(req, res, day);
  }
  else if (q.pathname === '/postEventEntry') {
    if(req.method === 'POST'){
      let body = '';
      req.on('data', chunk => {
          body += chunk.toString(); // convert Buffer to string
      });
      req.on('end', () => {
        let split = unescape(body).split("&");
        let day = ""
        jsonString = "{";
        for(i = 0; i < split.length; i++) {
          innerSplit = split[i].split("=");
          key = '"'+innerSplit[0]+'"';
          value = '"'+innerSplit[1]+'"';
          if(key === '"event"') {
            key = '"name"'
          }
          if(key === '"day"') {
            day = innerSplit[1].toLowerCase();
          } 
          else {
            jsonString += key + ":" + value;
            if(i < split.length - 1) {
              jsonString += ",";
            }
          }
        }
        jsonString += "}"
        json = JSON.parse(jsonString);
      postEventEntry(req, res, day, json);
      });
    }
  }
  else {
    res.writeHead(404, { 'Content-Type': 'text/html' });
    return res.end("404 Not Found");
  }
}).listen(port);


function indexPage(req, res) {
  fs.readFile('client/index.html', (err, html) => {
    if (err) {
      throw err;
    }
    res.statusCode = 200;
    res.setHeader('Content-type', 'text/html');
    res.write(html);
    res.end();
  });
}

function schedulePage(req, res) {
  fs.readFile('client/schedule.html', (err, html) => {
    if (err) {
      throw err;
    }
    res.statusCode = 200;
    res.setHeader('Content-type', 'text/html');
    res.write(html);
    res.end();
  });
}

function addEventPage(req, res) {
  fs.readFile('client/addEvent.html', (err, html) => {
    if (err) {
      throw err;
    }
    res.statusCode = 200;
    res.setHeader('Content-type', 'text/html');
    res.write(html);
    res.end();
  });
}

function getSchedule(req, res, day) {
  fs.readFile('schedule.json', (err, json) => {
    if (err) {
      throw err;
    }
    let scheduleObj = JSON.parse(json);
    let dayEvents = scheduleObj[day];
    dayEvents.sort((a,b) => {
      splitA = a.start.split(":");
      splitB = b.start.split(":");
      if(parseInt(splitA[0]) == parseInt(splitB[0])) {
        return parseInt(splitA[1]) - parseInt(splitB[1]);
      }
      return parseInt(splitA[0]) - parseInt(splitB[0]);
    })
    res.statusCode = 200;
    res.setHeader('Content-type', 'application/json');
    res.write(JSON.stringify(dayEvents));
    res.end();
  });
}

function postEventEntry(req, res, day, jsonObj) {
  fs.readFile('schedule.json', (err, json) => {
    if (err) {
      throw err;
    }
    let scheduleObj = JSON.parse(json);
    let dayEvents = scheduleObj[day];
    dayEvents.push(jsonObj);
    dayEvents.sort((a,b) => {
      splitA = a.start.split(":");
      splitB = b.start.split(":");
      if(parseInt(splitA[0]) == parseInt(splitB[0])) {
        return parseInt(splitA[1]) - parseInt(splitB[1]);
      }
      return parseInt(splitA[0]) - parseInt(splitB[0]);
    })
    fs.writeFile('schedule.json', JSON.stringify(scheduleObj, null, 2), err => {
      if (err) {
        throw err;
      }
    });
    res.statusCode = 302;
    res.setHeader('Location', "/schedule.html");
    res.end();
  });
}