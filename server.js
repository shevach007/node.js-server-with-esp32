const express = require('express')
const { SerialPort } = require('serialport')
const { ReadlineParser } = require('@serialport/parser-readline')
const app = express() //create an instance of an express module (usses the http protocol for communication)
const portServer = 3000 // port from which we will recieve and send data

Status = '';
const port = new SerialPort({ path: 'COM7', baudRate: 9600, dataBit: 8, parity:'none', 'stopBits': 1, flowControl: false })
function controlLed(OnOrOff){
  
port.write(OnOrOff, function(err) {
  if (err) {
    return console.log('Error on write: ', err.message)
  }
  console.log('message written')
})
}

const parser = port.pipe(new ReadlineParser({ delimiter: '\n' }))
parser.on('data',(data) => {
  
  console.log(data)
  Status = data;
})


//middleware
app.use(express.static('public')) //sends the index.html page inside the public folder as a default
app.use(express.json()) //let express know that we expect a json response from client

app.get('/info/:dynamic', (req, res) => {  //get method on the server side, the request url is 'http://localhost:3000/info', :dynamic is a way to get extra info(in form of a string) from the front-end the the server
    const {dynamic} = req.params //decompose the dynamic variable
    console.log(dynamic)
    res.status(200).json({ info: Status}) // status 200 indicates that the request was successful, we use the json format to communicate from the server to the client
})

app.post('/', (req, res) => { //post method from client to server
    const {parcel} = req.body
    controlLed(parcel);
    console.log(parcel)
    if(!parcel) {return res.status(400).send({status: failed})} //if we didnt get any data from the client we respond with a 'failed' status
    res.status(200).send({status: 'recieved'}) // send acknowledge back to the browser that the data was recieved
})
app.listen(portServer ,() => console.log(`Server is listening on port ${portServer}`))