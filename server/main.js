const express = require('express');
const app = express();
const bodyParser = require('body-parser');

let weather = {};
app
    .use(bodyParser.json())
    .use(bodyParser.urlencoded({ extended: true }));

app.listen(process.env.PORT || 5000, function(){
    console.log('Server Running');
});

app.post('/', function(req,res){
    weather._temperature = req.body.temperature;
    weather._humidity = req.body.humidity;
    weather._time = Date.now();
    res.sendStatus(202);
    console.log(weather);
});

app.get('/', function(req,res){
    res.json(weather);
});
