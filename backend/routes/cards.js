var express = require('express');
var router = express.Router();
var cards = require('../models/cards_model');
const { getErrorResponse } = require('../utils/status_codes');
const { getSuccessResponse } = require('../utils/status_codes');
/* GET rs listing. */
// Kutsuu cards_model tiedostosta getAll funktiota. Tämä on postmanissa: "localhost:3000/cards/"
router.get('/', function(request, response){
    cards.getAll(function(err,result){
        if(err)
            return response.json(getErrorResponse(4000));
        else if (!result || result.length == 0)
            return response.json(getErrorResponse(4006));
        else
            response.json(result);
    })
});
// Kutsuu cards_model tiedostosta getByNumber funktiota. Tämä on postmanissa: "localhost:3000/cards/3"
router.get('/:id',function(request,response){
    cards.getByNumber(request.params.id,function(err,result){
        if(err)
            return response.json(getErrorResponse(4001));
        else if (!result || result.length == 0)
            return response.json(getErrorResponse(4006));
        else
            response.json(result[0]);

    })
});
// Kutsuu cards_model tiedostosta add funktiota. Tämä on postmanissa: "localhost:3000/cards/" ja lisänä Body osioon lisättynä { Key: fname Value: Pentti } jne jne
router.post('/', function(request,response){
    cards.add(request.body,function(err,result){
        if (err)
            return response.json(getErrorResponse(4002));
        else 
            response.json(getSuccessResponse(400));
    })
});

//double kortti
router.post('/double/', function(request,response){
    cards.addDouble(request.body,function(err,result){
        if (err)
            return response.json(getErrorResponse(4006));
        else 
            response.json(getSuccessResponse(401));
    })
});

// Kutsuu cards_model tiedostosta update funktiota. Tämä on postmanissa: "localhost:3000/cards/3" ja lisänä Body osioon lisättynä { Key: fname Value: Pentti } jne jne
router.put('/:id', function(request,response){
    cards.getByNumber(request.params.id,function(err,result){
        if(err)
            return response.json(getErrorResponse(4001));
        else if (!result || result.length == 0)
            return response.json(getErrorResponse(4006));

        cards.update(request.params.id, request.body, function(err,result){
            if (err)
                return response.json(getErrorResponse(4003));
            else 
                response.json(getSuccessResponse(402));
        })
    });
});


// Kutsuu cards_model tiedostosta delete funktiota. Tämä on postmanissa: "localhost:3000/cards/3"
router.delete('/:id', function(request, response){
    cards.getByNumber(request.params.id,function(err,result){
        if(err)
            return response.json(getErrorResponse(4001));
        else if (!result || result.length == 0)
            return response.json(getErrorResponse(4006));

        cards.delete(request.params.id, function(err,result){
            if (err)
                return response.json(getErrorResponse(4004));
            else 
                response.json(getSuccessResponse(403));
        })
    });
});

router.put('/updateattempt/:id', function(request,response){
    cards.getByNumber(request.params.id,function(err,result){
        if(err)
            return response.json(getErrorResponse(4001));
        else if (!result || result.length == 0)
            return response.json(getErrorResponse(4006));

        cards.setAttempts(request.params.id, request.body.card_attempts, function(err,result){
            if (err)
                return response.json(getErrorResponse(4005));
            else 
                response.json(getSuccessResponse(404));
        })
    });
});

//Päivittää kortin statuksen joko "Active" tai "Locked"
router.put('/status/:id', function(request,response){
    cards.getByNumber(request.params.id,function(err,result){
        if(err)
            return response.json(getErrorResponse(4001));
        else if (!result || result.length == 0)
            return response.json(getErrorResponse(4006));

        cards.updateStatus(request.params.id, request.body.card_state, function(err,result){
            if (err)
                return response.json(getErrorResponse(4008));
            else 
                response.json(getSuccessResponse(405));
        })
    })
});

module.exports = router;