var app = require('express')();
var http = require('http').Server(app);
var io = require('socket.io')(http);
var roompasses = {};
function valid(roomname,key){
  return roompasses[roomname][0] == key;
}
// var roomname = 'DEFAULT_ROOM'
app.get('/', function(req, res){
  res.sendFile(__dirname + '/index.html');
});

io.on('connection', function(socket){
  console.log("ye Olde Servarrr: A user hath Connected")
  socket.on('host to server', function(matrixData){
    roompasses[matrixData.roomname][1][0] = matrixData.hostMatrix            //datapair[0] is the hostMatrix
    roompasses[matrixData.roomname][1][2] = matrixData.hostPos           //datapair[2] is the hostPos
    io.to(matrixData.roomname).emit('server to client');
  })
  socket.on("client back to server", function(clientData){
    roompasses[clientData.roomname][1][1]= clientData.clientMatrix;
    roompasses[clientData.roomname][1][3]= clientData.clientPos;
    io.to(clientData.roomname).emit("tick and update canvas", roompasses[clientData.roomname][1]);
  })
  
  socket.on('chat message', function(data){
    var msg = data.msg;
    var roomname = data.room;
    var key = data.password;
    var user = data.userid;
    if(valid(roomname,key)){
      socket.join(roomname);
      // console.log("MSG SENT: "+msg+" \tTo room "+roomname);
      io.to(roomname).emit('chat  message', msg);  
    }
    else{
      io.to(roomname).emit('chat message', '---A user has failed to connect to your room ~ Wrong Password*---');
    }
    
  });
  socket.on('join room', function(data){
    var roomname = data.roomname;
    var key = data.key;
    if(valid(roomname, key)){
      socket.join(roomname);
      console.log("client has joined room");
      io.to(roomname).emit('start play', 'idk')
    }
    else{
      io.to(roomname).emit('chat message', '---A user has failed to connect to your room ~ Wrong Password*---');
    }
  });
  socket.on("create room", function(door){
    var roomname = door.roomname;
    socket.join(roomname);
    if(roompasses[door.roomname] == null){
    roompasses[door.roomname]=[door.key, [-1, -1, -1, -1]];
    console.log("Room: "+ door.roomname+"\t Key: "+door.key)
    }
    else{
      io.to(roomname).emit('invalid roomname', 'roomname already taken');
    }
  })
  socket.on('chat message', function(data){
    var msg = data.msg;
    var roomname = data.room;
    var key = data.password;
    if(valid(roomname,key)){
      socket.join(roomname);
      console.log("MSG SENT: "+msg+" \tTo room "+roomname);
      io.to(roomname).emit('chat message', msg);  
    }
    else{
      io.to(roomname).emit('chat message', '---A user has failed to connect to your room ~ Wrong Password*---');
    }
    
  });
 
});

http.listen(process.env.PORT, function(){
  console.log("ye Olde Servarrr: yer Server is Now Running, matey")
  // console.log('listening on *:');
});
// BELOW IS OUTDATED, USE ABOVE FOR MULTIPLE-ROOM COMPATABILITY
/*
var app = require('express')();
var http = require('http').Server(app);
var io = require('socket.io')(http);
var roompasses = {};
var datapair = [-1,-1,-1,-1];
function valid(roomname,key){
  return roompasses[roomname] == key;
}
// var roomname = 'DEFAULT_ROOM'
app.get('/', function(req, res){
  res.sendFile(__dirname + '/index.html');
});

io.on('connection', function(socket){
  console.log("ye Olde Servarrr: A user hath Connected")
  socket.on('host to server', function(matrixData){
    datapair[0]= matrixData.hostMatrix            //datapair[0] is the hostMatrix
    datapair[2]= matrixData.hostPos           //datapair[2] is the hostPos
    io.to(matrixData.roomname).emit('server to client');
  })
  socket.on("client back to server", function(clientData){
    datapair[1]= clientData.clientMatrix;
    datapair[3]= clientData.clientPos;
    io.to(clientData.roomname).emit("tick and update canvas", datapair);
  })
  
  socket.on('chat message', function(data){
    var msg = data.msg;
    var roomname = data.room;
    var key = data.password;
    var user = data.userid;
    if(valid(roomname,key)){
      socket.join(roomname);
      // console.log("MSG SENT: "+msg+" \tTo room "+roomname);
      io.to(roomname).emit('chat  message', msg);  
    }
    else{
      io.to(roomname).emit('chat message', '---A user has failed to connect to your room ~ Wrong Password*---');
    }
    
  });
  socket.on('join room', function(data){
    var roomname = data.roomname;
    var key = data.key;
    if(valid(roomname, key)){
      socket.join(roomname);
      console.log("client has joined room");
      io.to(roomname).emit('start play', 'idk')
    }
    else{
      io.to(roomname).emit('chat message', '---A user has failed to connect to your room ~ Wrong Password*---');
    }
  });
  socket.on("create room", function(door){
    var roomname = door.roomname;
    socket.join(roomname);
    if(roompasses[door.roomname] == null){
    roompasses[door.roomname]=door.key;
    console.log("Room: "+ door.roomname+"\t Key: "+door.key)
    }
    else{
      io.to(roomname).emit('invalid roomname', 'roomname already taken');
    }
  })
  socket.on('chat message', function(data){
    var msg = data.msg;
    var roomname = data.room;
    var key = data.password;
    if(valid(roomname,key)){
      socket.join(roomname);
      console.log("MSG SENT: "+msg+" \tTo room "+roomname);
      io.to(roomname).emit('chat message', msg);  
    }
    else{
      io.to(roomname).emit('chat message', '---A user has failed to connect to your room ~ Wrong Password*---');
    }
    
  });
 
});

http.listen(process.env.PORT, function(){
  console.log("ye Olde Servarrr: yer Server is Now Running, matey")
  // console.log('listening on *:');
});*/