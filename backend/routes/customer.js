var express = require('express');
var router = express.Router();
var customer = require('../models/customer_model');
const { getErrorResponse } = require('../utils/status_codes');
const { getSuccessResponse } = require('../utils/status_codes');
/* GET customers listing. */
// Kutsuu customer_model tiedostosta getAll funktiota. Tämä on postmanissa: "localhost:3000/customer/"
router.get('/', function(request, response){
    customer.getAll(function(err,result){
        if(err)
            return response.json(getErrorResponse(3000));
        else if (!result || result.length == 0)
            return response.json(getErrorResponse(3005));
        else
            response.json(result);
    })
});
// Kutsuu customer_model tiedostosta getByID funktiota. Tämä on postmanissa: "localhost:3000/customer/3"
router.get('/:id',function(request,response){
    customer.getById(request.params.id,function(err,result){
        if(err)
            return response.json(getErrorResponse(3001));
        else if (!result || result.length == 0)
            return response.json(getErrorResponse(3005));
        else
            response.json(result[0]);

    })
});
// Kutsuu customer_model tiedostosta add funktiota. Tämä on postmanissa: "localhost:3000/customer/" ja lisänä Body osioon lisättynä { Key: fname Value: Pentti } jne jne
router.post('/', function(request,response){
    customer.add(request.body,function(err,result){
        if (err)
            return response.json(getErrorResponse(3002));
        else 
            response.json(getSuccessResponse(300));
    })
});
// Kutsuu customer_model tiedostosta update funktiota. Tämä on postmanissa: "localhost:3000/customer/3" ja lisänä Body osioon lisättynä { Key: fname Value: Pentti } jne jne
router.put('/:id', function(request,response){
    customer.getById(request.params.id,function(err,result){
        if(err)
            return response.json(getErrorResponse(3001));
        else if (!result || result.length == 0)
            return response.json(getErrorResponse(3005));
        customer.update(request.params.id, request.body, function(err,result){
            if (err)
                return response.json(getErrorResponse(-3003));
            else 
                response.json(getSuccessResponse(301));
        })
    });
});
// Kutsuu customer_model tiedostosta delete funktiota. Tämä on postmanissa: "localhost:3000/customer/3"
router.delete('/:id', function(request, response){
    customer.getById(request.params.id,function(err,result){
        if(err)
            return response.json(getErrorResponse(3001));
        else if (!result || result.length == 0)
            return response.json(getErrorResponse(3005));
        
       customer.delete(request.params.id, function(err,result){
            if (err)
                return response.json(getErrorResponse(3004));
            else 
                response.json(getSuccessResponse(302));
        })
    });
});

module.exports = router;