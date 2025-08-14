var express = require('express');
var path = require('path');
var cookieParser = require('cookie-parser');
var logger = require('morgan');

const { getErrorResponse } = require('./utils/status_codes');
var indexRouter = require('./routes/index');
var customerRouter = require('./routes/customer');
var loginRouter = require('./routes/login');
var accountsRouter = require('./routes/accounts');
var transactionsRouter = require('./routes/transactions');
var cardsRouter = require('./routes/cards');

var jwt=require('jsonwebtoken');

var app = express();

app.use(logger('dev'));
app.use(express.json());
app.use(express.urlencoded({ extended: false }));
app.use(cookieParser());
app.use(express.static(path.join(__dirname, 'public')));

app.use('/', indexRouter);
app.use('/login', loginRouter);
app.use('/cards', cardsRouter);
app.use(authenticateToken);
app.use('/customer', customerRouter);
app.use('/accounts', accountsRouter);
app.use('/transactions', transactionsRouter);

module.exports = app;


function authenticateToken(req, res, next) {
    const authHeader = req.headers['authorization']
    const token = authHeader && authHeader.split(' ')[1]
  
    console.log("token = "+token);
    if (token == null) return res.send(getErrorResponse(5003))
  
    jwt.verify(token, process.env.MY_TOKEN, function(err, user) {
  
      if (err) return res.send(getErrorResponse(5003))

      req.user = user
  
      next()
    })
  }