const express = require('express');
const router = express.Router();
const bcrypt = require('bcryptjs');
const cards = require('../models/cards_model');
const jwt = require('jsonwebtoken');
const dotenv = require('dotenv');
const { getErrorResponse } = require('../utils/status_codes');
const { getSuccessResponse } = require('../utils/status_codes');

router.post('/', function (request, response) {
  const cardNum = request.body.card_number;
  const cardPin = request.body.card_pin;

  cards.getByNumber(cardNum, function (err, result) {
    if (err)
      return response.json(getErrorResponse(4001));
    else if (!result || result.length == 0)
      return response.json(getErrorResponse(4006));

    const cardData = result[0];
    
    if (cardData.card_state == "locked"){
      return response.json(getErrorResponse(4009));
    }
    bcrypt.compare(cardPin, cardData.card_pin, function(err,compareResult) {
      if (compareResult){
        cards.setAttempts(cardNum, 3, function (err, result) {
          if (err)
            return response.json(getErrorResponse(4005));

          const token = generateAccessToken(cardNum, cardData.card_owner_id);
          response.setHeader('Content-Type', 'application/json');
          response.send({ successCode: 500, message: token });
        })}
      else if (cardData.card_attempts > 0){
        cards.setAttempts(cardNum, cardData.card_attempts - 1, function (err, result) {
          if (err)
            return response.json(getErrorResponse(4005));
          else
            return response.json(getErrorResponse(5001));
        })
      }
      else{
        cards.updateStatus(cardNum, "Locked", function (err, result) {
          if (err)
            return response.json(getErrorResponse(4007));
          else
            response.json(getSuccessResponse(501));
        })
      }
    });
  })
});

function generateAccessToken(cardNum, owner) {
  dotenv.config();
  return jwt.sign({ card_number: cardNum, owner_id: owner }, process.env.MY_TOKEN, { expiresIn: '1800s' });
}

module.exports = router;