#!/usr/bin/nodejs
var PythonShell = require('python-shell');
var express = require('express');
var bodyParser = require('body-parser')
var contentDisposition = require('content-disposition')
var destroy = require('destroy')
var child_process = require('child_process')
var onFinished = require('on-finished')
var fs = require('fs')
var path = require('path');
//var http = require('http').Server(app);
var fileUpload = require('express-fileupload');
var app = express();
app.use(bodyParser.urlencoded({ extended: false }));

// -------------- express initialization -------------- //

// Here, we set the port (these settings are specific to our site)
app.set('port', process.env.PORT || 8080 );
app.use(fileUpload());
function decode(password, beforeName, res){
    console.log("in decode")
    /*var pyshell = new PythonShell('d.py');
    pyshell.send(password);
    pyshell.on('message', function (message) {
      // received a message sent from the Python script (a simple "print" statement) 
        console.log(message);
        //res.send(message);
    });*/
    const spawn = require("child_process").spawn;
    const pyFile = 'd2.py';
    const args = [beforeName, password];
    args.unshift(pyFile);
    
    const pyspawn = spawn('python2', args);
    pyspawn.stdout.on('data', (data) => {
        console.log(`stdout: ${data}`);
        res.send(data)
    });
    
    pyspawn.stderr.on('data', (data) => {
        console.log(`stderr: ${data}`);
        res.send(data)
    });
    
}
function deleteFile (file) { 
    fs.unlink(file, function (err) {
        if (err) {
            console.error(err.toString());
        } else {
            console.warn(file + ' deleted');
        }
    });
}
function justSendSomething(res){
    res.send("encoded.png")
}
/*function encode(message, password, beforeName, res) {
    console.log("in encode");
    var pyshell = new PythonShell('e.py');
    
    // sends a message to the Python script via stdin
    //console.log(beforeName.toString());
    pyshell.send(beforeName);
    pyshell.send(message);
    pyshell.send(password);
    fileName = "wrongwrongwrong";
    counter = 0;
    pyshell.on('message', function (message) {
      // received a message sent from the Python script (a simple "print" statement) 
        counter += 1;
        fileName = message
        res.send("encoded.png")
    });
     
    // end the input stream and allow the process to exit 
    pyshell.end(function (err) {
      if (err){
          throw err;
      }
      filePath = __dirname + '/' + fileName;
      console.log('finished encoding');
      //res.setHeader('Content-Type', 'image/png')
      //res.setHeader('Content-Disposition', contentDisposition(filePath))
 

      res.send("encoded.png")
            // send file 
      var stream = fs.createReadStream(filePath)
        stream.pipe(res).once("close", function () {
            stream.destroy(); // makesure stream closed, not close if download aborted.
            //deleteFile(filename);
        });
      //res.sendFile(__dirname + '/' + fileName)
    });
    
}*/
function encode(message, password, beforeName, res){
    /*var obj = { "message":message, "password":password, "fileName":beforeName};
      python_exe = 'python2';

    // the python file
    pythonFile = path.join(__dirname, 'e.py');
    //pythonFile = "e.py"
    //produce json data for python input
    jsonData = JSON.stringify(obj);
    feed_dict = { input: jsonData };
    
    // spawn the (python) child process
    py = child_process.spawnSync(python_exe, [pythonFile], feed_dict);
        // extract the result of the python operation
    py_response = py['stdout'].toString();
    py_error = py['stderr'].toString()
    // send the result back to the user
    res.send("Response: " + py_error);
    */
    const spawn = require("child_process").spawn;
    const pyFile = 'e.py';
    const args = [beforeName, message, password];
    args.unshift(pyFile);
    
    const pyspawn = spawn('python2', args);
    pyspawn.stdout.on('data', (data) => {
        console.log(`stdout: ${data}`);
        res.send(data)
    });
    
    pyspawn.stderr.on('data', (data) => {
        console.log(`stderr: ${data}`);
        res.send(data)
    });

}

var listener = app.listen(app.get('port'), function() {
  console.log("server running")
  console.log( 'Express server started on port: '+listener.address().port );
});

app.get('/', function(req, res){
  res.sendFile(__dirname + '/index.html');
  
});
var getImageListener = app.get('/getImage', function(req, res){
    fileName = req.query.fileName;
    res.sendFile(__dirname + '/' + fileName);
    //res.sendFile(__dirname + "/" + "doesn'texist")
    
});

var getCapsListener = app.get('/caps', function(req, res){
    res.sendFile(__dirname + '/' + "caps.html")
})
var decodingListener = app.post('/decodeMessage', function(req, res){
   password = req.body.password; 
   var vessel = req.files.vessel;
   var image_file_name = vessel.name;
   var image_file_ext  = path.extname(image_file_name);

    // construct complete file path
    
    
    d = new Date();
    t = d.getTime();
    var date_stamp = t.toString();
    var file_string = 'decode_date_' + date_stamp + image_file_ext; 
    image_file_path = path.join(__dirname, file_string );
    vessel.mv( image_file_path, function(err) {
        if (err)
            res.send("error saving file");
        else{
            decode(password, image_file_path, res);
        }
    });
});
var hilistener = app.post("/hi", function(req, res){
    justSendSomething(res)
});

var encodingListener = app.post('/encodeMessage', function(req, res){
    if (!req.files) {
        return res.status(400).send('no file uploaded');
    }
    message = req.body.message;
    password = req.body.password;
    var vessel = req.files.vessel;
    if (message === null || password === null || vessel === null){
        return res.status(400).send("um why")
    }
    var image_file_name = vessel.name;
    var image_file_ext  = path.extname(image_file_name);

    // construct complete file path
    
    
    d = new Date();
    t = d.getTime();
    var date_stamp = t.toString();
    var file_string = 'date_' + date_stamp + image_file_ext; 
    image_file_path = path.join(__dirname, file_string );
    vessel.mv( image_file_path, function(err) {
        if (err)
          return res.status(500).send(err);
        else{
            encode(message, password, image_file_path, res);
        }
    });
    /*fs.writeFile(image_file_path, vessel, function (err) {
          if (err){
              res.send("couldnt save file")
          }
          console.log('Saved!' + beforeName.toString());
          
        });*/
    //justSendSomething(res)  
    
    //res.send("maybe success");
});