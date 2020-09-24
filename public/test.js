 var child_process = require("child_process")
 var obj = { "message":message, "password":password, "fileName":beforeName};
  feed_dict = { input: jsonData };
    
    // spawn the (python) child process
    py = child_process.spawnSync(python_exe, [pythonFile], feed_dict );
    
    // extract the result of the python operation
    py_response = py['stdout'].toString();
    
    // send the result back to the user
    console.log(py_response)
    //res.send("Response: " + py_response);